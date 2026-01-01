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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "deoalAVideoPlayer.h"
#include "deoalVideoPlayer.h"
#include "../deoalBasics.h"
#include "../deAudioOpenAL.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATLogger.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/video/deVideo.h>
#include <dragengine/resources/video/deVideoAudioDecoder.h>
#include <dragengine/resources/video/deVideoManager.h>


// Class deoalAVideoPlayer
////////////////////////////

// Constructor, destructor
////////////////////////////

deoalAVideoPlayer::deoalAVideoPlayer(deoalAudioThread &audioThread) :
pAudioThread(audioThread),
pPlayState(deVideoPlayer::epsStopped),
pPlayPosition(0.0f),
pPlayFrom(0.0f),
pPlayTo(0.0f),
pPlaySpeed(1.0f),
pLooping(false),
pBytesPerSample(1),
pChannelCount(1),
pSampleRate(11025),
pSampleCount(0),
pBufferSampleSize(pBytesPerSample * pSampleRate),
pSeekNextRead(false),
pUpdateTracker(1)
{
	LEAK_CHECK_CREATE(audioThread, VideoPlayer);
}

deoalAVideoPlayer::~deoalAVideoPlayer(){
	LEAK_CHECK_FREE(pAudioThread, VideoPlayer);
	
	pCleanUp();
}



// Management
///////////////

void deoalAVideoPlayer::SetVideo(deVideo *video){
	// WARNING Called during synchronization time by main thread
	
	pDecoder = NULL;
	pBytesPerSample = 1;
	pChannelCount = 1;
	pSampleRate = 11025;
	pSampleCount = 0;
	pBufferSampleSize = pBytesPerSample * pSampleRate;
	
	if(!video){
		return;
	}
	
	switch(video->GetBytesPerSample()){
	case 1:
	case 2:
		break;
		
	default:
		return;
	}
	
	pDecoder = pAudioThread.GetOal().GetGameEngine()->GetVideoManager()->CreateAudioDecoder(video);
	pBytesPerSample = video->GetBytesPerSample();
	pChannelCount = video->GetChannelCount();
	pSampleRate = video->GetSampleRate();
	pSampleCount = video->GetSampleCount();
	pBufferSampleSize = pBytesPerSample * pChannelCount;
	
	pAdvanceUpdateTracker();
}

void deoalAVideoPlayer::SetPlayState(deVideoPlayer::ePlayState playState){
	pPlayState = playState;
}

void deoalAVideoPlayer::SetPlayPosition(float position, bool seeking){
	pPlayPosition = position;
	pSeekNextRead |= seeking;
}

void deoalAVideoPlayer::SetPlayRange(float from, float to){
	pPlayFrom = from;
	pPlayTo = to;
}

void deoalAVideoPlayer::SetPlaySpeed(float playSpeed){
	pPlaySpeed = playSpeed;
}

void deoalAVideoPlayer::SetLooping(bool looping){
	pLooping = looping;
}



void deoalAVideoPlayer::ReadSamples(void *buffer, int bufferSize, int offset, int samples){
	char *dataBuffer = reinterpret_cast<char*>(buffer);
	
	if(pSeekNextRead){
		pSeekNextRead = false;
		pDecoder->SetPosition((int)(pPlayPosition * pSampleRate));
	}
	
	while(true){
		const int writtenCount = pDecoder->ReadSamples(dataBuffer, samples * pBufferSampleSize) / pBufferSampleSize;
		if(writtenCount == samples){
			return;
		}
		
		if(pLooping){
			dataBuffer += pBufferSampleSize * writtenCount;
			samples -= writtenCount;
			
		}else{
			int i;
			if(pBytesPerSample == 1){
				int8_t * const silenceData = (int8_t*)dataBuffer + writtenCount;
				for(i=writtenCount; i<samples; i++){
					silenceData[i] = 0;
				}
				
			}else{
				int16_t * const silenceData = (int16_t*)dataBuffer + writtenCount * 2;
				for(i=writtenCount; i<samples; i++){
					silenceData[i] = 0;
				}
			}
			return;
		}
	}
}



// Private Functions
//////////////////////

void deoalAVideoPlayer::pCleanUp(){
}

void deoalAVideoPlayer::pAdvanceUpdateTracker(){
	pUpdateTracker++;
	if(pUpdateTracker == 0){
		pUpdateTracker = 1;
	}
}
