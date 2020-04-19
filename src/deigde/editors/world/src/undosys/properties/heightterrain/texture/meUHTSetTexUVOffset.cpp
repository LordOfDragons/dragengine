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

#include "meUHTSetTexUVOffset.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/terrain/meHeightTerrain.h"
#include "../../../../world/terrain/meHeightTerrainSector.h"
#include "../../../../world/terrain/meHeightTerrainTexture.h"

#include <dragengine/common/exceptions.h>



// Class meUHTSetTexUVOffset
/////////////////////////

// Constructor, destructor
////////////////////////////

meUHTSetTexUVOffset::meUHTSetTexUVOffset( meWorld *world, meHeightTerrainSector *sector, meHeightTerrainTexture *texture, const decVector2 &newOffset ){
	if( ! world || ! sector || ! texture ) DETHROW( deeInvalidParam );
	
	pWorld = world;
	pSector = sector;
	
	pOldOffset.x = texture->GetProjectionOffsetU();
	pOldOffset.y = texture->GetProjectionOffsetV();
	pNewOffset = newOffset;
	
	SetShortInfo( "Set Height Terrain Texture UV Offset" );
	
	world->AddReference();
	
	pTexture = texture;
	texture->AddReference();
}

meUHTSetTexUVOffset::~meUHTSetTexUVOffset(){
	if( pTexture ) pTexture->FreeReference();
	if( pWorld ) pWorld->FreeReference();
}



// Management
///////////////

void meUHTSetTexUVOffset::Undo(){
	pTexture->SetProjectionOffsetU( pOldOffset.x );
	pTexture->SetProjectionOffsetV( pOldOffset.y );
}

void meUHTSetTexUVOffset::Redo(){
	pTexture->SetProjectionOffsetU( pNewOffset.x );
	pTexture->SetProjectionOffsetV( pNewOffset.y );
}
