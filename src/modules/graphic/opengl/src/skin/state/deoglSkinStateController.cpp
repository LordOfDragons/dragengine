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

#include "deoglSkinStateController.h"
#include "deoglSkinState.h"
#include "../deoglRSkin.h"
#include "../deoglSkinTexture.h"
#include "../../video/deoglSharedVideoPlayer.h"
#include "../../video/deoglSharedVideoPlayerList.h"
#include "../../video/deoglVideoPlayer.h"
#include "../../world/deoglWorld.h"
#include "../../deGraphicOpenGl.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/video/deVideoPlayer.h>
#include <dragengine/resources/video/deVideoPlayerManager.h>
#include <dragengine/deEngine.h>



// Class deoglSkinStateController
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinStateController::deoglSkinStateController() :
pSharedVideoPlayers(NULL),
pSharedVideoPlayerCount(0),
pHasCalculatedProperties(false),
pHasConstructedProperties(false){
}

deoglSkinStateController::~deoglSkinStateController(){
	SetVideoPlayerCount(0);
}



// Management
///////////////

void deoglSkinStateController::SetVideoPlayerCount(int count){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count == pSharedVideoPlayerCount){
		return;
	}
	
	// video players
	pVideoPlayers.RemoveAll();
	while(pVideoPlayers.GetCount() < count){
		pVideoPlayers.Add(NULL);
	}
	
	// shared video players
	if(pSharedVideoPlayers){
		while(pSharedVideoPlayerCount > 0){
			pSharedVideoPlayerCount--;
			if(pSharedVideoPlayers[pSharedVideoPlayerCount]){
				pSharedVideoPlayers[pSharedVideoPlayerCount]->FreeUsage();
				pSharedVideoPlayers[pSharedVideoPlayerCount] = NULL;
			}
		}
		
		delete [] pSharedVideoPlayers;
		pSharedVideoPlayers = NULL;
	}
	
	if(count > 0){
		pSharedVideoPlayers = new deoglSharedVideoPlayer*[count];
		
		for(pSharedVideoPlayerCount=0; pSharedVideoPlayerCount<count; pSharedVideoPlayerCount++){
			pSharedVideoPlayers[pSharedVideoPlayerCount] = NULL;
		}
	}
}

deVideoPlayer *deoglSkinStateController::GetVideoPlayerAt(int index) const{
	return (deVideoPlayer*)pVideoPlayers.GetAt(index);
}

deoglSharedVideoPlayer *deoglSkinStateController::GetSharedVideoPlayerAt(int index) const{
	if(index < 0 || index >= pSharedVideoPlayerCount){
		DETHROW(deeInvalidParam);
	}
	return pSharedVideoPlayers[index];
}

void deoglSkinStateController::SetVideoPlayerAt(int index, deVideoPlayer *videoPlayer){
	pVideoPlayers.SetAt(index, videoPlayer);
}

void deoglSkinStateController::SetSharedVideoPlayerAt(int index, deoglSharedVideoPlayer *videoPlayer){
	if(index < 0 || index >= pSharedVideoPlayerCount){
		DETHROW(deeInvalidParam);
	}
	
	if(videoPlayer == pSharedVideoPlayers[index]){
		return;
	}
	
	if(pSharedVideoPlayers[index]){
		pSharedVideoPlayers[index]->FreeUsage();
	}
	
	pSharedVideoPlayers[index] = videoPlayer;
}



void deoglSkinStateController::SetHasCalculatedProperties(bool hasCalculatedProperties){
	pHasCalculatedProperties = hasCalculatedProperties;
}

void deoglSkinStateController::SetHasConstructedProperties(bool hasConstructedProperties){
	pHasConstructedProperties = hasConstructedProperties;
}



bool deoglSkinStateController::RequiresSyncEveryFrameUpdate() const{
	return pVideoPlayers.GetCount() > 0 || pHasCalculatedProperties || pHasConstructedProperties;
}

bool deoglSkinStateController::RequiresPrepareRenderables() const{
	return pVideoPlayers.GetCount() > 0;
}

void deoglSkinStateController::Init(deoglSkinState &skinState, deoglRSkin *skin, deoglWorld *world){
	if(! skin || ! world){
		skinState.SetVideoPlayerCount(0);
		SetVideoPlayerCount(0);
		pHasCalculatedProperties = false;
		pHasConstructedProperties = false;
		return;
	}
	
	const int textureCount = skin->GetTextureCount();
	if(textureCount == 0){
		skinState.SetVideoPlayerCount(0);
		SetVideoPlayerCount(0);
		pHasCalculatedProperties = false;
		pHasConstructedProperties = false;
		return;
	}
	
	skinState.SetVideoPlayerCount(skin->GetVideoPlayerCount());
	SetVideoPlayerCount(skin->GetVideoPlayerCount());
	
	int i;
	for(i=0; i<textureCount; i++){
		pUpdateSkinVideoPlayers(skinState, skin->GetTextureAt(i), *world);
	}
	
	pHasCalculatedProperties = skin->GetCalculatedPropertyCount() > 0;
	pHasConstructedProperties = skin->GetConstructedPropertyCount() > 0;
}

void deoglSkinStateController::Init(deoglSkinState &skinState, deoglRSkin *skin,
int textureIndex, deoglWorld *world){
	if(! skin || ! world){
		skinState.SetVideoPlayerCount(0);
		SetVideoPlayerCount(0);
		pHasCalculatedProperties = false;
		pHasConstructedProperties = false;
		return;
	}
	
	const int textureCount = skin->GetTextureCount();
	if(textureCount == 0){
		skinState.SetVideoPlayerCount(0);
		SetVideoPlayerCount(0);
		pHasCalculatedProperties = false;
		pHasConstructedProperties = false;
		return;
	}
	
	skinState.SetVideoPlayerCount(skin->GetVideoPlayerCount());
	SetVideoPlayerCount(skin->GetVideoPlayerCount());
	pUpdateSkinVideoPlayers(skinState, skin->GetTextureAt(textureIndex), *world);
	
	pHasCalculatedProperties = skin->GetCalculatedPropertyCount() > 0;
	pHasConstructedProperties = skin->GetConstructedPropertyCount() > 0;
}

void deoglSkinStateController::ResetTime(){
	const int count = pVideoPlayers.GetCount();
	int i;
	
	for(i=0; i< count; i++){
		deVideoPlayer * const videoPlayer = (deVideoPlayer*)pVideoPlayers.GetAt(i);
		if(videoPlayer){
			videoPlayer->SetPlayPosition(0.0f);
		}
	}
}

void deoglSkinStateController::AdvanceTime(float timeStep){
	const int count = pVideoPlayers.GetCount();
	int i;
	
	for(i=0; i< count; i++){
		deVideoPlayer * const videoPlayer = (deVideoPlayer*)pVideoPlayers.GetAt(i);
		if(videoPlayer){
			videoPlayer->Update(timeStep);
		}
	}
}

void deoglSkinStateController::SyncToRender(){
	const int count = pVideoPlayers.GetCount();
	int i;
	
	for(i=0; i< count; i++){
		deVideoPlayer * const videoPlayer = (deVideoPlayer*)pVideoPlayers.GetAt(i);
		if(videoPlayer){
			((deoglVideoPlayer*)videoPlayer->GetPeerGraphic())->SyncToRender();
		}
	}
}

void deoglSkinStateController::Clear(){
	SetVideoPlayerCount(0);
	pHasCalculatedProperties = false;
	pHasConstructedProperties = false;
}



// Private Functions
//////////////////////

void deoglSkinStateController::pUpdateSkinVideoPlayers(deoglSkinState &skinState,
deoglSkinTexture &texture, deoglWorld &world){
	deoglSharedVideoPlayerList &svplist = world.GetSharedVideoPlayerList();
	int i;
	
	for(i=0; i<deoglSkinChannel::CHANNEL_COUNT; i++){
		if(! texture.IsChannelEnabled((deoglSkinChannel::eChannelTypes)i)){
			continue;
		}
		
		const deoglSkinChannel &channel = *texture.GetChannelAt((deoglSkinChannel::eChannelTypes)i);
		deoglSharedVideoPlayer *sharedVideoPlayer = NULL;
		const int vpindex = channel.GetVideoPlayer();
		deVideoPlayer *videoPlayer = NULL;
		
		deVideo *oldVideo = NULL;
		if(vpindex != -1){
			if(channel.GetSharedVideoPlayer()){
				sharedVideoPlayer = GetSharedVideoPlayerAt(vpindex);
				if(sharedVideoPlayer){
					oldVideo = sharedVideoPlayer->GetVideoPlayer()->GetVideo();
				}
				
			}else{
				videoPlayer = (deVideoPlayer*)pVideoPlayers.GetAt(vpindex);
				if(videoPlayer){
					oldVideo = videoPlayer->GetVideo();
				}
			}
		}
		
		if(oldVideo == channel.GetVideo()){
			continue;
		}
		
		if(channel.GetVideo()){
			if(channel.GetSharedVideoPlayer()){
				sharedVideoPlayer = NULL;
				
				try{
					// TODO hosting component or other object has to provide the playback speed. until
					//      this is done use 1.0f always.
					sharedVideoPlayer = svplist.GetVideoPlayerFor(channel.GetVideo(), 0, 1.0f);
					SetSharedVideoPlayerAt(vpindex, sharedVideoPlayer);
					skinState.SetVideoPlayerAt(vpindex, ((deoglVideoPlayer*)sharedVideoPlayer->
						GetVideoPlayer()->GetPeerGraphic())->GetRVideoPlayer());
					
				}catch(const deException &){
					if(sharedVideoPlayer){
						sharedVideoPlayer->FreeUsage();
					}
					throw;
				}
				
			}else{
				if(videoPlayer){
					videoPlayer->SetVideo(channel.GetVideo());
					
				}else{
					try{
						videoPlayer = world.GetOgl().GetGameEngine()->GetVideoPlayerManager()->CreateVideoPlayer();
						videoPlayer->SetVideo(channel.GetVideo());
						videoPlayer->SetLooping(true);
						videoPlayer->SetPlaySpeed(1.0f);
						videoPlayer->Play();
						pVideoPlayers.SetAt(vpindex, videoPlayer);
						videoPlayer->FreeReference();
						
					}catch(const deException &){
						if(videoPlayer){
							videoPlayer->FreeReference();
						}
						throw;
					}
					
					skinState.SetVideoPlayerAt(vpindex, ((deoglVideoPlayer*)
						videoPlayer->GetPeerGraphic())->GetRVideoPlayer());
				}
			}
			
		}else{
			if(channel.GetSharedVideoPlayer()){
				skinState.SetVideoPlayerAt(vpindex, NULL);
				
			}else{
				pVideoPlayers.SetAt(vpindex, NULL);
			}
			
			SetSharedVideoPlayerAt(vpindex, NULL);
		}
	}
}
