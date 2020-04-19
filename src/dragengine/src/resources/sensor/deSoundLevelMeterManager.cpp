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

#include "deSoundLevelMeter.h"
#include "deSoundLevelMeterManager.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deAudioSystem.h"
#include "../../systems/deScriptingSystem.h"



// Class deSoundLevelMeterManager
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deSoundLevelMeterManager::deSoundLevelMeterManager( deEngine *engine ) :
deResourceManager( engine, ertSoundLevelMeter ){
	SetLoggingName( "sound level meter" );
}

deSoundLevelMeterManager::~deSoundLevelMeterManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deSoundLevelMeterManager::GetSoundLevelMeterCount() const{
	return pSoundLevelMeters.GetCount();
}

deSoundLevelMeter *deSoundLevelMeterManager::GetRootSoundLevelMeter() const{
	return ( deSoundLevelMeter* )pSoundLevelMeters.GetRoot();
}

deSoundLevelMeter *deSoundLevelMeterManager::CreateSoundLevelMeter(){
	deSoundLevelMeter *soundLevelMeter = NULL;
	
	try{
		soundLevelMeter = new deSoundLevelMeter( this );
		
		GetAudioSystem()->LoadSoundLevelMeter( soundLevelMeter );
		GetScriptingSystem()->LoadSoundLevelMeter( soundLevelMeter );
		
		pSoundLevelMeters.Add( soundLevelMeter );
		
	}catch( const deException & ){
		if( soundLevelMeter ){
			soundLevelMeter->FreeReference();
		}
		throw;
	}
	
	return soundLevelMeter;
}



void deSoundLevelMeterManager::ReleaseLeakingResources(){
	const int count = GetSoundLevelMeterCount();
	if( count == 0 ){
		return;
	}
	
	LogWarnFormat( "%i leaking sound level meters", count );
	pSoundLevelMeters.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
}



// Systems support
////////////////////

void deSoundLevelMeterManager::SystemAudioLoad(){
	deSoundLevelMeter *soundLevelMeter = ( deSoundLevelMeter* )pSoundLevelMeters.GetRoot();
	deAudioSystem &audioSystem = *GetAudioSystem();
	
	while( soundLevelMeter ){
		if( ! soundLevelMeter->GetPeerAudio() ){
			audioSystem.LoadSoundLevelMeter( soundLevelMeter );
		}
		soundLevelMeter = ( deSoundLevelMeter* )soundLevelMeter->GetLLManagerNext();
	}
}

void deSoundLevelMeterManager::SystemAudioUnload(){
	deSoundLevelMeter *soundLevelMeter = ( deSoundLevelMeter* )pSoundLevelMeters.GetRoot();
	
	while( soundLevelMeter ){
		soundLevelMeter->SetPeerAudio( NULL );
		soundLevelMeter = ( deSoundLevelMeter* )soundLevelMeter->GetLLManagerNext();
	}
}

void deSoundLevelMeterManager::SystemScriptingLoad(){
	deSoundLevelMeter *soundLevelMeter = ( deSoundLevelMeter* )pSoundLevelMeters.GetRoot();
	deScriptingSystem &scriptingSystem = *GetScriptingSystem();
	
	while( soundLevelMeter ){
		if( ! soundLevelMeter->GetPeerScripting() ){
			scriptingSystem.LoadSoundLevelMeter( soundLevelMeter );
		}
		soundLevelMeter = ( deSoundLevelMeter* )soundLevelMeter->GetLLManagerNext();
	}
}

void deSoundLevelMeterManager::SystemScriptingUnload(){
	deSoundLevelMeter *soundLevelMeter = ( deSoundLevelMeter* )pSoundLevelMeters.GetRoot();
	
	while( soundLevelMeter ){
		soundLevelMeter->SetPeerScripting( NULL );
		soundLevelMeter = ( deSoundLevelMeter* )soundLevelMeter->GetLLManagerNext();
	}
}



void deSoundLevelMeterManager::RemoveResource( deResource *resource ){
	pSoundLevelMeters.RemoveIfPresent( resource );
}
