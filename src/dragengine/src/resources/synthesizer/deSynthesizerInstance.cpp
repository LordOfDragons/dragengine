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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deSynthesizer.h"
#include "deSynthesizerInstance.h"
#include "deSynthesizerInstanceManager.h"
#include "deSynthesizerController.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/audio/deBaseAudioSynthesizerInstance.h"
#include "../../systems/modules/synthesizer/deBaseSynthesizerSynthesizerInstance.h"



// Class deSynthesizerInstance
////////////////////////////////

// Constructor, destructor
////////////////////////////

deSynthesizerInstance::deSynthesizerInstance( deSynthesizerInstanceManager *manager ) :
deResource( manager ),

pSampleCount( 0 ),

pPeerSynthesizer( NULL ),
pPeerAudio( NULL ){
}

deSynthesizerInstance::~deSynthesizerInstance(){
	pCleanUp();
}



// Management
///////////////

void deSynthesizerInstance::SetSynthesizer( deSynthesizer *synthesizer ){
	if( pSynthesizer == synthesizer ){
		return;
	}
	
	pSynthesizer = synthesizer;
	
	if( synthesizer ){
		pSampleCount = synthesizer->GetSampleCount();
		
	}else{
		pSampleCount = 0;
	}
	
	pUpdateControllers();
	
	if( pPeerSynthesizer ){
		pPeerSynthesizer->SynthesizerChanged();
	}
	if( pPeerAudio ){
		pPeerAudio->SynthesizerChanged();
	}
}



void deSynthesizerInstance::SetSampleCount( int sampleCount ){
	sampleCount = decMath::max( sampleCount, 0 );
	
	if( sampleCount == pSampleCount ){
		return;
	}
	
	pSampleCount = sampleCount;
	
	if( pPeerSynthesizer ){
		pPeerSynthesizer->PlayTimeChanged();
	}
	if( pPeerAudio ){
		pPeerAudio->PlayTimeChanged();
	}
}



int deSynthesizerInstance::GetControllerCount() const{
	return pControllers.GetCount();
}

deSynthesizerController *deSynthesizerInstance::GetControllerAt( int index ) const{
	return ( deSynthesizerController* )pControllers.GetAt( index );
}

int deSynthesizerInstance::IndexOfControllerNamed( const char *name ) const{
	const int count = pControllers.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( deSynthesizerController* )pControllers.GetAt( i ) )->GetName() == name ){
			return i;
		}
	}
	
	return -1;
}

void deSynthesizerInstance::NotifyControllerChangedAt( int index ){
	if( index < 0 || index >= pControllers.GetCount() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pPeerSynthesizer ){
		pPeerSynthesizer->ControllerChanged( index );
	}
}



void deSynthesizerInstance::Reset(){
	if( pPeerSynthesizer ){
		return pPeerSynthesizer->Reset();
	}
}

void deSynthesizerInstance::GenerateSound( void *buffer, int bufferSize, int offset, int samples ){
	if( ! pPeerSynthesizer ){
		DETHROW( deeInvalidParam );
	}
	
	pPeerSynthesizer->GenerateSound( buffer, bufferSize, offset, samples );
}



// System Peers
/////////////////

void deSynthesizerInstance::SetPeerSynthesizer( deBaseSynthesizerSynthesizerInstance *peer ){
	if( peer == pPeerSynthesizer ){
		return;
	}
	
	if( pPeerSynthesizer ){
		delete pPeerSynthesizer;
	}
	
	pPeerSynthesizer = peer;
}

void deSynthesizerInstance::SetPeerAudio( deBaseAudioSynthesizerInstance *peer ){
	if( peer == pPeerAudio ){
		return;
	}
	
	if( pPeerAudio ){
		delete pPeerAudio;
	}
	
	pPeerAudio = peer;
}



// Private function
/////////////////////

void deSynthesizerInstance::pCleanUp(){
	if( pPeerAudio ){
		delete pPeerAudio;
		pPeerAudio = NULL;
	}
	if( pPeerSynthesizer ){
		delete pPeerSynthesizer;
		pPeerSynthesizer = NULL;
	}
}

void deSynthesizerInstance::pUpdateControllers(){
	pControllers.RemoveAll();
	
	if( ! pSynthesizer ){
		return;
	}
	
	const int controllerCount = pSynthesizer->GetControllerCount();
	deSynthesizerController *controller = NULL;
	int i;
	
	try{
		for( i=0; i<controllerCount; i++ ){
			controller = new deSynthesizerController( *pSynthesizer->GetControllerAt( i ) );
			pControllers.Add( controller );
			controller->FreeReference();
			controller = NULL;
		}
		
	}catch( const deException & ){
		if( controller ){
			controller->FreeReference();
		}
		throw;
	}
}
