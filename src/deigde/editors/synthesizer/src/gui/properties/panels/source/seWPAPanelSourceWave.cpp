/* 
 * Drag[en]gine IGDE Synthesizer Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <math.h>
#include <unistd.h>

#include "seWPAPanelSourceWave.h"
#include "../../seWPSource.h"
#include "../../../seWindowMain.h"
#include "../../../../synthesizerEditor.h"
#include "../../../../synthesizer/seSynthesizer.h"
#include "../../../../synthesizer/controller/seController.h"
#include "../../../../synthesizer/source/seSourceWave.h"
#include "../../../../undosys/source/wave/seUSourceWaveSetType.h"
#include "../../../../undosys/source/wave/seUSourceWaveSetMinFrequency.h"
#include "../../../../undosys/source/wave/seUSourceWaveSetMaxFrequency.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	seWPAPanelSourceWave &pPanel;
	
public:
	cBaseTextFieldListener( seWPAPanelSourceWave &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		seSourceWave * const source = ( seSourceWave* )pPanel.GetSource();
		if( ! source ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( textField, source ) );
		if( undo ){
			source->GetSynthesizer()->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, seSourceWave *source ) = 0;
};


class cComboType : public igdeComboBoxListener{
	seWPAPanelSourceWave &pPanel;
	
public:
	cComboType( seWPAPanelSourceWave &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		seSourceWave * const source = ( seSourceWave* )pPanel.GetSource();
		if( ! source || ! comboBox->GetSelectedItem() ){
			return;
		}
		
		const deSynthesizerSourceWave::eWaveType type = ( deSynthesizerSourceWave::eWaveType )
			( intptr_t )comboBox->GetSelectedItem()->GetData();
		if( type == source->GetWaveType() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new seUSourceWaveSetType( source, type ) );
		source->GetSynthesizer()->GetUndoSystem()->Add( undo );
	}
};

class cTextMinFrequency : public cBaseTextFieldListener {
public:
	cTextMinFrequency( seWPAPanelSourceWave &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo * OnChanged( igdeTextField *textField, seSourceWave *source ){
		const int value = textField->GetInteger();
		return value != source->GetMinFrequency() ? new seUSourceWaveSetMinFrequency( source, value ) : NULL;
	}
};

class cTextMaxFrequency : public cBaseTextFieldListener {
public:
	cTextMaxFrequency( seWPAPanelSourceWave &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo * OnChanged( igdeTextField *textField, seSourceWave *source ){
		const int value = textField->GetInteger();
		return value != source->GetMaxFrequency() ? new seUSourceWaveSetMaxFrequency( source, value ) : NULL;
	}
};

}



// Class seWPAPanelSourceWave
///////////////////////////////

// Constructor, destructor
////////////////////////////

seWPAPanelSourceWave::seWPAPanelSourceWave( seWPSource &wpSource ) :
seWPAPanelSource( wpSource, deSynthesizerSourceVisitorIdentify::estWave )
{
	igdeEnvironment &env = wpSource.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference groupBox;
	
	
	helper.GroupBox( *this, groupBox, "Wave:" );
	
	helper.ComboBox( groupBox, "Wave Type:", "Select wave type", pCBType, new cComboType( *this ) );
	pCBType->AddItem( "Sine", NULL, ( void* )( intptr_t )deSynthesizerSourceWave::ewtSine );
	pCBType->AddItem( "Square", NULL, ( void* )( intptr_t )deSynthesizerSourceWave::ewtSquare );
	pCBType->AddItem( "Saw-Tooth", NULL, ( void* )( intptr_t )deSynthesizerSourceWave::ewtSawTooth );
	pCBType->AddItem( "Triangle", NULL, ( void* )( intptr_t )deSynthesizerSourceWave::ewtTriangle );
	
	helper.EditInteger( groupBox, "Minimum Frequency:", "Minimum frequency in Hz.",
		pEditMinFrequency, new cTextMinFrequency( *this ) );
	helper.EditInteger( groupBox, "Maximum Frequency:", "Maximum frequency in Hz",
		pEditMaxFrequency, new cTextMaxFrequency( *this ) );
}

seWPAPanelSourceWave::~seWPAPanelSourceWave(){
}



// Management
///////////////

void seWPAPanelSourceWave::UpdateSource(){
	seWPAPanelSource::UpdateSource();
	
	const seSourceWave * const source = ( seSourceWave* )GetSource();
	if( source ){
		pCBType->SetSelectionWithData( ( void* )( intptr_t )source->GetWaveType() );
		pEditMinFrequency->SetInteger( source->GetMinFrequency() );
		pEditMaxFrequency->SetInteger( source->GetMaxFrequency() );
		
	}else{
		pCBType->SetSelectionWithData( ( void* )( intptr_t )deSynthesizerSourceWave::ewtSine );
		pEditMinFrequency->ClearText();
		pEditMaxFrequency->ClearText();
	}
	
	const bool enabled = source;
	pCBType->SetEnabled( enabled );
	pEditMinFrequency->SetEnabled( enabled );
	pEditMaxFrequency->SetEnabled( enabled );
}

void seWPAPanelSourceWave::UpdateTargetList(){
	seSourceWave *source = ( seSourceWave* )GetSource();
	
	seWPAPanelSource::UpdateTargetList();
	
	if( source ){
		AddTarget( "Frequency", &source->GetTargetFrequency() );
	}
}
