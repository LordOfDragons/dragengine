/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "meCLVisibilityPaint.h"
#include "../utils/meBitArray.h"
#include "../world/meWorld.h"
#include "../world/meWorldGuiParameters.h"
#include "../world/terrain/meHeightTerrain.h"
#include "../world/terrain/meHeightTerrainSector.h"
#include "../undosys/gui/heightterrain/meUHTPaintVisibility.h"

#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/common/exceptions.h>



// Defines
////////////

#define LOGSOURCE "World Editor"



// Class meCLVisibilityPaint
//////////////////////////////

// Constructor, destructor
////////////////////////////

meCLVisibilityPaint::meCLVisibilityPaint( meWorld *world ){
	if( ! world ) DETHROW( deeInvalidParam );
	
	pWorld = world;
	
	pOldVis = NULL;
	pModifyWidth = 0;
	pModifyHeight = 0;
}

meCLVisibilityPaint::~meCLVisibilityPaint(){
	if( pOldVis ) delete pOldVis;
}



// Management
///////////////

void meCLVisibilityPaint::SetRay( const decDVector &origin, const decVector &direction ){
	pRayOrigin = origin;
	pRayDirection = direction;
}

void meCLVisibilityPaint::BeginSession(){
	if( pOldVis ){
		delete pOldVis;
		pOldVis = NULL;
	}
	pModifyWidth = 0;
	pModifyHeight = 0;
	
	pAreaSector = decBoundary();
	pAreaGrid = decBoundary();
	
	pSessionSector.SetZero();
	pSessionGrid.SetZero();
}

void meCLVisibilityPaint::PreparePaint(){
	meHeightTerrain *hterrain = pWorld->GetHeightTerrain();
	float sectorDim = hterrain->GetSectorSize();
	int imageDim = hterrain->GetSectorResolution();
	const meWorldGuiParameters &guiparams = pWorld->GetGuiParameters();
	
	pDrawMode = guiparams.GetVPDrawMode();
	pRadius = guiparams.GetVPRadius();
	
	pPaintRadius = ( float )imageDim * pRadius / sectorDim;
	pPaintInverseScale = sectorDim / ( float )imageDim;
	
	pHitDistance = 0.0f;
	pHasHit = false;
}

void meCLVisibilityPaint::Paint(){
	if( pHasHit ){
		meHeightTerrain *hterrain = pWorld->GetHeightTerrain();
		bool visValue = pDrawMode == meWorldGuiParameters::evpdmVisible;
		float sectorDim = hterrain->GetSectorSize();
		int s, sectorCount = hterrain->GetSectorCount();
		int imageDim = hterrain->GetSectorResolution();
		meHeightTerrainSector *htsector;
		meBitArray *htsvis;
		decBoundary region;
		decVector2 center;
		double divX, divZ;
		int x, y;
		
		// determine the paint location
		divX = pHitPoint.x / sectorDim + 0.5;
		divZ = 0.5 - pHitPoint.z / sectorDim;
		
		pPaintSector.x = ( int )divX;
		pPaintSector.y = ( int )divZ;
		
		pPaintGrid.x = ( float )( ( divX - ( double )pPaintSector.x ) * ( double )( imageDim + 1 ) );
		if( pPaintGrid.x < 0.0f ) pPaintGrid.x += sectorDim;
		pPaintGrid.y = ( float )( ( divZ - ( double )pPaintSector.y ) * ( double )( imageDim + 1 ) );
		if( pPaintGrid.y < 0.0f ) pPaintGrid.y += sectorDim;
		
		// update the old visibility
		if( ! pOldVis ){
			pSessionSector = pPaintSector;
			
			pSessionGrid.x = ( int )pPaintGrid.x;
			if( pSessionGrid.x < 0 ) pSessionGrid.x = 0;
			if( pSessionGrid.x >= imageDim ) pSessionGrid.x = imageDim - 1;
			
			pSessionGrid.y = ( int )pPaintGrid.y;
			if( pSessionGrid.y < 0 ) pSessionGrid.y = 0;
			if( pSessionGrid.y >= imageDim ) pSessionGrid.y = imageDim - 1;
			
//pWorld->GetLogger()->LogInfoFormat( LOGSOURCE, "[meCLVisibilityPaint::Paint] session: s=(%i,%i) g=(%i,%i)\n", pSessionSector.x, pSessionSector.y, pSessionGrid.x, pSessionGrid.y );
		}
		
		pUpdateOldVisibility( pPaintSector, pPaintGrid, decVector2( pPaintRadius, pPaintRadius ) );
		
		// paint
		for( s=0; s<sectorCount; s++ ){
			htsector = hterrain->GetSectorAt( s );
			const decPoint &scoord = htsector->GetCoordinates();
			
	//		if( scoord.x >= pAreaModifySector.x1 && scoord.y >= pAreaModifySector.y1
	//		&&  scoord.x <= pAreaModifySector.x2 && scoord.y <= pAreaModifySector.y2 ){
				region.x1 = ( pAreaModifySector.x1 - scoord.x ) * imageDim + pAreaModifyGrid.x1;
				region.y1 = ( pAreaModifySector.y1 - scoord.y ) * imageDim + pAreaModifyGrid.y1;
				region.x2 = ( pAreaModifySector.x2 - scoord.x ) * imageDim + pAreaModifyGrid.x2;
				region.y2 = ( pAreaModifySector.y2 - scoord.y ) * imageDim + pAreaModifyGrid.y2;
				
				if( region.x2 >= 0 && region.y2 >= 0 && region.x1 < imageDim && region.y1 < imageDim ){
					if( region.x1 < 0 ) region.x1 = 0;
					if( region.y1 < 0 ) region.y1 = 0;
					if( region.x2 >= imageDim ) region.x2 = imageDim - 1;
					if( region.y2 >= imageDim ) region.y2 = imageDim - 1;
					
					center.x = ( pPaintSector.x - scoord.x ) * imageDim + pPaintGrid.x;
					center.y = ( pPaintSector.y - scoord.y ) * imageDim + pPaintGrid.y;
					
					htsvis = htsector->GetVisibilityFaces();
					
					for( y=region.y1; y<=region.y2; y++ ){
						for( x=region.x1; x<=region.x2; x++ ){
							htsvis->SetValueAt( x, y, visValue );
						}
					}
					
					htsector->SetVisibilityChanged( true );
					
					if( htsector->GetEngineSector() ){
						htsector->UpdateVisibilitySector( htsector->GetEngineSector() );
					}
					htsector->RebuildEngineSector();
					
					pWorld->NotifyHTSVisibilityChanged( htsector );
				}
	//		}
		}
		
// pWorld->GetLogger()->LogInfoFormat( LOGSOURCE, "[meCLVisibilityPaint::Paint] modified: s=(%i,%i,%i,%i) g=(%i,%i,%i,%i)\n", pAreaModifySector.x1, pAreaModifySector.y1,
// pAreaModifySector.x2, pAreaModifySector.y2, pAreaModifyGrid.x1, pAreaModifyGrid.y1, pAreaModifyGrid.x2, pAreaModifyGrid.y2 );
		
		// notify
		hterrain->NotifyHeightsChanged( pAreaModifySector, pAreaModifyGrid );
	}
}

void meCLVisibilityPaint::EndSession(){
	// check if we have any changes at all
	if( pOldVis ){
		meUHTPaintVisibility::Ref undo(meUHTPaintVisibility::Ref::New(new meUHTPaintVisibility( pDrawMode, pWorld, decPoint( pAreaSector.x1, pAreaSector.y1 ),
			decPoint( pAreaGrid.x1, pAreaGrid.y1 ), decPoint( pModifyWidth, pModifyHeight ), pOldVis )));
// 		pWorld->GetLogger()->LogInfoFormat( LOGSOURCE, "Visibility Paint: Adding Undo with %i bytes memory consumption.\n", undo->GetMemoryConsumption() );
		pWorld->GetUndoSystem()->Add( undo, false );
	}
	
	// clean up
	if( pOldVis ){
		delete pOldVis;
		pOldVis = NULL;
	}
	pModifyWidth = 0;
	pModifyHeight = 0;
}



// Notifications
//////////////////

void meCLVisibilityPaint::CollisionResponse( deCollider *owner, deCollisionInfo *info ){
	if( info->IsHTSector() ){
		const float distance = info->GetDistance();
		
		if( ! pHasHit || distance < pHitDistance ){
			pHasHit = true;
			pHitPoint = pRayOrigin + pRayDirection * distance;
			pHitDistance = distance;
		}
	}
}

bool meCLVisibilityPaint::CanHitCollider( deCollider *owner, deCollider *collider ){
	return false;
}

void meCLVisibilityPaint::ColliderChanged( deCollider *owner ){
}



// Private Functions
//////////////////////

void meCLVisibilityPaint::pUpdateOldVisibility( const decPoint &sector, const decVector2 &grid, const decVector2 &size ){
	meHeightTerrain *hterrain = pWorld->GetHeightTerrain();
	int imageDim = hterrain->GetSectorResolution();
	decBoundary growMargins;
	decPoint toGrid;
	decPoint growBy;
	int adjust;
	
//pWorld->GetLogger()->LogInfoFormat( LOGSOURCE, "[meCLVisibilityPaint::pGrowHeights] se=(%i,%i) gr=(%g,%g) si=(%g,%g)\n", sector.x, sector.y, grid.x, grid.y, size.x, size.y );
	// determine the covered area
	pAreaModifySector.x1 = sector.x;
	pAreaModifySector.y1 = sector.y;
	pAreaModifySector.x2 = sector.x;
	pAreaModifySector.y2 = sector.y;
	
	pAreaModifyGrid.x1 = ( int )floor( grid.x - size.x );
	if( pAreaModifyGrid.x1 < 0 ){
		adjust = 1 - ( pAreaModifyGrid.x1 + 1 ) / imageDim;
		pAreaModifySector.x1 -= adjust;
		pAreaModifyGrid.x1 += adjust * imageDim;
	}
	
	pAreaModifyGrid.y1 = ( int )floor( grid.y - size.y );
	if( pAreaModifyGrid.y1 < 0 ){
		adjust = 1 - ( pAreaModifyGrid.y1 + 1 ) / imageDim;
		pAreaModifySector.y1 -= adjust;
		pAreaModifyGrid.y1 += adjust * imageDim;
	}
	
	pAreaModifyGrid.x2 = ( int )ceil( grid.x + size.x );
	if( pAreaModifyGrid.x2 >= imageDim ){
		adjust = pAreaModifyGrid.x2 / imageDim;
		pAreaModifySector.x2 += adjust;
		pAreaModifyGrid.x2 -= adjust * imageDim;
	}
	
	pAreaModifyGrid.y2 = ( int )ceil( grid.y + size.y );
	if( pAreaModifyGrid.y2 >= imageDim ){
		adjust = pAreaModifyGrid.y2 / imageDim;
		pAreaModifySector.y2 += adjust;
		pAreaModifyGrid.y2 -= adjust * imageDim;
	}
	
	// determine the grow parameters
//pWorld->GetLogger()->LogInfoFormat( LOGSOURCE, "[meCLVisibilityPaint::pGrowHeights] as=(%i,%i,%i,%i) ag=(%i,%i,%i,%i)\n", pAreaModifySector.x1, pAreaModifySector.y1, pAreaModifySector.x2, pAreaModifySector.y2, pAreaModifyGrid.x1, pAreaModifyGrid.y1, pAreaModifyGrid.x2, pAreaModifyGrid.y2 );
	if( pOldVis ){
		if( pAreaModifySector.x1 < pAreaSector.x1 || ( pAreaModifySector.x1 == pAreaSector.x1 && pAreaModifyGrid.x1 < pAreaGrid.x1 ) ){
			growMargins.x1 = ( pAreaSector.x1 - pAreaModifySector.x1 ) * imageDim + ( pAreaGrid.x1 - pAreaModifyGrid.x1 );
			
			pAreaSector.x1 = pAreaModifySector.x1;
			pAreaGrid.x1 = pAreaModifyGrid.x1;
		}
		
		if( pAreaModifySector.y1 < pAreaSector.y1 || ( pAreaModifySector.y1 == pAreaSector.y1 && pAreaModifyGrid.y1 < pAreaGrid.y1 ) ){
			growMargins.y1 = ( pAreaSector.y1 - pAreaModifySector.y1 ) * imageDim + ( pAreaGrid.y1 - pAreaModifyGrid.y1 );
			
			pAreaSector.y1 = pAreaModifySector.y1;
			pAreaGrid.y1 = pAreaModifyGrid.y1;
		}
		
		if( pAreaModifySector.x2 > pAreaSector.x2 || ( pAreaModifySector.x2 == pAreaSector.x2 && pAreaModifyGrid.x2 > pAreaGrid.x2 ) ){
			growMargins.x2 = ( pAreaModifySector.x2 - pAreaSector.x2 ) * imageDim + ( pAreaModifyGrid.x2 - pAreaGrid.x2 );
			
			pAreaSector.x2 = pAreaModifySector.x2;
			pAreaGrid.x2 = pAreaModifyGrid.x2;
		}
		if( pAreaModifySector.y2 > pAreaSector.y2 || ( pAreaModifySector.y2 == pAreaSector.y2 && pAreaModifyGrid.y2 > pAreaGrid.y2 ) ){
			growMargins.y2 = ( pAreaModifySector.y2 - pAreaSector.y2 ) * imageDim + ( pAreaModifyGrid.y2 - pAreaGrid.y2 );
			
			pAreaSector.y2 = pAreaModifySector.y2;
			pAreaGrid.y2 = pAreaModifyGrid.y2;
		}
		
	}else{
		growMargins.x1 = 0;
		growMargins.y1 = 0;
		growMargins.x2 = ( pAreaModifySector.x2 - pAreaModifySector.x1 ) * imageDim + ( pAreaModifyGrid.x2 - pAreaModifyGrid.x1 ) + 1;
		growMargins.y2 = ( pAreaModifySector.y2 - pAreaModifySector.y1 ) * imageDim + ( pAreaModifyGrid.y2 - pAreaModifyGrid.y1 ) + 1;
		
		pAreaSector = pAreaModifySector;
		pAreaGrid = pAreaModifyGrid;
	}
pWorld->GetLogger()->LogInfoFormat( LOGSOURCE, "[meCLVisibilityPaint::pGrowHeights] new as=(%i,%i,%i,%i) ag=(%i,%i,%i,%i)\n", pAreaSector.x1, pAreaSector.y1, pAreaSector.x2, pAreaSector.y2, pAreaGrid.x1, pAreaGrid.y1, pAreaGrid.x2, pAreaGrid.y2 );
	
	growBy.x = growMargins.x1 + growMargins.x2;
	growBy.y = growMargins.y1 + growMargins.y2;
	
//pWorld->GetLogger()->LogInfoFormat( LOGSOURCE, "[meCLVisibilityPaint::pGrowHeights] gm=(%i,%i,%i,%i) gb=(%i,%i)\n", growMargins.x1, growMargins.y1, growMargins.x2, growMargins.y2, growBy.x, growBy.y );
	// grow if required preserving the old height values
	if( growBy.x > 0 || growBy.y > 0 ){
		int y, newHeight = pModifyHeight + growBy.y;
		int x, newWidth = pModifyWidth + growBy.x;
		meHeightTerrainSector *htsector;
		meBitArray *oldvis = NULL;
		decBoundary retain;
		decPoint scoord;
		int sgx, sgy;
		
		retain.x1 = growMargins.x1;
		retain.y1 = growMargins.y1;
		retain.x2 = growMargins.x1 + pModifyWidth;
		retain.y2 = growMargins.y1 + pModifyHeight;
		
		try{
			oldvis = new meBitArray( newWidth, newHeight );
			if( ! oldvis ) DETHROW( deeOutOfMemory );
			
			for( y=0; y<newHeight; y++ ){
				for( x=0; x<newWidth; x++ ){
					if( x>=retain.x1 && y>=retain.y1 && x<retain.x2 && y<retain.y2 ){
						oldvis->SetValueAt( x, y, pOldVis->GetValueAt( x - retain.x1, y - retain.y1 ) );
						
					}else{
						sgx = pAreaGrid.x1 + x;
						adjust = sgx / imageDim;
						scoord.x = pAreaSector.x1 + adjust;
						sgx -= adjust * imageDim;
						
						sgy = pAreaGrid.y1 + y;
						adjust = sgy / imageDim;
						scoord.y = pAreaSector.y1 + adjust;
						sgy -= adjust * imageDim;
						
						htsector = hterrain->GetSectorWith( scoord );
						if( htsector ){
							oldvis->SetValueAt( x, y, htsector->GetVisibilityFaces()->GetValueAt( sgx, sgy ) );
							
						}else{
							oldvis->SetValueAt( x, y, true );
						}
					}
				}
			}
			
		}catch( const deException & ){
			if( oldvis ) delete oldvis;
			throw;
		}
		
		if( pOldVis ) delete pOldVis;
		pOldVis = oldvis;
		
		pModifyWidth = newWidth;
		pModifyHeight = newHeight;
	}
}
