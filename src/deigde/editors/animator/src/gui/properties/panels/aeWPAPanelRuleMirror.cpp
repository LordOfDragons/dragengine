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

#include "aeWPAPanelRuleMirror.h"
#include "../aeWPRule.h"
#include "../aeWindowProperties.h"
#include "../../aeWindowMain.h"
#include "../../dialog/aeDialogMirrorMatchName.h"
#include "../../../animator/aeAnimator.h"
#include "../../../animator/controller/aeController.h"
#include "../../../animator/rule/aeRuleMirror.h"
#include "../../../undosys/rule/mirror/aeURuleMirrorAddMatchName.h"
#include "../../../undosys/rule/mirror/aeURuleMirrorRemoveAllMatchNames.h"
#include "../../../undosys/rule/mirror/aeURuleMirrorRemoveMatchName.h"
#include "../../../undosys/rule/mirror/aeURuleMirrorSetAxis.h"
#include "../../../undosys/rule/mirror/aeURuleMirrorSetEnablePosition.h"
#include "../../../undosys/rule/mirror/aeURuleMirrorSetEnableRotation.h"
#include "../../../undosys/rule/mirror/aeURuleMirrorSetEnableSize.h"
#include "../../../undosys/rule/mirror/aeURuleMirrorSetEnableVertexPositionSet.h"
#include "../../../undosys/rule/mirror/aeURuleMirrorSetMatchName.h"
#include "../../../undosys/rule/mirror/aeURuleMirrorSetMirrorBone.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/dialog/igdeDialog.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	aeWPAPanelRuleMirror &pPanel;
	
public:
	using Ref = deTObjectReference<cBaseAction>;
	cBaseAction(aeWPAPanelRuleMirror &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleMirror * const rule = (aeRuleMirror*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(animator, rule));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(aeAnimator *animator, aeRuleMirror *rule) = 0;
	
	void Update() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleMirror * const rule = (aeRuleMirror*)pPanel.GetRule();
		if(animator && rule){
			Update(*animator, *rule);
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const aeAnimator &, const aeRuleMirror &){
		SetEnabled(true);
		SetSelected(false);
	}
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	aeWPAPanelRuleMirror &pPanel;
	
public:
	using Ref = deTObjectReference<cBaseComboBoxListener>;
	cBaseComboBoxListener(aeWPAPanelRuleMirror &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRuleMirror * const rule = (aeRuleMirror*)pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(comboBox, animator, rule));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox *comboBox, aeAnimator *animator, aeRuleMirror *rule) = 0;
};


class cComboMirrorAxis : public cBaseComboBoxListener{
public:
	using Ref = deTObjectReference<cComboMirrorAxis>;
	cComboMirrorAxis(aeWPAPanelRuleMirror &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, aeAnimator*, aeRuleMirror *rule) override{
		if(!comboBox->GetSelectedItem()){
			return {};
		}
		
		deAnimatorRuleMirror::eMirrorAxis value = (deAnimatorRuleMirror::eMirrorAxis)
			(intptr_t)comboBox->GetSelectedItem()->GetData();
		return rule->GetMirrorAxis() != value ? aeURuleMirrorSetAxis::Ref::New(rule, value) : aeURuleMirrorSetAxis::Ref();
	}
};

class cComboMirrorBone : public cBaseComboBoxListener{
public:
	using Ref = deTObjectReference<cComboMirrorBone>;
	cComboMirrorBone(aeWPAPanelRuleMirror &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, aeAnimator*, aeRuleMirror *rule) override{
		return rule->GetMirrorBone() != comboBox->GetText()
			? aeURuleMirrorSetMirrorBone::Ref::New(rule, comboBox->GetText()) : igdeUndo::Ref();
	}
};


class cActionMatchNameAdd : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionMatchNameAdd>;
	
public:
	cActionMatchNameAdd(aeWPAPanelRuleMirror &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleMirror.ActionMatchNameAdd.Label",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"@Animator.WPAPanelRuleMirror.ActionMatchNameAdd.ToolTip"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleMirror *rule) override{
		aeDialogMirrorMatchName::Ref dialog(aeDialogMirrorMatchName::Ref::New(
			pPanel.GetEnvironment(), "Add match name"));
		return dialog->Run(&pPanel) ? aeURuleMirrorAddMatchName::Ref::New(rule, dialog->CreateMatchName()) : igdeUndo::Ref();
	}
};

class cActionMatchNameAddTemplate : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionMatchNameAddTemplate>;
	
private:
	const decString pFirst;
	const decString pSecond;
	const deAnimatorRuleMirror::eMatchNameType pType;
public:
	cActionMatchNameAddTemplate(aeWPAPanelRuleMirror &panel, const char *first, const char *second,
		deAnimatorRuleMirror::eMatchNameType type) :
	cBaseAction(panel, CreateText(first, second, type), panel.GetEnvironment().GetStockIcon(
		igdeEnvironment::esiPlus), "Add match name"),
	pFirst(first), pSecond(second), pType(type){}
	
	static decString CreateText(const char *first, const char *second, deAnimatorRuleMirror::eMatchNameType type){
		if(!first || !second){
			DETHROW(deeNullPointer);
		}
		
		const char *typeStr;
		decString text;
		
		switch(type){
		case deAnimatorRuleMirror::emntFirst:
			typeStr = "Begin";
			break;
			
		case deAnimatorRuleMirror::emntLast:
			typeStr = "End";
			break;
			
		case deAnimatorRuleMirror::emntMiddle:
			typeStr = "Middle";
			break;
			
		default:
			typeStr = "?";
		}
		
		text.Format("Add: %s '%s' <-> '%s'", typeStr, first, second);
		return text;
	}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleMirror *rule) override{
		return aeURuleMirrorAddMatchName::Ref::New(rule,
			aeRuleMirror::MatchName::Ref::New(pFirst, pSecond, pType));
	}
};

class cActionMatchNameEdit : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionMatchNameEdit>;
	
	cActionMatchNameEdit(aeWPAPanelRuleMirror &panel) :
	cBaseAction(panel, "@Animator.WPAPanelRuleMirror.ActionMatchNameEdit.Label",
		nullptr, "@Animator.WPAPanelRuleMirror.ActionMatchNameEdit.ToolTip"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleMirror *rule) override{
		aeRuleMirror::MatchName * const matchName = pPanel.GetSelectedMatchBone();
		if(!matchName){
			return {};
		}
		
		aeDialogMirrorMatchName::Ref dialog(aeDialogMirrorMatchName::Ref::New(
			pPanel.GetEnvironment(), "Edit match name"));
		((aeDialogMirrorMatchName&)(igdeDialog&)dialog).Set(*matchName);
		if(!dialog->Run(&pPanel)){
			return {};
		}
		
		const aeRuleMirror::MatchName::Ref edited(
			((aeDialogMirrorMatchName&)(igdeDialog&)dialog).CreateMatchName());
		
		return edited->operator!=(*matchName) ? aeURuleMirrorSetMatchName::Ref::New(rule,
			rule->GetMatchNames().IndexOf(matchName), edited) : igdeUndo::Ref();
	}
	
	void Update(const aeAnimator &, const aeRuleMirror &) override{
		SetEnabled(pPanel.GetSelectedMatchBone());
	}
};

class cActionMatchNameRemove : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionMatchNameRemove>;
	
public:
	cActionMatchNameRemove(aeWPAPanelRuleMirror &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleMirror.ActionMatchNameRemove.Label",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"@Animator.WPAPanelRuleMirror.ActionMatchNameRemove.ToolTip"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleMirror *rule) override{
		aeRuleMirror::MatchName * const matchName = pPanel.GetSelectedMatchBone();
		return matchName ? aeURuleMirrorRemoveMatchName::Ref::New(rule, matchName) : aeURuleMirrorRemoveMatchName::Ref();
	}
	
	void Update(const aeAnimator &, const aeRuleMirror &) override{
		SetEnabled(pPanel.GetSelectedMatchBone());
	}
};

class cActionMatchNameRemoveAll : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionMatchNameRemoveAll>;
	
public:
	cActionMatchNameRemoveAll(aeWPAPanelRuleMirror &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleMirror.ActionMatchNameRemoveAll.Label",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"@Animator.WPAPanelRuleMirror.ActionMatchNameRemoveAll.ToolTip"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleMirror *rule) override{
		return rule->GetMatchNames().IsNotEmpty()
			? aeURuleMirrorRemoveAllMatchNames::Ref::New(rule)
			: aeURuleMirrorRemoveAllMatchNames::Ref();
	}
	
	void Update(const aeAnimator &, const aeRuleMirror &rule) override{
		SetEnabled(rule.GetMatchNames().IsNotEmpty());
	}
};

class cListMatchNames : public igdeListBoxListener{
	aeWPAPanelRuleMirror &pPanel;
	
public:
	using Ref = deTObjectReference<cListMatchNames>;
	cListMatchNames(aeWPAPanelRuleMirror &panel) : pPanel(panel){}
	
	void OnDoubleClickItem(igdeListBox*, int) override{
		((cBaseAction&)cActionMatchNameEdit::Ref::New(pPanel)).OnAction();
	}
	
	void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu) override{
		if(!pPanel.GetRule()){
			return;
		}
		
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(menu, cActionMatchNameAdd::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionMatchNameAddTemplate::Ref::New(pPanel,
			".l", ".r", deAnimatorRuleMirror::emntLast));
		helper.MenuCommand(menu, cActionMatchNameAddTemplate::Ref::New(pPanel,
			".L", ".R", deAnimatorRuleMirror::emntLast));
		helper.MenuCommand(menu, cActionMatchNameAddTemplate::Ref::New(pPanel,
			"Left", "Right", deAnimatorRuleMirror::emntMiddle));
		helper.MenuCommand(menu, cActionMatchNameAddTemplate::Ref::New(pPanel,
			"LEFT", "RIGHT", deAnimatorRuleMirror::emntMiddle));
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, cActionMatchNameEdit::Ref::New(pPanel));
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, cActionMatchNameRemove::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionMatchNameRemoveAll::Ref::New(pPanel));
	}
};

class cActionEnablePosition : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionEnablePosition>;
	
public:
	cActionEnablePosition(aeWPAPanelRuleMirror &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleMirror.EnablePosition.Label", nullptr,
		"@Animator.WPAPanelRuleMirror.EnablePosition.ToolTip"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleMirror *rule) override{
		return aeURuleMirrorSetEnablePosition::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleMirror &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnablePosition());
	}
};

class cActionEnableRotation : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionEnableRotation>;
	
public:
	cActionEnableRotation(aeWPAPanelRuleMirror &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleMirror.EnableRotation.Label", nullptr,
		"@Animator.WPAPanelRuleMirror.EnableRotation.ToolTip"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleMirror *rule) override{
		return aeURuleMirrorSetEnableRotation::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleMirror &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableOrientation());
	}
};

class cActionEnableSize : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionEnableSize>;
	
public:
	cActionEnableSize(aeWPAPanelRuleMirror &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleMirror.EnableSize.Label", nullptr,
		"@Animator.WPAPanelRuleMirror.EnableSize.ToolTip"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleMirror *rule) override{
		return aeURuleMirrorSetEnableSize::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleMirror &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableSize());
	}
};

class cActionEnableVertexPositionSet : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionEnableVertexPositionSet>;
	
public:
	cActionEnableVertexPositionSet(aeWPAPanelRuleMirror &panel) : cBaseAction(panel,
		"@Animator.WPAPanelRuleMirror.EnableVertexPositionSet.Label", nullptr,
		"@Animator.WPAPanelRuleMirror.EnableVertexPositionSet.ToolTip"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRuleMirror *rule) override{
		return aeURuleMirrorSetEnableVertexPositionSet::Ref::New(rule);
	}
	
	void Update(const aeAnimator & , const aeRuleMirror &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnableVertexPositionSet());
	}
};

}



// Class aeWPAPanelRuleMirror
///////////////////////////////

// Constructor, destructor
////////////////////////////

aeWPAPanelRuleMirror::aeWPAPanelRuleMirror(aeWPRule &wpRule) :
aeWPAPanelRule(wpRule, deAnimatorRuleVisitorIdentify::ertMirror)
{
	igdeEnvironment &env = wpRule.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref groupBox;
	
	
	helper.GroupBox(*this, groupBox, "@Animator.WPAPanelRuleMirror.Mirror.Label");
	
	helper.ComboBox(groupBox, "@Animator.WPAPanelRuleMirror.Axis.Label",
		"@Animator.WPAPanelRuleMirror.Axis.ToolTip", pCBMirrorAxis, cComboMirrorAxis::Ref::New(*this));
	pCBMirrorAxis->SetAutoTranslateItems(true);
	pCBMirrorAxis->AddItem("@Animator.WPAPanelRuleMirror.Axis.X", nullptr, (void*)(intptr_t)deAnimatorRuleMirror::emaX);
	pCBMirrorAxis->AddItem("@Animator.WPAPanelRuleMirror.Axis.Y", nullptr, (void*)(intptr_t)deAnimatorRuleMirror::emaY);
	pCBMirrorAxis->AddItem("@Animator.WPAPanelRuleMirror.Axis.Z", nullptr, (void*)(intptr_t)deAnimatorRuleMirror::emaZ);
	
	helper.ComboBoxFilter(groupBox, "@Animator.WPAPanelRuleMirror.Bone.Label", true,
		"@Animator.WPAPanelRuleMirror.Bone.ToolTip",
		pCBMirrorBone, cComboMirrorBone::Ref::New(*this));
	pCBMirrorBone->SetDefaultSorter();
	
	helper.CheckBox(groupBox, pChkEnablePosition, cActionEnablePosition::Ref::New(*this));
	helper.CheckBox(groupBox, pChkEnableRotation, cActionEnableRotation::Ref::New(*this));
	helper.CheckBox(groupBox, pChkEnableSize, cActionEnableSize::Ref::New(*this));
	helper.CheckBox(groupBox, pChkEnableVertexPositionSet, cActionEnableVertexPositionSet::Ref::New(*this));
	
	helper.ListBox(groupBox, "@Animator.WPAPanelRuleMirror.Pairs.Label", 3,
		"@Animator.WPAPanelRuleMirror.Pairs.ToolTip",
		pListMatchName, cListMatchNames::Ref::New(*this));
}

aeWPAPanelRuleMirror::~aeWPAPanelRuleMirror(){
}



// Management
///////////////

aeRuleMirror::MatchName *aeWPAPanelRuleMirror::GetSelectedMatchBone() const{
	return pListMatchName->GetSelectedItem()
		? (aeRuleMirror::MatchName*)pListMatchName->GetSelectedItem()->GetData() : nullptr;
}

void aeWPAPanelRuleMirror::UpdateRigBoneList(){
	aeWPAPanelRule::UpdateRigBoneList();
	
	const decString selection(pCBMirrorBone->GetText());
	
	pCBMirrorBone->RemoveAllItems();
	
	if(GetAnimator()){
		const deRig * const rig = GetAnimator()->GetEngineRig();
		if(rig){
			const int count = rig->GetBoneCount();
			int i;
			for(i=0; i<count; i++){
				pCBMirrorBone->AddItem(rig->GetBoneAt(i)->GetName());
			}
		}
		pCBMirrorBone->SortItems();
	}
	
	pCBMirrorBone->StoreFilterItems();
	pCBMirrorBone->SetText(selection);
}

void aeWPAPanelRuleMirror::UpdateRule(){
	aeWPAPanelRule::UpdateRule();
	
	const aeRuleMirror * const rule = (aeRuleMirror*)GetRule();
	
	if(rule){
		pCBMirrorAxis->SetSelectionWithData((void*)(intptr_t)rule->GetMirrorAxis());
		pCBMirrorBone->SetText(rule->GetMirrorBone());
		
		aeRuleMirror::MatchName * const selection = GetSelectedMatchBone();
		pListMatchName->RemoveAllItems();
		
		const char *typeStr;
		decString text;
		
		rule->GetMatchNames().Visit([&](aeRuleMirror::MatchName *matchName){
			switch(matchName->type){
			case deAnimatorRuleMirror::emntFirst:
				typeStr = "Begin";
				break;
				
			case deAnimatorRuleMirror::emntLast:
				typeStr = "End";
				break;
				
			case deAnimatorRuleMirror::emntMiddle:
				typeStr = "Middle";
				break;
				
			default:
				typeStr = "?";
			}
			
			text.Format("%s: '%s' <-> '%s'", typeStr,
				matchName->first.GetString(), matchName->second.GetString());
			pListMatchName->AddItem(text, nullptr, matchName);
		});
		
		pListMatchName->SetSelectionWithData(selection);
		
	}else{
		pCBMirrorAxis->SetSelectionWithData((void*)(intptr_t)deAnimatorRuleMirror::emaX);
		pCBMirrorBone->ClearText();
		pListMatchName->RemoveAllItems();
	}
	
	const bool enabled = rule;
	pCBMirrorAxis->SetEnabled(enabled);
	pCBMirrorBone->SetEnabled(enabled);
	pListMatchName->SetEnabled(enabled);
	
	pChkEnablePosition->GetAction()->Update();
	pChkEnableRotation->GetAction()->Update();
	pChkEnableSize->GetAction()->Update();
	pChkEnableVertexPositionSet->GetAction()->Update();
}
