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

#include "meUNavSpaceSetOrientation.h"
#include "../../../world/meWorld.h"
#include "../../../world/meWorldGuiParameters.h"
#include "../../../world/navspace/meNavigationSpace.h"

#include <dragengine/common/exceptions.h>



// Class meUNavSpaceSetOrientation
////////////////////////////////

// Constructor, destructor
////////////////////////////

meUNavSpaceSetOrientation::meUNavSpaceSetOrientation( meNavigationSpace *navspace, const decVector &newOrientation ){
	if( ! navspace ){
		DETHROW( deeInvalidParam );
	}
	
	meWorld *world = navspace->GetWorld();
	if( ! world ){
		DETHROW( deeInvalidParam );
	}
	
	pNavSpace = NULL;
	
	SetShortInfo( "NavSpace Set Orientation" );
	
	pOldOrientation = navspace->GetOrientation();
	pNewOrientation = newOrientation;
	
	pNavSpace = navspace;
	navspace->AddReference();
}

meUNavSpaceSetOrientation::~meUNavSpaceSetOrientation(){
	if( pNavSpace ){
		pNavSpace->FreeReference();
	}
}



// Management
///////////////

void meUNavSpaceSetOrientation::Undo(){
	pNavSpace->SetOrientation( pOldOrientation );
	pNavSpace->GetWorld()->GetGuiParameters().SetElementMode( meWorldGuiParameters::eemNavSpace );
}

void meUNavSpaceSetOrientation::Redo(){
	pNavSpace->SetOrientation( pNewOrientation );
	pNavSpace->GetWorld()->GetGuiParameters().SetElementMode( meWorldGuiParameters::eemNavSpace );
}
