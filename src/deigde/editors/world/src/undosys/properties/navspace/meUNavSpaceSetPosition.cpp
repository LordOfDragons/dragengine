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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "meUNavSpaceSetPosition.h"
#include "../../../world/meWorld.h"
#include "../../../world/meWorldGuiParameters.h"
#include "../../../world/navspace/meNavigationSpace.h"

#include <dragengine/common/exceptions.h>



// Class meUNavSpaceSetPosition
////////////////////////////////

// Constructor, destructor
////////////////////////////

meUNavSpaceSetPosition::meUNavSpaceSetPosition( meNavigationSpace *navspace, const decDVector &newPosition ){
	if( ! navspace ){
		DETHROW( deeInvalidParam );
	}
	
	meWorld *world = navspace->GetWorld();
	if( ! world ){
		DETHROW( deeInvalidParam );
	}
	
	pNavSpace = NULL;
	
	SetShortInfo( "NavSpace Set Position" );
	
	pOldPosition = navspace->GetPosition();
	pNewPosition = newPosition;
	
	pNavSpace = navspace;
	navspace->AddReference();
}

meUNavSpaceSetPosition::~meUNavSpaceSetPosition(){
	if( pNavSpace ){
		pNavSpace->FreeReference();
	}
}



// Management
///////////////

void meUNavSpaceSetPosition::Undo(){
	pNavSpace->SetPosition( pOldPosition );
	pNavSpace->GetWorld()->GetGuiParameters().SetElementMode( meWorldGuiParameters::eemNavSpace );
}

void meUNavSpaceSetPosition::Redo(){
	pNavSpace->SetPosition( pNewPosition );
	pNavSpace->GetWorld()->GetGuiParameters().SetElementMode( meWorldGuiParameters::eemNavSpace );
}
