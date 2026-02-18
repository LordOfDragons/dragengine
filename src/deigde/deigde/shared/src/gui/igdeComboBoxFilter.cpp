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

#include "igdeComboBoxFilter.h"
#include "igdeContainer.h"
#include "native/toolkit.h"
#include "igdeCommonDialogs.h"
#include "event/igdeComboBoxListener.h"
#include "model/igdeListItem.h"
#include "model/igdeListItemSorter.h"
#include "resources/igdeIcon.h"
#include "resources/igdeFont.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>


// Class igdeComboBoxFilter
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeComboBoxFilter::igdeComboBoxFilter(igdeEnvironment &environment, int columns,
	const char *description) :
igdeComboBox(environment, columns, description),
pFilterCaseInsensitive(true),
pNativeComboBoxFilter(nullptr){
}

igdeComboBoxFilter::igdeComboBoxFilter(igdeEnvironment &environment, int columns, bool editable,
	const char *description) :
igdeComboBox(environment, columns, editable, description),
pFilterCaseInsensitive(true),
pNativeComboBoxFilter(nullptr){
}

igdeComboBoxFilter::igdeComboBoxFilter(igdeEnvironment &environment, int columns, int rows,
	const char *description) :
igdeComboBox(environment, columns, rows, description),
pFilterCaseInsensitive(true),
pNativeComboBoxFilter(nullptr){
}

igdeComboBoxFilter::igdeComboBoxFilter(igdeEnvironment &environment, int columns, int rows,
	bool editable, const char *description) :
igdeComboBox(environment, columns, rows, editable, description),
pFilterCaseInsensitive(true),
pNativeComboBoxFilter(nullptr){
}

igdeComboBoxFilter::~igdeComboBoxFilter(){
}



// Management
///////////////

void igdeComboBoxFilter::StoreFilterItems(){
	pFilterItems = GetItems();
	FilterItems();
}

void igdeComboBoxFilter::ReleaseFilterItems(){
	pFilterItems.RemoveAll();
}

void igdeComboBoxFilter::FilterItems(){
	const bool noFilter = pFilterString.IsEmpty();
	const decString text(GetText());
	
	RemoveAllItems();
	
	if(pFilterCaseInsensitive){
		const decString filter(pFilterString.GetLower());
		
		pFilterItems.Visit([&](igdeListItem *item){
			if(noFilter || item->GetText().GetLower().FindString(filter) != -1){
				AddItem(item);
			}
		});
		
	}else{
		pFilterItems.Visit([&](igdeListItem *item){
			if(noFilter || item->GetText().FindString(pFilterString) != -1){
				AddItem(item);
			}
		});
	}
	
	SetText(text);
}

void igdeComboBoxFilter::SetFilterCaseInsentive(bool caseInsensitive){
	if(caseInsensitive == pFilterCaseInsensitive){
		return;
	}
	
	pFilterCaseInsensitive = caseInsensitive;
	FilterItems();
}

void igdeComboBoxFilter::SetFilterString(const char *filterString){
	if(pFilterString == filterString){
		return;
	}
	
	pFilterString = filterString;
	OnFilterStringChanged();
	
	FilterItems();
	
	// if the current selection is filtered out use the first one if available
	if(GetSelection() == -1 && GetItems().IsNotEmpty() && !GetEditable()){
		SetSelection(0);
	}
}


void igdeComboBoxFilter::CreateNativeWidget(){
	if(GetNativeWidget()){
		return;
	}
	
	igdeNativeComboBoxFilter * const native = igdeNativeComboBoxFilter::CreateNativeWidget(*this);
	SetNativeWidget(native);
	pNativeComboBoxFilter = native;
	pNativeComboBox = native;
	native->PostCreateNativeWidget();
}

void igdeComboBoxFilter::DestroyNativeWidget(){
	if(!GetNativeWidget()){
		return;
	}
	
	((igdeNativeComboBoxFilter*)GetNativeWidget())->DestroyNativeWidget();
	DropNativeWidget();
}

void igdeComboBoxFilter::DropNativeWidget(){
	pNativeComboBoxFilter = nullptr;
	igdeComboBox::DropNativeWidget();
}


void igdeComboBoxFilter::OnFilterStringChanged(){
	if(pNativeComboBoxFilter){
		pNativeComboBoxFilter->UpdateFilterString();
	}
}
