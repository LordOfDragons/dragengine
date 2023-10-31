/* 
 * Drag[en]gine OpenAL Audio Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#include "deoalEffectSlot.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATLogger.h"

#include <dragengine/common/exceptions.h>



// #define DO_DEBUG 1

#ifdef DO_DEBUG
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif



// Class deoalEffectSlot
//////////////////////////

// Constructor, destructor
////////////////////////////

deoalEffectSlot::deoalEffectSlot( deoalAudioThread &audioThread ) :
pAudioThread( audioThread ),
pSlot( 0 ),
pEffect( 0 ),
pOwner( nullptr ),
pImportance( -1000.0f ),
pEffectType( AL_EFFECT_NULL ),
	pParametersChanged ( false ),
pKeepAliveElapsed( 0.0f ),
pKeepAliveTimeout( 0.0f )
{
	try{
		alGetError();
		palGenAuxiliaryEffectSlots( 1, &pSlot );
		if( alGetError() != AL_NO_ERROR ){
			DETHROW( deeOutOfMemory );
		}
		
		OAL_CHECK( audioThread, palGenEffects( 1, &pEffect ) );
		
		// prevent reverb effects apply distance based statistics model
		OAL_CHECK( audioThread, palAuxiliaryEffectSloti( pSlot, AL_EFFECTSLOT_AUXILIARY_SEND_AUTO, AL_FALSE ) );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoalEffectSlot::~deoalEffectSlot(){
	pCleanUp();
}



// Management
///////////////

void deoalEffectSlot::AssignOwner( void *owner, float importance ){
	DEASSERT_NOTNULL( owner )
	
	DEBUG( pAudioThread.GetLogger().LogInfoFormat( "EffectSlot: AssignOwner %d with %p[%g]",
		pSlot, owner, importance ) );
	pOwner = owner;
	pImportance = importance;
	pKeepAliveElapsed = 0.0f;
	pKeepAliveTimeout = 0.0f;
}

void deoalEffectSlot::ClearOwner(){
	if( ! pOwner ){
		return;
	}
	
	DEBUG( pAudioThread.GetLogger().LogInfoFormat(
		"EffectSlot: ClearOwner %d with %p[%g] keep-alive %g",
		pSlot, pOwner, pImportance, pKeepAliveTimeout ) );
	pOwner = nullptr;
	pImportance = -1000.0f;
}

void deoalEffectSlot::SetImportance( float importance ){
	pImportance = importance;
}



void deoalEffectSlot::SetEffectType( ALenum type ){
	if( type == pEffectType ){
		return;
	}
	
	pEffectType = type;
	pParametersChanged = true;
	
	OAL_CHECK( pAudioThread, palEffecti( pEffect, AL_EFFECT_TYPE, type ) );
}



void deoalEffectSlot::UpdateSlot( float timeout ){
	pKeepAliveElapsed = 0.0f;
	pKeepAliveTimeout = timeout;
	
	pUpdateSlotParameters();
}



void deoalEffectSlot::Update( float elapsed ){
	if( pOwner ){
		return;
	}
	
	pKeepAliveElapsed += elapsed;
	
	if( pKeepAliveElapsed >= pKeepAliveTimeout && pEffectType != AL_EFFECT_NULL ){
		DEBUG( pAudioThread.GetLogger().LogInfoFormat( "EffectSlot: Timeout %d with %p[%g]",
			pSlot, pOwner, pImportance ) );
		SetEffectType( AL_EFFECT_NULL );
		pUpdateSlotParameters();
	}
}



// Private Functions
//////////////////////

void deoalEffectSlot::pCleanUp(){
	if( pSlot ){
		palDeleteAuxiliaryEffectSlots( 1, &pSlot );
	}
	if( pEffect ){
		palDeleteEffects( 1, &pEffect );
	}
}

void deoalEffectSlot::pUpdateSlotParameters(){
	if( ! pParametersChanged ){
		return;
	}
	
	OAL_CHECK( pAudioThread, palAuxiliaryEffectSloti( pSlot, AL_EFFECTSLOT_EFFECT, pEffect ) );
	DEBUG( pAudioThread.GetLogger().LogInfoFormat( "EffectSlot: UpdateSlot %d with %p[%g]",
		pSlot, pOwner, pImportance ) );
}
