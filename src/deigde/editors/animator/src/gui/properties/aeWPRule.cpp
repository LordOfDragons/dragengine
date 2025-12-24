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
#include <string.h>

#include "aeWPRule.h"
#include "aeWPRuleListener.h"
#include "aeWindowProperties.h"
#include "panels/aeWPAPanelRule.h"
#include "panels/aeWPAPanelRuleAnimation.h"
#include "panels/aeWPAPanelRuleAnimationDifference.h"
#include "panels/aeWPAPanelRuleAnimationSelect.h"
#include "panels/aeWPAPanelRuleBoneTransformator.h"
#include "panels/aeWPAPanelRuleStateManipulator.h"
#include "panels/aeWPAPanelRuleStateSnapshot.h"
#include "panels/aeWPAPanelRuleInverseKinematic.h"
#include "panels/aeWPAPanelRuleForeignState.h"
#include "panels/aeWPAPanelRuleSubAnimator.h"
#include "panels/aeWPAPanelRuleTrackTo.h"
#include "panels/aeWPAPanelRuleLimit.h"
#include "panels/aeWPAPanelRuleGroup.h"
#include "panels/aeWPAPanelRuleMirror.h"
#include "../aeWindowMain.h"
#include "../../animator/aeAnimator.h"
#include "../../animator/rule/aeRule.h"
#include "../../animator/rule/aeRuleGroup.h"
#include "../../clipboard/aeClipboardDataRule.h"
#include "../../undosys/rule/aeUPasteRule.h"
#include "../../undosys/rule/aeURemoveRule.h"
#include "../../undosys/rule/group/aeURuleGroupPasteRule.h"
#include "../../undosys/rule/group/aeURuleGroupRemoveRule.h"

#include <deigde/clipboard/igdeClipboardData.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeTreeListListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeTreeItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/common/exceptions.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	aeWPRule &pPanel;
	
public:
	typedef deTObjectReference<cBaseAction> Ref;
	cBaseAction(aeWPRule &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRule * const rule = pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(animator, rule));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(aeAnimator *animator, aeRule *rule) = 0;
	
	void Update() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRule * const rule = pPanel.GetRule();
		if(animator && rule){
			Update(*animator, *rule);
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const aeAnimator &, const aeRule &){
		SetEnabled(true);
		SetSelected(false);
	}
};


class cActionCopy : public cBaseAction{
public:
	typedef deTObjectReference<cActionCopy> Ref;
	
public:
	cActionCopy(aeWPRule &panel) : cBaseAction(panel, "Copy",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
		"Copy rule to clipboard"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRule *rule) override{
		pPanel.GetWindowProperties().GetWindowMain().GetClipboard().Set(
			aeClipboardDataRule::Ref::New(rule));
		return {};
	}
};

class cActionCut : public cBaseAction{
public:
	typedef deTObjectReference<cActionCut> Ref;
	
public:
	cActionCut(aeWPRule &panel) : cBaseAction(panel, "Cut",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCut),
		"Cut rule into clipboard"){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator, aeRule *rule) override{
		pPanel.GetWindowProperties().GetWindowMain().GetClipboard().Set(
			aeClipboardDataRule::Ref::New(rule));
		
		if(rule->GetParentGroup()){
			return aeURuleGroupRemoveRule::Ref::New(rule->GetParentGroup(), rule);
			
		}else{
			return aeURemoveRule::Ref::New(animator, rule);
		}
	}
};

class cActionPaste : public cBaseAction{
public:
	typedef deTObjectReference<cActionPaste> Ref;
	
private:
protected:
	const bool pInsert;
	
public:
	cActionPaste(aeWPRule &panel, const char *name, igdeIcon *icon, const char *description, bool insert) :
		cBaseAction(panel, name, icon, description), pInsert(insert){}
	
	igdeUndo::Ref OnAction(aeAnimator *animator, aeRule *rule) override{
		aeClipboardDataRule * const cdata = (aeClipboardDataRule*)pPanel.GetWindowProperties()
			.GetWindowMain().GetClipboard().GetWithTypeName(aeClipboardDataRule::TYPE_NAME);
		if(!cdata){
			return {};
		}
		
		if(rule->GetParentGroup()){
			const aeRule::List &list = rule->GetParentGroup()->GetRules();
			return aeURuleGroupPasteRule::Ref::New(rule->GetParentGroup(), cdata->GetRules(),
				pInsert ? list.IndexOf(rule) : list.GetCount());
			
		}else{
			const aeRule::List &list = animator->GetRules();
			return aeUPasteRule::Ref::New(animator, cdata->GetRules(),
				pInsert ? list.IndexOf(rule) : list.GetCount());
		}
	}
	
	void Update(const aeAnimator &, const aeRule &) override{
		SetEnabled(pPanel.GetWindowProperties().GetWindowMain().GetClipboard()
			.GetWithTypeName(aeClipboardDataRule::TYPE_NAME));
	}
};

class cActionPasteAppend : public cActionPaste{
public:
	typedef deTObjectReference<cActionPasteAppend> Ref;
	cActionPasteAppend(aeWPRule &panel) : cActionPaste(panel, "Paste Append",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"Paste and append rule from clipboard", false){}
};

class cActionPasteInsert : public cActionPaste{
public:
	typedef deTObjectReference<cActionPasteInsert> Ref;
	cActionPasteInsert(aeWPRule &panel) : cActionPaste(panel, "Paste Insert",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"Paste and insert rule from clipboard ", true){}
};

class cActionPasteIntoGroup : public cActionPaste{
public:
	typedef deTObjectReference<cActionPasteIntoGroup> Ref;
	cActionPasteIntoGroup(aeWPRule &panel) : cActionPaste(panel, "Paste Into Group",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPaste),
		"Paste rules from clipboard into group", false){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRule *rule) override{
		aeClipboardDataRule * const cdata = (aeClipboardDataRule*)pPanel.GetWindowProperties()
			.GetWindowMain().GetClipboard().GetWithTypeName(aeClipboardDataRule::TYPE_NAME);
		if(!cdata || rule->GetType() != deAnimatorRuleVisitorIdentify::ertGroup){
			return {};
		}
		
		aeRuleGroup * const group = (aeRuleGroup*)rule;
		const aeRule::List &list = group->GetRules();
		return aeURuleGroupPasteRule::Ref::New(group, cdata->GetRules(), list.GetCount());
	}
	
	void Update(const aeAnimator & , const aeRule &rule) override{
		SetEnabled(rule.GetType() == deAnimatorRuleVisitorIdentify::ertGroup
			&& pPanel.GetWindowProperties().GetWindowMain().GetClipboard()
				.GetWithTypeName(aeClipboardDataRule::TYPE_NAME));
	}
};

class cTreeRules : public igdeTreeListListener{
	aeWPRule &pPanel;
	
public:
	typedef deTObjectReference<cTreeRules> Ref;
	cTreeRules(aeWPRule &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeTreeList *treeList){
		aeAnimator * const animator = pPanel.GetAnimator();
		if(animator){
			animator->SetActiveRule(treeList->GetSelection()
				? (aeRule*)treeList->GetSelection()->GetData() : nullptr);
		}
	}
	
	virtual void OnItemExpanded(igdeTreeList*, igdeTreeItem *item){
		aeRule * const rule = (aeRule*)item->GetData();
		if(rule && rule->GetType() == deAnimatorRuleVisitorIdentify::ertGroup){
			((aeRuleGroup*)rule)->SetTreeListExpanded(true);
		}
	}
	
	virtual void OnItemCollapsed(igdeTreeList*, igdeTreeItem *item){
		aeRule * const rule = (aeRule*)item->GetData();
		if(rule && rule->GetType() == deAnimatorRuleVisitorIdentify::ertGroup){
			((aeRuleGroup*)rule)->SetTreeListExpanded(false);
		}
	}
	
	virtual void AddContextMenuEntries(igdeTreeList*, igdeMenuCascade &menu){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		
		const aeWindowMain &windowMain = pPanel.GetWindowProperties().GetWindowMain();
		igdeMenuCascade::Ref submenu(igdeMenuCascade::Ref::New(menu.GetEnvironment(), "Add"));
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddAnim());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddAnimDiff());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddAnimSelect());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddBoneRot());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddInvKin());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddStateManip());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddStateSnap());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddForeignState());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddGroup());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddSubAnimator());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddTrackTo());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddLimit());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddMirror());
		menu.AddChild(submenu);
		
		submenu = igdeMenuCascade::Ref::New(menu.GetEnvironment(), "Add Into Group");
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddIntoGroupAnim());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddIntoGroupAnimDiff());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddIntoGroupAnimSelect());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddIntoGroupBoneRot());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddIntoGroupInvKin());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddIntoGroupStateManip());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddIntoGroupStateSnap());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddIntoGroupForeignState());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddIntoGroupGroup());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddIntoGroupSubAnimator());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddIntoGroupTrackTo());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddIntoGroupLimit());
		helper.MenuCommand(submenu, windowMain.GetActionRuleAddIntoGroupMirror());
		menu.AddChild(submenu);
		
		submenu = igdeMenuCascade::Ref::New(menu.GetEnvironment(), "Insert");
		helper.MenuCommand(submenu, windowMain.GetActionRuleInsertAnim());
		helper.MenuCommand(submenu, windowMain.GetActionRuleInsertAnimDiff());
		helper.MenuCommand(submenu, windowMain.GetActionRuleInsertAnimSelect());
		helper.MenuCommand(submenu, windowMain.GetActionRuleInsertBoneRot());
		helper.MenuCommand(submenu, windowMain.GetActionRuleInsertInvKin());
		helper.MenuCommand(submenu, windowMain.GetActionRuleInsertStateManip());
		helper.MenuCommand(submenu, windowMain.GetActionRuleInsertStateSnap());
		helper.MenuCommand(submenu, windowMain.GetActionRuleInsertForeignState());
		helper.MenuCommand(submenu, windowMain.GetActionRuleInsertGroup());
		helper.MenuCommand(submenu, windowMain.GetActionRuleInsertSubAnimator());
		helper.MenuCommand(submenu, windowMain.GetActionRuleInsertTrackTo());
		helper.MenuCommand(submenu, windowMain.GetActionRuleInsertLimit());
		helper.MenuCommand(submenu, windowMain.GetActionRuleInsertMirror());
		menu.AddChild(submenu);
		
		helper.MenuCommand(menu, windowMain.GetActionRuleRemove());
		helper.MenuCommand(menu, windowMain.GetActionRuleUp());
		helper.MenuCommand(menu, windowMain.GetActionRuleDown());
		
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, cActionCopy::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionCut::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionPasteAppend::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionPasteInsert::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionPasteIntoGroup::Ref::New(pPanel));
	}
};

}



// Class aeWPRule
////////////////////

// Constructor, destructor
////////////////////////////

aeWPRule::aeWPRule(aeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties),
pPanelAnim(nullptr),
pPanelAnimDiff(nullptr),
pPanelAnimSelect(nullptr),
pPanelBoneRot(nullptr),
pPanelFState(nullptr),
pPanelGroup(nullptr),
pPanelIK(nullptr),
pPanelSManip(nullptr),
pPanelSSnapshot(nullptr),
pPanelSubAnimator(nullptr),
pPanelTrackTo(nullptr),
pPanelLimit(nullptr),
pPanelMirror(nullptr),
pActivePanel(nullptr)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, formLine;
	
	pListener = aeWPRuleListener::Ref::New(*this);
	
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	
	helper.GroupBoxFlow(content, groupBox, "Rules:");
	helper.TreeList(groupBox, pTreeRule, 8, "Rules", cTreeRules::Ref::New(*this));
	
	
	pSwitcher = igdeSwitcher::Ref::New(env);
	content->AddChild(pSwitcher);
	
	pSwitcher->AddChild(igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY));
	
	pPanelAnim = aeWPAPanelRuleAnimation::Ref::New(*this);
	pSwitcher->AddChild(pPanelAnim);
	
	pPanelAnimDiff = aeWPAPanelRuleAnimationDifference::Ref::New(*this);
	pSwitcher->AddChild(pPanelAnimDiff);
	
	pPanelAnimSelect = aeWPAPanelRuleAnimationSelect::Ref::New(*this);
	pSwitcher->AddChild(pPanelAnimSelect);
	
	pPanelBoneRot = aeWPAPanelRuleBoneTransformator::Ref::New(*this);
	pSwitcher->AddChild(pPanelBoneRot);
	
	pPanelFState = aeWPAPanelRuleForeignState::Ref::New(*this);
	pSwitcher->AddChild(pPanelFState);
	
	pPanelGroup = aeWPAPanelRuleGroup::Ref::New(*this);
	pSwitcher->AddChild(pPanelGroup);
	
	pPanelIK = aeWPAPanelRuleInverseKinematic::Ref::New(*this);
	pSwitcher->AddChild(pPanelIK);
	
	pPanelSManip = aeWPAPanelRuleStateManipulator::Ref::New(*this);
	pSwitcher->AddChild(pPanelSManip);
	
	pPanelSSnapshot = aeWPAPanelRuleStateSnapshot::Ref::New(*this);
	pSwitcher->AddChild(pPanelSSnapshot);
	
	pPanelSubAnimator = aeWPAPanelRuleSubAnimator::Ref::New(*this);
	pSwitcher->AddChild(pPanelSubAnimator);
	
	pPanelTrackTo = aeWPAPanelRuleTrackTo::Ref::New(*this);
	pSwitcher->AddChild(pPanelTrackTo);
	
	pPanelLimit = aeWPAPanelRuleLimit::Ref::New(*this);
	pSwitcher->AddChild(pPanelLimit);
	
	pPanelMirror = aeWPAPanelRuleMirror::Ref::New(*this);
	pSwitcher->AddChild(pPanelMirror);
	
	pSwitcher->SetCurrent(0); // empty
}

aeWPRule::~aeWPRule(){
	SetAnimator(nullptr);
}



// Management
///////////////

void aeWPRule::SetAnimator(aeAnimator *animator){
	if(animator == pAnimator){
		return;
	}
	
	if(pAnimator){
		pAnimator->RemoveNotifier(pListener);
	}
	
	pAnimator = animator;
	
	if(animator){
		animator->AddNotifier(pListener);
	}
	
	if(pActivePanel){
		pActivePanel->OnAnimatorChanged();
		pActivePanel->OnAnimatorPathChanged();
	}
	
	UpdateRuleTree();
	UpdateRule();
	UpdateRuleMoveList();
	UpdateRuleBoneList();
	UpdateRuleVertexPositionSetList();
	UpdateLinkList();
	UpdateControllerList();
}

aeRule *aeWPRule::GetRule() const{
	return pAnimator ? pAnimator->GetActiveRule() : nullptr;
}



void aeWPRule::UpdateControllerList(){
	if(pActivePanel){
		pActivePanel->UpdateControllerList();
	}
}

void aeWPRule::UpdateLinkList(){
	if(pActivePanel){
		pActivePanel->UpdateLinkList();
	}
}

void aeWPRule::UpdateRuleTree(){
	if(pAnimator){
		igdeTreeItem *nextItem = pTreeRule->GetFirstChild();
		
		pAnimator->GetRules().Visit([&](aeRule *rule){
			if(!nextItem){
				igdeTreeItem::Ref newItem(igdeTreeItem::Ref::New(""));
				pTreeRule->AppendItem(nullptr, newItem);
				nextItem = newItem;
			}
			
			UpdateRuleTreeItem(nextItem, rule);
			
			nextItem = nextItem->GetNext();
		});
		
		while(nextItem){
			igdeTreeItem * const removeItem = nextItem;
			nextItem = nextItem->GetNext();
			pTreeRule->RemoveItem(removeItem);
		}
		
	}else{
		pTreeRule->RemoveAllItems();
	}
	
	SelectActiveRule();
	if(!pTreeRule->GetSelection() && pTreeRule->GetFirstChild()){
		pTreeRule->SetSelection(pTreeRule->GetFirstChild());
	}
}

void aeWPRule::UpdateRuleTreeItem(igdeTreeItem *item, aeRule *rule){
	if(!item || !rule){
		DETHROW(deeInvalidParam);
	}
	
	item->SetData(rule);
	
	int order = 0;
	const igdeTreeItem *findOrderItem = item->GetPrevious();
	while(findOrderItem){
		order++;
		findOrderItem = findOrderItem->GetPrevious();
	}
	
	decString text;
	text.Format("%i: %s", order, rule->GetName().GetString());
	item->SetText(text);
	item->SetIcon(pWindowProperties.GetWindowMain().GetRuleIcon(rule->GetType()));
	pTreeRule->ItemChanged(item);
	
	if(rule->GetType() == deAnimatorRuleVisitorIdentify::ertGroup){
		const aeRuleGroup &ruleGroup = *((aeRuleGroup*)rule);
		igdeTreeItem *nextItem = item->GetFirstChild();
		
		if(item->GetExpanded() != ruleGroup.GetTreeListExpanded()){
			item->SetExpanded(ruleGroup.GetTreeListExpanded());
			pTreeRule->ItemChanged(item);
		}
		
		ruleGroup.GetRules().Visit([&](aeRule *rule2){
			if(!nextItem){
				igdeTreeItem::Ref newItem(igdeTreeItem::Ref::New(""));
				pTreeRule->AppendItem(item, newItem);
				nextItem = newItem;
			}
			
			UpdateRuleTreeItem(nextItem, rule2);
			
			nextItem = nextItem->GetNext();
		});
		
		while(nextItem){
			igdeTreeItem * const removeItem = nextItem;
			nextItem = nextItem->GetNext();
			pTreeRule->RemoveItem(removeItem);
		}
		
	}else{
		pTreeRule->RemoveAllItems(item);
	}
}

void aeWPRule::SelectActiveRule(){
	pTreeRule->SetSelectionWithData(GetRule());
}

void aeWPRule::ShowActiveSourcePanel(){
	const aeRule * const rule = GetRule();
	if(!rule){
		pSwitcher->SetCurrent(0);  // empty
		return;
	}
	
	const deAnimatorRuleVisitorIdentify::eRuleTypes type = rule->GetType();
	
	if(type == pPanelAnim->GetRequiredType()){
		pSwitcher->SetCurrent(1);
		pActivePanel = pPanelAnim;
		
	}else if(type == pPanelAnimDiff->GetRequiredType()){
		pSwitcher->SetCurrent(2);
		pActivePanel = pPanelAnimDiff;
		
	}else if(type == pPanelAnimSelect->GetRequiredType()){
		pSwitcher->SetCurrent(3);
		pActivePanel = pPanelAnimSelect;
		
	}else if(type == pPanelBoneRot->GetRequiredType()){
		pSwitcher->SetCurrent(4);
		pActivePanel = pPanelBoneRot;
		
	}else if(type == pPanelFState->GetRequiredType()){
		pSwitcher->SetCurrent(5);
		pActivePanel = pPanelFState;
		
	}else if(type == pPanelGroup->GetRequiredType()){
		pSwitcher->SetCurrent(6);
		pActivePanel = pPanelGroup;
		
	}else if(type == pPanelIK->GetRequiredType()){
		pSwitcher->SetCurrent(7);
		pActivePanel = pPanelIK;
		
	}else if(type == pPanelSManip->GetRequiredType()){
		pSwitcher->SetCurrent(8);
		pActivePanel = pPanelSManip;
		
	}else if(type == pPanelSSnapshot->GetRequiredType()){
		pSwitcher->SetCurrent(9);
		pActivePanel = pPanelSSnapshot;
		
	}else if(type == pPanelSubAnimator->GetRequiredType()){
		pSwitcher->SetCurrent(10);
		pActivePanel = pPanelSubAnimator;
		
	}else if(type == pPanelTrackTo->GetRequiredType()){
		pSwitcher->SetCurrent(11);
		pActivePanel = pPanelTrackTo;
		
	}else if(type == pPanelLimit->GetRequiredType()){
		pSwitcher->SetCurrent(12);
		pActivePanel = pPanelLimit;
		
	}else if(type == pPanelMirror->GetRequiredType()){
		pSwitcher->SetCurrent(13);
		pActivePanel = pPanelMirror;
		
	}else{
		pSwitcher->SetCurrent(0);  // empty
	}
	
	if(pActivePanel){
		pActivePanel->OnActivated();
	}
}

void aeWPRule::UpdateRule(){
	if(!pActivePanel){
		return;
	}
	
	aeRule * const rule = GetRule();
	igdeTreeItem * const item = pTreeRule->GetItemWithData(rule);
	if(item){
		UpdateRuleTreeItem(item, rule);
	}
	
	pActivePanel->UpdateRule();
}

void aeWPRule::UpdateRuleBoneList(){
	if(pActivePanel){
		pActivePanel->UpdateRigBoneList();
	}
}

void aeWPRule::UpdateRuleVertexPositionSetList(){
	if(pActivePanel){
		pActivePanel->UpdateModelVertexPositionSetList();
	}
}

void aeWPRule::UpdateRuleMoveList(){
	if(pActivePanel){
		pActivePanel->UpdateAnimMoveList();
	}
}

void aeWPRule::OnAnimatorPathChanged(){
	if(pActivePanel){
		pActivePanel->OnAnimatorPathChanged();
	}
}
