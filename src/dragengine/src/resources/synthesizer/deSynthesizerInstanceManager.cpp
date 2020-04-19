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

#include "deSynthesizerInstance.h"
#include "deSynthesizerInstanceManager.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deAudioSystem.h"
#include "../../systems/deSynthesizerSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/audio/deBaseAudioSynthesizerInstance.h"
#include "../../systems/modules/synthesizer/deBaseSynthesizerSynthesizerInstance.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"


// Class deSynthesizerInstanceManager
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deSynthesizerInstanceManager::deSynthesizerInstanceManager( deEngine *engine ) :
deResourceManager( engine, ertSynthesizerInstance ){
	SetLoggingName( "synthesizer instance" );
}

deSynthesizerInstanceManager::~deSynthesizerInstanceManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deSynthesizerInstanceManager::GetSynthesizerInstanceCount() const{
	return pInstances.GetCount();
}

deSynthesizerInstance *deSynthesizerInstanceManager::GetRootSynthesizerInstance() const{
	return ( deSynthesizerInstance* )pInstances.GetRoot();
}

deSynthesizerInstance *deSynthesizerInstanceManager::CreateSynthesizerInstance(){
	deSynthesizerInstance *instance = NULL;
	
	try{
		instance = new deSynthesizerInstance( this );
		
		GetSynthesizerSystem()->LoadSynthesizerInstance( instance );
		GetAudioSystem()->LoadSynthesizerInstance( instance );
		
		pInstances.Add( instance );
		
	}catch( const deException & ){
		if( instance ){
			instance->FreeReference();
		}
		
		throw;
	}
	
	return instance;
}



void deSynthesizerInstanceManager::ReleaseLeakingResources(){
	const int count = GetSynthesizerInstanceCount();
	
	if( count > 0 ){
		LogWarnFormat( "%i leaking synthesizer instances", count );
		pInstances.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deSynthesizerInstanceManager::SystemSynthesizerLoad(){
	deSynthesizerInstance *instance = ( deSynthesizerInstance* )pInstances.GetRoot();
	deSynthesizerSystem &synthSys = *GetSynthesizerSystem();
	
	while( instance ){
		if( ! instance->GetPeerSynthesizer() ){
			synthSys.LoadSynthesizerInstance( instance );
		}
		
		instance = ( deSynthesizerInstance* )instance->GetLLManagerNext();
	}
}

void deSynthesizerInstanceManager::SystemSynthesizerUnload(){
	deSynthesizerInstance *instance = ( deSynthesizerInstance* )pInstances.GetRoot();
	
	while( instance ){
		instance->SetPeerSynthesizer( NULL );
		instance = ( deSynthesizerInstance* )instance->GetLLManagerNext();
	}
}

void deSynthesizerInstanceManager::SystemAudioLoad(){
	deSynthesizerInstance *instance = ( deSynthesizerInstance* )pInstances.GetRoot();
	deAudioSystem &system = *GetAudioSystem();
	
	while( instance ){
		if( ! instance->GetPeerAudio() ){
			system.LoadSynthesizerInstance( instance );
		}
		instance = ( deSynthesizerInstance* )instance->GetLLManagerNext();
	}
}

void deSynthesizerInstanceManager::SystemAudioUnload(){
	deSynthesizerInstance *instance = ( deSynthesizerInstance* )pInstances.GetRoot();
	
	while( instance ){
		instance->SetPeerAudio( NULL );
		instance = ( deSynthesizerInstance* )instance->GetLLManagerNext();
	}
}



void deSynthesizerInstanceManager::RemoveResource( deResource *resource ){
	pInstances.RemoveIfPresent( resource );
}
