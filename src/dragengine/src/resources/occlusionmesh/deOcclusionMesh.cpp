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

#include "deOcclusionMesh.h"
#include "deOcclusionMeshManager.h"
#include "deOcclusionMeshBone.h"
#include "deOcclusionMeshVertex.h"
#include "deOcclusionMeshWeight.h"
#include "../../systems/modules/graphic/deBaseGraphicOcclusionMesh.h"
#include "../../common/exceptions.h"



// Class deOcclusionMesh
//////////////////////////

// Constructor, destructor
////////////////////////////

deOcclusionMesh::deOcclusionMesh(deOcclusionMeshManager *manager, deVirtualFileSystem *vfs,
	const char *filename, TIME_SYSTEM modificationTime) :
deFileResource(manager, vfs, filename, modificationTime),
pDoubleSidedFaceCount(0),
pPeerGraphic (nullptr){
}

deOcclusionMesh::~deOcclusionMesh(){
	if(pPeerGraphic){
		delete pPeerGraphic;
		pPeerGraphic = nullptr;
	}
}


// Management
///////////////

bool deOcclusionMesh::Verify() const{
	bool success = true;
	
	// verify that all bones are free of cyclic parentship
	if(pBones.GetCount() > 0){
		decTList<bool> visited(pBones.GetCount(), false);
		
		// check bone parents are valid
		int i, j;
		for(i=0; i<pBones.GetCount(); i++){
			for(j=0; j<pBones.GetCount(); j++){
				visited[j] = false;
			}
			
			int parent = pBones[i].GetParent();
			while(parent != -1){
				if(parent < -1 || parent >= pBones.GetCount() || visited[parent]){
					success = false;
					break;
				}
				visited[parent] = true;
				parent = pBones[parent].GetParent();
			}
			
			if(!success){
				break;
			}
		}
	}
	
	// finished
	return success;
}

void deOcclusionMesh::Prepare(){
}



// Bones
//////////

int deOcclusionMesh::IndexOfBoneNamed(const char *name) const{
	return pBones.IndexOfMatching([&](const deOcclusionMeshBone &bone) {
		return bone.GetName() == name;
	});
}

bool deOcclusionMesh::HasBoneNamed(const char *name) const{
	return pBones.HasMatching([&](const deOcclusionMeshBone &bone) {
		return bone.GetName() == name;
	});
}



// Faces
//////////

void deOcclusionMesh::SetDoubleSidedFaceCount(int count){
	DEASSERT_TRUE(count >= 0)
	
	pDoubleSidedFaceCount = count;
}



// System Peers
/////////////////

void deOcclusionMesh::SetPeerGraphic(deBaseGraphicOcclusionMesh *peer){
	if(pPeerGraphic){
		delete pPeerGraphic;
	}
	
	pPeerGraphic = peer;
}
