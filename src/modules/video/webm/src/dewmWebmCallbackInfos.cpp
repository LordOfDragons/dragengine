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
#include "dewmInfos.h"
#include "dewmAudioTrackCallback.h"
#include "dewmWebmCallbackInfos.h"
#include "dewmVorbisStream.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/parallel/deParallelProcessing.h>

#include <vpx/vpx_decoder.h>
#include <vpx/vp8dx.h>



// Class dewmWebmCallbackInfos
////////////////////////////////

// Constructor, destructor
////////////////////////////

dewmWebmCallbackInfos::dewmWebmCallbackInfos(deVideoWebm &module, dewmInfos &infos) :
pModule(module),
pInfos(infos),
pState(esNone),
pFirstVideoFrame(false),
pFirstAudioFrame(false),
pTimecodeScale(1000000),
pTimecodeFactor(1.0 / 1e3),
pFrameCount(0){
}

dewmWebmCallbackInfos::~dewmWebmCallbackInfos(){
}



// Management
///////////////

webm::Status dewmWebmCallbackInfos::OnSegmentBegin(const webm::ElementMetadata &,
webm::Action *action){
	*action = webm::Action::kRead;
	return webm::Status(webm::Status::Code::kOkCompleted);
}

webm::Status dewmWebmCallbackInfos::OnSegmentEnd(const webm::ElementMetadata &){
	if(pFrameCount > (std::uint64_t)(1 << 31)){
		pModule.LogError("too many frames");
		return webm::Status(100);
	}
	
	pModule.LogInfoFormat("SegmentEnd: frames=%u duration=%g rate=%g",
		(std::uint32_t)pFrameCount, pInfos.GetDuration(),
		(double)pFrameCount / pInfos.GetDuration());
	
	pInfos.SetFrameCount((int)pFrameCount);
	pInfos.SetFrameRate((float)((double)pFrameCount / pInfos.GetDuration()));
	
	return webm::Status(webm::Status::Code::kOkPartial); // stop here
}

webm::Status dewmWebmCallbackInfos::OnInfo(const webm::ElementMetadata &, const webm::Info &info){
	if(! info.duration.is_present()){
		pModule.LogError("info.duration absent");
		return webm::Status(100);
	}
	
	pTimecodeScale = info.timecode_scale.value();
	pTimecodeFactor = (double)pTimecodeScale / 1e9;
	pInfos.SetDuration(info.duration.value() * pTimecodeFactor);
	return webm::Status(webm::Status::Code::kOkCompleted);
}

webm::Status dewmWebmCallbackInfos::OnTrackEntry(const webm::ElementMetadata &,
const webm::TrackEntry &track_entry){
	if(! track_entry.is_enabled.value()){
		return webm::Status(webm::Status::Code::kOkCompleted);
	}
	
	switch(track_entry.track_type.value()){
	case webm::TrackType::kVideo:
		return pOnTrackVideo(track_entry);
		
	case webm::TrackType::kAudio:
		return pOnTrackAudio(track_entry);
		
	default:
		return webm::Status(webm::Status::Code::kOkCompleted);
	}
}

webm::Status dewmWebmCallbackInfos::OnClusterBegin(const webm::ElementMetadata &,
const webm::Cluster &, webm::Action *action){
	*action = webm::Action::kRead;
	return webm::Status(webm::Status::Code::kOkCompleted);
}

webm::Status dewmWebmCallbackInfos::OnSimpleBlockBegin(const webm::ElementMetadata &,
const webm::SimpleBlock &simple_block, webm::Action *action){
	pProcessBlock(simple_block);
	*action = webm::Action::kRead;
	return webm::Status(webm::Status::Code::kOkCompleted);
}

webm::Status dewmWebmCallbackInfos::OnBlockBegin(const webm::ElementMetadata &,
const webm::Block &block, webm::Action *action){
	pProcessBlock(block);
	*action = webm::Action::kRead;
	return webm::Status(webm::Status::Code::kOkCompleted);
}

webm::Status dewmWebmCallbackInfos::OnFrame(const webm::FrameMetadata &,
webm::Reader *reader, std::uint64_t *bytes_remaining){
	std::uint64_t bytesSkipped;
	webm::Status status;
	
	switch(pState){
	case esBlockVideo:
		if(pFirstVideoFrame){
			pFirstVideoFrame = false;
			status = pProcessFirstFrameVideo(*reader, *bytes_remaining);
			
		}else{
			status = webm::Status(webm::Status::Code::kOkCompleted);
		}
		break;
		
	case esBlockAudio:
		if(pFirstAudioFrame){
			pFirstAudioFrame = false;
			status = pProcessFirstFrameAudio(*reader, *bytes_remaining);
			
		}else{
			status = webm::Status(webm::Status::Code::kOkCompleted);
		}
		break;
		
	default:
		status = webm::Status(webm::Status::Code::kOkCompleted);
	}
	
	if(! status.completed_ok()){
		return status;
	}
	
	if(*bytes_remaining > 0){
		status = reader->Skip(*bytes_remaining, &bytesSkipped);
	}
	if(status.completed_ok()){
		*bytes_remaining = 0;
	}
	return status;
}



// Protected Functions
////////////////////////

webm::Status dewmWebmCallbackInfos::pOnTrackVideo(const webm::TrackEntry &track){
	if(pInfos.GetVideoCodec() != dewmInfos::evcUnsupported){
		return webm::Status(webm::Status::Code::kOkCompleted);
	}
	
	dewmInfos::eVideoCodec codec = dewmInfos::evcUnsupported;
	const char *codecName = "";
	
	if(track.codec_id.value() == "V_VP9"){
		codec = dewmInfos::evcVp9;
		codecName = "VP9";
		
	}else if(track.codec_id.value() == "V_VP8"){
		codec = dewmInfos::evcVp8;
		codecName = "VP8";
		
	}else{
		return webm::Status(webm::Status::Code::kOkCompleted);
	}
	
	const webm::Video &tvideo = track.video.value();
	
	pModule.LogInfoFormat("Found video track '%s': track=%d size=%dx%d alpha=%d",
		codecName, (int)track.track_number.value(), (int)tvideo.pixel_width.value(),
		(int)tvideo.pixel_height.value(),
		tvideo.alpha_mode.is_present() ? (int)tvideo.alpha_mode.value() : -1);
	
	pInfos.SetVideoCodec(codec);
	pInfos.SetVideoTrackNumber(track.track_number.value());
	pInfos.SetWidth((int)tvideo.pixel_width.value());
	pInfos.SetHeight((int)tvideo.pixel_height.value());
	
	if(tvideo.alpha_mode.is_present() && tvideo.alpha_mode.value()){
		pInfos.SetComponentCount(4);
		
	}else{
		pInfos.SetComponentCount(3);
	}
	
	pModule.LogInfoFormat("Using video track %d", (int)track.track_number.value());
	
	pFirstVideoFrame = true; // read one frame to find video information
	return webm::Status(webm::Status::Code::kOkCompleted);
}

webm::Status dewmWebmCallbackInfos::pOnTrackAudio(const webm::TrackEntry &track){
	if(pInfos.GetAudioCodec() != dewmInfos::eacUnsupported){
		return webm::Status(webm::Status::Code::kOkCompleted);
	}
	
	dewmInfos::eAudioCodec codec = dewmInfos::eacUnsupported;
	const char *codecName = "";
	
	if(track.codec_id.value() == "A_VORBIS"){
		codec = dewmInfos::eacVorbis;
		codecName = "Vorbis";
		
	}else if(track.codec_id.value() == "A_OPUS"){
		codec = dewmInfos::eacOpus;
		codecName = "Opus";
		
	}else{
		return webm::Status(webm::Status::Code::kOkCompleted);
	}
	
	pModule.LogInfoFormat("Found audio track '%s': track=%d", codecName, (int)track.track_number.value());
	
	try{
		dewmAudioTrackCallback audioCallback(pModule);
		DEASSERT_TRUE(audioCallback.OpenTrack(track));
		audioCallback.UpdateInfos(pInfos);
		
	}catch(const deException &e){
		pModule.LogException(e);
		return webm::Status(webm::Status::Code::kOkCompleted);
	}
	
	pInfos.SetAudioCodec(codec);
	pInfos.SetAudioTrackNumber(track.track_number.value());
	pInfos.SetSampleCount((int)(pInfos.GetDuration() * (double)pInfos.GetSampleRate() + 0.5));
	
	pModule.LogInfoFormat("Using audio track %d", (int)track.track_number.value());
	
	return webm::Status(webm::Status::Code::kOkCompleted);
}

void dewmWebmCallbackInfos::pProcessBlock(const webm::Block &block){
	if(block.track_number == pInfos.GetVideoTrackNumber()){
		pFrameCount += block.num_frames;
		pState = esBlockVideo;
		
	}else if(block.track_number == pInfos.GetAudioTrackNumber()){
		pState = esBlockAudio;
		
	}else{
		pState = esNone;
	}
}

webm::Status dewmWebmCallbackInfos::pProcessFirstFrameVideo(webm::Reader &reader,
std::uint64_t &bytes_remaining){
	vpx_codec_ctx_t *context = nullptr;
	uint8_t *data = nullptr;
	
	vpx_codec_iter_t iter = nullptr;
	vpx_codec_dec_cfg_t config;
	vpx_codec_iface_t *iface;
	vpx_image_t *image;
	
	try{
		memset(&config, 0, sizeof(config));
		
		context = new vpx_codec_ctx_t;
		memset(context, 0, sizeof(vpx_codec_ctx_t));
		
		config.w = pInfos.GetWidth();
		config.h = pInfos.GetHeight();
		config.threads = 1; //pModule.GetGameEngine()->GetParallelProcessing().GetCoreCount();
		
		switch(pInfos.GetVideoCodec()){
		case dewmInfos::evcVp9:
			iface = vpx_codec_vp9_dx();
			break;
			
		case dewmInfos::evcVp8:
			iface = vpx_codec_vp8_dx();
			break;
			
		default:
			DETHROW_INFO(deeInvalidParam, "invalid video vodec");
		}
		
		DEASSERT_TRUE(vpx_codec_dec_init(context, iface, &config, 0) == VPX_CODEC_OK)
		
		data = new uint8_t[bytes_remaining];
		std::uint64_t readCount;
		DEASSERT_TRUE(reader.Read(bytes_remaining, data, &readCount).completed_ok());
		bytes_remaining -= readCount;
		
		DEASSERT_TRUE(vpx_codec_decode(context, data,
			(unsigned int)readCount, nullptr, 0) == VPX_CODEC_OK)
		
		image = vpx_codec_get_frame(context, &iter);
		DEASSERT_NOTNULL(image)
		
		switch(image->fmt){
		case VPX_IMG_FMT_I420:
		case VPX_IMG_FMT_I422:
		case VPX_IMG_FMT_I444:
			break;
			
		default:
			decString error;
			error.Format("Video format %x not supported", (int)image->fmt);
			DETHROW_INFO(deeInvalidParam, error);
		}
		
		DEASSERT_TRUE(image->bit_depth == 8)
		
		pInfos.SetWidth(image->d_w);
		pInfos.SetHeight(image->d_h);
		
		pInfos.SetColorConversionMatrix(pCreateColorConversionMatrix());
		
		delete [] data;
		vpx_codec_destroy(context);
		delete context;
		
	}catch(const deException &e){
		pModule.LogException(e);
		if(data){
			delete [] data;
		}
		if(context){
			vpx_codec_destroy(context);
		}
		pInfos.SetVideoCodec(dewmInfos::evcUnsupported);
		return webm::Status(100);
	}
	
	return webm::Status(webm::Status::Code::kOkCompleted);
}

webm::Status dewmWebmCallbackInfos::pProcessFirstFrameAudio(webm::Reader &, std::uint64_t &){
	return webm::Status(webm::Status::Code::kOkCompleted);
}

decColorMatrix3 dewmWebmCallbackInfos::pCreateColorConversionMatrix() const{
	// offsets: 16, 128, 128
	// exclusions: 219, 224, 224
	// kr = 0.299
	// kb = 0.114
	// 
	// Y' = (Y'-offY)/excY
	// Pb = (Cb-offCb)/excCb
	// Pr = (Cr-offCr)/excCr
	// 
	// R' = Y' + c1*Pr
	// G' = Y' + c2*Pb + c3*Pr
	// B' = Y' + c4*Pb
	// 
	// c1 = 2(1-kr)
	// c2 = -2(1-kb)kb / (1-kb-kr)
	// c3 = -2(1-kr)kr / (1-kb-kr)
	// c4 = 2(1-kb)
	// 
	// | R |   | a11 a12 a13 a14 |   | Y' |
	// | G | = | a21 a22 a23 a24 | * | Cb |
	// | B |   | a31 a32 a33 a34 |   | Cr |
	// 
	// a11 = 1/excY
	// a12 = 0
	// a13 = c1/excCr
	// a14 = -offY/excY - c1*offCr/excCr
	// a21 = 1/excY
	// a22 = c2/excCb
	// a23 = c3/excCr
	// a24 = -offY/excY - c2*offCb/excCb - c3*offCr/excCr
	// a31 = 1/excY
	// a32 = c4/excCb
	// a33 = 0
	// a34 = -offY/excY - c4*offCb/excCb
	// 
	// important note for creating the color conversation matrix. the game engine
	// expects the matrix to transform from [0..1] to [0..1] hence both the input
	// YCbCr values and the output RGB values have to be in the range from 0 to 1.
	
	float offY = 16.0f / 255.0f;
	float offCb = 128.0f / 255.0f;
	float offCr = 128.0f / 255.0f;
	
	float excY = 219.0f / 255.0f;
	float excCb = 224.0f / 255.0f;
	float excCr = 224.0f / 255.0f;
	
	float kr = 0.299f;
	float kb = 0.114f;
	
	float c1 = 2.0f * (1.0f - kr);
	float c2 = -2.0f * (1.0f - kb) * kb / (1.0f - kb - kr);
	float c3 = -2.0f * (1.0f - kr) * kr / (1.0f - kb - kr);
	float c4 = 2.0f * (1.0f - kb);
	
	// calculate matrix from parameters
	decColorMatrix3 m;
	
	m.a11 = 1.0f / excY;
	m.a12 = 0.0f;
	m.a13 = c1 / excCr;
	m.a14 = -offY / excY - c1 * offCr / excCr;
	
	m.a21 = m.a11;
	m.a22 = c2 / excCb;
	m.a23 = c3 / excCr;
	m.a24 = -offY / excY - c2 * offCb / excCb - c3 * offCr / excCr;
	
	m.a31 = m.a11;
	m.a32 = c4 / excCb;
	m.a33 = 0.0f;
	m.a34 = -offY / excY - c4 * offCb / excCb;
	
	return m;
}
