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

#include "igdeWPTriggerTable.h"
#include "../igdeCommonDialogs.h"
#include "../igdeUIHelper.h"
#include "../igdeContainer.h"
#include "../igdeTextField.h"
#include "../igdeListBox.h"
#include "../event/igdeAction.h"
#include "../event/igdeListBoxListener.h"
#include "../event/igdeTextFieldListener.h"
#include "../layout/igdeContainerFlow.h"
#include "../model/igdeListItem.h"
#include "../menu/igdeMenuCascade.h"
#include "../resources/igdeIcon.h"
#include "../../environment/igdeEnvironment.h"
#include "../../gameproject/igdeGameProject.h"
#include "../../triggersystem/igdeTriggerTarget.h"
#include "../../triggersystem/igdeTriggerTargetList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Actions
////////////

namespace{

class cTextFilter : public igdeTextFieldListener{
	igdeWPTriggerTable &pPanel;
	
public:
	cTextFilter(igdeWPTriggerTable &panel) : pPanel(panel){}
	
	virtual void OnTextChanging(igdeTextField*){
		pPanel.UpdateTable();
	}
	
	virtual void OnTextChanged(igdeTextField*){
		pPanel.UpdateTable();
	}
};

class cListTable : public igdeListBoxListener{
	igdeWPTriggerTable &pPanel;
	
public:
	cListTable(igdeWPTriggerTable &panel) : pPanel(panel){}
	
	virtual void OnDoubleClickItem(igdeListBox*, int){
		pPanel.GetActionToggle()->OnAction();
	}
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		
		helper.MenuCommand(menu, pPanel.GetActionClear());
		helper.MenuSeparator(menu);
		
		helper.MenuCommand(menu, pPanel.GetActionAdd());
		helper.MenuCommand(menu, pPanel.GetActionRemove());
		helper.MenuSeparator(menu);
		
		helper.MenuCommand(menu, pPanel.GetActionFire());
		helper.MenuCommand(menu, pPanel.GetActionReset());
		helper.MenuCommand(menu, pPanel.GetActionFullReset());
	}
};


class cActionToggle : public igdeAction{
	igdeWPTriggerTable &pPanel;
	
public:
	typedef deTObjectReference<cActionToggle> Ref;
	
	cActionToggle(igdeWPTriggerTable &panel) :
	igdeAction("Toggle State", nullptr, "Toggle selected target between fired and reset state"),
	pPanel(panel){}
	
	void OnAction() override{
		igdeTriggerTarget * const target = pPanel.GetSelectedTarget();
		if(!target){
			return;
		}
		
		if(target->GetFired()){
			target->Reset();
			
		}else{
			target->Fire();
		}
		
		pPanel.OnAction();
	}
	
	void Update() override{
		SetEnabled(pPanel.GetSelectedTarget());
	}
};

class cActionClear : public igdeAction{
	igdeWPTriggerTable &pPanel;
	
public:
	typedef deTObjectReference<cActionClear> Ref;
	
	cActionClear(igdeWPTriggerTable &panel) :
	igdeAction("Clear Table", nullptr, "Remove all targets from the table"),
	pPanel(panel){}
	
	void OnAction() override{
		igdeTriggerTargetList * const list = pPanel.GetTriggerTargetList();
		if(!list){
			return;
		}
		
		list->RemoveAll();
		pPanel.OnAction();
	}
	
	void Update() override{
		SetEnabled(pPanel.GetTriggerTargetList() && pPanel.GetTriggerTargetList()->GetCount() > 0);
	}
};

class cActionAdd : public igdeAction{
	igdeWPTriggerTable &pPanel;
	
public:
	typedef deTObjectReference<cActionAdd> Ref;
	
	cActionAdd(igdeWPTriggerTable &panel) :
	igdeAction("Add Target...", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Add target to table"),
	pPanel(panel){}
	
	void OnAction() override{
		igdeTriggerTargetList * const list = pPanel.GetTriggerTargetList();
		if(!list){
			return;
		}
		
		decString name("Target");
		while(igdeCommonDialogs::GetString(&pPanel, "Add Target", "Name:", name)){
			if(list->HasNamed(name)){
				igdeCommonDialogs::Information(&pPanel, "Add Target", "A target with this name exists already.");
				continue;
			}
			
			const igdeTriggerTarget::Ref target(igdeTriggerTarget::Ref::New(name));
			list->Add(target);
			
			pPanel.SelectTarget(target);
			pPanel.OnAction();
			return;
		}
	}
	
	void Update() override{
		SetEnabled(pPanel.GetTriggerTargetList());
	}
};

class cActionRemove : public igdeAction{
	igdeWPTriggerTable &pPanel;
	
public:
	typedef deTObjectReference<cActionRemove> Ref;
	
	cActionRemove(igdeWPTriggerTable &panel) :
	igdeAction("Remove Target", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove selected target from table"),
	pPanel(panel){}
	
	void OnAction() override{
		igdeTriggerTargetList * const list = pPanel.GetTriggerTargetList();
		igdeTriggerTarget * const target = pPanel.GetSelectedTarget();
		if(!list || !target){
			return;
		}
		
		list->Remove(target);
		pPanel.OnAction();
	}
	
	void Update() override{
		SetEnabled(pPanel.GetSelectedTarget());
	}
};

class cActionFire : public igdeAction{
	igdeWPTriggerTable &pPanel;
	
public:
	typedef deTObjectReference<cActionFire> Ref;
	
	cActionFire(igdeWPTriggerTable &panel) :
	igdeAction("Fire Target", nullptr, "Fire selected target"),
	pPanel(panel){}
	
	void OnAction() override{
		igdeTriggerTarget * const target = pPanel.GetSelectedTarget();
		if(!target){
			return;
		}
		
		target->Fire();
		pPanel.OnAction();
	}
	
	void Update() override{
		SetEnabled(pPanel.GetSelectedTarget() && !pPanel.GetSelectedTarget()->GetFired());
	}
};

class cActionReset : public igdeAction{
	igdeWPTriggerTable &pPanel;
	
public:
	typedef deTObjectReference<cActionReset> Ref;
	
	cActionReset(igdeWPTriggerTable &panel) :
	igdeAction("Reset Target", nullptr, "Reset selected target"),
	pPanel(panel){}
	
	void OnAction() override{
		igdeTriggerTarget * const target = pPanel.GetSelectedTarget();
		if(!target){
			return;
		}
		
		target->Reset();
		pPanel.OnAction();
	}
	
	void Update() override{
		SetEnabled(pPanel.GetSelectedTarget() && pPanel.GetSelectedTarget()->GetFired());
	}
};

class cActionFullReset : public igdeAction{
	igdeWPTriggerTable &pPanel;
	
public:
	typedef deTObjectReference<cActionFullReset> Ref;
	
	cActionFullReset(igdeWPTriggerTable &panel) :
	igdeAction("Full Reset Target", nullptr, "Full reset selected target"),
	pPanel(panel){}
	
	void OnAction() override{
		igdeTriggerTarget * const target = pPanel.GetSelectedTarget();
		if(!target){
			return;
		}
		
		target->FullReset();
		pPanel.OnAction();
	}
	
	void Update() override{
		SetEnabled(pPanel.GetSelectedTarget() && (pPanel.GetSelectedTarget()->GetFired()
			|| pPanel.GetSelectedTarget()->GetHasFired()));
	}
};

}



// Class igdeWPTriggerTable
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWPTriggerTable::igdeWPTriggerTable(igdeEnvironment &environment) :
igdeContainerFlow(environment, eaY),
pTriggerTargetList(nullptr)
{
	pCreateContent();
}

igdeWPTriggerTable::igdeWPTriggerTable(igdeEnvironment &environment, igdeAction *action) :
igdeContainerFlow(environment, eaY),
pTriggerTargetList(nullptr)
{
	pCreateContent();
	SetAction(action);
}

igdeWPTriggerTable::~igdeWPTriggerTable(){
	DestroyNativeWidget();
	SetAction(nullptr);
}



// Management
///////////////

void igdeWPTriggerTable::SetTriggerTargetList(igdeTriggerTargetList *triggerTargetList){
	if(triggerTargetList == pTriggerTargetList){
		return;
	}
	
	pTriggerTargetList = triggerTargetList;
	UpdateTable();
}

igdeTriggerTarget *igdeWPTriggerTable::GetSelectedTarget() const{
	const igdeListItem * const selection = pListTriggerTable->GetSelectedItem();
	return selection ? (igdeTriggerTarget*)selection->GetData() : nullptr;
}

void igdeWPTriggerTable::SelectTarget(igdeTriggerTarget *target){
	pListTriggerTable->SetSelectionWithData(target);
}

void igdeWPTriggerTable::UpdateTable(){
	const decString filter(pEditTriggerTableFilter->GetText().GetLower());
	igdeTriggerTarget *selectedTarget = GetSelectedTarget();
	const bool hasFilter = !filter.IsEmpty();
	
	pListTriggerTable->RemoveAllItems();
	
	if(pTriggerTargetList){
		const int count = pTriggerTargetList->GetCount();
		decString text;
		int i;
		
		for(i=0; i<count; i++){
			igdeTriggerTarget * const target = pTriggerTargetList->GetAt(i);
			
			if(hasFilter && target->GetName().GetLower().FindString(filter) == -1){
				continue;
			}
			
			text.Format("%s (%s)", target->GetName().GetString(),
				target->GetHasFired() ? "has fired" : "never fired");
			pListTriggerTable->AddItem(text, target->GetFired() ? pIconFired : pIconNotFired, target);
		}
	}
	
	pListTriggerTable->SortItems();
	
	pListTriggerTable->SetSelectionWithData(selectedTarget);
	if(!pListTriggerTable->GetSelectedItem() && pListTriggerTable->GetItemCount() > 0){
		pListTriggerTable->SetSelection(0);
	}
}



void igdeWPTriggerTable::SetAction(igdeAction *action){
	if(pAction == action){
		return;
	}
	
	if(pAction){
		pAction->RemoveListener(this);
	}
	
	pAction = action;
	
	if(action){
		action->AddListener(this);
		OnParameterChanged(action);
	}
}

void igdeWPTriggerTable::OnAction(){
	if(pAction){
		pAction->OnAction();
	}
}

void igdeWPTriggerTable::OnParameterChanged(igdeAction *action){
	//SetEnabled( action->GetEnabled() );
}

void igdeWPTriggerTable::OnDestroyed(igdeAction *action){
	GetLogger()->LogWarnFormat("IGDE", "igdeWPTriggerTable::OnDestroyed: "
		"Action(%s) destroyed while still listening on it", action->GetText().GetString());
	pAction = nullptr;
}



// Private Functions
//////////////////////

void igdeWPTriggerTable::pCreateContent(){
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pIconFired = env.GetStockIcon(igdeEnvironment::esiSmallPlus);
	pIconNotFired = env.GetStockIcon(igdeEnvironment::esiSmallMinus);
	
	pActionToggle = cActionToggle::Ref::New(*this);
	pActionClear = cActionClear::Ref::New(*this);
	pActionAdd = cActionAdd::Ref::New(*this);
	pActionRemove = cActionRemove::Ref::New(*this);
	pActionFire = cActionFire::Ref::New(*this);
	pActionReset = cActionReset::Ref::New(*this);
	pActionFullReset = cActionFullReset::Ref::New(*this);
	
	helper.ListBox(*this, 8, "List of targets in the table", pListTriggerTable, new cListTable(*this));
	pListTriggerTable->SetDefaultSorter();
	
	igdeContainerFlow::Ref frameLine(igdeContainerFlow::Ref::New(env, eaX, esLast));
	helper.Label(frameLine, "Filter:");
	helper.EditString(frameLine, "Filter targets in the list case insensitive",
		pEditTriggerTableFilter, new cTextFilter(*this));
	AddChild(frameLine);
}
