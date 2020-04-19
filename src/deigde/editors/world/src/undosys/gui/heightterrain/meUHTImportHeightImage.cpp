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

#include "meUHTImportHeightImage.h"
#include "../../../world/meWorld.h"
#include "../../../world/terrain/meHeightTerrain.h"
#include "../../../world/terrain/meHeightTerrainSector.h"

#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/common/exceptions.h>



// Class meUHTImportHeightImage
/////////////////////////////////

// Constructor, destructor
////////////////////////////

meUHTImportHeightImage::meUHTImportHeightImage( meWorld *world, meHeightTerrainSector *sector, deImage *image ){
	if( ! world || ! sector || ! image ) DETHROW( deeInvalidParam );
	
	if( image->GetComponentCount() != 1 ) DETHROW( deeInvalidParam );
	
	int resolution = sector->GetHeightTerrain()->GetSectorSize();
	
	if( image->GetWidth() != resolution || image->GetHeight() != resolution ) DETHROW( deeInvalidParam );
	
	int p, pixelCount = resolution * resolution;
	
	pWorld = NULL;
	pSector = sector;
	
	pOldHeights = NULL;
	pNewHeights = NULL;
	
	SetShortInfo( "Import Height Image" );
	SetMemoryConsumption( sizeof( meUHTImportHeightImage ) + 2 * sizeof( float ) * pixelCount );
	
	try{
		// create arrays
		pOldHeights = new float[ pixelCount ];
		pNewHeights = new float[ pixelCount ];
		
		// snapshot old heights
		memcpy( pOldHeights, sector->GetHeightImage()->GetDataGrayscale32(), sizeof( float ) * pixelCount );
		
		// store new heights
		if( image->GetBitCount() == 8 ){
			sGrayscale8 *data = image->GetDataGrayscale8();
			
			for( p=0; p<pixelCount; p++ ){
				pNewHeights[ p ] = ( float )( data[ p ].value - HT_8BIT_BASE ) * HT_8BIT_PTOH;
			}
			
		}else if( image->GetBitCount() == 16 ){
			sGrayscale16 *data = image->GetDataGrayscale16();
			
			for( p=0; p<pixelCount; p++ ){
				pNewHeights[ p ] = ( float )( data[ p ].value - HT_16BIT_BASE ) * HT_16BIT_PTOH;
			}
			
		}else{ // image->GetBitCount() == 32
			memcpy( pNewHeights, image->GetDataGrayscale32(), sizeof( float ) * pixelCount );
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	
	pWorld = world;
	world->AddReference();
}

meUHTImportHeightImage::~meUHTImportHeightImage(){
	pCleanUp();
}



// Management
///////////////

void meUHTImportHeightImage::Undo(){
	pDoIt( pOldHeights );
}

void meUHTImportHeightImage::Redo(){
	pDoIt( pNewHeights );
}



// Private Functions
//////////////////////

void meUHTImportHeightImage::pCleanUp(){
	if( pNewHeights ) delete [] pNewHeights;
	if( pOldHeights ) delete [] pOldHeights;
	
	if( pWorld ) pWorld->FreeReference();
}

void meUHTImportHeightImage::pDoIt( float *heights ){
	const decPoint &scoord = pSector->GetCoordinates();
	int resolution = pSector->GetHeightTerrain()->GetSectorResolution();
	int pixelCount = resolution * resolution;
	decBoundary areaSector, areaGrid;
	
	memcpy( pSector->GetHeightImage()->GetDataGrayscale32(), heights, sizeof( float ) * pixelCount );
	pSector->SetHeightImageChanged( true );
	
	areaSector.x1 = scoord.x;
	areaSector.x2 = scoord.x;
	areaSector.y1 = scoord.y;
	areaSector.y2 = scoord.y;
	areaGrid.x1 = 0;
	areaGrid.x2 = resolution - 1;
	areaGrid.y1 = 0;
	areaGrid.y2 = resolution - 1;
	pWorld->GetHeightTerrain()->NotifyHeightsChanged( areaSector, areaGrid );
	
	pWorld->NotifyHTSHeightChanged( pSector );
}
