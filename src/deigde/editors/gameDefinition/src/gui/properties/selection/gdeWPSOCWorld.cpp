/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#include <stdint.h>

#include "gdeWPSOCWorld.h"
#include "gdeWPSOCWorldListener.h"
#include "../gdeWindowProperties.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/category/gdeCategory.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/world/gdeOCWorld.h"
#include "../../../gamedef/property/gdeProperty.h"
#include "../../../undosys/objectClass/world/gdeUOCWorldSetPath.h"
#include "../../../undosys/objectClass/world/gdeUOCWorldSetPosition.h"
#include "../../../undosys/objectClass/world/gdeUOCWorldSetPropertyName.h"
#include "../../../undosys/objectClass/world/gdeUOCWorldSetRotation.h"

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
	gdeWPSOCWorld &pPanel;
	
public:
	cBaseTextFieldListener(gdeWPSOCWorld &panel) : pPanel(panel){ }
	
	void OnTextChanged(igdeTextField *textField) override{
		gdeOCWorld * const world = pPanel.GetWorld();
		if(!world){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(*textField, pPanel.GetObjectClass(), world)));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeTextField &textField,
		gdeObjectClass *objectClass, gdeOCWorld *world) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	gdeWPSOCWorld &pPanel;
	
public:
	cBaseEditVectorListener(gdeWPSOCWorld &panel) : pPanel(panel){ }
	
	void OnVectorChanged(igdeEditVector *editVector) override{
		gdeOCWorld * const world = pPanel.GetWorld();
		if(!world){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(
			OnChanged(editVector->GetVector(), pPanel.GetObjectClass(), world)));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(const decVector &vector,
		gdeObjectClass *objectClass, gdeOCWorld *world) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	gdeWPSOCWorld &pPanel;
	
public:
	cBaseComboBoxListener(gdeWPSOCWorld &panel) : pPanel(panel){ }
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		gdeOCWorld * const world = pPanel.GetWorld();
		if(!world){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(*comboBox, pPanel.GetObjectClass(), world)));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeComboBox &comboBox,
		gdeObjectClass *objectClass, gdeOCWorld *world) = 0;
};


class cEditPath : public igdeEditPathListener{
	gdeWPSOCWorld &pPanel;
	
public:
	cEditPath(gdeWPSOCWorld &panel) : pPanel(panel){ }
	
	void OnEditPathChanged(igdeEditPath *editPath) override{
		gdeOCWorld * const world = pPanel.GetWorld();
		if(!world || world->GetPath() == editPath->GetPath()){
			return;
		}
		
		gdeUOCWorldSetPath::Ref undo(gdeUOCWorldSetPath::Ref::New(
		new gdeUOCWorldSetPath(pPanel.GetObjectClass(), world, editPath->GetPath())));
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
	}
};

class cEditPosition : public cBaseEditVectorListener {
public:
	cEditPosition(gdeWPSOCWorld &panel) : cBaseEditVectorListener(panel){ }
	
	igdeUndo *OnChanged(const decVector &vector, gdeObjectClass *objectClass, gdeOCWorld *world) override{
		if(world->GetPosition().IsEqualTo(vector)){
			return nullptr;
		}
		return new gdeUOCWorldSetPosition(objectClass, world, vector);
	}
};

class cEditRotation : public cBaseEditVectorListener {
public:
	cEditRotation(gdeWPSOCWorld &panel) : cBaseEditVectorListener(panel){ }
	
	igdeUndo *OnChanged(const decVector &vector, gdeObjectClass *objectClass, gdeOCWorld *world) override{
		if(world->GetRotation().IsEqualTo(vector)){
			return nullptr;
		}
		return new gdeUOCWorldSetRotation(objectClass, world, vector);
	}
};

class cComboPropertyNames : public igdeComboBoxListener{
	gdeWPSOCWorld &pPanel;
	
public:
	cComboPropertyNames(gdeWPSOCWorld &panel) : pPanel(panel){ }
	
	void OnTextChanged(igdeComboBox*) override{
		if(pPanel.GetWorld()){
			pPanel.UpdatePropertyName();
		}
	}
};

class cComboPropertyNameTarget : public igdeComboBoxListener{
	gdeWPSOCWorld &pPanel;
	
public:
	cComboPropertyNameTarget(gdeWPSOCWorld &panel) : pPanel(panel){ }
	
	void OnTextChanged(igdeComboBox *comboBox) override{
		if(!pPanel.GetWorld()){
			return;
		}
		
		const gdeOCWorld::eProperties propertyName = pPanel.GetPropertyName();
		if(pPanel.GetWorld()->GetPropertyName(propertyName) == comboBox->GetText()){
			return;
		}
		
		gdeUOCWorldSetPropertyName::Ref undo(gdeUOCWorldSetPropertyName::Ref::New(new gdeUOCWorldSetPropertyName(pPanel.GetObjectClass(),
			pPanel.GetWorld(), propertyName, comboBox->GetText())));
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
	}
};

}



// Class gdeWPSOCWorld
////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSOCWorld::gdeWPSOCWorld(gdeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties),
pListener(nullptr),
pGameDefinition(nullptr)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, frameLine;
	
	pListener = new gdeWPSOCWorldListener(*this);
	
	content.TakeOver(new igdeContainerFlow(env, igdeContainerFlow::eaY));
	AddChild(content);
	
	helper.GroupBox(content, groupBox, "Object Class World:");
	
	helper.EditPath(groupBox, "Path:", "Path to world", igdeEnvironment::efpltWorld,
		pEditPath, new cEditPath(*this));
	helper.EditVector(groupBox, "Position:", "Position relative to object class",
		pEditPosition, new cEditPosition(*this));
	helper.EditVector(groupBox, "Rotation:", "Rotation in degrees relative to object class", 4, 1,
		pEditRotation, new cEditRotation(*this));
	
	// property targets
	helper.GroupBox(content, groupBox, "Properties:");
	helper.ComboBox(groupBox, "Property:", "Property to set target for",
		pCBPropertyNames, new cComboPropertyNames(*this));
	pCBPropertyNames->AddItem("Path", nullptr, (void*)(intptr_t)gdeOCWorld::epPath);
	pCBPropertyNames->AddItem("Position", nullptr, (void*)(intptr_t)gdeOCWorld::epPosition);
	pCBPropertyNames->AddItem("Rotation", nullptr, (void*)(intptr_t)gdeOCWorld::epRotation);
	
	helper.ComboBoxFilter(groupBox, "Target:", true, "Object class property to target",
		pCBPropertyNameTarget, new cComboPropertyNameTarget(*this));
	pCBPropertyNameTarget->SetEditable(true);
	pCBPropertyNameTarget->SetDefaultSorter();
	pCBPropertyNameTarget->SetFilterCaseInsentive(true);
}

gdeWPSOCWorld::~gdeWPSOCWorld(){
	SetGameDefinition(nullptr);
	
	if(pListener){
		pListener->FreeReference();
	}
}


// Management
///////////////

void gdeWPSOCWorld::SetGameDefinition(gdeGameDefinition *gameDefinition){
	if(gameDefinition == pGameDefinition){
		return;
	}
	
	if(pGameDefinition){
		pGameDefinition->RemoveListener(pListener);
		pGameDefinition->FreeReference();
	}
	
	pGameDefinition = gameDefinition;
	
	if(gameDefinition){
		gameDefinition->AddListener(pListener);
		gameDefinition->AddReference();
	}
	
	UpdatePropertyList();
	UpdateWorld();
}


gdeObjectClass *gdeWPSOCWorld::GetObjectClass() const{
	return pGameDefinition ? pGameDefinition->GetActiveObjectClass() : nullptr;
}

gdeOCWorld *gdeWPSOCWorld::GetWorld() const{
	const gdeObjectClass * const objectClass = GetObjectClass();
	return objectClass ? pGameDefinition->GetActiveOCWorld() : nullptr;
}

const gdeOCWorld::eProperties gdeWPSOCWorld::GetPropertyName() const{
	return (gdeOCWorld::eProperties)(intptr_t)pCBPropertyNames->GetSelectedItem()->GetData();
}


void gdeWPSOCWorld::UpdatePropertyList(){
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

void gdeWPSOCWorld::UpdateWorld(){
	const gdeOCWorld * const world = GetWorld();
	
	if(world){
		pEditPath->SetPath(world->GetPath());
		pEditPosition->SetVector(world->GetPosition());
		pEditRotation->SetVector(world->GetRotation());
		
	}else{
		pEditPath->ClearPath();
		pEditPosition->SetVector(decVector());
		pEditRotation->SetVector(decVector());
	}
	
	const bool enabled = world;
	pEditPath->SetEnabled(enabled);
	pEditPosition->SetEnabled(enabled);
	pEditRotation->SetEnabled(enabled);
	
	UpdatePropertyName();
}

void gdeWPSOCWorld::UpdatePropertyName(){
	const gdeOCWorld * const world = GetWorld();
	
	if(world){
		pCBPropertyNameTarget->SetText(world->GetPropertyName(GetPropertyName()));
		
	}else{
		pCBPropertyNameTarget->ClearText();
	}
	
	pCBPropertyNameTarget->SetEnabled(world);
}
