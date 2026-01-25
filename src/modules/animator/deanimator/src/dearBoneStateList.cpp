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

#include "dearBoneState.h"
#include "dearBoneStateList.h"
#include "component/dearComponent.h"
#include "component/dearComponentBoneState.h"

#include <dragengine/common/collection/decTList.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>



// Class dearBoneStateList
////////////////////////////

// Constructor, destructor
////////////////////////////

dearBoneStateList::dearBoneStateList(){
}

dearBoneStateList::dearBoneStateList(const dearBoneStateList& other) {
	*this = other;
}

dearBoneStateList& dearBoneStateList::operator=(const dearBoneStateList& other){
	pStates = decTList<dearBoneState>(other.pStates.GetCount(), dearBoneState());
	
	other.pStates.VisitIndexed([&](int i, const dearBoneState &stateFrom){
		dearBoneState &stateTo = pStates.GetAt(i);
		
		stateTo.SetRigBone(stateFrom.GetRigBone());
		stateTo.SetRigBoneName(stateFrom.GetRigBoneName());
		stateTo.SetIndex(stateFrom.GetIndex());
		if(stateFrom.GetParentState()){
			stateTo.SetParentState(&pStates.GetAt(stateFrom.GetParentState()->GetIndex()));
		}
		stateTo.SetRigLocalMatrix(stateFrom.GetRigLocalMatrix());
		stateTo.SetDirty(true);
		
		stateFrom.GetChildStates().Visit([&](dearBoneState *cs){
			stateTo.AddChildState(&pStates.GetAt(cs->GetIndex()));
		});
	});
	
	return *this;
}

dearBoneStateList::~dearBoneStateList(){
}



// Management
///////////////

void dearBoneStateList::SetStateCount(int count){
	if(count == pStates.GetCount()){
		return;
	}
	
	pStates = decTList<dearBoneState>(count, dearBoneState());
}

dearBoneState &dearBoneStateList::GetStateAt(int index){
	return pStates.GetAt(index);
}

const dearBoneState &dearBoneStateList::GetStateAt(int index) const{
	return pStates.GetAt(index);
}

int dearBoneStateList::IndexOfStateNamed(const char *name) const{
	DEASSERT_NOTNULL(name)
	return pStates.IndexOfMatching([&](const dearBoneState &state){
		return state.GetRigBoneName() && strcmp(state.GetRigBoneName(), name) == 0;
	});
}



dearBoneStateList *dearBoneStateList::CreateCopy() const{
	return new dearBoneStateList(*this);
}

void dearBoneStateList::SetFrom(const dearBoneStateList &stateList){
	*this = stateList;
}



void dearBoneStateList::UpdateStates(){
	pStates.Visit([](dearBoneState &state){
		state.UpdateMatrices();
	});
}

void dearBoneStateList::MarkDirty(){
	pStates.Visit([](dearBoneState &state){
		state.SetDirty(true);
	});
}



void dearBoneStateList::UpdateMappings(const deAnimator &animator){
	deRig * const rig = animator.GetRig();
	if(!rig){
		SetStateCount(0);
		return;
	}
	
	const decStringSet &bones = animator.GetListBones();
	int boneCount = bones.GetCount();
	
	pStates.Visit([](dearBoneState &state){
		state.RemoveAllChildStates();
	});
	
	if(boneCount == 0){
		boneCount = rig->GetBoneCount();
		
		if(boneCount != pStates.GetCount()){
			SetStateCount(boneCount);
		}
		
		pStates.VisitIndexed([&](int s, dearBoneState &state){
			deRigBone &rigBone = rig->GetBoneAt(s);
			const int parent = rigBone.GetParent();
			
			state.SetIndex(s);
			state.SetRigIndex(s);
			state.SetRigBone(&rigBone);
			state.SetRigBoneName(rigBone.GetName());
			state.SetRigLocalMatrix(decMatrix::CreateRT(rigBone.GetRotation(), rigBone.GetPosition()));
			
			if(parent == -1){
				state.SetParentState(nullptr);
				
			}else{
				state.SetParentState(&pStates.GetAt(parent));
				pStates.GetAt(parent).AddChildState(&state);
			}
			state.SetProtected(false);
		});
		
	}else{
		decTList<int> foundBones;
		
		bones.Visit([&](const decString &boneName){
			const int boneIndex = rig->IndexOfBoneNamed(boneName);
			if(boneIndex != -1){
				foundBones.Add(boneIndex);
			}
		});
		boneCount = foundBones.GetCount();
		
		if(boneCount != pStates.GetCount()){
			SetStateCount(boneCount);
		}
		
		pStates.VisitIndexed([&](int s, dearBoneState &state){
			const int boneIndex = foundBones.GetAt(s);
			deRigBone &rigBone = rig->GetBoneAt(boneIndex);
			int parent = rigBone.GetParent();
			
			state.SetIndex(s);
			state.SetRigIndex(boneIndex);
			state.SetRigBone(&rigBone);
			state.SetRigBoneName(rigBone.GetName());
			state.SetRigLocalMatrix(decMatrix::CreateRT(
				rigBone.GetRotation(), rigBone.GetPosition()));
			
			if(parent != -1){
				parent = foundBones.IndexOf(parent);
			}
			
			if(parent == -1){
				state.SetParentState(nullptr);
				
			}else{
				state.SetParentState(&pStates.GetAt(parent));
				pStates.GetAt(parent).AddChildState(&state);
			}
			state.SetProtected(false);
		});
	}
}



void dearBoneStateList::ApplyToComponent(deComponent *component) const{
	if(!component){
		DETHROW(deeInvalidParam);
	}
	
	pStates.Visit([&](const dearBoneState &state){
		if(state.GetRigIndex() == -1){
			return;
		}
		if(state.GetProtected()){
			return;
		}
		
		//state.UpdateMatrices(); // only required if matrices are set
		
		const decVector &nposition = state.GetPosition();
		const decQuaternion &nrotation = state.GetOrientation();
		decVector nscale = state.GetScale();
		
		deComponentBone &componentBone = component->GetBoneAt(state.GetRigIndex());
		componentBone.SetPosition(nposition);
		componentBone.SetRotation(nrotation);
		
		// apply a little threshold around unit scaling to obtain pure unit scaling in the case
		// of small numerical instability. this should prevent problems with lazy modules
		if(fabsf(1.0f - nscale.x) < 1e-5f){
			nscale.x = 1.0f;
		}
		if(fabsf(1.0f - nscale.y) < 1e-5f){
			nscale.y = 1.0f;
		}
		if(fabsf(1.0f - nscale.z) < 1e-5f){
			nscale.z = 1.0f;
		}
		
		componentBone.SetScale(nscale);
	});
	
	component->InvalidateBones();
}

void dearBoneStateList::ApplyToComponent(deComponent *component, deAnimatorRule::eBlendModes blendMode, float blendFactor) const{
	if(!component){
		DETHROW(deeInvalidParam);
	}
	
	decVector scale;
	
	pStates.Visit([&](const dearBoneState &state){
		if(state.GetRigIndex() == -1){
			return;
		}
		if(state.GetProtected()){
			return;
		}
		
		//state.UpdateMatrices(); // only required if matrices are set
		
		const decVector &nposition = state.GetPosition();
		const decQuaternion &nrotation = state.GetOrientation();
		const decVector &nscale = state.GetScale();
		
		deComponentBone &componentBone = component->GetBoneAt(state.GetRigIndex());
		
		if(blendMode == deAnimatorRule::ebmBlend){
			componentBone.SetPosition(componentBone.GetPosition() * (1.0f - blendFactor) + nposition * blendFactor);
			componentBone.SetRotation(componentBone.GetRotation().Slerp(nrotation, blendFactor));
			scale = componentBone.GetScale() * (1.0f - blendFactor) + nscale * blendFactor;
			
		}else if(blendMode == deAnimatorRule::ebmOverlay){
			componentBone.SetPosition(componentBone.GetPosition() + nposition * blendFactor);
			componentBone.SetRotation(componentBone.GetRotation() * decQuaternion().Slerp(nrotation, blendFactor));
			scale = componentBone.GetScale() + nscale * blendFactor;
			
		}else{
			DETHROW(deeInvalidParam);
		}
		
		// apply a little threshold around unit scaling to obtain pure unit scaling in the case
		// of small numerical instability. this should prevent problems with lazy modules
		if(fabsf(1.0f - scale.x) < 1e-5f){
			scale.x = 1.0f;
		}
		if(fabsf(1.0f - scale.y) < 1e-5f){
			scale.y = 1.0f;
		}
		if(fabsf(1.0f - scale.z) < 1e-5f){
			scale.z = 1.0f;
		}
		
		componentBone.SetScale(scale);
		
		/*
		componentBone.SetMatrix(state.GetGlobalMatrix());
		componentBone.SetInverseMatrix(state.GetInverseGlobalMatrix());
		componentBone.SetWeightMatrix(state.GetRigWeightMatrix().QuickMultiply(state.GetGlobalMatrix()));
		componentBone.SetUpdated(true);
		*/
	});
	
	component->InvalidateBones();
}

void dearBoneStateList::ApplyToComponent(dearComponent &component) const{
	decTList<dearComponentBoneState> &boneStates = component.GetBoneStates();
	const int boneStateCount = boneStates.GetCount();
	
	/*
	this check is wrong. if a bone list is used on the animator the number of states
	are different although they belong to the same bone. check boneIndex against
	boneStateCount instead to make sure the index is valid.
	if(boneStateCount != pStateCount){
		return; // not matching
	}
	*/
	
	pStates.Visit([&](const dearBoneState &state){
		const int boneIndex = state.GetRigIndex();
		if(boneIndex < 0 || boneIndex >= boneStateCount){
			return;
		}
		if(state.GetProtected()){
			return;
		}
		
		dearComponentBoneState &boneState = boneStates[boneIndex];
		
		boneState.SetPosition(state.GetPosition());
		boneState.SetRotation(state.GetOrientation());
		
		// apply a little threshold around unit scaling to obtain pure unit scaling in the case
		// of small numerical instability. this should prevent problems with lazy modules
		decVector nscale = state.GetScale();
		
		if(fabsf(1.0f - nscale.x) < 1e-5f){
			nscale.x = 1.0f;
		}
		if(fabsf(1.0f - nscale.y) < 1e-5f){
			nscale.y = 1.0f;
		}
		if(fabsf(1.0f - nscale.z) < 1e-5f){
			nscale.z = 1.0f;
		}
		
		boneState.SetScale(nscale);
	});
}

void dearBoneStateList::ApplyToComponent(dearComponent &component,
deAnimatorRule::eBlendModes blendMode, float blendFactor) const{
	decTList<dearComponentBoneState> &boneStates = component.GetBoneStates();
	const int boneStateCount = boneStates.GetCount();
	
	/*
	this check is wrong. if a bone list is used on the animator the number of states
	are different although they belong to the same bone. check boneIndex against
	boneStateCount instead to make sure the index is valid.
	if(boneStateCount != pStateCount){
		return; // not matching
	}
	*/
	
	pStates.Visit([&](const dearBoneState &state){
		const int boneIndex = state.GetRigIndex();
		if(boneIndex < 0 || boneIndex >= boneStateCount){
			return;
		}
		if(state.GetProtected()){
			return;
		}
		
		dearComponentBoneState &boneState = boneStates[boneIndex];
		
		const decVector &nposition = state.GetPosition();
		const decQuaternion &nrotation = state.GetOrientation();
		const decVector &nscale = state.GetScale();
		decVector scale;
		
		if(blendMode == deAnimatorRule::ebmBlend){
			boneState.SetPosition(boneState.GetPosition() * (1.0f - blendFactor) + nposition * blendFactor);
			boneState.SetRotation(boneState.GetRotation().Slerp(nrotation, blendFactor));
			scale = boneState.GetScale() * (1.0f - blendFactor) + nscale * blendFactor;
			
		}else if(blendMode == deAnimatorRule::ebmOverlay){
			boneState.SetPosition(boneState.GetPosition() + nposition * blendFactor);
			boneState.SetRotation(boneState.GetRotation() * decQuaternion().Slerp(nrotation, blendFactor));
			scale = boneState.GetScale() + nscale * blendFactor;
			
		}else{
			DETHROW(deeInvalidParam);
		}
		
		// apply a little threshold around unit scaling to obtain pure unit scaling in the case
		// of small numerical instability. this should prevent problems with lazy modules
		if(fabsf(1.0f - scale.x) < 1e-5f){
			scale.x = 1.0f;
		}
		if(fabsf(1.0f - scale.y) < 1e-5f){
			scale.y = 1.0f;
		}
		if(fabsf(1.0f - scale.z) < 1e-5f){
			scale.z = 1.0f;
		}
		
		boneState.SetScale(scale);
	});
}
