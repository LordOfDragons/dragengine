/* 
 * Drag[en]gine IGDE Sky Editor
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

#include "seWPUndoHistory.h"
#include "seWPUndoHistoryListener.h"
#include "../../sky/seSky.h"

#include <dragengine/common/exceptions.h>



// Class seWPUndoHistory
//////////////////////////

// Constructor, destructor
////////////////////////////

seWPUndoHistory::seWPUndoHistory( igdeEnvironment &environment ) :
igdeWPUndoHistory( environment ),
pListener( new seWPUndoHistoryListener( *this ) ),
pSky( NULL ){
}

seWPUndoHistory::~seWPUndoHistory(){
	SetSky( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void seWPUndoHistory::SetSky( seSky *sky ){
	if( sky == pSky ){
		return;
	}
	
	SetUndoSystem( NULL );
	
	if( pSky ){
		pSky->RemoveListener( pListener );
		pSky->FreeReference();
	}
	
	pSky = sky;
	
	if( sky ){
		sky->AddListener( pListener );
		sky->AddReference();
		
		SetUndoSystem( sky->GetUndoSystem() );
	}
}
