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

#include "igdeNativeFoxComboBox.h"
#include "foxIcons.h"
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



// Events
///////////

FXDEFMAP( igdeNativeFoxComboBox ) igdeNativeFoxComboBoxMap[] = {
	FXMAPFUNC( SEL_COMMAND, igdeNativeFoxComboBox::ID_SELF, igdeNativeFoxComboBox::onCommand ),
	FXMAPFUNC( SEL_CHANGED, igdeNativeFoxComboBox::ID_SELF, igdeNativeFoxComboBox::onChanged )
};



// Class igdeNativeFoxComboBox
////////////////////////////////

FXIMPLEMENT( igdeNativeFoxComboBox, FXComboBox, igdeNativeFoxComboBoxMap, ARRAYNUMBER( igdeNativeFoxComboBoxMap ) )

// Constructor, destructor
////////////////////////////

igdeNativeFoxComboBox::igdeNativeFoxComboBox(){ }

igdeNativeFoxComboBox::igdeNativeFoxComboBox( igdeComboBox &owner, FXComposite *parent,
int layoutFlags, const igdeGuiTheme &guitheme ) :
FXComboBox( parent, owner.GetColumns(), this, ID_SELF, layoutFlags | ComboBoxFlags( owner ),
	0, 0, 0, 0,
	ComboBoxPadLeft( guitheme ), ComboBoxPadRight( guitheme ),
	ComboBoxPadTop( guitheme ), ComboBoxPadBottom( guitheme ) ),
pOwner( &owner ),
pFont( ComboBoxFont( owner, guitheme ) ),
pOrgBackColor( field->getBackColor() ),
pInvalidBackColor( igdeUIFoxHelper::BlendColor( pOrgBackColor, FXRGB( 255, 0, 0 ), 0.25f ) )
{
	setFont( (FXFont*)pFont->GetNativeFont() );
	
	setEditable( owner.GetEditable() );
	if( ! owner.GetEnabled() ){
		disable();
	}
	
	setTipText( owner.GetDescription().GetString() );
	setHelpText( owner.GetDescription().GetString() );
	
	BuildList();
	setCurrentItem( owner.GetSelection() );
	setText( owner.GetText().GetString() );
	
	UpdateRowCount();
}

igdeNativeFoxComboBox::~igdeNativeFoxComboBox(){
}

igdeNativeFoxComboBox *igdeNativeFoxComboBox::CreateNativeWidget( igdeComboBox &owner ){
	if( ! owner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const parent = ( FXComposite* )owner.GetParent()->GetNativeContainer();
	if( ! parent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxComboBox( owner, parent,
		igdeUIFoxHelper::GetChildLayoutFlags( &owner ), *owner.GetGuiTheme() );
}

void igdeNativeFoxComboBox::PostCreateNativeWidget(){
	FXComposite &parent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( parent.id() ){
		create();
	}
}

void igdeNativeFoxComboBox::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxComboBox::BuildList(){
	const int count = pOwner->GetItemCount();
	int i;
	
	clearItems();
	
	for( i=0; i<count; i++ ){
		const igdeListItem &item = *pOwner->GetItemAt( i );
		
		appendItem( item.GetText().GetString() );
		
		if( item.GetIcon() ){
			list->setItemIcon( i, ( FXIcon* )item.GetIcon()->GetNativeIcon() );
		}
	}
}

void igdeNativeFoxComboBox::UpdateItem( int index ){
	const igdeListItem &item = *pOwner->GetItemAt( index );
	
	list->setItemText( index, item.GetText().GetString() );
	
	if( item.GetIcon() ){
		list->setItemIcon( index, ( FXIcon* )item.GetIcon()->GetNativeIcon() );
		
	}else{
		list->setItemIcon( index, NULL );
	}
}

void igdeNativeFoxComboBox::SyncSelection( bool changing ){
	// keep in mind FXComboBox list selection can go out of synch with text. we need to
	// set the text, then update the selection index to match the text and eventually to
	// update the combo box list selection. since setting the combo box list selection
	// trashes the set text we can not use the simple solution. using SetText() is more
	// complex than required but it does work
	pOwner->SetText( getText().text(), changing );
}

void igdeNativeFoxComboBox::OnInvalidValueChanged(){
	field->setBackColor( pOwner->GetInvalidValue() ? pInvalidBackColor : pOrgBackColor );
}

void igdeNativeFoxComboBox::UpdateText(){
	// documentation states setText() selects the matching list item. this is true if the
	// list item is present but not if it is absent. in this case the old selection is kept
	// causing the text in the combobox and the selected list item to be out of sync.
	// we have to ensure this situation to never happen. the igde combo box widget does set
	// selection to -1 if not matching the text.
	// 
	// unfortunately setCurrentItem() also changes the text although it is not stated in
	// the documentation that this side-effects is present. we thus have to do this in the
	// right order to avoid a total mess to break loose.
	setCurrentItem( pOwner->GetSelection() );
	setText( pOwner->GetText().GetString() );
}

void igdeNativeFoxComboBox::InsertItem( int index, const igdeListItem &item ){
	list->insertItem( index, item.GetText().GetString() );
	
	if( item.GetIcon() ){
		list->setItemIcon( index, ( FXIcon* )item.GetIcon()->GetNativeIcon() );
		
	}else{
		list->setItemIcon( index, NULL );
	}
}

void igdeNativeFoxComboBox::RemoveItem( int index ){
	removeItem( index );
}

void igdeNativeFoxComboBox::RemoveAllItems(){
	clearItems();
}

void igdeNativeFoxComboBox::MoveItem( int fromIndex, int toIndex ){
	moveItem( toIndex, fromIndex );
}

void igdeNativeFoxComboBox::Focus(){
	field->setFocus();
}

void igdeNativeFoxComboBox::UpdateRowCount(){
	const int count = decMath::max( decMath::min( pOwner->GetRows(), getNumItems() ), 1 );
	if( count == getNumVisible() ){
		return;
	}
	
	setNumVisible( count );
	
	// FOX needs this otherwise the drop-down list can stay at a wrong size
	layout();
}

void igdeNativeFoxComboBox::UpdateEnabled(){
	if( pOwner->GetEnabled() ){
		enable();
		
	}else{
		disable();
	}
}

void igdeNativeFoxComboBox::UpdateEditable(){
	setEditable( pOwner->GetEditable() );
}

void igdeNativeFoxComboBox::UpdateDescription(){
	const char * const description = pOwner->GetDescription();
	setTipText( description );
	setHelpText( description );
}



int igdeNativeFoxComboBox::ComboBoxFlags( const igdeComboBox & ){
	return FRAME_SUNKEN | COMBOBOX_NORMAL;
}

igdeFont *igdeNativeFoxComboBox::ComboBoxFont( const igdeComboBox &owner, const igdeGuiTheme &guitheme ){
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

int igdeNativeFoxComboBox::ComboBoxPadLeft( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::comboBoxPaddingLeft, DEFAULT_PAD );
}

int igdeNativeFoxComboBox::ComboBoxPadRight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::comboBoxPaddingRight, DEFAULT_PAD );
}

int igdeNativeFoxComboBox::ComboBoxPadTop( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::comboBoxPaddingTop, DEFAULT_PAD );
}

int igdeNativeFoxComboBox::ComboBoxPadBottom( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::comboBoxPaddingBottom, DEFAULT_PAD );
}



// Events
///////////

long igdeNativeFoxComboBox::onCommand( FXObject*, FXSelector, void* ){
	if( ! pOwner->GetEnabled() ){
		return 0;
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
		return 0;
	}
	
	return 1;
}

long igdeNativeFoxComboBox::onChanged( FXObject*, FXSelector, void* ){
	if( ! pOwner->GetEnabled() ){
		return 0;
	}
	
	try{
		SyncSelection( true );
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
		return 0;
	}
	
	return 1;
}
