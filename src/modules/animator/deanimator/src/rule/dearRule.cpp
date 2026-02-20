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

#include "dearRule.h"
#include "../deDEAnimator.h"
#include "../dearAnimator.h"
#include "../dearAnimatorInstance.h"
#include "../dearBoneStateList.h"
#include "../dearVPSStateList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorLink.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>



// Class dearRule
///////////////////

// Constructors and Destructors
/////////////////////////////////

dearRule::dearRule(dearAnimatorInstance &instance, const dearAnimator &animator,
	int firstLink, const deAnimatorRule &rule) :
pInstance(instance),
pAnimator(animator),
pRule(rule),

pUseAllBones(false),
pUseAllVPS(false),

pTargetBlendFactor(rule.GetTargetBlendFactor(), firstLink),

pBlendMode(rule.GetBlendMode()),
pBlendFactor(rule.GetBlendFactor()),
pInvertBlendFactor(rule.GetInvertBlendFactor()),
pEnabled(rule.GetEnabled()){
}

dearRule::~dearRule(){
}



// Management
///////////////

deDEAnimator &dearRule::GetModule() const{
	return pInstance.GetModule();
}



int dearRule::GetBoneMappingCount() const{
	return pUseAllBones ? pInstance.GetBoneStateList().GetStateCount() : pBoneMappings.GetCount();
}

int dearRule::GetBoneMappingFor(int boneIndex) const{
	if(pUseAllBones){
		DEASSERT_TRUE(boneIndex >= 0)
		DEASSERT_TRUE(boneIndex < pInstance.GetBoneStateList().GetStateCount())
		return boneIndex;
		
	}else{
		return pBoneMappings[boneIndex];
	}
}



int dearRule::GetVPSMappingCount() const{
	return pUseAllVPS ? pInstance.GetVPSStateList().GetStateCount() : pVPSMappings.GetCount();
}

int dearRule::GetVPSMappingFor(int vpsIndex) const{
	if(pUseAllVPS){
		DEASSERT_TRUE(vpsIndex >= 0)
		DEASSERT_TRUE(vpsIndex < pInstance.GetVPSStateList().GetStateCount())
		return vpsIndex;
		
	}else{
		return pVPSMappings[vpsIndex];
	}
}



float dearRule::GetBlendFactor() const{
	const float blendFactor = pTargetBlendFactor.GetValue(pInstance, pBlendFactor);
	return pInvertBlendFactor ? 1.0f - blendFactor : blendFactor;
}

dearAnimation *dearRule::GetUseAnimation() const{
	return pInstance.GetAnimation() ? pInstance.GetAnimation() : pAnimator.GetAnimation();
}



void dearRule::CaptureStateInto(int){
}

void dearRule::StoreFrameInto(int, const char *, float){
}

bool dearRule::RebuildInstance() const{
	return false;
}

void dearRule::ControllerChanged(int){
}

void dearRule::RuleChanged(){
	pUpdateBoneMappings();
	pUpdateVPSMappings();
}



// Private functions
//////////////////////

void dearRule::pUpdateBoneMappings(){
	const dearBoneStateList &stalist = pInstance.GetBoneStateList();
	const decStringSet &boneList = pRule.GetListBones();
	const int boneCount = boneList.GetCount();
	
	// determine the count of bones
	pUseAllBones = boneCount == 0;
	
	// if the count differs recreate the array
	pBoneMappings.SetCountDiscard(boneCount);
	
	// update the bone mappings. the bones in the animator are the
	// same as the bones in the component rig.
	boneList.VisitIndexed([&](int i, const decString &bone){
		pBoneMappings[i] = stalist.IndexOfStateNamed(bone);
	});
}

void dearRule::pUpdateVPSMappings(){
	const dearVPSStateList &stalist = pInstance.GetVPSStateList();
	const decStringSet &vpsList = pRule.GetListVertexPositionSets();
	const int vpsCount = vpsList.GetCount();
	
	// determine the count of vertex position sets
	pUseAllVPS = vpsCount == 0;
	
	// if the count differs recreate the array
	pVPSMappings.SetCountDiscard(vpsCount);
	
	// update the vertex position set mappings. the vertex position sets in the animator are the
	// same as the vertex position sets in the component model.
	vpsList.VisitIndexed([&](int i, const decString &vps){
		pVPSMappings[i] = stalist.IndexOfStateNamed(vps);
	});
}
