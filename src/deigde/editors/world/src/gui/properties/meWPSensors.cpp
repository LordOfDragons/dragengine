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
	using Ref = deTObjectReference<cActionLMTrackCam>;
	cActionLMTrackCam(meWPSensors &panel) : igdeAction("@World.WPSensors.Action.TrackCamera", "@World.WPSensors.TrackCamera"),
	pPanel(panel){}
	
	void OnAction() override{
		meWorld * const world = pPanel.GetWindowProperties().GetWindowMain().GetWorld();
		meLumimeter * const lumimeter = world ? world->GetLumimeter().Pointer() : nullptr;
		if(lumimeter){
			lumimeter->SetTrackCamera(!lumimeter->GetTrackCamera());
		}
	}
};

class cEditLMPos : public igdeEditDVectorListener{
	meWPSensors &pPanel;
	
public:
	using Ref = deTObjectReference<cEditLMPos>;
	cEditLMPos(meWPSensors &panel) : pPanel(panel){}
	
	void OnDVectorChanged(igdeEditDVector *editDVector) override{
		meWorld * const world = pPanel.GetWindowProperties().GetWindowMain().GetWorld();
		meLumimeter * const lumimeter = world ? world->GetLumimeter().Pointer() : nullptr;
		if(lumimeter){
			lumimeter->SetPosition(editDVector->GetDVector());
		}
		
	}
};

class cEditLMDir : public igdeEditVectorListener{
	meWPSensors &pPanel;
	
public:
	using Ref = deTObjectReference<cEditLMDir>;
	cEditLMDir(meWPSensors &panel) : pPanel(panel){}
	
	void OnVectorChanged(igdeEditVector *editVector) override{
		meWorld * const world = pPanel.GetWindowProperties().GetWindowMain().GetWorld();
		meLumimeter * const lumimeter = world ? world->GetLumimeter().Pointer() : nullptr;
		if(lumimeter){
			lumimeter->SetDirection(editVector->GetVector());
		}
		
	}
};

class cTextLMConeIA : public igdeTextFieldListener{
	meWPSensors &pPanel;
	
public:
	using Ref = deTObjectReference<cTextLMConeIA>;
	cTextLMConeIA(meWPSensors &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		meWorld * const world = pPanel.GetWindowProperties().GetWindowMain().GetWorld();
		meLumimeter * const lumimeter = world ? world->GetLumimeter().Pointer() : nullptr;
		if(lumimeter){
			lumimeter->SetConeInnerAngle(textField->GetFloat());
		}
	}
};

class cTextLMConeOA : public igdeTextFieldListener{
	meWPSensors &pPanel;
	
public:
	using Ref = deTObjectReference<cTextLMConeOA>;
	cTextLMConeOA(meWPSensors &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		meWorld * const world = pPanel.GetWindowProperties().GetWindowMain().GetWorld();
		meLumimeter * const lumimeter = world ? world->GetLumimeter().Pointer() : nullptr;
		if(lumimeter){
			lumimeter->SetConeOuterAngle(textField->GetFloat());
		}
	}
};

class cTextLMConeExp : public igdeTextFieldListener{
	meWPSensors &pPanel;
	
public:
	using Ref = deTObjectReference<cTextLMConeExp>;
	cTextLMConeExp(meWPSensors &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		meWorld * const world = pPanel.GetWindowProperties().GetWindowMain().GetWorld();
		meLumimeter * const lumimeter = world ? world->GetLumimeter().Pointer() : nullptr;
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
	
// 	pListener = meWPViewListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	
	// lumimeter
	helper.GroupBox(content, groupBox, "@World.WPSensors.Lumimeter");
	
	helper.CheckBox(groupBox, pChkLMTrackCam, cActionLMTrackCam::Ref::New(*this));
	helper.EditDVector(groupBox, "@World.WPSensors.Label.Position", "@World.WPSensors.Position.ToolTip", pEditLMPos, cEditLMPos::Ref::New(*this));
	helper.EditVector(groupBox, "@World.WPSensors.Direction", "@World.WPSensors.Direction.ToolTip", pEditLMDir, cEditLMDir::Ref::New(*this));
	helper.EditFloat(groupBox, "@World.WPSensors.InnerAngle", "@World.WPSensors.ConeInnerAngle.ToolTip",
		pEditLMConeIA, cTextLMConeIA::Ref::New(*this));
	helper.EditFloat(groupBox, "@World.WPSensors.OuterAngle", "@World.WPSensors.ConeOuterAngle.ToolTip",
		pEditLMConeOA, cTextLMConeOA::Ref::New(*this));
	helper.EditFloat(groupBox, "@World.WPSensors.Exponent",
		"@World.WPSensors.ConeExponent.ToolTip",
		pEditLMConeExp, cTextLMConeExp::Ref::New(*this));
	
	helper.EditFloat(groupBox, "@World.WPSensors.MeasuredLuminance", "@World.WPSensors.MeasuredLuminance.ToolTip", pEditLMLumi, {});
	pEditLMLumi->SetEditable(false);
	helper.ColorBox(groupBox, "@World.WPSensors.MeasuredColor", "@World.WPSensors.MeasuredColor.ToolTip", pEditLMColor, {});
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
	meLumimeter * lumimeter = world ? world->GetLumimeter().Pointer() : nullptr;
	
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
