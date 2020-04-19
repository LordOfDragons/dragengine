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
#include <string.h>

#include "reWPUndoHistory.h"
#include "reWPUndoHistoryListener.h"
#include "../../rig/reRig.h"

#include <dragengine/common/exceptions.h>



// Class reWPUndoHistory
//////////////////////////

// Constructor, destructor
////////////////////////////

reWPUndoHistory::reWPUndoHistory( igdeEnvironment &environment ) :
igdeWPUndoHistory( environment ),
pRig( NULL ),
pListener( new reWPUndoHistoryListener( *this ) ){
}

reWPUndoHistory::~reWPUndoHistory(){
	SetRig( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void reWPUndoHistory::SetRig( reRig *rig ){
	if( rig == pRig ){
		return;
	}
	
	SetUndoSystem( NULL );
	
	if( pRig ){
		pRig->RemoveNotifier( pListener );
		pRig->FreeReference();
	}
	
	pRig = rig;
	
	if( rig ){
		rig->AddNotifier( pListener );
		rig->AddReference();
		
		SetUndoSystem( rig->GetUndoSystem() );
	}
}
