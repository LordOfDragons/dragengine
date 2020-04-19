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

#include "igdeToggleTags.h"

#include "../igdeCommonDialogs.h"
#include "../igdeListBox.h"
#include "../igdeUIHelper.h"
#include "../igdeContainerReference.h"
#include "../event/igdeAction.h"
#include "../event/igdeListBoxListener.h"
#include "../event/igdeListBoxListenerReference.h"
#include "../menu/igdeMenuCascade.h"
#include "../model/igdeListItem.h"
#include "../../environment/igdeEnvironment.h"
#include "../../undo/igdeUndo.h"
#include "../../undo/igdeUndoReference.h"
#include "../../undo/igdeUndoSystem.h"

#include <dragengine/deEngine.h>
#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/logger/deLogger.h>



// Definitions
////////////////

#define TAG_ENABLED ((void*)1)
#define TAG_DISABLED ((void*)0)


// Actions
////////////

igdeToggleTags::cActionEnableAll::cActionEnableAll( igdeToggleTags &widget ) :
igdeAction( "Enable All", widget.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ),
	"Enable all tags" ),
pWidget( widget ){
}

void igdeToggleTags::cActionEnableAll::OnAction(){
	pWidget.EnableAllTags();
}


igdeToggleTags::cActionDisableAll::cActionDisableAll( igdeToggleTags &widget ) :
igdeAction( "Disable All", widget.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ),
	"Disable all tags" ),
pWidget( widget ){
}

void igdeToggleTags::cActionDisableAll::OnAction(){
	pWidget.DisableAllTags();
}


class igdeToggleTags_ListTags : public igdeListBoxListener {
	igdeToggleTags &pWidget;
	
public:
	igdeToggleTags_ListTags( igdeToggleTags &widget ) : pWidget( widget ){
	}
	
	virtual void OnDoubleClickItem( igdeListBox *listBox, int index ){
		pWidget.ToggleTagEnabled( index );
	}
	
	virtual void AddContextMenuEntries( igdeListBox*, igdeMenuCascade &menu ){
		igdeUIHelper &helper = pWidget.GetEnvironment().GetUIHelper();
		helper.MenuCommand( menu, pWidget.GetActionEnableAll() );
		helper.MenuCommand( menu, pWidget.GetActionDisableAll() );
	}
};



// Class igdeToggleTags
///////////////////////

// Constructor, destructor
////////////////////////////

igdeToggleTags::igdeToggleTags( igdeUIHelper &helper, int rows ) :
igdeContainerFlow( helper.GetEnvironment(), igdeContainerFlow::eaY, igdeContainerFlow::esNone )
{
	pCreateContent( helper, rows );
}

igdeToggleTags::igdeToggleTags( igdeUIHelper &helper, int rows, igdeAction *action ) :
igdeContainerFlow( helper.GetEnvironment(), igdeContainerFlow::eaY, igdeContainerFlow::esNone )
{
	SetAction( action );
	pCreateContent( helper, rows );
}

igdeToggleTags::~igdeToggleTags(){
	DestroyNativeWidget();
	SetAction( NULL );
}



// Management
///////////////

void igdeToggleTags::SetIconEnabled( igdeIcon *icon ){
	if( pIconEnabled == icon ){
		return;
	}
	pIconEnabled = icon;
	pUpdateAllIcons();
}

void igdeToggleTags::SetIconDisabled( igdeIcon *icon ){
	if( pIconDisabled == icon ){
		return;
	}
	pIconDisabled = icon;
	pUpdateAllIcons();
}

void igdeToggleTags::Focus(){
	pListBox->Focus();
}

decStringSet igdeToggleTags::GetTags() const{
	const int count = pListBox->GetItemCount();
	decStringSet set;
	int i;
	
	for( i=0; i<count; i++ ){
		set.Add( pListBox->GetItemAt( i )->GetText() );
	}
	
	return set;
}

void igdeToggleTags::SetTags( const decStringSet &tags ){
	const decString selection( pListBox->GetSelectedItem() ? pListBox->GetSelectedItem()->GetText() : "" );
	const int count = tags.GetCount();
	int i;
	
	pListBox->RemoveAllItems();
	for( i=0; i<count; i++ ){
		pListBox->AddItem( tags.GetAt( i ), pIconDisabled, TAG_DISABLED );
	}
	pListBox->SortItems();
}

void igdeToggleTags::Clear(){
	pListBox->RemoveAllItems();
}

decStringSet igdeToggleTags::GetEnabledTags() const{
	const int count = pListBox->GetItemCount();
	decStringSet set;
	int i;
	
	for( i=0; i<count; i++ ){
		if( pListBox->GetItemAt( i )->GetData() == TAG_ENABLED ){
			set.Add( pListBox->GetItemAt( i )->GetText() );
		}
	}
	
	return set;
}

decStringSet igdeToggleTags::GetDisabledTags() const{
	const int count = pListBox->GetItemCount();
	decStringSet set;
	int i;
	
	for( i=0; i<count; i++ ){
		if( pListBox->GetItemAt( i )->GetData() == TAG_DISABLED ){
			set.Add( pListBox->GetItemAt( i )->GetText() );
		}
	}
	
	return set;
}

const decString &igdeToggleTags::GetSelectedTag() const{
	if( pListBox->GetSelectedItem() != NULL ){
		return pListBox->GetSelectedItem()->GetText();
		
	}else{
		static const decString noselection;
		return noselection;
	}
}

void igdeToggleTags::SelectTag( const char *tag ){
	pListBox->SetSelection( pListBox->IndexOfItem( tag ) );
}

void igdeToggleTags::EnableAllTags(){
	const int count = pListBox->GetItemCount();
	bool changed = false;
	int i;
	
	for( i=0; i<count; i++ ){
		igdeListItem &item = *pListBox->GetItemAt( i );
		if( item.GetData() == TAG_ENABLED ){
			continue;
		}
		
		item.SetData( TAG_ENABLED );
		pUpdateIcon( i );
		changed = true;
	}
	
	if( changed ){
		OnAction();
	}
}

void igdeToggleTags::DisableAllTags(){
	const int count = pListBox->GetItemCount();
	bool changed = false;
	int i;
	
	for( i=0; i<count; i++ ){
		igdeListItem &item = *pListBox->GetItemAt( i );
		if( item.GetData() == TAG_DISABLED ){
			continue;
		}
		
		item.SetData( TAG_DISABLED );
		pUpdateIcon( i );
		changed = true;
	}
	
	if( changed ){
		OnAction();
	}
}

void igdeToggleTags::EnableTags( const decStringSet &set ){
	const int count = pListBox->GetItemCount();
	bool changed = false;
	int i;
	
	for( i=0; i<count; i++ ){
		igdeListItem &item = *pListBox->GetItemAt( i );
		const bool inSet = set.Has( item.GetText() );
		if( inSet == ( item.GetData() == TAG_ENABLED ) ){
			continue;
		}
		
		item.SetData( inSet ? TAG_ENABLED : TAG_DISABLED );
		pUpdateIcon( i );
		changed = true;
	}
	
	if( changed ){
		OnAction();
	}
}

bool igdeToggleTags::GetTagEnabled( const char *tag ) const{
	return pListBox->GetItemAt( pListBox->IndexOfItem( tag ) )->GetData() == TAG_ENABLED;
}

void igdeToggleTags::SetTagEnabled( const char *tag, bool enabled ){
	const int index = pListBox->IndexOfItem( tag );
	igdeListItem &item = *pListBox->GetItemAt( index );
	if( enabled == ( item.GetData() == TAG_ENABLED ) ){
		return;
	}
	
	item.SetData( enabled ? TAG_ENABLED : TAG_DISABLED );
	pUpdateIcon( index );
	OnAction();
}

void igdeToggleTags::ToggleTagEnabled( const char *tag ){
	ToggleTagEnabled( pListBox->IndexOfItem( tag ) );
}



void igdeToggleTags::SetAction( igdeAction *action ){
	if( pAction == action ){
		return;
	}
	
	if( pAction ){
		pAction->RemoveListener( this );
	}
	
	pAction = action;
	
	if( action ){
		action->AddListener( this );
		OnParameterChanged( action );
	}
}

void igdeToggleTags::OnAction(){
	if( pAction ){
		pAction->OnAction();
	}
}

void igdeToggleTags::OnParameterChanged( igdeAction *action ){
	if( pListBox ){
		pListBox->SetDescription( action->GetDescription() );
	}
}

void igdeToggleTags::OnDestroyed( igdeAction *action ){
	GetLogger()->LogWarnFormat( "IGDE", "igdeToggleTags::OnDestroyed: "
		"Action(%s) destroyed while still listening on it", action->GetText().GetString() );
	pAction = NULL;
}



void igdeToggleTags::ToggleTagEnabled( int index ){
	igdeListItem &item = *pListBox->GetItemAt( index );
	item.SetData( item.GetData() == TAG_ENABLED ? TAG_DISABLED : TAG_ENABLED );
	pUpdateIcon( index );
	OnAction();
}



// Private Functions
//////////////////////

void igdeToggleTags::pCreateContent( igdeUIHelper &helper, int rows ){
	pIconEnabled = helper.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallPlus );
	pIconDisabled = helper.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallMinus );
	
	pActionEnableAll.TakeOver( new cActionEnableAll( *this ) );
	pActionDisableAll.TakeOver( new cActionDisableAll( *this ) );
	
	helper.ListBox( *this, rows, "", pListBox, new igdeToggleTags_ListTags( *this ) );
	pListBox->SetDefaultSorter();
}

void igdeToggleTags::pUpdateAllIcons(){
	const int count = pListBox->GetItemCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pUpdateIcon( i );
	}
}

void igdeToggleTags::pUpdateIcon( int index ){
	igdeListItem &item = *pListBox->GetItemAt( index );
	
	igdeIcon * const icon = item.GetData() == TAG_ENABLED ? pIconEnabled : pIconDisabled;
	if( icon == item.GetIcon() ){
		return;
	}
	
	item.SetIcon( icon );
	pListBox->ItemChangedAt( index );
}
