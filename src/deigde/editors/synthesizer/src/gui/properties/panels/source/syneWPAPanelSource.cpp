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

#include "syneWPAPanelSource.h"
#include "../../syneWPSource.h"
#include "../../syneWPEffect.h"
#include "../../../syneWindowMain.h"
#include "../../../syneViewSynthesizer.h"
#include "../../../../synthesizerEditor.h"
#include "../../../../synthesizer/syneSynthesizer.h"
#include "../../../../synthesizer/controller/syneController.h"
#include "../../../../synthesizer/controller/syneControllerTarget.h"
#include "../../../../synthesizer/link/syneLink.h"
#include "../../../../synthesizer/source/syneSource.h"
#include "../../../../undosys/source/syneUSetSourceName.h"
#include "../../../../undosys/source/syneUSetSourceMixMode.h"
#include "../../../../undosys/source/syneUSetSourceBlendFactor.h"
#include "../../../../undosys/source/syneUSetSourceMinVolume.h"
#include "../../../../undosys/source/syneUSetSourceMaxVolume.h"
#include "../../../../undosys/source/syneUSetSourceMinPanning.h"
#include "../../../../undosys/source/syneUSetSourceMaxPanning.h"
#include "../../../../undosys/source/syneUSourceToggleEnabled.h"
#include "../../../../undosys/source/syneUSourceTargetAddLink.h"
#include "../../../../undosys/source/syneUSourceTargetRemoveLink.h"

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
	syneWPAPanelSource &pPanel;
	
public:
	cBaseTextFieldListener(syneWPAPanelSource &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		syneSource * const source = pPanel.GetSource();
		if(!source){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, source));
		if(undo){
			source->GetSynthesizer()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, syneSource *source) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	syneWPAPanelSource &pPanel;
	
public:
	cBaseComboBoxListener(syneWPAPanelSource &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		syneSource * const source = pPanel.GetSource();
		if(!source){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(comboBox, source));
		if(undo){
			source->GetSynthesizer()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox *comboBox, syneSource *source) = 0;
};

class cBaseAction : public igdeAction{
protected:
	syneWPAPanelSource &pPanel;
	
public:
	cBaseAction(syneWPAPanelSource &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	virtual void OnAction(){
		syneSource * const source = pPanel.GetSource();
		if(!source){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(source));
		if(undo){
			source->GetSynthesizer()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(syneSource *source) = 0;
};


class cTextName : public cBaseTextFieldListener {
public:
	typedef deTObjectReference<cTextName> Ref;
	cTextName(syneWPAPanelSource &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(igdeTextField *textField, syneSource *source){
		return textField->GetText() != source->GetName()
			? syneUSetSourceName::Ref::New(source, textField->GetText()) : igdeUndo::Ref();
	}
};

class cComboMixMode : public cBaseComboBoxListener {
public:
	typedef deTObjectReference<cComboMixMode> Ref;
	cComboMixMode(syneWPAPanelSource &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(igdeComboBox *comboBox, syneSource *source){
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if(!selection){
			return {};
		}
		
		const deSynthesizerSource::eMixModes mixMode =
			(deSynthesizerSource::eMixModes)(intptr_t)selection->GetData();
		return mixMode != source->GetMixMode() ? syneUSetSourceMixMode::Ref::New(source, mixMode) : igdeUndo::Ref();
	}
};

class cTextBlendFactor : public cBaseTextFieldListener {
public:
	typedef deTObjectReference<cTextBlendFactor> Ref;
	cTextBlendFactor(syneWPAPanelSource &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(igdeTextField *textField, syneSource *source){
		const float value = textField->GetFloat();
		return fabsf(value - source->GetBlendFactor()) > FLOAT_SAFE_EPSILON
			? syneUSetSourceBlendFactor::Ref::New(source, value) : igdeUndo::Ref();
	}
};

class cTextMinVolume : public cBaseTextFieldListener {
public:
	typedef deTObjectReference<cTextMinVolume> Ref;
	cTextMinVolume(syneWPAPanelSource &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(igdeTextField *textField, syneSource *source){
		const float value = textField->GetFloat();
		return fabsf(value - source->GetMinVolume()) > FLOAT_SAFE_EPSILON
			? syneUSetSourceMinVolume::Ref::New(source, value) : igdeUndo::Ref();
	}
};

class cTextMaxVolume : public cBaseTextFieldListener {
public:
	typedef deTObjectReference<cTextMaxVolume> Ref;
	cTextMaxVolume(syneWPAPanelSource &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(igdeTextField *textField, syneSource *source){
		const float value = textField->GetFloat();
		return fabsf(value - source->GetMaxVolume()) > FLOAT_SAFE_EPSILON
			? syneUSetSourceMaxVolume::Ref::New(source, value) : igdeUndo::Ref();
	}
};

class cTextMinPanning : public cBaseTextFieldListener {
public:
	typedef deTObjectReference<cTextMinPanning> Ref;
	cTextMinPanning(syneWPAPanelSource &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(igdeTextField *textField, syneSource *source){
		const float value = textField->GetFloat();
		return fabsf(value - source->GetMinPanning()) > FLOAT_SAFE_EPSILON
			? syneUSetSourceMinPanning::Ref::New(source, value) : igdeUndo::Ref();
	}
};

class cTextMaxPanning : public cBaseTextFieldListener {
public:
	typedef deTObjectReference<cTextMaxPanning> Ref;
	cTextMaxPanning(syneWPAPanelSource &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(igdeTextField *textField, syneSource *source){
		const float value = textField->GetFloat();
		return fabsf(value - source->GetMaxPanning()) > FLOAT_SAFE_EPSILON
			? syneUSetSourceMaxPanning::Ref::New(source, value) : igdeUndo::Ref();
	}
};

class cActionEnable : public cBaseAction {
public:
	typedef deTObjectReference<cActionEnable> Ref;
	cActionEnable(syneWPAPanelSource &panel) : cBaseAction(panel, "Enable source",
		NULL, "Determines if the source is affecting the model"){ }
	
	virtual igdeUndo::Ref OnAction(syneSource *source){
		return syneUSourceToggleEnabled::Ref::New(source);
	}
};


class cComboTarget : public cBaseComboBoxListener {
public:
	typedef deTObjectReference<cComboTarget> Ref;
	cComboTarget(syneWPAPanelSource &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(igdeComboBox*, syneSource*){
		pPanel.UpdateTarget();
		return {};
	}
};

class cListLinks : public igdeListBoxListener{
	syneWPAPanelSource &pPanel;
	
public:
	typedef deTObjectReference<cListLinks> Ref;
	cListLinks(syneWPAPanelSource &panel) : pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(menu, pPanel.GetActionLinkAdd());
		helper.MenuCommand(menu, pPanel.GetActionLinkRemove());
	}
};

class cActionLinkAdd : public cBaseAction {
public:
	typedef deTObjectReference<cActionLinkAdd> Ref;
	cActionLinkAdd(syneWPAPanelSource &panel) : cBaseAction(panel, "Add",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add link"){}
	
	igdeUndo::Ref OnAction(syneSource *source) override{
		syneControllerTarget * const target = pPanel.GetTarget();
		syneLink * const link = pPanel.GetCBLink();
		return target && link && !target->GetLinks().Has(link)
			? syneUSourceTargetAddLink::Ref::New(source, target, link) : igdeUndo::Ref();
	}
	
	void Update() override{
		syneControllerTarget * const target = pPanel.GetTarget();
		syneLink * const link = pPanel.GetCBLink();
		SetSelected(target && link && !target->GetLinks().Has(link));
	}
};

class cActionLinkRemove : public cBaseAction {
public:
	typedef deTObjectReference<cActionLinkRemove> Ref;
	cActionLinkRemove(syneWPAPanelSource &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove link"){}
	
	igdeUndo::Ref OnAction(syneSource *source) override{
		syneControllerTarget * const target = pPanel.GetTarget();
		syneLink * const link = pPanel.GetListLink();
		return target && link && target->GetLinks().Has(link)
			? syneUSourceTargetRemoveLink::Ref::New(source, target, link) : igdeUndo::Ref();
	}
	
	void Update() override{
		syneControllerTarget * const target = pPanel.GetTarget();
		syneLink * const link = pPanel.GetListLink();
		SetSelected(target && link && target->GetLinks().Has(link));
	}
};

}



// Class syneWPAPanelSource
///////////////////////////

// Constructor, destructor
////////////////////////////

syneWPAPanelSource::syneWPAPanelSource(syneWPSource &wpSource,
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
	pWPEffect = syneWPEffect::Ref::New(wpSource.GetViewSynthesizer());
	groupBox->AddChild(pWPEffect);
}

syneWPAPanelSource::~syneWPAPanelSource(){
}



// Management
///////////////

syneWindowMain &syneWPAPanelSource::GetWindowMain() const{
	return pWPSource.GetViewSynthesizer().GetWindowMain();
}

syneSynthesizer *syneWPAPanelSource::GetSynthesizer() const{
	return pWPSource.GetSynthesizer();
}

syneSource *syneWPAPanelSource::GetSource() const{
	syneSource * const source = pWPSource.GetSource();
	return source && source->GetType() == pRequiredType ? source : nullptr;
}

syneControllerTarget *syneWPAPanelSource::GetTarget() const{
	const igdeListItem * const selection = pCBTarget->GetSelectedItem();
	return selection ? (syneControllerTarget*)selection->GetData() : nullptr;
}

syneLink *syneWPAPanelSource::GetCBLink() const{
	const igdeListItem * const selection = pCBLinks->GetSelectedItem();
	return selection ? (syneLink*)selection->GetData() : nullptr;
}

syneLink *syneWPAPanelSource::GetListLink() const{
	const igdeListItem * const selection = pListLinks->GetSelectedItem();
	return selection ? (syneLink*)selection->GetData() : nullptr;
}



void syneWPAPanelSource::SetSynthesizer(syneSynthesizer *synthesizer){
	RemoveAllTargets();
	pWPEffect->SetSynthesizer(synthesizer);
}

void syneWPAPanelSource::OnSynthesizerPathChanged(){
}

void syneWPAPanelSource::OnActivated(){
	UpdateLinkList();
	UpdateTargetList();
	UpdateControllerList();
	
	UpdateSource();
	
	if(!pCBTarget->GetSelectedItem() && pCBTarget->GetItems().IsNotEmpty()){
		pCBTarget->SetSelection(0);
	}
}

void syneWPAPanelSource::OnDeactivated(){
	RemoveAllTargets();
}

void syneWPAPanelSource::RemoveAllTargets(){
	pCBTarget->RemoveAllItems();
}

void syneWPAPanelSource::AddTarget(const char *name, syneControllerTarget *target){
	pCBTarget->AddItem(name, nullptr, target);
}



void syneWPAPanelSource::UpdateSynthesizer(){
	pWPEffect->SetSynthesizer(GetSynthesizer());
}

void syneWPAPanelSource::UpdateControllerList(){
	pWPEffect->UpdateControllerList();
}

void syneWPAPanelSource::UpdateLinkList(){
	syneLink * const selection = pCBLinks->GetSelectedItem() ? (syneLink*)pCBLinks->GetSelectedItem()->GetData() : nullptr;
	
	pCBLinks->RemoveAllItems();
	
	const syneSynthesizer * const synthesizer = GetSynthesizer();
	if(synthesizer){
		const syneLink::List &list = synthesizer->GetLinks();
		const int count = list.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			syneLink * const link = list.GetAt(i);
			pCBLinks->AddItem(link->GetName(), nullptr, link);
		}
	}
	
	pCBLinks->SortItems();
	
	pCBLinks->SetSelectionWithData(selection);
	
	pWPEffect->UpdateLinkList();
}

void syneWPAPanelSource::UpdateSource(){
	const syneSource * const source = GetSource();
	
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

void syneWPAPanelSource::UpdateTargetList(){
	pCBTarget->RemoveAllItems();
	
	syneSource * const source = GetSource();
	if(source){
		AddTarget("Blend Factor", source->GetTargetBlendFactor());
		AddTarget("Volume", source->GetTargetVolume());
		AddTarget("Panning", source->GetTargetPanning());
	}
}

void syneWPAPanelSource::UpdateTarget(){
	syneControllerTarget * const target = GetTarget();
	
	pListLinks->RemoveAllItems();
	
	if(target){
		target->GetLinks().Visit([&](syneLink *link){
			pListLinks->AddItem(link->GetName(), nullptr, link);
		});
		
		pListLinks->SortItems();
	}
	
	pListLinks->SetEnabled(target);
	pBtnLinkAdd->GetAction()->Update();
}
