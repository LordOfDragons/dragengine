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

#include "deglDialogProfileListParameter.h"
#include "../deglWindowMain.h"
#include "../deglGuiBuilder.h"

#include <delauncher/engine/modules/parameter/delEMParameter.h>
#include <delauncher/game/profile/delGameProfile.h>
#include <delauncher/game/profile/delGPModule.h>
#include <delauncher/game/profile/delGPMParameter.h>
#include <delauncher/game/profile/delGPMParameterList.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>



// Class deglDialogProfileListParameter
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deglDialogProfileListParameter::deglDialogProfileListParameter(delEMParameter &parameter,
	delGameProfile &profile, const char *moduleName, FXMatrix *container,
	FXObject *target, int labelSelector, int valueSelector) :
pParameter(parameter),
pProfile(profile),
pModuleName(moduleName),
pLabel(nullptr),
pComboBox(nullptr),
pFrameSlider(nullptr),
pSlider(nullptr),
pSliderText(nullptr),
pCheckBox(nullptr),
pTextField(nullptr),
pCustomized(false)
{
	if(!container){
		DETHROW(deeInvalidParam);
	}
	
	// create description
	pDescription = parameter.GetInfo().GetDescription();
	
	if(parameter.GetInfo().GetType() == deModuleParameter::eptSelection){
		const int count = parameter.GetInfo().GetSelectionEntryCount();
		int i;
		for(i=0; i<count; i++){
			pDescription += "\n\n";
			pDescription += parameter.GetInfo().GetSelectionEntryAt(i).displayName;
			pDescription += ": ";
			pDescription += parameter.GetInfo().GetSelectionEntryAt(i).description;
		}
	}
	
	// add label
	const char * const name = !parameter.GetInfo().GetDisplayName().IsEmpty()
		? parameter.GetInfo().GetDisplayName() : parameter.GetInfo().GetName();
	
	pLabel = new FXLabel(container, name, nullptr, LAYOUT_FILL_X | LAYOUT_FILL_Y | LABEL_NORMAL | JUSTIFY_LEFT);
	pLabel->setTarget(target);
	pLabel->setSelector(labelSelector);
	pLabel->enable();
	pLabel->create();
	
	// add edit widget matching parameter type
	switch(parameter.GetInfo().GetType()){
	case deModuleParameter::eptBoolean:
		pCheckBox = new FXCheckButton(container, "", target, valueSelector,
			LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_FILL_COLUMN | CHECKBUTTON_NORMAL);
		pCheckBox->create();
		break;
		
	case deModuleParameter::eptRanged:{
		pFrameSlider = new FXHorizontalFrame(container, LAYOUT_FILL_X | LAYOUT_FILL_Y
			| LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0);
		pFrameSlider->create();
		
		pSlider = new FXRealSlider(pFrameSlider, target, valueSelector,
			LAYOUT_FILL_X | LAYOUT_FILL_Y | REALSLIDER_NORMAL | REALSLIDER_TICKS_BOTTOM);
		pSlider->setRange(parameter.GetInfo().GetMinimumValue(), parameter.GetInfo().GetMaximumValue());
		pSlider->setGranularity(parameter.GetInfo().GetValueStepSize());
		pSlider->setIncrement(parameter.GetInfo().GetValueStepSize());
		pSlider->setTickDelta(parameter.GetInfo().GetValueStepSize());
		pSlider->create();
		
		pSliderText = new FXTextField(pFrameSlider, 5, nullptr, 0, TEXTFIELD_NORMAL | TEXTFIELD_READONLY);
		pSliderText->create();
		}break;
		
	case deModuleParameter::eptNumeric:
		pTextField = new FXTextField(container, 10, target, valueSelector,
			LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_FILL_COLUMN | TEXTFIELD_NORMAL | TEXTFIELD_REAL);
		pTextField->create();
		break;
		
	case deModuleParameter::eptSelection:{
		pComboBox = new FXComboBox(container, 20, target, valueSelector,
			LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_FILL_COLUMN | COMBOBOX_NORMAL);
		pComboBox->setEditable(false);
		pComboBox->create();
		
		const int count = parameter.GetInfo().GetSelectionEntryCount();
		int i;
		for(i=0; i<count; i++){
			const deModuleParameter::SelectionEntry &entry = parameter.GetInfo().GetSelectionEntryAt(i);
			pComboBox->appendItem(entry.displayName.GetString(), (FXptr)entry.value.GetString());
		}
		pComboBox->setNumVisible(count < 10 ? count : 10);
		pComboBox->layout();
		}break;
	
	case deModuleParameter::eptString:
	default:
		pTextField = new FXTextField(container, 30, target, valueSelector,
			LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_FILL_COLUMN | TEXTFIELD_NORMAL);
		pTextField->create();
	}
	
	Update();
}

deglDialogProfileListParameter::~deglDialogProfileListParameter(){
	if(pLabel){
		delete pLabel;
	}
	if(pComboBox){
		delete pComboBox;
	}
	if(pFrameSlider){
		delete pFrameSlider;
	}
	if(pCheckBox){
		delete pCheckBox;
	}
	if(pTextField){
		delete pTextField;
	}
}



// Management
///////////////

bool deglDialogProfileListParameter::SenderMatchesLabel(FXObject *sender) const{
	return pLabel == sender;
}

bool deglDialogProfileListParameter::SenderMatchesEdit(FXObject *sender) const{
	return pComboBox == sender || pSlider == sender || pCheckBox == sender || pTextField == sender;
}

bool deglDialogProfileListParameter::ProcessSelCommand(FXObject *sender){
	if(pComboBox == sender){
		if(pComboBox->getCurrentItem() != -1){
			SetParameterValue((const char *)pComboBox->getItemData(pComboBox->getCurrentItem()));
		}
		return true;
		
	}else if(pSlider == sender){
		SetParameterValue(pSlider->getValue());
		return true;
		
	}else if(pCheckBox == sender){
		SetParameterValue(pCheckBox->getCheck() ? "1" : "0");
		return true;
		
	}else if(pTextField == sender){
		if(pParameter.GetInfo().GetType() == deModuleParameter::eptNumeric){
			FXbool success;
			const FXdouble value = pTextField->getText().toFloat(&success);
			if(success){
				SetParameterValue(value);
				
			}else{
				Update();
			}
			
		}else{
			SetParameterValue(pTextField->getText().text());
		}
		return true;
		
	}else{
		return false;
	}
}

bool deglDialogProfileListParameter::ProcessSelChanged(FXObject *sender){
	if(pComboBox == sender){
		return true;
		
	}else if(pSlider == sender){
		SetParameterValue(pSlider->getValue());
		return true;
		
	}else if(pCheckBox == sender){
		return true;
		
	}else if(pTextField == sender){
		return true;
		
	}else{
		return false;
	}
}

void deglDialogProfileListParameter::SetParameterValue(const char *value){
	delGPModule::Ref profileModule(pProfile.GetModules().GetNamed(pModuleName));
	if(!profileModule){
		if(pParameter.GetValue() == value){
			return;
		}
		profileModule.TakeOver(new delGPModule(pModuleName));
		pProfile.GetModules().Add(profileModule);
	}
	
	delGPMParameterList &mpParamList = profileModule->GetParameters();
	
	const decString &parameterName = pParameter.GetInfo().GetName();
	delGPMParameter::Ref profileParameter(mpParamList.GetNamed(parameterName));
	if(!profileParameter){
		if(pParameter.GetValue() == value){
			return;
		}
		profileParameter.TakeOver(new delGPMParameter(parameterName));
		mpParamList.Add(profileParameter);
	}
	
	profileParameter->SetValue(value);
	
	Update();
}

void deglDialogProfileListParameter::SetParameterValue(FXdouble value){
	decString string;
	string.Format("%f", value);
	
	int i;
	for(i=string.GetLength()-1; i>0; i--){
		if(string[i] == '.'){
			string[i] = 0;
			break;
			
		}else if(string[i] == '0'){
			string[i] = 0;
			
		}else{
			break;
		}
	}
	
	SetParameterValue(string);
}

void deglDialogProfileListParameter::Update(){
	// get value
	decString value(pParameter.GetValue());
	pCustomized = false;
	
	const delGPModule * const module = pProfile.GetModules().GetNamed(pModuleName);
	if(module){
		const delGPMParameter * const parameter = module->GetParameters()
			.GetNamed(pParameter.GetInfo().GetName());
		if(parameter){
			value = parameter->GetValue();
			pCustomized = true;
		}
	}
	
	// update widget with value
	switch(pParameter.GetInfo().GetType()){
	case deModuleParameter::eptBoolean:
		pCheckBox->setCheck(value == "1");
		break;
		
	case deModuleParameter::eptRanged:{
		pSlider->setValue(value.ToFloat());
		pSliderText->setText(value.GetString());
		}break;
		
	case deModuleParameter::eptNumeric:
		pTextField->setText(value.GetString());
		break;
		
	case deModuleParameter::eptSelection:{
		const int count = pComboBox->getNumItems();
		int i;
		for(i=0; i<count; i++){
			if(value == (const char *)pComboBox->getItemData(i)){
				pComboBox->setCurrentItem(i);
				pComboBox->setText(pComboBox->getItemText(i));
				break;
			}
		}
		}break;
		
	case deModuleParameter::eptString:
	default:
		pTextField->setText(value.GetString());
	}
	
	// label color to reflect if customized
	FXColor bgcolor = pLabel->getApp()->getBackColor();
	
	if(pCustomized){
		FXColor changedColor = FXRGB(255, 0, 0);
		const float blendFactor = 0.25f;
		
		bgcolor = FXRGB(
			(FXuchar)(FXREDVAL(changedColor) * blendFactor + FXREDVAL(bgcolor) * (1.0f - blendFactor)),
			(FXuchar)(FXGREENVAL(changedColor) * blendFactor + FXGREENVAL(bgcolor) * (1.0f - blendFactor)),
			(FXuchar)(FXBLUEVAL(changedColor) * blendFactor + FXBLUEVAL(bgcolor) * (1.0f - blendFactor))
);
	}
	
	pLabel->setBackColor(bgcolor);
}

void deglDialogProfileListParameter::Reset(){
	delGPMParameter *parameter = nullptr;
	delGPModule * const module = pProfile.GetModules().GetNamed(pModuleName);
	if(module){
		parameter = module->GetParameters().GetNamed(pParameter.GetInfo().GetName());
	}
	
	if(module){
		if(parameter){
			module->GetParameters().Remove(parameter);
		}
		
		if(module->GetParameters().GetCount() == 0){
			pProfile.GetModules().Remove(module);
		}
	}
	
	Update();
}

void deglDialogProfileListParameter::UpdateVisibility(deModuleParameter::eCategory category){
	const bool visible = pCustomized || pParameter.GetInfo().GetCategory() <= category;
	const FXSelector selector = FXSEL(SEL_COMMAND, visible ? FXWindow::ID_SHOW : FXWindow::ID_HIDE);
	
	if(pLabel){
		pLabel->tryHandle(pLabel, selector, 0);
	}
	if(pComboBox){
		pComboBox->tryHandle(pLabel, selector, 0);
	}
	if(pFrameSlider){
		pFrameSlider->tryHandle(pLabel, selector, 0);
	}
	if(pCheckBox){
		pCheckBox->tryHandle(pLabel, selector, 0);
	}
	if(pTextField){
		pTextField->tryHandle(pLabel, selector, 0);
	}
}
