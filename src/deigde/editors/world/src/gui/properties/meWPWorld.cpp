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
		
		igdeUndo::Ref undo(OnChanged(textField, world));
		if(undo){
			world->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, meWorld *world) = 0;
};

class cBaseAction : public igdeAction{
protected:
	meWPWorld &pPanel;
	
public:
	cBaseAction(meWPWorld &panel, const char *text, igdeIcon *icon, const char *description) :
	igdeAction(text, icon, description),
	pPanel(panel){}
	
	void OnAction() override{
		meWorld * const world = pPanel.GetWorld();
		if(!world){
			return;
		}
		
		igdeUndo::Ref undo(OnAction(world));
		if(undo){
			world->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnAction(meWorld *world) = 0;
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
		
		igdeUndo::Ref undo(OnChanged(comboBox, world));
		if(undo){
			world->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeComboBox *comboBox, meWorld *world) = 0;
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
		
		igdeUndo::Ref undo(OnChanged(editVector->GetVector(), world));
		if(undo){
			world->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(const decVector &vector, meWorld *world) = 0;
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
		
		igdeUndo::Ref undo(OnChanged(editDVector->GetDVector(), world));
		if(undo){
			world->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(const decDVector &vector, meWorld *world) = 0;
};


class cEditWorldProperties : public meWPPropertyList {
	meWPWorld &pPanel;
	
public:
	typedef deTObjectReference<cEditWorldProperties> Ref;
	cEditWorldProperties(meWPWorld &panel) : meWPPropertyList(panel.GetEnvironment()), pPanel(panel){}
	
	virtual decString GetGDDefaultValue(const char *key) const{
		const igdeGDProperty * const gdProperty = GetGDProperty(key);
		return gdProperty ? gdProperty->GetDefaultValue() : vEmptyString;
	}
	
	const igdeGDProperty *GetGDProperty(const char *key) const override{
		const meWorld * const world = pPanel.GetWorld();
		return world ? world->GetGameDefinition()->GetListWorldProperties().FindOrDefault([&](const igdeGDProperty &p){
			return p.GetName() == key;
		}) : nullptr;
	}
	
	virtual decStringSet GetGDPropertyKeys() const{
		const meWorld * const world = pPanel.GetWorld();
		decStringSet keys;
		if(world){
			const igdeGDProperty::List &list = world->GetGameDefinition()->GetListWorldProperties();
			list.Visit([&](igdeGDProperty::Ref &prop){
				keys.Add(prop->GetName());
			});
		}
		return keys;
	}
	
	virtual void OnPropertySelected(const decString &key, const decString &){
		meWorld * const world = pPanel.GetWorld();
		if(world){
			world->SetActiveProperty(key);
		}
	}
	
	virtual igdeUndo::Ref UndoAddProperty(const decString &key, const decString &value){
		meWorld * const world = pPanel.GetWorld();
		return world ? meUWorldAddProperty::Ref::New(world, key, value) : meUWorldAddProperty::Ref();
	}
	
	virtual igdeUndo::Ref UndoRemoveProperty(const decString &key){
		meWorld * const world = pPanel.GetWorld();
		return world ? meUWorldRemoveProperty::Ref::New(world, key, world->GetProperties().GetAt(key)) : meUWorldRemoveProperty::Ref();
	}
	
	virtual igdeUndo::Ref UndoSetProperty(const decString &key, const decString &oldValue, const decString &newValue){
		meWorld * const world = pPanel.GetWorld();
		return world ? meUWorldSetProperty::Ref::New(world, key, oldValue, newValue) : meUWorldSetProperty::Ref();
	}
	
	virtual igdeUndo::Ref UndoSetProperties(const decStringDictionary &properties){
		meWorld * const world = pPanel.GetWorld();
		return world ? meUWorldSetProperties::Ref::New(world, properties) : meUWorldSetProperties::Ref();
	}
};


class cEditSize : public cBaseEditDVectorListener{
public:
	typedef deTObjectReference<cEditSize> Ref;
	cEditSize(meWPWorld &panel) : cBaseEditDVectorListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(const decDVector &vector, meWorld *world){
		return !world->GetSize().IsEqualTo(vector) ? meUWorldSetSize::Ref::New(world, vector) : meUWorldSetSize::Ref();
	}
};


class cEditGravity : public cBaseEditVectorListener{
public:
	typedef deTObjectReference<cEditGravity> Ref;
	cEditGravity(meWPWorld &panel) : cBaseEditVectorListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(const decVector &vector, meWorld *world){
		return !world->GetGravity().IsEqualTo(vector) ? meUWorldSetGravity::Ref::New(world, vector) : meUWorldSetGravity::Ref();
	}
};

class cEditPFTStartPosition : public cBaseEditDVectorListener{
public:
	typedef deTObjectReference<cEditPFTStartPosition> Ref;
	cEditPFTStartPosition(meWPWorld &panel) : cBaseEditDVectorListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(const decDVector &vector, meWorld *world){
		world->GetPathFindTest()->SetStartPosition(vector);
		return {};
	}
};

class cActionPFTStartPosFromCamera : public cBaseAction{
public:
	typedef deTObjectReference<cActionPFTStartPosFromCamera> Ref;
	cActionPFTStartPosFromCamera(meWPWorld &panel) : cBaseAction(panel,
		"Set", nullptr, "Set start position from camera position"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		world->GetPathFindTest()->SetStartPosition(world->GetActiveCamera()->GetPosition());
		return {};
	}
};

class cEditPFTGoalPosition : public cBaseEditDVectorListener{
public:
	typedef deTObjectReference<cEditPFTGoalPosition> Ref;
	cEditPFTGoalPosition(meWPWorld &panel) : cBaseEditDVectorListener(panel){}
	
	virtual igdeUndo::Ref  OnChanged(const decDVector &vector, meWorld *world){
		world->GetPathFindTest()->SetGoalPosition(vector);
		return {};
	}
};

class cActionPFTGoalPosFromCamera : public cBaseAction{
public:
	typedef deTObjectReference<cActionPFTGoalPosFromCamera> Ref;
	cActionPFTGoalPosFromCamera(meWPWorld &panel) : cBaseAction(panel,
		"Set", nullptr, "Set goal position from camera position"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		world->GetPathFindTest()->SetGoalPosition(world->GetActiveCamera()->GetPosition());
		return {};
	}
};

class cEditPFTLayer : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cEditPFTLayer> Ref;
	cEditPFTLayer(meWPWorld &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, meWorld *world) override{
		world->GetPathFindTest()->SetLayer(textField->GetInteger());
		return {};
	}
};

class cComboPFTSpaceType : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboPFTSpaceType> Ref;
	cComboPFTSpaceType(meWPWorld &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox *comboBox, meWorld *world) override{
		if(comboBox->GetSelectedItem()){
			world->GetPathFindTest()->SetSpaceType((deNavigationSpace::eSpaceTypes)
				(intptr_t)comboBox->GetSelectedItem()->GetData());
		}
		return {};
	}
};

class cEditPFTBlockingCost : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cEditPFTBlockingCost> Ref;
	cEditPFTBlockingCost(meWPWorld &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, meWorld *world) override{
		world->GetPathFindTest()->SetBlockingCost(textField->GetFloat());
		return {};
	}
};

class cActionPFTShowPath : public cBaseAction{
public:
	typedef deTObjectReference<cActionPFTShowPath> Ref;
	cActionPFTShowPath(meWPWorld &panel) : cBaseAction(panel,
		"Show Path", nullptr, "Show path"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		world->GetPathFindTest()->SetShowPath(!world->GetPathFindTest()->GetShowPath());
		return {};
	}
};


class cComboPFTType : public cBaseComboBoxListener{
public:
	typedef deTObjectReference<cComboPFTType> Ref;
	cComboPFTType(meWPWorld &panel) : cBaseComboBoxListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeComboBox*, meWorld*) override{
		pPanel.UpdatePathFindTestType();
		return {};
	}
};

class cActionPFTTypeAdd : public cBaseAction{
public:
	typedef deTObjectReference<cActionPFTTypeAdd> Ref;
	cActionPFTTypeAdd(meWPWorld &panel) : cBaseAction(panel, "Add...",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add type"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		mePathFindTestType::List &list = world->GetPathFindTest()->GetTypeList();
		int newValue = list.Inject(0, [](int v, const mePathFindTestType &t){
			return decMath::max(v, t.GetTypeNumber() + 1);
		});
		
		while(igdeCommonDialogs::GetInteger(&pPanel, "Add Type", "Type Number:", newValue)){
			if(list.HasMatching([&](const mePathFindTestType &t){
				return t.GetTypeNumber() == newValue;
			})){
				igdeCommonDialogs::Error(&pPanel, "Add Type", "A type with this type number exists already.");
				continue;
			}
			
			const mePathFindTestType::Ref type(mePathFindTestType::Ref::New(newValue));
			list.Add(type);
			world->GetPathFindTest()->NotifyTypesChanged();
			pPanel.SelectPFType(type);
			break;
		}
		return {};
	}
	
	void Update() override{
		SetEnabled(pPanel.GetWorld());
	}
};

class cActionPFTTypeRemove : public cBaseAction{
public:
	typedef deTObjectReference<cActionPFTTypeRemove> Ref;
	cActionPFTTypeRemove(meWPWorld &panel) : cBaseAction(panel, "Remove",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove type"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		mePathFindTestType * const type = pPanel.GetActivePathFindTestType();
		if(type){
			world->GetPathFindTest()->GetTypeList().Remove(type);
			world->GetPathFindTest()->NotifyTypesChanged();
		}
		return {};
	}
	
	void Update() override{
		SetEnabled(pPanel.GetActivePathFindTestType());
	}
};

class cActionPFTTypeClear : public cBaseAction{
public:
	typedef deTObjectReference<cActionPFTTypeClear> Ref;
	cActionPFTTypeClear(meWPWorld &panel) : cBaseAction(panel, "Clear",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove all types"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		if(world->GetPathFindTest()->GetTypeList().IsNotEmpty()){
			world->GetPathFindTest()->GetTypeList().RemoveAll();
			world->GetPathFindTest()->NotifyTypesChanged();
		}
		return {};
	}
	
	void Update() override{
		SetEnabled(pPanel.GetWorld() && pPanel.GetWorld()->GetPathFindTest()->GetTypeList().IsNotEmpty());
	}
};

class cActionPFTTypes : public igdeActionContextMenu{
	meWPWorld &pPanel;
	
public:
	typedef deTObjectReference<cActionPFTTypes> Ref;
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
	typedef deTObjectReference<cEditPFTTypeNumber> Ref;
	cEditPFTTypeNumber(meWPWorld &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, meWorld *world) override{
		mePathFindTestType * const type = pPanel.GetActivePathFindTestType();
		const int value = textField->GetInteger();
		if(!type || value == type->GetTypeNumber()){
			return {};
		}
		
		if(pPanel.GetWorld()->GetPathFindTest()->GetTypeList().HasMatching([&](const mePathFindTestType &t){
			return t.GetTypeNumber() == value;
		})){
			igdeCommonDialogs::Error(&pPanel, "Change Type Number", "Type number already exists");
			textField->SetInteger(type->GetTypeNumber());
			
		}else{
			type->SetTypeNumber(value);
			world->GetPathFindTest()->NotifyTypesChanged();
		}
		return {};
	}
};

class cEditPFTTypeName : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cEditPFTTypeName> Ref;
	cEditPFTTypeName(meWPWorld &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, meWorld *world) override{
		mePathFindTestType * const type = pPanel.GetActivePathFindTestType();
		if(type && textField->GetText() != type->GetName()){
			type->SetName(textField->GetText());
			world->GetPathFindTest()->NotifyTypesChanged();
		}
		return {};
	}
};

class cEditPFTTypeFixCost : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cEditPFTTypeFixCost> Ref;
	cEditPFTTypeFixCost(meWPWorld &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, meWorld *world) override{
		mePathFindTestType * const type = pPanel.GetActivePathFindTestType();
		const float value = textField->GetFloat();
		if(type && fabsf(value - type->GetFixCost()) > FLOAT_SAFE_EPSILON){
			type->SetFixCost(value);
			world->GetPathFindTest()->NotifyTypesChanged();
		}
		return {};
	}
};

class cEditPFTTypeCostPerMeter : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cEditPFTTypeCostPerMeter> Ref;
	cEditPFTTypeCostPerMeter(meWPWorld &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, meWorld *world) override{
		mePathFindTestType * const type = pPanel.GetActivePathFindTestType();
		const float value = textField->GetFloat();
		if(type && fabsf(value - type->GetCostPerMeter()) > FLOAT_SAFE_EPSILON){
			type->SetCostPerMeter(value);
			world->GetPathFindTest()->NotifyTypesChanged();
		}
		return {};
	}
};


class cEditMusicPath : public igdeEditPathListener{
	meWPWorld &pPanel;
public:
	typedef deTObjectReference<cEditMusicPath> Ref;
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
	typedef deTObjectReference<cEditMusicVolume> Ref;
	cEditMusicVolume(meWPWorld &panel) : pPanel(panel){}
	
	void OnSliderTextValueChanging(igdeEditSliderText *sliderText) override{
		if(pPanel.GetWorld()){
			pPanel.GetWorld()->GetMusic().SetVolume(sliderText->GetValue());
		}
	}
};

class cActionMusicPlay : public cBaseAction{
public:
	typedef deTObjectReference<cActionMusicPlay> Ref;
	cActionMusicPlay(meWPWorld &panel) : cBaseAction(panel, "Play", nullptr, "Play"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		world->GetMusic().Play();
		return {};
	}
	
	void Update() override{
		SetEnabled(pPanel.GetWorld());
	}
};

class cActionMusicPause : public cBaseAction{
public:
	typedef deTObjectReference<cActionMusicPause> Ref;
	cActionMusicPause(meWPWorld &panel) : cBaseAction(panel, "Pause", nullptr, "Pause"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		world->GetMusic().Pause();
		return {};
	}
	
	void Update() override{
		SetEnabled(pPanel.GetWorld());
	}
};

class cActionMusicStop : public cBaseAction{
public:
	typedef deTObjectReference<cActionMusicStop> Ref;
	cActionMusicStop(meWPWorld &panel) : cBaseAction(panel, "Stop", nullptr, "Stop"){}
	
	igdeUndo::Ref OnAction(meWorld *world) override{
		world->GetMusic().Stop();
		return {};
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
	igdeContainer::Ref content, groupBox, formLine;
	
	pListener = meWPWorldListener::Ref::New(*this);
	
	
	pActionPFTTypeAdd = cActionPFTTypeAdd::Ref::New(*this);
	pActionPFTTypeRemove = cActionPFTTypeRemove::Ref::New(*this);
	pActionPFTTypeClear = cActionPFTTypeClear::Ref::New(*this);
	pActionMusicPlay = cActionMusicPlay::Ref::New(*this);
	pActionMusicPause = cActionMusicPause::Ref::New(*this);
	pActionMusicStop = cActionMusicStop::Ref::New(*this);
	
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	
	// parameters
	helper.GroupBox(content, groupBox, "World Parameters:");
	
	helper.EditDVector(groupBox, "Size", "Size of world in meters where modules can expect content",
		8, 0, pEditSize, cEditSize::Ref::New(*this));
	
	helper.EditVector(groupBox, "Gravity", "World gravity", pEditGravity, cEditGravity::Ref::New(*this));
	
	
	// properties
	helper.GroupBoxFlow(content, groupBox, "World Properties:", false, false);
	
	pEditProperties = cEditWorldProperties::Ref::New(*this);
	groupBox->AddChild(pEditProperties);
	
	
	// find path test
	helper.GroupBox(content, groupBox, "Path Find Test:", true);
	
	helper.FormLineStretchFirst(groupBox, "Start Position:", "Start position of the test path", formLine);
	helper.EditDVector(formLine, "Start position of the test path",
		pEditPFTStartPosition, cEditPFTStartPosition::Ref::New(*this));
	helper.Button(formLine, pBtnPFTStartPosFromCamera, cActionPFTStartPosFromCamera::Ref::New(*this));
	
	helper.FormLineStretchFirst(groupBox, "Goal Position:", "Goal position of the test path", formLine);
	helper.EditDVector(formLine, "Goal position of the test path",
		pEditPFTGoalPosition, cEditPFTGoalPosition::Ref::New(*this));
	helper.Button(formLine, pBtnPFTGoalPosFromCamera, cActionPFTGoalPosFromCamera::Ref::New(*this));
	
	helper.EditInteger(groupBox, "Layer:", "Layer to navigate.",
		pEditPFTLayer, cEditPFTLayer::Ref::New(*this));
	
	helper.ComboBox(groupBox, "Space Type:", "Space type to navigate.",
		pCBPFTSpaceType, cComboPFTSpaceType::Ref::New(*this));
	pCBPFTSpaceType->AddItem("Grid", nullptr, (void*)(intptr_t)deNavigationSpace::estGrid);
	pCBPFTSpaceType->AddItem("Mesh", nullptr, (void*)(intptr_t)deNavigationSpace::estMesh);
	pCBPFTSpaceType->AddItem("Volume", nullptr, (void*)(intptr_t)deNavigationSpace::estVolume);
	
	helper.EditFloat(groupBox, "Blocking Cost:", "Blocking cost.",
		pEditPFTBlockingCost, cEditPFTBlockingCost::Ref::New(*this));
	helper.CheckBox(groupBox, pChkPFTShowPath, cActionPFTShowPath::Ref::New(*this));
	
	
	// find path test types
	helper.GroupBox(content, groupBox, "Path Find Test Types:", true);
	
	helper.FormLineStretchFirst(groupBox, "Type:", "Type to edit", formLine);
	helper.ComboBox(formLine, "Type to edit", pCBPFTType, cComboPFTType::Ref::New(*this));
	pActionPFTTypes = cActionPFTTypes::Ref::New(*this);
	helper.Button(formLine, pBtnPFTTypes, pActionPFTTypes);
	pActionPFTTypes->SetWidget(pBtnPFTTypes);
	
	helper.EditInteger(groupBox, "Type Number:", "Type number.",
		pEditPFTTypeNumber, cEditPFTTypeNumber::Ref::New(*this));
	helper.EditString(groupBox, "Name:", "Name of the type.",
		pEditPFTTypeName, cEditPFTTypeName::Ref::New(*this));
	helper.EditFloat(groupBox, "Fix Cost:", "Fix cost to move into type.",
		pEditPFTTypeFixCost, cEditPFTTypeFixCost::Ref::New(*this));
	helper.EditFloat(groupBox, "Cost Per Meter:", "Cost per meter travelled inside type.",
		pEditPFTTypeCPM, cEditPFTTypeCostPerMeter::Ref::New(*this));
	
	
	// music testing
	helper.GroupBox(content, groupBox, "Music Testing:", true);
	
	helper.EditPath(groupBox, "Path:", "Path to sound file to play.",
		igdeEnvironment::efpltSound, pEditMusicPath, cEditMusicPath::Ref::New(*this));
	
	helper.EditSliderText(groupBox, "Volume:", "Volume to play music.",
		0.0f, 1.0f, 4, 2, 0.1f, pEditMusicVolume, cEditMusicVolume::Ref::New(*this));
	
	helper.FormLine(groupBox, "", "", formLine);
	helper.Button(formLine, pBtnMusicPlay, cActionMusicPlay::Ref::New(*this));
	helper.Button(formLine, pBtnMusicPause, cActionMusicPause::Ref::New(*this));
	helper.Button(formLine, pBtnMusicStop, cActionMusicStop::Ref::New(*this));
}

meWPWorld::~meWPWorld(){
	SetWorld(nullptr);
}



// Management
///////////////

void meWPWorld::SetWorld(meWorld *world){
	if(world == pWorld){
		return;
	}
	
	meWPPropertyList &editProperties = (meWPPropertyList&)(igdeWidget&)pEditProperties;
	
	if(pWorld){
		editProperties.SetClipboard(nullptr);
		editProperties.SetUndoSystem(nullptr);
		editProperties.SetTriggerTargetList(nullptr);
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
		return nullptr;
	}
	
	const igdeListItem * const selection = pCBPFTType->GetSelectedItem();
	return selection ? (mePathFindTestType*)selection->GetData() : nullptr;
}

void meWPWorld::UpdatePathFindTestTypeList(){
	mePathFindTestType * const selection = GetActivePathFindTestType();
	
	pCBPFTType->RemoveAllItems();
	
	if(pWorld){
		pWorld->GetPathFindTest()->GetTypeList().Visit([&](mePathFindTestType *type){
			decString text;
			text.Format("%d: %s", type->GetTypeNumber(), type->GetName().GetString());
			pCBPFTType->AddItem(text, nullptr, type);
		});
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
