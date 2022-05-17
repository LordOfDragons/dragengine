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
#include <string.h>

#include "deoglLODCalculator.h"
#include "../../collidelist/deoglCollideList.h"
#include "../../collidelist/deoglCollideListComponent.h"
#include "../../component/deoglRComponent.h"
#include "../../model/deoglRModel.h"
#include "../../model/deoglModelLOD.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>



// Class deoglLODCalculator
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglLODCalculator::deoglLODCalculator() :
pMaxPixelError( 2 ),
pLodOffset( 0 ){
}

deoglLODCalculator::~deoglLODCalculator(){
}



// Management
///////////////

void deoglLODCalculator::SetMaxPixelError( int maxPixelError ){
	pMaxPixelError = decMath::max( maxPixelError, 1 );
}

void deoglLODCalculator::SetLodOffset( int offset ){
	pLodOffset = offset;
}



void deoglLODCalculator::SetComponentLOD0( const deoglCollideList &collideList ){
	const int count = collideList.GetComponentCount();
	int i;
	
	for( i=0; i<count; i++ ){
		collideList.GetComponentAt( i )->SetLODLevel( 0 );
	}
}

void deoglLODCalculator::SetComponentLODMax( const deoglCollideList &collideList ){
	const int count = collideList.GetComponentCount();
	int i;
	
	for( i=0; i<count; i++ ){
		collideList.GetComponentAt( i )->SetLODLevelMax();
	}
}

void deoglLODCalculator::SetComponentLODProjection( const deoglCollideList &collideList, const decDVector &position,
const decDVector &view, float fovX, float fovY, int screenWidth, int screenHeight ){
	// test lod levels starting with the smallest one for the first lod level which still
	// yields an error on screen not larger than the maximum pixel error.
	// 
	// z [m]: object distance minus radius
	// le(i) [m]: max error at lod i
	// se: screen max error ratio (maxError[px] / screenWidth[px])
	// 
	// hw [m] = z [m] * tan(fov): half width of screen at distance z
	// e [m] = hw [m] * se: max error at distance z
	//       = z [m] * tan(fov) * se
	const float factorX = tanf( fovX * 0.5f ) * pMaxPixelError / screenWidth;
	const float factorY = tanf( fovY * 0.5f ) * pMaxPixelError / screenHeight;
	const float factor = decMath::min( factorX, factorY );
	
	const int componentCount = collideList.GetComponentCount();
	int i, j;
	
	for( i=0; i<componentCount; i++ ){
		deoglCollideListComponent &clistComponent = *collideList.GetComponentAt( i );
		const deoglRComponent &component = *clistComponent.GetComponent();
		int lodLevel = 0;
		
		if( component.GetModel() ){
			const deoglRModel &model = *component.GetModel();
			const int lodLevelCount = model.GetLODCount();
			
			if( lodLevelCount > 1 ){
				// calculate a sphere around the component extends. this causes the lod calculation to be very
				// cautious but this can be compensated with better calculation later on for components with
				// elongated box shapes.
				const decDVector &minExtend = component.GetMinimumExtend();
				const decDVector &maxExtend = component.GetMaximumExtend();
				const decDVector center = ( minExtend + maxExtend ) * 0.5;
				const float radius = ( float )( ( maxExtend - minExtend ).Length() * 0.5 );
				const float componentDistance = ( float )( ( center - position ) * view ) - radius;
				
				if( componentDistance > 1.0f ){
					const float errorScaling = component.GetLODErrorScaling();
					const float maxAllowedError = factor * componentDistance;
					
					for( j=1; j<lodLevelCount; j++ ){
						const deoglModelLOD &lod = model.GetLODAt( j );
						
						if( lod.GetMaxError() * errorScaling > maxAllowedError ){
							break;
						}
						
						lodLevel = j;
					}
					
					lodLevel = decMath::clamp( lodLevel + pLodOffset, 0, lodLevelCount - 1 );
				}
			}
		}
		
		//lodLevel = 0;
		clistComponent.SetLODLevel( lodLevel );
	}
}

void deoglLODCalculator::SetComponentLODOrtho( const deoglCollideList &collideList, float boxWidth, float boxHeight,
int screenWidth, int screenHeight ){
	// test lod levels starting with the smallest one for the first lod level which still
	// yields an error on screen not larger than the maximum pixel error. in contrary to
	// the projective case for the orthogonal case the calculation is simplified using:
	// 
	// e = maximum error for lod level in meters
	// bs = size in meters of orthogonal box
	// vs = size in pixels of viewport
	// 
	// s = ( e / bs ) * vs
	// 
	// with a constant:
	//    factor = vs / bs => s = e * factor 
	// 
	// for non-square screens the factor for both directions is calculated and the largest factor
	// used since we are interested in the largest error on screen. once the maximum error is calculated
	// the smallest lod level is searched where the calculated maximum error is less than the maximum
	// pixel error.
	// 
	const int componentCount = collideList.GetComponentCount();
	const float factorX = ( float )screenWidth / boxWidth;
	const float factorY = ( float )screenHeight / boxHeight;
	const float factor = ( factorX > factorY ) ? factorX : factorY;
	int i, j;
	
	for( i=0; i<componentCount; i++ ){
		deoglCollideListComponent &clistComponent = *collideList.GetComponentAt( i );
		const deoglRComponent &component = *clistComponent.GetComponent();
		
		if( ! component.GetModel() ){
			clistComponent.SetLODLevel( 0 );
			continue;
		}
		
		const deoglRModel &model = *component.GetModel();
		const int lodLevelCount = model.GetLODCount();
		
		if( lodLevelCount < 2 ){
			clistComponent.SetLODLevel( 0 );
			continue;
		}
		
		const float errorScaling = component.GetLODErrorScaling();
		int lodLevel = 0;
		
		for( j=1; j<lodLevelCount; j++ ){
			const deoglModelLOD &lod = model.GetLODAt( j );
			
			const float maxError = lod.GetMaxError() * errorScaling;
			if( maxError * factor > pMaxPixelError ){
				break;
			}
			
			lodLevel = j;
		}
		
		//lodLevel = 0;
		clistComponent.SetLODLevel( decMath::clamp( lodLevel + pLodOffset, 0, lodLevelCount - 1 ) );
	}
}

void deoglLODCalculator::SetComponentLODOrtho( deoglCollideListComponent &clistComponent,
float boxWidth, float boxHeight, int screenWidth, int screenHeight ){
	const float factorX = ( float )screenWidth / boxWidth;
	const float factorY = ( float )screenHeight / boxHeight;
	const float factor = ( factorX > factorY ) ? factorX : factorY;
	const deoglRComponent &component = *clistComponent.GetComponent();
	
	if( ! component.GetModel() ){
		clistComponent.SetLODLevel( 0 );
		return;
	}
	
	const deoglRModel &model = *component.GetModel();
	const int lodLevelCount = model.GetLODCount();
	
	if( lodLevelCount < 2 ){
		clistComponent.SetLODLevel( 0 );
		return;
	}
	
	const float errorScaling = component.GetLODErrorScaling();
	int i, lodLevel = 0;
	
	for( i=1; i<lodLevelCount; i++ ){
		const deoglModelLOD &lod = model.GetLODAt( i );
		
		const float maxError = lod.GetMaxError() * errorScaling;
		if( maxError * factor > pMaxPixelError ){
			break;
		}
		
		lodLevel = i;
	}
	
	clistComponent.SetLODLevel( decMath::clamp( lodLevel + pLodOffset, 0, lodLevelCount - 1 ) );
}

void deoglLODCalculator::SetComponentLODOmniDir( const deoglCollideList &collideList,
const decDVector &position, int size ){
	const int componentCount = collideList.GetComponentCount();
	const float factor = ( float )pMaxPixelError / ( float )size;
	int i, j;
	
	for( i=0; i<componentCount; i++ ){
		deoglCollideListComponent &clistComponent = *collideList.GetComponentAt( i );
		const deoglRComponent &component = *clistComponent.GetComponent();
		int lodLevel = 0;
		
		if( component.GetModel() ){
			const deoglRModel &model = *component.GetModel();
			const int lodLevelCount = model.GetLODCount();
			
			if( lodLevelCount > 1 ){
				// calculate a sphere around the component extends. this causes the lod
				// calculation to be very cautious but this can be compensated with better
				// calculation later on for components with elongated box shapes
				const decDVector &minExtend = component.GetMinimumExtend();
				const decDVector &maxExtend = component.GetMaximumExtend();
				const decDVector center( ( minExtend + maxExtend ) * 0.5 );
				const float radius = ( float )( ( maxExtend - minExtend ).Length() * 0.5 );
				const float componentDistance = ( float )( ( center - position ).Length() ) - radius;
				
				if( componentDistance > 1.0f ){
					const float errorScaling = component.GetLODErrorScaling();
					const float maxAllowedError = factor * componentDistance;
					
					for( j=1; j<lodLevelCount; j++ ){
						const deoglModelLOD &lod = model.GetLODAt( j );
						
						if( lod.GetMaxError() * errorScaling > maxAllowedError ){
							break;
						}
						
						lodLevel = j;
					}
					
					lodLevel = decMath::clamp( lodLevel + pLodOffset, 0, lodLevelCount - 1 );
				}
			}
		}
		
		clistComponent.SetLODLevel( lodLevel );
	}
}
