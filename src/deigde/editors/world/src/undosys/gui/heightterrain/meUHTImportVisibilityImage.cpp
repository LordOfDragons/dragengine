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

#include "meUHTImportVisibilityImage.h"
#include "../../../utils/meBitArray.h"
#include "../../../world/meWorld.h"
#include "../../../world/terrain/meHeightTerrain.h"
#include "../../../world/terrain/meHeightTerrainSector.h"

#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/common/exceptions.h>



// Class meUHTImportVisibilityImage
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

meUHTImportVisibilityImage::meUHTImportVisibilityImage( meWorld *world, meHeightTerrainSector *sector, deImage *image ){
	if( ! world || ! sector || ! image ) DETHROW( deeInvalidParam );
	
	if( image->GetComponentCount() != 1 ) DETHROW( deeInvalidParam );
	
	int resolution = sector->GetHeightTerrain()->GetSectorResolution();
	
	if( image->GetWidth() != resolution || image->GetHeight() != resolution ) DETHROW( deeInvalidParam );
	
	int x, y, pixelCount = resolution * resolution;
	
	pWorld = NULL;
	pSector = sector;
	
	pOldVis = NULL;
	pNewVis = NULL;
	
	SetShortInfo( "Import Visibility Image" );
	SetMemoryConsumption( sizeof( meUHTImportVisibilityImage ) + 2 * ( pixelCount / 8 + 1 ) );
	
	try{
		// create arrays
		pOldVis = new meBitArray( resolution, resolution );
		if( ! pOldVis ) DETHROW( deeOutOfMemory );
		
		pNewVis = new meBitArray( resolution, resolution );
		if( ! pNewVis ) DETHROW( deeOutOfMemory );
		
		// snapshot old heights
		sector->GetVisibilityFaces()->CopyTo( *pOldVis );
		
		// store new heights
		if( image->GetBitCount() == 8 ){
			sGrayscale8 *data = image->GetDataGrayscale8();
			
			for( y=0; y<resolution; y++ ){
				for( x=0; x<resolution; x++ ){
					pNewVis->SetValueAt( x, y, data[ y * resolution + x ].value > HT_8BIT_BASE );
				}
			}
			
		}else if( image->GetBitCount() == 16 ){
			sGrayscale16 *data = image->GetDataGrayscale16();
			
			for( y=0; y<resolution; y++ ){
				for( x=0; x<resolution; x++ ){
					pNewVis->SetValueAt( x, y, data[ y * resolution + x ].value > HT_16BIT_BASE );
				}
			}
			
		}else{ // image->GetBitCount() == 32
			sGrayscale32 *data = image->GetDataGrayscale32();
			
			for( y=0; y<resolution; y++ ){
				for( x=0; x<resolution; x++ ){
					pNewVis->SetValueAt( x, y, data[ y * resolution + x ].value > 0.0f );
				}
			}
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	
	pWorld = world;
	world->AddReference();
}

meUHTImportVisibilityImage::~meUHTImportVisibilityImage(){
	pCleanUp();
}



// Management
///////////////

void meUHTImportVisibilityImage::Undo(){
	pDoIt( pOldVis );
}

void meUHTImportVisibilityImage::Redo(){
	pDoIt( pNewVis );
}



// Private Functions
//////////////////////

void meUHTImportVisibilityImage::pCleanUp(){
	if( pNewVis ) delete pNewVis;
	if( pOldVis ) delete pOldVis;
	
	if( pWorld ) pWorld->FreeReference();
}

void meUHTImportVisibilityImage::pDoIt( meBitArray *vis ){
	vis->CopyTo( *pSector->GetVisibilityFaces() );
	pSector->SetVisibilityChanged( true );
	
	if( pSector->GetEngineSector() ){
		pSector->UpdateVisibilitySector( pSector->GetEngineSector() );
	}
	pSector->RebuildEngineSector();
	
	pWorld->NotifyHTSVisibilityChanged( pSector );
}
