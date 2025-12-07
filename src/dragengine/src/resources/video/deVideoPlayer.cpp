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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "deVideo.h"
#include "deVideoManager.h"
#include "deVideoPlayer.h"
#include "deVideoPlayerManager.h"
#include "../deResource.h"
#include "../deResourceManager.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/audio/deBaseAudioVideoPlayer.h"
#include "../../systems/modules/graphic/deBaseGraphicVideoPlayer.h"



// Class deVideoPlayer
////////////////////////

// Constructor, destructor
////////////////////////////

deVideoPlayer::deVideoPlayer(deVideoPlayerManager *manager) :
deResource(manager),

pPlayState(epsStopped),
pLooping(false),
pPlayFrom(0.0f),
pPlayTo(0.0f),
pPlaySpeed(1.0f),
pPlayPosition(0.0f),

pPeerGraphic(NULL),
pPeerAudio(NULL){
}

deVideoPlayer::~deVideoPlayer(){
	pCleanUp();
}



// Management
///////////////

void deVideoPlayer::SetVideo(deVideo *video){
	if(video == pVideo){
		return;
	}
	
	pVideo = video;
	
	pPlayFrom = 0.0f;
	
	if(video){
		pPlayTo = video->GetPlayTime();
		
	}else{
		pPlayTo = 0.0f;
	}
	
	pPlayPosition = pPlayFrom;
	
	if(pPeerGraphic){
		pPeerGraphic->SourceChanged();
	}
	if(pPeerAudio){
		pPeerAudio->SourceChanged();
	}
}



void deVideoPlayer::SetLooping(bool looping){
	if(looping == pLooping){
		return;
	}
	
	pLooping = looping;
	
	if(pPeerGraphic){
		pPeerGraphic->LoopingChanged();
	}
	if(pPeerAudio){
		pPeerAudio->LoopingChanged();
	}
}

void deVideoPlayer::SetPlayRange(float playFrom, float playTo){
	playFrom = decMath::max(playFrom, 0.0f);
	playTo = decMath::min(playTo, playFrom);
	
	if(fabsf(playFrom - pPlayFrom) <= FLOAT_SAFE_EPSILON && fabsf(playTo - pPlayTo) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pPlayFrom = playFrom;
	pPlayTo = playTo;
	
	if(pPlayPosition < pPlayFrom){
		pPlayPosition = pPlayFrom;
	}
	if(pPlayPosition > pPlayTo){
		pPlayPosition = pPlayTo;
	}
	
	if(pPeerGraphic){
		pPeerGraphic->PlayRangeChanged();
	}
	if(pPeerAudio){
		pPeerAudio->PlayRangeChanged();
	}
}

void deVideoPlayer::SetPlaySpeed(float playSpeed){
	playSpeed = decMath::max(playSpeed, 0.0f);
	
	if(fabsf(playSpeed - pPlaySpeed) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pPlaySpeed = playSpeed;
	
	if(pPeerGraphic){
		pPeerGraphic->PlaySpeedChanged();
	}
	if(pPeerAudio){
		pPeerAudio->PlaySpeedChanged();
	}
}

void deVideoPlayer::SetPlayPosition(float position){
	pSetPlayPosition(position, true);
}



void deVideoPlayer::SetPlayState(ePlayState playState){
	if(playState == pPlayState){
		return;
	}
	
	pPlayState = playState;
	
	if(pPeerGraphic){
		pPeerGraphic->PlayStateChanged();
	}
	if(pPeerAudio){
		pPeerAudio->PlayStateChanged();
	}
}

void deVideoPlayer::Play(){
	SetPlayState(epsPlaying);
}

void deVideoPlayer::Stop(){
	SetPlayState(epsStopped);
}

void deVideoPlayer::Pause(){
	SetPlayState(epsPaused);
}



void deVideoPlayer::Update(float elapsed){
	if(pPlayState != epsPlaying){
		return;
	}
	
	const float position = pPlayPosition + elapsed * pPlaySpeed;
	
	if(pLooping){
		pSetPlayPosition(decMath::normalize(position, pPlayFrom, pPlayTo), false);
		
	}else{
		pSetPlayPosition(position, false);
		
		if(position >= pPlayTo){
			Stop();
		}
	}
}



// System Peers
/////////////////

void deVideoPlayer::SetPeerGraphic(deBaseGraphicVideoPlayer *peer){
	if(peer == pPeerGraphic){
		return;
	}
	
	if(pPeerGraphic){
		delete pPeerGraphic;
	}
	pPeerGraphic = peer;
}

void deVideoPlayer::SetPeerAudio(deBaseAudioVideoPlayer *peer){
	if(peer == pPeerAudio){
		return;
	}
	
	if(pPeerAudio){
		delete pPeerAudio;
	}
	pPeerAudio = peer;
}



// Private Functions
//////////////////////

void deVideoPlayer::pCleanUp(){
	if(pPeerGraphic){
		delete pPeerGraphic;
		pPeerGraphic = NULL;
	}
	if(pPeerAudio){
		delete pPeerAudio;
		pPeerAudio = NULL;
	}
}

void deVideoPlayer::pSetPlayPosition(float position, bool seeking){
	position = decMath::clamp(position, pPlayFrom, pPlayTo);
	if(fabsf(position - pPlayPosition) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pPlayPosition = position;
	
	if(pPeerGraphic){
		pPeerGraphic->PlayPositionChanged(seeking);
	}
	if(pPeerAudio){
		pPeerAudio->PlayPositionChanged(seeking);
	}
}
