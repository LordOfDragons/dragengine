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

#include "gdeWPSSkin.h"
#include "gdeWPSSkinListener.h"
#include "../gdeWindowProperties.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/category/gdeCategory.h"
#include "../../../gamedef/filepattern/gdeFilePattern.h"
#include "../../../gamedef/skin/gdeSkin.h"
#include "../../../undosys/skin/gdeUSkinSetPath.h"
#include "../../../undosys/skin/gdeUSkinSetName.h"
#include "../../../undosys/skin/gdeUSkinSetDescription.h"
#include "../../../undosys/skin/gdeUSkinSetCategory.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeWindow.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeTextAreaListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringList.h>



// Actions
////////////

namespace{

class cEditPath : public igdeEditPathListener {
	gdeWPSSkin &pPanel;
	
public:
	cEditPath(gdeWPSSkin &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		gdeSkin * const skin = pPanel.GetSkin();
		if(! skin || skin->GetPath() == editPath->GetPath()){
			return;
		}
		
		if(pPanel.GetGameDefinition()->GetSkins().HasWithPath(editPath->GetPath())){
			igdeCommonDialogs::Information(pPanel.GetParentWindow(), "Change skin emitter path",
				"A skin emitter with this path exists already.");
			editPath->SetPath(skin->GetPath());
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUSkinSetPath::Ref::NewWith(skin, editPath->GetPath()));
	}
};

class cTextName : public igdeTextFieldListener {
	gdeWPSSkin &pPanel;
	
public:
	cTextName(gdeWPSSkin &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		gdeSkin * const skin = pPanel.GetSkin();
		if(! skin || skin->GetName() == textField->GetText()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUSkinSetName::Ref::NewWith(skin, textField->GetText()));
	}
};

class cTextDescription : public igdeTextAreaListener {
	gdeWPSSkin &pPanel;
	
public:
	cTextDescription(gdeWPSSkin &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextArea *textArea){
		gdeSkin * const skin = pPanel.GetSkin();
		if(! skin || skin->GetDescription() == textArea->GetDescription()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUSkinSetDescription::Ref::NewWith(skin, textArea->GetText()));
	}
};

class cComboCategory : public igdeComboBoxListener {
	gdeWPSSkin &pPanel;
	
public:
	cComboCategory(gdeWPSSkin &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		gdeSkin * const skin = pPanel.GetSkin();
		if(! skin || skin->GetCategory() == comboBox->GetText()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUSkinSetCategory::Ref::NewWith(skin, comboBox->GetText()));
	}
};

class cActionJumpToCategory : public igdeAction {
	gdeWPSSkin &pPanel;
	
public:
	cActionJumpToCategory(gdeWPSSkin &panel) : 
	igdeAction("", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallStrongRight),
		"Jump to Category"),
	pPanel(panel){}
	
	virtual void OnAction(){
		gdeSkin * const skin = pPanel.GetSkin();
		if(! skin){
			return;
		}
		
		gdeGameDefinition &gameDefinition = *pPanel.GetGameDefinition();
		gdeCategory * const category = gameDefinition.GetCategoriesSkin()
			.GetWithPath(skin->GetCategory());
		if(! category){
			return;
		}
		
		gameDefinition.SetActiveCategory(category);
		gameDefinition.SetSelectedObjectType(gdeGameDefinition::eotCategorySkin);
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetSkin());
	}
};

}



// Class gdeWPSSkin
/////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSSkin::gdeWPSSkin(gdeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties),
pListener(NULL),
pGameDefinition(NULL)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, frameLine;
	
	pListener = new gdeWPSSkinListener(*this);
	
	content.TakeOver(new igdeContainerFlow(env, igdeContainerFlow::eaY));
	AddChild(content);
	
	// skin
	helper.GroupBox(content, groupBox, "Skin:");
	helper.EditPath(groupBox, "Path:", "Path to skin emitter",
		igdeEnvironment::efpltSkin, pEditPath, new cEditPath(*this));
	helper.EditString(groupBox, "Name:", "Skin name", pEditName, new cTextName(*this));
	helper.EditString(groupBox, "Description:", "Skin description",
		pEditDescription, 15, 5, new cTextDescription(*this));
	
	helper.FormLineStretchFirst(groupBox, "Category: ", "Category", frameLine);
	helper.ComboBoxFilter(frameLine, true, "Category", pCBCategory, new cComboCategory(*this));
	pCBCategory->SetEditable(true);
	pCBCategory->SetDefaultSorter();
	pCBCategory->SetFilterCaseInsentive(true);
	helper.Button(frameLine, pBtnJumpToCategory, new cActionJumpToCategory(*this), true);
}

gdeWPSSkin::~gdeWPSSkin(){
	SetGameDefinition(NULL);
	
	if(pListener){
		pListener->FreeReference();
	}
}



// Management
///////////////

void gdeWPSSkin::SetGameDefinition(gdeGameDefinition *gameDefinition){
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
	
	UpdateSkin();
	
	UpdateCategoryList();
}



gdeSkin *gdeWPSSkin::GetSkin() const{
	return pGameDefinition ? pGameDefinition->GetActiveSkin() : NULL;
}



void gdeWPSSkin::UpdateCategoryList(){
	const decString selection(pCBCategory->GetText());
	
	pCBCategory->RemoveAllItems();
	
	if(pGameDefinition){
		const gdeCategoryList &categories = pGameDefinition->GetCategoriesSkin();
		if(categories.GetCount() > 0){
			UpdateCategoryList(categories, "");
		}
		
		pCBCategory->SortItems();
		pCBCategory->StoreFilterItems();
	}
	
	pCBCategory->SetText(selection);
	pCBCategory->SetInvalidValue(! pCBCategory->GetText().IsEmpty() && ! pCBCategory->GetSelectedItem());
}

void gdeWPSSkin::UpdateCategoryList(const gdeCategoryList &list, const char *prefix){
	const int count = list.GetCount();
	decString text;
	int i;
	
	for(i=0; i<count; i++){
		const gdeCategory &category = *list.GetAt(i);
		text.Format("%s%s", prefix, category.GetName().GetString());
		pCBCategory->AddItem(text);
		if(category.GetCategories().GetCount() > 0){
			text.AppendCharacter('/');
			UpdateCategoryList(category.GetCategories(), text);
		}
	}
}



void gdeWPSSkin::UpdateSkin(){
	const gdeSkin * const skin = GetSkin();
	
	if(skin){
		pEditPath->SetPath(skin->GetPath());
		pEditName->SetText(skin->GetName());
		pEditDescription->SetText(skin->GetDescription());
		pCBCategory->SetText(skin->GetCategory());
		pCBCategory->SetInvalidValue(! pCBCategory->GetText().IsEmpty()
			&& ! pCBCategory->GetSelectedItem());
		
	}else{
		pEditPath->ClearPath();
		pEditName->ClearText();
		pEditDescription->ClearText();
		pCBCategory->ClearText();
		pCBCategory->SetInvalidValue(false);
	}
	
	const bool enabled = skin != NULL;
	pEditPath->SetEnabled(enabled);
	pEditName->SetEnabled(enabled);
	pEditDescription->SetEnabled(enabled);
	pCBCategory->SetEnabled(enabled);
}
