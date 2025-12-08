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

#include "gdeOCComponent.h"
#include "gdeOCComponentTexture.h"

#include <dragengine/common/exceptions.h>



// Class gdeOCComponent
/////////////////////////

// Constructor, destructor
////////////////////////////

gdeOCComponent::gdeOCComponent() :
pDoNotScale(false),
pStatic(true),
pRenderEnvMap(true),
pAffectsAudio(true),
pPartialHide(false),
pAttachTarget(true),
pLightShadowIgnore(false),
pColliderResponseType(deCollider::ertStatic){
}

gdeOCComponent::gdeOCComponent(const gdeOCComponent &component) :
pModelPath(component.pModelPath),
pSkinPath(component.pSkinPath),
pRigPath(component.pRigPath),
pAnimatorPath(component.pAnimatorPath),
pOccMeshPath(component.pOccMeshPath),
pAudioModelPath(component.pAudioModelPath),
pPlaybackController(component.pPlaybackController),
pAnimationPath(component.pAnimationPath),
pMove(component.pMove),
pDoNotScale(component.pDoNotScale),
pStatic(component.pStatic),
pRenderEnvMap(component.pRenderEnvMap),
pAffectsAudio(component.pAffectsAudio),
pPartialHide(component.pPartialHide),
pAttachTarget(component.pAttachTarget),
pLightShadowIgnore(component.pLightShadowIgnore),
pColliderResponseType(component.pColliderResponseType),
pPosition(component.pPosition),
pRotation(component.pRotation),
pBoneName(component.pBoneName)
{
	const int textureCount = component.pTextures.GetCount();
	gdeOCComponentTexture::Ref texture = NULL;
	int i;
	
	try{
		for(i=0; i<textureCount; i++){
			texture.TakeOver(new gdeOCComponentTexture(*component.pTextures.GetAt(i)));
			pTextures.Add(texture);
			texture = NULL;
		}
		
		for(i=0; i<=epMove; i++){
			pPropertyNames[i] = component.pPropertyNames[i];
		}
		
	}catch(const deException &){
		throw;
	}
}

gdeOCComponent::~gdeOCComponent(){
}



// Management
///////////////

void gdeOCComponent::SetModelPath(const char *path){
	pModelPath = path;
}

void gdeOCComponent::SetSkinPath(const char *path){
	pSkinPath = path;
}

void gdeOCComponent::SetRigPath(const char *path){
	pRigPath = path;
}

void gdeOCComponent::SetAnimatorPath(const char *path){
	pAnimatorPath = path;
}

void gdeOCComponent::SetAnimationPath(const char *path){
	pAnimationPath = path;
}

void gdeOCComponent::SetMove(const char *move){
	pMove = move;
}

void gdeOCComponent::SetOcclusionMeshPath(const char *path){
	pOccMeshPath = path;
}

void gdeOCComponent::SetAudioModelPath(const char *path){
	pAudioModelPath = path;
}

void gdeOCComponent::SetPlaybackController(const char *controller){
	pPlaybackController = controller;
}

void gdeOCComponent::SetDoNotScale(bool doNotScale){
	pDoNotScale = doNotScale;
}

void gdeOCComponent::SetStatic(bool bStatic){
	pStatic = bStatic;
}

void gdeOCComponent::SetPartialHide(bool partiallyHidden){
	pPartialHide = partiallyHidden;
}

void gdeOCComponent::SetAttachTarget(bool attachTarget){
	pAttachTarget = attachTarget;
}

void gdeOCComponent::SetColliderResponseType(deCollider::eResponseType responseType){
	pColliderResponseType = responseType;
}

void gdeOCComponent::SetRenderEnvMap(bool renderEnvMap){
	pRenderEnvMap = renderEnvMap;
}

void gdeOCComponent::SetAffectsAudio(bool affectsAudio){
	pAffectsAudio = affectsAudio;
}

void gdeOCComponent::SetLightShadowIgnore(bool lightShadowIgnore){
	pLightShadowIgnore = lightShadowIgnore;
}

void gdeOCComponent::SetPosition(const decVector &position){
	pPosition = position;
}

void gdeOCComponent::SetRotation(const decVector &orientation){
	pRotation = orientation;
}

void gdeOCComponent::SetBoneName(const char *boneName){
	pBoneName = boneName;
}



bool gdeOCComponent::IsPropertySet(eProperties property) const{
	return !pPropertyNames[property].IsEmpty();
}

const decString &gdeOCComponent::GetPropertyName(eProperties property) const{
	return pPropertyNames[property];
}

void gdeOCComponent::SetPropertyName(eProperties property, const char *name){
	pPropertyNames[property] = name;
}

bool gdeOCComponent::HasPropertyWithName(const char *name) const{
	int i;
	for(i=0; i<=epMove; i++){
		if(pPropertyNames[i] == name){
			return true;
		}
	}
	
	return false;
}



void gdeOCComponent::SetActiveTexture(gdeOCComponentTexture::Ref texture){
	if(texture == pActiveTexture){
		return;
	}
	pActiveTexture = texture;
}
