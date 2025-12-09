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

#include "deoalVideoPlayer.h"
#include "deoalAVideoPlayer.h"
#include "../deoalBasics.h"
#include "../deAudioOpenAL.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATDebug.h"
#include "../speaker/deoalSpeaker.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/sound/deSpeaker.h>
#include <dragengine/resources/video/deVideo.h>



// Class deoalVideoPlayer
///////////////////////////

// Constructor, destructor
////////////////////////////

deoalVideoPlayer::deoalVideoPlayer(deAudioOpenAL &oal, deVideoPlayer &videoPlayer) :
pOal(oal),
pVideoPlayer(videoPlayer),
pAVideoPlayer(deoalAVideoPlayer::Ref::NewWith(oal.GetAudioThread())),
pDirtyVideo(true),
pDirtyParameters(true),
pDirtyPlayPosition(true),
pRequiresSeeking(false){
}

deoalVideoPlayer::~deoalVideoPlayer(){
}



// Management
///////////////

void deoalVideoPlayer::AddSpeaker(deoalSpeaker *speaker){
	pSpeakers.AddIfAbsent(speaker);
}

void deoalVideoPlayer::RemoveSpeaker(deoalSpeaker *speaker){
	pSpeakers.RemoveIfPresent(speaker);
}

void deoalVideoPlayer::Synchronize(){
	// WARNING Called during synchronization time from main thread.
	
	if(pDirtyVideo){
		pAVideoPlayer->SetVideo(pVideoPlayer.GetVideo());
		pDirtyVideo = false;
	}
	
	if(pDirtyParameters){
		pAVideoPlayer->SetLooping(pVideoPlayer.GetLooping());
		pAVideoPlayer->SetPlaySpeed(pVideoPlayer.GetPlaySpeed());
		pAVideoPlayer->SetPlayState(pVideoPlayer.GetPlayState());
		pAVideoPlayer->SetPlayRange(pVideoPlayer.GetPlayFrom(), pVideoPlayer.GetPlayTo());
		pDirtyParameters = false;
	}
	
	if(pDirtyPlayPosition){
		pAVideoPlayer->SetPlayPosition(pVideoPlayer.GetPlayPosition(), pRequiresSeeking);
		pRequiresSeeking = false;
		pDirtyPlayPosition = false;
	}
}



// Notifications
//////////////////

void deoalVideoPlayer::SourceChanged(){
	PlayRangeChanged(); // also sends VideoPlayerRequiresSync notification
	
	pDirtyVideo = true;
}

void deoalVideoPlayer::LoopingChanged(){
	const int count = pSpeakers.GetCount();
	int i;
	for(i=0; i<count; i++){
		deoalSpeaker &speaker = *((deoalSpeaker*)pSpeakers.GetAt(i));
		speaker.GetSpeaker().SetLooping(pVideoPlayer.GetLooping());
		speaker.VideoPlayerRequiresSync();
	}
	
	pDirtyParameters = true;
}

void deoalVideoPlayer::PlayRangeChanged(){
	const int count = pSpeakers.GetCount();
	if(count > 0){
		int from = 0, to = 0;
		if(pVideoPlayer.GetVideo()){
			const int sampleRate = pVideoPlayer.GetVideo()->GetSampleRate();
			from = (int)(pVideoPlayer.GetPlayFrom() * sampleRate);
			to = (int)(pVideoPlayer.GetPlayTo() * sampleRate);
		}
		
		int i;
		for(i=0; i<count; i++){
			deoalSpeaker &speaker = *((deoalSpeaker*)pSpeakers.GetAt(i));
			speaker.GetSpeaker().SetPlayPosition(from, to);
			speaker.VideoPlayerRequiresSync();
			speaker.DirtyResetStreaming();
		}
	}
	
	pDirtyParameters = true;
}

void deoalVideoPlayer::PlaySpeedChanged(){
	const int count = pSpeakers.GetCount();
	int i;
	for(i=0; i<count; i++){
		deoalSpeaker &speaker = *((deoalSpeaker*)pSpeakers.GetAt(i));
		speaker.GetSpeaker().SetPlaySpeed(pVideoPlayer.GetPlaySpeed());
		speaker.VideoPlayerRequiresSync();
	}
	
	pDirtyParameters = true;
}

void deoalVideoPlayer::PlayPositionChanged(bool seeking){
	const int count = pSpeakers.GetCount();
	int i;
	for(i=0; i<count; i++){
		deoalSpeaker &speaker = *((deoalSpeaker*)pSpeakers.GetAt(i));
		speaker.VideoPlayerRequiresSync();
		if(seeking){
			speaker.DirtyResetStreaming();
		}
	}
	
	pDirtyPlayPosition = true;
	pRequiresSeeking |= seeking;
}

void deoalVideoPlayer::PlayStateChanged(){
	const int count = pSpeakers.GetCount();
	int i;
	for(i=0; i<count; i++){
		deoalSpeaker &speaker = *((deoalSpeaker*)pSpeakers.GetAt(i));
		
		switch(pVideoPlayer.GetPlayState()){
		case deVideoPlayer::epsPlaying:
			speaker.GetSpeaker().SetPlayState(deSpeaker::epsPlaying);
			break;
			
		case deVideoPlayer::epsPaused:
			speaker.GetSpeaker().SetPlayState(deSpeaker::epsPaused);
			break;
			
		case deVideoPlayer::epsStopped:
			speaker.GetSpeaker().SetPlayState(deSpeaker::epsStopped);
			break;
		}
		
		speaker.VideoPlayerRequiresSync();
	}
	
	pDirtyParameters = true;
}
