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

#include "meUHTSetTexUVRotation.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/terrain/meHeightTerrain.h"
#include "../../../../world/terrain/meHeightTerrainSector.h"
#include "../../../../world/terrain/meHeightTerrainTexture.h"

#include <dragengine/common/exceptions.h>



// Class meUHTSetTexUVRotation
/////////////////////////

// Constructor, destructor
////////////////////////////

meUHTSetTexUVRotation::meUHTSetTexUVRotation( meWorld *world, meHeightTerrainSector *sector, meHeightTerrainTexture *texture, float newRotation ){
	if( ! world || ! sector || ! texture ) DETHROW( deeInvalidParam );
	
	pWorld = world;
	pSector = sector;
	
	pOldRotation = texture->GetProjectionRotation();
	pNewRotation = newRotation;
	
	SetShortInfo( "Set Height Terrain Texture UV Rotation" );
	
	world->AddReference();
	
	pTexture = texture;
	texture->AddReference();
}

meUHTSetTexUVRotation::~meUHTSetTexUVRotation(){
	if( pTexture ) pTexture->FreeReference();
	if( pWorld ) pWorld->FreeReference();
}



// Management
///////////////


void meUHTSetTexUVRotation::Undo(){
	pTexture->SetProjectionRotation( pOldRotation );
}

void meUHTSetTexUVRotation::Redo(){
	pTexture->SetProjectionRotation( pNewRotation );
}
