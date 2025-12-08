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

#include "peeWPType.h"
#include "peeWPTypeListener.h"
#include "peeWindowProperties.h"
#include "../peeWindowMain.h"
#include "../../emitter/peeEmitter.h"
#include "../../emitter/peeType.h"
#include "../../emitter/peeController.h"
#include "../../undosys/emitter/peeUEmitterSetBurstLifetime.h"
#include "../../undosys/emitter/peeUEmitterToggleEmitBurst.h"
#include "../../undosys/type/peeUTypeAdd.h"
#include "../../undosys/type/peeUTypeMoveDown.h"
#include "../../undosys/type/peeUTypeMoveUp.h"
#include "../../undosys/type/peeUTypeRemove.h"
#include "../../undosys/type/peeUTypeSetModelPath.h"
#include "../../undosys/type/peeUTypeSetModelSkinPath.h"
#include "../../undosys/type/peeUTypeSetCastFrom.h"
#include "../../undosys/type/peeUTypeSetSimulationType.h"
#include "../../undosys/type/peeUTypeSetName.h"
#include "../../undosys/type/peeUTypeSetSkinPath.h"
#include "../../undosys/type/peeUTypeSetPhysicsSize.h"
#include "../../undosys/type/peeUTypeSetCollisionResponse.h"
#include "../../undosys/type/peeUTypeSetPathCollisionEmitter.h"
#include "../../undosys/type/peeUTypeSetPathTrailEmitter.h"
#include "../../undosys/type/peeUTypeSetEmitMinImpulse.h"
#include "../../undosys/type/peeUTypeSetEmitController.h"
#include "../../undosys/type/peeUTypeSetTrailController.h"
#include "../../undosys/type/peeUTypeSetIntervalAsDistance.h"
#include "../../undosys/parameter/peeUParameterSetValue.h"
#include "../../undosys/parameter/peeUParameterSetSpread.h"
#include "../../undosys/parameter/peeUParameterSetControllerValue.h"
#include "../../undosys/parameter/peeUParameterSetControllerSpread.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/igdeLabel.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerBorder.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionSelectFile.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>


// Events
///////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	peeWPType &pPanel;
	
public:
	cBaseTextFieldListener(peeWPType &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		peeEmitter * const emitter = pPanel.GetEmitter();
		peeType * const type = pPanel.GetType();
		if(!emitter || !type){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(textField, emitter, type)));
		if(undo){
			emitter->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, peeEmitter *emitter, peeType *type) = 0;
};

class cBaseAction : public igdeAction{
protected:
	peeWPType &pPanel;
	
public:
	cBaseAction(peeWPType &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	virtual void OnAction(){
		peeEmitter * const emitter = pPanel.GetEmitter();
		peeType * const type = pPanel.GetType();
		if(!emitter || !type){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnAction(emitter, type)));
		if(undo){
			emitter->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnAction(peeEmitter *emitter, peeType *type) = 0;
};

class cBasePathListener : public igdeEditPathListener{
protected:
	peeWPType &pPanel;
	
public:
	cBasePathListener(peeWPType &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		peeEmitter * const emitter = pPanel.GetEmitter();
		peeType * const type = pPanel.GetType();
		if(!emitter || !type){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(editPath->GetPath(), emitter, type)));
		if(undo){
			emitter->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(const decString &path, peeEmitter *emitter, peeType *type) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	peeWPType &pPanel;
	
public:
	cBaseComboBoxListener(peeWPType &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		peeEmitter * const emitter = pPanel.GetEmitter();
		peeType * const type = pPanel.GetType();
		if(!emitter || !type){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(comboBox, emitter, type)));
		if(undo){
			emitter->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeComboBox *comboBox, peeEmitter *emitter, peeType *type) = 0;
};



class cActionEmitBurst : public igdeAction{
	peeWPType &pPanel;
public:
	cActionEmitBurst(peeWPType &panel) : igdeAction("Emit Burst", NULL,
		"Determines if particles are emit as burst or continuous"),
	pPanel(panel){}
	
	virtual void OnAction(){
		peeEmitter * const emitter = pPanel.GetEmitter();
		if(!emitter){
			return;
		}
		
		emitter->GetUndoSystem()->Add(peeUEmitterToggleEmitBurst::Ref::NewWith(emitter));
	}
};

class cTextBurstLifetime : public igdeTextFieldListener{
	peeWPType &pPanel;
public:
	cTextBurstLifetime(peeWPType &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		peeEmitter * const emitter = pPanel.GetEmitter();
		if(!emitter){
			return;
		}
		
		const float value = textField->GetFloat();
		if(fabsf(value - emitter->GetBurstLifetime()) < FLOAT_SAFE_EPSILON){
			return;
		}
		
		emitter->GetUndoSystem()->Add(peeUEmitterSetBurstLifetime::Ref::NewWith(emitter, value));
	}
};



class cComboType : public igdeComboBoxListener{
	peeWPType &pPanel;
public:
	cComboType(peeWPType &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox * comboBox){
		peeEmitter * const emitter = pPanel.GetEmitter();
		if(!emitter){
			return;
		}
		
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		emitter->SetActiveType(selection ? (peeType*)selection->GetData() : NULL);
	}
};

class cActionType : public igdeAction{
	peeWPType &pPanel;
	igdeButton::Ref &pButton;
public:
	cActionType(peeWPType &panel, igdeButton::Ref &button) :
	igdeAction("", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown),
		"Edit type menu"),
	pPanel(panel), pButton(button){}
	
	virtual void OnAction(){
		peeEmitter * const emitter = pPanel.GetEmitter();
		if(!emitter){
			return;
		}
		
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelperProperties();
		igdeMenuCascade::Ref menu(igdeMenuCascade::Ref::NewWith(pPanel.GetEnvironment()));
		helper.MenuCommand(menu, pPanel.GetActionTypeAdd());
		helper.MenuCommand(menu, pPanel.GetActionTypeRemove());
		helper.MenuCommand(menu, pPanel.GetActionTypeRename());
		menu->PopupBottom(pButton);
	}
};

class cActionTypeAdd : public igdeAction{
	peeWPType &pPanel;
public:
	cActionTypeAdd(peeWPType &panel) :
	igdeAction("Add...", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"Add type"),
	pPanel(panel){}
	
	virtual void OnAction(){
		peeEmitter * const emitter = pPanel.GetEmitter();
		if(!emitter){
			return;
		}
		
		decString name("Type");
		
		while(igdeCommonDialogs::GetString(&pPanel, "Add Type", "Name:", name)){
			if(emitter->GetTypeList().HasNamed(name)){
				igdeCommonDialogs::Error(&pPanel, "Add Type", "A type with this name exists already.");
				continue;
			}
			
			const peeType::Ref type(peeType::Ref::NewWith(emitter->GetEngine(), name));
			
			emitter->GetUndoSystem()->Add(peeUTypeAdd::Ref::NewWith(emitter, type));
			return;
		}
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetEmitter() != NULL);
	}
};

class cActionTypeRemove : public cBaseAction{
public:
	cActionTypeRemove(peeWPType &panel) :
	cBaseAction(panel, "Remove", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove selected type"){}
	
	virtual igdeUndo *OnAction(peeEmitter*, peeType *type){
		return new peeUTypeRemove(type);
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetType() != NULL);
	}
};

class cActionTypeRename : public cBaseAction{
public:
	cActionTypeRename(peeWPType &panel) :
	cBaseAction(panel, "Rename...", NULL, "Rename selected type"){}
	
	virtual igdeUndo *OnAction(peeEmitter *emitter, peeType *type){
		decString name(type->GetName());
		
		while(igdeCommonDialogs::GetString(&pPanel, "Rename Type", "Name:", name)){
			if(name == type->GetName()){
				break;
				
			}else if(emitter->GetTypeList().HasNamed(name)){
				igdeCommonDialogs::Error(&pPanel, "Rename Type", "A type with this name exists already.");
				
			}else{
				return new peeUTypeSetName(type, name);
			}
		}
		
		return NULL;
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetType() != NULL);
	}
};

class cPathSkin : public cBasePathListener{
public:
	cPathSkin(peeWPType &panel) : cBasePathListener(panel){}
	
	virtual igdeUndo *OnChanged(const decString &path, peeEmitter*, peeType *type){
		if(type->GetSkinPath() == path){
			return NULL;
		}
		return new peeUTypeSetSkinPath(type, path);
	}
};

class cPathModel : public cBasePathListener{
public:
	cPathModel(peeWPType &panel) : cBasePathListener(panel){}
	
	virtual igdeUndo *OnChanged(const decString &path, peeEmitter*, peeType *type){
		if(type->GetModelPath() == path){
			return NULL;
		}
		return new peeUTypeSetModelPath(type, path);
	}
};

class cPathModelSkin : public cBasePathListener{
public:
	cPathModelSkin(peeWPType &panel) : cBasePathListener(panel){}
	
	virtual igdeUndo *OnChanged(const decString &path, peeEmitter*, peeType *type){
		if(type->GetModelSkinPath() == path){
			return NULL;
		}
		return new peeUTypeSetModelSkinPath(type, path);
	}
};

class cComboCastFrom : public cBaseComboBoxListener{
public:
	cComboCastFrom(peeWPType &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeComboBox *comboBox, peeEmitter*, peeType *type){
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if(!selection){
			DETHROW(deeInvalidParam);
		}
		
		const deParticleEmitterType::eCastFrom castFrom =
			(deParticleEmitterType::eCastFrom)(intptr_t)selection->GetData();
		if(type->GetCastFrom() == castFrom){
			return NULL;
		}
		return new peeUTypeSetCastFrom(type, castFrom);
	}
};

class cComboSimType : public cBaseComboBoxListener{
public:
	cComboSimType(peeWPType &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeComboBox *comboBox, peeEmitter*, peeType *type){
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if(!selection){
			DETHROW(deeInvalidParam);
		}
		
		const deParticleEmitterType::eSimulationTypes simulationType =
			(deParticleEmitterType::eSimulationTypes)(intptr_t)selection->GetData();
		if(type->GetSimulationType() == simulationType){
			return NULL;
		}
		return new peeUTypeSetSimulationType(type, simulationType);
	}
};

class cActionIntervalAsDistance : public cBaseAction{
public:
	cActionIntervalAsDistance(peeWPType &panel) :
	cBaseAction(panel, "Interval As Distance", NULL,
		"Determines if the interval is used with distance instead of time"){ }
	
	virtual igdeUndo *OnAction(peeEmitter*, peeType *type){
		return new peeUTypeSetIntervalAsDistance(type);
	}
};

class cTextPhysicsSize : public cBaseTextFieldListener{
public:
	cTextPhysicsSize(peeWPType &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, peeEmitter*, peeType *type){
		const float value = textField->GetFloat();
		if(fabsf(value - type->GetPhysicsSize()) < FLOAT_SAFE_EPSILON){
			return NULL;
		}
		return new peeUTypeSetPhysicsSize(type, value);
	}
};


class cPathTrailEmitter : public cBasePathListener{
public:
	cPathTrailEmitter(peeWPType &panel) : cBasePathListener(panel){}
	
	virtual igdeUndo *OnChanged(const decString &path, peeEmitter*, peeType *type){
		if(type->GetPathTrailEmitter() == path){
			return NULL;
		}
		return new peeUTypeSetPathTrailEmitter(type, path);
	}
};

class cComboTrailController : public cBaseComboBoxListener{
public:
	cComboTrailController(peeWPType &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeComboBox *comboBox, peeEmitter*, peeType *type){
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if(!selection){
			DETHROW(deeInvalidParam);
		}
		
		type->SetActiveTrailController(
			(deParticleEmitterType::eEmitControllers)(intptr_t)selection->GetData());
		return NULL;
	}
};

class cTextTrailTargetController : public cBaseTextFieldListener{
	igdeComboBox &pComboBox;
public:
	cTextTrailTargetController(peeWPType &panel, igdeComboBox &comboBox) :
	cBaseTextFieldListener(panel),
	pComboBox(comboBox){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, peeEmitter*, peeType *type){
		const igdeListItem * const selection = pComboBox.GetSelectedItem();
		if(!selection){
			DETHROW(deeInvalidParam);
		}
		
		const deParticleEmitterType::eEmitControllers controller =
			(deParticleEmitterType::eEmitControllers)(intptr_t)selection->GetData();
		return type->GetTrailController(controller) != textField->GetText()
			? new peeUTypeSetTrailController(type, controller, textField->GetText()) : NULL;
	}
};


class cComboCollisionResponse : public cBaseComboBoxListener{
public:
	cComboCollisionResponse(peeWPType &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeComboBox *comboBox, peeEmitter*, peeType *type){
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if(!selection){
			DETHROW(deeInvalidParam);
		}
		
		const deParticleEmitterType::eCollisionResponses response =
			(deParticleEmitterType::eCollisionResponses)(intptr_t)selection->GetData();
		if(response == type->GetCollisionResponse()){
			return NULL;
		}
		
		return new peeUTypeSetCollisionResponse(type, response);
	}
};

class cPathCollisionEmitter : public cBasePathListener{
public:
	cPathCollisionEmitter(peeWPType &panel) : cBasePathListener(panel){}
	
	virtual igdeUndo *OnChanged(const decString &path, peeEmitter*, peeType *type){
		if(type->GetPathCollisionEmitter() == path){
			return NULL;
		}
		return new peeUTypeSetPathCollisionEmitter(type, path);
	}
};

class cTextEmitMinImpulse : public cBaseTextFieldListener{
public:
	cTextEmitMinImpulse(peeWPType &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, peeEmitter*, peeType *type){
		const float value = textField->GetFloat();
		if(fabsf(value - type->GetEmitMinImpulse()) < FLOAT_SAFE_EPSILON){
			return NULL;
		}
		return new peeUTypeSetEmitMinImpulse(type, value);
	}
};

class cComboEmitController : public cBaseComboBoxListener{
public:
	cComboEmitController(peeWPType &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeComboBox *comboBox, peeEmitter*, peeType *type){
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if(!selection){
			DETHROW(deeInvalidParam);
		}
		
		type->SetActiveEmitController(
			(deParticleEmitterType::eEmitControllers)(intptr_t)selection->GetData());
		return NULL;
	}
};

class cTextEmitTargetController : public cBaseTextFieldListener{
	igdeComboBox &pComboBox;
public:
	cTextEmitTargetController(peeWPType &panel, igdeComboBox &comboBox) :
	cBaseTextFieldListener(panel),
	pComboBox(comboBox){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, peeEmitter*, peeType *type){
		const igdeListItem * const selection = pComboBox.GetSelectedItem();
		if(!selection){
			DETHROW(deeInvalidParam);
		}
		
		const deParticleEmitterType::eEmitControllers controller =
			(deParticleEmitterType::eEmitControllers)(intptr_t)selection->GetData();
		return type->GetEmitController(controller) != textField->GetText()
			? new peeUTypeSetEmitController(type, controller, textField->GetText()) : NULL;
	}
};


class cListTypes : public igdeListBoxListener{
	peeWPType &pPanel;
public:
	cListTypes(peeWPType &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeListBox *listBox){
		peeType * const type = pPanel.GetType();
		if(!type){
			return;
		}
		
		const igdeListItem * const selection = listBox->GetSelectedItem();
		if(!selection){
			DETHROW(deeInvalidParam);
		}
		
		type->SetActiveParameter(type->GetParameterAt(
			(deParticleEmitterType::eParameters)(intptr_t)selection->GetData()));
	}
};


class cTextParamValue : public cBaseTextFieldListener{
public:
	cTextParamValue(peeWPType &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, peeEmitter*, peeType *type){
		peeParameter * const parameter = type->GetActiveParameter();
		const float value = textField->GetFloat();
		if(fabsf(value - parameter->GetValue()) < FLOAT_SAFE_EPSILON){
			return NULL;
		}
		return new peeUParameterSetValue(type, parameter, value);
	}
};

class cTextParamSpread : public cBaseTextFieldListener{
public:
	cTextParamSpread(peeWPType &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, peeEmitter*, peeType *type){
		peeParameter * const parameter = type->GetActiveParameter();
		const float value = textField->GetFloat();
		if(fabsf(value - parameter->GetSpread()) < FLOAT_SAFE_EPSILON){
			return NULL;
		}
		return new peeUParameterSetSpread(type, parameter, value);
	}
};

class cComboControllerValue : public cBaseComboBoxListener{
	bool &pPreventUpdate;
public:
	cComboControllerValue(peeWPType &panel, bool &preventUpdate) :
	cBaseComboBoxListener(panel), pPreventUpdate(preventUpdate){}
	
	virtual igdeUndo *OnChanged(igdeComboBox *comboBox, peeEmitter*, peeType *type){
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if(!selection || pPreventUpdate){
			return NULL;
		}
		
		peeController * const controller = (peeController*)selection->GetData();
		peeParameter * const parameter = type->GetActiveParameter();
		if(controller == parameter->GetControllerValue()){
			return NULL;
		}
		
		return new peeUParameterSetControllerValue(type, parameter, controller);
	}
};

class cComboControllerSpread : public cBaseComboBoxListener{
	bool &pPreventUpdate;
public:
	cComboControllerSpread(peeWPType &panel, bool &preventUpdate) :
	cBaseComboBoxListener(panel), pPreventUpdate(preventUpdate){}
	
	virtual igdeUndo *OnChanged(igdeComboBox *comboBox, peeEmitter*, peeType *type){
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if(!selection || pPreventUpdate){
			return NULL;
		}
		
		peeController * const controller = (peeController*)selection->GetData();
		peeParameter * const parameter = type->GetActiveParameter();
		if(controller == parameter->GetControllerSpread()){
			return NULL;
		}
		
		return new peeUParameterSetControllerSpread(type, parameter, controller);
	}
};

}



// Helper methods
///////////////////

static void AddControllersToComboBox(igdeComboBox &comboBox){
	comboBox.AddItem("Lifetime", NULL,
		(void*)(intptr_t)deParticleEmitterType::eecLifetime);
	comboBox.AddItem("Mass", NULL,
		(void*)(intptr_t)deParticleEmitterType::eecMass);
	comboBox.AddItem("Linear Velocity", NULL,
		(void*)(intptr_t)deParticleEmitterType::eecLinearVelocity);
	comboBox.AddItem("Angular Velocity", NULL,
		(void*)(intptr_t)deParticleEmitterType::eecAngularVelocity);
}

static void AddParametersToListBox(igdeListBox &listBox, igdeIcon *icon){
	listBox.AddItem("Time to live", icon,
		(void*)(intptr_t)deParticleEmitterType::epTimeToLive);
	listBox.AddItem("Interval", icon,
		(void*)(intptr_t)deParticleEmitterType::epInterval);
	listBox.AddItem("Particle Count", icon,
		(void*)(intptr_t)deParticleEmitterType::epParticleCount);
	listBox.AddItem("Cast Angle X", icon,
		(void*)(intptr_t)deParticleEmitterType::epCastAngleX);
	listBox.AddItem("Cast Angle Y", icon,
		(void*)(intptr_t)deParticleEmitterType::epCastAngleY);
	listBox.AddItem("Size", icon,
		(void*)(intptr_t)deParticleEmitterType::epSize);
	listBox.AddItem("Red", icon,
		(void*)(intptr_t)deParticleEmitterType::epRed);
	listBox.AddItem("Green", icon,
		(void*)(intptr_t)deParticleEmitterType::epGreen);
	listBox.AddItem("Blue", icon,
		(void*)(intptr_t)deParticleEmitterType::epBlue);
	listBox.AddItem("Transparency", icon,
		(void*)(intptr_t)deParticleEmitterType::epTransparency);
	listBox.AddItem("Emissivity", icon,
		(void*)(intptr_t)deParticleEmitterType::epEmissivity);
	listBox.AddItem("Mass", icon,
		(void*)(intptr_t)deParticleEmitterType::epMass);
	listBox.AddItem("Rotation", icon,
		(void*)(intptr_t)deParticleEmitterType::epRotation);
	listBox.AddItem("Linear Velocity", icon,
		(void*)(intptr_t)deParticleEmitterType::epLinearVelocity);
	listBox.AddItem("Angular Velocity", icon,
		(void*)(intptr_t)deParticleEmitterType::epAngularVelocity);
	listBox.AddItem("Brown Motion", icon,
		(void*)(intptr_t)deParticleEmitterType::epBrownMotion);
	listBox.AddItem("Damping", icon,
		(void*)(intptr_t)deParticleEmitterType::epDamping);
	listBox.AddItem("Drag", icon,
		(void*)(intptr_t)deParticleEmitterType::epDrag);
	listBox.AddItem("Gravity X", icon,
		(void*)(intptr_t)deParticleEmitterType::epGravityX);
	listBox.AddItem("Gravity Y", icon,
		(void*)(intptr_t)deParticleEmitterType::epGravityY);
	listBox.AddItem("Gravity Z", icon,
		(void*)(intptr_t)deParticleEmitterType::epGravityZ);
	listBox.AddItem("Local Gravity", icon,
		(void*)(intptr_t)deParticleEmitterType::epLocalGravity);
	listBox.AddItem("Force Field Direct", icon,
		(void*)(intptr_t)deParticleEmitterType::epForceFieldDirect);
	listBox.AddItem("Force Field Surface", icon,
		(void*)(intptr_t)deParticleEmitterType::epForceFieldSurface);
	listBox.AddItem("Force Field Mass", icon,
		(void*)(intptr_t)deParticleEmitterType::epForceFieldMass);
	listBox.AddItem("Force Field Speed", icon,
		(void*)(intptr_t)deParticleEmitterType::epForceFieldSpeed);
	listBox.AddItem("Elasticity", icon,
		(void*)(intptr_t)deParticleEmitterType::epElasticity);
	listBox.AddItem("Roughness", icon,
		(void*)(intptr_t)deParticleEmitterType::epRoughness);
	listBox.AddItem("Emit Direction", icon,
		(void*)(intptr_t)deParticleEmitterType::epEmitDirection);
}



// Class peeWPType
////////////////////

// Constructor, destructor
////////////////////////////

peeWPType::peeWPType(peeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties),
pListener(NULL),
pEmitter(NULL),
pPreventUpdate(false)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainer::Ref content, groupBox, form, frameLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener.TakeOver(new peeWPTypeListener(*this));
	
	content.TakeOver(new igdeContainerFlow(env, igdeContainerFlow::eaY));
	AddChild(content);
	
	pIconUnused = env.GetStockIcon(igdeEnvironment::esiSmallMinus);
	pIconUsed = env.GetStockIcon(igdeEnvironment::esiSmallPlus);
	
	pActionTypeAdd.TakeOver(new cActionTypeAdd(*this));
	pActionTypeRemove.TakeOver(new cActionTypeRemove(*this));
	pActionTypeRename.TakeOver(new cActionTypeRename(*this));
	
	
	// emitter settings
	helper.GroupBox(content, groupBox, "Emitter:");
	
	helper.CheckBox(groupBox, pChkEmitBurst, new cActionEmitBurst(*this), true);
	helper.EditFloat(groupBox, "Lifetime:", "Lifetime of burst",
		pEditBurstLifetime, new cTextBurstLifetime(*this));
	
	
	// type settings
	helper.GroupBox(content, groupBox, "Types:");
	
	helper.FormLineStretchFirst(groupBox, "Type:", "Type to edit", frameLine);
	helper.ComboBox(frameLine, "Types", pCBType, new cComboType(*this));
	pCBType->SetDefaultSorter();
	helper.Button(frameLine, pBtnType, new cActionType(*this, pBtnType), true);
	
	helper.EditPath(groupBox, "Skin:", "Path to the skin for the particles",
		igdeEnvironment::efpltSkin, pEditSkin, new cPathSkin(*this));
	helper.EditPath(groupBox, "Model:", "Path to the model to emit particles from",
		igdeEnvironment::efpltModel, pEditModel, new cPathModel(*this));
	helper.EditPath(groupBox, "Model Skin:", "Path to the skin for the model to emit particles from",
		igdeEnvironment::efpltSkin, pEditModelSkin, new cPathModelSkin(*this));
	
	helper.ComboBox(groupBox, "Cast From:", "Type of element to cast particles from if a model is set",
		pCBCastFrom, new cComboCastFrom(*this));
	pCBCastFrom->AddItem("Vertex", NULL, (void*)(intptr_t)deParticleEmitterType::ecfVertex);
	pCBCastFrom->AddItem("Face", NULL, (void*)(intptr_t)deParticleEmitterType::ecfFace);
	pCBCastFrom->AddItem("Volume", NULL, (void*)(intptr_t)deParticleEmitterType::ecfVolume);
	
	helper.ComboBox(groupBox, "Simulation Type:", "Type of simulation to use",
		pCBSimType, new cComboSimType(*this));
	pCBSimType->AddItem("Particle", NULL, (void*)(intptr_t)deParticleEmitterType::estParticle);
	pCBSimType->AddItem("Ribbon", NULL, (void*)(intptr_t)deParticleEmitterType::estRibbon);
	pCBSimType->AddItem("Beam", NULL, (void*)(intptr_t)deParticleEmitterType::estBeam);
	
	helper.CheckBox(groupBox, pChkIntervalAsDistance, new cActionIntervalAsDistance(*this), true);
	helper.EditFloat(groupBox, "Physics Size:", "Size of particles for physics simulation",
		pEditPhysicsSize, new cTextPhysicsSize(*this));
	
	
	// type trail
	helper.GroupBox(content, groupBox, "Trail:");
	
	helper.EditPath(groupBox, "Emitter:", "Path to the particle emitter to use as trail",
		igdeEnvironment::efpltParticleEmitter, pEditPathTrailEmitter, new cPathTrailEmitter(*this));
	
	helper.FormLineStretchFirst(groupBox, "Controller:", "Type of controller to set", frameLine);
	helper.ComboBox(frameLine, "Type of controller to set",
		pCBTrailController, new cComboTrailController(*this));
	AddControllersToComboBox(pCBTrailController);
	helper.EditString(frameLine, "Controller in the trail emitter set to this type",
		pEditTrailTargetController, new cTextTrailTargetController(*this, pCBTrailController));
	
	
	// type collision
	helper.GroupBox(content, groupBox, "Collision:");
	
	helper.ComboBox(groupBox, "Response:", "Collision response to use",
		pCBCollisionResponse, new cComboCollisionResponse(*this));
	pCBCollisionResponse->AddItem("Destroy", NULL,
		(void*)(intptr_t)deParticleEmitterType::ecrDestroy);
	pCBCollisionResponse->AddItem("Physical Response", NULL,
		(void*)(intptr_t)deParticleEmitterType::ecrPhysical);
	pCBCollisionResponse->AddItem("Custom Response", NULL,
		(void*)(intptr_t)deParticleEmitterType::ecrCustom);
	
	helper.EditPath(groupBox, "Emitter:", "Path to the particle emitter to create for impacts",
		igdeEnvironment::efpltParticleEmitter, pEditPathCollisionEmitter,
		new cPathCollisionEmitter(*this));
	helper.EditFloat(groupBox, "Min Impulse:",
		"Minimal impact impulse required to create a new collision emitter instance",
		pEditEmitMinImpulse, new cTextEmitMinImpulse(*this));
	
	helper.FormLineStretchFirst(groupBox, "Controller:", "Type of controller to set", frameLine);
	helper.ComboBox(frameLine, "Type of controller to set",
		pCBEmitController, new cComboEmitController(*this));
	AddControllersToComboBox(pCBEmitController);
	helper.EditString(frameLine, "Controller in the collision emitter set to this type",
		pEditEmitTargetController, new cTextEmitTargetController(*this, pCBEmitController));
	
	
	// parameter list
	helper.GroupBoxFlow(content, groupBox, "Parameters:");
	
	helper.ListBox(groupBox, 10, "Parameters", pListParams, new cListTypes(*this));
	AddParametersToListBox(pListParams, pIconUnused);
	
	form.TakeOver(new igdeContainerForm(env));
	groupBox->AddChild(form);
	
	helper.EditFloat(form, "Cast Range:",
		"Pick cast value randomly between range given by a value and a spread",
		pEditParamValue, new cTextParamValue(*this));
	helper.EditFloat(form, "Cast Spread:",
		"Maximum spread around the cast value", pEditParamSpread, new cTextParamSpread(*this));
	
	helper.ComboBox(form, "Controller Value:", "Controller used to sample value from value curve",
		pCBParamCtrlValue, new cComboControllerValue(*this, pPreventUpdate));
	helper.ComboBox(form, "Controller Spread:", "Controller used to sample value from spread curve",
		pCBParamCtrlSpread, new cComboControllerSpread(*this, pPreventUpdate));
}

peeWPType::~peeWPType(){
	if(pEmitter){
		pEmitter->RemoveListener(pListener);
		pEmitter = NULL;
	}
	
}



// Management
///////////////

void peeWPType::SetEmitter(peeEmitter *emitter){
	if(emitter == pEmitter){
		return;
	}
	
	if(pEmitter){
		pEmitter->RemoveListener(pListener);
	}
	
	pEmitter = emitter;
	
	if(emitter){
		emitter->AddListener(pListener);
		emitter->AddReference();
	}
	
	UpdateEmitter();
	UpdateControllerList();
	UpdateTypeList();
	SelectActiveParameter();
	OnEmitterPathChanged();
}

void peeWPType::OnEmitterPathChanged(){
	if(pEmitter){
		pEditSkin->SetBasePath(pEmitter->GetDirectoryPath());
		pEditModel->SetBasePath(pEmitter->GetDirectoryPath());
		pEditModelSkin->SetBasePath(pEmitter->GetDirectoryPath());
		pEditPathTrailEmitter->SetBasePath(pEmitter->GetDirectoryPath());
		pEditPathCollisionEmitter->SetBasePath(pEmitter->GetDirectoryPath());
		
	}else{
		pEditSkin->SetBasePath("");
		pEditModel->SetBasePath("");
		pEditModelSkin->SetBasePath("");
		pEditPathTrailEmitter->SetBasePath("");
		pEditPathCollisionEmitter->SetBasePath("");
	}
}

peeType *peeWPType::GetType() const{
	return pEmitter ? pEmitter->GetActiveType() : NULL;
}

peeParameter *peeWPType::GetParameter() const{
	const peeType * const activeType = GetType();
	return activeType ? activeType->GetActiveParameter() : NULL;
}



void peeWPType::UpdateEmitter(){
	if(pEmitter){
		pEditBurstLifetime->SetFloat(pEmitter->GetBurstLifetime());
		pChkEmitBurst->SetChecked(pEmitter->GetEmitBurst());
		
	}else{
		pEditBurstLifetime->ClearText();
		pChkEmitBurst->SetChecked(false);
	}
	
	const bool enabled = pEmitter != NULL;
	pEditBurstLifetime->SetEnabled(enabled);
	pChkEmitBurst->SetEnabled(enabled);
}

void peeWPType::UpdateTypeList(){
	peeType * const activeType = GetType();
	
	pCBType->RemoveAllItems();
	
	if(pEmitter){
		const peeTypeList &typeList = pEmitter->GetTypeList();
		const int typeCount = typeList.GetCount();
		int i;
		
		for(i=0; i<typeCount; i++){
			peeType * const type = typeList.GetAt(i);
			pCBType->AddItem(type->GetName(), NULL, type);
		}
	}
	
	pCBType->SortItems();
	
	if(activeType){
		pCBType->SetSelectionWithData(activeType);
		
	}else if(pCBType->GetItemCount() > 0){
		pEmitter->SetActiveType((peeType*)pCBType->GetItemAt(0)->GetData());
	}
	
	UpdateType();  // can be duplicate but required to not skip it in certain situations
}

void peeWPType::SelectActiveType(){
	pCBType->SetSelectionWithData(GetType());
	UpdateType();
}

void peeWPType::UpdateType(){
	const peeType * const type = GetType();
	int i;
	
	if(type){
		pEditSkin->SetPath(type->GetSkinPath());
		pEditModel->SetPath(type->GetModelPath());
		pEditModelSkin->SetPath(type->GetModelSkinPath());
		pCBCastFrom->SetSelectionWithData((void*)(intptr_t)type->GetCastFrom());
		pCBSimType->SetSelectionWithData((void*)(intptr_t)type->GetSimulationType());
		pChkIntervalAsDistance->SetChecked(type->GetIntervalAsDistance());
		pEditPathTrailEmitter->SetPath(type->GetPathTrailEmitter());
		pEditTrailTargetController->SetText(type->GetTrailController(
			(deParticleEmitterType::eEmitControllers)(intptr_t)
				pCBTrailController->GetSelectedItem()->GetData()));
		
		pEditPhysicsSize->SetFloat(type->GetPhysicsSize());
		pCBCollisionResponse->SetSelectionWithData((void*)(intptr_t)type->GetCollisionResponse());
		pEditPathCollisionEmitter->SetPath(type->GetPathCollisionEmitter());
		pEditEmitMinImpulse->SetFloat(type->GetEmitMinImpulse());
		pEditEmitTargetController->SetText(type->GetEmitController(
			(deParticleEmitterType::eEmitControllers)(intptr_t)
				pCBEmitController->GetSelectedItem()->GetData()));
		
		// update usage icons
		for(i=0; i<pListParams->GetItemCount(); i++){
			UpdateParameterUsage(*type->GetParameterAt(
				(deParticleEmitterType::eParameters)(intptr_t)pListParams->GetItemAt(i)->GetData()));
		}
		
	}else{
		pEditSkin->ClearPath();
		pEditModel->ClearPath();
		pEditModelSkin->ClearPath();
		pCBCastFrom->SetSelectionWithData((void*)(intptr_t)deParticleEmitterType::ecfVertex);
		pCBSimType->SetSelectionWithData((void*)(intptr_t)deParticleEmitterType::estParticle);
		pChkIntervalAsDistance->SetChecked(false);
		pEditPathTrailEmitter->ClearPath();
		pEditTrailTargetController->ClearText();
		
		pEditPhysicsSize->ClearText();
		pCBCollisionResponse->SetSelectionWithData((void*)(intptr_t)deParticleEmitterType::ecrPhysical);
		pEditPathCollisionEmitter->ClearPath();
		pEditEmitMinImpulse->ClearText();
		pEditEmitTargetController->ClearText();
		
		// update usage icons
		for(i=0; i<pListParams->GetItemCount(); i++){
			pListParams->GetItemAt(i)->SetIcon(pIconUnused);
		}
	}
	
	const bool enabled = type != nullptr;
	
	pEditSkin->SetEnabled(enabled);
	pEditModel->SetEnabled(enabled);
	pEditModelSkin->SetEnabled(enabled);
	pCBCastFrom->SetEnabled(enabled);
	pCBSimType->SetEnabled(enabled);
	pChkIntervalAsDistance->SetEnabled(enabled);
	pEditPhysicsSize->SetEnabled(enabled);
	pEditPathTrailEmitter->SetEnabled(enabled);
	pCBTrailController->SetEnabled(enabled);
	pEditTrailTargetController->SetEnabled(enabled);
	
	pCBCollisionResponse->SetEnabled(enabled);
	pEditPathCollisionEmitter->SetEnabled(enabled);
	pEditEmitMinImpulse->SetEnabled(enabled);
	pCBEmitController->SetEnabled(enabled);
	pEditEmitTargetController->SetEnabled(enabled);
	
	pListParams->SetEnabled(enabled);
	
	UpdateParameter();
}

void peeWPType::UpdateControllerList(){
	try{
		pPreventUpdate = true;
		
		pCBParamCtrlValue->RemoveAllItems();
		pCBParamCtrlSpread->RemoveAllItems();
		
		pCBParamCtrlValue->AddItem("< None >", NULL, NULL);
		pCBParamCtrlSpread->AddItem("< None >", NULL, NULL);
		
		pCBParamCtrlValue->SetEnabled(pEmitter != NULL);
		pCBParamCtrlSpread->SetEnabled(pEmitter != NULL);
		
		if(!pEmitter){
			return;
		}
		
		const peeControllerList &controllerList = pEmitter->GetControllers();
		const int controllerCount = controllerList.GetCount();
		decString text;
		int i;
		
		for(i=0; i<controllerCount; i++){
			peeController * const controller = controllerList.GetAt(i);
			text.Format("%d: %s", i, controller->GetName().GetString());
			
			pCBParamCtrlValue->AddItem(text, NULL, controller);
			pCBParamCtrlSpread->AddItem(text, NULL, controller);
		}
		
		pPreventUpdate = false;
		
	}catch(const deException &){
		pPreventUpdate = false;
		throw;
	}
}

void peeWPType::SelectActiveTrailController(){
	const peeType * const type = GetType();
	
	deParticleEmitterType::eEmitControllers index = deParticleEmitterType::eecLifetime;
	if(type){
		index = type->GetActiveTrailController();
	}
	
	pCBTrailController->SetSelectionWithData((void*)(intptr_t)index);
}

void peeWPType::SelectActiveEmitController(){
	peeType * const type = GetType();
	
	deParticleEmitterType::eEmitControllers index = deParticleEmitterType::eecLifetime;
	if(type){
		index = type->GetActiveEmitController();
	}
	
	pCBEmitController->SetSelectionWithData((void*)(intptr_t)index);
}

void peeWPType::SelectActiveParameter(){
	const peeType * const type = GetType();
	
	if(type){
		pListParams->SetSelectionWithData((void*)(intptr_t)
			type->IndexOfParameter(GetParameter()));
		
	}else{
		pListParams->SetSelection(-1);
	}
	
	UpdateParameter();
}

void peeWPType::UpdateParameter(){
	const peeParameter * const parameter = GetParameter();
	
	if(parameter){
		pEditParamValue->SetFloat(parameter->GetValue());
		pEditParamSpread->SetFloat(parameter->GetSpread());
		pCBParamCtrlValue->SetSelectionWithData(parameter->GetControllerValue());
		pCBParamCtrlSpread->SetSelectionWithData(parameter->GetControllerSpread());
		
	}else{
		pEditParamValue->ClearText();
		pEditParamSpread->ClearText();
		pCBParamCtrlValue->SetSelectionWithData(NULL);
		pCBParamCtrlSpread->SetSelectionWithData(NULL);
	}
	
	const bool enabled = parameter != NULL;
	pEditParamValue->SetEnabled(enabled);
	pEditParamSpread->SetEnabled(enabled);
	pCBParamCtrlValue->SetEnabled(enabled);
	pCBParamCtrlSpread->SetEnabled(enabled);
}

void peeWPType::UpdateParameterUsage(const peeParameter &parameter){
	const int index = pListParams->IndexOfItemWithData((void*)(intptr_t)parameter.GetIndex());
	if(index == -1){
		return;
	}
	
	pListParams->GetItemAt(index)->SetIcon(
		(parameter.GetCurveValue().GetPointCount() > 0
		|| parameter.GetCurveSpread().GetPointCount() > 0
		|| parameter.GetCurveProgress().GetPointCount() > 0
		|| parameter.GetCurveBeam().GetPointCount() > 0)
			? pIconUsed : pIconUnused);
	pListParams->ItemChangedAt(index);
}
