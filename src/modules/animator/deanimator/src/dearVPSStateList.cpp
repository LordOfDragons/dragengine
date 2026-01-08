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

#include <string.h>

#include "dearVPSState.h"
#include "dearVPSStateList.h"
#include "component/dearComponent.h"
#include "component/dearComponentVPSState.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decTList.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelVertexPositionSet.h>



// Class dearVPSStateList
///////////////////////////

// Constructor, destructor
////////////////////////////

dearVPSStateList::dearVPSStateList(){
}

dearVPSStateList::dearVPSStateList(const dearVPSStateList& other){
	*this = other;
}

dearVPSStateList& dearVPSStateList::operator=(const dearVPSStateList& other){
	pStates = decTList<dearVPSState>(other.pStates.GetCount(), dearVPSState());
	
	other.pStates.VisitIndexed([&](int i, const dearVPSState &state){
		dearVPSState &newState = pStates.GetAt(i);
		newState = state;
		newState.SetDirty(true);
	});
	
	return *this;
}

dearVPSStateList::~dearVPSStateList(){
}



// Management
///////////////

void dearVPSStateList::SetStateCount(int count){
	if(count == pStates.GetCount()){
		return;
	}
	
	pStates = decTList<dearVPSState>(count, dearVPSState());
}

dearVPSState &dearVPSStateList::GetStateAt(int index){
	return pStates.GetAt(index);
}

const dearVPSState &dearVPSStateList::GetStateAt(int index) const{
	return pStates.GetAt(index);
}

int dearVPSStateList::IndexOfStateNamed(const char *name) const{
	DEASSERT_NOTNULL(name)
	return pStates.IndexOfMatching([&](const dearVPSState &state){
		return state.GetName() && strcmp(state.GetName(), name) == 0;
	});
}



dearVPSStateList *dearVPSStateList::CreateCopy() const{
	return new dearVPSStateList(*this);
}

void dearVPSStateList::SetFrom(const dearVPSStateList &stateList){
	*this = stateList;
}



void dearVPSStateList::MarkDirty(){
	pStates.Visit([](dearVPSState &state){
		state.SetDirty(true);
	});
}



void dearVPSStateList::UpdateMappings(const deAnimator &animator, const deComponent *component){
	deModel * const model = component ? component->GetModel().Pointer() : nullptr;
	if(!model){
		SetStateCount(0);
		return;
	}
	
	const decStringSet &vpslist = animator.GetListVertexPositionSets();
	int vpsCount = vpslist.GetCount();
	
	if(vpsCount == 0){
		vpsCount = model->GetVertexPositionSetCount();
		
		if(vpsCount != pStates.GetCount()){
			SetStateCount(vpsCount);
		}
		
		pStates.VisitIndexed([&](int i, dearVPSState &state){
			state.SetIndex(i);
			state.SetModelIndex(i);
			state.SetName(model->GetVertexPositionSetAt(i)->GetName());
			state.SetProtected(false);
		});
		
	}else{
		decTList<int> foundVPS;
		
		vpslist.Visit([&](const decString &name){
			const int modelIndex = model->IndexOfVertexPositionSetNamed(name);
			if(modelIndex != -1){
				foundVPS.Add(modelIndex);
			}
		});
		vpsCount = foundVPS.GetCount();
		
		if(vpsCount != pStates.GetCount()){
			SetStateCount(vpsCount);
		}
		
		pStates.VisitIndexed([&](int i, dearVPSState &state){
			const int modelIndex = foundVPS.GetAt(i);
			
			state.SetIndex(i);
			state.SetModelIndex(modelIndex);
			state.SetName(model->GetVertexPositionSetAt(modelIndex)->GetName());
			state.SetProtected(false);
		});
	}
}



void dearVPSStateList::ApplyToComponent(deComponent &component) const{
	pStates.Visit([&](const dearVPSState &state){
		if(state.GetModelIndex() == -1 || state.GetProtected()){
			return;
		}
		component.SetVertexPositionSetWeightAt(state.GetModelIndex(), state.GetWeight());
	});
	component.InvalidateMesh();
}

void dearVPSStateList::ApplyToComponent(deComponent &component,
deAnimatorRule::eBlendModes blendMode, float blendFactor) const{
	pStates.Visit([&](const dearVPSState &state){
		if(state.GetModelIndex() == -1 || state.GetProtected()){
			return;
		}
		
		const int index = state.GetModelIndex();
		const float oldWeight = component.GetVertexPositionSetWeightAt(index);
		const float newWeight = state.GetWeight();
		
		switch(blendMode){
		case deAnimatorRule::ebmBlend:
			component.SetVertexPositionSetWeightAt(index,
				oldWeight * (1.0f - blendFactor) + newWeight * blendFactor);
			break;
			
		case deAnimatorRule::ebmOverlay:
			component.SetVertexPositionSetWeightAt(index, oldWeight + newWeight * blendFactor);
			break;
		}
	});
	component.InvalidateMesh();
}

void dearVPSStateList::ApplyToComponent(dearComponent &component) const{
	dearComponentVPSState * const vpsStates = component.GetVPSStates();
	const int vpsStateCount = component.GetVPSStateCount();
	
	/*
	this check is wrong. if a bone list is used on the animator the number of states
	are different although they belong to the same bone. check modelIndex against
	boneStateCount instead to make sure the index is valid.
	if(boneStateCount != pStateCount){
		return; // not matching
	}
	*/
	
	pStates.Visit([&](const dearVPSState &state){
		const int modelIndex = state.GetModelIndex();
		if(modelIndex < 0 || modelIndex >= vpsStateCount || state.GetProtected()){
			return;
		}
		
		vpsStates[modelIndex].SetWeight(state.GetWeight());
	});
}

void dearVPSStateList::ApplyToComponent(dearComponent &component,
deAnimatorRule::eBlendModes blendMode, float blendFactor) const{
	dearComponentVPSState * const vpsStates = component.GetVPSStates();
	const int vpsStateCount = component.GetVPSStateCount();
	
	/*
	this check is wrong. if a bone list is used on the animator the number of states
	are different although they belong to the same bone. check modelIndex against
	boneStateCount instead to make sure the index is valid.
	if(boneStateCount != pStateCount){
		return; // not matching
	}
	*/
	
	pStates.Visit([&](const dearVPSState &state){
		const int modelIndex = state.GetModelIndex();
		if(modelIndex < 0 || modelIndex >= vpsStateCount || state.GetProtected()){
			return;
		}
		
		dearComponentVPSState &vpsState = vpsStates[modelIndex];
		
		const float newWeight = state.GetWeight();
		
		switch(blendMode){
		case deAnimatorRule::ebmBlend:
			vpsState.SetWeight(vpsState.GetWeight() * (1.0f - blendFactor) + newWeight * blendFactor);
			break;
			
		case deAnimatorRule::ebmOverlay:
			vpsState.SetWeight(vpsState.GetWeight() + newWeight * blendFactor);
			break;
		}
	});
}
