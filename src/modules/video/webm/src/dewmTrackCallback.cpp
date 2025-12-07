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
#include "dewmTrackCallback.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class dewmTrackCallback
////////////////////////////

// Constructor, destructor
////////////////////////////

dewmTrackCallback::dewmTrackCallback(deVideoWebm &module) :
pModule(module),
pTrackOpen(false),
pTrackNumber(0),
pBuffer(nullptr),
pBufferSize(0),
pStopParsing(false),
pNeedMoreFrames(false){
}

dewmTrackCallback::~dewmTrackCallback(){
	if(pBuffer){
		delete [] pBuffer;
	}
}



// Management
///////////////

webm::Status dewmTrackCallback::OnSegmentEnd(const webm::ElementMetadata &){
	pEndSegment();
	pStopParsing = false;
	return webm::Status(webm::Status::Code::kOkPartial); // stop here
}

webm::Status dewmTrackCallback::OnTrackEntry(const webm::ElementMetadata &,
const webm::TrackEntry &track_entry){
	if(pTrackOpen){
		return webm::Status(webm::Status::Code::kOkCompleted);
	}
	if(! track_entry.is_enabled.value()){
		return webm::Status(webm::Status::Code::kOkCompleted);
	}
	
	try{
		if(! pOpenTrack(track_entry)){
			return webm::Status(webm::Status::Code::kOkCompleted);
		}
		
	}catch(const deException &e){
		pModule.LogException(e);
		return webm::Status(100);
	}
	
	pTrackNumber = track_entry.track_number.value();
	pTrackOpen = true;
	return webm::Status(webm::Status::Code::kOkCompleted);
}

webm::Status dewmTrackCallback::OnSimpleBlockBegin(const webm::ElementMetadata &,
const webm::SimpleBlock &simple_block, webm::Action *action){
	return pProcessBlock(simple_block, action);
}

webm::Status dewmTrackCallback::OnBlockGroupBegin(const webm::ElementMetadata &, webm::Action *action){
	*action = webm::Action::kRead;
	return webm::Status(webm::Status::Code::kOkCompleted);
}

webm::Status dewmTrackCallback::OnBlockGroupEnd(const webm::ElementMetadata &,
const webm::BlockGroup &block_group){
	// WebM requires BlockMore.id to be 1. since BlockMore.id have to be unique
	// this means there exists at most 1 BlockMore per BlockGroup
	if(! block_group.additions.value().block_mores.empty()){
		try{
			pProcessAdditional(block_group.additions.value().block_mores.front().value().data.value());
			
		}catch(const deException &e){
			pModule.LogException(e);
			return webm::Status(100);
		}
	}
	
	return webm::Status(webm::Status::Code::kOkCompleted);
}

webm::Status dewmTrackCallback::OnBlockBegin(const webm::ElementMetadata &,
const webm::Block &block, webm::Action *action){
	return pProcessBlock(block, action);
}

webm::Status dewmTrackCallback::OnFrame(const webm::FrameMetadata &, webm::Reader *reader,
std::uint64_t *bytes_remaining){
	if(! pTrackOpen){
		pModule.LogError("Tried to process frame without open track");
		return webm::Status(100);
	}
	
	if(pStopParsing){
		pStopParsing = false;
		return webm::Status(webm::Status::Code::kOkPartial);
	}
	
	try{
		pProcessFrame(*reader, *bytes_remaining);
		if(*bytes_remaining > 0){
			// frame not consumed
			return webm::Status(webm::Status::Code::kOkPartial);
		}
		
	}catch(const deException &e){
		pModule.LogException(e);
		return webm::Status(100);
	}
	
	if(pNeedMoreFrames){
		pNeedMoreFrames = false;
		
	}else{
		// stop parsing at the next frame. we can not return kOkPartial here or
		// we will be parsing the same frame again causing problems
		pStopParsing = true;
	}
	
	return webm::Status(webm::Status::Code::kOkCompleted);
}



// Protected Functions
////////////////////////

void dewmTrackCallback::pProcessFrame(webm::Reader &reader, std::uint64_t &bytes_remaining){
	std::uint64_t skippedBytes;
	DEASSERT_TRUE(reader.Skip(bytes_remaining, &skippedBytes).completed_ok())
	bytes_remaining = 0;
}

void dewmTrackCallback::pProcessAdditional(const std::vector<unsigned char> &){
}

void dewmTrackCallback::pReadFrameData(webm::Reader &reader, std::uint64_t &bytes_remaining){
	if(bytes_remaining > pBufferSize){
		if(pBuffer){
			delete [] pBuffer;
			pBuffer = nullptr;
		}
		
		pBuffer = new uint8_t[bytes_remaining];
		pBufferSize = bytes_remaining;
	}
	
	std::uint64_t readCount;
	DEASSERT_TRUE(reader.Read(bytes_remaining, pBuffer, &readCount).completed_ok())
	bytes_remaining = 0;
}

void dewmTrackCallback::SetNeedMoreFrames(bool needMoreFrames){
	pNeedMoreFrames = needMoreFrames;
}

void dewmTrackCallback::pEndSegment(){
}



// Private Functions
//////////////////////

webm::Status dewmTrackCallback::pProcessBlock(const webm::Block &block, webm::Action *action){
	if(! pTrackOpen){
		pModule.LogError("Tried to process block without open track");
		*action = webm::Action::kSkip;
		return webm::Status(100);
	}
	
	if(block.track_number == pTrackNumber){
		*action = webm::Action::kRead;
		
	}else{
		*action = webm::Action::kSkip;
	}
	
	return webm::Status(webm::Status::Code::kOkCompleted);
}
