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
	cBaseEditVector2Listener(gdeWPSOCBillboard &panel) : pPanel(panel){}
	
	virtual void OnVector2Changed(igdeEditVector2 *editVector2){
		gdeOCBillboard * const billboard = pPanel.GetBillboard();
		if(!billboard){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(
			 OnChanged(pPanel.GetObjectClass(), billboard, editVector2->GetVector2())));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(gdeObjectClass *objectClass,
		gdeOCBillboard *billboard, const decVector2 &vector) = 0;
};

class cBaseAction : public igdeAction{
protected:
	gdeWPSOCBillboard &pPanel;
	
public:
	cBaseAction(gdeWPSOCBillboard &panel, const char *name, const char *description) :
		igdeAction(name, description), pPanel(panel){}
	
	virtual void OnAction(){
		gdeOCBillboard * const billboard = pPanel.GetBillboard();
		if(!billboard){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnActionBillboard(pPanel.GetObjectClass(), billboard)));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnActionBillboard(gdeObjectClass *objectClass, gdeOCBillboard *billboard) = 0;
	
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
	cBaseEditVectorListener(gdeWPSOCBillboard &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		gdeOCBillboard * const billboard = pPanel.GetBillboard();
		if(!billboard){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(
			 OnChanged(editVector->GetVector(), pPanel.GetObjectClass(), billboard)));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(const decVector &vector, gdeObjectClass *objectClass,
		gdeOCBillboard *billboard) = 0;
};

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	gdeWPSOCBillboard &pPanel;
	
public:
	cBaseTextFieldListener(gdeWPSOCBillboard &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		gdeOCBillboard * const billboard = pPanel.GetBillboard();
		if(!billboard){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(
			 OnChanged(*textField, pPanel.GetObjectClass(), billboard)));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField,
		gdeObjectClass *objectClass, gdeOCBillboard *billboard) = 0;
};



class cEditPathSkin : public igdeEditPathListener{
	gdeWPSOCBillboard &pPanel;
	
public:
	cEditPathSkin(gdeWPSOCBillboard &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		gdeOCBillboard * const billboard = pPanel.GetBillboard();
		if(!billboard || billboard->GetSkinPath() == editPath->GetPath()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUOCBillboardSetSkinPath::Ref::NewWith(
			pPanel.GetObjectClass(), billboard, editPath->GetPath()));
	}
};

class cEditAxis : public cBaseEditVectorListener{
public:
	cEditAxis(gdeWPSOCBillboard &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo *OnChanged (const decVector &vector, gdeObjectClass *objectClass, gdeOCBillboard *billboard){
		return !billboard->GetAxis().IsEqualTo(vector)
			? new gdeUOCBillboardSetAxis(objectClass, billboard, vector) : NULL;
	}
};

class cEditSize : public cBaseEditVector2Listener{
public:
	cEditSize(gdeWPSOCBillboard &panel) : cBaseEditVector2Listener(panel){}
	
	virtual igdeUndo *OnChanged(gdeObjectClass *objectClass, gdeOCBillboard *billboard,
	const decVector2 &vector){
		if(billboard->GetSize().IsEqualTo(vector)){
			return NULL;
		}
		return new gdeUOCBillboardSetSize(objectClass, billboard, vector);
	}
};

class cEditOffset : public cBaseEditVector2Listener{
public:
	cEditOffset(gdeWPSOCBillboard &panel) : cBaseEditVector2Listener(panel){}
	
	virtual igdeUndo *OnChanged(gdeObjectClass *objectClass, gdeOCBillboard *billboard,
	const decVector2 &vector){
		if(billboard->GetOffset().IsEqualTo(vector)){
			return NULL;
		}
		return new gdeUOCBillboardSetOffset(objectClass, billboard, vector);
	}
};

class cActionLocked : public cBaseAction{
public:
	cActionLocked(gdeWPSOCBillboard &panel) : cBaseAction(panel, "Locked",
		"Billboard is locked to axis while facing the camera as well as possible"){ }
	
	virtual igdeUndo *OnActionBillboard(gdeObjectClass *objectClass, gdeOCBillboard *billboard){
		return new gdeUOCBillboardToggleLocked(objectClass, billboard);
	}
	
	void UpdateBillboard (const gdeObjectClass &objectClass, const gdeOCBillboard &billboard) override{
		cBaseAction::UpdateBillboard(objectClass, billboard);
		SetSelected(billboard.GetLocked());
	}
};

class cActionSpherical : public cBaseAction{
public:
	cActionSpherical(gdeWPSOCBillboard &panel) : cBaseAction(panel, "Spherical",
		"Billboard is aligned using spherical mode"){}
	
	virtual igdeUndo *OnActionBillboard(gdeObjectClass *objectClass, gdeOCBillboard *billboard){
		return new gdeUOCBillboardToggleSpherical(objectClass, billboard);
	}
	
	void UpdateBillboard (const gdeObjectClass &objectClass, const gdeOCBillboard &billboard) override{
		cBaseAction::UpdateBillboard(objectClass, billboard);
		SetSelected(billboard.GetSpherical());
	}
};

class cActionRelativeSize : public cBaseAction{
public:
	cActionRelativeSize(gdeWPSOCBillboard &panel) : cBaseAction(panel, "Relative Size",
		"Size is relative to screen size"){}
	
	virtual igdeUndo *OnActionBillboard(gdeObjectClass *objectClass, gdeOCBillboard *billboard){
		return new gdeUOCBillboardToggleSizeFixedToScreen(objectClass, billboard);
	}
	
	void UpdateBillboard (const gdeObjectClass &objectClass, const gdeOCBillboard &billboard) override{
		cBaseAction::UpdateBillboard(objectClass, billboard);
		SetSelected(billboard.GetSizeFixedToScreen());
	}
};

class cActionDoNotScale : public cBaseAction{
public:
	cActionDoNotScale(gdeWPSOCBillboard &panel) : cBaseAction(panel, "Do not scale",
		"Billboard scale does not change with scale of parent object class instance"){}
	
	virtual igdeUndo *OnActionBillboard(gdeObjectClass *objectClass, gdeOCBillboard *billboard){
		return new gdeUOCBillboardToggleDoNotScale(objectClass, billboard);
	}
	
	void UpdateBillboard (const gdeObjectClass &objectClass, const gdeOCBillboard &billboard) override{
		cBaseAction::UpdateBillboard(objectClass, billboard);
		SetSelected(billboard.GetDoNotScale());
	}
};

class cActionRenderEnvMap : public cBaseAction{
public:
	cActionRenderEnvMap(gdeWPSOCBillboard &panel) : cBaseAction(panel, "Render Env-Map",
		"Billboard is included in environment maps"){}
	
	virtual igdeUndo *OnActionBillboard(gdeObjectClass *objectClass, gdeOCBillboard *billboard){
		return new gdeUOCBillboardToggleRenderEnvMap(objectClass, billboard);
	}
	
	void UpdateBillboard (const gdeObjectClass &objectClass, const gdeOCBillboard &billboard) override{
		cBaseAction::UpdateBillboard(objectClass, billboard);
		SetSelected(billboard.GetRenderEnvMap());
	}
};

class cActionPartialHide : public cBaseAction{
public:
	cActionPartialHide(gdeWPSOCBillboard &panel) : cBaseAction(panel, "Partial hide",
		"Billboard is hidden if partial hide tags match"){ }
	
	virtual igdeUndo *OnActionBillboard(gdeObjectClass *objectClass, gdeOCBillboard *billboard){
		return new gdeUOCBillboardTogglePartialHide(objectClass, billboard);
	}
	
	void UpdateBillboard (const gdeObjectClass &objectClass, const gdeOCBillboard &billboard) override{
		cBaseAction::UpdateBillboard(objectClass, billboard);
		SetSelected(billboard.GetPartialHide());
	}
};

class cEditPosition : public cBaseEditVectorListener {
public:
	cEditPosition(gdeWPSOCBillboard &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo *OnChanged(const decVector &vector, gdeObjectClass *objectClass,
	gdeOCBillboard *billboard){
		if(billboard->GetPosition().IsEqualTo(vector)){
			return NULL;
		}
		return new gdeUOCBillboardSetPosition(objectClass, billboard, vector);
	}
};

class cTextBoneName : public cBaseTextFieldListener{
public:
	cTextBoneName(gdeWPSOCBillboard &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCBillboard *billboard){
		if(billboard->GetBoneName() == textField.GetText()){
			return NULL;
		}
		return new gdeUOCBillboardSetBoneName(objectClass, billboard, textField.GetText());
	}
};


class cComboPropertyNames : public igdeComboBoxListener{
	gdeWPSOCBillboard &pPanel;
	
public:
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
			gdeUOCBillboardSetPropertyName::Ref::NewWith(pPanel.GetObjectClass(),
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
pWindowProperties(windowProperties),
pListener(NULL),
pGameDefinition(NULL)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, frameLine;
	
	pListener.TakeOverWith(*this);
	
	content.TakeOverWith(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	// billboard
	helper.GroupBox(content, groupBox, "Object Class Billboard:");
	helper.EditPath(groupBox, "Skin:", "Path to skin file to use",
		igdeEnvironment::efpltSkin, pEditPathSkin, new cEditPathSkin(*this));
	helper.EditVector(groupBox, "Axis:", "Axis to align billboard with",
		pEditAxis, new cEditAxis(*this));
	helper.EditVector2(groupBox, "Size:", "Size of billboard",
		pEditSize, new cEditSize(*this));
	helper.EditVector2(groupBox, "Offset:", "Offset of texture on billboard",
		pEditOffset, new cEditOffset(*this));
	
	helper.EditVector(groupBox, "Position:", "Position relative to object class",
		pEditPosition, new cEditPosition(*this));
	helper.EditString(groupBox, "Bone:", "Bone name or empty string if not used",
		pEditBoneName, new cTextBoneName(*this));
	
	helper.CheckBox(groupBox, pChkLocked, new cActionLocked(*this), true);
	helper.CheckBox(groupBox, pChkSpherical, new cActionSpherical(*this), true);
	helper.CheckBox(groupBox, pChkRelativeSize, new cActionRelativeSize(*this), true);
	helper.CheckBox(groupBox, pChkDoNotScale, new cActionDoNotScale(*this), true);
	helper.CheckBox(groupBox, pChkPartialHide, new cActionPartialHide(*this), true);
	helper.CheckBox(groupBox, pChkRenderEnvMap, new cActionRenderEnvMap(*this), true);
	
	// properties targets
	helper.GroupBox(content, groupBox, "Properties:");
	helper.ComboBox(groupBox, "Property:", "Property to set target for",
		pCBPropertyNames, new cComboPropertyNames(*this));
	pCBPropertyNames->AddItem("Skin", NULL, (void*)(intptr_t)gdeOCBillboard::epSkin);
	pCBPropertyNames->AddItem("Axis", NULL, (void*)(intptr_t)gdeOCBillboard::epAxis);
	pCBPropertyNames->AddItem("Offset", NULL, (void*)(intptr_t)gdeOCBillboard::epOffset);
	pCBPropertyNames->AddItem("Locked", NULL, (void*)(intptr_t)gdeOCBillboard::epLocked);
	pCBPropertyNames->AddItem("Spherical", NULL, (void*)(intptr_t)gdeOCBillboard::epSpherical);
	pCBPropertyNames->AddItem("Render EnvironmentMap", NULL, (void*)(intptr_t)gdeOCBillboard::epRenderEnvMap);
	pCBPropertyNames->AddItem("Attach Position", NULL, (void*)(intptr_t)gdeOCBillboard::epAttachPosition);
	
	helper.ComboBoxFilter(groupBox, "Target:", true, "Object class property to target",
		pCBPropertyNameTarget, new cComboPropertyNameTarget(*this));
	pCBPropertyNameTarget->SetDefaultSorter();
	pCBPropertyNameTarget->SetFilterCaseInsentive(true);
}

gdeWPSOCBillboard::~gdeWPSOCBillboard(){
	SetGameDefinition(NULL);
	
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
		gameDefinition->AddReference();
	}
	
	UpdateBillboard();
	UpdatePropertyList();
}



gdeObjectClass *gdeWPSOCBillboard::GetObjectClass() const{
	return pGameDefinition ? pGameDefinition->GetActiveObjectClass() : NULL;
}

gdeOCBillboard *gdeWPSOCBillboard::GetBillboard() const{
	const gdeObjectClass * const objectClass = GetObjectClass();
	return objectClass ? pGameDefinition->GetActiveOCBillboard() : NULL;
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
	
	const bool enabled = billboard != NULL;
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
