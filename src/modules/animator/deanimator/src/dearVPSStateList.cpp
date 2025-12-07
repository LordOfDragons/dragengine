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
#include <dragengine/common/collection/decIntList.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelVertexPositionSet.h>



// Class dearVPSStateList
///////////////////////////

// Constructor, destructor
////////////////////////////

dearVPSStateList::dearVPSStateList() :
pStates(nullptr),
pStateCount(0),
pStateSize(0){
}

dearVPSStateList::~dearVPSStateList(){
	if(pStates){
		delete [] pStates;
	}
}



// Management
///////////////

void dearVPSStateList::SetStateCount(int count){
	if(count > pStateSize){
		dearVPSState * const newArray = new dearVPSState[count];
		
		if(pStates){
			int i;
			for(i=0; i<pStateSize; i++){
				newArray[i] = pStates[i];
			}
			delete [] pStates;
		}
		
		pStates = newArray;
		pStateSize = count;
	}
	
	pStateCount = count;
}

dearVPSState &dearVPSStateList::GetStateAt(int index) const{
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pStateCount)
	
	return pStates[index];
}

int dearVPSStateList::IndexOfStateNamed(const char *name) const{
	DEASSERT_NOTNULL(name)
	
	int i;
	for(i=0; i<pStateCount; i++){
		const char * const stateName = pStates[i].GetName();
		if(stateName && strcmp(stateName, name) == 0){
			return i;
		}
	}
	
	return -1;
}



dearVPSStateList *dearVPSStateList::CreateCopy() const{
	dearVPSStateList * const stalist = new dearVPSStateList;
	
	try{
		stalist->SetStateCount(pStateCount);
		
		int i;
		for(i=0; i<pStateCount; i++){
			dearVPSState &state = stalist->GetStateAt(i);
			state = pStates[i];
			state.SetDirty(true);
		}
		
	}catch(const deException &){
		delete stalist;
		throw;
	}
	
	return stalist;
}

void dearVPSStateList::SetFrom(const dearVPSStateList &stateList){
	SetStateCount(stateList.pStateCount);
	
	int i;
	for(i=0; i<stateList.pStateCount; i++){
		pStates[i] = stateList.GetStateAt(i);
		pStates[i].SetDirty(true);
	}
}



void dearVPSStateList::MarkDirty(){
	int i;
	for(i=0; i<pStateCount; i++){
		pStates[i].SetDirty(true);
	}
}



void dearVPSStateList::UpdateMappings(const deAnimator &animator, const deComponent *component){
	deModel * const model = component ? component->GetModel() : nullptr;
	if(!model){
		SetStateCount(0);
		return;
	}
	
	const decStringSet &vpslist = animator.GetListVertexPositionSets();
	int i, vpsCount = vpslist.GetCount();
	
	if(vpsCount == 0){
		vpsCount = model->GetVertexPositionSetCount();
		
		if(vpsCount != pStateCount){
			SetStateCount(vpsCount);
		}
		
		for(i=0; i<pStateCount; i++){
			const deModelVertexPositionSet &modelVPS = *model->GetVertexPositionSetAt(i);
			
			pStates[i].SetIndex(i);
			pStates[i].SetModelIndex(i);
			pStates[i].SetName(modelVPS.GetName());
			pStates[i].SetProtected(false);
		}
		
	}else{
		decIntList foundVPS;
		
		for(i=0; i<vpsCount; i++){
			const int modelIndex = model->IndexOfVertexPositionSetNamed(vpslist.GetAt(i));
			if(modelIndex != -1){
				foundVPS.Add(modelIndex);
			}
		}
		vpsCount = foundVPS.GetCount();
		
		if(vpsCount != pStateCount){
			SetStateCount(vpsCount);
		}
		
		for(i=0; i<pStateCount; i++){
			const int modelIndex = foundVPS.GetAt(i);
			const deModelVertexPositionSet &modelVPS = *model->GetVertexPositionSetAt(modelIndex);
			
			pStates[i].SetIndex(i);
			pStates[i].SetModelIndex(modelIndex);
			pStates[i].SetName(modelVPS.GetName());
			pStates[i].SetProtected(false);
		}
	}
}



void dearVPSStateList::ApplyToComponent(deComponent &component) const{
	int i;
	for(i=0; i<pStateCount; i++){
		if(pStates[i].GetModelIndex() == -1 || pStates[i].GetProtected()){
			continue;
		}
		component.SetVertexPositionSetWeightAt(pStates[i].GetModelIndex(), pStates[i].GetWeight());
	}
	component.InvalidateMesh();
}

void dearVPSStateList::ApplyToComponent(deComponent &component,
deAnimatorRule::eBlendModes blendMode, float blendFactor) const{
	int i;
	for(i=0; i<pStateCount; i++){
		if(pStates[i].GetModelIndex() == -1 || pStates[i].GetProtected()){
			continue;
		}
		
		const int index = pStates[i].GetModelIndex();
		const float oldWeight = component.GetVertexPositionSetWeightAt(index);
		const float newWeight = pStates[i].GetWeight();
		
		switch(blendMode){
		case deAnimatorRule::ebmBlend:
			component.SetVertexPositionSetWeightAt(index,
				oldWeight * (1.0f - blendFactor) + newWeight * blendFactor);
			break;
			
		case deAnimatorRule::ebmOverlay:
			component.SetVertexPositionSetWeightAt(index, oldWeight + newWeight * blendFactor);
			break;
		}
	}
	component.InvalidateMesh();
}

void dearVPSStateList::ApplyToComponent(dearComponent &component) const{
	dearComponentVPSState * const vpsStates = component.GetVPSStates();
	const int vpsStateCount = component.GetVPSStateCount();
	int i;
	
	/*
	this check is wrong. if a bone list is used on the animator the number of states
	are different although they belong to the same bone. check modelIndex against
	boneStateCount instead to make sure the index is valid.
	if(boneStateCount != pStateCount){
		return; // not matching
	}
	*/
	
	for(i=0; i<pStateCount; i++){
		const int modelIndex = pStates[i].GetModelIndex();
		if(modelIndex < 0 || modelIndex >= vpsStateCount || pStates[i].GetProtected()){
			continue;
		}
		
		vpsStates[modelIndex].SetWeight(pStates[i].GetWeight());
	}
}

void dearVPSStateList::ApplyToComponent(dearComponent &component,
deAnimatorRule::eBlendModes blendMode, float blendFactor) const{
	dearComponentVPSState * const vpsStates = component.GetVPSStates();
	const int vpsStateCount = component.GetVPSStateCount();
	int i;
	
	/*
	this check is wrong. if a bone list is used on the animator the number of states
	are different although they belong to the same bone. check modelIndex against
	boneStateCount instead to make sure the index is valid.
	if(boneStateCount != pStateCount){
		return; // not matching
	}
	*/
	
	for(i=0; i<pStateCount; i++){
		const int modelIndex = pStates[i].GetModelIndex();
		if(modelIndex < 0 || modelIndex >= vpsStateCount || pStates[i].GetProtected()){
			continue;
		}
		
		dearComponentVPSState &vpsState = vpsStates[modelIndex];
		
		const float newWeight = pStates[i].GetWeight();
		
		switch(blendMode){
		case deAnimatorRule::ebmBlend:
			vpsState.SetWeight(vpsState.GetWeight() * (1.0f - blendFactor) + newWeight * blendFactor);
			break;
			
		case deAnimatorRule::ebmOverlay:
			vpsState.SetWeight(vpsState.GetWeight() + newWeight * blendFactor);
			break;
		}
	}
}
