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

#ifdef IGDE_TOOLKIT_FOX

#include "igdeNativeFoxComboBoxFilter.h"
#include "foxIcons.h"
#include "../../igdeComboBoxFilter.h"
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

FXDEFMAP( igdeNativeFoxComboBoxFilter ) igdeNativeFoxComboBoxFilterMap[] = {
	FXMAPFUNC( SEL_COMMAND, igdeNativeFoxComboBoxFilter::ID_SELF, igdeNativeFoxComboBoxFilter::onCommand ),
	FXMAPFUNC( SEL_CHANGED, igdeNativeFoxComboBoxFilter::ID_SELF, igdeNativeFoxComboBoxFilter::onChanged ),
	FXMAPFUNC( SEL_CHANGED, igdeNativeFoxComboBoxFilter::ID_EDIT_FILTER, igdeNativeFoxComboBoxFilter::onEditFilterChanged )
};



// Class igdeNativeFoxComboBoxFilter
//////////////////////////////////////

FXIMPLEMENT( igdeNativeFoxComboBoxFilter, FXComboBox, igdeNativeFoxComboBoxFilterMap, ARRAYNUMBER( igdeNativeFoxComboBoxFilterMap ) )

// Constructor, destructor
////////////////////////////

igdeNativeFoxComboBoxFilter::igdeNativeFoxComboBoxFilter(){ }

igdeNativeFoxComboBoxFilter::igdeNativeFoxComboBoxFilter( igdeComboBoxFilter &powner, FXComposite *pparent,
int layoutFlags, const igdeGuiTheme &guitheme ) :
FXComboBox( pparent, powner.GetColumns(), this, ID_SELF, layoutFlags | ComboBoxFlags( powner ),
	0, 0, 0, 0,
	ComboBoxPadLeft( guitheme ), ComboBoxPadRight( guitheme ),
	ComboBoxPadTop( guitheme ), ComboBoxPadBottom( guitheme ) ),
pOwner( &powner ),
pFont( ComboBoxFont( powner, guitheme ) ),
pLabelFilter( NULL ),
pEditFilter( NULL ),
pOrgBackColor( field->getBackColor() ),
pInvalidBackColor( igdeUIFoxHelper::BlendColor( pOrgBackColor, FXRGB( 255, 0, 0 ), 0.25f ) )
{
	FXHorizontalFrame * const lineFilter = new FXHorizontalFrame(
		pane, LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0 );
	pLabelFilter = new FXLabel( lineFilter, "Filter:", NULL, LABEL_NORMAL, 0, 0, 0, 0, 0, 2 );
	pLabelFilter->setJustify( JUSTIFY_LEFT | JUSTIFY_CENTER_Y );
	pEditFilter = new FXTextField( lineFilter, 5, this, ID_EDIT_FILTER, FRAME_SUNKEN | LAYOUT_FILL_X );
	
	setFont( (FXFont*)pFont->GetNativeFont() );
	
	setEditable( powner.GetEditable() );
	if( ! powner.GetEnabled() ){
		disable();
	}
	
	setTipText( powner.GetDescription().GetString() );
	setHelpText( powner.GetDescription().GetString() );
	
	BuildList();
	setCurrentItem( powner.GetSelection() );
	setText( powner.GetText().GetString() );
	
	UpdateRowCount();
}

igdeNativeFoxComboBoxFilter::~igdeNativeFoxComboBoxFilter(){
}

igdeNativeFoxComboBoxFilter *igdeNativeFoxComboBoxFilter::CreateNativeWidget( igdeComboBoxFilter &powner ){
	if( ! powner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const pparent = ( FXComposite* ) powner.GetParent()->GetNativeContainer();
	if( ! pparent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxComboBoxFilter( powner, pparent,
		igdeUIFoxHelper::GetChildLayoutFlags( &powner ), *powner.GetGuiTheme() );
}

void igdeNativeFoxComboBoxFilter::PostCreateNativeWidget(){
	FXComposite &pparent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( pparent.id() ){
		create();
	}
}

void igdeNativeFoxComboBoxFilter::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxComboBoxFilter::BuildList(){
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

void igdeNativeFoxComboBoxFilter::UpdateItem( int index ){
	const igdeListItem &item = *pOwner->GetItemAt( index );
	
	list->setItemText( index, item.GetText().GetString() );
	
	if( item.GetIcon() ){
		list->setItemIcon( index, ( FXIcon* )item.GetIcon()->GetNativeIcon() );
		
	}else{
		list->setItemIcon( index, NULL );
	}
}

void igdeNativeFoxComboBoxFilter::SyncSelection( bool changing ){
	// keep in mind FXComboBox list selection can go out of synch with text. we need to
	// set the text, then update the selection index to match the text and eventually to
	// update the combo box list selection. since setting the combo box list selection
	// trashes the set text we can not use the simple solution. using SetText() is more
	// complex than required but it does work
	pOwner->SetText( getText().text(), changing );
}

void igdeNativeFoxComboBoxFilter::OnInvalidValueChanged(){
	field->setBackColor( pOwner->GetInvalidValue() ? pInvalidBackColor : pOrgBackColor );
}

void igdeNativeFoxComboBoxFilter::UpdateText(){
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

void igdeNativeFoxComboBoxFilter::InsertItem( int index, const igdeListItem &item ){
	list->insertItem( index, item.GetText().GetString() );
	
	if( item.GetIcon() ){
		list->setItemIcon( index, ( FXIcon* )item.GetIcon()->GetNativeIcon() );
		
	}else{
		list->setItemIcon( index, NULL );
	}
}

void igdeNativeFoxComboBoxFilter::RemoveItem( int index ){
	removeItem( index );
}

void igdeNativeFoxComboBoxFilter::RemoveAllItems(){
	clearItems();
}

void igdeNativeFoxComboBoxFilter::MoveItem( int fromIndex, int toIndex ){
	moveItem( toIndex, fromIndex );
}

void igdeNativeFoxComboBoxFilter::Focus(){
	field->setFocus();
}

void igdeNativeFoxComboBoxFilter::UpdateRowCount(){
	const int count = decMath::max( decMath::min( pOwner->GetRows(),
		decMath::max( pOwner->GetItemCount(), pOwner->GetFilterItemCount() ) ), 1 );
	if( count == getNumVisible() ){
		return;
	}
	
	setNumVisible( count );
	
	// FOX needs this otherwise the drop-down list can stay at a wrong size
	layout();
}

void igdeNativeFoxComboBoxFilter::UpdateFilterString(){
	pEditFilter->setText( pOwner->GetFilterString().GetString() );
}

void igdeNativeFoxComboBoxFilter::UpdateEnabled(){
	if( pOwner->GetEnabled() ){
		enable();
		
	}else{
		disable();
	}
}

void igdeNativeFoxComboBoxFilter::UpdateEditable(){
	setEditable( pOwner->GetEditable() );
}

void igdeNativeFoxComboBoxFilter::UpdateDescription(){
	const char * const description = pOwner->GetDescription();
	setTipText( description );
	setHelpText( description );
}



int igdeNativeFoxComboBoxFilter::ComboBoxFlags( const igdeComboBoxFilter & ){
	return FRAME_SUNKEN | COMBOBOX_NORMAL;
}

igdeFont *igdeNativeFoxComboBoxFilter::ComboBoxFont( const igdeComboBoxFilter &powner, const igdeGuiTheme &guitheme ){
	igdeFont::sConfiguration configuration;
	powner.GetEnvironment().GetApplicationFont( configuration );
	
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
	
	return powner.GetEnvironment().GetSharedFont( configuration );
}

int igdeNativeFoxComboBoxFilter::ComboBoxPadLeft( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::comboBoxPaddingLeft, DEFAULT_PAD );
}

int igdeNativeFoxComboBoxFilter::ComboBoxPadRight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::comboBoxPaddingRight, DEFAULT_PAD );
}

int igdeNativeFoxComboBoxFilter::ComboBoxPadTop( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::comboBoxPaddingTop, DEFAULT_PAD );
}

int igdeNativeFoxComboBoxFilter::ComboBoxPadBottom( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::comboBoxPaddingBottom, DEFAULT_PAD );
}



// Events
///////////

long igdeNativeFoxComboBoxFilter::onCommand( FXObject*, FXSelector, void* ){
	// SEL_COMMAND will be send by FXComboBox if either the user clicked on a list box
	// item or if he finished entering text into the text field. Since we do not know
	// which event caused SEL_COMMAND to be fired we need to deal with the same problem
	// we are facing in UpdateText(). in particular we need to fix the list box selection
	// to not go out of sync with the text
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

long igdeNativeFoxComboBoxFilter::onChanged( FXObject*, FXSelector, void* ){
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

long igdeNativeFoxComboBoxFilter::onEditFilterChanged( FXObject*, FXSelector, void* ){
	if( ! pOwner->GetEnabled() ){
		return 0;
	}
	
	try{
		pOwner->SetFilterString( pEditFilter->getText().text() );
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
		return 0;
	}
	
	return 1;
}

#endif
