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

#include "meUDecalTCRotation.h"
#include "../../../world/meWorld.h"
#include "../../../world/decal/meDecal.h"
#include "../../../world/meWorldGuiParameters.h"

#include <dragengine/common/exceptions.h>



// Class meUDecalTCRotation
/////////////////////////////

// Constructor, destructor
////////////////////////////

meUDecalTCRotation::meUDecalTCRotation( meDecal *decal, float newRotation ){
	if( ! decal ){
		DETHROW( deeInvalidParam );
	}
	
	meWorld * const world = decal->GetWorld();
	if( ! world ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Decal Texture Rotation" );
	
	pDecal = NULL;
	
	pOldRotation = decal->GetTexCoordRotation();
	pNewRotation = newRotation;
	
	pDecal = decal;
	decal->AddReference();
}

meUDecalTCRotation::~meUDecalTCRotation(){
	if( pDecal ){
		pDecal->FreeReference();
	}
}



// Management
///////////////

void meUDecalTCRotation::Undo(){
	pDecal->SetTexCoordRotation( pOldRotation );
	pDecal->GetWorld()->GetGuiParameters().SetElementMode( meWorldGuiParameters::eemDecal );
}

void meUDecalTCRotation::Redo(){
	pDecal->SetTexCoordRotation( pNewRotation );
	pDecal->GetWorld()->GetGuiParameters().SetElementMode( meWorldGuiParameters::eemDecal );
}
