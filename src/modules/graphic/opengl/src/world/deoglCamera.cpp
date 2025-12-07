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

#include "deoglCamera.h"
#include "deoglRCamera.h"
#include "deoglWorld.h"
#include "../deGraphicOpenGl.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../canvas/deoglCanvasRenderWorld.h"
#include "../effects/deoglEffect.h"
#include "../propfield/deoglPropField.h"
#include "../rendering/plan/deoglRenderPlan.h"
#include "../renderthread/deoglRenderThread.h"
#include "../skin/dynamic/renderables/deoglDSRenderableCamera.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/effect/deEffect.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/world/deWorld.h>



// Class deoglCamera
//////////////////////

// Constructor, destructor
////////////////////////////

deoglCamera::deoglCamera(deGraphicOpenGl &ogl, const deCamera &camera) :
pOgl(ogl),
pCamera(camera),
pRCamera(NULL),

pParentWorld(NULL),

pNextSyncUpdateTime(0.0f),

pDirtyGeometry(true),
pDirtyMatrices(true),
pDirtyAdaptionParams(true),
pDirtyLayerMask(true),
pDirtyPlanCamParams(true),
pDirtyPropFields(true),
pDirtyEffects(true),
pResetAdaptedIntensity(true),
pDirtyVR(true),
pEnableVR(false)
{
	try{
		pRCamera = new deoglRCamera(ogl.GetRenderThread());
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglCamera::~deoglCamera(){
	if(pRCamera){
		// add camera for delayed clean up if properly contructred. breaks reference loop
		// camera -> world -> component -> dynamic skin -> canvas or camera -> camera
		// which would cause leaking of render thread resources. this is only required for
		// a proper destructor call since during a failed constructor call no reference
		// loop can be produced yet. doing this would be superfluous
		pOgl.GetRenderThread().GetDelayedOperations().AddCleanUpCamera(pRCamera);
	}
	
	if(pOgl.GetVRCamera() == this){
		pOgl.SetVRCamera(nullptr);
	}
	
	pCleanUp();
}



// Management
///////////////

void deoglCamera::SetParentWorld(deoglWorld *parentWorld){
	if(parentWorld == pParentWorld){
		return;
	}
	
	pParentWorld = parentWorld;
	pDirtyPropFields = true;
}



void deoglCamera::Update(float elapsed){
	pNextSyncUpdateTime += elapsed;
	
	// updating is the only event causing owners to be notified about the need to sync
	pRequiresSync();
}



void deoglCamera::SyncToRender(){
// 		decTimer timer;
	if(pParentWorld){
		pParentWorld->SyncToRender();
		pRCamera->SetParentWorld(pParentWorld->GetRWorld());
// 			pOgl.LogInfoFormat( "Camera.Sync world: %d ys", (int)(timer.GetElapsedTime() * 1e6f) );
		
	}else{
		pRCamera->SetParentWorld(NULL);
	}
	
	if(pDirtyGeometry){
		pRCamera->SetPosition(pCamera.GetPosition());
		pDirtyGeometry = false;
		pDirtyMatrices = true;
// 			pOgl.LogInfoFormat( "Camera.Sync geometry: %d ys", (int)(timer.GetElapsedTime() * 1e6f) );
	}
	
	if(pDirtyMatrices){
		const decDMatrix rotMat(decDMatrix::CreateFromQuaternion(pCamera.GetOrientation()));
		const decDMatrix matrix(decDMatrix::CreateCamera(pCamera.GetPosition(), rotMat.TransformView(), rotMat.TransformUp()));
		pRCamera->SetCameraMatrices(matrix);
		pRCamera->GetPlan().SetCameraMatrix(matrix);
		pDirtyMatrices = false;
// 			pOgl.LogInfoFormat( "Camera.Sync matrices: %d ys", (int)(timer.GetElapsedTime() * 1e6f) );
	}
	
	if(pDirtyAdaptionParams){
		pRCamera->SetExposure(pCamera.GetExposure());
		pRCamera->SetLowestIntensity(pCamera.GetLowestIntensity());
		pRCamera->SetHighestIntensity(pCamera.GetHighestIntensity());
		pRCamera->SetAdaptionTime(pCamera.GetAdaptionTime());
		pRCamera->SetWhiteIntensity(pCamera.GetWhiteIntensity());
		pRCamera->SetBloomIntensity(pCamera.GetBloomIntensity());
		pRCamera->SetBloomStrength(pCamera.GetBloomStrength());
		pRCamera->SetBloomBlend(pCamera.GetBloomBlend());
		pRCamera->SetBloomSize(pCamera.GetBloomSize());
		pRCamera->SetToneMapCurve(pCamera.GetToneMapCurve());
		pDirtyAdaptionParams = false;
// 			pOgl.LogInfoFormat( "Camera.Sync adaption params: %d ys", (int)(timer.GetElapsedTime() * 1e6f) );
	}
	
	if(pDirtyLayerMask){
		pRCamera->GetPlan().SetLayerMask(pCamera.GetLayerMask());
		pRCamera->GetPlan().SetUseLayerMask(pCamera.GetLayerMask().IsNotEmpty());
		pDirtyLayerMask = false;
// 			pOgl.LogInfoFormat( "Camera.Sync layer mask: %d ys", (int)(timer.GetElapsedTime() * 1e6f) );
	}
	
	if(pDirtyPropFields){
		if(pParentWorld){
			const decDVector &position = pCamera.GetPosition();
			dePropField *propField = pParentWorld->GetWorld().GetRootPropField();
			while(propField){
				deoglPropField * const oglPropField = (deoglPropField*)propField->GetPeerGraphic();
				if(oglPropField){
					oglPropField->TestLODLevel(position);
				}
				propField = propField->GetLLWorldNext();
			}
		}
		
		pDirtyPropFields = false;
// 			pOgl.LogInfoFormat( "Camera.Sync prop fields: %d ys", (int)(timer.GetElapsedTime() * 1e6f) );
	}
	
	if(pDirtyPlanCamParams){
		if(! pEnableVR){
			pRCamera->GetPlan().SetCameraParameters(pCamera.GetFov(), pCamera.GetFovRatio(),
				pCamera.GetImageDistance(), pCamera.GetViewDistance());
		}
		pRCamera->SetEnableHDRR(pCamera.GetEnableHDRR());
		pRCamera->SetEnableGI(pCamera.GetEnableGI());
		pDirtyPlanCamParams = false;
// 			pOgl.LogInfoFormat( "Camera.Sync plan cam params: %d ys", (int)(timer.GetElapsedTime() * 1e6f) );
	}
	
	pRCamera->GetPlan().GetDirectEnvMapFader().Update(pNextSyncUpdateTime);
	pRCamera->SetElapsedToneMapAdaption(pRCamera->GetElapsedToneMapAdaption() + pNextSyncUpdateTime);
	pNextSyncUpdateTime = 0.0f;
	
	if(pResetAdaptedIntensity){
		pResetAdaptedIntensity = false;
		pRCamera->SetForceToneMapAdaption(true);
	}
	
	// effects
	const int effectCount = pCamera.GetEffectCount();
	int i;
	
	if(pDirtyEffects){
		pRCamera->RemoveAllEffects();
		for(i=0; i<effectCount; i++){
			pRCamera->AddEffect(((deoglEffect*)pCamera.GetEffectAt(i)->GetPeerGraphic())->GetREffect());
		}
		pDirtyEffects = false;
// 			pOgl.LogInfoFormat( "Camera.Sync effects structure: %d ys", (int)(timer.GetElapsedTime() * 1e6f) );
	}
	
	for(i=0; i<effectCount; i++){
		((deoglEffect*)pCamera.GetEffectAt(i)->GetPeerGraphic())->SyncToRender();
	}
// 		pOgl.LogInfoFormat( "Camera.Sync effects sync: %d ys", (int)(timer.GetElapsedTime() * 1e6f) );
	
	// vr
	if(pDirtyVR){
		pRCamera->EnableVR(pEnableVR);
		if(! pEnableVR && pOgl.GetVRCamera() == this){
			pOgl.SetVRCamera(nullptr);
		}
		pDirtyVR = false;
	}
}



// Notifications
//////////////////

void deoglCamera::PositionChanged(){
	pDirtyGeometry = true;
	pDirtyMatrices = true;
	pDirtyPropFields = true;
}

void deoglCamera::OrientationChanged(){
	pDirtyMatrices = true;
}

void deoglCamera::ParameterChanged(){
	pDirtyPlanCamParams = true;
}

void deoglCamera::AdaptionChanged(){
	pDirtyAdaptionParams = true;
}

void deoglCamera::LayerMaskChanged(){
	pDirtyLayerMask = true;
}

void deoglCamera::ResetAdaptedIntensity(){
	pResetAdaptedIntensity = true;
}



void deoglCamera::EffectAdded(int index, deEffect *effect){
	pDirtyEffects = true;
}

void deoglCamera::EffectRemoved(int index, deEffect *effect){
	pDirtyEffects = true;
}

void deoglCamera::AllEffectsRemoved(){
	pDirtyEffects = true;
}




// For use by VR Module only
//////////////////////////////

void deoglCamera::VRAssignedToHMD(){
	pDirtyVR = true;
	pEnableVR = true;
	pOgl.SetVRCamera(this);
}

void deoglCamera::VRResignedFromHMD(){
	//pOgl.SetVRCamera( nullptr ); // not done here or sync is missing
	pDirtyPlanCamParams = true;
	pDirtyVR = true;
	pEnableVR = false;
}

void deoglCamera::VRRenderParametersChanged(){
	pDirtyVR = true;
}



// Private Functions
//////////////////////

void deoglCamera::pCleanUp(){
	SetParentWorld(NULL);
	
	if(pRCamera){
		pRCamera->FreeReference();
	}
	
	// notify owners we are about to be deleted. required since owners hold only a weak pointer
	// to the dynamic skin and are notified only after switching to a new dynamic skin. in this
	// case they can not use the old pointer to remove themselves from the dynamic skin
	int i, count = pNotifyRenderables.GetCount();
	for(i=0; i<count; i++){
		((deoglDSRenderableCamera*)pNotifyRenderables.GetAt(i))->DropCamera();
	}
	
	count = pNotifyCanvas.GetCount();
	for(i=0; i<count; i++){
		((deoglCanvasRenderWorld*)pNotifyCanvas.GetAt(i))->DropCamera();
	}
}

void deoglCamera::pRequiresSync(){
	int i, count = pNotifyRenderables.GetCount();
	for(i=0; i<count; i++){
		((deoglDSRenderableCamera*)pNotifyRenderables.GetAt(i))->CameraRequiresSync();
	}
	
	count = pNotifyCanvas.GetCount();
	for(i=0; i<count; i++){
		((deoglCanvasRenderWorld*)pNotifyCanvas.GetAt(i))->CameraRequiresSync();
	}
}
