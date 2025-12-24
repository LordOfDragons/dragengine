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
	typedef deTObjectReference<cBaseAction> Ref;
	cBaseAction(aeWPAPanelRule &panel, const char *text, igdeIcon *icon, const char *description) :
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

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	aeWPAPanelRule &pPanel;
	
public:
	typedef deTObjectReference<cBaseTextFieldListener> Ref;
	cBaseTextFieldListener(aeWPAPanelRule &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRule * const rule = pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, animator, rule));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator *animator, aeRule *rule) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	aeWPAPanelRule &pPanel;
	
public:
	typedef deTObjectReference<cBaseComboBoxListener> Ref;
	cBaseComboBoxListener(aeWPAPanelRule &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeRule * const rule = pPanel.GetRule();
		if(!animator || !rule){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(comboBox, animator, rule));
		if(undo){
			animator->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox *comboBox, aeAnimator *animator, aeRule *rule) = 0;
};


class cTextName : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextName> Ref;
	cTextName(aeWPAPanelRule &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator*, aeRule *rule) override{
		return textField->GetText() != rule->GetName() ? aeUSetRuleName::Ref::New(rule, textField->GetText()) : aeUSetRuleName::Ref();
	}
};

class cComboBlendMode : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboBlendMode> Ref;
	cComboBlendMode(aeWPAPanelRule &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, aeAnimator*, aeRule *rule) override{
		if(!comboBox->GetSelectedItem()){
			return {};
		}
		
		const deAnimatorRule::eBlendModes mode =
			(deAnimatorRule::eBlendModes)(intptr_t)comboBox->GetSelectedItem()->GetData();
		return mode != rule->GetBlendMode() ? aeUSetRuleBlendMode::Ref::New(rule, mode) : aeUSetRuleBlendMode::Ref();
	}
};

class cTextBlendFactor : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextBlendFactor> Ref;
	cTextBlendFactor(aeWPAPanelRule &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, aeAnimator*, aeRule *rule) override{
		const float factor = textField->GetFloat();
		return fabsf(factor - rule->GetBlendFactor()) > FLOAT_SAFE_EPSILON
			? aeUSetRuleBlendFactor::Ref::New(rule, factor) : igdeUndo::Ref();
	}
};

class cActionInvertBlendFactor : public cBaseAction{
public:
	typedef deTObjectReference<cActionInvertBlendFactor> Ref;
	
public:
	cActionInvertBlendFactor(aeWPAPanelRule &panel) : cBaseAction(panel,
		"Invert Blend Factor", nullptr, "Use '1 - blendFactor' instead of 'blendFactor'"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRule *rule) override{
		return aeUToggleRuleInvertBlendFactor::Ref::New(rule);
	}
	
	void Update(const aeAnimator &, const aeRule &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetInvertBlendFactor());
	}
};

class cActionEnabled : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnabled> Ref;
	
public:
	cActionEnabled(aeWPAPanelRule &panel) : cBaseAction(panel, "Enable Rule", nullptr,
		"Determines if the rule is affecting animation"){ }
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRule *rule) override{
		return aeUSetRuleEnabled::Ref::New(rule);
	}
	
	void Update(const aeAnimator &, const aeRule &rule) override{
		SetEnabled(true);
		SetSelected(rule.GetEnabled());
	}
};


class cActionBoneAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionBoneAdd> Ref;
	
public:
	cActionBoneAdd(aeWPAPanelRule &panel) : cBaseAction(panel, "Add",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add bone"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRule *rule) override{
		const decString &name = pPanel.GetCBBoneText();
		return !name.IsEmpty() && !rule->GetListBones().Has(name)
			? aeUSetRuleAddBone::Ref::New(rule, name) : igdeUndo::Ref();
	}
	
	void Update(const aeAnimator &, const aeRule &rule) override{
		const decString &name = pPanel.GetCBBoneText();
		SetEnabled(!name.IsEmpty() && !rule.GetListBones().Has(name));
	}
};

class cActionBoneRemoves : public cBaseAction{
public:
	typedef deTObjectReference<cActionBoneRemoves> Ref;
	
public:
	cActionBoneRemoves(aeWPAPanelRule &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove bones"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRule *rule) override{
		const decString &name = pPanel.GetCBBoneText();
		if(name.IsEmpty()){
			return {};
		}
		const aeUSetRuleRemoveBone::Ref undo(aeUSetRuleRemoveBone::Ref::New(rule, name));
		return undo->HasBones() ? undo : aeUSetRuleRemoveBone::Ref();
	}
	
	void Update(const aeAnimator &, const aeRule &rule) override{
		SetEnabled(!pPanel.GetCBBoneText().IsEmpty());
	}
};

class cActionBoneRemovesAll : public cBaseAction{
public:
	typedef deTObjectReference<cActionBoneRemovesAll> Ref;
	
public:
	cActionBoneRemovesAll(aeWPAPanelRule &panel) : cBaseAction(panel, "Remove All",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove all bones"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRule *rule) override{
		return rule->GetListBones().GetCount() > 0
			? aeUSetRuleRemoveAllBones::Ref::New(rule) : igdeUndo::Ref();
	}
	
	void Update(const aeAnimator &, const aeRule &rule) override{
		SetEnabled(rule.GetListBones().GetCount() > 0);
	}
};

class cActionBoneSelectedRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionBoneSelectedRemove> Ref;
	
public:
	cActionBoneSelectedRemove(aeWPAPanelRule &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove selected bone"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRule *rule) override{
		const char * const name = pPanel.GetListBoneSelection();
		return name ? aeUSetRuleRemoveBone::Ref::New(rule, name) : aeUSetRuleRemoveBone::Ref();
	}
	
	void Update(const aeAnimator &, const aeRule &rule) override{
		SetEnabled(pPanel.GetListBoneSelection());
	}
};

class cActionMirrorRigBones : public cBaseAction{
public:
	typedef deTObjectReference<cActionMirrorRigBones> Ref;
	
public:
	cActionMirrorRigBones(aeWPAPanelRule &panel) : cBaseAction(panel, "Mirror Bones",
		nullptr, "Mirror rig bones"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRule *rule) override{
		// TODO add a dialog to allow changing the mirror parameter (or add a new menu command)
		return aeURuleMirrorBones::Ref::New(rule);
	}
	
	void Update(const aeAnimator &, const aeRule &rule) override{
		SetEnabled(rule.GetListBones().GetCount() > 0);
	}
};

class cActionCopyBones : public cBaseAction{
public:
	typedef deTObjectReference<cActionCopyBones> Ref;
	
public:
	cActionCopyBones(aeWPAPanelRule &panel) : cBaseAction(panel, "Copy",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy), "Copy bones"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRule *rule) override{
		pPanel.GetWindowMain().GetClipboard().Set(
			aeClipboardDataBones::Ref::New(rule->GetListBones()));
		return {};
	}
};

class cActionPasteBones : public cBaseAction{
public:
	typedef deTObjectReference<cActionPasteBones> Ref;
	
public:
	cActionPasteBones(aeWPAPanelRule &panel) : cBaseAction(panel, "Paste",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy), "Copy bones"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRule *rule) override{
		aeClipboardDataBones * const clip = (aeClipboardDataBones*)pPanel.GetWindowMain()
			.GetClipboard().GetWithTypeName(aeClipboardDataBones::TYPE_NAME);
		if(!clip){
			return {};
		}
		
		const aeUSetRuleBones::Ref undo = aeUSetRuleBones::Ref::New(rule, rule->GetListBones() + clip->GetBones());
		undo->SetShortInfo("Rule paste bones");
		return undo;
	}
	
	void Update(const aeAnimator &, const aeRule &) override{
		SetEnabled(pPanel.GetWindowMain().GetClipboard().HasWithTypeName(aeClipboardDataBones::TYPE_NAME));
	}
};

class cActionExportBones : public cBaseAction{
public:
	typedef deTObjectReference<cActionExportBones> Ref;
	
public:
	cActionExportBones(aeWPAPanelRule &panel) : cBaseAction(panel, "Export To Text",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave), "Export bones"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRule *rule) override{
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
		return {};
	}
	
	void Update(const aeAnimator &, const aeRule &rule) override{
		SetEnabled(rule.GetListBones().GetCount() > 0);
	}
};

class cActionImportBones : public cBaseAction{
public:
	typedef deTObjectReference<cActionImportBones> Ref;
	
public:
	cActionImportBones(aeWPAPanelRule &panel) : cBaseAction(panel, "Import From Text",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen), "Import bones"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRule *rule) override{
		decString text;
		while(true){
			if(!igdeCommonDialogs::GetMultilineString(pPanel.GetParentWindow(),
			"Import From Text", "Bones. One bone per line.", text)){
				return {};
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
		
		const aeUSetRuleBones::Ref undo = aeUSetRuleBones::Ref::New(rule, rule->GetListBones() + bones);
		undo->SetShortInfo("Rule import bones");
		return undo;
	}
};


class cListBones : public igdeListBoxListener{
	aeWPAPanelRule &pPanel;
	
public:
	typedef deTObjectReference<cListBones> Ref;
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
		
		helper.MenuCommand(menu, cActionBoneAdd::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionBoneSelectedRemove::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionBoneRemovesAll::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionMirrorRigBones::Ref::New(pPanel));
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, cActionCopyBones::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionPasteBones::Ref::New(pPanel));
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, cActionExportBones::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionImportBones::Ref::New(pPanel));
	}
};


class cActionVertexPositionSetAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionVertexPositionSetAdd> Ref;
	
public:
	cActionVertexPositionSetAdd(aeWPAPanelRule &panel) : cBaseAction(panel, "Add",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add vertex position set"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRule *rule) override{
		const decString &name = pPanel.GetCBVertexPositionSetText();
		return !name.IsEmpty() && !rule->GetListVertexPositionSets().Has(name)
			? aeUSetRuleAddVertexPositionSet::Ref::New(rule, name) : igdeUndo::Ref();
	}
	
	void Update(const aeAnimator &, const aeRule &rule) override{
		const decString &name = pPanel.GetCBVertexPositionSetText();
		SetEnabled(!name.IsEmpty() && !rule.GetListVertexPositionSets().Has(name));
	}
};

class cActionVertexPositionSetRemoves : public cBaseAction{
public:
	typedef deTObjectReference<cActionVertexPositionSetRemoves> Ref;
	
public:
	cActionVertexPositionSetRemoves(aeWPAPanelRule &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove vertex position sets"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRule *rule) override{
		const decString &name = pPanel.GetCBVertexPositionSetText();
		if(name.IsEmpty()){
			return {};
		}
		const aeUSetRuleRemoveVertexPositionSet::Ref undo(aeUSetRuleRemoveVertexPositionSet::Ref::New(rule, name));
		return undo->HasVertexPositionSets() ? undo : aeUSetRuleRemoveVertexPositionSet::Ref();
	}
	
	void Update(const aeAnimator &, const aeRule &rule) override{
		SetEnabled(!pPanel.GetCBVertexPositionSetText().IsEmpty());
	}
};

class cActionVertexPositionSetRemovesAll : public cBaseAction{
public:
	typedef deTObjectReference<cActionVertexPositionSetRemovesAll> Ref;
	
public:
	cActionVertexPositionSetRemovesAll(aeWPAPanelRule &panel) : cBaseAction(panel, "Remove All",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove all vertex position sets"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRule *rule) override{
		return rule->GetListVertexPositionSets().GetCount() > 0
			? aeUSetRuleRemoveAllVertexPositionSets::Ref::New(rule) : igdeUndo::Ref();
	}
	
	void Update(const aeAnimator &, const aeRule &rule) override{
		SetEnabled(rule.GetListVertexPositionSets().GetCount() > 0);
	}
};

class cActionVertexPositionSetSelectedRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionVertexPositionSetSelectedRemove> Ref;
	
public:
	cActionVertexPositionSetSelectedRemove(aeWPAPanelRule &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove selected vertex position set"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRule *rule) override{
		const char * const name = pPanel.GetListVertexPositionSetSelection();
		return name ? aeUSetRuleRemoveVertexPositionSet::Ref::New(rule, name) : aeUSetRuleRemoveVertexPositionSet::Ref();
	}
	
	void Update(const aeAnimator &, const aeRule &rule) override{
		SetEnabled(pPanel.GetListVertexPositionSetSelection());
	}
};

class cActionMirrorRigVertexPositionSets : public cBaseAction{
public:
	typedef deTObjectReference<cActionMirrorRigVertexPositionSets> Ref;
	
public:
	cActionMirrorRigVertexPositionSets(aeWPAPanelRule &panel) : cBaseAction(panel,
		"Mirror", nullptr, "Mirror vertex position sets"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRule *rule) override{
		// TODO add a dialog to allow changing the mirror parameter (or add a new menu command)
		return aeURuleMirrorVertexPositionSets::Ref::New(rule);
	}
	
	void Update(const aeAnimator &, const aeRule &rule) override{
		SetEnabled(rule.GetListVertexPositionSets().GetCount() > 0);
	}
};

class cActionCopyVertexPositionSets : public cBaseAction{
public:
	typedef deTObjectReference<cActionCopyVertexPositionSets> Ref;
	
public:
	cActionCopyVertexPositionSets(aeWPAPanelRule &panel) : cBaseAction(panel, "Copy",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy), "Copy vertex position set"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRule *rule) override{
		pPanel.GetWindowMain().GetClipboard().Set(
			aeClipboardDataVertexPositionSets::Ref::New(rule->GetListVertexPositionSets()));
		return {};
	}
};

class cActionPasteVertexPositionSets : public cBaseAction{
public:
	typedef deTObjectReference<cActionPasteVertexPositionSets> Ref;
	
public:
	cActionPasteVertexPositionSets(aeWPAPanelRule &panel) : cBaseAction(panel, "Paste",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy), "Copy vertex position sets"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRule *rule) override{
		aeClipboardDataVertexPositionSets * const clip = (aeClipboardDataVertexPositionSets*)pPanel.GetWindowMain()
			.GetClipboard().GetWithTypeName(aeClipboardDataVertexPositionSets::TYPE_NAME);
		if(!clip){
			return {};
		}
		
		const aeUSetRuleVertexPositionSets::Ref undo = aeUSetRuleVertexPositionSets::Ref::New(
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
	typedef deTObjectReference<cActionExportVertexPositionSets> Ref;
	
public:
	cActionExportVertexPositionSets(aeWPAPanelRule &panel) : cBaseAction(panel, "Export To Text",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSave), "Export vertex position set"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRule *rule) override{
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
		return {};
	}
	
	void Update(const aeAnimator &, const aeRule &rule) override{
		SetEnabled(rule.GetListVertexPositionSets().GetCount() > 0);
	}
};

class cActionImportVertexPositionSets : public cBaseAction{
public:
	typedef deTObjectReference<cActionImportVertexPositionSets> Ref;
	
public:
	cActionImportVertexPositionSets(aeWPAPanelRule &panel) : cBaseAction(panel, "Import From Text",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiOpen), "Import vertex position sets"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRule *rule) override{
		decString text;
		while(true){
			if(!igdeCommonDialogs::GetMultilineString(pPanel.GetParentWindow(),
			"Import From Text", "Vertex position sets. One vertex position set per line.", text)){
				return {};
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
		
		const aeUSetRuleVertexPositionSets::Ref undo = aeUSetRuleVertexPositionSets::Ref::New(
			rule, rule->GetListVertexPositionSets() + vpslist);
		undo->SetShortInfo("Rule import vertex position sets");
		return undo;
	}
};

class cListVertexPositionSets : public igdeListBoxListener{
	aeWPAPanelRule &pPanel;
	
public:
	typedef deTObjectReference<cListVertexPositionSets> Ref;
	cListVertexPositionSets(aeWPAPanelRule &panel) : pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu){
		if(!pPanel.GetRule()){
			return;
		}
		
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		
		helper.MenuCommand(menu, cActionVertexPositionSetAdd::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionVertexPositionSetSelectedRemove::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionVertexPositionSetRemovesAll::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionMirrorRigVertexPositionSets::Ref::New(pPanel));
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, cActionCopyVertexPositionSets::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionPasteVertexPositionSets::Ref::New(pPanel));
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, cActionExportVertexPositionSets::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionImportVertexPositionSets::Ref::New(pPanel));
	}
};


class cComboTarget : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboTarget> Ref;
	cComboTarget(aeWPAPanelRule &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, aeAnimator*, aeRule*) override{
		pPanel.SetTarget(comboBox->GetSelectedItem()
			? (aeControllerTarget*)comboBox->GetSelectedItem()->GetData() : nullptr);
		return {};
	}
};


class cActionLinkAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionLinkAdd> Ref;
	
public:
	cActionLinkAdd(aeWPAPanelRule &panel) : cBaseAction(panel, "Add",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add link"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRule *rule) override{
		aeControllerTarget * const target = pPanel.GetTarget();
		aeLink * const link = pPanel.GetCBLinkSelection();
		return target && link && !target->GetLinks().Has(link)
			? aeURuleTargetAddLink::Ref::New(rule, target, link) : igdeUndo::Ref();
	}
	
	void Update(const aeAnimator &, const aeRule &) override{
		const aeControllerTarget * const target = pPanel.GetTarget();
		aeLink * const link = pPanel.GetCBLinkSelection();
		SetEnabled(target && link && !target->GetLinks().Has(link));
	}
};

class cActionLinkRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionLinkRemove> Ref;
	
public:
	cActionLinkRemove(aeWPAPanelRule &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove link"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRule *rule) override{
		aeControllerTarget * const target = pPanel.GetTarget();
		aeLink * const link = pPanel.GetListLinkSelection();
		return target && link && target->GetLinks().Has(link)
			? aeURuleTargetRemoveLink::Ref::New(rule, target, link) : igdeUndo::Ref();
	}
	
	void Update(const aeAnimator &, const aeRule &) override{
		const aeControllerTarget * const target = pPanel.GetTarget();
		aeLink * const link = pPanel.GetListLinkSelection();
		SetEnabled(target && link && target->GetLinks().Has(link));
	}
};

class cActionLinkRemoveAll : public cBaseAction{
public:
	typedef deTObjectReference<cActionLinkRemoveAll> Ref;
	
public:
	cActionLinkRemoveAll(aeWPAPanelRule &panel) : cBaseAction(panel, "Remove All",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove all links"){}
	
	igdeUndo::Ref OnAction(aeAnimator*, aeRule *rule) override{
		aeControllerTarget * const target = pPanel.GetTarget();
		if(!target || target->GetLinks().IsEmpty()){
			return {};
		}
		
		const int count = target->GetLinks().GetCount();
		aeLink::List list;
		int i;
		for(i=0; i<count; i++){
			list.Add(target->GetLinks().GetAt(i));
		}
		return aeURuleTargetRemoveAllLinks::Ref::New(rule, target, list);
	}
	
	void Update(const aeAnimator &, const aeRule &) override{
		const aeControllerTarget * const target = pPanel.GetTarget();
		SetEnabled(target && target->GetLinks().IsNotEmpty());
	}
};


class cListLinks : public igdeListBoxListener{
	aeWPAPanelRule &pPanel;
	
public:
	typedef deTObjectReference<cListLinks> Ref;
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
		
		helper.MenuCommand(menu, cActionLinkAdd::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionLinkRemove::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionLinkRemoveAll::Ref::New(pPanel));
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
pTarget(nullptr)
{
	igdeEnvironment &env = wpRule.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref groupBox, formLine;
	
	
	// animation
	helper.GroupBox(*this, groupBox, "General Settings:");
	
	helper.EditString(groupBox, "Name:", "Name of the rule", pEditName, cTextName::Ref::New(*this));
	
	helper.ComboBox(groupBox, "Blend Mode:", "Sets the blend mode",
		pCBBlendMode, cComboBlendMode::Ref::New(*this));
	pCBBlendMode->AddItem("Blend", nullptr, (void*)(intptr_t)deAnimatorRule::ebmBlend);
	pCBBlendMode->AddItem("Overlay", nullptr, (void*)(intptr_t)deAnimatorRule::ebmOverlay);
	
	helper.EditFloat(groupBox, "Blend Factor:", "Sets the blend factor",
		pEditBlendFactor, cTextBlendFactor::Ref::New(*this));
	helper.CheckBox(groupBox, pChkInvertBlendFactor, cActionInvertBlendFactor::Ref::New(*this));
	helper.CheckBox(groupBox, pChkEnabled, cActionEnabled::Ref::New(*this));
	
	
	// affected bones
	helper.GroupBoxFlow(*this, groupBox, "Affected bones:", false, true);
	
	formLine = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst);
	groupBox->AddChild(formLine);
	helper.ComboBoxFilter(formLine, true, "Bones", pCBBones, {});
	pCBBones->SetDefaultSorter();
	helper.Button(formLine, pBtnBoneAdd, cActionBoneAdd::Ref::New(*this));
	helper.Button(formLine, pBtnBoneDel, cActionBoneRemoves::Ref::New(*this));
	
	helper.ListBox(groupBox, 4, "Bones affected by rule", pListBones, cListBones::Ref::New(*this));
	pListBones->SetDefaultSorter();
	
	
	// affected vertex position sets
	helper.GroupBoxFlow(*this, groupBox, "Affected Vertex Position Sets:", false, true);
	
	formLine = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst);
	groupBox->AddChild(formLine);
	helper.ComboBoxFilter(formLine, true, "Vertex Position Sets", pCBVertexPositionSets, {});
	pCBVertexPositionSets->SetDefaultSorter();
	helper.Button(formLine, pBtnVertexPositionSetAdd, cActionVertexPositionSetAdd::Ref::New(*this));
	helper.Button(formLine, pBtnVertexPositionSetDel, cActionVertexPositionSetRemoves::Ref::New(*this));
	
	helper.ListBox(groupBox, 4, "Vertex position sets affected by rule",
		pListVertexPositionSets, cListVertexPositionSets::Ref::New(*this));
	pListVertexPositionSets->SetDefaultSorter();
	
	
	// targets
	helper.GroupBoxFlow(*this, groupBox, "Targets and Links:");
	
	formLine = igdeContainerForm::Ref::New(env);
	groupBox->AddChild(formLine);
	helper.ComboBox(formLine, "Target:", "Targets", pCBTarget, cComboTarget::Ref::New(*this));
	
	formLine = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst);
	groupBox->AddChild(formLine);
	helper.ComboBoxFilter(formLine, "Links", pCBLinks, {});
	pCBLinks->SetDefaultSorter();
	helper.Button(formLine, pBtnLinkAdd, cActionLinkAdd::Ref::New(*this));
	
	helper.ListBox(groupBox, 4, "Links affecting target", pListLinks, cListLinks::Ref::New(*this));
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
	return rule && rule->GetType() == pRequiredType ? rule : nullptr;
}

void aeWPAPanelRule::SetTarget(aeControllerTarget *target){
	if(target == pTarget){
		return;
	}
	
	pTarget = target;
	UpdateTarget();
}



void aeWPAPanelRule::OnActivated(){
	SetTarget(nullptr);
	
	UpdateRigBoneList();
	UpdateModelVertexPositionSetList();
	UpdateAnimMoveList();
	
	UpdateLinkList();
	UpdateTargetList();
	UpdateControllerList();
	
	OnAnimatorPathChanged();
	
	if(pCBTarget->GetItems().IsNotEmpty()){
		pCBTarget->SetSelection(0);
	}
}

void aeWPAPanelRule::OnAnimatorChanged(){
	SetTarget(nullptr);
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
		GetAnimator()->GetLinks().Visit([&](aeLink *link){
			pCBLinks->AddItem(link->GetName(), nullptr, link);
		});
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
		pListLinks->RemoveAllItems();
		pTarget->GetLinks().Visit([&](aeLink *link){
			pListLinks->AddItem(link->GetName(), nullptr, link);
		});
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
	return pListBones->GetSelectedItem() ? pListBones->GetSelectedItem()->GetText().GetString() : nullptr;
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
	return pCBLinks->GetSelectedItem() ? (aeLink*)pCBLinks->GetSelectedItem()->GetData() : nullptr;
}

void aeWPAPanelRule::SetCBLinkSelection(aeLink *selection){
	pCBLinks->SetSelectionWithData(selection);
}

aeLink *aeWPAPanelRule::GetListLinkSelection() const{
	return pListLinks->GetSelectedItem() ? (aeLink*)pListLinks->GetSelectedItem()->GetData() : nullptr;
}

void aeWPAPanelRule::SetListLinkSelection(aeLink *selection){
	pListLinks->SetSelectionWithData(selection);
}



void aeWPAPanelRule::RemoveAllTargets(){
	pCBTarget->RemoveAllItems();
}

void aeWPAPanelRule::AddTarget(const char *name, aeControllerTarget *target){
	pCBTarget->AddItem(name, nullptr, target);
}
