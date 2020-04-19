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

#include "meUDecalPosition.h"
#include "../../../world/meWorld.h"
#include "../../../world/decal/meDecal.h"
#include "../../../world/meWorldGuiParameters.h"

#include <dragengine/common/exceptions.h>



// Class meUDecalPosition
///////////////////////////

// Constructor, destructor
////////////////////////////

meUDecalPosition::meUDecalPosition( meDecal *decal, const decDVector &newPosition ){
	if( ! decal ){
		DETHROW( deeInvalidParam );
	}
	
	meWorld * const world = decal->GetWorld();
	if( ! world ){
		DETHROW( deeInvalidParam );
	}
	
	pDecal = NULL;
	
	SetShortInfo( "Set decal position" );
	
	pOldPosition = decal->GetPosition();
	pNewPosition = newPosition;
	
	pDecal = decal;
	decal->AddReference();
}

meUDecalPosition::~meUDecalPosition(){
	if( pDecal ){
		pDecal->FreeReference();
	}
}



// Management
///////////////

void meUDecalPosition::Undo(){
	pDecal->SetPosition( pOldPosition );
	pDecal->GetWorld()->GetGuiParameters().SetElementMode( meWorldGuiParameters::eemDecal );
}

void meUDecalPosition::Redo(){
	pDecal->SetPosition( pNewPosition );
	pDecal->GetWorld()->GetGuiParameters().SetElementMode( meWorldGuiParameters::eemDecal );
}
