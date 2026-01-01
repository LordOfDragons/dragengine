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
#include "ceWPTTreeModel.h"
#include "../../../conversation/ceConversation.h"

#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/resources/igdeIcon.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTreeItemModel::ceWPTTreeItemModel(ceWindowMain &windowMain,
ceConversation &conversation, eTypes type) :
pWindowMain(windowMain),
pConversation(conversation),
pType(type),
pExpanded(false),
pTree(nullptr),
pParent(nullptr),
pTreeItem(nullptr){
}

ceWPTTreeItemModel::~ceWPTTreeItemModel(){
	RemoveAllChildren();
}



// Management
///////////////

void ceWPTTreeItemModel::SetText(const char *text){
	pText = text;
	
	if(pTreeItem && pTreeItem->GetText() != text){
		pTreeItem->SetText(text);
		
		if(pTree && pTree->GetTreeList()){
			pTree->GetTreeList()->ItemChanged(pTreeItem);
		}
	}
}

void ceWPTTreeItemModel::SetDescription(const char *description){
	pDescription = description;
	
	if(pTreeItem && pTreeItem->GetDescription() != description){
		pTreeItem->SetDescription(description);
		
		if(pTree && pTree->GetTreeList()){
			pTree->GetTreeList()->ItemChanged(pTreeItem);
		}
	}
}

void ceWPTTreeItemModel::SetIcon(igdeIcon *icon){
	pIcon = icon;
	
	if(pTreeItem && pTreeItem->GetIcon() != icon){
		pTreeItem->SetIcon(icon);
		
		if(pTree && pTree->GetTreeList()){
			pTree->GetTreeList()->ItemChanged(pTreeItem);
		}
	}
}

void ceWPTTreeItemModel::SetExpanded(bool expanded){
	pExpanded = expanded;
	
	if(pTreeItem && pTreeItem->GetExpanded() != expanded){
		pTreeItem->SetExpanded(expanded);
		
		if(pTree && pTree->GetTreeList()){
			pTree->GetTreeList()->ItemChanged(pTreeItem);
		}
	}
}



void ceWPTTreeItemModel::AddChild(ceWPTTreeItemModel *child){
	if(!child || child->GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	pChildren.Add(child);
	child->SetParent(this);
	
	if(pTreeItem){
		child->SetTree(pTree);
		pTreeItem->AddItem(child);
	}
}

void ceWPTTreeItemModel::InsertChild(ceWPTTreeItemModel *child, int position){
	if(!child || child->GetParent() || position < 0 || position > pChildren.GetCount()){
		DETHROW(deeInvalidParam);
	}
	
	pChildren.Insert(child, position);
	child->SetParent(this);
	
	if(pTreeItem){
		child->SetTree(pTree);
		pTreeItem->InsertItem(child, position);
	}
}

void ceWPTTreeItemModel::RemoveChild(ceWPTTreeItemModel *child){
	if(!child || child->GetParent() != this){
		DETHROW(deeInvalidParam);
	}
	
	if(pTreeItem && child->GetTreeItem()){
		pTreeItem->RemoveItem(child);
		child->SetTree(nullptr);
	}
	
	child->SetParent(nullptr);
	pChildren.Remove(child);
}

void ceWPTTreeItemModel::RemoveAllChildren(){
	if(pTreeItem){
		pTreeItem->RemoveAllItems();
	}
	
	pChildren.Visit([](ceWPTTreeItemModel &c){
		c.SetTree(nullptr);
		c.SetParent(nullptr);
	});
	pChildren.RemoveAll();
}

void ceWPTTreeItemModel::MoveChild(ceWPTTreeItemModel *child, int to){
	if(!child){
		DETHROW(deeInvalidParam);
	}
	
	MoveChild(pChildren.IndexOf(child), to);
}

void ceWPTTreeItemModel::MoveChild(int from, int to){
	const int count = pChildren.GetCount();
	if(from < 0 || from >= count || to < 0 || to > count){
		DETHROW(deeInvalidParam);
	}
	
	if(from == to){
		return;
	}
	
	ceWPTTreeItemModel * const child = pChildren.GetAt(from);
	
	ceWPTTreeItemModel *otherChild = nullptr;
	if(to < count){
		otherChild = pChildren.GetAt(to);
	}
	
	pChildren.Move(child, to);
	
	if(pTreeItem && child->GetTreeItem() && (!otherChild || otherChild->GetTreeItem())){
		igdeTreeItem *otherItem = nullptr;
		if(otherChild){
			otherItem = otherChild->GetTreeItem();
		}
		
		pTreeItem->GetTreeList().MoveItemBefore(child->GetTreeItem(), otherItem);
	}
}



void ceWPTTreeItemModel::SetTree(ceWPTTreeModel *tree){
	pTree = tree;
}

ceWPTTreeModel *ceWPTTreeItemModel::GetFirstTree() const{
	const ceWPTTreeItemModel *model = this;
	
	while(model){
		if(model->pTree){
			return model->pTree;
		}
		model = model->pParent;
	}
	
	return nullptr;
}

void ceWPTTreeItemModel::SetParent(ceWPTTreeItemModel *parent){
	pParent = parent;
}

void ceWPTTreeItemModel::SetTreeItem(ceWPTTreeItem *treeItem){
	if(pTreeItem == treeItem){
		return;
	}
	
	if(pTreeItem){
		pTreeItem->RemoveAllItems();
	}
	
	pTreeItem = treeItem;
	
	if(treeItem){
		treeItem->SetText(pText);
		treeItem->SetDescription(pDescription);
		treeItem->SetIcon(pIcon);
		treeItem->SetExpanded(pExpanded);
		
		if(pTree && pTree->GetTreeList()){
			pTree->GetTreeList()->ItemChanged(treeItem);
		}
		
		pChildren.Visit([&](ceWPTTreeItemModel *c){
			c->SetTree(pTree);
			treeItem->AddItem(c);
		});
		
		treeItem->SortItems();
	}
}

void ceWPTTreeItemModel::SortChildren(){
	if(pTreeItem){
		pTreeItem->SortItems();
	}
}

void ceWPTTreeItemModel::ParentSortItems(){
	if(pTreeItem && pTreeItem->GetParent()){
		((ceWPTTreeItem*)pTreeItem->GetParent())->SortItems();
	}
}

void ceWPTTreeItemModel::SetAsCurrentItem(){
	if(pTreeItem){
		pTreeItem->SetAsCurrentItem();
	}
}



void ceWPTTreeItemModel::OnSelected(){
}

void ceWPTTreeItemModel::OnContextMenu(igdeMenuCascade&){
}

void ceWPTTreeItemModel::ContextMenuAction(igdeMenuCascade&, ceConversationAction*){
}

void ceWPTTreeItemModel::ContextMenuCondition(igdeMenuCascade&, ceConversationCondition*){
}

ceWPTTIMAction *ceWPTTreeItemModel::DeepFindAction(ceConversationAction *action){
	for(const auto &c : pChildren){
		ceWPTTIMAction * const child = c->DeepFindAction(action);
		if(child){
			return child;
		}
	}
	return nullptr;
}

ceWPTTIMCondition *ceWPTTreeItemModel::DeepFindCondition(ceConversationCondition *condition){
	for(const auto &c : pChildren){
		ceWPTTIMCondition * const child = c->DeepFindCondition(condition);
		if(child){
			return child;
		}
	}
	return nullptr;
}

void ceWPTTreeItemModel::OnExpandedChanged(){
	if(pTreeItem){
		pExpanded = pTreeItem->GetExpanded();
	}
}

ceConversationAction *ceWPTTreeItemModel::GetOwnerAction() const{
	if(pParent){
		return pParent->GetOwnerAction();
		
	}else{
		return nullptr;
	}
}

ceConversationCondition *ceWPTTreeItemModel::GetOwnerCondition() const{
	if(pParent){
		return pParent->GetOwnerCondition();
		
	}else{
		return nullptr;
	}
}

void ceWPTTreeItemModel::BuildPlaybackFromHere() const{
	if(pParent){
		pParent->BuildPlaybackFromHere();
	}
}
