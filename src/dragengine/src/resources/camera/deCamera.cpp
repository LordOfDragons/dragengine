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

#include "deCamera.h"
#include "deCameraManager.h"
#include "../effect/deEffect.h"
#include "../effect/deEffectChain.h"
#include "../world/deWorld.h"
#include "../../common/exceptions.h"
#include "../../systems/deGraphicSystem.h"
#include "../../systems/modules/graphic/deBaseGraphicCamera.h"



// Class deCamera
///////////////////

// Constructor, destructor
////////////////////////////

deCamera::deCamera(deCameraManager *manager) :
deResource(manager),

pFov(DEG2RAD * 90.0f),
pFovRatio(1.0f),
pImageDistance(0.01f),
pViewDistance(200.0f),

pEnableHDRR(true),
pExposure(1.0f),
pLowestIntensity(1.0f),
pHighestIntensity(1.0f),
pAdaptionTime(0.1f),

pEnableGI(false),

pWhiteIntensity(2.0f),
pBloomIntensity(1.0f),
pBloomStrength(0.25f),
pBloomBlend(1.0f),
pBloomSize(0.25f),

pEffects(nullptr),

pPeerGraphic(nullptr),

pParentWorld(nullptr),
pLLWorldPrev(nullptr),
pLLWorldNext(nullptr)
{
	pLayerMask.SetBit(0);
	
	try{
		pEffects = new deEffectChain;
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deCamera::~deCamera(){
	pCleanUp();
}



// Management
///////////////

void deCamera::SetPosition(const decDVector &position){
	if(position.IsEqualTo(pPosition)){
		return;
	}
	
	pPosition = position;
	
	if(pPeerGraphic){
		pPeerGraphic->PositionChanged();
	}
}

void deCamera::SetOrientation(const decQuaternion &orientation){
	if(orientation.IsEqualTo(pOrientation)){
		return;
	}
	
	pOrientation = orientation;
	
	if(pPeerGraphic){
		pPeerGraphic->OrientationChanged();
	}
}

void deCamera::SetFov(float fov){
	fov = decMath::clamp(fov, 0.0f, PI);
	
	if(fabs(fov - pFov) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pFov = fov;
	
	if(pPeerGraphic){
		pPeerGraphic->ParameterChanged();
	}
}

void deCamera::SetFovRatio(float ratio){
	ratio = decMath::max(ratio, 0.01f);
	
	if(fabs(ratio - pFovRatio) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pFovRatio = ratio;
	
	if(pPeerGraphic){
		pPeerGraphic->ParameterChanged();
	}
}

void deCamera::SetImageDistance(float distance){
	distance = decMath::max(distance, 1e-4f);
	
	if(fabs(distance - pImageDistance) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pImageDistance = distance;
	
	if(pPeerGraphic){
		pPeerGraphic->ParameterChanged();
	}
}

void deCamera::SetViewDistance(float distance){
	distance = decMath::max(distance, 1e-3f);
	
	if(fabs(distance - pViewDistance) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pViewDistance = distance;
	
	if(pPeerGraphic){
		pPeerGraphic->ParameterChanged();
	}
}



void deCamera::SetEnableHDRR(bool enable){
	if(enable == pEnableHDRR){
		return;
	}
	
	pEnableHDRR = enable;
	
	if(pPeerGraphic){
		pPeerGraphic->ParameterChanged();
	}
}

void deCamera::SetExposure(float exposure){
	exposure = decMath::max(exposure, 0.0f);
	
	if(fabs(exposure - pExposure) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pExposure = exposure;
	
	if(pPeerGraphic){
		pPeerGraphic->AdaptionChanged();
	}
}

void deCamera::SetLowestIntensity(float lowestIntensity){
	lowestIntensity = decMath::max(lowestIntensity, 0.0f);
	
	if(fabs(lowestIntensity - pLowestIntensity) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pLowestIntensity = lowestIntensity;
	
	if(pPeerGraphic){
		pPeerGraphic->AdaptionChanged();
	}
}

void deCamera::SetHighestIntensity(float highestIntensity){
	highestIntensity = decMath::max(highestIntensity, 0.0f);
	
	if(fabs(highestIntensity - pHighestIntensity) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pHighestIntensity = highestIntensity;
	
	if(pPeerGraphic){
		pPeerGraphic->AdaptionChanged();
	}
}

void deCamera::SetAdaptionTime(float adaptionTime){
	adaptionTime = decMath::max(adaptionTime, 0.0f);
	
	if(fabs(adaptionTime - pAdaptionTime) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pAdaptionTime = adaptionTime;
	
	if(pPeerGraphic){
		pPeerGraphic->AdaptionChanged();
	}
}



void deCamera::SetEnableGI(bool enable){
	if(enable == pEnableGI){
		return;
	}
	
	pEnableGI = enable;
	
	if(pPeerGraphic){
		pPeerGraphic->ParameterChanged();
	}
}



void deCamera::SetWhiteIntensity(float intensity){
	intensity = decMath::max(intensity, 0.01f);
	
	if(fabs(intensity - pWhiteIntensity) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pWhiteIntensity = intensity;
	
	if(pPeerGraphic){
		pPeerGraphic->AdaptionChanged();
	}
}

void deCamera::SetBloomIntensity(float intensity){
	intensity = decMath::max(intensity, 0.0f);
	
	if(fabs(intensity - pBloomIntensity) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pBloomIntensity = intensity;
	
	if(pPeerGraphic){
		pPeerGraphic->AdaptionChanged();
	}
}

void deCamera::SetBloomStrength(float strength){
	strength = decMath::max(strength, 0.0f);
	
	if(fabs(strength - pBloomStrength) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pBloomStrength = strength;
	
	if(pPeerGraphic){
		pPeerGraphic->AdaptionChanged();
	}
}

void deCamera::SetBloomBlend(float blend){
	blend = decMath::clamp(blend, 0.0f, 1.0f);
	
	if(fabs(blend - pBloomBlend) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pBloomBlend = blend;
	
	if(pPeerGraphic){
		pPeerGraphic->AdaptionChanged();
	}
}

void deCamera::SetBloomSize(float size){
	size = decMath::clamp(size, 0.0f, 1.0f);
	
	if(fabs(size - pBloomSize) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pBloomSize = size;
	
	if(pPeerGraphic){
		pPeerGraphic->AdaptionChanged();
	}
}



void deCamera::SetToneMapCurve(const decCurveBezier &curve){
	if(curve == pToneMapCurve){
		return;
	}
	
	pToneMapCurve = curve;
	
	if(pPeerGraphic){
		pPeerGraphic->AdaptionChanged();
	}
}



void deCamera::NotifyLayerMaskChanged(){
	if(pPeerGraphic){
		pPeerGraphic->LayerMaskChanged();
	}
}

void deCamera::ResetAdaptedIntensity(){
	if(pPeerGraphic){
		pPeerGraphic->ResetAdaptedIntensity();
	}
}



// Effects
////////////

int deCamera::GetEffectCount() const{
	return pEffects->GetEffectCount();
}

deEffect *deCamera::GetEffectAt(int index) const{
	return pEffects->GetEffectAt(index);
}

void deCamera::AddEffect(deEffect::Ref effect){
	pEffects->AddEffect(effect);
	
	if(pPeerGraphic){
		pPeerGraphic->EffectAdded(pEffects->GetEffectCount() - 1, effect);
	}
}

void deCamera::RemoveEffect(deEffect::Ref effect){
	const int index = pEffects->IndexOfEffect(effect);
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	try{
		pEffects->RemoveEffect(effect);
		
		if(pPeerGraphic){
			pPeerGraphic->EffectRemoved(index, effect);
		}
		
	}catch(const deException &){
		throw;
	}
}

void deCamera::RemoveAllEffects(){
	pEffects->RemoveAllEffects();
	
	if(pPeerGraphic){
		pPeerGraphic->AllEffectsRemoved();
	}
}



// System Peers
/////////////////

void deCamera::SetPeerGraphic(deBaseGraphicCamera *peer){
	if(peer == pPeerGraphic){
		return;
	}
	
	if(pPeerGraphic){
		delete pPeerGraphic;
	}
	
	pPeerGraphic = peer;
}



// Linked List
////////////////

void deCamera::SetParentWorld(deWorld *world){
	pParentWorld = world;
}

void deCamera::SetLLWorldPrev(deCamera *camera){
	pLLWorldPrev = camera;
}

void deCamera::SetLLWorldNext(deCamera *camera){
	pLLWorldNext = camera;
}



// Private Functions
//////////////////////

void deCamera::pCleanUp(){
	if(pPeerGraphic){
		delete pPeerGraphic;
		pPeerGraphic = nullptr;
	}
	
	if(pEffects){
		delete pEffects;
	}
}
