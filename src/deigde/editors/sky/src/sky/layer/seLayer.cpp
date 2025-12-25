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

#include "seLayer.h"
#include "../seSky.h"
#include "../body/seBody.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/engine/igdeEngineController.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>



// Class seLayer
/////////////////////

// Constructor, destructor
////////////////////////////

seLayer::seLayer(igdeEnvironment &environment) :
pEnvironment(environment),

pSky(nullptr),

pName("Layer"),

pColor(1.0f, 1.0f, 1.0f),
pIntensity(20.0f),
pTransparency(1.0f),

pLightColor(1.0f, 1.0f, 1.0f),
pLightIntensity(0.0f),
pAmbientIntensity(0.0f),

pMulBySkyLight(false),
pMulBySkyColor(false),

pActiveTarget(deSkyLayer::etRotationX),

pSelected(false),
pActive(false){
}

seLayer::~seLayer(){
	RemoveAllBodies();
}



// Management
///////////////

void seLayer::SetSky(seSky *sky){
	if(sky == pSky){
		return;
	}
	
	pSky = sky;
	UpdateRelativeResources();
}

void seLayer::SetName(const char *name){
	if(pName == name){
		return;
	}
	
	pName = name;
	if(pSky){
		pSky->NotifyLayerNameChanged(this);
	}
}



void seLayer::SetOffset(const decVector &offset){
	if(offset.IsEqualTo(pOffset)){
		return;
	}
	
	pOffset = offset;
	if(pSky){
		pSky->NotifyLayerChanged(this);
	}
}

void seLayer::SetOrientation(const decVector &orientation){
	if(orientation.IsEqualTo(pOrientation)){
		return;
	}
	
	pOrientation = orientation;
	if(pSky){
		pSky->NotifyLayerChanged(this);
	}
}

void seLayer::SetColor(const decColor &color){
	if(color.IsEqualTo(pColor)){
		return;
	}
	
	pColor = color;
	if(pSky){
		pSky->NotifyLayerChanged(this);
	}
}

void seLayer::SetIntensity(float intensity){
	intensity = decMath::max(intensity, 0.0f);
	if(fabsf(intensity - pIntensity) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pIntensity = intensity;
	if(pSky){
		pSky->NotifyLayerChanged(this);
	}
}

void seLayer::SetTransparency(float transparency){
	transparency = decMath::clamp(transparency, 0.0f, 1.0f);
	if(fabsf(transparency - pTransparency) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pTransparency = transparency;
	if(pSky){
		pSky->NotifyLayerChanged(this);
	}
}

void seLayer::SetSkinPath(const char *skinPath){
	if(pPathSkin == skinPath){
		return;
	}
	
	pPathSkin = skinPath;
	pUpdateSkin();
	if(pSky){
		pSky->NotifyLayerChanged(this);
	}
}



void seLayer::SetLightOrientation(const decVector &orientation){
	if(orientation.IsEqualTo(pLightOrientation)){
		return;
	}
	
	pLightOrientation = orientation;
	if(pSky){
		pSky->NotifyLayerChanged(this);
	}
}

void seLayer::SetLightColor(const decColor &color){
	if(color.IsEqualTo(pLightColor)){
		return;
	}
	
	pLightColor = color;
	if(pSky){
		pSky->NotifyLayerChanged(this);
	}
}

void seLayer::SetLightIntensity(float intensity){
	intensity = decMath::max(intensity, 0.0f);
	if(fabsf(intensity - pLightIntensity) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pLightIntensity = intensity;
	if(pSky){
		pSky->NotifyLayerChanged(this);
	}
}

void seLayer::SetAmbientIntensity(float intensity){
	intensity = decMath::max(intensity, 0.0f);
	if(fabsf(intensity - pAmbientIntensity) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pAmbientIntensity = intensity;
	if(pSky){
		pSky->NotifyLayerChanged(this);
	}
}

void seLayer::SetMultiplyBySkyLight(bool multiply){
	if(multiply == pMulBySkyLight){
		return;
	}
	
	pMulBySkyLight = multiply;
	if(pSky){
		pSky->NotifyLayerChanged(this);
	}
}

void seLayer::SetMultiplyBySkyColor(bool multiply){
	if(multiply == pMulBySkyColor){
		return;
	}
	
	pMulBySkyColor = multiply;
	if(pSky){
		pSky->NotifyLayerChanged(this);
	}
}

void seLayer::SetSelected(bool selected){
	pSelected = selected;
}

void seLayer::SetActive(bool active){
	pActive = active;
}

void seLayer::UpdateRelativeResources(){
	pUpdateSkin();
	
	const int bodyCount = pBodies.GetCount();
	int i;
	for(i=0; i<bodyCount; i++){
		pBodies.GetAt(i)->UpdateRelativeResources();
	}
}



// Bodies
///////////

void seLayer::AddBody(seBody *body){
	if(!body){
		DETHROW(deeInvalidParam);
	}
	
	pBodies.Add(body);
	body->SetLayer(this);
	
	if(pSky){
		pSky->NotifyBodyStructureChanged(this);
	}
	
	if(!pActiveBody){
		SetActiveBody(body);
	}
}

void seLayer::InsertBodyAt(seBody *body, int index){
	if(!body){
		DETHROW(deeInvalidParam);
	}
	
	pBodies.Insert(body, index);
	body->SetLayer(this);
	
	if(pSky){
		pSky->NotifyBodyStructureChanged(this);
	}
	
	if(!pActiveBody){
		SetActiveBody(body);
	}
}

void seLayer::MoveBodyTo(seBody *body, int index){
	pBodies.Move(body, index);
	
	if(pSky){
		pSky->NotifyBodyStructureChanged(this);
	}
}

void seLayer::RemoveBody(seBody *body){
	if(!pBodies.Has(body)){
		DETHROW(deeInvalidParam);
	}
	
	if(body->GetActive()){
		const int count = pBodies.GetCount();
		const int index = pBodies.IndexOf(body);
		
		if(index < count - 1){
			SetActiveBody(pBodies.GetAt(index + 1));
			
		}else if(index > 0){
			SetActiveBody(pBodies.GetAt(index - 1));
			
		}else{
			SetActiveBody(nullptr);
		}
	}
	
	body->SetLayer(nullptr);
	pBodies.Remove(body);
	
	if(pSky){
		pSky->NotifyBodyStructureChanged(this);
	}
}

void seLayer::RemoveAllBodies(){
	const int count = pBodies.GetCount();
	int i;
	
	SetActiveBody(nullptr);
	
	for(i=0; i<count; i++){
		pBodies.GetAt(i)->SetLayer(nullptr);
	}
	pBodies.RemoveAll();
	
	if(pSky){
		pSky->NotifyBodyStructureChanged(this);
	}
}

void seLayer::SetActiveBody(seBody *body){
	if(body == pActiveBody){
		return;
	}
	
	if(pActiveBody){
		pActiveBody->SetActive(false);
	}
	
	pActiveBody = body;
	
	if(body){
		body->SetActive(true);
	}
	
	if(pSky){
		pSky->NotifyActiveBodyChanged(this);
	}
}



// Targets
////////////

const seControllerTarget &seLayer::GetTarget(deSkyLayer::eTargets target) const{
	if(target < deSkyLayer::etOffsetX || target > deSkyLayer::etAmbientIntensity){
		DETHROW(deeInvalidParam);
	}
	return pTargets[target];
}

seControllerTarget &seLayer::GetTarget(deSkyLayer::eTargets target){
	if(target < deSkyLayer::etOffsetX || target > deSkyLayer::etAmbientIntensity){
		DETHROW(deeInvalidParam);
	}
	return pTargets[target];
}

void seLayer::SetActiveTarget(deSkyLayer::eTargets target){
	if(target == pActiveTarget){
		return;
	}
	
	pActiveTarget = target;
	if(pSky){
		pSky->NotifyActiveTargetChanged(this);
	}
}

void seLayer::NotifyTargetChanged(deSkyLayer::eTargets target){
	if(target < deSkyLayer::etOffsetX || target > deSkyLayer::etAmbientIntensity){
		DETHROW(deeInvalidParam);
	}
	
	if(pSky){
		pSky->NotifyTargetChanged(this, target);
	}
}



// Private Functions
//////////////////////

void seLayer::pUpdateSkin(){
	if(pPathSkin.IsEmpty()){
		pEngSkin = nullptr;
		return;
	}
	
	deSkinManager &skinMgr = *pEnvironment.GetEngineController()->GetEngine()->GetSkinManager();
	const char *basePath = "/";
	if(pSky){
		basePath = pSky->GetDirectoryPath();
	}
	
	try{
		pEngSkin = skinMgr.LoadSkin(pPathSkin, basePath);
		
	}catch(const deException &){
		if(pSky){
			pEngSkin = pSky->GetEnvironment()->GetStockSkin(igdeEnvironment::essError);
		}
	}
}
