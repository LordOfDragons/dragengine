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

#include "deoalASpeaker.h"
#include "../deAudioOpenAL.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATLogger.h"
#include "../audiothread/deoalATDelayed.h"
#include "../audiothread/deoalDelayedDeletion.h"
#include "../environment/deoalEnvironment.h"
#include "../effect/deoalFilter.h"
#include "../extensions/deoalExtensions.h"
#include "../microphone/deoalAMicrophone.h"
#include "../sound/deoalDecodeBuffer.h"
#include "../sound/deoalASound.h"
#include "../soundLevelMeter/deoalASoundLevelMeter.h"
#include "../soundLevelMeter/deoalASoundLevelMeterSpeaker.h"
#include "../source/deoalSource.h"
#include "../source/deoalSourceManager.h"
#include "../synthesizer/deoalASynthesizerInstance.h"
#include "../synthesizer/deoalSynthesizerInstance.h"
#include "../video/deoalAVideoPlayer.h"
#include "../video/deoalVideoPlayer.h"
#include "../world/deoalAWorld.h"
#include "../world/octree/deoalWorldOctree.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/deResource.h>
#include <dragengine/resources/deResourceManager.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/resources/sound/deSpeaker.h>
#include <dragengine/resources/sound/deSound.h>
#include <dragengine/resources/sound/deSoundDecoder.h>
#include <dragengine/resources/sound/deSoundManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/deSynthesizerInstance.h>



// Class deoalASpeaker
////////////////////////

// Constructor, destructor
////////////////////////////

deoalASpeaker::deoalASpeaker( deoalAudioThread &audioThread ) :
pAudioThread( audioThread ),
pBackLink( NULL ),

pParentWorld( NULL ),
pParentMicrophone( NULL ),
pOctreeNode( NULL ),
pSound( NULL ),
pSynthesizer( NULL ),
pVideoPlayer( NULL ),
pSourceUpdateTracker( 0 ),
pSpeakerType( deSpeaker::estPoint ),
pPositionless( true ),
pStreaming( false ),
pEnabled( false ),
pFlag( false ),
pNeedsInitialDecode( false ),

pDirtyGeometry( true ),
pDirtyGain( true ),
pDirtyLooping( true ),
pDirtyAttenuation( true ),
pDirtyPlaySpeed( true ),
pDirtyPlayState( true ),
pDirtyPlayPosition( true ),

pPlayState( deSpeaker::epsStopped ),
pLooping( false ),
pMuted( false ),
pRestart( true ),

pSource( NULL ),
pBufferSampleCount( 1 ),
pBufferSize( 1 ),
pBufferSampleSize( 1 ),
pBufferData( NULL ),
pBufferDataCapacity( 0 ),
pQueueSampleOffset( 0 ),

pSampleRate( 44100 ),
pPlayPosition( 0 ),
pPlayFrom( 0 ),
pPlayTo( 0 ),
pPlayRange( 0 ),
pPlaySpeed( 1.0f ),
pSpeakerPlayFrom( 0 ),
pSpeakerPlayTo( 0 ),

pPlayFinished( false ),

pRange( 1.0f ),
pRangeSquared( 1.0f ),
pVolume( 1.0f ),
pAttenuationRefDist( 1.0f ),
pAttenuationRolloff( 1.0f ),

pEnvironment( NULL ),

pMicrophoneMarkedRemove( false ),
pWorldMarkedRemove( false ),
pLLWorldPrev( NULL ),
pLLWorldNext( NULL )
{
	LEAK_CHECK_CREATE( audioThread, Speaker );
}

deoalASpeaker::~deoalASpeaker(){
	LEAK_CHECK_FREE( pAudioThread, Speaker );
	
	pCleanUp();
}



// Management
///////////////

void deoalASpeaker::SetBackLink( deoalSpeaker *speaker ){
	pBackLink = speaker;
}



void deoalASpeaker::SetSource( deoalASound *sound, deoalASynthesizerInstance *synthesizer,
deoalAVideoPlayer *videoPlayer ){
	// WARNING Called during synchronization time from main thread.
	
	if( pSound == sound && pSynthesizer == synthesizer && pVideoPlayer == videoPlayer ){
		return;
	}
	// drop old source and decoder if present
	pSoundDecoder = NULL;
	
	if( pVideoPlayer ){
		pVideoPlayer->FreeReference();
		pVideoPlayer = NULL;
	}
	if( pSynthesizer ){
		pSynthesizer->FreeReference();
		pSynthesizer = NULL;
	}
	if( pSound ){
		pSound->FreeReference();
		pSound = NULL;
	}
	pSourceUpdateTracker = 0;
	
	pDirtyPlayState = true;
	pRestart = true;
	
	// store new source
	pSound = sound;
	if( sound ){
		sound->AddReference();
	}
	
	pSynthesizer = synthesizer;
	if( synthesizer ){
		synthesizer->AddReference();
	}
	
	pVideoPlayer = videoPlayer;
	if( videoPlayer ){
		videoPlayer->AddReference();
	}
	
	// update play position since speaker updates it from new source if present
	pUpdatePlayRange();
}

void deoalASpeaker::SetSoundDecoder( deSoundDecoder *decoder ){
	// WARNING Called during synchronization time from main thread.
	
	pSoundDecoder = decoder;
	pNeedsInitialDecode = decoder != NULL;
}



void deoalASpeaker::SetSpeakerType( deSpeaker::eSpeakerType type ){
	pSpeakerType = type;
}

void deoalASpeaker::SetGeometry( const decDVector &position, const decQuaternion &orientation ){
	pPosition = position;
	pOrientation = orientation;
	
	if( pEnvironment ){
		pEnvironment->SetPosition( position );
	}
	
	const int count = pSoundLevelMeters.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		deoalASoundLevelMeterSpeaker * const speaker =
			( ( deoalASoundLevelMeter* )pSoundLevelMeters.GetAt( i ) )->GetSpeakerWith( this );
		if( speaker ){
			speaker->SpeakerPositionChanged();
		}
	}
	
	pDirtyGeometry = true;
}

void deoalASpeaker::SetVelocity( const decVector &velocity ){
	pVelocity = velocity;
	
	pDirtyGeometry = true;
}

void deoalASpeaker::SetLayerMask( const decLayerMask &layerMask ){
	pLayerMask = layerMask;
	
	if( pEnvironment ){
		pEnvironment->SetLayerMask( layerMask );
	}
	
	const int count = pSoundLevelMeters.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		deoalASoundLevelMeterSpeaker * const speaker =
			( ( deoalASoundLevelMeter* )pSoundLevelMeters.GetAt( i ) )->GetSpeakerWith( this );
		if( speaker ){
			speaker->SpeakerLayerMaskChanged();
		}
	}
}



void deoalASpeaker::SetPlayState( deSpeaker::ePlayStates playState ){
	// WARNING Called during synchronization time from main thread.
	
	pPlayState = playState;
	
	pDirtyPlayState = true;
}

void deoalASpeaker::SetLooping( bool looping ){
	pLooping = looping;
	
	pDirtyLooping = true;
}

void deoalASpeaker::SetMuted( bool muted ){
	pMuted = muted;
	
	pDirtyGain = true;
}

void deoalASpeaker::SetSpeakerPlayRange( int from, int to ){
	pSpeakerPlayFrom = from;
	pSpeakerPlayTo = to;
	
	pDirtyPlayPosition = true;
	
	pUpdatePlayRange();
	
	pQueueSampleOffset = pPlayPosition = pPlayFrom;
}



void deoalASpeaker::PrepareProcessAudio(){
	if( pDirtyPlayState ){
		pDirtyPlayState = false;
		pDoPlayState();
	}
	
	pCheckStillSourceOwner();
	if( ! pSource ){
		if( pPlayState == deSpeaker::epsPlaying ){
			// there ware two ways to handle this.
			// 
			// the first way is the precise way by advancing the sample offset. this is accurate
			// and ensures sound continues while not audible. this allows a player to drop in on
			// an already running sound. without this simulation the sound would start at the
			// beginning not somewhere in the middle.
			// 
			// for non-looping sound a second solution is possible. you simply stop immediatly.
			// this prevents players from dropping into running sound but is also simpler.
			// 
			// right now the first solution is used unless it is shown to be a problem
			pPlayPosition += ( int )( ( float )pSampleRate * pAudioThread.GetElapsed() );
			if( pPlayPosition >= pPlayTo ){
				if( pLooping ){
					pPlayPosition = decMath::normalize( pPlayPosition, pPlayFrom, pPlayTo );
					
				}else{
					pPlayPosition = pPlayTo;
					pPlayFinished = true;
				}
			}
		}
		
		return;
	}
	
	if( pDirtyPlayPosition ){
		pDirtyPlayPosition = false;
		if( ! pStreaming ){
			OAL_CHECK( pAudioThread, alSourcei( pSource->GetSource(), AL_SAMPLE_OFFSET, pPlayPosition ) );
		}
	}
	
	// check for end of play
	bool underrun = false;
	pPlayFinished = false;
	
	if( pPlayState == deSpeaker::epsPlaying && ! pNeedsInitialDecode ){
		ALint state;
		OAL_CHECK( pAudioThread, alGetSourcei( pSource->GetSource(), AL_SOURCE_STATE, &state ) );
		
		if( pLooping ){
			if( state == AL_STOPPED ){
				pAudioThread.GetLogger().LogWarnFormat( "Buffer Underrun: sound=%p synthesizer=%p videoplayer=%p",
					pSound, pSynthesizer, pVideoPlayer );
				underrun = true;
			}
			
		}else{
			// NOTE pNeedsInitialDecode check is required since some code path (especially streaming
			//      ones) require an initial decode. this is done at the bottom of this method.
			//      without this initial decode the no sound buffers are yet present and this
			//      check here would incorrectly consider the sound having finished playing
			ALint queueOffset;
			OAL_CHECK( pAudioThread, alGetSourcei( pSource->GetSource(), AL_SAMPLE_OFFSET, &queueOffset ) );
			pPlayPosition = pQueueSampleOffset + queueOffset;
			
			// if the playback stopped AL_SAMPLE_OFFSET will return 0 which is a problem because
			// we need to know where we are at the end of the last buffer. so we have to calculate
			// this on our own. we lookat the number of processed buffers and assume for all these
			// to contain pBufferSampleCount. this is not fully correct since the last buffer can
			// be smaller if it is really the last buffer in the source to play. by clamping to
			// the maximum play range this problem can be solved
			if( state == AL_STOPPED ){
				ALint numProcessed;
				OAL_CHECK( pAudioThread, alGetSourcei( pSource->GetSource(), AL_BUFFERS_PROCESSED, &numProcessed ) );
				pPlayPosition = decMath::min( pPlayPosition + pBufferSampleCount * numProcessed, pPlayTo );
			}
			
			if( state == AL_STOPPED || pPlayPosition >= pPlayTo ){
				// we have to be careful about not getting tricked by buffer underrun condition.
				// openal makes it difficult for us to properly detect this
				if( state == AL_STOPPED && pPlayPosition < pPlayTo && pStreaming ){
					pAudioThread.GetLogger().LogWarnFormat( "Buffer Underrun: sound=%p synthesizer=%p videoplayer=%p",
						pSound, pSynthesizer, pVideoPlayer );
					underrun = true;
					
				}else{
					pPlayPosition = pPlayTo;
					pQueueSampleOffset = 0;
					pPlayFinished = true;
					return;
				}
			}
		}
	}
	
	// update if playing
	if( pPlayState == deSpeaker::epsPlaying ){
		if( pDirtyGeometry ){
			pDirtyGeometry = false;
			
			if( pPositionless ){
				OAL_CHECK( pAudioThread, alSourcei( pSource->GetSource(),
					AL_SOURCE_RELATIVE, AL_TRUE ) );
				OAL_CHECK( pAudioThread, alSource3f( pSource->GetSource(),
					AL_POSITION, 0.0f, 0.0f, 0.0f ) );
				OAL_CHECK( pAudioThread, alSource3f( pSource->GetSource(),
					AL_VELOCITY, 0.0f, 0.0f, 0.0f ) );
				
			}else{
				OAL_CHECK( pAudioThread, alSourcei( pSource->GetSource(),
					AL_SOURCE_RELATIVE, AL_FALSE ) );
				OAL_CHECK( pAudioThread, alSource3f( pSource->GetSource(), AL_POSITION,
					( float )pPosition.x, ( float )pPosition.y, ( float )-pPosition.z ) );
				OAL_CHECK( pAudioThread, alSource3f( pSource->GetSource(), AL_VELOCITY,
					pVelocity.x, pVelocity.y, -pVelocity.z ) );
			}
			
			if( pPositionless ){
				OAL_CHECK( pAudioThread, alSource3f( pSource->GetSource(),
					AL_DIRECTION, 0.0f, 0.0f, 0.0f ) );
				
			}else if( pSpeakerType == deSpeaker::estPoint ){
				OAL_CHECK( pAudioThread, alSource3f( pSource->GetSource(),
					AL_DIRECTION, 0.0f, 0.0f, 0.0f ) );
				
			}else{
				const decVector view( decMatrix::CreateFromQuaternion( pOrientation ).TransformView() );
				OAL_CHECK( pAudioThread, alSource3f( pSource->GetSource(),
					AL_DIRECTION, view.x, view.y, -view.z ) );
			}
		}
		
		if( pDirtyAttenuation ){
			pDirtyAttenuation = false;
			
			OAL_CHECK( pAudioThread, alSourcef( pSource->GetSource(),
				AL_REFERENCE_DISTANCE, pAttenuationRefDist ) );
			OAL_CHECK( pAudioThread, alSourcef( pSource->GetSource(),
				AL_ROLLOFF_FACTOR, pAttenuationRolloff ) );
		}
		
		if( pDirtyGain ){
			pDirtyGain = false;
			
			pAttenuatedGain = pMuted ? 0.0f : pVolume;
			OAL_CHECK( pAudioThread, alSourcef( pSource->GetSource(), AL_GAIN, pAttenuatedGain ) );
		}
		
		if( pDirtyLooping ){
			pDirtyLooping = false;
			
			OAL_CHECK( pAudioThread, alSourcei( pSource->GetSource(), AL_LOOPING,
				pLooping && ! pStreaming ? AL_TRUE : AL_FALSE ) );
		}
		
		if( pDirtyPlaySpeed ){
			pDirtyPlaySpeed = false;
			
			OAL_CHECK( pAudioThread, alSourcef( pSource->GetSource(), AL_PITCH, pPlaySpeed ) );
		}
	}
	
	// update sources
	if( pVideoPlayer ){
		pVideoPlayerNext( underrun );
		
	}else if( pSynthesizer ){
		pSynthNext( underrun );
		
	}else if( pSoundDecoder ){
		if( pNeedsInitialDecode ){
			pNeedsInitialDecode = false;
			pDecodeInitial();
			pSource->Play(); // required since previous play with empty buffers stops immediatly
			
		}else{
			pDecodeNext( underrun );
		}
	}
}

void deoalASpeaker::ProcessDeactivate(){
	pDoPlayState();
}

void deoalASpeaker::UpdateEffects(){
	if( ! pSource ){
		return;
	}
	
	// update environment
	if( pEnvironment && pAudioThread.GetExtensions().GetHasEFX() && pAudioThread.GetConfiguration().GetEnableEFX()){
		pEnvironment->Update();
	}
	
	// update gain by attenutation. extends openal attenuation system not supporting this.
	// takes into account environment calculated parameters
	pUpdateAttenuatedGain();
	
	// update environment effect
	pUpdateEnvironmentEffect();
}

void deoalASpeaker::SynchronizeStreaming(){
	// WARNING Called during synchronization time from main thread.
	
	pCheckStillSourceOwner();
}

void deoalASpeaker::ResetStreaming(){
	pRestart = true;
	pDirtyPlayState = true;
	pDirtyPlayPosition = true;
}

void deoalASpeaker::PrepareQuickDispose(){
	pParentMicrophone = NULL;
	pParentWorld = NULL;
	pOctreeNode = NULL;
}


void deoalASpeaker::SetEnabled( bool enabled ){
	// WARNING Called during synchronization time from main thread.
	
	if( enabled == pEnabled ){
		return;
	}
	
	pEnabled = enabled;
	
	pDirtyPlayState = true;
	
	UpdateOctreeNode();
}

void deoalASpeaker::SetPositionless( bool positionless ){
	// WARNING Called during synchronization time from main thread.
	
	if( positionless == pPositionless ){
		return;
	}
	
	pPositionless = positionless;
	
	if( pEnabled ){
		pDirtyGeometry = true;
	}
	
	UpdateOctreeNode();
	pEnsureEnvironment();
}

void deoalASpeaker::SetFlag( bool flag ){
	pFlag = flag;
}

void deoalASpeaker::SetParentWorld( deoalAWorld *world ){
	// WARNING Called during synchronization time from main thread.
	
	if( pParentMicrophone ){
		DETHROW( deeInvalidParam );
	}
	
	if( world == pParentWorld ){
		return;
	}
	
	if( pOctreeNode ){
		pOctreeNode->RemoveSpeaker( this );
	}
	pRemoveFromSoundLevelMeters();
	
	pParentWorld = world;
	
	pEnsureEnvironment();
	if( pEnvironment ){
		pEnvironment->SetWorld( world );
	}
}

void deoalASpeaker::SetParentMicrophone( deoalAMicrophone *microphone ){
	// WARNING Called during synchronization time from main thread.
	
	if( pParentWorld ){
		DETHROW( deeInvalidParam );
	}
	
	pParentMicrophone = microphone;
}

void deoalASpeaker::SetOctreeNode( deoalWorldOctree *node ){
	pOctreeNode = node;
}

void deoalASpeaker::UpdateOctreeNode(){
	// NOTE
	// - enabled: set by microphone for all speakers that can affect a microphone.
	//            for SLM this is incorrect since it operates on speakers independently
	// - source: available only if speaker is enabled in a microphone and not muted for
	//           performance or other reasons. also something we can not use for SLM
	
// 	pAudioThread.GetLogger().LogInfoFormat( "UpdateOctreeNode: %p %p %d %p %d", this, pParentWorld, pEnabled, pSource, GetPlaying() );
// 	if( pParentWorld && pEnabled && pSource && ! pPositionless && ! pMuted && GetPlaying() ){
	if( pParentWorld && ! pPositionless && ! pMuted && GetPlaying() ){
		pParentWorld->GetOctree()->InsertSpeakerIntoTree( this, 8 );
		
	}else if( pOctreeNode ){
		pOctreeNode->RemoveSpeaker( this );
	}
}



void deoalASpeaker::SetPlaySpeed( float speed ){
	pPlaySpeed = speed;
	
	pDirtyPlaySpeed = true;
}

void deoalASpeaker::SetRange( float range ){
	pRange = range;
	pRangeSquared = range * range;
	
	if( pEnvironment ){
		pEnvironment->SetRange( range );
	}
	
	const int count = pSoundLevelMeters.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		deoalASoundLevelMeterSpeaker * const speaker =
			( ( deoalASoundLevelMeter* )pSoundLevelMeters.GetAt( i ) )->GetSpeakerWith( this );
		if( speaker ){
			speaker->SpeakerRangeChanged();
		}
	}
}

void deoalASpeaker::SetVolume( float volume ){
	pVolume = volume;
	
	pDirtyGain = true;
}

void deoalASpeaker::SetAttenuationRolloff( float rolloff ){
	pAttenuationRolloff = rolloff;
	
	pUpdateAttenuation();
}

float deoalASpeaker::AttenuatedGain( float distance ) const{
	// OpenAL Inverse Distance Clamped Attenuation Model:
	// distance = max( distance, AL_REFERENCE_DISTANCE );
	// distance = min( distance, AL_MAX_DISTANCE );
	// gain = AL_REFERENCE_DISTANCE / (AL_REFERENCE_DISTANCE
	//        + AL_ROLLOFF_FACTOR * ( distance - AL_REFERENCE_DISTANCE ) );
	return pAttenuationRefDist / ( pAttenuationRefDist + pAttenuationRolloff
		* ( decMath::max( distance, pAttenuationRefDist ) - pAttenuationRefDist ) );
}

bool deoalASpeaker::AffectsActiveMicrophone() const{
	const deoalAMicrophone * const microphone = pAudioThread.GetActiveMicrophone();
	if( ! microphone ){
		return false;
	}
	if( microphone == pParentMicrophone ){
		return true;
	}
	return pParentWorld && microphone->GetParentWorld() == pParentWorld
		&& ( pPosition - microphone->GetPosition() ).LengthSquared() <= pRangeSquared
		&& pLayerMask.Matches( microphone->GetLayerMask() );
}



// Render world usage
///////////////////////

void deoalASpeaker::SetMicrophoneMarkedRemove( bool marked ){
	pMicrophoneMarkedRemove = marked;
}

void deoalASpeaker::SetWorldMarkedRemove( bool marked ){
	pWorldMarkedRemove = marked;
}

void deoalASpeaker::SetLLWorldPrev( deoalASpeaker *speaker ){
	pLLWorldPrev = speaker;
}

void deoalASpeaker::SetLLWorldNext( deoalASpeaker *speaker ){
	pLLWorldNext = speaker;
}



// Private Functions
//////////////////////

class deoalASpeakerDeletion : public deoalDelayedDeletion{
public:
	deoalSource *source;
	
	deoalASpeakerDeletion() :
	source( 0 ){
	}
	
	virtual ~deoalASpeakerDeletion(){
	}
	
	virtual void DeleteObjects( deoalAudioThread &audioThread ){
		if( source && source->GetOwner() == this ){
			audioThread.GetSourceManager().UnbindSource( source );
		}
	}
};

void deoalASpeaker::pCleanUp(){
	pSoundDecoder = NULL;
	if( pBufferData ){
		delete [] pBufferData;
	}
	if( pEnvironment ){
		delete pEnvironment;
	}
	if( pSound ){
		pSound->FreeReference();
	}
	if( pSynthesizer ){
		pSynthesizer->FreeReference();
	}
	if( pVideoPlayer ){
		pVideoPlayer->FreeReference();
	}
	
	// delayed deletion
	pCheckStillSourceOwner();
	if( pSource ){
		deoalASpeakerDeletion *delayedDeletion = NULL;
		
		try{
			delayedDeletion = new deoalASpeakerDeletion;
			delayedDeletion->source = pSource;
			pAudioThread.GetDelayed().AddDeletion( delayedDeletion );
			
		}catch( const deException &e ){
			if( delayedDeletion ){
				delete delayedDeletion;
			}
			pAudioThread.GetLogger().LogException( e );
			throw;
		}
		
		pSource->SetOwner( delayedDeletion );
	}
}



void deoalASpeaker::pDecodeInitial(){
	if( ! pSoundDecoder || ! pSound ){
		DETHROW( deeInvalidParam );
	}
	
	deoalDecodeBuffer &buffer = pAudioThread.GetDecodeBuffer();
	const ALenum format = pSound->GetFormat();
	const int sampleRate = pSound->GetSampleRate();
	int i;
	
	pQueueSampleOffset = pPlayPosition = pPlayFrom;
	
	pSoundDecoder->SetPosition( pQueueSampleOffset );
	
	for( i=0; i<pSource->GetBufferCount(); i++ ){
		if( pLooping ){
			buffer.DecodeLooping( pSoundDecoder, pBufferSize );
			
		}else{
			if( buffer.Decode( pSoundDecoder, pBufferSize ) == 0 ){
				break;
			}
		}
		
		const ALuint albuffer = pSource->GetBufferAt( i );
		OAL_CHECK( pAudioThread, alBufferData( albuffer, format,
			( const ALvoid * )buffer.GetBuffer(), pBufferSize, sampleRate ) );
		OAL_CHECK( pAudioThread, alSourceQueueBuffers( pSource->GetSource(), 1, &albuffer ) );
	}
}

void deoalASpeaker::pDecodeNext( bool underrun ){
	if( ! pSoundDecoder || ! pSound ){
		DETHROW( deeInvalidParam );
	}
	
	ALint queueOffset;
	OAL_CHECK( pAudioThread, alGetSourcei( pSource->GetSource(), AL_SAMPLE_OFFSET, &queueOffset ) );
	if( pLooping ){
		pPlayPosition = decMath::normalize( pQueueSampleOffset + queueOffset, pPlayFrom, pPlayTo );
		
	}else{
		pPlayPosition = decMath::clamp( pQueueSampleOffset + queueOffset, pPlayFrom, pPlayTo );
	}
	
	ALint numFinished;
	OAL_CHECK( pAudioThread, alGetSourcei( pSource->GetSource(), AL_BUFFERS_PROCESSED, &numFinished ) );
	if( numFinished == 0 && ! underrun ){
		return;
	}
	
	deoalDecodeBuffer &buffer = pAudioThread.GetDecodeBuffer();
	bool restartPlaying = underrun;
	ALuint buffers[ 10 ];
	int i;
	
	OAL_CHECK( pAudioThread, alSourceUnqueueBuffers( pSource->GetSource(), numFinished, &buffers[ 0 ] ) );
	
	pQueueSampleOffset += pBufferSampleCount * numFinished;
	
	if( numFinished == pSource->GetBufferCount() && pLooping ){
		restartPlaying = true;
	}
	
	for( i=0; i<numFinished; i++ ){
		if( pLooping ){
			buffer.DecodeLooping( pSoundDecoder, pBufferSize );
			
		}else{
			if( buffer.Decode( pSoundDecoder, pBufferSize ) == 0 ){
				numFinished = i;
				break;
			}
		}
		
		OAL_CHECK( pAudioThread, alBufferData( buffers[ i ], pSound->GetFormat(),
			( const ALvoid * )buffer.GetBuffer(), pBufferSize, pSound->GetSampleRate() ) );
	}
	
	if( numFinished > 0 ){
		OAL_CHECK( pAudioThread, alSourceQueueBuffers( pSource->GetSource(), numFinished, &buffers[ 0 ] ) );
		
	}else{
		return;
	}
	
	// if we ran out of buffers we have to restart plpaying or playback stops
	if( restartPlaying ){
		pPlayFinished = false;
		pSource->Play();
	}
}

void deoalASpeaker::pSynthInit(){
	if( ! pSynthesizer ){
		DETHROW( deeInvalidAction );
	}
	
	ALuint albuffer;
	int i;
	
	pQueueSampleOffset = pPlayPosition = pPlayFrom;
	
	int bufferCount = pSource->GetBufferCount();
	int samplesFrom = pQueueSampleOffset;
	
	pSynthesizer->Reset();
	
	if( pBufferSize > pBufferDataCapacity ){
		if( pBufferData ){
			delete [] pBufferData;
			pBufferData = NULL;
		}
		
		pBufferData = new char[ pBufferSize ];
		pBufferDataCapacity = pBufferSize;
	}
	
	for( i=0; i<bufferCount; i++ ){
		const int remainingSampleCount = pPlayTo - samplesFrom;
		int bufferSize = pBufferSize;
		
		if( pBufferSampleCount > remainingSampleCount ){
			pSynthesizer->GenerateSound( pBufferData, pBufferSampleSize * remainingSampleCount,
				samplesFrom, remainingSampleCount );
			
			if( pLooping ){
				samplesFrom = pBufferSampleCount - remainingSampleCount;
				pSynthesizer->GenerateSound( pBufferData + pBufferSampleSize * remainingSampleCount,
					pBufferSampleSize * samplesFrom, 0, samplesFrom );
				
			}else{
				bufferSize = pBufferSampleSize * remainingSampleCount;
				bufferCount = i + 1;
			}
			
		}else{
			pSynthesizer->GenerateSound( pBufferData, pBufferSampleSize * pBufferSampleCount,
				samplesFrom, pBufferSampleCount );
			samplesFrom += pBufferSampleCount;
		}
		
		albuffer = pSource->GetBufferAt( i );
		OAL_CHECK( pAudioThread, alBufferData( albuffer, pBufferFormat, ( const ALvoid * )pBufferData, bufferSize, pSampleRate ) );
		//pAudioThread.LoIgnfoFormat( "pSynthInit: queue buffer (source=%u buffer=%u format=%x sampleRate=%i size=%i)",
		//	pSource->GetSource(), albuffer, pBufferFormat, pSampleRate, bufferSize );
		OAL_CHECK( pAudioThread, alSourceQueueBuffers( pSource->GetSource(), 1, &albuffer ) );
	}
}

//#include <dragengine/logger/deLoggerConsole.h>
void deoalASpeaker::pSynthNext( bool underrun ){
	ALint queueOffset;
	OAL_CHECK( pAudioThread, alGetSourcei( pSource->GetSource(), AL_SAMPLE_OFFSET, &queueOffset ) );
	if( pLooping ){
		pPlayPosition = decMath::normalize( pQueueSampleOffset + queueOffset, pPlayFrom, pPlayTo );
		
	}else{
		pPlayPosition = decMath::clamp( pQueueSampleOffset + queueOffset, pPlayFrom, pPlayTo );
	}
// 	pAudioThread.GetLogger().LogInfoFormat( "SynthNext: pos=%i(%.3g)", pPlayPosition, (float)pPlayPosition / (float)pSampleRate );
	
	ALint numFinished = 0;
	OAL_CHECK( pAudioThread, alGetSourcei( pSource->GetSource(), AL_BUFFERS_PROCESSED, &numFinished ) );
//	deLoggerConsole().GetLogger().LogInfoFormat("Dragengine", "speaker %p: num finished %i", this, numFinished);
	if( numFinished == 0 && ! underrun ){
		return;
	}
	
	bool restartPlaying = underrun;
	ALuint buffers[ 10 ];
	int i;
	
	if( numFinished == pSource->GetBufferCount() && pLooping ){
		restartPlaying = true;
	}
	
	OAL_CHECK( pAudioThread, alSourceUnqueueBuffers( pSource->GetSource(), numFinished, &buffers[ 0 ] ) );
	
	pQueueSampleOffset += pBufferSampleCount * numFinished;
	
	int samplesFrom;
	
	if( pLooping ){
		samplesFrom = decMath::normalize( pQueueSampleOffset
			+ ( pSource->GetBufferCount() - numFinished ) * pBufferSampleCount, pPlayFrom, pPlayTo );
		
	}else{
		samplesFrom = decMath::min( pQueueSampleOffset
			+ ( pSource->GetBufferCount() - numFinished ) * pBufferSampleCount, pPlayTo );
	}
	
	for( i=0; i<numFinished; i++ ){
		const int remainingSampleCount = pPlayTo - samplesFrom;
		if( remainingSampleCount == 0 && ! pLooping ){
			numFinished = i;
			break;
		}
		
		int bufferSize = pBufferSize;
		
		if( pBufferSampleCount > remainingSampleCount ){
			pSynthesizer->GenerateSound( pBufferData, pBufferSampleSize * remainingSampleCount,
				samplesFrom, remainingSampleCount );
			
			if( pLooping ){
				samplesFrom = pBufferSampleCount - remainingSampleCount;
				pSynthesizer->GenerateSound( pBufferData + pBufferSampleSize * remainingSampleCount,
					pBufferSampleSize * samplesFrom, 0, samplesFrom );
				
			}else{
				bufferSize = pBufferSampleSize * remainingSampleCount;
				samplesFrom = pPlayTo;
			}
			
		}else{
			pSynthesizer->GenerateSound( pBufferData, pBufferSampleSize * pBufferSampleCount,
				samplesFrom, pBufferSampleCount );
			samplesFrom += pBufferSampleCount;
		}
		
		//pAudioThread.GetLogger().LogInfoFormat( "SynthNext: queue buffer (source=%u buffer=%u format=%x sampleRate=%i)",
		//	pSource->GetSource(), buffers[ i ], pBufferFormat, pSampleRate );
		OAL_CHECK( pAudioThread, alBufferData( buffers[ i ], pBufferFormat,
			( const ALvoid * )pBufferData, bufferSize, pSampleRate ) );
	}
	
	//pAudioThread.GetLogger().LogInfoFormat( "SynthNext: queue %i buffers (format=%x sampleRate=%i)", numFinished, pBufferFormat, pSampleRate );
	if( numFinished > 0 ){
//	deLoggerConsole().GetLogger().LogInfoFormat("Dragengine", "speaker %p: queued %i", this, numFinished);
		OAL_CHECK( pAudioThread, alSourceQueueBuffers( pSource->GetSource(), numFinished, buffers ) );
		
	}else{
		return;
	}
	
	// if we ran out of buffers we have to restart plpaying or playback stops
	if( restartPlaying ){
		pPlayFinished = false;
		pSource->Play();
	}
}

void deoalASpeaker::pVideoPlayerInit(){
	if( ! pVideoPlayer ){
		DETHROW( deeInvalidAction );
	}
	
	ALuint albuffer;
	int i;
	
	pQueueSampleOffset = pPlayPosition =
		( int )( pVideoPlayer->GetPlayPosition() * pVideoPlayer->GetSampleRate() );
	
	int bufferCount = pSource->GetBufferCount();
	int samplesFrom = pQueueSampleOffset;
	
	if( pBufferSize > pBufferDataCapacity ){
		if( pBufferData ){
			delete [] pBufferData;
			pBufferData = NULL;
		}
		
		pBufferData = new char[ pBufferSize ];
		pBufferDataCapacity = pBufferSize;
	}
	
	for( i=0; i<bufferCount; i++ ){
		const int remainingSampleCount = pPlayTo - samplesFrom;
		if( remainingSampleCount < 1 ){
			break;
		}
		
		int bufferSize = pBufferSize;
		
		if( remainingSampleCount < pBufferSampleCount ){
			pVideoPlayer->ReadSamples( pBufferData, pBufferSampleSize * remainingSampleCount,
				samplesFrom, remainingSampleCount );
			
			if( pLooping ){
				samplesFrom = pBufferSampleCount - remainingSampleCount;
				pVideoPlayer->ReadSamples( pBufferData + pBufferSampleSize * remainingSampleCount,
					pBufferSampleSize * samplesFrom, 0, samplesFrom );
				
			}else{
				bufferSize = pBufferSampleSize * remainingSampleCount;
				bufferCount = i + 1;
			}
			
		}else{
			pVideoPlayer->ReadSamples( pBufferData, pBufferSampleSize * pBufferSampleCount,
				samplesFrom, pBufferSampleCount );
			samplesFrom += pBufferSampleCount;
		}
		
		albuffer = pSource->GetBufferAt( i );
		OAL_CHECK( pAudioThread, alBufferData( albuffer, pBufferFormat, ( const ALvoid * )pBufferData, bufferSize, pSampleRate ) );
		//pAudioThread.LoIgnfoFormat( "pSynthInit: queue buffer (source=%u buffer=%u format=%x sampleRate=%i size=%i)",
		//	pSource->GetSource(), albuffer, pBufferFormat, pSampleRate, bufferSize );
		OAL_CHECK( pAudioThread, alSourceQueueBuffers( pSource->GetSource(), 1, &albuffer ) );
	}
}

void deoalASpeaker::pVideoPlayerNext( bool underrun ){
	ALint queueOffset;
	OAL_CHECK( pAudioThread, alGetSourcei( pSource->GetSource(), AL_SAMPLE_OFFSET, &queueOffset ) );
	if( pLooping ){
		pPlayPosition = decMath::normalize( pQueueSampleOffset + queueOffset, pPlayFrom, pPlayTo );
		
	}else{
		pPlayPosition = decMath::clamp( pQueueSampleOffset + queueOffset, pPlayFrom, pPlayTo );
	}
	//pAudioThread.GetLogger().LogInfoFormat( "SynthNext: pos=%i(%.3g)", pPlayPosition, (float)pPlayPosition / (float)pSampleRate );
	
	ALint numFinished = 0;
	OAL_CHECK( pAudioThread, alGetSourcei( pSource->GetSource(), AL_BUFFERS_PROCESSED, &numFinished ) );
	//pAudioThread.GetLogger().LogInfoFormat("speaker %p: num finished %i", this, numFinished);
	if( numFinished == 0 && ! underrun ){
		return;
	}
	
	bool restartPlaying = underrun;
	ALuint buffers[ 10 ];
	int i;
	
	if( numFinished == pSource->GetBufferCount() ){
		restartPlaying = true;
	}
	
	OAL_CHECK( pAudioThread, alSourceUnqueueBuffers( pSource->GetSource(), numFinished, &buffers[ 0 ] ) );
	
	pQueueSampleOffset += pBufferSampleCount * numFinished;
	
	int samplesFrom;
	
	if( pLooping ){
		samplesFrom = decMath::normalize( pQueueSampleOffset
			+ ( pSource->GetBufferCount() - numFinished ) * pBufferSampleCount, pPlayFrom, pPlayTo );
		
	}else{
		samplesFrom = decMath::min( pQueueSampleOffset
			+ ( pSource->GetBufferCount() - numFinished ) * pBufferSampleCount, pPlayTo );
	}
	
	for( i=0; i<numFinished; i++ ){
		const int remainingSampleCount = pPlayTo - samplesFrom;
		if( remainingSampleCount == 0 && ! pLooping ){
			numFinished = i;
			break;
		}
		
		int bufferSize = pBufferSize;
		
		if( pBufferSampleCount > remainingSampleCount ){
			pVideoPlayer->ReadSamples( pBufferData, pBufferSampleSize * remainingSampleCount,
				samplesFrom, remainingSampleCount );
			
			if( pLooping ){
				samplesFrom = pBufferSampleCount - remainingSampleCount;
				pVideoPlayer->ReadSamples( pBufferData + pBufferSampleSize * remainingSampleCount,
					pBufferSampleSize * samplesFrom, 0, samplesFrom );
				
			}else{
				bufferSize = pBufferSampleSize * remainingSampleCount;
				samplesFrom = pPlayTo;
			}
			
		}else{
			pVideoPlayer->ReadSamples( pBufferData, pBufferSampleSize * pBufferSampleCount,
				samplesFrom, pBufferSampleCount );
			samplesFrom += pBufferSampleCount;
		}
		
// 		pAudioThread.GetLogger().LogInfoFormat( "pVideoPlayerNext: queue buffer (source=%u buffer=%u format=%x sampleRate=%i)",
// 			pSource->GetSource(), buffers[ i ], pBufferFormat, pSampleRate );
		OAL_CHECK( pAudioThread, alBufferData( buffers[ i ], pBufferFormat,
			( const ALvoid * )pBufferData, bufferSize, pSampleRate ) );
	}
	
	//pAudioThread.GetLogger().LogInfoFormat( "SynthNext: queue %i buffers (format=%x sampleRate=%i)", numFinished, pBufferFormat, pSampleRate );
	if( numFinished > 0 ){
//	deLoggerConsole().GetLogger().LogInfoFormat("Dragengine", "speaker %p: queued %i", this, numFinished);
		OAL_CHECK( pAudioThread, alSourceQueueBuffers( pSource->GetSource(), numFinished, buffers ) );
		
	}else{
		return;
	}
	
	// if we ran out of buffers we have to restart plpaying or playback stops
	if( restartPlaying ){
		pPlayFinished = false;
		pSource->Play();
	}
}

void deoalASpeaker::pUpdatePlayRange(){
	// get sample rate and play time in samples from the source in use
	int sampleCount = 0;
	if( pSound ){
		pSampleRate = pSound->GetSampleRate();
		sampleCount = pSound->GetSampleCount();
		
	}else if( pSynthesizer ){
		pSampleRate = pSynthesizer->GetSampleRate();
		sampleCount = pSynthesizer->GetSampleCount();
		
	}else if( pVideoPlayer ){
		pSampleRate = pVideoPlayer->GetSampleRate();
		sampleCount = pVideoPlayer->GetSampleCount();
		
	}else{
		pSampleRate = 44100;
	}
	
	// calculate from and to play position in samples clamped to valid values
	pPlayFrom = decMath::clamp( pSpeakerPlayFrom, 0, sampleCount );
	pPlayTo = decMath::clamp( pSpeakerPlayTo, pPlayFrom, sampleCount );
	pPlayRange = decMath::max( pPlayTo - pPlayFrom, 0 );
// 	pAudioThread.GetLogger().LogInfoFormat( "UpdatePlayPosition sampleRate=%i from=%i to=%i | %d %d %p",
// 		pSampleRate, pPlayFrom, pPlayTo, pSpeakerPlayFrom, pSpeakerPlayTo, (deSynthesizerInstance*)pSynthesizer );
}

void deoalASpeaker::pUpdateAttenuation(){
	// formula:
	//   gain = refDist / ( refDist + rollOff * ( clamp(dist,refDist,maxDist) - refDist ) )
	// 
	// for dist = refDist:
	//   gain = 1
	// 
	// for dist = maxDist;
	//   gain = refDist / maxDist
	// 
	// problem: never gets to zero. only solution will be to calculate sound attenutation on our
	// own updated every frame for every object in audible distance.
	// 
	// note about roll-off factor. rolloff=1 is the default and represents the physically
	// correct solution. rolloff=0 indicates no attenuation at all. larger values indicate
	// stronger attenuation while smaller values indicate weaker sttenuation than physically
	// correct.
	// 
	// note about reference distance. default value is 1 which means below 1m no attenuation.
	// physically the gain is 1/d which means below 1m the gain becomes larger than 1 and
	// requires clamping. ranges below 2m do not make sense in this regard but we need to
	// support it. using refDist=1 is similar to opengl where the attenuation is 1/(1+a*d*d).
	// 
	// now what goes for the half-intensity-distance this is not the point where the gain becomes
	// 0.5 since this would result in a value like 0.01 which is difficult to handle. to get a
	// meaning similar to lighting a different meaning is used so the value of 0.1 becomes the
	// realistic value as with lighting. the half-intensity-distance is defined as the RT30
	// point. at this point the sound dropped by 30dB which is half the RT60 which is commonly
	// considered the sound range (sound becomes inaudible). thus half means here half the dB.
	// in intensity values RT30 equals a gain of 0.03125 .
	// 
	// calculation:
	//   gain = refDist / ( refDist + rollOff * ( range * halfIntDist - refDist ) )
	//   RT30 = refDist / ( refDist + rollOff * ( range * halfIntDist - refDist ) )
	//   RT30 * ( refDist + rollOff * ( range * halfIntDist - refDist ) ) = refDist
	//   RT30 * refDist + RT30 * rollOff * range * halfIntDist - RT30 * rollOff * refDist = refDist
	//   RT30 * rollOff * range * halfIntDist - RT30 * rollOff * refDist = refDist - RT30 * refDist
	//   rollOff * ( RT30 * range * halfIntDist - RT30 * refDist ) = refDist - RT30 * refDist
	//   rollOff = ( refDist - RT30 * refDist ) / ( RT30 * range * halfIntDist - RT30 * refDist )
	//   rollOff = ( refDist / RT30 - refDist ) / ( range * halfIntDist - refDist )
	//   rollOff = ( 1 / RT30 - 1 ) / ( range * halfIntDist / refDist - 1 )
	//   
	//   
	//   
	//   0.5 = refDist / ( refDist + rollOff * ( (refDist+(range-refDist)*halfIntDist - refDist ) )
	//   0.5 = refDist / ( refDist + rollOff * ( ( range - refDist ) * halfIntDist ) )
	//   ( refDist + rollOff * ( ( range - refDist ) * halfIntDist ) ) = refDist * 2
	//   rollOff * ( ( range - refDist ) * halfIntDist ) = refDist * 2 - refDist
	//   rollOff * ( ( range - refDist ) * halfIntDist ) = refDist
	//   rollOff = refDist / ( ( range - refDist ) * halfIntDist )
	// 
	// 1% => 1m on 100m or 0.1m on 10m
	// 0.1% => 0.1m on 100m or 0.01m on 10m
// 	const float fullVolumeArea = 0.01; //0.001f;
	
	/*
	const float halfVolumeDistance = pSpeaker.GetHalfVolumeDistance();
	const float range = decMath::max( pSpeaker.GetRange(), 0.01f ); // too small makes no sense only problems
	
	pAttenuationRefDist = 1.0f; //decMath::max( range * fullVolumeArea, 0.001f ); // don't go lower to avoid accuracy problems
	//pAttenuationRolloff = decMath::max( pAttenuationRefDist / ( range * halfVolumeDistance - pAttenuationRefDist ), 0.0f );
	pAttenuationRolloff = pAttenuationRefDist / ( decMath::max( range - pAttenuationRefDist, 0.001f ) * halfVolumeDistance );
	*/
	
	pAttenuationRefDist = 1.0f;
	
	if( pEnvironment ){
		pEnvironment->SetAttenuation( pAttenuationRefDist, pAttenuationRolloff );
		pEnvironment->SetLayerMask( pLayerMask );
	}
	
	const int count = pSoundLevelMeters.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		deoalASoundLevelMeterSpeaker * const speaker =
			( ( deoalASoundLevelMeter* )pSoundLevelMeters.GetAt( i ) )->GetSpeakerWith( this );
		if( speaker ){
			speaker->SpeakerAttenuationChanged();
			speaker->SpeakerLayerMaskChanged();
		}
	}
	
	pDirtyAttenuation = true;
}



void deoalASpeaker::pDoPlayState(){
	if( pRestart ){
		pStopPlaySource();
		pRestart = false;
	}
	
	if( pEnabled && ( pSound || pSynthesizer || pVideoPlayer ) && pPlayRange > 0 ){
		switch( pPlayState ){
		case deSpeaker::epsPlaying:
			pStartPlaySource();
			return;
			
		case deSpeaker::epsPaused:
			pStartPlaySource();
			pSource->Pause();
			return;
			
		case deSpeaker::epsStopped:
		default:
			break;
		}
	}
	
	pStopPlaySource();
}

void deoalASpeaker::pStartPlaySource(){
	pCheckStillSourceOwner();
	
	if( ! pSource ){
		pSource = pAudioThread.GetSourceManager().BindSource( this );
		pUpdateSourceImportance();
		
		// parameters not defined by the source
		OAL_CHECK( pAudioThread, alSourcef( pSource->GetSource(), AL_PITCH, 1.0f ) );
		const ALfloat parameters[ 3 ] = { 0.0f, 0.0f, 0.0f };
		OAL_CHECK( pAudioThread, alSourcefv( pSource->GetSource(), AL_DIRECTION, &parameters[ 0 ] ) );
		
		// init play range. required by pInitSoundSource to be done first
		pUpdatePlayRange();
		
		// set up sound
		pInitSoundSource();
		
		// initial parameters
		pDirtyGeometry = true;
		pDirtyGain = true;
		pDirtyLooping = true;
		pDirtyPlaySpeed = true;
		pDirtyPlayPosition = true;
	}
	
	pPlayFinished = false;
	pSource->Play();
}

void deoalASpeaker::pStopPlaySource(){
	pCheckStillSourceOwner();
	if( ! pSource ){
		return;
	}
	
	pAudioThread.GetSourceManager().UnbindSource( pSource );
	pSource = NULL;
	pQueueSampleOffset = 0;
}



void deoalASpeaker::pCheckStillSourceOwner(){
	if( ! pSource || pSource->GetOwner() == this ){
		return;
	}
	
	pSource = NULL;
}

void deoalASpeaker::pUpdateSourceImportance(){
	if( pSound && pSound->GetStreaming() ){
		pSource->SetImportance( 1.0f );
		
	}else if( pSynthesizer ){
		pSource->SetImportance( 1.0f );
		
	}else if( pVideoPlayer ){
		pSource->SetImportance( 1.0f );
		
	}else{
		pSource->SetImportance( 0.0f );
	}
}

void deoalASpeaker::pUpdateAttenuatedGain(){
	if( pMuted ){
		pAttenuatedGain = 0.0f;
		OAL_CHECK( pAudioThread, alSourcef( pSource->GetSource(), AL_GAIN, 0.0f ) );
		return;
	}
	
	// what we do here is fixing the problem with openal attenuation. the openal supports
	// only an inverse-linear model without pulling to 0. this results in all real-world
	// scenarios to end up with around 9% volume at the range instead of 0%. this in turn
	// results in sounds to be audible way beyond the range making it impossible to
	// create a localized sound sources. a hack proposed on the internet is to alter the
	// gain manually to do the kind of pull-to-zero by multiplying in a linear drop.
	// this leaves sounds near the source at the openal calculated volume while fading it
	// out properly to 0 at the range
	const decDVector &microphonePos = pAudioThread.GetActiveMicrophone()->GetPosition();
	const float distance = ( float )( microphonePos - pPosition ).Length();
	
	float gainMultiplier = decMath::linearStep( distance, pAttenuationRefDist, pRange, 1.0f, 0.0f );
	
	if( pEnvironment && ( ! pAudioThread.GetExtensions().GetHasEFX()
	|| ! pAudioThread.GetConfiguration().GetEnableEFX() ) ){
		gainMultiplier *= pEnvironment->GetGainMedium();
	}
	
	pAttenuatedGain = pVolume * gainMultiplier;
	OAL_CHECK( pAudioThread, alSourcef( pSource->GetSource(), AL_GAIN, pAttenuatedGain ) );
	
	// NOTE: maybe it would be better to do the entire gain calculation here instead of
	// modifying the openal one
}



void deoalASpeaker::pInitSoundSource(){
	if( pSound ){
		pSound->PrepareBuffers();
		
		if( pSound->GetStreaming() ){
			pSource->SetBufferCount( 5 );
			
			pBufferSampleCount = pSound->GetSampleRate() / 5; // 200ms slices
			pBufferSampleSize = pSound->GetBytesPerSample() * pSound->GetChannelCount();
			pBufferSize = pBufferSampleCount * pBufferSampleSize;
			pBufferFormat = pSound->GetFormat();
			
			pStreaming = true;
			pNeedsInitialDecode = true;
			
		}else{
			pSource->SetBufferCount( 0 );
			
			OAL_CHECK( pAudioThread, alSourcei( pSource->GetSource(), AL_BUFFER, pSound->GetBuffer() ) );
			pStreaming = false;
			pNeedsInitialDecode = false;
		}
		
	}else if( pSynthesizer ){
		if( pSynthesizer->GetUpdateTracker() != pSourceUpdateTracker ){
			pSourceUpdateTracker = pSynthesizer->GetUpdateTracker();
			
			pDirtyPlayPosition = true;
			pUpdatePlayRange();
		}
		
		pSampleRate = pSynthesizer->GetSampleRate();
		
		// clamp to supported values
		const int channelCount = decMath::clamp( pSynthesizer->GetChannelCount(), 1, 2 );
		const int bytesPerSample = decMath::clamp( pSynthesizer->GetBytesPerSample(), 1, 2 );
		
		// determine format to use
		if( bytesPerSample == 1 ){
			if( channelCount == 1 ){
				pBufferFormat = AL_FORMAT_MONO8;
				
			}else{ // pChannelCount == 2
				pBufferFormat = AL_FORMAT_STEREO8;
			}
			
		}else{ // pBytesPerSample == 2
			if( channelCount == 1 ){
				pBufferFormat = AL_FORMAT_MONO16;
				
			}else{ // pChannelCount == 2
				pBufferFormat = AL_FORMAT_STEREO16;
			}
		}
		
		// determine streaming parameters
		pSource->SetBufferCount( 2 ); // 100 ms buffered
		//pSource->SetBufferCount( 3 ); // 150 ms buffered
		
		pBufferSampleCount = pSampleRate / 20; // 50 ms slices
		pBufferSampleSize = bytesPerSample * channelCount;
		pBufferSize = pBufferSampleCount * pBufferSampleSize;
		
		pSynthInit();
		pStreaming = true;
		
	}else if( pVideoPlayer ){
		if( pVideoPlayer->GetUpdateTracker() != pSourceUpdateTracker ){
			pSourceUpdateTracker = pVideoPlayer->GetUpdateTracker();
			
			pDirtyPlayPosition = true;
			pUpdatePlayRange();
		}
		
		pSampleRate = pVideoPlayer->GetSampleRate();
		
		// determine format to use
		if( pVideoPlayer->GetBytesPerSample() == 1 ){
			if( pVideoPlayer->GetChannelCount() == 1 ){
				pBufferFormat = AL_FORMAT_MONO8;
				
			}else{ // pChannelCount == 2
				pBufferFormat = AL_FORMAT_STEREO8;
			}
			
		}else{ // pBytesPerSample == 2
			if( pVideoPlayer->GetChannelCount() == 1 ){
				pBufferFormat = AL_FORMAT_MONO16;
				
			}else{ // pChannelCount == 2
				pBufferFormat = AL_FORMAT_STEREO16;
			}
		}
		
		// determine streaming parameters
		// 
		// for regular videos 1s buffering with 5 buffers is good. if the play speed can
		// change then this might have to be adjusted to something closer to the synthesizer
		// values above (100ms buffering with 2 buffers)
		pSource->SetBufferCount( 5 );
		pBufferSampleCount = pSampleRate / 5; // 200 ms slices
		pBufferSampleSize = pVideoPlayer->GetBytesPerSample() * pVideoPlayer->GetChannelCount();
		pBufferSize = pBufferSampleCount * pBufferSampleSize;
		
		pVideoPlayerInit();
		pStreaming = true;
	}
}

void deoalASpeaker::pEnsureEnvironment(){
	// WARNING Called during synchronization time from main thread.
	
	if( pParentWorld && ! pPositionless ){
		if( ! pEnvironment ){
			pEnvironment = new deoalEnvironment( pAudioThread );
			pEnvironment->SetWorld( pParentWorld );
			pEnvironment->SetPosition( pPosition );
			pEnvironment->SetRange( pRange );
			pEnvironment->SetAttenuation( pAttenuationRefDist, pAttenuationRolloff );
			pEnvironment->SetLayerMask( pLayerMask );
		}
		
	}else if( pEnvironment ){
		delete pEnvironment;
		pEnvironment = NULL;
	}
}

void deoalASpeaker::pUpdateEnvironmentEffect(){
	if( ! pEnvironment || ! pAudioThread.GetExtensions().GetHasEFX()
	|| ! pAudioThread.GetConfiguration().GetEnableEFX() ){
		return;
	}
	
	// NOTE openal has a little problem. if volume is at most 1 then filters and effects are
	//      properly reduced in master gain if the speaker volume goes down. if the volume
	//      though is above 1 then filters and effects are not affected anymore. they actually
	//      are using volume=1 albeit direct path gain raises. this has the effect that direct
	//      path gain gets louder and louder while filter and effects clamp at gain=1. this is
	//      not the acceptable behavior but also not simple to fix.
	//      
	//      a possible hack would be to check what is the maximum gain and then apply a
	//      correction gain if the attenuated gain goes beyond this maximum. needs a real
	//      solution since this would only work around the problem in a small range. maybe
	//      we really need to real-time process the volumes to avoid openal getting the
	//      better of us.
	//      
	//      for filters the maximum gain is hard-coded as 4 (FILTER_MAX_GAIN) but how to
	//      get this value using regular openal calls?
	//      
	//      for effects the maximum gain is AL_EAXREVERB_MAX_GAIN and has the default of 1
	//      
	//      so for effects it looks like we are out of luck? how to solve this mess?!
	
	// direct path muffling effect using band-pass filter. the name band-pass filter is quite
	// missleading. if GainHF is less than GainLF then the filter behaves like a low-pass filter
	// otherwise like a high-pass filter. a real band-pass filter looks different
	const ALuint filter = pSource->GetFilter();
	OAL_CHECK( pAudioThread, palFilteri( filter, AL_FILTER_TYPE, AL_FILTER_BANDPASS ) );
	OAL_CHECK( pAudioThread, palFilterf( filter, AL_BANDPASS_GAIN, pEnvironment->GetBandPassGain() ) );
	OAL_CHECK( pAudioThread, palFilterf( filter, AL_BANDPASS_GAINLF, pEnvironment->GetBandPassGainLF() ) );
	OAL_CHECK( pAudioThread, palFilterf( filter, AL_BANDPASS_GAINHF, pEnvironment->GetBandPassGainHF() ) );
	pSource->AssignFilter();
	
	// indirect path reverbe using effect
	const ALuint effect = pSource->GetSendEffect( 0 );
	OAL_CHECK( pAudioThread, palEffecti( effect, AL_EFFECT_TYPE, AL_EFFECT_EAXREVERB ) );
	OAL_CHECK( pAudioThread, palEffectf( effect, AL_EAXREVERB_GAIN, pEnvironment->GetReverbGain() ) );
	OAL_CHECK( pAudioThread, palEffectf( effect, AL_EAXREVERB_GAINHF, pEnvironment->GetReverbGainHF() ) );
	OAL_CHECK( pAudioThread, palEffectf( effect, AL_EAXREVERB_GAINLF, pEnvironment->GetReverbGainLF() ) );
	OAL_CHECK( pAudioThread, palEffectf( effect, AL_EAXREVERB_DECAY_TIME,
		pEnvironment->GetReverbDecayTime() ) );
	OAL_CHECK( pAudioThread, palEffectf( effect, AL_EAXREVERB_DECAY_HFRATIO,
		pEnvironment->GetReverbDecayHFRatio() ) );
	OAL_CHECK( pAudioThread, palEffectf( effect, AL_EAXREVERB_DECAY_LFRATIO,
		pEnvironment->GetReverbDecayLFRatio() ) );
	OAL_CHECK( pAudioThread, palEffectf( effect, AL_EAXREVERB_REFLECTIONS_GAIN,
		pEnvironment->GetReverbReflectionGain() ) );
	OAL_CHECK( pAudioThread, palEffectf( effect, AL_EAXREVERB_REFLECTIONS_DELAY,
		pEnvironment->GetReverbReflectionDelay() ) );
	OAL_CHECK( pAudioThread, palEffectf( effect, AL_EAXREVERB_LATE_REVERB_GAIN,
		pEnvironment->GetReverbLateReverbGain() ) );
	OAL_CHECK( pAudioThread, palEffectf( effect, AL_EAXREVERB_LATE_REVERB_DELAY,
		pEnvironment->GetReverbLateReverbDelay() ) );
	OAL_CHECK( pAudioThread, palEffectf( effect, AL_EAXREVERB_ECHO_TIME,
		pEnvironment->GetReverbEchoTime() ) );
	
	ALfloat alvector[ 3 ];
	alvector[ 0 ] = ( ALfloat )pEnvironment->GetReverbReflectionPan().x;
	alvector[ 1 ] = ( ALfloat )pEnvironment->GetReverbReflectionPan().y;
	alvector[ 2 ] = ( ALfloat )pEnvironment->GetReverbReflectionPan().z;
	OAL_CHECK( pAudioThread, palEffectfv( effect, AL_EAXREVERB_REFLECTIONS_PAN, &alvector[ 0 ] ) );
	
	alvector[ 0 ] = ( ALfloat )pEnvironment->GetReverbLateReverbPan().x;
	alvector[ 1 ] = ( ALfloat )pEnvironment->GetReverbLateReverbPan().y;
	alvector[ 2 ] = ( ALfloat )pEnvironment->GetReverbLateReverbPan().z;
	OAL_CHECK( pAudioThread, palEffectfv( effect, AL_EAXREVERB_LATE_REVERB_PAN, &alvector[ 0 ] ) );
	
	// this one here is difficult to understand and handle. the documentation is lacking in this
	// regard so look at an anwser found online:
	// https://openal.opensource.creative.narkive.com/N3Uwb9th/efx-clarifications-2
	// 
	// basically the reverbe effect uses a statistical model to alter the gain of the reverbe
	// added using distance to listener and room parameters. this is bad for us because we
	// calculate a set of parameters for a specific combination of listener and source position.
	// by doing gain manipulation on its own the reverbe plugin fumbles up our calculations.
	// it is though not know how much this can be disabled and how much we need the statistics
	// to be in place for the effect to work
	// 
	// see deoalSource::GetSendSlot()
	pSource->AssignSendEffect( 0 );
}



void deoalASpeaker::pRemoveFromSoundLevelMeters(){
	const int count = pSoundLevelMeters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoalASoundLevelMeter* )pSoundLevelMeters.GetAt( i ) )->RemoveSpeakerIfPresent( this );
	}
	pSoundLevelMeters.RemoveAll();
}

void deoalASpeaker::pDropEnvProbeOctreeNodeAllSLMs(){
	const int count = pSoundLevelMeters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoalASoundLevelMeter* )pSoundLevelMeters.GetAt( i ) )->SpeakerDropEnvProbeOctreeNode( this );
	}
}
