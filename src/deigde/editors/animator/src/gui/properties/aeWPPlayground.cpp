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

#include "aeWPPlayground.h"
#include "aeWPPlaygroundListener.h"
#include "aeWindowProperties.h"
#include "../aeWindowMain.h"
#include "../../animator/aeAnimator.h"
#include "../../animator/locomotion/aeAnimatorLocomotion.h"
#include "../../animator/locomotion/aeAnimatorLocomotionLeg.h"
#include "../../animator/controller/aeController.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeSpinTextField.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditSliderText.h>
#include <deigde/gui/composed/igdeEditSliderTextListener.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeSpinTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/model/igdeListItem.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animator/deAnimator.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	aeWPPlayground &pPanel;
	
public:
	typedef deTObjectReference<cBaseAction> Ref;
	cBaseAction(aeWPPlayground &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		if(animator){
			OnAction(*animator);
		}
	}
	
	virtual void OnAction(aeAnimator &animator) = 0;
	
	void Update() override{
		aeAnimator * const animator = pPanel.GetAnimator();
		if(animator){
			Update(*animator);
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const aeAnimator &){
		SetEnabled(true);
		SetSelected(false);
	}
};

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	aeWPPlayground &pPanel;
	
public:
	typedef deTObjectReference<cBaseTextFieldListener> Ref;
	cBaseTextFieldListener(aeWPPlayground &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		aeAnimator * const animator = pPanel.GetAnimator();
		if(animator){
			OnChanged(*textField, *animator);
		}
	}
	
	virtual void OnChanged(igdeTextField &textField, aeAnimator &animator) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	aeWPPlayground &pPanel;
	
public:
	typedef deTObjectReference<cBaseEditVectorListener> Ref;
	cBaseEditVectorListener(aeWPPlayground &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		aeAnimator * const animator = pPanel.GetAnimator();
		if(animator){
			OnChanged(*editVector, *animator);
		}
	}
	
	virtual void OnChanged(igdeEditVector &editVector, aeAnimator &animator) = 0;
};


class cComboLocomotionType : public igdeComboBoxListener{
	aeWPPlayground &pPanel;
public:
	typedef deTObjectReference<cComboLocomotionType> Ref;
	cComboLocomotionType(aeWPPlayground &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		aeAnimator * const animator = pPanel.GetAnimator();
		if(!animator || !comboBox->GetSelectedItem()){
			return;
		}
		
		animator->GetLocomotion().SetLocomotionType((aeAnimatorLocomotion::eLocomotionTypes)
			(intptr_t)comboBox->GetSelectedItem()->GetData());
	}
};

class cTextLocoLimitUp : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextLocoLimitUp> Ref;
	cTextLocoLimitUp(aeWPPlayground &panel) : cBaseTextFieldListener(panel){}
	
	virtual void OnChanged(igdeTextField &textField, aeAnimator &animator){
		animator.GetLocomotion().SetLimitLookUp(textField.GetFloat());
	}
};

class cTextLocoLimitDown : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextLocoLimitDown> Ref;
	cTextLocoLimitDown(aeWPPlayground &panel) : cBaseTextFieldListener(panel){}
	
	virtual void OnChanged(igdeTextField &textField, aeAnimator &animator){
		animator.GetLocomotion().SetLimitLookDown(textField.GetFloat());
	}
};

class cTextLocoLimitLeft : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextLocoLimitLeft> Ref;
	cTextLocoLimitLeft(aeWPPlayground &panel) : cBaseTextFieldListener(panel){}
	
	virtual void OnChanged(igdeTextField &textField, aeAnimator &animator){
		animator.GetLocomotion().SetLimitLookLeft(textField.GetFloat());
	}
};

class cTextLocoLimitRight : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextLocoLimitRight> Ref;
	cTextLocoLimitRight(aeWPPlayground &panel) : cBaseTextFieldListener(panel){}
	
	virtual void OnChanged(igdeTextField &textField, aeAnimator &animator){
		animator.GetLocomotion().SetLimitLookRight(textField.GetFloat());
	}
};

class cTextLocoSpeedWalk : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextLocoSpeedWalk> Ref;
	cTextLocoSpeedWalk(aeWPPlayground &panel) : cBaseTextFieldListener(panel){}
	
	virtual void OnChanged(igdeTextField &textField, aeAnimator &animator){
		animator.GetLocomotion().SetWalkSpeed(textField.GetFloat());
	}
};

class cTextLocoSpeedRun : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextLocoSpeedRun> Ref;
	cTextLocoSpeedRun(aeWPPlayground &panel) : cBaseTextFieldListener(panel){}
	
	virtual void OnChanged(igdeTextField &textField, aeAnimator &animator){
		animator.GetLocomotion().SetRunSpeed(textField.GetFloat());
	}
};

class cTextLocoAdjTimeUD : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextLocoAdjTimeUD> Ref;
	cTextLocoAdjTimeUD(aeWPPlayground &panel) : cBaseTextFieldListener(panel){}
	
	virtual void OnChanged(igdeTextField &textField, aeAnimator &animator){
		const float value = textField.GetFloat();
		if(fabs(animator.GetLocomotion().GetLookUpDown().GetAdjustTime() - value) < FLOAT_SAFE_EPSILON){
			return;
		}
		
		animator.GetLocomotion().GetLookUpDown().SetAdjustTime(value);
		animator.NotifyLocomotionChanged();
		animator.SetChanged(true);
	}
};

class cTextLocoAdjTimeLR : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextLocoAdjTimeLR> Ref;
	cTextLocoAdjTimeLR(aeWPPlayground &panel) : cBaseTextFieldListener(panel){}
	
	virtual void OnChanged(igdeTextField &textField, aeAnimator &animator){
		const float value = textField.GetFloat();
		if(fabs(animator.GetLocomotion().GetLookLeftRight().GetAdjustTime() - value) < FLOAT_SAFE_EPSILON){
			return;
		}
		
		animator.GetLocomotion().GetLookLeftRight().SetAdjustTime(value);
		animator.NotifyLocomotionChanged();
		animator.SetChanged(true);
	}
};

class cTextLocoAdjTimeStance : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextLocoAdjTimeStance> Ref;
	cTextLocoAdjTimeStance(aeWPPlayground &panel) : cBaseTextFieldListener(panel){}
	
	virtual void OnChanged(igdeTextField &textField, aeAnimator &animator){
		const float value = textField.GetFloat();
		if(fabs(animator.GetLocomotion().GetStance().GetAdjustTime() - value) < FLOAT_SAFE_EPSILON){
			return;
		}
		
		animator.GetLocomotion().GetStance().SetAdjustTime(value);
		animator.NotifyLocomotionChanged();
		animator.SetChanged(true);
	}
};

class cTextLocoAdjTimeOrientation : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextLocoAdjTimeOrientation> Ref;
	cTextLocoAdjTimeOrientation(aeWPPlayground &panel) : cBaseTextFieldListener(panel){}
	
	virtual void OnChanged(igdeTextField &textField, aeAnimator &animator){
		const float value = textField.GetFloat();
		if(fabs(animator.GetLocomotion().GetOrientation().GetAdjustTime() - value) < FLOAT_SAFE_EPSILON){
			return;
		}
		
		animator.GetLocomotion().GetOrientation().SetAdjustTime(value);
		animator.NotifyLocomotionChanged();
		animator.SetChanged(true);
	}
};

class cTextLocoAdjTimeVelocity : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextLocoAdjTimeVelocity> Ref;
	cTextLocoAdjTimeVelocity(aeWPPlayground &panel) : cBaseTextFieldListener(panel){}
	
	virtual void OnChanged(igdeTextField &textField, aeAnimator &animator){
		const float value = textField.GetFloat();
		if(fabs(animator.GetLocomotion().GetLinearVelocity().GetAdjustTime() - value) < FLOAT_SAFE_EPSILON){
			return;
		}
		
		animator.GetLocomotion().GetLinearVelocity().SetAdjustTime(value);
		animator.NotifyLocomotionChanged();
		animator.SetChanged(true);
	}
};

class cTextLocoAdjTimeTurnIP : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextLocoAdjTimeTurnIP> Ref;
	cTextLocoAdjTimeTurnIP(aeWPPlayground &panel) : cBaseTextFieldListener(panel){}
	
	virtual void OnChanged(igdeTextField &textField, aeAnimator &animator){
		animator.GetLocomotion().SetAdjustTimeTurnIP(textField.GetFloat());
	}
};

class cTextLocoLegBlendTime : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextLocoLegBlendTime> Ref;
	cTextLocoLegBlendTime(aeWPPlayground &panel) : cBaseTextFieldListener(panel){}
	
	virtual void OnChanged(igdeTextField &textField, aeAnimator &animator){
		animator.GetLocomotion().SetLegBlendTime(textField.GetFloat());
	}
};

class cSpinLocoUseLegPairs : public igdeSpinTextFieldListener{
	aeWPPlayground &pPanel;
public:
	typedef deTObjectReference<cSpinLocoUseLegPairs> Ref;
	cSpinLocoUseLegPairs(aeWPPlayground &panel) : pPanel(panel){}
	
	virtual void OnValueChanged(igdeSpinTextField *textField){
		aeAnimator * const animator = pPanel.GetAnimator();
		if(!animator){
			return;
		}
		
		animator->GetLocomotion().SetUseLegPairCount(textField->GetValue());
	}
};

class cSpinLocoLeg : public igdeSpinTextFieldListener{
	aeWPPlayground &pPanel;
public:
	typedef deTObjectReference<cSpinLocoLeg> Ref;
	cSpinLocoLeg(aeWPPlayground &panel) : pPanel(panel){}
	
	virtual void OnValueChanged(igdeSpinTextField*){
		if(pPanel.GetAnimator()){
			pPanel.UpdateLocomotionLeg();
		}
	}
};

class cTextLocoLegLiftOffTime : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextLocoLegLiftOffTime> Ref;
	cTextLocoLegLiftOffTime(aeWPPlayground &panel) : cBaseTextFieldListener(panel){}
	
	virtual void OnChanged(igdeTextField &textField, aeAnimator &animator){
		aeAnimatorLocomotionLeg * const leg = pPanel.GetLeg();
		if(!leg){
			return;
		}
		
		leg->SetLiftOffTime(textField.GetFloat());
	}
};

class cTextLocoLegPutDownTime : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextLocoLegPutDownTime> Ref;
	cTextLocoLegPutDownTime(aeWPPlayground &panel) : cBaseTextFieldListener(panel){}
	
	virtual void OnChanged(igdeTextField &textField, aeAnimator &animator){
		aeAnimatorLocomotionLeg * const leg = pPanel.GetLeg();
		if(!leg){
			return;
		}
		
		leg->SetPutDownTime(textField.GetFloat());
	}
};

class cEditLocoLegPDPosStand : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditLocoLegPDPosStand> Ref;
	cEditLocoLegPDPosStand(aeWPPlayground &panel) : cBaseEditVectorListener(panel){}
	
	virtual void OnChanged(igdeEditVector &editVector, aeAnimator &animator){
		aeAnimatorLocomotionLeg * const leg = pPanel.GetLeg();
		if(!leg){
			return;
		}
		
		leg->SetPutDownPositionStand(editVector.GetVector());
	}
};

class cEditLocoLegPDPosWalk : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditLocoLegPDPosWalk> Ref;
	cEditLocoLegPDPosWalk(aeWPPlayground &panel) : cBaseEditVectorListener(panel){}
	
	virtual void OnChanged(igdeEditVector &editVector, aeAnimator &animator){
		aeAnimatorLocomotionLeg * const leg = pPanel.GetLeg();
		if(!leg){
			return;
		}
		
		leg->SetPutDownPositionWalk(editVector.GetVector());
	}
};

class cEditLocoLegPDPosRun : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditLocoLegPDPosRun> Ref;
	cEditLocoLegPDPosRun(aeWPPlayground &panel) : cBaseEditVectorListener(panel){}
	
	virtual void OnChanged(igdeEditVector &editVector, aeAnimator &animator){
		aeAnimatorLocomotionLeg * const leg = pPanel.GetLeg();
		if(!leg){
			return;
		}
		
		leg->SetPutDownPositionRun(editVector.GetVector());
	}
};

class cActionLocoShowShapes : public cBaseAction{
public:
	typedef deTObjectReference<cActionLocoShowShapes> Ref;
	
public:
	cActionLocoShowShapes(aeWPPlayground &panel) : cBaseAction(panel,
		"Show Shapes", nullptr, "Determines if shapes are shown"){ }
	
	void OnAction(aeAnimator &animator) override{
		animator.GetLocomotion().SetShowShapes(!animator.GetLocomotion().GetShowShapes());
	}
	
	void Update(const aeAnimator &animator) override{
		SetEnabled(true);
		SetSelected(animator.GetLocomotion().GetShowShapes());
	}
};

class cActionLocoUseFoGIK : public cBaseAction{
public:
	typedef deTObjectReference<cActionLocoUseFoGIK> Ref;
	
public:
	cActionLocoUseFoGIK(aeWPPlayground &panel) : cBaseAction(panel,
		"Use Feet-On-Ground IK", nullptr, "Determines the feet-on-ground IK is used"){}
	
	void OnAction(aeAnimator &animator) override{
		animator.GetLocomotion().SetUseFoGIK(!animator.GetLocomotion().GetUseFoGIK());
	}
	
	void Update(const aeAnimator &animator) override{
		SetEnabled(true);
		SetSelected(animator.GetLocomotion().GetUseFoGIK());
	}
};


class cSliderController : public igdeEditSliderTextListener{
	aeWPPlayground &pPanel;
	int pIndex;
	
public:
	typedef deTObjectReference<cSliderController> Ref;
	cSliderController(aeWPPlayground &panel, int index) : pPanel(panel), pIndex(index){}
	
	virtual void OnSliderTextValueChanged(igdeEditSliderText *sliderText){
		aeAnimator * const animator = pPanel.GetAnimator();
		if(animator){
			OnChanged(sliderText->GetValue(), *animator);
		}
	}
	
	virtual void OnSliderTextValueChanging(igdeEditSliderText *sliderText){
		aeAnimator * const animator = pPanel.GetAnimator();
		if(animator){
			OnChanged(sliderText->GetValue(), *animator);
		}
	}
	
	void OnChanged(float value, aeAnimator&){
		pPanel.GetControllerAt(pIndex).controller->SetCurrentValue(value);
	}
};

}



// Class aeWPPlayground
/////////////////////////

// Constructor, destructor
////////////////////////////

aeWPPlayground::aeWPPlayground(aeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties),
pControllers(nullptr),
pControllerCount(0)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, formLine;
	
	pListener = aeWPPlaygroundListener::Ref::New(*this);
	
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	
	// controllers
	helper.GroupBox(content, pFraContent, "Controllers:");
	
	
	// locomotion testing
	helper.GroupBox(content, groupBox, "Locomotion Testing:", true);
	
	helper.ComboBox(groupBox, "Locomotion Type:", "Locomotion type",
		pCBLocomotionType, cComboLocomotionType::Ref::New(*this));
	pCBLocomotionType->AddItem("Natural", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eltNatural);
	pCBLocomotionType->AddItem("FPS", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eltFPS);
	pCBLocomotionType->AddItem("Vehicle", nullptr, (void*)(intptr_t)aeAnimatorLocomotion::eltVehicle);
	
	helper.EditFloat(groupBox, "Look Up Limit:", "Look up limit in euler angles",
		pEditLocoLimitUp, cTextLocoLimitUp::Ref::New(*this));
	helper.EditFloat(groupBox, "Look Down Limit:", "Look down limit in euler angles",
		pEditLocoLimitDown, cTextLocoLimitDown::Ref::New(*this));
	helper.EditFloat(groupBox, "Look Left Limit:", "Look left limit in euler angles",
		pEditLocoLimitLeft, cTextLocoLimitLeft::Ref::New(*this));
	helper.EditFloat(groupBox, "Look Right Limit:", "Look right limit in euler angles",
		pEditLocoLimitRight, cTextLocoLimitRight::Ref::New(*this));
	
	
	// movement speeds
	helper.GroupBox(content, groupBox, "Movement Speeds:", true);
	
	helper.EditFloat(groupBox, "Walk:", "Walk speed in meters per second",
		pEditLocoSpeedWalk, cTextLocoSpeedWalk::Ref::New(*this));
	helper.EditFloat(groupBox, "Run:", "Run speed in meters per second",
		pEditLocoSpeedRun, cTextLocoSpeedRun::Ref::New(*this));
	
	
	// adjustment times
	helper.GroupBox(content, groupBox, "Adjustment Times:", true);
	
	helper.EditFloat(groupBox, "Look Up-Down:", "Adjustment time for looking up and down",
		pEditLocoAdjTimeUD, cTextLocoAdjTimeUD::Ref::New(*this));
	helper.EditFloat(groupBox, "Look Left-Right:", "Adjustment time for looking left and right",
		pEditLocoAdjTimeLR, cTextLocoAdjTimeLR::Ref::New(*this));
	helper.EditFloat(groupBox, "Stance:", "Adjustment time for changing between uprect and crouch stance",
		pEditLocoAdjTimeStance, cTextLocoAdjTimeStance::Ref::New(*this));
	helper.EditFloat(groupBox, "Orientation:", "Adjustment time in seconds for actor turning",
		pEditLocoAdjTimeOrientation, cTextLocoAdjTimeOrientation::Ref::New(*this));
	helper.EditFloat(groupBox, "Velocity:", "Adjustment time in seconds for velocity changing",
		pEditLocoAdjTimeVelocity, cTextLocoAdjTimeVelocity::Ref::New(*this));
	helper.EditFloat(groupBox, "Turn In-Place:", "Adjustment time in seconds for turning in-place",
		pEditLocoAdjTimeTurnIP, cTextLocoAdjTimeTurnIP::Ref::New(*this));
	
	
	// legs
	helper.GroupBox(content, groupBox, "Legs:", true);
	
	helper.EditFloat(groupBox, "Blend Time:", "Time in seconds to blend at lift off and put down time",
		pEditLocoLegBlendTime, cTextLocoLegBlendTime::Ref::New(*this));
	
	helper.EditSpinInteger(groupBox, "Leg Pairs:", "Number of leg pairs to use", 1, 1,
		pSpinLocoUseLegPairs, cSpinLocoUseLegPairs::Ref::New(*this));
	helper.EditSpinInteger(groupBox, "Leg:", "Leg to set parameters for", 0, 0,
		pSpinLocoLeg, cSpinLocoLeg::Ref::New(*this));
	
	helper.EditFloat(groupBox, "Lift Off Time:", "Time in motion units where the foot starts to lift off the ground",
		pEditLocoLegLiftOffTime, cTextLocoLegLiftOffTime::Ref::New(*this));
	helper.EditFloat(groupBox, "Put Down Time:", "Time in motion units where the foot stopps putting down on the ground",
		pEditLocoLegPutDownTime, cTextLocoLegPutDownTime::Ref::New(*this));
	
	helper.EditVector(groupBox, "Put Down Stand:", "Sets the stand put down position",
		pEditLocoLegPDPosStand, cEditLocoLegPDPosStand::Ref::New(*this));
	helper.EditVector(groupBox, "Put Down Walk:", "Sets the walk put down position",
		pEditLocoLegPDPosWalk, cEditLocoLegPDPosWalk::Ref::New(*this));
	helper.EditVector(groupBox, "Put Down Run:", "Sets the run put down position",
		pEditLocoLegPDPosRun, cEditLocoLegPDPosRun::Ref::New(*this));
	
	
	// visualization
	helper.GroupBox(content, groupBox, "Visualization:", true);
	
	helper.CheckBox(groupBox, pChkLocoShowShapes, cActionLocoShowShapes::Ref::New(*this));
	helper.CheckBox(groupBox, pChkLocoUseFoGIK, cActionLocoUseFoGIK::Ref::New(*this));
}

aeWPPlayground::~aeWPPlayground(){
	if(pControllers) delete [] pControllers;
	
	if(pAnimator){
		pAnimator->RemoveNotifier(pListener);
		pAnimator = nullptr;
	}
}



// Management
///////////////

void aeWPPlayground::SetAnimator(aeAnimator *animator){
	if(animator == pAnimator){
		return;
	}
	
	if(pAnimator){
		pAnimator->RemoveNotifier(pListener);
	}
	
	pAnimator = animator;
	
	if(animator){
		animator->AddNotifier(pListener);
		pSpinLocoUseLegPairs->SetRange(1, animator->GetLocomotion().GetLegCount() / 2);
		pSpinLocoLeg->SetRange(0, animator->GetLocomotion().GetLegCount() - 1);
		
	}else{
		pSpinLocoUseLegPairs->SetRange(1, 1);
		pSpinLocoLeg->SetRange(0, 0);
	}
	
	RebuildControllers();
	UpdateLocomotion();
}

aeAnimatorLocomotionLeg *aeWPPlayground::GetLeg() const{
	if(!pAnimator){
		return nullptr;
	}
	
	const int legnum = pSpinLocoLeg->GetValue();
	if(legnum < 0 || legnum >= pAnimator->GetLocomotion().GetLegCount()){
		return nullptr;
	}
	
	return pAnimator->GetLocomotion().GetLegAt(legnum);
}

aeWPPlayground::sController &aeWPPlayground::GetControllerAt(int index) const{
	if(index < 0 || index >= pControllerCount){
		DETHROW(deeInvalidParam);
	}
	return pControllers[index];
}


void aeWPPlayground::RebuildControllers(){
	// remove all widgets from controller container
	pFraContent->RemoveAllChildren();
	
	// create array holding controller widgets ( even if not all are used in the end )
	if(pControllers){
		delete [] pControllers;
		pControllers = nullptr;
	}
	pControllerCount = 0;
	
	int controllerCount = 0;
	if(pAnimator){
		controllerCount = pAnimator->GetControllers().GetCount();
		if(controllerCount > 0){
			pControllers = new sController[controllerCount];
		}
	}
	
	// create widgets
	igdeUIHelper &helper = GetEnvironment().GetUIHelperProperties();
	int i;
	
	for(i=0; i<controllerCount; i++){
		sController &controller = pControllers[pControllerCount];
		
		aeController * const animatorController = pAnimator->GetControllers().GetAt(i);
		
		controller.controller = animatorController;
		
		const decString &name = animatorController->GetName();
		decString text, description;
		text.Format("%s:", name.GetString());
		description.Format("Current value of the controller '%s'", name.GetString());
		
		igdeEditSliderText::Ref slider;
		helper.EditSliderText(pFraContent, text, description, 0.0f, 1.0f, 6, 3, 0.1f,
			slider, cSliderController::Ref::New(*this, pControllerCount));
		controller.slider = slider;
		
		pControllerCount++;
		
		UpdateController(animatorController);
		UpdateControllerValue(animatorController);
	}
}

void aeWPPlayground::UpdateController(aeController *controller){
	int i;
	for(i=0; i<pControllerCount; i++){
		if(pControllers[i].controller != controller){
			continue;
		}
		
		const float minimum = controller->GetMinimumValue();
		const float maximum = controller->GetMaximumValue();
		pControllers[i].slider->SetRange(minimum, maximum);
		pControllers[i].slider->SetTickSpacing((maximum - minimum) * 0.1f);
		return;
	}
}

void aeWPPlayground::UpdateControllerValue(aeController *controller){
	int i;
	for(i=0; i<pControllerCount; i++){
		if(pControllers[i].controller != controller){
			continue;
		}
		
		pControllers[i].slider->SetValue(controller->GetCurrentValue());
		break;
	}
}

void aeWPPlayground::UpdateLocomotion(){
	if(pAnimator){
		const aeAnimatorLocomotion &locomotion = pAnimator->GetLocomotion();
		
		pCBLocomotionType->SetSelectionWithData((void*)(intptr_t)locomotion.GetLocomotionType());
		pEditLocoLimitDown->SetFloat(locomotion.GetLimitLookDown());
		pEditLocoLimitUp->SetFloat(locomotion.GetLimitLookUp());
		pEditLocoLimitLeft->SetFloat(locomotion.GetLimitLookLeft());
		pEditLocoLimitRight->SetFloat(locomotion.GetLimitLookRight());
		pEditLocoSpeedWalk->SetFloat(locomotion.GetWalkSpeed());
		pEditLocoSpeedRun->SetFloat(locomotion.GetRunSpeed());
		pEditLocoAdjTimeUD->SetFloat(locomotion.GetLookUpDown().GetAdjustTime());
		pEditLocoAdjTimeLR->SetFloat(locomotion.GetLookLeftRight().GetAdjustTime());
		pEditLocoAdjTimeStance->SetFloat(locomotion.GetStance().GetAdjustTime());
		pEditLocoAdjTimeOrientation->SetFloat(locomotion.GetOrientation().GetAdjustTime());
		pEditLocoAdjTimeVelocity->SetFloat(locomotion.GetLinearVelocity().GetAdjustTime());
		pEditLocoAdjTimeTurnIP->SetFloat(locomotion.GetAdjustTimeTurnIP());
		
	}else{
		pCBLocomotionType->SetSelectionWithData((void*)(intptr_t)aeAnimatorLocomotion::eltNatural);
		pEditLocoLimitDown->ClearText();
		pEditLocoLimitUp->ClearText();
		pEditLocoLimitLeft->ClearText();
		pEditLocoLimitRight->ClearText();
		pEditLocoSpeedWalk->ClearText();
		pEditLocoSpeedRun->ClearText();
		pEditLocoAdjTimeUD->ClearText();
		pEditLocoAdjTimeLR->ClearText();
		pEditLocoAdjTimeStance->ClearText();
		pEditLocoAdjTimeOrientation->ClearText();
		pEditLocoAdjTimeVelocity->ClearText();
		pEditLocoAdjTimeTurnIP->ClearText();
	}
	
	const bool enabled = pAnimator;
	pCBLocomotionType->SetEnabled(enabled);
	pEditLocoLimitDown->SetEnabled(enabled);
	pEditLocoLimitUp->SetEnabled(enabled);
	pEditLocoLimitLeft->SetEnabled(enabled);
	pEditLocoLimitRight->SetEnabled(enabled);
	pEditLocoSpeedWalk->SetEnabled(enabled);
	pEditLocoSpeedRun->SetEnabled(enabled);
	pEditLocoAdjTimeUD->SetEnabled(enabled);
	pEditLocoAdjTimeLR->SetEnabled(enabled);
	pEditLocoAdjTimeStance->SetEnabled(enabled);
	pEditLocoAdjTimeOrientation->SetEnabled(enabled);
	pEditLocoAdjTimeVelocity->SetEnabled(enabled);
	pEditLocoAdjTimeTurnIP->SetEnabled(enabled);
	
	pChkLocoShowShapes->GetAction()->Update();
	pChkLocoUseFoGIK->GetAction()->Update();
	
	UpdateLocomotionLeg();
}

void aeWPPlayground::UpdateLocomotionLeg(){
	const aeAnimatorLocomotionLeg * const leg = GetLeg();
	
	if(pAnimator){
		pEditLocoLegBlendTime->SetFloat(pAnimator->GetLocomotion().GetLegBlendTime());
		
	}else{
		pEditLocoLegBlendTime->ClearText();
	}
	
	pEditLocoLegBlendTime->SetEnabled(pAnimator);
	
	if(leg){
		pEditLocoLegLiftOffTime->SetFloat(leg->GetLiftOffTime());
		pEditLocoLegPutDownTime->SetFloat(leg->GetPutDownTime());
		pEditLocoLegPDPosStand->SetVector(leg->GetPutDownPositionStand());
		pEditLocoLegPDPosWalk->SetVector(leg->GetPutDownPositionWalk());
		pEditLocoLegPDPosRun->SetVector(leg->GetPutDownPositionRun());
		
	}else{
		pEditLocoLegLiftOffTime->ClearText();
		pEditLocoLegPutDownTime->ClearText();
		pEditLocoLegPDPosStand->SetVector(decVector());
		pEditLocoLegPDPosWalk->SetVector(decVector());
		pEditLocoLegPDPosRun->SetVector(decVector());
	}
}
