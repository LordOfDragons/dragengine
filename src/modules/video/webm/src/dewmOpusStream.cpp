/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <string.h>

#include "deVideoWebm.h"
#include "dewmOpusStream.h"
#include "dewmAudioTrackCallback.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>



// Class dewmOpusStream
/////////////////////////

// The maximum number of samples per channel per Opus frame is 5760 (120ms at 48kHz).
static const int vMaxFrameSamples = 5760;

// Constructor, destructor
////////////////////////////

dewmOpusStream::dewmOpusStream(dewmAudioTrackCallback &callback) :
pCallback(callback),
pBytesPerSample(2),
pSampleRate(48000),
pChannelCount(1),
pPreSkip(0),
pBufferSampleSize(2),
pDecoder(nullptr),
pPcmBufferPos(0),
pPcmBufferCount(0){
}

dewmOpusStream::~dewmOpusStream(){
	if(pDecoder){
		opus_multistream_decoder_destroy(pDecoder);
	}
}



// Management
///////////////

int dewmOpusStream::GetBytesPerSample() const{
	return pBytesPerSample;
}

int dewmOpusStream::GetSampleRate() const{
	return pSampleRate;
}

int dewmOpusStream::GetChannelCount() const{
	return pChannelCount;
}

int dewmOpusStream::GetBufferSampleSize() const{
	return pBufferSampleSize;
}

bool dewmOpusStream::OpenTrack(const webm::TrackEntry &track){
	if(track.track_type.value() != webm::TrackType::kAudio){
		pCallback.GetModule().LogError("OpusStream.OpenTrack: track type is not audio");
		return false;
	}
	if(pDecoder){
		pCallback.GetModule().LogError("OpusStream.OpenTrack: decoder already created");
		return false;
	}
	if(track.codec_id.value() != "A_OPUS"){
		pCallback.GetModule().LogError("OpusStream.OpenTrack: codec is not opus");
		return false;
	}
	
	// The codec private data is the binary OpusHead blob:
	// https://www.rfc-editor.org/rfc/rfc7845#section-5.1
	//
	// Structure:
	//   Bytes  0- 7: magic "OpusHead"
	//   Byte   8:    version (must be 1)
	//   Byte   9:    channel count
	//   Bytes 10-11: pre-skip (uint16LE)
	//   Bytes 12-15: input sample rate (uint32LE, informational)
	//   Bytes 16-17: output gain (int16LE, Q7.8 dB)
	//   Byte  18:    channel mapping family
	//   If family != 0:
	//     Byte  19:  stream count
	//     Byte  20:  coupled count
	//     Bytes 21+: channel_mapping[channel_count]
	
	if(!track.codec_private.is_present()){
		pCallback.GetModule().LogError("OpusStream.OpenTrack: missing codec private data");
		return false;
	}
	
	const std::vector<uint8_t> &codecPrivateData = track.codec_private.value();
	
	if(codecPrivateData.size() < 19){
		pCallback.GetModule().LogErrorFormat(
			"OpusStream.OpenTrack: codec private too small (%d)", (int)codecPrivateData.size());
		return false;
	}
	
	if(memcmp(codecPrivateData.data(), "OpusHead", 8) != 0){
		pCallback.GetModule().LogError("OpusStream.OpenTrack: invalid OpusHead magic");
		return false;
	}
	
	const int version = (int)codecPrivateData[8];
	if(version != 1){
		pCallback.GetModule().LogErrorFormat(
			"OpusStream.OpenTrack: unsupported version %d", (int)version);
		return false;
	}
	
	pChannelCount = (int)codecPrivateData[9];
	if(pChannelCount < 1 || pChannelCount > 255){
		pCallback.GetModule().LogErrorFormat(
			"OpusStream.OpenTrack: invalid channel count %d", pChannelCount);
		return false;
	}
	
	pPreSkip = (int)(((uint16_t)codecPrivateData[10]) | ((uint16_t)codecPrivateData[11] << 8));
	
	// input_sample_rate is informational; libopus always outputs at 48000 Hz
	pSampleRate = 48000;
	
	const int mappingFamily = (int)codecPrivateData[18];
	
	int streamCount = 1;
	int coupledCount = pChannelCount > 1 ? 1 : 0;
	uint8_t mapping[255];
	
	if(mappingFamily == 0){
		// standard mono or stereo
		if(pChannelCount > 2){
			pCallback.GetModule().LogErrorFormat(
				"OpusStream.OpenTrack: mapping family 0 with %d channels invalid", pChannelCount);
			return false;
		}
		streamCount = 1;
		coupledCount = pChannelCount - 1;
		for(int i=0; i<pChannelCount; i++){
			mapping[i] = (uint8_t)i;
		}
		
	}else{
		// multi-stream mapping (family 1 or 255)
		if(codecPrivateData.size() < (size_t)(21 + pChannelCount)){
			pCallback.GetModule().LogErrorFormat(
				"OpusStream.OpenTrack: codec private too small for mapping family %d (%d)",
				mappingFamily, (int)codecPrivateData.size());
			return false;
		}
		
		streamCount = (int)codecPrivateData[19];
		coupledCount = (int)codecPrivateData[20];
		
		for(int i=0; i<pChannelCount; i++){
			mapping[i] = codecPrivateData[21 + i];
		}
	}
	
	int error = OPUS_OK;
	pDecoder = opus_multistream_decoder_create(pSampleRate, pChannelCount,
		streamCount, coupledCount, mapping, &error);
	
	if(!pDecoder || error != OPUS_OK){
		pCallback.GetModule().LogErrorFormat(
			"OpusStream.OpenTrack: failed to create decoder (error %d)", error);
		pDecoder = nullptr;
		return false;
	}
	
	pBytesPerSample = 2; // int16
	pBufferSampleSize = pBytesPerSample * pChannelCount;
	
	// pre-allocate pcm buffer for max frame size
	pPcmBuffer.SetCountDiscard(vMaxFrameSamples * pChannelCount);
	pPcmBufferPos = 0;
	pPcmBufferCount = 0;
	
	return true;
}

void dewmOpusStream::CopySamples(){
	if(pPcmBufferCount == 0){
		return;
	}
	
	const int bufferSampleCount = pCallback.GetResSize() - pCallback.GetResPosition();
	const int copiedSampleCount = decMath::min(pPcmBufferCount, bufferSampleCount);
	
	if(pCallback.GetResBuffer()){
		const int16_t * const srcSamples = pPcmBuffer.GetArrayPointer() + pPcmBufferPos * pChannelCount;
		int16_t * const dstBuffer = reinterpret_cast<int16_t*>(
			pCallback.GetResBuffer() + pCallback.GetResPosition() * pBufferSampleSize);
		memcpy(dstBuffer, srcSamples, copiedSampleCount * pBufferSampleSize);
	}
	
	pCallback.AdvanceResPosition(copiedSampleCount);
	pPcmBufferPos += copiedSampleCount;
	pPcmBufferCount -= copiedSampleCount;
}

void dewmOpusStream::LoadFrameData(std::uint64_t frameSize){
	if(!pDecoder || frameSize == 0){
		return;
	}
	
	const uint8_t * const data = pCallback.GetFrameBuffer().GetArrayPointer();
	
	const int decodedSamples = opus_multistream_decode(
		pDecoder, data, (opus_int32)frameSize,
		pPcmBuffer.GetArrayPointer(), vMaxFrameSamples, 0);
	
	if(decodedSamples < 0){
		pCallback.GetModule().LogErrorFormat(
			"OpusStream.LoadFrameData: decode failed (error %d)", decodedSamples);
		pPcmBufferPos = 0;
		pPcmBufferCount = 0;
		return;
	}
	
	// skip pre-skip samples on first few packets
	int firstSample = 0;
	if(pPreSkip > 0){
		firstSample = decMath::min(pPreSkip, decodedSamples);
		pPreSkip -= firstSample;
	}
	
	pPcmBufferPos = firstSample;
	pPcmBufferCount = decodedSamples - firstSample;
}

void dewmOpusStream::Rewind(){
	if(pDecoder){
		opus_multistream_decoder_ctl(pDecoder, OPUS_RESET_STATE);
		pPreSkip = 0;
	}
	pPcmBufferPos = 0;
	pPcmBufferCount = 0;
}
