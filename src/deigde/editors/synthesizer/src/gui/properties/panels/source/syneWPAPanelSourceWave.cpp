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

#include "syneWPAPanelSourceWave.h"
#include "../../syneWPSource.h"
#include "../../../syneWindowMain.h"
#include "../../../../synthesizerEditor.h"
#include "../../../../synthesizer/syneSynthesizer.h"
#include "../../../../synthesizer/controller/syneController.h"
#include "../../../../synthesizer/source/syneSourceWave.h"
#include "../../../../undosys/source/wave/syneUSourceWaveSetType.h"
#include "../../../../undosys/source/wave/syneUSourceWaveSetMinFrequency.h"
#include "../../../../undosys/source/wave/syneUSourceWaveSetMaxFrequency.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	syneWPAPanelSourceWave &pPanel;
	
public:
	cBaseTextFieldListener(syneWPAPanelSourceWave &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		syneSourceWave * const source = (syneSourceWave*)pPanel.GetSource();
		if(!source){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, source));
		if(undo){
			source->GetSynthesizer()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, syneSourceWave *source) = 0;
};


class cComboType : public igdeComboBoxListener{
	syneWPAPanelSourceWave &pPanel;
	
public:
	typedef deTObjectReference<cComboType> Ref;
	cComboType(syneWPAPanelSourceWave &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		syneSourceWave * const source = (syneSourceWave*)pPanel.GetSource();
		if(!source || !comboBox->GetSelectedItem()){
			return;
		}
		
		const deSynthesizerSourceWave::eWaveType type = (deSynthesizerSourceWave::eWaveType)
			(intptr_t)comboBox->GetSelectedItem()->GetData();
		if(type == source->GetWaveType()){
			return;
		}
		
		source->GetSynthesizer()->GetUndoSystem()->Add(
			syneUSourceWaveSetType::Ref::New(source, type));
	}
};

class cTextMinFrequency : public cBaseTextFieldListener {
public:
	typedef deTObjectReference<cTextMinFrequency> Ref;
	cTextMinFrequency(syneWPAPanelSourceWave &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(igdeTextField *textField, syneSourceWave *source){
		const float value = (float)textField->GetInteger();
		return fabsf(value - source->GetMinFrequency()) < FLOAT_SAFE_EPSILON
			? syneUSourceWaveSetMinFrequency::Ref::New(source, value) : igdeUndo::Ref();
	}
};

class cTextMaxFrequency : public cBaseTextFieldListener {
public:
	typedef deTObjectReference<cTextMaxFrequency> Ref;
	cTextMaxFrequency(syneWPAPanelSourceWave &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(igdeTextField *textField, syneSourceWave *source){
		const float value = (float)textField->GetInteger();
		return fabsf(value - source->GetMaxFrequency()) < FLOAT_SAFE_EPSILON
			? syneUSourceWaveSetMaxFrequency::Ref::New(source, value) : igdeUndo::Ref();
	}
};

}



// Class syneWPAPanelSourceWave
///////////////////////////////

// Constructor, destructor
////////////////////////////

syneWPAPanelSourceWave::syneWPAPanelSourceWave(syneWPSource &wpSource) :
syneWPAPanelSource(wpSource, deSynthesizerSourceVisitorIdentify::estWave)
{
	igdeEnvironment &env = wpSource.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref groupBox;
	
	
	helper.GroupBox(*this, groupBox, "Wave:");
	
	helper.ComboBox(groupBox, "Wave Type:", "Select wave type", pCBType, cComboType::Ref::New(*this));
	pCBType->AddItem("Sine", nullptr, (void*)(intptr_t)deSynthesizerSourceWave::ewtSine);
	pCBType->AddItem("Square", nullptr, (void*)(intptr_t)deSynthesizerSourceWave::ewtSquare);
	pCBType->AddItem("Saw-Tooth", nullptr, (void*)(intptr_t)deSynthesizerSourceWave::ewtSawTooth);
	pCBType->AddItem("Triangle", nullptr, (void*)(intptr_t)deSynthesizerSourceWave::ewtTriangle);
	
	helper.EditInteger(groupBox, "Minimum Frequency:", "Minimum frequency in Hz.",
		pEditMinFrequency, cTextMinFrequency::Ref::New(*this));
	helper.EditInteger(groupBox, "Maximum Frequency:", "Maximum frequency in Hz",
		pEditMaxFrequency, cTextMaxFrequency::Ref::New(*this));
}

syneWPAPanelSourceWave::~syneWPAPanelSourceWave(){
}



// Management
///////////////

void syneWPAPanelSourceWave::UpdateSource(){
	syneWPAPanelSource::UpdateSource();
	
	const syneSourceWave * const source = (syneSourceWave*)GetSource();
	if(source){
		pCBType->SetSelectionWithData((void*)(intptr_t)source->GetWaveType());
		pEditMinFrequency->SetInteger((int)(source->GetMinFrequency() + 0.1f));
		pEditMaxFrequency->SetInteger((int)(source->GetMaxFrequency() + 0.1f));
		
	}else{
		pCBType->SetSelectionWithData((void*)(intptr_t)deSynthesizerSourceWave::ewtSine);
		pEditMinFrequency->ClearText();
		pEditMaxFrequency->ClearText();
	}
	
	const bool enabled = source;
	pCBType->SetEnabled(enabled);
	pEditMinFrequency->SetEnabled(enabled);
	pEditMaxFrequency->SetEnabled(enabled);
}

void syneWPAPanelSourceWave::UpdateTargetList(){
	syneSourceWave *source = (syneSourceWave*)GetSource();
	
	syneWPAPanelSource::UpdateTargetList();
	
	if(source){
		AddTarget("Frequency", source->GetTargetFrequency());
	}
}
