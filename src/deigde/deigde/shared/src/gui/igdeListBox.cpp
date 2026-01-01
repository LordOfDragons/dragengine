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

#include "igdeListBox.h"
#include "igdeContainer.h"
#include "igdeCommonDialogs.h"
#include "event/igdeListBoxListener.h"
#include "menu/igdeMenuCascade.h"
#include "model/igdeListItem.h"
#include "model/igdeListItemSorter.h"
#include "native/toolkit.h"
#include "resources/igdeIcon.h"
#include "resources/igdeFont.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeListBox
//////////////////////

// Constructor, destructor
////////////////////////////

igdeListBox::igdeListBox(igdeEnvironment &environment, int rows, const char *description) :
igdeWidget(environment),
pEnabled(true),
pSelectionMode(esmSingle),
pSelection(-1),
pRows(rows),
pDescription(description)
{
	if(rows < 1){
		DETHROW(deeInvalidParam);
	}
}

igdeListBox::~igdeListBox(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeListBox::SetEnabled(bool enabled){
	if(pEnabled == enabled){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeListBox::SetRows(int rows){
	if(rows < 1){
		DETHROW(deeInvalidParam);
	}
	
	if(rows == pRows){
		return;
	}
	
	pRows = rows;
	OnRowsChanged();
}

void igdeListBox::SetDescription(const char *description){
	if(pDescription == description){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeListBox::Focus(){
	if(GetNativeWidget()){
		((igdeNativeListBox*)GetNativeWidget())->Focus();
	}
}



igdeListItem *igdeListBox::GetItemWithData(void *data) const{
	return pItems.FindOrDefault([&](const igdeListItem &item){
		return item.GetData() == data;
	});
}

igdeListItem *igdeListBox::GetItemWithRefData(const deObject::Ref &refData) const{
	return pItems.FindOrDefault([&](const igdeListItem &item){
		return item.GetRefData() == refData;
	});
}

bool igdeListBox::HasItem(const char *item) const{
	return IndexOfItem(item) != -1;
}

bool igdeListBox::HasItemWithData(void *data) const{
	return IndexOfItemWithData(data) != -1;
}

bool igdeListBox::HasItemWithRefData(const deObject::Ref &refData) const{
	return IndexOfItemWithRefData(refData) != -1;
}

int igdeListBox::IndexOfItem(const char *item) const{
	if(!item){
		DETHROW(deeInvalidParam);
	}
	
	return pItems.IndexOfMatching([&](const igdeListItem &each){
		return each.GetText() == item;
	});
}

int igdeListBox::IndexOfItemWithData(void *data) const{
	return pItems.IndexOfMatching([&](const igdeListItem &item){
		return item.GetData() == data;
	});
}

int igdeListBox::IndexOfItemWithRefData(const deObject::Ref &refData) const{
	return pItems.IndexOfMatching([&](const igdeListItem &item){
		return item.GetRefData() == refData;
	});
}

void igdeListBox::AddItem(igdeListItem *item){
	if(!item){
		DETHROW(deeInvalidParam);
	}
	pItems.Add(item);
	OnItemAdded(pItems.GetCount() - 1);
	
	if(pItems.GetCount() == 1){
		SetSelection(0);
	}
}

void igdeListBox::AddItem(const char *text, igdeIcon *icon, void *data){
	igdeListItem::Ref item;
	AddItem(item, text, icon, data);
}

void igdeListBox::AddItemRef(const char *text, igdeIcon *icon, const deObject::Ref &refData){
	igdeListItem::Ref item;
	AddItemRef(item, text, icon, refData);
}

void igdeListBox::AddItem(igdeListItem::Ref &item, const char *text, igdeIcon *icon, void *data){
	item = igdeListItem::Ref::New(text, icon, data);
	AddItem(item);
}

void igdeListBox::AddItemRef(igdeListItem::Ref &item, const char *text, igdeIcon *icon, const deObject::Ref &refData){
	item = igdeListItem::Ref::New(text, icon);
	item->SetRefData(refData);
	AddItem(item);
}

void igdeListBox::InsertItem(int index, igdeListItem *item){
	if(!item){
		DETHROW(deeInvalidParam);
	}
	
	pItems.Insert(item, index);
	if(pSelection >= index){
		pSelection++;
	}
	
	OnItemAdded(index);
	
	if(pItems.GetCount() == 1){
		SetSelection(0);
	}
}

void igdeListBox::InsertItem(int index, const char *text, igdeIcon *icon, void *data){
	igdeListItem::Ref item;
	InsertItem(item, index, text, icon, data);
}

void igdeListBox::InsertItemRef(int index, const char *text, igdeIcon *icon,
const deObject::Ref &refData){
	igdeListItem::Ref item;
	InsertItemRef(item, index, text, icon, refData);
}

void igdeListBox::InsertItem(igdeListItem::Ref &item, int index, const char *text,
igdeIcon *icon, void *data){
	item = igdeListItem::Ref::New(text, icon, data);
	InsertItem(index, item);
}

void igdeListBox::InsertItemRef(igdeListItem::Ref &item, int index, const char *text,
igdeIcon *icon, const deObject::Ref &refData){
	item = igdeListItem::Ref::New(text, icon);
	item->SetRefData(refData);
	InsertItem(index, item);
}

void igdeListBox::MoveItem(int fromIndex, int toIndex){
	pItems.MoveIndex(fromIndex, toIndex);
	
	if(pSelection != -1){
		if(fromIndex < toIndex){
			if(pSelection == fromIndex){
				pSelection = toIndex;
				
			}else if(pSelection > fromIndex && pSelection <= toIndex){
				pSelection--;
			}
			
		}else{
			if(pSelection == fromIndex){
				pSelection = toIndex;
				
			}else if(pSelection >= toIndex && pSelection < fromIndex){
				pSelection++;
			}
		}
	}
	
	OnItemMoved(fromIndex, toIndex);
}

void igdeListBox::RemoveItem(int index){
	pItems.RemoveFrom(index);
	
	bool notifySelectionChanged = false;
	
	if(pSelection == index){
		pSelection = decMath::min(pSelection, pItems.GetCount() - 1);
		notifySelectionChanged = true;
		
	}else if(pSelection > index){
		pSelection--;
	}
	
	OnItemRemoved(index);
	
	if(notifySelectionChanged){
		NotifySelectionChanged();
	}
}

void igdeListBox::RemoveAllItems(){
	if(pItems.GetCount() == 0){
		return;
	}
	
	pItems.RemoveAll();
	pSelection = -1;
	OnAllItemsRemoved();
	
	NotifySelectionChanged();
}

void igdeListBox::ItemChangedAt(int index){
	if(index < 0 || index >= pItems.GetCount()){
		DETHROW(deeInvalidParam);
	}
	OnItemChanged(index);
}



void igdeListBox::SetSorter(igdeListItemSorter *sorter){
	pSorter = sorter;
}

void igdeListBox::SetDefaultSorter(){
	pSorter = igdeListItemSorter::Ref::New();
}

void igdeListBox::SortItems(){
	if(!pSorter){
		return;
	}
	
	const int count = pItems.GetCount();
	if(count < 2){
		return;
	}
	
	igdeListItem * const selected = GetSelectedItem();
	
	pItems.Sort([&](const igdeListItem &a, const igdeListItem &b){
		return pSorter->Precedes(a, b) ? -1 : 1;
	});
	
	if(selected){
		pSelection = pItems.IndexOf(selected);
	}
	
	OnItemsSorted();
}



void igdeListBox::SetSelectionMode(eSelectionMode mode){
	if(mode == pSelectionMode){
		return;
	}
	
	pSelectionMode = mode;
	OnSelectionModeChanged();
}

igdeListItem *igdeListBox::GetSelectedItem() const{
	return pSelection != -1 ? pItems.GetAt(pSelection) : nullptr;
}

void *igdeListBox::GetSelectedItemData() const{
	igdeListItem * const item = GetSelectedItem();
	return item ? item->GetData() : nullptr;
}

deObject::Ref igdeListBox::GetSelectedItemRefData() const{
	igdeListItem * const item = GetSelectedItem();
	return item ? item->GetRefData() : deObject::Ref();
}

void igdeListBox::SetSelection(int selection){
	if(selection < -1 || selection >= pItems.GetCount()){
		DETHROW(deeInvalidParam);
	}
	
	if(selection == pSelection){
		return;
	}
	
	pSelection = selection;
	OnSelectionChanged();
	NotifySelectionChanged();
}

void igdeListBox::SetSelectionWithData(void *data){
	SetSelection(IndexOfItemWithData(data));
}

void igdeListBox::SetSelectionWithRefData(const deObject::Ref &refData){
	SetSelection(IndexOfItemWithRefData(refData));
}

void igdeListBox::SelectItem(int index){
	if(pSelectionMode != esmMultiple){
		DETHROW(deeInvalidParam);
	}
	if(index < -1 || index >= pItems.GetCount()){
		DETHROW(deeInvalidParam);
	}
	
	igdeListItem * const item = pItems.GetAt(index);
	if(item->GetSelected()){
		return;
	}
	
	item->SetSelected(true);
	OnSelectionChanged();
}

void igdeListBox::SelectAllItems(){
	if(pSelectionMode != esmMultiple){
		DETHROW(deeInvalidParam);
	}
	
	bool changed = false;
	
	pItems.Visit([&](igdeListItem &item){
		if(item.GetSelected()){
			return;
		}
		
		item.SetSelected(true);
		changed = true;
	});
	
	if(changed){
		OnSelectionChanged();
	}
}

void igdeListBox::DeselectItem(int index){
	if(pSelectionMode != esmMultiple){
		DETHROW(deeInvalidParam);
	}
	if(index < -1 || index >= pItems.GetCount()){
		DETHROW(deeInvalidParam);
	}
	
	igdeListItem * const item = pItems.GetAt(index);
	if(!item->GetSelected()){
		return;
	}
	
	item->SetSelected(false);
	OnSelectionChanged();
}

void igdeListBox::DeselectAllItems(){
	if(pSelectionMode != esmMultiple){
		DETHROW(deeInvalidParam);
	}
	
	bool changed = false;
	pItems.Visit([&](igdeListItem &item){
		if(!item.GetSelected()){
			return;
		}
		
		item.SetSelected(false);
		changed = true;
	});
	
	if(changed){
		OnSelectionChanged();
	}
}

void igdeListBox::MakeItemVisible(int index){
	if(GetNativeWidget()){
		((igdeNativeListBox*)GetNativeWidget())->MakeItemVisible(index);
	}
}

void igdeListBox::MakeSelectionVisible(){
	if(pSelection != -1){
		MakeItemVisible(pSelection);
	}
}

void igdeListBox::ShowContextMenu(const decPoint &position){
	if(!GetNativeWidget()){
		return;
	}
	
	igdeMenuCascade::Ref menu(igdeMenuCascade::Ref::New(GetEnvironment()));
	
	const int count = pListeners.GetCount();
	int i;
	for(i=0; i<count; i++){
		((igdeListBoxListener*)pListeners.GetAt(i))->AddContextMenuEntries(this, menu);
	}
	
	if(menu->GetChildren().IsNotEmpty()){
		menu->Popup(*this, position);
	}
}



void igdeListBox::AddListener(igdeListBoxListener *listener){
	if(!listener){
		DETHROW(deeInvalidParam);
	}
	pListeners.Add(listener);
}

void igdeListBox::RemoveListener(igdeListBoxListener *listener){
	pListeners.Remove(listener);
}
void igdeListBox::NotifySelectionChanged(){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeListBoxListener &l){
		l.OnSelectionChanged(this);
	});
}

void igdeListBox::NotifyItemSelected(int index){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeListBoxListener &l){
		l.OnItemSelected(this, index);
	});
}

void igdeListBox::NotifyItemDeselected(int index){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeListBoxListener &l){
		l.OnItemDeselected(this, index);
	});
}

void igdeListBox::NotifyDoubleClickItem(int index){
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pItems.GetCount())
	
	const auto listeners(pListeners);
	listeners.Visit([&](igdeListBoxListener &l){
		l.OnDoubleClickItem(this, index);
	});
}



void igdeListBox::CreateNativeWidget(){
	if(GetNativeWidget()){
		return;
	}
	
	igdeNativeListBox * const native = igdeNativeListBox::CreateNativeWidget(*this);
	SetNativeWidget(native);
	native->PostCreateNativeWidget();
}

void igdeListBox::DestroyNativeWidget(){
	if(!GetNativeWidget()){
		return;
	}
	
	((igdeNativeListBox*)GetNativeWidget())->DestroyNativeWidget();
	DropNativeWidget();
}



void igdeListBox::OnItemAdded(int index){
	if(GetNativeWidget()){
		((igdeNativeListBox*)GetNativeWidget())->InsertItem(index);
	}
}

void igdeListBox::OnItemRemoved(int index){
	if(GetNativeWidget()){
		((igdeNativeListBox*)GetNativeWidget())->RemoveItem(index);
	}
}

void igdeListBox::OnAllItemsRemoved(){
	if(GetNativeWidget()){
		((igdeNativeListBox*)GetNativeWidget())->RemoveAllItems();
	}
}

void igdeListBox::OnItemChanged(int index){
	if(GetNativeWidget()){
		((igdeNativeListBox*)GetNativeWidget())->UpdateItem(index);
	}
}

void igdeListBox::OnItemMoved(int fromIndex, int toIndex){
	if(GetNativeWidget()){
		((igdeNativeListBox*)GetNativeWidget())->MoveItem(fromIndex, toIndex);
	}
}

void igdeListBox::OnItemsSorted(){
	if(GetNativeWidget()){
		((igdeNativeListBox*)GetNativeWidget())->BuildList();
	}
}

void igdeListBox::OnSelectionChanged(){
	if(GetNativeWidget()){
		((igdeNativeListBox*)GetNativeWidget())->UpdateSelection();
	}
}

void igdeListBox::OnSelectionModeChanged(){
	if(GetNativeWidget()){
		((igdeNativeListBox*)GetNativeWidget())->UpdateStyles();
	}
}

void igdeListBox::OnEnabledChanged(){
	if(GetNativeWidget()){
		((igdeNativeListBox*)GetNativeWidget())->UpdateEnabled();
	}
}

void igdeListBox::OnRowsChanged(){
	if(GetNativeWidget()){
		((igdeNativeListBox*)GetNativeWidget())->UpdateRowCount();
	}
}

void igdeListBox::OnDescriptionChanged(){
	if(GetNativeWidget()){
		((igdeNativeListBox*)GetNativeWidget())->UpdateDescription();
	}
}
