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

#include "meWPSensors.h"
#include "meWindowProperties.h"
#include "../meWindowMain.h"
#include "../../configuration/meConfiguration.h"
#include "../../world/meWorld.h"
#include "../../world/meLumimeter.h"
#include "../../worldedit.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeColorBox.h>
#include <deigde/gui/composed/igdeEditDVector.h>
#include <deigde/gui/composed/igdeEditDVectorListener.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>

#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace{

class cActionLMTrackCam : public igdeAction{
	meWPSensors &pPanel;
	
public:
	cActionLMTrackCam(meWPSensors &panel) : igdeAction("Track Camera", "Track camera."),
	pPanel(panel){}
	
	virtual void OnAction(){
		meWorld * const world = pPanel.GetWindowProperties().GetWindowMain().GetWorld();
		meLumimeter * const lumimeter = world ? world->GetLumimeter() : NULL;
		if(lumimeter){
			lumimeter->SetTrackCamera(! lumimeter->GetTrackCamera());
		}
	}
};

class cEditLMPos : public igdeEditDVectorListener{
	meWPSensors &pPanel;
	
public:
	cEditLMPos(meWPSensors &panel) : pPanel(panel){}
	
	virtual void OnDVectorChanged(igdeEditDVector *editDVector){
		meWorld * const world = pPanel.GetWindowProperties().GetWindowMain().GetWorld();
		meLumimeter * const lumimeter = world ? world->GetLumimeter() : NULL;
		if(lumimeter){
			lumimeter->SetPosition(editDVector->GetDVector());
		}
		
	}
};

class cEditLMDir : public igdeEditVectorListener{
	meWPSensors &pPanel;
	
public:
	cEditLMDir(meWPSensors &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		meWorld * const world = pPanel.GetWindowProperties().GetWindowMain().GetWorld();
		meLumimeter * const lumimeter = world ? world->GetLumimeter() : NULL;
		if(lumimeter){
			lumimeter->SetDirection(editVector->GetVector());
		}
		
	}
};

class cTextLMConeIA : public igdeTextFieldListener{
	meWPSensors &pPanel;
	
public:
	cTextLMConeIA(meWPSensors &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		meWorld * const world = pPanel.GetWindowProperties().GetWindowMain().GetWorld();
		meLumimeter * const lumimeter = world ? world->GetLumimeter() : NULL;
		if(lumimeter){
			lumimeter->SetConeInnerAngle(textField->GetFloat());
		}
	}
};

class cTextLMConeOA : public igdeTextFieldListener{
	meWPSensors &pPanel;
	
public:
	cTextLMConeOA(meWPSensors &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		meWorld * const world = pPanel.GetWindowProperties().GetWindowMain().GetWorld();
		meLumimeter * const lumimeter = world ? world->GetLumimeter() : NULL;
		if(lumimeter){
			lumimeter->SetConeOuterAngle(textField->GetFloat());
		}
	}
};

class cTextLMConeExp : public igdeTextFieldListener{
	meWPSensors &pPanel;
	
public:
	cTextLMConeExp(meWPSensors &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		meWorld * const world = pPanel.GetWindowProperties().GetWindowMain().GetWorld();
		meLumimeter * const lumimeter = world ? world->GetLumimeter() : NULL;
		if(lumimeter){
			lumimeter->SetConeExponent(textField->GetFloat());
		}
	}
};

}



// Class meWPSensors
//////////////////////

// Constructor, destructor
////////////////////////////

meWPSensors::meWPSensors(meWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox;
	
// 	pListener = new meWPViewListener( *this );
	
	content.TakeOver(new igdeContainerFlow(env, igdeContainerFlow::eaY));
	AddChild(content);
	
	
	// lumimeter
	helper.GroupBox(content, groupBox, "Lumimeter:");
	
	helper.CheckBox(groupBox, pChkLMTrackCam, new cActionLMTrackCam(*this), true);
	helper.EditDVector(groupBox, "Position:", "Position", pEditLMPos, new cEditLMPos(*this));
	helper.EditVector(groupBox, "Direction:", "Direction", pEditLMDir, new cEditLMDir(*this));
	helper.EditFloat(groupBox, "Inner Angle:", "Cone inner angle in degrees",
		pEditLMConeIA, new cTextLMConeIA(*this));
	helper.EditFloat(groupBox, "Outer Angle:", "Cone outer angle in degrees",
		pEditLMConeOA, new cTextLMConeOA(*this));
	helper.EditFloat(groupBox, "Exponent:",
		"Cone exponent smoothing between inner and outer angle",
		pEditLMConeExp, new cTextLMConeExp(*this));
	
	helper.EditFloat(groupBox, "Measured Luminance:", "Measured luminance", pEditLMLumi, NULL);
	pEditLMLumi->SetEditable(false);
	helper.ColorBox(groupBox, "Measured Color:", "Measured color", pEditLMColor, NULL);
	pEditLMColor->SetEnabled(false);
}

meWPSensors::~meWPSensors(){
}



// Management
///////////////

void meWPSensors::UpdateSensors(){
	UpdateLumimeter();
}

void meWPSensors::UpdateLumimeter(){
	meWorld * const world = pWindowProperties.GetWindowMain().GetWorld();
	meLumimeter * lumimeter = world ? world->GetLumimeter() : NULL;
	
	if(lumimeter){
		pChkLMTrackCam->SetChecked(lumimeter->GetTrackCamera());
		pEditLMPos->SetDVector(lumimeter->GetPosition());
		pEditLMDir->SetVector(lumimeter->GetDirection());
		pEditLMConeIA->SetFloat(lumimeter->GetConeInnerAngle());
		pEditLMConeOA->SetFloat(lumimeter->GetConeOuterAngle());
		pEditLMConeExp->SetFloat(lumimeter->GetConeExponent());
		
		pEditLMLumi->SetFloat(lumimeter->MeasureLuminance());
		pEditLMColor->SetColor(lumimeter->MeasureColor());
		
	}else{
		pChkLMTrackCam->SetChecked(false);
		pEditLMPos->SetDVector(decDVector());
		pEditLMDir->SetVector(decVector());
		pEditLMConeIA->ClearText();
		pEditLMConeOA->ClearText();
		pEditLMConeExp->ClearText();
		pEditLMLumi->ClearText();
		pEditLMColor->SetColor(decColor());
	}
	
	const bool enable = lumimeter;
	pChkLMTrackCam->SetEnabled(enable);
	pEditLMPos->SetEnabled(enable);
	pEditLMDir->SetEnabled(enable);
	pEditLMConeIA->SetEnabled(enable);
	pEditLMConeOA->SetEnabled(enable);
	pEditLMConeExp->SetEnabled(enable);
}
