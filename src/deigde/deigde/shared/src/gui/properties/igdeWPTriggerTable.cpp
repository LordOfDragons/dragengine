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
	typedef deTObjectReference<cTextFilter> Ref;
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
	typedef deTObjectReference<cListTable> Ref;
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
	igdeAction("@Igde.WPTriggerTable.Action.ToggleState", nullptr, "@Igde.WPTriggerTable.Action.ToggleState.ToolTip"),
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
		
		pPanel.UpdateTarget(target);
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
	igdeAction("@Igde.WPTriggerTable.Action.ClearTable", nullptr, "@Igde.WPTriggerTable.Action.ClearTable.ToolTip"),
	pPanel(panel){}
	
	void OnAction() override{
		igdeTriggerTargetList * const list = pPanel.GetTriggerTargetList();
		if(!list){
			return;
		}
		
		list->RemoveAll();
		pPanel.UpdateTable();
		pPanel.OnAction();
	}
	
	void Update() override{
		SetEnabled(pPanel.GetTriggerTargetList()
			&& pPanel.GetTriggerTargetList()->GetTargets().IsNotEmpty());
	}
};

class cActionAdd : public igdeAction{
	igdeWPTriggerTable &pPanel;
	
public:
	typedef deTObjectReference<cActionAdd> Ref;
	
	cActionAdd(igdeWPTriggerTable &panel) :
	igdeAction("@Igde.WPTriggerTable.Action.AddTarget", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"@Igde.WPTriggerTable.Action.AddTarget.ToolTip"),
	pPanel(panel){}
	
	void OnAction() override{
		igdeTriggerTargetList * const list = pPanel.GetTriggerTargetList();
		if(!list){
			return;
		}
		
		decString name(pPanel.GetSelectedTarget()
			? pPanel.GetSelectedTarget()->GetName().GetString() : "Target");
		
		while(igdeCommonDialogs::GetString(pPanel, "@Igde.WPTriggerTable.Dialog.AddTarget.Title", "@Igde.WPTriggerTable.Dialog.AddTarget.Name", name)){
			if(list->GetTargets().HasNamed(name)){
				igdeCommonDialogs::Information(pPanel, "@Igde.WPTriggerTable.Dialog.AddTarget.Title",
					"A target with this name exists already.");
				continue;
			}
			
			const igdeTriggerTarget::Ref target(igdeTriggerTarget::Ref::New(name));
			list->Add(target);
			
			pPanel.UpdateTable();
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
	igdeAction("@Igde.WPTriggerTable.Action.RemoveTarget", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"@Igde.WPTriggerTable.Action.RemoveTarget.ToolTip"),
	pPanel(panel){}
	
	void OnAction() override{
		igdeTriggerTargetList * const list = pPanel.GetTriggerTargetList();
		igdeTriggerTarget * const target = pPanel.GetSelectedTarget();
		if(!list || !target){
			return;
		}
		
		list->Remove(target);
		pPanel.UpdateTable();
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
	igdeAction("@Igde.WPTriggerTable.Action.FireTarget", nullptr, "@Igde.WPTriggerTable.Action.FireTarget.ToolTip"),
	pPanel(panel){}
	
	void OnAction() override{
		igdeTriggerTarget * const target = pPanel.GetSelectedTarget();
		if(!target){
			return;
		}
		
		target->Fire();
		pPanel.UpdateTarget(target);
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
	igdeAction("@Igde.WPTriggerTable.Action.ResetTarget", nullptr, "@Igde.WPTriggerTable.Action.ResetTarget.ToolTip"),
	pPanel(panel){}
	
	void OnAction() override{
		igdeTriggerTarget * const target = pPanel.GetSelectedTarget();
		if(!target){
			return;
		}
		
		target->Reset();
		pPanel.UpdateTarget(target);
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
	igdeAction("@Igde.WPTriggerTable.Action.FullResetTarget", nullptr, "@Igde.WPTriggerTable.Action.FullResetTarget.ToolTip"),
	pPanel(panel){}
	
	void OnAction() override{
		igdeTriggerTarget * const target = pPanel.GetSelectedTarget();
		if(!target){
			return;
		}
		
		target->FullReset();
		pPanel.UpdateTarget(target);
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
	pListTriggerTable->UpdateRestoreSelection([&](){
		const decString filter(pEditTriggerTableFilter->GetText().GetLower());
		const bool hasFilter = !filter.IsEmpty();
		
		pListTriggerTable->RemoveAllItems();
		
		if(pTriggerTargetList){
			pTriggerTargetList->GetTargets().Visit([&](igdeTriggerTarget *t){
				if(hasFilter && t->GetName().GetLower().FindString(filter) == -1){
					return;
				}
				
				decString text;
				text.FormatSafe(Translate("Igde.WPTriggerTable.Target.Text").ToUTF8(), t->GetName(),
					Translate(t->GetFired() ? "Igde.WPTriggerTable.Target.HasFired" : "Igde.WPTriggerTable.Target.NeverFired").ToUTF8());
				pListTriggerTable->AddItem(text, t->GetFired() ? pIconFired : pIconNotFired, t);
			});
			
			pListTriggerTable->SortItems();
		}
	});
}

void igdeWPTriggerTable::UpdateTarget(igdeTriggerTarget *target){
	int index = pListTriggerTable->IndexOfItemWithData(target);
	if(index == -1){
		return;
	}
	
	decString text;
	text.FormatSafe(Translate("Igde.WPTriggerTable.Target.Text").ToUTF8(), target->GetName(),
		Translate(target->GetHasFired() ? "Igde.WPTriggerTable.Target.HasFired" : "Igde.WPTriggerTable.Target.NeverFired").ToUTF8());
	
	igdeListItem &item = pListTriggerTable->GetItems().GetAt(index);
	if(item.GetText() == text){
		return;
	}
	
	item.SetText(text);
	pListTriggerTable->ItemChangedAt(index);
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
	
	helper.ListBox(*this, 8, "@Igde.WPTriggerTable.ListBox.ToolTip", pListTriggerTable, cListTable::Ref::New(*this));
	pListTriggerTable->SetDefaultSorter();
	
	igdeContainerFlow::Ref frameLine(igdeContainerFlow::Ref::New(env, eaX, esLast));
	helper.Label(frameLine, "@Igde.WPTriggerTable.Filter");
	helper.EditString(frameLine, "@Igde.WPTriggerTable.Filter.ToolTip",
		pEditTriggerTableFilter, cTextFilter::Ref::New(*this));
	AddChild(frameLine);
}
