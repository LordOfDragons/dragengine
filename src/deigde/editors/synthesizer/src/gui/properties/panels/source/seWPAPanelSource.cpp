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

#include "seWPAPanelSource.h"
#include "../../seWPSource.h"
#include "../../seWPEffect.h"
#include "../../../seWindowMain.h"
#include "../../../seViewSynthesizer.h"
#include "../../../../synthesizerEditor.h"
#include "../../../../synthesizer/seSynthesizer.h"
#include "../../../../synthesizer/controller/seController.h"
#include "../../../../synthesizer/controller/seControllerTarget.h"
#include "../../../../synthesizer/link/seLink.h"
#include "../../../../synthesizer/source/seSource.h"
#include "../../../../undosys/source/seUSetSourceName.h"
#include "../../../../undosys/source/seUSetSourceMixMode.h"
#include "../../../../undosys/source/seUSetSourceBlendFactor.h"
#include "../../../../undosys/source/seUSetSourceMinVolume.h"
#include "../../../../undosys/source/seUSetSourceMaxVolume.h"
#include "../../../../undosys/source/seUSetSourceMinPanning.h"
#include "../../../../undosys/source/seUSetSourceMaxPanning.h"
#include "../../../../undosys/source/seUSourceToggleEnabled.h"
#include "../../../../undosys/source/seUSourceTargetAddLink.h"
#include "../../../../undosys/source/seUSourceTargetRemoveLink.h"

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
	seWPAPanelSource &pPanel;
	
public:
	cBaseTextFieldListener(seWPAPanelSource &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		seSource * const source = pPanel.GetSource();
		if(!source){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, source));
		if(undo){
			source->GetSynthesizer()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, seSource *source) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	seWPAPanelSource &pPanel;
	
public:
	cBaseComboBoxListener(seWPAPanelSource &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		seSource * const source = pPanel.GetSource();
		if(!source){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(comboBox, source));
		if(undo){
			source->GetSynthesizer()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox *comboBox, seSource *source) = 0;
};

class cBaseAction : public igdeAction{
protected:
	seWPAPanelSource &pPanel;
	
public:
	cBaseAction(seWPAPanelSource &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	virtual void OnAction(){
		seSource * const source = pPanel.GetSource();
		if(!source){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(source));
		if(undo){
			source->GetSynthesizer()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(seSource *source) = 0;
};


class cTextName : public cBaseTextFieldListener {
public:
	typedef deTObjectReference<cTextName> Ref;
	cTextName(seWPAPanelSource &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(igdeTextField *textField, seSource *source){
		return textField->GetText() != source->GetName()
			? seUSetSourceName::Ref::New(source, textField->GetText()) : igdeUndo::Ref();
	}
};

class cComboMixMode : public cBaseComboBoxListener {
public:
	typedef deTObjectReference<cComboMixMode> Ref;
	cComboMixMode(seWPAPanelSource &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(igdeComboBox *comboBox, seSource *source){
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if(!selection){
			return {};
		}
		
		const deSynthesizerSource::eMixModes mixMode =
			(deSynthesizerSource::eMixModes)(intptr_t)selection->GetData();
		return mixMode != source->GetMixMode() ? seUSetSourceMixMode::Ref::New(source, mixMode) : igdeUndo::Ref();
	}
};

class cTextBlendFactor : public cBaseTextFieldListener {
public:
	typedef deTObjectReference<cTextBlendFactor> Ref;
	cTextBlendFactor(seWPAPanelSource &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(igdeTextField *textField, seSource *source){
		const float value = textField->GetFloat();
		return fabsf(value - source->GetBlendFactor()) > FLOAT_SAFE_EPSILON
			? seUSetSourceBlendFactor::Ref::New(source, value) : igdeUndo::Ref();
	}
};

class cTextMinVolume : public cBaseTextFieldListener {
public:
	typedef deTObjectReference<cTextMinVolume> Ref;
	cTextMinVolume(seWPAPanelSource &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(igdeTextField *textField, seSource *source){
		const float value = textField->GetFloat();
		return fabsf(value - source->GetMinVolume()) > FLOAT_SAFE_EPSILON
			? seUSetSourceMinVolume::Ref::New(source, value) : igdeUndo::Ref();
	}
};

class cTextMaxVolume : public cBaseTextFieldListener {
public:
	typedef deTObjectReference<cTextMaxVolume> Ref;
	cTextMaxVolume(seWPAPanelSource &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(igdeTextField *textField, seSource *source){
		const float value = textField->GetFloat();
		return fabsf(value - source->GetMaxVolume()) > FLOAT_SAFE_EPSILON
			? seUSetSourceMaxVolume::Ref::New(source, value) : igdeUndo::Ref();
	}
};

class cTextMinPanning : public cBaseTextFieldListener {
public:
	typedef deTObjectReference<cTextMinPanning> Ref;
	cTextMinPanning(seWPAPanelSource &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(igdeTextField *textField, seSource *source){
		const float value = textField->GetFloat();
		return fabsf(value - source->GetMinPanning()) > FLOAT_SAFE_EPSILON
			? seUSetSourceMinPanning::Ref::New(source, value) : igdeUndo::Ref();
	}
};

class cTextMaxPanning : public cBaseTextFieldListener {
public:
	typedef deTObjectReference<cTextMaxPanning> Ref;
	cTextMaxPanning(seWPAPanelSource &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(igdeTextField *textField, seSource *source){
		const float value = textField->GetFloat();
		return fabsf(value - source->GetMaxPanning()) > FLOAT_SAFE_EPSILON
			? seUSetSourceMaxPanning::Ref::New(source, value) : igdeUndo::Ref();
	}
};

class cActionEnable : public cBaseAction {
public:
	typedef deTObjectReference<cActionEnable> Ref;
	cActionEnable(seWPAPanelSource &panel) : cBaseAction(panel, "Enable source",
		NULL, "Determines if the source is affecting the model"){ }
	
	virtual igdeUndo::Ref OnAction(seSource *source){
		return seUSourceToggleEnabled::Ref::New(source);
	}
};


class cComboTarget : public cBaseComboBoxListener {
public:
	typedef deTObjectReference<cComboTarget> Ref;
	cComboTarget(seWPAPanelSource &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(igdeComboBox*, seSource*){
		pPanel.UpdateTarget();
		return {};
	}
};

class cListLinks : public igdeListBoxListener{
	seWPAPanelSource &pPanel;
	
public:
	typedef deTObjectReference<cListLinks> Ref;
	cListLinks(seWPAPanelSource &panel) : pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(menu, pPanel.GetActionLinkAdd());
		helper.MenuCommand(menu, pPanel.GetActionLinkRemove());
	}
};

class cActionLinkAdd : public cBaseAction {
public:
	typedef deTObjectReference<cActionLinkAdd> Ref;
	cActionLinkAdd(seWPAPanelSource &panel) : cBaseAction(panel, "Add",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add link"){}
	
	igdeUndo::Ref OnAction(seSource *source) override{
		seControllerTarget * const target = pPanel.GetTarget();
		seLink * const link = pPanel.GetCBLink();
		return target && link && !target->GetLinks().Has(link)
			? seUSourceTargetAddLink::Ref::New(source, target, link) : igdeUndo::Ref();
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
	cActionLinkRemove(seWPAPanelSource &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove link"){}
	
	igdeUndo::Ref OnAction(seSource *source) override{
		seControllerTarget * const target = pPanel.GetTarget();
		seLink * const link = pPanel.GetListLink();
		return target && link && target->GetLinks().Has(link)
			? seUSourceTargetRemoveLink::Ref::New(source, target, link) : igdeUndo::Ref();
	}
	
	void Update() override{
		seControllerTarget * const target = pPanel.GetTarget();
		seLink * const link = pPanel.GetListLink();
		SetSelected(target && link && target->GetLinks().Has(link));
	}
};

}



// Class seWPAPanelSource
///////////////////////////

// Constructor, destructor
////////////////////////////

seWPAPanelSource::seWPAPanelSource(seWPSource &wpSource,
	deSynthesizerSourceVisitorIdentify::eSourceTypes requiredType) :
igdeContainerFlow(wpSource.GetEnvironment(), igdeContainerFlow::eaY),
pWPSource(wpSource),
pRequiredType(requiredType),
pWPEffect(nullptr)
{
	igdeEnvironment &env = wpSource.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref groupBox, form, formLine;
	
	
	pActionLinkAdd = cActionLinkAdd::Ref::New(*this);
	pActionLinkRemove = cActionLinkRemove::Ref::New(*this);
	
	
	// general settings
	helper.GroupBox(*this, groupBox, "General Settings:");
	helper.EditString(groupBox, "Name:", "Name of the source", pEditName, cTextName::Ref::New(*this));
	
	helper.ComboBox(groupBox, "Mix Mode:", "Set mix mode", pCBMixMode, cComboMixMode::Ref::New(*this));
	pCBMixMode->AddItem("Blend", nullptr, (void*)(intptr_t)deSynthesizerSource::emmBlend);
	pCBMixMode->AddItem("Add", nullptr, (void*)(intptr_t)deSynthesizerSource::emmAdd);
	
	helper.EditFloat(groupBox, "Blend Factor:", "Set blend factor", pEditBlendFactor, cTextBlendFactor::Ref::New(*this));
	helper.EditFloat(groupBox, "Minimum Volume:", "Minimum volume", pEditMinVolume, cTextMinVolume::Ref::New(*this));
	helper.EditFloat(groupBox, "Maximum Volume:", "Maximum volume", pEditMaxVolume, cTextMaxVolume::Ref::New(*this));
	helper.EditFloat(groupBox, "Minimum Panning:", "Minimum panning", pEditMinPanning, cTextMinPanning::Ref::New(*this));
	helper.EditFloat(groupBox, "Maximum Panning:", "Maximum panning", pEditMaxPanning, cTextMaxPanning::Ref::New(*this));
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
	
	
	
	// effects
	helper.GroupBoxFlow(*this, groupBox, "Effects:", false, true);
	pWPEffect = seWPEffect::Ref::New(wpSource.GetViewSynthesizer());
	groupBox->AddChild(pWPEffect);
}

seWPAPanelSource::~seWPAPanelSource(){
}



// Management
///////////////

seWindowMain &seWPAPanelSource::GetWindowMain() const{
	return pWPSource.GetViewSynthesizer().GetWindowMain();
}

seSynthesizer *seWPAPanelSource::GetSynthesizer() const{
	return pWPSource.GetSynthesizer();
}

seSource *seWPAPanelSource::GetSource() const{
	seSource * const source = pWPSource.GetSource();
	return source && source->GetType() == pRequiredType ? source : nullptr;
}

seControllerTarget *seWPAPanelSource::GetTarget() const{
	const igdeListItem * const selection = pCBTarget->GetSelectedItem();
	return selection ? (seControllerTarget*)selection->GetData() : nullptr;
}

seLink *seWPAPanelSource::GetCBLink() const{
	const igdeListItem * const selection = pCBLinks->GetSelectedItem();
	return selection ? (seLink*)selection->GetData() : nullptr;
}

seLink *seWPAPanelSource::GetListLink() const{
	const igdeListItem * const selection = pListLinks->GetSelectedItem();
	return selection ? (seLink*)selection->GetData() : nullptr;
}



void seWPAPanelSource::SetSynthesizer(seSynthesizer *synthesizer){
	RemoveAllTargets();
	pWPEffect->SetSynthesizer(synthesizer);
}

void seWPAPanelSource::OnSynthesizerPathChanged(){
}

void seWPAPanelSource::OnActivated(){
	UpdateLinkList();
	UpdateTargetList();
	UpdateControllerList();
	
	UpdateSource();
	
	if(!pCBTarget->GetSelectedItem() && pCBTarget->GetItems().IsNotEmpty()){
		pCBTarget->SetSelection(0);
	}
}

void seWPAPanelSource::OnDeactivated(){
	RemoveAllTargets();
}

void seWPAPanelSource::RemoveAllTargets(){
	pCBTarget->RemoveAllItems();
}

void seWPAPanelSource::AddTarget(const char *name, seControllerTarget *target){
	pCBTarget->AddItem(name, nullptr, target);
}



void seWPAPanelSource::UpdateSynthesizer(){
	pWPEffect->SetSynthesizer(GetSynthesizer());
}

void seWPAPanelSource::UpdateControllerList(){
	pWPEffect->UpdateControllerList();
}

void seWPAPanelSource::UpdateLinkList(){
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
	
	pWPEffect->UpdateLinkList();
}

void seWPAPanelSource::UpdateSource(){
	const seSource * const source = GetSource();
	
	if(source){
		pEditName->SetText(source->GetName());
		pCBMixMode->SetSelectionWithData((void*)(intptr_t)source->GetMixMode());
		pEditBlendFactor->SetFloat(source->GetBlendFactor());
		pEditMinVolume->SetFloat(source->GetMinVolume());
		pEditMaxVolume->SetFloat(source->GetMaxVolume());
		pEditMinPanning->SetFloat(source->GetMinPanning());
		pEditMaxPanning->SetFloat(source->GetMaxPanning());
		pChkEnabled->SetChecked(source->GetEnabled());
		
	}else{
		pEditName->ClearText();
		pCBMixMode->SetSelectionWithData((void*)(intptr_t)deSynthesizerSource::emmBlend);
		pEditBlendFactor->ClearText();
		pEditMinVolume->ClearText();
		pEditMaxVolume->ClearText();
		pEditMinPanning->ClearText();
		pEditMaxPanning->ClearText();
		pChkEnabled->SetChecked(false);
	}
	
	const bool enabled = source;
	pEditName->SetEnabled(enabled);
	pCBMixMode->SetEnabled(enabled);
	pEditBlendFactor->SetEnabled(enabled);
	pEditMinVolume->SetEnabled(enabled);
	pEditMaxVolume->SetEnabled(enabled);
	pEditMinPanning->SetEnabled(enabled);
	pEditMaxPanning->SetEnabled(enabled);
	pChkEnabled->SetEnabled(enabled);
	
	UpdateTarget();
}

void seWPAPanelSource::UpdateTargetList(){
	pCBTarget->RemoveAllItems();
	
	seSource * const source = GetSource();
	if(source){
		AddTarget("Blend Factor", source->GetTargetBlendFactor());
		AddTarget("Volume", source->GetTargetVolume());
		AddTarget("Panning", source->GetTargetPanning());
	}
}

void seWPAPanelSource::UpdateTarget(){
	seControllerTarget * const target = GetTarget();
	
	pListLinks->RemoveAllItems();
	
	if(target){
		target->GetLinks().Visit([&](seLink *link){
			pListLinks->AddItem(link->GetName(), nullptr, link);
		});
		
		pListLinks->SortItems();
	}
	
	pListLinks->SetEnabled(target);
	pBtnLinkAdd->GetAction()->Update();
}
