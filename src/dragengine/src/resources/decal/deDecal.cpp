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

#include "deDecal.h"
#include "deDecalManager.h"
#include "deDecalBoneState.h"
#include "../skin/deSkin.h"
#include "../skin/dynamic/deDynamicSkin.h"
#include "../component/deComponent.h"
#include "../component/deComponentBone.h"
#include "../../deEngine.h"
#include "../../systems/modules/graphic/deBaseGraphicDecal.h"
#include "../../systems/modules/physics/deBasePhysicsDecal.h"
#include "../../systems/modules/audio/deBaseAudioDecal.h"
#include "../../common/exceptions.h"



// Class deDecal
//////////////////

// Constructor, destructor
////////////////////////////

deDecal::deDecal(deDecalManager *manager) :
deResource(manager),

pSize(0.5f, 0.5f, 1.0f),

pTexture(0),

pVisible(true),

pBoneStates(NULL),
pBoneStateCount(0),

pParentComponent(NULL),
pLLComponentPrev(NULL),
pLLComponentNext(NULL),

pParentHeightTerrainSector(NULL),
pLLHeightTerrainSectorPrev(NULL),
pLLHeightTerrainSectorNext(NULL),

pPeerGraphic(NULL),
pPeerPhysics(NULL),
pPeerAudio(NULL){
}

deDecal::~deDecal(){
	pCleanUp();
}



// Management
///////////////

void deDecal::SetPosition(const decVector &position){
	if(position.IsEqualTo(pPosition)){
		return;
	}
	
	pPosition = position;
	
	if(pPeerGraphic){
		pPeerGraphic->GeometryChanged();
	}
	if(pPeerPhysics){
		pPeerPhysics->GeometryChanged();
	}
	if(pPeerAudio){
		pPeerAudio->GeometryChanged();
	}
}

void deDecal::SetOrientation(const decQuaternion &orientation){
	if(orientation.IsEqualTo(pOrientation)){
		return;
	}
	
	pOrientation = orientation;
	
	if(pPeerGraphic){
		pPeerGraphic->GeometryChanged();
	}
	if(pPeerPhysics){
		pPeerPhysics->GeometryChanged();
	}
	if(pPeerAudio){
		pPeerAudio->GeometryChanged();
	}
}

void deDecal::SetSize(const decVector &size){
	if(size.IsEqualTo(pSize)){
		return;
	}
	
	pSize = size;
	
	if(pPeerGraphic){
		pPeerGraphic->GeometryChanged();
	}
	if(pPeerPhysics){
		pPeerPhysics->GeometryChanged();
	}
	if(pPeerAudio){
		pPeerAudio->GeometryChanged();
	}
}

void deDecal::SetTransform(const decTexMatrix2 &matrix){
	if(matrix.IsEqualTo(pTransform)){
		return;
	}
	
	pTransform = matrix;
	
	if(pPeerGraphic){
		pPeerGraphic->TransformChanged();
	}
	if(pPeerPhysics){
		pPeerPhysics->TransformChanged();
	}
	if(pPeerAudio){
		pPeerAudio->TransformChanged();
	}
}



void deDecal::SetSkin(deSkin *skin){
	if(skin == pSkin){
		return;
	}
	
	pSkin = skin;
	
	if(pPeerGraphic){
		pPeerGraphic->SkinChanged();
	}
	if(pPeerPhysics){
		pPeerPhysics->SkinChanged();
	}
	if(pPeerAudio){
		pPeerAudio->SkinChanged();
	}
}

void deDecal::SetTexture(int texture){
	if(texture < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(texture == pTexture){
		return;
	}
	
	pTexture = texture;
	
	if(pPeerGraphic){
		pPeerGraphic->SkinChanged();
	}
	if(pPeerPhysics){
		pPeerPhysics->SkinChanged();
	}
	if(pPeerAudio){
		pPeerAudio->SkinChanged();
	}
}

void deDecal::SetDynamicSkin(deDynamicSkin *dynamicSkin){
	if(dynamicSkin == pDynamicSkin){
		return;
	}
	
	pDynamicSkin = dynamicSkin;
	
	if(pPeerGraphic){
		pPeerGraphic->DynamicSkinChanged();
	}
	if(pPeerPhysics){
		pPeerPhysics->DynamicSkinChanged();
	}
	if(pPeerAudio){
		pPeerAudio->DynamicSkinChanged();
	}
}



void deDecal::SetVisible(bool visible){
	if(visible == pVisible){
		return;
	}
	
	pVisible = visible;
	
	if(pPeerGraphic){
		pPeerGraphic->VisibleChanged();
	}
	if(pPeerPhysics){
		pPeerPhysics->VisibleChanged();
	}
	if(pPeerAudio){
		pPeerAudio->VisibleChanged();
	}
}



// Bone States
////////////////

deDecalBoneState &deDecal::GetBoneStateAt(int index){
	if(index < 0 || index >= pBoneStateCount){
		DETHROW(deeInvalidParam);
	}
	
	// if caching is used this has to be checked first
	//if( ! pBoneStates ){
	//	pRestoreBoneStates();
	//}
	
	return pBoneStates[index];
}

void deDecal::SetBoneStateCount(int count){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	if(count == pBoneStateCount){
		return;
	}
	
	deDecalBoneState *boneStates = NULL;
	if(count > 0){
		boneStates = new deDecalBoneState[count];
	}
	
	if(pBoneStates){
		delete [] pBoneStates;
	}
	pBoneStates = boneStates;
	
	pBoneStateCount = count;
}

void deDecal::SnapshotBoneStatesFrom(deComponent *component){
	if(! component){
		DETHROW(deeInvalidParam);
	}
	
	const int boneCount = component->GetBoneCount();
	int i;
	
	SetBoneStateCount(boneCount);
	
	for(i=0; i<boneCount; i++){
		const deComponentBone &bone = component->GetBoneAt(i);
		pBoneStates[i].SetPosition(bone.GetPosition());
		pBoneStates[i].SetRotation(bone.GetRotation());
		pBoneStates[i].SetSize(bone.GetScale());
	}
}



// Linked lists
/////////////////

void deDecal::SetParentComponent(deComponent *component){
	pParentComponent = component;
}

void deDecal::SetLLComponentNext(deDecal *decal){
	pLLComponentNext = decal;
}

void deDecal::SetLLComponentPrev(deDecal *decal){
	pLLComponentPrev = decal;
}



void deDecal::SetParentHeightTerrainSector(deHeightTerrainSector *sector){
	pParentHeightTerrainSector = sector;
}

void deDecal::SetLLHeightTerrainSectorNext(deDecal *decal){
	pLLHeightTerrainSectorNext = decal;
}

void deDecal::SetLLHeightTerrainSectorPrev(deDecal *decal){
	pLLHeightTerrainSectorPrev = decal;
}



// System Peers
/////////////////

void deDecal::SetPeerGraphic(deBaseGraphicDecal *peer){
	if(peer == pPeerGraphic){
		return;
	}
	
	if(pPeerGraphic){
		delete pPeerGraphic;
	}
	pPeerGraphic = peer;
}

void deDecal::SetPeerPhysics(deBasePhysicsDecal *peer){
	if(peer == pPeerPhysics){
		return;
	}
	
	if(pPeerPhysics){
		delete pPeerPhysics;
	}
	pPeerPhysics = peer;
}

void deDecal::SetPeerAudio(deBaseAudioDecal *peer){
	if(peer == pPeerAudio){
		return;
	}
	
	if(pPeerAudio){
		delete pPeerAudio;
	}
	pPeerAudio = peer;
}



// Private Functions
//////////////////////

void deDecal::pCleanUp(){
	if(pPeerGraphic){
		delete pPeerGraphic;
		pPeerGraphic = NULL;
	}
	if(pPeerPhysics){
		delete pPeerPhysics;
		pPeerPhysics = NULL;
	}
	if(pPeerAudio){
		delete pPeerAudio;
		pPeerAudio = NULL;
	}
	
	if(pBoneStates){
		delete [] pBoneStates;
	}
}

void deDecal::pRestoreBoneStates(){
	// not used yet, comes with caching
}
