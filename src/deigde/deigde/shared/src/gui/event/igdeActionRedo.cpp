/* 
 * Drag[en]gine IGDE
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

#include "igdeActionRedo.h"
#include "../../environment/igdeEnvironment.h"
#include "../../undo/igdeUndoSystem.h"

#include <dragengine/common/exceptions.h>



// Class igdeActionRedo
/////////////////////////

// Constructor, destructor
////////////////////////////

igdeActionRedo::igdeActionRedo( igdeEnvironment &environment, igdeUndoSystem *undoSystem ) :
igdeAction(
	"Redo",
	environment.GetStockIcon( igdeEnvironment::esiRedo ),
	"Redo last action",
	deInputEvent::ekcR,
	igdeHotKey( deInputEvent::esmControl, deInputEvent::ekcY ) ),
pEnvironment( environment ),
pUndoSystem( undoSystem ){
}

igdeActionRedo::~igdeActionRedo(){
}



// Management
///////////////

void igdeActionRedo::SetUndoSystem( igdeUndoSystem *undoSystem ){
	if( undoSystem == pUndoSystem ){
		return;
	}
	
	pUndoSystem = undoSystem;
	Update();
}



void igdeActionRedo::OnAction(){
	if( pUndoSystem && pUndoSystem->GetRedoableCount() > 0 ){
		pUndoSystem->Redo();
	}
}

void igdeActionRedo::Update(){
	SetEnabled( pUndoSystem && pUndoSystem->GetRedoableCount() > 0 );
}
