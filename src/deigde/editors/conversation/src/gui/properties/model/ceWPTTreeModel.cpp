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
#include <deigde/gui/menu/igdeMenuCascadeReference.h>
#include <deigde/gui/model/igdeTreeItemReference.h>

#include <dragengine/common/exceptions.h>



// ceWPTTreeModel::PreventUpdateGuard
///////////////////////////////////////

ceWPTTreeModel::PreventUpdateGuard::PreventUpdateGuard( ceWPTTreeModel &model ) :
pModel( model ), pPrevPreventUpdate( model.pPreventUpdate ){
	model.pPreventUpdate = true;
}

ceWPTTreeModel::PreventUpdateGuard::~PreventUpdateGuard(){
	pModel.pPreventUpdate = pPrevPreventUpdate;
}



// Constructor, destructor
////////////////////////////

ceWPTTreeModel::ceWPTTreeModel( ceWindowMain &windowMain, ceConversation *conversation,
	ceConversationListener &forwardListener ) :
pWindowMain( windowMain ),
pConversation( NULL ),
pListener( NULL ),
pForwardListener( forwardListener ),
pTreeList( NULL ),
pPreventUpdate( false )
{
	if( ! conversation ){
		DETHROW( deeInvalidParam );
	}
	
	try{
		pListener = new ceWPTTreeModelListener( *this );
		conversation->AddListener( pListener );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	
	pConversation = conversation;
	conversation->AddReference();
}

ceWPTTreeModel::~ceWPTTreeModel(){
	pCleanUp();
}



// Management
///////////////

void ceWPTTreeModel::SetTreeList( igdeTreeList *treeList ){
	if( pTreeList == treeList ){
		return;
	}
	
	if( pTreeList ){
		RemoveAllChildren();
	}
	
	pTreeList = treeList;
	
	if( treeList ){
		UpdateActions();
		      SelectTopicActive();
	}
}



int ceWPTTreeModel::GetChildCount() const{
	return pChildren.GetCount();
}

ceWPTTreeItemModel *ceWPTTreeModel::GetChildAt( int index ) const{
	return ( ceWPTTreeItemModel* )pChildren.GetAt( index );
}

void ceWPTTreeModel::AddChild( ceWPTTreeItemModel *child ){
	if( ! pTreeList || ! child || child->GetParent() || child->GetTree() ){
		DETHROW( deeInvalidParam );
	}
	
	pChildren.Add( child );
	child->SetTree( this );
	
	igdeTreeItemReference item;
	item.TakeOver( new ceWPTTreeItem( pTreeList ) );
	pTreeList->AppendItem( NULL, item );
	( ( ceWPTTreeItem& )( igdeTreeItem& )item ).SetModel( child );
}

void ceWPTTreeModel::InsertChild( ceWPTTreeItemModel *child, int position ){
	if( ! pTreeList || ! child || child->GetParent() || position < 0 || position > pChildren.GetCount() ){
		DETHROW( deeInvalidParam );
	}
	
	pChildren.Insert( child, position );
	child->SetTree( this );
	
	igdeTreeItem *beforeItem = NULL;
	if( position < pChildren.GetCount() ){
		beforeItem = pTreeList->GetFirstChild();
		while( position > 0 ){
			beforeItem = beforeItem->GetNext();
			position--;
		}
	}
	
	igdeTreeItemReference item;
	item.TakeOver( new ceWPTTreeItem( pTreeList ) );
	
	if( beforeItem ){
		pTreeList->InsertItemBefore( beforeItem, item );
		
	}else{
		pTreeList->AppendItem( NULL, item );
	}
	
	( ( ceWPTTreeItem& )( igdeTreeItem& )item ).SetModel( child );
}

void ceWPTTreeModel::RemoveChild( ceWPTTreeItemModel *child ){
	if( ! pTreeList ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! child || child->GetTree() != this ){
		DETHROW( deeInvalidParam );
	}
	
	ceWPTTreeItem * const item = child->GetTreeItem();
	if( ! item ){
		DETHROW( deeInvalidParam );
	}
	
	item->SetModel( NULL );
	pTreeList->RemoveItem( item );
	
	child->SetTree( NULL );
	pChildren.Remove( child );
}

void ceWPTTreeModel::RemoveAllChildren(){
	if( ! pTreeList ){
		DETHROW( deeInvalidParam );
	}
	
	ceWPTTreeItem *item = ( ceWPTTreeItem* )pTreeList->GetFirstChild();
	while( item ){
		item->RemoveAllItems();
		item->SetModel( NULL );
		item = ( ceWPTTreeItem* )item->GetNext();
	}
	pTreeList->RemoveAllItems();
	
	const int count = pChildren.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( ceWPTTreeItemModel* )pChildren.GetAt( i ) )->SetTree( NULL );
	}
	
	pChildren.RemoveAll();
}

void ceWPTTreeModel::MoveChild( ceWPTTreeItemModel *child, int to ){
	if( ! child ){
		DETHROW( deeInvalidParam );
	}
	
	MoveChild( pChildren.IndexOf( child ), to );
}

void ceWPTTreeModel::MoveChild( int from, int to ){
	if( ! pTreeList ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pChildren.GetCount();
	if( from < 0 || from >= count || to < 0 || to > count ){
		DETHROW( deeInvalidParam );
	}
	
	if( from == to ){
		return;
	}
	
	ceWPTTreeItemModel * const child = ( ceWPTTreeItemModel* )pChildren.GetAt( from );
	
	ceWPTTreeItemModel *otherChild = NULL;
	if( to < count ){
		otherChild = ( ceWPTTreeItemModel* )pChildren.GetAt( to );
	}
	
	pChildren.Move( child, to );
	
	igdeTreeItem *otherItem = NULL;
	if( otherChild ){
		otherItem = otherChild->GetTreeItem();
	}
	
	pTreeList->MoveItemBefore( child->GetTreeItem(), otherItem );
}



ceWPTTIMAction *ceWPTTreeModel::GetChildWith( ceConversationAction *action ) const{
	const int count = GetChildCount();
	int i;
	
	for( i=0; i<count; i++ ){
		ceWPTTIMAction * const child = ( ceWPTTIMAction* )GetChildAt( i );
		if( child->GetAction() == action ){
			return child;
		}
	}
	
	return NULL;
}



void ceWPTTreeModel::UpdateActions(){
	if( ! pTreeList ){
		return;
	}
	
	const PreventUpdateGuard preventUpdate( *this );
	
	const ceConversationFile * const file = pConversation->GetActiveFile();
	if( ! file ){
		RemoveAllChildren();
		return;
	}
	
	const ceConversationTopic * const topic = file->GetActiveTopic();
	if( ! topic ){
		RemoveAllChildren();
		return;
	}
	
	const ceConversationActionList &actions = topic->GetActionList();
	const int count = actions.GetCount();
	int i, j;
	
	for( i=0; i<count; i++ ){
		ceConversationAction * const action = actions.GetAt( i );
		
		// find item matching action if present
		ceWPTTIMAction *model = NULL;
		const int childCount = GetChildCount();
		for( j=i; j<childCount; j++ ){
			ceWPTTIMAction * const child = ( ceWPTTIMAction* )GetChildAt( j );
			if( child->GetAction() == action ){
				model = child;
				break;
			}
		}
		
		// if model exists move it to the right location if required and update it.
		// if model does not exist create it and add it at the current location.
		if( model ){
			if( j != i ){
				MoveChild( j, i );
			}
			model->Update();
			
		}else{
			model = ceWPTTIMActions::CreateActionModel( pWindowMain, *pConversation, action );
			
			try{
				InsertChild( model, i );
				model->Update();
				
			}catch( const deException & ){
				model->FreeReference();
				throw;
			}
			
			model->FreeReference();
		}
	}
	
	// remove non-matching nodes
	while( GetChildCount() > count ){
		RemoveChild( GetChildAt( GetChildCount() - 1 ) );
	}
}


void ceWPTTreeModel::OnContextMenu( igdeMenuCascade &contextMenu ){
	ContextMenuTopic( contextMenu );
}

void ceWPTTreeModel::ContextMenuAction( igdeMenuCascade &contextMenu, ceConversationAction *action ){
	if( ! action ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! pConversation ){
		return;
	}
	
	ceConversationFile * const file = pConversation->GetActiveFile();
	ceConversationTopic * const topic = file ? file->GetActiveTopic() : NULL;
	if( ! topic ){
		return;
	}
	
	igdeEnvironment &environment = pWindowMain.GetEnvironment();
	igdeUIHelper &helper = environment.GetUIHelper();
	const ceConversationActionList &actions = topic->GetActionList();
	const int indexAction = action ? actions.IndexOf( action ) : -1;
	igdeMenuCascadeReference subMenu;
	int i;
	
	// child action specific
	helper.MenuCommand( contextMenu, new ceWPTMATopicMoveAction( pWindowMain, *pConversation,
		*topic, action, indexAction - 1, "Move Action Up",
		pWindowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiUp ) ), true );
	helper.MenuCommand( contextMenu, new ceWPTMATopicMoveAction( pWindowMain, *pConversation,
		*topic, action, indexAction + 1, "Move Action Down",
		pWindowMain.GetEnvironment().GetStockIcon( igdeEnvironment::esiDown ) ), true );
	helper.MenuSeparator( contextMenu );
	
	helper.MenuCommand( contextMenu, new ceWPTMACopyAction( pWindowMain, action ), true );
	helper.MenuCommand( contextMenu, new ceWPTMATopicCutAction(
		pWindowMain, *pConversation, *topic, action ), true );
	helper.MenuCommand( contextMenu, new ceWPTMATopicPasteActions( pWindowMain, *pConversation,
		*topic, indexAction, "Paste Actions Before" ), true );
	helper.MenuCommand( contextMenu, new ceWPTMATopicPasteActions( pWindowMain, *pConversation,
		*topic, indexAction + 1, "Paste Actions After" ), true );
	helper.MenuSeparator( contextMenu );
	helper.MenuCommand( contextMenu, new ceWPTMATopicPasteSnippet(
		pWindowMain, *pConversation, *topic, indexAction + 1 ), true );
	
	helper.MenuCommand( contextMenu, new ceWPTMATopicRemoveAction(
		pWindowMain, *pConversation, *topic, action ), true );
	helper.MenuSeparator( contextMenu );
	
	subMenu.TakeOver( new igdeMenuCascade( environment, "Insert Action Before",
		environment.GetStockIcon( igdeEnvironment::esiPlus ) ) );
	for( i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++ ){
		helper.MenuCommand( subMenu, new ceWPTMATopicAddAction( pWindowMain, *pConversation, *topic,
			ceWPTTIMAction::ListAddMenuActions[ i ], indexAction ), true );
	}
	contextMenu.AddChild( subMenu );
	
	subMenu.TakeOver( new igdeMenuCascade( environment, "Insert Action After",
		environment.GetStockIcon( igdeEnvironment::esiPlus ) ) );
	for( i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++ ){
		helper.MenuCommand( subMenu, new ceWPTMATopicAddAction( pWindowMain, *pConversation, *topic,
			ceWPTTIMAction::ListAddMenuActions[ i ], indexAction + 1 ), true );
	}
	contextMenu.AddChild( subMenu );
	helper.MenuSeparator( contextMenu );
	
	// topic specific
	subMenu.TakeOver( new igdeMenuCascade( environment, "Topic", NULL ) );
	ContextMenuTopic( subMenu );
	contextMenu.AddChild( subMenu );
}

void ceWPTTreeModel::ContextMenuTopic( igdeMenuCascade &contextMenu ){
	if( ! pConversation ){
		return;
	}
	
	ceConversationFile * const file = pConversation->GetActiveFile();
	ceConversationTopic * const topic = file ? file->GetActiveTopic() : NULL;
	if( ! topic ){
		return;
	}
	
	igdeEnvironment &environment = pWindowMain.GetEnvironment();
	igdeUIHelper &helper = environment.GetUIHelper();
	const int indexAppend = topic->GetActionList().GetCount();
	igdeMenuCascadeReference subMenu;
	int i;
	
	subMenu.TakeOver( new igdeMenuCascade( environment, "Add Action",
		environment.GetStockIcon( igdeEnvironment::esiPlus ) ) );
	for( i=0; i<ceWPTTIMAction::ListAddMenuActionsCount; i++ ){
		helper.MenuCommand( subMenu, new ceWPTMATopicAddAction( pWindowMain, *pConversation, *topic,
			ceWPTTIMAction::ListAddMenuActions[ i ], indexAppend ), true );
	}
	contextMenu.AddChild( subMenu );
	
	helper.MenuCommand( contextMenu, new ceWPTMATopicCopyActions( pWindowMain, *topic ), true );
	helper.MenuCommand( contextMenu, new ceWPTMATopicPasteActions(
		pWindowMain, *pConversation, *topic, indexAppend ), true );
	helper.MenuCommand( contextMenu, new ceWPTMATopicPasteSnippet(
		pWindowMain, *pConversation, *topic, indexAppend ), true );
	
	helper.MenuSeparator( contextMenu );
	helper.MenuCommand( contextMenu, new ceWPTMATopicRemoveAllActions(
		pWindowMain, *pConversation, *topic ), true );
}

ceWPTTIMAction *ceWPTTreeModel::DeepFindAction( ceConversationAction *action ) const{
	const int count = pChildren.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		ceWPTTIMAction * const child = ( ( ceWPTTreeItemModel* )
			pChildren.GetAt( i ) )->DeepFindAction( action );
		if( child ){
			return child;
		}
	}
	
	return NULL;
}

ceWPTTIMCondition *ceWPTTreeModel::DeepFindCondition( ceConversationCondition *condition ) const{
	const int count = pChildren.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		ceWPTTIMCondition * const child = ( ( ceWPTTreeItemModel* )
			pChildren.GetAt( i ) )->DeepFindCondition( condition );
		if( child ){
			return child;
		}
	}
	
	return NULL;
}

void ceWPTTreeModel::SelectTopicActive(){
	if( ! pTreeList ){
		return;
	}
	
	const ceConversationFile * const file = pConversation->GetActiveFile();
	if( ! file ){
		return;
	}
	
	const ceConversationTopic * const topic = file->GetActiveTopic();
	if( ! topic ){
		return;
	}
	
	ceConversationCondition * const condition = topic->GetActiveCondition();
	if( condition ){
		ceWPTTIMCondition * const model = DeepFindCondition( condition );
		if( model ){
			model->SetAsCurrentItem();
			pWindowMain.GetWindowProperties().GetPanelTopic().SelectActivePanel();
			return;
		}
	}
	
	ceConversationAction * const action = topic->GetActiveAction();
	if( action ){
		ceWPTTIMAction * const model = DeepFindAction( action );
		if( model ){
			model->SetAsCurrentItem();
			pWindowMain.GetWindowProperties().GetPanelTopic().SelectActivePanel();
		}
	}
}



// Private Functions
//////////////////////

void ceWPTTreeModel::pCleanUp(){
	SetTreeList( NULL );
	
	if( pListener ){
		if( pConversation ){
			pConversation->RemoveListener( pListener );
		}
		delete pListener;
	}
	
	if( pConversation ){
		pConversation->FreeReference();
	}
}
