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

#include "reUBoneScaleMass.h"
#include "../../../rig/reRig.h"
#include "../../../rig/bone/reSelectionBones.h"

#include <dragengine/common/exceptions.h>



// Class reUBoneScaleMass
///////////////////////////

// Constructor, destructor
////////////////////////////

reUBoneScaleMass::reUBoneScaleMass(reRig *rig, const reRigBone::List &bones, float newMass){
	DEASSERT_NOTNULL(rig)
	DEASSERT_TRUE(bones.IsNotEmpty())
	
	if(newMass < 0.0f){
		newMass = 0.0f;
	}
	
	float oldMass = 0.0f;
	
	SetShortInfo("Scale bone mass");
	
	// store bones summing up the their mass. ignore bones without a shape
	bones.Visit([&](reRigBone *bone){
		if(bone->GetShapes().IsNotEmpty()){
			const cBone::Ref ubone(cBone::Ref::New());
			ubone->bone = bone;
			ubone->newMass = ubone->oldMass = bone->GetMass();
			oldMass += bone->GetMass();
			pBones.Add(ubone);
		}
	});
	
	// calculate the new mass for each bone
	if(oldMass > FLOAT_SAFE_EPSILON){
		const float factor = newMass / oldMass;
		pBones.Visit([&](cBone &bone){
			bone.newMass = bone.oldMass * factor;
		});
	}
	
	pRig = rig;
}

reUBoneScaleMass::~reUBoneScaleMass(){
}



// Undo and Redo operations
/////////////////////////////

void reUBoneScaleMass::Undo(){
	pBones.Visit([&](cBone &bone){
		bone.bone->SetMass(bone.oldMass);
	});
}

void reUBoneScaleMass::Redo(){
	pBones.Visit([&](cBone &bone){
		bone.bone->SetMass(bone.newMass);
	});
}
