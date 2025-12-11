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

#include "deoalAComponent.h"
#include "deoalComponent.h"
#include "deoalComponentDebug.h"
#include "deoalComponentTexture.h"
#include "../deAudioOpenAL.h"
#include "../devmode/deoalDevMode.h"
#include "../model/deoalModel.h"
#include "../model/deoalAModel.h"
#include "../skin/deoalSkin.h"
#include "../skin/deoalSkinTexture.h"
#include "../world/deoalWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentTexture.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/skin/deSkin.h>



// Class deoalComponent
/////////////////////////

// Constructor, destructor
////////////////////////////

deoalComponent::deoalComponent(deAudioOpenAL &oal, deComponent &component) :
pOal(oal),
pComponent(component),
pTextures(NULL),
pTextureCount(0),
pParentWorld(NULL),
pModel(NULL),
pSkin(NULL),
pAudioModel(NULL),

pDirtyComponent(true),
pDirtyAffectsSound(true),
pDirtyOctreeNode(true),
pDirtyGeometry(true),
pDirtyModel(true),
pDirtySkin(true),
pDirtyRig(true),
pDirtyDynamicSkin(true),
pDirtyLayerMask(true),
pDirtyBones(true),
pDirtyBonesGeometry(true),
pDirtyTextures(true),

pDynamicSkinRequiresSync(true),
pTextureDynamicSkinRequiresSync(true),

pDebug(NULL),

pLLSyncWorld(this)
{
	(void)pDirtyRig; // not used yet
	(void)pDirtyDynamicSkin; // not used yet
	
	try{
		pAComponent.TakeOverWith(oal.GetAudioThread());
		
		ModelAndSkinChanged();
		AudioModelChanged();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoalComponent::~deoalComponent(){
	pCleanUp();
}



// Management
///////////////

void deoalComponent::SetParentWorld(deoalWorld *world){
	if(world == pParentWorld){
		return;
	}
	
	pParentWorld = world;
	
	pDirtyOctreeNode = true;
	
	if(pDebug){
		pDebug->DirtyFaces();
	}
}



void deoalComponent::Synchronize(){
	pSyncModel();
	pSyncSkin();
// 	if( pDirtyRig ){
// 		pAComponent->RigChanged();
// 		pDirtyRig = false;
// 	}
	pSyncTextures();
	
	if(pDirtyComponent){
		pDirtyComponent = false;
	}
	
	if(pDirtyBones){
		pAComponent->InitBones(pComponent);
		pDirtyBones = false;
	}
	if(pDirtyBonesGeometry){
		pComponent.WaitAnimatorTaskFinished();
		pAComponent->UpdateBoneGeometry(pComponent);
		pDirtyBonesGeometry = false;
	}
	
	if(pDirtyGeometry){
		pAComponent->SetGeometry(pComponent.GetPosition(),
			pComponent.GetOrientation(), pComponent.GetScaling());
		pDirtyGeometry = false;
	}
	
	if(pDirtyLayerMask){
		pAComponent->SetLayerMask(pComponent.GetLayerMask());
		pDirtyLayerMask = false;
	}
	
	if(pDirtyAffectsSound){
		pAComponent->UpdateAffectsSound();
		pDirtyAffectsSound = false;
	}
	
	if(pDirtyOctreeNode){
		// depends on affected sound
		pAComponent->UpdateOctreeNode();
		pDirtyOctreeNode = false;
	}
	
	pSyncDebug();
}

void deoalComponent::SetOctreeDirty(){
	pDirtyOctreeNode = true;
}

void deoalComponent::DevModeChanged(){
	if(pDebug){
		pDebug->DirtyFaces();
	}
	
	pRequiresSync();
}



deoalComponentTexture &deoalComponent::GetTextureAt(int index) const{
	if(index < 0 || index >= pTextureCount){
		DETHROW(deeInvalidParam);
	}
	return *pTextures[index];
}



void deoalComponent::DynamicSkinRequiresSync(){
	pDynamicSkinRequiresSync = true;
	pRequiresSync();
}

void deoalComponent::TextureDynamicSkinRequiresSync(){
	pTextureDynamicSkinRequiresSync = true;
	pRequiresSync();
}

void deoalComponent::DropDynamicSkin(){
// 	pDynamicSkin = NULL;
}



// Notifications
//////////////////

void deoalComponent::PositionChanged(){
	pDirtyGeometry = true;
	pDirtyOctreeNode = true;
	
	pRequiresSync();
}

void deoalComponent::ScalingChanged(){
	pDirtyGeometry = true;
	pDirtyOctreeNode = true;
	
	pRequiresSync();
}

void deoalComponent::OrientationChanged(){
	pDirtyGeometry = true;
	pDirtyOctreeNode = true;
	
	pRequiresSync();
}

void deoalComponent::ModelChanged(){
	pDropTextures();
	pCreateTextures();
	
	pDirtyModel = true;
	pDirtyTextures = true;
	
	pDirtyOctreeNode = true;
	
	if(pDebug){
		pDebug->DirtyFaces();
	}
	
	pRequiresSync();
}

void deoalComponent::RigChanged(){
	pDirtyBones = true;
	pDirtyBonesGeometry = true;
	
	if(pDebug){
		pDebug->DirtyFaces();
	}
	
	pRequiresSync();
}

void deoalComponent::SkinChanged(){
	int i;
	for(i=0; i<pTextureCount; i++){
		pTextures[i]->MarkDirty();
	}
	
	pDirtySkin = true;
	pDirtyAffectsSound = true;
	
	pRequiresSync();
}

void deoalComponent::ModelAndSkinChanged(){
	pDropTextures();
	pCreateTextures();
	
	pDirtyModel = true;
	pDirtySkin = true;
	pDirtyTextures = true;
	
	pDirtyOctreeNode = true;
	pDirtyAffectsSound = true;
	
	if(pDebug){
		pDebug->DirtyFaces();
	}
	
	pRequiresSync();
}

void deoalComponent::AudioModelChanged(){
	pDirtyModel = true;
	
	if(pDebug){
		pDebug->DirtyFaces();
	}
	
	pRequiresSync();
}

void deoalComponent::VisibilityChanged(){
}

void deoalComponent::ExtendsChanged(){
}

void deoalComponent::MeshDirty(){
	pDirtyBonesGeometry = true;
	
	if(pDebug){
		pDebug->DirtyFaces();
	}
	
	pRequiresSync();
}

void deoalComponent::LayerMaskChanged(){
	pDirtyLayerMask = true;
	pDirtyAffectsSound = true;
	pDirtyOctreeNode = true;
	
	if(pDebug){
		pDebug->DirtyFaces();
	}
	
	pRequiresSync();
}

void deoalComponent::ParametersChanged(){
// 	pDirtyComponent = true;
// 	
// 	pRequiresSync();
}

void deoalComponent::TextureChanged(int index, deComponentTexture &texture){
	if(index < 0 || index >= pTextureCount){
		DETHROW(deeInvalidParam);
	}
	
	pTextures[index]->TextureChanged(texture);
	
	pTextureDynamicSkinRequiresSync = true;
	pDirtyOctreeNode = true;
	pDirtyAffectsSound = true;
	
	pRequiresSync();
}

void deoalComponent::DynamicSkinChanged(){
}



// Private Functions
//////////////////////

void deoalComponent::pCleanUp(){
	if(pDebug){
		delete pDebug;
		pDebug = NULL;
	}
	
	pDropTextures();
}



void deoalComponent::pDropTextures(){
	if(!pTextures){
		return;
	}
	
	while(pTextureCount > 0){
		pTextureCount--;
		delete pTextures[pTextureCount];
	}
	delete [] pTextures;
	pTextures = NULL;
	pTextureCount = 0;
}

void deoalComponent::pCreateTextures(){
	const deModel * const model = pComponent.GetModel();
	if(!model || model->GetTextureCount() == 0){
		return;
	}
	
	const int textureCount = model->GetTextureCount();
	
	pTextures = new deoalComponentTexture*[textureCount];
	
	for(pTextureCount=0; pTextureCount<textureCount; pTextureCount++){
		pTextures[pTextureCount] = new deoalComponentTexture(*this, pTextureCount);
	}
}



void deoalComponent::pSyncModel(){
	if(!pDirtyModel){
		return;
	}
	
	const decStringList *textureNames = NULL;
	
	if(pComponent.GetModel()){
		pModel = (deoalModel*)pComponent.GetModel()->GetPeerAudio();
		textureNames = &pModel->GetAModel()->GetTextureNames();
		
	}else{
		pModel = NULL;
	}
	
	if(pComponent.GetAudioModel()){
		pAudioModel = (deoalModel*)pComponent.GetAudioModel()->GetPeerAudio();
		
	}else{
		pAudioModel = NULL;
	}
	
	if(pAudioModel){
		pAComponent->SetModel(pAudioModel->GetAModel(), textureNames);
		
	}else{
// 		pAComponent->SetModel( pModel->GetAModel(), textureNames );
		pAComponent->SetModel(NULL, textureNames);
	}
	
	pDirtyModel = false;
}

void deoalComponent::pSyncSkin(){
	if(!pDirtySkin){
		return;
	}
	
	if(pComponent.GetSkin()){
		pSkin = (deoalSkin*)pComponent.GetSkin()->GetPeerAudio();
		pAComponent->SetSkin(pSkin->GetASkin());
		
	}else{
		pSkin = NULL;
		pAComponent->SetSkin(NULL);
	}
	
	pDirtySkin = false;
}

void deoalComponent::pSyncDynamicSkin(){
	// TODO
}

void deoalComponent::pSyncTextures(){
	int i;
	
	if(pDirtyTextures){
		pAComponent->RemoveAllTextures();
		for(i=0; i<pTextureCount; i++){
			pAComponent->AddTexture(pTextures[i]->GetATexture());
		}
		
		pDirtyTextures = false;
	}
	
	for(i=0; i<pTextureCount; i++){
		pTextures[i]->Synchronize();
	}
}



void deoalComponent::pSyncDebug(){
	// NOTE affects sound is updated during synchronization time and during audio thread time
	
	if(pOal.GetDevMode()->GetEnabled() && pOal.GetDevMode()->GetShowAudioModels()){
		if(!pDebug){
			pDebug = new deoalComponentDebug(*this);
		}
		pDebug->Update();
		
	}else if(pDebug){
		delete pDebug;
		pDebug = NULL;
	}
}


void deoalComponent::pRequiresSync(){
	if(!pLLSyncWorld.GetList() && pParentWorld){
		pParentWorld->AddSyncComponent(this);
	}
}
