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

#include "deLight.h"
#include "deLightManager.h"
#include "../deResourceManager.h"
#include "../canvas/deCanvasView.h"
#include "../component/deComponent.h"
#include "../skin/deSkin.h"
#include "../skin/dynamic/deDynamicSkin.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/graphic/deBaseGraphicLight.h"



// Class deLight
//////////////////

// Constructor, destructor
////////////////////////////

deLight::deLight(deLightManager *manager) :
deResource(manager),

pType(eltPoint),
pColor(1.0f, 1.0f, 1.0f),
pIntensity(1.0f),
pRange(10.0f),
pHalfIntensityDistance(0.1f),
pAmbientRatio(0.0f),

pSpotAngle(DEG2RAD * 30.0f),
pSpotRatio(1.0f),
pSpotSmoothness(1.0f),
pSpotExponent(1.0f),

pActivated(true),
pCastShadows(true),

pHintLightImportance(100),
pHintShadowImportance(100),
pHintMovement(emhStationary),
pHintParameter(ephStatic),

pPeerGraphic(NULL),

pParentWorld(NULL),
pLLWorldPrev(NULL),
pLLWorldNext(NULL){
}

deLight::~deLight(){
	if(pPeerGraphic){
		delete pPeerGraphic;
		pPeerGraphic = NULL;
	}
}



// Management
///////////////

void deLight::SetType(eLightTypes type){
	if(type < eltPoint || type > eltProjector){
		DETHROW(deeInvalidParam);
	}
	
	if(type == pType){
		return;
	}
	
	pType = type;
	
	if(pPeerGraphic){
		pPeerGraphic->TypeChanged();
	}
}

void deLight::SetColor(const decColor &color){
	if(color.IsEqualTo(pColor)){
		return;
	}
	
	pColor = color;
	
	if(pPeerGraphic){
		pPeerGraphic->LightParameterChanged();
	}
}

void deLight::SetIntensity(float intensity){
	intensity = decMath::max(intensity, 0.0f);
	
	if(fabsf(intensity - pIntensity) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pIntensity = intensity;
	
	if(pPeerGraphic){
		pPeerGraphic->LightParameterChanged();
	}
}

void deLight::SetRange(float range){
	range = decMath::max(range, 0.01f);
	
	if(fabsf(range - pRange) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pRange = range;
	
	if(pPeerGraphic){
		pPeerGraphic->GeometryParameterChanged();
	}
}

void deLight::SetHalfIntensityDistance(float distance){
	distance = decMath::clamp(distance, 0.01f, 0.99f);
	
	if(fabsf(distance - pHalfIntensityDistance) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pHalfIntensityDistance = distance;
	
	if(pPeerGraphic){
		pPeerGraphic->AttenuationChanged();
	}
}

void deLight::SetAmbientRatio(float ratio){
	ratio = decMath::clamp(ratio, 0.0f, 1.0f);
	
	if(fabsf(ratio - pAmbientRatio) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pAmbientRatio = ratio;
	
	if(pPeerGraphic){
		pPeerGraphic->LightParameterChanged();
	}
}

void deLight::SetPosition(const decDVector &position){
	if(position.IsEqualTo(pPosition)){
		return;
	}
	
	pPosition = position;
	
	if(pPeerGraphic){
		pPeerGraphic->PositionChanged();
	}
}

void deLight::SetOrientation(const decQuaternion &orientation){
	if(orientation.IsEqualTo(pOrientation)){
		return;
	}
	
	pOrientation = orientation;
	
	if(pPeerGraphic){
		pPeerGraphic->OrientationChanged();
	}
}

void deLight::SetSpotAngle(float angle){
	angle = decMath::clamp(angle, 0.0f, DEG2RAD * 179.0f);
	if(fabsf(angle - pSpotAngle) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pSpotAngle = angle;
	
	if(pPeerGraphic){
		pPeerGraphic->GeometryParameterChanged();
	}
}

void deLight::SetSpotRatio(float ratio){
	ratio = decMath::max(ratio, 0.0f);
	if(fabsf(ratio - pSpotRatio) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pSpotRatio = ratio;
	
	if(pPeerGraphic){
		pPeerGraphic->GeometryParameterChanged();
	}
}

void deLight::SetSpotSmoothness(float smoothness){
	smoothness = decMath::clamp(smoothness, 0.0f, 1.0f);
	if(fabsf(smoothness - pSpotSmoothness) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pSpotSmoothness = smoothness;
	
	if(pPeerGraphic){
		pPeerGraphic->LightParameterChanged();
	}
}

void deLight::SetSpotExponent(float exponent){
	exponent = decMath::max(exponent, 0.0f);
	
	if(fabsf(exponent - pSpotExponent) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pSpotExponent = exponent;
	
	if(pPeerGraphic){
		pPeerGraphic->LightParameterChanged();
	}
}

void deLight::SetShape(const decShapeList &shape){
	pShape = shape;
	
	if(pPeerGraphic){
		pPeerGraphic->ShapeChanged();
	}
}



void deLight::SetLightSkin(deSkin *skin){
	if(skin == pLightSkin){
		return;
	}
	
	pLightSkin = skin;
	
	if(pPeerGraphic){
		pPeerGraphic->SourceChanged();
	}
}

void deLight::SetLightCanvas(deCanvasView *canvas){
	if(canvas == pLightCanvas){
		return;
	}
	
	pLightCanvas = canvas;
	
	if(pPeerGraphic){
		pPeerGraphic->SourceChanged();
	}
}

void deLight::SetDynamicSkin(deDynamicSkin *dynamicSkin){
	if(dynamicSkin == pDynamicSkin){
		return;
	}
	
	pDynamicSkin = dynamicSkin;
	
	if(pPeerGraphic){
		pPeerGraphic->SourceChanged();
	}
}

void deLight::SetTransform(const decTexMatrix2 &matrix){
	if(! matrix.IsEqualTo(pTransform)){
		return;
	}
	
	pTransform = matrix;
	
	if(pPeerGraphic){
		pPeerGraphic->TransformChanged();
	}
}



void deLight::SetHintLightImportance(int importance){
	importance = decMath::clamp(importance, 0, 100);
	
	if(importance == pHintLightImportance){
		return;
	}
	
	pHintLightImportance = importance;
	
	if(pPeerGraphic){
		pPeerGraphic->HintChanged();
	}
}

void deLight::SetHintShadowImportance(int importance){
	importance = decMath::clamp(importance, 0, 100);
	
	if(importance == pHintShadowImportance){
		return;
	}
	
	pHintShadowImportance = importance;
	
	if(pPeerGraphic){
		pPeerGraphic->HintChanged();
	}
}

void deLight::SetHintMovement(eMovementHints hint){
	if(hint < emhStationary || hint > emhDynamic){
		DETHROW(deeInvalidParam);
	}
	
	if(hint == pHintMovement){
		return;
	}
	
	pHintMovement = hint;
	
	if(pPeerGraphic){
		pPeerGraphic->HintChanged();
	}
}

void deLight::SetHintParameter(eParameterHints hint){
	if(hint < ephStatic || hint > ephDynamic){
		DETHROW(deeInvalidParam);
	}
	
	if(hint == pHintParameter){
		return;
	}
	
	pHintParameter = hint;
	
	if(pPeerGraphic){
		pPeerGraphic->HintChanged();
	}
}

void deLight::SetCage(const decShapeList &cage){
	pCage = cage;
	
	if(pPeerGraphic){
		pPeerGraphic->HintChanged();
	}
}



void deLight::SetLayerMask(const decLayerMask &layerMask){
	if(layerMask == pLayerMask){
		return;
	}
	
	pLayerMask = layerMask;
	
	if(pPeerGraphic){
		pPeerGraphic->LayerMaskChanged();
	}
}

void deLight::SetLayerMaskShadow(const decLayerMask &layerMask){
	if(layerMask == pLayerMaskShadow){
		return;
	}
	
	pLayerMaskShadow = layerMask;
	
	if(pPeerGraphic){
		pPeerGraphic->ShadowLayerMaskChanged();
	}
}



void deLight::SetActivated(bool activated){
	if(pActivated == activated){
		return;
	}
	
	pActivated = activated;
	
	if(pPeerGraphic){
		pPeerGraphic->ActivatedChanged();
	}
}

void deLight::SetCastShadows(bool castShadows){
	if(castShadows == pCastShadows){
		return;
	}
	
	pCastShadows = castShadows;
	
	if(pPeerGraphic){
		pPeerGraphic->ShadowParameterChanged();
	}
}



// Shadow ignore components
/////////////////////////////

int deLight::GetShadowIgnoreComponentCount() const{
	return pShadowIgnoreComponents.GetCount();
}

deComponent *deLight::GetShadowIgnoreComponentAt(int index) const{
	return (deComponent*)pShadowIgnoreComponents.GetAt(index);
}

bool deLight::HasShadowIgnoreComponent(deComponent *component) const{
	return pShadowIgnoreComponents.Has(component);
}

void deLight::AddShadowIgnoreComponent(deComponent *component){
	if(! component){
		DETHROW(deeInvalidParam);
	}
	
	pShadowIgnoreComponents.Add(component);
	
	if(pPeerGraphic){
		pPeerGraphic->ShadowIgnoreComponentsChanged();
	}
}

void deLight::RemoveShadowIgnoreComponent(deComponent *component){
	pShadowIgnoreComponents.Remove(component);
	
	if(pPeerGraphic){
		pPeerGraphic->ShadowIgnoreComponentsChanged();
	}
}

void deLight::RemoveAllShadowIgnoreComponents(){
	if(pShadowIgnoreComponents.GetCount() == 0){
		return;
	}
	
	pShadowIgnoreComponents.RemoveAll();
	
	if(pPeerGraphic){
		pPeerGraphic->ShadowIgnoreComponentsChanged();
	}
}



// System Peers
/////////////////

void deLight::SetPeerGraphic(deBaseGraphicLight *peer){
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

void deLight::SetParentWorld(deWorld *world){
	pParentWorld = world;
}

void deLight::SetLLWorldPrev(deLight *light){
	pLLWorldPrev = light;
}

void deLight::SetLLWorldNext(deLight *light){
	pLLWorldNext = light;
}
