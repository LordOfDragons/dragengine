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
