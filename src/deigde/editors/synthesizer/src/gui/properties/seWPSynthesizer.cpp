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
#include <stdint.h>

#include "seWPSynthesizer.h"
#include "seWPSynthesizerListener.h"
#include "../seWindowMain.h"
#include "../seViewSynthesizer.h"
#include "../../synthesizer/seSynthesizer.h"
#include "../../undosys/synthesizer/seUSynthesizerSetBytesPerSample.h"
#include "../../undosys/synthesizer/seUSynthesizerSetChannelCount.h"
#include "../../undosys/synthesizer/seUSynthesizerSetSampleRate.h"
#include "../../undosys/synthesizer/seUSynthesizerSetSampleCount.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/exceptions.h>



// Actions
///////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	seWPSynthesizer &pPanel;
	
public:
	cBaseTextFieldListener(seWPSynthesizer &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		seSynthesizer * const synthesizer = pPanel.GetSynthesizer();
		if(!synthesizer){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, synthesizer));
		if(undo){
			synthesizer->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, seSynthesizer *synthesizer) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	seWPSynthesizer &pPanel;
	
public:
	cBaseComboBoxListener(seWPSynthesizer &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		seSynthesizer * const synthesizer = pPanel.GetSynthesizer();
		if(!synthesizer){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(comboBox, synthesizer));
		if(undo){
			synthesizer->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox *comboBox, seSynthesizer *synthesizer) = 0;
};

class cBaseAction : public igdeAction{
protected:
	seWPSynthesizer &pPanel;
	
public:
	cBaseAction(seWPSynthesizer &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		seSynthesizer * const synthesizer = pPanel.GetSynthesizer();
		if(!synthesizer){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(synthesizer));
		if(undo){
			synthesizer->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(seSynthesizer *synthesizer) = 0;
	
	void Update() override{
		seSynthesizer * const synthesizer = pPanel.GetSynthesizer();
		if(synthesizer){
			Update(*synthesizer);
			
		}else{
			SetEnabled(false);
		}
	}
	
	virtual void Update(const seSynthesizer &){
		SetEnabled(true);
	}
};


class cComboChannelCount : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboChannelCount> Ref;
	cComboChannelCount(seWPSynthesizer &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, seSynthesizer *synthesizer) override{
		const int value = decMath::max(comboBox->GetText().ToInt(), 1);
		return synthesizer->GetChannelCount() != value
			? seUSynthesizerSetChannelCount::Ref::New(synthesizer, value) : igdeUndo::Ref();
	}
};

class cComboSampleRate : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboSampleRate> Ref;
	cComboSampleRate(seWPSynthesizer &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, seSynthesizer *synthesizer) override{
		const int value = decMath::max(comboBox->GetText().ToInt(), 1);
		return synthesizer->GetSampleRate() != value
			? seUSynthesizerSetSampleRate::Ref::New(synthesizer, value) : igdeUndo::Ref();
	}
};

class cComboBytesPerSample : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboBytesPerSample> Ref;
	cComboBytesPerSample(seWPSynthesizer &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, seSynthesizer *synthesizer) override{
		const int value = decMath::max(comboBox->GetText().ToInt(), 1);
		return synthesizer->GetBytesPerSample() != value
			? seUSynthesizerSetBytesPerSample::Ref::New(synthesizer, value) : igdeUndo::Ref();
	}
};

class cTextSampleCount : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextSampleCount> Ref;
	cTextSampleCount(seWPSynthesizer &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, seSynthesizer *synthesizer) override{
		const int value = decMath::max(textField->GetInteger(), 1);
		return synthesizer->GetSampleCount() != value
			? seUSynthesizerSetSampleCount::Ref::New(synthesizer, value) : igdeUndo::Ref();
	}
};

class cTextPlayTime : public cBaseTextFieldListener{
	bool &pPreventUpdate;
public:
	typedef deTObjectReference<cTextPlayTime> Ref;
	cTextPlayTime(seWPSynthesizer &panel, bool &preventUpdate) :
	cBaseTextFieldListener(panel), pPreventUpdate(preventUpdate){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, seSynthesizer *synthesizer) override{
		if(pPreventUpdate){
			return {};
		}
		const int value = decMath::max((int)(textField->GetFloat() * synthesizer->GetSampleRate() + 0.5f), 1);
		return synthesizer->GetSampleCount() != value
			? seUSynthesizerSetSampleCount::Ref::New(synthesizer, value) : igdeUndo::Ref();
	}
};


class cActionLooping : public cBaseAction{
public:
	typedef deTObjectReference<cActionLooping> Ref;
	cActionLooping(seWPSynthesizer &panel) : cBaseAction(panel, "Looping",
		nullptr, "Playback looping"){}
	
	igdeUndo::Ref OnAction(seSynthesizer *synthesizer) override{
		synthesizer->SetLooping(!synthesizer->GetLooping());
		return {};
	}
};

class cActionPlay : public cBaseAction{
public:
	typedef deTObjectReference<cActionPlay> Ref;
	cActionPlay(seWPSynthesizer &panel) : cBaseAction(panel, "Play",
		panel.GetViewSynthesizer().GetWindowMain().GetIconPlay(),
		"Start playing back"){}
	
	igdeUndo::Ref OnAction(seSynthesizer *synthesizer) override{
		synthesizer->Play();
		return {};
	}
	
	void Update(const seSynthesizer &synthesizer) override{
		SetEnabled(synthesizer.GetStopped());
	}
};

class cActionPause : public cBaseAction{
public:
	typedef deTObjectReference<cActionPause> Ref;
	cActionPause(seWPSynthesizer &panel) : cBaseAction(panel, "Pause",
		panel.GetViewSynthesizer().GetWindowMain().GetIconPause(),
		"Pause playing back"){}
	
	igdeUndo::Ref OnAction(seSynthesizer *synthesizer) override{
		synthesizer->Pause();
		return {};
	}
	
	void Update(const seSynthesizer &synthesizer) override{
		SetEnabled(synthesizer.GetPlaying());
	}
};

class cActionStop : public cBaseAction{
public:
	typedef deTObjectReference<cActionStop> Ref;
	cActionStop(seWPSynthesizer &panel) : cBaseAction(panel, "Stop",
		panel.GetViewSynthesizer().GetWindowMain().GetIconStop(),
		"Stop playing back"){}
	
	igdeUndo::Ref OnAction(seSynthesizer *synthesizer) override{
		synthesizer->Stop();
		return {};
	}
	
	void Update(const seSynthesizer &synthesizer) override{
		SetEnabled(!synthesizer.GetStopped());
	}
};

}



// Class seWPSynthesizer
/////////////////////////

// Constructor, destructor
////////////////////////////

seWPSynthesizer::seWPSynthesizer(seViewSynthesizer &viewSynthesizer) :
igdeContainerScroll(viewSynthesizer.GetEnvironment(), false, true),
pViewSynthesizer(viewSynthesizer),
pPreventUpdate(false)
{
	igdeEnvironment &env = viewSynthesizer.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, formLine;
	
	pListener = seWPSynthesizerListener::Ref::New(*this);
	
	
	pActionPlay = cActionPlay::Ref::New(*this);
	pActionPause = cActionPause::Ref::New(*this);
	pActionStop = cActionStop::Ref::New(*this);
	
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	
	helper.GroupBoxFlow(content, groupBox, "Synthesizer Settings:");
	
	helper.ComboBox(groupBox, "Channels:", true, "Number of channels",
		pCBChannelCount, cComboChannelCount::Ref::New(*this));
	pCBChannelCount->AddItem("1");
	pCBChannelCount->AddItem("2");
	
	helper.ComboBox(groupBox, "Sample rate:", true, "Sample rate in Hz",
		pCBSampleRate, cComboSampleRate::Ref::New(*this));
	pCBSampleRate->AddItem("44100");
	pCBSampleRate->AddItem("22050");
	pCBSampleRate->AddItem("11025");
	
	helper.ComboBox(groupBox, "Bytes per sample:", true, "Bytes per sample",
		pCBBytesPerSample, cComboBytesPerSample::Ref::New(*this));
	pCBBytesPerSample->AddItem("1");
	pCBBytesPerSample->AddItem("2");
	
	
	helper.EditInteger(groupBox, "Sample count:", "Number of samples to create", 8,
		pEditSampleCount, cTextSampleCount::Ref::New(*this));
	helper.EditFloat(groupBox, "Play time:", "Play time in seconds", 6, 1,
		pEditPlayTime, cTextPlayTime::Ref::New(*this, pPreventUpdate));
	
	
	helper.GroupBoxFlow(content, groupBox, "Playback / Testing:");
	helper.CheckBox(groupBox, pChkLooping, cActionLooping::Ref::New(*this));
	helper.Button(groupBox, pBtnPlay, pActionPlay);
	helper.Button(groupBox, pBtnPause, pActionPause);
	helper.Button(groupBox, pBtnStop, pActionStop);
}

seWPSynthesizer::~seWPSynthesizer(){
	if(pSynthesizer){
		pSynthesizer->RemoveNotifier(pListener);
	}
}



// Management
///////////////

void seWPSynthesizer::SetSynthesizer(seSynthesizer *synthesizer){
	if(synthesizer == pSynthesizer){
		return;
	}
	
	if(pSynthesizer){
		pSynthesizer->RemoveNotifier(pListener);
	}
	
	pSynthesizer = synthesizer;
	
	if(synthesizer){
		synthesizer->AddNotifier(pListener);
	}
	
	UpdateSynthesizer();
	UpdatePlayback();
}



void seWPSynthesizer::UpdateSynthesizer(){
	if(pSynthesizer){
		decString text;
		
		text.Format("%d", pSynthesizer->GetChannelCount());
		pCBChannelCount->SetText(text);
		
		text.Format("%d", pSynthesizer->GetSampleRate());
		pCBSampleRate->SetText(text);
		
		text.Format("%d", pSynthesizer->GetBytesPerSample());
		pCBBytesPerSample->SetText(text);
		
		pEditSampleCount->SetInteger(pSynthesizer->GetSampleCount());
		
		pPreventUpdate = true;
		pEditPlayTime->SetFloat(pSynthesizer->GetPlayTime());
		pPreventUpdate = false;
		
	}else{
		pCBChannelCount->SetSelection(0);
		pCBSampleRate->SetSelection(0);
		pCBBytesPerSample->SetSelection(0);
		pEditSampleCount->ClearText();
		pEditPlayTime->ClearText();
	}
	
	const bool enabled = pSynthesizer;
	pCBChannelCount->SetEnabled(enabled);
	pCBSampleRate->SetEnabled(enabled);
	pCBBytesPerSample->SetEnabled(enabled);
	pEditSampleCount->SetEnabled(enabled);
	pEditPlayTime->SetEnabled(enabled);
}

void seWPSynthesizer::UpdatePlayback(){
	if(pSynthesizer){
		pChkLooping->SetChecked(pSynthesizer->GetLooping());
		
	}else{
		pChkLooping->SetChecked(false);
	}
	
	const bool enabled = pSynthesizer;
	pChkLooping->SetEnabled(enabled);
	
	pActionPlay->Update();
	pActionPause->Update();
	pActionStop->Update();
}
