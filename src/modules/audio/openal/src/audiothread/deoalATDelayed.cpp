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

#include "deoalATDelayed.h"
#include "deoalDelayedDeletion.h"
#include "deoalAudioThread.h"
#include "deoalATLogger.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/threading/deMutexGuard.h>


// Class deoalATDelayed
/////////////////////////

// Constructor, destructor
////////////////////////////

deoalATDelayed::deoalATDelayed( deoalAudioThread &audioThread ) :
pAudioThread( audioThread ),

pHasFreeOperations( false ),
pRootDeletion( NULL ),
pTailDeletion( NULL ),
pDeletionCount( 0 ){
}

deoalATDelayed::~deoalATDelayed(){
	ProcessFreeOperations( true );
}



// Management
//////////////

void deoalATDelayed::ProcessFreeOperations( bool deleteAll ){
	deMutexGuard lock( pMutexFree );
	
	if( pHasFreeOperations ){
		int countThreshold = 1000;
		
		while( pRootDeletion && ( deleteAll || countThreshold-- > 0 ) ){
			deoalDelayedDeletion * const deletion = pRootDeletion;
			pRootDeletion = pRootDeletion->GetLLNext();
			if( pRootDeletion ){
				pRootDeletion->SetLLPrev( NULL );
			}
			pDeletionCount--;
			
			deletion->DeleteObjects( pAudioThread );
			delete deletion;
		}
		
		if( pDeletionCount == 0 ){
			pTailDeletion = NULL;
		}
		
		pHasFreeOperations = pDeletionCount > 0;
	}
}

void deoalATDelayed::AddDeletion( deoalDelayedDeletion *deletion ){
	if( ! deletion ){
		DETHROW( deeInvalidParam );
	}
	
	deMutexGuard lock( pMutexFree );
	
	if( pTailDeletion ){
		pTailDeletion->SetLLNext( deletion );
		deletion->SetLLPrev( pTailDeletion );
		pTailDeletion = deletion;
		
	}else{
		pRootDeletion = deletion;
		pTailDeletion = deletion;
	}
	
	pDeletionCount++;
	
	pHasFreeOperations = true;
}
