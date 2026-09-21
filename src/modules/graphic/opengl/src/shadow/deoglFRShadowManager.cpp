/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "deoglFRShadowManager.h"
#include "deoglShadowCaster.h"
#include "deoglSCSolid.h"
#include "../capabilities/deoglCapabilities.h"
#include "../capabilities/deoglCapsFmtSupport.h"
#include "../collidelist/deoglCollideList.h"
#include "../collidelist/deoglCollideListLight.h"
#include "../extensions/deoglExtensions.h"
#include "../extensions/deoglExtResult.h"
#include "../light/deoglRLight.h"
#include "../light/shader/deoglLightShader.h"
#include "../rendering/plan/deoglRenderPlan.h"
#include "../rendering/plan/deoglRenderPlanLight.h"
#include "../rendering/plan/deoglRenderPlanSkyLight.h"
#include "../rendering/deoglRenderBase.h"
#include "../rendering/deoglRenderWorld.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTTexture.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../shaders/paramblock/deoglSPBMapBuffer.h"
#include "../shaders/paramblock/deoglSPBAccessor.h"
#include "../sky/deoglRSkyInstance.h"
#include "../sky/deoglRSkyInstanceLayer.h"
#include "../texture/deoglTextureStageManager.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/light/deLight.h>


// Class deoglFRShadowManager
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglFRShadowManager::deoglFRShadowManager(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pSpotShadowSize(512),
pPointShadowSize(256),
pSkyLight(nullptr),
pLightSlotPlans{},
pLightCount(0){
}

deoglFRShadowManager::~deoglFRShadowManager() = default;


// Management
///////////////

const deTUniqueReference<deoglArrayTexture> &deoglFRShadowManager::GetSpotShadowArray(){
	if(!pSpotShadowTexture){
		pCreateSpotShadowArray();
	}
	return pSpotShadowTexture;
}

const deTUniqueReference<deoglArrayCubeMap> &deoglFRShadowManager::GetPointShadowArray(){
	if(!pPointShadowTexture){
		pCreatePointShadowArray();
	}
	return pPointShadowTexture;
}

void deoglFRShadowManager::SetSPBParameterLightParams(deoglSPBParameter &parameter){
	deoglSPBParameter::ParameterList m;
	
	m.Add({deoglSPBParameter::evtInt, 1, 1, 1}); // int type (LightMode*)
	m.Add({deoglSPBParameter::evtInt, 1, 1, 1}); // int arrayIndex
	m.Add({deoglSPBParameter::evtInt, 1, 1, 1}); // int pcfMode
	m.Add({deoglSPBParameter::evtBool, 2, 1, 1}); // bvec2 hasTextureShadow
	
	deoglSPBParameter p;
	deoglLightShader::SetSPBParameterLightParam(p, 1);
	m.Add(std::move(p)); // sFRLightParameters light
	
	deoglLightShader::SetSPBParameterInstParam(p, 1);
	m.Add(std::move(p)); // sFRLightInstanceParameters instance
	
	m.Add({deoglSPBParameter::evtFloat, 4, 1, 1}); // vec4 positionAndType
	m.Add({deoglSPBParameter::evtFloat, 4, 1, 1}); // vec4 colorAndRange
	m.Add({deoglSPBParameter::evtFloat, 4, 1, 1}); // vec4 attenuations
	m.Add({deoglSPBParameter::evtFloat, 4, 1, 1}); // vec4 spotParams
	m.Add({deoglSPBParameter::evtFloat, 4, 4, 1}); // mat4 shadowMatrix
	m.Add({deoglSPBParameter::evtFloat, 4, 1, 1}); // vec4 shadowParams
	m.Add({deoglSPBParameter::evtInt, 4, 1, 1}); // ivec4 shadowLayer
	m.Add({deoglSPBParameter::evtFloat, 4, 1, 1}); // vec4 lightDirection
	m.Add({deoglSPBParameter::evtFloat, 4, 1, 1}); // vec4 colorAmbient
	parameter.SetStruct(std::move(m), MaxLights);
}

void deoglFRShadowManager::PrepareForRender(deoglRenderPlan &plan){
	for(int i=0; i<SlotCount; i++){
		if(pLightSlotPlans[i]){
			pLightSlotPlans[i]->SetFRSlot(-1);
			pLightSlotPlans[i] = nullptr;
		}
	}
	if(pSkyLight){
		pSkyLight->SetFRSlot(-1);
		pSkyLight = nullptr;
	}
	pLightCount = 0;
	
	// first sky light if present
	if(plan.GetSkyLightCount() > 0 && plan.GetSkyLightAt(0)->GetUseLight()){
		pSkyLight = plan.GetSkyLightAt(0);
		pSkyLight->SetFRSlot(pLightCount++);
	}
	
	// collect spot/point lights sorted by distance
	decTList<deoglRenderPlanLight*> planLights;
	
	plan.GetLights().Visit(0, plan.GetLightCount(), [&](deoglRenderPlanLight *pl){
		if(pl->GetLight() && !pl->GetLight()->GetCulled()){
			planLights.Add(pl);
		}
	});
	
	planLights.Sort([](const deoglRenderPlanLight *a, const deoglRenderPlanLight *b){
		return DECompare(a->GetDistance(), b->GetDistance());
	});
	
	// assign up to 8 lights from sorted list
	deoglRenderPlanLight *lightSlotPlans[SlotCount]{};
	int usedSlotCount = 0, spotCount = 0, pointCount = 0;
	
	planLights.VisitWhile([&](deoglRenderPlanLight *pl){
		if(usedSlotCount == SlotCount){
			return false;
		}
		
		auto rlight = pl->GetLight()->GetLight();
		
		switch(rlight->GetLightType()){
		case deLight::eltSpot:
		case deLight::eltProjector:
			if(spotCount < SlotCount){
				spotCount++;
				lightSlotPlans[usedSlotCount++] = pl;
			}
			break;
			
		case deLight::eltPoint:
			if(pointCount < SlotCount){
				pointCount++;
				lightSlotPlans[usedSlotCount++] = pl;
			}
			break;
		}
		return true;
	});
	
	// assign shadow slots to lights. for lights already present in the last frame retain
	// the layer assignment to avoid re-rendering the shadow maps
	
	// first remove lights not present anymore in the current frame
	for(int i=0; i<SlotCount; i++){
		deoglRLight *light = pLightSlot[i];
		if(!light){
			continue;
		}
		
		bool found = false;
		for(int j=0; j<usedSlotCount; j++){
			if(lightSlotPlans[j]->GetLight()->GetLight() == light){
				found = true;
				break;
			}
		}
		if(found){
			continue;
		}
		
		pLightSlot[i].Clear();
		if(pLightSlotPlans[i]){
			pLightSlotPlans[i]->SetFRSlot(-1);
			pLightSlotPlans[i] = nullptr;
		}
		
		auto sc = light->GetShadowCaster();
		if(sc){
			sc->SetFRSpotLayer(-1);
			sc->SetFRPointLayer(-1);
		}
		light->SetDirtyShadows();
		
		for(int j=0; j<SlotCount; j++){
			if(pSpotSlot[j] == light){
				pSpotSlot[j].Clear();
			}
			if(pPointSlot[j] == light){
				pPointSlot[j].Clear();
			}
		}
	}
	
	// then assign lights not already existing. all other lights keep their assignments.
	// for all lights though assign the render plan light plan as this is required by SSBO update
	for(int i=0; i<usedSlotCount; i++){
		auto *pl = lightSlotPlans[i];
		auto light = pl->GetLight()->GetLight();
		
		// if present in last frame, retain slot assignment and continue with next light
		int slotIndex = -1;
		for(int j=0; j<SlotCount; j++){
			if(pLightSlot[j] == light){
				pLightSlotPlans[j] = pl;
				slotIndex = j;
				pl->SetFRSlot(pLightCount++);
				break;
			}
		}
		if(slotIndex != -1){
			continue;
		}
		
		// if absent assign next free slot
		for(int j=0; j<SlotCount; j++){
			if(!pLightSlot[j]){
				slotIndex = j;
				break;
			}
		}
		if(slotIndex == -1){
			continue; // sanity check. should never happen
		}
		
		pLightSlotPlans[slotIndex] = pl;
		pLightSlot[slotIndex] = light;
		pl->SetFRSlot(pLightCount++);
		
		// assign shadow layers if used
		auto sc = light->GetShadowCaster();
		if(!sc || !pl->GetUseShadow()){
			continue;
		}
		
		switch(light->GetLightType()){
		case deLight::eltSpot:
		case deLight::eltProjector:
			for(int k=0; k<SlotCount; k++){
				if(!pSpotSlot[k]){
					pSpotSlot[k] = light;
					sc->SetFRSpotLayer(k);
					break;
				}
			}
			break;
			
		case deLight::eltPoint:
			for(int k=0; k<SlotCount; k++){
				if(!pPointSlot[k]){
					pPointSlot[k] = light;
					sc->SetFRPointLayer(k);
					break;
				}
			}
			break;
		}
	}
}


// Private Functions
//////////////////////

void deoglFRShadowManager::pCreateSpotShadowArray(){
	pSpotShadowTexture = deTUniqueReference<deoglArrayTexture>::New(pRenderThread);
	pSpotShadowTexture->SetDepthFormat(false, false);
	pSpotShadowTexture->SetSize(pSpotShadowSize, pSpotShadowSize, SlotCount * LayerPerLight);
	pSpotShadowTexture->CreateTexture();
	pSpotShadowTexture->SetDebugObjectLabel("FRShadow.Spot");
	
	// configure for shadow comparison
	auto &tsmgr = pRenderThread.GetTexture().GetStages();
	tsmgr.EnableBareArrayTexture(0, pSpotShadowTexture);
	OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE));
	OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL));
	tsmgr.DisableStage(0);
}

void deoglFRShadowManager::pCreatePointShadowArray(){
	pPointShadowTexture = deTUniqueReference<deoglArrayCubeMap>::New(pRenderThread);
	pPointShadowTexture->SetDepthFormat(false, false);
	pPointShadowTexture->SetSize(pPointShadowSize, SlotCount * LayerPerLight);
	pPointShadowTexture->CreateTexture();
	pPointShadowTexture->SetDebugObjectLabel("FRShadow.Point");
	
	auto &tsmgr = pRenderThread.GetTexture().GetStages();
	tsmgr.EnableBareArrayCubeMap(0, pPointShadowTexture);
	OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAX_LEVEL, 0));
	OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE));
	OGL_CHECK(pRenderThread, glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL));
	
	tsmgr.DisableStage(0);
}
