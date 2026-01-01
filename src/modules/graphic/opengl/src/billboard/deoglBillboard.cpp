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

#include "deoglBillboard.h"
#include "deoglRBillboard.h"
#include "../deGraphicOpenGl.h"
#include "../skin/deoglSkin.h"
#include "../skin/dynamic/deoglDynamicSkin.h"
#include "../skin/state/deoglSkinStateController.h"
#include "../world/deoglWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/billboard/deBillboard.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>



// Class deoglBillboard
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglBillboard::deoglBillboard(deGraphicOpenGl &ogl, const deBillboard &billboard) :
pOgl(ogl),
pBillboard(billboard),

pParentWorld(NULL),
pSkinStateController(NULL),
pDynamicSkin(NULL),

pAccumUpdate(0.0f),

pDirtyGeometry(true),
pDirtyOctreeNode(true),
pDirtyExtends(true),
pDirtyLayerMask(true),
pDirtySkinStateController(true),
pDirtyCullSphere(true),
pDirtyRenderEnvMap(true),
pDirtySkin(true),
pDirtyDynamicSkin(true),
pDirtyRenderableMapping(true),
pDirtySkinStateStates(true),
pSkinStatePrepareRenderables(true),

pDynamicSkinRenderablesChanged(true),
pDynamicSkinRequiresSync(true),
pRequiresUpdateEverySync(false),

pLLSyncWorld(this)
{
	try{
		pRBillboard = deoglRBillboard::Ref::New(ogl.GetRenderThread());
		pSkinStateController = new deoglSkinStateController;
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglBillboard::~deoglBillboard(){
	pCleanUp();
}



// Management
///////////////

void deoglBillboard::SyncToRender(){
	pSyncSkin();
	pSyncDynamicSkin();
	
	if(pDirtyGeometry){
		pRBillboard->SetPosition(pBillboard.GetPosition());
		pRBillboard->SetAxis(pBillboard.GetAxis());
		pRBillboard->SetSize(pBillboard.GetSize());
		pRBillboard->SetOffset(pBillboard.GetOffset());
		pRBillboard->SetLocked(pBillboard.GetLocked());
		pRBillboard->SetSpherical(pBillboard.GetSpherical());
		pRBillboard->SetSizeFixedToScreen(pBillboard.GetSizeFixedToScreen());
		pRBillboard->MarkParamBlocksDirty();
		pDirtyGeometry = false;
	}
	if(pDirtyLayerMask){
		pRBillboard->SetLayerMask(pBillboard.GetLayerMask());
		pDirtyLayerMask = false;
	}
	if(pDirtyCullSphere){
		pRBillboard->SetDirtyCulling();
		pDirtyCullSphere = false;
	}
	if(pDirtyRenderEnvMap){
		pRBillboard->InvalidateRenderEnvMap();
		pDirtyRenderEnvMap = false;
	}
	if(pDirtyRenderableMapping){
		pRBillboard->UpdateRenderableMapping();
		pDirtyRenderableMapping = false;
	}
	
	// sync skin state controller
	if(pDirtySkinStateController){
		pSkinStateController->Init(*pRBillboard->GetSkinState(), pRBillboard->GetSkin(), pParentWorld);
		pDirtySkinStateController = false;
	}
	
	// accumulated times
	const float skinUpdate = pAccumUpdate;
	
	pSkinStateController->AdvanceTime(skinUpdate);
	pSkinStateController->SyncToRender();
	
	pRBillboard->UpdateSkin(skinUpdate);
	
	pAccumUpdate = 0.0f;
	pCheckRequiresUpdateEverySync();
	
	// sync skin state properties. has to come after pSkinStateController->SyncToRender()
	// and pRBillboard->UpdateSkin()
	if(pDirtySkinStateStates){
		pRBillboard->InitSkinStateStates();
		pDirtySkinStateStates = false;
	}
	
	if(pSkinStatePrepareRenderables){
		pRBillboard->DirtyPrepareSkinStateRenderables();
		pSkinStatePrepareRenderables = false;
	}
	
	pRBillboard->UpdateSkinStateStates(); // has to be done better. only some need this
	
	// octree, extends and matrices. order is important
	if(pDirtyExtends){
		pRBillboard->UpdateExtends(pBillboard);
		pDirtyExtends = false;
	}
	if(pDirtyOctreeNode){
		pRBillboard->SetVisible(pBillboard.GetVisible());
		pRBillboard->UpdateOctreeNode();
		pDirtyOctreeNode = false;
	}
}

void deoglBillboard::Update(float elapsed){
	pAccumUpdate += elapsed;
	
	if(pRequiresUpdateEverySync){
		pRequiresSync();
	}
}



void deoglBillboard::SetParentWorld(deoglWorld *parentWorld){
	if(parentWorld == pParentWorld){
		return;
	}
	
	if(pSkinStateController){
		pSkinStateController->Clear();
	}
	
	pParentWorld = parentWorld;
	
	pDirtyOctreeNode = true;
	pDirtySkinStateController = true;
	pDirtyRenderEnvMap = true;
}



// Dynamic skin listener
//////////////////////////

void deoglBillboard::DynamicSkinDestroyed(){
	pDynamicSkin = NULL;
}

void deoglBillboard::DynamicSkinRenderablesChanged(){
	pDynamicSkinRenderablesChanged = true;
	pDynamicSkinRequiresSync = true;
	pDirtyRenderableMapping = true;
// 	pDirtyStaticTexture = true;
	pSkinStatePrepareRenderables = true;
// 	pNotifyTexturesChanged = true;
// 	pDirtySolid = true;
	
	pRequiresSync();
}

void deoglBillboard::DynamicSkinRenderableChanged(deoglDSRenderable&){
	pDynamicSkinRenderablesChanged = true;
	pDynamicSkinRequiresSync = true;
// 	pDirtyStaticTexture = true;
	pSkinStatePrepareRenderables = true;
// 	pNotifyTexturesChanged = true;
// 	pDirtySolid = true;
	
	pRequiresSync();
}

void deoglBillboard::DynamicSkinRenderableRequiresSync(deoglDSRenderable&){
	pDynamicSkinRequiresSync = true;
	pSkinStatePrepareRenderables = true;
	
	pRequiresSync();
}



// Notifications
//////////////////

void deoglBillboard::PositionChanged(){
	pDirtyGeometry = true;
	pDirtyOctreeNode = true;
	pDirtyExtends = true;
	pDirtyCullSphere = true;
	pDirtyRenderEnvMap = true;
	
	pRequiresSync();
}

void deoglBillboard::AxisChanged(){
	pDirtyGeometry = true;
	pDirtyOctreeNode = true;
	pDirtyExtends = true;
	pDirtyCullSphere = true;
	pDirtyRenderEnvMap = true;
	
	pRequiresSync();
}

void deoglBillboard::SizeChanged(){
	pDirtyGeometry = true;
	pDirtyOctreeNode = true;
	pDirtyExtends = true;
	pDirtyCullSphere = true;
	pDirtyRenderEnvMap = true;
	
	pRequiresSync();
}

void deoglBillboard::OffsetChanged(){
	pDirtyGeometry = true;
	pDirtyOctreeNode = true;
	pDirtyExtends = true;
	pDirtyCullSphere = true;
	pDirtyRenderEnvMap = true;
	
	pRequiresSync();
}

void deoglBillboard::SkinChanged(){
	pDirtySkin = true;
	pDirtySkinStateController = true;
	pDirtySkinStateStates = true;
	pDirtyRenderableMapping = true;
	pSkinStatePrepareRenderables = true;
	
	pRequiresSync();
}

void deoglBillboard::DynamicSkinChanged(){
	if(pDynamicSkin){
		pDynamicSkin->RemoveListener(this);
	}
	
	if(pBillboard.GetDynamicSkin()){
		pDynamicSkin = (deoglDynamicSkin*)pBillboard.GetDynamicSkin()->GetPeerGraphic();
		pDynamicSkin->AddListener(this);
		
	}else{
		pDynamicSkin = NULL;
	}
	
	pDirtyDynamicSkin = true;
	pDynamicSkinRequiresSync = true;
	pDynamicSkinRenderablesChanged = true;
	pDirtyRenderableMapping = true;
	pSkinStatePrepareRenderables = true;
	
	pRequiresSync();
}

void deoglBillboard::ParametersChanged(){
	pDirtyGeometry = true;
	pDirtyOctreeNode = true;
	pDirtyExtends = true;
	pDirtyCullSphere = true;
	pDirtyRenderEnvMap = true;
	
	pRequiresSync();
}

void deoglBillboard::VisibilityChanged(){
	pDirtyOctreeNode = true;
	pDirtyExtends = true;
	
	pRequiresSync();
}

void deoglBillboard::LayerMaskChanged(){
	pDirtyLayerMask = true;
	
	pRequiresSync();
}



// Private Functions
//////////////////////

void deoglBillboard::pCleanUp(){
	if(pSkinStateController){
		delete pSkinStateController;
	}
	if(pDynamicSkin){
		pDynamicSkin->RemoveListener(this);
	}
}



void deoglBillboard::pSyncSkin(){
	if(!pDirtySkin){
		return;
	}
	
	if(pBillboard.GetSkin()){
		pRBillboard->SetSkin(((deoglSkin*)pBillboard.GetSkin()->GetPeerGraphic())->GetRSkin());
		
	}else{
		pRBillboard->SetSkin(NULL);
	}
	
	pDirtySkin = false;
}

void deoglBillboard::pSyncDynamicSkin(){
	if(pDirtyDynamicSkin){
		if(pDynamicSkin){
			pRBillboard->SetDynamicSkin(pDynamicSkin->GetRDynamicSkin());
			
		}else{
			pRBillboard->SetDynamicSkin(NULL);
		}
		
		pDirtyDynamicSkin = false;
	}
	
	if(pDynamicSkinRenderablesChanged){
		pDynamicSkinRenderablesChanged = false;
		pRBillboard->DynamicSkinRenderablesChanged();
	}
	
	if(pDynamicSkinRequiresSync){
		pDynamicSkinRequiresSync = false;
		if(pDynamicSkin){
			pDynamicSkin->SyncToRender();
		}
	}
}

void deoglBillboard::pCheckRequiresUpdateEverySync(){
	if(pSkinStateController->RequiresSyncEveryFrameUpdate()){
		pRequiresUpdateEverySync = true;
		if(pSkinStateController->RequiresPrepareRenderables()){
			pRBillboard->DirtyPrepareSkinStateRenderables();
		}
	}
}



void deoglBillboard::pRequiresSync(){
	if(!pLLSyncWorld.GetList() && pParentWorld){
		pParentWorld->AddSyncBillboard(this);
	}
}
