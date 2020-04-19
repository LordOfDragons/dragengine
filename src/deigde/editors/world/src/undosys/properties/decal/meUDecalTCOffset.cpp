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

#include "meUDecalTCOffset.h"
#include "../../../world/meWorld.h"
#include "../../../world/decal/meDecal.h"
#include "../../../world/meWorldGuiParameters.h"

#include <dragengine/common/exceptions.h>



// Class meUDecalTCOffset
///////////////////////////

// Constructor, destructor
////////////////////////////

meUDecalTCOffset::meUDecalTCOffset( meDecal *decal, const decVector2 &newOffset ){
	if( ! decal ){
		DETHROW( deeInvalidParam );
	}
	
	meWorld * const world = decal->GetWorld();
	if( ! world ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Decal Texture Offset" );
	
	pDecal = NULL;
	
	pOldOffset = decal->GetTexCoordOffset();
	pNewOffset = newOffset;
	
	pDecal = decal;
	decal->AddReference();
}

meUDecalTCOffset::~meUDecalTCOffset(){
	if( pDecal ){
		pDecal->FreeReference();
	}
}



// Management
///////////////

void meUDecalTCOffset::Undo(){
	pDecal->SetTexCoordOffset( pOldOffset );
	pDecal->GetWorld()->GetGuiParameters().SetElementMode( meWorldGuiParameters::eemDecal );
}

void meUDecalTCOffset::Redo(){
	pDecal->SetTexCoordOffset( pNewOffset );
	pDecal->GetWorld()->GetGuiParameters().SetElementMode( meWorldGuiParameters::eemDecal );
}
