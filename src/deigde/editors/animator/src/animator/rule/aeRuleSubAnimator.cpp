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

aeRuleSubAnimator::aeRuleSubAnimator(aeWindowMain &windowMain, const char *aname) :
aeRule(windowMain, aeMCRuleSubAnimator::Ref::New(windowMain, this),
	deAnimatorRuleVisitorIdentify::ertSubAnimator, aname),
pathSubAnimator(windowMain.GetMCAnimatorProperties().ruleSubAnimator.pathSubAnimator, GetMetaContext().StaticCast<aeMCRuleSubAnimator>()),
enablePosition(windowMain.GetMCAnimatorProperties().ruleSubAnimator.enablePosition, GetMetaContext().StaticCast<aeMCRuleSubAnimator>()),
enableOrientation(windowMain.GetMCAnimatorProperties().ruleSubAnimator.enableOrientation, GetMetaContext().StaticCast<aeMCRuleSubAnimator>()),
enableSize(windowMain.GetMCAnimatorProperties().ruleSubAnimator.enableSize, GetMetaContext().StaticCast<aeMCRuleSubAnimator>()),
enableVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleSubAnimator.enableVertexPositionSet, GetMetaContext().StaticCast<aeMCRuleSubAnimator>())
{
	pathSubAnimator.SetOnChanged([this](){
		NotifyRuleChanged();
		LoadSubAnimator();
	});
	
	enablePosition.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleSubAnimator*)GetEngineRule())->SetEnablePosition(enablePosition);
		}
		NotifyRuleChanged();
	});
	
	enableOrientation.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleSubAnimator*)GetEngineRule())->SetEnableOrientation(enableOrientation);
		}
		NotifyRuleChanged();
	});
	
	enableSize.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleSubAnimator*)GetEngineRule())->SetEnableSize(enableSize);
		}
		NotifyRuleChanged();
	});
	
	enableVertexPositionSet.SetOnChanged([this](){
		if(GetEngineRule()){
			((deAnimatorRuleSubAnimator*)GetEngineRule())->SetEnableVertexPositionSet(enableVertexPositionSet);
		}
		NotifyRuleChanged();
	});
}

aeRuleSubAnimator::aeRuleSubAnimator(aeWindowMain &windowMain, const aeRuleSubAnimator &copy) :
aeRuleSubAnimator(windowMain, copy.name)
{
	pInitCopy(copy);
	enablePosition.SetValue(copy.enablePosition, false);
	enableOrientation.SetValue(copy.enableOrientation, false);
	enableSize.SetValue(copy.enableSize, false);
	enableVertexPositionSet.SetValue(copy.enableVertexPositionSet, false);
}

aeRuleSubAnimator::~aeRuleSubAnimator() = default;


// Management
///////////////

void aeRuleSubAnimator::SetPathSubAnimator(const char *value){
	pathSubAnimator = value;
}

void aeRuleSubAnimator::SetEnablePosition(bool value){
	enablePosition = value;
}

void aeRuleSubAnimator::SetEnableOrientation(bool value){
	enableOrientation = value;
}

void aeRuleSubAnimator::SetEnableSize(bool value){
	enableSize = value;
}

void aeRuleSubAnimator::SetEnableVertexPositionSet(bool value){
	enableVertexPositionSet = value;
}

void aeRuleSubAnimator::LoadSubAnimator(){
	// clear connections only if not manually set
	const bool autoConnections = !pConnections.HasMatching([](const aeController *controller){
		return controller != nullptr;
	});
	
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
	aeAnimator::Ref animator;
	
	// try to load the animator
	if(!pathSubAnimator.GetValue().IsEmpty()){
		parentAnimator->GetLogger()->LogInfoFormat(LOGSOURCE,
			"Rule Sub Animator: Loading animator %s...", pathSubAnimator.GetValue().GetString());
		
		try{
			// load from file
			animator = parentAnimator->GetWindowMain().GetLoadSaveSystem().LoadAnimator(
				decPath::AbsolutePathUnix(pathSubAnimator, parentAnimator->GetDirectoryPath()).GetPathUnix());
			
			
			// create animator
			pSubAnimator = engine->GetAnimatorManager()->CreateAnimator();
			pSubAnimator->SetRig(animator->GetEngineAnimator()->GetRig());
			pSubAnimator->SetAnimation(animator->GetEngineAnimator()->GetAnimation());
			
			// add controllers
			animator->GetControllers().Visit([&](const aeController &controller){
				const deAnimatorController::Ref engController(deAnimatorController::Ref::New());
				engController->SetName(controller.GetName());
				engController->SetValueRange(controller.GetMinimumValue(), controller.GetMaximumValue());
				engController->SetCurrentValue(controller.GetCurrentValue());
				engController->SetFrozen(controller.GetFrozen());
				engController->SetClamp(controller.GetClamp());
				engController->SetVector(controller.GetVector());
				
				pSubAnimator->AddController(engController);
			});
			
			// add links
			animator->GetLinks().Visit([&](const aeLink &link){
				const deAnimatorLink::Ref engLink(deAnimatorLink::Ref::New());
				
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
			});
			
			// add rules
			animator->GetRules().Visit([&](aeRule &rule2){
				pSubAnimator->AddRule(rule2.CreateEngineRule());
			});
			
			// free the loaded animator as it is no more needed
		}catch(const deException &e){
			parentAnimator->GetLogger()->LogException("Animator Editor", e);
		}
	}
	
	if(pSubAnimator && autoConnections){
		while(pConnections.GetCount() < pSubAnimator->GetControllers().GetCount()){
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


void aeRuleSubAnimator::SetControllerAt(int position, aeController *controller){
	if(controller == pConnections.GetAt(position)){
		return;
	}
	
	pConnections.SetAt(position, controller);
	
	deAnimatorRuleSubAnimator * const rule = (deAnimatorRuleSubAnimator*)GetEngineRule();
	if(rule){
		pUpdateConnections(*rule);
	}
	
	NotifyRuleChanged();
}


deAnimatorRule::Ref aeRuleSubAnimator::CreateEngineRule(){
	const deAnimatorRuleSubAnimator::Ref engRule(deAnimatorRuleSubAnimator::Ref::New());
	
	InitEngineRule(engRule);
	
	engRule->SetSubAnimator(pSubAnimator);
	
	pUpdateConnections(*engRule);
	
	engRule->SetEnablePosition(enablePosition);
	engRule->SetEnableOrientation(enableOrientation);
	engRule->SetEnableSize(enableSize);
	engRule->SetEnableVertexPositionSet(enableVertexPositionSet);
	
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
	pathSubAnimator = copy.pathSubAnimator;
	enablePosition = copy.enablePosition;
	enableOrientation = copy.enableOrientation;
	enableSize = copy.enableSize;
	enableVertexPositionSet = copy.enableVertexPositionSet;
	
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

void aeRuleSubAnimator::ListLinks(aeLink::List &list){
	aeRule::ListLinks(list);
}

void aeRuleSubAnimator::OnParentAnimatorChanged(){
	LoadSubAnimator();
}



// Private Functions
//////////////////////

void aeRuleSubAnimator::pUpdateConnections(deAnimatorRuleSubAnimator &rule) const{
	if(pConnections.HasMatching([](const aeController *controller){
		return controller != nullptr;
	})){
		// custom connections
		pConnections.VisitIndexed([&](int i, const aeController *controller){
			int engController = -1;
			if(controller){
				engController = controller->GetIndex();
			}
			rule.SetConnectionAt(i, engController);
		});
		
	}else{
		// auto connections
		if(GetAnimator()){
			const aeController::List &controllers = GetAnimator()->GetControllers();
			int i;
			for(i=0; i<pConnections.GetCount(); i++){
				rule.SetConnectionAt(i, controllers.IndexOfNamed(
					pSubAnimator->GetControllers()[i]->GetName()));
			}
			
		}else{
			rule.ClearConnections();
		}
	}
}
