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

#include "meCLHeightPaint.h"
#include "../world/meWorld.h"
#include "../world/meWorldGuiParameters.h"
#include "../world/terrain/meHeightTerrain.h"
#include "../world/terrain/meHeightTerrainSector.h"
#include "../undosys/gui/terrain/meUHTPaintHeight.h"

#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/collider/deCollisionInfo.h>



// Definitions
////////////////

#define LOGSOURCE "World Editor"



// Class meCLHeightPaint
//////////////////////////

// Constructor, destructor
////////////////////////////

meCLHeightPaint::meCLHeightPaint( meWorld *world ){
	if( ! world ) DETHROW( deeInvalidParam );
	
	pWorld = world;
	
	pOldHeights = NULL;
	pModifyWidth = 0;
	pModifyHeight = 0;
	
	float factor = 1.0f / 16.0f;
	pGaussKernel3x3[ 0 ][ 0 ] = factor;
	pGaussKernel3x3[ 0 ][ 1 ] = factor * 2.0f;
	pGaussKernel3x3[ 0 ][ 2 ] = factor;
	pGaussKernel3x3[ 1 ][ 0 ] = factor * 2.0f;
	pGaussKernel3x3[ 1 ][ 1 ] = factor * 4.0f;
	pGaussKernel3x3[ 1 ][ 2 ] = factor * 2.0f;
	pGaussKernel3x3[ 2 ][ 0 ] = factor;
	pGaussKernel3x3[ 2 ][ 1 ] = factor * 2.0f;
	pGaussKernel3x3[ 2 ][ 2 ] = factor;
}

meCLHeightPaint::~meCLHeightPaint(){
	if( pOldHeights ) delete [] pOldHeights;
}



// Management
///////////////

void meCLHeightPaint::SetRay( const decDVector &origin, const decVector &direction ){
	pRayOrigin = origin;
	pRayDirection = direction;
}

void meCLHeightPaint::BeginSession(){
	if( pOldHeights ){
		delete [] pOldHeights;
		pOldHeights = NULL;
	}
	pModifyWidth = 0;
	pModifyHeight = 0;
	
	pAreaSector = decBoundary();
	pAreaGrid = decBoundary();
	
	pSessionSector.SetZero();
	pSessionGrid.SetZero();
}

void meCLHeightPaint::PreparePaint(){
	meHeightTerrain *hterrain = pWorld->GetHeightTerrain();
	float sectorDim = hterrain->GetSectorSize();
	int imageDim = hterrain->GetSectorResolution();
	const meWorldGuiParameters &guiparams = pWorld->GetGuiParameters();
	
	pDrawMode = guiparams.GetHPDrawMode();
	
	pRadius = guiparams.GetHPRadius();
	
	pPaintRadius = ( float )imageDim * pRadius / sectorDim;
	pPaintInverseScale = sectorDim / ( float )imageDim;
	
	pCalculateGaussParameter();
	
	if( pDrawMode == meWorldGuiParameters::ehpdmRaise || pDrawMode == meWorldGuiParameters::ehpdmLower ){
		pStrength = guiparams.GetHPDrawStrength();
		
	}else if( pDrawMode == meWorldGuiParameters::ehpdmLevel ){
		pStrength = guiparams.GetHPLevelStrength();
		
	}else{
		pStrength = guiparams.GetHPSmoothStrength();
	}
	
	pHitDistance = 0.0f;
	pHasHit = false;
}

void meCLHeightPaint::Paint( float elapsed ){
	if( pHasHit ){
		meHeightTerrain *hterrain = pWorld->GetHeightTerrain();
		float sectorDim = hterrain->GetSectorSize();
		int imageDim = hterrain->GetSectorResolution();
		double divX, divZ;
		
		pPaintStrength = pStrength * elapsed;
		
		// determine the paint location
		divX = pHitPoint.x / sectorDim + 0.5;
		divZ = 0.5 - pHitPoint.z / sectorDim;
		
		pPaintSector.x = ( int )divX;
		pPaintSector.y = ( int )divZ;
		
		pPaintGrid.x = ( float )( ( divX - ( double )pPaintSector.x ) * ( double )( imageDim + 1 ) );
		if( pPaintGrid.x < 0.0f ) pPaintGrid.x += sectorDim;
		pPaintGrid.y = ( float )( ( divZ - ( double )pPaintSector.y ) * ( double )( imageDim + 1 ) );
		if( pPaintGrid.y < 0.0f ) pPaintGrid.y += sectorDim;
		
		// update the old heights
		if( ! pOldHeights ){
			pSessionSector = pPaintSector;
			
			pSessionGrid.x = ( int )pPaintGrid.x;
			if( pSessionGrid.x < 0 ) pSessionGrid.x = 0;
			if( pSessionGrid.x >= imageDim ) pSessionGrid.x = imageDim - 1;
			
			pSessionGrid.y = ( int )pPaintGrid.y;
			if( pSessionGrid.y < 0 ) pSessionGrid.y = 0;
			if( pSessionGrid.y >= imageDim ) pSessionGrid.y = imageDim - 1;
			
//pWorld->GetLogger()->LogInfoFormat( LOGSOURCE, "[meCLHeightPaint::Paint] session: s=(%i,%i) g=(%i,%i)\n", pSessionSector.x, pSessionSector.y, pSessionGrid.x, pSessionGrid.y );
		}
		
		pUpdateOldHeights( pPaintSector, pPaintGrid, decVector2( pPaintRadius, pPaintRadius ) );
		
		if( pDrawMode == meWorldGuiParameters::ehpdmRaise ){
			pPaintRaise();
			
		}else if( pDrawMode == meWorldGuiParameters::ehpdmLower ){
			pPaintLower();
			
		}else if( pDrawMode == meWorldGuiParameters::ehpdmLevel ){
			pPaintLevel();
			
		}else{
			pPaintSmooth();
		}
		
// pWorld->GetLogger()->LogInfoFormat( LOGSOURCE, "[meCLHeightPaint::Paint] modified: s=(%i,%i,%i,%i) g=(%i,%i,%i,%i)\n", pAreaModifySector.x1, pAreaModifySector.y1,
// pAreaModifySector.x2, pAreaModifySector.y2, pAreaModifyGrid.x1, pAreaModifyGrid.y1, pAreaModifyGrid.x2, pAreaModifyGrid.y2 );
		
		hterrain->NotifyHeightsChanged( pAreaModifySector, pAreaModifyGrid );
	}
}

void meCLHeightPaint::EndSession(){
	// check if we have any changes at all
	if( pOldHeights ){
		meUHTPaintHeight::Ref undo(meUHTPaintHeight::Ref::New(new meUHTPaintHeight( pDrawMode, pWorld, decPoint( pAreaSector.x1, pAreaSector.y1 ),
			decPoint( pAreaGrid.x1, pAreaGrid.y1 ), decPoint( pModifyWidth, pModifyHeight ), pOldHeights )));
//		pWorld->GetLogger()->LogInfoFormat( LOGSOURCE, "Height Paint: Adding Undo with %i bytes memory consumption.\n", undo->GetMemoryConsumption() );
		pWorld->GetUndoSystem()->Add( undo, false );
	}
	
	// clean up
	if( pOldHeights ){
		delete [] pOldHeights;
		pOldHeights = NULL;
	}
	pModifyWidth = 0;
	pModifyHeight = 0;
}



// Notifications
//////////////////

void meCLHeightPaint::CollisionResponse( deCollider *owner, deCollisionInfo *info ){
	if( info->IsHTSector() ){
		const float distance = info->GetDistance();
		
		if( ! pHasHit || distance < pHitDistance ){
			pHasHit = true;
			pHitPoint = pRayOrigin + pRayDirection * distance;
			pHitDistance = distance;
		}
	}
}

bool meCLHeightPaint::CanHitCollider( deCollider *owner, deCollider *collider ){
	return false;
}

void meCLHeightPaint::ColliderChanged( deCollider *owner ){
}



// Private Functions
//////////////////////

void meCLHeightPaint::pCalculateGaussParameter(){
	// gauss is: 1/(2*pi*sigma^2) * e^(-(x^2 + y^2)/(2*sigma^2))
	// since we want gauss(0,0) to be 'strength' the constant before e^x is 'strength'
	// param = -1/(2*sigma^2)
	// 'radius' is assumed to be 3*sigma
	
	float sigma = pPaintRadius / 3.0f;
	
	pGaussParam = -1.0f / ( 2.0f * sigma * sigma );
}

float meCLHeightPaint::pGauss( float x, float y ){
	return pPaintStrength * expf( pGaussParam * ( x * x + y * y ) );
}



void meCLHeightPaint::pPaintRaise(){
	meHeightTerrain *hterrain = pWorld->GetHeightTerrain();
	int imageDim = hterrain->GetSectorResolution();
	int s, sectorCount = hterrain->GetSectorCount();
	sGrayscale32 *pixels;
	decBoundary region;
	decVector2 center;
	float heightScale;
	int x, y, base;
	
	for( s=0; s<sectorCount; s++ ){
		meHeightTerrainSector &htsector = *hterrain->GetSectorAt( s );
		const decPoint &scoord = htsector.GetCoordinates();
		
//		if( scoord.x >= pAreaModifySector.x1 && scoord.y >= pAreaModifySector.y1
//		&&  scoord.x <= pAreaModifySector.x2 && scoord.y <= pAreaModifySector.y2 ){
			region.x1 = ( pAreaModifySector.x1 - scoord.x ) * imageDim + pAreaModifyGrid.x1;
			region.y1 = ( pAreaModifySector.y1 - scoord.y ) * imageDim + pAreaModifyGrid.y1;
			region.x2 = ( pAreaModifySector.x2 - scoord.x ) * imageDim + pAreaModifyGrid.x2;
			region.y2 = ( pAreaModifySector.y2 - scoord.y ) * imageDim + pAreaModifyGrid.y2;
			
			if( region.x2 >= 0 && region.y2 >= 0 && region.x1 < imageDim && region.y1 < imageDim ){
				const bool clampValues = htsector.DoClampValues();
				
				if( region.x1 < 0 ){
					region.x1 = 0;
				}
				if( region.y1 < 0 ){
					region.y1 = 0;
				}
				if( region.x2 >= imageDim ){
					region.x2 = imageDim - 1;
				}
				if( region.y2 >= imageDim ){
					region.y2 = imageDim - 1;
				}
				
				center.x = ( pPaintSector.x - scoord.x ) * imageDim + pPaintGrid.x;
				center.y = ( pPaintSector.y - scoord.y ) * imageDim + pPaintGrid.y;
				
				pixels = htsector.GetHeightImage()->GetDataGrayscale32();
				heightScale = 1.0f / htsector.GetHeightTerrain()->GetHeightScaling();
				
				for( y=region.y1; y<=region.y2; y++ ){
					base = imageDim * y;
					
					for( x=region.x1; x<=region.x2; x++ ){
						pixels[ base + x ].value += pGauss( ( float )x - center.x, ( float )y - center.y ) * heightScale;
						
						if( clampValues ){
							if( pixels[ base + x ].value < -0.5f ){
								pixels[ base + x ].value = -0.5f;
								
							}else if( pixels[ base + x ].value > 0.5f ){
								pixels[ base + x ].value = 0.5f;
							}
						}
					}
				}
				
				htsector.SetHeightImageChanged( true );
				pWorld->NotifyHTSHeightChanged( &htsector );
			}
//		}
	}
}

void meCLHeightPaint::pPaintLower(){
	meHeightTerrain *hterrain = pWorld->GetHeightTerrain();
	int imageDim = hterrain->GetSectorResolution();
	int s, sectorCount = hterrain->GetSectorCount();
	sGrayscale32 *pixels;
	decBoundary region;
	decVector2 center;
	float heightScale;
	int x, y, base;
	
	for( s=0; s<sectorCount; s++ ){
		meHeightTerrainSector &htsector = *hterrain->GetSectorAt( s );
		const decPoint &scoord = htsector.GetCoordinates();
		
//		if( scoord.x >= pAreaModifySector.x1 && scoord.y >= pAreaModifySector.y1
//		&&  scoord.x <= pAreaModifySector.x2 && scoord.y <= pAreaModifySector.y2 ){
			region.x1 = ( pAreaModifySector.x1 - scoord.x ) * imageDim + pAreaModifyGrid.x1;
			region.y1 = ( pAreaModifySector.y1 - scoord.y ) * imageDim + pAreaModifyGrid.y1;
			region.x2 = ( pAreaModifySector.x2 - scoord.x ) * imageDim + pAreaModifyGrid.x2;
			region.y2 = ( pAreaModifySector.y2 - scoord.y ) * imageDim + pAreaModifyGrid.y2;
			
			if( region.x2 >= 0 && region.y2 >= 0 && region.x1 < imageDim && region.y1 < imageDim ){
				const bool clampValues = htsector.DoClampValues();
				
				if( region.x1 < 0 ){
					region.x1 = 0;
				}
				if( region.y1 < 0 ){
					region.y1 = 0;
				}
				if( region.x2 >= imageDim ){
					region.x2 = imageDim - 1;
				}
				if( region.y2 >= imageDim ){
					region.y2 = imageDim - 1;
				}
				
				center.x = ( pPaintSector.x - scoord.x ) * imageDim + pPaintGrid.x;
				center.y = ( pPaintSector.y - scoord.y ) * imageDim + pPaintGrid.y;
				
				pixels = htsector.GetHeightImage()->GetDataGrayscale32();
				heightScale = 1.0f / htsector.GetHeightTerrain()->GetHeightScaling();
				
				for( y=region.y1; y<=region.y2; y++ ){
					base = imageDim * y;
					
					for( x=region.x1; x<=region.x2; x++ ){
						pixels[ base + x ].value -= pGauss( ( float )x - center.x, ( float )y - center.y ) * heightScale;
						
						if( clampValues ){
							if( pixels[ base + x ].value < -0.5f ){
								pixels[ base + x ].value = -0.5f;
								
							}else if( pixels[ base + x ].value > 0.5f ){
								pixels[ base + x ].value = 0.5f;
							}
						}
					}
				}
				
				htsector.SetHeightImageChanged( true );
				pWorld->NotifyHTSHeightChanged( &htsector );
			}
//		}
	}
}

void meCLHeightPaint::pPaintLevel(){
	meHeightTerrain *hterrain = pWorld->GetHeightTerrain();
	int imageDim = hterrain->GetSectorResolution();
	int s, sectorCount = hterrain->GetSectorCount();
	sGrayscale32 *pixels;
	float level, weight;
	decBoundary region;
	decVector2 center;
	float heightScale;
	int x, y, base;
	
	// determine the center point
	meHeightTerrainSector *htsector = hterrain->GetSectorWith( pPaintSector );
	if( htsector ){
		pixels = htsector->GetHeightImage()->GetDataGrayscale32();
		heightScale = htsector->GetHeightTerrain()->GetHeightScaling();
		
		// TODO get the box average of the four involved pixels instead of this hack
		level = pixels[ imageDim * ( int )floorf( pPaintGrid.y ) + ( int )floorf( pPaintGrid.x ) ].value * heightScale;
		
	}else{
		return; // no useful level value here. solution would be to use the last one
	}
	
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
				const bool clampValues = htsector->DoClampValues();
				
				if( region.x1 < 0 ){
					region.x1 = 0;
				}
				if( region.y1 < 0 ){
					region.y1 = 0;
				}
				if( region.x2 >= imageDim ){
					region.x2 = imageDim - 1;
				}
				if( region.y2 >= imageDim ){
					region.y2 = imageDim - 1;
				}
				
				center.x = ( pPaintSector.x - scoord.x ) * imageDim + pPaintGrid.x;
				center.y = ( pPaintSector.y - scoord.y ) * imageDim + pPaintGrid.y;
				
				pixels = htsector->GetHeightImage()->GetDataGrayscale32();
				heightScale = htsector->GetHeightTerrain()->GetHeightScaling();
				
				for( y=region.y1; y<=region.y2; y++ ){
					base = imageDim * y;
					
					for( x=region.x1; x<=region.x2; x++ ){
						weight = pGauss( ( float )x - center.x, ( float )y - center.y );
						pixels[ base + x ].value = ( pixels[ base + x ].value * heightScale * ( 1.0f - weight ) + level * weight ) / heightScale;
						
						if( clampValues ){
							if( pixels[ base + x ].value < -0.5f ){
								pixels[ base + x ].value = -0.5f;
								
							}else if( pixels[ base + x ].value > 0.5f ){
								pixels[ base + x ].value = 0.5f;
							}
						}
					}
				}
				
				htsector->SetHeightImageChanged( true );
				pWorld->NotifyHTSHeightChanged( htsector );
			}
//		}
	}
}

void meCLHeightPaint::pPaintSmooth(){
	meHeightTerrain *hterrain = pWorld->GetHeightTerrain();
	int imageDim = hterrain->GetSectorResolution();
	int s, sectorCount = hterrain->GetSectorCount();
	meHeightTerrainSector *htsector;
	float valueMatrix[ 9 ];
	sGrayscale32 *pixels;
	decBoundary region;
	decVector2 center;
	//float heightScale;
	int x, y, base;
	float average;
	float weight;
	
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
				const bool clampValues = htsector->DoClampValues();
				
				if( region.x1 < 0 ){
					region.x1 = 0;
				}
				if( region.y1 < 0 ){
					region.y1 = 0;
				}
				if( region.x2 >= imageDim ){
					region.x2 = imageDim - 1;
				}
				if( region.y2 >= imageDim ){
					region.y2 = imageDim - 1;
				}
				
				center.x = ( pPaintSector.x - scoord.x ) * imageDim + pPaintGrid.x;
				center.y = ( pPaintSector.y - scoord.y ) * imageDim + pPaintGrid.y;
				
				pixels = htsector->GetHeightImage()->GetDataGrayscale32();
				//heightScale = 1.0f / htsector->GetHeightScaling();
				
				for( y=region.y1; y<=region.y2; y++ ){
					base = imageDim * y;
					
					for( x=region.x1; x<=region.x2; x++ ){
						pFetch3x3( &valueMatrix[ 0 ], scoord.x, scoord.y, x, y );
						average = pSmooth3x3( &valueMatrix[ 0 ] );
						weight = pGauss( ( float )x - center.x, ( float )y - center.y );
						pixels[ base + x ].value = pixels[ base + x ].value * ( 1.0f - weight ) + average * weight;
						
						if( clampValues ){
							if( pixels[ base + x ].value < -0.5f ){
								pixels[ base + x ].value = -0.5f;
								
							}else if( pixels[ base + x ].value > 0.5f ){
								pixels[ base + x ].value = 0.5f;
							}
						}
					}
				}
				
				htsector->SetHeightImageChanged( true );
				pWorld->NotifyHTSHeightChanged( htsector );
			}
//		}
	}
}



void meCLHeightPaint::pFetch3x3( float *values, int sectorX, int sectorY, int x/*gridX*/, int y/*gridY*/ ){
	meHeightTerrain *hterrain = pWorld->GetHeightTerrain();
	int imageDim = hterrain->GetSectorResolution();
	meHeightTerrainSector *htsector;
//	int lsx, lsy, lgx, lgy, x, y;
	sGrayscale32 *pixels;
	
	htsector = hterrain->GetSectorWith( decPoint( sectorX, sectorY ) );
	if( ! htsector ) DETHROW( deeInvalidParam );
	
	pixels = htsector->GetHeightImage()->GetDataGrayscale32();
	
	/*
	for( y=gridY-1; y<gridY+2; y++ ){
		lsy = sectorY;
		lgy = y;
		if( lgy < 0 ){
			lsy--;
			lgy += imageDim;
			
		}else if( lgy >= imageDim ){
			lsy++;
			lgy -= imageDim;
		}
		
		for( x=gridX-1; x<gridX+2; x++ ){
			lsx = sectorX;
			lgx = x;
			if( lgx < 0 ){
				lsx--;
				lgx += imageDim;
				
			}else if( lgx >= imageDim ){
				lsx++;
				lgx -= imageDim;
			}
			
			htsector = hterrain->GetSectorWith( decPoint( lsx, lsy ) );
			if( htsector ){
				
			pixels = htsector->GetHeightImage()->GetDataGrayscale32();
			heightScale = 1.0f / htsector->GetHeightScaling();
			
		}
	}
	*/
	
	bool canGoRight = x < imageDim - 1;
	bool canGoLeft = x > 0;
	
	int base2, base = imageDim * y + x;
	float centerValue = pixels[ base ].value;
	
	// top row
	if( x > 0 ){
		base2 = base - imageDim;
		
		if( canGoLeft ){
			values[ 0 ] = pixels[ base2 - 1 ].value;
			
		}else{
			values[ 0 ] = centerValue;
		}
		
		values[ 1 ] = pixels[ base2 ].value;
		
		if( canGoRight ){
			values[ 2 ] = pixels[ base2 + 1 ].value;
			
		}else{
			values[ 2 ] = centerValue;
		}
	}
	
	// middle row
	if( canGoLeft ){
		values[ 3 ] = pixels[ base - 1 ].value;
		
	}else{
		values[ 3 ] = centerValue;
	}
	
	values[ 4 ] = centerValue;
	
	if( canGoRight ){
		values[ 5 ] = pixels[ base + 1 ].value;
		
	}else{
		values[ 5 ] = centerValue;
	}
	
	// bottom row
	if( y < imageDim - 1 ){
		base2 = base + imageDim;
		
		if( canGoLeft ){
			values[ 6 ] = pixels[ base2 - 1 ].value;
			
		}else{
			values[ 6 ] = centerValue;
		}
		
		values[ 7 ] = pixels[ base2 ].value;
		
		if( canGoRight ){
			values[ 8 ] = pixels[ base2 + 1 ].value;
			
		}else{
			values[ 8 ] = centerValue;
		}
	}
}

float meCLHeightPaint::pSmooth3x3( float *values ){
	return values[ 0 ] * pGaussKernel3x3[ 0 ][ 0 ] + values[ 1 ] * pGaussKernel3x3[ 0 ][ 1 ]
		+ values[ 2 ] * pGaussKernel3x3[ 0 ][ 2 ] + values[ 3 ] * pGaussKernel3x3[ 1 ][ 0 ]
		+ values[ 4 ] * pGaussKernel3x3[ 1 ][ 1 ] + values[ 5 ] * pGaussKernel3x3[ 1 ][ 2 ]
		+ values[ 6 ] * pGaussKernel3x3[ 2 ][ 0 ] + values[ 7 ] * pGaussKernel3x3[ 2 ][ 1 ]
		+ values[ 8 ] * pGaussKernel3x3[ 2 ][ 2 ];
}



void meCLHeightPaint::pUpdateOldHeights( const decPoint &sector, const decVector2 &grid, const decVector2 &size ){
	meHeightTerrain *hterrain = pWorld->GetHeightTerrain();
	int imageDim = hterrain->GetSectorResolution();
	decBoundary growMargins;
	decPoint toGrid;
	decPoint growBy;
	int adjust;
	
//pWorld->GetLogger()->LogInfoFormat( LOGSOURCE, "[meCLHeightPaint::pGrowHeights] se=(%i,%i) gr=(%g,%g) si=(%g,%g)\n", sector.x, sector.y, grid.x, grid.y, size.x, size.y );
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
//pWorld->GetLogger()->LogInfoFormat( LOGSOURCE, "[meCLHeightPaint::pGrowHeights] as=(%i,%i,%i,%i) ag=(%i,%i,%i,%i)\n", pAreaModifySector.x1, pAreaModifySector.y1, pAreaModifySector.x2, pAreaModifySector.y2, pAreaModifyGrid.x1, pAreaModifyGrid.y1, pAreaModifyGrid.x2, pAreaModifyGrid.y2 );
	if( pOldHeights ){
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
// pWorld->GetLogger()->LogInfoFormat( LOGSOURCE, "[meCLHeightPaint::pGrowHeights] new as=(%i,%i,%i,%i) ag=(%i,%i,%i,%i)\n", pAreaSector.x1, pAreaSector.y1, pAreaSector.x2, pAreaSector.y2, pAreaGrid.x1, pAreaGrid.y1, pAreaGrid.x2, pAreaGrid.y2 );
	
	growBy.x = growMargins.x1 + growMargins.x2;
	growBy.y = growMargins.y1 + growMargins.y2;
	
//pWorld->GetLogger()->LogInfoFormat( LOGSOURCE, "[meCLHeightPaint::pGrowHeights] gm=(%i,%i,%i,%i) gb=(%i,%i)\n", growMargins.x1, growMargins.y1, growMargins.x2, growMargins.y2, growBy.x, growBy.y );
	// grow if required preserving the old height values
	if( growBy.x > 0 || growBy.y > 0 ){
		int y, newHeight = pModifyHeight + growBy.y;
		int x, newWidth = pModifyWidth + growBy.x;
		meHeightTerrainSector *htsector;
		sGrayscale32 *htsHeights;
		float *oldHeights = NULL;
		decBoundary retain;
		decPoint scoord;
		int sgx, sgy;
		
		retain.x1 = growMargins.x1;
		retain.y1 = growMargins.y1;
		retain.x2 = growMargins.x1 + pModifyWidth;
		retain.y2 = growMargins.y1 + pModifyHeight;
		
		try{
			oldHeights = new float[ newWidth * newHeight ];
			if( ! oldHeights ) DETHROW( deeOutOfMemory );
			
			for( y=0; y<newHeight; y++ ){
				for( x=0; x<newWidth; x++ ){
					if( x>=retain.x1 && y>=retain.y1 && x<retain.x2 && y<retain.y2 ){
						oldHeights[ y * newWidth + x ] = pOldHeights[ ( y - retain.y1 ) * pModifyWidth + ( x - retain.x1 ) ];
						
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
							htsHeights = htsector->GetHeightImage()->GetDataGrayscale32();
							oldHeights[ y * newWidth + x ] = htsHeights[ sgy * imageDim + sgx ].value;
							
						}else{
							oldHeights[ y * newWidth + x ] = 0.0f;
						}
					}
				}
			}
			
		}catch( const deException & ){
			if( oldHeights ) delete [] oldHeights;
			throw;
		}
		
		if( pOldHeights ) delete [] pOldHeights;
		pOldHeights = oldHeights;
		
		pModifyWidth = newWidth;
		pModifyHeight = newHeight;
	}
}
