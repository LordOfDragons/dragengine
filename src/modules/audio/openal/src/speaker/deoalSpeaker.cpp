/* 
 * Drag[en]gine OpenAL Audio Module
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

#include "deoalSpeaker.h"
#include "deoalASpeaker.h"
#include "../deAudioOpenAL.h"
#include "../microphone/deoalMicrophone.h"
#include "../sound/deoalDecodeBuffer.h"
#include "../sound/deoalASound.h"
#include "../sound/deoalSound.h"
#include "../synthesizer/deoalASynthesizerInstance.h"
#include "../synthesizer/deoalSynthesizerInstance.h"
#include "../video/deoalAVideoPlayer.h"
#include "../video/deoalVideoPlayer.h"
#include "../world/deoalWorld.h"
#include "../world/deoalWorld.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/deResource.h>
#include <dragengine/resources/deResourceManager.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/resources/sound/deSpeaker.h>
#include <dragengine/resources/sound/deSound.h>
#include <dragengine/resources/sound/deSoundDecoder.h>
#include <dragengine/resources/sound/deSoundDecoderReference.h>
#include <dragengine/resources/sound/deSoundManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/video/deVideo.h>
#include <dragengine/resources/video/deVideoPlayer.h>
#include <dragengine/resources/synthesizer/deSynthesizerInstance.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>



// Class deoalSpeaker
///////////////////////

// Constructor, destructor
////////////////////////////

deoalSpeaker::deoalSpeaker( deAudioOpenAL &oal, deSpeaker &speaker ) :
pOal( oal ),
pSpeaker( speaker ),
pASpeaker( NULL ),
pParentWorld( NULL ),
pParentMicrophone( NULL ),
pSound( NULL ),
pSynthesizer( NULL ),
pVideoPlayer( NULL ),

pSynthesizerUpdateTracker( 0 ),

pDirtySpeaker( true ),
pDirtyGeometry( true),
pDirtyOctreeNode( true ),
pDirtySource( true ),
pDirtySoundDecoder( true ),
pDirtyVelocity( true ),
pDirtyRange( true ),
pDirtyAttenuation( true ),
pDirtyPlayRange( true ),
pDirtyLayerMask( true ),
pDirtyVideoPlayer( false ),
pDirtyResetStreaming( false ),

pLLSyncWorld( this ),
pLLSyncMic( this )
{
	try{
		pASpeaker = new deoalASpeaker( oal.GetAudioThread() );
		pASpeaker->SetBackLink( this );
		
		SourceChanged();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoalSpeaker::~deoalSpeaker(){
	pCleanUp();
}



// Management
///////////////

void deoalSpeaker::SetParentWorld( deoalWorld *world ){
	if( world == pParentWorld ){
		return;
	}
	
	pParentWorld = world;
	
	pDirtyOctreeNode = true;
}

void deoalSpeaker::SetParentMicrophone( deoalMicrophone *microphone ){
	pParentMicrophone = microphone;
}



void deoalSpeaker::Synchronize(){
	if( pASpeaker->GetPlaying() && pASpeaker->GetPlayFinished() ){
		pSpeaker.Stop();
	}
	
	pSyncSource();
	
	if( pSpeaker.GetPlaying() ){
		if( pDirtySoundDecoder ){
			deSoundDecoderReference soundDecoder;
			if( pSound && pSound->GetASound()->GetStreaming() ){
				soundDecoder.TakeOver( pOal.GetGameEngine()->GetSoundManager()
					->CreateDecoder( &pSound->GetSound() ) );
			}
			pASpeaker->SetSoundDecoder( soundDecoder );
			pDirtySoundDecoder = false;
		}
		
		pASpeaker->SynchronizeStreaming();
		
		if( pASpeaker->GetSource() && pSynthesizer ){
			pSynthesizer->Synchronize();
			if( pSynthesizer->GetUpdateTracker() != pSynthesizerUpdateTracker ){
				pSynthesizerUpdateTracker = pSynthesizer->GetUpdateTracker();
				pDirtyResetStreaming = true;
			}
		}
	}
	
	if( pDirtyVideoPlayer ){
		if( pVideoPlayer ){
			pVideoPlayer->Synchronize();
		}
		pDirtyVideoPlayer = false;
	}
	
	if( pDirtySpeaker ){
		pASpeaker->SetSpeakerType( pSpeaker.GetType() );
		pASpeaker->SetMuted( pSpeaker.GetMuted() );
		pASpeaker->SetVolume( pSpeaker.GetVolume() );
		pASpeaker->SetAttenuationRolloff( pSpeaker.GetRollOff() );
		
		if( pVideoPlayer ){
			// this is required to allow video player to update internal states too
			pVideoPlayer->Synchronize();
			
			// ensure values are taken from video player. When deoalVideoPlayer changes these
			// values no notification will be triggered here since the values are identical.
			// if the user tried to change the values they will be reset here
			const deVideoPlayer &videoPlayer = pVideoPlayer->GetVideoPlayer();
			
			pASpeaker->SetLooping( videoPlayer.GetLooping() );
			pASpeaker->SetPlaySpeed( videoPlayer.GetPlaySpeed() );
			
			switch( videoPlayer.GetPlayState() ){
			case deVideoPlayer::epsPlaying:
				pASpeaker->SetPlayState( deSpeaker::epsPlaying );
				break;
				
			case deVideoPlayer::epsPaused:
				pASpeaker->SetPlayState( deSpeaker::epsPaused );
				break;
				
			case deVideoPlayer::epsStopped:
				pASpeaker->SetPlayState( deSpeaker::epsStopped );
				break;
			}
			
		}else{
			pASpeaker->SetLooping( pSpeaker.GetLooping() );
			pASpeaker->SetPlaySpeed( pSpeaker.GetPlaySpeed() );
			pASpeaker->SetPlayState( pSpeaker.GetPlayState() );
		}
		
		pDirtySpeaker = false;
	}
	
	if( pDirtyPlayRange ){
		if( pVideoPlayer && pVideoPlayer->GetVideoPlayer().GetVideo() ){
			// ensure values are taken from video player
			const deVideoPlayer &videoPlayer = pVideoPlayer->GetVideoPlayer();
			const int sampleRate = videoPlayer.GetVideo()->GetSampleRate();
			
			pASpeaker->SetSpeakerPlayRange(
				( int )( videoPlayer.GetPlayFrom() * sampleRate ),
				( int )( videoPlayer.GetPlayTo() * sampleRate ) );
			
		}else{
			pASpeaker->SetSpeakerPlayRange( pSpeaker.GetPlayFrom(), pSpeaker.GetPlayTo() );
		}
		
		pDirtyPlayRange = false;
	}
	
	if( pDirtyResetStreaming ){
		pASpeaker->ResetStreaming();
		pDirtyResetStreaming = false;
	}
	
	if( pDirtyGeometry ){
		pASpeaker->SetGeometry( pSpeaker.GetPosition(), pSpeaker.GetOrientation() );
		pDirtyGeometry = false;
	}
	
	if( pDirtyRange ){
		pASpeaker->SetRange( pSpeaker.GetRange() );
		pDirtyRange = false;
	}
	
	if( pDirtyAttenuation ){
		pASpeaker->SetAttenuationRolloff( pSpeaker.GetRollOff() );
		pDirtyRange = false;
	}
	
	if( pDirtyVelocity ){
		pASpeaker->SetVelocity( pSpeaker.GetVelocity() );
		pDirtyVelocity = false;
	}
	
	if( pDirtyLayerMask ){
		pASpeaker->SetLayerMask( pSpeaker.GetLayerMask() );
		pDirtyLayerMask = false;
	}
	
	if( pDirtyOctreeNode ){
		pASpeaker->UpdateOctreeNode();
		pDirtyOctreeNode = false;
	}
	
	// force synchronization the next time if playing non-looping. this is required since
	// no synchronization is triggered if the playing reaches the end in deoalASpeaker.
	// without keeping synchronizing this event can not be detected
	// 
	// NOTE extended to non-looping case otherwise moving out of range stops the sound
	//      processing causing sound to start where it left of if entering range again
	if( pSpeaker.GetPlaying() ){ //  && ! pSpeaker.GetLooping()
		if( pParentWorld ){
			pParentWorld->AddSyncSpeaker( this );
		}
		if( pParentMicrophone ){
			pParentMicrophone->AddSyncSpeaker( this );
		}
	}
}

void deoalSpeaker::VideoPlayerRequiresSync(){
	pDirtyVideoPlayer = true;
	
	pRequiresSync();
}

void deoalSpeaker::DirtyResetStreaming(){
	pDirtyResetStreaming = true;
	
	pRequiresSync();
}



// Notifications
//////////////////

void deoalSpeaker::TypeChanged(){
	pDirtySpeaker = true;
	
	pRequiresSync();
}

void deoalSpeaker::SourceChanged(){
	pDirtySource = true;
	pDirtyPlayRange = true;
	pDirtySoundDecoder = true;
	pDirtyOctreeNode = true;
	
	pRequiresSync();
}

void deoalSpeaker::PositionChanged(){
	pDirtyGeometry = true;
	pDirtyOctreeNode = true;
	
	pRequiresSync();
}

void deoalSpeaker::OrientationChanged(){
	pDirtyGeometry = true;
	pDirtyOctreeNode = true;
	
	pRequiresSync();
}

void deoalSpeaker::VelocityChanged(){
	pDirtyVelocity = true;
	
	pRequiresSync();
}

void deoalSpeaker::MutedChanged(){
	pDirtySpeaker = true;
	
	pRequiresSync();
}

void deoalSpeaker::LoopingChanged(){
	pDirtySpeaker = true;
	
	pRequiresSync();
}

void deoalSpeaker::PlayPositionChanged(){
	pDirtyPlayRange = true;
	
	pRequiresSync();
}

void deoalSpeaker::PlaySpeedChanged(){
	pDirtySpeaker = true;
	
	pRequiresSync();
}

void deoalSpeaker::VolumeChanged(){
	pDirtySpeaker = true;
	
	pRequiresSync();
}

void deoalSpeaker::RangeChanged(){
	pDirtyAttenuation = true;
	pDirtyOctreeNode = true;
	pDirtyRange = true;
	
	pRequiresSync();
}

void deoalSpeaker::RollOffChanged(){
	pDirtyAttenuation = true;
	
	pRequiresSync();
}

void deoalSpeaker::LayerMaskChanged(){
	pDirtyLayerMask = true;
	
	pRequiresSync();
}

void deoalSpeaker::PlayStateChanged(){
	pDirtySpeaker = true;
	pDirtyOctreeNode = true;
	
	pRequiresSync();
}



// Private Functions
//////////////////////

void deoalSpeaker::pCleanUp(){
	if( pVideoPlayer ){
		pVideoPlayer->RemoveSpeaker( this );
		pVideoPlayer = NULL;
	}
	
	if( pASpeaker ){
		pASpeaker->SetBackLink( NULL );
		pASpeaker->FreeReference();
	}
}



void deoalSpeaker::pSyncSource(){
	if( ! pDirtySource ){
		return;
	}
	
	pSound = NULL;
	if( pSpeaker.GetSound() ){
		pSound = ( deoalSound* )pSpeaker.GetSound()->GetPeerAudio();
	}
	if( pSound ){
		pSound->Synchronize();
	}
	
	pSynthesizer = NULL;
	pSynthesizerUpdateTracker = 0;
	if( pSpeaker.GetSynthesizer() ){
		pSynthesizer = ( deoalSynthesizerInstance* )pSpeaker.GetSynthesizer()->GetPeerAudio();
	}
	if( pSynthesizer ){
		pSynthesizer->Synchronize();
	}
	
	if( pVideoPlayer ){
		pVideoPlayer->RemoveSpeaker( this );
		pVideoPlayer = NULL;
	}
	if( pSpeaker.GetVideoPlayer() ){
		pVideoPlayer = ( deoalVideoPlayer* )pSpeaker.GetVideoPlayer()->GetPeerAudio();
		pVideoPlayer->AddSpeaker( this );
	}
	if( pVideoPlayer ){
		pVideoPlayer->Synchronize();
	}
	
	pASpeaker->SetSource( pSound ? pSound->GetASound() : NULL,
		pSynthesizer ? pSynthesizer->GetAInstance() : NULL,
		pVideoPlayer ? pVideoPlayer->GetAVideoPlayer() : NULL );
	
	pDirtySoundDecoder = true;
	
	pDirtySource = false;
}


void deoalSpeaker::pRequiresSync(){
	if( ! pLLSyncWorld.GetList() && pParentWorld ){
		pParentWorld->AddSyncSpeaker( this );
	}
	if( ! pLLSyncMic.GetList() && pParentMicrophone ){
		pParentMicrophone->AddSyncSpeaker( this );
	}
}
