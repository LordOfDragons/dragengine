/* 
 * Drag[en]gine IGDE World Editor
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

#include <stdlib.h>

#include "meUAddNavSpace.h"
#include "../../../world/meWorld.h"
#include "../../../world/navspace/meNavigationSpace.h"
#include "../../../world/navspace/meNavigationSpaceSelection.h"

#include <dragengine/common/exceptions.h>



// Class meUAddNavSpace
/////////////////////////

// Constructor, destructor
////////////////////////////

meUAddNavSpace::meUAddNavSpace( meWorld *world, const decPoint3 &sector, meNavigationSpace *object ) :
pWorld( NULL ),
pNavSpace( NULL )
{
	if( ! world || ! object ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Add Navigation Space" );
	
	pWorld = world;
	world->AddReference();
	
	pNavSpace = object;
	object->AddReference();
}

meUAddNavSpace::~meUAddNavSpace(){
	pCleanUp();
}



// Management
///////////////

void meUAddNavSpace::Undo(){
	meNavigationSpaceSelection &selection = pWorld->GetSelectionNavigationSpace();
	
	selection.Remove( pNavSpace );
	if( pNavSpace->GetActive() ){
		selection.ActivateNext();
	}
	
	pWorld->RemoveNavSpace( pNavSpace );
	
	pWorld->NotifyNavSpaceCountChanged();
	pWorld->NotifyNavSpaceSelectionChanged();
}

void meUAddNavSpace::Redo(){
	meNavigationSpaceSelection &selection = pWorld->GetSelectionNavigationSpace();
	
	pWorld->AddNavSpace( pNavSpace );
	
	selection.Reset();
	selection.Add( pNavSpace );
	selection.SetActive( pNavSpace );
	
	pWorld->NotifyNavSpaceCountChanged();
	pWorld->NotifyNavSpaceSelectionChanged();
}



// Private Functions
//////////////////////

void meUAddNavSpace::pCleanUp(){
	if( pNavSpace ){
		pNavSpace->FreeReference();
	}
	if( pWorld ){
		pWorld->FreeReference();
	}
}
