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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deEffectOverlayImage.h"
#include "deEffectFilterKernel.h"
#include "deEffectColorMatrix.h"
#include "deEffectDistortImage.h"
#include "deEffectManager.h"
#include "../../systems/deGraphicSystem.h"
#include "../../systems/dePhysicsSystem.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"


// class deEffectManager
/////////////////////////////

// constructor, destructor
////////////////////////////

deEffectManager::deEffectManager(deEngine *engine) : deResourceManager(engine, ertEffect){
	SetLoggingName("effect");
}

deEffectManager::~deEffectManager(){
	ReleaseLeakingResources();
}



// management
///////////////

int deEffectManager::GetEffectCount() const{
	return pEffects.GetCount();
}

deEffect *deEffectManager::GetRootEffect() const{
	return (deEffect*)pEffects.GetRoot();
}

deEffectFilterKernel::Ref deEffectManager::CreateEffectFilterKernel(){
	const deEffectFilterKernel::Ref effect(deEffectFilterKernel::Ref::New(this));
	GetGraphicSystem()->LoadEffect(effect);
	//GetPhysicsSystem()->LoadEffect( effect );
	pEffects.Add(effect);
	return effect;
}

deEffectOverlayImage::Ref deEffectManager::CreateEffectOverlayImage(){
	const deEffectOverlayImage::Ref effect(deEffectOverlayImage::Ref::New(this));
	GetGraphicSystem()->LoadEffect(effect);
	pEffects.Add(effect);
	return effect;
}

deEffectColorMatrix::Ref deEffectManager::CreateEffectColorMatrix(){
	const deEffectColorMatrix::Ref effect(deEffectColorMatrix::Ref::New(this));
	GetGraphicSystem()->LoadEffect(effect);
	pEffects.Add(effect);
	return effect;
}

deEffectDistortImage::Ref deEffectManager::CreateEffectDistortImage(){
	const deEffectDistortImage::Ref effect(deEffectDistortImage::Ref::New(this));
	GetGraphicSystem()->LoadEffect(effect);
	pEffects.Add(effect);
	return effect;
}



void deEffectManager::ReleaseLeakingResources(){
	if(GetEffectCount() > 0){
		LogWarnFormat("%i leaking effects", GetEffectCount());
		pEffects.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// systems support
////////////////////

void deEffectManager::SystemGraphicLoad(){
	deEffect *effect = (deEffect*)pEffects.GetRoot();
	
	while(effect){
		if(!effect->GetPeerGraphic()){
			GetGraphicSystem()->LoadEffect(effect);
		}
		
		effect = (deEffect*)effect->GetLLManagerNext();
	}
}

void deEffectManager::SystemGraphicUnload(){
	deEffect *effect = (deEffect*)pEffects.GetRoot();
	
	while(effect){
		effect->SetPeerGraphic(NULL);
		effect = (deEffect*)effect->GetLLManagerNext();
	}
}
/*
void deEffectManager::SystemPhysicsLoad(){
	deEffect *effect;
	int i;
	for(i=0; i<pEffects.GetResourceCount(); i++){
		effect = (deEffect*)pEffects.GetResourceAt(i);
		if(effect->GetPhysicsEffect()) continue;
		GetPhysicsSystem()->LoadEffect(effect);
	}
}

void deEffectManager::SystemPhysicsUnload(){
	deEffect *effect;
	int i;
	for(i=0; i<pEffects.GetResourceCount(); i++){
		effect = (deEffect*)pEffects.GetResourceAt(i);
		effect->SetPhysicsEffect(NULL);
	}
}
*/
void deEffectManager::RemoveResource(deResource *resource){
	pEffects.RemoveIfPresent(resource);
}
