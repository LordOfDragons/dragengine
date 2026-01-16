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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deModel.h"
#include "deModelBone.h"
#include "deModelTexture.h"
#include "deModelLOD.h"
#include "deModelVertex.h"
#include "deModelFace.h"
#include "deModelManager.h"
#include "deModelWeight.h"
#include "deModelTextureCoordinatesSet.h"
#include "deModelVertexPositionSet.h"
#include "deModelLodVertexPositionSet.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/audio/deBaseAudioModel.h"
#include "../../systems/modules/graphic/deBaseGraphicModel.h"
#include "../../systems/modules/physics/deBasePhysicsModel.h"



// Class deModel
//////////////////

// Constructor, destructor
////////////////////////////

deModel::deModel(deModelManager *manager, deVirtualFileSystem *vfs, const char *filename,
	TIME_SYSTEM modificationTime) :
deFileResource(manager, vfs, filename, modificationTime),

pPeerGraphic (nullptr),
pPeerPhysics (nullptr),
pPeerAudio (nullptr){
}

deModel::~deModel(){
	if(pPeerAudio){
		delete pPeerAudio;
		pPeerAudio = nullptr;
	}
	if(pPeerPhysics){
		delete pPeerPhysics;
		pPeerPhysics = nullptr;
	}
	if(pPeerGraphic){
		delete pPeerGraphic;
		pPeerGraphic = nullptr;
	}
}



// Management
///////////////

bool deModel::Verify(){
	bool *visited = nullptr;
	int i, j, parent;
	bool success = true;
	
	// verify there is at least one LOD mesh
	if(pLODs.GetCount() < 1){
		return false;
	}
	
	// verify that all bones are free of cyclic parentship
	const int boneCount = pBones.GetCount();
	if(boneCount > 0){
		try{
			visited = new bool[boneCount];
			
			// check bone parents are valid
			for(i=0; i<boneCount; i++){
				for(j=0; j<boneCount; j++){
					visited[j] = false;
				}
				
				parent = pBones.GetAt(i)->GetParent();
				while(parent != -1){
					if(parent < -1 || parent >= boneCount || visited[parent]){
						success = false;
						break;
					}
					visited[parent] = true;
					parent = pBones.GetAt(parent)->GetParent();
				}
				
				if(!success){
					break;
				}
			}
			
			// clean up
			delete [] visited;
			
		}catch(const deException &){
			if(visited){
				delete [] visited;
			}
			throw;
		}
	}
	
	// verify that each lod level has the correct number of texture coordinates sets and texture coordinates per set
	const int texCoordSetCount = pTextureCoordinatesSetList.GetCount();
	
	if(texCoordSetCount < 1){
		return false;
	}
	
	const int lodCount = pLODs.GetCount();
	for(i=0; i<lodCount; i++){
		const int texCoordCount = pLODs.GetAt(i)->GetTextureCoordinatesCount();
		
		if(pLODs.GetAt(i)->GetTextureCoordinatesSetCount() != texCoordSetCount){
			return false;
		}
		
		
		for(j=0; j<texCoordSetCount; j++){
			if(pLODs.GetAt(i)->GetTextureCoordinatesSetAt(j).GetTextureCoordinates().GetCount() != texCoordCount){
				return false;
			}
		}
	}
	
	// verify each lod level has the correct count of vertex position sets
	const int vertexPositionSetCount = pVertexPositionSets.GetCount();
	for(i=0; i<lodCount; i++){
		if(pLODs.GetAt(i)->GetVertexPositionSetCount() != vertexPositionSetCount){
			return false;
		}
	}
	
	// finished
	return success;
}

void deModel::Prepare(){
	pCalcBoneMatrices();
}



// Bones
//////////

int deModel::IndexOfBoneNamed(const char *name) const{
	return pBones.IndexOfMatching([&](const deModelBone &bone){
		return bone.GetName() == name;
	});
}

bool deModel::HasBoneNamed(const char *name) const{
	return pBones.HasMatching([&](const deModelBone &bone){
		return bone.GetName() == name;
	});
}

void deModel::AddBone(deTUniqueReference<deModelBone> &&bone){
	if(!bone){
		DETHROW(deeInvalidParam);
	}
	pBones.Add(std::move(bone));
}



// Textures
/////////////

int deModel::IndexOfTextureNamed(const char *name) const{
	return pTextures.IndexOfMatching([&](const deModelTexture &texture){
		return texture.GetName() == name;
	});
}

bool deModel::HasTextureNamed(const char *name) const{
	return pTextures.HasMatching([&](const deModelTexture &texture){
		return texture.GetName() == name;
	});
}

void deModel::AddTexture(deTUniqueReference<deModelTexture> &&texture){
	if(!texture){
		DETHROW(deeInvalidParam);
	}
	pTextures.Add(std::move(texture));
}



// LODs
/////////

void deModel::AddLOD(deTUniqueReference<deModelLOD> &&lod){
	if(!lod){
		DETHROW(deeInvalidParam);
	}
	pLODs.Add(std::move(lod));
}



// Vertex position sets
/////////////////////////

int deModel::IndexOfVertexPositionSetNamed(const char *name) const{
	return pVertexPositionSets.IndexOfMatching([&](const deModelVertexPositionSet &set){
		return set.GetName() == name;
	});
}

bool deModel::HasVertexPositionSetNamed(const char *name) const{
	return pVertexPositionSets.HasMatching([&](const deModelVertexPositionSet &set){
		return set.GetName() == name;
	});
}

void deModel::AddVertexPositionSet(deTUniqueReference<deModelVertexPositionSet> &&set){
	DEASSERT_NOTNULL(set)
	
	pVertexPositionSets.Add(std::move(set));
}



// System Peers
/////////////////

void deModel::SetPeerGraphic(deBaseGraphicModel *peer){
	if(peer == pPeerGraphic){
		return;
	}
	
	deBaseGraphicModel * const deletePeer = pPeerGraphic;
	
	pPeerGraphic = peer;
	
	if(deletePeer){
		delete deletePeer;
	}
}

void deModel::SetPeerAudio (deBaseAudioModel *peer){
	if(peer == pPeerAudio){
		return;
	}
	
	deBaseAudioModel * const deletePeer = pPeerAudio;
	
	pPeerAudio = peer;
	
	if(deletePeer){
		delete deletePeer;
	}
}

void deModel::SetPeerPhysics(deBasePhysicsModel *peer){
	if(peer == pPeerPhysics){
		return;
	}
	
	deBasePhysicsModel * const deletePeer = pPeerPhysics;
	
	pPeerPhysics = peer;
	
	if(deletePeer){
		delete deletePeer;
	}
}



// Private Functions
//////////////////////

void deModel::pCalcBoneMatrices(){
	const int boneCount = pBones.GetCount();
	if(boneCount == 0){
		return;
	}
	
	bool *calculated = nullptr;
	int i;
	
	try{
		// create a temporary array to hold the calculated status
		calculated = new bool[boneCount];
		for(i=0; i<boneCount; i++){
			calculated[i] = false;
		}
		
		// keeps track of the count of bones in need of calculation
		int remaining = boneCount;
		
		// loop until there are no more remaining bones
		while(remaining > 0){
			// loop over all bones and calculate those not done so yet
			for(i=0; i<boneCount; i++){
				// if calculated skip the bone
				if(calculated[i]){
					continue;
				}
				deModelBone &bone = *pBones.GetAt(i);
				
				// check if the parent if present is calculated
				const int parent = bone.GetParent();
				if(parent != -1 && !calculated[parent]){
					continue;
				}
				
				// calculate the matrix
				decMatrix matrix(decMatrix::CreateWorld(bone.GetPosition(), bone.GetOrientation()));
				if(parent != -1){
					matrix *= pBones.GetAt(parent)->GetMatrix();
				}
				bone.SetMatrix(matrix);
				
				// mark as calculate and decrease count of remaining bones by one
				calculated[i] = true;
				remaining--;
			}
		}
		
		// free temporary array
		delete [] calculated;
		
	}catch(const deException &){
		if(calculated){
			delete [] calculated;
		}
		throw;
	}
}
