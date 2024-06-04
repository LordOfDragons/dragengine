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
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/undo/igdeUndoReference.h>
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
	cBaseTextFieldListener( seWPAPanelSourceSound &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		seSourceSound * const source = ( seSourceSound* )pPanel.GetSource();
		if( ! source ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( textField, source ) );
		if( undo ){
			source->GetSynthesizer()->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, seSourceSound *source ) = 0;
};


class cPathSound : public igdeEditPathListener{
	seWPAPanelSourceSound &pPanel;
	
public:
	cPathSound( seWPAPanelSourceSound &panel ) : pPanel( panel ){ }
	
	virtual void OnEditPathChanged( igdeEditPath * editPath ){
		seSourceSound * const source = ( seSourceSound* )pPanel.GetSource();
		if( ! source || source->GetPathSound() == editPath->GetPath() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new seUSetSourceSoundPathSound( source, editPath->GetPath() ) );
		source->GetSynthesizer()->GetUndoSystem()->Add( undo );
	}
};

class cTextMinSpeed : public cBaseTextFieldListener {
public:
	cTextMinSpeed( seWPAPanelSourceSound &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo * OnChanged( igdeTextField *textField, seSourceSound *source ){
		const float value = textField->GetFloat();
		return fabsf( value - source->GetMinSpeed() ) > FLOAT_SAFE_EPSILON
			? new seUSetSourceSoundMinSpeed( source, value ) : NULL;
	}
};

class cTextMaxSpeed : public cBaseTextFieldListener {
public:
	cTextMaxSpeed( seWPAPanelSourceSound &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo * OnChanged( igdeTextField *textField, seSourceSound *source ){
		const float value = textField->GetFloat();
		return fabsf( value - source->GetMaxSpeed() ) > FLOAT_SAFE_EPSILON
			? new seUSetSourceSoundMaxSpeed( source, value ) : NULL;
	}
};

class cActionLooping : public igdeAction {
	seWPAPanelSourceSound &pPanel;
	
public:
	cActionLooping( seWPAPanelSourceSound &panel ) : igdeAction( "Looping",
		NULL, "Sound is played back looping" ), pPanel( panel ){ }
	
	virtual void OnAction(){
		seSourceSound * const source = ( seSourceSound* )pPanel.GetSource();
		if( ! source ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new seUToggleSourceSoundLooping( source ) );
		source->GetSynthesizer()->GetUndoSystem()->Add( undo );
	}
};

}



// Class seWPAPanelSourceSound
////////////////////////////////

// Constructor, destructor
////////////////////////////

seWPAPanelSourceSound::seWPAPanelSourceSound( seWPSource &wpSource ) :
seWPAPanelSource( wpSource, deSynthesizerSourceVisitorIdentify::estSound )
{
	igdeEnvironment &env = wpSource.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference groupBox;
	
	
	helper.GroupBox( *this, groupBox, "Sound:" );
	helper.EditPath( groupBox, "Sound:", "Sound file to use", igdeEnvironment::efpltSound,
		pEditPathSound, new cPathSound( *this ) );
	
	helper.EditString( groupBox, "", "", pLabSoundInfo, NULL );
	pLabSoundInfo->SetEditable( false );
	
	helper.EditFloat( groupBox, "Minimum Speed:",
		"Minimum play speed in percentage of normal playback speed. Use negative values to play backwards.",
		pEditMinSpeed, new cTextMinSpeed( *this ) );
	helper.EditFloat( groupBox, "Maximum Speed:",
		"Maximum play speed in percentage of normal playback speed. Use negative values to play backwards.",
		pEditMaxSpeed, new cTextMaxSpeed( *this ) );
	
	helper.CheckBox( groupBox, pChkLooping, new cActionLooping( *this ), true );
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
	if( synthesizer ){
		pEditPathSound->SetBasePath( synthesizer->GetDirectoryPath() );
		
	}else{
		pEditPathSound->SetBasePath( "" );
	}
}

void seWPAPanelSourceSound::UpdateSource(){
	seWPAPanelSource::UpdateSource();
	
	const seSourceSound * const source = ( seSourceSound* )GetSource();
	if( source ){
		pEditPathSound->SetPath( source->GetPathSound() );
		pEditMinSpeed->SetFloat( source->GetMinSpeed() );
		pEditMaxSpeed->SetFloat( source->GetMaxSpeed() );
		pChkLooping->SetChecked( source->GetLooping() );
		
	}else{
		pEditPathSound->ClearPath();
		pEditMinSpeed->ClearText();
		pEditMaxSpeed->ClearText();
		pChkLooping->SetChecked( false );
	}
	
	const bool enabled = source;
	pEditPathSound->SetEnabled( enabled );
	pEditMinSpeed->SetEnabled( enabled );
	pEditMaxSpeed->SetEnabled( enabled );
	pChkLooping->SetEnabled( enabled );
	
	UpdateSoundInfo();
}

void seWPAPanelSourceSound::UpdateTargetList(){
	seWPAPanelSource::UpdateTargetList();
	
	seSourceSound * const source = ( seSourceSound* )GetSource();
	if( source ){
		AddTarget( "Speed", &source->GetTargetSpeed() );
		AddTarget( "Play", &source->GetTargetPlay() );
	}
}

void seWPAPanelSourceSound::UpdateSoundInfo(){
	const seSourceSound * const source = ( seSourceSound* )GetSource();
	
	if( source && source->GetSound() ){
		const deSound &sound = *source->GetSound();
		decString text, description;
		bool invalidValue = false;
		
		text.Format( "%d Channels, %dHz, %d-bit, %d samples (%.2fs)",
			sound.GetChannelCount(), sound.GetSampleRate(), sound.GetBytesPerSample() * 8,
			sound.GetSampleCount(), ( float )sound.GetSampleCount() / ( float )sound.GetSampleRate() );
		
		if( GetSynthesizer()->GetChannelCount() < sound.GetChannelCount() ){
			if( ! description.IsEmpty() ){
				description.AppendCharacter( '\n' );
			}
			description += "Sound has more channels than synthesizer!";
			invalidValue = true;
		}
		
		if( GetSynthesizer()->GetSampleRate() != sound.GetSampleRate() ){
			if( ! description.IsEmpty() ){
				description.AppendCharacter( '\n' );
			}
			description += "Sound sample rate differs synthesizer!";
			invalidValue = true;
		}
		
		pLabSoundInfo->SetText( text );
		pLabSoundInfo->SetDescription( description );
		pLabSoundInfo->SetInvalidValue( invalidValue );
		
	}else{
		pLabSoundInfo->ClearText();
		pLabSoundInfo->SetDescription( "" );
		pLabSoundInfo->SetInvalidValue( false );
	}
}
