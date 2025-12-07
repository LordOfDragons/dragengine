/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
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
#include "dewmAudioTrackCallback.h"
#include "dewmVorbisStream.h"
#include "dewmInfos.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>



// Class dewmAudioTrackCallback
/////////////////////////////////

// Constructor, destructor
////////////////////////////

dewmAudioTrackCallback::dewmAudioTrackCallback(deVideoWebm &module) :
dewmTrackCallback(module),
pResBuffer(nullptr),
pResSize(0),
pResPosition(0),
pSampleSize(1),
pStreamVorbis(nullptr){
}

dewmAudioTrackCallback::~dewmAudioTrackCallback(){
	if(pStreamVorbis){
		delete pStreamVorbis;
	}
}



// Management
///////////////

void dewmAudioTrackCallback::SetResBuffer(void *buffer, int samples){
	DEASSERT_TRUE(! buffer || IsStreamOpen())
	
	pResBuffer = (uint8_t*)buffer;
	pResSize = samples;
	pResPosition = 0;
}

bool dewmAudioTrackCallback::IsStreamOpen(){
	return pStreamVorbis;
}

void dewmAudioTrackCallback::Rewind(){
	DEASSERT_TRUE(IsStreamOpen())
	
	if(pStreamVorbis){
		pStreamVorbis->Rewind();
	}
}

bool dewmAudioTrackCallback::OpenTrack(const webm::TrackEntry &track){
	return pOpenTrack(track);
}

void dewmAudioTrackCallback::UpdateInfos(dewmInfos &infos){
	DEASSERT_TRUE(IsStreamOpen())
	
	if(pStreamVorbis){
		infos.SetBytesPerSample(pStreamVorbis->GetBytesPerSample());
		infos.SetSampleRate(pStreamVorbis->GetSampleRate());
		infos.SetChannelCount(pStreamVorbis->GetChannelCount());
	}
}

void dewmAudioTrackCallback::AdvanceResPosition(int bytes){
	pResPosition += bytes;
}



// Protected Functions
////////////////////////

bool dewmAudioTrackCallback::pOpenTrack(const webm::TrackEntry &track){
	if(track.track_type.value() != webm::TrackType::kAudio){
		return false;
	}
	
	if(pStreamVorbis){
		return false;
	}
	
	if(track.codec_id.value() == "A_VORBIS"){
		pStreamVorbis = new dewmVorbisStream(*this);
		if(pStreamVorbis->OpenTrack(track)){
			pSampleSize = pStreamVorbis->GetBufferSampleSize();
			return true;
		}
		
		delete pStreamVorbis;
		pStreamVorbis = nullptr;
	}
	
	return false;
}

void dewmAudioTrackCallback::pProcessFrame(webm::Reader &reader, std::uint64_t &bytes_remaining){
	// copy remaining samples in context until buffer is full
	pCopySamples();
	
	if(pResPosition == pResSize){
		// found enough remaining samples to fill the buffer. the data from this frame is
		// not required. by not touching bytes_remaining this frame will be process again
		// the next time the parser is run
		return;
	}
	
	// we need more samples. load frame data and copy samples until buffer is full.
	// after this call bytes_remaining will be 0 and the next frame will be process
	pLoadFrameData(reader, bytes_remaining);
	pCopySamples();
	
	// if the buffer is not full we need more frames to complete it otherwise we can stop
	SetNeedMoreFrames(pResPosition < pResSize);
}

void dewmAudioTrackCallback::pEndSegment(){
	// we have a really annoying problem here. the webm format has no way to store the
	// total count of samples nor a reliable way to calculate this. it is thus possible
	// we are asked for more samples at the end of the video than we can provide.
	// in this case we fill up with last sample written
	/*
	GetModule().LogInfoFormat("pEndSegment %d %d", pResPosition, pResSize);
	if(pResPosition < pResSize){
		if(pStreamVorbis){
			pStreamVorbis->FillUpBuffer();
			pResPosition = pResSize;
		}
	}
	*/
}



// Private Functions
//////////////////////

void dewmAudioTrackCallback::pCopySamples(){
	if(pStreamVorbis){
		pStreamVorbis->CopySamples();
	}
}

void dewmAudioTrackCallback::pLoadFrameData(webm::Reader &reader, std::uint64_t &bytes_remaining){
	if(bytes_remaining == 0){
		// frame consumed
		return;
	}
	
	// read frame data and load it. bytes_remaining will become 0 after reading
	// the frame so if we know the next time the frame has been consumed
	const uint64_t frameSize = bytes_remaining;
	pReadFrameData(reader, bytes_remaining);
	
	if(pStreamVorbis){
		pStreamVorbis->LoadFrameData(frameSize);
	}
}
