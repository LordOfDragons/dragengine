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

#include "igdeComboBoxFilter.h"
#include "igdeContainer.h"
#include "native/toolkit.h"
#include "igdeCommonDialogs.h"
#include "event/igdeComboBoxListener.h"
#include "model/igdeListItem.h"
#include "model/igdeListItemReference.h"
#include "model/igdeListItemSorter.h"
#include "resources/igdeIcon.h"
#include "resources/igdeFont.h"
#include "resources/igdeFontReference.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>


// Class igdeComboBoxFilter
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeComboBoxFilter::igdeComboBoxFilter( igdeEnvironment &environment, int columns,
	const char *description ) :
igdeComboBox( environment, columns, description ),
pFilterCaseInsensitive( true ){
}

igdeComboBoxFilter::igdeComboBoxFilter( igdeEnvironment &environment, int columns, bool editable,
	const char *description ) :
igdeComboBox( environment, columns, editable, description ),
pFilterCaseInsensitive( true ){
}

igdeComboBoxFilter::igdeComboBoxFilter( igdeEnvironment &environment, int columns, int rows,
	const char *description ) :
igdeComboBox( environment, columns, rows, description ),
pFilterCaseInsensitive( true ){
}

igdeComboBoxFilter::igdeComboBoxFilter( igdeEnvironment &environment, int columns, int rows,
	bool editable, const char *description ) :
igdeComboBox( environment, columns, rows, editable, description ),
pFilterCaseInsensitive( true ){
}

igdeComboBoxFilter::~igdeComboBoxFilter(){
}



// Management
///////////////

void igdeComboBoxFilter::StoreFilterItems(){
	const int count = GetItemCount();
	int i;
	
	pFilterItems.RemoveAll();
	
	for( i=0; i<count; i++ ){
		pFilterItems.Add( GetItemAt( i ) );
	}
	
	FilterItems();
}

void igdeComboBoxFilter::ReleaseFilterItems(){
	pFilterItems.RemoveAll();
}

void igdeComboBoxFilter::FilterItems(){
	const bool noFilter = pFilterString.IsEmpty();
	const int count = pFilterItems.GetCount();
	const decString text( GetText() );
	int i;
	
	RemoveAllItems();
	
	if( pFilterCaseInsensitive ){
		const decString filter( pFilterString.GetLower() );
		
		for( i=0; i<count; i++ ){
			igdeListItem * const item = ( igdeListItem* )pFilterItems.GetAt( i );
			
			if( noFilter || item->GetText().GetLower().FindString( filter ) != -1 ){
				AddItem( item );
			}
		}
		
	}else{
		for( i=0; i<count; i++ ){
			igdeListItem * const item = ( igdeListItem* )pFilterItems.GetAt( i );
			
			if( noFilter || item->GetText().FindString( pFilterString ) != -1 ){
				AddItem( item );
			}
		}
	}
	
	SetText( text );
}

void igdeComboBoxFilter::SetFilterCaseInsentive( bool caseInsensitive ){
	if( caseInsensitive == pFilterCaseInsensitive ){
		return;
	}
	
	pFilterCaseInsensitive = caseInsensitive;
	FilterItems();
}

void igdeComboBoxFilter::SetFilterString( const char *filterString ){
	if( pFilterString == filterString ){
		return;
	}
	
	pFilterString = filterString;
	OnFilterStringChanged();
	
	FilterItems();
	
	// if the current selection is filtered out use the first one if available
	if( GetSelection() == -1 && GetItemCount() > 0 && ! GetEditable() ){
		SetSelection( 0 );
	}
}

int igdeComboBoxFilter::GetFilterItemCount() const{
	return pFilterItems.GetCount();
}

igdeListItem *igdeComboBoxFilter::GetFilterItemAt( int index ) const{
	return ( igdeListItem* )pFilterItems.GetAt( index );
}



void igdeComboBoxFilter::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	igdeNativeComboBoxFilter * const native = igdeNativeComboBoxFilter::CreateNativeWidget( *this );
	SetNativeWidget( native );
	native->PostCreateNativeWidget();
}

void igdeComboBoxFilter::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeComboBoxFilter* )GetNativeWidget() )->DestroyNativeWidget();
	DropNativeWidget();
}



void igdeComboBoxFilter::OnItemAdded( int index ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeComboBoxFilter &native = *( ( igdeNativeComboBoxFilter* )GetNativeWidget() );
	native.InsertItem( index, *GetItemAt( index ) );
	//native.SyncSelection( false );
	native.UpdateText();
	native.UpdateRowCount();
}

void igdeComboBoxFilter::OnItemRemoved( int index ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeComboBoxFilter &native = *( ( igdeNativeComboBoxFilter* )GetNativeWidget() );
	native.RemoveItem( index );
	//native.SyncSelection( false );
	native.UpdateText();
	native.UpdateRowCount();
}

void igdeComboBoxFilter::OnAllItemsRemoved(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeComboBoxFilter &native = *( ( igdeNativeComboBoxFilter* )GetNativeWidget() );
	native.RemoveAllItems();
	//native.SyncSelection( false );
	native.UpdateText();
	native.UpdateRowCount();
}

void igdeComboBoxFilter::OnItemChanged( int index ){
	if( GetNativeWidget() ){
		( ( igdeNativeComboBoxFilter* )GetNativeWidget() )->UpdateItem( index );
	}
}

void igdeComboBoxFilter::OnItemMoved( int fromIndex, int toIndex ){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeComboBoxFilter &native = *( ( igdeNativeComboBoxFilter* )GetNativeWidget() );
	native.MoveItem( fromIndex, toIndex );
	//native.SyncSelection( false );
	native.UpdateText();
}

void igdeComboBoxFilter::OnItemsSorted(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeComboBoxFilter &native = *( ( igdeNativeComboBoxFilter* )GetNativeWidget() );
	native.BuildList();
	//native.SyncSelection( false );
	native.UpdateText();
}

void igdeComboBoxFilter::OnTextChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeComboBoxFilter* )GetNativeWidget() )->UpdateText();
	}
}

void igdeComboBoxFilter::OnEnabledChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeComboBoxFilter* )GetNativeWidget() )->UpdateEnabled();
}

void igdeComboBoxFilter::OnRowsChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeComboBoxFilter* )GetNativeWidget() )->UpdateRowCount();
	}
}

void igdeComboBoxFilter::OnEditableChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeComboBoxFilter* )GetNativeWidget() )->UpdateEditable();
	}
}

void igdeComboBoxFilter::OnDescriptionChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeComboBoxFilter* )GetNativeWidget() )->UpdateDescription();
	}
}

void igdeComboBoxFilter::OnFilterStringChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeComboBoxFilter* )GetNativeWidget() )->UpdateFilterString();
	}
}

void igdeComboBoxFilter::OnInvalidValueChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeComboBoxFilter* )GetNativeWidget() )->OnInvalidValueChanged();
	}
}

void igdeComboBoxFilter::OnRequestFocus(){
	if( GetNativeWidget() ){
		( ( igdeNativeComboBoxFilter* )GetNativeWidget() )->Focus();
	}
}
