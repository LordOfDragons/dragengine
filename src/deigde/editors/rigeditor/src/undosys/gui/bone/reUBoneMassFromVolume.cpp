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

#include "reUBoneMassFromVolume.h"
#include "../../../rig/reRig.h"
#include "../../../rig/bone/reSelectionBones.h"
#include "../../../rig/shape/reRigShape.h"
#include "../../../rig/shape/reRigShapeSphere.h"
#include "../../../rig/shape/reRigShapeBox.h"
#include "../../../rig/shape/reRigShapeCylinder.h"
#include "../../../rig/shape/reRigShapeCapsule.h"
#include "../../../rig/shape/reRigShapeHull.h"

#include <dragengine/common/exceptions.h>



// Class reUBoneMassFromVolume
////////////////////////////////

// Constructor, destructor
////////////////////////////

reUBoneMassFromVolume::reUBoneMassFromVolume(reRig *rig, const reRigBone::List &bones, float density){
	DEASSERT_NOTNULL(rig);
	DEASSERT_TRUE(bones.IsNotEmpty());
	
	if(density < 0.0f){
		density = 0.0f;
	}
	
	SetShortInfo("Bone mass from volume");
	
	bones.Visit([&](reRigBone *bone){
		const cBone::Ref ubone(cBone::Ref::New());
		ubone->bone = bone;
		ubone->oldMass = bone->GetMass();
		ubone->newMass = pCalcVolume(*bone) * density;
		pBones.Add(ubone);
	});
	
	pRig = rig;
}

reUBoneMassFromVolume::~reUBoneMassFromVolume(){
}



// Undo and Redo operations
/////////////////////////////

void reUBoneMassFromVolume::Undo(){
	pBones.Visit([&](cBone &bone){
		bone.bone->SetMass(bone.oldMass);
	});
}

void reUBoneMassFromVolume::Redo(){
	pBones.Visit([&](cBone &bone){
		bone.bone->SetMass(bone.newMass);
	});
}



// Private Functions
//////////////////////

float reUBoneMassFromVolume::pCalcVolume(const reRigBone &bone) const{
	float volume = 0.0f;
	
	bone.GetShapes().Visit([&](const reRigShape &shape){
		const reRigShape::eShapeTypes type = shape.GetShapeType();
		
		switch(type){
		case reRigShape::estSphere:{
			const reRigShapeSphere &sphere = (reRigShapeSphere&)shape;
			const float radius = sphere.GetRadius();
			volume += PI * radius * radius * radius * 4.0f / 3.0f;
			}break;
			
		case reRigShape::estBox:{
			const reRigShapeBox &box = *((reRigShapeBox*)&shape);
			const decVector extends = box.GetHalfExtends() * 2.0f;
			volume += extends.x * extends.y * extends.z;
			}break;
			
		case reRigShape::estCylinder:{
			const reRigShapeCylinder &cylinder = (reRigShapeCylinder&)shape;
			const float height = cylinder.GetHalfHeight() * 2.0f;
			const float radiusBottom = cylinder.GetBottomRadius();
			const float radiusTop = cylinder.GetTopRadius();
			const float radius = (radiusTop + radiusBottom) * 0.5f;
			volume += PI * radius * radius * height;
			}break;
			
		case reRigShape::estCapsule:{
			// NOTE this solution is not fully correct. the spheres at the top and bottom are
			//      calculate as if the top and bottom radius are equal. correctly though due
			//      to the different radi the spheres are slightly moved in or out of the
			//      cylinder. this can be achieved by calculating the tilt angle of the base
			//      cylinder and using this as the angle where the sphere is truely cut.
			const reRigShapeCapsule &capsule = (reRigShapeCapsule&)shape;
			const float height = capsule.GetHalfHeight() * 2.0f;
			const float radiusBottom = capsule.GetBottomRadius();
			const float radiusTop = capsule.GetTopRadius();
			const float radius = (radiusTop + radiusBottom) * 0.5f;
			volume += PI * radius * radius * height; // base cylinder
			volume += PI * radiusTop * radiusTop * radiusTop * 2.0f / 3.0f; // top half-sphere
			volume += PI * radiusBottom * radiusBottom * radiusBottom * 2.0f / 3.0f; // bottom half-sphere
			}break;
			
		case reRigShape::estHull:{
			// this calculation is a crude approximation for the time being
			const reRigShapeHull::PointList &points = ((reRigShapeHull&)shape).GetPoints();
			const int pointCount = points.GetCount();
			decVector minExtend, maxExtend;
			int i;
			
			for(i=0; i<pointCount; i++){
				const decVector &point = points.GetAt(i);
				if(i == 0){
					minExtend = maxExtend = point;
					
				}else{
					minExtend.SetSmallest(point);
					maxExtend.SetLargest(point);
				}
			}
			
			const decVector extends(maxExtend - minExtend);
			volume += extends.x * extends.y * extends.z;
			}break;
			
		default:
			break;
		}
	});
	
	return volume;
}
