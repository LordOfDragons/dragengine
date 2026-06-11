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

#include "igdeListBoxFilter.h"
#include "igdeContainer.h"
#include "native/toolkit.h"
#include "igdeCommonDialogs.h"
#include "event/igdeListBoxListener.h"
#include "model/igdeListItem.h"
#include "model/igdeListItemSorter.h"
#include "resources/igdeIcon.h"
#include "resources/igdeFont.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>


// Class igdeListBoxFilter
////////////////////////////

// Constructor, destructor
///////////////////////////

igdeListBoxFilter::igdeListBoxFilter(igdeEnvironment &environment, int rows, const char *description) :
igdeListBox(environment, rows, description),
pFilterCaseInsensitive(true),
pNativeListBoxFilter(nullptr){
}

igdeListBoxFilter::~igdeListBoxFilter(){
}


// Management
///////////////

void igdeListBoxFilter::StoreFilterItems(){
	pFilterItems = GetItems();
	FilterItems();
}

void igdeListBoxFilter::ReleaseFilterItems(){
	pFilterItems.RemoveAll();
}

void igdeListBoxFilter::FilterItems(){
	const bool noFilter = pFilterString.IsEmpty();
	
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
}

void igdeListBoxFilter::SetFilterCaseInsensitive(bool caseInsensitive){
	if(caseInsensitive == pFilterCaseInsensitive){
		return;
	}
	
	pFilterCaseInsensitive = caseInsensitive;
	FilterItems();
}

void igdeListBoxFilter::SetFilterString(const char *filterString){
	if(pFilterString == filterString){
		return;
	}
	
	pFilterString = filterString;
	OnFilterStringChanged();
	
	FilterItems();
	
	// if the current selection is filtered out use the first one if available
	if(GetSelection() == -1 && GetItems().IsNotEmpty()){
		SetSelection(0);
	}
}

void igdeListBoxFilter::CreateNativeWidget(){
	if(GetNativeWidget()){
		return;
	}
	
	igdeNativeListBoxFilter * const native = igdeNativeListBoxFilter::CreateNativeWidget(*this);
	SetNativeWidget(native);
	pNativeListBoxFilter = native;
	pNativeListBox = native;
	native->PostCreateNativeWidget();
}

void igdeListBoxFilter::DestroyNativeWidget(){
	if(!GetNativeWidget()){
		return;
	}
	
	((igdeNativeListBoxFilter*)GetNativeWidget())->DestroyNativeWidget();
	DropNativeWidget();
}

void igdeListBoxFilter::DropNativeWidget(){
	pNativeListBoxFilter = nullptr;
	igdeListBox::DropNativeWidget();
}

void igdeListBoxFilter::OnFilterStringChanged(){
	if(pNativeListBoxFilter){
		pNativeListBoxFilter->UpdateFilterString();
	}
}
