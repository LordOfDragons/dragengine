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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "debpCDVHitModelFace.h"
#include "collision/debpDCollisionTriangle.h"
#include "collision/debpDCollisionBox.h"
#include "../collider/debpCollider.h"
#include "../collider/debpColliderVolume.h"
#include "../component/debpComponent.h"
#include "../component/debpModel.h"
#include "../component/debpModelOctree.h"
#include "../shape/debpShape.h"
#include "../shape/debpShapeList.h"

#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/common/exceptions.h>



// Class debpCDVHitModelFace
//////////////////////////////

// Constructor, destructor
////////////////////////////

debpCDVHitModelFace::debpCDVHitModelFace( debpCollisionDetection *coldet ){
	if( ! coldet ) DETHROW( deeInvalidParam );
	
	pColDet = coldet;
	
	pColliderVolume = NULL;
	pColliderComponent = NULL;
	pShape = NULL;
	
	pComponent = NULL;
	
	pHasCollision = false;
}

debpCDVHitModelFace::~debpCDVHitModelFace(){
}



// Management
///////////////

void debpCDVHitModelFace::Reset(){
	pHasCollision = false;
}

void debpCDVHitModelFace::SetComponent( debpComponent *component ){
	pComponent = component;
}

void debpCDVHitModelFace::SetTestShape( debpShape *shape ){
	pShape = shape;
	
	pColliderVolume = NULL;
	pColliderComponent = NULL;
}

void debpCDVHitModelFace::SetTestCollider( debpCollider *collider ){
	pShape = NULL;
	
	pColliderVolume = NULL;
	pColliderComponent = NULL;
	
	if( collider ){
		if( collider->IsVolume() ){
			pColliderVolume = ( debpColliderVolume* )collider;
			
		}else{
			pColliderComponent = ( debpColliderComponent* )collider;
		}
	}
}



// Visiting
/////////////

void debpCDVHitModelFace::VisitNode( debpDOctree *node, int intersection ){
	if( ! pHasCollision ){
		const debpModelOctree &rnode = *( ( debpModelOctree* )node );
		int f, faceIndex, faceCount = rnode.GetFaceCount();
		
		if( pShape ){
			for( f=0; f<faceCount; f++ ){
				faceIndex = rnode.GetFaceAt( f );
				
				if( pColDet->ShapeHitsModelFace( *pShape, *pComponent, faceIndex ) ){
					pResult.shape1 = 0;
					pResult.face = faceIndex;
					pHasCollision = true;
					break;
				}
			}
			
		}else if( pColliderVolume ){
			const debpShapeList &shapes = pColliderVolume->GetShapes();
			int s, shapeCount = shapes.GetShapeCount();
			
			for( s=0; s<shapeCount; s++ ){
				debpShape &shape = *shapes.GetShapeAt( s );
				
				for( f=0; f<faceCount; f++ ){
					faceIndex = rnode.GetFaceAt( f );
					
					if( pColDet->ShapeHitsModelFace( shape, *pComponent, faceIndex ) ){
						pResult.shape1 = s;
						pResult.face = faceIndex;
						pHasCollision = true;
						break;
					}
				}
			}
		}
	}
}
