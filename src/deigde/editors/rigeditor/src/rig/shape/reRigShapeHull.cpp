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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "reRigShapeHull.h"
#include "../reRig.h"
#include "../bone/reRigBone.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeHull.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/collider/deColliderVolume.h>



// Class reRigShapeHull
/////////////////////////

// Constructor, destructor
////////////////////////////

reRigShapeHull::reRigShapeHull(deEngine *engine) :
reRigShape(engine, estHull),
pPoints(NULL),
pPointCount(0){
}

reRigShapeHull::~reRigShapeHull(){
	if(pPoints){
		delete [] pPoints;
	}
}



// Management
///////////////

const decVector &reRigShapeHull::GetPointAt(int index) const{
	if(index < 0 || index >= pPointCount){
		DETHROW(deeOutOfBoundary);
	}
	
	return pPoints[index];
}

void reRigShapeHull::AddPoint(const decVector &point){
	InsertPoint(point, pPointCount);
}

void reRigShapeHull::InsertPoint(const decVector &point, int index){
	if(index < 0 || index > pPointCount){
		DETHROW(deeInvalidParam);
	}
	
	decVector * const newArray = new decVector[pPointCount + 1];
	if(pPoints){
		int i;
		for(i=0; i<index; i++){
			newArray[i] = pPoints[i];
		}
		for(i=index; i<pPointCount; i++){
			newArray[i + 1] = pPoints[i];
		}
		delete [] pPoints;
	}
	pPoints = newArray;
	pPointCount++;
	
	pPoints[index] = point;
	
	NotifyShapeChanged();
}

void reRigShapeHull::SetPointAt(int index, const decVector &point){
	if(index < 0 || index >= pPointCount){
		DETHROW(deeOutOfBoundary);
	}
	
	if(pPoints[index].IsEqualTo(point)){
		return;
	}
	
	pPoints[index] = point;
	
	NotifyShapeChanged();
}

void reRigShapeHull::RemovePoint(int index){
	if(index < 0 || index >= pPointCount){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	for(i=index+1; i<pPointCount; i++){
		pPoints[i - 1] = pPoints[i];
	}
	pPointCount--;
	
	NotifyShapeChanged();
}



reRigShape *reRigShapeHull::Duplicate() const{
	const reRigShapeHull::Ref shape(reRigShapeHull::Ref::NewWith(GetEngine()));
	shape->SetPosition(GetPosition());
	shape->SetOrientation(GetOrientation());
	if(pPointCount > 0){
		shape->pPoints = new decVector[pPointCount];
		shape->pPointCount = pPointCount;
		memcpy(shape->pPoints, pPoints, sizeof(decVector) * pPointCount);
	}
	return shape;
}

void reRigShapeHull::Scale(float scale){
	SetPosition(GetPosition() * scale);
	int i;
	for(i=0; i<pPointCount; i++){
		pPoints[i] *= scale;
	}
}

decShape *reRigShapeHull::CreateShape(){
	decShapeHull * const shape = new decShapeHull(GetPosition(),
		decQuaternion::CreateFromEuler(GetOrientation() * DEG2RAD));
	if(pPointCount > 0){
		try{
			shape->SetPointCount(pPointCount);
			int i;
			for(i=0; i<pPointCount; i++){
				shape->SetPointAt(i, pPoints[i]);
			}
			
		}catch(const deException &){
			delete shape;
			throw;
		}
	}
	
	return shape;
}
