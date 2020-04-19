/* 
 * Drag[en]gine IGDE Skin Editor
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
#include "../../skin/seSkin.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class seWPUndoHistory
//////////////////////////

// Constructor, destructor
////////////////////////////

seWPUndoHistory::seWPUndoHistory( igdeEnvironment &environment ) :
igdeWPUndoHistory( environment ),
pListener( new seWPUndoHistoryListener( *this ) ),
pSkin( NULL ){
}

seWPUndoHistory::~seWPUndoHistory(){
	SetSkin( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void seWPUndoHistory::SetSkin( seSkin *skin ){
	if( skin == pSkin ){
		return;
	}
	
	SetUndoSystem( NULL );
	
	if( pSkin ){
		pSkin->RemoveListener( pListener );
		pSkin->FreeReference();
	}
	
	pSkin = skin;
	
	if( skin ){
		skin->AddListener( pListener );
		skin->AddReference();
		
		SetUndoSystem( skin->GetUndoSystem() );
	}
}
