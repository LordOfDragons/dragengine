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

#include "deVideoPlayer.h"
#include "deVideoPlayerManager.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../common/file/decPath.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deAudioSystem.h"
#include "../../systems/deGraphicSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/graphic/deBaseGraphicVideoPlayer.h"
#include "../../systems/modules/audio/deBaseAudioVideoPlayer.h"



// Class deVideoPlayerManager
///////////////////////////////

// Constructor, destructor
////////////////////////////

deVideoPlayerManager::deVideoPlayerManager(deEngine *engine) : deResourceManager(engine, ertVideoPlayer){
	SetLoggingName("video player");
}

deVideoPlayerManager::~deVideoPlayerManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deVideoPlayerManager::GetVideoPlayerCount() const{
	return pVideoPlayers.GetCount();
}

deVideoPlayer *deVideoPlayerManager::GetRootVideoPlayer() const{
	return (deVideoPlayer*)pVideoPlayers.GetRoot();
}

deVideoPlayer::Ref deVideoPlayerManager::CreateVideoPlayer(){
	const deVideoPlayer::Ref videoPlayer(deVideoPlayer::Ref::New(this));
	GetGraphicSystem()->LoadVideoPlayer(videoPlayer);
	GetAudioSystem()->LoadVideoPlayer(videoPlayer);
	pVideoPlayers.Add(videoPlayer);
	return videoPlayer;
}



void deVideoPlayerManager::ReleaseLeakingResources(){
	int count = GetVideoPlayerCount();
	
	if(count > 0){
		LogWarnFormat("%i leaking video players", count);
		pVideoPlayers.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deVideoPlayerManager::SystemAudioLoad(){
	deVideoPlayer *videoPlayer = (deVideoPlayer*)pVideoPlayers.GetRoot();
	deAudioSystem &system = *GetAudioSystem();
	
	while(videoPlayer){
		if(!videoPlayer->GetPeerAudio()){
			system.LoadVideoPlayer(videoPlayer);
		}
		
		videoPlayer = (deVideoPlayer*)videoPlayer->GetLLManagerNext();
	}
}

void deVideoPlayerManager::SystemAudioUnload(){
	deVideoPlayer *videoPlayer = (deVideoPlayer*)pVideoPlayers.GetRoot();
	
	while(videoPlayer){
		videoPlayer->SetPeerAudio(NULL);
		videoPlayer = (deVideoPlayer*)videoPlayer->GetLLManagerNext();
	}
}

void deVideoPlayerManager::SystemGraphicLoad(){
	deVideoPlayer *videoPlayer = (deVideoPlayer*)pVideoPlayers.GetRoot();
	deGraphicSystem &grasys = *GetGraphicSystem();
	
	while(videoPlayer){
		if(!videoPlayer->GetPeerGraphic()){
			grasys.LoadVideoPlayer(videoPlayer);
		}
		
		videoPlayer = (deVideoPlayer*)videoPlayer->GetLLManagerNext();
	}
}

void deVideoPlayerManager::SystemGraphicUnload(){
	deVideoPlayer *videoPlayer = (deVideoPlayer*)pVideoPlayers.GetRoot();
	
	while(videoPlayer){
		videoPlayer->SetPeerGraphic(NULL);
		videoPlayer = (deVideoPlayer*)videoPlayer->GetLLManagerNext();
	}
}

void deVideoPlayerManager::RemoveResource(deResource *resource){
	pVideoPlayers.RemoveIfPresent(resource);
}
