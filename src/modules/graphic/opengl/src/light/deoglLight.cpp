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

#include "deoglLight.h"
#include "deoglLightGatherOcclusionMeshes.h"
#include "deoglRLight.h"
#include "shader/deoglLightShader.h"
#include "shader/deoglLightShaderManager.h"
#include "volume/deoglLightVolume.h"
#include "volume/deoglLightVolumeBuilder.h"

#include "../deGraphicOpenGl.h"
#include "../canvas/deoglCanvasView.h"
#include "../collidelist/deoglCollideList.h"
#include "../collidelist/deoglCollideListComponent.h"
#include "../component/deoglComponent.h"
#include "../configuration/deoglConfiguration.h"
#include "../occlusiontest/deoglOcclusionTest.h"
#include "../occlusiontest/mesh/deoglOcclusionMesh.h"
#include "../occquery/deoglOcclusionQuery.h"
#include "../optimizer/deoglOptimizerLight.h"
#include "../optimizer/deoglOptimizerManager.h"
#include "../rendering/plan/deoglRenderPlan.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTShader.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../shadow/deoglSCSolid.h"
#include "../shadow/deoglSCTransparent.h"
#include "../shadow/deoglShadowCaster.h"
#include "../skin/deoglSkin.h"
#include "../skin/dynamic/deoglDynamicSkin.h"
#include "../skin/state/deoglSkinStateController.h"
#include "../triangles/deoglTriangleSorter.h"
#include "../visitors/deoglTransformVolume.h"
#include "../world/deoglWorld.h"
#include "../utils/collision/deoglDCollisionBox.h"
#include "../utils/collision/deoglDCollisionDetection.h"
#include "../utils/collision/deoglDCollisionSphere.h"
#include "../utils/collision/deoglDCollisionTriangle.h"
#include "../utils/collision/deoglDCollisionVolume.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decConvexVolume.h>
#include <dragengine/common/math/decConvexVolumeFace.h>
#include <dragengine/common/math/decConvexVolumeList.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>
#include <dragengine/resources/light/deLight.h>
#include <dragengine/resources/world/deWorld.h>



// Class deoglLight
/////////////////////

// Constructor, destructor
////////////////////////////

deoglLight::deoglLight(deGraphicOpenGl &ogl, const deLight &light) :
pOgl(ogl),
pLight(light),
pSkinStateController(NULL),
pParentWorld(NULL),
pDynamicSkin(NULL),
pLightSkin(NULL),
pLightCanvas(NULL),

pAccumUpdate(0.0f),

pDirtyAttenuation(true),
pDirtyCollideLists(true),
pDirtyColVol(true),
pDirtyConvexVolumeList(true),
pDirtyExtends(true),
pDirtyFullExtends(true),
pDirtyLight(true),
pDirtyLightParameters(true),
pDirtyMatrices(true),
pDirtyOctree(true),
pDirtyShadowParameters(true),
pDirtyShadows(true),
pDirtySource(true),
pDirtyDynamicSkin(false),
pDirtyTouching(true),
pDirtyTransform(true),
pDirtyType(true),
pDirtyEnvMapNotifyLightChanged(true),

pDirtyRenderableMapping(true),
pDirtySkinStateController(true),
pDirtySkinStateStates(true),
pSkinStatePrepareRenderables(true),
pDynamicSkinRenderablesChanged(true),
pDynamicSkinRequiresSync(false),
pLightCanvasRequiresSync(false),
pRequiresUpdateEverySync(false),

pLLSyncWorld(this)
{
	try{
		pRLight = deoglRLight::Ref::New(ogl.GetRenderThread());
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglLight::~deoglLight(){
	pCleanUp();
}



// Management
///////////////

void deoglLight::SetParentWorld(deoglWorld *world){
	if(world == pParentWorld){
		return;
	}
	
	if(pSkinStateController){
		pSkinStateController->Clear();
	}
	
	pParentWorld = world;
	
	pDirtyOctree = true;
	pDirtySkinStateController = true;
}

void deoglLight::SyncToRender(){
	if(pDirtyLight){
		pRLight->SetActive(pLight.GetActivated());
		pRLight->SetSpotAngle(pLight.GetSpotAngle());
		pRLight->SetSpotRatio(pLight.GetSpotRatio());
		pRLight->SetHintMovement(pLight.GetHintMovement());
		pRLight->SetHintShadowImportance(pLight.GetHintShadowImportance());
		
		pDirtyLight = false;
		if(pLight.GetHintMovement() != deLight::emhDynamic){
			pDirtyEnvMapNotifyLightChanged = true;
		}
		// TODO use parameter hint
	}
	
	if(pDirtyType){
		pRLight->SetLightType(pLight.GetType());
		pRLight->DropPipelines();
		
		pDirtyType = false;
		if(pLight.GetHintMovement() != deLight::emhDynamic){
			pDirtyEnvMapNotifyLightChanged = true;
		}
	}
	
	if(pDirtyLightParameters){
		pRLight->SetIntensity(pLight.GetIntensity());
		pRLight->SetAmbientRatio(pLight.GetAmbientRatio());
		pRLight->SetColor(pLight.GetColor());
		pRLight->SetSpotSmoothness(pLight.GetSpotSmoothness());
		pRLight->SetSpotExponent(pLight.GetSpotExponent());
		pRLight->SetLayerMask(pLight.GetLayerMask());
		
		pDirtyLightParameters = false;
		if(pLight.GetHintMovement() == deLight::emhStationary){
			//pDirtyEnvMapNotifyLightChanged = true; // track if changed enough
		}
		// TODO use parameter hint
	}
	
	if(pDirtyShadowParameters){
		pRLight->SetCastShadows(pLight.GetCastShadows());
		pRLight->SetLayerMaskShadow(pLight.GetLayerMaskShadow());
		
		pRLight->RemoveAllShadowIgnoreComponents();
		const int ignoreCount = pLight.GetShadowIgnoreComponentCount();
		int i;
		for(i=0; i<ignoreCount; i++){
			pRLight->AddShadowIgnoreComponent(((deoglComponent*)pLight.
				GetShadowIgnoreComponentAt(i)->GetPeerGraphic())->GetRComponent());
		}
		
		pDirtyShadowParameters = false;
		if(pLight.GetHintMovement() != deLight::emhDynamic){
			pDirtyEnvMapNotifyLightChanged = true;
		}
	}
	
	if(pDirtyAttenuation){
		pRLight->UpdateAttenuation(pLight.GetRange(), pLight.GetHalfIntensityDistance());
		
		pDirtyAttenuation = false;
		if(pLight.GetHintMovement() != deLight::emhDynamic){
			pDirtyEnvMapNotifyLightChanged = true;
		}
	}
	
	if(pDirtyTransform){
		pRLight->SetTransform(pLight.GetTransform());
		pDirtyTransform = false;
	}
	
	pSyncSource();
	
	if(pDirtyRenderableMapping){
		pRLight->UpdateRenderableMapping();
		pDirtyRenderableMapping = false;
	}
	
	if(pDirtySkinStateController){
		if(pSkinStateController && pRLight->GetSkinState()){
			pSkinStateController->Init(*pRLight->GetSkinState(), pRLight->GetLightSkin(), pParentWorld);
		}
		
		pDirtySkinStateController = false;
	}
	
	if(pAccumUpdate > 0.0f){
		if(pSkinStateController){
			pSkinStateController->AdvanceTime(pAccumUpdate);
			pSkinStateController->SyncToRender();
		}
		
		pRLight->UpdateSkin(pAccumUpdate);
		
		pAccumUpdate = 0.0f;
	}
	
	// sync skin state properties. has to come after pSkinStateController->SyncToRender()
	// and pRComponent->UpdateSkin()
	if(pDirtySkinStateStates){
		pRLight->InitSkinStateStates();
		pDirtySkinStateStates = false;
	}
	
	if(pSkinStatePrepareRenderables){
		pRLight->DirtyPrepareSkinStateRenderables();
		pSkinStatePrepareRenderables = false;
	}
	
	pRLight->UpdateSkinStateStates(); // has to be done better. only some need this
	
	if(pDirtyMatrices){
		pRLight->SetMatrix(decDMatrix::CreateWorld(pLight.GetPosition(), pLight.GetOrientation()));
		pDirtyMatrices = false;
		
		if(pLight.GetHintMovement() == deLight::emhStationary){ // jitter: if change is large enough?
			pDirtyEnvMapNotifyLightChanged = true;
		}
	}
	
	if(pDirtyFullExtends){
		pRLight->SetDirtyFullExtends();
		pDirtyFullExtends = false;
	}
	
	if(pDirtyExtends){
		pRLight->SetDirtyExtends();
		pDirtyExtends = false;
	}
	
	if(pDirtyCollideLists){
		pRLight->SetDirtyCollideLists();
		pDirtyCollideLists = false;
	}
	
	if(pDirtyColVol){
		pRLight->SetDirtyCollisionVolume();
		pDirtyColVol = false;
	}
	
	if(pDirtyConvexVolumeList){
		pRLight->SetLightVolumeCropBox(NULL);
		pRLight->SetLightVolumeDirty();
		pDirtyConvexVolumeList = false;
	}
	
	if(pDirtyShadows){
		pRLight->SetDirtyShadows();
		pDirtyShadows = false;
	}
	
	if(pDirtyTouching){
		pRLight->SetDirtyTouching();
		pDirtyTouching = false;
	}
	
	if(pDirtyOctree){
		pRLight->UpdateOctreeNode();
		pDirtyOctree = false;
	}
	
	if(pDirtyEnvMapNotifyLightChanged){
		pRLight->EnvMapNotifyLightChanged();
		pDirtyEnvMapNotifyLightChanged = false;
	}
}

void deoglLight::DynamicSkinRenderableRequiresSync(deoglDSRenderable &renderable){
	pDynamicSkinRequiresSync = true;
	pSkinStatePrepareRenderables = true;
	
	pRequiresSync();
}

void deoglLight::DirtyRenderableMapping(){
	pDirtyRenderableMapping = true;
	
	pRequiresSync();
}


// Dynamic skin listener
//////////////////////////

void deoglLight::DynamicSkinDestroyed(){
	pDynamicSkin = NULL;
}

void deoglLight::DynamicSkinRenderablesChanged(){
	pDynamicSkinRenderablesChanged = true;
	pDynamicSkinRequiresSync = true;
	pDirtyRenderableMapping = true;
	pSkinStatePrepareRenderables = true;
	
	pRequiresSync();
}

void deoglLight::DynamicSkinRenderableChanged(deoglDSRenderable&){
	pDynamicSkinRenderablesChanged = true;
	pDynamicSkinRequiresSync = true;
	pSkinStatePrepareRenderables = true;
	
	pRequiresSync();
}

void deoglLight::DynamicSkinRequiresSync(){
	pDynamicSkinRequiresSync = true;
	pSkinStatePrepareRenderables = true;
	
	pRequiresSync();
}



// Canvas view listener
/////////////////////////

void deoglLight::CanvasViewDestroyed(){
	pLightCanvas = NULL;
}

void deoglLight::CanvasViewRequiresSync(){
	pLightCanvasRequiresSync = true;
	
	pRequiresSync();
}



// Notifications
//////////////////

void deoglLight::TypeChanged(){
	pDirtyCollideLists = true;
	pDirtyColVol = true;
	pDirtyConvexVolumeList = true;
	pDirtyExtends = true;
	pDirtyFullExtends = true;
	pDirtyOctree = true;
	pDirtyShadows = true;
	pDirtyTouching = true;
	pDirtyType = true;
}

void deoglLight::GeometryParameterChanged(){
	pDirtyAttenuation = true;
	pDirtyCollideLists = true;
	pDirtyColVol = true;
	pDirtyConvexVolumeList = true;
	pDirtyExtends = true;
	pDirtyFullExtends = true;
	pDirtyLight = true;
	pDirtyOctree = true;
	pDirtyShadows = true;
	pDirtyTouching = true;
}

void deoglLight::AttenuationChanged(){
	pDirtyAttenuation = true;
	pDirtyCollideLists = true;
	pDirtyColVol = true;
	pDirtyConvexVolumeList = true;
	pDirtyExtends = true;
	pDirtyFullExtends = true;
	pDirtyOctree = true;
	pDirtyShadows = true;
	pDirtyTouching = true;
}

void deoglLight::LightParameterChanged(){
	pDirtyLightParameters = true;
}

void deoglLight::ShadowParameterChanged(){
	pDirtyCollideLists = true;
	pDirtyColVol = true;
	pDirtyConvexVolumeList = true;
	pDirtyExtends = true;
	pDirtyFullExtends = true;
	pDirtyOctree = true;
	pDirtyShadowParameters = true;
	pDirtyShadows = true;
	pDirtyTouching = true;
}

void deoglLight::ShapeChanged(){
	pDirtyAttenuation = true;
	pDirtyCollideLists = true;
	pDirtyColVol = true;
	pDirtyConvexVolumeList = true;
	pDirtyExtends = true;
	pDirtyFullExtends = true;
	pDirtyLight = true;
	pDirtyOctree = true;
	pDirtyShadows = true;
	pDirtyTouching = true;
	pDirtyType = true;
}

void deoglLight::LayerMaskChanged(){
	pDirtyLightParameters = true;
}

void deoglLight::ShadowLayerMaskChanged(){
	pDirtyCollideLists = true;
	pDirtyColVol = true;
	pDirtyConvexVolumeList = true;
	pDirtyExtends = true;
	pDirtyFullExtends = true;
	pDirtyOctree = true;
	pDirtyShadowParameters = true;
	pDirtyShadows = true;
	pDirtyTouching = true;
}

void deoglLight::ShadowIgnoreComponentsChanged(){
	pDirtyCollideLists = true;
	pDirtyColVol = true;
	pDirtyConvexVolumeList = true;
	pDirtyExtends = true;
	pDirtyFullExtends = true;
	pDirtyOctree = true;
	pDirtyShadowParameters = true;
	pDirtyShadows = true;
	pDirtyTouching = true;
}

void deoglLight::PositionChanged(){
	pDirtyCollideLists = true;
	pDirtyColVol = true;
	pDirtyConvexVolumeList = true;
	pDirtyExtends = true;
	pDirtyFullExtends = true;
	pDirtyMatrices = true;
	pDirtyOctree = true;
	pDirtyShadows = true;
	pDirtyTouching = true;
}

void deoglLight::OrientationChanged(){
	pDirtyCollideLists = true;
	pDirtyColVol = true;
	pDirtyConvexVolumeList = true;
	pDirtyExtends = true;
	pDirtyFullExtends = true;
	pDirtyMatrices = true;
	pDirtyOctree = true;
	pDirtyShadows = true;
	pDirtyTouching = true;
}

void deoglLight::SourceChanged(){
	deoglDynamicSkin * const dynamicSkin = pLight.GetDynamicSkin()
		? (deoglDynamicSkin*)pLight.GetDynamicSkin()->GetPeerGraphic() : NULL;
	
	if(dynamicSkin != pDynamicSkin){
		if(pDynamicSkin){
			pDynamicSkin->RemoveListener(this);
		}
		
		pDynamicSkin = dynamicSkin;
		
		if(dynamicSkin){
			dynamicSkin->AddListener(this);
		}
		
		pDirtyDynamicSkin = true;
		pDynamicSkinRequiresSync = true;
		pDynamicSkinRenderablesChanged = true;
		pDirtySkinStateController = true;
		pDirtyRenderableMapping = true;
		pSkinStatePrepareRenderables = true;
	}
	
	// light skin
	deoglSkin * const lightSkin = pLight.GetLightSkin()
		? (deoglSkin*)pLight.GetLightSkin()->GetPeerGraphic() : NULL;
	
	if(lightSkin != pLightSkin){
		pLightSkin = lightSkin;
		pDirtySkinStateStates = true;
	}
	
	// light canvas
	deoglCanvasView * const lightCanvas = pLight.GetLightCanvas()
		? (deoglCanvasView*)pLight.GetLightCanvas()->GetPeerGraphic() : NULL;
	
	if(lightCanvas != pLightCanvas){
		if(pLightCanvas){
			pLightCanvas->RemoveListener(this);
		}
		
		pLightCanvas = lightCanvas;
		
		if(lightCanvas){
			lightCanvas->AddListener(this);
		}
		
		pLightCanvasRequiresSync = true;
	}
	
	pDirtySource = true;
	
	pRequiresSync();
}

void deoglLight::TransformChanged(){
	pDirtyTransform = true;
}

void deoglLight::ActivatedChanged(){
	pDirtyLight = true;
	pDirtyOctree = true;
	pDirtyShadows = true;
	pDirtyTouching = true;
}

void deoglLight::HintChanged(){
	pDirtyLight = true;
	pDirtyFullExtends = true;
	pDirtyExtends = true;
	pDirtyColVol = true;
	pDirtyCollideLists = true;
	pDirtyConvexVolumeList = true;
	pDirtyShadows = true;
}



// Private Functions
//////////////////////

void deoglLight::pCleanUp(){
	if(pSkinStateController){
		delete pSkinStateController;
	}
	if(pLightCanvas){
		pLightCanvas->RemoveListener(this);
	}
	if(pDynamicSkin){
		pDynamicSkin->RemoveListener(this);
	}
}

void deoglLight::pSyncSource(){
	if(!pDirtySource){
		return;
	}
	
	// light skin
	if(pLightSkin){
		pRLight->SetLightSkin(pLightSkin->GetRSkin());
		
		if(!pSkinStateController){
			pSkinStateController = new deoglSkinStateController;
		}
		
	}else{
		pRLight->SetLightSkin(NULL);
	}
	
	// dynamic skin
	pRLight->SetDynamicSkin(pDynamicSkin ? pDynamicSkin->GetRDynamicSkin() : nullptr);
	
	if(pDynamicSkinRenderablesChanged){
		pDynamicSkinRenderablesChanged = false;
		pRLight->DynamicSkinRenderablesChanged();
	}
	
	if(pDynamicSkinRequiresSync){
		pDynamicSkinRequiresSync = false;
		if(pDynamicSkin){
			pDynamicSkin->SyncToRender();
		}
	}
	
	// light canvas
	pRLight->SetLightCanvas(pLightCanvas ? pLightCanvas->GetRCanvasView() : nullptr);
	
	if(pLightCanvasRequiresSync){
		pLightCanvasRequiresSync = false;
		if(pLightCanvas){
			pLightCanvas->SyncToRender();
		}
	}
	
	// common
	pRLight->DropPipelines();
	
	pDirtySource = false;
	
	if(pLight.GetHintMovement() != deLight::emhDynamic){
		pDirtyEnvMapNotifyLightChanged = true;
	}
}

void deoglLight::pCheckRequiresUpdateEverySync(){
	if(pSkinStateController->RequiresSyncEveryFrameUpdate()){
		pRequiresUpdateEverySync = true;
		if(pSkinStateController->RequiresPrepareRenderables()){
			pRLight->DirtyPrepareSkinStateRenderables();
		}
	}
}

void deoglLight::pRequiresSync(){
// 	if( ! pLLSyncWorld.GetList() && pParentWorld ){
// 		pParentWorld->AddSyncLight( this );
// 	}
}
