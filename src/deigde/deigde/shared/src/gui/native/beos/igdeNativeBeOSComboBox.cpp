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
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeBeOSComboBox::igdeNativeBeOSComboBox( igdeComboBox &owner, const igdeGuiTheme &guitheme ) :
BMenuField( "", new BPopUpMenu( "popup" ) ),
pOwner( owner ),
pFont( ComboBoxFont( owner, guitheme ) ),
pOrgBackColor( ViewColor() ),
pInvalidBackColor( igdeUIBeOSHelper::BlendColor( pOrgBackColor, rgb_color{ 255, 0, 0, 255 }, 0.25f ) ),
pMenu( *( ( BPopUpMenu* )Menu() ) ),
pTopItemIndex( 0 ),
pCustomItem( NULL )
{
	//ComboBoxPadLeft( guitheme ), ComboBoxPadRight( guitheme ),
	//ComboBoxPadTop( guitheme ), ComboBoxPadBottom( guitheme ) ),
	
	SetFont( ( BFont* )pFont->GetNativeFont() );
	
	//MakeEditable( owner.GetEditable() );
	SetEnabled( owner.GetEnabled() );
	SetToolTip( owner.GetDescription() );
	
	BuildList();
	
	UpdateRowCount();
}

igdeNativeBeOSComboBox::~igdeNativeBeOSComboBox(){
}

igdeNativeBeOSComboBox *igdeNativeBeOSComboBox::CreateNativeWidget( igdeComboBox &owner ){
	igdeNativeBeOSComboBox * const native = new igdeNativeBeOSComboBox( owner, *owner.GetGuiTheme() );
	igdeUIBeOSHelper::AddView( native, owner.GetParent() );
	return native;
}

void igdeNativeBeOSComboBox::PostCreateNativeWidget(){
}

void igdeNativeBeOSComboBox::DestroyNativeWidget(){
	igdeUIBeOSHelper::DestroyView( this );
}



// Management
///////////////

void igdeNativeBeOSComboBox::BuildList(){
	const int count = pOwner.GetItemCount();
	int i;
	
	pMenu.RemoveItems( 0, pMenu.CountItems(), true );
	pTopItemIndex = 0;
	pCustomItem = NULL;
	
	if( pOwner.GetEditable() ){
		pCustomItem = new BMenuItem( pOwner.GetText(), new BMessage( eeCustomText ) );
		pMenu.AddItem( pCustomItem );
		pMenu.AddSeparatorItem();
		pTopItemIndex = 2;
	}
	
	for( i=0; i<count; i++ ){
		const igdeListItem &item = *pOwner.GetItemAt( i );
		BMenuItem * const menuItem = new BMenuItem( item.GetText(), new BMessage( eeSelectItem ) );
		pMenu.AddItem( menuItem );
		
		if( item.GetIcon() ){
			// how?
			// list->setItemIcon( i, ( BBitmap* )item.GetIcon()->GetNativeIcon() );
		}
	}
	
	if( pOwner.GetSelection() != -1 ){
		pMenu.ItemAt( pTopItemIndex + pOwner.GetSelection() )->SetMarked( true );
		
	}else if( pCustomItem ){
		pCustomItem->SetMarked( true );
	}
}

void igdeNativeBeOSComboBox::UpdateItem( int index ){
	const igdeListItem &item = *pOwner.GetItemAt( index );
	
	BMenuItem &menuItem = *pMenu.ItemAt( pTopItemIndex + index );
	menuItem.SetLabel( item.GetText() );
	
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
	const BMenuItem * const marked = pMenu.FindMarked();
	pOwner.SetText( marked ? marked->Label() : "", changing );
}

void igdeNativeBeOSComboBox::OnInvalidValueChanged(){
	SetViewColor( pOwner.GetInvalidValue() ? pInvalidBackColor : pOrgBackColor );
}

void igdeNativeBeOSComboBox::UpdateText(){
	if( pOwner.GetSelection() != -1 ){
		pMenu.ItemAt( pTopItemIndex + pOwner.GetSelection() )->SetMarked( true );
		
	}else if( pCustomItem ){
		pCustomItem->SetMarked( true );
		
	}else{
		BMenuItem * const marked = pMenu.FindMarked();
		if( marked ){
			marked->SetMarked( false );
		}
	}
	
	if( pCustomItem ){
		pCustomItem->SetLabel( pOwner.GetText() );
	}
}

void igdeNativeBeOSComboBox::InsertItem( int index, const igdeListItem &item ){
	BuildList();
}

void igdeNativeBeOSComboBox::RemoveItem( int index ){
	BuildList();
}

void igdeNativeBeOSComboBox::RemoveAllItems(){
	BuildList();
}

void igdeNativeBeOSComboBox::MoveItem( int fromIndex, int toIndex ){
	BuildList();
}

void igdeNativeBeOSComboBox::Focus(){
	MakeFocus( true );
}

void igdeNativeBeOSComboBox::UpdateRowCount(){
	// automatic in haiku if I'm not mistaken
	/*
	const int count = decMath::max( decMath::min( pOwner.GetRows(), pListView->getNumItems() ), 1 );
	if( count == getNumVisible() ){
		return;
	}
	
	setNumVisible( count );
	
	// FOX needs this otherwise the drop-down list can stay at a wrong size
	layout();
	*/
}

void igdeNativeBeOSComboBox::UpdateEnabled(){
	SetEnabled( pOwner.GetEnabled() );
}

void igdeNativeBeOSComboBox::UpdateEditable(){
	BuildList();
}

void igdeNativeBeOSComboBox::UpdateDescription(){
	SetToolTip( pOwner.GetDescription() );
}



void igdeNativeBeOSComboBox::MessageReceived( BMessage *message ){
	if( message->what == eeCustomText ){
		if( ! pOwner.GetEnabled() ){
			return;
		}
		
		BAlert * const alert = new BAlert( "", pOwner.GetText(), "Cancel", "Save",
			NULL, B_WIDTH_AS_USUAL, B_OFFSET_SPACING, B_EMPTY_ALERT );
		//alert->TextView()->SetLabel( "Edit Custom Value:" );
		alert->TextView()->MakeEditable( true );
		alert->SetShortcut( 0, B_ESCAPE );
		
		if( alert->Go() == 0 ){
			return;
		}
		
		pCustomItem->SetLabel( alert->TextView()->Text() );
		
		try{
			SyncSelection( true );
			
		}catch( const deException &e ){
			pOwner.GetLogger()->LogException( "IGDE", e );
			igdeCommonDialogs::Exception( &pOwner, e );
			return;
		}
		
	}else if( message->what == eeSelectItem ){
		if( ! pOwner.GetEnabled() ){
			return;
		}
		
		try{
			SyncSelection( true );
			
		}catch( const deException &e ){
			pOwner.GetLogger()->LogException( "IGDE", e );
			igdeCommonDialogs::Exception( &pOwner, e );
			return;
		}
		
	}else{
		BMenuField::MessageReceived( message );
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
