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

#include "deoglRSky.h"
#include "deoglRSkyInstance.h"
#include "deoglRSkyInstanceLayer.h"
#include "deoglSkyLayerTracker.h"
#include "../envmap/deoglEnvironmentMap.h"
#include "../world/deoglRWorld.h"
#include "../configuration/deoglConfiguration.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../deGraphicOpenGl.h"

#include <dragengine/resources/sky/deSkyInstance.h>
#include <dragengine/resources/sky/deSkyController.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/exceptions.h>



// Class deoglRSkyInstance
////////////////////

// Constructor, destructor
////////////////////////////

deoglRSkyInstance::deoglRSkyInstance(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pParentWorld(nullptr),
pOrder(0),
pPassthroughTransparency(0.0f),
pControllerStates(nullptr),
pControllerStateCount(0),
pTotalSkyLightIntensity(0.0f),
pTotalSkyAmbientIntensity(0.0f),
pEnvMapTimer(0.0f),
pSkyNeedsUpdate(false),
pWorldMarkedRemove(false)
{
	LEAK_CHECK_CREATE(renderThread, SkyInstance);
}

deoglRSkyInstance::~deoglRSkyInstance(){
	LEAK_CHECK_FREE(pRenderThread, SkyInstance);
	
	pLayers.RemoveAll();
	
	if(pControllerStates){
		delete [] pControllerStates;
	}
}



// Management
///////////////

void deoglRSkyInstance::SetParentWorld(deoglRWorld *world){
	if(world == pParentWorld){
		return;
	}
	
	DropAllGIStates();
	
	pParentWorld = world;
	
	pSkyNeedsUpdate = true;
}



void deoglRSkyInstance::SetRSky(deoglRSky *rsky){
	// called during synchronization time
	if(rsky == pRSky){
		return;
	}
	pRSky = rsky;
	RebuildLayers();
}



void deoglRSkyInstance::SetOrder(int order){
	if(order == pOrder){
		return;
	}
	
	pOrder = order;
	
	if(pParentWorld){
		pParentWorld->MarkSkyOrderDirty();
	}
}

void deoglRSkyInstance::SetLayerMask(const decLayerMask &layerMask){
	pLayerMask = layerMask;
}

void deoglRSkyInstance::SetPassthroughTransparency(float transparency){
	pPassthroughTransparency = decMath::clamp(transparency, 0.0f, 1.0f);
}


float deoglRSkyInstance::GetControllerStateAt(int index) const{
	if(index < 0 || index >= pControllerStateCount){
		DETHROW(deeInvalidParam);
	}
	return pControllerStates[index];
}

void deoglRSkyInstance::UpdateControllerStates(const deSkyInstance &instance){
	const int count = instance.GetControllers().GetCount();
	
	if(count != pControllerStateCount){
		if(pControllerStates){
			delete [] pControllerStates;
			pControllerStates = nullptr;
			pControllerStateCount = 0;
		}
		
		if(count > 0){
			pControllerStates = new float[count];
			pControllerStateCount = count;
		}
	}
	
	int i;
	for(i=0; i<count; i++){
		const deSkyController &controller = instance.GetControllers().GetAt(i);
		const float minimum = controller.GetMinimumValue();
		const float range = controller.GetMaximumValue() - minimum;
		
		if(range > FLOAT_SAFE_EPSILON){
			pControllerStates[i] = (controller.GetCurrentValue() - minimum) / range;
			
		}else{
			pControllerStates[i] = minimum;
		}
	}
}



deoglRSkyInstanceLayer &deoglRSkyInstance::GetLayerAt(int index) const{
	return pLayers.GetAt(index);
}

void deoglRSkyInstance::RebuildLayers(){
	pSkyNeedsUpdate = true;
	
	pLayers.RemoveAll();
	
	if(!pRSky){
		return;
	}
	
	const int layerCount = pRSky->GetLayerCount();
	if(layerCount == 0){
		return;
	}
	
	int i;
	for(i=0; i<layerCount; i++){
		pLayers.Add(deoglRSkyInstanceLayer::Ref::New(*this, i));
	}
}

void deoglRSkyInstance::UpdateLayers(){
	int i;
	
	pTotalSkyLightIntensity = 0.0f;
	pTotalSkyAmbientIntensity = 0.0f;
	pTotalSkyLightColor.SetZero();
	
	pLayers.Visit([&](deoglRSkyInstanceLayer &l){
		l.Update();
		
		const float ambientIntensity = l.GetAmbientIntensity();
		const float lightIntensity = ambientIntensity + l.GetLightIntensity();
		
		pTotalSkyLightIntensity += lightIntensity;
		pTotalSkyAmbientIntensity += ambientIntensity;
		pTotalSkyLightColor += l.GetLightColor() * lightIntensity;
		
		pSkyNeedsUpdate |= l.GetSkyNeedsUpdate();
	});
	
	if(pTotalSkyLightIntensity > FLOAT_SAFE_EPSILON){
		pTotalSkyLightColor /= pTotalSkyLightIntensity;
		pTotalSkyLightColor.a = 1.0f;
	}
	
	pLayers.Visit([&](deoglRSkyInstanceLayer &l){
		l.UpdateWithModifiers();
	});
}



void deoglRSkyInstance::SetEnvironmentMapTimer(float timer){
	pEnvMapTimer = timer;
}



void deoglRSkyInstance::PrepareForRender(){
	// TODO dynamic skin using update timer
}



void deoglRSkyInstance::NotifySkyChanged(){
	if(pSkyNeedsUpdate && pParentWorld){
		pParentWorld->EnvMapsNotifySkyChanged();
	}
	
	pSkyNeedsUpdate = false;
}

void deoglRSkyInstance::DropGIState(const deoglGIState *giState){
	pLayers.Visit([&](deoglRSkyInstanceLayer &l){
		l.RemoveAllGICascades(*giState);
	});
}

void deoglRSkyInstance::DropAllGIStates(){
	pLayers.Visit([&](deoglRSkyInstanceLayer &l){
		l.RemoveAllGICascades();
	});
}



void deoglRSkyInstance::PrepareQuickDispose(){
	pParentWorld = nullptr;
}

void deoglRSkyInstance::NotifyUpdateStaticComponent(deoglRComponent *component){
	pLayers.Visit([&](deoglRSkyInstanceLayer &l){
		l.NotifyUpdateStaticComponent(component);
	});
}



// Render world usage
///////////////////////

void deoglRSkyInstance::SetWorldMarkedRemove(bool marked){
	pWorldMarkedRemove = marked;
}
