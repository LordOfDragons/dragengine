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
	
	void OnTextChanged(igdeTextField *textField) override{
		peeEmitter * const emitter = pPanel.GetEmitter();
		peeType * const type = pPanel.GetType();
		if(!emitter || !type){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, emitter, type));
		if(undo){
			emitter->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, peeEmitter *emitter, peeType *type) = 0;
};

class cBaseAction : public igdeAction{
protected:
	peeWPType &pPanel;
	
public:
	cBaseAction(peeWPType &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		peeEmitter * const emitter = pPanel.GetEmitter();
		peeType * const type = pPanel.GetType();
		if(!emitter || !type){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(emitter, type));
		if(undo){
			emitter->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(peeEmitter *emitter, peeType *type) = 0;
};

class cBasePathListener : public igdeEditPathListener{
protected:
	peeWPType &pPanel;
	
public:
	cBasePathListener(peeWPType &panel) : pPanel(panel){}
	
	void OnEditPathChanged(igdeEditPath *editPath) override{
		peeEmitter * const emitter = pPanel.GetEmitter();
		peeType * const type = pPanel.GetType();
		if(!emitter || !type){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(editPath->GetPath(), emitter, type));
		if(undo){
			emitter->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(const decString &path, peeEmitter *emitter, peeType *type) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	peeWPType &pPanel;
	
public:
	cBaseComboBoxListener(peeWPType &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		peeEmitter * const emitter = pPanel.GetEmitter();
		peeType * const type = pPanel.GetType();
		if(!emitter || !type){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(comboBox, emitter, type));
		if(undo){
			emitter->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox *comboBox, peeEmitter *emitter, peeType *type) = 0;
};



class cActionEmitBurst : public igdeAction{
	peeWPType &pPanel;
public:
	using Ref = deTObjectReference<cActionEmitBurst>;
	cActionEmitBurst(peeWPType &panel) : igdeAction("@ParticleEmitter.WPType.EmitBurst", nullptr,
		"@ParticleEmitter.WPType.EmitBurst.ToolTip"),
	pPanel(panel){}
	
	void OnAction() override{
		peeEmitter * const emitter = pPanel.GetEmitter();
		if(!emitter){
			return;
		}
		
		emitter->GetUndoSystem()->Add(peeUEmitterToggleEmitBurst::Ref::New(emitter));
	}
};

class cTextBurstLifetime : public igdeTextFieldListener{
	peeWPType &pPanel;
public:
	using Ref = deTObjectReference<cTextBurstLifetime>;
	cTextBurstLifetime(peeWPType &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		peeEmitter * const emitter = pPanel.GetEmitter();
		if(!emitter){
			return;
		}
		
		const float value = textField->GetFloat();
		if(fabsf(value - emitter->GetBurstLifetime()) < FLOAT_SAFE_EPSILON){
			return;
		}
		
		emitter->GetUndoSystem()->Add(peeUEmitterSetBurstLifetime::Ref::New(emitter, value));
	}
};



class cComboType : public igdeComboBoxListener{
	peeWPType &pPanel;
public:
	using Ref = deTObjectReference<cComboType>;
	cComboType(peeWPType &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeComboBox * comboBox) override{
		peeEmitter * const emitter = pPanel.GetEmitter();
		if(!emitter){
			return;
		}
		
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		emitter->SetActiveType(selection ? (peeType*)selection->GetData() : nullptr);
	}
};

class cActionType : public igdeAction{
	peeWPType &pPanel;
	igdeButton::Ref &pButton;
public:
	using Ref = deTObjectReference<cActionType>;
	cActionType(peeWPType &panel, igdeButton::Ref &button) :
	igdeAction("", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown),
		"@ParticleEmitter.WPType.TypeMenu.ToolTip"),
	pPanel(panel), pButton(button){}
	
	void OnAction() override{
		peeEmitter * const emitter = pPanel.GetEmitter();
		if(!emitter){
			return;
		}
		
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelperProperties();
		igdeMenuCascade::Ref menu(igdeMenuCascade::Ref::New(pPanel.GetEnvironment()));
		helper.MenuCommand(menu, pPanel.GetActionTypeAdd());
		helper.MenuCommand(menu, pPanel.GetActionTypeRemove());
		helper.MenuCommand(menu, pPanel.GetActionTypeRename());
		menu->PopupBottom(pButton);
	}
};

class cActionTypeAdd : public igdeAction{
	peeWPType &pPanel;
public:
	using Ref = deTObjectReference<cActionTypeAdd>;
	cActionTypeAdd(peeWPType &panel) :
	igdeAction("@ParticleEmitter.WPType.Action.TypeAdd", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus),
		"@ParticleEmitter.WPType.Action.TypeAdd.ToolTip"),
	pPanel(panel){}
	
	void OnAction() override{
		peeEmitter * const emitter = pPanel.GetEmitter();
		if(!emitter){
			return;
		}
		
		decString name(pPanel.Translate("ParticleEmitter.WPType.DefaultName").ToUTF8());
		
		while(igdeCommonDialogs::GetString(pPanel, "@ParticleEmitter.WPType.Dialog.AddType.Title", "@ParticleEmitter.WPType.Dialog.AddType.Name", name)){
			if(emitter->GetTypes().HasNamed(name)){
				igdeCommonDialogs::Error(pPanel, "@ParticleEmitter.WPType.Dialog.AddType.Title", "@ParticleEmitter.WPType.Dialog.AddType.Error");
				continue;
			}
			
			const peeType::Ref type(peeType::Ref::New(emitter->GetEngine(), name));
			
			emitter->GetUndoSystem()->Add(peeUTypeAdd::Ref::New(emitter, type));
			return;
		}
	}
	
	void Update() override{
		SetEnabled(pPanel.GetEmitter().IsNotNull());
	}
};

class cActionTypeRemove : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionTypeRemove>;
	cActionTypeRemove(peeWPType &panel) :
	cBaseAction(panel, "@ParticleEmitter.WPType.Action.TypeRemove", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"@ParticleEmitter.WPType.Action.TypeRemove.ToolTip"){}
	
	igdeUndo::Ref OnAction(peeEmitter*, peeType *type) override{
		return peeUTypeRemove::Ref::New(type);
	}
	
	void Update() override{
		SetEnabled(pPanel.GetType() != nullptr);
	}
};

class cActionTypeRename : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionTypeRename>;
	cActionTypeRename(peeWPType &panel) :
	cBaseAction(panel, "@ParticleEmitter.WPType.Action.TypeRename", nullptr, "@ParticleEmitter.WPType.Action.TypeRename.ToolTip"){}
	
	igdeUndo::Ref OnAction(peeEmitter *emitter, peeType *type) override{
		decString name(type->GetName());
		
		while(igdeCommonDialogs::GetString(pPanel, "@ParticleEmitter.WPType.Dialog.RenameType.Title", "@ParticleEmitter.WPType.Dialog.RenameType.Name", name)){
			if(name == type->GetName()){
				break;
				
			}else if(emitter->GetTypes().HasNamed(name)){
				igdeCommonDialogs::Error(pPanel, "@ParticleEmitter.WPType.Dialog.RenameType.Title", "@ParticleEmitter.WPType.Dialog.RenameType.Error");
				
			}else{
				return peeUTypeSetName::Ref::New(type, name);
			}
		}
		
		return {};
	}
	
	void Update() override{
		SetEnabled(pPanel.GetType() != nullptr);
	}
};

class cPathSkin : public cBasePathListener{
public:
	using Ref = deTObjectReference<cPathSkin>;
	cPathSkin(peeWPType &panel) : cBasePathListener(panel){}
	
	igdeUndo::Ref OnChanged(const decString &path, peeEmitter*, peeType *type) override{
		if(type->GetSkinPath() == path){
			return {};
		}
		return peeUTypeSetSkinPath::Ref::New(type, path);
	}
};

class cPathModel : public cBasePathListener{
public:
	using Ref = deTObjectReference<cPathModel>;
	cPathModel(peeWPType &panel) : cBasePathListener(panel){}
	
	igdeUndo::Ref OnChanged(const decString &path, peeEmitter*, peeType *type) override{
		if(type->GetModelPath() == path){
			return {};
		}
		return peeUTypeSetModelPath::Ref::New(type, path);
	}
};

class cPathModelSkin : public cBasePathListener{
public:
	using Ref = deTObjectReference<cPathModelSkin>;
	cPathModelSkin(peeWPType &panel) : cBasePathListener(panel){}
	
	igdeUndo::Ref OnChanged(const decString &path, peeEmitter*, peeType *type) override{
		if(type->GetModelSkinPath() == path){
			return {};
		}
		return peeUTypeSetModelSkinPath::Ref::New(type, path);
	}
};

class cComboCastFrom : public cBaseComboBoxListener{
public:
	using Ref = deTObjectReference<cComboCastFrom>;
	cComboCastFrom(peeWPType &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, peeEmitter*, peeType *type) override{
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if(!selection){
			DETHROW(deeInvalidParam);
		}
		
		const deParticleEmitterType::eCastFrom castFrom =
			(deParticleEmitterType::eCastFrom)(intptr_t)selection->GetData();
		if(type->GetCastFrom() == castFrom){
			return {};
		}
		return peeUTypeSetCastFrom::Ref::New(type, castFrom);
	}
};

class cComboSimType : public cBaseComboBoxListener{
public:
	using Ref = deTObjectReference<cComboSimType>;
	cComboSimType(peeWPType &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, peeEmitter*, peeType *type) override{
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if(!selection){
			DETHROW(deeInvalidParam);
		}
		
		const deParticleEmitterType::eSimulationTypes simulationType =
			(deParticleEmitterType::eSimulationTypes)(intptr_t)selection->GetData();
		if(type->GetSimulationType() == simulationType){
			return {};
		}
		return peeUTypeSetSimulationType::Ref::New(type, simulationType);
	}
};

class cActionIntervalAsDistance : public cBaseAction{
public:
	using Ref = deTObjectReference<cActionIntervalAsDistance>;
	cActionIntervalAsDistance(peeWPType &panel) :
	cBaseAction(panel, "@ParticleEmitter.WPType.IntervalAsDistance", nullptr,
		"@ParticleEmitter.WPType.IntervalAsDistance.ToolTip"){ }
	
	igdeUndo::Ref OnAction(peeEmitter*, peeType *type) override{
		return peeUTypeSetIntervalAsDistance::Ref::New(type);
	}
};

class cTextPhysicsSize : public cBaseTextFieldListener{
public:
	using Ref = deTObjectReference<cTextPhysicsSize>;
	cTextPhysicsSize(peeWPType &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, peeEmitter*, peeType *type) override{
		const float value = textField->GetFloat();
		if(fabsf(value - type->GetPhysicsSize()) < FLOAT_SAFE_EPSILON){
			return {};
		}
		return peeUTypeSetPhysicsSize::Ref::New(type, value);
	}
};


class cPathTrailEmitter : public cBasePathListener{
public:
	using Ref = deTObjectReference<cPathTrailEmitter>;
	cPathTrailEmitter(peeWPType &panel) : cBasePathListener(panel){}
	
	igdeUndo::Ref OnChanged(const decString &path, peeEmitter*, peeType *type) override{
		if(type->GetPathTrailEmitter() == path){
			return {};
		}
		return peeUTypeSetPathTrailEmitter::Ref::New(type, path);
	}
};

class cComboTrailController : public cBaseComboBoxListener{
public:
	using Ref = deTObjectReference<cComboTrailController>;
	cComboTrailController(peeWPType &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, peeEmitter*, peeType *type) override{
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if(!selection){
			DETHROW(deeInvalidParam);
		}
		
		type->SetActiveTrailController(
			(deParticleEmitterType::eEmitControllers)(intptr_t)selection->GetData());
		return {};
	}
};

class cTextTrailTargetController : public cBaseTextFieldListener{
	igdeComboBox &pComboBox;
public:
	using Ref = deTObjectReference<cTextTrailTargetController>;
	cTextTrailTargetController(peeWPType &panel, igdeComboBox &comboBox) :
	cBaseTextFieldListener(panel),
	pComboBox(comboBox){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, peeEmitter*, peeType *type) override{
		const igdeListItem * const selection = pComboBox.GetSelectedItem();
		if(!selection){
			DETHROW(deeInvalidParam);
		}
		
		const deParticleEmitterType::eEmitControllers controller =
			(deParticleEmitterType::eEmitControllers)(intptr_t)selection->GetData();
		return type->GetTrailController(controller) != textField->GetText()
			? peeUTypeSetTrailController::Ref::New(type, controller, textField->GetText()) : igdeUndo::Ref();
	}
};


class cComboCollisionResponse : public cBaseComboBoxListener{
public:
	using Ref = deTObjectReference<cComboCollisionResponse>;
	cComboCollisionResponse(peeWPType &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, peeEmitter*, peeType *type) override{
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if(!selection){
			DETHROW(deeInvalidParam);
		}
		
		const deParticleEmitterType::eCollisionResponses response =
			(deParticleEmitterType::eCollisionResponses)(intptr_t)selection->GetData();
		if(response == type->GetCollisionResponse()){
			return {};
		}
		
		return peeUTypeSetCollisionResponse::Ref::New(type, response);
	}
};

class cPathCollisionEmitter : public cBasePathListener{
public:
	using Ref = deTObjectReference<cPathCollisionEmitter>;
	cPathCollisionEmitter(peeWPType &panel) : cBasePathListener(panel){}
	
	igdeUndo::Ref OnChanged(const decString &path, peeEmitter*, peeType *type) override{
		if(type->GetPathCollisionEmitter() == path){
			return {};
		}
		return peeUTypeSetPathCollisionEmitter::Ref::New(type, path);
	}
};

class cTextEmitMinImpulse : public cBaseTextFieldListener{
public:
	using Ref = deTObjectReference<cTextEmitMinImpulse>;
	cTextEmitMinImpulse(peeWPType &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, peeEmitter*, peeType *type) override{
		const float value = textField->GetFloat();
		if(fabsf(value - type->GetEmitMinImpulse()) < FLOAT_SAFE_EPSILON){
			return {};
		}
		return peeUTypeSetEmitMinImpulse::Ref::New(type, value);
	}
};

class cComboEmitController : public cBaseComboBoxListener{
public:
	using Ref = deTObjectReference<cComboEmitController>;
	cComboEmitController(peeWPType &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, peeEmitter*, peeType *type) override{
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if(!selection){
			DETHROW(deeInvalidParam);
		}
		
		type->SetActiveEmitController(
			(deParticleEmitterType::eEmitControllers)(intptr_t)selection->GetData());
		return {};
	}
};

class cTextEmitTargetController : public cBaseTextFieldListener{
	igdeComboBox &pComboBox;
public:
	using Ref = deTObjectReference<cTextEmitTargetController>;
	cTextEmitTargetController(peeWPType &panel, igdeComboBox &comboBox) :
	cBaseTextFieldListener(panel),
	pComboBox(comboBox){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, peeEmitter*, peeType *type) override{
		const igdeListItem * const selection = pComboBox.GetSelectedItem();
		if(!selection){
			DETHROW(deeInvalidParam);
		}
		
		const deParticleEmitterType::eEmitControllers controller =
			(deParticleEmitterType::eEmitControllers)(intptr_t)selection->GetData();
		return type->GetEmitController(controller) != textField->GetText()
			? peeUTypeSetEmitController::Ref::New(type, controller, textField->GetText()) : igdeUndo::Ref();
	}
};


class cListTypes : public igdeListBoxListener{
	peeWPType &pPanel;
public:
	using Ref = deTObjectReference<cListTypes>;
	cListTypes(peeWPType &panel) : pPanel(panel){}
	
	void OnSelectionChanged(igdeListBox *listBox) override{
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
	using Ref = deTObjectReference<cTextParamValue>;
	cTextParamValue(peeWPType &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, peeEmitter*, peeType *type) override{
		peeParameter * const parameter = type->GetActiveParameter();
		const float value = textField->GetFloat();
		if(fabsf(value - parameter->GetValue()) < FLOAT_SAFE_EPSILON){
			return {};
		}
		return peeUParameterSetValue::Ref::New(type, parameter, value);
	}
};

class cTextParamSpread : public cBaseTextFieldListener{
public:
	using Ref = deTObjectReference<cTextParamSpread>;
	cTextParamSpread(peeWPType &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, peeEmitter*, peeType *type) override{
		peeParameter * const parameter = type->GetActiveParameter();
		const float value = textField->GetFloat();
		if(fabsf(value - parameter->GetSpread()) < FLOAT_SAFE_EPSILON){
			return {};
		}
		return peeUParameterSetSpread::Ref::New(type, parameter, value);
	}
};

class cComboControllerValue : public cBaseComboBoxListener{
	bool &pPreventUpdate;
public:
	using Ref = deTObjectReference<cComboControllerValue>;
	cComboControllerValue(peeWPType &panel, bool &preventUpdate) :
	cBaseComboBoxListener(panel), pPreventUpdate(preventUpdate){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, peeEmitter*, peeType *type) override{
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if(!selection || pPreventUpdate){
			return {};
		}
		
		peeController * const controller = (peeController*)selection->GetData();
		peeParameter * const parameter = type->GetActiveParameter();
		if(controller == parameter->GetControllerValue()){
			return {};
		}
		
		return peeUParameterSetControllerValue::Ref::New(type, parameter, controller);
	}
};

class cComboControllerSpread : public cBaseComboBoxListener{
	bool &pPreventUpdate;
public:
	using Ref = deTObjectReference<cComboControllerSpread>;
	cComboControllerSpread(peeWPType &panel, bool &preventUpdate) :
	cBaseComboBoxListener(panel), pPreventUpdate(preventUpdate){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, peeEmitter*, peeType *type) override{
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if(!selection || pPreventUpdate){
			return {};
		}
		
		peeController * const controller = (peeController*)selection->GetData();
		peeParameter * const parameter = type->GetActiveParameter();
		if(controller == parameter->GetControllerSpread()){
			return {};
		}
		
		return peeUParameterSetControllerSpread::Ref::New(type, parameter, controller);
	}
};

}



// Helper methods
///////////////////

static void AddControllersToComboBox(igdeComboBox &comboBox){
	comboBox.AddItem("@ParticleEmitter.WPType.Trail.Controller.Lifetime", nullptr,
		(void*)(intptr_t)deParticleEmitterType::eecLifetime);
	comboBox.AddItem("@ParticleEmitter.WPType.Trail.Controller.Mass", nullptr,
		(void*)(intptr_t)deParticleEmitterType::eecMass);
	comboBox.AddItem("@ParticleEmitter.WPType.Trail.Controller.LinearVelocity", nullptr,
		(void*)(intptr_t)deParticleEmitterType::eecLinearVelocity);
	comboBox.AddItem("@ParticleEmitter.WPType.Trail.Controller.AngularVelocity", nullptr,
		(void*)(intptr_t)deParticleEmitterType::eecAngularVelocity);
}

static void AddParametersToListBox(igdeListBox &listBox, igdeIcon *icon){
	listBox.AddItem("@ParticleEmitter.WPType.Parameters.TimeToLive", icon,
		(void*)(intptr_t)deParticleEmitterType::epTimeToLive);
	listBox.AddItem("@ParticleEmitter.WPType.Parameters.Interval", icon,
		(void*)(intptr_t)deParticleEmitterType::epInterval);
	listBox.AddItem("@ParticleEmitter.WPType.Parameters.ParticleCount", icon,
		(void*)(intptr_t)deParticleEmitterType::epParticleCount);
	listBox.AddItem("@ParticleEmitter.WPType.Parameters.CastAngleX", icon,
		(void*)(intptr_t)deParticleEmitterType::epCastAngleX);
	listBox.AddItem("@ParticleEmitter.WPType.Parameters.CastAngleY", icon,
		(void*)(intptr_t)deParticleEmitterType::epCastAngleY);
	listBox.AddItem("@ParticleEmitter.WPType.Parameters.Size", icon,
		(void*)(intptr_t)deParticleEmitterType::epSize);
	listBox.AddItem("@ParticleEmitter.WPType.Parameters.Red", icon,
		(void*)(intptr_t)deParticleEmitterType::epRed);
	listBox.AddItem("@ParticleEmitter.WPType.Parameters.Green", icon,
		(void*)(intptr_t)deParticleEmitterType::epGreen);
	listBox.AddItem("@ParticleEmitter.WPType.Parameters.Blue", icon,
		(void*)(intptr_t)deParticleEmitterType::epBlue);
	listBox.AddItem("@ParticleEmitter.WPType.Parameters.Transparency", icon,
		(void*)(intptr_t)deParticleEmitterType::epTransparency);
	listBox.AddItem("@ParticleEmitter.WPType.Parameters.Emissivity", icon,
		(void*)(intptr_t)deParticleEmitterType::epEmissivity);
	listBox.AddItem("@ParticleEmitter.WPType.Parameters.Mass", icon,
		(void*)(intptr_t)deParticleEmitterType::epMass);
	listBox.AddItem("@ParticleEmitter.WPType.Parameters.Rotation", icon,
		(void*)(intptr_t)deParticleEmitterType::epRotation);
	listBox.AddItem("@ParticleEmitter.WPType.Parameters.LinearVelocity", icon,
		(void*)(intptr_t)deParticleEmitterType::epLinearVelocity);
	listBox.AddItem("@ParticleEmitter.WPType.Parameters.AngularVelocity", icon,
		(void*)(intptr_t)deParticleEmitterType::epAngularVelocity);
	listBox.AddItem("@ParticleEmitter.WPType.Parameters.BrownMotion", icon,
		(void*)(intptr_t)deParticleEmitterType::epBrownMotion);
	listBox.AddItem("@ParticleEmitter.WPType.Parameters.Damping", icon,
		(void*)(intptr_t)deParticleEmitterType::epDamping);
	listBox.AddItem("@ParticleEmitter.WPType.Parameters.Drag", icon,
		(void*)(intptr_t)deParticleEmitterType::epDrag);
	listBox.AddItem("@ParticleEmitter.WPType.Parameters.GravityX", icon,
		(void*)(intptr_t)deParticleEmitterType::epGravityX);
	listBox.AddItem("@ParticleEmitter.WPType.Parameters.GravityY", icon,
		(void*)(intptr_t)deParticleEmitterType::epGravityY);
	listBox.AddItem("@ParticleEmitter.WPType.Parameters.GravityZ", icon,
		(void*)(intptr_t)deParticleEmitterType::epGravityZ);
	listBox.AddItem("@ParticleEmitter.WPType.Parameters.LocalGravity", icon,
		(void*)(intptr_t)deParticleEmitterType::epLocalGravity);
	listBox.AddItem("@ParticleEmitter.WPType.Parameters.ForceFieldDirect", icon,
		(void*)(intptr_t)deParticleEmitterType::epForceFieldDirect);
	listBox.AddItem("@ParticleEmitter.WPType.Parameters.ForceFieldSurface", icon,
		(void*)(intptr_t)deParticleEmitterType::epForceFieldSurface);
	listBox.AddItem("@ParticleEmitter.WPType.Parameters.ForceFieldMass", icon,
		(void*)(intptr_t)deParticleEmitterType::epForceFieldMass);
	listBox.AddItem("@ParticleEmitter.WPType.Parameters.ForceFieldSpeed", icon,
		(void*)(intptr_t)deParticleEmitterType::epForceFieldSpeed);
	listBox.AddItem("@ParticleEmitter.WPType.Parameters.Elasticity", icon,
		(void*)(intptr_t)deParticleEmitterType::epElasticity);
	listBox.AddItem("@ParticleEmitter.WPType.Parameters.Roughness", icon,
		(void*)(intptr_t)deParticleEmitterType::epRoughness);
	listBox.AddItem("@ParticleEmitter.WPType.Parameters.EmitDirection", icon,
		(void*)(intptr_t)deParticleEmitterType::epEmitDirection);
}



// Class peeWPType
////////////////////

// Constructor, destructor
////////////////////////////

peeWPType::peeWPType(peeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties),
pPreventUpdate(false)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainer::Ref content, groupBox, form, frameLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = peeWPTypeListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	pIconUnused = env.GetStockIcon(igdeEnvironment::esiSmallMinus);
	pIconUsed = env.GetStockIcon(igdeEnvironment::esiSmallPlus);
	
	pActionTypeAdd = cActionTypeAdd::Ref::New(*this);
	pActionTypeRemove = cActionTypeRemove::Ref::New(*this);
	pActionTypeRename = cActionTypeRename::Ref::New(*this);
	
	
	// emitter settings
	helper.GroupBox(content, groupBox, "@ParticleEmitter.WPType.Group.Emitter");
	
	helper.CheckBox(groupBox, pChkEmitBurst, cActionEmitBurst::Ref::New(*this));
	helper.EditFloat(groupBox, "@ParticleEmitter.WPType.Lifetime", "@ParticleEmitter.WPType.Lifetime.ToolTip",
		pEditBurstLifetime, cTextBurstLifetime::Ref::New(*this));
	
	
	// type settings
	helper.GroupBox(content, groupBox, "@ParticleEmitter.WPType.Group.Types");
	
	helper.FormLineStretchFirst(groupBox, "@ParticleEmitter.WPType.Type", "@ParticleEmitter.WPType.Type.ToolTip", frameLine);
	helper.ComboBox(frameLine, "@ParticleEmitter.WPType.Types.ToolTip", pCBType, cComboType::Ref::New(*this));
	pCBType->SetDefaultSorter();
	helper.Button(frameLine, pBtnType, cActionType::Ref::New(*this, pBtnType));
	
	helper.EditPath(groupBox, "@ParticleEmitter.WPType.Skin", "@ParticleEmitter.WPType.Skin.ToolTip",
		igdeEnvironment::efpltSkin, pEditSkin, cPathSkin::Ref::New(*this));
	helper.EditPath(groupBox, "@ParticleEmitter.WPType.Model", "@ParticleEmitter.WPType.Model.ToolTip",
		igdeEnvironment::efpltModel, pEditModel, cPathModel::Ref::New(*this));
	helper.EditPath(groupBox, "@ParticleEmitter.WPType.ModelSkin", "@ParticleEmitter.WPType.ModelSkin.ToolTip",
		igdeEnvironment::efpltSkin, pEditModelSkin, cPathModelSkin::Ref::New(*this));
	
	helper.ComboBox(groupBox, "@ParticleEmitter.WPType.CastFrom", "@ParticleEmitter.WPType.CastFrom.ToolTip",
		pCBCastFrom, cComboCastFrom::Ref::New(*this));
	pCBCastFrom->SetAutoTranslateItems(true);
	pCBCastFrom->AddItem("@ParticleEmitter.WPType.CastFrom.Vertex", nullptr, (void*)(intptr_t)deParticleEmitterType::ecfVertex);
	pCBCastFrom->AddItem("@ParticleEmitter.WPType.CastFrom.Face", nullptr, (void*)(intptr_t)deParticleEmitterType::ecfFace);
	pCBCastFrom->AddItem("@ParticleEmitter.WPType.CastFrom.Volume", nullptr, (void*)(intptr_t)deParticleEmitterType::ecfVolume);
	
	helper.ComboBox(groupBox, "@ParticleEmitter.WPType.SimulationType", "@ParticleEmitter.WPType.SimulationType.ToolTip",
		pCBSimType, cComboSimType::Ref::New(*this));
	pCBSimType->SetAutoTranslateItems(true);
	pCBSimType->AddItem("@ParticleEmitter.WPType.SimulationType.Particle", nullptr, (void*)(intptr_t)deParticleEmitterType::estParticle);
	pCBSimType->AddItem("@ParticleEmitter.WPType.SimulationType.Ribbon", nullptr, (void*)(intptr_t)deParticleEmitterType::estRibbon);
	pCBSimType->AddItem("@ParticleEmitter.WPType.SimulationType.Beam", nullptr, (void*)(intptr_t)deParticleEmitterType::estBeam);
	
	helper.CheckBox(groupBox, pChkIntervalAsDistance, cActionIntervalAsDistance::Ref::New(*this));
	helper.EditFloat(groupBox, "@ParticleEmitter.WPType.PhysicsSize", "@ParticleEmitter.WPType.PhysicsSize.ToolTip",
		pEditPhysicsSize, cTextPhysicsSize::Ref::New(*this));
	
	
	// type trail
	helper.GroupBox(content, groupBox, "@ParticleEmitter.WPType.Group.Trail");
	
	helper.EditPath(groupBox, "@ParticleEmitter.WPType.Trail.Emitter", "@ParticleEmitter.WPType.Trail.Emitter.ToolTip",
		igdeEnvironment::efpltParticleEmitter, pEditPathTrailEmitter, cPathTrailEmitter::Ref::New(*this));
	
	helper.FormLineStretchFirst(groupBox, "@ParticleEmitter.WPType.Trail.Controller", "@ParticleEmitter.WPType.Trail.Controller.ToolTip", frameLine);
	helper.ComboBox(frameLine, "@ParticleEmitter.WPType.Trail.Controller.ToolTip",
		pCBTrailController, cComboTrailController::Ref::New(*this));
	pCBTrailController->SetAutoTranslateItems(true);
	AddControllersToComboBox(pCBTrailController);
	helper.EditString(frameLine, "@ParticleEmitter.WPType.Trail.TargetController.ToolTip",
		pEditTrailTargetController, cTextTrailTargetController::Ref::New(*this, pCBTrailController));
	
	
	// type collision
	helper.GroupBox(content, groupBox, "@ParticleEmitter.WPType.Group.Collision");
	
	helper.ComboBox(groupBox, "@ParticleEmitter.WPType.Collision.Response", "@ParticleEmitter.WPType.Collision.Response.ToolTip",
		pCBCollisionResponse, cComboCollisionResponse::Ref::New(*this));
	pCBCollisionResponse->SetAutoTranslateItems(true);
	pCBCollisionResponse->AddItem("@ParticleEmitter.WPType.Collision.Response.Destroy", nullptr,
		(void*)(intptr_t)deParticleEmitterType::ecrDestroy);
	pCBCollisionResponse->AddItem("@ParticleEmitter.WPType.Collision.Response.Physical", nullptr,
		(void*)(intptr_t)deParticleEmitterType::ecrPhysical);
	pCBCollisionResponse->AddItem("@ParticleEmitter.WPType.Collision.Response.Custom", nullptr,
		(void*)(intptr_t)deParticleEmitterType::ecrCustom);
	
	helper.EditPath(groupBox, "@ParticleEmitter.WPType.Collision.Emitter", "@ParticleEmitter.WPType.Collision.Emitter.ToolTip",
		igdeEnvironment::efpltParticleEmitter, pEditPathCollisionEmitter, cPathCollisionEmitter::Ref::New(*this));
	helper.EditFloat(groupBox, "@ParticleEmitter.WPType.Collision.MinImpulse",
		"@ParticleEmitter.WPType.Collision.MinImpulse.ToolTip",
		pEditEmitMinImpulse, cTextEmitMinImpulse::Ref::New(*this));
	
	helper.FormLineStretchFirst(groupBox, "@ParticleEmitter.WPType.Collision.Controller", "@ParticleEmitter.WPType.Collision.Controller.ToolTip", frameLine);
	helper.ComboBox(frameLine, "@ParticleEmitter.WPType.Collision.Controller.ToolTip",
		pCBEmitController, cComboEmitController::Ref::New(*this));
	pCBEmitController->SetAutoTranslateItems(true);
	AddControllersToComboBox(pCBEmitController);
	helper.EditString(frameLine, "@ParticleEmitter.WPType.Collision.TargetController.ToolTip",
		pEditEmitTargetController, cTextEmitTargetController::Ref::New(*this, pCBEmitController));
	
	
	// parameter list
	helper.GroupBoxFlow(content, groupBox, "@ParticleEmitter.WPType.Group.Parameters");
	
	helper.ListBox(groupBox, 10, "@ParticleEmitter.WPType.Parameters.ToolTip", pListParams, cListTypes::Ref::New(*this));
	pListParams->SetAutoTranslateItems(true);
	AddParametersToListBox(pListParams, pIconUnused);
	
	form = igdeContainerForm::Ref::New(env);
	groupBox->AddChild(form);
	
	helper.EditFloat(form, "@ParticleEmitter.WPType.Parameter.CastRange",
		"@ParticleEmitter.WPType.Parameter.CastRange.ToolTip",
		pEditParamValue, cTextParamValue::Ref::New(*this));
	helper.EditFloat(form, "@ParticleEmitter.WPType.Parameter.CastSpread",
		"@ParticleEmitter.WPType.Parameter.CastSpread.ToolTip", pEditParamSpread, cTextParamSpread::Ref::New(*this));
	
	helper.ComboBox(form, "@ParticleEmitter.WPType.Parameter.ControllerValue", "@ParticleEmitter.WPType.Parameter.ControllerValue.ToolTip",
		pCBParamCtrlValue, cComboControllerValue::Ref::New(*this, pPreventUpdate));
	helper.ComboBox(form, "@ParticleEmitter.WPType.Parameter.ControllerSpread", "@ParticleEmitter.WPType.Parameter.ControllerSpread.ToolTip",
		pCBParamCtrlSpread, cComboControllerSpread::Ref::New(*this, pPreventUpdate));
}

peeWPType::~peeWPType(){
	if(pEmitter){
		pEmitter->RemoveListener(pListener);
		pEmitter = nullptr;
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
	return pEmitter ? pEmitter->GetActiveType().Pointer() : nullptr;
}

peeParameter *peeWPType::GetParameter() const{
	const peeType * const activeType = GetType();
	return activeType ? activeType->GetActiveParameter() : nullptr;
}



void peeWPType::UpdateEmitter(){
	if(pEmitter){
		pEditBurstLifetime->SetFloat(pEmitter->GetBurstLifetime());
		pChkEmitBurst->SetChecked(pEmitter->GetEmitBurst());
		
	}else{
		pEditBurstLifetime->ClearText();
		pChkEmitBurst->SetChecked(false);
	}
	
	const bool enabled = pEmitter != nullptr;
	pEditBurstLifetime->SetEnabled(enabled);
	pChkEmitBurst->SetEnabled(enabled);
}

void peeWPType::UpdateTypeList(){
	pCBType->UpdateRestoreSelection([&](){
		pCBType->RemoveAllItems();
		
		if(pEmitter){
			pEmitter->GetTypes().Visit([&](peeType *t){
				pCBType->AddItem(t->GetName(), nullptr, t);
			});
			
			pCBType->SortItems();
		}
	}, 0);
	
	UpdateType();
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
		for(i=0; i<pListParams->GetItems().GetCount(); i++){
			UpdateParameterUsage(*type->GetParameterAt(
				(deParticleEmitterType::eParameters)(intptr_t)pListParams->GetItems().GetAt(i)->GetData()));
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
		for(i=0; i<pListParams->GetItems().GetCount(); i++){
			pListParams->GetItems().GetAt(i)->SetIcon(pIconUnused);
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
		
		pCBParamCtrlValue->AddItem(Translate("ParticleEmitter.WPType.Parameter.None").ToUTF8(), nullptr, nullptr);
		pCBParamCtrlSpread->AddItem(Translate("ParticleEmitter.WPType.Parameter.None").ToUTF8(), nullptr, nullptr);
		
		pCBParamCtrlValue->SetEnabled(pEmitter.IsNotNull());
		pCBParamCtrlSpread->SetEnabled(pEmitter.IsNotNull());
		
		if(!pEmitter){
			return;
		}
		
		const peeController::List &controllers = pEmitter->GetControllers();
		const int controllerCount = controllers.GetCount();
		decString text;
		int i;
		
		for(i=0; i<controllerCount; i++){
			peeController * const controller = controllers.GetAt(i);
			text.Format("%d: %s", i, controller->GetName().GetString());
			
			pCBParamCtrlValue->AddItem(text, nullptr, controller);
			pCBParamCtrlSpread->AddItem(text, nullptr, controller);
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
		pCBParamCtrlValue->SetSelectionWithData(nullptr);
		pCBParamCtrlSpread->SetSelectionWithData(nullptr);
	}
	
	const bool enabled = parameter != nullptr;
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
	
	pListParams->GetItems().GetAt(index)->SetIcon(
		(parameter.GetCurveValue().GetPointCount() > 0
		|| parameter.GetCurveSpread().GetPointCount() > 0
		|| parameter.GetCurveProgress().GetPointCount() > 0
		|| parameter.GetCurveBeam().GetPointCount() > 0)
			? pIconUsed : pIconUnused);
	pListParams->ItemChangedAt(index);
}
