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

#include "deoalSource.h"
#include "../audiothread/deoalAudioThread.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>



// Class deoalSource
//////////////////////

// Constructor, destructor
////////////////////////////

deoalSource::deoalSource( deoalAudioThread &audioThread ) :
pAudioThread( audioThread ),

pState( epsStopped ),
pSource( 0 ),
pBuffers( NULL ),
pBufferCount( 0 ),

pOwner( NULL ),
pImportance( 1.0f ),

pFilter( 0 ),
pSendSlot( 0 ),
pSendEffect( 0 )
{
	try{
		// create sound source and set some default parameters. throws an exception
		// to let the sound manager figure out too many sources are used
		alGetError();
		alGenSources( 1, &pSource );
		if( alGetError() != AL_NO_ERROR ){
			DETHROW( deeOutOfMemory );
		}
		//OAL_CHECK( pAudioThread, alGenSources( 1, &pSource ) );
		
		OAL_CHECK( pAudioThread, alSourcef( pSource, AL_PITCH, 1.0f ) );
		const ALfloat parameters[ 3 ] = { 0.0f, 0.0f, 0.0f };
		OAL_CHECK( pAudioThread, alSourcefv( pSource, AL_DIRECTION, &parameters[ 0 ] ) );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoalSource::~deoalSource(){
	pCleanUp();
}



// Management
///////////////

void deoalSource::SetBufferCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count == pBufferCount ){
		return;
	}
	
	Stop(); // ensure no buffers are held in any way
	
	if( pBuffers ){
		OAL_CHECK( pAudioThread, alDeleteBuffers( pBufferCount, pBuffers ) );
		delete [] pBuffers;
		pBuffers = NULL;
		pBufferCount = 0;
	}
	
	if( count > 0 ){
		pBuffers = new ALuint[ count ];
		OAL_CHECK( pAudioThread, alGenBuffers( count, pBuffers ) );
		pBufferCount = count;
	}
}

ALuint deoalSource::GetBufferAt( int position ) const{
	if( position < 0 || position >= pBufferCount ){
		DETHROW( deeInvalidParam );
	}
	return pBuffers[ position ];
}



void deoalSource::SetOwner( void *owner ){
	pOwner = owner;
}

void deoalSource::SetImportance( float importance ){
	pImportance = importance;
}



bool deoalSource::IsBound() const{
	return pOwner != NULL;
}

bool deoalSource::IsUnbound() const{
	return pOwner == NULL;
}



void deoalSource::Play(){
// 	if( pState == epsPlaying ){
// 		return;
// 	}
	
	OAL_CHECK( pAudioThread, alSourcePlay( pSource ) );
	pState = epsPlaying;
}

void deoalSource::Pause(){
// 	if( pState == epsPaused ){
// 		return;
// 	}
	
	OAL_CHECK( pAudioThread, alSourcePause( pSource ) );
	pState = epsPaused;
}

void deoalSource::Stop(){
// 	if( pState == epsStopped ){
// 		return;
// 	}
	
	// NOTE OpenAL sources can be in three possible states: undefined, static or streaming.
	//      The initial state is undefined. Setting a static buffer using AL_BUFFER switches
	//      to the static state. Using QueueBuffers instead switches to the streaming state.
	//      Once in a state it is not allowed to switch to the other state (static to streaming
	//      or vice-versa). To reset the state to undefined AL_NONE has to be used with AL_BUFFER.
	//      This is sort of a magic action also clearing the buffers if present and removing
	//      any sources to play. This is though only allowed while the source is stopped or
	//      in the initial state. Thus the proper order is 'stop' then AL_BUFFER(AL_NONE).
	OAL_CHECK( pAudioThread, alSourceStop( pSource ) );
	OAL_CHECK( pAudioThread, alSourcei( pSource, AL_BUFFER, AL_NONE ) );
	pState = epsStopped;
}



ALuint deoalSource::GetFilter(){
	if( ! pFilter ){
		OAL_CHECK( pAudioThread, palGenFilters( 1, &pFilter ) );
	}
	return pFilter;
}

void deoalSource::AssignFilter(){
	if( pFilter ){
		OAL_CHECK( pAudioThread, alSourcei( pSource, AL_DIRECT_FILTER, pFilter ) );
		
	}else{
		OAL_CHECK( pAudioThread, alSourcei( pSource, AL_DIRECT_FILTER, AL_FILTER_NULL ) );
	}
}

void deoalSource::ClearFilter(){
	OAL_CHECK( pAudioThread, alSourcei( pSource, AL_DIRECT_FILTER, AL_FILTER_NULL ) );
}



ALuint deoalSource::GetSendSlot( int index ){
	if( ! pSendSlot ){
		OAL_CHECK( pAudioThread, palGenAuxiliaryEffectSlots( 1, &pSendSlot ) );
		
		// prevent reverb effects apply distance based statistics model
		OAL_CHECK( pAudioThread, alSourcei( pSource, AL_AUXILIARY_SEND_FILTER_GAIN_AUTO, AL_FALSE ) );
		OAL_CHECK( pAudioThread, alSourcei( pSource, AL_AUXILIARY_SEND_FILTER_GAINHF_AUTO, AL_FALSE ) );
	}
	
	return pSendSlot;
}

ALuint deoalSource::GetSendEffect( int index ){
	if( ! pSendEffect ){
		OAL_CHECK( pAudioThread, palGenEffects( 1, &pSendEffect ) );
	}
	return pSendEffect;
}

void deoalSource::AssignSendEffect( int index ){
	if( pSendEffect ){
		const ALuint slot = GetSendSlot( index );
		
		// prevent reverb effects apply distance based statistics model
		OAL_CHECK( pAudioThread, palAuxiliaryEffectSloti( slot, AL_EFFECTSLOT_AUXILIARY_SEND_AUTO, AL_FALSE ) );
		
		OAL_CHECK( pAudioThread, palAuxiliaryEffectSloti( slot, AL_EFFECTSLOT_EFFECT, pSendEffect ) );
		OAL_CHECK( pAudioThread, alSource3i( pSource, AL_AUXILIARY_SEND_FILTER,
			slot, 0/*index*/, AL_FILTER_NULL ) );
		
	}else{
		ClearSendEffect( index );
	}
}

void deoalSource::ClearSendEffect( int index ){
	OAL_CHECK( pAudioThread, alSource3i( pSource, AL_AUXILIARY_SEND_FILTER,
		AL_EFFECTSLOT_NULL, 0/*index*/, AL_FILTER_NULL ) );
}

void deoalSource::ClearAllSendEffects(){
	OAL_CHECK( pAudioThread, alSource3i( pSource, AL_AUXILIARY_SEND_FILTER,
		AL_EFFECTSLOT_NULL, 0/*index*/, AL_FILTER_NULL ) );
}



// Private Functions
//////////////////////

void deoalSource::pCleanUp(){
	ClearSendEffect( 0 );
	if( pSendSlot ){
		palDeleteAuxiliaryEffectSlots( 1, &pSendSlot );
		pSendSlot = 0;
	}
	if( pSendEffect ){
		palDeleteEffects( 1, &pSendEffect );
		pSendEffect = 0;
	}
	
	ClearFilter();
	if( pFilter ){
		palDeleteFilters( 1, &pFilter );
		pFilter = 0;
	}
	
	if( pBuffers ){
		alDeleteBuffers( pBufferCount, pBuffers );
		delete [] pBuffers;
	}
	
	if( pSource ){
		alDeleteSources( 1, &pSource );
	}
}
