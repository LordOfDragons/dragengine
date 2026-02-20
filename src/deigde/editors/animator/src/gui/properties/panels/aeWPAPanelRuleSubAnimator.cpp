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

#include <math.h>

#include "aeWPAPanelRuleSubAnimator.h"
#include "../aeWPRule.h"
#include "../aeWindowProperties.h"
#include "../../aeWindowMain.h"
#include "../../../animator/aeAnimator.h"
#include "../../../animator/controller/aeController.h"
#include "../../../animator/rule/aeRuleSubAnimator.h"
#include "../../../undosys/rule/subanimator/aeURuleSASetPathAnimator.h"
#include "../../../undosys/rule/subanimator/aeURuleSASetConController.h"
#include "../../../undosys/rule/subanimator/aeURuleSubAnimToggleEnablePosition.h"
#include "../../../undosys/rule/subanimator/aeURuleSubAnimToggleEnableRotation.h"
#include "../../../undosys/rule/subanimator/aeURuleSubAnimToggleEnableSize.h"
#include "../../../undosys/rule/subanimator/aeURuleSubAnimToggleEnableVertexPositionSet.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleSubAnimator.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	aeWPAPanelRuleSubAnimator &pPanel;
	
public:
	using Ref = deTObjectReference<cBaseAction>;
	cBaseAction(aeWPAPanelRuleSubAnimator &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleSubAnimator * const rule = (aeRuleSubAnimator*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(animator, rule));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(aeAnimator *animator, aeRuleSubAnimator *rule) = 0;
	
	void Update() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleSubAnimator * const rule = (aeRuleSubAnimator*)pPanel.GetRule();
		if(animator && rule){
			Update(*animator, *rule);
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const aeAnimator &, const aeRuleSubAnimator &){
		SetEnabled(true);
		SetSelected(false);
	}
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	aeWPAPanelRuleSubAnimator &pPanel;
	
public:
	using Ref = deTObjectReference<cBaseComboBoxListener>;
	cBaseComboBoxListener(aeWPAPanelRuleSubAnimator &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleSubAnimator * const rule = (aeRuleSubAnimator*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(comboBox, animator, rule));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox *comboBox, aeAnimator *animator, aeRuleSubAnimator *rule) = 0;
};


class cPathAnimator : public igdeEditPathListener{
	aeWPAPanelRuleSubAnimator &pPanel;
	
public:
	using Ref = deTObjectReference<cPathAnimator>;
	cPathAnimator(aeWPAPanelRuleSubAnimator &panel) : pPanel(panel){}
	
	void OnEditPathChanged(igdeEditPath *editPath) override{
		aeRuleSubAnimator * const rule = (aeRuleSubAnimator*)pPanel.GetRule();
		if(!rule || rule->GetPathSubAnimator() == editPath->GetPath()){
			return;
		}
		
		pPanel.GetAnimator()->GetUndoSystem()->Add(
			aeURuleSASetPathAnimator::Ref::New(rule, editPath->GetPath()));
	}
};

class cComboConnection : public cBaseComboBoxListener{
public:
	using Ref = deTObjectReference<cComboConnection>;
	cComboConnection(aeWPAPanelRuleSubAnimator &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox*, aeAnimator*, aeRuleSubAnimator*) override{
		pPanel.UpdateConnection();
		return {};
	}
};

class cActionEnablePosition : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionEnablePosition>;
	
public:
	cActionEnablePosition(aeWPAPanelRuleSubAnimator &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleSubAnimator.EnablePosition", nullptr,
		"@Animator.WPAPanelRuleSubAnimator.EnablePosition.ToolTip"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleSubAnimator *rule) override{
		return aeURuleSubAnimToggleEnablePosition::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleSubAnimator &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnablePosition());
	}
};

class cActionEnableRotation : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionEnableRotation>;
	
public:
	cActionEnableRotation(aeWPAPanelRuleSubAnimator &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleSubAnimator.EnableRotation", nullptr,
		"@Animator.WPAPanelRuleSubAnimator.EnableRotation.ToolTip"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleSubAnimator *rule) override{
		return aeURuleSubAnimToggleEnableRotation::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleSubAnimator &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableOrientation());
	}
};

class cActionEnableSize : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionEnableSize>;
	
public:
	cActionEnableSize(aeWPAPanelRuleSubAnimator &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleSubAnimator.EnableSize", nullptr,
		"@Animator.WPAPanelRuleSubAnimator.EnableSize.ToolTip"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleSubAnimator *rule) override{
		return aeURuleSubAnimToggleEnableSize::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleSubAnimator &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableSize());
	}
};

class cActionEnableVertexPositionSet : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionEnableVertexPositionSet>;
	
public:
	cActionEnableVertexPositionSet(aeWPAPanelRuleSubAnimator &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleSubAnimator.EnableVertexPositionSet", nullptr,
		"@Animator.WPAPanelRuleSubAnimator.EnableVertexPositionSet.ToolTip"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleSubAnimator *rule) override{
		return aeURuleSubAnimToggleEnableVertexPositionSet::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleSubAnimator &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableVertexPositionSet());
	}
};

class cComboConnectionController : public cBaseComboBoxListener{
	bool &pPreventUpdate;
	
public:
	using Ref = deTObjectReference<cComboConnectionController>;
	cComboConnectionController(aeWPAPanelRuleSubAnimator &panel, bool &preventUpdate) :
	cBaseComboBoxListener(panel), pPreventUpdate(preventUpdate){}
	
	igdeUndo::Ref OnChanged(igdeComboBox*, aeAnimator*, aeRuleSubAnimator *rule) override{
		if(pPreventUpdate){
			return {};
		}
		
		const int connection = pPanel.GetCBConnection();
		aeController * const controller = pPanel.GetCBConnectionController();
		return connection != -1 && rule->GetConnections().GetAt(connection) != controller
			? aeURuleSASetConController::Ref::New(rule, connection, controller) : igdeUndo::Ref();
	}
};

}



// Class aeWPAPanelRuleSubAnimator
////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeWPAPanelRuleSubAnimator::aeWPAPanelRuleSubAnimator(aeWPRule &wpRule) :
aeWPAPanelRule(wpRule, deAnimatorRuleVisitorIdentify::ertSubAnimator),
pPreventUpdate(false)
{
	igdeEnvironment &env = wpRule.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref groupBox, formLine;
	
	
	helper.GroupBox(*this, groupBox, "@Animator.WPAPanelRuleSubAnimator.SubAnimator");
	
	helper.EditPath(groupBox, "@Animator.WPAPanelRuleSubAnimator.Path",
		"@Animator.WPAPanelRuleSubAnimator.Path.ToolTip", igdeEnvironment::efpltAnimator,
		pEditPathAnimator, cPathAnimator::Ref::New(*this));
	
	helper.CheckBox(groupBox, pChkEnablePosition, cActionEnablePosition::Ref::New(*this));
	helper.CheckBox(groupBox, pChkEnableRotation, cActionEnableRotation::Ref::New(*this));
	helper.CheckBox(groupBox, pChkEnableSize, cActionEnableSize::Ref::New(*this));
	helper.CheckBox(groupBox, pChkEnableVertexPositionSet, cActionEnableVertexPositionSet::Ref::New(*this));
	
	
	helper.GroupBox(*this, groupBox, "@Animator.WPAPanelRuleSubAnimator.Connections");
	
	helper.Label(groupBox, "@Animator.WPAPanelRuleSubAnimator.ConnectionInfo");
	helper.Label(groupBox, "@Animator.WPAPanelRuleSubAnimator.ConnectionInfo.ToolTip",
		"@Animator.WPAPanelRuleSubAnimator.ConnectionInfo.Value");
	
	helper.ComboBox(groupBox, "@Animator.WPAPanelRuleSubAnimator.Target",
		"@Animator.WPAPanelRuleSubAnimator.Target.ToolTip", pCBConnection, cComboConnection::Ref::New(*this));
	
	helper.ComboBox(groupBox, "@Animator.WPAPanelRuleSubAnimator.Controller",
		"@Animator.WPAPanelRuleSubAnimator.Controller.ToolTip",
		pCBConnectionController, cComboConnectionController::Ref::New(*this, pPreventUpdate));
	
	
	UpdateControllerList();
}

aeWPAPanelRuleSubAnimator::~aeWPAPanelRuleSubAnimator(){
}



// Management
///////////////

void aeWPAPanelRuleSubAnimator::OnAnimatorPathChanged(){
	if(GetAnimator()){
		pEditPathAnimator->SetBasePath(GetAnimator()->GetDirectoryPath());
		
	}else{
		pEditPathAnimator->SetBasePath("");
	}
}

void aeWPAPanelRuleSubAnimator::UpdateConnectionList(){
	const aeRuleSubAnimator * const rule = (aeRuleSubAnimator*)GetRule();
	const int selection = pCBConnection->GetSelection();
	const decString selectionText(pCBConnection->GetText());
	
	pCBConnection->RemoveAllItems();
	
	if(rule && rule->GetSubAnimator()){
		const deAnimator &subAnimator = *rule->GetSubAnimator();
		const int nameCount = subAnimator.GetControllers().GetCount();
		const int count = rule->GetConnections().GetCount();
		decString text;
		int i;
		
		for(i=0; i<count; i++){
			if(i < nameCount){
				text.Format("%d: %s", i, subAnimator.GetControllers().GetAt(i)->GetName().GetString());
				
			}else{
				text.Format("%d: < invalid >", i);
			}
			
			pCBConnection->AddItem(text);
		}
	}
	
	if(selection < pCBConnection->GetItems().GetCount()){
		pCBConnection->SetSelection(selection);
		
	}else if(pCBConnectionController->GetItems().IsNotEmpty()){
		pCBConnection->SetSelection(pCBConnection->GetItems().GetCount() - 1);
	}
	
	if(pCBConnection->GetText() != selectionText){
		pCBConnection->SetSelection(-1);
	}
}

void aeWPAPanelRuleSubAnimator::UpdateControllerList(){
	const igdeUIHelper::EnableBoolGuard pu(pPreventUpdate);
	void * const selection = GetCBConnectionController();
	
	pCBConnectionController->RemoveAllItems();
	pCBConnectionController->AddItem(Translate("Animator.WPAPanelRuleSubAnimator.Controller.NotAssigned").ToUTF8(), nullptr);
	
	if(GetAnimator()){
		decString text;
		GetAnimator()->GetControllers().VisitIndexed([&](int i, aeController *controller){
			text.Format("%d: %s", i, controller->GetName().GetString());
			pCBConnectionController->AddItem(text, nullptr, controller);
		});
	}
	
	pCBConnectionController->SetSelectionWithData(selection);
}

void aeWPAPanelRuleSubAnimator::UpdateRule(){
	aeWPAPanelRule::UpdateRule();
	
	const aeRuleSubAnimator * const rule = (aeRuleSubAnimator*)GetRule();
	
	if(rule){
		pEditPathAnimator->SetPath(rule->GetPathSubAnimator());
		
	}else{
		pEditPathAnimator->ClearPath();
	}
	
	const bool enabled = rule;
	pEditPathAnimator->SetEnabled(enabled);
	pCBConnection->SetEnabled(enabled);
	
	pChkEnablePosition->GetAction()->Update();
	pChkEnableRotation->GetAction()->Update();
	pChkEnableSize->GetAction()->Update();
	pChkEnableVertexPositionSet->GetAction()->Update();
	
	UpdateConnectionList();
}

void aeWPAPanelRuleSubAnimator::UpdateConnection(){
	aeWPAPanelRule::UpdateRule();
	
	const aeRuleSubAnimator * const rule = (aeRuleSubAnimator*)GetRule();
	const int connection = GetCBConnection();
	
	if(rule && connection != -1){
		pCBConnectionController->SetSelectionWithData(rule->GetConnections().GetAt(connection));
		
	}else{
		pCBConnectionController->SetSelectionWithData(nullptr);
	}
	
	pCBConnectionController->SetEnabled(rule && connection != -1);
}

int aeWPAPanelRuleSubAnimator::GetCBConnection() const{
	return pCBConnection->GetSelection();
}

aeController *aeWPAPanelRuleSubAnimator::GetCBConnectionController() const{
	return pCBConnectionController->GetSelectedItem() ?
		(aeController*)pCBConnectionController->GetSelectedItem()->GetData() : nullptr;
}
