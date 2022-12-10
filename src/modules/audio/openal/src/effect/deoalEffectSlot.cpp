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

#include <dragengine/common/exceptions.h>



// Class deoalEffectSlot
//////////////////////////

// Constructor, destructor
////////////////////////////

deoalEffectSlot::deoalEffectSlot( deoalAudioThread &audioThread ) :
pAudioThread( audioThread ),
pSlot( 0 ),
pEffect( 0 ),
pOwner( nullptr ),
pImportance( 1000.0f )
{
	try{
		OAL_CHECK( audioThread, palGenAuxiliaryEffectSlots( 1, &pSlot ) );
		OAL_CHECK( audioThread, palGenEffects( 1, &pEffect ) );
		OAL_CHECK( audioThread, palAuxiliaryEffectSloti( pSlot, AL_EFFECTSLOT_EFFECT, pEffect ) );
		
		// prevent reverb effects apply distance based statistics model
		OAL_CHECK( pAudioThread, palAuxiliaryEffectSloti( pSlot, AL_EFFECTSLOT_AUXILIARY_SEND_AUTO, AL_FALSE ) );
		
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
	
	pOwner = owner;
	pImportance = importance;
	ClearEffect();
}

void deoalEffectSlot::ClearOwner(){
	if( pOwner ){
		return;
	}
	
	ClearEffect();
	pOwner = nullptr;
	pImportance = 1000.0f;
}

void deoalEffectSlot::SetImportance( float importance ){
	pImportance = importance;
}

void deoalEffectSlot::ClearEffect(){
	OAL_CHECK( pAudioThread, palEffecti( pEffect, AL_EFFECT_TYPE, AL_EFFECT_NULL ) );
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
