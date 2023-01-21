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

#include <stdio.h>
#include <stdlib.h>

#include "deoalEffectSlot.h"
#include "deoalEffectSlotManager.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATLogger.h"
#include "../capabilities/deoalCapabilities.h"

#include <dragengine/common/exceptions.h>


// Class deoalEffectSlotManager
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoalEffectSlotManager::deoalEffectSlotManager( deoalAudioThread &audioThread ) :
pAudioThread( audioThread ),
pMaxCount( 0 ){
}

deoalEffectSlotManager::~deoalEffectSlotManager(){
}



// Management
///////////////

deoalEffectSlot *deoalEffectSlotManager::Bind( void *owner, float importance ){
	DEASSERT_NOTNULL( owner )
	
	deoalEffectSlot *slot = pNextUnbound();
	if( slot ){
		slot->AssignOwner( owner, importance );
		return slot;
	}
	
	slot = pCreateNew();
	if( slot ){
		slot->AssignOwner( owner, importance );
		return slot;
	}
	
	slot = pBestRebindable();
	if( slot && importance <= slot->GetImportance() ){
		slot->AssignOwner( owner, importance );
		return slot;
	}
	
	return nullptr;
}

void deoalEffectSlotManager::Unbind( deoalEffectSlot *slot ){
	DEASSERT_NOTNULL( slot )
	DEASSERT_NOTNULL( slot->GetOwner() )
	
	slot->ClearOwner();
}



// Private Functions
//////////////////////

deoalEffectSlot *deoalEffectSlotManager::pNextUnbound() const{
	const int count = pSlots.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoalEffectSlot * const slot = ( deoalEffectSlot* )pSlots.GetAt( i );
		if( slot->IsUnbound() ){
			return slot;
		}
	}
	
	return nullptr;
}

deoalEffectSlot *deoalEffectSlotManager::pBestRebindable() const{
	const int count = pSlots.GetCount();
	deoalEffectSlot *bestSlot = nullptr;
	int i;
	
	for( i=0; i<count; i++ ){
		deoalEffectSlot * const slot = ( deoalEffectSlot* )pSlots.GetAt( i );
		if( slot->IsBound() && ( ! bestSlot || slot->GetImportance() < bestSlot->GetImportance() ) ){
			bestSlot = slot;
		}
	}
	
	return bestSlot;
}

deoalEffectSlot *deoalEffectSlotManager::pCreateNew(){
	if( pMaxCount != 0 && pSlots.GetCount() == pMaxCount ){
		return nullptr;
	}
	
	try{
		const deoalEffectSlot::Ref slot( deoalEffectSlot::Ref::New( new deoalEffectSlot( pAudioThread ) ) );
		pSlots.Add( slot );
		return slot;
		
	}catch( const deeOutOfMemory & ){
		// assume we hit the maximum count of effect slots. according to openal code
		// this should be 64 but since we can not query this limit we use this version
		pMaxCount = pSlots.GetCount();
		pAudioThread.GetLogger().LogInfoFormat( "OutOfMemoy while creating effect."
			" Assuming maximum effect slot count %d", pMaxCount );
		return nullptr;
		
	}catch( const deException &e ){
		pAudioThread.GetLogger().LogException( e );
		pMaxCount = pSlots.GetCount();
		pAudioThread.GetLogger().LogInfoFormat( "Exception while creating effect."
			" Assuming maximum effect slot count %d", pMaxCount );
		return nullptr;
	}
}
