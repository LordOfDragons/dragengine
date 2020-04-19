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

#include "meUHTVLayerUp.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/terrain/meHeightTerrain.h"
#include "../../../../world/terrain/meHeightTerrain.h"
#include "../../../../world/heightterrain/meHTVegetationLayer.h"

#include <dragengine/common/exceptions.h>



// Class meUHTVLayerUp
////////////////////////

// Constructor, destructor
////////////////////////////

meUHTVLayerUp::meUHTVLayerUp( meWorld *world, meHeightTerrain *heightTerrain, meHTVegetationLayer *vlayer ){
	if( ! world || ! heightTerrain || ! vlayer ) DETHROW( deeInvalidParam );
	
	pWorld = world;
	pHeightTerrain = heightTerrain;
	
	pIndex = heightTerrain->IndexOfVLayer( vlayer );
	if( pIndex < 1 ) DETHROW( deeInvalidParam );
	
	SetShortInfo( "Move Height Terrain Vegetation Layer Up" );
	
	world->AddReference();
	
	pVLayer = vlayer;
	vlayer->AddReference();
}

meUHTVLayerUp::~meUHTVLayerUp(){
	if( pVLayer ) pVLayer->FreeReference();
	if( pWorld ) pWorld->FreeReference();
}



// Management
///////////////

void meUHTVLayerUp::Undo(){
	pHeightTerrain->MoveVLayer( pVLayer, pIndex );
}

void meUHTVLayerUp::Redo(){
	pHeightTerrain->MoveVLayer( pVLayer, pIndex - 1 );
}
