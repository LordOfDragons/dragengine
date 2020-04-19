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
