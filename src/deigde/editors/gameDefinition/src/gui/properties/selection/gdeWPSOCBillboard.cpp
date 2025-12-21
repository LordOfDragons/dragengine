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

#include "gdeWPSOCBillboard.h"
#include "gdeWPSOCBillboardListener.h"
#include "../gdeWindowProperties.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/category/gdeCategory.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/property/gdeProperty.h"
#include "../../../gamedef/skin/gdeSkin.h"
#include "../../../undosys/objectClass/billboard/gdeUOCBillboardSetAxis.h"
#include "../../../undosys/objectClass/billboard/gdeUOCBillboardSetOffset.h"
#include "../../../undosys/objectClass/billboard/gdeUOCBillboardSetPropertyName.h"
#include "../../../undosys/objectClass/billboard/gdeUOCBillboardSetSize.h"
#include "../../../undosys/objectClass/billboard/gdeUOCBillboardSetSkinPath.h"
#include "../../../undosys/objectClass/billboard/gdeUOCBillboardToggleLocked.h"
#include "../../../undosys/objectClass/billboard/gdeUOCBillboardToggleSizeFixedToScreen.h"
#include "../../../undosys/objectClass/billboard/gdeUOCBillboardToggleSpherical.h"
#include "../../../undosys/objectClass/billboard/gdeUOCBillboardToggleDoNotScale.h"
#include "../../../undosys/objectClass/billboard/gdeUOCBillboardTogglePartialHide.h"
#include "../../../undosys/objectClass/billboard/gdeUOCBillboardToggleRenderEnvMap.h"
#include "../../../undosys/objectClass/billboard/gdeUOCBillboardSetPosition.h"
#include "../../../undosys/objectClass/billboard/gdeUOCBillboardSetBoneName.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/composed/igdeEditVector2.h>
#include <deigde/gui/composed/igdeEditVector2Listener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace{

class cBaseEditVector2Listener : public igdeEditVector2Listener{
protected:
	gdeWPSOCBillboard &pPanel;
	
public:
	typedef deTObjectReference<cBaseEditVector2Listener> Ref;
	cBaseEditVector2Listener(gdeWPSOCBillboard &panel) : pPanel(panel){}
	
	virtual void OnVector2Changed(igdeEditVector2 *editVector2){
		gdeOCBillboard * const billboard = pPanel.GetBillboard();
		if(!billboard){
			return;
		}
		
		igdeUndo::Ref undo(
			 OnChanged(pPanel.GetObjectClass(), billboard, editVector2->GetVector2()));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(gdeObjectClass *objectClass,
		gdeOCBillboard *billboard, const decVector2 &vector) = 0;
};

class cBaseAction : public igdeAction{
protected:
	gdeWPSOCBillboard &pPanel;
	
public:
	typedef deTObjectReference<cBaseAction> Ref;
	cBaseAction(gdeWPSOCBillboard &panel, const char *name, const char *description) :
		igdeAction(name, description), pPanel(panel){}
	
	virtual void OnAction(){
		gdeOCBillboard * const billboard = pPanel.GetBillboard();
		if(!billboard){
			return;
		}
		
		igdeUndo::Ref undo(OnActionBillboard(pPanel.GetObjectClass(), billboard));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnActionBillboard(gdeObjectClass *objectClass, gdeOCBillboard *billboard) = 0;
	
	virtual void Update(){
		gdeOCBillboard * const billboard = pPanel.GetBillboard();
		if(billboard && pPanel.GetObjectClass()){
			UpdateBillboard(*pPanel.GetObjectClass(), *billboard);
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void UpdateBillboard(const gdeObjectClass &objectClass, const gdeOCBillboard &billboard){
		SetEnabled(true);
		SetSelected(false);
	}
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	gdeWPSOCBillboard &pPanel;
	
public:
	typedef deTObjectReference<cBaseEditVectorListener> Ref;
	cBaseEditVectorListener(gdeWPSOCBillboard &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		gdeOCBillboard * const billboard = pPanel.GetBillboard();
		if(!billboard){
			return;
		}
		
		igdeUndo::Ref undo(
			 OnChanged(editVector->GetVector(), pPanel.GetObjectClass(), billboard));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(const decVector &vector, gdeObjectClass *objectClass,
		gdeOCBillboard *billboard) = 0;
};

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	gdeWPSOCBillboard &pPanel;
	
public:
	typedef deTObjectReference<cBaseTextFieldListener> Ref;
	cBaseTextFieldListener(gdeWPSOCBillboard &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		gdeOCBillboard * const billboard = pPanel.GetBillboard();
		if(!billboard){
			return;
		}
		
		igdeUndo::Ref undo(
			 OnChanged(*textField, pPanel.GetObjectClass(), billboard));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField,
		gdeObjectClass *objectClass, gdeOCBillboard *billboard) = 0;
};



class cEditPathSkin : public igdeEditPathListener{
	gdeWPSOCBillboard &pPanel;
	
public:
	typedef deTObjectReference<cEditPathSkin> Ref;
	cEditPathSkin(gdeWPSOCBillboard &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		gdeOCBillboard * const billboard = pPanel.GetBillboard();
		if(!billboard || billboard->GetSkinPath() == editPath->GetPath()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUOCBillboardSetSkinPath::Ref::New(
			pPanel.GetObjectClass(), billboard, editPath->GetPath()));
	}
};

class cEditAxis : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditAxis> Ref;
	cEditAxis(gdeWPSOCBillboard &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, gdeObjectClass *objectClass, gdeOCBillboard *billboard) override{
		return !billboard->GetAxis().IsEqualTo(vector)
			? gdeUOCBillboardSetAxis::Ref::New(objectClass, billboard, vector) : igdeUndo::Ref();
	}
};

class cEditSize : public cBaseEditVector2Listener{
public:
	typedef deTObjectReference<cEditSize> Ref;
	cEditSize(gdeWPSOCBillboard &panel) : cBaseEditVector2Listener(panel){}
	
	igdeUndo::Ref OnChanged(gdeObjectClass *objectClass, gdeOCBillboard *billboard,
	const decVector2 &vector) override{
		if(billboard->GetSize().IsEqualTo(vector)){
			return {};
		}
		return gdeUOCBillboardSetSize::Ref::New(objectClass, billboard, vector);
	}
};

class cEditOffset : public cBaseEditVector2Listener{
public:
	typedef deTObjectReference<cEditOffset> Ref;
	cEditOffset(gdeWPSOCBillboard &panel) : cBaseEditVector2Listener(panel){}
	
	virtual igdeUndo::Ref OnChanged(gdeObjectClass *objectClass, gdeOCBillboard *billboard,
	const decVector2 &vector){
		if(billboard->GetOffset().IsEqualTo(vector)){
			return {};
		}
		return gdeUOCBillboardSetOffset::Ref::New(objectClass, billboard, vector);
	}
};

class cActionLocked : public cBaseAction{
public:
	typedef deTObjectReference<cActionLocked> Ref;
	
public:
	cActionLocked(gdeWPSOCBillboard &panel) : cBaseAction(panel, "Locked",
		"Billboard is locked to axis while facing the camera as well as possible"){ }
	
	virtual igdeUndo::Ref OnActionBillboard(gdeObjectClass *objectClass, gdeOCBillboard *billboard){
		return gdeUOCBillboardToggleLocked::Ref::New(objectClass, billboard);
	}
	
	void UpdateBillboard (const gdeObjectClass &objectClass, const gdeOCBillboard &billboard) override{
		cBaseAction::UpdateBillboard(objectClass, billboard);
		SetSelected(billboard.GetLocked());
	}
};

class cActionSpherical : public cBaseAction{
public:
	typedef deTObjectReference<cActionSpherical> Ref;
	
public:
	cActionSpherical(gdeWPSOCBillboard &panel) : cBaseAction(panel, "Spherical",
		"Billboard is aligned using spherical mode"){}
	
	virtual igdeUndo::Ref OnActionBillboard(gdeObjectClass *objectClass, gdeOCBillboard *billboard){
		return gdeUOCBillboardToggleSpherical::Ref::New(objectClass, billboard);
	}
	
	void UpdateBillboard (const gdeObjectClass &objectClass, const gdeOCBillboard &billboard) override{
		cBaseAction::UpdateBillboard(objectClass, billboard);
		SetSelected(billboard.GetSpherical());
	}
};

class cActionRelativeSize : public cBaseAction{
public:
	typedef deTObjectReference<cActionRelativeSize> Ref;
	
public:
	cActionRelativeSize(gdeWPSOCBillboard &panel) : cBaseAction(panel, "Relative Size",
		"Size is relative to screen size"){}
	
	virtual igdeUndo::Ref OnActionBillboard(gdeObjectClass *objectClass, gdeOCBillboard *billboard){
		return gdeUOCBillboardToggleSizeFixedToScreen::Ref::New(objectClass, billboard);
	}
	
	void UpdateBillboard (const gdeObjectClass &objectClass, const gdeOCBillboard &billboard) override{
		cBaseAction::UpdateBillboard(objectClass, billboard);
		SetSelected(billboard.GetSizeFixedToScreen());
	}
};

class cActionDoNotScale : public cBaseAction{
public:
	typedef deTObjectReference<cActionDoNotScale> Ref;
	
public:
	cActionDoNotScale(gdeWPSOCBillboard &panel) : cBaseAction(panel, "Do not scale",
		"Billboard scale does not change with scale of parent object class instance"){}
	
	virtual igdeUndo::Ref OnActionBillboard(gdeObjectClass *objectClass, gdeOCBillboard *billboard){
		return gdeUOCBillboardToggleDoNotScale::Ref::New(objectClass, billboard);
	}
	
	void UpdateBillboard (const gdeObjectClass &objectClass, const gdeOCBillboard &billboard) override{
		cBaseAction::UpdateBillboard(objectClass, billboard);
		SetSelected(billboard.GetDoNotScale());
	}
};

class cActionRenderEnvMap : public cBaseAction{
public:
	typedef deTObjectReference<cActionRenderEnvMap> Ref;
	
public:
	cActionRenderEnvMap(gdeWPSOCBillboard &panel) : cBaseAction(panel, "Render Env-Map",
		"Billboard is included in environment maps"){}
	
	virtual igdeUndo::Ref OnActionBillboard(gdeObjectClass *objectClass, gdeOCBillboard *billboard){
		return gdeUOCBillboardToggleRenderEnvMap::Ref::New(objectClass, billboard);
	}
	
	void UpdateBillboard (const gdeObjectClass &objectClass, const gdeOCBillboard &billboard) override{
		cBaseAction::UpdateBillboard(objectClass, billboard);
		SetSelected(billboard.GetRenderEnvMap());
	}
};

class cActionPartialHide : public cBaseAction{
public:
	typedef deTObjectReference<cActionPartialHide> Ref;
	
public:
	cActionPartialHide(gdeWPSOCBillboard &panel) : cBaseAction(panel, "Partial hide",
		"Billboard is hidden if partial hide tags match"){ }
	
	virtual igdeUndo::Ref OnActionBillboard(gdeObjectClass *objectClass, gdeOCBillboard *billboard){
		return gdeUOCBillboardTogglePartialHide::Ref::New(objectClass, billboard);
	}
	
	void UpdateBillboard (const gdeObjectClass &objectClass, const gdeOCBillboard &billboard) override{
		cBaseAction::UpdateBillboard(objectClass, billboard);
		SetSelected(billboard.GetPartialHide());
	}
};

class cEditPosition : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditPosition> Ref;
	cEditPosition(gdeWPSOCBillboard &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(const decVector &vector, gdeObjectClass *objectClass,
	gdeOCBillboard *billboard){
		if(billboard->GetPosition().IsEqualTo(vector)){
			return {};
		}
		return gdeUOCBillboardSetPosition::Ref::New(objectClass, billboard, vector);
	}
};

class cTextBoneName : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextBoneName> Ref;
	cTextBoneName(gdeWPSOCBillboard &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCBillboard *billboard){
		if(billboard->GetBoneName() == textField.GetText()){
			return {};
		}
		return gdeUOCBillboardSetBoneName::Ref::New(objectClass, billboard, textField.GetText());
	}
};


class cComboPropertyNames : public igdeComboBoxListener{
	gdeWPSOCBillboard &pPanel;
	
public:
	typedef deTObjectReference<cComboPropertyNames> Ref;
	cComboPropertyNames(gdeWPSOCBillboard &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox*){
		if(pPanel.GetBillboard()){
			pPanel.UpdatePropertyName();
		}
	}
};

class cComboPropertyNameTarget : public igdeComboBoxListener{
	gdeWPSOCBillboard &pPanel;
	
public:
	typedef deTObjectReference<cComboPropertyNameTarget> Ref;
	cComboPropertyNameTarget(gdeWPSOCBillboard &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		gdeOCBillboard * const billboard = pPanel.GetBillboard();
		if(!billboard){
			return;
		}
		
		const gdeOCBillboard::eProperties propertyName = pPanel.GetPropertyName();
		if(billboard->GetPropertyName(propertyName) == comboBox->GetText()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUOCBillboardSetPropertyName::Ref::New(pPanel.GetObjectClass(),
				billboard, propertyName, comboBox->GetText()));
	}
};

}



// Class gdeWPSOCBillboard
////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSOCBillboard::gdeWPSOCBillboard(gdeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, frameLine;
	
	pListener = gdeWPSOCBillboardListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	// billboard
	helper.GroupBox(content, groupBox, "Object Class Billboard:");
	helper.EditPath(groupBox, "Skin:", "Path to skin file to use",
		igdeEnvironment::efpltSkin, pEditPathSkin, cEditPathSkin::Ref::New(*this));
	helper.EditVector(groupBox, "Axis:", "Axis to align billboard with",
		pEditAxis, cEditAxis::Ref::New(*this));
	helper.EditVector2(groupBox, "Size:", "Size of billboard",
		pEditSize, cEditSize::Ref::New(*this));
	helper.EditVector2(groupBox, "Offset:", "Offset of texture on billboard",
		pEditOffset, cEditOffset::Ref::New(*this));
	
	helper.EditVector(groupBox, "Position:", "Position relative to object class",
		pEditPosition, cEditPosition::Ref::New(*this));
	helper.EditString(groupBox, "Bone:", "Bone name or empty string if not used",
		pEditBoneName, cTextBoneName::Ref::New(*this));
	
	helper.CheckBox(groupBox, pChkLocked, cActionLocked::Ref::New(*this));
	helper.CheckBox(groupBox, pChkSpherical, cActionSpherical::Ref::New(*this));
	helper.CheckBox(groupBox, pChkRelativeSize, cActionRelativeSize::Ref::New(*this));
	helper.CheckBox(groupBox, pChkDoNotScale, cActionDoNotScale::Ref::New(*this));
	helper.CheckBox(groupBox, pChkPartialHide, cActionPartialHide::Ref::New(*this));
	helper.CheckBox(groupBox, pChkRenderEnvMap, cActionRenderEnvMap::Ref::New(*this));
	
	// properties targets
	helper.GroupBox(content, groupBox, "Properties:");
	helper.ComboBox(groupBox, "Property:", "Property to set target for",
		pCBPropertyNames, cComboPropertyNames::Ref::New(*this));
	pCBPropertyNames->AddItem("Skin", nullptr, (void*)(intptr_t)gdeOCBillboard::epSkin);
	pCBPropertyNames->AddItem("Axis", nullptr, (void*)(intptr_t)gdeOCBillboard::epAxis);
	pCBPropertyNames->AddItem("Offset", nullptr, (void*)(intptr_t)gdeOCBillboard::epOffset);
	pCBPropertyNames->AddItem("Locked", nullptr, (void*)(intptr_t)gdeOCBillboard::epLocked);
	pCBPropertyNames->AddItem("Spherical", nullptr, (void*)(intptr_t)gdeOCBillboard::epSpherical);
	pCBPropertyNames->AddItem("Render EnvironmentMap", nullptr, (void*)(intptr_t)gdeOCBillboard::epRenderEnvMap);
	pCBPropertyNames->AddItem("Attach Position", nullptr, (void*)(intptr_t)gdeOCBillboard::epAttachPosition);
	
	helper.ComboBoxFilter(groupBox, "Target:", true, "Object class property to target",
		pCBPropertyNameTarget, cComboPropertyNameTarget::Ref::New(*this));
	pCBPropertyNameTarget->SetDefaultSorter();
	pCBPropertyNameTarget->SetFilterCaseInsentive(true);
}

gdeWPSOCBillboard::~gdeWPSOCBillboard(){
	SetGameDefinition(nullptr);
}



// Management
///////////////

void gdeWPSOCBillboard::SetGameDefinition(gdeGameDefinition *gameDefinition){
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
	
	UpdateBillboard();
	UpdatePropertyList();
}



gdeObjectClass *gdeWPSOCBillboard::GetObjectClass() const{
	return pGameDefinition ? pGameDefinition->GetActiveObjectClass() : nullptr;
}

gdeOCBillboard *gdeWPSOCBillboard::GetBillboard() const{
	const gdeObjectClass * const objectClass = GetObjectClass();
	return objectClass ? pGameDefinition->GetActiveOCBillboard() : nullptr;
}

const gdeOCBillboard::eProperties gdeWPSOCBillboard::GetPropertyName() const{
	return (gdeOCBillboard::eProperties)(intptr_t)pCBPropertyNames->GetSelectedItem()->GetData();
}



void gdeWPSOCBillboard::UpdatePropertyList(){
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



void gdeWPSOCBillboard::UpdateBillboard(){
	const gdeOCBillboard * const billboard = GetBillboard();
	
	if(billboard){
		pEditPathSkin->SetPath(billboard->GetSkinPath());
		pEditAxis->SetVector(billboard->GetAxis());
		pEditSize->SetVector2(billboard->GetSize());
		pEditOffset->SetVector2(billboard->GetOffset());
		pEditPosition->SetVector(billboard->GetPosition());
		pEditBoneName->SetText(billboard->GetBoneName());
		
	}else{
		pEditPathSkin->ClearPath();
		pEditAxis->SetVector(decVector());
		pEditSize->SetVector2(decVector2());
		pEditOffset->SetVector2(decVector2());
		pEditPosition->SetVector(decVector());
		pEditBoneName->ClearText();
	}
	
	const bool enabled = billboard != nullptr;
	pEditPathSkin->SetEnabled(enabled);
	pEditAxis->SetEnabled(enabled);
	pEditSize->SetEnabled(enabled);
	pEditOffset->SetEnabled(enabled);
	pEditPosition->SetEnabled(enabled);
	pEditBoneName->SetEnabled(enabled);
	
	pChkLocked->GetAction()->Update();
	pChkSpherical->GetAction()->Update();
	pChkRelativeSize->GetAction()->Update();
	pChkDoNotScale->GetAction()->Update();
	pChkPartialHide->GetAction()->Update();
	pChkRenderEnvMap->GetAction()->Update();
	
	UpdatePropertyName();
}

void gdeWPSOCBillboard::UpdatePropertyName(){
	const gdeOCBillboard * const billboard = GetBillboard();
	
	if(billboard){
		pCBPropertyNameTarget->SetText(billboard->GetPropertyName(GetPropertyName()));
		
	}else{
		pCBPropertyNameTarget->ClearText();
	}
	
	pCBPropertyNameTarget->SetEnabled(billboard);
}
