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


// Class aeRuleSubAnimator::Connection
////////////////////////////////////////

aeRuleSubAnimator::Connection::MetaContext::Ref aeRuleSubAnimator::Connection::CreateMetaContext(
aeWindowMain &windowMain, Connection *connection){
	return MetaContext::Ref::New("animator.rule_subanimator_connection",
		"Connection", "Connection properties",
		windowMain.GetMCAnimatorProperties().ruleSubAnimator.connection.metaProperties, connection);
}

aeRuleSubAnimator::Connection::Connection(aeWindowMain &awindowMain) :
windowMain(awindowMain),
metaContext(CreateMetaContext(windowMain, this)),
mpTarget(windowMain.GetMCAnimatorProperties().ruleSubAnimator.connection.target, metaContext),
mpController(windowMain.GetMCAnimatorProperties().ruleSubAnimator.connection.controller, metaContext)
{
	mpTarget.onValueChanged = [this](){
		if(parentRule){
			const auto sa = parentRule->GetSubAnimator();
			targetIndex = sa ? sa->GetControllers().IndexOfNamed(mpTarget) : -1;
			
			parentRule->UpdateEngineConnections();
			parentRule->NotifyRuleChanged();
			
		}else{
			targetIndex = -1;
		}
	};
	mpController.onValueChanged = mpTarget.onValueChanged;
}

aeRuleSubAnimator::Connection::Connection(const Connection &copy) :
Connection(copy.windowMain){
	mpTarget.SetValue(copy.mpTarget, false);
	mpController.SetValue(copy.mpController, false);
}

aeRuleSubAnimator::Connection::~Connection(){
	if(metaContext){
		metaContext->Dispose();
	}
}

igdeEnvironment &aeRuleSubAnimator::Connection::GetEnvironment() const{
	return windowMain.GetEnvironment();
}

igdeUndoSystem *aeRuleSubAnimator::Connection::GetUndoSystem() const{
	return windowMain.GetAnimator()->GetUndoSystem();
}


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
mpEnableVertexPositionSet(windowMain.GetMCAnimatorProperties().ruleSubAnimator.enableVertexPositionSet, metaContext),
mpConnections(windowMain.GetMCAnimatorProperties().ruleSubAnimator.connection.connections, metaContext),
mpConnection(windowMain.GetMCAnimatorProperties().ruleSubAnimator.connection.connection, metaContext)
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
	
	mpConnections.onValueChanged = [this](){
		UpdateEngineConnections();
		NotifyRuleChanged();
	};
	mpConnections.onObjectAdded = [this](const Connection::Ref &connection){
		connection->parentRule = this;
	};
	mpConnections.onObjectRemoved = [this](const Connection::Ref &connection){
		connection->parentRule = nullptr;
	};
	mpConnections.onActiveChanged = [this](){
		mpConnection = mpConnections.GetActive()
			? mpConnections.GetActive()->metaContext
			: mpConnection.Property().GetDefaultValue();
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
	
	auto connections = igdeMetaPropertyListStorage<Connection>::Storage::ListType();
	copy.mpConnections->Visit([&](const Connection::Ref &connection){
		connections.Add(Connection::Ref::New(connection));
	});
	mpConnections.SetValue(connections, false);
}

aeRuleSubAnimator::~aeRuleSubAnimator() = default;


// Management
///////////////

void aeRuleSubAnimator::LoadSubAnimator(){
	mpConnections->Visit([](Connection &connection){
		connection.targetIndex = -1;
	});
	pSubAnimator.Clear();
	
	// if there is no parent animator no loading can be done
	aeAnimator * const parentAnimator = GetAnimator();
	if(!parentAnimator){
		return;
	}
	
	// otherwise try loading the animator
	auto rule = dynamic_cast<deAnimatorRuleSubAnimator*>(GetEngineRule());
	auto engine = parentAnimator->GetEngine();
	
	// try to load the animator
	if(!mpPathSubAnimator->IsEmpty()){
		parentAnimator->GetLogger()->LogInfoFormat(LOGSOURCE,
			"Rule Sub Animator: Loading animator %s...", mpPathSubAnimator->GetString());
		
		try{
			// load from file
			const auto animator = parentAnimator->GetWindowMain().GetLoadSaveSystem().LoadAnimator(
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
	
	if(pSubAnimator){
		mpConnections->Visit([&](Connection &connection){
			const auto &target = connection.mpTarget.GetValue();
			if(!target.IsEmpty()){
				connection.targetIndex = pSubAnimator->GetControllers().IndexOfNamed(target);
			}
		});
	}
	
	// if the engine rule exists assign sub animator
	if(!rule){
		return;
	}
	
	rule->SetSubAnimator(pSubAnimator);
	
	pUpdateEngineConnections(*rule);
	NotifyRuleChanged();
}


deAnimatorRule::Ref aeRuleSubAnimator::CreateEngineRule(){
	auto engRule = deAnimatorRuleSubAnimator::Ref::New();
	
	InitEngineRule(engRule);
	
	engRule->SetSubAnimator(pSubAnimator);
	pUpdateEngineConnections(*engRule);
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

void aeRuleSubAnimator::UpdateEngineConnections() const{
	auto rule = dynamic_cast<deAnimatorRuleSubAnimator*>(GetEngineRule());
	if(rule){
		pUpdateEngineConnections(*rule);
	}
}


// Private Functions
//////////////////////

void aeRuleSubAnimator::pUpdateEngineConnections(deAnimatorRuleSubAnimator &rule) const{
	if(mpConnections->IsEmpty()){
		rule.SetMatchingConnections(*GetAnimator()->GetEngineAnimator());
		return;
	}
	
	rule.ClearConnections();
	
	mpConnections->Visit([&](const Connection &connection){
		if(connection.targetIndex == -1){
			return;
		}
		
		const auto &name = connection.mpTarget.GetValue();
		if(name.IsEmpty()){
			return;
		}
		
		const auto &controller = connection.mpController;
		rule.SetConnectionAt(connection.targetIndex, controller ? controller->GetIndex() : -1);
	});
}
