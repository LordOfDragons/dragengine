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

#include "gdeWPSParticleEmitter.h"
#include "gdeWPSParticleEmitterListener.h"
#include "../gdeWindowProperties.h"
#include "../../gdeWindowMain.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/category/gdeCategory.h"
#include "../../../gamedef/filepattern/gdeFilePattern.h"
#include "../../../gamedef/particleemitter/gdeParticleEmitter.h"
#include "../../../undosys/particleemitter/gdeUParticleEmitterSetPath.h"
#include "../../../undosys/particleemitter/gdeUParticleEmitterSetName.h"
#include "../../../undosys/particleemitter/gdeUParticleEmitterSetDescription.h"
#include "../../../undosys/particleemitter/gdeUParticleEmitterSetCategory.h"

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
///////////

namespace{

class cEditPath : public igdeEditPathListener{
	gdeWPSParticleEmitter &pPanel;
	
public:
	typedef deTObjectReference<cEditPath> Ref;
	cEditPath(gdeWPSParticleEmitter &panel) : pPanel(panel){}
	
	virtual void OnEditPathChanged(igdeEditPath *editPath){
		gdeParticleEmitter * const particleEmitter = pPanel.GetParticleEmitter();
		if(!particleEmitter || particleEmitter->GetPath() == editPath->GetPath()){
			return;
		}
		
		if(pPanel.GetGameDefinition()->GetParticleEmitters().HasMatching([&](const gdeParticleEmitter &pe){
			return pe.GetPath() == editPath->GetPath();
		})){
			igdeCommonDialogs::Information(pPanel.GetParentWindow(), "Change particle emitter path",
				"A particle emitter with this path exists already.");
			editPath->SetPath(particleEmitter->GetPath());
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUParticleEmitterSetPath::Ref::New(
			particleEmitter, editPath->GetPath()));
	}
};

class cTextName : public igdeTextFieldListener{
	gdeWPSParticleEmitter &pPanel;
	
public:
	typedef deTObjectReference<cTextName> Ref;
	cTextName(gdeWPSParticleEmitter &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		gdeParticleEmitter * const particleEmitter = pPanel.GetParticleEmitter();
		if(!particleEmitter || particleEmitter->GetName() == textField->GetText()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(gdeUParticleEmitterSetName::Ref::New(
			particleEmitter, textField->GetText()));
	}
};

class cTextDescription : public igdeTextAreaListener{
	gdeWPSParticleEmitter &pPanel;
	
public:
	typedef deTObjectReference<cTextDescription> Ref;
	cTextDescription(gdeWPSParticleEmitter &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextArea *textArea){
		gdeParticleEmitter * const particleEmitter = pPanel.GetParticleEmitter();
		if(!particleEmitter || particleEmitter->GetDescription() == textArea->GetDescription()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUParticleEmitterSetDescription::Ref::New(particleEmitter, textArea->GetText()));
	}
};

class cComboCategory : public igdeComboBoxListener{
	gdeWPSParticleEmitter &pPanel;
	
public:
	typedef deTObjectReference<cComboCategory> Ref;
	cComboCategory(gdeWPSParticleEmitter &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		gdeParticleEmitter * const particleEmitter = pPanel.GetParticleEmitter();
		if(!particleEmitter || particleEmitter->GetCategory() == comboBox->GetText()){
			return;
		}
		
		pPanel.GetGameDefinition()->GetUndoSystem()->Add(
			gdeUParticleEmitterSetCategory::Ref::New(particleEmitter, comboBox->GetText()));
	}
};

class cActionJumpToCategory : public igdeAction{
	gdeWPSParticleEmitter &pPanel;
	
public:
	typedef deTObjectReference<cActionJumpToCategory> Ref;
	cActionJumpToCategory(gdeWPSParticleEmitter &panel) : 
	igdeAction("", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallStrongRight),
		"Jump to Category"),
	pPanel(panel){}
	
	void OnAction() override{
		gdeParticleEmitter * const particleEmitter = pPanel.GetParticleEmitter();
		if(!particleEmitter){
			return;
		}
		
		gdeGameDefinition &gameDefinition = *pPanel.GetGameDefinition();
		gdeCategory * const category = gameDefinition.GetCategoriesParticleEmitter()
			.GetWithPath(particleEmitter->GetCategory());
		if(!category){
			return;
		}
		
		gameDefinition.SetActiveCategory(category);
		gameDefinition.SetSelectedObjectType(gdeGameDefinition::eotCategoryParticleEmitter);
	}
	
	void Update() override{
		SetEnabled(pPanel.GetParticleEmitter());
	}
};

}



// Class gdeWPSParticleEmitter
////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSParticleEmitter::gdeWPSParticleEmitter(gdeWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, frameLine;
	
	pListener = gdeWPSParticleEmitterListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	// particle emitter
	helper.GroupBox(content, groupBox, "Particle Emitter:");
	helper.EditPath(groupBox, "Path:", "Path to particle emitter",
		igdeEnvironment::efpltParticleEmitter, pEditPath, cEditPath::Ref::New(*this));
	helper.EditString(groupBox, "Name:", "Particle emitter name", pEditName, cTextName::Ref::New(*this));
	helper.EditString(groupBox, "Description:", "Particle emitter description",
		pEditDescription, 15, 5, cTextDescription::Ref::New(*this));
	
	helper.FormLineStretchFirst(groupBox, "Category: ", "Category", frameLine);
	helper.ComboBoxFilter(frameLine, true, "Category", pCBCategory, cComboCategory::Ref::New(*this));
	pCBCategory->SetDefaultSorter();
	pCBCategory->SetFilterCaseInsentive(true);
	helper.Button(frameLine, pBtnJumpToCategory, cActionJumpToCategory::Ref::New(*this));
}

gdeWPSParticleEmitter::~gdeWPSParticleEmitter(){
	SetGameDefinition(nullptr);
}



// Management
///////////////

void gdeWPSParticleEmitter::SetGameDefinition(gdeGameDefinition *gameDefinition){
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
	
	UpdateParticleEmitter();
	
	UpdateCategoryList();
}



gdeParticleEmitter *gdeWPSParticleEmitter::GetParticleEmitter() const{
	return pGameDefinition ? pGameDefinition->GetActiveParticleEmitter() : nullptr;
}



void gdeWPSParticleEmitter::UpdateCategoryList(){
	const decString selection(pCBCategory->GetText());
	
	pCBCategory->RemoveAllItems();
	
	if(pGameDefinition){
		const gdeCategoryList &categories = pGameDefinition->GetCategoriesParticleEmitter();
		if(categories.GetCount() > 0){
			UpdateCategoryList(categories, "");
		}
		
		pCBCategory->SortItems();
		pCBCategory->StoreFilterItems();
	}
	
	pCBCategory->SetText(selection);
	pCBCategory->SetInvalidValue(!pCBCategory->GetText().IsEmpty() && !pCBCategory->GetSelectedItem());
}

void gdeWPSParticleEmitter::UpdateCategoryList(const gdeCategoryList &list, const char *prefix){
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



void gdeWPSParticleEmitter::UpdateParticleEmitter(){
	const gdeParticleEmitter * const particleEmitter = GetParticleEmitter();
	
	if(particleEmitter){
		pEditPath->SetPath(particleEmitter->GetPath());
		pEditName->SetText(particleEmitter->GetName());
		pEditDescription->SetText(particleEmitter->GetDescription());
		pCBCategory->SetText(particleEmitter->GetCategory());
		pCBCategory->SetInvalidValue(!pCBCategory->GetText().IsEmpty()
			&& !pCBCategory->GetSelectedItem());
		
	}else{
		pEditPath->ClearPath();
		pEditName->ClearText();
		pEditDescription->ClearText();
		pCBCategory->ClearText();
		pCBCategory->SetInvalidValue(false);
	}
	
	const bool enabled = particleEmitter != nullptr;
	pEditPath->SetEnabled(enabled);
	pEditName->SetEnabled(enabled);
	pEditDescription->SetEnabled(enabled);
	pCBCategory->SetEnabled(enabled);
}
