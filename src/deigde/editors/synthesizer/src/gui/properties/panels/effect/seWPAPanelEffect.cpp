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

#include "seWPAPanelEffect.h"
#include "../../seWPEffect.h"
#include "../../../seWindowMain.h"
#include "../../../seViewSynthesizer.h"
#include "../../../../synthesizerEditor.h"
#include "../../../../synthesizer/seSynthesizer.h"
#include "../../../../synthesizer/controller/seController.h"
#include "../../../../synthesizer/controller/seControllerTarget.h"
#include "../../../../synthesizer/link/seLink.h"
#include "../../../../synthesizer/effect/seEffect.h"
#include "../../../../undosys/source/effect/seUEffectSetStrength.h"
#include "../../../../undosys/source/effect/seUEffectToggleEnabled.h"
#include "../../../../undosys/source/effect/seUEffectTargetAddLink.h"
#include "../../../../undosys/source/effect/seUEffectTargetRemoveLink.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	seWPAPanelEffect &pPanel;
	
public:
	cBaseTextFieldListener(seWPAPanelEffect &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		seEffect * const effect = pPanel.GetEffect();
		if(!effect){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, effect));
		if(undo){
			effect->GetSynthesizer()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, seEffect *effect) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	seWPAPanelEffect &pPanel;
	
public:
	cBaseComboBoxListener(seWPAPanelEffect &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		seEffect * const effect = pPanel.GetEffect();
		if(!effect){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(comboBox, effect));
		if(undo){
			effect->GetSynthesizer()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox *comboBox, seEffect *effect) = 0;
};

class cBaseAction : public igdeAction{
protected:
	seWPAPanelEffect &pPanel;
	
public:
	cBaseAction(seWPAPanelEffect &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	virtual void OnAction(){
		seEffect * const effect = pPanel.GetEffect();
		if(!effect){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(effect));
		if(undo){
			effect->GetSynthesizer()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(seEffect *effect) = 0;
};


class cTextStrength : public cBaseTextFieldListener {
public:
	typedef deTObjectReference<cTextStrength> Ref;
	cTextStrength(seWPAPanelEffect &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(igdeTextField *textField, seEffect *effect){
		const float value = textField->GetFloat();
		return fabsf(value - effect->GetStrength()) > FLOAT_SAFE_EPSILON
			? seUEffectSetStrength::Ref::New(effect, value) : igdeUndo::Ref();
	}
};

class cActionEnable : public cBaseAction {
public:
	typedef deTObjectReference<cActionEnable> Ref;
	cActionEnable(seWPAPanelEffect &panel) : cBaseAction(panel, "Enable effect",
		NULL, "Determines if the effect is affecting the source"){ }
	
	virtual igdeUndo::Ref OnAction(seEffect *effect){
		return seUEffectToggleEnabled::Ref::New(effect);
	}
};


class cComboTarget : public cBaseComboBoxListener {
public:
	typedef deTObjectReference<cComboTarget> Ref;
	cComboTarget(seWPAPanelEffect &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(igdeComboBox*, seEffect*){
		pPanel.UpdateTarget();
		return {};
	}
};

class cListLinks : public igdeListBoxListener{
	seWPAPanelEffect &pPanel;
	
public:
	typedef deTObjectReference<cListLinks> Ref;
	cListLinks(seWPAPanelEffect &panel) : pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(menu, pPanel.GetActionLinkAdd());
		helper.MenuCommand(menu, pPanel.GetActionLinkRemove());
	}
};

class cActionLinkAdd : public cBaseAction {
public:
	typedef deTObjectReference<cActionLinkAdd> Ref;
	cActionLinkAdd(seWPAPanelEffect &panel) : cBaseAction(panel, "Add",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add link"){}
	
	igdeUndo::Ref OnAction(seEffect *effect) override{
		seControllerTarget * const target = pPanel.GetTarget();
		seLink * const link = pPanel.GetCBLink();
		return target && link && !target->GetLinks().Has(link)
			? seUEffectTargetAddLink::Ref::New(effect, target, link) : igdeUndo::Ref();
	}
	
	void Update() override{
		seControllerTarget * const target = pPanel.GetTarget();
		seLink * const link = pPanel.GetCBLink();
		SetSelected(target && link && !target->GetLinks().Has(link));
	}
};

class cActionLinkRemove : public cBaseAction {
public:
	typedef deTObjectReference<cActionLinkRemove> Ref;
	cActionLinkRemove(seWPAPanelEffect &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove link"){}
	
	igdeUndo::Ref OnAction(seEffect *effect) override{
		seControllerTarget * const target = pPanel.GetTarget();
		seLink * const link = pPanel.GetListLink();
		return target && link && target->GetLinks().Has(link)
			? seUEffectTargetRemoveLink::Ref::New(effect, target, link) : igdeUndo::Ref();
	}
	
	void Update() override{
		seControllerTarget * const target = pPanel.GetTarget();
		seLink * const link = pPanel.GetListLink();
		SetSelected(target && link && target->GetLinks().Has(link));
	}
};

}



// Class seWPAPanelEffect
///////////////////////////

// Constructor, destructor
////////////////////////////

seWPAPanelEffect::seWPAPanelEffect(seWPEffect &wpEffect,
	deSynthesizerEffectVisitorIdentify::eEffectTypes requiredType) :
igdeContainerFlow(wpEffect.GetEnvironment(), igdeContainerFlow::eaY),
pWPEffect(wpEffect),
pRequiredType(requiredType)
{
	igdeEnvironment &env = wpEffect.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref groupBox, form, formLine;
	
	
	pActionLinkAdd = cActionLinkAdd::Ref::New(*this);
	pActionLinkRemove = cActionLinkRemove::Ref::New(*this);
	
	
	// general settings
	helper.GroupBox(*this, groupBox, "General Settings:");
	helper.EditString(groupBox, "Strength:", "Set effect strength", pEditStrength, cTextStrength::Ref::New(*this));
	helper.CheckBox(groupBox, pChkEnabled, cActionEnable::Ref::New(*this));
	
	
	// targets and links
	helper.GroupBoxFlow(*this, groupBox, "Targets and Links:");
	
	form = igdeContainerForm::Ref::New(env);
	groupBox->AddChild(form);
	helper.ComboBox(form, "Target:", "Displays all links of for a given target",
		pCBTarget, cComboTarget::Ref::New(*this));
	
	helper.FormLineStretchFirst(form, "Link:", "Link to add to target", formLine);
	helper.ComboBox(formLine, "Link to add to target", pCBLinks, cComboTarget::Ref::New(*this));
	helper.Button(formLine, pBtnLinkAdd, pActionLinkAdd);
	
	helper.ListBox(groupBox, 4, "Links used by target", pListLinks, cListLinks::Ref::New(*this));
	pListLinks->SetDefaultSorter();
}

seWPAPanelEffect::~seWPAPanelEffect(){
}



// Management
///////////////

seWindowMain &seWPAPanelEffect::GetWindowMain() const{
	return pWPEffect.GetViewSynthesizer().GetWindowMain();
}

seSynthesizer *seWPAPanelEffect::GetSynthesizer() const{
	return pWPEffect.GetSynthesizer();
}

seEffect *seWPAPanelEffect::GetEffect() const{
	return pWPEffect.GetEffect();
}

seControllerTarget *seWPAPanelEffect::GetTarget() const{
	const igdeListItem * const selection = pCBTarget->GetSelectedItem();
	return selection ? (seControllerTarget*)selection->GetData() : nullptr;
}

seLink *seWPAPanelEffect::GetCBLink() const{
	const igdeListItem * const selection = pCBLinks->GetSelectedItem();
	return selection ? (seLink*)selection->GetData() : nullptr;
}

seLink *seWPAPanelEffect::GetListLink() const{
	const igdeListItem * const selection = pListLinks->GetSelectedItem();
	return selection ? (seLink*)selection->GetData() : nullptr;
}



void seWPAPanelEffect::OnActivated(){
	UpdateLinkList();
	UpdateTargetList();
	UpdateControllerList();
	
	UpdateTarget();
}



void seWPAPanelEffect::RemoveAllTargets(){
	pCBTarget->RemoveAllItems();
}

void seWPAPanelEffect::AddTarget(const char *name, seControllerTarget *target){
	pCBTarget->AddItem(name, nullptr, target);
}



void seWPAPanelEffect::UpdateControllerList(){
}

void seWPAPanelEffect::UpdateLinkList(){
	seLink * const selection = pCBLinks->GetSelectedItem() ? (seLink*)pCBLinks->GetSelectedItem()->GetData() : nullptr;
	
	pCBLinks->RemoveAllItems();
	
	const seSynthesizer * const synthesizer = GetSynthesizer();
	if(synthesizer){
		const seLink::List &list = synthesizer->GetLinks();
		const int count = list.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			seLink * const link = list.GetAt(i);
			pCBLinks->AddItem(link->GetName(), nullptr, link);
		}
	}
	
	pCBLinks->SortItems();
	
	pCBLinks->SetSelectionWithData(selection);
}

void seWPAPanelEffect::UpdateEffect(){
	const seEffect * const effect = GetEffect();
	
	if(effect){
		pEditStrength->SetFloat(effect->GetStrength());
		pChkEnabled->SetChecked(effect->GetEnabled());
		
	}else{
		pEditStrength->ClearText();
		pChkEnabled->SetChecked(false);
	}
	
	const bool enabled = effect;
	pEditStrength->SetEnabled(enabled);
	pChkEnabled->SetEnabled(enabled);
	
	UpdateTarget();
}

void seWPAPanelEffect::UpdateTargetList(){
	pCBTarget->RemoveAllItems();
	
	seEffect * const effect = GetEffect();
	if(effect){
		AddTarget("Strength", effect->GetTargetStrength());
	}
}

void seWPAPanelEffect::UpdateTarget(){
	seControllerTarget * const target = GetTarget();
	
	pListLinks->RemoveAllItems();
	
	if(target){
		target->GetLinks().Visit([this](seLink *link){
			pListLinks->AddItem(link->GetName(), nullptr, link);
		});
		pListLinks->SortItems();
	}
	
	pListLinks->SetEnabled(target);
	pBtnLinkAdd->GetAction()->Update();
}
