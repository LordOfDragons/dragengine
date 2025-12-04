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

#include "igdeComboBoxFilter.h"
#include "igdeContainer.h"
#include "native/toolkit.h"
#include "igdeCommonDialogs.h"
#include "event/igdeComboBoxListener.h"
#include "model/igdeListItem.h"
#include "model/igdeListItem.h"
#include "model/igdeListItemSorter.h"
#include "resources/igdeIcon.h"
#include "resources/igdeFont.h"
#include "resources/igdeFont.h"
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
