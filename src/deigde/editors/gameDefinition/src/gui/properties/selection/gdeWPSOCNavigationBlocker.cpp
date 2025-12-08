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

#include "gdeWPSOCNavigationBlocker.h"
#include "gdeWPSOCNavigationBlockerListener.h"
#include "../gdeWindowProperties.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/category/gdeCategory.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/navblocker/gdeOCNavigationBlocker.h"
#include "../../../gamedef/property/gdeProperty.h"
#include "../../../undosys/objectClass/navblocker/gdeUOCNavBlockerSetShapeList.h"
#include "../../../undosys/objectClass/navblocker/gdeUOCNavBlockerSetBlockingPriority.h"
#include "../../../undosys/objectClass/navblocker/gdeUOCNavBlockerSetScaling.h"
#include "../../../undosys/objectClass/navblocker/gdeUOCNavBlockerSetBoneName.h"
#include "../../../undosys/objectClass/navblocker/gdeUOCNavBlockerSetLayer.h"
#include "../../../undosys/objectClass/navblocker/gdeUOCNavBlockerSetPosition.h"
#include "../../../undosys/objectClass/navblocker/gdeUOCNavBlockerSetRotation.h"
#include "../../../undosys/objectClass/navblocker/gdeUOCNavBlockerToggleEnabled.h"
#include "../../../undosys/objectClass/navblocker/gdeUOCNavBlockerSetPropertyName.h"
#include "../../../undosys/objectClass/navblocker/gdeUOCNavBlockerSetType.h"

#include <deigde/codec/igdeCodecPropertyString.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeWindow.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/logger/deLogger.h>



// Actions
////////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	gdeWPSOCNavigationBlocker &pPanel;
	
public:
	cBaseTextFieldListener(gdeWPSOCNavigationBlocker &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		gdeOCNavigationBlocker * const navblocker = pPanel.GetNavigationBlocker();
		if(!navblocker){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(
			 OnChanged(*textField, pPanel.GetObjectClass(), navblocker)));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField,
		gdeObjectClass *objectClass, gdeOCNavigationBlocker *navblocker) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	gdeWPSOCNavigationBlocker &pPanel;
	
public:
	cBaseEditVectorListener(gdeWPSOCNavigationBlocker &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		gdeOCNavigationBlocker * const navblocker = pPanel.GetNavigationBlocker();
		if(!navblocker){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(
			 OnChanged(editVector->GetVector(), pPanel.GetObjectClass(), navblocker)));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(const decVector &vector, gdeObjectClass *objectClass,
		gdeOCNavigationBlocker *navblocker) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	gdeWPSOCNavigationBlocker &pPanel;
	
public:
	cBaseComboBoxListener(gdeWPSOCNavigationBlocker &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		gdeOCNavigationBlocker * const navblocker = pPanel.GetNavigationBlocker();
		if(!navblocker){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(
			 OnChanged(*comboBox, pPanel.GetObjectClass(), navblocker)));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeComboBox &comboBox,
		gdeObjectClass *objectClass, gdeOCNavigationBlocker *navblocker) = 0;
};

class cBaseAction : public igdeAction{
protected:
	gdeWPSOCNavigationBlocker &pPanel;
	
public:
	cBaseAction(gdeWPSOCNavigationBlocker &panel, const char *text, const char *description) :
		igdeAction(text, description), pPanel(panel){}
	
	virtual void OnAction(){
		gdeOCNavigationBlocker * const navblocker = pPanel.GetNavigationBlocker();
		if(!navblocker){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(
			 OnActionNavBlocker(pPanel.GetObjectClass(), navblocker)));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnActionNavBlocker(gdeObjectClass *objectClass, gdeOCNavigationBlocker *navblocker) = 0;
};


class cEditPosition : public cBaseEditVectorListener {
public:
	cEditPosition(gdeWPSOCNavigationBlocker &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo *OnChanged(const decVector &vector, gdeObjectClass *objectClass,
	gdeOCNavigationBlocker *navblocker){
		if(navblocker->GetPosition().IsEqualTo(vector)){
			return NULL;
		}
		return new gdeUOCNavBlockerSetPosition(objectClass, navblocker, vector);
	}
};

class cEditRotation : public cBaseEditVectorListener {
public:
	cEditRotation(gdeWPSOCNavigationBlocker &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo *OnChanged(const decVector &vector, gdeObjectClass *objectClass,
	gdeOCNavigationBlocker *navblocker){
		if(navblocker->GetRotation().IsEqualTo(vector)){
			return NULL;
		}
		return new gdeUOCNavBlockerSetRotation(objectClass, navblocker, vector);
	}
};

class cEditScaling : public cBaseEditVectorListener {
public:
	cEditScaling(gdeWPSOCNavigationBlocker &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo *OnChanged(const decVector &vector, gdeObjectClass *objectClass,
	gdeOCNavigationBlocker *navblocker){
		if(navblocker->GetScaling().IsEqualTo(vector)){
			return NULL;
		}
		return new gdeUOCNavBlockerSetScaling(objectClass, navblocker, vector);
	}
};

class cTextBoneName : public cBaseTextFieldListener{
public:
	cTextBoneName(gdeWPSOCNavigationBlocker &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCNavigationBlocker *navBlocker){
		if(navBlocker->GetBoneName() == textField.GetText()){
			return NULL;
		}
		return new gdeUOCNavBlockerSetBoneName(objectClass, navBlocker, textField.GetText());
	}
};

class cActionEnabled : public cBaseAction{
public:
	cActionEnabled(gdeWPSOCNavigationBlocker &panel) :
	cBaseAction(panel, "Enabled", "Navigation blocker is enabled"){}
	
	virtual igdeUndo *OnActionNavBlocker(gdeObjectClass *objectClass, gdeOCNavigationBlocker *navblocker){
		return new gdeUOCNavBlockerToggleEnabled(objectClass, navblocker);
	}
};

class cTextLayer : public cBaseTextFieldListener{
public:
	cTextLayer(gdeWPSOCNavigationBlocker &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCNavigationBlocker *navblocker){
		const int value = textField.GetInteger();
		if(navblocker->GetLayer() == value){
			return NULL;
		}
		return new gdeUOCNavBlockerSetLayer(objectClass, navblocker, value);
	}
};

class cComboType : public cBaseComboBoxListener{
public:
	cComboType(gdeWPSOCNavigationBlocker &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeComboBox &comboBox, gdeObjectClass *objectClass,
	gdeOCNavigationBlocker *navblocker){
		const deNavigationSpace::eSpaceTypes value =
			(deNavigationSpace::eSpaceTypes)(intptr_t)comboBox.GetSelectedItem()->GetData();
		if(value == navblocker->GetType()){
			return NULL;
		}
		return new gdeUOCNavBlockerSetType(objectClass, navblocker, value);
	}
};

class cTextBlockingPriority : public cBaseTextFieldListener{
public:
	cTextBlockingPriority(gdeWPSOCNavigationBlocker &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCNavigationBlocker *navblocker){
		const int value = textField.GetInteger();
		if(navblocker->GetBlockingPriority() == value){
			return NULL;
		}
		return new gdeUOCNavBlockerSetBlockingPriority(objectClass, navblocker, value);
	}
};

class cTextShape : public cBaseTextFieldListener{
public:
	cTextShape(gdeWPSOCNavigationBlocker &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCNavigationBlocker *navblocker){
		igdeCodecPropertyString codec;
		decString oldEncoded;
		codec.EncodeShapeList(navblocker->GetShapeList(), oldEncoded);
		const decString encoded(textField.GetText());
		if(encoded == oldEncoded){
			return NULL;
		}
		
		decShapeList shapeList;
		try{
			codec.DecodeShapeList(encoded, shapeList);
			
		}catch(const deException &){
			igdeCommonDialogs::Error(pPanel.GetParentWindow(), "Invalid Input",
				"Input value does not decode to a proper shape list");
			textField.Focus();
			return NULL;
		}
		
		return new gdeUOCNavBlockerSetShapeList(objectClass, navblocker, shapeList);
	}
};


class cComboPropertyNames : public igdeComboBoxListener{
	gdeWPSOCNavigationBlocker &pPanel;
	
public:
	cComboPropertyNames(gdeWPSOCNavigationBlocker &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox*){
		if(pPanel.GetNavigationBlocker()){
			pPanel.UpdatePropertyName();
		}
	}
};

class cComboPropertyNameTarget : public igdeComboBoxListener{
	gdeWPSOCNavigationBlocker &pPanel;
	
public:
	cComboPropertyNameTarget(gdeWPSOCNavigationBlocker &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		if(!pPanel.GetNavigationBlocker()){
			return;
		}
		
		const gdeOCNavigationBlocker::eProperties propertyName = pPanel.GetPropertyName();
		if(pPanel.GetNavigationBlocker()->GetPropertyName(propertyName) == comboBox->GetText()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUOCNavBlockerSetPropertyName::Ref::NewWith(pPanel.GetObjectClass(),
				pPanel.GetNavigationBlocker(), propertyName, comboBox->GetText()));
	}
};

}



// Class gdeWPSOCNavigationBlocker
////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSOCNavigationBlocker::gdeWPSOCNavigationBlocker(gdeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, frameLine;
	
	content.TakeOver(new igdeContainerFlow(env, igdeContainerFlow::eaY));
	AddChild(content);
	
	pListener.TakeOver(new gdeWPSOCNavigationBlockerListener(*this));
	
	helper.GroupBox(content, groupBox, "Object Class Navigation Blocker:");
	
	helper.EditVector(groupBox, "Position:", "Position relative to object class",
		pEditPosition, new cEditPosition(*this));
	helper.EditVector(groupBox, "Rotation:", "Rotation in degrees relative to object class", 4, 1,
		pEditRotation, new cEditRotation(*this));
	helper.EditVector(groupBox, "Scaling:", "Scaling relative to object class",
		pEditScaling, new cEditScaling(*this));
	helper.EditString(groupBox, "Bone:", "Bone name or empty string if not used",
		pEditBoneName, new cTextBoneName(*this));
	helper.CheckBox(groupBox, pChkEnabled, new cActionEnabled(*this), true);
	helper.EditInteger(groupBox, "Layer:", "Navigation layer the blocker affects",
		pEditLayer, new cTextLayer(*this));
	
	helper.ComboBox(groupBox, "Type:", "Navigation space type the blocker affects",
		pCBType, new cComboType(*this));
	pCBType->AddItem("Grid", NULL, (void*)(intptr_t)deNavigationSpace::estGrid);
	pCBType->AddItem("Mesh", NULL, (void*)(intptr_t)deNavigationSpace::estMesh);
	pCBType->AddItem("Volume", NULL, (void*)(intptr_t)deNavigationSpace::estVolume);
	
	helper.EditInteger(groupBox, "Blocking priority:",
		"Blocks navigation spaces with the same or lower priority",
		pEditBlockingPriority, new cTextBlockingPriority(*this));
	helper.EditString(groupBox, "Shape:", "Shape of navigation blocker",
		pEditShape, new cTextShape(*this));
	
	// property targets
	helper.GroupBox(content, groupBox, "Properties:");
	helper.ComboBox(groupBox, "Property:", "Property to set target for",
		pCBPropertyNames, new cComboPropertyNames(*this));
	pCBPropertyNames->AddItem("Enabled", NULL,
		(void*)(intptr_t)gdeOCNavigationBlocker::epEnabled);
	pCBPropertyNames->AddItem("Shape", NULL,
		(void*)(intptr_t)gdeOCNavigationBlocker::epShape);
	pCBPropertyNames->AddItem("Layer", NULL,
		(void*)(intptr_t)gdeOCNavigationBlocker::epLayer);
	pCBPropertyNames->AddItem("Blocking priority", NULL,
		(void*)(intptr_t)gdeOCNavigationBlocker::epBlockingPriority);
	pCBPropertyNames->AddItem("Attach position", NULL,
		(void*)(intptr_t)gdeOCNavigationBlocker::epAttachPosition);
	pCBPropertyNames->AddItem("Attach rotation", NULL,
		(void*)(intptr_t)gdeOCNavigationBlocker::epAttachRotation);
	
	helper.ComboBoxFilter(groupBox, "Target:", true, "Object class property to target",
		pCBPropertyNameTarget, new cComboPropertyNameTarget(*this));
	pCBPropertyNameTarget->SetEditable(true);
	pCBPropertyNameTarget->SetDefaultSorter();
	pCBPropertyNameTarget->SetFilterCaseInsentive(true);
}

gdeWPSOCNavigationBlocker::~gdeWPSOCNavigationBlocker(){
	SetGameDefinition(NULL);
}



// Management
///////////////

void gdeWPSOCNavigationBlocker::SetGameDefinition(gdeGameDefinition::Ref gameDefinition){
	if(gameDefinition == pGameDefinition){
		return;
	}
	
	if(pGameDefinition){
		pGameDefinition->RemoveListener(pListener);
	}
	
	pGameDefinition = gameDefinition;
	
	if(gameDefinition){
		gameDefinition->AddListener(pListener);
	}
	
	UpdatePropertyList();
	UpdateNavigationBlocker();
}



gdeObjectClass *gdeWPSOCNavigationBlocker::GetObjectClass() const{
	return pGameDefinition ? pGameDefinition->GetActiveObjectClass() : NULL;
}

gdeOCNavigationBlocker *gdeWPSOCNavigationBlocker::GetNavigationBlocker() const{
	const gdeObjectClass * const objectClass = GetObjectClass();
	return objectClass ? pGameDefinition->GetActiveOCNavigationBlocker() : NULL;
}

const gdeOCNavigationBlocker::eProperties gdeWPSOCNavigationBlocker::GetPropertyName() const{
	return (gdeOCNavigationBlocker::eProperties)(intptr_t)pCBPropertyNames->GetSelectedItem()->GetData();
}



void gdeWPSOCNavigationBlocker::UpdatePropertyList(){
	const gdeObjectClass *objectClass = GetObjectClass();
	int i;
	
	decStringSet properties;
	if(objectClass){
		objectClass->AddPropertyNamesTo(properties, true);
	}
	
	const decString selectionProperty(pCBPropertyNameTarget->GetText());
	pCBPropertyNameTarget->RemoveAllItems();
	
	const int count = properties.GetCount();
	for(i=0; i<count; i++){
		pCBPropertyNameTarget->AddItem(properties.GetAt(i));
	}
	
	pCBPropertyNameTarget->SortItems();
	pCBPropertyNameTarget->SetText(selectionProperty);
}

void gdeWPSOCNavigationBlocker::UpdateNavigationBlocker(){
	const gdeOCNavigationBlocker * const navblocker = GetNavigationBlocker();
	
	if(navblocker){
		pEditPosition->SetVector(navblocker->GetPosition());
		pEditRotation->SetVector(navblocker->GetRotation());
		pEditScaling->SetVector(navblocker->GetScaling());
		pEditBoneName->SetText(navblocker->GetBoneName());
		pChkEnabled->SetChecked(navblocker->GetEnabled());
		pEditLayer->SetInteger(navblocker->GetLayer());
		pCBType->SetSelectionWithData((void*)(intptr_t)navblocker->GetType());
		pEditBlockingPriority->SetInteger(navblocker->GetBlockingPriority());
		
		igdeCodecPropertyString codec;
		decString encoded;
		codec.EncodeShapeList(navblocker->GetShapeList(), encoded);
		pEditShape->SetText(encoded);
		
	}else{
		pEditPosition->SetVector(decVector());
		pEditRotation->SetVector(decVector());
		pEditScaling->SetVector(decVector());
		pEditBoneName->ClearText();
		pChkEnabled->SetChecked(false);
		pEditLayer->ClearText();
		pCBType->SetSelectionWithData((void*)(intptr_t)deNavigationSpace::estGrid);
		pEditBlockingPriority->ClearText();
		pEditShape->ClearText();
	}
	
	const bool enabled = navblocker;
	pEditPosition->SetEnabled(enabled);
	pEditRotation->SetEnabled(enabled);
	pEditScaling->SetEnabled(enabled);
	pEditBoneName->SetEnabled(enabled);
	pChkEnabled->SetEnabled(enabled);
	pEditLayer->SetEnabled(enabled);
	pCBType->SetEnabled(enabled);
	pEditBlockingPriority->SetEnabled(enabled);
	pEditShape->SetEnabled(enabled);
	
	UpdatePropertyName();
}

void gdeWPSOCNavigationBlocker::UpdatePropertyName(){
	const gdeOCNavigationBlocker * const navblocker = GetNavigationBlocker();
	
	if(navblocker){
		pCBPropertyNameTarget->SetText(navblocker->GetPropertyName(GetPropertyName()));
		
	}else{
		pCBPropertyNameTarget->ClearText();
	}
	
	pCBPropertyNameTarget->SetEnabled(navblocker);
}
