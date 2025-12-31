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

#include "skyeLayer.h"
#include "../skyeSky.h"
#include "../body/skyeBody.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/engine/igdeEngineController.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>



// Class skyeLayer
/////////////////////

// Constructor, destructor
////////////////////////////

skyeLayer::skyeLayer(igdeEnvironment &environment) :
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
pActive(false)
{
	for(int i=0; i<=(int)deSkyLayer::etAmbientIntensity; i++){
		pTargets.Add(skyeControllerTarget::Ref::New());
	}
}

skyeLayer::~skyeLayer(){
	RemoveAllBodies();
}



// Management
///////////////

void skyeLayer::SetSky(skyeSky *sky){
	if(sky == pSky){
		return;
	}
	
	pSky = sky;
	UpdateRelativeResources();
}

void skyeLayer::SetName(const char *name){
	if(pName == name){
		return;
	}
	
	pName = name;
	if(pSky){
		pSky->NotifyLayerNameChanged(this);
	}
}



void skyeLayer::SetOffset(const decVector &offset){
	if(offset.IsEqualTo(pOffset)){
		return;
	}
	
	pOffset = offset;
	if(pSky){
		pSky->NotifyLayerChanged(this);
	}
}

void skyeLayer::SetOrientation(const decVector &orientation){
	if(orientation.IsEqualTo(pOrientation)){
		return;
	}
	
	pOrientation = orientation;
	if(pSky){
		pSky->NotifyLayerChanged(this);
	}
}

void skyeLayer::SetColor(const decColor &color){
	if(color.IsEqualTo(pColor)){
		return;
	}
	
	pColor = color;
	if(pSky){
		pSky->NotifyLayerChanged(this);
	}
}

void skyeLayer::SetIntensity(float intensity){
	intensity = decMath::max(intensity, 0.0f);
	if(fabsf(intensity - pIntensity) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pIntensity = intensity;
	if(pSky){
		pSky->NotifyLayerChanged(this);
	}
}

void skyeLayer::SetTransparency(float transparency){
	transparency = decMath::clamp(transparency, 0.0f, 1.0f);
	if(fabsf(transparency - pTransparency) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pTransparency = transparency;
	if(pSky){
		pSky->NotifyLayerChanged(this);
	}
}

void skyeLayer::SetSkinPath(const char *skinPath){
	if(pPathSkin == skinPath){
		return;
	}
	
	pPathSkin = skinPath;
	pUpdateSkin();
	if(pSky){
		pSky->NotifyLayerChanged(this);
	}
}



void skyeLayer::SetLightOrientation(const decVector &orientation){
	if(orientation.IsEqualTo(pLightOrientation)){
		return;
	}
	
	pLightOrientation = orientation;
	if(pSky){
		pSky->NotifyLayerChanged(this);
	}
}

void skyeLayer::SetLightColor(const decColor &color){
	if(color.IsEqualTo(pLightColor)){
		return;
	}
	
	pLightColor = color;
	if(pSky){
		pSky->NotifyLayerChanged(this);
	}
}

void skyeLayer::SetLightIntensity(float intensity){
	intensity = decMath::max(intensity, 0.0f);
	if(fabsf(intensity - pLightIntensity) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pLightIntensity = intensity;
	if(pSky){
		pSky->NotifyLayerChanged(this);
	}
}

void skyeLayer::SetAmbientIntensity(float intensity){
	intensity = decMath::max(intensity, 0.0f);
	if(fabsf(intensity - pAmbientIntensity) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pAmbientIntensity = intensity;
	if(pSky){
		pSky->NotifyLayerChanged(this);
	}
}

void skyeLayer::SetMultiplyBySkyLight(bool multiply){
	if(multiply == pMulBySkyLight){
		return;
	}
	
	pMulBySkyLight = multiply;
	if(pSky){
		pSky->NotifyLayerChanged(this);
	}
}

void skyeLayer::SetMultiplyBySkyColor(bool multiply){
	if(multiply == pMulBySkyColor){
		return;
	}
	
	pMulBySkyColor = multiply;
	if(pSky){
		pSky->NotifyLayerChanged(this);
	}
}

void skyeLayer::SetSelected(bool selected){
	pSelected = selected;
}

void skyeLayer::SetActive(bool active){
	pActive = active;
}

void skyeLayer::UpdateRelativeResources(){
	pUpdateSkin();
	
	const int bodyCount = pBodies.GetCount();
	int i;
	for(i=0; i<bodyCount; i++){
		pBodies.GetAt(i)->UpdateRelativeResources();
	}
}



// Bodies
///////////

void skyeLayer::AddBody(skyeBody *body){
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

void skyeLayer::InsertBodyAt(skyeBody *body, int index){
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

void skyeLayer::MoveBodyTo(skyeBody *body, int index){
	pBodies.Move(body, index);
	
	if(pSky){
		pSky->NotifyBodyStructureChanged(this);
	}
}

void skyeLayer::RemoveBody(skyeBody *body){
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

void skyeLayer::RemoveAllBodies(){
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

void skyeLayer::SetActiveBody(skyeBody *body){
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

const skyeControllerTarget::Ref &skyeLayer::GetTarget(deSkyLayer::eTargets target) const{
	return pTargets.GetAt((int)target);
}

void skyeLayer::SetActiveTarget(deSkyLayer::eTargets target){
	if(target == pActiveTarget){
		return;
	}
	
	pActiveTarget = target;
	if(pSky){
		pSky->NotifyActiveTargetChanged(this);
	}
}

void skyeLayer::NotifyTargetChanged(deSkyLayer::eTargets target){
	if(target < deSkyLayer::etOffsetX || target > deSkyLayer::etAmbientIntensity){
		DETHROW(deeInvalidParam);
	}
	
	if(pSky){
		pSky->NotifyTargetChanged(this, target);
	}
}



// Private Functions
//////////////////////

void skyeLayer::pUpdateSkin(){
	if(pPathSkin.IsEmpty() || !pSky){
		pEngSkin = nullptr;
		return;
	}
	
	try{
		pEngSkin = pEnvironment.GetEngineController()->GetEngine()->GetSkinManager()->
			LoadSkin(pPathSkin, pSky->GetDirectoryPath());
		
	}catch(const deException &){
		pEngSkin = pSky->GetEnvironment()->GetStockSkin(igdeEnvironment::essError);
	}
}
