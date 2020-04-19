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

#include "igdeListBox.h"
#include "igdeContainer.h"
#include "igdeCommonDialogs.h"
#include "event/igdeListBoxListener.h"
#include "menu/igdeMenuCascade.h"
#include "menu/igdeMenuCascadeReference.h"
#include "model/igdeListItem.h"
#include "model/igdeListItemReference.h"
#include "model/igdeListItemSorter.h"
#include "native/fox/igdeNativeFoxListBox.h"
#include "resources/igdeIcon.h"
#include "resources/igdeFont.h"
#include "resources/igdeFontReference.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeListBox
//////////////////////

// Constructor, destructor
////////////////////////////

igdeListBox::igdeListBox( igdeEnvironment &environment, int rows, const char *description ) :
igdeWidget( environment ),
pEnabled( true ),
pSelectionMode( esmSingle ),
pSelection( -1 ),
pRows( rows ),
pDescription( description )
{
	if( rows < 1 ){
		DETHROW( deeInvalidParam );
	}
}

igdeListBox::~igdeListBox(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeListBox::SetEnabled( bool enabled ){
	if( pEnabled == enabled ){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeListBox::SetRows( int rows ){
	if( rows < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( rows == pRows ){
		return;
	}
	
	pRows = rows;
	OnRowsChanged();
}

void igdeListBox::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeListBox::Focus(){
	if( GetNativeWidget() ){
		( ( igdeNativeFoxListBox* )GetNativeWidget() )->Focus();
	}
}



int igdeListBox::GetItemCount() const{
	return pItems.GetCount();
}

igdeListItem *igdeListBox::GetItemAt( int index ) const{
	return ( igdeListItem* )pItems.GetAt( index );
}

igdeListItem *igdeListBox::GetItemWithData( void *data ) const{
	const int count = pItems.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		igdeListItem * const item = ( igdeListItem* )pItems.GetAt( i );
		if( item->GetData() == data ){
			return item;
		}
	}
	
	return NULL;
}

bool igdeListBox::HasItem( igdeListItem *item ) const{
	return pItems.Has( item );
}

bool igdeListBox::HasItem( const char *item ) const{
	return IndexOfItem( item ) != -1;
}

bool igdeListBox::HasItemWithData( void *data ) const{
	return IndexOfItemWithData( data ) != -1;
}

int igdeListBox::IndexOfItem( igdeListItem *item ) const{
	return pItems.IndexOf( item );
}

int igdeListBox::IndexOfItem( const char *item ) const{
	if( ! item ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pItems.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( const igdeListItem* )pItems.GetAt( i ) )->GetText() == item ){
			return i;
		}
	}
	
	return -1;
}

int igdeListBox::IndexOfItemWithData( void *data ) const{
	const int count = pItems.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( const igdeListItem* )pItems.GetAt( i ) )->GetData() == data ){
			return i;
		}
	}
	
	return -1;
}

void igdeListBox::AddItem( igdeListItem *item ){
	if( ! item ){
		DETHROW( deeInvalidParam );
	}
	pItems.Add( item );
	OnItemAdded( pItems.GetCount() - 1 );
	
	if( pItems.GetCount() == 1 ){
		SetSelection( 0 );
	}
}

void igdeListBox::AddItem( const char *text, igdeIcon *icon, void *data ){
	igdeListItemReference item;
	AddItem( item, text, icon, data );
}

void igdeListBox::AddItem( igdeListItemReference &item, const char *text, igdeIcon *icon, void *data ){
	item.TakeOver( new igdeListItem( text, icon, data ) );
	AddItem( item );
}

void igdeListBox::InsertItem( int index, igdeListItem *item ){
	if( ! item ){
		DETHROW( deeInvalidParam );
	}
	
	pItems.Insert( item, index );
	if( pSelection >= index ){
		pSelection++;
	}
	
	OnItemAdded( index );
	
	if( pItems.GetCount() == 1 ){
		SetSelection( 0 );
	}
}

void igdeListBox::InsertItem( int index, const char *text, igdeIcon *icon, void *data ){
	igdeListItemReference item;
	InsertItem( item, index, text, icon, data );
}

void igdeListBox::InsertItem( igdeListItemReference &item, int index, const char *text,
igdeIcon *icon, void *data ){
	item.TakeOver( new igdeListItem( text, icon, data ) );
	InsertItem( index, item );
}

void igdeListBox::MoveItem( int fromIndex, int toIndex ){
	pItems.Move( fromIndex, toIndex );
	
	if( pSelection != -1 ){
		if( fromIndex < toIndex ){
			if( pSelection == fromIndex ){
				pSelection = toIndex;
				
			}else if( pSelection > fromIndex && pSelection <= toIndex ){
				pSelection--;
			}
			
		}else{
			if( pSelection == fromIndex ){
				pSelection = toIndex;
				
			}else if( pSelection >= toIndex && pSelection < fromIndex ){
				pSelection++;
			}
		}
	}
	
	OnItemMoved( fromIndex, toIndex );
}

void igdeListBox::RemoveItem( int index ){
	pItems.RemoveFrom( index );
	
	bool notifySelectionChanged = false;
	
	if( pSelection == index ){
		pSelection = decMath::min( pSelection, pItems.GetCount() - 1 );
		notifySelectionChanged = true;
		
	}else if( pSelection > index ){
		pSelection--;
	}
	
	OnItemRemoved( index );
	
	if( notifySelectionChanged ){
		NotifySelectionChanged();
	}
}

void igdeListBox::RemoveAllItems(){
	if( pItems.GetCount() == 0 ){
		return;
	}
	
	pItems.RemoveAll();
	pSelection = -1;
	OnAllItemsRemoved();
	
	NotifySelectionChanged();
}

void igdeListBox::ItemChangedAt( int index ){
	if( index < 0 || index >= pItems.GetCount() ){
		DETHROW( deeInvalidParam );
	}
	OnItemChanged( index );
}



void igdeListBox::SetSorter( igdeListItemSorter *sorter ){
	pSorter = sorter;
}

void igdeListBox::SetDefaultSorter(){
	pSorter.TakeOver( new igdeListItemSorter );
}

static void igdeListBox_Sort( decObjectList &items, igdeListItemSorter &sorter, int left, int right ){
	igdeListItemReference pivot( ( igdeListItem* )items.GetAt( left ) );
	const int r_hold = right;
	const int l_hold = left;
	
	while( left < right ){
		while( left < right && sorter.Precedes( pivot, *( ( igdeListItem* )items.GetAt( right ) ) ) ){
			right--;
		}
		if( left != right ){
			items.SetAt( left, items.GetAt( right ) );
			left++;
		}
		while( left < right && sorter.Precedes( *( ( igdeListItem* )items.GetAt( left ) ), pivot ) ){
			left++;
		}
		if( left != right ){
			items.SetAt( right, items.GetAt( left ) );
			right--;
		}
	}
	
	items.SetAt( left, ( igdeListItem* )pivot );
	if( l_hold < left ){
		igdeListBox_Sort( items, sorter, l_hold, left - 1 );
	}
	if( r_hold > left ){
		igdeListBox_Sort( items, sorter, left + 1, r_hold );
	}
}

void igdeListBox::SortItems(){
	if( ! pSorter ){
		return;
	}
	
	const int count = pItems.GetCount();
	if( count < 2 ){
		return;
	}
	
	igdeListItem * const selected = GetSelectedItem();
	
	igdeListBox_Sort( pItems, pSorter, 0, count - 1 );
	
	if( selected ){
		pSelection = pItems.IndexOf( selected );
	}
	
	OnItemsSorted();
}



void igdeListBox::SetSelectionMode( eSelectionMode mode ){
	if( mode == pSelectionMode ){
		return;
	}
	
	pSelectionMode = mode;
	OnSelectionModeChanged();
}

igdeListItem *igdeListBox::GetSelectedItem() const{
	if( pSelection != -1 ){
		return ( igdeListItem* )pItems.GetAt( pSelection );
		
	}else{
		return NULL;
	}
}

void igdeListBox::SetSelection( int selection ){
	if( selection < -1 || selection >= pItems.GetCount() ){
		DETHROW( deeInvalidParam );
	}
	
	if( selection == pSelection ){
		return;
	}
	
	pSelection = selection;
	OnSelectionChanged();
	NotifySelectionChanged();
}

void igdeListBox::SetSelectionWithData( void *data ){
	SetSelection( IndexOfItemWithData( data ) );
}

void igdeListBox::SelectItem( int index ){
	if( pSelectionMode != esmMultiple ){
		DETHROW( deeInvalidParam );
	}
	if( index < -1 || index >= pItems.GetCount() ){
		DETHROW( deeInvalidParam );
	}
	
	igdeListItem &item = *( ( igdeListItem* )pItems.GetAt( index ) );
	if( item.GetSelected() ){
		return;
	}
	
	item.SetSelected( true );
	OnSelectionChanged();
}

void igdeListBox::SelectAllItems(){
	if( pSelectionMode != esmMultiple ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pItems.GetCount();
	bool changed = false;
	int i;
	
	for( i=0; i<count; i++ ){
		igdeListItem &item = *( ( igdeListItem* )pItems.GetAt( i ) );
		if( item.GetSelected() ){
			continue;
		}
		
		item.SetSelected( true );
		changed = true;
	}
	
	if( changed ){
		OnSelectionChanged();
	}
}

void igdeListBox::DeselectItem( int index ){
	if( pSelectionMode != esmMultiple ){
		DETHROW( deeInvalidParam );
	}
	if( index < -1 || index >= pItems.GetCount() ){
		DETHROW( deeInvalidParam );
	}
	
	igdeListItem &item = *( ( igdeListItem* )pItems.GetAt( index ) );
	if( ! item.GetSelected() ){
		return;
	}
	
	item.SetSelected( false );
	OnSelectionChanged();
}

void igdeListBox::DeselectAllItems(){
	if( pSelectionMode != esmMultiple ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pItems.GetCount();
	bool changed = false;
	int i;
	
	for( i=0; i<count; i++ ){
		igdeListItem &item = *( ( igdeListItem* )pItems.GetAt( i ) );
		if( ! item.GetSelected() ){
			continue;
		}
		
		item.SetSelected( false );
		changed = true;
	}
	
	if( changed ){
		OnSelectionChanged();
	}
}

void igdeListBox::MakeItemVisible( int index ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeFoxListBox &native = *( ( igdeNativeFoxListBox* )GetNativeWidget() );
	native.GetListBox()->makeItemVisible( index );
}

void igdeListBox::MakeSelectionVisible(){
	if( pSelection != -1 ){
		MakeItemVisible( pSelection );
	}
}

void igdeListBox::ShowContextMenu( const decPoint &position ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeMenuCascadeReference menu;
	menu.TakeOver( new igdeMenuCascade( GetEnvironment() ) );
	
	const int count = pListeners.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( igdeListBoxListener* )pListeners.GetAt( i ) )->AddContextMenuEntries( this, menu );
	}
	
	if( menu->GetChildCount() > 0 ){
		menu->Popup( *this, position );
	}
}



void igdeListBox::AddListener( igdeListBoxListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pListeners.Add( listener );
}

void igdeListBox::RemoveListener( igdeListBoxListener *listener ){
	pListeners.Remove( listener );
}

void igdeListBox::NotifySelectionChanged(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeListBoxListener* )listeners.GetAt( i ) )->OnSelectionChanged( this );
	}
}

void igdeListBox::NotifyItemSelected( int index ){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeListBoxListener* )listeners.GetAt( i ) )->OnItemSelected( this, index );
	}
}

void igdeListBox::NotifyItemDeselected( int index ){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeListBoxListener* )listeners.GetAt( i ) )->OnItemDeselected( this, index );
	}
}

void igdeListBox::NotifyDoubleClickItem( int index ){
	if( index < 0 || index >= pItems.GetCount() ){
		DETHROW( deeInvalidParam );
	}
	
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeListBoxListener* )listeners.GetAt( i ) )->OnDoubleClickItem( this, index );
	}
}



void igdeListBox::CreateNativeWidget(){
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
	
	igdeNativeFoxListBox * const native = new igdeNativeFoxListBox( *this, foxParent,
		igdeUIFoxHelper::GetChildLayoutFlagsAll( this ), *GetGuiTheme() );
	SetNativeWidget( native );
	if( foxParent->id() ){
		native->create();
	}
}

void igdeListBox::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	delete ( igdeNativeFoxListBox* )GetNativeWidget();
	DropNativeWidget();
}



void igdeListBox::OnItemAdded( int index ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	const igdeListItem &item = *( ( igdeListItem* )pItems.GetAt( index ) );
	igdeNativeFoxListBox &native = *( ( igdeNativeFoxListBox* )GetNativeWidget() );
	native.GetListBox()->insertItem( index, item.GetText().GetString(),
		item.GetIcon() ? ( FXIcon* )item.GetIcon()->GetNativeIcon() : NULL );
}

void igdeListBox::OnItemRemoved( int index ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeFoxListBox &native = *( ( igdeNativeFoxListBox* )GetNativeWidget() );
	native.GetListBox()->removeItem( index );
}

void igdeListBox::OnAllItemsRemoved(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeFoxListBox &native = *( ( igdeNativeFoxListBox* )GetNativeWidget() );
	native.GetListBox()->clearItems();
}

void igdeListBox::OnItemChanged( int index ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeFoxListBox &native = *( ( igdeNativeFoxListBox* )GetNativeWidget() );
	native.UpdateItem( index );
}

void igdeListBox::OnItemMoved( int fromIndex, int toIndex ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeFoxListBox &native = *( ( igdeNativeFoxListBox* )GetNativeWidget() );
	native.GetListBox()->moveItem( toIndex, fromIndex );
}

void igdeListBox::OnItemsSorted(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeFoxListBox &native = *( ( igdeNativeFoxListBox* )GetNativeWidget() );
	native.BuildList();
}

void igdeListBox::OnSelectionChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeFoxListBox &native = *( ( igdeNativeFoxListBox* )GetNativeWidget() );
	native.UpdateSelection();
}

void igdeListBox::OnSelectionModeChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeFoxListBox* )GetNativeWidget() )->UpdateStyles();
	}
}

void igdeListBox::OnEnabledChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeFoxListBox &native = *( ( igdeNativeFoxListBox* )GetNativeWidget() );
	
	if( pEnabled ){
		native.GetListBox()->enable();
		
	}else{
		native.GetListBox()->disable();
	}
}

void igdeListBox::OnRowsChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeFoxListBox &native = *( ( igdeNativeFoxListBox* )GetNativeWidget() );
	native.GetListBox()->setNumVisible( pRows );
}

void igdeListBox::OnDescriptionChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeFoxListBox &native = *( ( igdeNativeFoxListBox* )GetNativeWidget() );
	//native.GetListBox()->setTipText( pDescription.GetString() ); // not supported
	native.GetListBox()->setHelpText( pDescription.GetString() );
}
