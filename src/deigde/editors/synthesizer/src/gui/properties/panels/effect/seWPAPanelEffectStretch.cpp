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

#include "seWPAPanelEffectStretch.h"
#include "../../seWPEffect.h"
#include "../../../seWindowMain.h"
#include "../../../../synthesizerEditor.h"
#include "../../../../synthesizer/seSynthesizer.h"
#include "../../../../synthesizer/controller/seController.h"
#include "../../../../synthesizer/effect/seEffectStretch.h"
#include "../../../../undosys/source/effect/stretch/seUEffectStretchSetMaxPitch.h"
#include "../../../../undosys/source/effect/stretch/seUEffectStretchSetMaxTime.h"
#include "../../../../undosys/source/effect/stretch/seUEffectStretchSetMinPitch.h"
#include "../../../../undosys/source/effect/stretch/seUEffectStretchSetMinTime.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	seWPAPanelEffectStretch &pPanel;
	
public:
	cBaseTextFieldListener(seWPAPanelEffectStretch &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		seEffectStretch * const effect = (seEffectStretch*)pPanel.GetEffect();
		if(!effect){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, effect));
		if(undo){
			effect->GetSynthesizer()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, seEffectStretch *effect) = 0;
};


class cTextMinTime : public cBaseTextFieldListener {
public:
	typedef deTObjectReference<cTextMinTime> Ref;
	cTextMinTime(seWPAPanelEffectStretch &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(igdeTextField *textField, seEffectStretch *effect){
		const float value = textField->GetFloat();
		return fabsf(value - effect->GetMinTime()) > FLOAT_SAFE_EPSILON
			? seUEffectStretchSetMinTime::Ref::New(effect, value) : igdeUndo::Ref();
	}
};

class cTextMaxTime : public cBaseTextFieldListener {
public:
	typedef deTObjectReference<cTextMaxTime> Ref;
	cTextMaxTime(seWPAPanelEffectStretch &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(igdeTextField *textField, seEffectStretch *effect){
		const float value = textField->GetFloat();
		return fabsf(value - effect->GetMaxTime()) > FLOAT_SAFE_EPSILON
			? seUEffectStretchSetMaxTime::Ref::New(effect, value) : igdeUndo::Ref();
	}
};

class cTextMinPitch : public cBaseTextFieldListener {
public:
	typedef deTObjectReference<cTextMinPitch> Ref;
	cTextMinPitch(seWPAPanelEffectStretch &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(igdeTextField *textField, seEffectStretch *effect){
		const float value = textField->GetFloat();
		return fabsf(value - effect->GetMinPitch()) > FLOAT_SAFE_EPSILON
			? seUEffectStretchSetMinPitch::Ref::New(effect, value) : igdeUndo::Ref();
	}
};

class cTextMaxPitch : public cBaseTextFieldListener {
public:
	typedef deTObjectReference<cTextMaxPitch> Ref;
	cTextMaxPitch(seWPAPanelEffectStretch &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(igdeTextField *textField, seEffectStretch *effect){
		const float value = textField->GetFloat();
		return fabsf(value - effect->GetMaxPitch()) > FLOAT_SAFE_EPSILON
			? seUEffectStretchSetMaxPitch::Ref::New(effect, value) : igdeUndo::Ref();
	}
};

}



// Class seWPAPanelEffectStretch
//////////////////////////////////

// Constructor, destructor
////////////////////////////

seWPAPanelEffectStretch::seWPAPanelEffectStretch(seWPEffect &wpEffect) :
seWPAPanelEffect(wpEffect, deSynthesizerEffectVisitorIdentify::eetStretch)
{
	igdeEnvironment &env = wpEffect.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref groupBox;
	
	
	helper.GroupBox(*this, groupBox, "Stretch Time/Pitch:");
	helper.EditFloat(groupBox, "Time Minimum:", "Set minimum time stretch",
		pEditMinTime, cTextMinTime::Ref::New(*this));
	helper.EditFloat(groupBox, "Time Maximum:", "Set maximum time stretch",
		pEditMaxTime, cTextMaxTime::Ref::New(*this));
	helper.EditFloat(groupBox, "Pitch Minimum:", "Set minimum pitch stretch",
		pEditMinPitch, cTextMinPitch::Ref::New(*this));
	helper.EditFloat(groupBox, "Pitch Maximum:", "Set maximum pitch stretch",
		pEditMaxPitch, cTextMaxPitch::Ref::New(*this));
}

seWPAPanelEffectStretch::~seWPAPanelEffectStretch(){
}



// Management
///////////////

void seWPAPanelEffectStretch::UpdateEffect(){
	seWPAPanelEffect::UpdateEffect();
	
	const seEffectStretch * const effect = (seEffectStretch*)GetEffect();
	if(effect){
		pEditMinTime->SetFloat(effect->GetMinTime());
		pEditMaxTime->SetFloat(effect->GetMaxTime());
		pEditMinPitch->SetFloat(effect->GetMinPitch());
		pEditMaxPitch->SetFloat(effect->GetMaxPitch());
		
	}else{
		pEditMinTime->ClearText();
		pEditMaxTime->ClearText();
		pEditMinPitch->ClearText();
		pEditMaxPitch->ClearText();
	}
	
	const bool enabled = effect;
	pEditMinTime->SetEnabled(enabled);
	pEditMaxTime->SetEnabled(enabled);
	pEditMinPitch->SetEnabled(enabled);
	pEditMaxPitch->SetEnabled(enabled);
}

void seWPAPanelEffectStretch::UpdateTargetList(){
	seWPAPanelEffect::UpdateTargetList();
	
	seEffectStretch * const effect = (seEffectStretch*)GetEffect();
	if(effect){
		AddTarget("Time", &effect->GetTargetTime());
		AddTarget("Pitch", &effect->GetTargetPitch());
	}
}
