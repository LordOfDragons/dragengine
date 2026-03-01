/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include <MenuItem.h>

#include "deglbDialogProfileListParameter.h"

#include <delauncher/engine/modules/parameter/delEMParameter.h>
#include <delauncher/game/profile/delGPModule.h>
#include <delauncher/game/profile/delGameProfile.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>


// Class deglbDialogProfileListParameter
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deglbDialogProfileListParameter::deglbDialogProfileListParameter(delEMParameter &parameter,
	delGameProfile &profile, const char *moduleName, uint32 msgWhat) :
pParameter(parameter),
pProfile(profile),
pModuleName(moduleName),
pDescription(parameter.GetInfo().GetDescription()),
pLabel(nullptr),
pMenuField(nullptr),
pPopUpMenu(nullptr),
pCheckBox(nullptr),
pSlider(nullptr),
pTextControl(nullptr),
pCustomized(false),
pMsgWhat(msgWhat)
{
	// build description including selection entry descriptions
	if(parameter.GetInfo().GetType() == deModuleParameter::eptSelection){
		parameter.GetInfo().GetSelectionEntries().Visit([&](const deModuleParameter::SelectionEntry &entry){
			pDescription += "\n\n";
			pDescription += entry.displayName;
			pDescription += ": ";
			pDescription += entry.description;
		});
	}
	
	// create label
	const char * const displayName = !parameter.GetInfo().GetDisplayName().IsEmpty()
		? parameter.GetInfo().GetDisplayName() : parameter.GetInfo().GetName();
	pLabel = new BStringView("label", displayName);
	
	// create appropriate edit widget
	switch(parameter.GetInfo().GetType()){
	case deModuleParameter::eptBoolean:{
		BMessage * const msg = new BMessage(msgWhat);
		msg->AddPointer("param", this);
		pCheckBox = new BCheckBox("value", "", msg);
		}break;
		
	case deModuleParameter::eptRanged:{
		const float minVal = parameter.GetInfo().GetMinimumValue();
		const float maxVal = parameter.GetInfo().GetMaximumValue();
		const float step = parameter.GetInfo().GetValueStepSize();
		const int minInt = (int)(minVal * 1000.0f);
		const int maxInt = (int)(maxVal * 1000.0f);
		const int stepInt = step > 0.0f ? (int)(step * 1000.0f) : 1;
		BMessage * const msg = new BMessage(msgWhat);
		msg->AddPointer("param", this);
		pSlider = new BSlider("value", "", msg, minInt, maxInt, B_HORIZONTAL);
		pSlider->SetKeyIncrementValue(stepInt);
		}break;
		
	case deModuleParameter::eptSelection:{
		pPopUpMenu = new BPopUpMenu("");
		parameter.GetInfo().GetSelectionEntries().Visit(
		[&](const deModuleParameter::SelectionEntry &entry){
			BMessage * const msg = new BMessage(msgWhat);
			msg->AddString("value", entry.value.GetString());
			msg->AddPointer("param", this);
			pPopUpMenu->AddItem(new BMenuItem(entry.displayName.GetString(), msg));
		});
		pMenuField = new BMenuField("value", "", pPopUpMenu);
		}break;
		
	case deModuleParameter::eptNumeric:
	case deModuleParameter::eptString:
	default:{
		BMessage * const msg = new BMessage(msgWhat);
		msg->AddPointer("param", this);
		pTextControl = new BTextControl("value", "", "", msg);
		}break;
	}
	
	Update();
}

deglbDialogProfileListParameter::~deglbDialogProfileListParameter(){
	// BWindow owns widgets once added as children; here we only delete if not yet parented
	// Callers must RemoveChild() before calling Reset()/destructor if added to a parent
}



// Management
///////////////

BView *deglbDialogProfileListParameter::GetEditWidget() const{
	if(pMenuField){
		return pMenuField;
	}
	if(pCheckBox){
		return pCheckBox;
	}
	if(pSlider){
		return pSlider;
	}
	return pTextControl;
}

void deglbDialogProfileListParameter::Update(){
	// get parameter value (profile override or default)
	decString value(pParameter.GetValue());
	pCustomized = false;
	
	const delGPModule * const module = pProfile.GetModules().FindNamed(pModuleName);
	if(module){
		const decString *foundValue;
		if(module->GetParameters().GetAt(pParameter.GetInfo().GetName(), foundValue)){
			value = *foundValue;
			pCustomized = true;
		}
	}
	
	// update widget
	switch(pParameter.GetInfo().GetType()){
	case deModuleParameter::eptBoolean:
		if(pCheckBox){
			pCheckBox->SetValue(value == "1" ? B_CONTROL_ON : B_CONTROL_OFF);
		}
		break;
		
	case deModuleParameter::eptRanged:
		if(pSlider){
			pSlider->SetValue((int)(value.ToFloat() * 1000.0f));
		}
		break;
		
	case deModuleParameter::eptSelection:
		if(pPopUpMenu){
			const int count = pPopUpMenu->CountItems();
			int i;
			for(i=0; i<count; i++){
				BMenuItem * const item = pPopUpMenu->ItemAt(i);
				BMessage * const msg = item ? item->Message() : nullptr;
				if(!msg){
					continue;
				}
				const char *v = nullptr;
				if(msg->FindString("value", &v) == B_OK && v && value == v){
					item->SetMarked(true);
					break;
				}
			}
			if(i == count && count > 0){
				pPopUpMenu->ItemAt(0)->SetMarked(true);
			}
		}
		break;
		
	case deModuleParameter::eptNumeric:
	case deModuleParameter::eptString:
	default:
		if(pTextControl){
			pTextControl->SetText(value.GetString());
		}
		break;
	}
	
	// update label color to indicate customization
	if(pLabel){
		rgb_color color = ui_color(B_PANEL_BACKGROUND_COLOR);
		if(pCustomized){
			color.red = (uint8)(0xff * 0.25f + color.red * 0.75f);
			color.green = (uint8)(color.green * 0.75f);
			color.blue = (uint8)(color.blue * 0.75f);
		}
		pLabel->SetViewColor(color);
		pLabel->Invalidate();
	}
}

void deglbDialogProfileListParameter::ApplyMenuSelection(){
	if(!pPopUpMenu){
		return;
	}
	BMenuItem * const item = pPopUpMenu->FindMarked();
	if(!item){
		return;
	}
	const char *v = nullptr;
	if(item->Message() && item->Message()->FindString("value", &v) == B_OK && v){
		pSetParameterValue(v);
	}
}

void deglbDialogProfileListParameter::ApplyCheckBox(){
	if(pCheckBox){
		pSetParameterValue(pCheckBox->Value() == B_CONTROL_ON ? "1" : "0");
	}
}

void deglbDialogProfileListParameter::ApplySlider(){
	if(pSlider){
		pSetParameterValue((float)pSlider->Value() / 1000.0f);
	}
}

void deglbDialogProfileListParameter::ApplyTextControl(){
	if(pTextControl){
		if(pParameter.GetInfo().GetType() == deModuleParameter::eptNumeric){
			pSetParameterValue(decString(pTextControl->Text()).ToFloat());
		}else{
			pSetParameterValue(pTextControl->Text());
		}
	}
}

void deglbDialogProfileListParameter::Reset(){
	delGPModule * const module = pProfile.GetModules().FindNamed(pModuleName);
	if(module){
		module->GetParameters().RemoveIfPresent(pParameter.GetInfo().GetName());
		if(module->GetParameters().IsEmpty()){
			pProfile.GetModules().Remove(module);
		}
	}
	Update();
}

void deglbDialogProfileListParameter::UpdateVisibility(deModuleParameter::eCategory category){
	const bool visible = pCustomized || pParameter.GetInfo().GetCategory() <= category;
	
	if(pLabel){
		if(visible){
			pLabel->Show();
		}else{
			pLabel->Hide();
		}
	}
	
	BView * const edit = GetEditWidget();
	if(edit){
		if(visible){
			edit->Show();
		}else{
			edit->Hide();
		}
	}
}



// Private Functions
//////////////////////

void deglbDialogProfileListParameter::pSetParameterValue(const char *value){
	delGPModule::Ref profileModule(pProfile.GetModules().FindNamed(pModuleName));
	if(!profileModule){
		if(pParameter.GetValue() == value){
			return;
		}
		profileModule = delGPModule::Ref::New(pModuleName);
		pProfile.GetModules().Add(profileModule);
	}
	profileModule->GetParameters().SetAt(pParameter.GetInfo().GetName(), value);
	Update();
}

void deglbDialogProfileListParameter::pSetParameterValue(float value){
	decString string;
	string.Format("%f", value);
	
	int i;
	for(i=string.GetLength()-1; i>0; i--){
		if(string[i] == '.'){
			string = string.GetLeft(i);
			break;
		}else if(string[i] != '0'){
			string = string.GetLeft(i + 1);
			break;
		}
	}
	
	pSetParameterValue(string);
}
