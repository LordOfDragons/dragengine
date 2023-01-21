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

#include "deSpeaker.h"
#include "deMicrophone.h"
#include "deMicrophoneManager.h"
#include "../world/deWorld.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/audio/deBaseAudioMicrophone.h"



// Class deMicrophone
///////////////////////

// Constructor, destructor
////////////////////////////

deMicrophone::deMicrophone( deMicrophoneManager *manager ) :
deResource( manager ),

pType( emtPoint ),

pMuted( false ),
pVolume( 1.0f ),
pSpeakerGain( 1.0f ),

pSpeakerRoot( NULL ),
pSpeakerTail( NULL ),
pSpeakerCount( 0 ),

pPeerAudio( NULL ),

pParentWorld( NULL ),
pLLWorldPrev( NULL ),
pLLWorldNext( NULL )
{
	pLayerMask.SetBit( 0 );
}

deMicrophone::~deMicrophone(){
	pCleanUp();
}



// Management
///////////////

void deMicrophone::SetType( eMicrophoneType type ){
	if( type < emtPoint || type > emtDirected ){
		DETHROW( deeInvalidParam );
	}
	
	if( type == pType ){
		return;
	}
	
	pType = type;
	
	if( pPeerAudio ){
		pPeerAudio->TypeChanged();
	}
}



void deMicrophone::SetPosition( const decDVector &position ){
	if( position.IsEqualTo( pPosition ) ){
		return;
	}
	
	pPosition = position;
	
	if( pPeerAudio ){
		pPeerAudio->PositionChanged();
	}
}

void deMicrophone::SetOrientation( const decQuaternion &orientation ){
	if( orientation.IsEqualTo( pOrientation ) ){
		return;
	}
	
	pOrientation = orientation;
	
	if( pPeerAudio ){
		pPeerAudio->OrientationChanged();
	}
}

void deMicrophone::SetVelocity( const decVector &velocity ){
	if( velocity.IsEqualTo( pVelocity ) ){
		return;
	}
	
	pVelocity = velocity;
	
	if( pPeerAudio ){
		pPeerAudio->VelocityChanged();
	}
}



void deMicrophone::SetMuted( bool muted ){
	if( muted == pMuted ){
		return;
	}
	
	pMuted = muted;
	
	if( pPeerAudio ){
		pPeerAudio->MutedChanged();
	}
}

void deMicrophone::SetVolume( float volume ){
	if( volume < 0.0f ){
		volume = 0.0f;
	}
	
	if( fabsf( volume - pVolume ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pVolume = volume;
	
	if( pPeerAudio ){
		pPeerAudio->VolumeChanged();
	}
}



void deMicrophone::SetLayerMask( const decLayerMask &layerMask ){
	if( layerMask == pLayerMask ){
		return;
	}
	
	pLayerMask = layerMask;
	
	if( pPeerAudio ){
		pPeerAudio->LayerMaskChanged();
	}
}

void deMicrophone::SetSpeakerGain( float gain ){
	gain = decMath::max( gain, 0.0f );
	if( fabsf( gain - pSpeakerGain ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pSpeakerGain = gain;
	
	if( pPeerAudio ){
		pPeerAudio->SpeakerGainChanged();
	}
}



// Speakers
/////////////

void deMicrophone::AddSpeaker( deSpeaker *speaker ){
	if( ! speaker || speaker->GetParentMicrophone() || speaker->GetParentWorld() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pSpeakerTail ){
		pSpeakerTail->SetLLMicrophoneNext( speaker );
		speaker->SetLLMicrophonePrev( pSpeakerTail );
		speaker->SetLLMicrophoneNext( NULL ); // not required by definition, just to make sure...
		
	}else{
		speaker->SetLLMicrophonePrev( NULL ); // not required by definition, just to make sure...
		speaker->SetLLMicrophoneNext( NULL ); // not required by definition, just to make sure...
		pSpeakerRoot = speaker;
	}
	
	pSpeakerTail = speaker;
	pSpeakerCount++;
	speaker->SetParentMicrophone( this );
	speaker->AddReference();
	
	if( pPeerAudio ){
		pPeerAudio->SpeakerAdded( speaker );
	}
}

void deMicrophone::RemoveSpeaker( deSpeaker *speaker ){
	if( ! speaker || speaker->GetParentMicrophone() != this ){
		DETHROW( deeInvalidParam );
	}
	
	if( speaker->GetLLMicrophonePrev() ){
		speaker->GetLLMicrophonePrev()->SetLLMicrophoneNext( speaker->GetLLMicrophoneNext() );
	}
	if( speaker->GetLLMicrophoneNext() ){
		speaker->GetLLMicrophoneNext()->SetLLMicrophonePrev( speaker->GetLLMicrophonePrev() );
	}
	if( speaker == pSpeakerRoot ){
		pSpeakerRoot = speaker->GetLLMicrophoneNext();
	}
	if( speaker == pSpeakerTail ){
		pSpeakerTail = speaker->GetLLMicrophonePrev();
	}
	pSpeakerCount--;
	
	speaker->SetParentMicrophone( NULL );
	speaker->SetLLMicrophonePrev( NULL );
	speaker->SetLLMicrophoneNext( NULL );
	if( pPeerAudio ){
		pPeerAudio->SpeakerRemoved( speaker );
	}
	speaker->FreeReference();
}

void deMicrophone::RemoveAllSpeakers(){
	if( pPeerAudio ){
		pPeerAudio->AllSpeakersRemoved();
	}
	
	while( pSpeakerTail ){
		deSpeaker * const next = pSpeakerTail->GetLLMicrophonePrev();
		pSpeakerTail->FreeReference();
		pSpeakerTail = next;
		pSpeakerCount--;
	}
	pSpeakerRoot = NULL;
}



// System Peers
/////////////////

void deMicrophone::SetPeerAudio( deBaseAudioMicrophone *audMicrophone ){
	if( audMicrophone == pPeerAudio ){
		return;
	}
	
	if( pPeerAudio ){
		delete pPeerAudio;
	}
	
	pPeerAudio = audMicrophone;
}



// Linked List
////////////////

void deMicrophone::SetParentWorld( deWorld *world ){
	pParentWorld = world;
}

void deMicrophone::SetLLWorldPrev( deMicrophone *microphone ){
	pLLWorldPrev = microphone;
}

void deMicrophone::SetLLWorldNext( deMicrophone *microphone ){
	pLLWorldNext = microphone;
}



// Private Functions
//////////////////////

void deMicrophone::pCleanUp(){
	if( pPeerAudio ){
		delete pPeerAudio;
		pPeerAudio = NULL;
	}
	
	RemoveAllSpeakers();
}
