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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "aeWPAPanelRule.h"
#include "../aeWPRule.h"
#include "../aeWindowProperties.h"
#include "../../aeWindowMain.h"
#include "../../../animatoreditor.h"
#include "../../../animator/aeAnimator.h"
#include "../../../animator/controller/aeController.h"
#include "../../../animator/controller/aeControllerTarget.h"
#include "../../../animator/link/aeLink.h"
#include "../../../animator/rule/aeRule.h"
#include "../../../clipboard/aeClipboardDataBones.h"
#include "../../../clipboard/aeClipboardDataVertexPositionSets.h"
#include "../../../undosys/rule/aeURuleMirrorBones.h"
#include "../../../undosys/rule/aeURuleMirrorVertexPositionSets.h"
#include "../../../undosys/rule/aeURuleTargetAddLink.h"
#include "../../../undosys/rule/aeURuleTargetRemoveLink.h"
#include "../../../undosys/rule/aeURuleTargetRemoveAllLinks.h"
#include "../../../undosys/rule/aeUSetRuleAddBone.h"
#include "../../../undosys/rule/aeUSetRuleAddVertexPositionSet.h"
#include "../../../undosys/rule/aeUSetRuleBlendFactor.h"
#include "../../../undosys/rule/aeUToggleRuleInvertBlendFactor.h"
#include "../../../undosys/rule/aeUSetRuleBlendMode.h"
#include "../../../undosys/rule/aeUSetRuleEnabled.h"
#include "../../../undosys/rule/aeUSetRuleName.h"
#include "../../../undosys/rule/aeUSetRuleRemoveBone.h"
#include "../../../undosys/rule/aeUSetRuleRemoveAllBones.h"
#include "../../../undosys/rule/aeUSetRuleBones.h"
#include "../../../undosys/rule/aeUSetRuleRemoveVertexPositionSet.h"
#include "../../../undosys/rule/aeUSetRuleRemoveAllVertexPositionSets.h"
#include "../../../undosys/rule/aeUSetRuleVertexPositionSets.h"

#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/clipboard/igdeClipboardData.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeWindow.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelVertexPositionSet.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	aeWPAPanelRule &pPanel;
	
public:
	cBaseAction(aeWPAPanelRule &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	virtual void OnAction(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRule * const rule = pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnAction(animator, rule)));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnAction(aeAnimator *animator, aeRule *rule) = 0;
	
	virtual void Update(){
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

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	aeWPAPanelRule &pPanel;
	
public:
	cBaseTextFieldListener(aeWPAPanelRule &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRule * const rule = pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(textField, animator, rule)));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, aeAnimator *animator, aeRule *rule) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	aeWPAPanelRule &pPanel;
	
public:
	cBaseComboBoxListener(aeWPAPanelRule &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRule * const rule = pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(comboBox, animator, rule)));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeComboBox *comboBox, aeAnimator *animator, aeRule *rule) = 0;
};


class cTextName : public cBaseTextFieldListener{
public:
	cTextName(aeWPAPanelRule &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, aeAnimator*, aeRule *rule){
		return textField->GetText() != rule->GetName() ? new aeUSetRuleName(rule, textField->GetText()) : NULL;
	}
};

class cComboBlendMode : public cBaseComboBoxListener{
public:
	cComboBlendMode(aeWPAPanelRule &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeComboBox *comboBox, aeAnimator*, aeRule *rule){
		if(!comboBox->GetSelectedItem()){
			return NULL;
		}
		
		const deAnimatorRule::eBlendModes mode =
			(deAnimatorRule::eBlendModes)(intptr_t)comboBox->GetSelectedItem()->GetData();
		return mode != rule->GetBlendMode() ? new aeUSetRuleBlendMode(rule, mode) : NULL;
	}
};

class cTextBlendFactor : public cBaseTextFieldListener{
public:
	cTextBlendFactor(aeWPAPanelRule &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, aeAnimator*, aeRule *rule){
		const float factor = textField->GetFloat();
		return fabsf(factor - rule->GetBlendFactor()) > FLOAT_SAFE_EPSILON
			? new aeUSetRuleBlendFactor(rule, factor) : NULL;
	}
};

class cActionInvertBlendFactor : public cBaseAction{
public:
	cActionInvertBlendFactor(aeWPAPanelRule &panel) : cBaseAction(panel,
		"Invert Blend Factor", nullptr, "Use '1 - blendFactor' instead of 'blendFactor'"){}
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRule *rule){
		return new aeUToggleRuleInvertBlendFactor(rule);
	}
	
	void Update(const aeAnimator &, const aeRule &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetInvertBlendFactor());
	}
};

class cActionEnabled : public cBaseAction{
public:
	cActionEnabled(aeWPAPanelRule &panel) : cBaseAction(panel, "Enable Rule", NULL,
		"Determines if the rule is affecting animation"){ }
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRule *rule){
		return new aeUSetRuleEnabled(rule);
	}
	
	void Update(const aeAnimator &, const aeRule &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnabled());
	}
};


class cActionBoneAdd : public cBaseAction{
public:
	cActionBoneAdd(aeWPAPanelRule &panel) : cBaseAction(panel, "Add",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add bone"){}
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRule *rule){
		const decString &name = pPanel.GetCBBoneText();
		return !name.IsEmpty() && !rule->GetListBones().Has(name)
			? new aeUSetRuleAddBone(rule, name) : NULL;
	}
	
	void Update(const aeAnimator &, const aeRule &rule) override{
		const decString &name = pPanel.GetCBBoneText();
		SetEnabled(!name.IsEmpty() && !rule.GetListBones().Has(name));
	}
};

class cActionBoneRemoves : public cBaseAction{
public:
	cActionBoneRemoves(aeWPAPanelRule &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove bones"){}
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRule *rule){
		const decString &name = pPanel.GetCBBoneText();
		if(name.IsEmpty()){
			return NULL;
		}
		aeUSetRuleRemoveBone * const undo = new aeUSetRuleRemoveBone(rule, name);
		if(undo->HasBones()){
			return undo;
			
		}else{
			undo->FreeReference();
			return NULL;
		}
	}
	
	void Update(const aeAnimator &, const aeRule &rule) override{
		SetEnabled(!pPanel.GetCBBoneText().IsEmpty());
	}
};

class cActionBoneRemovesAll : public cBaseAction{
public:
	cActionBoneRemovesAll(aeWPAPanelRule &panel) : cBaseAction(panel, "Remove All",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove all bones"){}
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRule *rule){
		return rule->GetListBones().GetCount() > 0
			? new aeUSetRuleRemoveAllBones(rule) : NULL;
	}
	
	void Update(const aeAnimator &, const aeRule &rule) override{
		SetEnabled(rule.GetListBones().GetCount() > 0);
	}
};

class cActionBoneSelectedRemove : public cBaseAction{
public:
	cActionBoneSelectedRemove(aeWPAPanelRule &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove selected bone"){}
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRule *rule){
		const char * const name = pPanel.GetListBoneSelection();
		return name ? new aeUSetRuleRemoveBone(rule, name) : NULL;
	}
	
	void Update(const aeAnimator &, const aeRule &rule) override{
		SetEnabled(pPanel.GetListBoneSelection());
	}
};

class cActionMirrorRigBones : public cBaseAction{
public:
	cActionMirrorRigBones(aeWPAPanelRule &panel) : cBaseAction(panel, "Mirror Bones",
		NULL, "Mirror rig bones"){}
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRule *rule){
		// TODO add a dialog to allow changing the mirror parameter (or add a new menu command)
		return new aeURuleMirrorBones(rule);
	}
	
	void Update(const aeAnimator &, const aeRule &rule) override{
		SetEnabled(rule.GetListBones().GetCount() > 0);
	}
};

class cActionCopyBones : public cBaseAction{
public:
	cActionCopyBones(aeWPAPanelRule &panel) : cBaseAction(panel, "Copy",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy), "Copy bones"){}
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRule *rule){
		pPanel.GetWindowMain().GetClipboard().Set(
			aeClipboardDataBones::Ref::NewWith(rule->GetListBones()));
		return NULL;
	}
};

class cActionPasteBones : public cBaseAction{
public:
	cActionPasteBones(aeWPAPanelRule &panel) : cBaseAction(panel, "Paste",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy), "Copy bones"){}
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRule *rule){
		aeClipboardDataBones * const clip = (aeClipboardDataBones*)pPanel.GetWindowMain()
			.GetClipboard().GetWithTypeName(aeClipboardDataBones::TYPE_NAME);
		if(!clip){
			return NULL;
		}
		
		aeUSetRuleBones * const undo = new aeUSetRuleBones(rule, rule->GetListBones() + clip->GetBones());
		undo->SetShortInfo("Rule paste bones");
		return undo;
	}
	
	void Update(const aeAnimator &, const aeRule &) override{
		SetEnabled(pPanel.GetWindowMain().GetClipboard().HasWithTypeName(aeClipboardDataBones::TYPE_NAME));
	}
};

class cActionExportBones : public cBaseAction{
public:
	cActionExportBones(aeWPAPanelRule &panel) : cBaseAction(panel, "Export To Text",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave), "Export bones"){}
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRule *rule){
		const decStringSet bones = rule->GetListBones();
		const int count = bones.GetCount();
		decString text;
		int i;
		for(i=0; i<count; i++){
			if(i > 0){
				text.AppendCharacter('\n');
			}
			text.Append(bones.GetAt(i));
		}
		igdeCommonDialogs::GetMultilineString(pPanel.GetParentWindow(), "Export To Text", "Bones", text);
		return nullptr;
	}
	
	void Update(const aeAnimator &, const aeRule &rule) override{
		SetEnabled(rule.GetListBones().GetCount() > 0);
	}
};

class cActionImportBones : public cBaseAction{
public:
	cActionImportBones(aeWPAPanelRule &panel) : cBaseAction(panel, "Import From Text",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen), "Import bones"){}
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRule *rule){
		decString text;
		while(true){
			if(!igdeCommonDialogs::GetMultilineString(pPanel.GetParentWindow(),
			"Import From Text", "Bones. One bone per line.", text)){
				return nullptr;
			}
			break;
		}
		
		const decStringList lines(text.Split('\n'));
		const int count = lines.GetCount();
		decStringSet bones;
		int i;
		
		for(i=0; i<count; i++){
			if(!lines.GetAt(i).IsEmpty()){
				bones.Add(lines.GetAt(i));
			}
		}
		
		aeUSetRuleBones * const undo = new aeUSetRuleBones(rule, rule->GetListBones() + bones);
		undo->SetShortInfo("Rule import bones");
		return undo;
	}
};


class cListBones : public igdeListBoxListener{
	aeWPAPanelRule &pPanel;
	
public:
	cListBones(aeWPAPanelRule &panel) : pPanel(panel){}
	
	/*virtual void OnSelectionChanged( igdeListBox *listBox ){
		if(pPanel.GetRule() && listBox->GetSelectedItem()){
			pPanel.SetCBBoneText(listBox->GetSelectedItem()->GetText());
		}
	}*/
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu){
		if(!pPanel.GetRule()){
			return;
		}
		
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		
		helper.MenuCommand(menu, new cActionBoneAdd(pPanel), true);
		helper.MenuCommand(menu, new cActionBoneSelectedRemove(pPanel), true);
		helper.MenuCommand(menu, new cActionBoneRemovesAll(pPanel), true);
		helper.MenuCommand(menu, new cActionMirrorRigBones(pPanel), true);
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, new cActionCopyBones(pPanel), true);
		helper.MenuCommand(menu, new cActionPasteBones(pPanel), true);
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, new cActionExportBones(pPanel), true);
		helper.MenuCommand(menu, new cActionImportBones(pPanel), true);
	}
};


class cActionVertexPositionSetAdd : public cBaseAction{
public:
	cActionVertexPositionSetAdd(aeWPAPanelRule &panel) : cBaseAction(panel, "Add",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add vertex position set"){}
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRule *rule){
		const decString &name = pPanel.GetCBVertexPositionSetText();
		return !name.IsEmpty() && !rule->GetListVertexPositionSets().Has(name)
			? new aeUSetRuleAddVertexPositionSet(rule, name) : NULL;
	}
	
	void Update(const aeAnimator &, const aeRule &rule) override{
		const decString &name = pPanel.GetCBVertexPositionSetText();
		SetEnabled(!name.IsEmpty() && !rule.GetListVertexPositionSets().Has(name));
	}
};

class cActionVertexPositionSetRemoves : public cBaseAction{
public:
	cActionVertexPositionSetRemoves(aeWPAPanelRule &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove vertex position sets"){}
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRule *rule){
		const decString &name = pPanel.GetCBVertexPositionSetText();
		if(name.IsEmpty()){
			return NULL;
		}
		aeUSetRuleRemoveVertexPositionSet * const undo = new aeUSetRuleRemoveVertexPositionSet(rule, name);
		if(undo->HasVertexPositionSets()){
			return undo;
			
		}else{
			undo->FreeReference();
			return NULL;
		}
	}
	
	void Update(const aeAnimator &, const aeRule &rule) override{
		SetEnabled(!pPanel.GetCBVertexPositionSetText().IsEmpty());
	}
};

class cActionVertexPositionSetRemovesAll : public cBaseAction{
public:
	cActionVertexPositionSetRemovesAll(aeWPAPanelRule &panel) : cBaseAction(panel, "Remove All",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove all vertex position sets"){}
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRule *rule){
		return rule->GetListVertexPositionSets().GetCount() > 0
			? new aeUSetRuleRemoveAllVertexPositionSets(rule) : NULL;
	}
	
	void Update(const aeAnimator &, const aeRule &rule) override{
		SetEnabled(rule.GetListVertexPositionSets().GetCount() > 0);
	}
};

class cActionVertexPositionSetSelectedRemove : public cBaseAction{
public:
	cActionVertexPositionSetSelectedRemove(aeWPAPanelRule &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove selected vertex position set"){}
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRule *rule){
		const char * const name = pPanel.GetListVertexPositionSetSelection();
		return name ? new aeUSetRuleRemoveVertexPositionSet(rule, name) : NULL;
	}
	
	void Update(const aeAnimator &, const aeRule &rule) override{
		SetEnabled(pPanel.GetListVertexPositionSetSelection());
	}
};

class cActionMirrorRigVertexPositionSets : public cBaseAction{
public:
	cActionMirrorRigVertexPositionSets(aeWPAPanelRule &panel) : cBaseAction(panel,
		"Mirror", nullptr, "Mirror vertex position sets"){}
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRule *rule){
		// TODO add a dialog to allow changing the mirror parameter (or add a new menu command)
		return new aeURuleMirrorVertexPositionSets(rule);
	}
	
	void Update(const aeAnimator &, const aeRule &rule) override{
		SetEnabled(rule.GetListVertexPositionSets().GetCount() > 0);
	}
};

class cActionCopyVertexPositionSets : public cBaseAction{
public:
	cActionCopyVertexPositionSets(aeWPAPanelRule &panel) : cBaseAction(panel, "Copy",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy), "Copy vertex position set"){}
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRule *rule){
		pPanel.GetWindowMain().GetClipboard().Set(
			aeClipboardDataVertexPositionSets::Ref::NewWith(rule->GetListVertexPositionSets()));
		return nullptr;
	}
};

class cActionPasteVertexPositionSets : public cBaseAction{
public:
	cActionPasteVertexPositionSets(aeWPAPanelRule &panel) : cBaseAction(panel, "Paste",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy), "Copy vertex position sets"){}
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRule *rule){
		aeClipboardDataVertexPositionSets * const clip = (aeClipboardDataVertexPositionSets*)pPanel.GetWindowMain()
			.GetClipboard().GetWithTypeName(aeClipboardDataVertexPositionSets::TYPE_NAME);
		if(!clip){
			return NULL;
		}
		
		aeUSetRuleVertexPositionSets * const undo = new aeUSetRuleVertexPositionSets(
			rule, rule->GetListVertexPositionSets() + clip->GetVertexPositionSets());
		undo->SetShortInfo("Rule paste vertex position sets");
		return undo;
	}
	
	void Update(const aeAnimator &, const aeRule &) override{
		SetEnabled(pPanel.GetWindowMain().GetClipboard().HasWithTypeName(aeClipboardDataVertexPositionSets::TYPE_NAME));
	}
};

class cActionExportVertexPositionSets : public cBaseAction{
public:
	cActionExportVertexPositionSets(aeWPAPanelRule &panel) : cBaseAction(panel, "Export To Text",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave), "Export vertex position set"){}
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRule *rule){
		const decStringSet vpslist = rule->GetListVertexPositionSets();
		const int count = vpslist.GetCount();
		decString text;
		int i;
		for(i=0; i<count; i++){
			if(i > 0){
				text.AppendCharacter('\n');
			}
			text.Append(vpslist.GetAt(i));
		}
		igdeCommonDialogs::GetMultilineString(pPanel.GetParentWindow(), "Export To Text", "Vertex position sets", text);
		return nullptr;
	}
	
	void Update(const aeAnimator &, const aeRule &rule) override{
		SetEnabled(rule.GetListVertexPositionSets().GetCount() > 0);
	}
};

class cActionImportVertexPositionSets : public cBaseAction{
public:
	cActionImportVertexPositionSets(aeWPAPanelRule &panel) : cBaseAction(panel, "Import From Text",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen), "Import vertex position sets"){}
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRule *rule){
		decString text;
		while(true){
			if(!igdeCommonDialogs::GetMultilineString(pPanel.GetParentWindow(),
			"Import From Text", "Vertex position sets. One vertex position set per line.", text)){
				return nullptr;
			}
			break;
		}
		
		const decStringList lines(text.Split('\n'));
		const int count = lines.GetCount();
		decStringSet vpslist;
		int i;
		
		for(i=0; i<count; i++){
			if(!lines.GetAt(i).IsEmpty()){
				vpslist.Add(lines.GetAt(i));
			}
		}
		
		aeUSetRuleVertexPositionSets * const undo = new aeUSetRuleVertexPositionSets(
			rule, rule->GetListVertexPositionSets() + vpslist);
		undo->SetShortInfo("Rule import vertex position sets");
		return undo;
	}
};

class cListVertexPositionSets : public igdeListBoxListener{
	aeWPAPanelRule &pPanel;
	
public:
	cListVertexPositionSets(aeWPAPanelRule &panel) : pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu){
		if(!pPanel.GetRule()){
			return;
		}
		
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		
		helper.MenuCommand(menu, new cActionVertexPositionSetAdd(pPanel), true);
		helper.MenuCommand(menu, new cActionVertexPositionSetSelectedRemove(pPanel), true);
		helper.MenuCommand(menu, new cActionVertexPositionSetRemovesAll(pPanel), true);
		helper.MenuCommand(menu, new cActionMirrorRigVertexPositionSets(pPanel), true);
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, new cActionCopyVertexPositionSets(pPanel), true);
		helper.MenuCommand(menu, new cActionPasteVertexPositionSets(pPanel), true);
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, new cActionExportVertexPositionSets(pPanel), true);
		helper.MenuCommand(menu, new cActionImportVertexPositionSets(pPanel), true);
	}
};


class cComboTarget : public cBaseComboBoxListener{
public:
	cComboTarget(aeWPAPanelRule &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeComboBox *comboBox, aeAnimator*, aeRule*){
		pPanel.SetTarget(comboBox->GetSelectedItem()
			? (aeControllerTarget*)comboBox->GetSelectedItem()->GetData() : NULL);
		return NULL;
	}
};


class cActionLinkAdd : public cBaseAction{
public:
	cActionLinkAdd(aeWPAPanelRule &panel) : cBaseAction(panel, "Add",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add link"){}
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRule *rule){
		aeControllerTarget * const target = pPanel.GetTarget();
		aeLink * const link = pPanel.GetCBLinkSelection();
		return target && link && !target->HasLink(link)
			? new aeURuleTargetAddLink(rule, target, link) : NULL;
	}
	
	void Update(const aeAnimator &, const aeRule &) override{
		const aeControllerTarget * const target = pPanel.GetTarget();
		aeLink * const link = pPanel.GetCBLinkSelection();
		SetEnabled(target && link && !target->HasLink(link));
	}
};

class cActionLinkRemove : public cBaseAction{
public:
	cActionLinkRemove(aeWPAPanelRule &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove link"){}
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRule *rule){
		aeControllerTarget * const target = pPanel.GetTarget();
		aeLink * const link = pPanel.GetListLinkSelection();
		return target && link && target->HasLink(link)
			? new aeURuleTargetRemoveLink(rule, target, link) : NULL;
	}
	
	void Update(const aeAnimator &, const aeRule &) override{
		const aeControllerTarget * const target = pPanel.GetTarget();
		aeLink * const link = pPanel.GetListLinkSelection();
		SetEnabled(target && link && target->HasLink(link));
	}
};

class cActionLinkRemoveAll : public cBaseAction{
public:
	cActionLinkRemoveAll(aeWPAPanelRule &panel) : cBaseAction(panel, "Remove All",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove all links"){}
	
	virtual igdeUndo *OnAction(aeAnimator*, aeRule *rule){
		aeControllerTarget * const target = pPanel.GetTarget();
		if(!target || target->GetLinkCount() == 0){
			return NULL;
		}
		
		const int count = target->GetLinkCount();
		aeLinkList list;
		int i;
		for(i=0; i<count; i++){
			list.Add(target->GetLinkAt(i));
		}
		return new aeURuleTargetRemoveAllLinks(rule, target, list);
	}
	
	void Update(const aeAnimator &, const aeRule &) override{
		const aeControllerTarget * const target = pPanel.GetTarget();
		SetEnabled(target && target->GetLinkCount() > 0);
	}
};


class cListLinks : public igdeListBoxListener{
	aeWPAPanelRule &pPanel;
	
public:
	cListLinks(aeWPAPanelRule &panel) : pPanel(panel){}
	
	/*virtual void OnSelectionChanged( igdeListBox *listBox ){
		if(listBox->GetSelectedItem()){
			pPanel.SetCBLinkSelection((aeLink*)listBox->GetSelectedItem()->GetData());
		}
	}*/
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu){
		if(!pPanel.GetRule()){
			return;
		}
		
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		
		helper.MenuCommand(menu, new cActionLinkAdd(pPanel), true);
		helper.MenuCommand(menu, new cActionLinkRemove(pPanel), true);
		helper.MenuCommand(menu, new cActionLinkRemoveAll(pPanel), true);
	}
};

}



// Class aeWPAPanelRule
/////////////////////////

// Constructor, destructor
////////////////////////////

aeWPAPanelRule::aeWPAPanelRule(aeWPRule &wpRule, deAnimatorRuleVisitorIdentify::eRuleTypes requiredType) :
igdeContainerFlow(wpRule.GetEnvironment(), igdeContainerFlow::eaY),
pWPRule(wpRule),
pRequiredType(requiredType),
pTarget(NULL)
{
	igdeEnvironment &env = wpRule.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer *groupBox, formLine;
	
	
	// animation
	helper.GroupBox(*this, groupBox, "General Settings:");
	
	helper.EditString(groupBox, "Name:", "Name of the rule", pEditName, new cTextName(*this));
	
	helper.ComboBox(groupBox, "Blend Mode:", "Sets the blend mode",
		pCBBlendMode, new cComboBlendMode(*this));
	pCBBlendMode->AddItem("Blend", NULL, (void*)(intptr_t)deAnimatorRule::ebmBlend);
	pCBBlendMode->AddItem("Overlay", NULL, (void*)(intptr_t)deAnimatorRule::ebmOverlay);
	
	helper.EditFloat(groupBox, "Blend Factor:", "Sets the blend factor",
		pEditBlendFactor, new cTextBlendFactor(*this));
	helper.CheckBox(groupBox, pChkInvertBlendFactor, new cActionInvertBlendFactor(*this), true);
	helper.CheckBox(groupBox, pChkEnabled, new cActionEnabled(*this), true);
	
	
	// affected bones
	helper.GroupBoxFlow(*this, groupBox, "Affected bones:", false, true);
	
	formLine.TakeOver(new igdeContainerFlow(env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst));
	groupBox->AddChild(formLine);
	helper.ComboBoxFilter(formLine, true, "Bones", pCBBones, NULL);
	pCBBones->SetDefaultSorter();
	helper.Button(formLine, pBtnBoneAdd, new cActionBoneAdd(*this), true);
	helper.Button(formLine, pBtnBoneDel, new cActionBoneRemoves(*this), true);
	
	helper.ListBox(groupBox, 4, "Bones affected by rule", pListBones, new cListBones(*this));
	pListBones->SetDefaultSorter();
	
	
	// affected vertex position sets
	helper.GroupBoxFlow(*this, groupBox, "Affected Vertex Position Sets:", false, true);
	
	formLine.TakeOver(new igdeContainerFlow(env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst));
	groupBox->AddChild(formLine);
	helper.ComboBoxFilter(formLine, true, "Vertex Position Sets", pCBVertexPositionSets, NULL);
	pCBVertexPositionSets->SetDefaultSorter();
	helper.Button(formLine, pBtnVertexPositionSetAdd, new cActionVertexPositionSetAdd(*this), true);
	helper.Button(formLine, pBtnVertexPositionSetDel, new cActionVertexPositionSetRemoves(*this), true);
	
	helper.ListBox(groupBox, 4, "Vertex position sets affected by rule",
		pListVertexPositionSets, new cListVertexPositionSets(*this));
	pListVertexPositionSets->SetDefaultSorter();
	
	
	// targets
	helper.GroupBoxFlow(*this, groupBox, "Targets and Links:");
	
	formLine.TakeOver(new igdeContainerForm(env));
	groupBox->AddChild(formLine);
	helper.ComboBox(formLine, "Target:", "Targets", pCBTarget, new cComboTarget(*this));
	
	formLine.TakeOver(new igdeContainerFlow(env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst));
	groupBox->AddChild(formLine);
	helper.ComboBoxFilter(formLine, "Links", pCBLinks, NULL);
	pCBLinks->SetDefaultSorter();
	helper.Button(formLine, pBtnLinkAdd, new cActionLinkAdd(*this), true);
	
	helper.ListBox(groupBox, 4, "Links affecting target", pListLinks, new cListLinks(*this));
	pListLinks->SetDefaultSorter();
}

aeWPAPanelRule::~aeWPAPanelRule(){
}



// Management
///////////////

aeWindowMain &aeWPAPanelRule::GetWindowMain() const{
	return pWPRule.GetWindowProperties().GetWindowMain();
}

aeAnimator *aeWPAPanelRule::GetAnimator() const{
	return pWPRule.GetAnimator();
}

aeRule *aeWPAPanelRule::GetRule() const{
	aeRule * const rule = pWPRule.GetRule();
	return rule && rule->GetType() == pRequiredType ? rule : NULL;
}

void aeWPAPanelRule::SetTarget(aeControllerTarget *target){
	if(target == pTarget){
		return;
	}
	
	pTarget = target;
	UpdateTarget();
}



void aeWPAPanelRule::OnActivated(){
	SetTarget(NULL);
	
	UpdateRigBoneList();
	UpdateModelVertexPositionSetList();
	UpdateAnimMoveList();
	
	UpdateLinkList();
	UpdateTargetList();
	UpdateControllerList();
	
	OnAnimatorPathChanged();
	
	if(pCBTarget->GetItemCount() > 0){
		pCBTarget->SetSelection(0);
	}
}

void aeWPAPanelRule::OnAnimatorChanged(){
	SetTarget(NULL);
	OnAnimatorPathChanged();
}

void aeWPAPanelRule::OnAnimatorPathChanged(){
}



void aeWPAPanelRule::UpdateControllerList(){
}

void aeWPAPanelRule::UpdateLinkList(){
	aeLink * const selection = GetCBLinkSelection();
	
	pCBLinks->RemoveAllItems();
	
	if(GetAnimator()){
		const aeLinkList &list = GetAnimator()->GetLinks();
		const int count = list.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			aeLink * const link = list.GetAt(i);
			pCBLinks->AddItem(link->GetName(), NULL, link);
		}
		pCBLinks->SortItems();
	}
	
	pCBLinks->StoreFilterItems();
	pCBLinks->SetSelectionWithData(selection);
}

void aeWPAPanelRule::UpdateRigBoneList(){
	const decString selection(GetCBBoneText());
	
	pCBBones->RemoveAllItems();
	
	if(GetAnimator()){
		const deRig * const rig = GetAnimator()->GetEngineRig();
		if(rig){
			const int count = rig->GetBoneCount();
			int i;
			
			for(i=0; i<count; i++){
				pCBBones->AddItem(rig->GetBoneAt(i).GetName());
			}
		}
		pCBBones->SortItems();
	}
	
	pCBBones->StoreFilterItems();
	pCBBones->SetText(selection);
}

void aeWPAPanelRule::UpdateModelVertexPositionSetList(){
	const decString selection(GetCBVertexPositionSetText());
	
	pCBVertexPositionSets->RemoveAllItems();
	
	if(GetAnimator()){
		const deComponent * const component = GetAnimator()->GetEngineComponent();
		const deModel * const model = component ? component->GetModel() : nullptr;
		if(model){
			const int count = model->GetVertexPositionSetCount();
			int i;
			
			for(i=0; i<count; i++){
				pCBVertexPositionSets->AddItem(model->GetVertexPositionSetAt(i)->GetName());
			}
		}
		pCBVertexPositionSets->SortItems();
	}
	
	pCBVertexPositionSets->StoreFilterItems();
	pCBVertexPositionSets->SetText(selection);
}

void aeWPAPanelRule::UpdateAnimMoveList(){
}

void aeWPAPanelRule::UpdateRule(){
	const aeRule * const rule = GetRule();
	
	if(rule){
		// update rule
		pEditName->SetText(rule->GetName());
		pCBBlendMode->SetSelectionWithData((void*)(intptr_t)rule->GetBlendMode());
		pEditBlendFactor->SetFloat(rule->GetBlendFactor());
		
		// update affected bone list
		const decStringSet &boneList = rule->GetListBones();
		const int boneCount = boneList.GetCount();
		int i;
		
		pListBones->RemoveAllItems();
		for(i=0; i<boneCount; i++){
			pListBones->AddItem(boneList.GetAt(i));
		}
		pListBones->SortItems();
		
		// update affected vertex position set list
		const decStringSet &vpsList = rule->GetListVertexPositionSets();
		const int vpsCount = vpsList.GetCount();
		
		pListVertexPositionSets->RemoveAllItems();
		for(i=0; i<vpsCount; i++){
			pListVertexPositionSets->AddItem(vpsList.GetAt(i));
		}
		pListVertexPositionSets->SortItems();
		
	}else{
		pEditName->ClearText();
		pCBBlendMode->SetSelectionWithData((void*)(intptr_t)deAnimatorRule::ebmBlend);
		pEditBlendFactor->ClearText();
		pListBones->RemoveAllItems();
		pListVertexPositionSets->RemoveAllItems();
	}
	
	const bool enabled = rule;
	pEditName->SetEnabled(enabled);
	pCBBlendMode->SetEnabled(enabled);
	pEditBlendFactor->SetEnabled(enabled);
	pListBones->SetEnabled(enabled);
	pListVertexPositionSets->SetEnabled(enabled);
	
	pChkInvertBlendFactor->GetAction()->Update();
	pChkEnabled->GetAction()->Update();
	pBtnBoneAdd->GetAction()->Update();
	pBtnBoneDel->GetAction()->Update();
	pBtnVertexPositionSetAdd->GetAction()->Update();
	pBtnVertexPositionSetDel->GetAction()->Update();
	
	UpdateTarget();
}

void aeWPAPanelRule::UpdateTargetList(){
	pCBTarget->RemoveAllItems();
	if(GetRule()){
		AddTarget("Blend Factor", &GetRule()->GetTargetBlendFactor());
	}
}

void aeWPAPanelRule::UpdateTarget(){
	if(pTarget){
		const int linkCount = pTarget->GetLinkCount();
		int i;
		
		pListLinks->RemoveAllItems();
		for(i=0; i<linkCount; i++){
			aeLink * const link = pTarget->GetLinkAt(i);
			pListLinks->AddItem(link->GetName(), NULL, link);
		}
		pListLinks->SortItems();
		
	}else{
		pListLinks->RemoveAllItems();
	}
	
	pListLinks->SetEnabled(pTarget);
	
	pBtnLinkAdd->GetAction()->Update();
}

const decString &aeWPAPanelRule::GetCBBoneText() const{
	return pCBBones->GetText();
}

void aeWPAPanelRule::SetCBBoneText(const char *text){
	pCBBones->SetText(text);
}

const char *aeWPAPanelRule::GetListBoneSelection() const{
	return pListBones->GetSelectedItem() ? pListBones->GetSelectedItem()->GetText().GetString() : NULL;
}

const decString &aeWPAPanelRule::GetCBVertexPositionSetText() const{
	return pCBVertexPositionSets->GetText();
}

void aeWPAPanelRule::SetCBVertexPositionSetText(const char *text){
	pCBVertexPositionSets->SetText(text);
}

const char *aeWPAPanelRule::GetListVertexPositionSetSelection() const{
	return pListVertexPositionSets->GetSelectedItem()
		? pListVertexPositionSets->GetSelectedItem()->GetText().GetString() : nullptr;
}

aeLink *aeWPAPanelRule::GetCBLinkSelection() const{
	return pCBLinks->GetSelectedItem() ? (aeLink*)pCBLinks->GetSelectedItem()->GetData() : NULL;
}

void aeWPAPanelRule::SetCBLinkSelection(aeLink *selection){
	pCBLinks->SetSelectionWithData(selection);
}

aeLink *aeWPAPanelRule::GetListLinkSelection() const{
	return pListLinks->GetSelectedItem() ? (aeLink*)pListLinks->GetSelectedItem()->GetData() : NULL;
}

void aeWPAPanelRule::SetListLinkSelection(aeLink *selection){
	pListLinks->SetSelectionWithData(selection);
}



void aeWPAPanelRule::RemoveAllTargets(){
	pCBTarget->RemoveAllItems();
}

void aeWPAPanelRule::AddTarget(const char *name, aeControllerTarget *target){
	pCBTarget->AddItem(name, NULL, target);
}
