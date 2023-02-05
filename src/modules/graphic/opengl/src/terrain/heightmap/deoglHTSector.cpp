/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglHTSector.h"
#include "deoglRHTSector.h"
#include "deoglHeightTerrain.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>



// Class deoglHTSector
////////////////////////

// Constructor, destructor
////////////////////////////

deoglHTSector::deoglHTSector( deoglHeightTerrain &heightTerrain, const deHeightTerrainSector &sector ) :
pHeightTerrain( heightTerrain ),
pSector( sector ),
pSectorChanged( true ),
pDirtyHeights( false ){
}

deoglHTSector::~deoglHTSector(){
}



// Management
///////////////

void deoglHTSector::SyncToRender(){
	if( ! pRSector ){
		pRSector.TakeOver( new deoglRHTSector( pHeightTerrain.GetRHeightTerrain(), pSector ) );
		pSectorChanged = true;
	}
	
	if( pSectorChanged ){
		pRSector->SectorChanged( pSector );
		pSectorChanged = false;
		
		const int imageDim = pHeightTerrain.GetHeightTerrain().GetSectorResolution();
		pHeightChangeFrom.Set( 0, 0 );
		pHeightChangeTo.Set( imageDim - 1, imageDim -1 );
		pDirtyHeights = true;
	}
	
	if( pDirtyHeights ){
		pRSector->HeightChanged( pSector, pHeightChangeFrom, pHeightChangeTo );
		pDirtyHeights = false;
	}
}

void deoglHTSector::SectorChanged(){
	pSectorChanged = true;
}

void deoglHTSector::HeightChanged( const decPoint &from, const decPoint &to ){
	if( pDirtyHeights ){
		pHeightChangeFrom.SetSmallest( from );
		pHeightChangeTo.SetLargest( to );
		
	}else{
		pHeightChangeFrom = from;
		pHeightChangeTo = to;
		pDirtyHeights = true;
	}
}
