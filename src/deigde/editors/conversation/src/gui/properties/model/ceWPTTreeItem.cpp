/* 
 * Drag[en]gine IGDE Conversation Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ceWPTTreeItem.h"
#include "ceWPTTreeItemModel.h"

#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/model/igdeTreeItemReference.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTreeItem::ceWPTTreeItem( igdeTreeList *treeList ) :
igdeTreeItem( "" ),
pTreeList( treeList ),
pModel( NULL )
{
	if( ! treeList ){
		DETHROW( deeInvalidParam );
	}
}

ceWPTTreeItem::~ceWPTTreeItem(){
	SetModel( NULL );
}



// Management
///////////////

void ceWPTTreeItem::SetModel( ceWPTTreeItemModel *model ){
	if( pModel == model ){
		return;
	}
	
	if( pModel ){
		pModel->SetTreeItem( NULL );
		pModel->FreeReference();
	}
	
	pModel = model;
	
	if( model ){
		model->AddReference();
		model->SetTreeItem( this );
	}
}



void ceWPTTreeItem::AddItem( ceWPTTreeItemModel *model ){
	if( ! pTreeList || ! model ){
		DETHROW( deeInvalidParam );
	}
	
	igdeTreeItemReference item;
	item.TakeOver( new ceWPTTreeItem( pTreeList ) );
	pTreeList->AppendItem( this, item );
	( ( ceWPTTreeItem& )( igdeTreeItem& )item ).SetModel( model );
}

void ceWPTTreeItem::InsertItem( ceWPTTreeItemModel *model, int position ){
	if( ! pTreeList || ! model || position < 0 || position > GetChildrenCount() ){
		DETHROW( deeInvalidParam );
	}
	
	igdeTreeItem *beforeItem = NULL;
	if( position < GetChildrenCount() ){
		beforeItem = GetFirstChild();
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
		pTreeList->AppendItem( this, item );
	}
	
	( ( ceWPTTreeItem& )( igdeTreeItem& )item ).SetModel( model );
}

void ceWPTTreeItem::RemoveItem( ceWPTTreeItemModel *model ){
	if( ! pTreeList || ! model ){
		DETHROW( deeInvalidParam );
	}
	
	ceWPTTreeItem * const item = model->GetTreeItem();
	if( ! item ){
		DETHROW( deeInvalidParam );
	}
	
	item->SetModel( NULL );
	pTreeList->RemoveItem( item );
}

void ceWPTTreeItem::RemoveAllItems(){
	if( ! pTreeList ){
		DETHROW( deeInvalidParam );
	}
	
	if( GetChildrenCount() == 0 ){
		return;
	}
	
	ceWPTTreeItem *item = ( ceWPTTreeItem* )GetFirstChild();
	while( item ){
		item->RemoveAllItems();
		item->SetModel( NULL );
		item = ( ceWPTTreeItem* )item->GetNext();
	}
}

void ceWPTTreeItem::SortItems(){
	if( ! pTreeList ){
		DETHROW( deeInvalidParam );
	}
	
	pTreeList->SortItems( this );
}

void ceWPTTreeItem::SetAsCurrentItem(){
	if( ! pTreeList ){
		DETHROW( deeInvalidParam );
	}
	
	pTreeList->SetSelection( this );
}



void ceWPTTreeItem::OnSelected(){
	if( pTreeList && pModel ){
		pModel->OnSelected();
	}
}

void ceWPTTreeItem::OnContextMenu( igdeMenuCascade &contextMenu ){
	if( pTreeList && pModel ){
		pModel->OnContextMenu( contextMenu );
	}
}



// FOX internal
/////////////////

void ceWPTTreeItem::OnExpanded(){
	if( pModel ){
		pModel->SetExpanded( true );
		pModel->OnExpandedChanged();
	}
}

void ceWPTTreeItem::OnCollapsed(){
	if( pModel ){
		pModel->SetExpanded( false );
		pModel->OnExpandedChanged();
	}
}
