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

debpCDVMoveHitModelFace::debpCDVMoveHitModelFace(debpCollisionDetection *coldet){
	if(!coldet) DETHROW(deeInvalidParam);
	
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

void debpCDVMoveHitModelFace::SetComponent(debpComponent *component){
	pComponent = component;
}

void debpCDVMoveHitModelFace::SetTestShape(debpShape *shape, const decDVector &direction){
	pShape = shape;
	pCollider = NULL;
	pDirection = direction;
}

void debpCDVMoveHitModelFace::SetTestCollider(debpCollider *collider, const decDVector &direction){
	pShape = NULL;
	pCollider = collider;
	pDirection = direction;
}



// Visiting
/////////////

void debpCDVMoveHitModelFace::VisitNode(debpDOctree *node, int intersection){
	const debpModelOctree &rnode = *((debpModelOctree*)node);
	int f, faceIndex, faceCount = rnode.GetFaceCount();
	
	if(pShape){
		for(f=0; f<faceCount; f++){
			faceIndex = rnode.GetFaceAt(f);
			
			if(pColDet->ShapeMoveHitsModelFace(*pShape, pDirection, *pComponent, faceIndex, pResultTest)){
				if(!pHasCollision || pResultTest.distance < pResultFinal.distance){
					pResultFinal.shape1 = 0;
					pResultFinal.bone1 = -1;
					pResultFinal.face = faceIndex;
					pResultFinal.distance = pResultTest.distance;
					pResultFinal.normal = pResultTest.normal;
					
					pHasCollision = true;
				}
			}
		}
		
	}else if(pCollider){
		for(f=0; f<faceCount; f++){
			faceIndex = rnode.GetFaceAt(f);
			
			if(pColDet->ColliderMoveHitsModelFace(pCollider, pDirection, *pComponent, faceIndex, pResultTest)){
				if(!pHasCollision || pResultTest.distance < pResultFinal.distance){
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
