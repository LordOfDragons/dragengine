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

#include "meUHTNavSpaceTypeSetType.h"
#include "../../../../../world/terrain/meHeightTerrainNavSpaceType.h"

#include <dragengine/common/exceptions.h>



// Class meUHTNavSpaceTypeSetType
///////////////////////////////////

// Constructor, destructor
////////////////////////////

meUHTNavSpaceTypeSetType::meUHTNavSpaceTypeSetType( meHeightTerrainNavSpaceType *type, int newType ) :
pType( NULL ),
pNewType( newType )
{
	if( ! type ){
		DETHROW( deeInvalidParam );
	}
	
	pOldType = type->GetType();
	
	SetShortInfo( "Height terrain nav-space type set type" );
	
	pType = type;
	type->AddReference();
}

meUHTNavSpaceTypeSetType::~meUHTNavSpaceTypeSetType(){
	if( pType ){
		pType->FreeReference();
	}
}



// Management
///////////////

void meUHTNavSpaceTypeSetType::Undo(){
	pType->SetType( pOldType );
}

void meUHTNavSpaceTypeSetType::Redo(){
	pType->SetType( pNewType );
}
