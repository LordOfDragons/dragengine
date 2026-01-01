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

#include "ceDialogEditStringWithList.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/layout/igdeContainerForm.h>

#include <dragengine/common/exceptions.h>



// Class ceDialogEditStringWithList
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

ceDialogEditStringWithList::ceDialogEditStringWithList(igdeEnvironment &environment,
const char *windowTitle, const char *textLabel, const char *value, const decStringList &choices) :
igdeDialog(environment, windowTitle)
{
	igdeUIHelper &helper = environment.GetUIHelper();
	
	igdeContainerForm::Ref content(igdeContainerForm::Ref::New(environment));
	helper.ComboBoxFilter(content, textLabel, true, "", pCBString, {});
	pCBString->SetDefaultSorter();
	
	igdeContainer::Ref buttonBar;
	CreateButtonBar(buttonBar, "Accept", "Cancel");
	
	AddContent(content, buttonBar);
	
	SetSize(igdeApplication::app().DisplayScaled(decPoint(400, 150)));
	
	// init list
	const int count = choices.GetCount();
	int i;
	for(i=0; i<count; i++){
		pCBString->AddItem(choices.GetAt(i));
	}
	pCBString->SortItems();
	pCBString->StoreFilterItems();
	
	pCBString->SetText(value);
}

ceDialogEditStringWithList::~ceDialogEditStringWithList(){
}



// Management
///////////////

const decString &ceDialogEditStringWithList::GetString() const{
	return pCBString->GetText();
}



void ceDialogEditStringWithList::OnDialogShown(){
	pCBString->Focus();
}
