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

#include <stdint.h>

#include "igdeDEParameters.h"
#include "igdeDialogEngine.h"
#include "../igdeCommonDialogs.h"
#include "../igdeUIHelper.h"
#include "../igdeComboBox.h"
#include "../igdeButton.h"
#include "../igdeTextArea.h"
#include "../igdeTextField.h"
#include "../igdeContainer.h"
#include "../layout/igdeContainerForm.h"
#include "../event/igdeAction.h"
#include "../event/igdeComboBoxListener.h"
#include "../model/igdeListItem.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/collection/decGlobalFunctions.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deBaseModule.h>
#include <dragengine/systems/modules/deLoadableModule.h>



// Actions
////////////

class igdeDEParameters_ComboModule : public igdeComboBoxListener{
	igdeDEParameters &pPanel;
	
public:
	typedef deTObjectReference<igdeDEParameters_ComboModule> Ref;
	
	igdeDEParameters_ComboModule(igdeDEParameters &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox*){
		pPanel.UpdateParametersList();
		pPanel.UpdateParameter();
	}
};

class igdeDEParameters_ComboParameter : public igdeComboBoxListener{
	igdeDEParameters &pPanel;
	
public:
	typedef deTObjectReference<igdeDEParameters_ComboParameter> Ref;
	
	igdeDEParameters_ComboParameter(igdeDEParameters &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox*){
		pPanel.UpdateParameter();
	}
};

class igdeDEParameters_ActionSet : public igdeAction{
	igdeDEParameters &pPanel;
	
public:
	typedef deTObjectReference<igdeDEParameters_ActionSet> Ref;
	
	igdeDEParameters_ActionSet(igdeDEParameters &panel) :
		igdeAction("Set", nullptr, "Set parameter value"), pPanel(panel){}
	
	void OnAction() override{
		pPanel.ParameterSetValue();
	}
	
	void Update() override{
		SetEnabled(!pPanel.GetSelectedParameter().IsEmpty());
	}
};

class igdeDEParameters_ActionReset : public igdeAction{
	igdeDEParameters &pPanel;
	
public:
	typedef deTObjectReference<igdeDEParameters_ActionReset> Ref;
	
	igdeDEParameters_ActionReset(igdeDEParameters &panel) :
		igdeAction("Reset", nullptr, "Reset parameter value"), pPanel(panel){}
	
	void OnAction() override{
		pPanel.ParameterResetValue();
	}
	
	void Update() override{
		SetEnabled(!pPanel.GetSelectedParameter().IsEmpty());
	}
};



// class igdeDEParameters
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeDEParameters::igdeDEParameters(igdeDialogEngine &dialogEngine) :
igdeContainerFlow(dialogEngine.GetEnvironment(), igdeContainerFlow::eaY),
pDialogEngine(dialogEngine)
{
	igdeEnvironment &env = dialogEngine.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	igdeContainer::Ref groupBox, line;
	
	
	line = igdeContainerForm::Ref::New(env);
	AddChild(line);
	
	helper.ComboBox(line, "Module:", "Module to show parameters for",
		pCBModule, igdeDEParameters_ComboModule::Ref::New(*this));
	pCBModule->SetDefaultSorter();
	
	helper.ComboBox(line, "Parameter:", "Parameter to show",
		pCBParameter, igdeDEParameters_ComboParameter::Ref::New(*this));
	pCBParameter->SetDefaultSorter();
	
	
	// parameter information
	helper.GroupBoxStatic(*this, groupBox, "Parameter:");
	
	helper.EditString(groupBox, "Description:", "Parameter description", pEditDescription, 3, {});
	pEditDescription->SetEditable(false);
	
	helper.EditString(groupBox, "Type:", "Parameter type", pEditType, {});
	pEditType->SetEditable(false);
	
	helper.EditString(groupBox, "Allowed Values:", "Allowed Values", pEditAllowedValues, 5, {});
	pEditAllowedValues->SetEditable(false);
	
	helper.FormLineStretchFirst(groupBox, "Value:", "Parameter value", line);
	helper.EditString(line, "Parameter value", pEditValue, {});
	helper.Button(line, pBtnSet, igdeDEParameters_ActionSet::Ref::New(*this));
	helper.Button(line, pBtnReset, igdeDEParameters_ActionReset::Ref::New(*this));
	
	
	// load the lists
	UpdateModulesList();
	UpdateParametersList();
	UpdateParameter();
}

igdeDEParameters::~igdeDEParameters(){
}



// Management
///////////////

void igdeDEParameters::UpdateParameter(){
	deLoadableModule * const loadableModule = GetSelectedModule();
	if(!loadableModule || !loadableModule->GetModule() || !pCBParameter->GetSelectedItem()){
		pEditDescription->SetText("?");
		pEditType->SetText("?");
		pEditAllowedValues->SetText("?");
		pEditValue->SetText("?");
		
		pBtnSet->GetAction()->Update();
		pBtnReset->GetAction()->Update();
		return;
	}
	
	deBaseModule &module = *loadableModule->GetModule();
	const decString &name = GetSelectedParameter();
	decString text;
	
	module.GetParameterInfo(module.IndexOfParameterNamed(name), pParameterInfo);
	
	pEditDescription->SetText(pParameterInfo.GetDescription());
	
	switch(pParameterInfo.GetType()){
	case deModuleParameter::eptBoolean:
		pEditType->SetText("Boolean");
		pEditAllowedValues->SetText("Boolean value: 1 or 0");
		break;
		
	case deModuleParameter::eptNumeric:
		pEditType->SetText("Numeric");
		pEditAllowedValues->SetText("Numeric value.");
		break;
		
	case deModuleParameter::eptRanged:
		pEditType->SetText("Ranged");
		text.Format("Ranged value from %g to %g step size %g",
			pParameterInfo.GetMinimumValue(), pParameterInfo.GetMaximumValue(),
			pParameterInfo.GetValueStepSize());
		pEditAllowedValues->SetText(text);
		break;
		
	case deModuleParameter::eptSelection:{
		pEditType->SetText("Selection");
		text = "A value from this list:\n";
		const int count = pParameterInfo.GetSelectionEntryCount();
		decStringList list;
		int i;
		for(i=0; i<count; i++){
			list.Add(pParameterInfo.GetSelectionEntryAt(i).value);
		}
		pEditAllowedValues->SetText(text + DEJoin(list, ", "));
		}break;
		
	case deModuleParameter::eptString:
		pEditType->SetText("String");
		pEditAllowedValues->SetText("String value.");
		break;
		
	default:
		pEditType->SetText("Unknown Type");
		pEditAllowedValues->SetText("Unknown");
	}
	
	pEditValue->SetText(module.GetParameterValue(name));
	
	pBtnSet->GetAction()->Update();
	pBtnReset->GetAction()->Update();
}

void igdeDEParameters::UpdateParametersList(){
	deLoadableModule * const loadableModule = GetSelectedModule();
	
	pCBParameter->RemoveAllItems();
	
	if(loadableModule && loadableModule->GetModule()){
		deBaseModule &module = *loadableModule->GetModule();
		const int count = module.GetParameterCount();
		int i;
		for(i=0; i<count; i++){
			module.GetParameterInfo(i, pParameterInfo);
			pCBParameter->AddItem(pParameterInfo.GetName());
		}
		pCBParameter->SortItems();
	}
}

void igdeDEParameters::UpdateModulesList(){
	const deModuleSystem &moduleSystem = *GetEngine()->GetModuleSystem();
	const int count = moduleSystem.GetModuleCount();
	int i;
	
	pCBModule->RemoveAllItems();
	for(i=0; i<count; i++){
		deLoadableModule * const loadableModule = moduleSystem.GetModuleAt(i);
		if(!pCBModule->HasItem(loadableModule->GetName())){
			pCBModule->AddItem(loadableModule->GetName());
		}
	}
	pCBModule->SortItems();
}

deLoadableModule *igdeDEParameters::GetSelectedModule() const{
	return pCBModule->GetSelectedItem() ? GetEngine()->GetModuleSystem()->
		GetModuleNamed(pCBModule->GetSelectedItem()->GetText()) : nullptr;
}

const decString &igdeDEParameters::GetSelectedParameter() const{
	static decString empty;
	return GetSelectedModule() && pCBParameter->GetSelectedItem() ? pCBParameter->GetSelectedItem()->GetText() : empty;
}

void igdeDEParameters::ParameterSetValue(){
	deLoadableModule * const loadableModule = GetSelectedModule();
	const decString &name = GetSelectedParameter();
	if(!loadableModule || !loadableModule->GetModule() || name.IsEmpty()){
		return;
	}
	
	try{
		loadableModule->GetModule()->SetParameterValue(name, pEditValue->GetText());
		
	}catch(const deException &e){
		igdeCommonDialogs::Exception(this, e);
	}
	
	pEditValue->SetText(loadableModule->GetModule()->GetParameterValue(name));
}

void igdeDEParameters::ParameterResetValue(){
	deLoadableModule * const loadableModule = GetSelectedModule();
	const decString &name = GetSelectedParameter();
	if(!loadableModule || !loadableModule->GetModule() || name.IsEmpty()){
		return;
	}
	
	deBaseModule &module = *loadableModule->GetModule();
	module.GetParameterInfo(module.IndexOfParameterNamed(name), pParameterInfo);
	
	pEditValue->SetText(pParameterInfo.GetDefaultValue());
}
