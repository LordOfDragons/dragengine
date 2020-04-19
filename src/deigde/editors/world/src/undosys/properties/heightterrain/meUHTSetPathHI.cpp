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

#include "meUHTSetPathHI.h"
#include "../../../world/meWorld.h"
#include "../../../world/terrain/meHeightTerrain.h"
#include "../../../world/terrain/meHeightTerrainSector.h"

#include <dragengine/common/exceptions.h>



// Class meUHTSetPathHI
/////////////////////////

// Constructor, destructor
////////////////////////////

meUHTSetPathHI::meUHTSetPathHI( meWorld *world, meHeightTerrainSector *sector, const char *newPath ){
	if( ! world || ! sector || ! newPath ) DETHROW( deeInvalidParam );
	
	pWorld = world;
	pSector = sector;
	
	pOldPath = sector->GetPathHeightImage();
	pNewPath = newPath;
	
	SetShortInfo( "Set Height Terrain Height Image Path" );
	
	world->AddReference();
}

meUHTSetPathHI::~meUHTSetPathHI(){
	if( pWorld ) pWorld->FreeReference();
}



// Management
///////////////


void meUHTSetPathHI::Undo(){
	pSector->SetPathHeightImage( pOldPath.GetString(), false );
}

void meUHTSetPathHI::Redo(){
	pSector->SetPathHeightImage( pNewPath.GetString(), false );
}
