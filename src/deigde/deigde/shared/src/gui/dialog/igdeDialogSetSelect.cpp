/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "igdeDialogSetSelect.h"
#include "../igdeApplication.h"
#include "../igdeUIHelper.h"
#include "../igdeLabel.h"
#include "../igdeListBox.h"
#include "../igdeTextField.h"
#include "../igdeContainer.h"
#include "../event/igdeAction.h"
#include "../event/igdeListBoxListener.h"
#include "../event/igdeTextFieldListener.h"
#include "../layout/igdeContainerFlow.h"
#include "../layout/igdeContainerForm.h"
#include "../model/igdeListItem.h"
#include "../resources/igdeIcon.h"
#include "../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringList.h>


// Actions
////////////

namespace {

class cListSelection : public igdeListBoxListener{
private:
	igdeDialogSetSelect &pDialog;
	
public:
	cListSelection(igdeDialogSetSelect &dialog) : pDialog(dialog){}
	
	void OnDoubleClickItem(igdeListBox *listBox, int index) override{
		if(index >= 0 and listBox->GetSelectedItem()){
			pDialog.SetSelection(decStringSet(devctag, listBox->GetSelectedItem()->GetText()));
			pDialog.Accept();
		}
	}
};

}


// Class igdeDialogSetSelect
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeDialogSetSelect::igdeDialogSetSelect(igdeEnvironment &environment,
	const char *title, const char *message, const decStringSet &set) :
igdeDialog(environment, title)
{
	SetSize(igdeApplication::app().DisplayScaled(decPoint(300, 300)));
	
	auto &helper = environment.GetUIHelper();
	auto content = igdeContainerFlow::Ref::New(environment,
		igdeContainerFlow::eaY, igdeContainerFlow::esFirst, 5);
	
	helper.ListBoxFilter(content, 15, "", pListBox, deTObjectReference<cListSelection>::New(*this));
	pListBox->SetSelectionMode(igdeListBoxFilter::esmMultiple);
	pListBox->SetDefaultSorter();
	set.Visit([&](const decString &name){
		pListBox->AddItem(name);
	});
	pListBox->SortItems();
	pListBox->StoreFilterItems();
	
	igdeContainer::Ref buttonBar;
	CreateButtonBar(buttonBar, "@Igde.Accept", "@Igde.Discard");
	
	AddContent(content, igdeLabel::Ref::New(environment, message), buttonBar);
	
	pListBox->Focus();
}

igdeDialogSetSelect::~igdeDialogSetSelect() = default;


// Management
///////////////

decStringSet igdeDialogSetSelect::GetSelection() const{
	decStringSet set;
	pListBox->GetItems().Visit([&](const igdeListItem &item){
		if(item.GetSelected()){
			set.Add(item.GetText());
		}
	});
	return set;
}

void igdeDialogSetSelect::SetSelection(const decStringSet &selection){
	pListBox->DeselectAllItems();
	
	if(selection.IsNotEmpty()){
		pListBox->SetSelection(pListBox->IndexOfItem(
			decStringList(selection).GetSortedAscending().First()));
		
	}else{
		pListBox->SetSelection(-1);
	}
	
	selection.Visit([&](const decString &name){
		pListBox->SelectItem(pListBox->IndexOfItem(name));
	});
}

void igdeDialogSetSelect::MarkItems(const decStringSet &items,
igdeIcon *iconPresent, igdeIcon *iconAbsent){
	pListBox->GetItems().VisitIndexed([&](int index, igdeListItem &item){
		auto icon = items.Has(item.GetText()) ? iconPresent : iconAbsent;
		if(item.GetIcon() != icon){
			item.SetIcon(icon);
			pListBox->ItemChangedAt(index);
		}
	});
}
