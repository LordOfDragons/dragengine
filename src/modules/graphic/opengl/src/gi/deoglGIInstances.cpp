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
pGIState(giState),
pDynamicBoxes(NULL),
pDynamicBoxCount(0),
pDynamicBoxSize(0){
}

deoglGIInstances::~deoglGIInstances(){
	pCleanUp();
}



// Management
///////////////

bool deoglGIInstances::IsComponentStatic(const deoglRComponent &component){
	// weight check catches more cases than GetRenderStatic() or GetRenderMode() can.
	// static textures is checked to avoid cached rays with changing textures
	return component.GetRenderStatic()
		&& component.GetStaticTextures()
		&& component.GetMovementHint() == deComponent::emhStationary
		&& component.GetLODAt(-1).GetModelLODRef().GetWeightsCount() == 0;
}

bool deoglGIInstances::IsDecalStatic(const deoglRDecal &decal){
	if(decal.GetParentComponent()){
		return decal.GetStaticTexture() && IsComponentStatic(*decal.GetParentComponent());
		
	}else{
		return true;
	}
}

deoglGIInstance &deoglGIInstances::GetInstanceAt(int slot) const{
	return *((deoglGIInstance*)pInstances.GetAt(slot));
}

deoglGIInstance *deoglGIInstances::GetInstanceWithComponent(deoglRComponent *component) const{
#if 0
	if(!component){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pInstances.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deoglGIInstance * const instance = (deoglGIInstance*)pInstances.GetAt(i);
		if(instance->GetComponent() == component){
			return instance;
		}
	}
	
	return NULL;
#endif
	
	void *instance;
	if(pElementInstanceMap.GetAt(component, component->GetUniqueKey(), &instance)){
		return (deoglGIInstance*)instance;
		
	}else{
		return NULL;
	}
}

deoglGIInstance *deoglGIInstances::GetInstanceWithDecal(deoglRDecal *decal) const{
	void *instance;
	if(pElementInstanceMap.GetAt(decal, decal->GetUniqueKey(), &instance)){
		return (deoglGIInstance*)instance;
		
	}else{
		return NULL;
	}
}

deoglGIInstance &deoglGIInstances::AddInstance(){
	const deoglGIInstance::Ref instance(deoglGIInstance::Ref::NewWith(*this));
	pInstances.Add(instance);
	return instance;
}

deoglGIInstance &deoglGIInstances::NextFreeSlot(){
	if(pEmptyInstances.GetCount() == 0){
		return AddInstance();
	}
	
	const int index = pEmptyInstances.GetCount() - 1;
	deoglGIInstance * const instance = (deoglGIInstance*)pEmptyInstances.GetAt(index);
	pEmptyInstances.RemoveFrom(index);
	return *instance;
}

void deoglGIInstances::RegisterElement(deoglRComponent *component, deoglGIInstance *instance){
	RegisterElement(component, component->GetUniqueKey(), instance);
}

void deoglGIInstances::RegisterElement(deoglRDecal *decal, deoglGIInstance *instance){
	RegisterElement(decal, decal->GetUniqueKey(), instance);
}

void deoglGIInstances::RegisterElement(void *element, unsigned int hash, deoglGIInstance *instance){
	pElementInstanceMap.SetAt(element, hash, instance);
}

void deoglGIInstances::UnregisterElement(deoglRComponent *component){
	UnregisterElement(component, component->GetUniqueKey());
}

void deoglGIInstances::UnregisterElement(deoglRDecal *decal){
	UnregisterElement(decal, decal->GetUniqueKey());
}

void deoglGIInstances::UnregisterElement(void *element, unsigned int hash){
	pElementInstanceMap.Remove(element, hash);
}



void deoglGIInstances::UpdateDynamicBoxes(const decDVector &offset, const decVector &enlarge){
	pDynamicBoxCount = 0;
	
	const decVector halfEnlarge(enlarge * 0.5f);
	const int count = pInstances.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const deoglGIInstance &instance = *((deoglGIInstance*)pInstances.GetAt(i));
		if(!instance.GetDynamic()){
			continue;
		}
		
		if(pDynamicBoxCount == pDynamicBoxSize){
			const int newSize = pDynamicBoxCount * 3 / 2 + 1;
			         sBox * const newArray = new sBox[newSize];
			if(pDynamicBoxes){
				memcpy(newArray, pDynamicBoxes, sizeof(sBox) * pDynamicBoxCount);
				delete [] pDynamicBoxes;
			}
			pDynamicBoxes = newArray;
			pDynamicBoxSize = newSize;
		}
		
		      sBox &box = pDynamicBoxes[pDynamicBoxCount++];
		box.minExtend = (instance.GetMinimumExtend() + offset).ToVector() - halfEnlarge;
		box.maxExtend = (instance.GetMaximumExtend() + offset).ToVector() + halfEnlarge;
	}
}

bool deoglGIInstances::DynamicBoxesContain(const decVector &point) const{
	int i;
	for(i=0; i<pDynamicBoxCount; i++){
		if(point >= pDynamicBoxes[i].minExtend && point <= pDynamicBoxes[i].maxExtend){
			return true;
		}
	}
	return false;
}

int deoglGIInstances::CountDynamicBoxesContaining(const decVector &point) const{
	int i, count = 0;
	for(i=0; i<pDynamicBoxCount; i++){
		if(point >= pDynamicBoxes[i].minExtend && point <= pDynamicBoxes[i].maxExtend){
			count++;
		}
	}
	return count;
}



void deoglGIInstances::Clear(){
	pEmptyInstances.RemoveAll();
	
	const int count = pInstances.GetCount();
	int i;
	for(i=0; i<count; i++){
		deoglGIInstance * const instance = (deoglGIInstance*)pInstances.GetAt(i);
		instance->Clear();
		pEmptyInstances.Add(instance);
	}
	
	pChangedInstances.RemoveAll();
	pElementInstanceMap.RemoveAll();
}

void deoglGIInstances::ApplyChanges(){
	const int count = pChangedInstances.GetCount();
	if(count == 0){
		return;
	}
	
	int i;
	for(i=0; i<count; i++){
		deoglGIInstance &instance = *((deoglGIInstance*)pChangedInstances.GetAt(i));
		
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
	}
	
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
			if(((deoglGIInstance*)pInstances.GetAt(j))->GetComponent() == component){
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
	const int count = list.GetComponentCount();
	int i;
	for(i=0; i<count; i++){
		AddComponent(list.GetComponentAt(i)->GetComponent(), invalidate);
	}
}

void deoglGIInstances::AddDecal(deoglRDecal *decal, bool invalidate){
	deoglGIInstance &instance = NextFreeSlot();
	instance.SetDecal(decal, !IsDecalStatic(*decal));
	if(invalidate){
		pInvalidateAddInstance(instance);
	}
}

void deoglGIInstances::AddDecals(const deoglRComponent &component, bool invalidate){
	const int count = component.GetDecalCount();
	int i;
	for(i=0; i<count; i++){
		AddDecal(component.GetDecalAt(i), invalidate);
	}
}

void deoglGIInstances::RemoveComponent(deoglRComponent *component){
	deoglGIInstance * const instance = GetInstanceWithComponent(component);
	if(instance){
		RemoveInstance(*instance);
	}
}

void deoglGIInstances::RemoveComponents(const deoglCollideList &list){
	const int count = list.GetComponentCount();
	int i;
	for(i=0; i<count; i++){
		deoglGIInstance * const instance = GetInstanceWithComponent(list.GetComponentAt(i)->GetComponent());
		if(!instance){
			continue;
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
	}
	
#if 0
	if(list.GetComponentCount() == 0){
		return;
	}
	
	const int count = pInstances.GetCount();
	int i;
	
	MarkComponents(true);
	list.MarkComponents(false);
	
	for(i=0; i<count; i++){
		deoglGIInstance &instance = *((deoglGIInstance*)pInstances.GetAt(i));
		
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
	const int count = component.GetDecalCount();
	int i;
	for(i=0; i<count; i++){
		RemoveDecal(component.GetDecalAt(i));
	}
}

void deoglGIInstances::MarkComponents(bool marked){
	const int count = pInstances.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deoglGIInstance &instance = *((deoglGIInstance*)pInstances.GetAt(i));
		if(instance.GetComponent()){
			instance.GetComponent()->SetMarked(marked);
		}
	}
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
	const int count = pInstances.GetCount();
	int i;
	for(i=0; i<count; i++){
		const deoglGIInstance &instance = *((deoglGIInstance*)pInstances.GetAt(i));
		if(instance.GetComponent()){
			const decDVector p(instance.GetComponent()->GetMatrix().GetPosition());
				logger.LogInfoFormat("%d: component (%g,%g,%g) %s",
					i, p.x, p.y, p.z, instance.GetComponent()->GetModel()
						? instance.GetComponent()->GetModel()->GetFilename().GetString() : "-");
				
		}else if(instance.GetDecal()){
			if(instance.GetDecal()->GetParentComponent()){
				const deoglRComponent &c = *instance.GetDecal()->GetParentComponent();
				int index;
				for(index=0; index<c.GetDecalCount(); index++){
					if(c.GetDecalAt(index) == instance.GetDecal()){
						break;
					}
				}
				if(index == c.GetDecalCount()){
					index = -1;
				}
				const decDVector p(c.GetMatrix().GetPosition());
					logger.LogInfoFormat("%d: decal (%g,%g,%g)|%d %s", i, p.x, p.y, p.z, index,
						c.GetModel() ? c.GetModel()->GetFilename().GetString() : "-");
			}else{
				logger.LogInfoFormat("%d: decal (no parent)", i);
			}
		}
	}
}



// Private Functions
//////////////////////

void deoglGIInstances::pCleanUp(){
	if(pDynamicBoxes){
		delete [] pDynamicBoxes;
	}
}

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
