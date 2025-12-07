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

#include "deSky.h"
#include "deSkyInstance.h"
#include "deSkyInstanceManager.h"
#include "deSkyController.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/graphic/deBaseGraphicSkyInstance.h"


// Class deSkyInstance
////////////////////////

// Constructor, destructor
////////////////////////////

deSkyInstance::deSkyInstance(deSkyInstanceManager *manager) :
deResource(manager),
pOrder(0),

pControllers(nullptr),
pControllerCount(0),

pPassthroughTransparency(0.0f),

pPeerGraphic(nullptr),

pParentWorld(nullptr),
pLLWorldPrev(nullptr),
pLLWorldNext(nullptr){
}

deSkyInstance::~deSkyInstance(){
	if(pPeerGraphic){
		delete pPeerGraphic;
		pPeerGraphic = nullptr;
	}
	
	if(pControllers){
		delete [] pControllers;
	}
}



// Management
///////////////

void deSkyInstance::SetSky(deSky *sky){
	if(pSky == sky){
		return;
	}
	
	if(pControllers){
		delete [] pControllers;
		pControllers = NULL;
		pControllerCount = 0;
	}
	
	pSky = sky;
	
	if(sky){
		const int controllerCount = sky->GetControllerCount();
		if(controllerCount > 0){
			pControllers = new deSkyController[controllerCount];
			
			int i;
			for(i=0; i<controllerCount; i++){
				pControllers[i] = sky->GetControllerAt(i);
			}
			pControllerCount = controllerCount;
		}
	}
	
	if(pPeerGraphic){
		pPeerGraphic->SkyChanged();
	}
}

void deSkyInstance::SetOrder(int order){
	if(pOrder == order){
		return;
	}
	
	pOrder = order;
	
	if(pPeerGraphic){
		pPeerGraphic->OrderChanged();
	}
}



deSkyController &deSkyInstance::GetControllerAt(int index) const{
	if(index < 0 || index >= pControllerCount){
		DETHROW(deeInvalidParam);
	}
	return pControllers[index];
}

int deSkyInstance::IndexOfControllerNamed(const char *name) const{
	int i;
	for(i=0; i<pControllerCount; i++){
		if(pControllers[i].GetName() == name){
			return i;
		}
	}
	return -1;
}

void deSkyInstance::NotifyControllerChangedAt(int index){
	if(index < 0 || index >= pControllerCount){
		DETHROW(deeInvalidParam);
	}
	
	if(pPeerGraphic){
		pPeerGraphic->ControllerChanged(index);
	}
}



void deSkyInstance::SetLayerMask(const decLayerMask &layerMask){
	if(layerMask == pLayerMask){
		return;
	}
	
	pLayerMask = layerMask;
	
	if(pPeerGraphic){
		pPeerGraphic->LayerMaskChanged();
	}
}

void deSkyInstance::SetPassthroughTransparency(float transparency){
	transparency = decMath::clamp(transparency, 0.0f, 1.0f);
	if(fabsf(transparency - pPassthroughTransparency) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pPassthroughTransparency = transparency;
	
	if(pPeerGraphic){
		pPeerGraphic->PassthroughTransparencyChanged();
	}
}


// System Peers
/////////////////

void deSkyInstance::SetPeerGraphic(deBaseGraphicSkyInstance *peer){
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

void deSkyInstance::SetParentWorld(deWorld *world){
	pParentWorld = world;
}

void deSkyInstance::SetLLWorldPrev(deSkyInstance *sky){
	pLLWorldPrev = sky;
}

void deSkyInstance::SetLLWorldNext(deSkyInstance *sky){
	pLLWorldNext = sky;
}
