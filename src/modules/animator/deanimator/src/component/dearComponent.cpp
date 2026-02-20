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

#include "dearComponent.h"
#include "dearComponentBoneState.h"
#include "dearComponentVPSState.h"
#include "../deDEAnimator.h"
#include "../dearBoneState.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentBone.h>



// Class dearComponent
////////////////////////

// Constructors and Destructors
/////////////////////////////////

dearComponent::dearComponent(deDEAnimator &module, deComponent &component) :
pModule(module),
pComponent(component)
{
	ModelChanged();
	RigChanged();
}

dearComponent::~dearComponent(){
}



// Management
///////////////

void dearComponent::PrepareBones(){
	pBoneStates.Visit([](dearComponentBoneState &state){
		dearComponentBoneState * const parent = state.GetParent();
		
		if(parent){
			state.SetMatrix(decMatrix::CreateWorld(state.GetPosition(), state.GetRotation(), state.GetScale())
				.QuickMultiply(state.GetOriginalMatrix())
				.QuickMultiply(parent->GetMatrix()));
			
		}else{
			state.SetMatrix(decMatrix::CreateWorld(state.GetPosition(), state.GetRotation(), state.GetScale())
				.QuickMultiply(state.GetOriginalMatrix()));
		}
	});
}

void dearComponent::UpdateFromComponent(){
	pBoneStates.VisitIndexed([&](int i, dearComponentBoneState &state){
		const deComponentBone &bone = pComponent.GetBoneAt(i);
		state.SetPosition(bone.GetPosition());
		state.SetRotation(bone.GetRotation());
		state.SetScale(bone.GetScale());
	});
	
	pVPSStates.VisitIndexed([&](int i, dearComponentVPSState &state){
		state.SetWeight(pComponent.GetVertexPositionSetWeights()[i]);
	});
	
	pMatrix = pComponent.GetMatrix();
}

void dearComponent::UpdateMatrixFromComponent(){
	pMatrix = pComponent.GetMatrix();
}

void dearComponent::UpdateComponent(){
	pBoneStates.VisitIndexed([&](int i, const dearComponentBoneState &state){
		deComponentBone &bone = pComponent.GetBoneAt(i);
		bone.SetPosition(state.GetPosition());
		bone.SetRotation(state.GetRotation());
		bone.SetScale(state.GetScale());
	});
	
	pVPSStates.VisitIndexed([&](int i, const dearComponentVPSState &state){
		pComponent.SetVertexPositionSetWeightAt(i, state.GetWeight());
	});
}

void dearComponent::UpdateComponentPrepareBones(){
	pBoneStates.VisitIndexed([&](int i, const dearComponentBoneState &state){
		pComponent.GetBoneAt(i).SetMatrix(state.GetMatrix());
	});
	
	pComponent.ValidateBones();
}



void dearComponent::ModelChanged(){
	const int vpsCount = pComponent.GetVertexPositionSetWeights().GetCount();
	if(vpsCount == pVPSStates.GetCount()){
		return;
	}
	
	pVPSStates.SetAll(vpsCount, {});
}

void dearComponent::RigChanged(){
	const int boneCount = pComponent.GetBones().GetCount();
	if(boneCount == pBoneStates.GetCount()){
		return;
	}
	
	pBoneStates.SetAll(boneCount, {});
	
	int i;
	for(i=0; i<boneCount; i++){
		const deComponentBone &bone = pComponent.GetBoneAt(i);
		if(bone.GetParentBone() != -1){
			pBoneStates[i].SetParent(&pBoneStates[bone.GetParentBone()]);
		}
		pBoneStates[i].SetOriginalMatrix(bone.GetOriginalMatrix());
	}
}
