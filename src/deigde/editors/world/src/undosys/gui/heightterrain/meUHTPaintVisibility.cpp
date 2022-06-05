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

#include "meUHTPaintVisibility.h"
#include "../../../utils/meBitArray.h"
#include "../../../world/meWorld.h"
#include "../../../world/meWorldGuiParameters.h"
#include "../../../world/terrain/meHeightTerrain.h"
#include "../../../world/terrain/meHeightTerrainSector.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/common/exceptions.h>



// Class meUHTPaintVisibility
///////////////////////////////

// Constructor, destructor
////////////////////////////

meUHTPaintVisibility::meUHTPaintVisibility( int drawMode, meWorld *world, const decPoint &sector, const decPoint &grid,
const decPoint &size, meBitArray *oldVis ){
	if( ! world || ! oldVis ) DETHROW( deeInvalidParam );
	
	meHeightTerrain *hterrain = world->GetHeightTerrain();
	int imageDim = hterrain->GetSectorResolution();
	int pixelCount = size.x * size.y;
	int adjustX, adjustY;
	
	if( drawMode == meWorldGuiParameters::evpdmVisible ){
		SetShortInfo( "Face Visibible" );
		
	}else{
		SetShortInfo( "Face Invisible" );
	}
	
	pWorld = world;
	pSize = size;
	pOldVis = NULL;
	pNewVis = NULL;
	
	pSector.x1 = sector.x;
	pSector.y1 = sector.y;
	pGrid.x1 = grid.x;
	pGrid.y1 = grid.y;
	pGrid.x2 = grid.x + size.x;
	pGrid.y2 = grid.y + size.y;
	
	adjustX = pGrid.x2 / imageDim;
	adjustY = pGrid.y2 / imageDim;
	
	pSector.x2 = sector.x + adjustX;
	pSector.y2 = sector.y + adjustY;
	pGrid.x2 -= adjustX * imageDim;
	pGrid.y2 -= adjustY * imageDim;
	pWorld->GetLogger()->LogInfoFormat( "World Editor", "UHTPaintVisibility: s=(%i,%i,%i,%i) g=(%i,%i,%i,%i)", pSector.x1, pSector.y1, pSector.x2, pSector.y2, pGrid.x1, pGrid.y1, pGrid.x2, pGrid.y2 );
	
	SetMemoryConsumption( sizeof( meUHTPaintVisibility ) + 2 * ( pixelCount / 8 + 1 ) );
	
	try{
		pOldVis = new meBitArray( size.x, size.y );
		if( ! pOldVis ) DETHROW( deeOutOfMemory );
		
		pNewVis = new meBitArray( size.x, size.y );
		if( ! pNewVis ) DETHROW( deeOutOfMemory );
		
		pSaveVisibility();
		
		oldVis->CopyTo( *pOldVis );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

meUHTPaintVisibility::~meUHTPaintVisibility(){
	pCleanUp();
}



// Management
///////////////

void meUHTPaintVisibility::Undo(){
	pRestoreVisibility( pOldVis );
}

void meUHTPaintVisibility::Redo(){
	pRestoreVisibility( pNewVis );
}



// Private Functions
//////////////////////

void meUHTPaintVisibility::pCleanUp(){
	if( pNewVis ) delete pNewVis;
	if( pOldVis ) delete pOldVis;
}

void meUHTPaintVisibility::pSaveVisibility(){
	meHeightTerrain *hterrain = pWorld->GetHeightTerrain();
	int imageDim = hterrain->GetSectorResolution();
	meHeightTerrainSector *htsector;
	decPoint scoord;
	int sgx, sgy;
	int adjust;
	int x, y;
	
	for( y=0; y<pSize.y; y++ ){
		for( x=0; x<pSize.x; x++ ){
			sgx = pGrid.x1 + x;
			adjust = sgx / imageDim;
			scoord.x = pSector.x1 + adjust;
			sgx -= adjust * imageDim;
			
			sgy = pGrid.y1 + y;
			adjust = sgy / imageDim;
			scoord.y = pSector.y1 + adjust;
			sgy -= adjust * imageDim;
			
			htsector = hterrain->GetSectorWith( scoord );
			if( htsector ){
				pNewVis->SetValueAt( x, y, htsector->GetVisibilityFaces()->GetValueAt( sgx, sgy ) );
				
			}else{
				pNewVis->SetValueAt( x, y, true );
			}
		}
	}
}

void meUHTPaintVisibility::pRestoreVisibility( meBitArray *vis ){
	meHeightTerrain *hterrain = pWorld->GetHeightTerrain();
	int s, sectorCount = hterrain->GetSectorCount();
	int imageDim = hterrain->GetSectorResolution();
	meHeightTerrainSector *htsector;
	decPoint scoord;
	int sgx, sgy;
	int adjust;
	int x, y;
	
	for( y=0; y<pSize.y; y++ ){
		for( x=0; x<pSize.x; x++ ){
			sgx = pGrid.x1 + x;
			adjust = sgx / imageDim;
			scoord.x = pSector.x1 + adjust;
			sgx -= adjust * imageDim;
			
			sgy = pGrid.y1 + y;
			adjust = sgy / imageDim;
			scoord.y = pSector.y1 + adjust;
			sgy -= adjust * imageDim;
			
			htsector = hterrain->GetSectorWith( scoord );
			if( htsector ){
				htsector->GetVisibilityFaces()->SetValueAt( sgx, sgy, vis->GetValueAt( x, y ) );
			}
		}
	}
	
	for( s=0; s<sectorCount; s++ ){
		htsector = hterrain->GetSectorAt( s );
		const decPoint &scoord2 = htsector->GetCoordinates();
		
		if( scoord2.x >= pSector.x1 && scoord2.y >= pSector.y1 && scoord2.x <= pSector.x2 && scoord2.y <= pSector.y2 ){
			htsector->SetVisibilityChanged( true );
			
			if( htsector->GetEngineSector() ){
				htsector->UpdateVisibilitySector( htsector->GetEngineSector() );
			}
			htsector->RebuildEngineSector();
			
			pWorld->NotifyHTSVisibilityChanged( htsector );
		}
	}
}
