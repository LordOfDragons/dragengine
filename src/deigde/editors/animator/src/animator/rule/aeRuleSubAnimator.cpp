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

aeRuleSubAnimator::MetaContext::Ref aeRuleSubAnimator::CreateMetaContext(aeWindowMain &windowMain, aeRuleSubAnimator *rule){
	return MetaContext::Ref::New("animator.rule_sub_animator", "Rule Sub Animator", "Rule sub animator properties",
		windowMain.GetMCAnimatorProperties().ruleSubAnimator.metaProperties, rule);
}

// Constructor, destructor
////////////////////////////

aeRuleSubAnimator::aeRuleSubAnimator(aeWindowMain &windowMain, const char *aname) :
aeRuleSubAnimator(windowMain, aname, CreateMetaContext(windowMain, this)){}

aeRuleSubAnimator::aeRuleSubAnimator(aeWindowMain &windowMain, const char *aname, const MetaContext::Ref &metaContext) :
aeRule(windowMain, metaContext, deAnimatorRuleVisitorIdentify::ertSubAnimator, aname),
mpPathSubAnimator(windowMain.GetMCAnimatorProperties().ruleSubAnimator.pathSubAnimator, metaContext),
mpEnablePosition(windowMain.GetMCAnimatorProperties().ruleSubAnimator.enablePosition, metaContext),
mpEnableOrientation(windowMain.GetMCAnimatorProperties().ruleSubAnimator.enableOrientation, metaContext),
mpEnableSize(windowMain.GetMCAnimatorProperties().ruleSubAnimator.enableSize, metaContext),
mpEnableVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleSubAnimator.enableVertexPositionSet, metaContext)
{
	mpPathSubAnimator.onValueChanged = [this](){
		NotifyRuleChanged();
		LoadSubAnimator();
	};
	
	mpEnablePosition.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleSubAnimator*)GetEngineRule())->SetEnablePosition(mpEnablePosition);
		}
		NotifyRuleChanged();
	};
	
	mpEnableOrientation.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleSubAnimator*)GetEngineRule())->SetEnableOrientation(mpEnableOrientation);
		}
		NotifyRuleChanged();
	};
	
	mpEnableSize.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleSubAnimator*)GetEngineRule())->SetEnableSize(mpEnableSize);
		}
		NotifyRuleChanged();
	};
	
	mpEnableVertexPositionSet.onValueChanged = [this](){
		if(GetEngineRule()){
			((deAnimatorRuleSubAnimator*)GetEngineRule())->SetEnableVertexPositionSet(mpEnableVertexPositionSet);
		}
		NotifyRuleChanged();
	};
}

aeRuleSubAnimator::aeRuleSubAnimator(const aeRuleSubAnimator &copy) :
aeRuleSubAnimator(copy.GetWindowMain(), copy.mpName)
{
	pInitCopy(copy);
	mpPathSubAnimator.SetValue(copy.mpPathSubAnimator, false);
	mpEnablePosition.SetValue(copy.mpEnablePosition, false);
	mpEnableOrientation.SetValue(copy.mpEnableOrientation, false);
	mpEnableSize.SetValue(copy.mpEnableSize, false);
	mpEnableVertexPositionSet.SetValue(copy.mpEnableVertexPositionSet, false);
}

aeRuleSubAnimator::~aeRuleSubAnimator() = default;


// Management
///////////////

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
	if(!mpPathSubAnimator->IsEmpty()){
		parentAnimator->GetLogger()->LogInfoFormat(LOGSOURCE,
			"Rule Sub Animator: Loading animator %s...", mpPathSubAnimator->GetString());
		
		try{
			// load from file
			animator = parentAnimator->GetWindowMain().GetLoadSaveSystem().LoadAnimator(
				decPath::AbsolutePathUnix(mpPathSubAnimator, parentAnimator->GetDirectoryPath()).GetPathUnix());
			
			
			// create animator
			pSubAnimator = engine->GetAnimatorManager()->CreateAnimator();
			pSubAnimator->SetRig(animator->GetEngineAnimator()->GetRig());
			pSubAnimator->SetAnimation(animator->GetEngineAnimator()->GetAnimation());
			
			// add controllers
			animator->mpControllers->Visit([&](const aeController &controller){
				auto engController = deAnimatorController::Ref::New();
				engController->SetName(controller.mpName);
				engController->SetValueRange(controller.mpMinimumValue, controller.mpMaximumValue);
				engController->SetCurrentValue(controller.mpCurrentValue);
				engController->SetFrozen(controller.mpFrozen);
				engController->SetClamp(controller.mpClamp);
				engController->SetVector(controller.mpVector);
				pSubAnimator->AddController(engController);
			});
			
			// add links
			animator->mpLinks->Visit([&](const aeLink &link){
				auto engLink = deAnimatorLink::Ref::New();
				
				if(link.mpController){
					engLink->SetController(animator->mpControllers->IndexOf(link.mpController));
					
				}else{
					engLink->SetController(-1);
				}
				
				engLink->SetCurve(link.mpCurve);
				engLink->SetRepeat(link.mpRepeat);
				engLink->SetBone(link.mpBone);
				engLink->SetBoneParameter(link.mpBoneParameter);
				engLink->SetBoneValueRange(link.mpBoneMinimum, link.mpBoneMaximum);
				engLink->SetVertexPositionSet(link.mpVertexPositionSet);
				engLink->SetVertexPositionSetValueRange(link.mpVpsMinimum, link.mpVpsMaximum);
				
				pSubAnimator->AddLink(engLink);
			});
			
			// add rules
			animator->mpRules->Visit([&](const aeRule::Ref &rule2){
				pSubAnimator->AddRule(rule2->CreateEngineRule());
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
	auto engRule = deAnimatorRuleSubAnimator::Ref::New();
	
	InitEngineRule(engRule);
	
	engRule->SetSubAnimator(pSubAnimator);
	
	pUpdateConnections(*engRule);
	
	engRule->SetEnablePosition(mpEnablePosition);
	engRule->SetEnableOrientation(mpEnableOrientation);
	engRule->SetEnableSize(mpEnableSize);
	engRule->SetEnableVertexPositionSet(mpEnableVertexPositionSet);
	
	return engRule;
}


void aeRuleSubAnimator::UpdateCompAnim(){
	deAnimatorRuleSubAnimator * const rule = (deAnimatorRuleSubAnimator*)GetEngineRule();
	if(rule){
		rule->SetSubAnimator(pSubAnimator);
		NotifyRuleChanged();
	}
}


aeRule::Ref aeRuleSubAnimator::CreateCopy() const{
	return Ref::New(*this);
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
			rule.SetConnectionAt(i, controller ? controller->GetIndex() : -1);
		});
		
	}else{
		// auto connections
		if(GetAnimator()){
			const auto &controllers = GetAnimator()->mpControllers.GetValue();
			pSubAnimator->GetControllers().VisitIndexed([&](int i, const deAnimatorController &sac){
				const auto &name = sac.GetName();
				rule.SetConnectionAt(i, controllers.IndexOfMatching([&](const aeController &controller){
					return controller.mpName == name;
				}));
			});
			
		}else{
			rule.ClearConnections();
		}
	}
}
