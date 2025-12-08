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

#include "gdeWPSOCForceField.h"
#include "gdeWPSOCForceFieldListener.h"
#include "../gdeWindowProperties.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/category/gdeCategory.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../undosys/objectClass/forceField/gdeUOCForceFieldSetApplicationType.h"
#include "../../../undosys/objectClass/forceField/gdeUOCForceFieldSetBoneName.h"
#include "../../../undosys/objectClass/forceField/gdeUOCForceFieldSetDirection.h"
#include "../../../undosys/objectClass/forceField/gdeUOCForceFieldSetExponent.h"
#include "../../../undosys/objectClass/forceField/gdeUOCForceFieldSetFieldType.h"
#include "../../../undosys/objectClass/forceField/gdeUOCForceFieldSetFluctuationDirection.h"
#include "../../../undosys/objectClass/forceField/gdeUOCForceFieldSetFluctuationForce.h"
#include "../../../undosys/objectClass/forceField/gdeUOCForceFieldSetForce.h"
#include "../../../undosys/objectClass/forceField/gdeUOCForceFieldSetInfluenceArea.h"
#include "../../../undosys/objectClass/forceField/gdeUOCForceFieldSetPosition.h"
#include "../../../undosys/objectClass/forceField/gdeUOCForceFieldSetPropertyName.h"
#include "../../../undosys/objectClass/forceField/gdeUOCForceFieldSetRadius.h"
#include "../../../undosys/objectClass/forceField/gdeUOCForceFieldSetRotation.h"
#include "../../../undosys/objectClass/forceField/gdeUOCForceFieldSetShape.h"
#include "../../../undosys/objectClass/forceField/gdeUOCForceFieldSetTriggerName.h"
#include "../../../undosys/objectClass/forceField/gdeUOCForceFieldToggleEnabled.h"

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
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/logger/deLogger.h>



// Actions
////////////

namespace{

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	gdeWPSOCForceField &pPanel;
	
public:
	cBaseEditVectorListener(gdeWPSOCForceField &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		gdeOCForceField * const forceField = pPanel.GetForceField();
		if(!forceField){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(
			 OnChanged(editVector->GetVector(), pPanel.GetObjectClass(), forceField)));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(const decVector &vector, gdeObjectClass *objectClass,
		gdeOCForceField *forceField) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	gdeWPSOCForceField &pPanel;
	
public:
	cBaseComboBoxListener(gdeWPSOCForceField &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		gdeOCForceField * const forceField = pPanel.GetForceField();
		if(!forceField){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(
			 OnChanged(*comboBox, pPanel.GetObjectClass(), forceField)));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeComboBox &comboBox,
		gdeObjectClass *objectClass, gdeOCForceField *forceField) = 0;
};

class cBaseAction : public igdeAction{
protected:
	gdeWPSOCForceField &pPanel;
	
public:
	cBaseAction(gdeWPSOCForceField &panel, const char *text, const char *description) :
		igdeAction(text, description), pPanel(panel){}
	
	virtual void OnAction(){
		gdeOCForceField * const forceField = pPanel.GetForceField();
		if(!forceField){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(
			 OnActionForceField(pPanel.GetObjectClass(), forceField)));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnActionForceField(gdeObjectClass *objectClass, gdeOCForceField *forceField) = 0;
	
	virtual void Update(){
		gdeOCForceField * const forceField = pPanel.GetForceField();
		if(forceField){
			Update(*pPanel.GetObjectClass(), *forceField);
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const gdeObjectClass &, const gdeOCForceField &){
	}
};

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	gdeWPSOCForceField &pPanel;
	
public:
	cBaseTextFieldListener(gdeWPSOCForceField &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		gdeOCForceField * const forceField = pPanel.GetForceField();
		if(!forceField){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(
			 OnChanged(*textField, pPanel.GetObjectClass(), forceField)));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField,
		gdeObjectClass *objectClass, gdeOCForceField *forceField) = 0;
};


class cEditPosition : public cBaseEditVectorListener {
public:
	cEditPosition(gdeWPSOCForceField &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo *OnChanged(const decVector &vector, gdeObjectClass *objectClass,
	gdeOCForceField *forceField){
		return !forceField->GetPosition().IsEqualTo(vector)
			? new gdeUOCForceFieldSetPosition(objectClass, forceField, vector) : NULL;
	}
};

class cEditRotation : public cBaseEditVectorListener {
public:
	cEditRotation(gdeWPSOCForceField &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo *OnChanged(const decVector &vector, gdeObjectClass *objectClass,
	gdeOCForceField *forceField){
		return !forceField->GetRotation().IsEqualTo(vector)
			? new gdeUOCForceFieldSetRotation(objectClass, forceField, vector) : NULL;
	}
};

class cTextBoneName : public cBaseTextFieldListener{
public:
	cTextBoneName(gdeWPSOCForceField &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCForceField *forceField){
		return forceField->GetBoneName() != textField.GetText()
			? new gdeUOCForceFieldSetBoneName(objectClass, forceField, textField.GetText()) : NULL;
	}
};

class cEditInfluenceArea : public cBaseTextFieldListener{
public:
	cEditInfluenceArea(gdeWPSOCForceField &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCForceField *forceField){
		igdeCodecPropertyString codec;
		decString oldEncoded;
		codec.EncodeShapeList(forceField->GetInfluenceArea(), oldEncoded);
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
		
		return new gdeUOCForceFieldSetInfluenceArea(objectClass, forceField, shapeList);
	}
};

class cTextRadius : public cBaseTextFieldListener{
public:
	cTextRadius(gdeWPSOCForceField &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCForceField *forceField){
		const float value = textField.GetFloat();
		return fabsf(forceField->GetRadius() - value) > FLOAT_SAFE_EPSILON
			? new gdeUOCForceFieldSetRadius(objectClass, forceField, value) : NULL;
	}
};

class cTextExponent : public cBaseTextFieldListener{
public:
	cTextExponent(gdeWPSOCForceField &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCForceField *forceField){
		const float value = textField.GetFloat();
		return fabsf(forceField->GetExponent() - value) > FLOAT_SAFE_EPSILON
			? new gdeUOCForceFieldSetExponent(objectClass, forceField, value) : NULL;
	}
};

class cComboFieldType : public cBaseComboBoxListener{
public:
	cComboFieldType(gdeWPSOCForceField &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeComboBox &comboBox, gdeObjectClass *objectClass,
	gdeOCForceField *forceField){
		if(!comboBox.GetSelectedItem()){
			return NULL;
		}
		const deForceField::eFieldTypes value =
			(deForceField::eFieldTypes)(intptr_t)comboBox.GetSelectedItem()->GetData();
		return forceField->GetFieldType() != value
			? new gdeUOCForceFieldSetFieldType(objectClass, forceField, value) : NULL;
	}
};

class cComboApplicationType : public cBaseComboBoxListener{
public:
	cComboApplicationType(gdeWPSOCForceField &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeComboBox &comboBox, gdeObjectClass *objectClass,
	gdeOCForceField *forceField){
		if(!comboBox.GetSelectedItem()){
			return NULL;
		}
		const deForceField::eApplicationTypes value =
			(deForceField::eApplicationTypes)(intptr_t)comboBox.GetSelectedItem()->GetData();
		return forceField->GetApplicationType() != value
			? new gdeUOCForceFieldSetApplicationType(objectClass, forceField, value) : NULL;
	}
};

class cEditDirection : public cBaseEditVectorListener {
public:
	cEditDirection(gdeWPSOCForceField &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo *OnChanged(const decVector &vector, gdeObjectClass *objectClass,
	gdeOCForceField *forceField){
		return !forceField->GetDirection().IsEqualTo(vector)
			? new gdeUOCForceFieldSetDirection(objectClass, forceField, vector) : NULL;
	}
};

class cTextForce : public cBaseTextFieldListener{
public:
	cTextForce(gdeWPSOCForceField &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCForceField *forceField){
		const float value = textField.GetFloat();
		return fabsf(forceField->GetForce() - value) > FLOAT_SAFE_EPSILON
			? new gdeUOCForceFieldSetForce(objectClass, forceField, value) : NULL;
	}
};

class cTextFluctuationDirection : public cBaseTextFieldListener{
public:
	cTextFluctuationDirection(gdeWPSOCForceField &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCForceField *forceField){
		const float value = textField.GetFloat();
		return fabsf(forceField->GetFluctuationDirection() - value) > FLOAT_SAFE_EPSILON
			? new gdeUOCForceFieldSetFluctuationDirection(objectClass, forceField, value) : NULL;
	}
};

class cTextFluctuationForce : public cBaseTextFieldListener{
public:
	cTextFluctuationForce(gdeWPSOCForceField &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCForceField *forceField){
		const float value = textField.GetFloat();
		return fabsf(forceField->GetFluctuationForce() - value) > FLOAT_SAFE_EPSILON
			? new gdeUOCForceFieldSetFluctuationForce(objectClass, forceField, value) : NULL;
	}
};

class cEditShape : public cBaseTextFieldListener{
public:
	cEditShape(gdeWPSOCForceField &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCForceField *forceField){
		igdeCodecPropertyString codec;
		decString oldEncoded;
		codec.EncodeShapeList(forceField->GetShape(), oldEncoded);
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
		
		return new gdeUOCForceFieldSetShape(objectClass, forceField, shapeList);
	}
};

class cActionEnabled : public cBaseAction{
public:
	cActionEnabled(gdeWPSOCForceField &panel) :
	cBaseAction(panel, "Enabled", "Force field is enabled"){}
	
	virtual igdeUndo *OnActionForceField(gdeObjectClass *objectClass, gdeOCForceField *forceField){
		return new gdeUOCForceFieldToggleEnabled(objectClass, forceField);
	}
	
	void Update(const gdeObjectClass &, const gdeOCForceField &forceField) override{
		SetEnabled(true);
		SetSelected(forceField.GetEnabled());
	}
};


class cComboPropertyNames : public igdeComboBoxListener{
	gdeWPSOCForceField &pPanel;
	
public:
	cComboPropertyNames(gdeWPSOCForceField &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox*){
		if(pPanel.GetForceField()){
			pPanel.UpdatePropertyName();
		}
	}
};

class cComboPropertyNameTarget : public igdeComboBoxListener{
	gdeWPSOCForceField &pPanel;
	
public:
	cComboPropertyNameTarget(gdeWPSOCForceField &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		gdeOCForceField * const forceField = pPanel.GetForceField();
		if(!forceField){
			return;
		}
		
		const gdeOCForceField::eProperties propertyName = pPanel.GetPropertyName();
		if(forceField->GetPropertyName(propertyName) == comboBox->GetText()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUOCForceFieldSetPropertyName::Ref::NewWith(pPanel.GetObjectClass(),
				forceField, propertyName, comboBox->GetText()));
	}
};

class cComboTriggerNames : public igdeComboBoxListener{
	gdeWPSOCForceField &pPanel;
	
public:
	cComboTriggerNames(gdeWPSOCForceField &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox*){
		if(pPanel.GetForceField()){
			pPanel.UpdateTriggerName();
		}
	}
};

class cComboTriggerNameTarget : public igdeComboBoxListener{
	gdeWPSOCForceField &pPanel;
	
public:
	cComboTriggerNameTarget(gdeWPSOCForceField &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		if(!pPanel.GetForceField()){
			return;
		}
		
		const gdeOCForceField::eTriggers triggerName = pPanel.GetTriggerName();
		if(pPanel.GetForceField()->GetTriggerName(triggerName) == comboBox->GetText()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUOCForceFieldSetTriggerName::Ref::NewWith(pPanel.GetObjectClass(),
				pPanel.GetForceField(), triggerName, comboBox->GetText()));
	}
};

}



// Class gdeWPSOCForceField
/////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSOCForceField::gdeWPSOCForceField(gdeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties),
pListener(NULL),
pGameDefinition(NULL)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, frameLine;
	
	pListener.TakeOverWith(*this);
	
	content.TakeOver(new igdeContainerFlow(env, igdeContainerFlow::eaY));
	AddChild(content);
	
	helper.GroupBox(content, groupBox, "Object Class Particle Emitter:");
	
	helper.EditVector(groupBox, "Position:", "Position relative to object class",
		pEditPosition, new cEditPosition(*this));
	helper.EditVector(groupBox, "Rotation:", "Rotation in degrees relative to object class", 4, 1,
		pEditRotation, new cEditRotation(*this));
	helper.EditString(groupBox, "Bone:", "Bone name or empty string if not used",
		pEditBoneName, new cTextBoneName(*this));
	helper.EditString(groupBox, "Influence Area:", "Area influenced by the force field",
		pEditInfluenceArea, new cEditInfluenceArea(*this));
	helper.EditFloat(groupBox, "Radius:", "Radius in meters of force field effect",
		pEditRadius, new cTextRadius(*this));
	helper.EditFloat(groupBox, "Exponent:", "Force fall-off exponent",
		pEditExponent, new cTextExponent(*this));
	
	helper.ComboBox(groupBox, "Field Type:", "Force field type",
		pCBFieldType, new cComboFieldType(*this));
	pCBFieldType->AddItem("Radial", NULL, (void*)(intptr_t)deForceField::eftRadial);
	pCBFieldType->AddItem("Linear", NULL, (void*)(intptr_t)deForceField::eftLinear);
	pCBFieldType->AddItem("Vortex", NULL, (void*)(intptr_t)deForceField::eftVortex);
	
	helper.ComboBox(groupBox, "Application Type:", "Force application type",
		pCBApplicationType, new cComboApplicationType(*this));
	pCBApplicationType->AddItem("Direct", NULL, (void*)(intptr_t)deForceField::eatDirect);
	pCBApplicationType->AddItem("Surface", NULL, (void*)(intptr_t)deForceField::eatSurface);
	pCBApplicationType->AddItem("Mass", NULL, (void*)(intptr_t)deForceField::eatMass);
	pCBApplicationType->AddItem("Speed", NULL, (void*)(intptr_t)deForceField::eatSpeed);
	
	helper.EditVector(groupBox, "Direction:", "Direction the force is applied in (linear)",
		pEditDirection, new cEditDirection(*this));
	helper.EditFloat(groupBox, "Force:", "Force in newtons",
		pEditForce, new cTextForce(*this));
	helper.EditFloat(groupBox, "Fluctuation Direction:", "Fluctiation direction in degrees",
		pEditFluctuationDirection, new cTextFluctuationDirection(*this));
	helper.EditFloat(groupBox, "Fluctuation Force:", "Fluctiation force in newtons",
		pEditFluctuationForce, new cTextFluctuationForce(*this));
	helper.EditString(groupBox, "Shape:", "Shape of force source",
		pEditShape, new cEditShape(*this));
	helper.CheckBox(groupBox, pChkEnabled, new cActionEnabled(*this), true);
	
	// property targets
	helper.GroupBox(content, groupBox, "Properties:");
	helper.ComboBox(groupBox, "Property:", "Property to set target for",
		pCBPropertyNames, new cComboPropertyNames(*this));
	pCBPropertyNames->AddItem("Influence Area", NULL, (void*)(intptr_t)gdeOCForceField::epInfluenceArea);
	pCBPropertyNames->AddItem("Radius", NULL, (void*)(intptr_t)gdeOCForceField::epRadius);
	pCBPropertyNames->AddItem("Exponent", NULL, (void*)(intptr_t)gdeOCForceField::epExponent);
	pCBPropertyNames->AddItem("Field Type", NULL, (void*)(intptr_t)gdeOCForceField::epFieldType);
	pCBPropertyNames->AddItem("Application Type", NULL, (void*)(intptr_t)gdeOCForceField::epApplicationType);
	pCBPropertyNames->AddItem("Direction", NULL, (void*)(intptr_t)gdeOCForceField::epDirection);
	pCBPropertyNames->AddItem("Force", NULL, (void*)(intptr_t)gdeOCForceField::epForce);
	pCBPropertyNames->AddItem("Fluctuation Direction", NULL, (void*)(intptr_t)gdeOCForceField::epFluctuationDirection);
	pCBPropertyNames->AddItem("Fluctuation Force", NULL, (void*)(intptr_t)gdeOCForceField::epFluctuationForce);
	pCBPropertyNames->AddItem("Shape", NULL, (void*)(intptr_t)gdeOCForceField::epShape);
	pCBPropertyNames->AddItem("Enabled", NULL, (void*)(intptr_t)gdeOCForceField::epEnabled);
	pCBPropertyNames->AddItem("Attach position", NULL, (void*)(intptr_t)gdeOCForceField::epAttachPosition);
	pCBPropertyNames->AddItem("Attach rotation", NULL, (void*)(intptr_t)gdeOCForceField::epAttachRotation);
	
	helper.ComboBoxFilter(groupBox, "Target:", true, "Object class property to target",
		pCBPropertyNameTarget, new cComboPropertyNameTarget(*this));
	pCBPropertyNameTarget->SetEditable(true);
	pCBPropertyNameTarget->SetDefaultSorter();
	pCBPropertyNameTarget->SetFilterCaseInsentive(true);
	
	// trigger targets
	helper.GroupBox(content, groupBox, "Triggers:");
	helper.ComboBox(groupBox, "Trigger:", "Trigger to set target for",
		pCBTriggerNames, new cComboTriggerNames(*this));
	pCBTriggerNames->AddItem("Enabled", NULL, (void*)(intptr_t)gdeOCForceField::etEnabled);
	
	helper.ComboBoxFilter(groupBox, "Target:", true, "Object class property to target",
		pCBTriggerNameTarget, new cComboTriggerNameTarget(*this));
	pCBTriggerNameTarget->SetEditable(true);
	pCBTriggerNameTarget->SetDefaultSorter();
	pCBTriggerNameTarget->SetFilterCaseInsentive(true);
}

gdeWPSOCForceField::~gdeWPSOCForceField(){
	SetGameDefinition(NULL);
	
}



// Management
///////////////

void gdeWPSOCForceField::SetGameDefinition(gdeGameDefinition *gameDefinition){
	if(pGameDefinition == gameDefinition){
		return;
	}
	
	if(pGameDefinition){
		pGameDefinition->RemoveListener(pListener);
	}
	
	pGameDefinition = gameDefinition;
	
	if(gameDefinition){
		gameDefinition->AddListener(pListener);
		gameDefinition->AddReference();
	}
	
	UpdatePropertyList();
	UpdateForceField();
}



gdeObjectClass *gdeWPSOCForceField::GetObjectClass() const{
	return pGameDefinition ? pGameDefinition->GetActiveObjectClass() : NULL;
}

gdeOCForceField *gdeWPSOCForceField::GetForceField() const{
	const gdeObjectClass * const objectClass = GetObjectClass();
	return objectClass ? pGameDefinition->GetActiveOCForceField() : NULL;
}

const gdeOCForceField::eProperties gdeWPSOCForceField::GetPropertyName() const{
	return (gdeOCForceField::eProperties)(intptr_t)pCBPropertyNames->GetSelectedItem()->GetData();
}

const gdeOCForceField::eTriggers gdeWPSOCForceField::GetTriggerName() const{
	return (gdeOCForceField::eTriggers)(intptr_t)pCBTriggerNames->GetSelectedItem()->GetData();
}



void gdeWPSOCForceField::UpdatePropertyList(){
	const gdeObjectClass *objectClass = GetObjectClass();
	int i;
	
	decStringSet properties;
	if(objectClass){
		objectClass->AddPropertyNamesTo(properties, true);
	}
	
	const decString selectionProperty(pCBPropertyNameTarget->GetText());
	const decString selectionTrigger(pCBTriggerNameTarget->GetText());
	pCBPropertyNameTarget->RemoveAllItems();
	pCBTriggerNameTarget->RemoveAllItems();
	
	const int count = properties.GetCount();
	for(i=0; i<count; i++){
		pCBPropertyNameTarget->AddItem(properties.GetAt(i));
		pCBTriggerNameTarget->AddItem(properties.GetAt(i));
	}
	
	pCBPropertyNameTarget->SortItems();
	pCBPropertyNameTarget->SetText(selectionProperty);
	
	pCBTriggerNameTarget->SortItems();
	pCBTriggerNameTarget->SetText(selectionTrigger);
}



void gdeWPSOCForceField::UpdateForceField(){
	const gdeOCForceField * const forceField = GetForceField();
	
	if(forceField){
		igdeCodecPropertyString codec;
		decString encoded;
		
		pEditPosition->SetVector(forceField->GetPosition());
		pEditRotation->SetVector(forceField->GetRotation());
		pEditBoneName->SetText(forceField->GetBoneName());
		
		codec.EncodeShapeList(forceField->GetInfluenceArea(), encoded);
		pEditInfluenceArea->SetText(encoded);
		
		pEditRadius->SetFloat(forceField->GetRadius());
		pEditExponent->SetFloat(forceField->GetExponent());
		pCBFieldType->SetSelectionWithData((void*)(intptr_t)forceField->GetFieldType());
		pCBApplicationType->SetSelectionWithData((void*)(intptr_t)forceField->GetApplicationType());
		pEditDirection->SetVector(forceField->GetDirection());
		pEditForce->SetFloat(forceField->GetForce());
		pEditFluctuationDirection->SetFloat(forceField->GetFluctuationDirection());
		pEditFluctuationForce->SetFloat(forceField->GetFluctuationForce());
		
		codec.EncodeShapeList(forceField->GetShape(), encoded);
		pEditShape->SetText(encoded);
		
	}else{
		pEditPosition->SetVector(decVector());
		pEditRotation->SetVector(decVector());
		pEditBoneName->ClearText();
		pEditInfluenceArea->ClearText();
		pEditRadius->ClearText();
		pEditExponent->ClearText();
		pCBFieldType->SetSelectionWithData((void*)(intptr_t)deForceField::eftRadial);
		pCBApplicationType->SetSelectionWithData((void*)(intptr_t)deForceField::eatDirect);
		pEditDirection->SetVector(decVector());
		pEditForce->ClearText();
		pEditFluctuationDirection->ClearText();
		pEditFluctuationForce->ClearText();
		pEditShape->ClearText();
	}
	
	const bool enabled = forceField;
	pEditPosition->SetEnabled(enabled);	
	pEditRotation->SetEnabled(enabled);
	pEditBoneName->SetEnabled(enabled);
	pEditInfluenceArea->SetEnabled(enabled);
	pEditRadius->SetEnabled(enabled);
	pEditExponent->SetEnabled(enabled);
	pCBFieldType->SetEnabled(enabled);
	pCBApplicationType->SetEnabled(enabled);
	pEditDirection->SetEnabled(enabled);
	pEditForce->SetEnabled(enabled);
	pEditFluctuationDirection->SetEnabled(enabled);
	pEditFluctuationForce->SetEnabled(enabled);
	pEditShape->SetEnabled(enabled);
	
	pChkEnabled->GetAction()->Update();
	
	UpdatePropertyName();
	UpdateTriggerName();
}

void gdeWPSOCForceField::UpdatePropertyName(){
	const gdeOCForceField * const forceField = GetForceField();
	
	if(forceField){
		pCBPropertyNameTarget->SetText(forceField->GetPropertyName(GetPropertyName()));
		
	}else{
		pCBPropertyNameTarget->ClearText();
	}
	
	pCBPropertyNameTarget->SetEnabled(forceField);
}

void gdeWPSOCForceField::UpdateTriggerName(){
	const gdeOCForceField * const forceField = GetForceField();
	
	if(forceField){
		pCBTriggerNameTarget->SetText(forceField->GetTriggerName(GetTriggerName()));
		
	}else{
		pCBTriggerNameTarget->ClearText();
	}
	
	pCBTriggerNameTarget->SetEnabled(forceField);
}
