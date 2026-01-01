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
#include "ceWPTTreeModel.h"
#include "ceWPTTreeItemModel.h"
#include "ceWPTTreeModelListener.h"
#include "action/ceWPTTIMAction.h"
#include "action/ceWPTTIMActions.h"
#include "condition/ceWPTTIMCondition.h"
#include "menuactions/action/topic/ceWPTMATopicAddAction.h"
#include "menuactions/action/topic/ceWPTMATopicCopyActions.h"
#include "menuactions/action/topic/ceWPTMATopicRemoveAllActions.h"
#include "menuactions/action/topic/ceWPTMATopicPasteActions.h"
#include "menuactions/action/topic/ceWPTMATopicPasteSnippet.h"
#include "menuactions/action/topic/ceWPTMATopicRemoveAction.h"
#include "menuactions/action/topic/ceWPTMATopicMoveAction.h"
#include "menuactions/action/topic/ceWPTMATopicCutAction.h"
#include "menuactions/action/ceWPTMACopyAction.h"
#include "../ceWPTopic.h"
#include "../ceWindowProperties.h"
#include "../../ceWindowMain.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/condition/ceConversationCondition.h"
#include "../../../conversation/file/ceConversationFile.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeTreeItem.h>

#include <dragengine/common/exceptions.h>



// ceWPTTreeModel::PreventUpdateGuard
///////////////////////////////////////

ceWPTTreeModel::PreventUpdateGuard::PreventUpdateGuard(ceWPTTreeModel &model) :
pModel(model), pPrevPreventUpdate(model.pPreventUpdate){
	model.pPreventUpdate = true;
}

ceWPTTreeModel::PreventUpdateGuard::~PreventUpdateGuard(){
	pModel.pPreventUpdate = pPrevPreventUpdate;
}



// Constructor, destructor
////////////////////////////

ceWPTTreeModel::ceWPTTreeModel(ceWindowMain &windowMain, ceConversation *conversation,
	ceConversationListener &forwardListener) :
pWindowMain(windowMain),
pForwardListener(forwardListener),
pTreeList(nullptr),
pPreventUpdate(false)
{
	DEASSERT_NOTNULL(conversation)
	
	pListener = ceWPTTreeModelListener::Ref::New(*this);
	conversation->AddListener(pListener);
	pConversation = conversation;
}

ceWPTTreeModel::~ceWPTTreeModel(){
	pCleanUp();
}



// Management
///////////////

void ceWPTTreeModel::SetTreeList(igdeTreeList *treeList){
	if(pTreeList == treeList){
		return;
	}
	
	if(pTreeList){
		RemoveAllChildren();
	}
	
	pTreeList = treeList;
	
	if(treeList){
		UpdateActions();
		SelectTopicActive();
	}
}



void ceWPTTreeModel::AddChild(ceWPTTreeItemModel *child){
	if(!pTreeList || !child || child->GetParent() || child->GetTree()){
		DETHROW(deeInvalidParam);
	}
	
	pChildren.Add(child);
	child->SetTree(this);
	
	ceWPTTreeItem::Ref item(ceWPTTreeItem::Ref::New(pTreeList));
	pTreeList->AppendItem(nullptr, item);
	item->SetModel(child);
}

void ceWPTTreeModel::InsertChild(ceWPTTreeItemModel *child, int position){
	if(!pTreeList || !child || child->GetParent() || position < 0 || position > pChildren.GetCount()){
		DETHROW(deeInvalidParam);
	}
	
	pChildren.Insert(child, position);
	child->SetTree(this);
	
	igdeTreeItem *beforeItem = nullptr;
	if(position < pChildren.GetCount()){
		beforeItem = pTreeList->GetFirstChild();
		while(position > 0){
			beforeItem = beforeItem->GetNext();
			position--;
		}
	}
	
	const ceWPTTreeItem::Ref item(ceWPTTreeItem::Ref::New(pTreeList));
	
	if(beforeItem){
		pTreeList->InsertItemBefore(beforeItem, item);
		
	}else{
		pTreeList->AppendItem(nullptr, item);
	}
	
	item->SetModel(child);
}

void ceWPTTreeModel::RemoveChild(ceWPTTreeItemModel *child){
	if(!pTreeList){
		DETHROW(deeInvalidParam);
	}
	
	if(!child || child->GetTree() != this){
		DETHROW(deeInvalidParam);
	}
	
	ceWPTTreeItem * const item = child->GetTreeItem();
	if(!item){
		DETHROW(deeInvalidParam);
	}
	
	item->SetModel(nullptr);
	pTreeList->RemoveItem(item);
	
	child->SetTree(nullptr);
	pChildren.Remove(child);
}

void ceWPTTreeModel::RemoveAllChildren(){
	if(!pTreeList){
		DETHROW(deeInvalidParam);
	}
	
	ceWPTTreeItem *item = (ceWPTTreeItem*)pTreeList->GetFirstChild().Pointer();
	while(item){
		item->RemoveAllItems();
		item->SetModel(nullptr);
		item = (ceWPTTreeItem*)item->GetNext().Pointer();
	}
	pTreeList->RemoveAllItems();
	
	pChildren.Visit([](ceWPTTreeItemModel &c){ c.SetTree(nullptr); });
	pChildren.RemoveAll();
}

void ceWPTTreeModel::MoveChild(ceWPTTreeItemModel *child, int to){
	if(!child){
		DETHROW(deeInvalidParam);
	}
	
	MoveChild(pChildren.IndexOf(child), to);
}

void ceWPTTreeModel::MoveChild(int from, int to){
	if(!pTreeList){
		DETHROW(deeInvalidParam);
	}
	
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
	
	igdeTreeItem *otherItem = nullptr;
	if(otherChild){
		otherItem = otherChild->GetTreeItem();
	}
	
	pTreeList->MoveItemBefore(child->GetTreeItem(), otherItem);
}



ceWPTTIMAction *ceWPTTreeModel::GetChildWith(ceConversationAction *action) const{
	for(const auto &c : pChildren){
		ceWPTTIMAction * const child = c.DynamicCast<ceWPTTIMAction>();
		if(child && child->GetAction() == action){
			return child;
		}
	}
	return nullptr;
}



void ceWPTTreeModel::UpdateActions(){
	if(!pTreeList){
		return;
	}
	
	const PreventUpdateGuard preventUpdate(*this);
	
	const ceConversationFile * const file = pConversation->GetActiveFile();
	if(!file){
		RemoveAllChildren();
		return;
	}
	
	const ceConversationTopic * const topic = file->GetActiveTopic();
	if(!topic){
		RemoveAllChildren();
		return;
	}
	
	const ceConversationAction::List &actions = topic->GetActions();
	const int count = actions.GetCount();
	int i, j;
	
	for(i=0; i<count; i++){
		ceConversationAction * const action = actions.GetAt(i);
		
		// find item matching action if present
		ceWPTTIMAction *findModel = nullptr;
		const int childCount = GetChildren().GetCount();
		for(j=i; j<childCount; j++){
			const ceWPTTIMAction::Ref child(GetChildren().GetAt(j).DynamicCast<ceWPTTIMAction>());
			if(child->GetAction() == action){
				findModel = child;
				break;
			}
		}
		
		// if model exists move it to the right location if required and update it.
		// if model does not exist create it and add it at the current location.
		if(findModel){
			if(j != i){
				MoveChild(j, i);
			}
			findModel->Update();
			
		}else{
			const ceWPTTIMAction::Ref model(ceWPTTIMActions::CreateActionModel(pWindowMain, *pConversation, action));
			InsertChild(model, i);
			model->Update();
		}
	}
	
	// remove non-matching nodes
	while(pChildren.GetCount() > count){
		RemoveChild(pChildren.Last());
	}
}


void ceWPTTreeModel::OnContextMenu(igdeMenuCascade &contextMenu){
	ContextMenuTopic(contextMenu);
}

void ceWPTTreeModel::ContextMenuAction(igdeMenuCascade &contextMenu, ceConversationAction *action){
	if(!action){
		DETHROW(deeInvalidParam);
	}
	
	if(!pConversation){
		return;
	}
	
	ceConversationFile * const file = pConversation->GetActiveFile();
	ceConversationTopic * const topic = file ? file->GetActiveTopic() : nullptr;
	if(!topic){
		return;
	}
	
	igdeEnvironment &environment = pWindowMain.GetEnvironment();
	igdeUIHelper &helper = environment.GetUIHelper();
	const int indexAction = action ? topic->GetActions().IndexOf(action) : -1;
	igdeMenuCascade::Ref subMenu;
	int i;
	
	// child action specific
	helper.MenuCommand(contextMenu, ceWPTMATopicMoveAction::Ref::New(pWindowMain, *pConversation,
		*topic, action, indexAction - 1, "Move Action Up",
		pWindowMain.GetEnvironment().GetStockIcon(igdeEnvironment::esiUp)));
	helper.MenuCommand(contextMenu, ceWPTMATopicMoveAction::Ref::New(pWindowMain, *pConversation,
		*topic, action, indexAction + 1, "Move Action Down",
		pWindowMain.GetEnvironment().GetStockIcon(igdeEnvironment::esiDown)));
	helper.MenuSeparator(contextMenu);
	
	helper.MenuCommand(contextMenu, ceWPTMACopyAction::Ref::New(pWindowMain, action));
	helper.MenuCommand(contextMenu, ceWPTMATopicCutAction::Ref::New(
		pWindowMain, *pConversation, *topic, action));
	helper.MenuCommand(contextMenu, ceWPTMATopicPasteActions::Ref::New(
		pWindowMain, *pConversation, *topic, indexAction, "Paste Actions Before"));
	helper.MenuCommand(contextMenu, ceWPTMATopicPasteActions::Ref::New(
		pWindowMain, *pConversation, *topic, indexAction + 1, "Paste Actions After"));
	helper.MenuSeparator(contextMenu);
	helper.MenuCommand(contextMenu, ceWPTMATopicPasteSnippet::Ref::New(
		pWindowMain, *pConversation, *topic, indexAction + 1));
	
	helper.MenuCommand(contextMenu, ceWPTMATopicRemoveAction::Ref::New(
		pWindowMain, *pConversation, *topic, action));
	helper.MenuSeparator(contextMenu);
	
	subMenu = igdeMenuCascade::Ref::New(environment, "Insert Action Before",
		environment.GetStockIcon(igdeEnvironment::esiPlus));
	for(i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++){
		helper.MenuCommand(subMenu, ceWPTMATopicAddAction::Ref::New(
			pWindowMain, *pConversation, *topic,
			ceWPTTIMAction::ListAddMenuActions[i], indexAction));
	}
	contextMenu.AddChild(subMenu);
	
	subMenu = igdeMenuCascade::Ref::New(environment, "Insert Action After",
		environment.GetStockIcon(igdeEnvironment::esiPlus));
	for(i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++){
		helper.MenuCommand(subMenu, ceWPTMATopicAddAction::Ref::New(
			pWindowMain, *pConversation, *topic,
			ceWPTTIMAction::ListAddMenuActions[i], indexAction + 1));
	}
	contextMenu.AddChild(subMenu);
	helper.MenuSeparator(contextMenu);
	
	// topic specific
	subMenu = igdeMenuCascade::Ref::New(environment, "Topic", nullptr);
	ContextMenuTopic(subMenu);
	contextMenu.AddChild(subMenu);
}

void ceWPTTreeModel::ContextMenuTopic(igdeMenuCascade &contextMenu){
	if(!pConversation){
		return;
	}
	
	ceConversationFile * const file = pConversation->GetActiveFile();
	ceConversationTopic * const topic = file ? file->GetActiveTopic() : nullptr;
	if(!topic){
		return;
	}
	
	igdeEnvironment &environment = pWindowMain.GetEnvironment();
	igdeUIHelper &helper = environment.GetUIHelper();
	const int indexAppend = topic->GetActions().GetCount();
	igdeMenuCascade::Ref subMenu;
	int i;
	
	subMenu = igdeMenuCascade::Ref::New(environment, "Add Action",
		environment.GetStockIcon(igdeEnvironment::esiPlus));
	for(i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++){
		helper.MenuCommand(subMenu, ceWPTMATopicAddAction::Ref::New(pWindowMain, *pConversation, *topic,
			ceWPTTIMAction::ListAddMenuActions[i], indexAppend));
	}
	contextMenu.AddChild(subMenu);
	
	helper.MenuCommand(contextMenu, ceWPTMATopicCopyActions::Ref::New(pWindowMain, *topic));
	helper.MenuCommand(contextMenu, ceWPTMATopicPasteActions::Ref::New(
		pWindowMain, *pConversation, *topic, indexAppend));
	helper.MenuCommand(contextMenu, ceWPTMATopicPasteSnippet::Ref::New(
		pWindowMain, *pConversation, *topic, indexAppend));
	
	helper.MenuSeparator(contextMenu);
	helper.MenuCommand(contextMenu, ceWPTMATopicRemoveAllActions::Ref::New(
		pWindowMain, *pConversation, *topic));
}

ceWPTTIMAction *ceWPTTreeModel::DeepFindAction(ceConversationAction *action) const{
	for(const auto &c : pChildren){
		ceWPTTIMAction * const child = c->DeepFindAction(action);
		if(child){
			return child;
		}
	}
	return nullptr;
}

ceWPTTIMCondition *ceWPTTreeModel::DeepFindCondition(ceConversationCondition *condition) const{
	for(const auto &c : pChildren){
		ceWPTTIMCondition * const child = c->DeepFindCondition(condition);
		if(child){
			return child;
		}
	}
	return nullptr;
}

void ceWPTTreeModel::SelectTopicActive(){
	if(!pTreeList){
		return;
	}
	
	const ceConversationFile * const file = pConversation->GetActiveFile();
	if(!file){
		return;
	}
	
	const ceConversationTopic * const topic = file->GetActiveTopic();
	if(!topic){
		return;
	}
	
	ceConversationCondition * const condition = topic->GetActiveCondition();
	if(condition){
		ceWPTTIMCondition * const model = DeepFindCondition(condition);
		if(model){
			model->SetAsCurrentItem();
			pWindowMain.GetWindowProperties().GetPanelTopic()->SelectActivePanel();
			return;
		}
	}
	
	ceConversationAction * const action = topic->GetActiveAction();
	if(action){
		ceWPTTIMAction * const model = DeepFindAction(action);
		if(model){
			model->SetAsCurrentItem();
			pWindowMain.GetWindowProperties().GetPanelTopic()->SelectActivePanel();
		}
	}
}



// Private Functions
//////////////////////

void ceWPTTreeModel::pCleanUp(){
	SetTreeList(nullptr);
	
	if(pListener && pConversation){
		pConversation->RemoveListener(pListener);
	}
}
