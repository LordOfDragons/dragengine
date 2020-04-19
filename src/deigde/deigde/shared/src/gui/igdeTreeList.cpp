/* 
 * Drag[en]gine IGDE
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

#include "igdeTreeList.h"
#include "igdeContainer.h"
#include "igdeCommonDialogs.h"
#include "event/igdeTreeListListener.h"
#include "menu/igdeMenuCascade.h"
#include "menu/igdeMenuCascadeReference.h"
#include "model/igdeTreeItem.h"
#include "model/igdeTreeItemReference.h"
#include "model/igdeTreeItemSorter.h"
#include "native/fox/igdeNativeFoxTreeItem.h"
#include "native/fox/igdeNativeFoxTreeList.h"
#include "resources/igdeIcon.h"
#include "resources/igdeFont.h"
#include "resources/igdeFontReference.h"
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

igdeTreeList::igdeTreeList( igdeEnvironment &environment, int rows, const char *description ) :
igdeWidget( environment ),
pEnabled( true ),
pRows( rows ),
pDescription( description )
{
	if( rows < 1 ){
		DETHROW( deeInvalidParam );
	}
}

igdeTreeList::~igdeTreeList(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeTreeList::SetEnabled( bool enabled ){
	if( pEnabled == enabled ){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeTreeList::SetRows( int rows ){
	if( rows < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( rows == pRows ){
		return;
	}
	
	pRows = rows;
	OnRowsChanged();
}

void igdeTreeList::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeTreeList::Focus(){
	if( GetNativeWidget() ){
		( ( igdeNativeFoxTreeList* )GetNativeWidget() )->Focus();
	}
}



igdeTreeItem *igdeTreeList::GetLastChild() const{
	igdeTreeItem *item = pFirstChild;
	igdeTreeItem *lastChild = NULL;
	while( item ){
		lastChild = item;
		item = item->GetNext();
	}
	return lastChild;
}

int igdeTreeList::GetChildrenCount() const{
	int count = 0;
	igdeTreeItem *item = pFirstChild;
	while( item ){
		count++;
		item = item->GetNext();
	}
	return count;
}

igdeTreeItem *igdeTreeList::GetItemWithData( void *data ) const{
	return pGetItemWithData( NULL, data );
}

igdeTreeItem *igdeTreeList::GetItemWithData( igdeTreeItem *parent, void *data ) const{
	return pGetItemWithData( parent, data );
}

bool igdeTreeList::HasItem( igdeTreeItem *item ) const{
	return pHasItem( NULL, item );
}

bool igdeTreeList::HasItem( const char *text ) const{
	return pHasItem( NULL, text );
}

bool igdeTreeList::HasItemWithData( void *data ) const{
	return pHasItem( NULL, data );
}

void igdeTreeList::AppendItem( igdeTreeItem *parent, igdeTreeItem *item ){
	if( ! item || item->GetParent() || item->GetPrevious() || item->GetNext() ){
		DETHROW( deeInvalidParam );
	}
	
	pAppendItem( parent, item );
	OnItemAdded( item );
	
	if( ! pSelection ){
		SetSelection( item );
	}
}

igdeTreeItem *igdeTreeList::AppendItem( igdeTreeItem *parent, const char *text, igdeIcon *icon, void *data ){
	igdeTreeItemReference item;
	AppendItem( parent, item, text, icon, data );
	return item;
}

void igdeTreeList::AppendItem( igdeTreeItem *parent, igdeTreeItemReference &item,
const char *text, igdeIcon *icon, void *data ){
	item.TakeOver( new igdeTreeItem( text, icon, data ) );
	AppendItem( parent, item );
}

void igdeTreeList::InsertItemBefore( igdeTreeItem *beforeItem, igdeTreeItem *item ){
	if( ! item || item->GetParent() || item->GetPrevious() || item->GetNext() || ! beforeItem ){
		DETHROW( deeInvalidParam );
	}
	
	pInsertItemBefore( item, beforeItem );
	OnItemAdded( item );
	
	if( ! pSelection ){
		SetSelection( item );
	}
}

igdeTreeItem *igdeTreeList::InsertItemBefore( igdeTreeItem *beforeItem, const char *text,
igdeIcon *icon, void *data ){
	igdeTreeItemReference item;
	InsertItemBefore( beforeItem, item, text, icon, data );
	return item;
}

void igdeTreeList::InsertItemBefore( igdeTreeItem *beforeItem, igdeTreeItemReference &item,
const char *text, igdeIcon *icon, void *data ){
	item.TakeOver( new igdeTreeItem( text, icon, data ) );
	InsertItemBefore( beforeItem, item );
}

void igdeTreeList::InsertItemAfter( igdeTreeItem *afterItem, igdeTreeItem *item ){
	if( ! item || item->GetParent() || item->GetPrevious() || item->GetNext() || ! afterItem ){
		DETHROW( deeInvalidParam );
	}
	
	pInsertItemAfter( item, afterItem );
	OnItemAdded( item );
	
	if( ! pSelection ){
		SetSelection( item );
	}
}

igdeTreeItem *igdeTreeList::InsertItemAfter( igdeTreeItem *afterItem, const char *text,
igdeIcon *icon, void *data ){
	igdeTreeItemReference item;
	InsertItemAfter( afterItem, item, text, icon, data );
	return item;
}

void igdeTreeList::InsertItemAfter( igdeTreeItem *afterItem, igdeTreeItemReference &item,
const char *text, igdeIcon *icon, void *data ){
	item.TakeOver( new igdeTreeItem( text, icon, data ) );
	InsertItemAfter( afterItem, item );
}

void igdeTreeList::MoveItemBefore( igdeTreeItem *item, igdeTreeItem *beforeItem ){
	if( ! item || ! beforeItem ){
		DETHROW( deeInvalidParam );
	}
	
	const igdeTreeItemReference guard( item );
	pRemoveItem( item );
	pInsertItemBefore( item, beforeItem );
	OnItemMoved( item );
}

void igdeTreeList::MoveItemAfter( igdeTreeItem *item, igdeTreeItem *afterItem ){
	if( ! item || ! afterItem ){
		DETHROW( deeInvalidParam );
	}
	
	const igdeTreeItemReference guard( item );
	pRemoveItem( item );
	pInsertItemAfter( item, afterItem );
	
	OnItemMoved( item );
}

void igdeTreeList::RemoveItem( igdeTreeItem *item ){
	if( ! item ){
		DETHROW( deeInvalidParam );
	}
	
	const igdeTreeItemReference guard( item );
	pRemoveItem( item );
	
	bool selectionChanged = false;
	if( pSelection ){
		igdeTreeItem *check = pSelection;
		while( check ){
			if( check == item ){
				pSelection = NULL;
				selectionChanged = true;
				break;
			}
			check = check->GetParent();
		}
	}
	
	OnItemRemoved( item );
	
	if( selectionChanged ){
		NotifySelectionChanged();
	}
}

void igdeTreeList::RemoveAllItems(){
	if( ! pFirstChild ){
		return;
	}
	
	const bool selectionChanged = pSelection;
	pSelection = NULL;
	
	pRemoveAllItems( NULL );
	
	OnAllItemsRemoved( NULL );
	
	if( selectionChanged ){
		NotifySelectionChanged();
	}
}

void igdeTreeList::RemoveAllItems( igdeTreeItem *parent ){
	if( ! parent ){
		DETHROW( deeInvalidParam );
	}
	
	if( parent->GetChildrenCount() == 0 ){
		return;
	}
	
	bool selectionChanged = false;
	if( pSelection ){
		igdeTreeItem *check = pSelection;
		while( check ){
			if( check->GetParent() == parent ){
				pSelection = NULL;
				selectionChanged = true;
				break;
			}
			check = check->GetParent();
		}
	}
	
	pRemoveAllItems( parent );
	
	OnAllItemsRemoved( parent );
	
	if( selectionChanged ){
		NotifySelectionChanged();
	}
}

void igdeTreeList::ItemChanged( igdeTreeItem *item ){
	if( ! item ){
		DETHROW( deeInvalidParam );
	}
	
	OnItemChanged( item );
}



void igdeTreeList::SetSorter( igdeTreeItemSorter *sorter ){
	pSorter = sorter;
}

void igdeTreeList::SetDefaultSorter(){
	pSorter.TakeOver( new igdeTreeItemSorter );
}

static void igdeTreeList_Sort( decObjectList &items, igdeTreeItemSorter &sorter, int left, int right ){
	igdeTreeItemReference pivot( ( igdeTreeItem* )items.GetAt( left ) );
	const int r_hold = right;
	const int l_hold = left;
	
	while( left < right ){
		while( left < right && sorter.Precedes( pivot, *( ( igdeTreeItem* )items.GetAt( right ) ) ) ){
			right--;
		}
		if( left != right ){
			items.SetAt( left, items.GetAt( right ) );
			left++;
		}
		while( left < right && sorter.Precedes( *( ( igdeTreeItem* )items.GetAt( left ) ), pivot ) ){
			left++;
		}
		if( left != right ){
			items.SetAt( right, items.GetAt( left ) );
			right--;
		}
	}
	
	items.SetAt( left, ( igdeTreeItem* )pivot );
	if( l_hold < left ){
		igdeTreeList_Sort( items, sorter, l_hold, left - 1 );
	}
	if( r_hold > left ){
		igdeTreeList_Sort( items, sorter, left + 1, r_hold );
	}
}

void igdeTreeList::SortItems( igdeTreeItem *item ){
	if( ! pSorter ){
		return;
	}
	
	const int count = item ? item->GetChildrenCount() : GetChildrenCount();
	if( count < 2 ){
		return;
	}
	
	igdeTreeItem *child = item ? item->GetFirstChild() : ( igdeTreeItem* )pFirstChild;
	decObjectList items;
	while( child ){
		items.Add( child );
		child = child->GetNext();
	}
	
	igdeTreeList_Sort( items, pSorter, 0, count - 1 );
	
	igdeTreeItem *prevItem = ( igdeTreeItem* )items.GetAt( 0 );
	prevItem->SetPrevious( NULL );
	
	if( item ){
		item->SetFirstChild( prevItem );
		
	}else{
		pFirstChild = prevItem;
	}
	
	int i;
	for( i=1; i<count; i++ ){
		igdeTreeItem * const nextItem = ( igdeTreeItem* )items.GetAt( i );
		prevItem->SetNext( nextItem );
		nextItem->SetPrevious( prevItem );
		prevItem = nextItem;
	}
	
	prevItem->SetNext( NULL );
	
	OnItemsSorted( item );
}

void igdeTreeList::SortAllItems(){
	if( ! pSorter ){
		return;
	}
	
	SortItems( NULL );
	
	igdeTreeItem *child = pFirstChild;
	while( child ){
		SortItems( child );
		child = child->GetNext();
	}
}



void igdeTreeList::SetSelection( igdeTreeItem *selection ){
	if( selection == pSelection ){
		return;
	}
	
	pSelection = selection;
	
	OnSelectionChanged();
	NotifySelectionChanged();
}

void igdeTreeList::SetSelectionWithData( void *data ){
	SetSelection( pGetItemWithData( NULL, data ) );
}

void igdeTreeList::MakeItemVisible( igdeTreeItem *item ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeFoxTreeList &native = *( ( igdeNativeFoxTreeList* )GetNativeWidget() );
	native.MakeItemVisible( item );
}

void igdeTreeList::MakeSelectionVisible(){
	if( pSelection ){
		MakeItemVisible( pSelection );
	}
}

void igdeTreeList::ShowContextMenu( const decPoint &position ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeMenuCascadeReference menu;
	menu.TakeOver( new igdeMenuCascade( GetEnvironment() ) );
	
	const int count = pListeners.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( igdeTreeListListener* )pListeners.GetAt( i ) )->AddContextMenuEntries( this, menu );
	}
	
	if( menu->GetChildCount() > 0 ){
		menu->Popup( *this, position );
	}
}



void igdeTreeList::AddListener( igdeTreeListListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pListeners.Add( listener );
}

void igdeTreeList::RemoveListener( igdeTreeListListener *listener ){
	pListeners.Remove( listener );
}

void igdeTreeList::NotifySelectionChanged(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeTreeListListener* )listeners.GetAt( i ) )->OnSelectionChanged( this );
	}
}

void igdeTreeList::NotifyItemExpanded( igdeTreeItem *item ){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeTreeListListener* )listeners.GetAt( i ) )->OnItemExpanded( this, item );
	}
}

void igdeTreeList::NotifyItemCollapsed( igdeTreeItem *item ){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeTreeListListener* )listeners.GetAt( i ) )->OnItemCollapsed( this, item );
	}
}

void igdeTreeList::NotifyDoubleClickItem( igdeTreeItem *item ){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeTreeListListener* )listeners.GetAt( i ) )->OnDoubleClickItem( this, item );
	}
}



void igdeTreeList::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	if( ! GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const foxParent = ( FXComposite* )GetParent()->GetNativeContainer();
	if( ! foxParent ){
		DETHROW( deeInvalidParam );
	}
	
	igdeNativeFoxTreeList * const foxWidget = new igdeNativeFoxTreeList( *this, foxParent,
		igdeUIFoxHelper::GetChildLayoutFlagsAll( this ), *GetGuiTheme() );
	SetNativeWidget( foxWidget );
	if( foxParent->id() ){
		foxWidget->create();
	}
}

void igdeTreeList::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	delete ( igdeNativeFoxTreeList* )GetNativeWidget();
	DropNativeWidget();
}



void igdeTreeList::OnItemAdded( igdeTreeItem *item ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeFoxTreeList &native = *( ( igdeNativeFoxTreeList* )GetNativeWidget() );
	native.CreateAndInsertItem( item );
}

void igdeTreeList::OnItemRemoved( igdeTreeItem *item ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeFoxTreeList &native = *( ( igdeNativeFoxTreeList* )GetNativeWidget() );
	native.RemoveItem( item );
}

void igdeTreeList::OnAllItemsRemoved( igdeTreeItem *parent ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeFoxTreeList &native = *( ( igdeNativeFoxTreeList* )GetNativeWidget() );
	native.RemoveAllItems( parent );
}

void igdeTreeList::OnItemChanged( igdeTreeItem *item ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeFoxTreeList &native = *( ( igdeNativeFoxTreeList* )GetNativeWidget() );
	native.UpdateItem( item );
}

void igdeTreeList::OnItemMoved( igdeTreeItem *item ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeFoxTreeList &native = *( ( igdeNativeFoxTreeList* )GetNativeWidget() );
	native.ItemMoved( item );
}

void igdeTreeList::OnItemsSorted( igdeTreeItem *item ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeFoxTreeList &native = *( ( igdeNativeFoxTreeList* )GetNativeWidget() );
	native.ItemsSortedIn( item );
}

void igdeTreeList::OnSelectionChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeFoxTreeList &native = *( ( igdeNativeFoxTreeList* )GetNativeWidget() );
	native.SelectItem( pSelection );
}

void igdeTreeList::OnEnabledChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeFoxTreeList &native = *( ( igdeNativeFoxTreeList* )GetNativeWidget() );
	
	if( pEnabled ){
		native.GetTreeList()->enable();
		
	}else{
		native.GetTreeList()->disable();
	}
}

void igdeTreeList::OnRowsChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeFoxTreeList &native = *( ( igdeNativeFoxTreeList* )GetNativeWidget() );
	native.GetTreeList()->setNumVisible( pRows );
}

void igdeTreeList::OnDescriptionChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeFoxTreeList &native = *( ( igdeNativeFoxTreeList* )GetNativeWidget() );
	//native.GetTreeList()->setTipText( pDescription.GetString() ); // not supported
	native.GetTreeList()->setHelpText( pDescription.GetString() );
}

igdeTreeItem *igdeTreeList::pGetItemWithData( igdeTreeItem *parent, void *data ) const{
	igdeTreeItem *child = parent ? parent->GetFirstChild() : ( igdeTreeItem* )pFirstChild;
	
	while( child ){
		if( child->GetData() == data ){
			return child;
		}
		igdeTreeItem * const found = pGetItemWithData( child, data );
		if( found ){
			return found;
		}
		child = child->GetNext();
	}
	
	return NULL;
}

bool igdeTreeList::pHasItem( igdeTreeItem *parent, igdeTreeItem *item ) const{
	igdeTreeItem *child = parent ? parent->GetFirstChild() : ( igdeTreeItem* )pFirstChild;
	
	while( child ){
		if( child == item || pHasItem( child, item ) ){
			return true;
		}
		child = child->GetNext();
	}
	
	return false;
}

bool igdeTreeList::pHasItem( igdeTreeItem *parent, const char *text ) const{
	igdeTreeItem *child = parent ? parent->GetFirstChild() : ( igdeTreeItem* )pFirstChild;
	
	while( child ){
		if( child->GetText() == text || pHasItem( child, text ) ){
			return true;
		}
		child = child->GetNext();
	}
	
	return false;
}

bool igdeTreeList::pHasItem( igdeTreeItem *parent, void *data ) const{
	igdeTreeItem *child = parent ? parent->GetFirstChild() : ( igdeTreeItem* )pFirstChild;
	
	while( child ){
		if( child->GetData() == data || pHasItem( child, data ) ){
			return true;
		}
		child = child->GetNext();
	}
	
	return false;
}

void igdeTreeList::pRemoveItem( igdeTreeItem *item ){
	item->AddReference();  // guard
	
	if( item->GetPrevious() ){
		item->GetPrevious()->SetNext( item->GetNext() );
		
	}else if( item->GetParent() ){
		item->GetParent()->SetFirstChild( item->GetNext() );
		
	}else{
		pFirstChild = NULL;
	}
	
	if( item->GetNext() ){
		item->GetNext()->SetPrevious( item->GetPrevious() );
	}
	
	item->SetPrevious( NULL );
	item->SetNext( NULL );
	
	item->FreeReference();
}

void igdeTreeList::pAppendItem( igdeTreeItem *parent, igdeTreeItem *item ){
	item->SetParent( parent );
	
	igdeTreeItem * const lastChild = parent ? parent->GetLastChild() : GetLastChild();
	if( lastChild ){
		lastChild->SetNext( item );
		item->SetPrevious( lastChild );
		
	}else if( parent ){
		parent->SetFirstChild( item );
		
	}else{
		pFirstChild = item;
	}
}

void igdeTreeList::pInsertItemBefore( igdeTreeItem *item, igdeTreeItem *beforeItem ){
	item->SetParent( beforeItem->GetParent() );
	
	if( beforeItem->GetPrevious() ){
		beforeItem->GetPrevious()->SetNext( item );
		item->SetPrevious( beforeItem->GetPrevious() );
		
	}else if( beforeItem->GetParent() ){
		beforeItem->GetParent()->SetFirstChild( item );
		
	}else{
		pFirstChild = item;
	}
	
	item->SetNext( beforeItem );
	beforeItem->SetPrevious( item );
}

void igdeTreeList::pInsertItemAfter( igdeTreeItem *item, igdeTreeItem *afterItem ){
	item->SetParent( afterItem->GetParent() );
	
	if( afterItem->GetNext() ){
		afterItem->GetNext()->SetPrevious( item );
		item->SetNext( afterItem->GetNext() );
	}
	
	item->SetPrevious( afterItem );
	afterItem->SetNext( item );
}

void igdeTreeList::pRemoveAllItems( igdeTreeItem *item ){
	igdeTreeItem *child = item ? item->GetLastChild() : GetLastChild();
	
	while( child ){
		igdeTreeItem &clearChild = *child;
		child = child->GetPrevious();
		
		pRemoveAllItems( &clearChild );
		clearChild.SetNext( NULL );
		clearChild.SetPrevious( NULL );
		clearChild.SetParent( NULL );
	}
	
	if( item ){
		item->SetFirstChild( NULL );
		
	}else{
		pFirstChild = NULL;
	}
}
