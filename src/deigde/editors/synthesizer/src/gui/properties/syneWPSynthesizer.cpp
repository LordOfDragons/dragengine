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

#include "syneWPSynthesizer.h"
#include "syneWPSynthesizerListener.h"
#include "../syneWindowMain.h"
#include "../syneViewSynthesizer.h"
#include "../../synthesizer/syneSynthesizer.h"
#include "../../undosys/synthesizer/syneUSynthesizerSetBytesPerSample.h"
#include "../../undosys/synthesizer/syneUSynthesizerSetChannelCount.h"
#include "../../undosys/synthesizer/syneUSynthesizerSetSampleRate.h"
#include "../../undosys/synthesizer/syneUSynthesizerSetSampleCount.h"

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
	syneWPSynthesizer &pPanel;
	
public:
	cBaseTextFieldListener(syneWPSynthesizer &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		syneSynthesizer * const synthesizer = pPanel.GetSynthesizer();
		if(!synthesizer){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, synthesizer));
		if(undo){
			synthesizer->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, syneSynthesizer *synthesizer) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	syneWPSynthesizer &pPanel;
	
public:
	cBaseComboBoxListener(syneWPSynthesizer &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		syneSynthesizer * const synthesizer = pPanel.GetSynthesizer();
		if(!synthesizer){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(comboBox, synthesizer));
		if(undo){
			synthesizer->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox *comboBox, syneSynthesizer *synthesizer) = 0;
};

class cBaseAction : public igdeAction{
protected:
	syneWPSynthesizer &pPanel;
	
public:
	cBaseAction(syneWPSynthesizer &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		syneSynthesizer * const synthesizer = pPanel.GetSynthesizer();
		if(!synthesizer){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(synthesizer));
		if(undo){
			synthesizer->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(syneSynthesizer *synthesizer) = 0;
	
	void Update() override{
		syneSynthesizer * const synthesizer = pPanel.GetSynthesizer();
		if(synthesizer){
			Update(*synthesizer);
			
		}else{
			SetEnabled(false);
		}
	}
	
	virtual void Update(const syneSynthesizer &){
		SetEnabled(true);
	}
};


class cComboChannelCount : public cBaseComboBoxListener{
public:
	using Ref = deTObjectReference<cComboChannelCount>;
	cComboChannelCount(syneWPSynthesizer &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, syneSynthesizer *synthesizer) override{
		const int value = decMath::max(comboBox->GetText().ToInt(), 1);
		return synthesizer->GetChannelCount() != value
			? syneUSynthesizerSetChannelCount::Ref::New(synthesizer, value) : igdeUndo::Ref();
	}
};

class cComboSampleRate : public cBaseComboBoxListener{
public:
	using Ref = deTObjectReference<cComboSampleRate>;
	cComboSampleRate(syneWPSynthesizer &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, syneSynthesizer *synthesizer) override{
		const int value = decMath::max(comboBox->GetText().ToInt(), 1);
		return synthesizer->GetSampleRate() != value
			? syneUSynthesizerSetSampleRate::Ref::New(synthesizer, value) : igdeUndo::Ref();
	}
};

class cComboBytesPerSample : public cBaseComboBoxListener{
public:
	using Ref = deTObjectReference<cComboBytesPerSample>;
	cComboBytesPerSample(syneWPSynthesizer &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, syneSynthesizer *synthesizer) override{
		const int value = decMath::max(comboBox->GetText().ToInt(), 1);
		return synthesizer->GetBytesPerSample() != value
			? syneUSynthesizerSetBytesPerSample::Ref::New(synthesizer, value) : igdeUndo::Ref();
	}
};

class cTextSampleCount : public cBaseTextFieldListener{
public:
	using Ref = deTObjectReference<cTextSampleCount>;
	cTextSampleCount(syneWPSynthesizer &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, syneSynthesizer *synthesizer) override{
		const int value = decMath::max(textField->GetInteger(), 1);
		return synthesizer->GetSampleCount() != value
			? syneUSynthesizerSetSampleCount::Ref::New(synthesizer, value) : igdeUndo::Ref();
	}
};

class cTextPlayTime : public cBaseTextFieldListener{
	bool &pPreventUpdate;
public:
	using Ref = deTObjectReference<cTextPlayTime>;
	cTextPlayTime(syneWPSynthesizer &panel, bool &preventUpdate) :
	cBaseTextFieldListener(panel), pPreventUpdate(preventUpdate){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, syneSynthesizer *synthesizer) override{
		if(pPreventUpdate){
			return {};
		}
		const int value = decMath::max((int)(textField->GetFloat() * synthesizer->GetSampleRate() + 0.5f), 1);
		return synthesizer->GetSampleCount() != value
			? syneUSynthesizerSetSampleCount::Ref::New(synthesizer, value) : igdeUndo::Ref();
	}
};


class cActionLooping : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionLooping>;
	cActionLooping(syneWPSynthesizer &panel) : cBaseAction(panel, "@Synthesizer.WPSynthesizer.Action.Looping",
		nullptr, "@Synthesizer.WPSynthesizer.Action.Looping.ToolTip"){}
	
	igdeUndo::Ref OnAction(syneSynthesizer *synthesizer) override{
		synthesizer->SetLooping(!synthesizer->GetLooping());
		return {};
	}
};

class cActionPlay : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionPlay>;
	cActionPlay(syneWPSynthesizer &panel) : cBaseAction(panel, "@Synthesizer.WPSynthesizer.Action.Play",
		panel.GetViewSynthesizer().GetWindowMain().GetIconPlay(),
		"@Synthesizer.WPSynthesizer.Action.Play.ToolTip"){}
	
	igdeUndo::Ref OnAction(syneSynthesizer *synthesizer) override{
		synthesizer->Play();
		return {};
	}
	
	void Update(const syneSynthesizer &synthesizer) override{
		SetEnabled(synthesizer.GetStopped());
	}
};

class cActionPause : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionPause>;
	cActionPause(syneWPSynthesizer &panel) : cBaseAction(panel, "@Synthesizer.WPSynthesizer.Action.Pause",
		panel.GetViewSynthesizer().GetWindowMain().GetIconPause(),
		"@Synthesizer.WPSynthesizer.Action.Pause.ToolTip"){}
	
	igdeUndo::Ref OnAction(syneSynthesizer *synthesizer) override{
		synthesizer->Pause();
		return {};
	}
	
	void Update(const syneSynthesizer &synthesizer) override{
		SetEnabled(synthesizer.GetPlaying());
	}
};

class cActionStop : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionStop>;
	cActionStop(syneWPSynthesizer &panel) : cBaseAction(panel, "@Synthesizer.WPSynthesizer.Action.Stop",
		panel.GetViewSynthesizer().GetWindowMain().GetIconStop(),
		"@Synthesizer.WPSynthesizer.Action.Stop.ToolTip"){}
	
	igdeUndo::Ref OnAction(syneSynthesizer *synthesizer) override{
		synthesizer->Stop();
		return {};
	}
	
	void Update(const syneSynthesizer &synthesizer) override{
		SetEnabled(!synthesizer.GetStopped());
	}
};

}



// Class syneWPSynthesizer
/////////////////////////

// Constructor, destructor
////////////////////////////

syneWPSynthesizer::syneWPSynthesizer(syneViewSynthesizer &viewSynthesizer) :
igdeContainerScroll(viewSynthesizer.GetEnvironment(), false, true),
pViewSynthesizer(viewSynthesizer),
pPreventUpdate(false)
{
	igdeEnvironment &env = viewSynthesizer.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, formLine;
	
	pListener = syneWPSynthesizerListener::Ref::New(*this);
	
	
	pActionPlay = cActionPlay::Ref::New(*this);
	pActionPause = cActionPause::Ref::New(*this);
	pActionStop = cActionStop::Ref::New(*this);
	
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	
	helper.GroupBoxFlow(content, groupBox, "@Synthesizer.WPSynthesizer.GroupSynthesizerSettings");
	
	helper.ComboBox(groupBox, "@Synthesizer.WPSynthesizer.FieldChannels", true, "@Synthesizer.WPSynthesizer.FieldChannels.ToolTip",
		pCBChannelCount, cComboChannelCount::Ref::New(*this));
	pCBChannelCount->AddItem("1");
	pCBChannelCount->AddItem("2");
	
	helper.ComboBox(groupBox, "@Synthesizer.WPSynthesizer.FieldSampleRate", true, "@Synthesizer.WPSynthesizer.FieldSampleRate.ToolTip",
		pCBSampleRate, cComboSampleRate::Ref::New(*this));
	pCBSampleRate->AddItem("44100");
	pCBSampleRate->AddItem("22050");
	pCBSampleRate->AddItem("11025");
	
	helper.ComboBox(groupBox, "@Synthesizer.WPSynthesizer.FieldBytesPerSample", true, "@Synthesizer.WPSynthesizer.FieldBytesPerSample.ToolTip",
		pCBBytesPerSample, cComboBytesPerSample::Ref::New(*this));
	pCBBytesPerSample->AddItem("1");
	pCBBytesPerSample->AddItem("2");
	
	
	helper.EditInteger(groupBox, "@Synthesizer.WPSynthesizer.FieldSampleCount", "@Synthesizer.WPSynthesizer.FieldSampleCount.ToolTip", 8,
		pEditSampleCount, cTextSampleCount::Ref::New(*this));
	helper.EditFloat(groupBox, "@Synthesizer.WPSynthesizer.FieldPlayTime", "@Synthesizer.WPSynthesizer.FieldPlayTime.ToolTip", 6, 1,
		pEditPlayTime, cTextPlayTime::Ref::New(*this, pPreventUpdate));
	
	
	helper.GroupBoxFlow(content, groupBox, "@Synthesizer.WPSynthesizer.GroupPlaybackTesting");
	helper.CheckBox(groupBox, pChkLooping, cActionLooping::Ref::New(*this));
	helper.Button(groupBox, pBtnPlay, pActionPlay);
	helper.Button(groupBox, pBtnPause, pActionPause);
	helper.Button(groupBox, pBtnStop, pActionStop);
}

syneWPSynthesizer::~syneWPSynthesizer(){
	if(pSynthesizer){
		pSynthesizer->RemoveNotifier(pListener);
	}
}



// Management
///////////////

void syneWPSynthesizer::SetSynthesizer(syneSynthesizer *synthesizer){
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



void syneWPSynthesizer::UpdateSynthesizer(){
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

void syneWPSynthesizer::UpdatePlayback(){
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
