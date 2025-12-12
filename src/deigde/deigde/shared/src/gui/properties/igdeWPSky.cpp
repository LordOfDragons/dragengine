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

#include "igdeWPSky.h"
#include "../igdeUIHelper.h"
#include "../igdeCommonDialogs.h"
#include "../igdeTextField.h"
#include "../igdeButton.h"
#include "../igdeLabel.h"
#include "../igdeGroupBox.h"
#include "../igdeSlider.h"
#include "../browse/igdeDialogBrowserSky.h"
#include "../composed/igdeEditSliderText.h"
#include "../composed/igdeEditSliderTextListener.h"
#include "../event/igdeAction.h"
#include "../event/igdeTextFieldListener.h"
#include "../layout/igdeContainerForm.h"
#include "../layout/igdeContainerFlow.h"
#include "../wrapper/igdeWSky.h"
#include "../../environment/igdeEnvironment.h"
#include "../../gamedefinition/sky/igdeGDSky.h"
#include "../../gamedefinition/sky/igdeGDSkyManager.h"
#include "../../gamedefinition/igdeGameDefinition.h"
#include "../../gameproject/igdeGameProject.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/sky/deSkyController.h>



// Actions
////////////

namespace{

class cTextPathSky : public igdeTextFieldListener{
	igdeWPSky &pPanel;
	
public:
	cTextPathSky(igdeWPSky &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		if(!pPanel.GetSky() || textField->GetText() == pPanel.GetSky()->GetPath()){
			return;
		}
		
		pPanel.GetSky()->SetPath(textField->GetText());
		pPanel.UpdateSky();
		pPanel.OnAction();
	}
};

class cActionFromGDSky : public igdeAction{
	igdeWPSky &pPanel;
	igdeTextField &pTextField;
	
public:
	cActionFromGDSky(igdeWPSky &panel, igdeTextField &textField) :
	igdeAction("GDef", "Show dialog to select sky from game definition"),
	pPanel(panel), pTextField(textField){}
	
	virtual void OnAction(){
		if(!pPanel.GetSky()){
			return;
		}
		
		decString sky(pTextField.GetText());
		if(!igdeDialogBrowserSky::SelectSky(&pPanel, sky)){
			return;
		}
		
		if(sky == pPanel.GetSky()->GetPath()){
			return;
		}
		
		pPanel.GetSky()->SetPath(sky);
		pPanel.UpdateSky();
		pPanel.OnAction();
	}
};

class cActionPathSky : public igdeAction{
	igdeWPSky &pPanel;
	igdeTextField &pTextField;
	
public:
	cActionPathSky(igdeWPSky &panel, igdeTextField &textField) :
	igdeAction("...", "Show dialog to select the sky from file"),
	pPanel(panel), pTextField(textField){}
	
	virtual void OnAction(){
		if(!pPanel.GetSky()){
			return;
		}
		
		decString filename(pTextField.GetText().IsEmpty() ? decString("/igde/skies") : pTextField.GetText());
		if(!igdeCommonDialogs::GetFileOpen(&pPanel, "Select Sky",
		*pPanel.GetEnvironment().GetFileSystemGame(),
		*pPanel.GetEnvironment().GetOpenFilePatternList( igdeEnvironment::efpltSky ), filename ) ){
			return;
		}
		
		if(filename == pPanel.GetSky()->GetPath()){
			return;
		}
		
		pPanel.GetSky()->SetPath(filename);
		pPanel.UpdateSky();
		pPanel.OnAction();
	}
};

class cEditControllerValue : public igdeEditSliderTextListener{
protected:
	igdeWPSky &pPanel;
	int pIndex;
	
public:
	cEditControllerValue(igdeWPSky &panel, int index) :
	pPanel(panel), pIndex(index){}
	
	virtual void OnSliderTextValueChanging(igdeEditSliderText *sliderText){
		if(!pPanel.GetSky()){
			return;
		}
		
		pPanel.GetSky()->SetControllerValue(pIndex, sliderText->GetValue());
	}
	
	virtual void OnSliderTextValueChanged(igdeEditSliderText *sliderText){
		OnSliderTextValueChanging(sliderText);
		pPanel.OnAction();
	}
};

}



// Class igdeWPSky
////////////////////

// Constructor, destructor
////////////////////////////

igdeWPSky::igdeWPSky(igdeEnvironment &environment) :
igdeContainerFlow(environment, igdeContainerFlow::eaY),
pSky(NULL)
{
	pCreateContent();
}

igdeWPSky::igdeWPSky(igdeEnvironment &environment, igdeAction *action) :
igdeContainerFlow(environment, igdeContainerFlow::eaY),
pSky(NULL)
{
	pCreateContent();
	SetAction(action);
}

igdeWPSky::~igdeWPSky(){
	DestroyNativeWidget();
	SetAction(NULL);
}



// Management
///////////////

void igdeWPSky::SetSky(igdeWSky *sky){
	if(sky == pSky){
		return;
	}
	
	pSky = sky;
	UpdateSky();
}

void igdeWPSky::UpdateSky(){
	if(pSky){
		pEditSkyPath->SetText(pSky->GetPath());
		
	}else{
		pEditSkyPath->ClearText();
	}
	
	pEditSkyPath->SetEnabled(pSky != NULL);
	
	RebuildControllers();
}

void igdeWPSky::RebuildControllers(){
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	const int controllerCount = pSky ? pSky->GetControllerCount() : 0;
	
	// remove superfluous widgets
	while(pControllers.GetCount() > controllerCount){
		Controller &controller = *((Controller*)pControllers.GetAt(pControllers.GetCount() - 1));
		pFraControllers->RemoveChild(controller.slider);
		pFraControllers->RemoveChild(controller.label);
		pControllers.RemoveFrom(pControllers.GetCount() - 1);
	}
	
	// add new widgets
	while(pControllers.GetCount() < controllerCount){
		Controller::Ref controller(Controller::Ref::New());
		
		controller->controller = pControllers.GetCount();
		
		helper.EditSliderText(pFraControllers, "", "Current value of the controller ''",
			0.0f, 1.0f, 6, 3, 0.1f, controller->slider,
			new cEditControllerValue(*this, controller->controller));
		
		controller->label = (igdeLabel*)pFraControllers->GetChildAt(
			pFraControllers->IndexOfChild(controller->slider) - 1);
		
		pControllers.Add(controller);
	}
	
	// update widgets
	int i;
	for(i=0; i<controllerCount; i++){
		UpdateController(i);
	}
}

void igdeWPSky::UpdateController(int index){
	Controller &controller = *((Controller*)pControllers.GetAt(index));
	const deSkyController &skyController = pSky->GetControllerAt(index);
	
	const decString &name = skyController.GetName();
	
	if(name != controller.name){
		controller.name = name;
		controller.label->SetText(name);
		
		decString description;
		description.Format("Current value of the controller '%s'", name.GetString());
		controller.label->SetDescription(description);
		controller.slider->SetDescription(description);
	}
	
	const float minimum = skyController.GetMinimumValue();
	const float maximum = skyController.GetMaximumValue();
	const float value = skyController.GetCurrentValue();
	
	if(minimum != controller.minimum || maximum != controller.maximum){
		controller.minimum = minimum;
		controller.maximum = maximum;
		
		if(maximum > minimum){
			controller.slider->SetRange(minimum, maximum);
			controller.slider->SetTickSpacing((maximum - minimum) * 0.1f);
			controller.slider->SetValue(value);
			controller.slider->SetEnabled(true);
			
		}else{
			controller.slider->SetRange(0.0f, 1.0f);
			controller.slider->SetTickSpacing(0.1f);
			controller.slider->SetValue(0.0f);
			controller.slider->SetEnabled(false);
		}
	}
	
	controller.slider->SetValue(value);
}

void igdeWPSky::UpdateControllerValue(int controller){
	igdeEditSliderText &slider = ((Controller*)pControllers.GetAt(controller))->slider;
	const deSkyController &skyController = pSky->GetControllerAt(controller);
	
	slider.SetValue(skyController.GetCurrentValue());
}



void igdeWPSky::SetAction(igdeAction *action){
	if(pAction == action){
		return;
	}
	
	if(pAction){
		pAction->RemoveListener(this);
	}
	
	pAction = action;
	
	if(action){
		action->AddListener(this);
		OnParameterChanged(action);
	}
}

void igdeWPSky::OnAction(){
	if(pAction){
		pAction->OnAction();
	}
}

void igdeWPSky::OnParameterChanged(igdeAction *action){
	//SetEnabled( action->GetEnabled() );
}

void igdeWPSky::OnDestroyed(igdeAction *action){
	GetLogger()->LogWarnFormat("IGDE", "igdeWPSky::OnDestroyed: "
		"Action(%s) destroyed while still listening on it", action->GetText().GetString());
	pAction = NULL;
}



// Private Functions
//////////////////////

void igdeWPSky::pCreateContent(){
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref form, frameLine;
	
	form.TakeOver(new igdeContainerForm(env));
	AddChild(form);
	
	helper.FormLineStretchFirst(form, "Sky Path:", "", frameLine);
	helper.EditString(frameLine, "Path to the sky to use.",
		pEditSkyPath, new cTextPathSky(*this));
	helper.Button(frameLine, pBtnSkyPath, new cActionPathSky(*this, pEditSkyPath), true);
	helper.Button(frameLine, pBtnFromGDSky, new cActionFromGDSky(*this, pEditSkyPath), true);
	
	helper.GroupBox(*this, pFraControllers, "Controllers:");
}
