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

#include "igdeNativeFoxIconListBox.h"
#include "igdeNativeFoxResizer.h"
#include "../../igdeIconListBox.h"
#include "../../igdeIconListBox.h"
#include "../../igdeCommonDialogs.h"
#include "../../igdeContainer.h"
#include "../../model/igdeListItem.h"
#include "../../model/igdeListHeader.h"
#include "../../resources/igdeIcon.h"
#include "../../resources/igdeFont.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../theme/propertyNames.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Icon item
//////////////

class igdeNativeFoxIconListBoxItem : public FXIconItem{
	FXDECLARE( igdeNativeFoxIconListBoxItem )
	
protected:
	igdeNativeFoxIconListBoxItem();
	
private:
	igdeIconListBox *pList;
	igdeListItem *pListItem;
	
public:
	igdeNativeFoxIconListBoxItem( igdeIconListBox &list, igdeListItem &listItem );
	virtual ~igdeNativeFoxIconListBoxItem();
	
	inline igdeIconListBox &GetList() const{ return *pList; }
	inline igdeListItem &GetItem() const{ return *pListItem; }
	
	virtual FXString getTipText() const;
	void UpdateFromItem();
};

FXDEFMAP( igdeNativeFoxIconListBoxItem ) igdeNativeFoxIconListBoxItemMap[] = {};

FXIMPLEMENT( igdeNativeFoxIconListBoxItem, FXIconItem, igdeNativeFoxIconListBoxItemMap, ARRAYNUMBER( igdeNativeFoxIconListBoxItemMap ) )

igdeNativeFoxIconListBoxItem::igdeNativeFoxIconListBoxItem(){ }

igdeNativeFoxIconListBoxItem::igdeNativeFoxIconListBoxItem( igdeIconListBox &list, igdeListItem &listItem ) :
FXIconItem( "" ),
pList( &list ),
pListItem( &listItem )
{
	UpdateFromItem();
}

igdeNativeFoxIconListBoxItem::~igdeNativeFoxIconListBoxItem(){
}

FXString igdeNativeFoxIconListBoxItem::getTipText() const{
	if( ! pListItem->GetDescription().IsEmpty() ){
		return pListItem->GetDescription().GetString();
		
	}else{
		return pList->GetDescription().GetString();
	}
}

void igdeNativeFoxIconListBoxItem::UpdateFromItem(){
	decStringList text;
	text.Add( pListItem->GetText() );
	text += pListItem->GetDetails();
	
	FXIcon * const icon = pListItem->GetIcon() ? ( FXIcon* )pListItem->GetIcon()->GetNativeIcon() : NULL;
	
	setText( text.Join( "\t" ).GetString() );
	setBigIcon( icon );
	setMiniIcon( icon );
	setData( pListItem->GetData() );
}



// Native widget
//////////////////

FXDEFMAP( igdeNativeFoxIconListBox ) cNativeIgdeListBoxMap[] = {
	FXMAPFUNC( SEL_COMMAND, igdeNativeFoxIconListBox::ID_LISTBOX,
		igdeNativeFoxIconListBox::onListCommand ),
	
	FXMAPFUNC( SEL_CHANGED, igdeNativeFoxIconListBox::ID_LISTBOX,
		igdeNativeFoxIconListBox::onListChanged ),
	
	FXMAPFUNC( SEL_RIGHTBUTTONPRESS, igdeNativeFoxIconListBox::ID_LISTBOX,
		igdeNativeFoxIconListBox::onListRightMouseDown ),
	
	FXMAPFUNC( SEL_RIGHTBUTTONRELEASE, igdeNativeFoxIconListBox::ID_LISTBOX,
		igdeNativeFoxIconListBox::onListRightMouseUp ),
		
	FXMAPFUNC( SEL_SELECTED, igdeNativeFoxIconListBox::ID_LISTBOX,
		igdeNativeFoxIconListBox::onListSelected ),
		
	FXMAPFUNC( SEL_DESELECTED, igdeNativeFoxIconListBox::ID_LISTBOX,
		igdeNativeFoxIconListBox::onListDeselected ),
	
	FXMAPFUNC( SEL_DOUBLECLICKED, igdeNativeFoxIconListBox::ID_LISTBOX,
		igdeNativeFoxIconListBox::onListDoubleClicked ),
		
	FXMAPFUNC( SEL_CHANGED, igdeNativeFoxIconListBox::ID_HEADER,
		igdeNativeFoxIconListBox::onListHeaderChanged ),
		
	FXMAPFUNC( SEL_CLICKED, igdeNativeFoxIconListBox::ID_HEADER,
		igdeNativeFoxIconListBox::onListHeaderClicked ),
	
	FXMAPFUNC( SEL_COMMAND, igdeNativeFoxIconListBox::ID_RESIZER,
		igdeNativeFoxIconListBox::onResizerDrag ),
};


FXIMPLEMENT( igdeNativeFoxIconListBox, FXVerticalFrame,
	cNativeIgdeListBoxMap, ARRAYNUMBER( cNativeIgdeListBoxMap ) )


// class igdeNativeFoxIconListBox
///////////////////////////////////

igdeNativeFoxIconListBox::igdeNativeFoxIconListBox(){ }

igdeNativeFoxIconListBox::igdeNativeFoxIconListBox( igdeIconListBox &owner, FXComposite *parent,
	const igdeUIFoxHelper::sChildLayoutFlags &layoutFlags, const igdeGuiTheme &guitheme ) :
FXVerticalFrame( parent, layoutFlags.flags | IconListBoxFlagsBorder( owner ), 0, 0, 0, 0, 0, 0, 0, 0 ),
pOwner( &owner ),
pFont( IconListBoxFont( owner, guitheme ) ),
pListBox( new FXIconList( this, this, ID_LISTBOX, IconListBoxFlags( owner ) ) ),
pResizer( NULL )
{
	(void)IconListBoxPadLeft;
	(void)IconListBoxPadRight;
	(void)IconListBoxPadTop;
	(void)IconListBoxPadBottom;
	
	if( ! pOwner->GetVisible() ){
		hide();
	}
	pListBox->setFont( (FXFont*)pFont->GetNativeFont() );
	
	pDefaultSize.Set( 200, pListBox->getFont()->getFontHeight() * owner.GetRows() );
	
	//setNumVisible( owner.GetRows() ); // not supported
	if( ! owner.GetEnabled() ){
		pListBox->disable();
	}
	
	//setTipText( owner.GetDescription().GetString() ); // not supported
	pListBox->setHelpText( owner.GetDescription().GetString() );
	
	BuildHeader();
	BuildList();
	pListBox->setCurrentItem( owner.GetSelection() );
	
	if( layoutFlags.canResizeVertical || ( layoutFlags.flags & LAYOUT_FILL_Y ) == 0 ){
		pResizer = new igdeNativeFoxResizer( this, this, ID_RESIZER );
	}
}

igdeNativeFoxIconListBox::~igdeNativeFoxIconListBox(){
}

igdeNativeFoxIconListBox *igdeNativeFoxIconListBox::CreateNativeWidget( igdeIconListBox &owner ){
	if( ! owner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const parent = ( FXComposite* )owner.GetParent()->GetNativeContainer();
	if( ! parent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxIconListBox( owner, parent,
		igdeUIFoxHelper::GetChildLayoutFlagsAll( &owner ), *owner.GetGuiTheme() );
}

void igdeNativeFoxIconListBox::PostCreateNativeWidget(){
	FXComposite &parent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( parent.id() ){
		create();
	}
}

void igdeNativeFoxIconListBox::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxIconListBox::BuildHeader(){
	FXHeader &header = *pListBox->getHeader();
	header.setFont( pListBox->getFont() );
	header.setPadLeft( 3 );
	header.setPadTop( 0 );
	header.setPadRight( 3 );
	header.setPadBottom( 0 );
	header.setTarget( this );
	header.setSelector( ID_HEADER );
	
	UpdateHeader();
}

void igdeNativeFoxIconListBox::BuildList(){
	const int count = pOwner->GetItemCount();
	int i;
	
	pListBox->clearItems();
	
	for( i=0; i<count; i++ ){
		pListBox->appendItem( new igdeNativeFoxIconListBoxItem( *pOwner, *pOwner->GetItemAt( i ) ) );
	}
	
	UpdateSelection();
}

void igdeNativeFoxIconListBox::InsertItem( int index, igdeListItem &item ){
	igdeNativeFoxIconListBoxItem * const foxItem = new igdeNativeFoxIconListBoxItem( *pOwner, item );
	
	if( index == pListBox->getNumItems() ){
		pListBox->appendItem( foxItem );
		
	}else{
		pListBox->insertItem( index, foxItem );
	}
}

void igdeNativeFoxIconListBox::UpdateItem( int index ){
	( ( igdeNativeFoxIconListBoxItem* )pListBox->getItem( index ) )->UpdateFromItem();
	pListBox->updateItem( index );
}

void igdeNativeFoxIconListBox::MoveItem( int fromIndex, int toIndex ){
	pListBox->moveItem( toIndex, fromIndex );
}

void igdeNativeFoxIconListBox::RemoveItem( int index ){
	pListBox->removeItem( index );
}

void igdeNativeFoxIconListBox::UpdateSelection(){
	pListBox->setCurrentItem( pOwner->GetSelection() );
	if( pListBox->getCurrentItem() != -1 ){
		pListBox->makeItemVisible( pListBox->getCurrentItem() );
	}
	
	if( pOwner->GetSelectionMode() == igdeIconListBox::esmMultiple ){
		const int count = pListBox->getNumItems();
		int i;
		
		for( i=0; i<count; i++ ){
			igdeNativeFoxIconListBoxItem &item = *( ( igdeNativeFoxIconListBoxItem* )pListBox->getItem( i ) );
			const bool selected = item.GetItem().GetSelected();
			if( selected == item.isSelected() ){
				continue;
			}
			
			if( selected ){
				pListBox->selectItem( i );
				
			}else{
				pListBox->deselectItem( i );
			}
		}
		
	}else if( pListBox->getCurrentItem() != -1 ){
		pListBox->selectItem( pListBox->getCurrentItem() );
	}
}

void igdeNativeFoxIconListBox::UpdateHeader(){
	FXHeader &nativeHeader = *pListBox->getHeader();
	const int count = pOwner->GetHeaderCount();
	int i;
	
	nativeHeader.clearItems();
	
	for( i=0; i<count; i++ ){
		const igdeListHeader &header = *pOwner->GetHeaderAt( i );
		
		nativeHeader.appendItem( header.GetTitle().GetString(),
			header.GetIcon() ? ( FXIcon* )header.GetIcon()->GetNativeIcon() : NULL,
			header.GetSize() );
	}
}

void igdeNativeFoxIconListBox::UpdateStyles(){
	pListBox->setListStyle( IconListBoxFlags( *pOwner ) );
}

void igdeNativeFoxIconListBox::UpdateDescription(){
	//pListBox->setTipText( pOwner->GetDescription().GetString() ); // not supported
	pListBox->setHelpText( pOwner->GetDescription().GetString() );
}

void igdeNativeFoxIconListBox::UpdateEnabled(){
	if( pOwner->GetEnabled() ){
		pListBox->enable();
		
	}else{
		pListBox->disable();
	}
}

void igdeNativeFoxIconListBox::Focus(){
	pListBox->setFocus();
}

void igdeNativeFoxIconListBox::MakeItemVisible( int index ){
	pListBox->makeItemVisible( index );
}

void igdeNativeFoxIconListBox::RemoveAllItems(){
	pListBox->clearItems();
}



int igdeNativeFoxIconListBox::IconListBoxFlagsBorder( const igdeIconListBox & ){
	return FRAME_SUNKEN;
}

int igdeNativeFoxIconListBox::IconListBoxFlags( const igdeIconListBox &owner ){
	int flags = LAYOUT_FILL_X | LAYOUT_FILL_Y;
	
	switch( owner.GetViewMode() ){
	case igdeIconListBox::evmList:
		flags |= ICONLIST_DETAILED | ICONLIST_ROWS;
		break;
		
	case igdeIconListBox::evmIconHorizontal:
		flags |= ICONLIST_BIG_ICONS | ICONLIST_ROWS;
		break;
		
	case igdeIconListBox::evmIconVertical:
		flags |= ICONLIST_BIG_ICONS | ICONLIST_COLUMNS;
		break;
		
	default:
		flags |= ICONLIST_DETAILED | ICONLIST_ROWS;
	}
	
	switch( owner.GetSelectionMode() ){
	case igdeIconListBox::esmSingle:
		flags |= FOLDINGLIST_BROWSESELECT /*ICONLIST_SINGLESELECT*/;
		break;
		
	case igdeIconListBox::esmMultiple:
		flags |= ICONLIST_EXTENDEDSELECT;//ICONLIST_MULTIPLESELECT;
		break;
		
	default:
		flags |= FOLDINGLIST_BROWSESELECT;
		break;
	}
	
	return flags;
}

igdeFont *igdeNativeFoxIconListBox::IconListBoxFont( const igdeIconListBox &owner, const igdeGuiTheme &guitheme ){
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

int igdeNativeFoxIconListBox::IconListBoxPadLeft( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::listBoxPaddingLeft, DEFAULT_PAD );
}

int igdeNativeFoxIconListBox::IconListBoxPadRight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::listBoxPaddingRight, DEFAULT_PAD );
}

int igdeNativeFoxIconListBox::IconListBoxPadTop( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::listBoxPaddingTop, DEFAULT_PAD );
}

int igdeNativeFoxIconListBox::IconListBoxPadBottom( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::listBoxPaddingBottom, DEFAULT_PAD );
}



FXint igdeNativeFoxIconListBox::getDefaultWidth(){
	return pDefaultSize.x;
}

FXint igdeNativeFoxIconListBox::getDefaultHeight(){
	return pDefaultSize.y;
}



// Events
///////////

long igdeNativeFoxIconListBox::onListCommand( FXObject*, FXSelector, void* ){
	// send if user clicks on an item. also sends sends SEL_CLICKED, SEL_DOUBLECLICKED
	// and SEL_TRIPLECLICKED.
	return 1;
}

long igdeNativeFoxIconListBox::onListChanged( FXObject*, FXSelector, void* ){
	if( ! pOwner->GetEnabled() ){
		return 1;
	}
	
	try{
		pOwner->SetSelection( pListBox->getCurrentItem() );
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
	}
	return 1;
}

long igdeNativeFoxIconListBox::onListSelected( FXObject*, FXSelector, void *data ){
	const int index = ( int )( intptr_t )data;
	if( index >= 0 and index < pOwner->GetItemCount() ){
		pOwner->GetItemAt( index )->SetSelected( true );
		pOwner->NotifyItemSelected( index );
	}
	return 1;
}

long igdeNativeFoxIconListBox::onListDeselected( FXObject*, FXSelector, void *data ){
	const int index = ( int )( intptr_t )data;
	if( index >= 0 and index < pOwner->GetItemCount() ){
		pOwner->GetItemAt( index )->SetSelected( false );
		pOwner->NotifyItemDeselected( index );
	}
	return 1;
}

long igdeNativeFoxIconListBox::onListHeaderChanged( FXObject *sender, FXSelector selector, void *data ){
	const int index = ( int )( intptr_t )data;
	if( index >= 0 and index < pOwner->GetHeaderCount() ){
		pOwner->GetHeaderAt( index )->SetSize( pListBox->getHeader()->getItemSize( index ) );
	}
	return pListBox->onChgHeader( sender, selector, data );
}

long igdeNativeFoxIconListBox::onListHeaderClicked( FXObject *sender, FXSelector selector, void *data ){
	const int index = ( int )( intptr_t )data;
	if( index >= 0 and index < pOwner->GetHeaderCount() ){
		pOwner->NotifyHeaderClicked( index );
	}
	return pListBox->onClkHeader( sender, selector, data );
}

long igdeNativeFoxIconListBox::onListRightMouseDown( FXObject*, FXSelector, void *data ){
	if( ! pOwner->GetEnabled() ){
		return 1;
	}
	
	const FXEvent &event = *( ( FXEvent* )data );
	
	const int index = pListBox->getItemAt( event.win_x, event.win_y );
	if( index != -1 ){
		pListBox->selectItem( index, true );
		pListBox->setCurrentItem( index, true );
	}
	
	pOwner->ShowContextMenu( decPoint( event.win_x, event.win_y ) );
	return 1;
}

long igdeNativeFoxIconListBox::onListRightMouseUp( FXObject*, FXSelector, void* ){
	return 1;
}

long igdeNativeFoxIconListBox::onListDoubleClicked( FXObject*, FXSelector, void *data ){
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



long igdeNativeFoxIconListBox::onResizerDrag( FXObject*, FXSelector, void *data ){
	const int distance = igdeNativeFoxResizer::SelCommandDraggedDistance( data );
	pDefaultSize.y = decMath::max( 50, pDefaultSize.y + distance );
	recalc();
	return 0;
}

#endif
