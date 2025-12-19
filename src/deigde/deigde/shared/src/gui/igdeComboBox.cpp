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

#include "igdeComboBox.h"
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


// Class igdeComboBox
///////////////////////

// Constructor, destructor
////////////////////////////

igdeComboBox::igdeComboBox(igdeEnvironment &environment, int columns, const char *description) :
igdeWidget(environment),
pEnabled(true),
pSelection(-1),
pColumns(columns),
pRows(10),
pEditable(false),
pDescription(description),
pInvalidValue(false)
{
	if(columns < 1){
		DETHROW(deeInvalidParam);
	}
}

igdeComboBox::igdeComboBox(igdeEnvironment &environment, int columns, bool editable,
	const char *description) :
igdeWidget(environment),
pEnabled(true),
pSelection(-1),
pColumns(columns),
pRows(10),
pEditable(editable),
pDescription(description),
pInvalidValue(false)
{
	if(columns < 1){
		DETHROW(deeInvalidParam);
	}
}

igdeComboBox::igdeComboBox(igdeEnvironment &environment, int columns, int rows,
	const char *description) :
igdeWidget(environment),
pEnabled(true),
pSelection(-1),
pColumns(columns),
pRows(rows),
pEditable(false),
pDescription(description),
pInvalidValue(false)
{
	if(columns < 1 || rows < 1){
		DETHROW(deeInvalidParam);
	}
}

igdeComboBox::igdeComboBox(igdeEnvironment &environment, int columns, int rows,
	bool editable, const char *description) :
igdeWidget(environment),
pEnabled(true),
pSelection(-1),
pColumns(columns),
pRows(rows),
pEditable(editable),
pDescription(description),
pInvalidValue(false)
{
	if(columns < 1 || rows < 1){
		DETHROW(deeInvalidParam);
	}
}

igdeComboBox::~igdeComboBox(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeComboBox::SetEnabled(bool enabled){
	if(pEnabled == enabled){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeComboBox::SetRows(int rows){
	if(pRows == rows){
		return;
	}
	
	pRows = rows;
	OnRowsChanged();
}

void igdeComboBox::SetEditable(bool editable){
	if(pEditable == editable){
		return;
	}
	
	pEditable = editable;
	OnEditableChanged();
}

void igdeComboBox::SetDescription(const char *description){
	if(pDescription == description){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeComboBox::SetInvalidValue(bool invalidValue){
	if(pInvalidValue == invalidValue){
		return;
	}
	
	pInvalidValue = invalidValue;
	OnInvalidValueChanged();
}

void igdeComboBox::Focus(){
	OnRequestFocus();
}



int igdeComboBox::GetItemCount() const{
	return pItems.GetCount();
}

igdeListItem *igdeComboBox::GetItemAt(int index) const{
	return (igdeListItem*)pItems.GetAt(index);
}

bool igdeComboBox::HasItem(igdeListItem *item) const{
	return pItems.Has(item);
}

bool igdeComboBox::HasItem(const char *item) const{
	return IndexOfItem(item) != -1;
}

bool igdeComboBox::HasItemWithData(void *data) const{
	return IndexOfItemWithData(data) != -1;
}

bool igdeComboBox::HasItemWithRefData(const deObject::Ref &refData) const{
	return IndexOfItemWithRefData(refData) != -1;
}

int igdeComboBox::IndexOfItem(igdeListItem *item) const{
	return pItems.IndexOf(item);
}

int igdeComboBox::IndexOfItem(const char *item) const{
	if(!item){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pItems.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((const igdeListItem*)pItems.GetAt(i))->GetText() == item){
			return i;
		}
	}
	
	return -1;
}

int igdeComboBox::IndexOfItemWithData(void *data) const{
	const int count = pItems.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((const igdeListItem*)pItems.GetAt(i))->GetData() == data){
			return i;
		}
	}
	
	return -1;
}

int igdeComboBox::IndexOfItemWithRefData(const deObject::Ref &refData) const{
	const int count = pItems.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((const igdeListItem*)pItems.GetAt(i))->GetRefData() == refData){
			return i;
		}
	}
	
	return -1;
}

void igdeComboBox::AddItem(igdeListItem *item){
	if(!item){
		DETHROW(deeInvalidParam);
	}
	pItems.Add(item);
	OnItemAdded(pItems.GetCount() - 1);
	
	if(!pEditable && pItems.GetCount() == 1){
		SetSelection(0);
	}
}

igdeListItem::Ref igdeComboBox::AddItem(const char *text, igdeIcon *icon, void *data){
	igdeListItem::Ref item(igdeListItem::Ref::New(text, icon, data));
	AddItem(item);
	return item;
}

igdeListItem::Ref igdeComboBox::AddItemRef(const char *text, igdeIcon *icon, const deObject::Ref &refData){
	igdeListItem::Ref item(igdeListItem::Ref::New(text, icon));
	item->SetRefData(refData);
	AddItem(item);
	return item;
}

void igdeComboBox::InsertItem(int index, igdeListItem *item){
	if(!item){
		DETHROW(deeInvalidParam);
	}
	
	pItems.Insert(item, index);
	if(pSelection >= index){
		pSelection++;
	}
	
	OnItemAdded(index);
	
	if(!pEditable && pItems.GetCount() == 1){
		SetSelection(0);
	}
}

igdeListItem::Ref igdeComboBox::InsertItem(int index, const char *text, igdeIcon *icon, void *data){
	igdeListItem::Ref item(igdeListItem::Ref::New(text, icon, data));
	InsertItem(index, item);
	return item;
}

igdeListItem::Ref igdeComboBox::InsertItemRef(int index, const char *text, igdeIcon *icon, const deObject::Ref &refData){
	igdeListItem::Ref item(igdeListItem::Ref::New(text, icon));
	item->SetRefData(refData);
	InsertItem(index, item);
	return item;
}

void igdeComboBox::MoveItem(int fromIndex, int toIndex){
	pItems.Move(fromIndex, toIndex);
	
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

void igdeComboBox::RemoveItem(int index){
	pItems.RemoveFrom(index);
	
	bool textChanged = false;
	
	if(pSelection == index){
		if(pEditable){
			pSelection = -1;
			
		}else{
			pSelection = decMath::min(pSelection, pItems.GetCount() - 1);
			if(pSelection != -1){
				pText = ((igdeListItem*)pItems.GetAt(pSelection))->GetText();
				
			}else{
				pText.Empty();
			}
			textChanged = true;
		}
		
	}else if(pSelection > index){
		pSelection--;
	}
	
	OnItemRemoved(index);
	
	if(textChanged){
		NotifyTextChanged();
	}
}

void igdeComboBox::RemoveAllItems(){
	if(pItems.GetCount() == 0){
		return;
	}
	
	pItems.RemoveAll();
	pSelection = -1;
	if(!pEditable){
		pText.Empty();
	}
	
	OnAllItemsRemoved();
	
	if(!pEditable){
		NotifyTextChanged();
	}
}

void igdeComboBox::ItemChangedAt(int index){
	if(index < 0 || index >= pItems.GetCount()){
		DETHROW(deeInvalidParam);
	}
	OnItemChanged(index);
}



void igdeComboBox::SetSorter(igdeListItemSorter *sorter){
	pSorter = sorter;
}

void igdeComboBox::SetDefaultSorter(){
	pSorter = igdeListItemSorter::Ref::New();
}

static void igdeComboBox_Sort(decObjectList &items, igdeListItemSorter &sorter, int left, int right){
	igdeListItem::Ref pivot((igdeListItem*)items.GetAt(left));
	const int r_hold = right;
	const int l_hold = left;
	
	while(left < right){
		while(left < right && sorter.Precedes(pivot, *((igdeListItem*)items.GetAt(right)))){
			right--;
		}
		if(left != right){
			items.SetAt(left, items.GetAt(right));
			left++;
		}
		while(left < right && sorter.Precedes(*((igdeListItem*)items.GetAt(left)), pivot)){
			left++;
		}
		if(left != right){
			items.SetAt(right, items.GetAt(left));
			right--;
		}
	}
	
	items.SetAt(left, (igdeListItem*)pivot);
	if(l_hold < left){
		igdeComboBox_Sort(items, sorter, l_hold, left - 1);
	}
	if(r_hold > left){
		igdeComboBox_Sort(items, sorter, left + 1, r_hold);
	}
}

void igdeComboBox::SortItems(){
	if(!pSorter){
		return;
	}
	
	const int count = pItems.GetCount();
	if(count < 2){
		return;
	}
	
	deObject *selection = nullptr;
	if(pSelection != -1){
		selection = pItems.GetAt(pSelection);
	}
	
	igdeComboBox_Sort(pItems, pSorter, 0, count - 1);
	
	if(selection){
		pSelection = pItems.IndexOf(selection);
	}
	
	OnItemsSorted();
}



igdeListItem *igdeComboBox::GetSelectedItem() const{
	return pSelection != -1 ? (igdeListItem*)pItems.GetAt(pSelection) : nullptr;
}

void *igdeComboBox::GetSelectedItemData() const{
	return pSelection != -1 ? ((igdeListItem*)pItems.GetAt(pSelection))->GetData() : nullptr;
}

deObject::Ref igdeComboBox::GetSelectedItemRefData() const{
	return pSelection != -1 ? ((igdeListItem*)pItems.GetAt(pSelection))->GetRefData() : deObject::Ref();
}

void igdeComboBox::SetSelection(int selection){
	if(selection < -1 || selection >= pItems.GetCount()){
		DETHROW(deeInvalidParam);
	}
	
	if(selection == pSelection){
		return;
	}
	
	pSelection = selection;
	
	if(selection != -1){
		pText = ((igdeListItem*)pItems.GetAt(selection))->GetText();
		OnTextChanged();
		NotifyTextChanged();
		
	}else if(!pEditable){
		pText.Empty();
		OnTextChanged();
		NotifyTextChanged();
	}
}

void igdeComboBox::SetSelectionWithData(void *data){
	SetSelection(IndexOfItemWithData(data));
}

void igdeComboBox::SetSelectionWithRefData(const deObject::Ref &refData){
	SetSelection(IndexOfItemWithRefData(refData));
}



void igdeComboBox::SetText(const char *text, bool changing){
	if(pText == text){
		return;
	}
	
	pText = text;
	pSelection = IndexOfItem(text);
	
	OnTextChanged();
	
	if(changing){
		NotifyTextChanging();
		
	}else{
		NotifyTextChanged();
	}
}

void igdeComboBox::ClearText(){
	SetText("", false);
}



void igdeComboBox::AddListener(igdeComboBoxListener *listener){
	if(!listener){
		DETHROW(deeInvalidParam);
	}
	pListeners.Add(listener);
}

void igdeComboBox::RemoveListener(igdeComboBoxListener *listener){
	pListeners.Remove(listener);
}

void igdeComboBox::NotifyTextChanged(){
	const decObjectOrderedSet listeners(pListeners);
	const int count = listeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((igdeComboBoxListener*)listeners.GetAt(i))->OnTextChanged(this);
	}
}

void igdeComboBox::NotifyTextChanging(){
	const decObjectOrderedSet listeners(pListeners);
	const int count = listeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((igdeComboBoxListener*)listeners.GetAt(i))->OnTextChanging(this);
	}
}



void igdeComboBox::CreateNativeWidget(){
	if(GetNativeWidget()){
		return;
	}
	
	igdeNativeComboBox * const native = igdeNativeComboBox::CreateNativeWidget(*this);
	SetNativeWidget(native);
	native->PostCreateNativeWidget();
}

void igdeComboBox::DestroyNativeWidget(){
	if(!GetNativeWidget()){
		return;
	}
	
	((igdeNativeComboBox*)GetNativeWidget())->DestroyNativeWidget();
	DropNativeWidget();
}



void igdeComboBox::OnItemAdded(int index){
	if(!GetNativeWidget()){
		return;
	}
	
	const igdeListItem &item = *((igdeListItem*)pItems.GetAt(index));
	igdeNativeComboBox &native = *((igdeNativeComboBox*)GetNativeWidget());
	native.InsertItem(index, item);
	//native.SyncSelection( false );
	native.UpdateText();
	native.UpdateRowCount();
}

void igdeComboBox::OnItemRemoved(int index){
	if(!GetNativeWidget()){
		return;
	}
	
	igdeNativeComboBox &native = *((igdeNativeComboBox*)GetNativeWidget());
	native.RemoveItem(index);
	//native.SyncSelection( false );
	native.UpdateText();
	native.UpdateRowCount();
}

void igdeComboBox::OnAllItemsRemoved(){
	if(!GetNativeWidget()){
		return;
	}
	
	igdeNativeComboBox &native = *((igdeNativeComboBox*)GetNativeWidget());
	native.RemoveAllItems();
	//native.SyncSelection( false );
	native.UpdateText();
	native.UpdateRowCount();
}

void igdeComboBox::OnItemChanged(int index){
	if(GetNativeWidget()){
		((igdeNativeComboBox*)GetNativeWidget())->UpdateItem(index);
	}
}

void igdeComboBox::OnItemMoved(int fromIndex, int toIndex){
	if(!GetNativeWidget()){
		return;
	}
	
	igdeNativeComboBox &native = *((igdeNativeComboBox*)GetNativeWidget());
	native.MoveItem(fromIndex, toIndex);
	//native.SyncSelection( false );
	native.UpdateText();
}

void igdeComboBox::OnItemsSorted(){
	if(!GetNativeWidget()){
		return;
	}
	
	igdeNativeComboBox &native = *((igdeNativeComboBox*)GetNativeWidget());
	native.BuildList();
	//native.SyncSelection( false );
	native.UpdateText();
}

void igdeComboBox::OnTextChanged(){
	if(GetNativeWidget()){
		((igdeNativeComboBox*)GetNativeWidget())->UpdateText();
	}
}

void igdeComboBox::OnEnabledChanged(){
	if(!GetNativeWidget()){
		return;
	}
	
	((igdeNativeComboBox*)GetNativeWidget())->UpdateEnabled();
}

void igdeComboBox::OnRowsChanged(){
	if(GetNativeWidget()){
		((igdeNativeComboBox*)GetNativeWidget())->UpdateRowCount();
	}
}

void igdeComboBox::OnEditableChanged(){
	if(GetNativeWidget()){
		((igdeNativeComboBox*)GetNativeWidget())->UpdateEditable();
	}
}

void igdeComboBox::OnDescriptionChanged(){
	if(GetNativeWidget()){
		((igdeNativeComboBox*)GetNativeWidget())->UpdateDescription();
	}
}

void igdeComboBox::OnInvalidValueChanged(){
	if(GetNativeWidget()){
		((igdeNativeComboBox*)GetNativeWidget())->OnInvalidValueChanged();
	}
}

void igdeComboBox::OnRequestFocus(){
	if(GetNativeWidget()){
		((igdeNativeComboBox*)GetNativeWidget())->Focus();
	}
}
