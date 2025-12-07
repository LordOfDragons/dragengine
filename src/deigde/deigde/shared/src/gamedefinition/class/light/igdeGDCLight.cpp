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

#include "igdeGDCLight.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDCLight
///////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCLight::igdeGDCLight() :
pType(deLight::eltPoint),
pColor(1.0f, 1.0f, 1.0f),
pIntensity(1.0f),
pRange(10.0f),
pHalfIntensityDistance(0.1f),
pAmbientRatio(0.0f),
pSpotAngle(30.0f),
pSpotRatio(1.0f),
pSpotSmoothness(1.0f),
pSpotExponent(1.0f),
pActivated(true),
pCastShadows(true),
pHintLightImportance(100),
pHintShadowImportance(100),
pHintMovement(deLight::emhStationary),
pHintParameter(deLight::ephStatic){
}

igdeGDCLight::igdeGDCLight(const igdeGDCLight &light) :
pType(light.pType),
pColor(light.pColor),
pIntensity(light.pIntensity),
pPosition(light.pPosition),
pOrientation(light.pOrientation),
pBoneName(light.pBoneName),
pRange(light.pRange),
pHalfIntensityDistance(light.pHalfIntensityDistance),
pAmbientRatio(light.pAmbientRatio),
pSpotAngle(light.pSpotAngle),
pSpotRatio(light.pSpotRatio),
pSpotSmoothness(light.pSpotSmoothness),
pSpotExponent(light.pSpotExponent),
pActivated(light.pActivated),
pCastShadows(light.pCastShadows),
pHintLightImportance(light.pHintLightImportance),
pHintShadowImportance(light.pHintShadowImportance),
pHintMovement(light.pHintMovement),
pHintParameter(light.pHintParameter),
pLightSkinPath(light.pLightSkinPath)
{
	int i;
	for(i=0; i<=epAttachRotation; i++){
		pPropertyNames[i] = light.pPropertyNames[i];
	}
	for(i=0; i<=etActivated; i++){
		pTriggerNames[i] = light.pTriggerNames[i];
	}
}

igdeGDCLight::~igdeGDCLight(){
}



// Management
///////////////

void igdeGDCLight::SetType(deLight::eLightTypes type){
	if(type < deLight::eltPoint || type > deLight::eltProjector){
		DETHROW(deeInvalidParam);
	}
	pType = type;
}

void igdeGDCLight::SetColor(const decColor &color){
	pColor = color;
}

void igdeGDCLight::SetIntensity(float intensity){
	pIntensity = decMath::max(intensity, 0.0f);
}

void igdeGDCLight::SetRange(float range){
	pRange = decMath::max(range, 0.0f);
}

void igdeGDCLight::SetHalfIntensityDistance(float distance){
	pHalfIntensityDistance = decMath::clamp(distance, 0.01f, 0.99f);
}

void igdeGDCLight::SetAmbientRatio(float ratio){
	pAmbientRatio = decMath::clamp(ratio, 0.0f, 1.0f);
}

void igdeGDCLight::SetPosition(const decVector &position){
	pPosition = position;
}

void igdeGDCLight::SetOrientation(const decQuaternion &orientation){
	pOrientation = orientation;
}

void igdeGDCLight::SetBoneName(const char *boneName){
	pBoneName = boneName;
}

void igdeGDCLight::SetSpotAngle(float angle){
	pSpotAngle = decMath::clamp(angle, 0.0f, 179.0f);
}

void igdeGDCLight::SetSpotRatio(float ratio){
	pSpotRatio = decMath::max(ratio, 0.0f);
}

void igdeGDCLight::SetSpotSmoothness(float smoothness){
	pSpotSmoothness = decMath::clamp(smoothness, 0.0f, 1.0f);
}

void igdeGDCLight::SetSpotExponent(float exponent){
	pSpotExponent = decMath::max(exponent, 0.0f);
}

void igdeGDCLight::SetActivated(bool activated){
	pActivated = activated;
}

void igdeGDCLight::SetCastShadows(bool castShadows){
	pCastShadows = castShadows;
}

void igdeGDCLight::SetHintLightImportance(int importance){
	pHintLightImportance = decMath::clamp(importance, 0, 100);
}

void igdeGDCLight::SetHintShadowImportance(int importance){
	pHintShadowImportance = decMath::clamp(importance, 0, 100);
}

void igdeGDCLight::SetHintMovement(deLight::eMovementHints hint){
	if(hint < deLight::emhStationary || hint > deLight::emhDynamic){
		DETHROW(deeInvalidParam);
	}
	pHintMovement = hint;
}

void igdeGDCLight::SetHintParameter(deLight::eParameterHints hint){
	if(hint < deLight::ephStatic || hint > deLight::ephDynamic){
		DETHROW(deeInvalidParam);
	}
	pHintParameter = hint;
}

void igdeGDCLight::SetLightSkinPath(const char *path){
	pLightSkinPath = path;
}



bool igdeGDCLight::IsPropertySet(eProperties property) const{
	return ! pPropertyNames[property].IsEmpty();
}

const decString &igdeGDCLight::GetPropertyName(eProperties property) const{
	return pPropertyNames[property];
}

void igdeGDCLight::SetPropertyName(eProperties property, const char *name){
	if(! name){
		DETHROW(deeInvalidParam);
	}
	pPropertyNames[property] = name;
}

bool igdeGDCLight::HasPropertyWithName(const char *name) const{
	int i;
	
	for(i=0; i<=epAttachRotation; i++){
		if(pPropertyNames[i] == name){
			return true;
		}
	}
	
	return false;
}



bool igdeGDCLight::IsTriggerSet(eTriggers trigger) const{
	return ! pTriggerNames[trigger].IsEmpty();
}

const decString &igdeGDCLight::GetTriggerName(eTriggers trigger) const{
	return pTriggerNames[trigger];
}

void igdeGDCLight::SetTriggerName(eTriggers trigger, const char *name){
	pTriggerNames[trigger] = name;
}
