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
reRigShape(engine, estHull){
}

reRigShapeHull::~reRigShapeHull(){
}



// Management
///////////////

void reRigShapeHull::AddPoint(const decVector &point){
	InsertPoint(point, pPoints.GetCount());
}

void reRigShapeHull::InsertPoint(const decVector &point, int index){
	pPoints.Insert(point, index);
	NotifyShapeChanged();
}

void reRigShapeHull::SetPointAt(int index, const decVector &point){
	if(pPoints.GetAt(index).IsEqualTo(point)){
		return;
	}
	
	pPoints.SetAt(index, point);
	NotifyShapeChanged();
}

void reRigShapeHull::RemovePoint(int index){
	pPoints.RemoveFrom(index);
	NotifyShapeChanged();
}

reRigShape::Ref reRigShapeHull::Duplicate() const{
	const reRigShapeHull::Ref shape(reRigShapeHull::Ref::New(GetEngine()));
	shape->SetPosition(GetPosition());
	shape->SetOrientation(GetOrientation());
	shape->pPoints = pPoints;
	return shape;
}

void reRigShapeHull::Scale(float scale){
	SetPosition(GetPosition() * scale);
	pPoints.Visit([&](decVector &point){
		point *= scale;
	});
}

decShape *reRigShapeHull::CreateShape(){
	decShapeHull * const shape = new decShapeHull(GetPosition(),
		decQuaternion::CreateFromEuler(GetOrientation() * DEG2RAD));
	
	if(pPoints.IsNotEmpty()){
		try{
			const int count = pPoints.GetCount();
			int i;
			shape->SetPointCount(count);
			for(i=0; i<count; i++){
				shape->SetPointAt(i, pPoints.GetAt(i));
			}
			
		}catch(const deException &){
			delete shape;
			throw;
		}
	}
	
	return shape;
}
