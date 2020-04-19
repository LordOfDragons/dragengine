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

#include "meUHTSetTexSkin.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/terrain/meHeightTerrain.h"
#include "../../../../world/terrain/meHeightTerrainSector.h"
#include "../../../../world/terrain/meHeightTerrainTexture.h"

#include <dragengine/common/exceptions.h>



// Class meUHTSetTexSkin
//////////////////////////

// Constructor, destructor
////////////////////////////

meUHTSetTexSkin::meUHTSetTexSkin( meWorld *world, meHeightTerrainSector *sector, meHeightTerrainTexture *texture, const char *newPath ){
	if( ! world || ! sector || ! texture || ! newPath ) DETHROW( deeInvalidParam );
	
	pWorld = world;
	pSector = sector;
	
	pOldPath = texture->GetPathSkin();
	pNewPath = newPath;
	
	SetShortInfo( "Set Height Terrain Texture Skin" );
	
	world->AddReference();
	
	pTexture = texture;
	texture->AddReference();
}

meUHTSetTexSkin::~meUHTSetTexSkin(){
	if( pTexture ) pTexture->FreeReference();
	if( pWorld ) pWorld->FreeReference();
}



// Management
///////////////


void meUHTSetTexSkin::Undo(){
	pTexture->SetPathSkin( pOldPath.GetString() );
}

void meUHTSetTexSkin::Redo(){
	pTexture->SetPathSkin( pNewPath.GetString() );
}
