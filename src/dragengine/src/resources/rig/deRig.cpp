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
#include <string.h>

#include "deRig.h"
#include "deRigBone.h"
#include "deRigManager.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/physics/deBasePhysicsRig.h"



// Class deRig
////////////////

// Constructor, destructor
////////////////////////////

deRig::deRig(deRigManager *manager, deVirtualFileSystem *vfs, const char *filename,
	TIME_SYSTEM modificationTime) :
deFileResource(manager, vfs, filename, modificationTime),
pBones(NULL),
pBoneCount(0),
pBoneSize(0),
pRootBone(-1),
pModelCollision(false),
pPeerPhysics(NULL){
}

deRig::~deRig(){
	if(pPeerPhysics){
		delete pPeerPhysics;
		pPeerPhysics = NULL;
	}
	
	RemoveAllBones();
	if(pBones){
		delete [] pBones;
	}
}



// Management
///////////////

bool deRig::Verify() const{
	bool *boneVisited = NULL;
	int i, j, parent;
	bool success = true;
	
	// check bones
	if(pBoneCount > 0){
		try{
			boneVisited = new bool[pBoneCount];
			
			// check bone parents are valid
			for(i=0; i<pBoneCount; i++){
				for(j=0; j<pBoneCount; j++) boneVisited[j] = false;
				parent = pBones[i]->GetParent();
				while(parent != -1){
					if(parent < -1 || parent >= pBoneCount || boneVisited[parent]){
						success = false;
						break;
					}
					boneVisited[parent] = true;
					parent = pBones[parent]->GetParent();
				}
				if(! success) break;
			}
			
			// clean up
			delete [] boneVisited;
			boneVisited = NULL;
			
		}catch(const deException &){
			if(boneVisited) delete [] boneVisited;
			throw;
		}
	}
	
	// finished
	return success;
}

void deRig::Prepare(){
	deRigBone *bone;
	
	// nothing to do if there are no bones
	if(pBoneCount == 0){
		return;
	}
	
	// calculate the matrices of each bone
	bool *calculated = NULL;
	int i, remaining, parent;
	decMatrix boneMat;
	try{
		// create a temporary array to hold the calculated status
		calculated = new bool[pBoneCount];
		if(! calculated) DETHROW(deeOutOfMemory);
		for(i=0; i<pBoneCount; i++) calculated[i] = false;
		
		// keeps track of the count of bones in need of calculation
		remaining = pBoneCount;
		
		// loop until there are no more remaining bones
		while(remaining > 0){
			// loop over all bones and calculate those not done so yet
			for(i=0; i<pBoneCount; i++){
				// if calculated skip the bone
				if(calculated[i]){
					continue;
				}
				bone = pBones[i];
				
				// check if the parent if present is calculated
				parent = bone->GetParent();
				if(parent != -1 && ! calculated[parent]){
					continue;
				}
				
				// calculate the matrix
				boneMat = decMatrix::CreateRT(bone->GetRotation(), bone->GetPosition());
				if(parent != -1) boneMat *= pBones[parent]->GetMatrix();
				bone->SetMatrices(boneMat);
				
				// mark as calculate and decrease count of remaining bones by one
				calculated[i] = true;
				remaining--;
			}
		}
		
		// free temporary array
		delete [] calculated;
		
	}catch(const deException &){
		if(calculated) delete [] calculated;
		throw;
	}
	
	/*
	decMatrix boneMat;
	int i, parent;
	for(i=0; i<pBoneCount; i++){
		boneMat = decMatrix::CreateRotation(pBones[i]->GetRotation())
			* decMatrix::CreateTranslation( pBones[ i ]->GetPosition() );
		parent = pBones[i]->GetParent();
		while(parent != -1){
			boneMat *= decMatrix::CreateRotation(pBones[parent]->GetRotation())
				* decMatrix::CreateTranslation( pBones[ parent ]->GetPosition() );
			parent = pBones[parent]->GetParent();
		}
		pBones[i]->SetMatrices(boneMat);
	}
	*/
}



void deRig::SetCentralMassPoint(const decVector &cmp){
	pCMP = cmp;
}

void deRig::SetModelCollision(bool modelCollision){
	pModelCollision = modelCollision;
}



// Bones
//////////

int deRig::IndexOfBoneNamed(const char *name) const{
	int i;
	
	for(i=0; i<pBoneCount; i++){
		if(pBones[i]->GetName() == name){
			return i;
		}
	}
	
	return -1;
}

bool deRig::HasBoneNamed(const char *name) const{
	int i;
	
	for(i=0; i<pBoneCount; i++){
		if(pBones[i]->GetName() == name){
			return true;
		}
	}
	
	return false;
}

deRigBone &deRig::GetBoneAt(int index) const{
	if(index < 0 || index >= pBoneCount){
		DETHROW(deeInvalidParam);
	}
	
	return *pBones[index];
}

void deRig::AddBone(deRigBone *bone){
	if(! bone || HasBoneNamed(bone->GetName())){
		DETHROW(deeInvalidParam);
	}
	
	if(pBoneCount == pBoneSize){
		const int newSize = pBoneSize * 3 / 2 + 1;
		deRigBone ** const newArray = new deRigBone*[newSize];
		if(pBones){
			memcpy(newArray, pBones, sizeof(deRigBone*) * pBoneSize);
			delete [] pBones;
		}
		pBones = newArray;
		pBoneSize = newSize;
	}
	
	pBones[pBoneCount] = bone;
	pBoneCount++;
}

void deRig::RemoveAllBones(){
	if(pBoneCount > 0){
		while(pBoneCount > 0){
			delete pBones[pBoneCount - 1];
			pBoneCount--;
		}
	}
}

void deRig::SetRootBone(int rootBone){
	if(rootBone < -1 || rootBone >= pBoneCount){
		DETHROW(deeInvalidParam);
	}
	pRootBone = rootBone;
}



// Shapes
///////////

void deRig::SetShapes(const decShapeList &shapes){
	const int count = shapes.GetCount();
	int i;
	
	pShapes = shapes;
	
	pShapeProperties.RemoveAll();
	for(i=0; i<count; i++){
		pShapeProperties.Add("");
	}
}

void deRig::SetShapeProperties(const decStringList &properties){
	if(properties.GetCount() != pShapes.GetCount()){
		DETHROW(deeInvalidParam);
	}
	
	pShapeProperties = properties;
}



// System Peers
/////////////////

void deRig::SetPeerPhysics(deBasePhysicsRig *peer){
	if(peer == pPeerPhysics){
		return;
	}
	
	if(pPeerPhysics){
		delete pPeerPhysics;
	}
	pPeerPhysics = peer;
}
