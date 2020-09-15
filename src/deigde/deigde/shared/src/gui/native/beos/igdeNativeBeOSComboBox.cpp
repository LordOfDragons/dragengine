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

#ifdef IGDE_TOOLKIT_BEOS

#include "igdeNativeBeOSComboBox.h"
#include "../../igdeComboBox.h"
#include "../../igdeContainer.h"
#include "../../igdeCommonDialogs.h"
#include "../../event/igdeAction.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../theme/propertyNames.h"
#include "../../model/igdeListItem.h"
#include "../../resources/igdeIcon.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeNativeBeOSComboBox
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeBeOSComboBox::igdeNativeBeOSComboBox( igdeComboBox &owner, const igdeGuiTheme &guitheme ) :
BButton( owner.GetText(), new BMessage( eeDropDown ) ),
pOwner( &owner ),
pFont( ComboBoxFont( owner, guitheme ) ),
pOrgBackColor( ViewColor() ),
pInvalidBackColor( igdeUIBeOSHelper::BlendColor( pOrgBackColor, rgb_color{ 255, 0, 0, 255 }, 0.25f ) ),
pListView( new BListView( B_SINGLE_SELECTION_LIST ) )
{
	//ComboBoxPadLeft( guitheme ), ComboBoxPadRight( guitheme ),
	//ComboBoxPadTop( guitheme ), ComboBoxPadBottom( guitheme ) ),
	
	pListView->SetInvocationMessage( new BMessage( eeListInvocation ) );
	
	SetFont( ( BFont* )pFont->GetNativeFont() );
	
	//MakeEditable( owner.GetEditable() );
	SetEnabled( owner.GetEnabled() );
	SetToolTip( owner.GetDescription() );
	
	BuildList();
	pListView->Select( owner.GetSelection() );
	SetLabel( owner.GetText() );
	
	UpdateRowCount();
}

igdeNativeBeOSComboBox::~igdeNativeBeOSComboBox(){
	if( pListView ){
		delete pListView;
	}
}

igdeNativeBeOSComboBox *igdeNativeBeOSComboBox::CreateNativeWidget( igdeComboBox &owner ){
	return new igdeNativeBeOSComboBox( owner, *owner.GetGuiTheme() );
}

void igdeNativeBeOSComboBox::PostCreateNativeWidget(){
}

void igdeNativeBeOSComboBox::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeBeOSComboBox::BuildList(){
	const int count = pOwner->GetItemCount();
	int i;
	
	pListView->MakeEmpty();
	
	for( i=0; i<count; i++ ){
		const igdeListItem &item = *pOwner->GetItemAt( i );
		
		pListView->AddItem( new BStringItem( item.GetText() ), pListView->CountItems() );
		
		if( item.GetIcon() ){
			// how?
			// list->setItemIcon( i, ( BBitmap* )item.GetIcon()->GetNativeIcon() );
		}
	}
}

void igdeNativeBeOSComboBox::UpdateItem( int index ){
	const igdeListItem &item = *pOwner->GetItemAt( index );
	
	( ( BStringItem* )pListView->ItemAt( index ) )->SetText( item.GetText() );
	
	/*
	how?
	
	if( item.GetIcon() ){
		list->setItemIcon( index, ( BBitmap* )item.GetIcon()->GetNativeIcon() );
		
	}else{
		list->setItemIcon( index, NULL );
	}
	*/
}

void igdeNativeBeOSComboBox::SyncSelection( bool changing ){
	// keep in mind FXComboBox list selection can go out of synch with text. we need to
	// set the text, then update the selection index to match the text and eventually to
	// update the combo box list selection. since setting the combo box list selection
	// trashes the set text we can not use the simple solution. using SetText() is more
	// complex than required but it does work
	
	// how?
	//pOwner->SetText( getText().text(), changing );
}

void igdeNativeBeOSComboBox::OnInvalidValueChanged(){
	SetViewColor( pOwner->GetInvalidValue() ? pInvalidBackColor : pOrgBackColor );
}

void igdeNativeBeOSComboBox::UpdateText(){
	// documentation states setText() selects the matching list item. this is true if the
	// list item is present but not if it is absent. in this case the old selection is kept
	// causing the text in the combobox and the selected list item to be out of sync.
	// we have to ensure this situation to never happen. the igde combo box widget does set
	// selection to -1 if not matching the text.
	// 
	// unfortunately setCurrentItem() also changes the text although it is not stated in
	// the documentation that this side-effects is present. we thus have to do this in the
	// right order to avoid a total mess to break loose.
	pListView->Select( pOwner->GetSelection() );
	SetLabel( pOwner->GetText() );
}

void igdeNativeBeOSComboBox::InsertItem( int index, const igdeListItem &item ){
	pListView->AddItem( new BStringItem( item.GetText() ), index );
	
	/*
	how?
	
	if( item.GetIcon() ){
		list->setItemIcon( index, ( BBitmap* )item.GetIcon()->GetNativeIcon() );
		
	}else{
		list->setItemIcon( index, NULL );
	}
	*/
}

void igdeNativeBeOSComboBox::RemoveItem( int index ){
	pListView->RemoveItems( index, 1 );
}

void igdeNativeBeOSComboBox::RemoveAllItems(){
	pListView->MakeEmpty();
}

void igdeNativeBeOSComboBox::MoveItem( int fromIndex, int toIndex ){
	pListView->AddItem( pListView->RemoveItem( fromIndex ), toIndex );
}

void igdeNativeBeOSComboBox::Focus(){
	MakeFocus();
}

void igdeNativeBeOSComboBox::UpdateRowCount(){
	/*
	how?
	
	const int count = decMath::max( decMath::min( pOwner->GetRows(), pListView->getNumItems() ), 1 );
	if( count == getNumVisible() ){
		return;
	}
	
	setNumVisible( count );
	
	// FOX needs this otherwise the drop-down list can stay at a wrong size
	layout();
	*/
}

void igdeNativeBeOSComboBox::UpdateEnabled(){
	SetEnabled( pOwner->GetEnabled() );
}

void igdeNativeBeOSComboBox::UpdateEditable(){
	//MakeEditable( pOwner->GetEditable() );
}

void igdeNativeBeOSComboBox::UpdateDescription(){
	SetToolTip( pOwner->GetDescription() );
}



void igdeNativeBeOSComboBox::MessageReceived( BMessage *message ){
	if( message->what == eeTextChanged ){
		if( ! pOwner->GetEnabled() ){
			return;
		}
		
		try{
			SyncSelection( false );
			
			// SyncSelection calls SetText which does call NotifyTextChanged but only if the
			// text has not changed. Because onChanged is send right before onCommand the
			// text is already set and SyncSelection will not cause a notification to be send.
			// for this reason we have to do it manually here since onCommand will be only
			// send if user interaction changed the value.
			pOwner->NotifyTextChanged();
			
		}catch( const deException &e ){
			pOwner->GetLogger()->LogException( "IGDE", e );
			igdeCommonDialogs::Exception( pOwner, e );
			return;
		}
		
	}else if( message->what == eeDropDown ){
		if( ! pOwner->GetEnabled() ){
			return;
		}
		
		try{
			// show popup menu with list...
			
		}catch( const deException &e ){
			pOwner->GetLogger()->LogException( "IGDE", e );
			igdeCommonDialogs::Exception( pOwner, e );
		}
		
	}else if( message->what == eeListInvocation ){
		if( ! pOwner->GetEnabled() ){
			return;
		}
		
		try{
			SyncSelection( true );
			
		}catch( const deException &e ){
			pOwner->GetLogger()->LogException( "IGDE", e );
			igdeCommonDialogs::Exception( pOwner, e );
			return;
		}
		
	}else{
		BButton::MessageReceived( message );
	}
}



igdeFont *igdeNativeBeOSComboBox::ComboBoxFont( const igdeComboBox &owner, const igdeGuiTheme &guitheme ){
	igdeFont::sConfiguration configuration;
	owner.GetEnvironment().GetApplicationFont( configuration );
	
	if( guitheme.HasProperty( igdeGuiThemePropertyNames::comboBoxFontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::comboBoxFontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::comboBoxFontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::comboBoxFontSize, 1.0f );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f );
	}
	
	return owner.GetEnvironment().GetSharedFont( configuration );
}

int igdeNativeBeOSComboBox::ComboBoxPadLeft( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::comboBoxPaddingLeft, 0 );
}

int igdeNativeBeOSComboBox::ComboBoxPadRight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::comboBoxPaddingRight, 0 );
}

int igdeNativeBeOSComboBox::ComboBoxPadTop( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::comboBoxPaddingTop, 0 );
}

int igdeNativeBeOSComboBox::ComboBoxPadBottom( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::comboBoxPaddingBottom, 0 );
}

#endif
