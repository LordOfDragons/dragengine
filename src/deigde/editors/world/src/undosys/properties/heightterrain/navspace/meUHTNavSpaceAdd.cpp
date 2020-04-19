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

#include "meUHTNavSpaceAdd.h"
#include "../../../../world/terrain/meHeightTerrainSector.h"
#include "../../../../world/terrain/meHeightTerrainNavSpace.h"

#include <dragengine/common/exceptions.h>



// Class meUHTNavSpaceAdd
///////////////////////////

// Constructor, destructor
////////////////////////////

meUHTNavSpaceAdd::meUHTNavSpaceAdd( meHeightTerrainSector *sector, meHeightTerrainNavSpace *navspace ) :
pSector( NULL ),
pNavSpace( NULL )
{
	if( ! sector || ! navspace ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Height terrain add nav-space" );
	
	pSector = sector;
	sector->AddReference();
	
	pNavSpace = navspace;
	navspace->AddReference();
}

meUHTNavSpaceAdd::~meUHTNavSpaceAdd(){
	if( pNavSpace ){
		pNavSpace->FreeReference();
	}
	if( pSector ){
		pSector->FreeReference();
	}
}



// Management
///////////////

void meUHTNavSpaceAdd::Undo(){
	pSector->RemoveNavSpace( pNavSpace );
}

void meUHTNavSpaceAdd::Redo(){
	pSector->AddNavSpace( pNavSpace );
	pSector->SetActiveNavSpace( pNavSpace );
}
