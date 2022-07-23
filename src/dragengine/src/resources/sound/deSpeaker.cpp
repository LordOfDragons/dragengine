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

#include "deSpeaker.h"
#include "deSound.h"
#include "deSoundManager.h"
#include "deSpeakerManager.h"
#include "../deResource.h"
#include "../deResourceManager.h"
#include "../synthesizer/deSynthesizerInstance.h"
#include "../synthesizer/deSynthesizer.h"
#include "../video/deVideo.h"
#include "../video/deVideoPlayer.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/audio/deBaseAudioSpeaker.h"
#include "../../systems/modules/scripting/deBaseScriptingSpeaker.h"



// Class deSpeaker
////////////////////

// Constructor, destructor
////////////////////////////

deSpeaker::deSpeaker( deSpeakerManager *manager ) :
deResource( manager ),

pType( estPoint ),

pMuted( false ),
pPlayState( epsStopped ),
pLooping( false ),
pPlayFrom( 0 ),
pPlayTo( 0 ),
pPlaySpeed( 1.0f ),
pVolume( 1.0f ),
pRange( 1.0f ),
pRollOff( 1.0f ),
pDistanceOffset( 0.0f ),

pPeerAudio( NULL ),
pPeerScripting( NULL ),

pParentWorld( NULL ),
pLLWorldPrev( NULL ),
pLLWorldNext( NULL ),

pParentMicrophone( NULL ),
pLLMicrophonePrev( NULL ),
pLLMicrophoneNext( NULL )
{
	pLayerMask.SetBit( 0 );
}

deSpeaker::~deSpeaker(){
	pCleanUp();
}



// Management
///////////////

void deSpeaker::SetType( eSpeakerType type ){
	if( type < estPoint || type > estDirected ){
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

void deSpeaker::SetSound( deSound *sound ){
	if( sound == pSound ){
		return;
	}
	
	pSound = sound;
	
	pPlayFrom = 0;
	if( sound ){
		pPlayTo = sound->GetSampleCount();
		
	}else{
		pPlayTo = 0;
	}
	
	// notify the audio peer
	if( pPeerAudio ){
		pPeerAudio->SourceChanged();
	}
}

void deSpeaker::SetSynthesizer( deSynthesizerInstance *synthesizer ){
	if( synthesizer == pSynthesizer ){
		return;
	}
	
	pSynthesizer = synthesizer;
	
	pPlayFrom = 0;
	if( synthesizer ){
		pPlayTo = synthesizer->GetSampleCount();
		
	}else{
		pPlayTo = 0;
	}
	
	if( pPeerAudio ){
		pPeerAudio->SourceChanged();
	}
}

void deSpeaker::SetVideoPlayer( deVideoPlayer *videoPlayer ){
	if( videoPlayer == pVideoPlayer ){
		return;
	}
	
	pVideoPlayer = videoPlayer;
	
	pPlayFrom = 0;
	if( videoPlayer && videoPlayer->GetVideo() ){
		pPlayTo = videoPlayer->GetVideo()->GetSampleCount();
		
	}else{
		pPlayTo = 0;
	}
	
	if( pPeerAudio ){
		pPeerAudio->SourceChanged();
	}
}



void deSpeaker::SetPosition( const decDVector &position ){
	if( position.IsEqualTo( pPosition ) ){
		return;
	}
	
	pPosition = position;
	
	if( pPeerAudio ){
		pPeerAudio->PositionChanged();
	}
}

void deSpeaker::SetOrientation( const decQuaternion &orientation ){
	if( orientation.IsEqualTo( pOrientation ) ){
		return;
	}
	
	pOrientation = orientation;
	
	if( pPeerAudio ){
		pPeerAudio->OrientationChanged();
	}
}

void deSpeaker::SetVelocity( const decVector &velocity ){
	if( velocity.IsEqualTo( pVelocity ) ){
		return;
	}
	
	pVelocity = velocity;
	
	if( pPeerAudio ){
		pPeerAudio->VelocityChanged();
	}
}



void deSpeaker::SetMuted( bool muted ){
	if( muted == pMuted ){
		return;
	}
	
	pMuted = muted;
	
	if( pPeerAudio ){
		pPeerAudio->MutedChanged();
	}
}

void deSpeaker::SetLooping( bool looping ){
	if( looping == pLooping ){
		return;
	}
	
	pLooping = looping;
	
	if( pPeerAudio ){
		pPeerAudio->LoopingChanged();
	}
}

void deSpeaker::SetPlayPosition( int playFrom, int playTo ){
	playFrom = decMath::max( playFrom, 0 );
	playTo = decMath::max( playTo, playFrom );
	
	if( playFrom == pPlayFrom && playTo == pPlayTo ){
		return;
	}
	
	pPlayFrom = playFrom;
	pPlayTo = playTo;
	
	if( pPeerAudio ){
		pPeerAudio->PlayPositionChanged();
	}
}

void deSpeaker::SetPlaySpeed( float playSpeed ){
	playSpeed = decMath::max( playSpeed, 0.0f );
	
	if( fabsf( playSpeed - pPlaySpeed ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pPlaySpeed = playSpeed;
	
	if( pPeerAudio ){
		pPeerAudio->PlaySpeedChanged();
	}
}

void deSpeaker::SetVolume( float volume ){
	volume = decMath::max( volume, 0.0f );
	
	if( fabsf( volume - pVolume ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pVolume = volume;
	
	if( pPeerAudio ){
		pPeerAudio->VolumeChanged();
	}
}

void deSpeaker::SetRange( float range ){
	range = decMath::max( range, 0.01f );
	
	if( fabsf( range - pRange ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pRange = range;
	
	if( pPeerAudio ){
		pPeerAudio->RangeChanged();
	}
}

void deSpeaker::SetRollOff( float rollOff ){
	rollOff = decMath::clamp( rollOff, 0.0f, 1.0f );
	
	if( fabsf( rollOff - pRollOff ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pRollOff = rollOff;
	
	if( pPeerAudio ){
		pPeerAudio->RollOffChanged();
	}
}

void deSpeaker::SetDistanceOffset( float distanceOffset ){
	distanceOffset = decMath::max( distanceOffset, 0.0f );
	
	if( fabsf( distanceOffset - pDistanceOffset ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pDistanceOffset = distanceOffset;
	
	if( pPeerAudio ){
		pPeerAudio->DistanceOffsetChanged();
	}
}

void deSpeaker::SetShape( const decShapeList &shape ){
	pShape = shape;
	
	if( pPeerAudio ){
		pPeerAudio->ShapeChanged();
	}
}

void deSpeaker::SetLayerMask( const decLayerMask &layerMask ){
	if( layerMask == pLayerMask ){
		return;
	}
	
	pLayerMask = layerMask;
	
	if( pPeerAudio ){
		pPeerAudio->LayerMaskChanged();
	}
}



void deSpeaker::SetPlayState( ePlayStates playState ){
	if( playState == pPlayState ){
		return;
	}
	
	pPlayState = playState;
	
	if( pPeerAudio ){
		pPeerAudio->PlayStateChanged();
	}
}

void deSpeaker::Play(){
	SetPlayState( epsPlaying );
}

void deSpeaker::Stop(){
	SetPlayState( epsStopped );
}

void deSpeaker::Pause(){
	SetPlayState( epsPaused );
}



// System Peers
/////////////////

void deSpeaker::SetPeerAudio( deBaseAudioSpeaker *audSpeaker ){
	if( audSpeaker == pPeerAudio ){
		return;
	}
	
	if( pPeerAudio){
		delete pPeerAudio;
	}
	   pPeerAudio = audSpeaker;
}

void deSpeaker::SetPeerScripting( deBaseScriptingSpeaker *peer ){
	if( peer == pPeerScripting ){
		return;
	}
	
	if( pPeerScripting ){
		delete pPeerScripting;
	}
	pPeerScripting = peer;
}



// Linked list world
//////////////////////

void deSpeaker::SetParentWorld( deWorld *world ){
	pParentWorld = world;
}

void deSpeaker::SetLLWorldPrev( deSpeaker *speaker ){
	pLLWorldPrev = speaker;
}

void deSpeaker::SetLLWorldNext( deSpeaker *speaker ){
	pLLWorldNext = speaker;
}



// Linked list microphone
///////////////////////////

void deSpeaker::SetParentMicrophone( deMicrophone *microphone ){
	pParentMicrophone = microphone;
}

void deSpeaker::SetLLMicrophonePrev( deSpeaker *speaker ){
	pLLMicrophonePrev = speaker;
}

void deSpeaker::SetLLMicrophoneNext( deSpeaker *speaker ){
	pLLMicrophoneNext = speaker;
}



// Private Functions
//////////////////////

void deSpeaker::pCleanUp(){
	if( pPeerScripting ){
		delete pPeerScripting;
	}
	if( pPeerAudio ){
		delete pPeerAudio;
	}
}
