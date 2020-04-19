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

#include "meUHTNavSpaceTypeSetName.h"
#include "../../../../../world/terrain/meHeightTerrainNavSpaceType.h"

#include <dragengine/common/exceptions.h>



// Class meUHTNavSpaceTypeSetName
///////////////////////////////////

// Constructor, destructor
////////////////////////////

meUHTNavSpaceTypeSetName::meUHTNavSpaceTypeSetName( meHeightTerrainNavSpaceType *type, const char *newName ) :
pType( NULL ),
pNewName( newName )
{
	if( ! type ){
		DETHROW( deeInvalidParam );
	}
	
	pOldName = type->GetName();
	
	SetShortInfo( "Height terrain nav-space type set name" );
	
	pType = type;
	type->AddReference();
}

meUHTNavSpaceTypeSetName::~meUHTNavSpaceTypeSetName(){
	if( pType ){
		pType->FreeReference();
	}
}



// Management
///////////////

void meUHTNavSpaceTypeSetName::Undo(){
	pType->SetName( pOldName );
}

void meUHTNavSpaceTypeSetName::Redo(){
	pType->SetName( pNewName );
}
