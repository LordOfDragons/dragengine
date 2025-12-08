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

#include "gdeWPSOCNavigationSpace.h"
#include "gdeWPSOCNavigationSpaceListener.h"
#include "../gdeWindowProperties.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/category/gdeCategory.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/navspace/gdeOCNavigationSpace.h"
#include "../../../gamedef/property/gdeProperty.h"
#include "../../../undosys/objectClass/navspace/gdeUOCNavSpaceSetBlockerShapeList.h"
#include "../../../undosys/objectClass/navspace/gdeUOCNavSpaceSetBlockingPriority.h"
#include "../../../undosys/objectClass/navspace/gdeUOCNavSpaceSetLayer.h"
#include "../../../undosys/objectClass/navspace/gdeUOCNavSpaceSetPath.h"
#include "../../../undosys/objectClass/navspace/gdeUOCNavSpaceSetPosition.h"
#include "../../../undosys/objectClass/navspace/gdeUOCNavSpaceSetRotation.h"
#include "../../../undosys/objectClass/navspace/gdeUOCNavSpaceSetBoneName.h"
#include "../../../undosys/objectClass/navspace/gdeUOCNavSpaceSetPropertyName.h"
#include "../../../undosys/objectClass/navspace/gdeUOCNavSpaceSetSnapAngle.h"
#include "../../../undosys/objectClass/navspace/gdeUOCNavSpaceSetSnapDistance.h"
#include "../../../undosys/objectClass/navspace/gdeUOCNavSpaceSetType.h"

#include <deigde/codec/igdeCodecPropertyString.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeWindow.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
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
	gdeWPSOCNavigationSpace &pPanel;
	
public:
	cBaseTextFieldListener(gdeWPSOCNavigationSpace &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		gdeOCNavigationSpace * const navspace = pPanel.GetNavigationSpace();
		if(!navspace){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(
			 OnChanged(*textField, pPanel.GetObjectClass(), navspace)));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField,
		gdeObjectClass *objectClass, gdeOCNavigationSpace *navspace) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	gdeWPSOCNavigationSpace &pPanel;
	
public:
	cBaseEditVectorListener(gdeWPSOCNavigationSpace &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		gdeOCNavigationSpace * const navspace = pPanel.GetNavigationSpace();
		if(!navspace){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(
			 OnChanged(editVector->GetVector(), pPanel.GetObjectClass(), navspace)));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(const decVector &vector, gdeObjectClass *objectClass,
		gdeOCNavigationSpace *navspace) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	gdeWPSOCNavigationSpace &pPanel;
	
public:
	cBaseComboBoxListener(gdeWPSOCNavigationSpace &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		gdeOCNavigationSpace * const navspace = pPanel.GetNavigationSpace();
		if(!navspace){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(
			 OnChanged(*comboBox, pPanel.GetObjectClass(), navspace)));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeComboBox &comboBox,
		gdeObjectClass *objectClass, gdeOCNavigationSpace *navspace) = 0;
};


class cEditPath : public igdeEditPathListener{
	gdeWPSOCNavigationSpace &pPanel;
	
public:
	cEditPath(gdeWPSOCNavigationSpace &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		gdeOCNavigationSpace * const navspace = pPanel.GetNavigationSpace();
		if(!navspace || navspace->GetPath() == editPath->GetPath()){
			return;
		}
		
		gdeUOCNavSpaceSetPath::Ref undo(gdeUOCNavSpaceSetPath::Ref::NewWith(
			pPanel.GetObjectClass(), navspace, editPath->GetPath()));
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
	}
};

class cEditPosition : public cBaseEditVectorListener {
public:
	cEditPosition(gdeWPSOCNavigationSpace &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo *OnChanged(const decVector &vector, gdeObjectClass *objectClass,
	gdeOCNavigationSpace *navspace){
		if(navspace->GetPosition().IsEqualTo(vector)){
			return NULL;
		}
		return new gdeUOCNavSpaceSetPosition(objectClass, navspace, vector);
	}
};

class cEditRotation : public cBaseEditVectorListener {
public:
	cEditRotation(gdeWPSOCNavigationSpace &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo *OnChanged(const decVector &vector, gdeObjectClass *objectClass,
	gdeOCNavigationSpace *navspace){
		if(navspace->GetRotation().IsEqualTo(vector)){
			return NULL;
		}
		return new gdeUOCNavSpaceSetRotation(objectClass, navspace, vector);
	}
};

class cTextBoneName : public cBaseTextFieldListener{
public:
	cTextBoneName(gdeWPSOCNavigationSpace &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCNavigationSpace *navSpace){
		if(navSpace->GetBoneName() == textField.GetText()){
			return NULL;
		}
		return new gdeUOCNavSpaceSetBoneName(objectClass, navSpace, textField.GetText());
	}
};

class cTextLayer : public cBaseTextFieldListener{
public:
	cTextLayer(gdeWPSOCNavigationSpace &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCNavigationSpace *navspace){
		const int value = textField.GetInteger();
		if(navspace->GetLayer() == value){
			return NULL;
		}
		return new gdeUOCNavSpaceSetLayer(objectClass, navspace, value);
	}
};

class cTextSnapDistance : public cBaseTextFieldListener{
public:
	cTextSnapDistance(gdeWPSOCNavigationSpace &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCNavigationSpace *navspace){
		const float value = textField.GetFloat();
		if(fabsf(navspace->GetSnapDistance() - value) < FLOAT_SAFE_EPSILON){
			return NULL;
		}
		return new gdeUOCNavSpaceSetSnapDistance(objectClass, navspace, value);
	}
};

class cTextSnapAngle : public cBaseTextFieldListener{
public:
	cTextSnapAngle(gdeWPSOCNavigationSpace &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCNavigationSpace *navspace){
		const float value = textField.GetFloat();
		if(fabsf(navspace->GetSnapAngle() - value) < FLOAT_SAFE_EPSILON){
			return NULL;
		}
		return new gdeUOCNavSpaceSetSnapAngle(objectClass, navspace, value);
	}
};

class cComboType : public cBaseComboBoxListener{
public:
	cComboType(gdeWPSOCNavigationSpace &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeComboBox &comboBox, gdeObjectClass *objectClass,
	gdeOCNavigationSpace *navspace){
		const deNavigationSpace::eSpaceTypes value =
			(deNavigationSpace::eSpaceTypes)(intptr_t)comboBox.GetSelectedItem()->GetData();
		if(value == navspace->GetType()){
			return NULL;
		}
		return new gdeUOCNavSpaceSetType(objectClass, navspace, value);
	}
};

class cTextBlockingPriority : public cBaseTextFieldListener{
public:
	cTextBlockingPriority(gdeWPSOCNavigationSpace &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCNavigationSpace *navspace){
		const int value = textField.GetInteger();
		if(navspace->GetBlockingPriority() == value){
			return NULL;
		}
		return new gdeUOCNavSpaceSetBlockingPriority(objectClass, navspace, value);
	}
};

class cTextBlockingShape : public cBaseTextFieldListener{
public:
	cTextBlockingShape(gdeWPSOCNavigationSpace &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCNavigationSpace *navspace){
		igdeCodecPropertyString codec;
		decString oldEncoded;
		codec.EncodeShapeList(navspace->GetBlockerShapeList(), oldEncoded);
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
		
		return new gdeUOCNavSpaceSetBlockerShapeList(objectClass, navspace, shapeList);
	}
};


class cComboPropertyNames : public igdeComboBoxListener{
	gdeWPSOCNavigationSpace &pPanel;
	
public:
	cComboPropertyNames(gdeWPSOCNavigationSpace &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox*){
		if(pPanel.GetNavigationSpace()){
			pPanel.UpdatePropertyName();
		}
	}
};

class cComboPropertyNameTarget : public igdeComboBoxListener{
	gdeWPSOCNavigationSpace &pPanel;
	
public:
	cComboPropertyNameTarget(gdeWPSOCNavigationSpace &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		if(!pPanel.GetNavigationSpace()){
			return;
		}
		
		const gdeOCNavigationSpace::eProperties propertyName = pPanel.GetPropertyName();
		if(pPanel.GetNavigationSpace()->GetPropertyName(propertyName) == comboBox->GetText()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUOCNavSpaceSetPropertyName::Ref::NewWith(pPanel.GetObjectClass(),
				pPanel.GetNavigationSpace(), propertyName, comboBox->GetText()));
	}
};

}



// Class gdeWPSOCNavigationSpace
//////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSOCNavigationSpace::gdeWPSOCNavigationSpace(gdeWindowProperties &windowProperties) :
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
	
	helper.GroupBox(content, groupBox, "Object Class Navigation Space:");
	
	helper.EditPath(groupBox, "Path:", "Path to navigation space",
		igdeEnvironment::efpltNavigationSpace, pEditPath, new cEditPath(*this));
	helper.EditVector(groupBox, "Position:", "Position relative to object class",
		pEditPosition, new cEditPosition(*this));
	helper.EditVector(groupBox, "Rotation:", "Rotation in degrees relative to object class", 4, 1,
		pEditRotation, new cEditRotation(*this));
	helper.EditString(groupBox, "Bone:", "Bone name or empty string if not used",
		pEditBoneName, new cTextBoneName(*this));
	helper.EditInteger(groupBox, "Layer:", "Navigation layer the blocker affects",
		pEditLayer, new cTextLayer(*this));
	helper.EditFloat(groupBox, "Snap distance:", "Snap distance",
		pEditSnapDistance, new cTextSnapDistance(*this));
	helper.EditFloat(groupBox, "Snap angle:", "Snap angle", 4, 1,
		pEditSnapAngle, new cTextSnapAngle(*this));
	
	helper.ComboBox(groupBox, "Type:", "Navigation space ", pCBType, new cComboType(*this));
	pCBType->AddItem("Grid", NULL, (void*)(intptr_t)deNavigationSpace::estGrid);
	pCBType->AddItem("Mesh", NULL, (void*)(intptr_t)deNavigationSpace::estMesh);
	pCBType->AddItem("Volume", NULL, (void*)(intptr_t)deNavigationSpace::estVolume);
	
	helper.EditInteger(groupBox, "Blocking priority:",
		"Blocks navigation spaces with the same or lower priority",
		pEditBlockingPriority, new cTextBlockingPriority(*this));
	helper.EditString(groupBox, "Blocking shape:", "Space shape of navigation space if present",
		pEditBlockerShape, new cTextBlockingShape(*this));
	
	// property targets
	helper.GroupBox(content, groupBox, "Properties:");
	helper.ComboBox(groupBox, "Property:", "Property to set target for",
		pCBPropertyNames, new cComboPropertyNames(*this));
	pCBPropertyNames->AddItem("Path", NULL, (void*)(intptr_t)gdeOCNavigationSpace::epPath);
	pCBPropertyNames->AddItem("Layer", NULL, (void*)(intptr_t)gdeOCNavigationSpace::epLayer);
	pCBPropertyNames->AddItem("Snap distance", NULL, (void*)(intptr_t)gdeOCNavigationSpace::epSnapDistance);
	pCBPropertyNames->AddItem("Snap angle", NULL, (void*)(intptr_t)gdeOCNavigationSpace::epSnapAngle);
	pCBPropertyNames->AddItem("Space shape", NULL, (void*)(intptr_t)gdeOCNavigationSpace::epBlockerShape);
	pCBPropertyNames->AddItem("Blocking priority", NULL, (void*)(intptr_t)gdeOCNavigationSpace::epBlockingPriority);
	pCBPropertyNames->AddItem("Attach position", NULL, (void*)(intptr_t)gdeOCNavigationSpace::epAttachPosition);
	pCBPropertyNames->AddItem("Attach rotation", NULL, (void*)(intptr_t)gdeOCNavigationSpace::epAttachRotation);
	
	helper.ComboBoxFilter(groupBox, "Target:", true, "Object class property to target",
		pCBPropertyNameTarget, new cComboPropertyNameTarget(*this));
	pCBPropertyNameTarget->SetEditable(true);
	pCBPropertyNameTarget->SetDefaultSorter();
	pCBPropertyNameTarget->SetFilterCaseInsentive(true);
}

gdeWPSOCNavigationSpace::~gdeWPSOCNavigationSpace(){
	SetGameDefinition(NULL);
	
}



// Management
///////////////

void gdeWPSOCNavigationSpace::SetGameDefinition(gdeGameDefinition *gameDefinition){
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
	UpdateNavigationSpace();
}



gdeObjectClass *gdeWPSOCNavigationSpace::GetObjectClass() const{
	return pGameDefinition ? pGameDefinition->GetActiveObjectClass() : NULL;
}

gdeOCNavigationSpace *gdeWPSOCNavigationSpace::GetNavigationSpace() const{
	const gdeObjectClass * const objectClass = GetObjectClass();
	return objectClass ? pGameDefinition->GetActiveOCNavigationSpace() : NULL;
}

const gdeOCNavigationSpace::eProperties gdeWPSOCNavigationSpace::GetPropertyName() const{
	return (gdeOCNavigationSpace::eProperties)(intptr_t)pCBPropertyNames->GetSelectedItem()->GetData();
}



void gdeWPSOCNavigationSpace::UpdatePropertyList(){
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

void gdeWPSOCNavigationSpace::UpdateNavigationSpace(){
	const gdeOCNavigationSpace * const navspace = GetNavigationSpace();
	
	if(navspace){
		pEditPath->SetPath(navspace->GetPath());
		pEditPosition->SetVector(navspace->GetPosition());
		pEditRotation->SetVector(navspace->GetRotation());
		pEditBoneName->SetText(navspace->GetBoneName());
		pEditLayer->SetInteger(navspace->GetLayer());
		pEditSnapDistance->SetFloat(navspace->GetSnapDistance());
		pEditSnapAngle->SetFloat(navspace->GetSnapAngle());
		pCBType->SetSelectionWithData((void*)(intptr_t)navspace->GetType());
		pEditBlockingPriority->SetInteger(navspace->GetBlockingPriority());
		
		igdeCodecPropertyString codec;
		decString encoded;
		codec.EncodeShapeList(navspace->GetBlockerShapeList(), encoded);
		pEditBlockerShape->SetText(encoded);
		
	}else{
		pEditPath->ClearPath();
		pEditPosition->SetVector(decVector());
		pEditRotation->SetVector(decVector());
		pEditBoneName->ClearText();
		pEditLayer->ClearText();
		pEditSnapDistance->ClearText();
		pEditSnapAngle->ClearText();
		pCBType->SetSelectionWithData((void*)(intptr_t)deNavigationSpace::estGrid);
		pEditBlockingPriority->ClearText();
		pEditBlockerShape->ClearText();
	}
	
	const bool enabled = navspace;
	pEditPath->SetEnabled(enabled);
	pEditPosition->SetEnabled(enabled);
	pEditRotation->SetEnabled(enabled);
	pEditBoneName->SetEnabled(enabled);
	pEditLayer->SetEnabled(enabled);
	pEditSnapDistance->SetEnabled(enabled);
	pEditSnapAngle->SetEnabled(enabled);
	pCBType->SetEnabled(enabled);
	pEditBlockingPriority->SetEnabled(enabled);
	pEditBlockerShape->SetEnabled(enabled);
	
	UpdatePropertyName();
}

void gdeWPSOCNavigationSpace::UpdatePropertyName(){
	const gdeOCNavigationSpace * const navspace = GetNavigationSpace();
	
	if(navspace){
		pCBPropertyNameTarget->SetText(navspace->GetPropertyName(GetPropertyName()));
		
	}else{
		pCBPropertyNameTarget->ClearText();
	}
	
	pCBPropertyNameTarget->SetEnabled(navspace);
}
