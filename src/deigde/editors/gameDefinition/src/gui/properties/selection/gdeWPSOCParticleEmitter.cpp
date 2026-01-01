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

#include "gdeWPSOCParticleEmitter.h"
#include "gdeWPSOCParticleEmitterListener.h"
#include "../gdeWindowProperties.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/category/gdeCategory.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../undosys/objectClass/particleemitter/gdeUOCParticleEmitterSetPath.h"
#include "../../../undosys/objectClass/particleemitter/gdeUOCParticleEmitterSetPosition.h"
#include "../../../undosys/objectClass/particleemitter/gdeUOCParticleEmitterSetRotation.h"
#include "../../../undosys/objectClass/particleemitter/gdeUOCParticleEmitterSetBoneName.h"
#include "../../../undosys/objectClass/particleemitter/gdeUOCParticleEmitterToggleCasting.h"
#include "../../../undosys/objectClass/particleemitter/gdeUOCParticleEmitterSetPropertyName.h"
#include "../../../undosys/objectClass/particleemitter/gdeUOCParticleEmitterSetTriggerName.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeTextField.h>
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
#include <dragengine/logger/deLogger.h>



// Actions
////////////

namespace{

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	gdeWPSOCParticleEmitter &pPanel;
	
public:
	typedef deTObjectReference<cBaseEditVectorListener> Ref;
	cBaseEditVectorListener(gdeWPSOCParticleEmitter &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		gdeOCParticleEmitter * const particleEmitter = pPanel.GetParticleEmitter();
		if(!particleEmitter){
			return;
		}
		
		igdeUndo::Ref undo(
			 OnChanged(editVector->GetVector(), pPanel.GetObjectClass(), particleEmitter));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(const decVector &vector, gdeObjectClass *objectClass,
		gdeOCParticleEmitter *particleEmitter) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	gdeWPSOCParticleEmitter &pPanel;
	
public:
	typedef deTObjectReference<cBaseComboBoxListener> Ref;
	cBaseComboBoxListener(gdeWPSOCParticleEmitter &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		gdeOCParticleEmitter * const particleEmitter = pPanel.GetParticleEmitter();
		if(!particleEmitter){
			return;
		}
		
		igdeUndo::Ref undo(
			 OnChanged(*comboBox, pPanel.GetObjectClass(), particleEmitter));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox &comboBox,
		gdeObjectClass *objectClass, gdeOCParticleEmitter *particleEmitter) = 0;
};

class cBaseAction : public igdeAction{
protected:
	gdeWPSOCParticleEmitter &pPanel;
	
public:
	typedef deTObjectReference<cBaseAction> Ref;
	cBaseAction(gdeWPSOCParticleEmitter &panel, const char *text, const char *description) :
		igdeAction(text, description), pPanel(panel){}
	
	virtual void OnAction(){
		gdeOCParticleEmitter * const particleEmitter = pPanel.GetParticleEmitter();
		if(!particleEmitter){
			return;
		}
		
		igdeUndo::Ref undo(
			 OnActionParticleEmitter(pPanel.GetObjectClass(), particleEmitter));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnActionParticleEmitter(gdeObjectClass *objectClass, gdeOCParticleEmitter *particleEmitter) = 0;
	
	virtual void Update(){
		gdeOCParticleEmitter * const particleEmitter = pPanel.GetParticleEmitter();
		if(particleEmitter){
			Update(*pPanel.GetObjectClass(), *particleEmitter);
			
		}else{
			SetEnabled(false);
			SetSelected(false);
		}
	}
	
	virtual void Update(const gdeObjectClass &, const gdeOCParticleEmitter &){
	}
};

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	gdeWPSOCParticleEmitter &pPanel;
	
public:
	typedef deTObjectReference<cBaseTextFieldListener> Ref;
	cBaseTextFieldListener(gdeWPSOCParticleEmitter &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		gdeOCParticleEmitter * const particleEmitter = pPanel.GetParticleEmitter();
		if(!particleEmitter){
			return;
		}
		
		igdeUndo::Ref undo(
			 OnChanged(*textField, pPanel.GetObjectClass(), particleEmitter));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField,
		gdeObjectClass *objectClass, gdeOCParticleEmitter *particleEmitter) = 0;
};


class cEditPath : public igdeEditPathListener{
	gdeWPSOCParticleEmitter &pPanel;
	
public:
	typedef deTObjectReference<cEditPath> Ref;
	cEditPath(gdeWPSOCParticleEmitter &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		gdeOCParticleEmitter * const particleEmitter = pPanel.GetParticleEmitter();
		if(!particleEmitter || particleEmitter->GetPath() == editPath->GetPath()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUOCParticleEmitterSetPath::Ref::New(
			pPanel.GetObjectClass(), particleEmitter, editPath->GetPath()));
	}
};

class cEditPosition : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditPosition> Ref;
	cEditPosition(gdeWPSOCParticleEmitter &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, gdeObjectClass *objectClass,
	gdeOCParticleEmitter *particleEmitter) override{
		if(particleEmitter->GetPosition().IsEqualTo(vector)){
			return {};
		}
		return gdeUOCParticleEmitterSetPosition::Ref::New(objectClass, particleEmitter, vector);
	}
};

class cEditRotation : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditRotation> Ref;
	cEditRotation(gdeWPSOCParticleEmitter &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(const decVector &vector, gdeObjectClass *objectClass,
	gdeOCParticleEmitter *particleEmitter){
		if(particleEmitter->GetRotation().IsEqualTo(vector)){
			return {};
		}
		return gdeUOCParticleEmitterSetRotation::Ref::New(objectClass, particleEmitter, vector);
	}
};

class cTextBoneName : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextBoneName> Ref;
	cTextBoneName(gdeWPSOCParticleEmitter &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCParticleEmitter *particleEmitter){
		if(particleEmitter->GetBoneName() == textField.GetText()){
			return {};
		}
		return gdeUOCParticleEmitterSetBoneName::Ref::New(objectClass, particleEmitter, textField.GetText());
	}
};

class cActionCasting : public cBaseAction{
public:
	typedef deTObjectReference<cActionCasting> Ref;
	
public:
	cActionCasting(gdeWPSOCParticleEmitter &panel) :
	cBaseAction(panel, "Casting Particles", "Particle emitter is casting particles"){}
	
	virtual igdeUndo::Ref OnActionParticleEmitter(gdeObjectClass *objectClass, gdeOCParticleEmitter *particleEmitter){
		return gdeUOCParticleEmitterToggleCasting::Ref::New(objectClass, particleEmitter);
	}
	
	void Update(const gdeObjectClass &, const gdeOCParticleEmitter &particleEmitter) override{
		SetEnabled(true);
		SetSelected(particleEmitter.GetCasting());
	}
};


class cComboPropertyNames : public igdeComboBoxListener{
	gdeWPSOCParticleEmitter &pPanel;
	
public:
	typedef deTObjectReference<cComboPropertyNames> Ref;
	cComboPropertyNames(gdeWPSOCParticleEmitter &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox*){
		if(pPanel.GetParticleEmitter()){
			pPanel.UpdatePropertyName();
		}
	}
};

class cComboPropertyNameTarget : public igdeComboBoxListener{
	gdeWPSOCParticleEmitter &pPanel;
	
public:
	typedef deTObjectReference<cComboPropertyNameTarget> Ref;
	cComboPropertyNameTarget(gdeWPSOCParticleEmitter &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		gdeOCParticleEmitter * const particleEmitter = pPanel.GetParticleEmitter();
		if(!particleEmitter){
			return;
		}
		
		const gdeOCParticleEmitter::eProperties propertyName = pPanel.GetPropertyName();
		if(particleEmitter->GetPropertyName(propertyName) == comboBox->GetText()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUOCParticleEmitterSetPropertyName::Ref::New(pPanel.GetObjectClass(),
				particleEmitter, propertyName, comboBox->GetText()));
	}
};

class cComboTriggerNames : public igdeComboBoxListener{
	gdeWPSOCParticleEmitter &pPanel;
	
public:
	typedef deTObjectReference<cComboTriggerNames> Ref;
	cComboTriggerNames(gdeWPSOCParticleEmitter &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox*){
		if(pPanel.GetParticleEmitter()){
			pPanel.UpdateTriggerName();
		}
	}
};

class cComboTriggerNameTarget : public igdeComboBoxListener{
	gdeWPSOCParticleEmitter &pPanel;
	
public:
	typedef deTObjectReference<cComboTriggerNameTarget> Ref;
	cComboTriggerNameTarget(gdeWPSOCParticleEmitter &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		if(!pPanel.GetParticleEmitter()){
			return;
		}
		
		const gdeOCParticleEmitter::eTriggers triggerName = pPanel.GetTriggerName();
		if(pPanel.GetParticleEmitter()->GetTriggerName(triggerName) == comboBox->GetText()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUOCParticleEmitterSetTriggerName::Ref::New(pPanel.GetObjectClass(),
				pPanel.GetParticleEmitter(), triggerName, comboBox->GetText()));
	}
};

}



// Class gdeWPSOCParticleEmitter
//////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSOCParticleEmitter::gdeWPSOCParticleEmitter(gdeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, frameLine;
	
	pListener = gdeWPSOCParticleEmitterListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	helper.GroupBox(content, groupBox, "Object Class Particle Emitter:");
	
	helper.EditPath(groupBox, "Path:", "Path to particle emitter",
		igdeEnvironment::efpltParticleEmitter, pEditPath, cEditPath::Ref::New(*this));
	helper.EditVector(groupBox, "Position:", "Position relative to object class",
		pEditPosition, cEditPosition::Ref::New(*this));
	helper.EditVector(groupBox, "Rotation:", "Rotation in degrees relative to object class", 4, 1,
		pEditRotation, cEditRotation::Ref::New(*this));
	helper.CheckBox(groupBox, pChkCasting, cActionCasting::Ref::New(*this));
	helper.EditString(groupBox, "Bone:", "Bone name or empty string if not used",
		pEditBoneName, cTextBoneName::Ref::New(*this));
	
	// property targets
	helper.GroupBox(content, groupBox, "Properties:");
	helper.ComboBox(groupBox, "Property:", "Property to set target for",
		pCBPropertyNames, cComboPropertyNames::Ref::New(*this));
	pCBPropertyNames->AddItem("Path", nullptr, (void*)(intptr_t)gdeOCParticleEmitter::epPath);
	pCBPropertyNames->AddItem("Attach position", nullptr, (void*)(intptr_t)gdeOCParticleEmitter::epAttachPosition);
	pCBPropertyNames->AddItem("Attach rotation", nullptr, (void*)(intptr_t)gdeOCParticleEmitter::epAttachRotation);
	pCBPropertyNames->AddItem("Casting", nullptr, (void*)(intptr_t)gdeOCParticleEmitter::epCasting);
	
	helper.ComboBoxFilter(groupBox, "Target:", true, "Object class property to target",
		pCBPropertyNameTarget, cComboPropertyNameTarget::Ref::New(*this));
	pCBPropertyNameTarget->SetEditable(true);
	pCBPropertyNameTarget->SetDefaultSorter();
	pCBPropertyNameTarget->SetFilterCaseInsentive(true);
	
	// trigger targets
	helper.GroupBox(content, groupBox, "Triggers:");
	helper.ComboBox(groupBox, "Trigger:", "Trigger to set target for",
		pCBTriggerNames, cComboTriggerNames::Ref::New(*this));
	pCBTriggerNames->AddItem("Casting", nullptr, (void*)(intptr_t)gdeOCParticleEmitter::etCasting);
	
	helper.ComboBoxFilter(groupBox, "Target:", true, "Object class property to target",
		pCBTriggerNameTarget, cComboTriggerNameTarget::Ref::New(*this));
	pCBTriggerNameTarget->SetEditable(true);
	pCBTriggerNameTarget->SetDefaultSorter();
	pCBTriggerNameTarget->SetFilterCaseInsentive(true);
}

gdeWPSOCParticleEmitter::~gdeWPSOCParticleEmitter(){
	SetGameDefinition(nullptr);
}



// Management
///////////////

void gdeWPSOCParticleEmitter::SetGameDefinition(gdeGameDefinition *gameDefinition){
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
	UpdateParticleEmitter();
}



gdeObjectClass *gdeWPSOCParticleEmitter::GetObjectClass() const{
	return pGameDefinition ? pGameDefinition->GetActiveObjectClass() : nullptr;
}

gdeOCParticleEmitter *gdeWPSOCParticleEmitter::GetParticleEmitter() const{
	const gdeObjectClass * const objectClass = GetObjectClass();
	return objectClass ? pGameDefinition->GetActiveOCParticleEmitter() : nullptr;
}

const gdeOCParticleEmitter::eProperties gdeWPSOCParticleEmitter::GetPropertyName() const{
	return (gdeOCParticleEmitter::eProperties)(intptr_t)pCBPropertyNames->GetSelectedItem()->GetData();
}

const gdeOCParticleEmitter::eTriggers gdeWPSOCParticleEmitter::GetTriggerName() const{
	return (gdeOCParticleEmitter::eTriggers)(intptr_t)pCBTriggerNames->GetSelectedItem()->GetData();
}



void gdeWPSOCParticleEmitter::UpdatePropertyList(){
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



void gdeWPSOCParticleEmitter::UpdateParticleEmitter(){
	const gdeOCParticleEmitter * const particleEmitter = GetParticleEmitter();
	
	if(particleEmitter){
		pEditPath->SetPath(particleEmitter->GetPath());
		pEditPosition->SetVector(particleEmitter->GetPosition());
		pEditRotation->SetVector(particleEmitter->GetRotation());
		pEditBoneName->SetText(particleEmitter->GetBoneName());
		
		
	}else{
		pEditPath->ClearPath();
		pEditPosition->SetVector(decVector());
		pEditRotation->SetVector(decVector());
		pEditBoneName->ClearText();
	}
	
	const bool enabled = particleEmitter;
	pEditPath->SetEnabled(enabled);
	pEditPosition->SetEnabled(enabled);
	pEditRotation->SetEnabled(enabled);
	pEditBoneName->SetEnabled(enabled);
	
	pChkCasting->GetAction()->Update();
	
	UpdatePropertyName();
	UpdateTriggerName();
}

void gdeWPSOCParticleEmitter::UpdatePropertyName(){
	const gdeOCParticleEmitter * const particleEmitter = GetParticleEmitter();
	
	if(particleEmitter){
		pCBPropertyNameTarget->SetText(particleEmitter->GetPropertyName(GetPropertyName()));
		
	}else{
		pCBPropertyNameTarget->ClearText();
	}
	
	pCBPropertyNameTarget->SetEnabled(particleEmitter);
}

void gdeWPSOCParticleEmitter::UpdateTriggerName(){
	const gdeOCParticleEmitter * const particleEmitter = GetParticleEmitter();
	
	if(particleEmitter){
		pCBTriggerNameTarget->SetText(particleEmitter->GetTriggerName(GetTriggerName()));
		
	}else{
		pCBTriggerNameTarget->ClearText();
	}
	
	pCBTriggerNameTarget->SetEnabled(particleEmitter);
}
