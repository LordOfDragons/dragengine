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
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo::Ref.h>

#include <dragengine/common/exceptions.h>



// Actions
///////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	seWPSynthesizer &pPanel;
	
public:
	cBaseTextFieldListener( seWPSynthesizer &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		seSynthesizer * const synthesizer = pPanel.GetSynthesizer();
		if( ! synthesizer ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( OnChanged( textField, synthesizer ) );
		if( undo ){
			synthesizer->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, seSynthesizer *synthesizer ) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	seWPSynthesizer &pPanel;
	
public:
	cBaseComboBoxListener( seWPSynthesizer &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		seSynthesizer * const synthesizer = pPanel.GetSynthesizer();
		if( ! synthesizer ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( OnChanged( comboBox, synthesizer ) );
		if( undo ){
			synthesizer->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, seSynthesizer *synthesizer ) = 0;
};

class cBaseAction : public igdeAction{
protected:
	seWPSynthesizer &pPanel;
	
public:
	cBaseAction( seWPSynthesizer &panel, const char *text, igdeIcon *icon, const char *description ) :
	igdeAction( text, icon, description ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		seSynthesizer * const synthesizer = pPanel.GetSynthesizer();
		if( ! synthesizer ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( OnAction( synthesizer ) );
		if( undo ){
			synthesizer->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnAction( seSynthesizer *synthesizer ) = 0;
	
	virtual void Update(){
		seSynthesizer * const synthesizer = pPanel.GetSynthesizer();
		if( synthesizer ){
			Update( *synthesizer );
			
		}else{
			SetEnabled( false );
		}
	}
	
	virtual void Update( const seSynthesizer & ){
		SetEnabled( true );
	}
};


class cComboChannelCount : public cBaseComboBoxListener{
public:
	cComboChannelCount( seWPSynthesizer &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, seSynthesizer *synthesizer ){
		const int value = decMath::max( comboBox->GetText().ToInt(), 1 );
		return synthesizer->GetChannelCount() != value
			? new seUSynthesizerSetChannelCount( synthesizer, value ) : NULL;
	}
};

class cComboSampleRate : public cBaseComboBoxListener{
public:
	cComboSampleRate( seWPSynthesizer &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, seSynthesizer *synthesizer){
		const int value = decMath::max( comboBox->GetText().ToInt(), 1 );
		return synthesizer->GetSampleRate() != value
			? new seUSynthesizerSetSampleRate( synthesizer, value ) : NULL;
	}
};

class cComboBytesPerSample : public cBaseComboBoxListener{
public:
	cComboBytesPerSample( seWPSynthesizer &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox *comboBox, seSynthesizer *synthesizer){
		const int value = decMath::max( comboBox->GetText().ToInt(), 1 );
		return synthesizer->GetBytesPerSample() != value
			? new seUSynthesizerSetBytesPerSample( synthesizer, value ) : NULL;
	}
};

class cTextSampleCount : public cBaseTextFieldListener{
public:
	cTextSampleCount( seWPSynthesizer &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, seSynthesizer *synthesizer ){
		const int value = decMath::max( textField->GetInteger(), 1 );
		return synthesizer->GetSampleCount() != value
			? new seUSynthesizerSetSampleCount( synthesizer, value ) : NULL;
	}
};

class cTextPlayTime : public cBaseTextFieldListener{
	bool &pPreventUpdate;
public:
	cTextPlayTime( seWPSynthesizer &panel, bool &preventUpdate ) :
	cBaseTextFieldListener( panel ), pPreventUpdate( preventUpdate ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, seSynthesizer *synthesizer ){
		if( pPreventUpdate ){
			return NULL;
		}
		const int value = decMath::max( ( int )( textField->GetFloat() * synthesizer->GetSampleRate() + 0.5f ), 1 );
		return synthesizer->GetSampleCount() != value
			? new seUSynthesizerSetSampleCount( synthesizer, value ) : NULL;
	}
};


class cActionLooping : public cBaseAction{
public:
	cActionLooping( seWPSynthesizer &panel ) : cBaseAction( panel, "Looping",
		NULL, "Playback looping" ){ }
	
	virtual igdeUndo *OnAction( seSynthesizer *synthesizer ){
		synthesizer->SetLooping( ! synthesizer->GetLooping() );
		return NULL;
	}
};

class cActionPlay : public cBaseAction{
public:
	cActionPlay( seWPSynthesizer &panel ) : cBaseAction( panel, "Play",
		panel.GetViewSynthesizer().GetWindowMain().GetIconPlay(),
		"Start playing back" ){ }
	
	virtual igdeUndo *OnAction( seSynthesizer *synthesizer ){
		synthesizer->Play();
		return NULL;
	}
	
	virtual void Update(const seSynthesizer &synthesizer ){
		SetEnabled( synthesizer.GetStopped() );
	}
};

class cActionPause : public cBaseAction{
public:
	cActionPause( seWPSynthesizer &panel ) : cBaseAction( panel, "Pause",
		panel.GetViewSynthesizer().GetWindowMain().GetIconPause(),
		"Pause playing back" ){ }
	
	virtual igdeUndo *OnAction( seSynthesizer *synthesizer ){
		synthesizer->Pause();
		return NULL;
	}
	
	virtual void Update(const seSynthesizer &synthesizer ){
		SetEnabled( synthesizer.GetPlaying() );
	}
};

class cActionStop : public cBaseAction{
public:
	cActionStop( seWPSynthesizer &panel ) : cBaseAction( panel, "Stop",
		panel.GetViewSynthesizer().GetWindowMain().GetIconStop(),
		"Stop playing back" ){ }
	
	virtual igdeUndo *OnAction( seSynthesizer *synthesizer ){
		synthesizer->Stop();
		return NULL;
	}
	
	virtual void Update(const seSynthesizer &synthesizer ){
		SetEnabled( ! synthesizer.GetStopped() );
	}
};

}



// Class seWPSynthesizer
/////////////////////////

// Constructor, destructor
////////////////////////////

seWPSynthesizer::seWPSynthesizer( seViewSynthesizer &viewSynthesizer ) :
igdeContainerScroll( viewSynthesizer.GetEnvironment(), false, true ),
pViewSynthesizer( viewSynthesizer ),
pListener( NULL ),
pSynthesizer( NULL ),
pPreventUpdate( false )
{
	igdeEnvironment &env = viewSynthesizer.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference content, groupBox, formLine;
	
	pListener = new seWPSynthesizerListener( *this );
	
	
	pActionPlay.TakeOver( new cActionPlay( *this ) );
	pActionPause.TakeOver( new cActionPause( *this ) );
	pActionStop.TakeOver( new cActionStop( *this ) );
	
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	
	helper.GroupBoxFlow( content, groupBox, "Synthesizer Settings:" );
	
	helper.ComboBox( groupBox, "Channels:", true, "Number of channels",
		pCBChannelCount, new cComboChannelCount( *this ) );
	pCBChannelCount->AddItem( "1" );
	pCBChannelCount->AddItem( "2" );
	
	helper.ComboBox( groupBox, "Sample rate:", true, "Sample rate in Hz",
		pCBSampleRate, new cComboSampleRate( *this ) );
	pCBSampleRate->AddItem( "44100" );
	pCBSampleRate->AddItem( "22050" );
	pCBSampleRate->AddItem( "11025" );
	
	helper.ComboBox( groupBox, "Bytes per sample:", true, "Bytes per sample",
		pCBBytesPerSample, new cComboBytesPerSample( *this ) );
	pCBBytesPerSample->AddItem( "1" );
	pCBBytesPerSample->AddItem( "2" );
	
	
	helper.EditInteger( groupBox, "Sample count:", "Number of samples to create", 8,
		pEditSampleCount, new cTextSampleCount( *this ) );
	helper.EditFloat( groupBox, "Play time:", "Play time in seconds", 6, 1,
		pEditPlayTime, new cTextPlayTime( *this, pPreventUpdate ) );
	
	
	helper.GroupBoxFlow( content, groupBox, "Playback / Testing:" );
	helper.CheckBox( groupBox, pChkLooping, new cActionLooping( *this ), true );
	helper.Button( groupBox, pBtnPlay, pActionPlay );
	helper.Button( groupBox, pBtnPause, pActionPause );
	helper.Button( groupBox, pBtnStop, pActionStop );
}

seWPSynthesizer::~seWPSynthesizer(){
	if( pSynthesizer ){
		pSynthesizer->RemoveNotifier( pListener );
		pSynthesizer->FreeReference();
	}
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void seWPSynthesizer::SetSynthesizer( seSynthesizer *synthesizer ){
	if( synthesizer == pSynthesizer ){
		return;
	}
	
	if( pSynthesizer ){
		pSynthesizer->RemoveNotifier( pListener );
		pSynthesizer->FreeReference();
	}
	
	pSynthesizer = synthesizer;
	
	if( synthesizer ){
		synthesizer->AddNotifier( pListener );
		synthesizer->AddReference();
	}
	
	UpdateSynthesizer();
	UpdatePlayback();
}



void seWPSynthesizer::UpdateSynthesizer(){
	if( pSynthesizer ){
		decString text;
		
		text.Format( "%d", pSynthesizer->GetChannelCount() );
		pCBChannelCount->SetText( text );
		
		text.Format( "%d", pSynthesizer->GetSampleRate() );
		pCBSampleRate->SetText( text );
		
		text.Format( "%d", pSynthesizer->GetBytesPerSample()  );
		pCBBytesPerSample->SetText( text );
		
		pEditSampleCount->SetInteger( pSynthesizer->GetSampleCount() );
		
		pPreventUpdate = true;
		pEditPlayTime->SetFloat( pSynthesizer->GetPlayTime() );
		pPreventUpdate = false;
		
	}else{
		pCBChannelCount->SetSelection( 0 );
		pCBSampleRate->SetSelection( 0 );
		pCBBytesPerSample->SetSelection( 0 );
		pEditSampleCount->ClearText();
		pEditPlayTime->ClearText();
	}
	
	const bool enabled = pSynthesizer;
	pCBChannelCount->SetEnabled( enabled );
	pCBSampleRate->SetEnabled( enabled );
	pCBBytesPerSample->SetEnabled( enabled );
	pEditSampleCount->SetEnabled( enabled );
	pEditPlayTime->SetEnabled( enabled );
}

void seWPSynthesizer::UpdatePlayback(){
	if( pSynthesizer ){
		pChkLooping->SetChecked( pSynthesizer->GetLooping() );
		
	}else{
		pChkLooping->SetChecked( false );
	}
	
	const bool enabled = pSynthesizer;
	pChkLooping->SetEnabled( enabled );
	
	pActionPlay->Update();
	pActionPause->Update();
	pActionStop->Update();
}
