/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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

deVideoPlayerManager::deVideoPlayerManager( deEngine *engine ) : deResourceManager( engine, ertVideoPlayer ){
	SetLoggingName( "video player" );
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
	return ( deVideoPlayer* )pVideoPlayers.GetRoot();
}

deVideoPlayer *deVideoPlayerManager::CreateVideoPlayer(){
	deVideoPlayer *videoPlayer = NULL;
	
	try{
		videoPlayer = new deVideoPlayer( this );
		if( ! videoPlayer ) DETHROW( deeOutOfMemory );
		
		GetGraphicSystem()->LoadVideoPlayer( videoPlayer );
		GetAudioSystem()->LoadVideoPlayer( videoPlayer );
		
		pVideoPlayers.Add( videoPlayer );
		
	}catch( const deException & ){
		if( videoPlayer ){
			videoPlayer->FreeReference();
		}
		throw;
	}
	
	return videoPlayer;
}



void deVideoPlayerManager::ReleaseLeakingResources(){
	int count = GetVideoPlayerCount();
	
	if( count > 0 ){
		LogWarnFormat( "%i leaking video players", count );
		pVideoPlayers.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deVideoPlayerManager::SystemAudioLoad(){
	deVideoPlayer *videoPlayer = ( deVideoPlayer* )pVideoPlayers.GetRoot();
	deAudioSystem &system = *GetAudioSystem();
	
	while( videoPlayer ){
		if( ! videoPlayer->GetPeerAudio() ){
			system.LoadVideoPlayer( videoPlayer );
		}
		
		videoPlayer = ( deVideoPlayer* )videoPlayer->GetLLManagerNext();
	}
}

void deVideoPlayerManager::SystemAudioUnload(){
	deVideoPlayer *videoPlayer = ( deVideoPlayer* )pVideoPlayers.GetRoot();
	
	while( videoPlayer ){
		videoPlayer->SetPeerAudio( NULL );
		videoPlayer = ( deVideoPlayer* )videoPlayer->GetLLManagerNext();
	}
}

void deVideoPlayerManager::SystemGraphicLoad(){
	deVideoPlayer *videoPlayer = ( deVideoPlayer* )pVideoPlayers.GetRoot();
	deGraphicSystem &grasys = *GetGraphicSystem();
	
	while( videoPlayer ){
		if( ! videoPlayer->GetPeerGraphic() ){
			grasys.LoadVideoPlayer( videoPlayer );
		}
		
		videoPlayer = ( deVideoPlayer* )videoPlayer->GetLLManagerNext();
	}
}

void deVideoPlayerManager::SystemGraphicUnload(){
	deVideoPlayer *videoPlayer = ( deVideoPlayer* )pVideoPlayers.GetRoot();
	
	while( videoPlayer ){
		videoPlayer->SetPeerGraphic( NULL );
		videoPlayer = ( deVideoPlayer* )videoPlayer->GetLLManagerNext();
	}
}

void deVideoPlayerManager::RemoveResource( deResource *resource ){
	pVideoPlayers.RemoveIfPresent( resource );
}
