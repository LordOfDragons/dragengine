/* 
 * Drag[en]gine Bullet Physics Module
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
#include <stdint.h>

#include "debpCDVMoveHitModelFace.h"
#include "collision/debpDCollisionTriangle.h"
#include "collision/debpDCollisionBox.h"
#include "../collider/debpCollider.h"
#include "../component/debpComponent.h"
#include "../component/debpModel.h"
#include "../component/debpModelOctree.h"
#include "../shape/debpShape.h"
#include "../shape/debpShapeList.h"

#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/common/exceptions.h>



// Class debpCDVMoveHitModelFace
//////////////////////////////////

// Constructor, destructor
////////////////////////////

debpCDVMoveHitModelFace::debpCDVMoveHitModelFace( debpCollisionDetection *coldet ){
	if( ! coldet ) DETHROW( deeInvalidParam );
	
	pColDet = coldet;
	
	pCollider = NULL;
	pShape = NULL;
	
	pComponent = NULL;
	
	pHasCollision = false;
}

debpCDVMoveHitModelFace::~debpCDVMoveHitModelFace(){
}



// Management
///////////////

void debpCDVMoveHitModelFace::Reset(){
	pHasCollision = false;
}

void debpCDVMoveHitModelFace::SetComponent( debpComponent *component ){
	pComponent = component;
}

void debpCDVMoveHitModelFace::SetTestShape( debpShape *shape, const decDVector &direction ){
	pShape = shape;
	pCollider = NULL;
	pDirection = direction;
}

void debpCDVMoveHitModelFace::SetTestCollider( debpCollider *collider, const decDVector &direction ){
	pShape = NULL;
	pCollider = collider;
	pDirection = direction;
}



// Visiting
/////////////

void debpCDVMoveHitModelFace::VisitNode( debpDOctree *node, int intersection ){
	const debpModelOctree &rnode = *( ( debpModelOctree* )node );
	int f, faceIndex, faceCount = rnode.GetFaceCount();
	
	if( pShape ){
		for( f=0; f<faceCount; f++ ){
			faceIndex = rnode.GetFaceAt( f );
			
			if( pColDet->ShapeMoveHitsModelFace( *pShape, pDirection, *pComponent, faceIndex, pResultTest ) ){
				if( ! pHasCollision || pResultTest.distance < pResultFinal.distance ){
					pResultFinal.shape1 = 0;
					pResultFinal.bone1 = -1;
					pResultFinal.face = faceIndex;
					pResultFinal.distance = pResultTest.distance;
					pResultFinal.normal = pResultTest.normal;
					
					pHasCollision = true;
				}
			}
		}
		
	}else if( pCollider ){
		for( f=0; f<faceCount; f++ ){
			faceIndex = rnode.GetFaceAt( f );
			
			if( pColDet->ColliderMoveHitsModelFace( pCollider, pDirection, *pComponent, faceIndex, pResultTest ) ){
				if( ! pHasCollision || pResultTest.distance < pResultFinal.distance ){
					pResultFinal.shape1 = pResultTest.shape1;
					pResultFinal.bone1 = pResultTest.bone1;
					pResultFinal.face = faceIndex;
					pResultFinal.distance = pResultTest.distance;
					pResultFinal.normal = pResultTest.normal;
					
					pHasCollision = true;
				}
			}
		}
	}
}
