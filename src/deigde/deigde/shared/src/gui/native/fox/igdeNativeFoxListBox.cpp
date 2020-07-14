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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "igdeNativeFoxListBox.h"
#include "igdeNativeFoxResizer.h"
#include "../../igdeListBox.h"
#include "../../igdeContainer.h"
#include "../../igdeCommonDialogs.h"
#include "../../model/igdeListItem.h"
#include "../../resources/igdeIcon.h"
#include "../../resources/igdeFont.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../theme/propertyNames.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



FXDEFMAP( igdeNativeFoxListBox ) cNativeIgdeListBoxMap[] = {
	FXMAPFUNC( SEL_COMMAND, igdeNativeFoxListBox::ID_LISTBOX,
		igdeNativeFoxListBox::onListCommand ),
	
	FXMAPFUNC( SEL_CHANGED, igdeNativeFoxListBox::ID_LISTBOX,
		igdeNativeFoxListBox::onListChanged ),
	
	FXMAPFUNC( SEL_SELECTED, igdeNativeFoxListBox::ID_LISTBOX,
		igdeNativeFoxListBox::onListSelected ),
	
	FXMAPFUNC( SEL_DESELECTED, igdeNativeFoxListBox::ID_LISTBOX,
		igdeNativeFoxListBox::onListDeselected ),
	
	FXMAPFUNC( SEL_RIGHTBUTTONPRESS, igdeNativeFoxListBox::ID_LISTBOX,
		igdeNativeFoxListBox::onListRightMouseDown ),
	
	FXMAPFUNC( SEL_RIGHTBUTTONRELEASE, igdeNativeFoxListBox::ID_LISTBOX,
		igdeNativeFoxListBox::onListRightMouseUp ),
	
	FXMAPFUNC( SEL_DOUBLECLICKED, igdeNativeFoxListBox::ID_LISTBOX,
		igdeNativeFoxListBox::onListDoubleClicked ),
	
	FXMAPFUNC( SEL_COMMAND, igdeNativeFoxListBox::ID_RESIZER,
		igdeNativeFoxListBox::onResizerDrag )
};


FXIMPLEMENT( igdeNativeFoxListBox, FXVerticalFrame,
	cNativeIgdeListBoxMap, ARRAYNUMBER( cNativeIgdeListBoxMap ) )

#define MASK_FLAGS_LIST ( LIST_SINGLESELECT | LIST_BROWSESELECT | LIST_MULTIPLESELECT | LIST_AUTOSELECT )


// class igdeNativeFoxListBox
///////////////////////////////

igdeNativeFoxListBox::igdeNativeFoxListBox(){ }

igdeNativeFoxListBox::igdeNativeFoxListBox( igdeListBox &owner, FXComposite *parent,
	const igdeUIFoxHelper::sChildLayoutFlags &layoutFlags, const igdeGuiTheme &guitheme ) :
FXVerticalFrame( parent, layoutFlags.flags | FRAME_SUNKEN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ),
pOwner( &owner ),
pFont( ListBoxFont( owner, guitheme ) ),
pListBox( new FXList( this, this, ID_LISTBOX, LAYOUT_FILL | ListBoxFlags( owner ) ) ),
pResizer( NULL )
{
	(void)ListBoxPadLeft;
	(void)ListBoxPadRight;
	(void)ListBoxPadTop;
	(void)ListBoxPadBottom;
	
	if( ! pOwner->GetVisible() ){
		hide();
	}
	pListBox->setFont( (FXFont*)pFont->GetNativeFont() );
	
	pListBox->setNumVisible( owner.GetRows() );
	if( ! owner.GetEnabled() ){
		pListBox->disable();
	}
	
	//setTipText( owner.GetDescription().GetString() ); // nto supported
	pListBox->setHelpText( owner.GetDescription().GetString() );
	
	BuildList();
	pListBox->setCurrentItem( owner.GetSelection() );
	
	if( layoutFlags.canResizeVertical || ( layoutFlags.flags & LAYOUT_FILL_Y ) == 0 ){
		pResizer = new igdeNativeFoxResizer( this, this, ID_RESIZER );
	}
}

igdeNativeFoxListBox::~igdeNativeFoxListBox(){
}

igdeNativeFoxListBox *igdeNativeFoxListBox::CreateNativeWidget( igdeListBox &owner ){
	if( ! owner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const parent = ( FXComposite* )owner.GetParent()->GetNativeContainer();
	if( ! parent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxListBox( owner, parent,
		igdeUIFoxHelper::GetChildLayoutFlagsAll( &owner ), *owner.GetGuiTheme() );
}

void igdeNativeFoxListBox::PostCreateNativeWidget(){
	FXComposite &parent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( parent.id() ){
		create();
	}
}

void igdeNativeFoxListBox::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxListBox::BuildList(){
	const int count = pOwner->GetItemCount();
	int i;
	
	pListBox->clearItems();
	
	for( i=0; i<count; i++ ){
		const igdeListItem &item = *pOwner->GetItemAt( i );
		pListBox->appendItem( item.GetText().GetString(),
			item.GetIcon() ? ( FXIcon* )item.GetIcon()->GetNativeIcon() : NULL );
	}
	
	UpdateSelection();
}

void igdeNativeFoxListBox::UpdateItem( int index ){
	const igdeListItem &item = *pOwner->GetItemAt( index );
	pListBox->setItemText( index, item.GetText().GetString() );
	
	if( item.GetIcon() ){
		pListBox->setItemIcon( index, ( FXIcon* )item.GetIcon()->GetNativeIcon() );
		
	}else{
		pListBox->setItemIcon( index, NULL );
	}
	
	if( item.GetSelected() ){
		pListBox->selectItem( index );
		
	}else{
		pListBox->deselectItem( index );
	}
}

void igdeNativeFoxListBox::UpdateStyles(){
	pListBox->setListStyle( ListBoxFlags( *pOwner ) & MASK_FLAGS_LIST );
}

void igdeNativeFoxListBox::UpdateSelection(){
	pListBox->setCurrentItem( pOwner->GetSelection() );
	if( pListBox->getCurrentItem() != -1 ){
		pListBox->makeItemVisible( pListBox->getCurrentItem() );
	}
	
	if( pOwner->GetSelectionMode() == igdeListBox::esmMultiple ){
		const int count = pListBox->getNumItems();
		int i;
		
		for( i=0; i<count; i++ ){
			const bool selected = pOwner->GetItemAt( i )->GetSelected();
			FXListItem &item = *pListBox->getItem( i );
			
			if( selected == item.isSelected() ){
				continue;
			}
			
			if( selected ){
				pListBox->selectItem( i );
				
			}else{
				pListBox->deselectItem( i );
			}
		}
	}
}

void igdeNativeFoxListBox::Focus(){
	pListBox->setFocus();
}

void igdeNativeFoxListBox::MakeItemVisible( int index ){
	pListBox->makeItemVisible( index );
}

void igdeNativeFoxListBox::InsertItem( int index ){
	const igdeListItem &item = *pOwner->GetItemAt( index );
	pListBox->insertItem( index, item.GetText().GetString(),
		item.GetIcon() ? ( FXIcon* )item.GetIcon()->GetNativeIcon() : NULL );
}

void igdeNativeFoxListBox::RemoveItem( int index ){
	pListBox->removeItem( index );
}

void igdeNativeFoxListBox::RemoveAllItems(){
	pListBox->clearItems();
}

void igdeNativeFoxListBox::MoveItem( int fromIndex, int toIndex ){
	pListBox->moveItem( toIndex, fromIndex );
}

void igdeNativeFoxListBox::UpdateEnabled(){
	if( pOwner->GetEnabled() ){
		pListBox->enable();
		
	}else{
		pListBox->disable();
	}
}

void igdeNativeFoxListBox::UpdateRowCount(){
	pListBox->setNumVisible( pOwner->GetRows() );
}

void igdeNativeFoxListBox::UpdateDescription(){
	const char * const description = pOwner->GetDescription();
	//pListBox->setTipText( description ); // not supported
	pListBox->setHelpText( description );
}



int igdeNativeFoxListBox::ListBoxFlags( const igdeListBox &owner ){
	int flags = FRAME_SUNKEN;
	
	switch( owner.GetSelectionMode() ){
	case igdeListBox::esmSingle:
		flags |= LIST_BROWSESELECT;
		break;
		
	case igdeListBox::esmMultiple:
		flags |= LIST_EXTENDEDSELECT;
		break;
		
	default:
		break;
	}
	
	return flags;
}

igdeFont *igdeNativeFoxListBox::ListBoxFont( const igdeListBox &owner, const igdeGuiTheme &guitheme ){
	igdeFont::sConfiguration configuration;
	owner.GetEnvironment().GetApplicationFont( configuration );
	
	if( guitheme.HasProperty( igdeGuiThemePropertyNames::listBoxFontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::listBoxFontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::listBoxFontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::listBoxFontSize, 1.0f );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f );
	}
	
	return owner.GetEnvironment().GetSharedFont( configuration );
}

int igdeNativeFoxListBox::ListBoxPadLeft( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::listBoxPaddingLeft, DEFAULT_PAD );
}

int igdeNativeFoxListBox::ListBoxPadRight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::listBoxPaddingRight, DEFAULT_PAD );
}

int igdeNativeFoxListBox::ListBoxPadTop( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::listBoxPaddingTop, DEFAULT_PAD );
}

int igdeNativeFoxListBox::ListBoxPadBottom( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::listBoxPaddingBottom, DEFAULT_PAD );
}



// Events
///////////

long igdeNativeFoxListBox::onListCommand( FXObject*, FXSelector, void* ){
	if( ! pOwner->GetEnabled() || pOwner->GetSelection() == pListBox->getCurrentItem() ){
		return 1;
	}
	
	try{
		pOwner->SetSelection( pListBox->getCurrentItem() );
		pOwner->NotifySelectionChanged();
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
	}
	return 1;
}

long igdeNativeFoxListBox::onListChanged( FXObject *sender, FXSelector selector, void *data ){
	return onListCommand( sender, selector, data );
}

long igdeNativeFoxListBox::onListSelected( FXObject*, FXSelector, void *data ){
	const int index = ( int )( intptr_t )data;
	if( index < 0 || index >= pOwner->GetItemCount() ){
		return 1;
	}
	
	try{
		pOwner->GetItemAt( index )->SetSelected( true );
		pOwner->NotifyItemSelected( index );
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
	}
	return 1;
}

long igdeNativeFoxListBox::onListDeselected( FXObject*, FXSelector, void *data ){
	const int index = ( int )( intptr_t )data;
	if( index < 0 || index >= pOwner->GetItemCount() ){
		return 1;
	}
	
	try{
		pOwner->GetItemAt( index )->SetSelected( false );
		pOwner->NotifyItemDeselected( index );
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
	}
	return 1;
}

long igdeNativeFoxListBox::onListRightMouseDown( FXObject*, FXSelector, void *data ){
	if( ! pOwner->GetEnabled() ){
		return 1;
	}
	
	const FXEvent &event = *( ( FXEvent* )data );
	
	const int index = pListBox->getItemAt( event.win_x, event.win_y );
	if( index != -1 ){
		pListBox->selectItem( index, true );
		pListBox->setCurrentItem( index, true ); // changes selected state only in browse mode
	}
	
	pOwner->ShowContextMenu( decPoint( event.win_x, event.win_y ) );
	return 1;
}

long igdeNativeFoxListBox::onListRightMouseUp( FXObject*, FXSelector, void* ){
	return 1;
}

long igdeNativeFoxListBox::onListDoubleClicked( FXObject*, FXSelector, void *data ){
	const int index = ( int )( intptr_t )data;
	if( index < 0 || index >= pOwner->GetItemCount() ){
		return 1;
	}
	
	try{
		pOwner->NotifyDoubleClickItem( index );
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
	}
	return 1;
}



long igdeNativeFoxListBox::onResizerDrag( FXObject*, FXSelector, void *data ){
	const int distance = igdeNativeFoxResizer::SelCommandDraggedDistance( data );
	const int newHeight = getHeight() + distance;
	
	const int LINE_SPACING = 4;  // hard coded in FXList
	const int itemHeight = LINE_SPACING + pListBox->getFont()->getFontHeight();
	
	pOwner->SetRows( decMath::max( ( newHeight + itemHeight / 2 ) / itemHeight, 1 ) );
	
	return 0;
}

#endif
