/* 
 * Drag[en]gine IGDE Rig Editor
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

#include "reURemovePush.h"
#include "../../../rig/reRig.h"
#include "../../../rig/push/reRigPush.h"
#include "../../../rig/push/reRigPushList.h"
#include "../../../rig/push/reSelectionPushes.h"

#include "dragengine/common/exceptions.h"



// Class reURemovePush
////////////////////////

// Constructor, destructor
////////////////////////////

reURemovePush::reURemovePush( reRigPushList &list ){
	int pushCount = list.GetPushCount();
	
	if( pushCount == 0 ) DETHROW( deeInvalidParam );
	
	pPushes = NULL;
	pPushCount = 0;
	
	try{
		pPushes = new reRigPush*[ pushCount ];
		if( ! pPushes ) DETHROW( deeOutOfMemory );
		
		while( pPushCount < pushCount ){
			pPushes[ pPushCount ] = list.GetPushAt( pPushCount );
			pPushes[ pPushCount ]->AddReference();
			pPushCount++;
		}
		
		SetShortInfo( "Remove Pushes" );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

reURemovePush::~reURemovePush(){
	pCleanUp();
}



// Management
///////////////

void reURemovePush::Undo(){
	reRig *rig = pPushes[ 0 ]->GetRig();
	if( ! rig ) DETHROW( deeInvalidParam );
	
	reSelectionPushes *selection = rig->GetSelectionPushes();
	int p;
	
	selection->RemoveAllPushes();
	
	for( p=0; p<pPushCount; p++ ){
		rig->AddPush( pPushes[ p ] );
		
		selection->AddPush( pPushes[ p ] );
	}
	
}

void reURemovePush::Redo(){
	reRig *rig = pPushes[ 0 ]->GetRig();
	if( ! rig ) DETHROW( deeInvalidParam );
	
	reSelectionPushes *selection = rig->GetSelectionPushes();
	int p;
	
	for( p=0; p<pPushCount; p++ ){
		if( pPushes[ p ]->GetSelected() ){
			selection->RemovePush( pPushes[ p ] );
		}
		
		rig->RemovePush( pPushes[ p ] );
	}
}



// Private Functions
//////////////////////

void reURemovePush::pCleanUp(){
	if( pPushes ){
		while( pPushCount > 0 ){
			pPushCount--;
			pPushes[ pPushCount ]->FreeReference();
		}
		
		delete [] pPushes;
	}
}
