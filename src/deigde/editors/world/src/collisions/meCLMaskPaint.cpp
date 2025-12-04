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

#include "meCLMaskPaint.h"
#include "../world/meWorld.h"
#include "../world/meWorldGuiParameters.h"
#include "../world/terrain/meHeightTerrain.h"
#include "../world/terrain/meHeightTerrainSector.h"
#include "../world/terrain/meHeightTerrainTexture.h"
#include "../world/terrain/meTerrainMaskImage.h"
#include "../undosys/gui/heightterrain/meUHTPaintMask.h"

#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageReference.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/common/exceptions.h>



// Defines
////////////

#define LOGSOURCE "World Editor"



// Class meCLMaskPaint
////////////////////////

// Constructor, destructor
////////////////////////////

meCLMaskPaint::meCLMaskPaint( meWorld *world ){
	if( ! world ) DETHROW( deeInvalidParam );
	
	pWorld = world;
	pSector = NULL;
	pTexture = NULL;
	
	pOldValues = NULL;
	pModifyWidth = 0;
	pModifyHeight = 0;
}

meCLMaskPaint::~meCLMaskPaint(){
	if( pOldValues ) delete [] pOldValues;
}



// Management
///////////////

void meCLMaskPaint::SetTexture( meHeightTerrainSector *sector, meHeightTerrainTexture *texture ){
	pSector = sector;
	pTexture = texture;
}

void meCLMaskPaint::SetRay( const decDVector &origin, const decVector &direction ){
	pRayOrigin = origin;
	pRayDirection = direction;
}

bool meCLMaskPaint::CanPaint() const{
	return pSector && pTexture;
}

void meCLMaskPaint::BeginSession(){
	if( pOldValues ){
		delete [] pOldValues;
		pOldValues = NULL;
	}
	pModifyWidth = 0;
	pModifyHeight = 0;
	
	pAreaGrid = decBoundary();
	
	pSessionGrid.SetZero();
}

void meCLMaskPaint::PreparePaint(){
	if( ! pSector || ! pTexture ) return;
	
	meHeightTerrain *hterrain = pWorld->GetHeightTerrain();
	float sectorDim = hterrain->GetSectorSize();
	int imageDim = hterrain->GetSectorResolution();
	const meWorldGuiParameters &guiparams = pWorld->GetGuiParameters();
	
	pDrawMode = guiparams.GetMPDrawMode();
	
	//pRadius = pWorld->GetHPRadius();
	pRadius = guiparams.GetMPRadius() * ( float )imageDim / sectorDim;
	
	pPaintRadius = ( float )imageDim * pRadius / sectorDim;
	pPaintInverseScale = sectorDim / ( float )imageDim;
	
	pHitDistance = 0.0f;
	pHasHit = false;
}

void meCLMaskPaint::Paint(){
	if( pHasHit && pSector && pTexture ){
		const decPoint &scoord = pSector->GetCoordinates();
		meHeightTerrain *hterrain = pWorld->GetHeightTerrain();
		float sectorDim = hterrain->GetSectorSize();
		int imageDim = hterrain->GetSectorResolution();
		double divX, divZ;
		
		// determine the paint location
		divX = pHitPoint.x / sectorDim + 0.5;
		divZ = 0.5 - pHitPoint.z / sectorDim;
		
		pPaintGrid.x = ( float )( ( divX - ( double )scoord.x ) * ( double )( imageDim + 1 ) );
		if( pPaintGrid.x < 0.0f ) pPaintGrid.x += sectorDim;
		pPaintGrid.y = ( float )( ( divZ - ( double )scoord.y ) * ( double )( imageDim + 1 ) );
		if( pPaintGrid.y < 0.0f ) pPaintGrid.y += sectorDim;
		
		// update the old heights
		if( ! pOldValues ){
			pSessionGrid.x = ( int )pPaintGrid.x;
			if( pSessionGrid.x < 0 ) pSessionGrid.x = 0;
			if( pSessionGrid.x >= imageDim ) pSessionGrid.x = imageDim - 1;
			
			pSessionGrid.y = ( int )pPaintGrid.y;
			if( pSessionGrid.y < 0 ) pSessionGrid.y = 0;
			if( pSessionGrid.y >= imageDim ) pSessionGrid.y = imageDim - 1;
			
//pWorld->GetLogger()->LogInfoFormat( LOGSOURCE, "[meCLMaskPaint::Paint] session: s=(%i,%i) g=(%i,%i)\n", pSessionSector.x, pSessionSector.y, pSessionGrid.x, pSessionGrid.y );
		}
		
		pUpdateOldValues( pPaintGrid, decVector2( pPaintRadius, pPaintRadius ) );
		
		if( pDrawMode == meWorldGuiParameters::empdmDraw ){
			pPaintDraw();
			
		}else{
			pPaintErase();
		}
		
// pWorld->GetLogger()->LogInfoFormat( LOGSOURCE, "[meCLMaskPaint::Paint] modified: g=(%i,%i,%i,%i)\n", pAreaModifyGrid.x1,
// pAreaModifyGrid.y1, pAreaModifyGrid.x2, pAreaModifyGrid.y2 );
	}
}

void meCLMaskPaint::EndSession(){
	// check if we have any changes at all
	if( pOldValues ){
		igdeUndo::Ref undo;
		undo.TakeOver( new meUHTPaintMask( pDrawMode, pWorld, pSector, pTexture,
			decPoint( pAreaGrid.x1, pAreaGrid.y1 ), decPoint( pModifyWidth, pModifyHeight ), pOldValues ) );
//		pWorld->GetLogger()->LogInfoFormat( LOGSOURCE, "Mask Paint: Adding Undo with %i bytes memory consumption.\n", undo->GetMemoryConsumption() );
		pWorld->GetUndoSystem()->Add( undo, false );
	}
	
	// clean up
	if( pOldValues ){
		delete [] pOldValues;
		pOldValues = NULL;
	}
	pModifyWidth = 0;
	pModifyHeight = 0;
	
	pTexture = NULL;
	pSector = NULL;
}



// Notifications
//////////////////

void meCLMaskPaint::CollisionResponse( deCollider *owner, deCollisionInfo *info ){
	if( ! pSector || ! pTexture ){
		return;
	}
	
	if( info->IsHTSector() ){
		const float distance = info->GetDistance();
		
		if( ! pHasHit || distance < pHitDistance ){
			pHasHit = true;
			pHitPoint = pRayOrigin + pRayDirection * distance;
			pHitDistance = distance;
		}
	}
}

bool meCLMaskPaint::CanHitCollider( deCollider *owner, deCollider *collider ){
	return false;
}

void meCLMaskPaint::ColliderChanged( deCollider *owner ){
}



// Private Functions
//////////////////////

void meCLMaskPaint::pPaintDraw(){
	deImage * const mask = pTexture->GetOrAddMaskImage();
	meHeightTerrain *hterrain = pWorld->GetHeightTerrain();
	int imageDim = hterrain->GetSectorResolution();
	float doubleRadius = pRadius * pRadius;
	decBoundary region;
	decVector2 center;
	int x, y, base;
	float rx, ry;
	
	region.x1 = pAreaModifyGrid.x1;
	region.y1 = pAreaModifyGrid.y1;
	region.x2 = pAreaModifyGrid.x2;
	region.y2 = pAreaModifyGrid.y2;
	
	if( region.x2 >= 0 && region.y2 >= 0 && region.x1 < imageDim && region.y1 < imageDim ){
		meTerrainMaskImage tmi( mask );
		
		if( region.x1 < 0 ) region.x1 = 0;
		if( region.y1 < 0 ) region.y1 = 0;
		if( region.x2 >= imageDim ) region.x2 = imageDim - 1;
		if( region.y2 >= imageDim ) region.y2 = imageDim - 1;
		
		center.x = pPaintGrid.x;
		center.y = pPaintGrid.y;
		
		for( y=region.y1; y<=region.y2; y++ ){
			base = imageDim * y;
			
			for( x=region.x1; x<=region.x2; x++ ){
				rx = ( float )x - center.x;
				ry = ( float )y - center.y;
				if( rx * rx + ry * ry <= doubleRadius ){
					tmi.SetMaskValueAt( base + x, 255 );
				}
			}
		}
		
		pTexture->SetMaskChanged( true );
		pTexture->NotifyTextureMaskChanged();
	}
}

void meCLMaskPaint::pPaintErase(){
	deImage * const mask = pTexture->GetOrAddMaskImage();
	meHeightTerrain *hterrain = pWorld->GetHeightTerrain();
	int imageDim = hterrain->GetSectorResolution();
	float doubleRadius = pRadius * pRadius;
	decBoundary region;
	decVector2 center;
	int x, y, base;
	float rx, ry;
	
	region.x1 = pAreaModifyGrid.x1;
	region.y1 = pAreaModifyGrid.y1;
	region.x2 = pAreaModifyGrid.x2;
	region.y2 = pAreaModifyGrid.y2;
	
	if( region.x2 >= 0 && region.y2 >= 0 && region.x1 < imageDim && region.y1 < imageDim ){
		meTerrainMaskImage tmi( mask );
		
		if( region.x1 < 0 ) region.x1 = 0;
		if( region.y1 < 0 ) region.y1 = 0;
		if( region.x2 >= imageDim ) region.x2 = imageDim - 1;
		if( region.y2 >= imageDim ) region.y2 = imageDim - 1;
		
		center.x = pPaintGrid.x;
		center.y = pPaintGrid.y;
		
		for( y=region.y1; y<=region.y2; y++ ){
			base = imageDim * y;
			
			for( x=region.x1; x<=region.x2; x++ ){
				rx = ( float )x - center.x;
				ry = ( float )y - center.y;
				if( rx * rx + ry * ry <= doubleRadius ){
					tmi.SetMaskValueAt( base + x, 0 );
				}
			}
		}
		
		pTexture->SetMaskChanged( true );
		pTexture->NotifyTextureMaskChanged();
	}
}



void meCLMaskPaint::pUpdateOldValues( const decVector2 &grid, const decVector2 &size ){
	meTerrainMaskImage tmi( pTexture->GetOrAddMaskImage() );
	meHeightTerrain *hterrain = pWorld->GetHeightTerrain();
	const int imageDim = hterrain->GetSectorResolution();
	decBoundary growMargins;
	decPoint toGrid;
	decPoint growBy;
	
//pWorld->GetLogger()->LogInfoFormat( LOGSOURCE, "[meCLMaskPaint::pGrowHeights] se=(%i,%i) gr=(%g,%g) si=(%g,%g)\n", sector.x, sector.y, grid.x, grid.y, size.x, size.y );
	// determine the covered area
	pAreaModifyGrid.x1 = ( int )floor( grid.x - size.x );
	if( pAreaModifyGrid.x1 < 0 ) pAreaModifyGrid.x1 = 0;
	
	pAreaModifyGrid.y1 = ( int )floor( grid.y - size.y );
	if( pAreaModifyGrid.y1 < 0 ) pAreaModifyGrid.y1 = 0;
	
	pAreaModifyGrid.x2 = ( int )ceil( grid.x + size.x );
	if( pAreaModifyGrid.x2 >= imageDim ) pAreaModifyGrid.x2 = imageDim - 1;
	
	pAreaModifyGrid.y2 = ( int )ceil( grid.y + size.y );
	if( pAreaModifyGrid.y2 >= imageDim ) pAreaModifyGrid.y2 = imageDim - 1;
	
	// determine the grow parameters
//pWorld->GetLogger()->LogInfoFormat( LOGSOURCE, "[meCLMaskPaint::pGrowHeights] as=(%i,%i,%i,%i) ag=(%i,%i,%i,%i)\n", pAreaModifySector.x1, pAreaModifySector.y1, pAreaModifySector.x2, pAreaModifySector.y2, pAreaModifyGrid.x1, pAreaModifyGrid.y1, pAreaModifyGrid.x2, pAreaModifyGrid.y2 );
	if( pOldValues ){
		if( pAreaModifyGrid.x1 < pAreaGrid.x1 ){
			growMargins.x1 = pAreaGrid.x1 - pAreaModifyGrid.x1;
			pAreaGrid.x1 = pAreaModifyGrid.x1;
		}
		
		if( pAreaModifyGrid.y1 < pAreaGrid.y1 ){
			growMargins.y1 = pAreaGrid.y1 - pAreaModifyGrid.y1;
			pAreaGrid.y1 = pAreaModifyGrid.y1;
		}
		
		if( pAreaModifyGrid.x2 > pAreaGrid.x2 ){
			growMargins.x2 = pAreaModifyGrid.x2 - pAreaGrid.x2;
			pAreaGrid.x2 = pAreaModifyGrid.x2;
		}
		
		if( pAreaModifyGrid.y2 > pAreaGrid.y2 ){
			growMargins.y2 = pAreaModifyGrid.y2 - pAreaGrid.y2;
			pAreaGrid.y2 = pAreaModifyGrid.y2;
		}
		
	}else{
		growMargins.x1 = 0;
		growMargins.y1 = 0;
		growMargins.x2 = pAreaModifyGrid.x2 - pAreaModifyGrid.x1 + 1;
		growMargins.y2 = pAreaModifyGrid.y2 - pAreaModifyGrid.y1 + 1;
		pAreaGrid = pAreaModifyGrid;
	}
// pWorld->GetLogger()->LogInfoFormat( LOGSOURCE, "[meCLMaskPaint::pGrowHeights] new ag=(%i,%i,%i,%i)\n", pAreaGrid.x1, pAreaGrid.y1, pAreaGrid.x2, pAreaGrid.y2 );
	
	growBy.x = growMargins.x1 + growMargins.x2;
	growBy.y = growMargins.y1 + growMargins.y2;
	
//pWorld->GetLogger()->LogInfoFormat( LOGSOURCE, "[meCLMaskPaint::pGrowHeights] gm=(%i,%i,%i,%i) gb=(%i,%i)\n", growMargins.x1, growMargins.y1, growMargins.x2, growMargins.y2, growBy.x, growBy.y );
	// grow if required preserving the old height values
	if( growBy.x > 0 || growBy.y > 0 ){
		int y, newHeight = pModifyHeight + growBy.y;
		int x, newWidth = pModifyWidth + growBy.x;
		unsigned char *oldValues = NULL;
		decBoundary retain;
		int sgx, sgy;
		
		retain.x1 = growMargins.x1;
		retain.y1 = growMargins.y1;
		retain.x2 = growMargins.x1 + pModifyWidth;
		retain.y2 = growMargins.y1 + pModifyHeight;
		
		try{
			oldValues = new unsigned char[ newWidth * newHeight ];
			if( ! oldValues ) DETHROW( deeOutOfMemory );
			
			for( y=0; y<newHeight; y++ ){
				for( x=0; x<newWidth; x++ ){
					if( x>=retain.x1 && y>=retain.y1 && x<retain.x2 && y<retain.y2 ){
						oldValues[ y * newWidth + x ] = pOldValues[ ( y - retain.y1 ) * pModifyWidth + ( x - retain.x1 ) ];
						
					}else{
						sgx = pAreaGrid.x1 + x;
						sgy = pAreaGrid.y1 + y;
						
						oldValues[ y * newWidth + x ] = tmi.GetMaskValueAt( sgy * imageDim + sgx );
					}
				}
			}
			
		}catch( const deException & ){
			if( oldValues ) delete [] oldValues;
			throw;
		}
		
		if( pOldValues ) delete [] pOldValues;
		pOldValues = oldValues;
		
		pModifyWidth = newWidth;
		pModifyHeight = newHeight;
	}
}
