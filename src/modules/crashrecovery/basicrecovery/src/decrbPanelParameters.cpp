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

// includes
#include <stdint.h>
#include "decrbPanelParameters.h"
#include "decrbWindowMain.h"
#include <dragengine/deEngine.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deBaseModule.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/systems/modules/deModuleParameter.h>
#include <dragengine/common/exceptions.h>



// Events
///////////
FXDEFMAP(decrbPanelParameters) decrbPanelParametersMap[] = {
	FXMAPFUNC(SEL_COMMAND, decrbPanelParameters::ID_CBMODULE, decrbPanelParameters::onCBModuleChanged),
	FXMAPFUNC(SEL_COMMAND, decrbPanelParameters::ID_CBPARAMETER, decrbPanelParameters::onCBParameterChanged),
	FXMAPFUNC(SEL_COMMAND, decrbPanelParameters::ID_BTNSET, decrbPanelParameters::onBtnSetCommand),
	FXMAPFUNC(SEL_COMMAND, decrbPanelParameters::ID_BTNRESET, decrbPanelParameters::onBtnResetCommand),
};



// Sorting
////////////

static FXint fSortClasses(const FXListItem *item1, const FXListItem *item2){
	return strcmp(item1->getText().text(), item2->getText().text());
}



// Class decrbPanelParameters
////////////////////////////
	
FXIMPLEMENT(decrbPanelParameters, FXVerticalFrame, decrbPanelParametersMap, ARRAYNUMBER(decrbPanelParametersMap))

// Constructor, destructor
////////////////////////////

decrbPanelParameters::decrbPanelParameters(){}

decrbPanelParameters::decrbPanelParameters(decrbWindowMain *windowMain, FXComposite *container) :
FXVerticalFrame(container, LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_TOP | LAYOUT_LEFT, 5, 5, 5, 5){
	if(!windowMain) DETHROW(deeInvalidParam);
	int padding = 3;
	int spacing = 3;
	
	// prepare
	pWndMain = windowMain;
	pParameterInfo = new deModuleParameter;
	
	// module selection
	FXHorizontalFrame *frameLine = new FXHorizontalFrame(this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing);
	new FXLabel(frameLine, "Module:");
	pCBModule = new FXComboBox(frameLine, 10, this, ID_CBMODULE, FRAME_SUNKEN | LAYOUT_FILL_X);
	pCBModule->setEditable(false);
	pCBModule->setNumVisible(10);
	pCBModule->setSortFunc(fSortClasses);
	
	// parameter selection
	frameLine = new FXHorizontalFrame(this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing);
	new FXLabel(frameLine, "Parameter:");
	pCBParameter = new FXComboBox(frameLine, 10, this, ID_CBPARAMETER, FRAME_SUNKEN | LAYOUT_FILL_X);
	pCBParameter->setEditable(false);
	pCBParameter->setNumVisible(10);
	pCBParameter->setSortFunc(fSortClasses);
	
	// parameter information
	FXGroupBox *groupBox = new FXGroupBox(this, "Parameter:",
		GROUPBOX_TITLE_LEFT | FRAME_RIDGE | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0,
		padding, padding, padding, padding);
	FXVerticalFrame *frameBox = new FXVerticalFrame(groupBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing);
	
	frameLine = new FXHorizontalFrame(frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing);
	new FXLabel(frameLine, "Description:");
	pEditDescription = new FXText(frameLine, NULL, 0, FRAME_SUNKEN | TEXT_READONLY
		| TEXT_WORDWRAP | LAYOUT_FILL_X | LAYOUT_FILL_Y);
	
	frameLine = new FXHorizontalFrame(frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing);
	new FXLabel(frameLine, "Type:");
	pEditType = new FXTextField(frameLine, 10, NULL, 0, FRAME_SUNKEN | TEXTFIELD_READONLY
		| LAYOUT_FILL_X);
	
	frameLine = new FXHorizontalFrame(frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing);
	new FXLabel(frameLine, "Allowed Values:");
	pEditAllowedValues = new FXText(frameLine, NULL, 0, FRAME_SUNKEN | TEXT_READONLY
		| TEXT_WORDWRAP | LAYOUT_FILL_X | LAYOUT_FILL_Y);
	
	frameLine = new FXHorizontalFrame(frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing);
	new FXLabel(frameLine, "Current Value:");
	pEditValue = new FXTextField(frameLine, 10, NULL, 0, FRAME_SUNKEN);
	
	frameLine = new FXHorizontalFrame(frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing);
	pBtnSet = new FXButton(frameLine, "Set Parameter", NULL, this, ID_BTNSET, BUTTON_NORMAL);
	pBtnReset = new FXButton(frameLine, "Reset Parameter", NULL, this, ID_BTNRESET, BUTTON_NORMAL);
	
	// load the lists
	UpdateModulesList();
	UpdateParametersList();
	UpdateParameter();
}

decrbPanelParameters::~decrbPanelParameters(){
	if(pParameterInfo) delete pParameterInfo;
}



// Management
///////////////

void decrbPanelParameters::UpdateParameter(){
	int selection = pCBModule->getCurrentItem();
	deBaseModule *module = NULL;
	int i, count;
	FXString text;
	
	if(selection != -1){
		module = pWndMain->GetEngine()->GetModuleSystem()->
			GetModuleNamed(pCBModule->getItemText(selection).text())->GetModule();
		selection = pCBParameter->getCurrentItem();
	}
	
	if(selection == -1){
		pEditDescription->setText("?");
		pEditType->setText("?");
		pEditAllowedValues->setText("?");
		pEditValue->setText("?");
		
		pBtnSet->disable();
		pBtnReset->disable();
		
	}else{
		const decString name(pCBParameter->getText().text());
		module->GetParameterInfo(module->IndexOfParameterNamed(name), *pParameterInfo);
		
		pEditDescription->setText(pParameterInfo->GetDescription().GetString());
		
		switch(pParameterInfo->GetType()){
		case deModuleParameter::eptBoolean:
			pEditType->setText("Boolean");
			pEditAllowedValues->setText("Boolean value: 1 or 0");
			break;
			
		case deModuleParameter::eptNumeric:
			pEditType->setText("Numeric");
			pEditAllowedValues->setText("Numeric value.");
			break;
			
		case deModuleParameter::eptRanged:
			pEditType->setText("Ranged");
			text.format("Ranged value from %g to %g step size %g",
				pParameterInfo->GetMinimumValue(), pParameterInfo->GetMaximumValue(),
				pParameterInfo->GetValueStepSize());
			pEditAllowedValues->setText(text);
			break;
			
		case deModuleParameter::eptSelection:{
			pEditType->setText("Selection");
			text = "A value from this list:\n";
			count = pParameterInfo->GetSelectionEntryCount();
			for(i=0; i<count; i++){
				const deModuleParameter::SelectionEntry &entry = pParameterInfo->GetSelectionEntryAt(i);
				text.append(entry.value);
				if(i < count - 1) text.append(", ");
			}
			pEditAllowedValues->setText(text);
			}break;
			
		case deModuleParameter::eptString:
			pEditType->setText("String");
			pEditAllowedValues->setText("String value.");
			break;
			
		default:
			pEditType->setText("Unknown Type");
			pEditAllowedValues->setText("Unknown");
		}
		
		pEditValue->setText(module->GetParameterValue(name).GetString());
		
		pBtnSet->enable();
		pBtnReset->enable();
	}
}

void decrbPanelParameters::UpdateParametersList(){
	int selection = pCBModule->getCurrentItem();
	deLoadableModule *loadableModule;
	deBaseModule *module = NULL;
	int i, count;
	
	if(selection != -1){
		loadableModule = pWndMain->GetEngine()->GetModuleSystem()->
			GetModuleNamed(pCBModule->getItemText(selection).text());
		module = loadableModule->GetModule();
	}
	
	pCBParameter->clearItems();
	if(module){
		count = module->GetParameterCount();
		for(i=0; i<count; i++){
			module->GetParameterInfo(i, *pParameterInfo);
			pCBParameter->appendItem(pParameterInfo->GetName().GetString());
		}
		pCBParameter->sortItems();
	}
}

void decrbPanelParameters::UpdateModulesList(){
	deEngine *engine = pWndMain->GetEngine();
	deModuleSystem *modSys = engine->GetModuleSystem();
	int i, count = modSys->GetModuleCount();
	deLoadableModule *loadableModule;
	
	pCBModule->clearItems();
	for(i=0; i<count; i++){
		loadableModule = modSys->GetModuleAt(i);
		if(pCBModule->findItem(loadableModule->GetName().GetString()) == -1){
			pCBModule->appendItem(loadableModule->GetName().GetString());
		}
	}
	pCBModule->sortItems();
}



// Events
///////////

long decrbPanelParameters::onCBModuleChanged(FXObject* , FXSelector, void*){
	UpdateParametersList();
	UpdateParameter();
	return 1;
}

long decrbPanelParameters::onCBParameterChanged(FXObject* , FXSelector, void*){
	UpdateParameter();
	return 1;
}

long decrbPanelParameters::onBtnSetCommand(FXObject* , FXSelector, void*){
	int selection = pCBModule->getCurrentItem();
	deBaseModule *module = NULL;
	FXString text;
	
	if(selection != -1){
		module = pWndMain->GetEngine()->GetModuleSystem()->
			GetModuleNamed(pCBModule->getItemText(selection).text())->GetModule();
		selection = pCBParameter->getCurrentItem();
	}
	
	if(selection != -1){
		const decString name(pCBParameter->getText().text());
		
		try{
			module->SetParameterValue(name, pEditValue->getText().text());
		}catch(const deException &e){
			FXMessageBox::error(this, FX::MBOX_OK, "Set Parameter failed", "%s",
				e.FormatOutput().Join("\n").GetString());
		}
		
		pEditValue->setText(module->GetParameterValue(name).GetString());
	}
	
	return 1;
}

long decrbPanelParameters::onBtnResetCommand(FXObject* , FXSelector, void*){
	int selection = pCBModule->getCurrentItem();
	deBaseModule *module = NULL;
	
	if(selection != -1){
		module = pWndMain->GetEngine()->GetModuleSystem()->
			GetModuleNamed(pCBModule->getItemText(selection).text())->GetModule();
		selection = pCBParameter->getCurrentItem();
	}
	
	if(selection != -1){
		const decString name(pCBParameter->getText().text());
		pEditValue->setText(module->GetParameterValue(name).GetString());
	}
	
	return 1;
}



// Private Functions
//////////////////////
