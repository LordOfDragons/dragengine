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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "aeRuleSubAnimator.h"
#include "../aeAnimator.h"
#include "../controller/aeController.h"
#include "../link/aeLink.h"
#include "../../gui/aeWindowMain.h"
#include "../../loadsave/aeLoadSaveSystem.h"

#include <deigde/gamedefinition/igdeGameDefinition.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/curve/decCurveBezierPoint.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorManager.h>
#include <dragengine/resources/animator/deAnimatorLink.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleSubAnimator.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>



// Definitions
////////////////

#define LOGSOURCE "Animator Editor"



// Class aeRuleSubAnimator
////////////////////////////

// Constructor, destructor
////////////////////////////

aeRuleSubAnimator::aeRuleSubAnimator() :
aeRule(deAnimatorRuleVisitorIdentify::ertSubAnimator),
pEnablePosition(true),
pEnableOrientation(true),
pEnableSize(true),
pEnableVertexPositionSet(true)
{
	SetName("Sub Animator");
}

aeRuleSubAnimator::aeRuleSubAnimator(const aeRuleSubAnimator &copy) :
aeRule(copy),
pPathSubAnimator(copy.pPathSubAnimator),
pEnablePosition(copy.pEnablePosition),
pEnableOrientation(copy.pEnableOrientation),
pEnableSize(copy.pEnableSize),
pEnableVertexPositionSet(copy.pEnableVertexPositionSet),
pConnections(copy.pConnections)
{
	pSubAnimator = copy.pSubAnimator;
}

aeRuleSubAnimator::~aeRuleSubAnimator(){
	if(pSubAnimator) pSubAnimator->FreeReference();
}



// Management
///////////////

void aeRuleSubAnimator::SetPathSubAnimator(const char *path){
	if(pPathSubAnimator == path){
		return;
	}
	
	pPathSubAnimator = path;
	NotifyRuleChanged();
}

void aeRuleSubAnimator::LoadSubAnimator(){
	// clear connections only if not manually set
	int i, count = pConnections.GetCount();
	for(i=0; i<count; i++){
		if(pConnections.GetAt(i)){
			break;
		}
	}
	
	const bool autoConnections = i == count;
	if(autoConnections){
		pConnections.RemoveAll();
	}
	
	// release the sub animator
	if(pSubAnimator){
		pSubAnimator = nullptr;
	}
	
	// if there is no parent animator no loading can be done
	aeAnimator * const parentAnimator = GetAnimator();
	if(!parentAnimator){
		return;
	}
	
	// otherwise try loading the animator
	deAnimatorRuleSubAnimator *rule = (deAnimatorRuleSubAnimator*)GetEngineRule();
	deEngine *engine = parentAnimator->GetEngine();
	deAnimatorController *engController = nullptr;
	deAnimatorLink *engLink = nullptr;
	aeAnimator::Ref animator;
	int c, controllerCount;
	int l, linkCount;
	int r, ruleCount;
	
	// try to load the animator
	if(!pPathSubAnimator.IsEmpty()){
		parentAnimator->GetLogger()->LogInfoFormat(LOGSOURCE,
			"Rule Sub Animator: Loading animator %s...", pPathSubAnimator.GetString());
		
		try{
			// load from file
			animator = parentAnimator->GetWindowMain().GetLoadSaveSystem().LoadAnimator(
				decPath::AbsolutePathUnix(pPathSubAnimator, parentAnimator->GetDirectoryPath()).GetPathUnix());
			
			controllerCount = animator->GetControllers().GetCount();
			linkCount = animator->GetLinks().GetCount();
			ruleCount = animator->GetRules().GetCount();
			
			// create animator
			pSubAnimator = engine->GetAnimatorManager()->CreateAnimator();
			pSubAnimator->SetRig(animator->GetEngineAnimator()->GetRig());
			pSubAnimator->SetAnimation(animator->GetEngineAnimator()->GetAnimation());
			
			// add controllers
			for(c=0; c<controllerCount; c++){
				const aeController &controller = *animator->GetControllers().GetAt(c);
				
				engController = new deAnimatorController;
				engController->SetName(controller.GetName());
				engController->SetValueRange(controller.GetMinimumValue(), controller.GetMaximumValue());
				engController->SetCurrentValue(controller.GetCurrentValue());
				engController->SetFrozen(controller.GetFrozen());
				engController->SetClamp(controller.GetClamp());
				engController->SetVector(controller.GetVector());
				
				pSubAnimator->AddController(engController);
				engController = nullptr;
			}
			
			// add links
			for(l=0; l<linkCount; l++){
				const aeLink &link = *animator->GetLinks().GetAt(l);
				
				engLink = new deAnimatorLink;
				
				if(link.GetController()){
					engLink->SetController(animator->GetControllers().IndexOf(link.GetController()));
					
				}else{
					engLink->SetController(-1);
				}
				
				engLink->SetCurve(link.GetCurve());
				engLink->SetRepeat(link.GetRepeat());
				engLink->SetBone(link.GetBone());
				engLink->SetBoneParameter(link.GetBoneParameter());
				engLink->SetBoneValueRange(link.GetBoneMinimum(), link.GetBoneMaximum());
				engLink->SetVertexPositionSet(link.GetVertexPositionSet());
				engLink->SetVertexPositionSetValueRange(
					link.GetVertexPositionSetMinimum(), link.GetVertexPositionSetMaximum());
				
				pSubAnimator->AddLink(engLink);
				engLink = nullptr;
			}
			
			// add rules
			for(r=0; r<ruleCount; r++){
				pSubAnimator->AddRule(animator->GetRules().GetAt(r)->CreateEngineRule());
			}
			
			// free the loaded animator as it is no more needed
		}catch(const deException &e){
			parentAnimator->GetLogger()->LogException("Animator Editor", e);
			if(engLink){
				delete engLink;
			}
			if(engController){
				delete engController;
			}
		}
	}
	
	if(pSubAnimator && autoConnections){
		while(pConnections.GetCount() < pSubAnimator->GetControllerCount()){
			pConnections.Add(nullptr);
		}
	}
	
	// if the engine rule exists assign sub animator
	if(!rule){
		return;
	}
	
	rule->SetSubAnimator(pSubAnimator);
	
	pUpdateConnections(*rule);
	NotifyRuleChanged();
}



int aeRuleSubAnimator::GetConnectionCount() const{
	return pConnections.GetCount();
}

aeController *aeRuleSubAnimator::GetControllerAt(int position) const{
	return (aeController*)pConnections.GetAt(position);
}

void aeRuleSubAnimator::SetControllerAt(int position, aeController *controller){
	if(controller == (aeController*)pConnections.GetAt(position)){
		return;
	}
	
	pConnections.SetAt(position, controller);
	
	deAnimatorRuleSubAnimator * const rule = (deAnimatorRuleSubAnimator*)GetEngineRule();
	if(rule){
		pUpdateConnections(*rule);
	}
	
	NotifyRuleChanged();
}



void aeRuleSubAnimator::SetEnablePosition(bool enabled){
	if(enabled != pEnablePosition){
		pEnablePosition = enabled;
		
		if(GetEngineRule()){
			((deAnimatorRuleSubAnimator*)GetEngineRule())->SetEnablePosition(enabled);
			NotifyRuleChanged();
		}
	}
}

void aeRuleSubAnimator::SetEnableOrientation(bool enabled){
	if(enabled != pEnableOrientation){
		pEnableOrientation = enabled;
		
		if(GetEngineRule()){
			((deAnimatorRuleSubAnimator*)GetEngineRule())->SetEnableOrientation(enabled);
			NotifyRuleChanged();
		}
	}
}

void aeRuleSubAnimator::SetEnableSize(bool enabled){
	if(enabled != pEnableSize){
		pEnableSize = enabled;
		
		if(GetEngineRule()){
			((deAnimatorRuleSubAnimator*)GetEngineRule())->SetEnableSize(enabled);
			NotifyRuleChanged();
		}
	}
}

void aeRuleSubAnimator::SetEnableVertexPositionSet(bool enabled){
	if(enabled != pEnableVertexPositionSet){
		pEnableVertexPositionSet = enabled;
		
		if(GetEngineRule()){
			((deAnimatorRuleSubAnimator*)GetEngineRule())->SetEnableVertexPositionSet(enabled);
			NotifyRuleChanged();
		}
	}
}



deAnimatorRule::Ref aeRuleSubAnimator::CreateEngineRule(){
	const deAnimatorRuleSubAnimator *engRule(deAnimatorRuleSubAnimator::Ref::New());
	
	InitEngineRule(engRule);
	
	engRule->SetSubAnimator(pSubAnimator);
	
	pUpdateConnections(*engRule);
	
	engRule->SetEnablePosition(pEnablePosition);
	engRule->SetEnableOrientation(pEnableOrientation);
	engRule->SetEnableSize(pEnableSize);
	engRule->SetEnableVertexPositionSet(pEnableVertexPositionSet);
	
	return engRule;
}



void aeRuleSubAnimator::UpdateCompAnim(){
	deAnimatorRuleSubAnimator * const rule = (deAnimatorRuleSubAnimator*)GetEngineRule();
	if(rule){
		rule->SetSubAnimator(pSubAnimator);
		NotifyRuleChanged();
	}
}



// Operators
//////////////

aeRuleSubAnimator &aeRuleSubAnimator::operator=(const aeRuleSubAnimator &copy){
	SetPathSubAnimator(copy.pPathSubAnimator);
	
	SetEnablePosition(copy.pEnablePosition);
	SetEnableOrientation(copy.pEnableOrientation);
	SetEnableSize(copy.pEnableSize);
	SetEnableVertexPositionSet(copy.pEnableVertexPositionSet);
	
	pSubAnimator = copy.pSubAnimator;
	pConnections = copy.pConnections;
	
	deAnimatorRuleSubAnimator * const rule = static_cast<deAnimatorRuleSubAnimator*>(GetEngineRule());
	if(rule){
		pUpdateConnections(*rule);
	}
	
	aeRule::operator=(copy);
	return *this;
}



aeRule::Ref aeRuleSubAnimator::CreateCopy() const{
	return Ref::New(*this);
}

void aeRuleSubAnimator::ListLinks(aeLinkList &list){
	aeRule::ListLinks(list);
}

void aeRuleSubAnimator::OnParentAnimatorChanged(){
	LoadSubAnimator();
}



// Private Functions
//////////////////////

void aeRuleSubAnimator::pUpdateConnections(deAnimatorRuleSubAnimator &rule) const{
	const int count = pConnections.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(pConnections.GetAt(i)){
			break;
		}
	}
	
	if(i == count){
		// auto connections
		if(GetAnimator()){
			const aeControllerList &controllers = GetAnimator()->GetControllers();
			for(i=0; i<count; i++){
				rule.SetConnectionAt(i, controllers.IndexOfNamed(pSubAnimator->GetControllerAt(i)->GetName()));
			}
			
		}else{
			rule.ClearConnections();
		}
		
	}else{
		// custom connections
		for(i=0; i<count; i++){
			int engController = -1;
			if(pConnections[i]){
				engController = ((aeController*)pConnections.GetAt(i))->GetEngineControllerIndex();
			}
			rule.SetConnectionAt(i, engController);
		}
	}
}
