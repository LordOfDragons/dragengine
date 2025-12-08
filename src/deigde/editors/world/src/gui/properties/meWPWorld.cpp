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

#include "meWPWorld.h"
#include "meWPWorldListener.h"
#include "meWPPropertyList.h"
#include "meWindowProperties.h"
#include "../meWindowMain.h"
#include "../../world/meWorld.h"
#include "../../world/pathfinding/mePathFindTest.h"
#include "../../world/pathfinding/types/mePathFindTestType.h"
#include "../../world/meCamera.h"
#include "../../world/idgroup/meIDGroup.h"
#include "../../world/idgroup/meIDGroupList.h"
#include "../../undosys/properties/world/meUWorldSetSize.h"
#include "../../undosys/properties/world/meUWorldSetGravity.h"
#include "../../undosys/properties/world/property/meUWorldAddProperty.h"
#include "../../undosys/properties/world/property/meUWorldRemoveProperty.h"
#include "../../undosys/properties/world/property/meUWorldSetProperty.h"
#include "../../undosys/properties/world/property/meUWorldSetProperties.h"

#include <deigde/codec/igdeCodecPropertyString.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/property/igdeGDProperty.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditDVector.h>
#include <deigde/gui/composed/igdeEditDVectorListener.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/composed/igdeEditSliderText.h>
#include <deigde/gui/composed/igdeEditSliderTextListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
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
	meWPWorld &pPanel;
	
public:
	cBaseTextFieldListener(meWPWorld &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		meWorld * const world = pPanel.GetWorld();
		if(!world){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(textField, world)));
		if(undo){
			world->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, meWorld *world) = 0;
};

class cBaseAction : public igdeAction{
protected:
	meWPWorld &pPanel;
	
public:
	cBaseAction(meWPWorld &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	virtual void OnAction(){
		meWorld * const world = pPanel.GetWorld();
		if(!world){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnAction(world)));
		if(undo){
			world->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnAction(meWorld *world) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	meWPWorld &pPanel;
	
public:
	cBaseComboBoxListener(meWPWorld &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		meWorld * const world = pPanel.GetWorld();
		if(!world){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(comboBox, world)));
		if(undo){
			world->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(igdeComboBox *comboBox, meWorld *world) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	meWPWorld &pPanel;
	
public:
	cBaseEditVectorListener(meWPWorld &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		meWorld * const world = pPanel.GetWorld();
		if(!world){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(editVector->GetVector(), world)));
		if(undo){
			world->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(const decVector &vector, meWorld *world) = 0;
};

class cBaseEditDVectorListener : public igdeEditDVectorListener{
protected:
	meWPWorld &pPanel;
	
public:
	cBaseEditDVectorListener(meWPWorld &panel) : pPanel(panel){}
	
	virtual void OnDVectorChanged(igdeEditDVector *editDVector){
		meWorld * const world = pPanel.GetWorld();
		if(!world){
			return;
		}
		
		igdeUndo::Ref undo(igdeUndo::Ref::New(OnChanged(editDVector->GetDVector(), world)));
		if(undo){
			world->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo *OnChanged(const decDVector &vector, meWorld *world) = 0;
};


class cEditWorldProperties : public meWPPropertyList {
	meWPWorld &pPanel;
	
public:
	cEditWorldProperties(meWPWorld &panel) : meWPPropertyList(panel.GetEnvironment()), pPanel(panel){}
	
	virtual decString GetGDDefaultValue(const char *key) const{
		const igdeGDProperty * const gdProperty = GetGDProperty(key);
		return gdProperty ? gdProperty->GetDefaultValue() : vEmptyString;
	}
	
	const igdeGDProperty *GetGDProperty(const char *key) const override{
		const meWorld * const world = pPanel.GetWorld();
		return world ? world->GetGameDefinition()->GetListWorldProperties().GetNamed(key) : NULL;
	}
	
	virtual decStringSet GetGDPropertyKeys() const{
		const meWorld * const world = pPanel.GetWorld();
		decStringSet keys;
		if(world){
			const igdeGDPropertyList &list = world->GetGameDefinition()->GetListWorldProperties();
			const int count = list.GetCount();
			int i;
			for(i=0; i<count; i++){
				keys.Add(list.GetAt(i)->GetName());
			}
		}
		return keys;
	}
	
	virtual void OnPropertySelected(const decString &key, const decString &){
		meWorld * const world = pPanel.GetWorld();
		if(world){
			world->SetActiveProperty(key);
		}
	}
	
	virtual igdeUndo *UndoAddProperty(const decString &key, const decString &value){
		meWorld * const world = pPanel.GetWorld();
		return world ? new meUWorldAddProperty(world, key, value) : NULL;
	}
	
	virtual igdeUndo *UndoRemoveProperty(const decString &key){
		meWorld * const world = pPanel.GetWorld();
		return world ? new meUWorldRemoveProperty(world, key, world->GetProperties().GetAt(key)) : NULL;
	}
	
	virtual igdeUndo *UndoSetProperty(const decString &key, const decString &oldValue, const decString &newValue){
		meWorld * const world = pPanel.GetWorld();
		return world ? new meUWorldSetProperty(world, key, oldValue, newValue) : NULL;
	}
	
	virtual igdeUndo *UndoSetProperties(const decStringDictionary &properties){
		meWorld * const world = pPanel.GetWorld();
		return world ? new meUWorldSetProperties(world, properties) : NULL;
	}
};


class cEditSize : public cBaseEditDVectorListener{
public:
	cEditSize(meWPWorld &panel) : cBaseEditDVectorListener(panel){}
	
	virtual igdeUndo * OnChanged(const decDVector &vector, meWorld *world){
		return !world->GetSize().IsEqualTo(vector) ? new meUWorldSetSize(world, vector) : NULL;
	}
};


class cEditGravity : public cBaseEditVectorListener{
public:
	cEditGravity(meWPWorld &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo * OnChanged(const decVector &vector, meWorld *world){
		return !world->GetGravity().IsEqualTo(vector) ? new meUWorldSetGravity(world, vector) : NULL;
	}
};

class cEditPFTStartPosition : public cBaseEditDVectorListener{
public:
	cEditPFTStartPosition(meWPWorld &panel) : cBaseEditDVectorListener(panel){}
	
	virtual igdeUndo * OnChanged(const decDVector &vector, meWorld *world){
		world->GetPathFindTest()->SetStartPosition(vector);
		return NULL;
	}
};

class cActionPFTStartPosFromCamera : public cBaseAction{
public:
	cActionPFTStartPosFromCamera(meWPWorld &panel) : cBaseAction(panel,
		"Set", NULL, "Set start position from camera position"){}
	
	igdeUndo *OnAction(meWorld *world) override{
		world->GetPathFindTest()->SetStartPosition(world->GetActiveCamera()->GetPosition());
		return NULL;
	}
};

class cEditPFTGoalPosition : public cBaseEditDVectorListener{
public:
	cEditPFTGoalPosition(meWPWorld &panel) : cBaseEditDVectorListener(panel){}
	
	virtual igdeUndo * OnChanged(const decDVector &vector, meWorld *world){
		world->GetPathFindTest()->SetGoalPosition(vector);
		return NULL;
	}
};

class cActionPFTGoalPosFromCamera : public cBaseAction{
public:
	cActionPFTGoalPosFromCamera(meWPWorld &panel) : cBaseAction(panel,
		"Set", NULL, "Set goal position from camera position"){}
	
	igdeUndo *OnAction(meWorld *world) override{
		world->GetPathFindTest()->SetGoalPosition(world->GetActiveCamera()->GetPosition());
		return NULL;
	}
};

class cEditPFTLayer : public cBaseTextFieldListener{
public:
	cEditPFTLayer(meWPWorld &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, meWorld *world){
		world->GetPathFindTest()->SetLayer(textField->GetInteger());
		return NULL;
	}
};

class cComboPFTSpaceType : public cBaseComboBoxListener{
public:
	cComboPFTSpaceType(meWPWorld &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeComboBox *comboBox, meWorld *world){
		if(comboBox->GetSelectedItem()){
			world->GetPathFindTest()->SetSpaceType((deNavigationSpace::eSpaceTypes)
				(intptr_t)comboBox->GetSelectedItem()->GetData());
		}
		return NULL;
	}
};

class cEditPFTBlockingCost : public cBaseTextFieldListener{
public:
	cEditPFTBlockingCost(meWPWorld &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, meWorld *world){
		world->GetPathFindTest()->SetBlockingCost(textField->GetFloat());
		return NULL;
	}
};

class cActionPFTShowPath : public cBaseAction{
public:
	cActionPFTShowPath(meWPWorld &panel) : cBaseAction(panel,
		"Show Path", NULL, "Show path"){}
	
	igdeUndo *OnAction(meWorld *world) override{
		world->GetPathFindTest()->SetShowPath(!world->GetPathFindTest()->GetShowPath());
		return NULL;
	}
};


class cComboPFTType : public cBaseComboBoxListener{
public:
	cComboPFTType(meWPWorld &panel) : cBaseComboBoxListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeComboBox*, meWorld*){
		pPanel.UpdatePathFindTestType();
		return NULL;
	}
};

class cActionPFTTypeAdd : public cBaseAction{
public:
	cActionPFTTypeAdd(meWPWorld &panel) : cBaseAction(panel, "Add...",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add type"){}
	
	igdeUndo *OnAction(meWorld *world) override{
		mePathFindTestTypeList &list = world->GetPathFindTest()->GetTypeList();
		const int count = list.GetCount();
		int i, newValue = 0;
		
		for(i=0; i<count; i++){
			newValue = decMath::max(newValue, list.GetAt(i)->GetTypeNumber() + 1);
		}
		
		while(igdeCommonDialogs::GetInteger(&pPanel, "Add Type", "Type Number:", newValue)){
			if(list.HasWith(newValue)){
				igdeCommonDialogs::Error(&pPanel, "Add Type", "A type with this type number exists already.");
				continue;
			}
			
			const mePathFindTestType::Ref type(mePathFindTestType::Ref::NewWith(newValue));
			list.Add(type);
			world->GetPathFindTest()->NotifyTypesChanged();
			pPanel.SelectPFType(type);
			break;
		}
		return NULL;
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetWorld());
	}
};

class cActionPFTTypeRemove : public cBaseAction{
public:
	cActionPFTTypeRemove(meWPWorld &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove type"){}
	
	igdeUndo *OnAction(meWorld *world) override{
		mePathFindTestType * const type = pPanel.GetActivePathFindTestType();
		if(type){
			world->GetPathFindTest()->GetTypeList().Remove(type);
			world->GetPathFindTest()->NotifyTypesChanged();
		}
		return NULL;
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetActivePathFindTestType());
	}
};

class cActionPFTTypeClear : public cBaseAction{
public:
	cActionPFTTypeClear(meWPWorld &panel) : cBaseAction(panel, "Clear",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove all types"){}
	
	igdeUndo *OnAction(meWorld *world) override{
		if(world->GetPathFindTest()->GetTypeList().GetCount() > 0){
			world->GetPathFindTest()->GetTypeList().RemoveAll();
			world->GetPathFindTest()->NotifyTypesChanged();
		}
		return NULL;
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetWorld() && pPanel.GetWorld()->GetPathFindTest()->GetTypeList().GetCount() > 0);
	}
};

class cActionPFTTypes : public igdeActionContextMenu{
	meWPWorld &pPanel;
	
public:
	cActionPFTTypes(meWPWorld &panel) : igdeActionContextMenu("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallDown), "Menu"),
	pPanel(panel){}
	
	void AddContextMenuEntries(igdeMenuCascade &contextMenu) override{
		if(!pPanel.GetWorld()){
			return;
		}
		
		igdeUIHelper &helper = pPanel.GetEnvironment().GetUIHelper();
		helper.MenuCommand(contextMenu, pPanel.GetActionPFTTypeAdd());
		helper.MenuCommand(contextMenu, pPanel.GetActionPFTTypeRemove());
		helper.MenuCommand(contextMenu, pPanel.GetActionPFTTypeClear());
	}
};

class cEditPFTTypeNumber : public cBaseTextFieldListener{
public:
	cEditPFTTypeNumber(meWPWorld &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, meWorld *world){
		mePathFindTestType * const type = pPanel.GetActivePathFindTestType();
		const int value = textField->GetInteger();
		if(!type || value == type->GetTypeNumber()){
			return NULL;
		}
		
		if(pPanel.GetWorld()->GetPathFindTest()->GetTypeList().HasWith(value)){
			igdeCommonDialogs::Error(&pPanel, "Change Type Number", "Type number already exists");
			textField->SetInteger(type->GetTypeNumber());
			
		}else{
			type->SetTypeNumber(value);
			world->GetPathFindTest()->NotifyTypesChanged();
		}
		return NULL;
	}
};

class cEditPFTTypeName : public cBaseTextFieldListener{
public:
	cEditPFTTypeName(meWPWorld &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, meWorld *world){
		mePathFindTestType * const type = pPanel.GetActivePathFindTestType();
		if(type && textField->GetText() != type->GetName()){
			type->SetName(textField->GetText());
			world->GetPathFindTest()->NotifyTypesChanged();
		}
		return NULL;
	}
};

class cEditPFTTypeFixCost : public cBaseTextFieldListener{
public:
	cEditPFTTypeFixCost(meWPWorld &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, meWorld *world){
		mePathFindTestType * const type = pPanel.GetActivePathFindTestType();
		const float value = textField->GetFloat();
		if(type && fabsf(value - type->GetFixCost()) > FLOAT_SAFE_EPSILON){
			type->SetFixCost(value);
			world->GetPathFindTest()->NotifyTypesChanged();
		}
		return NULL;
	}
};

class cEditPFTTypeCostPerMeter : public cBaseTextFieldListener{
public:
	cEditPFTTypeCostPerMeter(meWPWorld &panel) : cBaseTextFieldListener(panel){}
	
	virtual igdeUndo *OnChanged(igdeTextField *textField, meWorld *world){
		mePathFindTestType * const type = pPanel.GetActivePathFindTestType();
		const float value = textField->GetFloat();
		if(type && fabsf(value - type->GetCostPerMeter()) > FLOAT_SAFE_EPSILON){
			type->SetCostPerMeter(value);
			world->GetPathFindTest()->NotifyTypesChanged();
		}
		return NULL;
	}
};


class cEditMusicPath : public igdeEditPathListener{
	meWPWorld &pPanel;
public:
	cEditMusicPath(meWPWorld &panel) : pPanel(panel){}
	
	void OnEditPathChanged(igdeEditPath *editPath) override{
		if(pPanel.GetWorld()){
			pPanel.GetWorld()->GetMusic().SetPath(editPath->GetPath());
		}
	}
};

class cEditMusicVolume : public igdeEditSliderTextListener{
	meWPWorld &pPanel;
public:
	cEditMusicVolume(meWPWorld &panel) : pPanel(panel){}
	
	void OnSliderTextValueChanging(igdeEditSliderText *sliderText) override{
		if(pPanel.GetWorld()){
			pPanel.GetWorld()->GetMusic().SetVolume(sliderText->GetValue());
		}
	}
};

class cActionMusicPlay : public cBaseAction{
public:
	cActionMusicPlay(meWPWorld &panel) : cBaseAction(panel, "Play", nullptr, "Play"){}
	
	igdeUndo *OnAction(meWorld *world) override{
		world->GetMusic().Play();
		return nullptr;
	}
	
	void Update() override{
		SetEnabled(pPanel.GetWorld());
	}
};

class cActionMusicPause : public cBaseAction{
public:
	cActionMusicPause(meWPWorld &panel) : cBaseAction(panel, "Pause", nullptr, "Pause"){}
	
	igdeUndo *OnAction(meWorld *world) override{
		world->GetMusic().Pause();
		return nullptr;
	}
	
	void Update() override{
		SetEnabled(pPanel.GetWorld());
	}
};

class cActionMusicStop : public cBaseAction{
public:
	cActionMusicStop(meWPWorld &panel) : cBaseAction(panel, "Stop", nullptr, "Stop"){}
	
	igdeUndo *OnAction(meWorld *world) override{
		world->GetMusic().Stop();
		return nullptr;
	}
	
	void Update() override{
		SetEnabled(pPanel.GetWorld());
	}
};

}





// Class meWPWorld
////////////////////

// Constructor, destructor
////////////////////////////

meWPWorld::meWPWorld(meWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer *content, groupBox, formLine;
	
	pListener.TakeOver(new meWPWorldListener(*this));
	
	
	pActionPFTTypeAdd.TakeOver(new cActionPFTTypeAdd(*this));
	pActionPFTTypeRemove.TakeOver(new cActionPFTTypeRemove(*this));
	pActionPFTTypeClear.TakeOver(new cActionPFTTypeClear(*this));
	pActionMusicPlay.TakeOver(new cActionMusicPlay(*this));
	pActionMusicPause.TakeOver(new cActionMusicPause(*this));
	pActionMusicStop.TakeOver(new cActionMusicStop(*this));
	
	
	content.TakeOver(new igdeContainerFlow(env, igdeContainerFlow::eaY));
	AddChild(content);
	
	
	// parameters
	helper.GroupBox(content, groupBox, "World Parameters:");
	
	helper.EditDVector(groupBox, "Size", "Size of world in meters where modules can expect content",
		8, 0, pEditSize, new cEditSize(*this));
	
	helper.EditVector(groupBox, "Gravity", "World gravity", pEditGravity, new cEditGravity(*this));
	
	
	// properties
	helper.GroupBoxFlow(content, groupBox, "World Properties:", false, false);
	
	pEditProperties.TakeOver(new cEditWorldProperties(*this));
	groupBox->AddChild(pEditProperties);
	
	
	// find path test
	helper.GroupBox(content, groupBox, "Path Find Test:", true);
	
	helper.FormLineStretchFirst(groupBox, "Start Position:", "Start position of the test path", formLine);
	helper.EditDVector(formLine, "Start position of the test path",
		pEditPFTStartPosition, new cEditPFTStartPosition(*this));
	helper.Button(formLine, pBtnPFTStartPosFromCamera, new cActionPFTStartPosFromCamera(*this), true);
	
	helper.FormLineStretchFirst(groupBox, "Goal Position:", "Goal position of the test path", formLine);
	helper.EditDVector(formLine, "Goal position of the test path",
		pEditPFTGoalPosition, new cEditPFTGoalPosition(*this));
	helper.Button(formLine, pBtnPFTGoalPosFromCamera, new cActionPFTGoalPosFromCamera(*this), true);
	
	helper.EditInteger(groupBox, "Layer:", "Layer to navigate.",
		pEditPFTLayer, new cEditPFTLayer(*this));
	
	helper.ComboBox(groupBox, "Space Type:", "Space type to navigate.",
		pCBPFTSpaceType, new cComboPFTSpaceType(*this));
	pCBPFTSpaceType->AddItem("Grid", NULL, (void*)(intptr_t)deNavigationSpace::estGrid);
	pCBPFTSpaceType->AddItem("Mesh", NULL, (void*)(intptr_t)deNavigationSpace::estMesh);
	pCBPFTSpaceType->AddItem("Volume", NULL, (void*)(intptr_t)deNavigationSpace::estVolume);
	
	helper.EditFloat(groupBox, "Blocking Cost:", "Blocking cost.",
		pEditPFTBlockingCost, new cEditPFTBlockingCost(*this));
	helper.CheckBox(groupBox, pChkPFTShowPath, new cActionPFTShowPath(*this), true);
	
	
	// find path test types
	helper.GroupBox(content, groupBox, "Path Find Test Types:", true);
	
	helper.FormLineStretchFirst(groupBox, "Type:", "Type to edit", formLine);
	helper.ComboBox(formLine, "Type to edit", pCBPFTType, new cComboPFTType(*this));
	pActionPFTTypes.TakeOver(new cActionPFTTypes(*this));
	helper.Button(formLine, pBtnPFTTypes, pActionPFTTypes, true);
	pActionPFTTypes->SetWidget(pBtnPFTTypes);
	
	helper.EditInteger(groupBox, "Type Number:", "Type number.",
		pEditPFTTypeNumber, new cEditPFTTypeNumber(*this));
	helper.EditString(groupBox, "Name:", "Name of the type.",
		pEditPFTTypeName, new cEditPFTTypeName(*this));
	helper.EditFloat(groupBox, "Fix Cost:", "Fix cost to move into type.",
		pEditPFTTypeFixCost, new cEditPFTTypeFixCost(*this));
	helper.EditFloat(groupBox, "Cost Per Meter:", "Cost per meter travelled inside type.",
		pEditPFTTypeCPM, new cEditPFTTypeCostPerMeter(*this));
	
	
	// music testing
	helper.GroupBox(content, groupBox, "Music Testing:", true);
	
	helper.EditPath(groupBox, "Path:", "Path to sound file to play.",
		igdeEnvironment::efpltSound, pEditMusicPath, new cEditMusicPath(*this));
	
	helper.EditSliderText(groupBox, "Volume:", "Volume to play music.",
		0.0f, 1.0f, 4, 2, 0.1f, pEditMusicVolume, new cEditMusicVolume(*this));
	
	helper.FormLine(groupBox, "", "", formLine);
	helper.Button(formLine, pBtnMusicPlay, new cActionMusicPlay(*this), true);
	helper.Button(formLine, pBtnMusicPause, new cActionMusicPause(*this), true);
	helper.Button(formLine, pBtnMusicStop, new cActionMusicStop(*this), true);
}

meWPWorld::~meWPWorld(){
	SetWorld(NULL);
}



// Management
///////////////

void meWPWorld::SetWorld(meWorld *world){
	if(world == pWorld){
		return;
	}
	
	meWPPropertyList &editProperties = (meWPPropertyList&)(igdeWidget&)pEditProperties;
	
	if(pWorld){
		editProperties.SetClipboard(NULL);
		editProperties.SetUndoSystem(NULL);
		editProperties.SetTriggerTargetList(NULL);
		editProperties.SetIdentifiers(decStringSet());
		
		pWorld->RemoveNotifier(pListener);
	}
	
	pWorld = world;
	
	if(world){
		world->AddNotifier(pListener);
		editProperties.SetClipboard(&pWindowProperties.GetWindowMain().GetClipboard());
		editProperties.SetUndoSystem(world->GetUndoSystem());
		editProperties.SetTriggerTargetList(&world->GetTriggerTable());
	}
	
	UpdateWorld();
	UpdateIdentifierLists();
}



void meWPWorld::UpdateWorld(){
	UpdateWorldParameters();
	UpdatePathFindTestTypeList();
	UpdatePathFindTest();
	UpdateMusic();
	UpdatePropertyKeys();
	UpdateProperties();
}

void meWPWorld::UpdateWorldParameters(){
	if(pWorld){
		pEditSize->SetDVector(pWorld->GetSize());
		pEditGravity->SetVector(pWorld->GetGravity());
		
	}else{
		pEditSize->SetDVector(decDVector(1000.0, 1000.0, 1000.0));
		pEditGravity->SetVector(decDVector(0.0f, -9.81f, 0.0f));
	}
	
	const bool enabled = pWorld;
	pEditSize->SetEnabled(enabled);
	pEditGravity->SetEnabled(enabled);
}

void meWPWorld::UpdatePathFindTest(){
	if(pWorld){
		const mePathFindTest &pft = *pWorld->GetPathFindTest();
		pEditPFTStartPosition->SetDVector(pft.GetStartPosition());
		pEditPFTGoalPosition->SetDVector(pft.GetGoalPosition());
		pEditPFTLayer->SetInteger(pft.GetLayer());
		pCBPFTSpaceType->SetSelectionWithData((void*)(intptr_t)pft.GetSpaceType());
		pEditPFTBlockingCost->SetInteger((int)(pft.GetBlockingCost() + 0.1f));
		pChkPFTShowPath->SetChecked(pft.GetShowPath());
		
	}else{
		pEditPFTStartPosition->SetDVector(decDVector());
		pEditPFTGoalPosition->SetDVector(decDVector());
		pEditPFTLayer->ClearText();
		pCBPFTSpaceType->SetSelectionWithData((void*)(intptr_t)deNavigationSpace::estMesh);
		pEditPFTBlockingCost->ClearText();
		pChkPFTShowPath->SetChecked(false);
	}
	
	const bool enabled = pWorld;
	pEditPFTStartPosition->SetEnabled(enabled);
	pEditPFTGoalPosition->SetEnabled(enabled);
	pEditPFTLayer->SetEnabled(enabled);
	pCBPFTSpaceType->SetEnabled(enabled);
	pEditPFTBlockingCost->SetEnabled(enabled);
	pChkPFTShowPath->SetEnabled(enabled);
	
	UpdatePathFindTestTypeList();
}

mePathFindTestType *meWPWorld::GetActivePathFindTestType() const{
	if(!pWorld){
		return NULL;
	}
	
	const igdeListItem * const selection = pCBPFTType->GetSelectedItem();
	return selection ? (mePathFindTestType*)selection->GetData() : NULL;
}

void meWPWorld::UpdatePathFindTestTypeList(){
	mePathFindTestType * const selection = GetActivePathFindTestType();
	
	pCBPFTType->RemoveAllItems();
	
	if(pWorld){
		const mePathFindTestTypeList &list = pWorld->GetPathFindTest()->GetTypeList();
		const int count = list.GetCount();
		decString text;
		int i;
		
		for(i=0; i<count; i++){
			mePathFindTestType * const type = list.GetAt(i);
			text.Format("%d: %s", type->GetTypeNumber(), type->GetName().GetString());
			pCBPFTType->AddItem(text, NULL, type);
		}
	}
	
	pCBPFTType->SetSelectionWithData(selection);
	
	UpdatePathFindTestType();
}

void meWPWorld::UpdatePathFindTestType(){
	mePathFindTestType * const activeType = GetActivePathFindTestType();
	
	if(activeType){
		pEditPFTTypeNumber->SetInteger(activeType->GetTypeNumber());
		pEditPFTTypeName->SetText(activeType->GetName());
		pEditPFTTypeFixCost->SetFloat(activeType->GetFixCost());
		pEditPFTTypeCPM->SetFloat(activeType->GetCostPerMeter());
		
	}else{
		pEditPFTTypeNumber->ClearText();
		pEditPFTTypeName->ClearText();
		pEditPFTTypeFixCost->ClearText();
		pEditPFTTypeCPM->ClearText();
	}
	
	const bool enabled = activeType;
	pEditPFTTypeNumber->SetEnabled(enabled);
	pEditPFTTypeName->SetEnabled(enabled);
	pEditPFTTypeFixCost->SetEnabled(enabled);
	pEditPFTTypeCPM->SetEnabled(enabled);
}


void meWPWorld::UpdateMusic(){
	if(pWorld){
		pEditMusicPath->SetPath(pWorld->GetMusic().GetPath());
		pEditMusicVolume->SetValue(pWorld->GetMusic().GetVolume());
		
	}else{
		pEditMusicPath->ClearPath();
		pEditMusicVolume->SetValue(1.0f);
	}
	
	const bool enabled = pWorld;
	pEditMusicPath->SetEnabled(enabled);
	pEditMusicVolume->SetEnabled(enabled);
	pActionMusicPlay->Update();
	pActionMusicPause->Update();
	pActionMusicStop->Update();
}


const decString &meWPWorld::GetActiveProperty() const{
	return pWorld ? pWorld->GetActiveProperty() : vEmptyString;
}

void meWPWorld::SelectActiveProperty(){
	((meWPPropertyList&)(igdeWidget&)pEditProperties).SelectProperty(GetActiveProperty());
}

void meWPWorld::UpdatePropertyKeys(){
	((meWPPropertyList&)(igdeWidget&)pEditProperties).UpdateKeys();
}

void meWPWorld::UpdateProperties(){
	((meWPPropertyList&)(igdeWidget&)pEditProperties).SetProperties(
		pWorld ? pWorld->GetProperties() : decStringDictionary());
	SelectActiveProperty();
}

void meWPWorld::UpdateIdentifierLists(){
	meWPPropertyList &editProperties = (meWPPropertyList&)(igdeWidget&)pEditProperties;
	const decString &property = GetActiveProperty();
	decStringSet identifiers;
	
	if(pWorld && !property.IsEmpty()){
		const igdeGDProperty * const gdProperty = editProperties.GetGDProperty(property);
		if(gdProperty && gdProperty->GetType() == igdeGDProperty::eptIdentifier){
			const meIDGroup * const idgroup = pWorld->GetIDGroupList().GetNamed(gdProperty->GetIdentifierGroup());
			if(idgroup){
				const decStringList &list = idgroup->GetIDList();
				const int count = list.GetCount();
				int i;
				
				for(i=0; i<count; i++){
					identifiers.Add(list.GetAt(i));
				}
			}
		}
	}
	editProperties.SetIdentifiers(identifiers);
}

void meWPWorld::SelectPFType(mePathFindTestType *type){
	pCBPFTType->SetSelectionWithData(type);
}



void meWPWorld::OnGameDefinitionChanged(){
	UpdatePropertyKeys();
	UpdateProperties();
	UpdateIdentifierLists();
}
