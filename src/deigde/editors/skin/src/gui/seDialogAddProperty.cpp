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

#include <stdlib.h>
#include <string.h>

#include "seWindowMain.h"
#include "seDialogAddProperty.h"
#include "../skin/seSkin.h"
#include "../skin/texture/seTexture.h"
#include "../skin/property/seProperty.h"
#include "../skin/property/node/sePropertyNodeGroup.h"

#include <deigde/engine/textureProperties/igdeTextureProperty.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeLabel.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/dialog/igdeDialogTexturePropertyList.h>
#include <deigde/gui/dialog/igdeDialog.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/resources/igdeIcon.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringList.h>



// Actions
////////////

namespace {

class cActionDetails : public igdeAction{
public:
	typedef deTObjectReference<cActionDetails> Ref;
	
private:
	seDialogAddProperty &pDialog;
	
public:
	cActionDetails(seDialogAddProperty &dialog) : igdeAction("Texture Property Details"),
	pDialog(dialog){}
	
	void OnAction() override{
		igdeDialogTexturePropertyList::Ref::New(pDialog.GetEnvironment())->Run(&pDialog);
	}
};

}



// Class seDialogAddProperty
//////////////////////////////

// Constructor, destructor
////////////////////////////

seDialogAddProperty::seDialogAddProperty(seWindowMain &windowMain) :
igdeDialog(windowMain.GetEnvironment(), "Add Property"),
pWindowMain(windowMain)
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	igdeContainer::Ref content, formLine;
	
	
	igdeLabel::Ref header(igdeLabel::Ref::New(
		env, "Multi-Select properties or enter custom name."));
	
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY, igdeContainerFlow::esLast, 5);
	
	helper.ListBox(content, 15, "Property names", pListProperties, {});
	pListProperties->SetSelectionMode(igdeListBox::esmMultiple);
	pListProperties->SetDefaultSorter();
	
	helper.Button(content, cActionDetails::Ref::New(*this));
	
	formLine = igdeContainerForm::Ref::New(env);
	helper.EditString(formLine, "Custom Name:", "Custom name (for special uses only)",
		15, pEditCustomPropertyName, {});
	content->AddChild(formLine);
	
	
	igdeContainer::Ref buttonBar;
	CreateButtonBar(buttonBar, "Add Properties", "Cancel");
	
	
	AddContent(content, header, buttonBar);
	
	
	pInitPropertyList();
	
	pListProperties->Focus();
}

seDialogAddProperty::~seDialogAddProperty(){
}



// Management
///////////////

decStringSet seDialogAddProperty::GetSelectedPropertyNames() const{
	decStringSet properties;
	
	pListProperties->GetItems().Visit([&](const igdeListItem &item){
		if(item.GetSelected()){
			properties.Add(item.GetText());
		}
	});
	
	return properties;
}

void seDialogAddProperty::SetSelectedPropertyNames(const decStringSet &list){
	pListProperties->DeselectAllItems();
	
	if(list.IsNotEmpty()){
		pListProperties->SetSelection(pListProperties->IndexOfItem(list.First()));
		
	}else{
		pListProperties->SetSelection(-1);
	}
	
	list.Visit([&](const decString &name){
		pListProperties->SelectItem(pListProperties->IndexOfItem(name));
	});
}

const decString &seDialogAddProperty::GetCustomPropertyName() const{
	return pEditCustomPropertyName->GetText();
}

void seDialogAddProperty::SetCustomPropertyName(const char *name){
	pEditCustomPropertyName->SetText(name);
}



// Private Functions
//////////////////////

void seDialogAddProperty::pInitPropertyList(){
	igdeEnvironment &environment = pWindowMain.GetEnvironment();
	seTexture &texture = *pWindowMain.GetSkin()->GetActiveTexture();
	const seProperty::List &propList = texture.GetProperties();
	igdeIcon::Ref iconUsed(environment.GetStockIcon(igdeEnvironment::esiSmallPlus));
	igdeIcon::Ref iconAvailable(environment.GetStockIcon(igdeEnvironment::esiSmallMinus));
	
	environment.GetTexturePropertyList().Visit([&](const igdeTextureProperty &property){
		const decString &propertyName = property.GetName();
		
		if(propList.HasMatching([&](const seProperty &p){
			return p.GetName() == propertyName;
		})){
			pListProperties->AddItem(propertyName, iconUsed);
			
		}else{
			pListProperties->AddItem(propertyName, iconAvailable);
		}
	});
	
	pListProperties->SortItems();
}
