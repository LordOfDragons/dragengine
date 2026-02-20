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

#include "deoglSkinState.h"
#include "deoglSkinStateRenderable.h"
#include "deoglSkinStateMapped.h"
#include "deoglSkinStateBone.h"
#include "deoglSkinStateCalculated.h"
#include "deoglSkinStateConstructed.h"
#include "../deoglRSkin.h"
#include "../deoglSkinRenderable.h"
#include "../dynamic/deoglRDynamicSkin.h"
#include "../dynamic/renderables/render/deoglRDSRenderable.h"
#include "../../billboard/deoglRBillboard.h"
#include "../../component/deoglRComponent.h"
#include "../../component/deoglRComponentTexture.h"
#include "../../decal/deoglRDecal.h"
#include "../../light/deoglRLight.h"
#include "../../rendering/plan/deoglRenderPlan.h"
#include "../../rendering/plan/deoglRenderPlanMasked.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../video/deoglRVideoPlayer.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class deoglSkinState
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinState::deoglSkinState(deoglRenderThread &renderThread) :
pRenderThread(renderThread)
{
	(void)pRenderThread; // silence compiler warning
	pSharedInit();
}

deoglSkinState::deoglSkinState(deoglRenderThread &renderThread, deoglRComponent &component, int texture) :
pRenderThread(renderThread)
{
	pSharedInit();
	pOwnerComponent = &component;
	pOwnerComponentTexture = texture;
}

deoglSkinState::deoglSkinState(deoglRenderThread &renderThread, deoglRBillboard &billboard) :
pRenderThread(renderThread)
{
	pSharedInit();
	pOwnerBillboard = &billboard;
}

deoglSkinState::deoglSkinState(deoglRenderThread &renderThread, deoglRDecal &decal) :
pRenderThread(renderThread)
{
	pSharedInit();
	pOwnerDecal = &decal;
}

deoglSkinState::deoglSkinState(deoglRenderThread &renderThread, deoglRLight &light) :
pRenderThread(renderThread)
{
	pSharedInit();
	pOwnerLight = &light;
}

deoglSkinState::~deoglSkinState() = default;



// Management
///////////////

deoglRSkin *deoglSkinState::GetOwnerSkin() const{
	if(pOwnerComponent){
		if(pOwnerComponentTexture != -1){
			return pOwnerComponent->GetTextureAt(pOwnerComponentTexture).GetSkin();
			
		}else{
			return pOwnerComponent->GetSkin();
		}
		
	}else if(pOwnerBillboard){
		return pOwnerBillboard->GetSkin();
		
	}else if(pOwnerDecal){
		return pOwnerDecal->GetSkin();
		
	}else if(pOwnerLight){
		return pOwnerLight->GetLightSkin();
		
	}else{
		return nullptr;
	}
}

void deoglSkinState::DropOwner(){
	pOwnerComponent = nullptr;
	pOwnerBillboard = nullptr;
	pOwnerDecal = nullptr;
	pOwnerLight = nullptr;
}

deoglRDynamicSkin *deoglSkinState::GetOwnerDynamicSkin() const{
	if(pOwnerComponent){
		if(pOwnerComponentTexture != -1){
			const deoglRDynamicSkin::Ref &dynamicSkin =
				pOwnerComponent->GetTextureAt(pOwnerComponentTexture).GetDynamicSkin();
			return dynamicSkin ? dynamicSkin : pOwnerComponent->GetDynamicSkin();
			
		}else{
			return pOwnerComponent->GetDynamicSkin();
		}
		
	}else if(pOwnerBillboard){
		return pOwnerBillboard->GetDynamicSkin();
		
	}else if(pOwnerDecal){
		return pOwnerDecal->GetUseDynamicSkin();
		
	}else if(pOwnerLight){
		return pOwnerLight->GetDynamicSkin();
		
	}else{
		return nullptr;
	}
}



void deoglSkinState::SetTime(float time){
	pTime = time;
}

void deoglSkinState::AdvanceTime(float timeStep){
	pTime += timeStep;
}



void deoglSkinState::SetVideoPlayerCount(int count){
	if(count == pVideoPlayers.GetCount()){
		return;
	}
	
	pVideoPlayers.RemoveAll();
	
	int i;
	for(i=0; i<count; i++){
		pVideoPlayers.Add(nullptr);
	}
}

void deoglSkinState::SetVideoPlayerAt(int index, deoglRVideoPlayer *videoPlayer){
	pVideoPlayers.SetAt(index, videoPlayer);
}



void deoglSkinState::PrepareRenderables(deoglRSkin *skin, deoglRDynamicSkin *dynamicSkin,
const deoglRenderPlanMasked *renderPlanMask){
	const int videoPlayerCount = pVideoPlayers.GetCount();
	int i;
	for(i=0; i< videoPlayerCount; i++){
		deoglRVideoPlayer * const videoPlayer = pVideoPlayers.GetAt(i);
		if(videoPlayer){
			videoPlayer->UpdateTexture();
		}
	}
	
	pRenderables.Visit([&](deoglSkinStateRenderable &r){
		r.Clear();
		
		const int hostIndex = r.GetHostRenderable();
		if(hostIndex != -1 && dynamicSkin){
			dynamicSkin->GetRenderableAt(hostIndex)->PrepareForRender(renderPlanMask);
		}
	});
}

void deoglSkinState::RenderRenderables(deoglRSkin *skin, deoglRDynamicSkin *dynamicSkin,
const deoglRenderPlanMasked *renderPlanMask){
	pRenderables.Visit([&](deoglSkinStateRenderable &r){
		r.Clear();
		
		const int hostIndex = r.GetHostRenderable();
		if(hostIndex != -1 && dynamicSkin){
			dynamicSkin->GetRenderableAt(hostIndex)->Render(renderPlanMask);
		}
	});
}

void deoglSkinState::AddRenderPlans(deoglRenderPlan &plan){
	// HACK: preventing a dead loop. has to be made better
	if(plan.GetLevel() > 0){
		return;
	}
	
	// TODO: currently only the mirror related plans are properly added.
	// renderable related plans are wrapped by a dirty-skin test and can be
	// skipped the second time.
	// required to verify if this is an issue or not ( and acting accordingly ).
	pRenderables.Visit([&](deoglSkinStateRenderable &r){
		r.AddRenderPlans(plan);
	});
}



// Renderables
////////////////

deoglSkinStateRenderable *deoglSkinState::GetRenderableAt(int index){
	return pRenderables.GetAt(index);
}

deoglSkinStateRenderable *deoglSkinState::AddRenderable(){
	pRenderables.Add(deoglSkinStateRenderable::Ref::New(*this, pRenderables.GetCount()));
	return pRenderables.Last();
}

void deoglSkinState::AddRenderables(deoglRSkin &skin, deoglRDynamicSkin &dynamicSkin){
	const int count = skin.GetRenderableCount();
	int i;
	
	for(i=0; i<count; i++){
		deoglSkinStateRenderable * const renderable = AddRenderable();
		renderable->SetHostRenderable(dynamicSkin.IndexOfRenderableNamed(skin.GetRenderableAt(i).GetName()));
	}
}

void deoglSkinState::RemoveAllRenderables(){
	pRenderables.RemoveAll();
}



// Mapped
///////////

deoglSkinStateMapped &deoglSkinState::GetMappedAt(int index){
	return pMapped[index];
}

void deoglSkinState::SetMappedCount(int count){
	pMapped.SetAll(count, {});
}



// Calculated properties
//////////////////////////

deoglSkinStateCalculated &deoglSkinState::GetCalculatedPropertyAt(int index){
	return pCalculatedProperties[index];
}

void deoglSkinState::SetCalculatedPropertyCount(int count){
	pCalculatedProperties.SetAll(count, {});
}



// Constructed properties
///////////////////////////

deoglSkinStateConstructed &deoglSkinState::GetConstructedPropertyAt(int index){
	return pConstructedProperties[index];
}

void deoglSkinState::SetConstructedPropertyCount(int count){
	pConstructedProperties.SetAll(count, {});
}

void deoglSkinState::PrepareConstructedProperties(){
	pConstructedProperties.Visit([&](deoglSkinStateConstructed &prop){
		prop.PrepareForRender(*this);
	});
}



// Bones
///////////

deoglSkinStateBone &deoglSkinState::GetBoneAt(int index){
	return pBones[index];
}

void deoglSkinState::SetBoneCount(int count){
	pBones.SetAll(count, {});
}



void deoglSkinState::InitAll(){
	deoglRSkin * const skin = GetOwnerSkin();
	
	if(!skin){
		SetMappedCount(0);
		SetBoneCount(0);
		SetCalculatedPropertyCount(0);
		SetConstructedPropertyCount(0);
		return;
	}
	
	SetMappedCount(skin->GetMappedCount());
	pMapped.VisitIndexed([&](int i, deoglSkinStateMapped &m){
		m.SetMapped(skin->GetMappedAt(i));
		m.SetComponent(pOwnerComponent);
	});
	
	SetBoneCount(skin->GetBoneCount());
	pBones.VisitIndexed([&](int i, deoglSkinStateBone &b){
		b.SetSkinBone(skin->GetBoneAt(i));
	});
	
	SetCalculatedPropertyCount(skin->GetCalculatedPropertyCount());
	pCalculatedProperties.VisitIndexed([&](int i, deoglSkinStateCalculated &p){
		p.SetProperty(skin->GetCalculatedPropertyAt(i));
	});
	
	SetConstructedPropertyCount(skin->GetConstructedPropertyCount());
	pConstructedProperties.VisitIndexed([&](int i, deoglSkinStateConstructed &p){
		p.SetProperty(skin->GetConstructedPropertyAt(i));
	});
}

void deoglSkinState::UpdateAll(){
	pMapped.Visit([&](deoglSkinStateMapped &m){
		m.Update(*this);
	});
	pCalculatedProperties.Visit([&](deoglSkinStateCalculated &p){
		p.Update(*this);
	});
	pConstructedProperties.Visit([&](deoglSkinStateConstructed &p){
		p.Update(*this);
	});
}

void deoglSkinState::MapBonesAll(const deComponent &component){
	pMapped.Visit([&](deoglSkinStateMapped &m){
		m.MapBone(component);
	});
	pBones.Visit([&](deoglSkinStateBone &b){
		b.MapBone(component);
	});
}

void deoglSkinState::UpdateBonesAll(const deComponent &component){
	pMapped.Visit([&](deoglSkinStateMapped &m){
		m.UpdateBone(component);
	});
	pBones.Visit([&](deoglSkinStateBone &b){
		b.UpdateBone(component);
	});
}



void deoglSkinState::SetVariationSeed(const decPoint &seed){
	pVariationSeed = seed;
}



// Private Functions
//////////////////////

void deoglSkinState::pSharedInit(){
	pOwnerComponent = nullptr;
	pOwnerComponentTexture = 0;
	pOwnerBillboard = nullptr;
	pOwnerDecal = nullptr;
	pOwnerLight = nullptr;
	
	pTime = 0.0f;
	
	pVariationSeed.x = (int)(((float)rand() / (float)RAND_MAX) * 100.0f);
	pVariationSeed.y = (int)(((float)rand() / (float)RAND_MAX) * 100.0f);
}
