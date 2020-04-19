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

#include "meUDecalTCScaling.h"
#include "../../../world/meWorld.h"
#include "../../../world/decal/meDecal.h"
#include "../../../world/meWorldGuiParameters.h"

#include <dragengine/common/exceptions.h>



// Class meUDecalTCScaling
////////////////////////////

// Constructor, destructor
////////////////////////////

meUDecalTCScaling::meUDecalTCScaling( meDecal *decal, const decVector2 &newScaling ){
	if( ! decal ){
		DETHROW( deeInvalidParam );
	}
	
	meWorld * const world = decal->GetWorld();
	if( ! world ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Decal Texture Scaling" );
	
	pDecal = NULL;
	
	pOldScaling = decal->GetTexCoordScaling();
	pNewScaling = newScaling;
	
	pDecal = decal;
	decal->AddReference();
}

meUDecalTCScaling::~meUDecalTCScaling(){
	if( pDecal ){
		pDecal->FreeReference();
	}
}



// Management
///////////////

void meUDecalTCScaling::Undo(){
	pDecal->SetTexCoordScaling( pOldScaling );
	pDecal->GetWorld()->GetGuiParameters().SetElementMode( meWorldGuiParameters::eemDecal );
}

void meUDecalTCScaling::Redo(){
	pDecal->SetTexCoordScaling( pNewScaling );
	pDecal->GetWorld()->GetGuiParameters().SetElementMode( meWorldGuiParameters::eemDecal );
}
