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

#include "meWPAdd.h"
#include "meWPAddListener.h"
#include "meWindowProperties.h"
#include "../meWindowMain.h"
#include "../../world/meWorld.h"
#include "../../world/meWorldGuiParameters.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/igdeGDCategory.h>
#include <deigde/gamedefinition/class/igdeGDClass.h>
#include <deigde/gamedefinition/class/igdeGDClassManager.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace{

class cActionClassAdd : public igdeAction{
	meWPAdd &pPanel;
	igdeComboBoxFilter::Ref &pComboClass;
	
public:
	typedef deTObjectReference<cActionClassAdd> Ref;
	cActionClassAdd(meWPAdd &panel, igdeComboBoxFilter::Ref &comboClass) :
	igdeAction("Add", nullptr, "Add class"), pPanel(panel), pComboClass(comboClass){}
	
	void OnAction() override{
		meWorld * const world = pPanel.GetWorld();
		if(!world || pComboClass->GetText().IsEmpty()){
			return;
		}
		
		meWorldGuiParameters &guiparams = world->GetGuiParameters();
		decStringSet set(guiparams.GetAddFilterObjectSet());
		set.Add(pComboClass->GetText());
		
		guiparams.SetAddFilterObjectSet(set);
	}
};

class cActionClassRemove : public igdeAction{
	meWPAdd &pPanel;
	igdeListBox::Ref &pListBox;
	
public:
	typedef deTObjectReference<cActionClassRemove> Ref;
	cActionClassRemove(meWPAdd &panel, igdeListBox::Ref &listBox) :
	igdeAction("Remove", nullptr, "Remove selected class"), pPanel(panel), pListBox(listBox){}
	
	void OnAction() override{
		meWorld * const world = pPanel.GetWorld();
		if(!world || !pListBox->GetSelectedItem()){
			return;
		}
		
		meWorldGuiParameters &guiparams = world->GetGuiParameters();
		decStringSet set(guiparams.GetAddFilterObjectSet());
		set.Remove(pListBox->GetSelectedItem()->GetText());
		
		guiparams.SetAddFilterObjectSet(set);
	}
};

class cActionClassClear : public igdeAction{
	meWPAdd &pPanel;
	
public:
	typedef deTObjectReference<cActionClassClear> Ref;
	cActionClassClear(meWPAdd &panel) : igdeAction("Clear", nullptr, "Remove all classes"),
	pPanel(panel){}
	
	void OnAction() override{
		meWorld * const world = pPanel.GetWorld();
		if(!world){
			return;
		}
		world->GetGuiParameters().SetAddFilterObjectSet(decStringSet());
	}
};


class cActionFilterObjects : public igdeAction{
	meWPAdd &pPanel;
	
public:
	typedef deTObjectReference<cActionFilterObjects> Ref;
	cActionFilterObjects(meWPAdd &panel) : igdeAction("Enable Object Filter",
		nullptr, "Determines if objects are filtered"), pPanel(panel){ }
	
	void OnAction() override{
	}
};

class cListObjectClasses : public igdeListBoxListener{
	meWPAdd &pPanel;
	
public:
	typedef deTObjectReference<cListObjectClasses> Ref;
	cListObjectClasses(meWPAdd &panel) : pPanel(panel){}
	
	virtual void AddContextMenuEntries(igdeListBox*, igdeMenuCascade &menu){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		helper.MenuCommand(menu, pPanel.GetActionClassAdd());
		helper.MenuCommand(menu, pPanel.GetActionClassRemove());
		helper.MenuCommand(menu, pPanel.GetActionClassClear());
	}
};

class cActionObjInclusive : public igdeAction{
	meWPAdd &pPanel;
	
public:
	typedef deTObjectReference<cActionObjInclusive> Ref;
	cActionObjInclusive(meWPAdd &panel) : igdeAction("Accept if in list", nullptr,
		"Determines if objects are accepted or rejected if they are in the list."),
	pPanel(panel){}
	
	void OnAction() override{
		meWorld * const world = pPanel.GetWorld();
		if(!world){
			return;
		}
		world->GetGuiParameters().SetAddFilterObjectInclusive(
			!world->GetGuiParameters().GetAddFilterObjectInclusive());
	}
};

class cActionRandomizeYAxis : public igdeAction {
	meWPAdd &pPanel;
	
public:
	typedef deTObjectReference<cActionRandomizeYAxis> Ref;
	cActionRandomizeYAxis(meWPAdd &panel) : igdeAction("Randomize Y Axis", nullptr,
		"Determines if objects are randomized around the Y axis when added."),
		pPanel(panel){}
	
	virtual void OnAction() {
		meWorld *const world = pPanel.GetWorld();
		if(!world){
			return;
		}
		world->GetGuiParameters().SetAddRandomizeYAxis(
			!world->GetGuiParameters().GetAddRandomizeYAxis());
	}
};

}



// Class meWPAdd
//////////////////

// Constructor, destructor
////////////////////////////

meWPAdd::meWPAdd(meWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, formLine;
	
	pListener = meWPAddListener::Ref::New(*this);
	
	pActionClassAdd = cActionClassAdd::Ref::New(*this, pComboObjClass);
	pActionClassRemove = cActionClassRemove::Ref::New(*this, pListObjClasses);
	pActionClassClear = cActionClassClear::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	// object filter
	/*
	helper.GroupBoxFlow(content, groupBox, "Object Filter:");
	
	helper.CheckBoxOnly(groupBox, pChkFilterObjects, cActionFilterObjects::Ref::New(*this));
	
	formLine = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst);
	groupBox->AddChild(formLine);
	helper.ComboBoxFilter(formLine, "Object filter", pComboObjClass, {});
	pComboObjClass->SetDefaultSorter();
	helper.Button(formLine, pActionClassAdd);
	
	helper.ListBox(groupBox, 5, "List of object class filters", pListObjClasses, cListObjectClasses::Ref::New(*this));
	pListObjClasses->SetDefaultSorter();
	
	helper.CheckBoxOnly(groupBox, pChkObjInclusive, cActionObjInclusive::Ref::New(*this));
	*/
	
	// randomize
	helper.GroupBoxFlow(content, groupBox, "Randomize:");
	
	helper.CheckBoxOnly(groupBox, pChkRandomizeYAxis, cActionRandomizeYAxis::Ref::New(*this));
}

meWPAdd::~meWPAdd(){
	SetWorld(nullptr);
}



// Management
///////////////

void meWPAdd::SetWorld(meWorld *world){
	if(world == pWorld){
		return;
	}
	
	if(pWorld){
		pWorld->RemoveNotifier(pListener);
	}
	
	pWorld = world;
	
	if(world){
		world->AddNotifier(pListener);
	}
	
	UpdateParameters();
}


void meWPAdd::UpdateParameters(){
	if(pWorld){
		const meWorldGuiParameters &guiparams = pWorld->GetGuiParameters();
		pChkRandomizeYAxis->SetChecked(guiparams.GetAddRandomizeYAxis());
		
	}else{
		pChkRandomizeYAxis->SetChecked(false);
	}
	
	UpdateObjectFilter();
}

void meWPAdd::UpdateObjectFilter(){
	#if 0
	if(!pWorld){
		return;
	}
	
	const meWorldGuiParameters &guiparams = pWorld->GetGuiParameters();
	
	pChkFilterObjects->SetChecked(true);
	
	const decString selection(pListObjClasses->GetSelectedItem()
		? pListObjClasses->GetSelectedItem()->GetText() : decString());
	const decStringSet &set = guiparams.GetAddFilterObjectSet();
	const int count = set.GetCount();
	int i;
	
	pListObjClasses->RemoveAllItems();
	for(i=0; i<count; i++){
		pListObjClasses->AddItem(set.GetAt(i));
	}
	pListObjClasses->SortItems();
	
	if(!selection.IsEmpty()){
		pListObjClasses->SetSelection(pListObjClasses->IndexOfItem(selection));
	}
	
	pChkObjInclusive->SetChecked(guiparams.GetAddFilterObjectInclusive());
	#endif
}

void meWPAdd::OnGameDefinitionChanged(){
	#if 0
	const decString selection(pComboObjClass->GetText());
	
	pComboObjClass->RemoveAllItems();
	
	if(pWorld){
		const igdeGDClassManager &classes = *GetGameDefinition()->GetClassManager();
		const int count = classes.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			const igdeGDClass &objectClass = *classes.GetAt(i);
			if(objectClass.GetCanInstantiate()){
				pComboObjClass->AddItem(objectClass.GetName());
			}
		}
		
		pComboObjClass->SortItems();
		pComboObjClass->StoreFilterItems();
	}
	
	pComboObjClass->SetText(selection);
	#endif
}
