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

#include <dragengine/common/collection/decIntList.h>
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
	pStates = NULL;
	pStateCount = 0;
	pStateSize = 0;
}

dearBoneStateList::~dearBoneStateList(){
	if(pStates){
		while(pStateCount > 0){
			delete pStates[pStateCount - 1];
			pStateCount--;
		}
		
		delete [] pStates;
	}
}



// Management
///////////////

void dearBoneStateList::SetStateCount(int count){
	if(count > pStateSize){
		dearBoneState ** const newArray = new dearBoneState*[count];
		
		if(pStates){
			int i;
			
			for(i=0; i<pStateSize; i++){
				newArray[i] = pStates[i];
			}
			
			delete [] pStates;
		}
		
		pStates = newArray;
		
		while(pStateSize < count){
			pStates[pStateSize] = new dearBoneState;
			pStateSize++;
		}
	}
	
	pStateCount = count;
}

dearBoneState *dearBoneStateList::GetStateAt(int index) const{
	if(index < 0 || index >= pStateCount){
		DETHROW(deeInvalidParam);
	}
	
	return pStates[index];
}

int dearBoneStateList::IndexOfStateNamed(const char *name) const{
	if(! name){
		DETHROW(deeInvalidParam);
	}
	
	const char *rigBoneName;
	int s;
	
	for(s=0; s<pStateCount; s++){
		rigBoneName = pStates[s]->GetRigBoneName();
		
		if(rigBoneName && strcmp(rigBoneName, name) == 0){
			return s;
		}
	}
	
	return -1;
}



dearBoneStateList *dearBoneStateList::CreateCopy() const{
	dearBoneStateList * const stalist = new dearBoneStateList;
	int s, c, childCount;
	
	try{
		stalist->SetStateCount(pStateCount);
		
		for(s=0; s<pStateCount; s++){
			const dearBoneState &stateFrom = *pStates[s];
			dearBoneState &stateTo = *stalist->GetStateAt(s);
			
			stateTo.SetRigBone(stateFrom.GetRigBone());
			stateTo.SetRigBoneName(stateFrom.GetRigBoneName());
			stateTo.SetIndex(stateFrom.GetIndex());
			if(stateFrom.GetParentState()){
				stateTo.SetParentState(stalist->GetStateAt(stateFrom.GetParentState()->GetIndex()));
			}
			stateTo.SetRigLocalMatrix(stateFrom.GetRigLocalMatrix());
			stateTo.SetDirty(true);
			
			childCount = stateFrom.GetChildStateCount();
			for(c=0; c<childCount; c++){
				stateTo.AddChildState(stalist->GetStateAt(stateFrom.GetChildStateAt(c)->GetIndex()));
			}
		}
		
	}catch(const deException &){
		delete stalist;
		throw;
	}
	
	return stalist;
}

void dearBoneStateList::SetFrom(const dearBoneStateList &stateList){
	int s, c, childCount;
	
	SetStateCount(stateList.pStateCount);
	
	for(s=0; s<stateList.pStateCount; s++){
		const dearBoneState &stateFrom = *stateList.GetStateAt(s);
		dearBoneState &stateTo = *pStates[s];
		
		stateTo.SetRigBone(stateFrom.GetRigBone());
		stateTo.SetRigBoneName(stateFrom.GetRigBoneName());
		stateTo.SetIndex(stateFrom.GetIndex());
		if(stateFrom.GetParentState()){
			stateTo.SetParentState(pStates[stateFrom.GetParentState()->GetIndex()]);
		}
		stateTo.SetRigLocalMatrix(stateFrom.GetRigLocalMatrix());
		stateTo.SetDirty(true);
		
		childCount = stateFrom.GetChildStateCount();
		for(c=0; c<childCount; c++){
			stateTo.AddChildState(pStates[stateFrom.GetChildStateAt(c)->GetIndex()]);
		}
	}
}



void dearBoneStateList::UpdateStates(){
	int s;
	
	for(s=0; s<pStateCount; s++){
		pStates[s]->UpdateMatrices();
	}
}

void dearBoneStateList::MarkDirty(){
	int s;
	
	for(s=0; s<pStateCount; s++){
		pStates[s]->SetDirty(true);
	}
}



void dearBoneStateList::UpdateMappings(const deAnimator &animator){
	deRig * const rig = animator.GetRig();
	if(! rig){
		SetStateCount(0);
		return;
	}
	
	const decStringSet &bones = animator.GetListBones();
	int boneCount = bones.GetCount();
	int boneIndex;
	int s, parent;
	
	for(s=0; s<pStateCount; s++){
		pStates[s]->RemoveAllChildStates();
	}
	
	if(boneCount == 0){
		boneCount = rig->GetBoneCount();
		
		if(boneCount != pStateCount){
			SetStateCount(boneCount);
		}
		
		for(s=0; s<pStateCount; s++){
			deRigBone &rigBone = rig->GetBoneAt(s);
			parent = rigBone.GetParent();
			
			pStates[s]->SetIndex(s);
			pStates[s]->SetRigIndex(s);
			pStates[s]->SetRigBone(&rigBone);
			pStates[s]->SetRigBoneName(rigBone.GetName());
			pStates[s]->SetRigLocalMatrix(decMatrix::CreateRT(
				rigBone.GetRotation(), rigBone.GetPosition()));
			
			if(parent == -1){
				pStates[s]->SetParentState(NULL);
				
			}else{
				pStates[s]->SetParentState(pStates[parent]);
				pStates[parent]->AddChildState(pStates[s]);
			}
			pStates[s]->SetProtected(false);
		}
		
	}else{
		decIntList foundBones;
		
		for(s=0; s<boneCount; s++){
			boneIndex = rig->IndexOfBoneNamed(bones.GetAt(s));
			if(boneIndex != -1){
				foundBones.Add(boneIndex);
			}
		}
		boneCount = foundBones.GetCount();
		
		if(boneCount != pStateCount){
			SetStateCount(boneCount);
		}
		
		for(s=0; s<pStateCount; s++){
			boneIndex = foundBones.GetAt(s);
			deRigBone &rigBone = rig->GetBoneAt(boneIndex);
			parent = rigBone.GetParent();
			
			pStates[s]->SetIndex(s);
			pStates[s]->SetRigIndex(boneIndex);
			pStates[s]->SetRigBone(&rigBone);
			pStates[s]->SetRigBoneName(rigBone.GetName());
			pStates[s]->SetRigLocalMatrix(decMatrix::CreateRT(
				rigBone.GetRotation(), rigBone.GetPosition()));
			
			if(parent != -1){
				parent = foundBones.IndexOf(parent);
			}
			
			if(parent == -1){
				pStates[s]->SetParentState(NULL);
				
			}else{
				pStates[s]->SetParentState(pStates[parent]);
				pStates[parent]->AddChildState(pStates[s]);
			}
			pStates[s]->SetProtected(false);
		}
	}
}



void dearBoneStateList::ApplyToComponent(deComponent *component) const{
	if(! component){
		DETHROW(deeInvalidParam);
	}
	
	int s;
	
	for(s=0; s<pStateCount; s++){
		if(pStates[s]->GetRigIndex() == -1){
			continue;
		}
		if(pStates[s]->GetProtected()){
			continue;
		}
		
		//pStates[ s ]->UpdateMatrices(); // only required if matrices are set
		
		const decVector &nposition = pStates[s]->GetPosition();
		const decQuaternion &nrotation = pStates[s]->GetOrientation();
		decVector nscale = pStates[s]->GetScale();
		
		deComponentBone &componentBone = component->GetBoneAt(pStates[s]->GetRigIndex());
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
	}
	
	component->InvalidateBones();
}

void dearBoneStateList::ApplyToComponent(deComponent *component, deAnimatorRule::eBlendModes blendMode, float blendFactor) const{
	if(! component){
		DETHROW(deeInvalidParam);
	}
	
	decVector scale;
	int s;
	
	for(s=0; s<pStateCount; s++){
		if(pStates[s]->GetRigIndex() == -1){
			continue;
		}
		if(pStates[s]->GetProtected()){
			continue;
		}
		
		//pStates[ s ]->UpdateMatrices(); // only required if matrices are set
		
		const decVector &nposition = pStates[s]->GetPosition();
		const decQuaternion &nrotation = pStates[s]->GetOrientation();
		const decVector &nscale = pStates[s]->GetScale();
		
		deComponentBone &componentBone = component->GetBoneAt(pStates[s]->GetRigIndex());
		
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
		componentBone.SetMatrix(pStates[i]->GetGlobalMatrix());
		componentBone.SetInverseMatrix(pStates[i]->GetInverseGlobalMatrix());
		componentBone.SetWeightMatrix(pStates[i]->GetRigWeightMatrix().QuickMultiply(pStates[i]->GetGlobalMatrix()));
		componentBone.SetUpdated(true);
		*/
	}
	
	component->InvalidateBones();
}

void dearBoneStateList::ApplyToComponent(dearComponent &component) const{
	dearComponentBoneState *boneStates = component.GetBoneStates();
	const int boneStateCount = component.GetBoneStateCount();
	int i;
	
	/*
	this check is wrong. if a bone list is used on the animator the number of states
	are different although they belong to the same bone. check boneIndex against
	boneStateCount instead to make sure the index is valid.
	if(boneStateCount != pStateCount){
		return; // not matching
	}
	*/
	
	for(i=0; i<pStateCount; i++){
		const int boneIndex = pStates[i]->GetRigIndex();
		if(boneIndex < 0 || boneIndex >= boneStateCount){
			continue;
		}
		if(pStates[i]->GetProtected()){
			continue;
		}
		
		dearComponentBoneState &boneState = boneStates[boneIndex];
		
		boneState.SetPosition(pStates[i]->GetPosition());
		boneState.SetRotation(pStates[i]->GetOrientation());
		
		// apply a little threshold around unit scaling to obtain pure unit scaling in the case
		// of small numerical instability. this should prevent problems with lazy modules
		decVector nscale = pStates[i]->GetScale();
		
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
	}
}

void dearBoneStateList::ApplyToComponent(dearComponent &component,
deAnimatorRule::eBlendModes blendMode, float blendFactor) const{
	dearComponentBoneState *boneStates = component.GetBoneStates();
	const int boneStateCount = component.GetBoneStateCount();
	int i;
	
	/*
	this check is wrong. if a bone list is used on the animator the number of states
	are different although they belong to the same bone. check boneIndex against
	boneStateCount instead to make sure the index is valid.
	if(boneStateCount != pStateCount){
		return; // not matching
	}
	*/
	
	for(i=0; i<pStateCount; i++){
		const int boneIndex = pStates[i]->GetRigIndex();
		if(boneIndex < 0 || boneIndex >= boneStateCount){
			continue;
		}
		if(pStates[i]->GetProtected()){
			continue;
		}
		
		dearComponentBoneState &boneState = boneStates[boneIndex];
		
		const decVector &nposition = pStates[i]->GetPosition();
		const decQuaternion &nrotation = pStates[i]->GetOrientation();
		const decVector &nscale = pStates[i]->GetScale();
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
	}
}
