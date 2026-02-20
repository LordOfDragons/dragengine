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

#include "deoglGIState.h"
#include "deoglGIInstance.h"
#include "deoglGIInstances.h"
#include "../renderthread/deoglRenderThread.h"
#include "../collidelist/deoglCollideList.h"
#include "../collidelist/deoglCollideListComponent.h"
#include "../component/deoglRComponent.h"
#include "../component/deoglRComponentLOD.h"
#include "../decal/deoglRDecal.h"
#include "../model/deoglRModel.h"
#include "../model/deoglModelLOD.h"
#include "../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>


// Class deoglGIInstances
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglGIInstances::deoglGIInstances(deoglGIState &giState) :
pGIState(giState){
}

deoglGIInstances::~deoglGIInstances() = default;



// Management
///////////////

bool deoglGIInstances::IsComponentStatic(const deoglRComponent &component){
	// weight check catches more cases than GetRenderStatic() or GetRenderMode() can.
	// static textures is checked to avoid cached rays with changing textures
	return component.GetRenderStatic()
		&& component.GetStaticTextures()
		&& component.GetMovementHint() == deComponent::emhStationary
		&& component.GetLODAt(-1).GetModelLODRef().GetWeightsCounts().IsEmpty();
}

bool deoglGIInstances::IsDecalStatic(const deoglRDecal &decal){
	if(decal.GetParentComponent()){
		return decal.GetStaticTexture() && IsComponentStatic(*decal.GetParentComponent());
		
	}else{
		return true;
	}
}

deoglGIInstance &deoglGIInstances::GetInstanceAt(int slot) const{
	return pInstances[slot];
}

deoglGIInstance *deoglGIInstances::GetInstanceWithComponent(deoglRComponent *component) const{
#if 0
	if(!component){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pInstances.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deoglGIInstance * const instance = pInstances.GetAt(i);
		if(instance->GetComponent() == component){
			return instance;
		}
	}
	
	return NULL;
#endif
	
	return pElementInstanceMap.GetAtOrDefault(component->GetUniqueKey());
}

deoglGIInstance *deoglGIInstances::GetInstanceWithDecal(deoglRDecal *decal) const{
	return pElementInstanceMap.GetAtOrDefault(decal->GetUniqueKey());
}

deoglGIInstance &deoglGIInstances::AddInstance(){
	auto instance = deoglGIInstance::Ref::New(*this);
	pInstances.Add(instance);
	return instance;
}

deoglGIInstance &deoglGIInstances::NextFreeSlot(){
	if(pEmptyInstances.IsEmpty()){
		return AddInstance();
	}
	
	deoglGIInstance * const instance = pEmptyInstances.Last();
	pEmptyInstances.RemoveLast();
	return *instance;
}

void deoglGIInstances::RegisterElement(const deoglRComponent &component, deoglGIInstance *instance){
	RegisterElement(component.GetUniqueKey(), instance);
}

void deoglGIInstances::RegisterElement(const deoglRDecal &decal, deoglGIInstance *instance){
	RegisterElement(decal.GetUniqueKey(), instance);
}

void deoglGIInstances::RegisterElement(unsigned int uniqueKey, deoglGIInstance *instance){
	pElementInstanceMap.SetAt(uniqueKey, instance);
}

void deoglGIInstances::UnregisterElement(const deoglRComponent &component){
	UnregisterElement(component.GetUniqueKey());
}

void deoglGIInstances::UnregisterElement(const deoglRDecal &decal){
	UnregisterElement(decal.GetUniqueKey());
}

void deoglGIInstances::UnregisterElement(unsigned int uniqueKey){
	pElementInstanceMap.Remove(uniqueKey);
}



void deoglGIInstances::UpdateDynamicBoxes(const decDVector &offset, const decVector &enlarge){
	pDynamicBoxes.SetCountDiscard(0);
	
	const decVector halfEnlarge(enlarge * 0.5f);
	pInstances.Visit([&](const deoglGIInstance &instance){
		if(!instance.GetDynamic()){
			return;
		}
		
		pDynamicBoxes.Add({
			(instance.GetMinimumExtend() + offset).ToVector() - halfEnlarge,
			(instance.GetMaximumExtend() + offset).ToVector() + halfEnlarge});
	});
}

bool deoglGIInstances::DynamicBoxesContain(const decVector &point) const{
	return pDynamicBoxes.HasMatching([&](const sBox &box){
		return point >= box.minExtend && point <= box.maxExtend;
	});
}

int deoglGIInstances::CountDynamicBoxesContaining(const decVector &point) const{
	return pDynamicBoxes.Inject(0, [&](int acc, const sBox &box){
		return acc + (point >= box.minExtend && point <= box.maxExtend ? 1 : 0);
	});
}



void deoglGIInstances::Clear(){
	pEmptyInstances.SetCountDiscard(0);
	
	pInstances.Visit([&](deoglGIInstance *instance){
		instance->Clear();
		pEmptyInstances.Add(instance);
	});
	
	pChangedInstances.SetCountDiscard(0);
	pElementInstanceMap.RemoveAll();
}

void deoglGIInstances::ApplyChanges(){
	if(pChangedInstances.IsEmpty()){
		return;
	}
	
	pChangedInstances.Visit([&](deoglGIInstance *inst){
		deoglGIInstance &instance = *inst;
		bool invalidate = !instance.GetDynamic();
		
		if(instance.GetRecheckDynamic()){
			instance.SetRecheckDynamic(false);
			
			const bool dynamic = instance.GetDynamic();
			
			if(instance.GetComponent()){
				instance.SetDynamic(!IsComponentStatic(*instance.GetComponent()));
				
			}else if(instance.GetDecal()){
				instance.SetDynamic(!IsDecalStatic(*instance.GetDecal()));
				
			}else{
				instance.SetDynamic(false);
			}
			
			invalidate |= instance.GetDynamic() != dynamic;
			
			// we do not invalidate hard if the dynamic flag changes. this is because something
			// like switching a texture temporarily switches render static flag off which in
			// turn cause IsComponentStatic to return false. this would cause a simple texture
			// switch to invalidate hard all disabled probes. to get good results we have to
			// not wake up disabled probes whenever possible even if we run the risk of missing
			// a potential chance for disabled probes to wake up
			
			if(invalidate){
				pGIState.TouchDynamicArea(instance.GetMinimumExtend(), instance.GetMaximumExtend());
			}
		}
		
		if(invalidate){
// 				pGIState.GetRenderThread().GetLogger().LogInfoFormat("GIInstances.ApplyChanges: %s",
// 					instance.GetComponent()?instance.GetComponent()->GetModel()->GetFilename().GetString():"-");
			pGIState.InvalidateArea(instance.GetMinimumExtend(),
				instance.GetMaximumExtend(), instance.GetHardChanged());
		}
		
		if(instance.GetMoved()){
			instance.SetMoved(false);
			
			if(instance.GetComponent()){
				if(instance.GetDynamic()){
					pGIState.TouchDynamicArea(instance.GetMinimumExtend(), instance.GetMaximumExtend());
				}
				
				const decDVector &minExtend = instance.GetComponent()->GetMinimumExtend();
				const decDVector &maxExtend = instance.GetComponent()->GetMaximumExtend();
				instance.SetExtends(minExtend, maxExtend);
				
				if(instance.GetDynamic()){
					pGIState.TouchDynamicArea(minExtend, maxExtend);
				}
				
				if(invalidate){
// 						pGIState.GetRenderThread().GetLogger().LogInfoFormat("GIInstances.AnyChanged: Moved %s",
// 							instance.GetComponent()?instance.GetComponent()->GetModel()->GetFilename().GetString():"-");
					pGIState.InvalidateArea(minExtend, maxExtend, true);
				}
				
			}else if(instance.GetDecal()){
				if(instance.GetDynamic()){
					pGIState.TouchDynamicArea(instance.GetMinimumExtend(), instance.GetMaximumExtend());
				}
				
				instance.SetExtendsFromBVHLocal();
				const decDVector &minExtend = instance.GetMinimumExtend();
				const decDVector &maxExtend = instance.GetMaximumExtend();
				
				if(instance.GetDynamic()){
					pGIState.TouchDynamicArea(minExtend, maxExtend);
				}
				
				if(invalidate){
// 						pGIState.GetRenderThread().GetLogger().LogInfoFormat("GIInstances.AnyChanged: Moved %s",
// 							instance.GetComponent()?instance.GetComponent()->GetModel()->GetFilename().GetString():"-");
					pGIState.InvalidateArea(minExtend, maxExtend, true);
				}
			}
		}
		
		instance.ClearChanged();
	});
	
	pChangedInstances.RemoveAll();
}

// #define DO_LOG_ADD_REMOVE 1

void deoglGIInstances::AddComponent(deoglRComponent *component, bool invalidate){
	deoglGIInstance &instance = NextFreeSlot();
	instance.SetComponent(component, !IsComponentStatic(*component));
	if(invalidate){
		#ifdef DO_LOG_ADD_REMOVE
// 			pGIState.GetRenderThread().GetLogger().LogInfoFormat("GIInstances.AddComponent: %s",
// 				component->GetModel()->GetFilename().GetString());
		#endif
		pInvalidateAddInstance(instance);
	}
	
	#ifdef DO_LOG_ADD_REMOVE
		{
		int j, index = -1;
		for(j=0; j<pInstances.GetCount(); j++){
			if(pInstances.GetAt(j)->GetComponent() == component){
				index = j;
				break;
			}
		}
		const decDVector p(component->GetMatrix().GetPosition());
		pGIState.GetRenderThread().GetLogger().LogInfoFormat("GIInstances: AddComponent: %d (%g,%g,%g) component=%s [%d] invalidate=%d",
			index, p.x, p.y, p.z, component->GetModel() ? component->GetModel()->GetFilename().GetString() : "-", isStatic, invalidate);
		}
	#endif
}

void deoglGIInstances::AddComponents(const deoglCollideList &list, bool invalidate){
	list.GetComponents().Visit([&](const deoglCollideListComponent &clc){
		AddComponent(clc.GetComponent(), invalidate);
	});
}

void deoglGIInstances::AddDecal(deoglRDecal *decal, bool invalidate){
	deoglGIInstance &instance = NextFreeSlot();
	instance.SetDecal(decal, !IsDecalStatic(*decal));
	if(invalidate){
		pInvalidateAddInstance(instance);
	}
}

void deoglGIInstances::AddDecals(const deoglRComponent &component, bool invalidate){
	component.GetDecals().Visit([&](deoglRDecal *decal){
		AddDecal(decal, invalidate);
	});
}

void deoglGIInstances::RemoveComponent(deoglRComponent *component){
	deoglGIInstance * const instance = GetInstanceWithComponent(component);
	if(instance){
		RemoveInstance(*instance);
	}
}

void deoglGIInstances::RemoveComponents(const deoglCollideList &list){
	list.GetComponents().Visit([&](const deoglCollideListComponent &clc){
		deoglGIInstance * const instance = GetInstanceWithComponent(clc.GetComponent());
		if(!instance){
			return;
		}
		
		#ifdef DO_LOG_ADD_REMOVE
			{
			const decDVector p(instance->GetComponent()->GetMatrix().GetPosition());
			pGIState.GetRenderThread().GetLogger().LogInfoFormat("GIInstances.RemoveComponents: %d (%g,%g,%g) component=%s",
				i, p.x, p.y, p.z, instance->GetComponent()->GetModel()
					? instance->GetComponent()->GetModel()->GetFilename().GetString() : "-");
			}
		#endif
		
		// GI field moved and component is no longer inside the GI field.
		// no invalidating is required
		
		instance->Clear();
		pEmptyInstances.Add(instance);
	});
	
#if 0
	if(list.GetComponentCount() == 0){
		return;
	}
	
	const int count = pInstances.GetCount();
	int i;
	
	MarkComponents(true);
	list.MarkComponents(false);
	
	for(i=0; i<count; i++){
		deoglGIInstance &instance = pInstances.GetAt(i);
		
		if(instance.GetComponent()){
			if(instance.GetComponent()->GetMarked()){
				continue;
			}
			
			#ifdef DO_LOG_ADD_REMOVE
				{
				const decDVector p(instance.GetComponent()->GetMatrix().GetPosition());
				pGIState.GetRenderThread().GetLogger().LogInfoFormat("GIInstances.RemoveComponents: %d (%g,%g,%g) component=%s",
					i, p.x, p.y, p.z, instance.GetComponent()->GetModel()
						? instance.GetComponent()->GetModel()->GetFilename().GetString() : "-");
				}
			#endif
			
			// GI field moved and component is no longer inside the GI field.
			// no invalidating is required
			
		}else{
			continue;
		}
		
		instance.Clear();
		pEmptyInstances.Add(&instance);
	}
#endif
}

void deoglGIInstances::RemoveDecal(deoglRDecal *decal){
	deoglGIInstance * const instance = GetInstanceWithDecal(decal);
	if(instance){
		RemoveInstance(*instance);
	}
}

void deoglGIInstances::RemoveDecals(const deoglRComponent &component){
	component.GetDecals().Visit([&](deoglRDecal *decal){
		RemoveDecal(decal);
	});
}

void deoglGIInstances::MarkComponents(bool marked){
	pInstances.Visit([&](deoglGIInstance &instance){
		if(instance.GetComponent()){
			instance.GetComponent()->SetMarked(marked);
		}
	});
}



void deoglGIInstances::RemoveInstance(deoglGIInstance &instance){
	if(instance.GetDynamic()){
		pGIState.TouchDynamicArea(instance.GetMinimumExtend(), instance.GetMaximumExtend());
		
	}else{
		#ifdef DO_LOG_ADD_REMOVE
			pGIState.GetRenderThread().GetLogger().LogInfoFormat("GIInstances.RemoveInstance: %p", &instance);
		#endif
		pGIState.InvalidateArea(instance.GetMinimumExtend(), instance.GetMaximumExtend(), true);
	}
	
	instance.Clear();
	pEmptyInstances.Add(&instance);
}

void deoglGIInstances::InstanceChanged(deoglGIInstance &instance){
	pChangedInstances.Add(&instance);
}



void deoglGIInstances::DebugPrint(){
	deoglRTLogger &logger = pGIState.GetRenderThread().GetLogger();
	pInstances.VisitIndexed([&](int i, const deoglGIInstance &instance){
		if(instance.GetComponent()){
			const decDVector p(instance.GetComponent()->GetMatrix().GetPosition());
				logger.LogInfoFormat("%d: component (%g,%g,%g) %s",
					i, p.x, p.y, p.z, instance.GetComponent()->GetModel()
						? instance.GetComponent()->GetModel()->GetFilename().GetString() : "-");
				
		}else if(instance.GetDecal()){
			if(instance.GetDecal()->GetParentComponent()){
				const deoglRComponent &c = *instance.GetDecal()->GetParentComponent();
				const decDVector p(c.GetMatrix().GetPosition());
					logger.LogInfoFormat("%d: decal (%g,%g,%g)|%d %s", i, p.x, p.y, p.z,
						c.GetDecals().IndexOf(instance.GetDecal()),
						c.GetModel() ? c.GetModel()->GetFilename().GetString() : "-");
			}else{
				logger.LogInfoFormat("%d: decal (no parent)", i);
			}
		}
	});
}



// Private Functions
//////////////////////

void deoglGIInstances::pInvalidateAddInstance(const deoglGIInstance &instance){
	if(instance.GetDynamic()){
		pGIState.TouchDynamicArea(instance.GetMinimumExtend(), instance.GetMaximumExtend());
		
	}else{
		pGIState.InvalidateArea(instance.GetMinimumExtend(), instance.GetMaximumExtend(), true);
			// WARNING InvalidateArea becomes expensive if called multiple times.
			//         unfortunately we can not collect all boxes into an enclosing box
			//         since then moving diagonally can invalidate lots of probes inside
			//         the area which should not be touched. not sure how to solve this.
			//         maybe a small octree for probe extends?
	}
}
