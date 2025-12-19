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

#include "ceWPTTreeItem.h"
#include "ceWPTTreeItemModel.h"

#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/model/igdeTreeItem.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTreeItem::ceWPTTreeItem(igdeTreeList *treeList) :
igdeTreeItem(""),
pTreeList(treeList)
{
	if(!treeList){
		DETHROW(deeInvalidParam);
	}
}

ceWPTTreeItem::~ceWPTTreeItem(){
	SetModel(nullptr);
}



// Management
///////////////

void ceWPTTreeItem::SetModel(ceWPTTreeItemModel *model){
	if(pModel == model){
		return;
	}
	
	if(pModel){
		pModel->SetTreeItem(nullptr);
	}
	
	pModel = model;
	
	if(model){
		model->SetTreeItem(this);
	}
}



void ceWPTTreeItem::AddItem(ceWPTTreeItemModel *model){
	if(!pTreeList || !model){
		DETHROW(deeInvalidParam);
	}
	
	ceWPTTreeItem::Ref item(ceWPTTreeItem::Ref::New(pTreeList));
	pTreeList->AppendItem(this, item);
	item->SetModel(model);
}

void ceWPTTreeItem::InsertItem(ceWPTTreeItemModel *model, int position){
	if(!pTreeList || !model || position < 0 || position > GetChildrenCount()){
		DETHROW(deeInvalidParam);
	}
	
	igdeTreeItem *beforeItem = nullptr;
	if(position < GetChildrenCount()){
		beforeItem = GetFirstChild();
		while(position > 0){
			beforeItem = beforeItem->GetNext();
			position--;
		}
	}
	
	const ceWPTTreeItem::Ref item(ceWPTTreeItem::Ref::New(pTreeList));
	
	if(beforeItem){
		pTreeList->InsertItemBefore(beforeItem, item);
		
	}else{
		pTreeList->AppendItem(this, item);
	}
	
	item->SetModel(model);
}

void ceWPTTreeItem::RemoveItem(ceWPTTreeItemModel *model){
	if(!pTreeList || !model){
		DETHROW(deeInvalidParam);
	}
	
	ceWPTTreeItem * const item = model->GetTreeItem();
	if(!item){
		DETHROW(deeInvalidParam);
	}
	
	item->SetModel(nullptr);
	pTreeList->RemoveItem(item);
}

void ceWPTTreeItem::RemoveAllItems(){
	if(!pTreeList){
		DETHROW(deeInvalidParam);
	}
	
	if(GetChildrenCount() == 0){
		return;
	}
	
	igdeTreeItem *item = GetFirstChild();
	while(item){
		ceWPTTreeItem &ti = *((ceWPTTreeItem*)item);
		ti.RemoveAllItems();
		ti.SetModel(nullptr);
		item = item->GetNext();
	}
}

void ceWPTTreeItem::SortItems(){
	if(!pTreeList){
		DETHROW(deeInvalidParam);
	}
	
	pTreeList->SortItems(this);
}

void ceWPTTreeItem::SetAsCurrentItem(){
	if(!pTreeList){
		DETHROW(deeInvalidParam);
	}
	
	pTreeList->SetSelection(this);
}



void ceWPTTreeItem::OnSelected(){
	if(pTreeList && pModel){
		pModel->OnSelected();
	}
}

void ceWPTTreeItem::OnContextMenu(igdeMenuCascade &contextMenu){
	if(pTreeList && pModel){
		pModel->OnContextMenu(contextMenu);
	}
}



// FOX internal
/////////////////

void ceWPTTreeItem::OnExpanded(){
	if(pModel){
		pModel->SetExpanded(true);
		pModel->OnExpandedChanged();
	}
}

void ceWPTTreeItem::OnCollapsed(){
	if(pModel){
		pModel->SetExpanded(false);
		pModel->OnExpandedChanged();
	}
}
