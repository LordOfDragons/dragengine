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

#include "meUHTRemoveTexture.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/terrain/meHeightTerrain.h"
#include "../../../../world/terrain/meHeightTerrainSector.h"
#include "../../../../world/terrain/meHeightTerrainTexture.h"

#include <dragengine/common/exceptions.h>



// Class meUHTRemoveTexture
/////////////////////////////

// Constructor, destructor
////////////////////////////

meUHTRemoveTexture::meUHTRemoveTexture( meWorld *world, meHeightTerrainSector *sector, meHeightTerrainTexture *texture ){
	if( ! world || ! sector || ! texture ) DETHROW( deeInvalidParam );
	
	pWorld = world;
	pSector = sector;
	
	SetShortInfo( "Height Terrain Add Texture" );
	
	world->AddReference();
	
	pTexture = texture;
	texture->AddReference();
}

meUHTRemoveTexture::~meUHTRemoveTexture(){
	if( pTexture ) pTexture->FreeReference();
	if( pWorld ) pWorld->FreeReference();
}



// Management
///////////////


void meUHTRemoveTexture::Undo(){
	pSector->AddTexture( pTexture );
	
	pWorld->NotifyHTSTextureCountChanged( pSector );
}

void meUHTRemoveTexture::Redo(){
	pSector->RemoveTexture( pTexture );
	
	pWorld->NotifyHTSTextureCountChanged( pSector );
}
