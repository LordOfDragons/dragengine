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

#include "seWPAPanelSourceChain.h"
#include "../../seWPSource.h"
#include "../../../seWindowMain.h"
#include "../../../../synthesizerEditor.h"
#include "../../../../synthesizer/seSynthesizer.h"
#include "../../../../synthesizer/source/seSourceChain.h"
#include "../../../../undosys/source/chain/seUSetSourceChainMinSpeed.h"
#include "../../../../undosys/source/chain/seUSetSourceChainMaxSpeed.h"
#include "../../../../undosys/source/chain/seUSourceChainSetPathSound.h"
#include "../../../../undosys/source/chain/seUSourceChainPathSoundMoveUp.h"
#include "../../../../undosys/source/chain/seUSourceChainPathSoundMoveDown.h"
#include "../../../../undosys/source/chain/seUSourceChainPathSoundAdd.h"
#include "../../../../undosys/source/chain/seUSourceChainPathSoundRemove.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/deEngine.h>
#include <dragengine/resources/sound/deSound.h>



// Actions
////////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	seWPAPanelSourceChain &pPanel;
	
public:
	cBaseTextFieldListener(seWPAPanelSourceChain &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		seSourceChain * const source = (seSourceChain*)pPanel.GetSource();
		if(!source){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, source));
		if(undo){
			source->GetSynthesizer()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, seSourceChain *source) = 0;
};

class cBaseAction : public igdeAction {
protected:
	seWPAPanelSourceChain &pPanel;
	
public:
	cBaseAction(seWPAPanelSourceChain &panel, const char *text, igdeIcon *icon,
		const char *description) : igdeAction(text, icon, description), pPanel(panel){}
	
	void OnAction() override{
		seSourceChain * const source = (seSourceChain*)pPanel.GetSource();
		if(!source){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(source));
		if(undo){
			source->GetSynthesizer()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(seSourceChain *source) = 0;
	
	void Update() override{
		const seSourceChain * const source = (seSourceChain*)pPanel.GetSource();
		if(source){
			Update(*source);
			
		}else{
			SetEnabled(false);
		}
		
	}
	
	virtual void Update(const seSourceChain &){
		SetEnabled(true);
	}
};


class cTextMinSpeed : public cBaseTextFieldListener {
public:
	typedef deTObjectReference<cTextMinSpeed> Ref;
	cTextMinSpeed(seWPAPanelSourceChain &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(igdeTextField *textField, seSourceChain *source){
		const float value = textField->GetFloat();
		return fabsf(value - source->GetMinSpeed()) > FLOAT_SAFE_EPSILON
			? seUSetSourceChainMinSpeed::Ref::New(source, value) : igdeUndo::Ref();
	}
};

class cTextMaxSpeed : public cBaseTextFieldListener {
public:
	typedef deTObjectReference<cTextMaxSpeed> Ref;
	cTextMaxSpeed(seWPAPanelSourceChain &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(igdeTextField *textField, seSourceChain *source){
		const float value = textField->GetFloat();
		return fabsf(value - source->GetMaxSpeed()) > FLOAT_SAFE_EPSILON
			? seUSetSourceChainMaxSpeed::Ref::New(source, value) : igdeUndo::Ref();
	}
};


class cListSounds : public igdeListBoxListener{
	seWPAPanelSourceChain &pPanel;
	
public:
	typedef deTObjectReference<cListSounds> Ref;
	cListSounds(seWPAPanelSourceChain &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeListBox*){
		pPanel.UpdateSound();
	}
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(menu, pPanel.GetActionSoundAdd());
		helper.MenuCommand(menu, pPanel.GetActionSoundRemove());
		helper.MenuCommand(menu, pPanel.GetActionSoundUp());
		helper.MenuCommand(menu, pPanel.GetActionSoundDown());
	}
};

class cActionSoundAdd : public cBaseAction {
public:
	typedef deTObjectReference<cActionSoundAdd> Ref;
	cActionSoundAdd(seWPAPanelSourceChain &panel) : cBaseAction(panel, "Add",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add sound to list"){}
	
	igdeUndo::Ref OnAction(seSourceChain *source) override{
		decString path;
		if(!igdeCommonDialogs::GetFileOpen(&pPanel, "Add Sound",
			*pPanel.GetEnvironment().GetFileSystemGame(),
			*pPanel.GetEnvironment().GetOpenFilePatternList( igdeEnvironment::efpltSound ), path ) ){
				return {};
		}
		
		pPanel.GetSynthesizer()->GetUndoSystem()->Add(
			seUSourceChainPathSoundAdd::Ref::New(source, path));
		
		pPanel.SelectSoundInList(source->GetPathSounds().GetCount() - 1);
		return {};
	}
};

class cActionSoundRemove : public cBaseAction {
public:
	typedef deTObjectReference<cActionSoundRemove> Ref;
	cActionSoundRemove(seWPAPanelSourceChain &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove sound from list"){}
	
	igdeUndo::Ref OnAction(seSourceChain *source) override{
		const int selection = pPanel.GetSelectedSoundInList();
		return selection != -1 ? seUSourceChainPathSoundRemove::Ref::New(source, selection) : seUSourceChainPathSoundRemove::Ref();
	}
	
	void Update(const seSourceChain &) override{
		SetEnabled(pPanel.GetSelectedSoundInList() != -1);
	}
};

class cActionSoundUp : public cBaseAction {
public:
	typedef deTObjectReference<cActionSoundUp> Ref;
	cActionSoundUp(seWPAPanelSourceChain &panel) : cBaseAction(panel, "Move Up",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp), "Move sound up in list"){}
	
	igdeUndo::Ref OnAction(seSourceChain *source) override{
		const int selection = pPanel.GetSelectedSoundInList();
		return selection > 0 ? seUSourceChainPathSoundMoveUp::Ref::New(source, selection) : seUSourceChainPathSoundMoveUp::Ref();
	}
	
	void Update(const seSourceChain &) override{
		SetEnabled(pPanel.GetSelectedSoundInList() > 0);
	}
};

class cActionSoundDown : public cBaseAction {
public:
	typedef deTObjectReference<cActionSoundDown> Ref;
	cActionSoundDown(seWPAPanelSourceChain &panel) : cBaseAction(panel, "Move Up",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp), "Move sound up in list"){}
	
	igdeUndo::Ref OnAction(seSourceChain *source) override{
		const int selection = pPanel.GetSelectedSoundInList();
		return selection > 0 ? seUSourceChainPathSoundMoveUp::Ref::New(source, selection) : seUSourceChainPathSoundMoveUp::Ref();
	}
	
	void Update(const seSourceChain &source) override{
		const int selection = pPanel.GetSelectedSoundInList();
		SetEnabled(selection != -1 && selection < source.GetPathSounds().GetCount() - 1);
	}
};

class cPathSound : public igdeEditPathListener{
	seWPAPanelSourceChain &pPanel;
	
public:
	typedef deTObjectReference<cPathSound> Ref;
	cPathSound(seWPAPanelSourceChain &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath * editPath){
		seSourceChain * const source = (seSourceChain*)pPanel.GetSource();
		const int selection = pPanel.GetSelectedSoundInList();
		if(!source || selection == -1 || source->GetPathSounds().GetAt(selection) == editPath->GetPath()){
			return;
		}
		
		source->GetSynthesizer()->GetUndoSystem()->Add(seUSourceChainSetPathSound::Ref::New(
			source, selection, editPath->GetPath()));
	}
};

}



// Class seWPAPanelSourceChain
////////////////////////////////

// Constructor, destructor
////////////////////////////

seWPAPanelSourceChain::seWPAPanelSourceChain(seWPSource &wpSource) :
seWPAPanelSource(wpSource, deSynthesizerSourceVisitorIdentify::estChain)
{
	igdeEnvironment &env = wpSource.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref groupBox, form;
	
	
	pActionSoundAdd = cActionSoundAdd::Ref::New(*this);
	pActionSoundRemove = cActionSoundRemove::Ref::New(*this);
	pActionSoundUp = cActionSoundUp::Ref::New(*this);
	pActionSoundDown = cActionSoundDown::Ref::New(*this);
	
	
	helper.GroupBox(*this, groupBox, "Chain:");
	
	helper.EditFloat(groupBox, "Minimum Speed:",
		"Minimum play speed in percentage of normal playback speed. Use negative values to play backwards.",
		pEditMinSpeed, cTextMinSpeed::Ref::New(*this));
	helper.EditFloat(groupBox, "Maximum Speed:",
		"Maximum play speed in percentage of normal playback speed. Use negative values to play backwards.",
		pEditMaxSpeed, cTextMaxSpeed::Ref::New(*this));
	
	
	helper.GroupBoxFlow(*this, groupBox, "Sounds:");
	helper.ListBox(groupBox, 5, "Sounds to play", pListSounds, cListSounds::Ref::New(*this));
	
	form = igdeContainerForm::Ref::New(env);
	groupBox->AddChild(form);
	
	helper.EditPath(form, "Sound:", "Sound file", igdeEnvironment::efpltSound,
		pEditPathSound, cPathSound::Ref::New(*this));
	helper.EditString(form, "", "", pLabSoundInfo, {});
	pLabSoundInfo->SetEditable(false);
}

seWPAPanelSourceChain::~seWPAPanelSourceChain(){
}



// Management
///////////////

void seWPAPanelSourceChain::UpdateSynthesizer(){
	UpdateSoundInfo();
}

void seWPAPanelSourceChain::OnSynthesizerPathChanged(){
	seSynthesizer * const synthesizer = GetSynthesizer();
	if(synthesizer){
		pEditPathSound->SetBasePath(synthesizer->GetDirectoryPath());
		
	}else{
		pEditPathSound->SetBasePath("");
	}
}

void seWPAPanelSourceChain::UpdateSource(){
	seWPAPanelSource::UpdateSource();
	
	const seSourceChain * const source = (seSourceChain*)GetSource();
	if(source){
		pEditMinSpeed->SetFloat(source->GetMinSpeed());
		pEditMaxSpeed->SetFloat(source->GetMaxSpeed());
		
	}else{
		pEditMinSpeed->ClearText();
		pEditMaxSpeed->ClearText();
	}
	
	const bool enabled = source;
	pEditMinSpeed->SetEnabled(enabled);
	pEditMaxSpeed->SetEnabled(enabled);
	
	UpdateSoundList();
}

void seWPAPanelSourceChain::UpdateTargetList(){
	seWPAPanelSource::UpdateTargetList();
	
	seSourceChain *source = (seSourceChain*)GetSource();
	if(source){
		AddTarget("Speed", source->GetTargetSpeed());
		AddTarget("Select", source->GetTargetSelect());
		AddTarget("Play", source->GetTargetPlay());
	}
}

void seWPAPanelSourceChain::UpdateSoundList(){
	const seSourceChain * const source = (const seSourceChain*)GetSource();
	const int selection = pListSounds->GetSelection();
	
	pListSounds->RemoveAllItems();
	
	if(source){
		const decStringList &list = source->GetPathSounds();
		const int count = list.GetCount();
		decString text;
		int i;
		
		for(i=0; i<count; i++){
			text.Format("%d: %s", i, list.GetAt(i).GetString());
			pListSounds->AddItem(text);
		}
	}
	
	if(pListSounds->GetItems().IsNotEmpty()){
		pListSounds->SetSelection(decMath::clamp(selection, 0, pListSounds->GetItems().GetCount() - 1));
	}
	
	UpdateSound();
}

void seWPAPanelSourceChain::UpdateSound(){
	const seSourceChain * const source = (const seSourceChain*)GetSource();
	const int selection = pListSounds->GetSelection();
	
	if(source && selection != -1){
		pEditPathSound->SetPath(source->GetPathSounds().GetAt(selection));
		
	}else{
		pEditPathSound->ClearPath();
	}
	
	pEditPathSound->SetEnabled(source && selection != -1);
	
	UpdateSoundInfo();
}

void seWPAPanelSourceChain::UpdateSoundInfo(){
	const seSourceChain * const source = (const seSourceChain*)GetSource();
	const int selection = pListSounds->GetSelection();
	const deSound * const sound = source && selection != -1 ? source->GetSounds().GetAt(selection) : nullptr;
	
	if(sound){
		decString text, description;
		bool invalidValue = false;
		
		text.Format("%d Channels, %dHz, %d-bit, %d samples (%.2fs)",
			sound->GetChannelCount(), sound->GetSampleRate(), sound->GetBytesPerSample() * 8,
			sound->GetSampleCount(), (float)sound->GetSampleCount() / (float)sound->GetSampleRate());
		
		if(GetSynthesizer()->GetChannelCount() < sound->GetChannelCount()){
			if(!description.IsEmpty()){
				description.AppendCharacter('\n');
			}
			description += "Sound has more channels than synthesizer!";
			invalidValue = true;
		}
		
		if(GetSynthesizer()->GetSampleRate() != sound->GetSampleRate()){
			if(!description.IsEmpty()){
				description.AppendCharacter('\n');
			}
			description += "Sound sample rate differs synthesizer!";
			invalidValue = true;
		}
		
		pLabSoundInfo->SetText(text);
		pLabSoundInfo->SetDescription(description);
		pLabSoundInfo->SetInvalidValue(invalidValue);
		
	}else{
		pLabSoundInfo->SetText("");
		pLabSoundInfo->SetDescription("");
		pLabSoundInfo->SetInvalidValue(false);
	}
}

int seWPAPanelSourceChain::GetSelectedSoundInList() const{
	return GetSource() ? pListSounds->GetSelection() : -1;
}

void seWPAPanelSourceChain::SelectSoundInList(int index){
	pListSounds->SetSelection(index);
}
