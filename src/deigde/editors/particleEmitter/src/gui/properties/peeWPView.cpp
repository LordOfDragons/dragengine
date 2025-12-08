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

#include "peeWPView.h"
#include "peeWPViewListener.h"
#include "peeWindowProperties.h"
#include "../peeWindowMain.h"
#include "../../emitter/peeEmitter.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCamera.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/properties/igdeWPCamera.h>
#include <deigde/gui/properties/igdeWPSky.h>
#include <deigde/gui/properties/igdeWPWObject.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	peeWPView &pPanel;
public:
	cBaseAction(peeWPView &panel, const char *text, const char *description) :
	igdeAction(text, description), pPanel(panel){}
	
	virtual void OnAction(){
		peeEmitter * const emitter = pPanel.GetEmitter();
		if(emitter){
			OnAction(*emitter);
		}
	}
	
	virtual void OnAction(peeEmitter &emitter) = 0;
};

class cActionSkyChanged : public cBaseAction{
public:
	cActionSkyChanged(peeWPView &panel) : cBaseAction(panel, "", ""){}
	
	void OnAction(peeEmitter &emitter) override{
		emitter.NotifySkyChanged();
	}
};

class cActionCameraChanged : public cBaseAction{
public:
	cActionCameraChanged(peeWPView &panel) : cBaseAction(panel, "", ""){}
	
	void OnAction(peeEmitter &emitter) override{
		emitter.NotifyCameraChanged();
	}
};

class cActionEnvObjChanged : public cBaseAction{
public:
	cActionEnvObjChanged(peeWPView &panel) : cBaseAction(panel, "", ""){}
	
	void OnAction(peeEmitter &emitter) override{
		emitter.NotifyEnvObjectChanged();
	}
};

class cEditPosition : public igdeEditVectorListener{
	peeWPView &pPanel;
public:
	cEditPosition(peeWPView &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		peeEmitter * const emitter = pPanel.GetEmitter();
		if(emitter){
			emitter->SetPosition(editVector->GetVector());
		}
	}
};

class cEditRotation : public igdeEditVectorListener{
	peeWPView &pPanel;
public:
	cEditRotation(peeWPView &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		peeEmitter * const emitter = pPanel.GetEmitter();
		if(emitter){
			emitter->SetOrientation(editVector->GetVector());
		}
	}
};

class cEditBurstInterval : public igdeTextFieldListener{
	peeWPView &pPanel;
public:
	cEditBurstInterval(peeWPView &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		peeEmitter * const emitter = pPanel.GetEmitter();
		if(emitter){
			emitter->SetBurstInterval(textField->GetFloat());
		}
	}
};

class cEditWarmUpTime : public igdeTextFieldListener{
	peeWPView &pPanel;
public:
	cEditWarmUpTime(peeWPView &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		peeEmitter * const emitter = pPanel.GetEmitter();
		if(emitter){
			emitter->SetWarmUpTime(textField->GetFloat());
		}
	}
};

class cActionEnableCasting : public cBaseAction{
public:
	cActionEnableCasting(peeWPView &panel) :
	cBaseAction(panel, "Enable Casting", "Determines if casting is enabled"){ }
	
	void OnAction(peeEmitter &emitter) override{
		emitter.SetEnableCasting(!emitter.GetEnableCasting());
	}
};

}



// Class peeWPView
///////////////////

// Constructor, destructor
////////////////////////////

peeWPView::peeWPView(peeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox;
	
	pListener.TakeOver(new peeWPViewListener(*this));
	
	content.TakeOver(new igdeContainerFlow(env, igdeContainerFlow::eaY));
	AddChild(content);
	
	helper.WPCamera(content, pWPCamera, new cActionCameraChanged(*this),
		"Camera:", false, false, true);
	helper.WPSky(content, pWPSky, new cActionSkyChanged(*this),
		"Sky:", false, false, true);
	helper.WPWObject(content, pWPEnvObject, new cActionEnvObjChanged(*this),
		"Environment Object:", false, false, true);
	
	// preview settings
	helper.GroupBox(content, groupBox, "Preview:");
	helper.EditVector(groupBox, "Position:", "Position of the emitter in meters.",
		pEditEmitterPosition, new cEditPosition(*this));
	helper.EditVector(groupBox, "Rotation:", "Rotation of the emitter in degrees",
		pEditEmitterRotation, new cEditRotation(*this));
	helper.EditFloat(groupBox, "Burst Interval:", "Interval in seconds between bursts",
		pEditEmitterBurstInterval, new cEditBurstInterval(*this));
	helper.EditFloat(groupBox, "Warm-Up Time:",
		"Warm-Up time in seconds for the first enabling casting",
		pEditEmitterWarmUpTime, new cEditWarmUpTime(*this));
	helper.CheckBox(groupBox, pChkEmitterEnableCasting, new cActionEnableCasting(*this), true);
}

peeWPView::~peeWPView(){
	SetEmitter(NULL);
}



// Management
///////////////

void peeWPView::SetEmitter(peeEmitter::Ref emitter){
	if(emitter == pEmitter){
		return;
	}
	
	pWPEnvObject->SetObject(nullptr);
	pWPSky->SetSky(NULL);
	pWPCamera->SetCamera(NULL);
	
	if(pEmitter){
		pEmitter->RemoveListener(pListener);
		pEmitter = NULL;
	}
	
	pEmitter = emitter;
	
	if(emitter){
		emitter->AddListener(pListener);
		pWPSky->SetSky(emitter->GetSky());
		pWPEnvObject->SetObject(emitter->GetEnvObject());
		pWPCamera->SetCamera(emitter->GetCamera());
	}
	
	UpdateView();
	UpdateSky();
	UpdateEnvObject();
	UpdateCamera();
}



void peeWPView::UpdateView(){
	if(pEmitter){
		pEditEmitterPosition->SetVector(pEmitter->GetPosition());
		pEditEmitterRotation->SetVector(pEmitter->GetOrientation());
		pEditEmitterBurstInterval->SetFloat(pEmitter->GetBurstInterval());
		pEditEmitterWarmUpTime->SetFloat(pEmitter->GetWarmUpTime());
		pChkEmitterEnableCasting->SetChecked(pEmitter->GetEnableCasting());
		
	}else{
		pEditEmitterPosition->SetVector(decVector());
		pEditEmitterRotation->SetVector(decVector());
		pEditEmitterBurstInterval->ClearText();
		pEditEmitterWarmUpTime->ClearText();
		pChkEmitterEnableCasting->SetChecked(false);
	}
	
	const bool enabled = pEmitter != NULL;
	pEditEmitterPosition->SetEnabled(enabled);
	pEditEmitterRotation->SetEnabled(enabled);
	pEditEmitterBurstInterval->SetEnabled(enabled);
	pEditEmitterWarmUpTime->SetEnabled(enabled);
	pChkEmitterEnableCasting->SetEnabled(enabled);
}

void peeWPView::UpdateSky(){
	pWPSky->UpdateSky();
}

void peeWPView::UpdateEnvObject(){
	pWPEnvObject->UpdateObject();
}

void peeWPView::UpdateCamera(){
	pWPCamera->UpdateCamera();
	pWPCamera->UpdateViewDirection();
}
