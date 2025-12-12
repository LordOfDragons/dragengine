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

#include "igdeDialogTexturePropertyList.h"
#include "../igdeApplication.h"
#include "../igdeUIHelper.h"
#include "../igdeListBox.h"
#include "../igdeTextArea.h"
#include "../igdeTextField.h"
#include "../igdeContainer.h"
#include "../event/igdeListBoxListener.h"
#include "../layout/igdeContainerFlow.h"
#include "../layout/igdeContainerForm.h"
#include "../layout/igdeContainerSplitted.h"
#include "../model/igdeListItem.h"
#include "../../environment/igdeEnvironment.h"
#include "../../engine/textureProperties/igdeTextureProperty.h"
#include "../../engine/textureProperties/igdeTexturePropertyList.h"

#include <dragengine/common/exceptions.h>



// Actions
///////////

class igdeDialogTexturePropertyList_ListBox : public igdeListBoxListener {
	igdeDialogTexturePropertyList &pDialog;
public:
	igdeDialogTexturePropertyList_ListBox(igdeDialogTexturePropertyList &dialog) : pDialog(dialog){}
	
	virtual void OnSelectionChanged(igdeListBox*){
		pDialog.UpdateProperty();
	}
};



// Class igdeDialogTexturePropertyList
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeDialogTexturePropertyList::igdeDialogTexturePropertyList(igdeEnvironment &environment) :
igdeDialog(environment, "Texture Property List"){
	igdeUIHelper &helper = environment.GetUIHelper();
	
	SetSize(igdeApplication::app().DisplayScaled(decPoint(1000, 500)));
	
	igdeContainerSplitted::Ref content(igdeContainerSplitted::Ref::New(
		environment, igdeContainerSplitted::espLeft, igdeApplication::app().DisplayScaled(300)));
	
	helper.ListBox(15, "Textue Property", pListProperties, new igdeDialogTexturePropertyList_ListBox(*this));
	pListProperties->SetDefaultSorter();
	content->AddChild(pListProperties, igdeContainerSplitted::eaSide);
	
	igdeContainerForm::Ref form(igdeContainerForm::Ref::New(
		environment, igdeContainerForm::esLast));
	content->AddChild(form, igdeContainerSplitted::eaCenter);
	
	helper.EditString(form, "Name:", "Property name", pEditName, NULL);
	pEditName->SetEditable(false);
	
	helper.EditString(form, "Type:", "Property type", pEditType, NULL);
	pEditType->SetEditable(false);
	
	helper.EditString(form, "Components:", "Used color components", pEditComponentCount, NULL);
	pEditComponentCount->SetEditable(false);
	
	helper.EditString(form, "Default Value:", "Default value", pEditDefault, NULL);
	pEditDefault->SetEditable(false);
	
	helper.EditString(form, "Affects:", "Modules affected by property", pEditAffectedModules, NULL);
	pEditAffectedModules->SetEditable(false);
	
	helper.EditString(form, "Description:", "Property description", pEditDescription, 40, 5, NULL);
	pEditDescription->SetEditable(false);
	
	
	igdeContainer::Ref buttonBar;
	CreateButtonBar(buttonBar, "Close");
	
	AddContent(content, buttonBar);
	
	
	UpdatePropertyList();
}

igdeDialogTexturePropertyList::~igdeDialogTexturePropertyList(){
}



// Management
///////////////

void igdeDialogTexturePropertyList::UpdatePropertyList(){
	const igdeTexturePropertyList &list = *GetEnvironment().GetTexturePropertyList();
	const igdeTextureProperty *selection = NULL;
	const int count = list.GetCount();
	int i;
	
	pListProperties->RemoveAllItems();
	
	for(i=0; i<count; i++){
		const igdeTextureProperty * const property = list.GetAt(i);
		pListProperties->AddItem(property->GetName(), NULL, (void*)property);
		if(property->GetName() == "color"){
			selection = property;
		}
	}
	
	pListProperties->SortItems();
	if(selection){
		pListProperties->SetSelectionWithData((void*)selection);
	}
	
	UpdateProperty();
}

void igdeDialogTexturePropertyList::UpdateProperty(){
	const igdeTextureProperty * const property = pListProperties->GetSelectedItem()
		? (const igdeTextureProperty *)pListProperties->GetSelectedItem()->GetData() : NULL;
	
	if(property){
		pEditName->SetText(property->GetName());
		pEditDescription->SetText(property->GetDescription());
		
		switch(property->GetType()){
		case igdeTextureProperty::eptValue:
			pEditType->SetText("Value");
			break;
			
		case igdeTextureProperty::eptColor:
			pEditType->SetText("Color");
			break;
			
		case igdeTextureProperty::eptImage:
			pEditType->SetText("Image");
			break;
			
		default:
			pEditType->SetText("?");
		}
		
		pEditComponentCount->SetInteger(property->GetComponentCount());
		
		decString defaultValue;
		if(property->GetType() == igdeTextureProperty::eptValue){
			defaultValue.Format("%g", property->GetDefaultValue());
			
		}else{
			const decColor &color = property->GetDefaultColor();
			
			if(property->GetComponentCount() == 1){
				defaultValue.Format("(%g)", color.r);
				
			}else if(property->GetComponentCount() == 2){
				defaultValue.Format("(%g, %g)", color.r, color.g);
				
			}else if(property->GetComponentCount() == 3){
				defaultValue.Format("(%g, %g, %g)", color.r, color.g, color.b);
				
			}else{
				defaultValue.Format("(%g, %g, %g, %g)", color.r, color.g, color.b, color.a);
			}
		}
		pEditDefault->SetText(defaultValue);
		
		const decStringSet &affectedList = property->GetAffectedModules();
		const int affectedCount = affectedList.GetCount();
		decStringList affects;
		int i;
		for(i=0; i<affectedCount; i++){
			affects.Add(affectedList.GetAt(i));
		}
		pEditAffectedModules->SetText(affects.Join(", "));
		
	}else{
		pEditName->ClearText();
		pEditDescription->ClearText();
		pEditType->ClearText();
		pEditComponentCount->ClearText();
		pEditDefault->ClearText();
		pEditAffectedModules->ClearText();
	}
}
