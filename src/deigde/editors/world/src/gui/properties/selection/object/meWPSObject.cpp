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

#include "meWPSObject.h"
#include "meWPSObjectListener.h"
#include "../meWPSelection.h"
#include "../../meWPBrowser.h"
#include "../../meWPPropertyList.h"
#include "../../meWindowProperties.h"
#include "../../../meWindowMain.h"
#include "../../../../utils/meHelpers.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/meCamera.h"
#include "../../../../world/object/meObject.h"
#include "../../../../world/object/meObjectSelection.h"
#include "../../../../world/object/texture/meObjectTexture.h"
#include "../../../../world/meWorldGuiParameters.h"
#include "../../../../world/idgroup/meIDGroup.h"
#include "../../../../undosys/properties/object/meUSetObjectPosition.h"
#include "../../../../undosys/properties/object/meUSetObjectRotation.h"
#include "../../../../undosys/properties/object/meUSetObjectSize.h"
#include "../../../../undosys/properties/object/meUObjectSetScaling.h"
#include "../../../../undosys/properties/object/meUSetObjectClass.h"
#include "../../../../undosys/properties/object/property/meUObjectAddProperty.h"
#include "../../../../undosys/properties/object/property/meUObjectSetProperty.h"
#include "../../../../undosys/properties/object/property/meUObjectSetProperties.h"
#include "../../../../undosys/properties/object/property/meUObjectRemoveProperty.h"
#include "../../../../undosys/properties/object/property/meUObjectPropertyCopyToSelected.h"
#include "../../../../undosys/properties/object/property/meUObjectPropertyRemoveFromSelected.h"
#include "../../../../undosys/properties/object/property/meUObjectClonePropertiesToSelected.h"
#include "../../../../undosys/properties/object/attachBehavior/meUObjectSetAttachBehaviors.h"
#include "../../../../undosys/properties/object/texture/meUObjectAddTexture.h"
#include "../../../../undosys/properties/object/texture/meUObjectRemoveTexture.h"
#include "../../../../undosys/properties/object/texture/meUObjectTextureSetSkin.h"
#include "../../../../undosys/properties/object/texture/meUObjectTextureTCOffset.h"
#include "../../../../undosys/properties/object/texture/meUObjectTextureTCScaling.h"
#include "../../../../undosys/properties/object/texture/meUObjectTextureTCRotation.h"
#include "../../../../undosys/properties/object/texture/meUObjectTextureColorTint.h"
#include "../../../../undosys/properties/object/texture/meUObjectRemoveAllTextures.h"
#include "../../../../undosys/properties/object/texture/meUObjectRemoveUnusedTextures.h"
#include "../../../../undosys/properties/object/texture/meUObjectAddUsedTextures.h"
#include "../../../../undosys/properties/object/texture/meUObjectTextureCopyToSelected.h"
#include "../../../../undosys/properties/object/texture/meUObjectCloneTexturesToSelected.h"
#include "../../../../undosys/properties/object/texture/property/meUObjTexAddProperty.h"
#include "../../../../undosys/properties/object/texture/property/meUObjTexRemoveProperty.h"
#include "../../../../undosys/properties/object/texture/property/meUObjTexSetProperty.h"
#include "../../../../undosys/properties/object/texture/property/meUObjTexSetProperties.h"

#include <deigde/codec/igdeCodecPropertyString.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/class/igdeGDClass.h>
#include <deigde/gamedefinition/class/igdeGDClassManager.h>
#include <deigde/gamedefinition/class/igdeGDClassInherit.h>
#include <deigde/gamedefinition/class/component/igdeGDCComponent.h>
#include <deigde/gamedefinition/class/component/igdeGDCCTexture.h>
#include <deigde/gamedefinition/property/igdeGDProperty.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeColorBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeSpinTextField.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/composed/igdeEditSliderText.h>
#include <deigde/gui/composed/igdeEditSliderTextListener.h>
#include <deigde/gui/composed/igdeEditDVector.h>
#include <deigde/gui/composed/igdeEditDVectorListener.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/composed/igdeEditVector2.h>
#include <deigde/gui/composed/igdeEditVector2Listener.h>
#include <deigde/gui/event/igdeColorBoxListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeSpinTextFieldListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCommand.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringList.h>



// Actions
///////////

static const decString vEmptyString;

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	meWPSObject &pPanel;
	
public:
	cBaseTextFieldListener(meWPSObject &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		meObject * const object = pPanel.GetActiveObject();
		if(!object){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, object));
		if(undo){
			object->GetWorld()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, meObject *object) = 0;
};

class cBaseAction : public igdeAction{
protected:
	meWPSObject &pPanel;
	
public:
	cBaseAction(meWPSObject &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	virtual void OnAction(){
		meObject * const object = pPanel.GetActiveObject();
		if(!object){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(object));
		if(undo){
			object->GetWorld()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(meObject *object) = 0;
	
	virtual void Update(){
		if(pPanel.GetActiveObject()){
			Update(*pPanel.GetActiveObject());
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const meObject &){
		SetEnabled(true);
		SetSelected(false);
	}
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	meWPSObject &pPanel;
	
public:
	cBaseComboBoxListener(meWPSObject &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		meObject * const object = pPanel.GetActiveObject();
		if(!object){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(comboBox, object));
		if(undo){
			object->GetWorld()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox *comboBox, meObject *object) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	meWPSObject &pPanel;
	
public:
	cBaseEditVectorListener(meWPSObject &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		meObject * const object = pPanel.GetActiveObject();
		if(!object){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(editVector->GetVector(), object));
		if(undo){
			object->GetWorld()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(const decVector &vector, meObject *object) = 0;
};

class cBaseEditDVectorListener : public igdeEditDVectorListener{
protected:
	meWPSObject &pPanel;
	
public:
	cBaseEditDVectorListener(meWPSObject &panel) : pPanel(panel){}
	
	virtual void OnDVectorChanged(igdeEditDVector *editDVector){
		meObject * const object = pPanel.GetActiveObject();
		if(!object){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(editDVector->GetDVector(), object));
		if(undo){
			object->GetWorld()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(const decDVector &vector, meObject *object) = 0;
};



class cSpinActive : public igdeSpinTextFieldListener{
	meWPSObject &pPanel;
	
public:
	typedef deTObjectReference<cSpinActive> Ref;
	cSpinActive(meWPSObject &panel) : pPanel(panel){}
	
	virtual void OnValueChanged(igdeSpinTextField *textField){
		meObjectSelection &selection = pPanel.GetWorld()->GetSelectionObject();
		const int index = textField->GetValue();
		
		selection.SetActive(index >= 0 && index <= selection.GetSelected().GetCount()
			? selection.GetSelected().GetAt(index) : nullptr);
		
		pPanel.GetWorld()->NotifyObjectSelectionChanged();
	}
};

class cComboClass : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboClass> Ref;
	cComboClass(meWPSObject &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox *comboBox, meObject *object){
		return comboBox->GetText() != object->GetClassName()
			? meUSetObjectClass::Ref::New(object, comboBox->GetText()) : igdeUndo::Ref();
	}
};


class cActionMenuClass : public igdeActionContextMenu{
	meWPSObject &pPanel;
	
public:
	typedef deTObjectReference<cActionMenuClass> Ref;
	cActionMenuClass(meWPSObject &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown), "Class menu"),
	pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu){
		if(!pPanel.GetActiveObject()){
			return;
		}
		
		meWindowMain &windowMain = pPanel.GetWPSelection().GetWindowProperties().GetWindowMain();
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, pPanel.GetActionClassBrowse());
		helper.MenuCommand(contextMenu, pPanel.GetActionClassEdit());
		helper.MenuSeparator(contextMenu);
		helper.MenuCommand(contextMenu, windowMain.GetActionObjectSubclassAsEclass());
	}
};

class cActionClassBrowse : public cBaseAction{
public:
	typedef deTObjectReference<cActionClassBrowse> Ref;
	cActionClassBrowse(meWPSObject &panel) : cBaseAction(panel,
		"Browse Class", nullptr, "Show class in class browser"){}
	
	igdeUndo::Ref OnAction(meObject *object) override{
		if(object->GetGDClass()){
			pPanel.GetWPSelection().GetWindowProperties().SwitchToBrowser();
			pPanel.GetWPSelection().GetWindowProperties().GetBrowser().SelectObjectClass(object->GetGDClass());
		}
		return {};
	}
	
	void Update() override{
		SetEnabled(pPanel.GetActiveObject());
	}
};

class cActionClassEdit : public cBaseAction{
public:
	typedef deTObjectReference<cActionClassEdit> Ref;
	cActionClassEdit(meWPSObject &panel) : cBaseAction(panel,
		"Edit Class", nullptr, "Edit class in Game Definition Editor"){}
	
	igdeUndo::Ref OnAction(meObject *object) override{
		if(object->GetGDClass()){
			// TODO
		}
		return {};
	}
	
	void Update() override{
		SetEnabled(pPanel.GetActiveObject());
	}
};


class cActionMenuId : public igdeActionContextMenu{
	meWPSObject &pPanel;
	
public:
	typedef deTObjectReference<cActionMenuId> Ref;
	cActionMenuId(meWPSObject &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown), "ID menu"),
	pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu){
		if(!pPanel.GetWorld()){
			return;
		}
		
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, pPanel.GetActionIdFind());
	}
};

class cActionIdFind : public igdeAction{
	meWPSObject &pPanel;

public:
	typedef deTObjectReference<cActionIdFind> Ref;
	cActionIdFind(meWPSObject &panel) : igdeAction("Find ID...",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSearch),
		"Find ID. Activates object if found."), pPanel(panel){}
	
	void OnAction() override{
		meWorld * const world = pPanel.GetWorld();
		if(!world){
			return;
		}
		
		decString value;
		if(!igdeCommonDialogs::GetString(&pPanel.GetWPSelection(), "Find ID", "ID:", value)){
			return;
		}
		
		value.ToLower();
		if(value.BeginsWith("0h")){
			value = value.GetMiddle(2);
		}
		
		decUniqueID id;
		try{
			id = decUniqueID(value);
			
		}catch(const deException &){
			igdeCommonDialogs::ErrorFormat(&pPanel.GetWPSelection(),
				"Find ID", "Invalid ID '%s'", value.GetString());
			return;
		}
		
		meObjectSelection &selection = world->GetSelectionObject();
		const meObject * const active = selection.GetActive();
		
		for(const auto &object : world->GetObjects()){
			if(object->GetID() != id){
				continue;
			}
			
			if(object != active){
				selection.Reset();
				selection.Add(object);
				selection.SetActive(object);
				world->NotifyObjectSelectionChanged();
				
				meCamera * const camera = world->GetActiveCamera();
				if(!camera->HasHostObject()){
					camera->SetPosition(object->GetPosition() + camera->GetViewMatrix()
						.TransformNormal(decDVector(0.0, 0.0, -5.0)));
				}
			}
			return;
		}
		
		igdeCommonDialogs::ErrorFormat(&pPanel.GetWPSelection(),
			"Find ID", "ID '%s' not found", value.GetString());
	}
	
	void Update() override{
		SetEnabled(pPanel.GetWorld() != nullptr);
	}
};


class cEditPosition : public cBaseEditDVectorListener{
public:
	typedef deTObjectReference<cEditPosition> Ref;
	cEditPosition(meWPSObject &panel) : cBaseEditDVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decDVector &vector, meObject *object) override{
		if(object->GetPosition().IsEqualTo(vector)){
			return {};
		}
		return meUSetObjectPosition::Ref::New(object, vector);
	}
};

class cActionResetPosition : public cBaseAction{
public:
	typedef deTObjectReference<cActionResetPosition> Ref;
	cActionResetPosition(meWPSObject &panel) : cBaseAction(panel, "Reset Position", nullptr, "Reset position to 0"){}
	
	igdeUndo::Ref OnAction(meObject *object) override{
		return !object->GetPosition().IsEqualTo(decVector(1.0f, 1.0f, 1.0f))
			? meUSetObjectPosition::Ref::New(object, decVector()) : igdeUndo::Ref();
	}
};

class cActionMenuPosition : public igdeActionContextMenu{
	meWPSObject &pPanel;
	
public:
	typedef deTObjectReference<cActionMenuPosition> Ref;
	cActionMenuPosition(meWPSObject &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown), "Position menu"),
	pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu){
		if(!pPanel.GetActiveObject()){
			return;
		}
		
		meWindowMain &windowMain = pPanel.GetWPSelection().GetWindowProperties().GetWindowMain();
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, windowMain.GetActionObjectCopyPositionX());
		helper.MenuCommand(contextMenu, windowMain.GetActionObjectCopyPositionY());
		helper.MenuCommand(contextMenu, windowMain.GetActionObjectCopyPositionZ());
		helper.MenuCommand(contextMenu, windowMain.GetActionObjectCopyPositionXZ());
		helper.MenuCommand(contextMenu, windowMain.GetActionObjectCopyPositionXYZ());
		helper.Separator(contextMenu);
		helper.MenuCommand(contextMenu, windowMain.GetActionObjectSnapToGrid());
		helper.MenuCommand(contextMenu, windowMain.GetActionObjectDropToGround());
		helper.Separator(contextMenu);
		helper.MenuCommand(contextMenu, cActionResetPosition::Ref::New(pPanel));
	}
};

class cEditRotation : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditRotation> Ref;
	cEditRotation(meWPSObject &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, meObject *object) override{
		if(object->GetRotation().IsEqualTo(vector)){
			return {};
		}
		return meUSetObjectRotation::Ref::New(object, vector);
	}
};

class cActionResetRotation : public cBaseAction{
public:
	typedef deTObjectReference<cActionResetRotation> Ref;
	cActionResetRotation(meWPSObject &panel) : cBaseAction(panel, "Reset Rotation", nullptr, "Reset rotation to 0"){}
	
	igdeUndo::Ref OnAction(meObject *object) override{
		return !object->GetRotation().IsEqualTo(decVector(1.0f, 1.0f, 1.0f))
			? meUSetObjectRotation::Ref::New(object, decVector()) : igdeUndo::Ref();
	}
};

class cActionMenuRotation : public igdeActionContextMenu{
	meWPSObject &pPanel;
	
public:
	typedef deTObjectReference<cActionMenuRotation> Ref;
	cActionMenuRotation(meWPSObject &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown), "Rotation menu"),
	pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu){
		if(!pPanel.GetActiveObject()){
			return;
		}
		
		meWindowMain &windowMain = pPanel.GetWPSelection().GetWindowProperties().GetWindowMain();
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, windowMain.GetActionObjectCopyRotationX());
		helper.MenuCommand(contextMenu, windowMain.GetActionObjectCopyRotationY());
		helper.MenuCommand(contextMenu, windowMain.GetActionObjectCopyRotationZ());
		helper.MenuCommand(contextMenu, windowMain.GetActionObjectCopyRotationXYZ());
		helper.Separator(contextMenu);
		helper.MenuCommand(contextMenu, windowMain.GetActionObjectRotateL45());
		helper.MenuCommand(contextMenu, windowMain.GetActionObjectRotateL90());
		helper.MenuCommand(contextMenu, windowMain.GetActionObjectRotateR45());
		helper.MenuCommand(contextMenu, windowMain.GetActionObjectRotateR90());
		helper.MenuCommand(contextMenu, windowMain.GetActionObjectRotate180());
		helper.Separator(contextMenu);
		helper.MenuCommand(contextMenu, cActionResetRotation::Ref::New(pPanel));
		helper.Separator(contextMenu);
		helper.MenuCommand(contextMenu, windowMain.GetActionObjectDropToGround());
		helper.Separator(contextMenu);
		helper.MenuCommand(contextMenu, windowMain.GetActionObjectSnapToGrid());
	}
};

static decVector uniformScaleVector(const decVector &oldValue,
const decVector &newValue, igdeGDClass::eScaleModes scaleMode){
	switch(scaleMode){
	case igdeGDClass::esmUniform:{
		decVector value(oldValue);
		
		if(fabsf(newValue.x - oldValue.x) > 1e-3f){
			if(fabsf(oldValue.x) > 1e-3f){
				const float factor = newValue.x / oldValue.x;
				value.x = newValue.x;
				value.y *= factor;
				value.z *= factor;
			}
		}
		
		if(fabsf(newValue.y - oldValue.y) > 1e-3f){
			if(fabsf(oldValue.y) > 1e-3f){
				const float factor = newValue.y / oldValue.y;
				value.x *= factor;
				value.y = newValue.y;
				value.z *= factor;
			}
		}
		
		if(fabsf(newValue.z - oldValue.z) > 1e-3f){
			if(fabsf(oldValue.z) > 1e-3f){
				const float factor = newValue.z / oldValue.z;
				value.x *= factor;
				value.y *= factor;
				value.z = newValue.z;
			}
		}
		
		return value;
		}
		
	case igdeGDClass::esmFree:
		return newValue;
		
	default:
		return oldValue;
	}
}

class cEditSize : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditSize> Ref;
	cEditSize(meWPSObject &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, meObject *object) override{
		const decVector size(uniformScaleVector(object->GetSize(), vector, object->GetScaleMode()));
		if(!size.IsEqualTo(object->GetSize())){
			return meUSetObjectSize::Ref::New(object, size);
			
		}else{
			pPanel.UpdateGeometry();
			return {};
		}
	}
};

class cEditScaling : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditScaling> Ref;
	cEditScaling(meWPSObject &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, meObject *object) override{
		const decVector scaling(uniformScaleVector(object->GetScaling(), vector, object->GetScaleMode()));
		if(!scaling.IsEqualTo(object->GetScaling())){
			return meUObjectSetScaling::Ref::New(object, scaling);
			
		}else{
			pPanel.UpdateGeometry();
			return {};
		}
	}
};

class cActionResetScaling : public cBaseAction{
public:
	typedef deTObjectReference<cActionResetScaling> Ref;
	cActionResetScaling(meWPSObject &panel) : cBaseAction(panel, "Reset Scaling", nullptr, "Reset scaling to 1"){}
	
	igdeUndo::Ref OnAction(meObject *object) override{
		return !object->GetScaling().IsEqualTo(decVector(1.0f, 1.0f, 1.0f))
			? meUObjectSetScaling::Ref::New(object, decVector(1.0f, 1.0f, 1.0f)) : igdeUndo::Ref();
	}
};

class cActionMenuScale : public igdeActionContextMenu{
	meWPSObject &pPanel;
	
public:
	typedef deTObjectReference<cActionMenuScale> Ref;
	cActionMenuScale(meWPSObject &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown), "Scale menu"),
	pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu){
		if(!pPanel.GetActiveObject()){
			return;
		}
		
		meWindowMain &windowMain = pPanel.GetWPSelection().GetWindowProperties().GetWindowMain();
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, windowMain.GetActionObjectCopyScaleX());
		helper.MenuCommand(contextMenu, windowMain.GetActionObjectCopyScaleY());
		helper.MenuCommand(contextMenu, windowMain.GetActionObjectCopyScaleZ());
		helper.MenuCommand(contextMenu, windowMain.GetActionObjectCopyScaleXYZ());
		helper.Separator(contextMenu);
		helper.MenuCommand(contextMenu, cActionResetScaling::Ref::New(pPanel));
	}
};


class cBaseActionTexture : public cBaseAction{
public:
	typedef deTObjectReference<cBaseActionTexture> Ref;
	cBaseActionTexture(meWPSObject &panel, const char *text, igdeIcon *icon, const char *description) :
	cBaseAction(panel, text, icon, description){}
	
	igdeUndo::Ref OnAction(meObject *object) override{
		meObjectTexture * const texture = object->GetActiveTexture();
		return texture ? OnActionTexture(object, texture) : igdeUndo::Ref();
	}
	
	virtual igdeUndo::Ref OnActionTexture(meObject *object, meObjectTexture *texture) = 0;
};

class cBaseTextFieldListenerTexture : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cBaseTextFieldListenerTexture> Ref;
	cBaseTextFieldListenerTexture(meWPSObject &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, meObject *object) override{
		meObjectTexture * const texture = object->GetActiveTexture();
		return texture ? OnChangedTexture(textField, object, texture) : igdeUndo::Ref();
	}
	
	virtual igdeUndo::Ref OnChangedTexture(igdeTextField *textField, meObject *object, meObjectTexture *texture) = 0;
};

class cBaseEditVector2ListenerTexture : public igdeEditVector2Listener{
protected:
	meWPSObject &pPanel;
	typedef deTObjectReference<cBaseEditVector2ListenerTexture> Ref;
	cBaseEditVector2ListenerTexture(meWPSObject &panel) : pPanel(panel){}
	
	virtual void OnVector2Changed(igdeEditVector2 *editVector2){
		meObjectTexture * const texture = pPanel.GetActiveTexture();
		if(!texture){
			return;
		}
		igdeUndo::Ref undo(OnChangedTexture(editVector2->GetVector2(), pPanel.GetActiveObject(), texture));
		if(undo){
			pPanel.GetWorld()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChangedTexture(const decVector2 &vector, meObject *object, meObjectTexture *texture) = 0;
};



class cComboTexture : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboTexture> Ref;
	cComboTexture(meWPSObject &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox *comboBox, meObject *object){
		if(comboBox->GetSelectedItem()){
			object->SetActiveTexture((meObjectTexture*)comboBox->GetSelectedItem()->GetData());
		}
		return {};
	}
};

class cActionTextureAddModel : public cBaseAction{
	const decString pTextureName;
	
public:
	typedef deTObjectReference<cActionTextureAddModel> Ref;
	cActionTextureAddModel(meWPSObject &panel, const char *name) :
	cBaseAction(panel, name, nullptr, "Add model texture"), pTextureName(name){}
	
	igdeUndo::Ref OnAction(meObject *object) override{
		meObjectTexture::Ref texture;
		meHelpers::CreateTexture(texture, object, pTextureName);
		
		object->GetWorld()->GetUndoSystem()->Add(meUObjectAddTexture::Ref::New(object, texture));
		
		object->SetActiveTexture(texture);
		return {};
	}
	
	void Update() override{
		SetEnabled(pPanel.GetActiveObject() && !pPanel.GetActiveObject()->HasTextureNamed(pTextureName));
	}
};

class cActionTextureRemoveModel : public cBaseAction{
	const decString pTextureName;
	
public:
	typedef deTObjectReference<cActionTextureRemoveModel> Ref;
	cActionTextureRemoveModel(meWPSObject &panel, const char *name) :
	cBaseAction(panel, name, nullptr, "Remove model texture"), pTextureName(name){}
	
	igdeUndo::Ref OnAction(meObject*) override{
		return {};
	}
	
	void Update() override{
		SetEnabled(pPanel.GetActiveObject() && pPanel.GetActiveObject()->HasTextureNamed(pTextureName));
	}
};

class cActionMenuTexture : public igdeActionContextMenu{
	meWPSObject &pPanel;
	
public:
	typedef deTObjectReference<cActionMenuTexture> Ref;
	cActionMenuTexture(meWPSObject &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown), "Texture menu"),
	pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeMenuCascade &contextMenu){
		const meObject * const object = pPanel.GetActiveObject();
		if(!object){
			return;
		}
		
		decStringList textureNames;
		object->GetModelTextureNameList(textureNames);
		textureNames.SortAscending();
		
		igdeEnvironment &env = pPanel.GetEnvironment();
		igdeUIHelper &helper = env.GetUIHelper();
		igdeMenuCascade::Ref cascade;
		int count, i;
		
		// add texture
		helper.MenuCommand(contextMenu, pPanel.GetActionTextureAddCustom());
		helper.MenuCommand(contextMenu, pPanel.GetActionTextureAddAllModel());
		
		cascade = igdeMenuCascade::Ref::New(env, "Add From Model",
			env.GetStockIcon(igdeEnvironment::esiPlus));
		contextMenu.AddChild(cascade);
		
		count = textureNames.GetCount();
		for(i=0; i<count; i++){
			const decString &texture = textureNames.GetAt(i);
			helper.MenuCommand(cascade, cActionTextureAddModel::Ref::New(pPanel, texture));
		}
		
		// remove texture
		helper.MenuSeparator(contextMenu);
		helper.MenuCommand(contextMenu, pPanel.GetActionTextureRemove());
		helper.MenuCommand(contextMenu, pPanel.GetActionTextureRemoveAll());
		helper.MenuCommand(contextMenu, pPanel.GetActionTextureRemoveAllNotModel());
		
		cascade = igdeMenuCascade::Ref::New(env, "Remove From Model",
			env.GetStockIcon(igdeEnvironment::esiMinus));
		contextMenu.AddChild(cascade);
		
		count = textureNames.GetCount();
		for(i=0; i<count; i++){
			const decString &texture = textureNames.GetAt(i);
			helper.MenuCommand(cascade, cActionTextureRemoveModel::Ref::New(pPanel, texture));
		}
		
		// copy
		helper.MenuSeparator(contextMenu);
		helper.MenuCommand(contextMenu, pPanel.GetActionTextureCopyToSelected());
		helper.MenuCommand(contextMenu, pPanel.GetActionTextureCloneToSelected());
	}
};

class cActionTextureAddCustom : public cBaseAction{
public:
	typedef deTObjectReference<cActionTextureAddCustom> Ref;
	cActionTextureAddCustom(meWPSObject &panel) : cBaseAction(panel, "Add...",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add texture"){}
	
	igdeUndo::Ref OnAction(meObject *object) override{
		decString name;
		while(igdeCommonDialogs::GetString(&pPanel, "Add Texture", "Name:", name)){
			if(object->HasTextureNamed(name)){
				igdeCommonDialogs::Error(&pPanel, "Add Texture", "Texture name exists already");
				continue;
			}
			
			meObjectTexture::Ref texture;
			meHelpers::CreateTexture(texture, object, name);
			
			pPanel.GetWorld()->GetUndoSystem()->Add(meUObjectAddTexture::Ref::New(object, texture));
			object->SetActiveTexture(texture);
			break;
		}
		return {};
	}
	
	void Update() override{
		SetEnabled(pPanel.GetActiveObject());
	}
};

class cActionTextureAddAllModel : public cBaseAction{
public:
	typedef deTObjectReference<cActionTextureAddAllModel> Ref;
	cActionTextureAddAllModel(meWPSObject &panel) : cBaseAction(panel, "Add All From Model",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add all textures from model"){}
	
	igdeUndo::Ref OnAction(meObject *object) override{
		const meUObjectAddUsedTextures::Ref undo = meUObjectAddUsedTextures::Ref::New(object);
		return undo->GetTextureNameList().IsNotEmpty() ? undo : igdeUndo::Ref();
	}
	
	void Update() override{
		const meObject * const object = pPanel.GetActiveObject();
		bool enabled = false;
		
		if(object){
			decStringList textureNames;
			object->GetModelTextureNameList(textureNames);
			const int count = textureNames.GetCount();
			int i;
			
			for(i=0; i<count; i++){
				if(!object->HasTextureNamed(textureNames.GetAt(i))){
					enabled = true;
					break;
				}
			}
		}
		
		SetEnabled(enabled);
	}
};

class cActionTextureRemove : public cBaseActionTexture{
public:
	typedef deTObjectReference<cActionTextureRemove> Ref;
	cActionTextureRemove(meWPSObject &panel) : cBaseActionTexture(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove texture"){}
	
	igdeUndo::Ref OnActionTexture(meObject*, meObjectTexture *texture) override{
		return meUObjectRemoveTexture::Ref::New(texture);
	}
	
	void Update() override{
		SetEnabled(pPanel.GetActiveTexture());
	}
};

class cActionTextureRemoveAll : public cBaseAction{
public:
	typedef deTObjectReference<cActionTextureRemoveAll> Ref;
	cActionTextureRemoveAll(meWPSObject &panel) : cBaseAction(panel, "Remove All",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove all textures"){}
	
	igdeUndo::Ref OnAction(meObject *object) override{
		return object->GetTextures().GetCount() > 0 ? meUObjectRemoveAllTextures::Ref::New(object) : meUObjectRemoveAllTextures::Ref();
	}
	
	void Update() override{
		SetEnabled(pPanel.GetActiveObject() && pPanel.GetActiveObject()->GetTextures().GetCount() > 0);
	}
};

class cActionTextureRemoveAllNotModel : public cBaseAction{
public:
	typedef deTObjectReference<cActionTextureRemoveAllNotModel> Ref;
	cActionTextureRemoveAllNotModel(meWPSObject &panel) : cBaseAction(panel, "Remove All Not From Model",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove all textures not from model"){}
	
	igdeUndo::Ref OnAction(meObject *object) override{
		return meUObjectRemoveUnusedTextures::Ref::New(object);
	}
	
	void Update() override{
		const meObject * const object = pPanel.GetActiveObject();
		bool enabled = false;
		
		if(object){
			decStringList textureNames;
			object->GetModelTextureNameList(textureNames);
			const int count = textureNames.GetCount();
			int i;
			
			for(i=0; i<count; i++){
				if(object->HasTextureNamed(textureNames.GetAt(i))){
					enabled = true;
					break;
				}
			}
		}
		
		SetEnabled(enabled);
	}
};

class cActionTextureCopyToSelected : public cBaseActionTexture{
public:
	typedef deTObjectReference<cActionTextureCopyToSelected> Ref;
	cActionTextureCopyToSelected(meWPSObject &panel) : cBaseActionTexture(panel,
		"Copy To Selected", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
			"Copy texture from active object to selected objects"){}
	
	igdeUndo::Ref OnActionTexture(meObject*, meObjectTexture *texture) override{
		const meObject::List &list = pPanel.GetWorld()->GetSelectionObject().GetSelected();
		return list.IsNotEmpty() ? meUObjectTextureCopyToSelected::Ref::New(list, texture) : meUObjectTextureCopyToSelected::Ref();
	}
	
	void Update() override{
		SetEnabled(pPanel.GetActiveTexture() && pPanel.GetWorld()->GetSelectionObject().GetSelected().GetCount() > 1);
	}
};

class cActionTextureCloneToSelected : public cBaseAction{
public:
	typedef deTObjectReference<cActionTextureCloneToSelected> Ref;
	cActionTextureCloneToSelected(meWPSObject &panel) : cBaseAction(panel, "Clone To Selected",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
			"Clone textures from active object to selected objects"){}
	
	igdeUndo::Ref OnAction(meObject *object) override{
		meObject::List list(pPanel.GetWorld()->GetSelectionObject().GetSelected());
		list.Remove(object);
		if(list.IsEmpty()){
			return {};
		}
		
		return meUObjectCloneTexturesToSelected::Ref::New(list, object->GetTextures());
	}
	
	void Update() override{
		SetEnabled(pPanel.GetActiveObject() && pPanel.GetWorld()->GetSelectionObject().GetSelected().GetCount() > 1);
	}
};


class cEditObjectProperties : public meWPPropertyList {
	meWPSObject &pPanel;
	
public:
	typedef deTObjectReference<cEditObjectProperties> Ref;
	cEditObjectProperties(meWPSObject &panel) : meWPPropertyList(panel.GetEnvironment()), pPanel(panel){}
	
	virtual decString GetGDDefaultValue(const char *key) const{
		const meObject * const object = pPanel.GetActiveObject();
		decString value;
		if(object && object->GetGDClass()){
			object->GetGDClass()->GetDefaultPropertyValue(key, value);
		}
		return value;
	}
	
	const igdeGDProperty *GetGDProperty(const char *key) const override{
		const meObject * const object = pPanel.GetActiveObject();
		if(!object || !object->GetGDClass()){
			return {};
		}
		
		const igdeGDClassManager &clsmgr = *pPanel.GetGameDefinition()->GetClassManager();
		const decStringList &attachBehaviors = object->GetAttachBehaviors();
		const int count = attachBehaviors.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			const igdeGDClass * const gdclass = clsmgr.GetNamed(attachBehaviors.GetAt(i));
			if(!gdclass){
				continue;
			}
			
			const igdeGDProperty * const property = gdclass->GetPropertyNamed(key);
			if(property){
				return property;
			}
		}
		
		return object->GetGDClass()->GetPropertyNamed(key);
	}
	
	virtual decStringSet GetGDPropertyKeys() const{
		const meObject * const object = pPanel.GetActiveObject();
		decStringSet keys;
		if(object){
			if(object->GetGDClass()){
				object->GetGDClass()->AddPropertyNames(keys, true);
			}
			
			const decStringList &attachBehaviors = object->GetAttachBehaviors();
			const int attachBehaviorCount = attachBehaviors.GetCount();
			if(attachBehaviorCount > 0 && pPanel.GetGameDefinition()){
				const igdeGDClassManager &classManager = *pPanel.GetGameDefinition()->GetClassManager();
				int i;
				
				for(i=0; i<attachBehaviorCount; i++){
					const igdeGDClass * const gdclass = classManager.GetNamed(attachBehaviors.GetAt(i));
					if(gdclass){
						gdclass->AddPropertyNames(keys, true);
					}
				}
			}
		}
		return keys;
	}
	
	virtual void OnPropertySelected(const decString &key, const decString &){
		meObject * const object = pPanel.GetActiveObject();
		if(object){
			object->SetActiveProperty(key);
		}
		pPanel.UpdateLight();
		pPanel.UpdateIdentifierLists();
	}
	
	void AddContextMenuEntries(igdeUIHelper &helper, igdeMenuCascade &menu) override{
		helper.MenuSeparator(menu);
		helper.MenuCommand(menu, pPanel.GetActionPropCopyToSel());
		helper.MenuCommand(menu, pPanel.GetActionPropCloneToSel());
		helper.MenuCommand(menu, pPanel.GetActionPropRemoveFromSel());
	}
	
	virtual igdeUndo::Ref UndoAddProperty(const decString &key, const decString &value){
		meObject * const object = pPanel.GetActiveObject();
		return object ? meUObjectAddProperty::Ref::New(object, key, value) : igdeUndo::Ref();
	}
	
	virtual igdeUndo::Ref UndoRemoveProperty(const decString &key){
		meObject * const object = pPanel.GetActiveObject();
		return object ? meUObjectRemoveProperty::Ref::New(object, key, object->GetProperties().GetAt(key)) : igdeUndo::Ref();
	}
	
	virtual igdeUndo::Ref UndoSetProperty(const decString &key, const decString &oldValue, const decString &newValue){
		meObject * const object = pPanel.GetActiveObject();
		return object ? meUObjectSetProperty::Ref::New(object, key, oldValue, newValue) : igdeUndo::Ref();
	}
	
	virtual igdeUndo::Ref UndoSetProperties(const decStringDictionary &properties){
		meObject * const object = pPanel.GetActiveObject();
		return object ? meUObjectSetProperties::Ref::New(object, properties) : igdeUndo::Ref();
	}
};

class cActionPropCopyToSel : public cBaseAction{
public:
	typedef deTObjectReference<cActionPropCopyToSel> Ref;
	cActionPropCopyToSel(meWPSObject &panel) : cBaseAction(panel, "Copy To Selected",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
			"Copy property from active object to all selected objects"){}
	
	igdeUndo::Ref OnAction(meObject *object) override{
		meObject::List list(pPanel.GetWorld()->GetSelectionObject().GetSelected());
		const decString &property = pPanel.GetActiveProperty();
		list.Remove(object);
		return !property.IsEmpty() && list.IsNotEmpty() ? meUObjectPropertyCopyToSelected::Ref::New(
			list, property, object->GetProperties().GetAt(property)) : igdeUndo::Ref();
	}
	
	void Update() override{
		SetEnabled(pPanel.GetActiveObject() && !pPanel.GetActiveProperty().IsEmpty()
			&& pPanel.GetWorld()->GetSelectionObject().GetSelected().GetCount() > 1);
	}
};

class cActionPropRemoveFromSel : public cBaseAction{
public:
	typedef deTObjectReference<cActionPropRemoveFromSel> Ref;
	cActionPropRemoveFromSel(meWPSObject &panel) : cBaseAction(panel, "Remove From Selected",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
			"Remove property from all selected objects"){}
	
	igdeUndo::Ref OnAction(meObject*) override{
		const meObject::List &list = pPanel.GetWorld()->GetSelectionObject().GetSelected();
		const decString &property = pPanel.GetActiveProperty();
		return !property.IsEmpty() && list.IsNotEmpty() ? meUObjectPropertyRemoveFromSelected::Ref::New(list, property) : igdeUndo::Ref();
	}
	
	void Update() override{
		SetEnabled(pPanel.GetActiveObject() && !pPanel.GetActiveProperty().IsEmpty()
			&& pPanel.GetWorld()->GetSelectionObject().GetSelected().IsNotEmpty());
	}
};

class cActionPropCloneToSel : public cBaseAction{
public:
	typedef deTObjectReference<cActionPropCloneToSel> Ref;
	cActionPropCloneToSel(meWPSObject &panel) : cBaseAction(panel, "Clone To Selected",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiCopy),
			"Clone all properties from active object to all selected objects"){}
	
	igdeUndo::Ref OnAction(meObject *object) override{
		meObject::List list(pPanel.GetWorld()->GetSelectionObject().GetSelected());
		list.Remove(object);
		return list.IsNotEmpty() ? meUObjectClonePropertiesToSelected::Ref::New(list, object->GetProperties()) : meUObjectClonePropertiesToSelected::Ref();
	}
	
	void Update() override{
		SetEnabled(pPanel.GetActiveObject() && pPanel.GetWorld()->GetSelectionObject().GetSelected().GetCount() > 1);
	}
};


class cActionAddAttachBehavior : public cBaseAction{
public:
	typedef deTObjectReference<cActionAddAttachBehavior> Ref;
	cActionAddAttachBehavior(meWPSObject &panel) : cBaseAction(panel, "Add Attachable Behavior...",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add attachable behavior"){}
	
	igdeUndo::Ref OnAction(meObject *object) override{
		decStringList names;
		if(pPanel.GetGameDefinition()){
			pPanel.GetGameDefinition()->GetClassManager()->GetClasses().Visit([&](const igdeGDClass &c){
				if(c.GetIsAttachableBehavior()){
					names.Add(c.GetName());
				}
			});
			
			names.SortAscending();
		}
		
		decString name;
		if(!igdeCommonDialogs::GetString(&pPanel, "Add Attachable Behavior", "Behavior:", name, names)){
			return {};
		}
		
		decStringList list(object->GetAttachBehaviors());
		list.Add(name);
		return meUObjectSetAttachBehaviors::Ref::New(object, list);
	}
	
	void Update() override{
		SetEnabled(pPanel.GetActiveObject());
	}
};

class cActionRemoveAttachBehavior : public cBaseAction{
public:
	typedef deTObjectReference<cActionRemoveAttachBehavior> Ref;
	cActionRemoveAttachBehavior(meWPSObject &panel) : cBaseAction(panel, "Remove Attachable Behavior",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove attachable behavior"){}
	
	igdeUndo::Ref OnAction(meObject *object) override{
		decStringList list(object->GetAttachBehaviors());
		list.RemoveFrom(object->GetActiveAttachBehavior());
		return meUObjectSetAttachBehaviors::Ref::New(object, list);
	}
	
	void Update() override{
		SetEnabled(pPanel.GetActiveObject()
			&& pPanel.GetActiveObject()->GetActiveAttachBehavior() != -1);
	}
};

class cActionRemoveAllAttachBehaviors : public cBaseAction{
public:
	typedef deTObjectReference<cActionRemoveAllAttachBehaviors> Ref;
	cActionRemoveAllAttachBehaviors(meWPSObject &panel) : cBaseAction(panel,
		"Remove All Attachable Behaviors",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus),
		"Remove all attachable behaviors"){}
	
	igdeUndo::Ref OnAction(meObject *object) override{
		if(pPanel.GetActiveObject()->GetAttachBehaviors().IsEmpty()){
			return {};
		}
		
		return meUObjectSetAttachBehaviors::Ref::New(object, decStringList());
	}
	
	void Update() override{
		SetEnabled(pPanel.GetActiveObject()
			&& pPanel.GetActiveObject()->GetAttachBehaviors().IsNotEmpty());
	}
};

class cListAttachBehavior : public igdeListBoxListener{
	meWPSObject &pPanel;
	bool &pPreventUpdate;
	
public:
	typedef deTObjectReference<cListAttachBehavior> Ref;
	cListAttachBehavior(meWPSObject &panel, bool &preventUpdate) :
		pPanel(panel), pPreventUpdate(preventUpdate){}
	
	void OnSelectionChanged(igdeListBox *listBox) override{
		if(pPreventUpdate){
			return;
		}
		
		meObject * const object = pPanel.GetActiveObject();
		if(object){
			object->SetActiveAttachBehavior(listBox->GetSelection());
		}
	}
	
	void AddContextMenuEntries(igdeListBox *listBox, igdeMenuCascade &menu) override{
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelper();
		helper.MenuCommand(menu, cActionAddAttachBehavior::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionRemoveAttachBehavior::Ref::New(pPanel));
		helper.MenuCommand(menu, cActionRemoveAllAttachBehaviors::Ref::New(pPanel));
	};
};


class cEditTextureSkin : public igdeEditPathListener{
	meWPSObject &pPanel;
	
public:
	typedef deTObjectReference<cEditTextureSkin> Ref;
	cEditTextureSkin(meWPSObject &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		meObjectTexture * const texture = pPanel.GetActiveTexture();
		if(!texture || texture->GetSkinPath() == editPath->GetPath()){
			return;
		}
		
		pPanel.GetWorld()->GetUndoSystem()->Add(meUObjectTextureSetSkin::Ref::New(
			texture, editPath->GetPath()));
	}
};

class cEditTextureTCOffset : public cBaseEditVector2ListenerTexture{
public:
	typedef deTObjectReference<cEditTextureTCOffset> Ref;
	cEditTextureTCOffset(meWPSObject &panel) : cBaseEditVector2ListenerTexture(panel){}
	
	virtual igdeUndo::Ref OnChangedTexture(const decVector2 &vector, meObject*, meObjectTexture *texture){
		return !texture->GetTexCoordOffset().IsEqualTo(vector)
			? meUObjectTextureTCOffset::Ref::New(texture, vector) : igdeUndo::Ref();
	}
};

class cEditTextureTCScaling : public cBaseEditVector2ListenerTexture{
public:
	typedef deTObjectReference<cEditTextureTCScaling> Ref;
	cEditTextureTCScaling(meWPSObject &panel) : cBaseEditVector2ListenerTexture(panel){}
	
	virtual igdeUndo::Ref OnChangedTexture(const decVector2 &vector, meObject*, meObjectTexture *texture){
		return !texture->GetTexCoordScaling().IsEqualTo(vector)
			? meUObjectTextureTCScaling::Ref::New(texture, vector) : igdeUndo::Ref();
	}
};

class cEditTextureTCRotation : public cBaseTextFieldListenerTexture{
public:
	typedef deTObjectReference<cEditTextureTCRotation> Ref;
	cEditTextureTCRotation(meWPSObject &panel) : cBaseTextFieldListenerTexture(panel){}
	
	virtual igdeUndo::Ref OnChangedTexture(igdeTextField *textField, meObject*, meObjectTexture *texture){
		const float value = textField->GetFloat();
		return fabsf(texture->GetTexCoordRotation() - value) > FLOAT_SAFE_EPSILON
			? meUObjectTextureTCRotation::Ref::New(texture, value) : igdeUndo::Ref();
	}
};

class cEditTextureColorTint : public igdeColorBoxListener{
	meWPSObject &pPanel;
	
public:
	typedef deTObjectReference<cEditTextureColorTint> Ref;
	cEditTextureColorTint(meWPSObject &panel) : pPanel(panel){}
	
	virtual void OnColorChanged(igdeColorBox *colorBox){
		meObjectTexture * const texture = pPanel.GetActiveTexture();
		if(!texture || texture->GetColorTint().IsEqualTo(colorBox->GetColor())){
			return;
		}
		
		pPanel.GetWorld()->GetUndoSystem()->Add(meUObjectTextureColorTint::Ref::New(
			texture, colorBox->GetColor()));
	}
};


class cActionShowMissingTextures : public cBaseAction{
public:
	typedef deTObjectReference<cActionShowMissingTextures> Ref;
	cActionShowMissingTextures(meWPSObject &panel) : cBaseAction(panel, "Show Missing Textures",
		nullptr, "Show mising textures using placeholder texture"){}
	
	igdeUndo::Ref OnAction(meObject *object) override{
		object->SetShowMissingTextures(!object->GetShowMissingTextures());
		return {};
	}
};


class cEditTextureProperties : public meWPPropertyList {
	meWPSObject &pPanel;
	
public:
	typedef deTObjectReference<cEditTextureProperties> Ref;
	cEditTextureProperties(meWPSObject &panel) : meWPPropertyList(panel.GetEnvironment()), pPanel(panel){}
	
	virtual decString GetGDDefaultValue(const char *key) const{
		const meObjectTexture * const texture = pPanel.GetActiveTexture();
		decString value;
		if(texture && texture->GetObject()->GetGDClass()){
			texture->GetObject()->GetGDClass()->GetDefaultTexturePropertyValue(key, value);
		}
		return value;
	}
	
	const igdeGDProperty *GetGDProperty(const char *key) const override{
		const meObjectTexture * const texture = pPanel.GetActiveTexture();
		return texture && texture->GetObject()->GetGDClass()
			? texture->GetObject()->GetGDClass()->GetTexturePropertyNamed(key) : nullptr;
	}
	
	virtual decStringSet GetGDPropertyKeys() const{
		const meObjectTexture * const texture = pPanel.GetActiveTexture();
		decStringSet keys;
		if(texture && texture->GetObject()->GetGDClass()){
			texture->GetObject()->GetGDClass()->AddTexturePropertyNames(keys, true);
		}
		return keys;
	}
	
	virtual void OnPropertySelected(const decString &key, const decString &){
		meObjectTexture * const texture = pPanel.GetActiveTexture();
		if(texture){
			texture->SetActiveProperty(key);
		}
	}
	
	virtual igdeUndo::Ref UndoAddProperty(const decString &key, const decString &value){
		meObjectTexture * const texture = pPanel.GetActiveTexture();
		return texture ? meUObjTexAddProperty::Ref::New(texture, key, value) : igdeUndo::Ref();
	}
	
	virtual igdeUndo::Ref UndoRemoveProperty(const decString &key){
		meObjectTexture * const texture = pPanel.GetActiveTexture();
		return texture ? meUObjTexRemoveProperty::Ref::New(texture, key, texture->GetProperties().GetAt(key)) : igdeUndo::Ref();
	}
	
	virtual igdeUndo::Ref UndoSetProperty(const decString &key, const decString &oldValue, const decString &newValue){
		meObjectTexture * const texture = pPanel.GetActiveTexture();
		return texture ? meUObjTexSetProperty::Ref::New(texture, key, oldValue, newValue) : igdeUndo::Ref();
	}
	
	virtual igdeUndo::Ref UndoSetProperties(const decStringDictionary &properties){
		meObjectTexture * const texture = pPanel.GetActiveTexture();
		return texture ? meUObjTexSetProperties::Ref::New(texture, properties) : igdeUndo::Ref();
	}
};


class cLightSlider : public igdeEditSliderTextListener{
protected:
	meWPSObject &pPanel;
	const igdeGDCLight::eProperties pProperty;
	
public:
	typedef deTObjectReference<cLightSlider> Ref;
	cLightSlider(meWPSObject &panel, igdeGDCLight::eProperties property) :
		pPanel(panel), pProperty(property){}
	
	virtual void OnSliderTextValueChanged(igdeEditSliderText *sliderText){
		decString value;
		value.Format("%g", sliderText->GetValue());
		pPanel.SlideLightProperty(pProperty, value, false);
	}
	
	virtual void OnSliderTextValueChanging(igdeEditSliderText *sliderText){
		decString value;
		value.Format("%g", sliderText->GetValue());
		pPanel.SlideLightProperty(pProperty, value, true);
	}
};

class cLightColor : public igdeColorBoxListener{
protected:
	meWPSObject &pPanel;
	
public:
	typedef deTObjectReference<cLightColor> Ref;
	cLightColor(meWPSObject &panel) : pPanel(panel){}
	
	virtual void OnColorChanged(igdeColorBox *colorBox){
		igdeCodecPropertyString codec;
		decString value;
		codec.EncodeColor3(colorBox->GetColor(), value);
		pPanel.SlideLightProperty(igdeGDCLight::epColor, value, false);
	}
};

class cLightIntForDistSlider : public igdeEditSliderTextListener{
protected:
	meWPSObject &pPanel;
	
public:
	typedef deTObjectReference<cLightIntForDistSlider> Ref;
	cLightIntForDistSlider(meWPSObject &panel) : pPanel(panel){}
	
	virtual void OnSliderTextValueChanged(igdeEditSliderText*){
		pPanel.UpdateIntensityForDistance();
	}
	
	virtual void OnSliderTextValueChanging(igdeEditSliderText*){
		pPanel.UpdateIntensityForDistance();
	}
};

}


// Class meWPSObject
/////////////////////

// Constructor, destructor
////////////////////////////

meWPSObject::meWPSObject(meWPSelection &wpselection) :
igdeContainerScroll(wpselection.GetEnvironment(), false, true),
pWPSelection(wpselection),
pPreventUpdate(false)
{
	igdeEnvironment &env = wpselection.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, formLine;
	
	pListener = meWPSObjectListener::Ref::New(*this);
	
	
	pActionMenuClass = cActionMenuClass::Ref::New(*this);
	pActionClassBrowse = cActionClassBrowse::Ref::New(*this);
	pActionClassEdit = cActionClassEdit::Ref::New(*this);
	
	pActionIdClass = cActionMenuId::Ref::New(*this);
	pActionIdFind = cActionIdFind::Ref::New(*this);
	
	pActionMenuPosition = cActionMenuPosition::Ref::New(*this);
	pActionMenuRotation = cActionMenuRotation::Ref::New(*this);
	pActionMenuScale = cActionMenuScale::Ref::New(*this);
	
	pActionMenuTexture = cActionMenuTexture::Ref::New(*this);
	pActionTextureAddCustom = cActionTextureAddCustom::Ref::New(*this);
	pActionTextureAddAllModel = cActionTextureAddAllModel::Ref::New(*this);
	pActionTextureRemove = cActionTextureRemove::Ref::New(*this);
	pActionTextureRemoveAll = cActionTextureRemoveAll::Ref::New(*this);
	pActionTextureRemoveAllNotModel = cActionTextureRemoveAllNotModel::Ref::New(*this);
	pActionTextureCopyToSelected = cActionTextureCopyToSelected::Ref::New(*this);
	pActionTextureCloneToSelected = cActionTextureCloneToSelected::Ref::New(*this);
	
	pActionPropCopyToSel = cActionPropCopyToSel::Ref::New(*this);
	pActionPropRemoveFromSel = cActionPropRemoveFromSel::Ref::New(*this);
	pActionPropCloneToSel = cActionPropCloneToSel::Ref::New(*this);
	
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	groupBox = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaX, igdeContainerFlow::esLast, 10);
	content->AddChild(groupBox);
	helper.Label(groupBox, "Selected:");
	helper.EditInteger(groupBox, "Number of selected objects", 3, pEditSelCount, {});
	pEditSelCount->SetEditable(false);
	helper.Label(groupBox, "Active:");
	helper.EditSpinInteger(groupBox, "Active object to edit", 0, 0, pSpinActive, cSpinActive::Ref::New(*this));
	
	
	// geometry
	helper.GroupBox(content, groupBox, "Geometry:");
	
	helper.FormLineStretchFirst(groupBox, "Class:", "Name of the active object class.", formLine);
	helper.ComboBoxFilter(formLine, true, "Name of the active object class.", pCBClass, cComboClass::Ref::New(*this));
	pCBClass->SetDefaultSorter();
	helper.Button(formLine, pBtnClassMenu, pActionMenuClass);
	pActionMenuClass->SetWidget(pBtnClassMenu);
	
	helper.FormLineStretchFirst(groupBox, "ID:", "Unique object ID.", formLine);
	helper.EditString(formLine, "Unique object ID", pEditID, {});
	pEditID->SetEditable(false);
	helper.Button(formLine, pBtnIdMenu, pActionIdClass);
	pActionIdClass->SetWidget(pBtnIdMenu);
	
	helper.EditString(groupBox, "Attach:", "ID of object to attach to", pEditAttach, {});
	pEditAttach->SetEditable(false);
	
	helper.FormLineStretchFirst(groupBox, "Position:", "Position of the object.", formLine);
	helper.EditDVector(formLine, "Position of the object.", pEditPosition, cEditPosition::Ref::New(*this));
	helper.Button(formLine, pBtnPositionMenu, pActionMenuPosition);
	pActionMenuPosition->SetWidget(pBtnPositionMenu);
	
	helper.FormLineStretchFirst(groupBox, "Rotation:", "Rotation of the object.", formLine);
	helper.EditVector(formLine, "Rotation of the object.", pEditRotation, cEditRotation::Ref::New(*this));
	helper.Button(formLine, pBtnRotationMenu, pActionMenuRotation);
	pActionMenuRotation->SetWidget(pBtnRotationMenu);
	
	helper.FormLineStretchFirst(groupBox, "Scaling:", "Scaling of the object.", formLine);
	helper.EditVector(formLine, "Scaling of the object.", pEditScaling, cEditScaling::Ref::New(*this));
	helper.Button(formLine, pBtnScaleMenu, pActionMenuScale);
	pActionMenuScale->SetWidget(pBtnScaleMenu);
	
	helper.EditVector(groupBox, "Size:", "Size of the object.", pEditSize, cEditSize::Ref::New(*this));
	
	
	// properties
	helper.GroupBoxFlow(content, groupBox, "Properties:");
	
	pEditProperties = cEditObjectProperties::Ref::New(*this);
	groupBox->AddChild(pEditProperties);
	
	
	// attachable behaviors
	helper.GroupBoxFlow(content, groupBox, "Attachable Behaviors:", true, true);
	
	helper.ListBox(groupBox, 3, "Attachable behaviors of the object.",
		pListAttachBehaviors, cListAttachBehavior::Ref::New(*this, pPreventUpdate));
	
	
	// display options
	helper.GroupBox(content, groupBox, "Display Options", true);
	
	helper.CheckBox(groupBox, pChkShowMissingTextures, cActionShowMissingTextures::Ref::New(*this));
	
	
	// texture
	helper.GroupBox(content, groupBox, "Texture:", true);
	
	helper.FormLineStretchFirst(groupBox, "Texture:", "Texture to edit.", formLine);
	helper.ComboBox(formLine, "Texture to edit.", pCBTexture, cComboTexture::Ref::New(*this));
	pCBTexture->SetDefaultSorter();
	helper.Button(formLine, pBtnTextureMenu, pActionMenuTexture);
	pActionMenuTexture->SetWidget(pBtnTextureMenu);
	
	helper.EditPath(groupBox, "Skin:", "Skin to use for the texture",
		igdeEnvironment::efpltSkin, pEditTexSkin, cEditTextureSkin::Ref::New(*this));
	
	
	// texture transform
	helper.GroupBox(content, groupBox, "Texture Transformation:", true);
	
	helper.EditVector2(groupBox, "Translation:", "Texture coordinates translation.",
		pEditTexTCOffset, cEditTextureTCOffset::Ref::New(*this));
	helper.EditVector2(groupBox, "Scaling:", "Texture coordinates scaling.",
		pEditTexTCScaling, cEditTextureTCScaling::Ref::New(*this));
	helper.EditFloat(groupBox, "Rotation:", "Texture coordinates rotation.",
		pEditTexTCRotation, cEditTextureTCRotation::Ref::New(*this));
	helper.ColorBox(groupBox, "Tint:", "Color tint (affects renderable named 'tint').", pClrTexTint, cEditTextureColorTint::Ref::New(*this));
	
	
	// texture properties tables
	helper.GroupBoxFlow(content, groupBox, "Texture Properties:", false, true);
	
	pEditTexProperties = cEditTextureProperties::Ref::New(*this);
	groupBox->AddChild(pEditTexProperties);
	
	
	// light properties
	helper.GroupBox(content, groupBox, "Light:", true);
	
	helper.EditSliderText(groupBox, "Intensity:", "Intensity of the light.", 0.0f, 10.0f,
		5, 2, 1.0f, pSldLigInt, cLightSlider::Ref::New(*this, igdeGDCLight::epIntensity));
	helper.ColorBox(groupBox, "Color:", "Color of the light.", pClrLight, cLightColor::Ref::New(*this));
	helper.EditSliderText(groupBox, "Range:", "Range of the light in meters.", 0.0f, 10.0f,
		5, 2, 1.0f, pSldLigRange, cLightSlider::Ref::New(*this, igdeGDCLight::epRange));
	helper.EditSliderText(groupBox, "Half-Int Dist:",
		"Half intensity distance as percentage of light range.", 0.0f, 1.0f,
		5, 2, 0.1f, pSldLigHID, cLightSlider::Ref::New(*this, igdeGDCLight::epHalfIntDist));
	
	helper.EditSliderText(groupBox, "Distance:",
		"Distance to the light source to evaluate distance for.", 0.0f, 10.0f,
		5, 2, 1.0f, pSldLigDist, cLightIntForDistSlider::Ref::New(*this));
	helper.EditString(groupBox, "Intensity:",
		"Evaluated intensity at distance to light source.", pEditLigDistInt, {});
	pEditLigDistInt->SetEditable(false);
}

meWPSObject::~meWPSObject(){
	SetWorld(nullptr);
}



// Management
///////////////

void meWPSObject::SetWorld(meWorld *world){
	if(world == pWorld){
		return;
	}
	
	meWPPropertyList &editProperties = (meWPPropertyList&)(igdeWidget&)pEditProperties;
	meWPPropertyList &editTextureProperties = (meWPPropertyList&)(igdeWidget&)pEditTexProperties;
	
	if(pWorld){
		editProperties.SetClipboard(nullptr);
		editProperties.SetUndoSystem(nullptr);
		editProperties.SetTriggerTargetList(nullptr);
		editProperties.SetIdentifiers(decStringSet());
		
		editTextureProperties.SetClipboard(nullptr);
		editTextureProperties.SetUndoSystem(nullptr);
		editTextureProperties.SetTriggerTargetList(nullptr);
		editTextureProperties.SetIdentifiers(decStringSet());
		
		pWorld->RemoveNotifier(pListener);
	}
	
	pWorld = world;
	
	if(world){
		world->AddNotifier(pListener);
		editProperties.SetClipboard(&pWPSelection.GetWindowProperties().GetWindowMain().GetClipboard());
		editProperties.SetUndoSystem(world->GetUndoSystem());
		editProperties.SetTriggerTargetList(&world->GetTriggerTable());
		
		editTextureProperties.SetClipboard(&pWPSelection.GetWindowProperties().GetWindowMain().GetClipboard());
		editTextureProperties.SetUndoSystem(world->GetUndoSystem());
		editTextureProperties.SetTriggerTargetList(&world->GetTriggerTable());
	}
	
	UpdateClassList();
	
	UpdateSelection();
	UpdateEnabled();
	UpdateObject();
	UpdateGeometry();
	UpdateLight();
	UpdatePropertyKeys();
	UpdateProperties();
	UpdateAttachBehaviors();
	
	UpdateTextureList();
	UpdateTextureEnabled();
	UpdateTexture();
	UpdateTexPropertyKeys();
	UpdateTexProperties();
	
	UpdateIdentifierLists();
}



meObject *meWPSObject::GetActiveObject() const{
	return pWorld ? pWorld->GetSelectionObject().GetActive() : nullptr;
}

const decString &meWPSObject::GetActiveProperty() const{
	meObject * const object = GetActiveObject();
	return object ? object->GetActiveProperty() : vEmptyString;
}

meObjectTexture *meWPSObject::GetActiveTexture() const{
	meObject * const object = GetActiveObject();
	return object ? object->GetActiveTexture() : nullptr;
}

const decString &meWPSObject::GetActiveTexProperty() const{
	meObjectTexture * const texture = GetActiveTexture();
	return texture ? texture->GetActiveProperty() : vEmptyString;
}



void meWPSObject::UpdateClassList(){
	const decString selection(pCBClass->GetText());
	
	pCBClass->ReleaseFilterItems();
	pCBClass->RemoveAllItems();
	
	GetGameDefinition()->GetClassManager()->GetClasses().Visit([&](const igdeGDClass &c){
		if(c.GetCanInstantiate()){
			pCBClass->AddItem(c.GetName());
		}
	});
	
	pCBClass->SortItems();
	pCBClass->StoreFilterItems();
	
	pCBClass->SetText(selection);
}

void meWPSObject::UpdateSelection(){
	int active = 0;
	int count = 0;
	
	pUndoSetProperty = nullptr;
	pUndoAddProperty = nullptr;
	
	if(pWorld){
		const meObjectSelection &selection = pWorld->GetSelectionObject();
		count = selection.GetSelected().GetCount();
		if(selection.HasActive()){
			active = selection.GetSelected().IndexOf(selection.GetActive());
		}
	}
	
	pEditSelCount->SetInteger(count);
	pSpinActive->SetRange(0, count - 1);
	pSpinActive->SetValue(active);
	pSpinActive->SetEnabled(count > 0);
}

void meWPSObject::UpdateEnabled(){
	const bool enabled = GetActiveObject() != nullptr;
	
	pEditPosition->SetEnabled(enabled);
	pEditSize->SetEnabled(enabled);
	pEditScaling->SetEnabled(enabled);
	pEditRotation->SetEnabled(enabled);
	pChkShowMissingTextures->SetEnabled(enabled);
}

void meWPSObject::UpdateObject(){
	const meObject * const object = GetActiveObject();
	
	if(object){
		pCBClass->SetText(object->GetClassName());
		
	}else{
		pCBClass->ClearText();
	}
}

void meWPSObject::UpdateGeometry(){
	const meObject * const object = GetActiveObject();
	
	if(object){
		pEditID->SetText(object->GetID().ToHexString());
		
		if(object->GetAttachedTo()){
			pEditAttach->SetText(object->GetAttachedTo()->GetID().ToHexString());
			
		}else{
			pEditAttach->SetText("<none>");
		}
		
		pEditPosition->SetDVector(object->GetPosition());
		pEditSize->SetVector(object->GetSize());
		pEditScaling->SetVector(object->GetScaling());
		pEditRotation->SetVector(object->GetRotation());
		pChkShowMissingTextures->SetChecked(object->GetShowMissingTextures());
		
	}else{
		pEditID->ClearText();
		pEditAttach->ClearText();
		pEditPosition->SetDVector(decDVector());
		pEditSize->SetVector(decVector(1.0f, 1.0f, 1.0f));
		pEditScaling->SetVector(decVector(1.0f, 1.0f, 1.0f));
		pEditRotation->SetVector(decVector());
		pChkShowMissingTextures->SetChecked(false);
	}
}

void meWPSObject::UpdateLight(){
	const meObject * const object = GetActiveObject();
	igdeGDCLight *gdLight = nullptr;
	decString gdpPrefix;
	if(object && object->GetGDClass()){
		meHelpers::FindFirstLight(*object->GetGDClass(), gdpPrefix, gdLight);
	}
	
	if(gdLight && object){
		decString value;
		
		// intensity
		const decString &pnameIntensity = gdLight->GetPropertyName(igdeGDCLight::epIntensity);
		if(!pnameIntensity.IsEmpty()){
			pSldLigInt->SetEnabled(true);
			
			if(pPropertyValue(*object, gdpPrefix, pnameIntensity, value)){
				pSldLigInt->SetValue(value.ToFloat());
				
			}else{
				pSldLigInt->SetValue(gdLight->GetIntensity());
			}
			
		}else{
			pSldLigInt->SetEnabled(false);
			pSldLigInt->SetValue(1.0f);
		}
		
		// color
		const decString &pnameColor = gdLight->GetPropertyName(igdeGDCLight::epColor);
		if(!pnameColor.IsEmpty()){
			pClrLight->SetEnabled(true);
			
			if(pPropertyValue(*object, gdpPrefix, pnameColor, value)){
				try{
					decColor color;
					igdeCodecPropertyString().DecodeColor(value, color, decColor(1.0f, 1.0f, 1.0f));
					pClrLight->SetColor(color);
					
				}catch(const deException &){
					GetLogger()->LogInfoFormat("WorldEditor", "Invalid color value '%s'", value.GetString());
					pClrLight->SetColor(decColor(1.0f, 1.0f, 1.0f));
				}
				
			}else{
				pClrLight->SetColor(gdLight->GetColor());
			}
			
		}else{
			pClrLight->SetEnabled(false);
			pClrLight->SetColor(decColor(1.0f, 1.0f, 1.0f));
		}
		
		// range
		const decString &pnameRange = gdLight->GetPropertyName(igdeGDCLight::epRange);
		if(!pnameRange.IsEmpty()){
			pSldLigRange->SetEnabled(true);
			
			if(pPropertyValue(*object, gdpPrefix, pnameRange, value)){
				pSldLigRange->SetValue(value.ToFloat());
				
			}else{
				pSldLigRange->SetValue(gdLight->GetRange());
			}
			
		}else{
			pSldLigRange->SetEnabled(false);
			pSldLigRange->SetValue(10.0f);
		}
		
		// half intensity distance
		const decString &pnameHalfIntDist = gdLight->GetPropertyName(igdeGDCLight::epHalfIntDist);
		if(!pnameHalfIntDist.IsEmpty()){
			pSldLigHID->SetEnabled(true);
			
			if(pPropertyValue(*object, gdpPrefix, pnameHalfIntDist, value)){
				pSldLigHID->SetValue(value.ToFloat());
				
			}else{
				pSldLigHID->SetValue(gdLight->GetHalfIntensityDistance());
			}
			
		}else{
			pSldLigHID->SetEnabled(false);
			pSldLigHID->SetValue(0.1f);
		}
		
	}else{
		pSldLigInt->SetValue(1.0f);
		pClrLight->SetColor(decColor(1.0f, 1.0f, 1.0f));
		pSldLigRange->SetValue(10.0f);
		pSldLigHID->SetValue(0.1f);
		
		pSldLigDist->SetValue(0.0f);
		pEditLigDistInt->ClearText();
	}
	
	const bool enabled = gdLight;
	pSldLigInt->SetEnabled(enabled);
	pClrLight->SetEnabled(enabled);
	pSldLigRange->SetEnabled(enabled);
	pSldLigHID->SetEnabled(enabled);
	pSldLigDist->SetEnabled(enabled);
	pEditLigDistInt->SetEnabled(enabled);
}

void meWPSObject::SelectActiveProperty(){
	if(!GetActiveProperty().IsEmpty()){
		((meWPPropertyList&)(igdeWidget&)pEditProperties).SelectProperty(GetActiveProperty());
	}
}

void meWPSObject::UpdatePropertyKeys(){
	((meWPPropertyList&)(igdeWidget&)pEditProperties).UpdateKeys();
}

void meWPSObject::UpdateProperties(){
	((meWPPropertyList&)(igdeWidget&)pEditProperties).SetProperties(
		GetActiveObject() ? GetActiveObject()->GetProperties() : decStringDictionary());
	SelectActiveProperty();
}

void meWPSObject::UpdateAttachBehaviors(){
	const meObject * const object = GetActiveObject();
	
	pPreventUpdate = true;
	try{
		pListAttachBehaviors->RemoveAllItems();
		
		if(object){
			const decStringList &list = object->GetAttachBehaviors();
			const int count = list.GetCount();
			int i;
			
			for(i=0; i<count; i++){
				pListAttachBehaviors->AddItem(list.GetAt(i));
			}
		}
		
		SelectActiveAttachBehavior();
		pPreventUpdate = false;
		
	}catch(const deException &){
		pPreventUpdate = false;
		throw;
	}
	
	pListAttachBehaviors->SetEnabled(object);
}

void meWPSObject::SelectActiveAttachBehavior(){
	meObject * const object = GetActiveObject();
	if(object){
		pListAttachBehaviors->SetSelection(object->GetActiveAttachBehavior());
	}
}


void meWPSObject::UpdateIntensityForDistance(){
	const float intensity = pSldLigInt->GetValue();
	const float range = pSldLigRange->GetValue();
	const float halfIntDist = pSldLigHID->GetValue();
	const float distance = pSldLigDist->GetValue();
	const float attquad = (1.0f / (halfIntDist * halfIntDist) - 2.0f) / (range * range);
	const float rangeInt = 1.0f / (1.0f + attquad * range * range);
	pEditLigDistInt->SetFloat(decMath::max(0.0f, intensity
		* ( 1.0f / ( 1.0f + attquad * distance * distance ) - rangeInt ) / ( 1.0f - rangeInt ) ) );
}



void meWPSObject::SelectActiveTexture(){
	pCBTexture->SetSelectionWithData(GetActiveTexture());
}

void meWPSObject::UpdateTextureList(){
	meObjectTexture * const texture = GetActiveTexture();
	const meObject * const object = GetActiveObject();
	
	pCBTexture->RemoveAllItems();
	
	if(object){
		const int count = object->GetTextures().GetCount();
		int i;
		
		for(i=0; i<count; i++){
			meObjectTexture * const texture2 = object->GetTextures().GetAt(i);
			pCBTexture->AddItem(texture2->GetName(), nullptr, texture2);
		}
		
		pCBTexture->SortItems();
	}
	
	if(texture){
		pCBTexture->SetSelectionWithData(texture);
	}
}

void meWPSObject::UpdateTextureEnabled(){
	const bool enabled = GetActiveTexture() != nullptr;
	
	pEditTexSkin->SetEnabled(enabled);
	pEditTexTCOffset->SetEnabled(enabled);
	pEditTexTCScaling->SetEnabled(enabled);
	pEditTexTCRotation->SetEnabled(enabled);
	pClrTexTint->SetEnabled(enabled);
}

void meWPSObject::UpdateTexture(){
	const meObjectTexture * const texture = GetActiveTexture();
	
	if(texture){
		pEditTexSkin->SetPath(texture->GetSkinPath());
		pEditTexTCOffset->SetVector2(texture->GetTexCoordOffset());
		pEditTexTCScaling->SetVector2(texture->GetTexCoordScaling());
		pEditTexTCRotation->SetFloat(texture->GetTexCoordRotation());
		pClrTexTint->SetColor(texture->GetColorTint());
		
	}else{
		pEditTexSkin->ClearPath();
		pEditTexTCOffset->SetVector2(decVector2());
		pEditTexTCScaling->SetVector2(decVector2());
		pEditTexTCRotation->ClearText();
		pClrTexTint->SetColor(decColor(1.0f, 1.0f, 1.0f));
	}
}

void meWPSObject::SelectTexActiveProperty(){
	if(!GetActiveTexProperty().IsEmpty()){
		((meWPPropertyList&)(igdeWidget&)pEditTexProperties).SelectProperty(GetActiveTexProperty());
	}
}

void meWPSObject::UpdateTexPropertyKeys(){
	((meWPPropertyList&)(igdeWidget&)pEditTexProperties).UpdateKeys();
}

void meWPSObject::UpdateTexProperties(){
	((meWPPropertyList&)(igdeWidget&)pEditTexProperties).SetProperties(
		GetActiveTexture() ? GetActiveTexture()->GetProperties() : decStringDictionary());
	SelectTexActiveProperty();
}



void meWPSObject::UpdateIdentifierLists(){
	if(!pWorld){
		pEditProperties->SetIdentifiers(decStringSet());
		pEditTexProperties->SetIdentifiers(decStringSet());
		return;
	}
	
	// object properties
	decStringSet identifiers;
	const decString &property = GetActiveProperty();
	if(!property.IsEmpty()){
		const igdeGDProperty * const gdProperty = pEditProperties->GetGDProperty(property);
		if(gdProperty && gdProperty->GetType() == igdeGDProperty::eptIdentifier){
			const meIDGroup * const idgroup = pWorld->GetIDGroupList().FindOrDefault([&](const meIDGroup &g){
				return g.GetName() == gdProperty->GetIdentifierGroup();
			});
			if(idgroup){
				idgroup->GetIDList().Visit([&](const decString &id){
					identifiers.Add(id);
				});
			}
		}
	}
	pEditProperties->SetIdentifiers(identifiers);
	
	// texture properties
	identifiers.RemoveAll();
	const decString &texProperty = GetActiveTexProperty();
	if(!texProperty.IsEmpty()){
		const igdeGDProperty * const gdProperty = pEditProperties->GetGDProperty(texProperty);
		if(gdProperty && gdProperty->GetType() == igdeGDProperty::eptIdentifier){
			const meIDGroup * const idgroup = pWorld->GetIDGroupList().FindOrDefault([&](const meIDGroup &g){
				return g.GetName() == gdProperty->GetIdentifierGroup();
			});
			if(idgroup){
				idgroup->GetIDList().Visit([&](const decString &id){
					identifiers.Add(id);
				});
			}
		}
	}
	pEditProperties->SetIdentifiers(identifiers);
}

void meWPSObject::UpdateTriggerTargetLists(){
	igdeTriggerTargetList * const list = pWorld ? &pWorld->GetTriggerTable() : nullptr;
	pEditProperties->SetTriggerTargetList(list);
	pEditTexProperties->SetTriggerTargetList(list);
}



void meWPSObject::SetDefaultSize(){
	/*
	meWindowMain *wndMain = pWPSelection->GetWindowProperties()->GetWindowMain();
	meObject *obj = wndMain->GetCurrentObject();
	igdeGameDefinition *gameDef = map->GetGameDefinition();
	igdeGDClass *classDef;
	
	// if classname matches a class use that size instead of the given one
	if(!obj) DETHROW(deeInvalidParam);
	classDef = gameDef->GetClassAt(obj->GetClassName());
	if(classDef){
		obj->SetSize(classDef->GetSize());
	}else{
		obj->SetSize(decVector(0.5f, 0.5f, 0.5f));
	}
	*/
}

void meWPSObject::OnGameDefinitionChanged(){
	UpdateClassList();
	UpdateIdentifierLists();
	
	UpdateLight();
	UpdatePropertyKeys();
	UpdateProperties();
	UpdateAttachBehaviors();
	pEditProperties->OnGameDefinitionChanged();
	
	UpdateTexPropertyKeys();
	UpdateTexProperties();
	pEditTexProperties->OnGameDefinitionChanged();
}

void meWPSObject::SlideLightProperty(igdeGDCLight::eProperties property, const char *value, bool scrubbing){
	meObject * const object = GetActiveObject();
	if(!object || !object->GetGDClass()){
		return;
	}
	
	igdeGDCLight *gdLight = nullptr;
	decString gdpPrefix;
	meHelpers::FindFirstLight(*object->GetGDClass(), gdpPrefix, gdLight);
	
	if(!gdLight || !gdLight->IsPropertySet(property)){
		return;
	}
	
	const decString propertyName(gdpPrefix + gdLight->GetPropertyName(property));
	
	try{
		if(pUndoSetProperty){
			pUndoSetProperty.DynamicCast<meUObjectSetProperty>()->SetNewValue(value);
			pUndoSetProperty.DynamicCast<meUObjectSetProperty>()->ProgressiveRedo();
			
		}else if(pUndoAddProperty){
			pUndoAddProperty.DynamicCast<meUObjectAddProperty>()->SetValue(value);
			pUndoAddProperty.DynamicCast<meUObjectAddProperty>()->ProgressiveRedo();
			
		}else{
			if(object->GetProperties().Has(propertyName)){
				pUndoSetProperty = meUObjectSetProperty::Ref::New(object, propertyName,
					object->GetProperties().GetAt(propertyName), value);
				pUndoSetProperty->Redo();
				
			}else{
				pUndoAddProperty = meUObjectAddProperty::Ref::New(object, propertyName, value);
				pUndoAddProperty->Redo();
			}
		}
		
		if(!scrubbing){
			if(pUndoSetProperty){
				pWorld->GetUndoSystem()->Add(pUndoSetProperty);
				pUndoSetProperty = nullptr;
				
			}else if(pUndoAddProperty){
				pWorld->GetUndoSystem()->Add(pUndoAddProperty);
				pUndoAddProperty = nullptr;
			}
		}
		
	}catch(const deException &){
		pUndoSetProperty = nullptr;
		pUndoAddProperty = nullptr;
		throw;
	}
}



// Private Functions
//////////////////////

bool meWPSObject::pPropertyValue(const meObject &object, const decString &prefix,
const decString &name, decString &value) const{
	if(name.IsEmpty()){
		return false;
	}
	
	const decString propertyName(prefix + name);
	
	const decString *propertyValue;
	if(object.GetProperties().GetAt(propertyName, propertyValue)){
		value = *propertyValue;
		return true;
	}
	
	const igdeGDClass * const gdClass = object.GetGDClass();
	decString defaultValue;
	if(gdClass && gdClass->GetDefaultPropertyValue(propertyName, defaultValue)){
		value = defaultValue;
		return true;
	}
	
	return false;
}
