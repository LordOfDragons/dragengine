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

#include "meUHTVVarSetRestitution.h"
#include "../../../../world/heightterrain/meHTVVariation.h"

#include <dragengine/common/exceptions.h>



// Class meUHTVVarSetRestitution
//////////////////////////////////

// Constructor, destructor
////////////////////////////

meUHTVVarSetRestitution::meUHTVVarSetRestitution( meHTVVariation *variation, float newRestitution ){
	if( ! variation ) DETHROW( deeInvalidParam );
	
	pVariation = variation;
	
	pOldRestitution = variation->GetRestitution();
	pNewRestitution = newRestitution;
	
	SetShortInfo( "Vegetation Layer Variation Set Restitution" );
	
	variation->AddReference();
}

meUHTVVarSetRestitution::~meUHTVVarSetRestitution(){
	if( pVariation ) pVariation->FreeReference();
}



// Management
///////////////

void meUHTVVarSetRestitution::Undo(){
	pVariation->SetRestitution( pOldRestitution );
}

void meUHTVVarSetRestitution::Redo(){
	pVariation->SetRestitution( pNewRestitution );
}
