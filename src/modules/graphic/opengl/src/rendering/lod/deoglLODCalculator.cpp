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

#include <dragengine/common/exceptions.h>



// Class deoglLODCalculator
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglLODCalculator::deoglLODCalculator(){
	pMaxPixelError = 2;
	pMaxErrorPerLevel = 0.1f;
}

deoglLODCalculator::~deoglLODCalculator(){
}



// Management
///////////////

void deoglLODCalculator::SetMaxPixelError( int maxPixelError ){
	if( maxPixelError < 1 ){
		pMaxPixelError = 1;
		
	}else{
		pMaxPixelError = maxPixelError;
	}
}

void deoglLODCalculator::SetMaxErrorPerLevel( float maxErrorPerLevel ){
	if( maxErrorPerLevel < 0.001f ){
		pMaxErrorPerLevel = 0.001f;
		
	}else{
		pMaxErrorPerLevel = maxErrorPerLevel;
	}
}



void deoglLODCalculator::SetComponentLOD0( deoglCollideList &collideList ){
	const int componentCount = collideList.GetComponentCount();
	int i;
	
	for( i=0; i<componentCount; i++ ){
		collideList.GetComponentAt( i )->SetLODLevel( 0 );
	}
}

void deoglLODCalculator::SetComponentLODMax( deoglCollideList &collideList ){
	const int componentCount = collideList.GetComponentCount();
	int i;
	
	for( i=0; i<componentCount; i++ ){
		deoglCollideListComponent &c = *collideList.GetComponentAt( i );
		c.SetLODLevel( c.GetComponent()->GetLODCount() - 1 );
	}
}

void deoglLODCalculator::SetComponentLODProjection( deoglCollideList &collideList, const decDVector &position,
const decDVector &view, float fovX, float fovY, int screenWidth, int screenHeight ){
	// test lod levels starting with the smallest one for the first lod level which still
	// yields an error on screen not larger than the maximum pixel error. for this the
	// minimum distance is calculated for each lod level where the error on screen
	// becomes exactly the maximum pixel error using the following:
	// 
	// z = distance to object (distance to closest point of object to matrix position)
	// z' = distance to reference plane (screen plane)
	// e = maximum error for lod level in meters
	// e' = maximum error for lod level on screen in meters
	// fov = field of view
	// v' = screen size in meters at distance z'
	// vs = viewport size
	// 
	// from projection mapping:
	//    e/z = e'/z' => e' = z'*e/z
	// from camera geometry:
	//    v' = 2*[z'*tan(fov/2)]
	// error relative to screen:
	//    s = e'/v' = [z'*e/z]/{2*[z'*tan(fov/2)]}
	// thus:
	//    s = (e/z) / [2*tan(fov/2)]
	// usin the viewport size this yield the error relative to screen in pixels:
	//    s = (e/z) * vs / [2*tan(fov/2)]
	// with a constant:
	//    factor = vs * 0.5 / tan( fov / 2 ) => s = (e/z) * factor 
	// 
	// this calculates thus the error in pixels for an object at distance z. what we want now is the
	// distance at which the error in pixels on screen matches the threshold:
	//    z = (e/s) * factor
	// with the maximum error from the configuration this yields the result:
	//    z = (e/maxError) * factor = e * factor2
	// with:
	//    factor2 = factor / maxError = vs * 0.5 / [ maxError * tan( fov / 2 ) ]
	// 
	// for non-square screens the factor2 for both directions is calculated and the largest factor
	// used since we are interested in the largest error on screen. once the distance is calculated
	// the smallest lod level is searched where the calculated distance is closer to the screen than
	// the object distance.
	// 
	const int componentCount = collideList.GetComponentCount();
	const float factorX = ( float )screenWidth * 0.5f / ( float )pMaxPixelError / tanf( fovX * 0.5f );
	const float factorY = ( float )screenHeight * 0.5f / ( float )pMaxPixelError / tanf( fovY * 0.5f );
	const float factor = ( factorX > factorY ) ? factorX : factorY;
	int lodLevel;
	int i, j;
	
	for( i=0; i<componentCount; i++ ){
		deoglCollideListComponent &clistComponent = *collideList.GetComponentAt( i );
		deoglRComponent &component = *clistComponent.GetComponent();
		
		lodLevel = 0;
		
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
					
					float clampError = pMaxErrorPerLevel;
					
					for( j=1; j<lodLevelCount; j++ ){
						const deoglModelLOD &lod = model.GetLODAt( j );
						
						const float maxError = decMath::min( lod.GetMaxError() * errorScaling, clampError );
						const float minErrorDistance = maxError * factor;
						if( componentDistance > minErrorDistance ){
							lodLevel = j;
							clampError += pMaxErrorPerLevel;
							
						}else{
							break;
						}
					}
				}
			}
		}
		
		//lodLevel = 0;
		clistComponent.SetLODLevel( lodLevel );
	}
}

void deoglLODCalculator::SetComponentLODOrtho( deoglCollideList &collideList, float boxWidth, float boxHeight,
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
	int lodLevel;
	int i, j;
	
	for( i=0; i<componentCount; i++ ){
		deoglCollideListComponent &clistComponent = *collideList.GetComponentAt( i );
		deoglRComponent &component = *clistComponent.GetComponent();
		
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
		float clampError = pMaxErrorPerLevel;
		lodLevel = 0;
		
		for( j=1; j<lodLevelCount; j++ ){
			const deoglModelLOD &lod = model.GetLODAt( j );
			
			const float maxError = decMath::min( lod.GetMaxError() * errorScaling, clampError );
			if( maxError * factor > pMaxPixelError ){
				break;
			}
			
			lodLevel = j;
			clampError += pMaxErrorPerLevel;
		}
		
		//lodLevel = 0;
		clistComponent.SetLODLevel( lodLevel );
	}
}
