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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deMicrophone.h"
#include "deMicrophoneManager.h"
#include "../../deEngine.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deAudioSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/audio/deBaseAudioMicrophone.h"
#include "../../common/file/decPath.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../common/exceptions.h"



// Class deMicrophoneManager
//////////////////////////////

// Constructor, destructor
////////////////////////////

deMicrophoneManager::deMicrophoneManager( deEngine *engine ) : deResourceManager( engine, ertMicrophone ){
	SetLoggingName( "microphone" );
}

deMicrophoneManager::~deMicrophoneManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deMicrophoneManager::GetMicrophoneCount() const{
	return pMicrophones.GetCount();
}

deMicrophone *deMicrophoneManager::GetRootMicrophone() const{
	return ( deMicrophone* )pMicrophones.GetRoot();
}

deMicrophone *deMicrophoneManager::CreateMicrophone(){
	deMicrophone *microphone = NULL;
	
	try{
		microphone = new deMicrophone( this );
		if( ! microphone ) DETHROW( deeOutOfMemory );
		
		GetAudioSystem()->LoadMicrophone( microphone );
		
		pMicrophones.Add( microphone );
		
	}catch( const deException & ){
		if( microphone ){
			microphone->FreeReference();
		}
		throw;
	}
	
	return microphone;
}



void deMicrophoneManager::ReleaseLeakingResources(){
	int count = GetMicrophoneCount();
	
	if( count > 0 ){
		LogWarnFormat( "%i leaking microphones", count );
		pMicrophones.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deMicrophoneManager::SystemAudioLoad(){
	deMicrophone *microphone = ( deMicrophone* )pMicrophones.GetRoot();
	deAudioSystem &audSys = *GetAudioSystem();
	
	while( microphone ){
		if( ! microphone->GetPeerAudio() ){
			audSys.LoadMicrophone( microphone );
		}
		
		microphone = ( deMicrophone* )microphone->GetLLManagerNext();
	}
}

void deMicrophoneManager::SystemAudioUnload(){
	deMicrophone *microphone = ( deMicrophone* )pMicrophones.GetRoot();
	
	while( microphone ){
		microphone->SetPeerAudio( NULL );
		microphone = ( deMicrophone* )microphone->GetLLManagerNext();
	}
}

void deMicrophoneManager::RemoveResource( deResource *resource ){
	pMicrophones.RemoveIfPresent( resource );
}
