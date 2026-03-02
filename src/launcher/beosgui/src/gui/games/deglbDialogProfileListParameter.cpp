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
#include <LayoutBuilder.h>
#include <MessageFilter.h>

#include "deglbDialogProfileListParameter.h"

#include <delauncher/engine/modules/parameter/delEMParameter.h>
#include <delauncher/game/profile/delGPModule.h>
#include <delauncher/game/profile/delGameProfile.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>


namespace{

class cMessageFilterMPParameter : public BMessageFilter{
	deglbDialogProfileListParameter &pParameter;
	
public:
	cMessageFilterMPParameter(deglbDialogProfileListParameter &parameter) :
	BMessageFilter(B_MOUSE_DOWN),
	pParameter(parameter){ 
	}
	
	filter_result Filter(BMessage *message, BHandler **target) override{
		switch(message->what){
		case B_MOUSE_DOWN:{
			int32 buttons = 0, clicks = 0;
			BPoint where;
			
			message->FindInt32("buttons", &buttons);
			message->FindInt32("clicks", &clicks);
			message->FindPoint("be:view_where", &where);
			
			auto view = dynamic_cast<BView*>(*target);
			
			if(buttons == B_SECONDARY_MOUSE_BUTTON && clicks == 1){
				view->ConvertToScreen(&where);
				pParameter.OnLabelContextMenu(where);
				return B_SKIP_MESSAGE;
				
			}else if(buttons == B_PRIMARY_MOUSE_BUTTON){
				pParameter.OnLabelClicked();
				return B_SKIP_MESSAGE;
			}
			}break;
			
		default:
			break;
		};
		
		return B_DISPATCH_MESSAGE;
	}
};

}


// Class deglbDialogProfileListParameter
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deglbDialogProfileListParameter::deglbDialogProfileListParameter(delEMParameter &parameter,
	delGameProfile &profile, const char *moduleName, uint32 msgWhat, BMessenger target) :
pParameter(parameter),
pProfile(profile),
pModuleName(moduleName),
pDescription(parameter.GetInfo().GetDescription()),
pLabel(nullptr),
pMenuField(nullptr),
pPopUpMenu(nullptr),
pViewCheckBox(nullptr),
pCheckBox(nullptr),
pSlider(nullptr),
pViewSlider(nullptr),
pTextControl(nullptr),
pCustomized(false),
pMsgWhat(msgWhat),
pTarget(target)
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
	pLabel->SetExplicitAlignment({B_ALIGN_USE_FULL_WIDTH, B_ALIGN_VERTICAL_UNSET});
	pLabel->AddFilter(new cMessageFilterMPParameter(*this));
	
	// create appropriate edit widget
	switch(parameter.GetInfo().GetType()){
	case deModuleParameter::eptBoolean:{
		BMessage * const msg = new BMessage(msgWhat);
		msg->AddPointer("param", this);
		msg->AddString("action", "valueChanged");
		pCheckBox = new BCheckBox("value", nullptr, msg);
		
		pViewCheckBox = new BGroupView();
		BLayoutBuilder::Group<>(pViewCheckBox, B_HORIZONTAL, B_USE_SMALL_SPACING)
			.SetInsets(0)
			.AddGlue()
			.Add(pCheckBox, 0.0f)
			.AddGlue()
		.End();
		}break;
		
	case deModuleParameter::eptRanged:{
		const float minVal = parameter.GetInfo().GetMinimumValue();
		const float maxVal = parameter.GetInfo().GetMaximumValue();
		const float step = parameter.GetInfo().GetValueStepSize();
		const int minInt = (int)(minVal * 1000.0f);
		const int maxInt = (int)(maxVal * 1000.0f);
		const int stepInt = step > 0.0f ? (int)(step * 1000.0f) : 1;
		BMessage *msg = new BMessage(msgWhat);
		msg->AddPointer("param", this);
		msg->AddString("action", "valueChanged");
		msg->AddInt16("selector", 0);
		pSlider = new BSlider("value", nullptr, msg, minInt, maxInt, B_HORIZONTAL);
		pSlider->SetKeyIncrementValue(stepInt);
		pSlider->SetHashMarks(B_HASH_MARKS_BOTH);
		pSlider->SetHashMarkCount((int)ceilf((maxVal - minVal) / decMath::max(step, 0.001f)));
		
		msg = new BMessage(msgWhat);
		msg->AddPointer("param", this);
		msg->AddString("action", "valueChanged");
		msg->AddInt16("selector", 0);
		pSlider->SetModificationMessage(msg);
		
		msg = new BMessage(msgWhat);
		msg->AddPointer("param", this);
		msg->AddString("action", "valueChanged");
		msg->AddInt16("selector", 1);
		pSliderInput = new BTextControl("valueInput", nullptr, "", msg);
		pSliderInput->SetExplicitPreferredSize({pSliderInput->StringWidth("M") * 4, B_SIZE_UNSET});
		
		pViewSlider = new BGroupView();
		BLayoutBuilder::Group<>(pViewSlider, B_HORIZONTAL, B_USE_SMALL_SPACING)
			.SetInsets(0)
			.Add(pSlider, 1.0f)
			.Add(pSliderInput, 0.0f)
		.End();
		}break;
		
	case deModuleParameter::eptSelection:{
		pPopUpMenu = new BPopUpMenu("");
		parameter.GetInfo().GetSelectionEntries().Visit(
		[&](const deModuleParameter::SelectionEntry &entry){
			BMessage * const msg = new BMessage(msgWhat);
			msg->AddString("value", entry.value.GetString());
			msg->AddPointer("param", this);
			msg->AddString("action", "valueChanged");
			pPopUpMenu->AddItem(new BMenuItem(entry.displayName.GetString(), msg));
		});
		pMenuField = new BMenuField("value", "", pPopUpMenu);
		}break;
		
	case deModuleParameter::eptNumeric:
	case deModuleParameter::eptString:
	default:{
		BMessage * const msg = new BMessage(msgWhat);
		msg->AddPointer("param", this);
		msg->AddString("action", "valueChanged");
		pTextControl = new BTextControl("value", nullptr, "", msg);
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
	if(pViewCheckBox){
		return pViewCheckBox;
	}
	if(pViewSlider){
		return pViewSlider;
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
			pSliderInput->SetText(value);
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

void deglbDialogProfileListParameter::Apply(int selector){
	ApplyMenuSelection();
	ApplyCheckBox();
	ApplySlider(selector);
	ApplyTextControl();
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

void deglbDialogProfileListParameter::ApplySlider(int selector){
	if(!pSlider){
		return;
	}
	
	const float minVal = pParameter.GetInfo().GetMinimumValue();
	const float maxVal = pParameter.GetInfo().GetMaximumValue();
	const float step = pParameter.GetInfo().GetValueStepSize();
	
	float value = minVal;
	if(selector == 0){
		value = (float)pSlider->Value() / 1000.0f;
		
	}else if(selector == 1){
		value = decString(pSliderInput->Text()).ToFloat();
	}
	
	value = floorf((value / step) + 0.5f) * step;
	value = decMath::clamp(value, minVal, maxVal);
	
	pSetParameterValue(value);
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

void deglbDialogProfileListParameter::OnLabelContextMenu(const BPoint &where){
	BPopUpMenu *menu = new BPopUpMenu("context", false, false);
	
	auto msg = new BMessage(pMsgWhat);
	msg->AddPointer("param", this);
	msg->AddString("action", "resetValue");
	BMenuItem *item = new BMenuItem("Reset Value", msg);
	item->SetTarget(pTarget);
	menu->AddItem(item);
	
	menu->Go(where, true, false, true);
	delete menu;
}

void deglbDialogProfileListParameter::OnLabelClicked(){
	auto msg = new BMessage(pMsgWhat);
	msg->AddPointer("param", this);
	msg->AddString("action", "updateInfoText");
	pTarget.SendMessage(msg);
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
