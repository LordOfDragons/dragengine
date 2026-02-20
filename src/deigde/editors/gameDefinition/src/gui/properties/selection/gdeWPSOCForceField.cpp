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
	typedef deTObjectReference<cBaseEditVectorListener> Ref;
	cBaseEditVectorListener(gdeWPSOCForceField &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		gdeOCForceField * const forceField = pPanel.GetForceField();
		if(!forceField){
			return;
		}
		
		igdeUndo::Ref undo(
			 OnChanged(editVector->GetVector(), pPanel.GetObjectClass(), forceField));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(const decVector &vector, gdeObjectClass *objectClass,
		gdeOCForceField *forceField) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	gdeWPSOCForceField &pPanel;
	
public:
	typedef deTObjectReference<cBaseComboBoxListener> Ref;
	cBaseComboBoxListener(gdeWPSOCForceField &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		gdeOCForceField * const forceField = pPanel.GetForceField();
		if(!forceField){
			return;
		}
		
		igdeUndo::Ref undo(
			 OnChanged(*comboBox, pPanel.GetObjectClass(), forceField));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox &comboBox,
		gdeObjectClass *objectClass, gdeOCForceField *forceField) = 0;
};

class cBaseAction : public igdeAction{
protected:
	gdeWPSOCForceField &pPanel;
	
public:
	typedef deTObjectReference<cBaseAction> Ref;
	cBaseAction(gdeWPSOCForceField &panel, const char *text, const char *description) :
		igdeAction(text, description), pPanel(panel){}
	
	virtual void OnAction(){
		gdeOCForceField * const forceField = pPanel.GetForceField();
		if(!forceField){
			return;
		}
		
		igdeUndo::Ref undo(
			 OnActionForceField(pPanel.GetObjectClass(), forceField));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnActionForceField(gdeObjectClass *objectClass, gdeOCForceField *forceField) = 0;
	
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
	typedef deTObjectReference<cBaseTextFieldListener> Ref;
	cBaseTextFieldListener(gdeWPSOCForceField &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		gdeOCForceField * const forceField = pPanel.GetForceField();
		if(!forceField){
			return;
		}
		
		igdeUndo::Ref undo(
			 OnChanged(*textField, pPanel.GetObjectClass(), forceField));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField,
		gdeObjectClass *objectClass, gdeOCForceField *forceField) = 0;
};


class cEditPosition : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditPosition> Ref;
	cEditPosition(gdeWPSOCForceField &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, gdeObjectClass *objectClass,
	gdeOCForceField *forceField) override{
		return !forceField->GetPosition().IsEqualTo(vector)
			? gdeUOCForceFieldSetPosition::Ref::New(objectClass, forceField, vector) : igdeUndo::Ref();
	}
};

class cEditRotation : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditRotation> Ref;
	cEditRotation(gdeWPSOCForceField &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, gdeObjectClass *objectClass,
	gdeOCForceField *forceField) override{
		return !forceField->GetRotation().IsEqualTo(vector)
			? gdeUOCForceFieldSetRotation::Ref::New(objectClass, forceField, vector) : igdeUndo::Ref();
	}
};

class cTextBoneName : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextBoneName> Ref;
	cTextBoneName(gdeWPSOCForceField &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCForceField *forceField) override{
		return forceField->GetBoneName() != textField.GetText()
			? gdeUOCForceFieldSetBoneName::Ref::New(objectClass, forceField, textField.GetText()) : igdeUndo::Ref();
	}
};

class cEditInfluenceArea : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cEditInfluenceArea> Ref;
	cEditInfluenceArea(gdeWPSOCForceField &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCForceField *forceField) override{
		igdeCodecPropertyString codec;
		decString oldEncoded;
		codec.EncodeShapeList(forceField->GetInfluenceArea(), oldEncoded);
		const decString encoded(textField.GetText());
		if(encoded == oldEncoded){
			return {};
		}
		
		decShape::List shapeList;
		try{
			codec.DecodeShapeList(encoded, shapeList);
			
		}catch(const deException &){
			igdeCommonDialogs::Error(*pPanel.GetParentWindow(), "@GameDefinition.OCForceField.InvalidInput.Error",
				"@GameDefinition.OCForceField.ShapeDecodeInvalidValue.Error");
			textField.Focus();
			return {};
		}
	
		return gdeUOCForceFieldSetInfluenceArea::Ref::New(objectClass, forceField, shapeList);
	}
};

class cTextRadius : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextRadius> Ref;
	cTextRadius(gdeWPSOCForceField &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCForceField *forceField){
		const float value = textField.GetFloat();
		return fabsf(forceField->GetRadius() - value) > FLOAT_SAFE_EPSILON
			? gdeUOCForceFieldSetRadius::Ref::New(objectClass, forceField, value) : igdeUndo::Ref();
	}
};

class cTextExponent : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextExponent> Ref;
	cTextExponent(gdeWPSOCForceField &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCForceField *forceField){
		const float value = textField.GetFloat();
		return fabsf(forceField->GetExponent() - value) > FLOAT_SAFE_EPSILON
			? gdeUOCForceFieldSetExponent::Ref::New(objectClass, forceField, value) : igdeUndo::Ref();
	}
};

class cComboFieldType : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboFieldType> Ref;
	cComboFieldType(gdeWPSOCForceField &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox &comboBox, gdeObjectClass *objectClass,
	gdeOCForceField *forceField){
		if(!comboBox.GetSelectedItem()){
			return {};
		}
		const deForceField::eFieldTypes value =
			(deForceField::eFieldTypes)(intptr_t)comboBox.GetSelectedItem()->GetData();
		return forceField->GetFieldType() != value
			? gdeUOCForceFieldSetFieldType::Ref::New(objectClass, forceField, value) : igdeUndo::Ref();
	}
};

class cComboApplicationType : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboApplicationType> Ref;
	cComboApplicationType(gdeWPSOCForceField &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox &comboBox, gdeObjectClass *objectClass,
	gdeOCForceField *forceField){
		if(!comboBox.GetSelectedItem()){
			return {};
		}
		const deForceField::eApplicationTypes value =
			(deForceField::eApplicationTypes)(intptr_t)comboBox.GetSelectedItem()->GetData();
		return forceField->GetApplicationType() != value
			? gdeUOCForceFieldSetApplicationType::Ref::New(objectClass, forceField, value) : igdeUndo::Ref();
	}
};

class cEditDirection : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditDirection> Ref;
	cEditDirection(gdeWPSOCForceField &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(const decVector &vector, gdeObjectClass *objectClass,
	gdeOCForceField *forceField){
		return !forceField->GetDirection().IsEqualTo(vector)
			? gdeUOCForceFieldSetDirection::Ref::New(objectClass, forceField, vector) : igdeUndo::Ref();
	}
};

class cTextForce : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextForce> Ref;
	cTextForce(gdeWPSOCForceField &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCForceField *forceField){
		const float value = textField.GetFloat();
		return fabsf(forceField->GetForce() - value) > FLOAT_SAFE_EPSILON
			? gdeUOCForceFieldSetForce::Ref::New(objectClass, forceField, value) : igdeUndo::Ref();
	}
};

class cTextFluctuationDirection : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextFluctuationDirection> Ref;
	cTextFluctuationDirection(gdeWPSOCForceField &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCForceField *forceField){
		const float value = textField.GetFloat();
		return fabsf(forceField->GetFluctuationDirection() - value) > FLOAT_SAFE_EPSILON
			? gdeUOCForceFieldSetFluctuationDirection::Ref::New(objectClass, forceField, value) : igdeUndo::Ref();
	}
};

class cTextFluctuationForce : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextFluctuationForce> Ref;
	cTextFluctuationForce(gdeWPSOCForceField &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCForceField *forceField){
		const float value = textField.GetFloat();
		return fabsf(forceField->GetFluctuationForce() - value) > FLOAT_SAFE_EPSILON
			? gdeUOCForceFieldSetFluctuationForce::Ref::New(objectClass, forceField, value) : igdeUndo::Ref();
	}
};

class cEditShape : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cEditShape> Ref;
	cEditShape(gdeWPSOCForceField &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCForceField *forceField){
		igdeCodecPropertyString codec;
		decString oldEncoded;
		codec.EncodeShapeList(forceField->GetShape(), oldEncoded);
		const decString encoded(textField.GetText());
		if(encoded == oldEncoded){
			return {};
		}
		
		decShape::List shapeList;
		try{
			codec.DecodeShapeList(encoded, shapeList);
			
		}catch(const deException &){
			igdeCommonDialogs::Error(*pPanel.GetParentWindow(), "@GameDefinition.OCForceField.InvalidInput.Error",
				"@GameDefinition.OCForceField.ShapeDecodeInvalidValue.Error");
			textField.Focus();
			return {};
		}
	
		return gdeUOCForceFieldSetShape::Ref::New(objectClass, forceField, shapeList);
	}
};

class cActionEnabled : public cBaseAction{
public:
	typedef deTObjectReference<cActionEnabled> Ref;
	
public:
	cActionEnabled(gdeWPSOCForceField &panel) :
	cBaseAction(panel, "@GameDefinition.WPSOCForceField.Enabled", "@GameDefinition.WPSOCForceField.Enabled.ToolTip"){}
	
	virtual igdeUndo::Ref OnActionForceField(gdeObjectClass *objectClass, gdeOCForceField *forceField){
		return gdeUOCForceFieldToggleEnabled::Ref::New(objectClass, forceField);
	}
	
	void Update(const gdeObjectClass &, const gdeOCForceField &forceField) override{
		SetEnabled(true);
		SetSelected(forceField.GetEnabled());
	}
};


class cComboPropertyNames : public igdeComboBoxListener{
	gdeWPSOCForceField &pPanel;
	
public:
	typedef deTObjectReference<cComboPropertyNames> Ref;
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
	typedef deTObjectReference<cComboPropertyNameTarget> Ref;
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
			gdeUOCForceFieldSetPropertyName::Ref::New(pPanel.GetObjectClass(),
				forceField, propertyName, comboBox->GetText()));
	}
};

class cComboTriggerNames : public igdeComboBoxListener{
	gdeWPSOCForceField &pPanel;
	
public:
	typedef deTObjectReference<cComboTriggerNames> Ref;
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
	typedef deTObjectReference<cComboTriggerNameTarget> Ref;
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
			gdeUOCForceFieldSetTriggerName::Ref::New(pPanel.GetObjectClass(),
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
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, frameLine;
	
	pListener = gdeWPSOCForceFieldListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	helper.GroupBox(content, groupBox, "@GameDefinition.PanelOCForceField.GroupBoxForceField");
	
	helper.EditVector(groupBox, "@GameDefinition.PanelOCForceField.Position", "@GameDefinition.PanelOCForceField.Position.ToolTip",
		pEditPosition, cEditPosition::Ref::New(*this));
	helper.EditVector(groupBox, "@GameDefinition.PanelOCForceField.Rotation", "@GameDefinition.PanelOCForceField.Rotation.ToolTip", 4, 1,
		pEditRotation, cEditRotation::Ref::New(*this));
	helper.EditString(groupBox, "@GameDefinition.PanelOCForceField.Bone", "@GameDefinition.PanelOCForceField.Bone.ToolTip",
		pEditBoneName, cTextBoneName::Ref::New(*this));
	helper.EditString(groupBox, "@GameDefinition.PanelOCForceField.InfluenceArea", "@GameDefinition.PanelOCForceField.InfluenceArea.ToolTip",
		pEditInfluenceArea, cEditInfluenceArea::Ref::New(*this));
	helper.EditFloat(groupBox, "@GameDefinition.PanelOCForceField.Radius", "@GameDefinition.PanelOCForceField.Radius.ToolTip",
		pEditRadius, cTextRadius::Ref::New(*this));
	helper.EditFloat(groupBox, "@GameDefinition.PanelOCForceField.Exponent", "@GameDefinition.PanelOCForceField.Exponent.ToolTip",
		pEditExponent, cTextExponent::Ref::New(*this));
	
	helper.ComboBox(groupBox, "@GameDefinition.PanelOCForceField.FieldType", "@GameDefinition.PanelOCForceField.FieldType.ToolTip",
		pCBFieldType, cComboFieldType::Ref::New(*this));
	pCBFieldType->SetAutoTranslateItems(true);
	pCBFieldType->AddItem("@GameDefinition.PanelOCForceField.FieldTypeRadial", nullptr, (void*)(intptr_t)deForceField::eftRadial);
	pCBFieldType->AddItem("@GameDefinition.PanelOCForceField.FieldTypeLinear", nullptr, (void*)(intptr_t)deForceField::eftLinear);
	pCBFieldType->AddItem("@GameDefinition.PanelOCForceField.FieldTypeVortex", nullptr, (void*)(intptr_t)deForceField::eftVortex);
	
	helper.ComboBox(groupBox, "@GameDefinition.PanelOCForceField.ApplicationType", "@GameDefinition.PanelOCForceField.ApplicationType.ToolTip",
		pCBApplicationType, cComboApplicationType::Ref::New(*this));
	pCBApplicationType->SetAutoTranslateItems(true);
	pCBApplicationType->AddItem("@GameDefinition.PanelOCForceField.ApplicationTypeDirect", nullptr, (void*)(intptr_t)deForceField::eatDirect);
	pCBApplicationType->AddItem("@GameDefinition.PanelOCForceField.ApplicationTypeSurface", nullptr, (void*)(intptr_t)deForceField::eatSurface);
	pCBApplicationType->AddItem("@GameDefinition.PanelOCForceField.ApplicationTypeMass", nullptr, (void*)(intptr_t)deForceField::eatMass);
	pCBApplicationType->AddItem("@GameDefinition.PanelOCForceField.ApplicationTypeSpeed", nullptr, (void*)(intptr_t)deForceField::eatSpeed);
	
	helper.EditVector(groupBox, "@GameDefinition.PanelOCForceField.Direction", "@GameDefinition.PanelOCForceField.Direction.ToolTip",
		pEditDirection, cEditDirection::Ref::New(*this));
	helper.EditFloat(groupBox, "@GameDefinition.PanelOCForceField.Force", "@GameDefinition.PanelOCForceField.Force.ToolTip",
		pEditForce, cTextForce::Ref::New(*this));
	helper.EditFloat(groupBox, "@GameDefinition.PanelOCForceField.FluctuationDirection", "@GameDefinition.PanelOCForceField.FluctuationDirection.ToolTip",
		pEditFluctuationDirection, cTextFluctuationDirection::Ref::New(*this));
	helper.EditFloat(groupBox, "@GameDefinition.PanelOCForceField.FluctuationForce", "@GameDefinition.PanelOCForceField.FluctuationForce.ToolTip",
		pEditFluctuationForce, cTextFluctuationForce::Ref::New(*this));
	helper.EditString(groupBox, "@GameDefinition.PanelOCForceField.Shape", "@GameDefinition.PanelOCForceField.Shape.ToolTip",
		pEditShape, cEditShape::Ref::New(*this));
	helper.CheckBox(groupBox, pChkEnabled, cActionEnabled::Ref::New(*this));
	
	// property targets
	helper.GroupBox(content, groupBox, "@GameDefinition.PanelOCForceField.GroupBoxProperties");
	helper.ComboBox(groupBox, "@GameDefinition.PanelOCForceField.Property", "@GameDefinition.PanelOCForceField.Property.ToolTip",
		pCBPropertyNames, cComboPropertyNames::Ref::New(*this));
	pCBPropertyNames->SetAutoTranslateItems(true);
	pCBPropertyNames->AddItem("@GameDefinition.PanelOCForceField.PropertyInfluenceArea", nullptr, (void*)(intptr_t)gdeOCForceField::epInfluenceArea);
	pCBPropertyNames->AddItem("@GameDefinition.PanelOCForceField.PropertyRadius", nullptr, (void*)(intptr_t)gdeOCForceField::epRadius);
	pCBPropertyNames->AddItem("@GameDefinition.PanelOCForceField.PropertyExponent", nullptr, (void*)(intptr_t)gdeOCForceField::epExponent);
	pCBPropertyNames->AddItem("@GameDefinition.PanelOCForceField.PropertyFieldType", nullptr, (void*)(intptr_t)gdeOCForceField::epFieldType);
	pCBPropertyNames->AddItem("@GameDefinition.PanelOCForceField.PropertyApplicationType", nullptr, (void*)(intptr_t)gdeOCForceField::epApplicationType);
	pCBPropertyNames->AddItem("@GameDefinition.PanelOCForceField.PropertyDirection", nullptr, (void*)(intptr_t)gdeOCForceField::epDirection);
	pCBPropertyNames->AddItem("@GameDefinition.PanelOCForceField.PropertyForce", nullptr, (void*)(intptr_t)gdeOCForceField::epForce);
	pCBPropertyNames->AddItem("@GameDefinition.PanelOCForceField.PropertyFluctuationDirection", nullptr, (void*)(intptr_t)gdeOCForceField::epFluctuationDirection);
	pCBPropertyNames->AddItem("@GameDefinition.PanelOCForceField.PropertyFluctuationForce", nullptr, (void*)(intptr_t)gdeOCForceField::epFluctuationForce);
	pCBPropertyNames->AddItem("@GameDefinition.PanelOCForceField.PropertyShape", nullptr, (void*)(intptr_t)gdeOCForceField::epShape);
	pCBPropertyNames->AddItem("@GameDefinition.PanelOCForceField.PropertyEnabled", nullptr, (void*)(intptr_t)gdeOCForceField::epEnabled);
	pCBPropertyNames->AddItem("@GameDefinition.PanelOCForceField.PropertyAttachPosition", nullptr, (void*)(intptr_t)gdeOCForceField::epAttachPosition);
	pCBPropertyNames->AddItem("@GameDefinition.PanelOCForceField.PropertyAttachRotation", nullptr, (void*)(intptr_t)gdeOCForceField::epAttachRotation);
	
	helper.ComboBoxFilter(groupBox, "@GameDefinition.PanelOCForceField.Target", true, "@GameDefinition.PanelOCForceField.Target.ToolTip",
		pCBPropertyNameTarget, cComboPropertyNameTarget::Ref::New(*this));
	pCBPropertyNameTarget->SetEditable(true);
	pCBPropertyNameTarget->SetDefaultSorter();
	pCBPropertyNameTarget->SetFilterCaseInsentive(true);
	
	// trigger targets
	helper.GroupBox(content, groupBox, "@GameDefinition.PanelOCForceField.GroupBoxTriggers");
	helper.ComboBox(groupBox, "@GameDefinition.PanelOCForceField.Trigger", "@GameDefinition.PanelOCForceField.Trigger.ToolTip",
		pCBTriggerNames, cComboTriggerNames::Ref::New(*this));
	pCBTriggerNames->SetAutoTranslateItems(true);
	pCBTriggerNames->AddItem("@GameDefinition.PanelOCForceField.TriggerEnabled", nullptr, (void*)(intptr_t)gdeOCForceField::etEnabled);
	
	helper.ComboBoxFilter(groupBox, "@GameDefinition.PanelOCForceField.TriggerTarget", true, "@GameDefinition.PanelOCForceField.TriggerTarget.ToolTip",
		pCBTriggerNameTarget, cComboTriggerNameTarget::Ref::New(*this));
	pCBTriggerNameTarget->SetEditable(true);
	pCBTriggerNameTarget->SetDefaultSorter();
	pCBTriggerNameTarget->SetFilterCaseInsentive(true);
}

gdeWPSOCForceField::~gdeWPSOCForceField(){
	SetGameDefinition(nullptr);
}



// Management
///////////////

void gdeWPSOCForceField::SetGameDefinition(gdeGameDefinition *gameDefinition){
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
	UpdateForceField();
}



gdeObjectClass *gdeWPSOCForceField::GetObjectClass() const{
	return pGameDefinition ? pGameDefinition->GetActiveObjectClass().Pointer() : nullptr;
}

gdeOCForceField *gdeWPSOCForceField::GetForceField() const{
	gdeObjectClass * const objectClass = GetObjectClass();
	return objectClass ? pGameDefinition->GetActiveOCForceField().Pointer() : nullptr;
}

gdeOCForceField::eProperties gdeWPSOCForceField::GetPropertyName() const{
	return (gdeOCForceField::eProperties)(intptr_t)pCBPropertyNames->GetSelectedItem()->GetData();
}

gdeOCForceField::eTriggers gdeWPSOCForceField::GetTriggerName() const{
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
