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

#include "meUHTNavSpaceFaceAdd.h"
#include "../../../../../../world/terrain/meHeightTerrainNavSpaceType.h"
#include "../../../../../../world/terrain/meHeightTerrainNavSpaceFace.h"

#include <dragengine/common/exceptions.h>



// Class meUHTNavSpaceFaceAdd
///////////////////////////////

// Constructor, destructor
////////////////////////////

meUHTNavSpaceFaceAdd::meUHTNavSpaceFaceAdd( meHeightTerrainNavSpaceType *type, meHeightTerrainNavSpaceFace *face ) :
pType( NULL ),
pFace( NULL )
{
	if( ! type || ! face ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Height terrain nav-space add face" );
	
	pType = type;
	type->AddReference();
	
	pFace = face;
	face->AddReference();
}

meUHTNavSpaceFaceAdd::~meUHTNavSpaceFaceAdd(){
	if( pFace ){
		pFace->FreeReference();
	}
	if( pType ){
		pType->FreeReference();
	}
}



// Management
///////////////

void meUHTNavSpaceFaceAdd::Undo(){
	pType->RemoveFace( pFace );
}

void meUHTNavSpaceFaceAdd::Redo(){
	pType->AddFace( pFace );
}
