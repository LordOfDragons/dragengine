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

#include "igdeTreeList.h"
#include "igdeContainer.h"
#include "igdeCommonDialogs.h"
#include "event/igdeTreeListListener.h"
#include "menu/igdeMenuCascade.h"
#include "model/igdeTreeItem.h"
#include "model/igdeTreeItemSorter.h"
#include "native/toolkit.h"
#include "resources/igdeIcon.h"
#include "resources/igdeFont.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/logger/deLogger.h>



// Class igdeTreeList
///////////////////////

// Constructor, destructor
////////////////////////////

igdeTreeList::igdeTreeList(igdeEnvironment &environment, int rows, const char *description) :
igdeWidget(environment),
pEnabled(true),
pRows(rows),
pDescription(description)
{
	if(rows < 1){
		DETHROW(deeInvalidParam);
	}
}

igdeTreeList::~igdeTreeList(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeTreeList::SetEnabled(bool enabled){
	if(pEnabled == enabled){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeTreeList::SetRows(int rows){
	if(rows < 1){
		DETHROW(deeInvalidParam);
	}
	
	if(rows == pRows){
		return;
	}
	
	pRows = rows;
	OnRowsChanged();
}

void igdeTreeList::SetDescription(const char *description){
	if(pDescription == description){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeTreeList::Focus(){
	if(GetNativeWidget()){
		((igdeNativeTreeList*)GetNativeWidget())->Focus();
	}
}



igdeTreeItem *igdeTreeList::GetLastChild() const{
	igdeTreeItem *item = pFirstChild;
	igdeTreeItem *lastChild = nullptr;
	while(item){
		lastChild = item;
		item = item->GetNext();
	}
	return lastChild;
}

int igdeTreeList::GetChildrenCount() const{
	int count = 0;
	igdeTreeItem *item = pFirstChild;
	while(item){
		count++;
		item = item->GetNext();
	}
	return count;
}

igdeTreeItem *igdeTreeList::GetItemWithData(void *data) const{
	return pGetItemWithData(nullptr, data);
}

igdeTreeItem *igdeTreeList::GetItemWithData(igdeTreeItem *parent, void *data) const{
	return pGetItemWithData(parent, data);
}

bool igdeTreeList::HasItem(igdeTreeItem *item) const{
	return pHasItem(nullptr, item);
}

bool igdeTreeList::HasItem(const char *text) const{
	return pHasItem(nullptr, text);
}

bool igdeTreeList::HasItemWithData(void *data) const{
	return pHasItem(nullptr, data);
}

void igdeTreeList::AppendItem(igdeTreeItem *parent, igdeTreeItem *item){
	if(!item || item->GetParent() || item->GetPrevious() || item->GetNext()){
		DETHROW(deeInvalidParam);
	}
	
	pAppendItem(parent, item);
	OnItemAdded(item);
	
	if(!pSelection){
		SetSelection(item);
	}
}

igdeTreeItem *igdeTreeList::AppendItem(igdeTreeItem *parent, const char *text, igdeIcon *icon, void *data){
	igdeTreeItem::Ref item;
	AppendItem(parent, item, text, icon, data);
	return item;
}

void igdeTreeList::AppendItem(igdeTreeItem *parent, igdeTreeItem::Ref &item,
const char *text, igdeIcon *icon, void *data){
	item = igdeTreeItem::Ref::New(text, icon, data);
	AppendItem(parent, item);
}

void igdeTreeList::InsertItemBefore(igdeTreeItem *beforeItem, igdeTreeItem *item){
	if(!item || item->GetParent() || item->GetPrevious() || item->GetNext() || !beforeItem){
		DETHROW(deeInvalidParam);
	}
	
	pInsertItemBefore(item, beforeItem);
	OnItemAdded(item);
	
	if(!pSelection){
		SetSelection(item);
	}
}

igdeTreeItem *igdeTreeList::InsertItemBefore(igdeTreeItem *beforeItem, const char *text,
igdeIcon *icon, void *data){
	igdeTreeItem::Ref item;
	InsertItemBefore(beforeItem, item, text, icon, data);
	return item;
}

void igdeTreeList::InsertItemBefore(igdeTreeItem *beforeItem, igdeTreeItem::Ref &item,
const char *text, igdeIcon *icon, void *data){
	item = igdeTreeItem::Ref::New(text, icon, data);
	InsertItemBefore(beforeItem, item);
}

void igdeTreeList::InsertItemAfter(igdeTreeItem *afterItem, igdeTreeItem *item){
	if(!item || item->GetParent() || item->GetPrevious() || item->GetNext() || !afterItem){
		DETHROW(deeInvalidParam);
	}
	
	pInsertItemAfter(item, afterItem);
	OnItemAdded(item);
	
	if(!pSelection){
		SetSelection(item);
	}
}

igdeTreeItem *igdeTreeList::InsertItemAfter(igdeTreeItem *afterItem, const char *text,
igdeIcon *icon, void *data){
	igdeTreeItem::Ref item;
	InsertItemAfter(afterItem, item, text, icon, data);
	return item;
}

void igdeTreeList::InsertItemAfter(igdeTreeItem *afterItem, igdeTreeItem::Ref &item,
const char *text, igdeIcon *icon, void *data){
	item = igdeTreeItem::Ref::New(text, icon, data);
	InsertItemAfter(afterItem, item);
}

void igdeTreeList::MoveItemBefore(igdeTreeItem *item, igdeTreeItem *beforeItem){
	if(!item || !beforeItem){
		DETHROW(deeInvalidParam);
	}
	
	const igdeTreeItem::Ref guard(item);
	pRemoveItem(item);
	pInsertItemBefore(item, beforeItem);
	OnItemMoved(item);
}

void igdeTreeList::MoveItemAfter(igdeTreeItem *item, igdeTreeItem *afterItem){
	if(!item || !afterItem){
		DETHROW(deeInvalidParam);
	}
	
	const igdeTreeItem::Ref guard(item);
	pRemoveItem(item);
	pInsertItemAfter(item, afterItem);
	
	OnItemMoved(item);
}

void igdeTreeList::RemoveItem(igdeTreeItem *item){
	if(!item){
		DETHROW(deeInvalidParam);
	}
	
	const igdeTreeItem::Ref guard(item);
	
	bool selectionChanged = false;
	if(pSelection){
		igdeTreeItem *check = pSelection;
		while(check){
			if(check == item){
				// there are two possible solutions for handling this. the first is to clear
				// the selection. the second is to move the selection to the next child if
				// there are any children or the parent which can be nullptr. most of the time
				// the second behavior is the more desired behavior
				//pSelection = nullptr;
				if(item->GetNext()){
					pSelection = item->GetNext();
					
				}else if(item->GetPrevious()){
					pSelection = item->GetPrevious();
					
				}else{
					pSelection = item->GetParent();
				}
				selectionChanged = true;
				break;
			}
			check = check->GetParent();
		}
	}
	
	pRemoveItem(item);
	item->SetParent(nullptr);
	OnItemRemoved(item);
	
	if(selectionChanged){
		NotifySelectionChanged();
	}
}

void igdeTreeList::RemoveAllItems(){
	if(!pFirstChild){
		return;
	}
	
	const bool selectionChanged = pSelection;
	pSelection = nullptr;
	
	pRemoveAllItems(nullptr);
	
	OnAllItemsRemoved(nullptr);
	
	if(selectionChanged){
		NotifySelectionChanged();
	}
}

void igdeTreeList::RemoveAllItems(igdeTreeItem *parent){
	if(!parent){
		DETHROW(deeInvalidParam);
	}
	
	if(parent->GetChildrenCount() == 0){
		return;
	}
	
	bool selectionChanged = false;
	if(pSelection){
		igdeTreeItem *check = pSelection;
		while(check){
			if(check->GetParent() == parent){
				pSelection = nullptr;
				selectionChanged = true;
				break;
			}
			check = check->GetParent();
		}
	}
	
	pRemoveAllItems(parent);
	
	OnAllItemsRemoved(parent);
	
	if(selectionChanged){
		NotifySelectionChanged();
	}
}

void igdeTreeList::ItemChanged(igdeTreeItem *item){
	if(!item){
		DETHROW(deeInvalidParam);
	}
	
	OnItemChanged(item);
}



void igdeTreeList::SetSorter(igdeTreeItemSorter *sorter){
	pSorter = sorter;
}

void igdeTreeList::SetDefaultSorter(){
	pSorter = igdeTreeItemSorter::Ref::New();
}

static void igdeTreeList_Sort(decObjectList &items, igdeTreeItemSorter &sorter, int left, int right){
	igdeTreeItem::Ref pivot((igdeTreeItem*)items.GetAt(left));
	const int r_hold = right;
	const int l_hold = left;
	
	while(left < right){
		while(left < right && sorter.Precedes(pivot, *((igdeTreeItem*)items.GetAt(right)))){
			right--;
		}
		if(left != right){
			items.SetAt(left, items.GetAt(right));
			left++;
		}
		while(left < right && sorter.Precedes(*((igdeTreeItem*)items.GetAt(left)), pivot)){
			left++;
		}
		if(left != right){
			items.SetAt(right, items.GetAt(left));
			right--;
		}
	}
	
	items.SetAt(left, (igdeTreeItem*)pivot);
	if(l_hold < left){
		igdeTreeList_Sort(items, sorter, l_hold, left - 1);
	}
	if(r_hold > left){
		igdeTreeList_Sort(items, sorter, left + 1, r_hold);
	}
}

void igdeTreeList::SortItems(igdeTreeItem *item){
	if(!pSorter){
		return;
	}
	
	const int count = item ? item->GetChildrenCount() : GetChildrenCount();
	if(count < 2){
		return;
	}
	
	igdeTreeItem *child = item ? item->GetFirstChild() : (igdeTreeItem*)pFirstChild;
	decObjectList items;
	while(child){
		items.Add(child);
		child = child->GetNext();
	}
	
	igdeTreeList_Sort(items, pSorter, 0, count - 1);
	
	igdeTreeItem *prevItem = (igdeTreeItem*)items.GetAt(0);
	prevItem->SetPrevious(nullptr);
	
	if(item){
		item->SetFirstChild(prevItem);
		
	}else{
		pFirstChild = prevItem;
	}
	
	int i;
	for(i=1; i<count; i++){
		igdeTreeItem * const nextItem = (igdeTreeItem*)items.GetAt(i);
		prevItem->SetNext(nextItem);
		nextItem->SetPrevious(prevItem);
		prevItem = nextItem;
	}
	
	prevItem->SetNext(nullptr);
	
	OnItemsSorted(item);
}

void igdeTreeList::SortAllItems(){
	if(!pSorter){
		return;
	}
	
	SortItems(nullptr);
	
	igdeTreeItem *child = pFirstChild;
	while(child){
		SortItems(child);
		child = child->GetNext();
	}
}



void igdeTreeList::SetSelection(igdeTreeItem *selection){
	if(selection == pSelection){
		return;
	}
	
	pSelection = selection;
	
	OnSelectionChanged();
	NotifySelectionChanged();
}

void igdeTreeList::SetSelectionWithData(void *data){
	SetSelection(pGetItemWithData(nullptr, data));
}

void igdeTreeList::MakeItemVisible(igdeTreeItem *item){
	if(GetNativeWidget()){
		((igdeNativeTreeList*)GetNativeWidget())->MakeItemVisible(item);
	}
}

void igdeTreeList::MakeSelectionVisible(){
	if(pSelection){
		MakeItemVisible(pSelection);
	}
}

void igdeTreeList::ShowContextMenu(const decPoint &position){
	if(!GetNativeWidget()){
		return;
	}
	
	igdeMenuCascade::Ref menu(igdeMenuCascade::Ref::New(GetEnvironment()));
	
	const int count = pListeners.GetCount();
	int i;
	for(i=0; i<count; i++){
		((igdeTreeListListener*)pListeners.GetAt(i))->AddContextMenuEntries(this, menu);
	}
	
	if(menu->GetChildren().IsNotEmpty()){
		menu->Popup(*this, position);
	}
}



void igdeTreeList::AddListener(igdeTreeListListener *listener){
	if(!listener){
		DETHROW(deeInvalidParam);
	}
	pListeners.Add(listener);
}

void igdeTreeList::RemoveListener(igdeTreeListListener *listener){
	pListeners.Remove(listener);
}
void igdeTreeList::NotifySelectionChanged(){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeTreeListListener &l){
		l.OnSelectionChanged(this);
	});
}

void igdeTreeList::NotifyItemExpanded(igdeTreeItem *item){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeTreeListListener &l){
		l.OnItemExpanded(this, item);
	});
}

void igdeTreeList::NotifyItemCollapsed(igdeTreeItem *item){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeTreeListListener &l){
		l.OnItemCollapsed(this, item);
	});
}

void igdeTreeList::NotifyDoubleClickItem(igdeTreeItem *item){
	const auto listeners(pListeners);
	listeners.Visit([&](igdeTreeListListener &l){
		l.OnDoubleClickItem(this, item);
	});
}



void igdeTreeList::CreateNativeWidget(){
	if(GetNativeWidget()){
		return;
	}
	
	igdeNativeTreeList * const native = igdeNativeTreeList::CreateNativeWidget(*this);
	SetNativeWidget(native);
	native->PostCreateNativeWidget();
}

void igdeTreeList::DestroyNativeWidget(){
	if(!GetNativeWidget()){
		return;
	}
	
	((igdeNativeTreeList*)GetNativeWidget())->DestroyNativeWidget();
	DropNativeWidget();
}



void igdeTreeList::OnItemAdded(igdeTreeItem *item){
	if(GetNativeWidget()){
		((igdeNativeTreeList*)GetNativeWidget())->CreateAndInsertItem(item);
	}
}

void igdeTreeList::OnItemRemoved(igdeTreeItem *item){
	if(GetNativeWidget()){
		((igdeNativeTreeList*)GetNativeWidget())->RemoveItem(item);
	}
}

void igdeTreeList::OnAllItemsRemoved(igdeTreeItem *parent){
	if(GetNativeWidget()){
		((igdeNativeTreeList*)GetNativeWidget())->RemoveAllItems(parent);
	}
}

void igdeTreeList::OnItemChanged(igdeTreeItem *item){
	if(GetNativeWidget()){
		((igdeNativeTreeList*)GetNativeWidget())->UpdateItem(item);
	}
}

void igdeTreeList::OnItemMoved(igdeTreeItem *item){
	if(GetNativeWidget()){
		((igdeNativeTreeList*)GetNativeWidget())->ItemMoved(item);
	}
}

void igdeTreeList::OnItemsSorted(igdeTreeItem *item){
	if(GetNativeWidget()){
		((igdeNativeTreeList*)GetNativeWidget())->ItemsSortedIn(item);
	}
}

void igdeTreeList::OnSelectionChanged(){
	if(GetNativeWidget()){
		((igdeNativeTreeList*)GetNativeWidget())->SelectItem(pSelection);
	}
}

void igdeTreeList::OnEnabledChanged(){
	if(GetNativeWidget()){
		((igdeNativeTreeList*)GetNativeWidget())->UpdateEnabled();
	}
}

void igdeTreeList::OnRowsChanged(){
	if(GetNativeWidget()){
		((igdeNativeTreeList*)GetNativeWidget())->UpdateRows();
	}
}

void igdeTreeList::OnDescriptionChanged(){
	if(GetNativeWidget()){
		((igdeNativeTreeList*)GetNativeWidget())->UpdateDescription();
	}
}

igdeTreeItem *igdeTreeList::pGetItemWithData(igdeTreeItem *parent, void *data) const{
	igdeTreeItem *child = parent ? parent->GetFirstChild() : (igdeTreeItem*)pFirstChild;
	
	while(child){
		if(child->GetData() == data){
			return child;
		}
		igdeTreeItem * const found = pGetItemWithData(child, data);
		if(found){
			return found;
		}
		child = child->GetNext();
	}
	
	return nullptr;
}

bool igdeTreeList::pHasItem(igdeTreeItem *parent, igdeTreeItem *item) const{
	igdeTreeItem *child = parent ? parent->GetFirstChild() : (igdeTreeItem*)pFirstChild;
	
	while(child){
		if(child == item || pHasItem(child, item)){
			return true;
		}
		child = child->GetNext();
	}
	
	return false;
}

bool igdeTreeList::pHasItem(igdeTreeItem *parent, const char *text) const{
	igdeTreeItem *child = parent ? parent->GetFirstChild() : (igdeTreeItem*)pFirstChild;
	
	while(child){
		if(child->GetText() == text || pHasItem(child, text)){
			return true;
		}
		child = child->GetNext();
	}
	
	return false;
}

bool igdeTreeList::pHasItem(igdeTreeItem *parent, void *data) const{
	igdeTreeItem *child = parent ? parent->GetFirstChild() : (igdeTreeItem*)pFirstChild;
	
	while(child){
		if(child->GetData() == data || pHasItem(child, data)){
			return true;
		}
		child = child->GetNext();
	}
	
	return false;
}

void igdeTreeList::pRemoveItem(igdeTreeItem *item){
	const igdeTreeItem::Ref guard(item);
	
	if(item->GetPrevious()){
		item->GetPrevious()->SetNext(item->GetNext());
		
	}else if(item->GetParent()){
		item->GetParent()->SetFirstChild(item->GetNext());
		
	}else{
		pFirstChild = nullptr;
	}
	
	if(item->GetNext()){
		item->GetNext()->SetPrevious(item->GetPrevious());
	}
	
	item->SetPrevious(nullptr);
	item->SetNext(nullptr);
}

void igdeTreeList::pAppendItem(igdeTreeItem *parent, igdeTreeItem *item){
	item->SetParent(parent);
	
	igdeTreeItem * const lastChild = parent ? parent->GetLastChild() : GetLastChild();
	if(lastChild){
		lastChild->SetNext(item);
		item->SetPrevious(lastChild);
		
	}else if(parent){
		parent->SetFirstChild(item);
		
	}else{
		pFirstChild = item;
	}
}

void igdeTreeList::pInsertItemBefore(igdeTreeItem *item, igdeTreeItem *beforeItem){
	item->SetParent(beforeItem->GetParent());
	
	if(beforeItem->GetPrevious()){
		beforeItem->GetPrevious()->SetNext(item);
		item->SetPrevious(beforeItem->GetPrevious());
		
	}else if(beforeItem->GetParent()){
		beforeItem->GetParent()->SetFirstChild(item);
		
	}else{
		pFirstChild = item;
	}
	
	item->SetNext(beforeItem);
	beforeItem->SetPrevious(item);
}

void igdeTreeList::pInsertItemAfter(igdeTreeItem *item, igdeTreeItem *afterItem){
	item->SetParent(afterItem->GetParent());
	
	if(afterItem->GetNext()){
		afterItem->GetNext()->SetPrevious(item);
		item->SetNext(afterItem->GetNext());
	}
	
	item->SetPrevious(afterItem);
	afterItem->SetNext(item);
}

void igdeTreeList::pRemoveAllItems(igdeTreeItem *item){
	igdeTreeItem *child = item ? item->GetLastChild() : GetLastChild();
	
	while(child){
		igdeTreeItem &clearChild = *child;
		child = child->GetPrevious();
		
		pRemoveAllItems(&clearChild);
		clearChild.SetNext(nullptr);
		clearChild.SetPrevious(nullptr);
		clearChild.SetParent(nullptr);
	}
	
	if(item){
		item->SetFirstChild(nullptr);
		
	}else{
		pFirstChild = nullptr;
	}
}
