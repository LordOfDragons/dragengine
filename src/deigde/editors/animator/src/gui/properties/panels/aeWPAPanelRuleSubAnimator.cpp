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
	typedef deTObjectReference<cBaseAction> Ref;
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
	typedef deTObjectReference<cBaseComboBoxListener> Ref;
	cBaseComboBoxListener(aeWPAPanelRuleSubAnimator &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
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
	typedef deTObjectReference<cPathAnimator> Ref;
	cPathAnimator(aeWPAPanelRuleSubAnimator &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
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
	typedef deTObjectReference<cComboConnection> Ref;
	cComboConnection(aeWPAPanelRuleSubAnimator &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox*, aeAnimator*, aeRuleSubAnimator*) override{
		pPanel.UpdateConnection();
		return {};
	}
};

class cActionEnablePosition : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnablePosition> Ref;
	
public:
	cActionEnablePosition(aeWPAPanelRuleSubAnimator &panel) : cBaseAction(panel,
		"Enable position manipulation", nullptr, "Determines if the position is modified or kept as it is"){ }
	
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
	typedef deTObjectReference<cActionEnableRotation> Ref;
	
public:
	cActionEnableRotation(aeWPAPanelRuleSubAnimator &panel) : cBaseAction(panel,
		"Enable rotation manipulation", nullptr, "Determines if the rotation is modified or kept as it is"){ }
	
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
	typedef deTObjectReference<cActionEnableSize> Ref;
	
public:
	cActionEnableSize(aeWPAPanelRuleSubAnimator &panel) : cBaseAction(panel,
		"Enable size manipulation", nullptr, "Determines if the size is modified or kept as it is"){ }
	
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
	typedef deTObjectReference<cActionEnableVertexPositionSet> Ref;
	
public:
	cActionEnableVertexPositionSet(aeWPAPanelRuleSubAnimator &panel) : cBaseAction(panel,
		"Enable vertex position set manipulation", nullptr,
		"Determines if vertex position set is modified or kept as it is"){ }
	
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
	typedef deTObjectReference<cComboConnectionController> Ref;
	cComboConnectionController(aeWPAPanelRuleSubAnimator &panel, bool &preventUpdate) :
	cBaseComboBoxListener(panel), pPreventUpdate(preventUpdate){}
	
	igdeUndo::Ref OnChanged(igdeComboBox*, aeAnimator*, aeRuleSubAnimator *rule) override{
		if(pPreventUpdate){
			return {};
		}
		
		const int connection = pPanel.GetCBConnection();
		aeController * const controller = pPanel.GetCBConnectionController();
		return connection != -1 && rule->GetControllerAt(connection) != controller
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
	
	
	helper.GroupBox(*this, groupBox, "Sub Animator:");
	
	helper.EditPath(groupBox, "Path:", "Sets the animator to use", igdeEnvironment::efpltAnimator,
		pEditPathAnimator, cPathAnimator::Ref::New(*this));
	
	helper.CheckBox(groupBox, pChkEnablePosition, cActionEnablePosition::Ref::New(*this));
	helper.CheckBox(groupBox, pChkEnableRotation, cActionEnableRotation::Ref::New(*this));
	helper.CheckBox(groupBox, pChkEnableSize, cActionEnableSize::Ref::New(*this));
	helper.CheckBox(groupBox, pChkEnableVertexPositionSet, cActionEnableVertexPositionSet::Ref::New(*this));
	
	
	helper.GroupBox(*this, groupBox, "Connections:");
	
	helper.Label(groupBox, "");
	helper.Label(groupBox, "Auto-Connected if empty", "Automatically creates connections by"
		" matching controller names if no custom connections are defined");
	
	helper.ComboBox(groupBox, "Target:", "Target controller", pCBConnection, cComboConnection::Ref::New(*this));
	
	helper.ComboBox(groupBox, "Controller:", "Controller to take value from",
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
		const int nameCount = subAnimator.GetControllerCount();
		const int count = rule->GetConnectionCount();
		decString text;
		int i;
		
		for(i=0; i<count; i++){
			if(i < nameCount){
				text.Format("%d: %s", i, subAnimator.GetControllerAt(i)->GetName().GetString());
				
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
	aeController * const selection = GetCBConnectionController();
	pPreventUpdate = true;
	
	try{
		pCBConnectionController->RemoveAllItems();
		pCBConnectionController->AddItem("< not assigned >", nullptr);
		
		if(GetAnimator()){
			const aeControllerList &list = GetAnimator()->GetControllers();
			const int count = list.GetCount();
			decString text;
			int i;
			
			for(i=0; i<count; i++){
				aeController * const controller = list.GetAt(i);
				text.Format("%d: %s", i, controller->GetName().GetString());
				pCBConnectionController->AddItem(text, nullptr, controller);
			}
		}
		
		pCBConnectionController->SetSelectionWithData(selection);
		pPreventUpdate = false;
		
	}catch(const deException &){
		pPreventUpdate = false;
		throw;
	}
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
		pCBConnectionController->SetSelectionWithData(rule->GetControllerAt(connection));
		
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
