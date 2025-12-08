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

#include "deoglRSkyInstance.h"
#include "deoglSky.h"
#include "deoglSkyInstance.h"
#include "../deGraphicOpenGl.h"
#include "../renderthread/deoglRenderThread.h"
#include "../world/deoglWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/sky/deSkyInstance.h>



// Class deoglSkyInstance
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkyInstance::deoglSkyInstance(deGraphicOpenGl &ogl, const deSkyInstance &instance) :
pOgl(ogl),
pInstance(instance),
pRInstance(NULL),
pParentWorld(NULL),
pOglSky(NULL),
pEnvMapTimer(0.0f),
pDirtySky(true),
pDirtyControllers(true),
pDirtyLayerMask(true),
pSkyUpdateState(0)
{
	pRInstance.TakeOverWithpOgl.GetRenderThread());
}

deoglSkyInstance::~deoglSkyInstance(){
}



// Management
///////////////

void deoglSkyInstance::SyncToRender(){
	if(pDirtySky){
		pOglSky = NULL;
		if(pInstance.GetSky()){
			pOglSky = (deoglSky*)pInstance.GetSky()->GetPeerGraphic();
		}
		
		if(pOglSky){
			pOglSky->SyncToRender();
			pSkyUpdateState = pOglSky->GetUpdateTracker();
			
			pRInstance->SetRSky(pOglSky->GetRSky());
			
		}else{
			pRInstance->SetRSky(NULL);
		}
		
		pRInstance->SetOrder(pInstance.GetOrder());
		
		pDirtySky = false;
		pDirtyControllers = true;
	}
	
	if(pOglSky && pOglSky->GetUpdateTracker() != pSkyUpdateState){
		pSkyUpdateState = pOglSky->GetUpdateTracker();
		
		pOglSky->SyncToRender();
		pRInstance->RebuildLayers();
		
		pDirtyControllers = true;
	}
	
	pRInstance->SetEnvironmentMapTimer(pEnvMapTimer);
	
	if(pDirtyControllers){
		pRInstance->UpdateControllerStates(pInstance);
		pRInstance->UpdateLayers();
	}
	
	if(pDirtyLayerMask){
		pRInstance->SetLayerMask(pInstance.GetLayerMask());
		pDirtyLayerMask = false;
	}
	
	pRInstance->NotifySkyChanged();
}



// Notifications
//////////////////

void deoglSkyInstance::Update(float elapsed){
	pEnvMapTimer += elapsed;
	// Update dynamic skins: TODO
}



void deoglSkyInstance::SetParentWorld(deoglWorld *parentWorld){
	if(parentWorld == pParentWorld){
		return;
	}
	
	pParentWorld = parentWorld;
}



void deoglSkyInstance::SkyChanged(){
	pDirtySky = true;
}

void deoglSkyInstance::OrderChanged(){
	pDirtySky = true;
}

void deoglSkyInstance::ControllerChanged(int){
	pDirtyControllers = true;
}

void deoglSkyInstance::LayerMaskChanged(){
	pDirtyLayerMask = true;
}
