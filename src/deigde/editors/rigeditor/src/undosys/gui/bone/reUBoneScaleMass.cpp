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

#include "reUBoneScaleMass.h"
#include "../../../rig/reRig.h"
#include "../../../rig/bone/reRigBone.h"
#include "../../../rig/bone/reSelectionBones.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>



// Class reUBoneScaleMass
///////////////////////////

// Constructor, destructor
////////////////////////////

reUBoneScaleMass::reUBoneScaleMass(reRig *rig, const decObjectOrderedSet &bones, float newMass){
	if(! rig || bones.GetCount() == 0){
		DETHROW(deeInvalidParam);
	}
	
	if(newMass < 0.0f){
		newMass = 0.0f;
	}
	
	const int boneCount = bones.GetCount();
	float oldMass = 0.0f;
	int i;
	
	pRig = NULL;
	
	pBones = NULL;
	pBoneCount = 0;
	
	SetShortInfo("Scale bone mass");
	
	try{
		if(boneCount > 0){
			// create bones array
			pBones = new sBone[boneCount];
			
			// store bones summing up the their mass. ignore bones without a shape
			for(i=0; i<boneCount; i++){
				pBones[i].bone = (reRigBone*)bones.GetAt(i);
				
				if(pBones[i].bone->GetShapeCount() == 0){
					pBones[i].bone = NULL;
					pBones[i].oldMass = 0.0f;
					
				}else{
					pBones[i].bone->AddReference();
					pBones[i].oldMass = pBones[i].bone->GetMass();
					oldMass += pBones[i].oldMass;
				}
			}
			
			pBoneCount = boneCount;
			
			// calculate the new mass for each bone
			if(oldMass < FLOAT_SAFE_EPSILON){
				for(i=0; i<boneCount; i++){
					pBones[i].newMass = pBones[i].oldMass;
				}
				
			}else{
				const float factor = newMass / oldMass;
				
				for(i=0; i<boneCount; i++){
					pBones[i].newMass = pBones[i].oldMass * factor;
				}
			}
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
	
	pRig = rig;
	rig->AddReference();
}

reUBoneScaleMass::~reUBoneScaleMass(){
	pCleanUp();
}



// Undo and Redo operations
/////////////////////////////

void reUBoneScaleMass::Undo(){
	int i;
	
	for(i=0; i<pBoneCount; i++){
		if(pBones[i].bone){
			pBones[i].bone->SetMass(pBones[i].oldMass);
		}
	}
}

void reUBoneScaleMass::Redo(){
	int i;
	
	for(i=0; i<pBoneCount; i++){
		if(pBones[i].bone){
			pBones[i].bone->SetMass(pBones[i].newMass);
		}
	}
}



// Private Functions
//////////////////////

void reUBoneScaleMass::pCleanUp(){
	if(pBones){
		while(pBoneCount > 0){
			pBoneCount--;
			if(pBones[pBoneCount].bone){
				pBones[pBoneCount].bone->FreeReference();
			}
		}
		
		delete [] pBones;
	}
	
	if(pRig){
		pRig->FreeReference();
	}
}
