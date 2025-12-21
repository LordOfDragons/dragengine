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

#include "gdeWPSOCSpeaker.h"
#include "gdeWPSOCSpeakerListener.h"
#include "../gdeWindowProperties.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/category/gdeCategory.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/speaker/gdeOCSpeaker.h"
#include "../../../gamedef/property/gdeProperty.h"
#include "../../../undosys/objectClass/speaker/gdeUOCSpeakerSetBoneName.h"
#include "../../../undosys/objectClass/speaker/gdeUOCSpeakerSetRollOff.h"
#include "../../../undosys/objectClass/speaker/gdeUOCSpeakerSetDistanceOffset.h"
#include "../../../undosys/objectClass/speaker/gdeUOCSpeakerSetPlaySpeed.h"
#include "../../../undosys/objectClass/speaker/gdeUOCSpeakerSetPathSound.h"
#include "../../../undosys/objectClass/speaker/gdeUOCSpeakerSetRange.h"
#include "../../../undosys/objectClass/speaker/gdeUOCSpeakerSetRotation.h"
#include "../../../undosys/objectClass/speaker/gdeUOCSpeakerSetPosition.h"
#include "../../../undosys/objectClass/speaker/gdeUOCSpeakerSetPropertyName.h"
#include "../../../undosys/objectClass/speaker/gdeUOCSpeakerSetTriggerName.h"
#include "../../../undosys/objectClass/speaker/gdeUOCSpeakerSetVolume.h"
#include "../../../undosys/objectClass/speaker/gdeUOCSpeakerToggleLooping.h"
#include "../../../undosys/objectClass/speaker/gdeUOCSpeakerTogglePlaying.h"

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
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/logger/deLogger.h>



// Actions
////////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	gdeWPSOCSpeaker &pPanel;
	
public:
	typedef deTObjectReference<cBaseTextFieldListener> Ref;
	cBaseTextFieldListener(gdeWPSOCSpeaker &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		gdeOCSpeaker * const speaker = pPanel.GetSpeaker();
		if(!speaker){
			return;
		}
		
		igdeUndo::Ref undo(
			 OnChanged(*textField, pPanel.GetObjectClass(), speaker));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField,
		gdeObjectClass *objectClass, gdeOCSpeaker *speaker) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	gdeWPSOCSpeaker &pPanel;
	
public:
	typedef deTObjectReference<cBaseEditVectorListener> Ref;
	cBaseEditVectorListener(gdeWPSOCSpeaker &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		gdeOCSpeaker * const speaker = pPanel.GetSpeaker();
		if(!speaker){
			return;
		}
		
		igdeUndo::Ref undo(
			 OnChanged(editVector->GetVector(), pPanel.GetObjectClass(), speaker));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(const decVector &vector, gdeObjectClass *objectClass,
		gdeOCSpeaker *speaker) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	gdeWPSOCSpeaker &pPanel;
	
public:
	typedef deTObjectReference<cBaseComboBoxListener> Ref;
	cBaseComboBoxListener(gdeWPSOCSpeaker &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		gdeOCSpeaker * const speaker = pPanel.GetSpeaker();
		if(!speaker){
			return;
		}
		
		igdeUndo::Ref undo(
			 OnChanged(*comboBox, pPanel.GetObjectClass(), speaker));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox &comboBox,
		gdeObjectClass *objectClass, gdeOCSpeaker *speaker) = 0;
};

class cBaseAction : public igdeAction{
protected:
	gdeWPSOCSpeaker &pPanel;
	
public:
	typedef deTObjectReference<cBaseAction> Ref;
	cBaseAction(gdeWPSOCSpeaker &panel, const char *text, const char *description) :
		igdeAction(text, description), pPanel(panel){}
	
	virtual void OnAction(){
		gdeOCSpeaker * const speaker = pPanel.GetSpeaker();
		if(!speaker){
			return;
		}
		
		igdeUndo::Ref undo(OnActionSpeaker(pPanel.GetObjectClass(), speaker));
		if(undo){
			pPanel.GetGameDefinition()->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnActionSpeaker(gdeObjectClass *objectClass, gdeOCSpeaker *speaker) = 0;
};


class cEditPathSound : public igdeEditPathListener{
	gdeWPSOCSpeaker &pPanel;
	
public:
	typedef deTObjectReference<cEditPathSound> Ref;
	cEditPathSound(gdeWPSOCSpeaker &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		gdeOCSpeaker * const speaker = pPanel.GetSpeaker();
		if(!speaker || speaker->GetPathSound() == editPath->GetPath()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUOCSpeakerSetPathSound::Ref::New(
			pPanel.GetObjectClass(), speaker, editPath->GetPath()));
	}
};

class cEditPosition : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditPosition> Ref;
	cEditPosition(gdeWPSOCSpeaker &panel) : cBaseEditVectorListener(panel){}
	
	igdeUndo::Ref OnChanged(const decVector &vector, gdeObjectClass *objectClass,
	gdeOCSpeaker *speaker) override{
		if(speaker->GetPosition().IsEqualTo(vector)){
			return {};
		}
		return gdeUOCSpeakerSetPosition::Ref::New(objectClass, speaker, vector);
	}
};

class cEditRotation : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditRotation> Ref;
	cEditRotation(gdeWPSOCSpeaker &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(const decVector &vector, gdeObjectClass *objectClass,
	gdeOCSpeaker *speaker){
		if(speaker->GetRotation().IsEqualTo(vector)){
			return {};
		}
		return gdeUOCSpeakerSetRotation::Ref::New(objectClass, speaker, vector);
	}
};

class cTextBoneName : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextBoneName> Ref;
	cTextBoneName(gdeWPSOCSpeaker &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCSpeaker *speaker){
		if(speaker->GetBoneName() == textField.GetText()){
			return {};
		}
		return gdeUOCSpeakerSetBoneName::Ref::New(objectClass, speaker, textField.GetText());
	}
};

class cActionLooping : public cBaseAction{
public:
	typedef deTObjectReference<cActionLooping> Ref;
	
public:
	cActionLooping(gdeWPSOCSpeaker &panel) :
	cBaseAction(panel, "Looping", "Speaker is looping"){}
	
	virtual igdeUndo::Ref OnActionSpeaker(gdeObjectClass *objectClass, gdeOCSpeaker *speaker){
		return gdeUOCSpeakerToggleLooping::Ref::New(objectClass, speaker);
	}
};

class cActionPlaying : public cBaseAction{
public:
	typedef deTObjectReference<cActionPlaying> Ref;
	
public:
	cActionPlaying(gdeWPSOCSpeaker &panel) :
	cBaseAction(panel, "Playing", "Speaker is playing"){}
	
	virtual igdeUndo::Ref OnActionSpeaker(gdeObjectClass *objectClass, gdeOCSpeaker *speaker){
		return gdeUOCSpeakerTogglePlaying::Ref::New(objectClass, speaker);
	}
};

class cTextVolume : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextVolume> Ref;
	cTextVolume(gdeWPSOCSpeaker &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCSpeaker *speaker){
		const float value = textField.GetFloat();
		if(fabsf(speaker->GetVolume() - value) < FLOAT_SAFE_EPSILON){
			return {};
		}
		return gdeUOCSpeakerSetVolume::Ref::New(objectClass, speaker, value);
	}
};

class cTextRange : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextRange> Ref;
	cTextRange(gdeWPSOCSpeaker &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCSpeaker *speaker){
		const float value = textField.GetFloat();
		if(fabsf(speaker->GetRange() - value) < FLOAT_SAFE_EPSILON){
			return {};
		}
		return gdeUOCSpeakerSetRange::Ref::New(objectClass, speaker, value);
	}
};

class cTextRollOff : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextRollOff> Ref;
	cTextRollOff(gdeWPSOCSpeaker &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCSpeaker *speaker){
		const float value = textField.GetFloat();
		if(fabsf(speaker->GetRollOff() - value) < FLOAT_SAFE_EPSILON){
			return {};
		}
		return gdeUOCSpeakerSetRollOff::Ref::New(objectClass, speaker, value);
	}
};

class cTextDistanceOffset : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextDistanceOffset> Ref;
	cTextDistanceOffset(gdeWPSOCSpeaker &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCSpeaker *speaker){
		const float value = textField.GetFloat();
		if(fabsf(speaker->GetDistanceOffset() - value) < FLOAT_SAFE_EPSILON){
			return {};
		}
		return gdeUOCSpeakerSetDistanceOffset::Ref::New(objectClass, speaker, value);
	}
};

class cTextPlaySpeed : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextPlaySpeed> Ref;
	cTextPlaySpeed(gdeWPSOCSpeaker &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField &textField, gdeObjectClass *objectClass,
	gdeOCSpeaker *speaker){
		const float value = textField.GetFloat();
		if(fabsf(speaker->GetPlaySpeed() - value) < FLOAT_SAFE_EPSILON){
			return {};
		}
		return gdeUOCSpeakerSetPlaySpeed::Ref::New(objectClass, speaker, value);
	}
};


class cComboPropertyNames : public igdeComboBoxListener{
	gdeWPSOCSpeaker &pPanel;
	
public:
	typedef deTObjectReference<cComboPropertyNames> Ref;
	cComboPropertyNames(gdeWPSOCSpeaker &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox*){
		if(pPanel.GetSpeaker()){
			pPanel.UpdatePropertyName();
		}
	}
};

class cComboPropertyNameTarget : public igdeComboBoxListener{
	gdeWPSOCSpeaker &pPanel;
	
public:
	typedef deTObjectReference<cComboPropertyNameTarget> Ref;
	cComboPropertyNameTarget(gdeWPSOCSpeaker &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		if(!pPanel.GetSpeaker()){
			return;
		}
		
		const gdeOCSpeaker::eProperties propertyName = pPanel.GetPropertyName();
		if(pPanel.GetSpeaker()->GetPropertyName(propertyName) == comboBox->GetText()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUOCSpeakerSetPropertyName::Ref::New(
			pPanel.GetObjectClass(), pPanel.GetSpeaker(), propertyName, comboBox->GetText()));
	}
};

class cComboTriggerNames : public igdeComboBoxListener{
	gdeWPSOCSpeaker &pPanel;
	
public:
	typedef deTObjectReference<cComboTriggerNames> Ref;
	cComboTriggerNames(gdeWPSOCSpeaker &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox*){
		if(pPanel.GetSpeaker()){
			pPanel.UpdateTriggerName();
		}
	}
};

class cComboTriggerNameTarget : public igdeComboBoxListener{
	gdeWPSOCSpeaker &pPanel;
	
public:
	typedef deTObjectReference<cComboTriggerNameTarget> Ref;
	cComboTriggerNameTarget(gdeWPSOCSpeaker &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		gdeOCSpeaker * const speaker = pPanel.GetSpeaker();
		if(!speaker){
			return;
		}
		
		const gdeOCSpeaker::eTriggers triggerName = pPanel.GetTriggerName();
		if(speaker->GetTriggerName(triggerName) == comboBox->GetText()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUOCSpeakerSetTriggerName::Ref::New(
			pPanel.GetObjectClass(), speaker, triggerName, comboBox->GetText()));
	}
};

}



// Class gdeWPSOCSpeaker
//////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSOCSpeaker::gdeWPSOCSpeaker(gdeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, frameLine;
	
	pListener = gdeWPSOCSpeakerListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	helper.GroupBox(content, groupBox, "Object Class Speaker:");
	
	helper.EditPath(groupBox, "Sound:", "Path to sound",
		igdeEnvironment::efpltSound, pEditPathSound, cEditPathSound::Ref::New(*this));
	helper.EditVector(groupBox, "Position:", "Position relative to object class",
		pEditPosition, cEditPosition::Ref::New(*this));
	helper.EditVector(groupBox, "Rotation:", "Rotation in degrees relative to object class", 4, 1,
		pEditRotation, cEditRotation::Ref::New(*this));
	helper.EditString(groupBox, "Bone:", "Bone name or empty string if not used",
		pEditBoneName, cTextBoneName::Ref::New(*this));
	helper.EditFloat(groupBox, "Volume:", "Speaker volume",
		pEditVolume, cTextVolume::Ref::New(*this));
	helper.EditFloat(groupBox, "Range:", "Speaker range",
		pEditRange, cTextRange::Ref::New(*this));
	helper.EditFloat(groupBox, "Roll-Off:", "Roll off factor",
		pEditRollOff, cTextRollOff::Ref::New(*this));
	helper.EditFloat(groupBox, "Distance Offset:", "Distance offset",
		pEditDistanceOffset, cTextDistanceOffset::Ref::New(*this));
	helper.EditFloat(groupBox, "Play Speed:", "Play Speed",
		pEditPlaySpeed, cTextPlaySpeed::Ref::New(*this));
	helper.CheckBox(groupBox, pChkLooping, cActionLooping::Ref::New(*this));
	helper.CheckBox(groupBox, pChkPlaying, cActionPlaying::Ref::New(*this));
	
	// property targets
	helper.GroupBox(content, groupBox, "Properties:");
	helper.ComboBox(groupBox, "Property:", "Property to set target for",
		pCBPropertyNames, cComboPropertyNames::Ref::New(*this));
	pCBPropertyNames->AddItem("Sound", nullptr, (void*)(intptr_t)gdeOCSpeaker::epSound);
	pCBPropertyNames->AddItem("Volume", nullptr, (void*)(intptr_t)gdeOCSpeaker::epVolume);
	pCBPropertyNames->AddItem("Range", nullptr, (void*)(intptr_t)gdeOCSpeaker::epRange);
	pCBPropertyNames->AddItem("Roll off factor", nullptr, (void*)(intptr_t)gdeOCSpeaker::epRollOff);
	pCBPropertyNames->AddItem("Distance offset", nullptr, (void*)(intptr_t)gdeOCSpeaker::epDistanceOffset);
	pCBPropertyNames->AddItem("Play speed", nullptr, (void*)(intptr_t)gdeOCSpeaker::epPlaySpeed);
	pCBPropertyNames->AddItem("Playing", nullptr, (void*)(intptr_t)gdeOCSpeaker::epPlaying);
	pCBPropertyNames->AddItem("Looping", nullptr, (void*)(intptr_t)gdeOCSpeaker::epLooping);
	pCBPropertyNames->AddItem("Attach position", nullptr, (void*)(intptr_t)gdeOCSpeaker::epAttachPosition);
	pCBPropertyNames->AddItem("Attach rotation", nullptr, (void*)(intptr_t)gdeOCSpeaker::epAttachRotation);
	
	helper.ComboBoxFilter(groupBox, "Target:", true, "Object class property to target",
		pCBPropertyNameTarget, cComboPropertyNameTarget::Ref::New(*this));
	pCBPropertyNameTarget->SetEditable(true);
	pCBPropertyNameTarget->SetDefaultSorter();
	pCBPropertyNameTarget->SetFilterCaseInsentive(true);
	
	// trigger targets
	helper.GroupBox(content, groupBox, "Triggers:");
	helper.ComboBox(groupBox, "Trigger:", "Trigger to set target for",
		pCBTriggerNames, cComboTriggerNames::Ref::New(*this));
	pCBTriggerNames->AddItem("Playing", nullptr, (void*)(intptr_t)gdeOCSpeaker::etPlaying);
	pCBTriggerNames->AddItem("Muted", nullptr, (void*)(intptr_t)gdeOCSpeaker::etMuted);
	
	helper.ComboBoxFilter(groupBox, "Target:", true, "Object class property to target",
		pCBTriggerNameTarget, cComboTriggerNameTarget::Ref::New(*this));
	pCBTriggerNameTarget->SetEditable(true);
	pCBTriggerNameTarget->SetDefaultSorter();
	pCBTriggerNameTarget->SetFilterCaseInsentive(true);
}

gdeWPSOCSpeaker::~gdeWPSOCSpeaker(){
	SetGameDefinition(nullptr);
}



// Management
///////////////

void gdeWPSOCSpeaker::SetGameDefinition(gdeGameDefinition *gameDefinition){
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
	UpdateSpeaker();
}



gdeObjectClass *gdeWPSOCSpeaker::GetObjectClass() const{
	return pGameDefinition ? pGameDefinition->GetActiveObjectClass() : nullptr;
}

gdeOCSpeaker *gdeWPSOCSpeaker::GetSpeaker() const{
	const gdeObjectClass * const objectClass = GetObjectClass();
	return objectClass ? pGameDefinition->GetActiveOCSpeaker() : nullptr;
}

const gdeOCSpeaker::eProperties gdeWPSOCSpeaker::GetPropertyName() const{
	return (gdeOCSpeaker::eProperties)(intptr_t)pCBPropertyNames->GetSelectedItem()->GetData();
}

const gdeOCSpeaker::eTriggers gdeWPSOCSpeaker::GetTriggerName() const{
	return (gdeOCSpeaker::eTriggers)(intptr_t)pCBTriggerNames->GetSelectedItem()->GetData();
}



void gdeWPSOCSpeaker::UpdatePropertyList(){
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

void gdeWPSOCSpeaker::UpdateSpeaker(){
	const gdeOCSpeaker * const speaker = GetSpeaker();
	
	if(speaker){
		pEditPathSound->SetPath(speaker->GetPathSound());
		pEditPosition->SetVector(speaker->GetPosition());
		pEditRotation->SetVector(speaker->GetRotation());
		pEditBoneName->SetText(speaker->GetBoneName());
		pChkLooping->SetChecked(speaker->GetLooping());
		pChkPlaying->SetChecked(speaker->GetPlaying());
		pEditVolume->SetFloat(speaker->GetVolume());
		pEditRange->SetFloat(speaker->GetRange());
		pEditRollOff->SetFloat(speaker->GetRollOff());
		pEditDistanceOffset->SetFloat(speaker->GetDistanceOffset());
		pEditPlaySpeed->SetFloat(speaker->GetPlaySpeed());
		
	}else{
		pEditPathSound->ClearPath();
		pEditPosition->SetVector(decVector());
		pEditRotation->SetVector(decVector());
		pEditBoneName->ClearText();
		pChkLooping->SetChecked(false);
		pChkPlaying->SetChecked(false);
		pEditVolume->ClearText();
		pEditRange->ClearText();
		pEditRollOff->ClearText();
		pEditDistanceOffset->ClearText();
		pEditPlaySpeed->ClearText();
	}
	
	const bool enabled = speaker;
	pEditPathSound->SetEnabled(enabled);
	pEditPosition->SetEnabled(enabled);
	pEditRotation->SetEnabled(enabled);
	pEditBoneName->SetEnabled(enabled);
	pChkLooping->SetEnabled(enabled);
	pChkPlaying->SetEnabled(enabled);
	pEditVolume->SetEnabled(enabled);
	pEditRange->SetEnabled(enabled);
	pEditRollOff->SetEnabled(enabled);
	pEditDistanceOffset->SetEnabled(enabled);
	pEditPlaySpeed->SetEnabled(enabled);
	
	UpdatePropertyName();
	UpdateTriggerName();
}

void gdeWPSOCSpeaker::UpdatePropertyName(){
	const gdeOCSpeaker * const speaker = GetSpeaker();
	
	if(speaker){
		pCBPropertyNameTarget->SetText(speaker->GetPropertyName(GetPropertyName()));
		
	}else{
		pCBPropertyNameTarget->ClearText();
	}
	
	pCBPropertyNameTarget->SetEnabled(speaker);
}

void gdeWPSOCSpeaker::UpdateTriggerName(){
	const gdeOCSpeaker * const speaker = GetSpeaker();
	
	if(speaker){
		pCBTriggerNameTarget->SetText(speaker->GetTriggerName(GetTriggerName()));
		
	}else{
		pCBTriggerNameTarget->ClearText();
	}
	
	pCBTriggerNameTarget->SetEnabled(speaker);
}
