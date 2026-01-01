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

#include "gdeWPSOCLight.h"
#include "gdeWPSOCLightListener.h"
#include "../gdeWindowProperties.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/category/gdeCategory.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/property/gdeProperty.h"
#include "../../../undosys/objectClass/light/gdeUOCLightSetAmbientRatio.h"
#include "../../../undosys/objectClass/light/gdeUOCLightSetBoneName.h"
#include "../../../undosys/objectClass/light/gdeUOCLightSetColor.h"
#include "../../../undosys/objectClass/light/gdeUOCLightSetHalfIntDist.h"
#include "../../../undosys/objectClass/light/gdeUOCLightSetHintLightImportance.h"
#include "../../../undosys/objectClass/light/gdeUOCLightSetHintMovement.h"
#include "../../../undosys/objectClass/light/gdeUOCLightSetHintParameter.h"
#include "../../../undosys/objectClass/light/gdeUOCLightSetHintShadowImportance.h"
#include "../../../undosys/objectClass/light/gdeUOCLightSetIntensity.h"
#include "../../../undosys/objectClass/light/gdeUOCLightSetLightSkinPath.h"
#include "../../../undosys/objectClass/light/gdeUOCLightSetPosition.h"
#include "../../../undosys/objectClass/light/gdeUOCLightSetPropertyName.h"
#include "../../../undosys/objectClass/light/gdeUOCLightSetRange.h"
#include "../../../undosys/objectClass/light/gdeUOCLightSetRotation.h"
#include "../../../undosys/objectClass/light/gdeUOCLightSetSpotAngle.h"
#include "../../../undosys/objectClass/light/gdeUOCLightSetSpotRatio.h"
#include "../../../undosys/objectClass/light/gdeUOCLightSetSpotSmoothness.h"
#include "../../../undosys/objectClass/light/gdeUOCLightSetSpotExponent.h"
#include "../../../undosys/objectClass/light/gdeUOCLightSetTriggerName.h"
#include "../../../undosys/objectClass/light/gdeUOCLightSetType.h"
#include "../../../undosys/objectClass/light/gdeUOCLightToggleActivated.h"
#include "../../../undosys/objectClass/light/gdeUOCLightToggleCastShadows.h"


#include <deigde/codec/igdeCodecPropertyString.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeColorBox.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/event/igdeColorBoxListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Actions
////////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	gdeWPSOCLight &pPanel;
	
public:
	typedef deTObjectReference<cBaseTextFieldListener> Ref;
	cBaseTextFieldListener(gdeWPSOCLight &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		gdeOCLight * const light = pPanel.GetLight();
		if(!light){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(*textField, pPanel.GetObjectClass(), light));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField,
		gdeObjectClass *objectClass, gdeOCLight *light) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	gdeWPSOCLight &pPanel;
	
public:
	typedef deTObjectReference<cBaseEditVectorListener> Ref;
	cBaseEditVectorListener(gdeWPSOCLight &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		gdeOCLight * const light = pPanel.GetLight();
		if(!light){
			return;
		}
		
		igdeUndo::Ref undo(
			 OnChanged(editVector->GetVector(), pPanel.GetObjectClass(), light));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(const decVector &vector, gdeObjectClass *objectClass,
		gdeOCLight *light) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	gdeWPSOCLight &pPanel;
	
public:
	typedef deTObjectReference<cBaseComboBoxListener> Ref;
	cBaseComboBoxListener(gdeWPSOCLight &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		gdeOCLight * const light = pPanel.GetLight();
		if(!light){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(*comboBox, pPanel.GetObjectClass(), light));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox &comboBox,
		gdeObjectClass *objectClass, gdeOCLight *light) = 0;
};

class cBaseAction : public igdeAction{
protected:
	gdeWPSOCLight &pPanel;
	
public:
	typedef deTObjectReference<cBaseAction> Ref;
	cBaseAction(gdeWPSOCLight &panel, const char *text, const char *description) :
		igdeAction(text, description), pPanel(panel){}
	
	virtual void OnAction(){
		gdeOCLight * const light = pPanel.GetLight();
		if(!light){
			return;
		}
		
		igdeUndo::Ref undo(OnActionLight(pPanel.GetObjectClass(), light));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnActionLight(gdeObjectClass *objectClass, gdeOCLight *light) = 0;
};


class cComboType : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboType> Ref;
	cComboType(gdeWPSOCLight &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox &comboBox, gdeObjectClass *objectClass,
	gdeOCLight *light) override{
		const deLight::eLightTypes value =
			(deLight::eLightTypes)(intptr_t)comboBox.GetSelectedItem()->GetData();
		if(value == light->GetType()){
			return {};
		}
		return gdeUOCLightSetType::Ref::New(objectClass, light, value);
	}
};

class cColorLight : public igdeColorBoxListener{
	gdeWPSOCLight &pPanel;
	
public:
	typedef deTObjectReference<cColorLight> Ref;
	cColorLight(gdeWPSOCLight &panel) : pPanel(panel){}
	
	virtual void OnColorChanged(igdeColorBox *colorBox){
		gdeOCLight * const light = pPanel.GetLight();
		if(!light || light->GetColor().IsEqualTo(colorBox->GetColor())){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUOCLightSetColor::Ref::New(
			pPanel.GetObjectClass(), light, colorBox->GetColor()));
	}
};

class cTextIntensity : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextIntensity> Ref;
	cTextIntensity(gdeWPSOCLight &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCLight *light){
		const float value = textField.GetFloat();
		if(fabsf(light->GetIntensity() - value) < FLOAT_SAFE_EPSILON){
			return {};
		}
		return gdeUOCLightSetIntensity::Ref::New(objectClass, light, value);
	}
};

class cTextAmbientRatio : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextAmbientRatio> Ref;
	cTextAmbientRatio(gdeWPSOCLight &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCLight *light){
		const float value = textField.GetFloat();
		if(fabsf(light->GetAmbientRatio() - value) < FLOAT_SAFE_EPSILON){
			return {};
		}
		return gdeUOCLightSetAmbientRatio::Ref::New(objectClass, light, value);
	}
};

class cEditPosition : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditPosition> Ref;
	cEditPosition(gdeWPSOCLight &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(const decVector &vector, gdeObjectClass *objectClass,
	gdeOCLight *light){
		if(light->GetPosition().IsEqualTo(vector)){
			return {};
		}
		return gdeUOCLightSetPosition::Ref::New(objectClass, light, vector);
	}
};

class cEditRotation : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditRotation> Ref;
	cEditRotation(gdeWPSOCLight &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(const decVector &vector, gdeObjectClass *objectClass,
	gdeOCLight *light){
		if(light->GetRotation().IsEqualTo(vector)){
			return {};
		}
		return gdeUOCLightSetRotation::Ref::New(objectClass, light, vector);
	}
};

class cTextBoneName : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextBoneName> Ref;
	cTextBoneName(gdeWPSOCLight &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCLight *light){
		if(light->GetBoneName() == textField.GetText()){
			return {};
		}
		return gdeUOCLightSetBoneName::Ref::New(objectClass, light, textField.GetText());
	}
};

class cTextRange : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextRange> Ref;
	cTextRange(gdeWPSOCLight &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCLight *light){
		const float value = textField.GetFloat();
		if(fabsf(light->GetRange() - value) < FLOAT_SAFE_EPSILON){
			return {};
		}
		return gdeUOCLightSetRange::Ref::New(objectClass, light, value);
	}
};

class cTextHalfIntensityDistance : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextHalfIntensityDistance> Ref;
	cTextHalfIntensityDistance(gdeWPSOCLight &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCLight *light){
		const float value = textField.GetFloat();
		if(fabsf(light->GetHalfIntensityDistance() - value) < FLOAT_SAFE_EPSILON){
			return {};
		}
		return gdeUOCLightSetHalfIntDist::Ref::New(objectClass, light, value);
	}
};

class cTextSpotAngle : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextSpotAngle> Ref;
	cTextSpotAngle(gdeWPSOCLight &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCLight *light){
		const float value = textField.GetFloat();
		if(fabsf(light->GetSpotAngle() - value) < FLOAT_SAFE_EPSILON){
			return {};
		}
		return gdeUOCLightSetSpotAngle::Ref::New(objectClass, light, value);
	}
};

class cTextSpotRatio : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextSpotRatio> Ref;
	cTextSpotRatio(gdeWPSOCLight &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCLight *light){
		const float value = textField.GetFloat();
		if(fabsf(light->GetSpotRatio() - value) < FLOAT_SAFE_EPSILON){
			return {};
		}
		return gdeUOCLightSetSpotRatio::Ref::New(objectClass, light, value);
	}
};

class cTextSpotSmoothness : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextSpotSmoothness> Ref;
	cTextSpotSmoothness(gdeWPSOCLight &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCLight *light){
		const float value = textField.GetFloat();
		if(fabsf(light->GetSpotSmoothness() - value) < FLOAT_SAFE_EPSILON){
			return {};
		}
		return gdeUOCLightSetSpotSmoothness::Ref::New(objectClass, light, value);
	}
};

class cTextSpotExponent : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextSpotExponent> Ref;
	cTextSpotExponent(gdeWPSOCLight &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCLight *light){
		const float value = textField.GetFloat();
		if(fabsf(light->GetSpotExponent() - value) < FLOAT_SAFE_EPSILON){
			return {};
		}
		return gdeUOCLightSetSpotExponent::Ref::New(objectClass, light, value);
	}
};

class cActionActivated : public cBaseAction{
public:
	typedef deTObjectReference<cActionActivated> Ref;
	
public:
	cActionActivated(gdeWPSOCLight &panel) :
	cBaseAction(panel, "Activated", "Light is activated"){}
	
	virtual igdeUndo::Ref OnActionLight(gdeObjectClass *objectClass, gdeOCLight *light){
		return gdeUOCLightToggleActivated::Ref::New(objectClass, light);
	}
};

class cActionCastShadows : public cBaseAction{
public:
	typedef deTObjectReference<cActionCastShadows> Ref;
	
public:
	cActionCastShadows(gdeWPSOCLight &panel) :
	cBaseAction(panel, "Cast Shadows", "Light casts shadows"){}
	
	virtual igdeUndo::Ref OnActionLight(gdeObjectClass *objectClass, gdeOCLight *light){
		return gdeUOCLightToggleCastShadows::Ref::New(objectClass, light);
	}
};

class cTextHintLightImportance : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextHintLightImportance> Ref;
	cTextHintLightImportance(gdeWPSOCLight &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCLight *light){
		const int value = textField.GetInteger();
		if(light->GetHintLightImportance() == value){
			return {};
		}
		return gdeUOCLightSetHintLightImportance::Ref::New(objectClass, light, value);
	}
};

class cTextHintShadowImportance : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextHintShadowImportance> Ref;
	cTextHintShadowImportance(gdeWPSOCLight &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCLight *light){
		const int value = textField.GetInteger();
		if(light->GetHintShadowImportance() == value){
			return {};
		}
		return gdeUOCLightSetHintShadowImportance::Ref::New(objectClass, light, value);
	}
};

class cComboHintMovement : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboHintMovement> Ref;
	cComboHintMovement(gdeWPSOCLight &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox &comboBox, gdeObjectClass *objectClass,
	gdeOCLight *light){
		const deLight::eMovementHints value =
			(deLight::eMovementHints)(intptr_t)comboBox.GetSelectedItem()->GetData();
		if(value == light->GetHintMovement()){
			return {};
		}
		return gdeUOCLightSetHintMovement::Ref::New(objectClass, light, value);
	}
};

class cComboHintParameter : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboHintParameter> Ref;
	cComboHintParameter(gdeWPSOCLight &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox &comboBox, gdeObjectClass *objectClass,
	gdeOCLight *light){
		const deLight::eParameterHints value =
			(deLight::eParameterHints)(intptr_t)comboBox.GetSelectedItem()->GetData();
		if(value == light->GetHintParameter()){
			return {};
		}
		return gdeUOCLightSetHintParameter::Ref::New(objectClass, light, value);
	}
};

class cTextLightSkin : public igdeEditPathListener{
	gdeWPSOCLight &pPanel;
	
public:
	typedef deTObjectReference<cTextLightSkin> Ref;
	cTextLightSkin(gdeWPSOCLight &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		gdeOCLight * const light = pPanel.GetLight();
		if(!light || light->GetLightSkinPath() == editPath->GetPath()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUOCLightSetLightSkinPath::Ref::New(
			pPanel.GetObjectClass(), light, editPath->GetPath()));
	}
};


class cComboPropertyNames : public igdeComboBoxListener{
	gdeWPSOCLight &pPanel;
	
public:
	typedef deTObjectReference<cComboPropertyNames> Ref;
	cComboPropertyNames(gdeWPSOCLight &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox*){
		if(pPanel.GetLight()){
			pPanel.UpdatePropertyName();
		}
	}
};

class cComboPropertyNameTarget : public igdeComboBoxListener{
	gdeWPSOCLight &pPanel;
	
public:
	typedef deTObjectReference<cComboPropertyNameTarget> Ref;
	cComboPropertyNameTarget(gdeWPSOCLight &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		gdeOCLight * const light = pPanel.GetLight();
		if(!light){
			return;
		}
		
		const gdeOCLight::eProperties propertyName = pPanel.GetPropertyName();
		if(light->GetPropertyName(propertyName) == comboBox->GetText()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUOCLightSetPropertyName::Ref::New(
			pPanel.GetObjectClass(), light, propertyName, comboBox->GetText()));
	}
};

class cComboTriggerNames : public igdeComboBoxListener{
	gdeWPSOCLight &pPanel;
	
public:
	typedef deTObjectReference<cComboTriggerNames> Ref;
	cComboTriggerNames(gdeWPSOCLight &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox*){
		if(pPanel.GetLight()){
			pPanel.UpdateTriggerName();
		}
	}
};

class cComboTriggerNameTarget : public igdeComboBoxListener{
	gdeWPSOCLight &pPanel;
	
public:
	typedef deTObjectReference<cComboTriggerNameTarget> Ref;
	cComboTriggerNameTarget(gdeWPSOCLight &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		if(!pPanel.GetLight()){
			return;
		}
		
		const gdeOCLight::eTriggers triggerName = pPanel.GetTriggerName();
		if(pPanel.GetLight()->GetTriggerName(triggerName) == comboBox->GetText()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUOCLightSetTriggerName::Ref::New(
			pPanel.GetObjectClass(), pPanel.GetLight(), triggerName, comboBox->GetText()));
	}
};

}



// Class gdeWPSOCLight
////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSOCLight::gdeWPSOCLight(gdeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, frameLine;
	
	pListener = gdeWPSOCLightListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	helper.GroupBox(content, groupBox, "Object Class Light:");
	
	helper.ComboBox(groupBox, "Type:", "Light type", pCBType, cComboType::Ref::New(*this));
	pCBType->AddItem("Point", nullptr, (void*)(intptr_t)deLight::eltPoint);
	pCBType->AddItem("Spot", nullptr, (void*)(intptr_t)deLight::eltSpot);
	pCBType->AddItem("Projector", nullptr, (void*)(intptr_t)deLight::eltProjector);
	
	helper.ColorBox(groupBox, "Color:", "Light color", pClrColor, cColorLight::Ref::New(*this));
	helper.EditFloat(groupBox, "Intensity:", "Light intensity",
		pEditIntensity, cTextIntensity::Ref::New(*this));
	helper.EditFloat(groupBox, "Ambient Ratio:", "Light ambient ratio",
		pEditAmbientRatio, cTextAmbientRatio::Ref::New(*this));
	helper.EditVector(groupBox, "Position:", "Position relative to object class",
		pEditPosition, cEditPosition::Ref::New(*this));
	helper.EditVector(groupBox, "Rotation:", "Rotation in degrees relative to object class", 4, 1,
		pEditRotation, cEditRotation::Ref::New(*this));
	helper.EditString(groupBox, "Bone:", "Bone name or empty string if not used",
		pEditBoneName, cTextBoneName::Ref::New(*this));
	helper.EditFloat(groupBox, "Range:", "Light range in meters", 4, 1,
		pEditRange, cTextRange::Ref::New(*this));
	helper.EditFloat(groupBox, "Half Intensity:",
		"half intensity distance relative to light range (0 to 1)",
		pEditHalfIntensityDistance, cTextHalfIntensityDistance::Ref::New(*this));
	helper.EditFloat(groupBox, "Spot Angle:", "Outer spot angle in degrees", 4, 1,
		pEditSpotAngle, cTextSpotAngle::Ref::New(*this));
	helper.EditFloat(groupBox, "Spot Ratio:", "Spot ratio as height divided by width",
		pEditSpotRatio, cTextSpotRatio::Ref::New(*this));
	helper.EditFloat(groupBox, "Spot Smoothness:",
		"Spot smoothess as fraction from outer spot angle where inner spot angle starts",
		pEditSpotSmoothness, cTextSpotSmoothness::Ref::New(*this));
	helper.EditFloat(groupBox, "Spot exponent:", "Spot exponent",
		pEditSpotExponent, cTextSpotExponent::Ref::New(*this));
	helper.CheckBox(groupBox, pChkActivated, cActionActivated::Ref::New(*this));
	helper.CheckBox(groupBox, pChkCastShadows, cActionCastShadows::Ref::New(*this));
	helper.EditPath(groupBox, "Light Skin:", "Path to light skin",
		igdeEnvironment::efpltSkin, pEditLightSkin, cTextLightSkin::Ref::New(*this));
	
	// hints
	helper.GroupBox(content, groupBox, "Hints:", true);
	helper.EditInteger(groupBox, "Light Importance:", "Light importance in percentage from 0 to 100",
		pEditHintLightImportance, cTextHintLightImportance::Ref::New(*this));
	helper.EditInteger(groupBox, "Shadow Importance:", "Shadow importance in percentage from 0 to 100",
		pEditHintShadowImportance, cTextHintShadowImportance::Ref::New(*this));
	
	helper.ComboBox(groupBox, "Movement:", "Light movement hint",
		pCBHintMovement, cComboHintMovement::Ref::New(*this));
	pCBHintMovement->AddItem("Static", nullptr, (void*)(intptr_t)deLight::emhStationary);
	pCBHintMovement->AddItem("Jittering", nullptr, (void*)(intptr_t)deLight::emhJittering);
	pCBHintMovement->AddItem("Dynamic", nullptr, (void*)(intptr_t)deLight::emhDynamic);
	
	helper.ComboBox(groupBox, "Parameter:", "Light parameter hint",
		pCBHintParameter, cComboHintParameter::Ref::New(*this));
	pCBHintParameter->AddItem("Static", nullptr, (void*)(intptr_t)deLight::ephStatic);
	pCBHintParameter->AddItem("Activation", nullptr, (void*)(intptr_t)deLight::ephActivation);
	pCBHintParameter->AddItem("Flicker", nullptr, (void*)(intptr_t)deLight::ephFlicker);
	pCBHintParameter->AddItem("Dynamic", nullptr, (void*)(intptr_t)deLight::ephDynamic);
	
	// property targets
	helper.GroupBox(content, groupBox, "Properties:");
	helper.ComboBox(groupBox, "Property:", "Property to set target for",
		pCBPropertyNames, cComboPropertyNames::Ref::New(*this));
	pCBPropertyNames->AddItem("Type", nullptr, (void*)(intptr_t)gdeOCLight::epType);
	pCBPropertyNames->AddItem("Color", nullptr, (void*)(intptr_t)gdeOCLight::epColor);
	pCBPropertyNames->AddItem("Intensity", nullptr, (void*)(intptr_t)gdeOCLight::epIntensity);
	pCBPropertyNames->AddItem("Ambient ratio", nullptr, (void*)(intptr_t)gdeOCLight::epAmbientRatio);
	pCBPropertyNames->AddItem("Range", nullptr, (void*)(intptr_t)gdeOCLight::epRange);
	pCBPropertyNames->AddItem("Half intensity distance", nullptr, (void*)(intptr_t)gdeOCLight::epHalfIntDist);
	pCBPropertyNames->AddItem("Spot Angle", nullptr, (void*)(intptr_t)gdeOCLight::epSpotAngle);
	pCBPropertyNames->AddItem("Spot Ratio", nullptr, (void*)(intptr_t)gdeOCLight::epSpotRatio);
	pCBPropertyNames->AddItem("Spot Smoothness", nullptr, (void*)(intptr_t)gdeOCLight::epSpotSmoothness);
	pCBPropertyNames->AddItem("Spot exponent", nullptr, (void*)(intptr_t)gdeOCLight::epSpotExponent);
	pCBPropertyNames->AddItem("Light skin", nullptr, (void*)(intptr_t)gdeOCLight::epLightSkin);
	pCBPropertyNames->AddItem("Activated", nullptr, (void*)(intptr_t)gdeOCLight::epActivated);
	pCBPropertyNames->AddItem("Cast shadows", nullptr, (void*)(intptr_t)gdeOCLight::epCastShadows);
	pCBPropertyNames->AddItem("Hint light importance", nullptr, (void*)(intptr_t)gdeOCLight::epHintLightImportance);
	pCBPropertyNames->AddItem("Hint shadow importance", nullptr, (void*)(intptr_t)gdeOCLight::epHintShadowImportance);
	pCBPropertyNames->AddItem("Attach position", nullptr, (void*)(intptr_t)gdeOCLight::epAttachPosition);
	pCBPropertyNames->AddItem("Attach rotation", nullptr, (void*)(intptr_t)gdeOCLight::epAttachRotation);
	
	helper.ComboBoxFilter(groupBox, "Target:", true, "Object class property to target",
		pCBPropertyNameTarget, cComboPropertyNameTarget::Ref::New(*this));
	pCBPropertyNameTarget->SetEditable(true);
	pCBPropertyNameTarget->SetDefaultSorter();
	pCBPropertyNameTarget->SetFilterCaseInsentive(true);
	
	// trigger targets
	helper.GroupBox(content, groupBox, "Triggers:");
	helper.ComboBox(groupBox, "Trigger:", "Trigger to set target for",
		pCBTriggerNames, cComboTriggerNames::Ref::New(*this));
	pCBTriggerNames->AddItem("Activated", nullptr, (void*)(intptr_t)gdeOCLight::etActivated);
	
	helper.ComboBoxFilter(groupBox, "Target:", true, "Object class property to target",
		pCBTriggerNameTarget, cComboTriggerNameTarget::Ref::New(*this));
	pCBTriggerNameTarget->SetEditable(true);
	pCBTriggerNameTarget->SetDefaultSorter();
	pCBTriggerNameTarget->SetFilterCaseInsentive(true);
}

gdeWPSOCLight::~gdeWPSOCLight(){
	SetGameDefinition(nullptr);
}



// Management
///////////////

void gdeWPSOCLight::SetGameDefinition(gdeGameDefinition *gameDefinition){
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
	UpdateLight();
}



gdeObjectClass *gdeWPSOCLight::GetObjectClass() const{
	return pGameDefinition ? pGameDefinition->GetActiveObjectClass() : nullptr;
}

gdeOCLight *gdeWPSOCLight::GetLight() const{
	const gdeObjectClass * const objectClass = GetObjectClass();
	return objectClass ? pGameDefinition->GetActiveOCLight() : nullptr;
}

const gdeOCLight::eProperties gdeWPSOCLight::GetPropertyName() const{
	return (gdeOCLight::eProperties)(intptr_t)pCBPropertyNames->GetSelectedItem()->GetData();
}

const gdeOCLight::eTriggers gdeWPSOCLight::GetTriggerName() const{
	return (gdeOCLight::eTriggers)(intptr_t)pCBTriggerNames->GetSelectedItem()->GetData();
}



void gdeWPSOCLight::UpdatePropertyList(){
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

void gdeWPSOCLight::UpdateLight(){
	const gdeOCLight * const light = GetLight();
	
	if(light){
		pCBType->SetSelectionWithData((void*)(intptr_t)light->GetType());
		pClrColor->SetColor(light->GetColor());
		pEditIntensity->SetFloat(light->GetIntensity());
		pEditPosition->SetVector(light->GetPosition());
		pEditRotation->SetVector(light->GetRotation());
		pEditBoneName->SetText(light->GetBoneName());
		pEditRange->SetFloat(light->GetRange());
		pEditHalfIntensityDistance->SetFloat(light->GetHalfIntensityDistance());
		pEditAmbientRatio->SetFloat(light->GetAmbientRatio());
		pEditSpotAngle->SetFloat(light->GetSpotAngle());
		pEditSpotRatio->SetFloat(light->GetSpotRatio());
		pEditSpotSmoothness->SetFloat(light->GetSpotSmoothness());
		pEditSpotExponent->SetFloat(light->GetSpotExponent());
		pChkActivated->SetChecked(light->GetActivated());
		pChkCastShadows->SetChecked(light->GetCastShadows());
		pEditHintLightImportance->SetInteger(light->GetHintLightImportance());
		pEditHintShadowImportance->SetInteger(light->GetHintShadowImportance());
		pCBHintMovement->SetSelectionWithData((void*)(intptr_t)light->GetHintMovement());
		pCBHintParameter->SetSelectionWithData((void*)(intptr_t)light->GetHintParameter());
		pEditLightSkin->SetPath(light->GetLightSkinPath());
		
	}else{
		pCBType->SetSelectionWithData((void*)(intptr_t)deLight::eltSpot);
		pClrColor->SetColor(decColor(1.0f, 1.0f, 1.0f));
		pEditIntensity->ClearText();
		pEditPosition->SetVector(decVector());
		pEditRotation->SetVector(decVector());
		pEditBoneName->ClearText();
		pEditRange->ClearText();
		pEditHalfIntensityDistance->ClearText();
		pEditAmbientRatio->ClearText();
		pEditSpotAngle->ClearText();
		pEditSpotRatio->ClearText();
		pEditSpotSmoothness->ClearText();
		pEditSpotExponent->ClearText();
		pChkActivated->SetChecked(false);
		pChkCastShadows->SetChecked(false);
		pEditHintLightImportance->ClearText();
		pEditHintShadowImportance->ClearText();
		pCBHintMovement->SetSelectionWithData((void*)(intptr_t)deLight::emhStationary);
		pCBHintParameter->SetSelectionWithData((void*)(intptr_t)deLight::ephStatic);
		pEditLightSkin->ClearPath();
	}
	
	const bool enabled = light;
	pCBType->SetEnabled(enabled);
	pClrColor->SetEnabled(enabled);
	pEditIntensity->SetEnabled(enabled);
	pEditPosition->SetEnabled(enabled);
	pEditRotation->SetEnabled(enabled);
	pEditBoneName->SetEnabled(enabled);
	pEditRange->SetEnabled(enabled);
	pEditHalfIntensityDistance->SetEnabled(enabled);
	pEditAmbientRatio->SetEnabled(enabled);
	pEditSpotAngle->SetEnabled(enabled);
	pEditSpotRatio->SetEnabled(enabled);
	pEditSpotSmoothness->SetEnabled(enabled);
	pEditSpotExponent->SetEnabled(enabled);
	pChkActivated->SetEnabled(enabled);
	pChkCastShadows->SetEnabled(enabled);
	pEditHintLightImportance->SetEnabled(enabled);
	pEditHintShadowImportance->SetEnabled(enabled);
	pCBHintMovement->SetEnabled(enabled);
	pCBHintParameter->SetEnabled(enabled);
	pEditLightSkin->SetEnabled(enabled);
	
	UpdatePropertyName();
	UpdateTriggerName();
}

void gdeWPSOCLight::UpdatePropertyName(){
	const gdeOCLight * const light = GetLight();
	
	if(light){
		pCBPropertyNameTarget->SetText(light->GetPropertyName(GetPropertyName()));
		
	}else{
		pCBPropertyNameTarget->ClearText();
	}
	
	pCBPropertyNameTarget->SetEnabled(light);
}

void gdeWPSOCLight::UpdateTriggerName(){
	const gdeOCLight * const light = GetLight();
	
	if(light){
		pCBTriggerNameTarget->SetText(light->GetTriggerName(GetTriggerName()));
		
	}else{
		pCBTriggerNameTarget->ClearText();
	}
	
	pCBTriggerNameTarget->SetEnabled(light);
}
