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

#include "seWPAPanelSourceSound.h"
#include "../../seWPSource.h"
#include "../../../seWindowMain.h"
#include "../../../../synthesizerEditor.h"
#include "../../../../synthesizer/seSynthesizer.h"
#include "../../../../synthesizer/controller/seController.h"
#include "../../../../synthesizer/source/seSourceSound.h"
#include "../../../../undosys/source/sound/seUSetSourceSoundPathSound.h"
#include "../../../../undosys/source/sound/seUSetSourceSoundMinSpeed.h"
#include "../../../../undosys/source/sound/seUSetSourceSoundMaxSpeed.h"
#include "../../../../undosys/source/sound/seUToggleSourceSoundLooping.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/sound/deSound.h>



// Actions
////////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	seWPAPanelSourceSound &pPanel;
	
public:
	cBaseTextFieldListener(seWPAPanelSourceSound &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		seSourceSound * const source = (seSourceSound*)pPanel.GetSource();
		if(!source){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, source));
		if(undo){
			source->GetSynthesizer()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, seSourceSound *source) = 0;
};


class cPathSound : public igdeEditPathListener{
	seWPAPanelSourceSound &pPanel;
	
public:
	typedef deTObjectReference<cPathSound> Ref;
	cPathSound(seWPAPanelSourceSound &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath * editPath){
		seSourceSound * const source = (seSourceSound*)pPanel.GetSource();
		if(!source || source->GetPathSound() == editPath->GetPath()){
			return;
		}
		
		source->GetSynthesizer()->GetUndoSystem()->Add(
			seUSetSourceSoundPathSound::Ref::New(source, editPath->GetPath()));
	}
};

class cTextMinSpeed : public cBaseTextFieldListener {
public:
	typedef deTObjectReference<cTextMinSpeed> Ref;
	cTextMinSpeed(seWPAPanelSourceSound &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(igdeTextField *textField, seSourceSound *source){
		const float value = textField->GetFloat();
		return fabsf(value - source->GetMinSpeed()) > FLOAT_SAFE_EPSILON
			? seUSetSourceSoundMinSpeed::Ref::New(source, value) : igdeUndo::Ref();
	}
};

class cTextMaxSpeed : public cBaseTextFieldListener {
public:
	typedef deTObjectReference<cTextMaxSpeed> Ref;
	cTextMaxSpeed(seWPAPanelSourceSound &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(igdeTextField *textField, seSourceSound *source){
		const float value = textField->GetFloat();
		return fabsf(value - source->GetMaxSpeed()) > FLOAT_SAFE_EPSILON
			? seUSetSourceSoundMaxSpeed::Ref::New(source, value) : igdeUndo::Ref();
	}
};

class cActionLooping : public igdeAction {
	seWPAPanelSourceSound &pPanel;
	
public:
	typedef deTObjectReference<cActionLooping> Ref;
	cActionLooping(seWPAPanelSourceSound &panel) : igdeAction("Looping",
		nullptr, "Sound is played back looping"), pPanel(panel){}
	
	void OnAction() override{
		seSourceSound * const source = (seSourceSound*)pPanel.GetSource();
		if(!source){
			return;
		}
		
		source->GetSynthesizer()->GetUndoSystem()->Add(
			seUToggleSourceSoundLooping::Ref::New(source));
	}
};

}



// Class seWPAPanelSourceSound
////////////////////////////////

// Constructor, destructor
////////////////////////////

seWPAPanelSourceSound::seWPAPanelSourceSound(seWPSource &wpSource) :
seWPAPanelSource(wpSource, deSynthesizerSourceVisitorIdentify::estSound)
{
	igdeEnvironment &env = wpSource.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref groupBox;
	
	
	helper.GroupBox(*this, groupBox, "Sound:");
	helper.EditPath(groupBox, "Sound:", "Sound file to use", igdeEnvironment::efpltSound,
		pEditPathSound, cPathSound::Ref::New(*this));
	
	helper.EditString(groupBox, "", "", pLabSoundInfo, {});
	pLabSoundInfo->SetEditable(false);
	
	helper.EditFloat(groupBox, "Minimum Speed:",
		"Minimum play speed in percentage of normal playback speed. Use negative values to play backwards.",
		pEditMinSpeed, cTextMinSpeed::Ref::New(*this));
	helper.EditFloat(groupBox, "Maximum Speed:",
		"Maximum play speed in percentage of normal playback speed. Use negative values to play backwards.",
		pEditMaxSpeed, cTextMaxSpeed::Ref::New(*this));
	
	helper.CheckBox(groupBox, pChkLooping, cActionLooping::Ref::New(*this));
}

seWPAPanelSourceSound::~seWPAPanelSourceSound(){
}



// Management
///////////////

void seWPAPanelSourceSound::UpdateSynthesizer(){
	UpdateSoundInfo();
}

void seWPAPanelSourceSound::OnSynthesizerPathChanged(){
	seSynthesizer * const synthesizer = GetSynthesizer();
	if(synthesizer){
		pEditPathSound->SetBasePath(synthesizer->GetDirectoryPath());
		
	}else{
		pEditPathSound->SetBasePath("");
	}
}

void seWPAPanelSourceSound::UpdateSource(){
	seWPAPanelSource::UpdateSource();
	
	const seSourceSound * const source = (seSourceSound*)GetSource();
	if(source){
		pEditPathSound->SetPath(source->GetPathSound());
		pEditMinSpeed->SetFloat(source->GetMinSpeed());
		pEditMaxSpeed->SetFloat(source->GetMaxSpeed());
		pChkLooping->SetChecked(source->GetLooping());
		
	}else{
		pEditPathSound->ClearPath();
		pEditMinSpeed->ClearText();
		pEditMaxSpeed->ClearText();
		pChkLooping->SetChecked(false);
	}
	
	const bool enabled = source;
	pEditPathSound->SetEnabled(enabled);
	pEditMinSpeed->SetEnabled(enabled);
	pEditMaxSpeed->SetEnabled(enabled);
	pChkLooping->SetEnabled(enabled);
	
	UpdateSoundInfo();
}

void seWPAPanelSourceSound::UpdateTargetList(){
	seWPAPanelSource::UpdateTargetList();
	
	seSourceSound * const source = (seSourceSound*)GetSource();
	if(source){
		AddTarget("Speed", source->GetTargetSpeed());
		AddTarget("Play", source->GetTargetPlay());
	}
}

void seWPAPanelSourceSound::UpdateSoundInfo(){
	const seSourceSound * const source = (seSourceSound*)GetSource();
	
	if(source && source->GetSound()){
		const deSound &sound = *source->GetSound();
		decString text, description;
		bool invalidValue = false;
		
		text.Format("%d Channels, %dHz, %d-bit, %d samples (%.2fs)",
			sound.GetChannelCount(), sound.GetSampleRate(), sound.GetBytesPerSample() * 8,
			sound.GetSampleCount(), (float)sound.GetSampleCount() / (float)sound.GetSampleRate());
		
		if(GetSynthesizer()->GetChannelCount() < sound.GetChannelCount()){
			if(!description.IsEmpty()){
				description.AppendCharacter('\n');
			}
			description += "Sound has more channels than synthesizer!";
			invalidValue = true;
		}
		
		if(GetSynthesizer()->GetSampleRate() != sound.GetSampleRate()){
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
		pLabSoundInfo->ClearText();
		pLabSoundInfo->SetDescription("");
		pLabSoundInfo->SetInvalidValue(false);
	}
}
