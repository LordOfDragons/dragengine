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

#include "deSpeaker.h"
#include "deSpeakerManager.h"
#include "../../deEngine.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deAudioSystem.h"
#include "../../systems/deScriptingSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/audio/deBaseAudioSpeaker.h"
#include "../../systems/modules/scripting/deBaseScriptingSpeaker.h"
#include "../../common/file/decPath.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../common/exceptions.h"



// Class deSpeakerManager
///////////////////////////

// Constructor, destructor
////////////////////////////

deSpeakerManager::deSpeakerManager( deEngine *engine ) : deResourceManager( engine, ertSpeaker ){
	SetLoggingName( "speaker" );
}

deSpeakerManager::~deSpeakerManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deSpeakerManager::GetSpeakerCount() const{
	return pSpeakers.GetCount();
}

deSpeaker *deSpeakerManager::GetRootSpeaker() const{
	return ( deSpeaker* )pSpeakers.GetRoot();
}

deSpeaker *deSpeakerManager::CreateSpeaker(){
	deSpeaker *speaker = NULL;
	
	try{
		speaker = new deSpeaker( this );
		if( ! speaker ) DETHROW( deeOutOfMemory );
		
		GetAudioSystem()->LoadSpeaker( speaker );
		GetScriptingSystem()->LoadSpeaker( speaker );
		
		pSpeakers.Add( speaker );
		
	}catch( const deException & ){
		if( speaker ){
			speaker->FreeReference();
		}
		throw;
	}
	
	return speaker;
}



void deSpeakerManager::ReleaseLeakingResources(){
	int count = GetSpeakerCount();
	
	if( count > 0 ){
		LogWarnFormat( "%i leaking speakers", count );
		pSpeakers.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deSpeakerManager::SystemAudioLoad(){
	deSpeaker *speaker = ( deSpeaker* )pSpeakers.GetRoot();
	deAudioSystem &audSys = *GetAudioSystem();
	
	while( speaker ){
		if( ! speaker->GetPeerAudio() ){
			audSys.LoadSpeaker( speaker );
		}
		
		speaker = ( deSpeaker* )speaker->GetLLManagerNext();
	}
}

void deSpeakerManager::SystemAudioUnload(){
	deSpeaker *speaker = ( deSpeaker* )pSpeakers.GetRoot();
	
	while( speaker ){
		speaker->SetPeerAudio( NULL );
		speaker = ( deSpeaker* )speaker->GetLLManagerNext();
	}
}

void deSpeakerManager::SystemScriptingLoad(){
	deSpeaker *speaker = ( deSpeaker* )pSpeakers.GetRoot();
	deScriptingSystem &scriptingSystem = *GetScriptingSystem();
	
	while( speaker ){
		if( ! speaker->GetPeerScripting() ){
			scriptingSystem.LoadSpeaker( speaker );
		}
		speaker = ( deSpeaker* )speaker->GetLLManagerNext();
	}
}

void deSpeakerManager::SystemScriptingUnload(){
	deSpeaker *speaker = ( deSpeaker* )pSpeakers.GetRoot();
	
	while( speaker ){
		speaker->SetPeerScripting( NULL );
		speaker = ( deSpeaker* )speaker->GetLLManagerNext();
	}
}



void deSpeakerManager::RemoveResource( deResource *resource ){
	pSpeakers.RemoveIfPresent( resource );
}
