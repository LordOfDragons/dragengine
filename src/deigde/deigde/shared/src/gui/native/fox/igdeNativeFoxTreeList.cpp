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

#include "igdeNativeFoxTreeList.h"
#include "igdeNativeFoxTreeItem.h"
#include "igdeNativeFoxResizer.h"
#include "../../igdeTreeList.h"
#include "../../igdeCommonDialogs.h"
#include "../../model/igdeTreeItem.h"
#include "../../model/igdeTreeItemReference.h"
#include "../../resources/igdeIcon.h"
#include "../../resources/igdeFont.h"
#include "../../theme/igdeGuiTheme.h"
#include "../../theme/propertyNames.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



FXDEFMAP( igdeNativeFoxTreeList ) cNativeIgdeTreeListMap[] = {
	FXMAPFUNC( SEL_COMMAND, igdeNativeFoxTreeList::ID_TREELIST,
		igdeNativeFoxTreeList::onListCommand ),
	
	FXMAPFUNC( SEL_CHANGED, igdeNativeFoxTreeList::ID_TREELIST,
		igdeNativeFoxTreeList::onListChanged ),
	
	FXMAPFUNC( SEL_EXPANDED, igdeNativeFoxTreeList::ID_TREELIST,
		igdeNativeFoxTreeList::onExpanded ),
	
	FXMAPFUNC( SEL_COLLAPSED, igdeNativeFoxTreeList::ID_TREELIST,
		igdeNativeFoxTreeList::onCollapsed ),
	
	FXMAPFUNC( SEL_RIGHTBUTTONPRESS, igdeNativeFoxTreeList::ID_TREELIST,
		igdeNativeFoxTreeList::onListRightMouseDown ),
	
	FXMAPFUNC( SEL_RIGHTBUTTONRELEASE, igdeNativeFoxTreeList::ID_TREELIST,
		igdeNativeFoxTreeList::onListRightMouseUp ),
	
	FXMAPFUNC( SEL_DOUBLECLICKED, igdeNativeFoxTreeList::ID_TREELIST,
		igdeNativeFoxTreeList::onListDoubleClicked ),
	
	FXMAPFUNC( SEL_COMMAND, igdeNativeFoxTreeList::ID_RESIZER,
		igdeNativeFoxTreeList::onResizerDrag )
};


FXIMPLEMENT( igdeNativeFoxTreeList, FXVerticalFrame,
	cNativeIgdeTreeListMap, ARRAYNUMBER( cNativeIgdeTreeListMap ) )


// class igdeNativeFoxTreeList
////////////////////////////////

igdeNativeFoxTreeList::igdeNativeFoxTreeList(){ }

igdeNativeFoxTreeList::igdeNativeFoxTreeList( igdeTreeList &owner, FXComposite *parent,
	const igdeUIFoxHelper::sChildLayoutFlags &layoutFlags, const igdeGuiTheme &guitheme ) :
FXVerticalFrame( parent, layoutFlags.flags | FRAME_SUNKEN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ),
pOwner( &owner ),
pFont( TreeListFont( owner, guitheme ) ),
pTreeList( new FXTreeList( this, this, ID_TREELIST, LAYOUT_FILL | TreeListFlags( owner ) ) ),
pResizer( NULL )
{
	(void)TreeListPadLeft;
	(void)TreeListPadRight;
	(void)TreeListPadTop;
	(void)TreeListPadBottom;
	
	if( ! pOwner->GetVisible() ){
		hide();
	}
	pTreeList->setFont( (FXFont*)pFont->GetNativeFont() );
	
	pTreeList->setNumVisible( owner.GetRows() );
	if( ! owner.GetEnabled() ){
		pTreeList->disable();
	}
	
	//setTipText( owner.GetDescription().GetString() ); // nto supported
	pTreeList->setHelpText( owner.GetDescription().GetString() );
	
	BuildTree();
	
	pTreeList->setCurrentItem( owner.GetSelection() ?
		( igdeNativeFoxTreeItem* )owner.GetSelection()->GetNativeWidget() : NULL );
	
	if( layoutFlags.canResizeVertical || ( layoutFlags.flags & LAYOUT_FILL_Y ) == 0 ){
		pResizer = new igdeNativeFoxResizer( this, this, ID_RESIZER );
	}
}

igdeNativeFoxTreeList::~igdeNativeFoxTreeList(){
	// drop native widget of all tree items
	pDropItemsNativeWidget( NULL );
}



// Management
///////////////

void igdeNativeFoxTreeList::BuildTree(){
	RemoveAllItems( NULL );
	
	igdeTreeItem *child = pOwner->GetFirstChild();
	while( child ){
		CreateAndAppendItem( child );
		child = child->GetNext();
	}
}

void igdeNativeFoxTreeList::UpdateItem( igdeTreeItem *item ){
	igdeNativeFoxTreeItem * const native = ( igdeNativeFoxTreeItem* )item->GetNativeWidget();
	if( ! native ){
		return;
	}
	
	native->setText( item->GetText().GetString() );
	
	if( item->GetIcon() ){
		native->setOpenIcon( ( FXIcon* )item->GetIcon()->GetNativeIcon() );
		
	}else{
		native->setOpenIcon( NULL );
	}
	native->setClosedIcon( native->getOpenIcon() );
	
	native->setExpanded( item->GetExpanded() );
	
	pTreeList->updateItem( native );
}

void igdeNativeFoxTreeList::MakeItemVisible( igdeTreeItem *item ){
	igdeNativeFoxTreeItem * const native = ( igdeNativeFoxTreeItem* )item->GetNativeWidget();
	if( ! native ){
		return;
	}
	
	/*
	igdeNativeFoxTreeItem *parent = native->getParent();
	while( parent ){
		pTreeList->expandTree( parent );
		parent = parent->getParent();
	}
	*/
	
	pTreeList->makeItemVisible( native );
}

static igdeNativeFoxTreeItem *igdeNativeFoxTreeListCreateItem( igdeTreeItem *item ){
	if( item->GetNativeWidget() ){
		DETHROW( deeInvalidParam );
	}
	
	igdeNativeFoxTreeItem * const nativeItem = new igdeNativeFoxTreeItem( *item );
	
	nativeItem->setText( item->GetText().GetString() );
	
	if( item->GetIcon() ){
		nativeItem->setOpenIcon( ( FXIcon* )item->GetIcon()->GetNativeIcon() );
		
	}else{
		nativeItem->setOpenIcon( NULL );
	}
	
	nativeItem->setClosedIcon( nativeItem->getOpenIcon() );
	
	nativeItem->setExpanded( item->GetExpanded() );
	
	return nativeItem;
}

void igdeNativeFoxTreeList::CreateAndInsertItem( igdeTreeItem *item ){
	igdeNativeFoxTreeItem *nativeParent = NULL;
	if( item->GetParent() ){
		nativeParent = ( igdeNativeFoxTreeItem* )item->GetParent()->GetNativeWidget();
		if( ! nativeParent ){
			DETHROW( deeInvalidParam );
		}
	}
	
	igdeNativeFoxTreeItem *nativeNext = NULL;
	if( item->GetNext() ){
		nativeNext = ( igdeNativeFoxTreeItem* )item->GetNext()->GetNativeWidget();
		if( ! nativeNext ){
			DETHROW( deeInvalidParam );
		}
	}
	
	igdeNativeFoxTreeItem * const nativeItem = igdeNativeFoxTreeListCreateItem( item );
	
	if( nativeNext ){
		pTreeList->insertItem( nativeNext, nativeParent, nativeItem );
		
	}else{
		pTreeList->appendItem( nativeParent, nativeItem );
	}
	
	item->SetNativeWidget( nativeItem );
	
	// add children
	igdeTreeItem *child = item->GetFirstChild();
	while( child ){
		CreateAndAppendItem( child );
		child = child->GetNext();
	}
}

void igdeNativeFoxTreeList::CreateAndAppendItem( igdeTreeItem *item ){
	if( item->GetNext() && item->GetNext()->GetNativeWidget() ){
		DETHROW( deeInvalidParam );
	}
	
	igdeNativeFoxTreeItem *nativeParent = NULL;
	if( item->GetParent() ){
		nativeParent = ( igdeNativeFoxTreeItem* )item->GetParent()->GetNativeWidget();
		if( ! nativeParent ){
			DETHROW( deeInvalidParam );
		}
	}
	
	igdeNativeFoxTreeItem * const nativeItem = igdeNativeFoxTreeListCreateItem( item );
	
	pTreeList->appendItem( nativeParent, nativeItem );
	
	item->SetNativeWidget( nativeItem );
	
	// add children
	igdeTreeItem *child = item->GetFirstChild();
	while( child ){
		CreateAndAppendItem( child );
		child = child->GetNext();
	}
}

void igdeNativeFoxTreeList::RemoveItem( igdeTreeItem *item ){
	igdeTreeItem *child = item->GetFirstChild();
	while( child ){
		RemoveItem( child );
		child = child->GetNext();
	}
	
	igdeNativeFoxTreeItem * const native = ( igdeNativeFoxTreeItem* )item->GetNativeWidget();
	if( native ){
		pTreeList->removeItem( native );
		item->SetNativeWidget( NULL );
	}
}

void igdeNativeFoxTreeList::RemoveAllItems( igdeTreeItem *parent ){
	igdeTreeItem *child = parent ? parent->GetFirstChild() : pOwner->GetFirstChild();
	while( child ){
		RemoveItem( child );
		child = child->GetNext();
	}
	
	if( parent ){
		igdeNativeFoxTreeItem * const native = ( igdeNativeFoxTreeItem* )parent->GetNativeWidget();
		if( native ){
			pTreeList->removeItems( native->getFirst(), native->getLast() );
			SelectItem( pOwner->GetSelection() );
		}
		
	}else{
		pTreeList->clearItems();
	}
}

void igdeNativeFoxTreeList::ItemMoved( igdeTreeItem *item ){
	igdeNativeFoxTreeItem * const native = ( igdeNativeFoxTreeItem* )item->GetNativeWidget();
	if( ! native ){
		DETHROW( deeInvalidParam );
	}
	
	igdeNativeFoxTreeItem *nativeParent = NULL;
	if( item->GetParent() ){
		nativeParent = ( igdeNativeFoxTreeItem* )item->GetParent()->GetNativeWidget();
		if( ! nativeParent ){
			DETHROW( deeInvalidParam );
		}
	}
	
	igdeNativeFoxTreeItem *nativeBefore = NULL;
	if( item->GetNext() ){
		nativeBefore = ( igdeNativeFoxTreeItem* )item->GetNext()->GetNativeWidget();
		if( ! nativeBefore ){
			DETHROW( deeInvalidParam );
		}
	}
	
	pTreeList->moveItem( nativeBefore, nativeParent, native );
}

void igdeNativeFoxTreeList::SelectItem( igdeTreeItem *item ){
	if( item ){
		igdeNativeFoxTreeItem * const native = ( igdeNativeFoxTreeItem* )item->GetNativeWidget();
		if( native ){
			pTreeList->setCurrentItem( native );
			pTreeList->makeItemVisible( native );
			
		}else{
			pTreeList->setCurrentItem( NULL );
		}
		
	}else{
		pTreeList->setCurrentItem( NULL );
		pTreeList->killSelection();
	}
}

void igdeNativeFoxTreeList::ItemsSortedIn( igdeTreeItem *item ){
	igdeNativeFoxTreeItem * const nativeParent = item ? ( igdeNativeFoxTreeItem * )item->GetNativeWidget() : NULL;
	if( item && ! nativeParent ){
		DETHROW( deeInvalidParam );
	}
	
	igdeTreeItem *child = item ? item->GetLastChild() : pOwner->GetLastChild();
	igdeNativeFoxTreeItem *nativeBefore = NULL;
	
	while( child ){
		igdeNativeFoxTreeItem * const nativeChild = ( igdeNativeFoxTreeItem* )child->GetNativeWidget();
		if( ! nativeChild ){
			DETHROW( deeInvalidParam );
		}
		
		pTreeList->moveItem( nativeBefore, nativeParent, nativeChild );
		nativeBefore = nativeChild;
		
		child = child->GetPrevious();
	}
}

void igdeNativeFoxTreeList::Focus(){
	pTreeList->setFocus();
}



int igdeNativeFoxTreeList::TreeListFlags( const igdeTreeList & ){
	return TREELIST_BROWSESELECT | TREELIST_SHOWS_LINES | TREELIST_SHOWS_BOXES | TREELIST_ROOT_BOXES;
}

igdeFont *igdeNativeFoxTreeList::TreeListFont( const igdeTreeList &owner, const igdeGuiTheme &guitheme ){
	igdeFont::sConfiguration configuration;
	owner.GetEnvironment().GetApplicationFont( configuration );
	
	if( guitheme.HasProperty( igdeGuiThemePropertyNames::treeListFontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::treeListFontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::treeListFontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::treeListFontSize, 1.0f );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSizeAbsolute ) ){
		configuration.size = guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f );
	}
	
	return owner.GetEnvironment().GetSharedFont( configuration );
}

int igdeNativeFoxTreeList::TreeListPadLeft( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::treeListPaddingLeft, DEFAULT_PAD );
}

int igdeNativeFoxTreeList::TreeListPadRight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::treeListPaddingRight, DEFAULT_PAD );
}

int igdeNativeFoxTreeList::TreeListPadTop( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::treeListPaddingTop, DEFAULT_PAD );
}

int igdeNativeFoxTreeList::TreeListPadBottom( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::treeListPaddingBottom, DEFAULT_PAD );
}



// Events
///////////

long igdeNativeFoxTreeList::onExpanded( FXObject*, FXSelector, void *data ){
	const igdeNativeFoxTreeItem &nativeItem = *( ( igdeNativeFoxTreeItem* )data );
	
	try{
		nativeItem.GetOwner()->SetExpanded( true );
		pOwner->NotifyItemExpanded( nativeItem.GetOwner() );
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
		return 0;
	}
	
	return 0;
}

long igdeNativeFoxTreeList::onCollapsed( FXObject*, FXSelector, void *data ){
	const igdeNativeFoxTreeItem &nativeItem = *( ( igdeNativeFoxTreeItem* )data );
	
	try{
		nativeItem.GetOwner()->SetExpanded( false );
		pOwner->NotifyItemCollapsed( nativeItem.GetOwner() );
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
		return 0;
	}
	
	return 0;
}


long igdeNativeFoxTreeList::onListCommand( FXObject*, FXSelector, void* ){
	return 1;
}

long igdeNativeFoxTreeList::onListChanged( FXObject *sender, FXSelector selector, void *data ){
	if( ! pOwner->GetEnabled() ){
		return 0;
	}
	
	igdeNativeFoxTreeItem * const nativeItem = ( igdeNativeFoxTreeItem* )pTreeList->getCurrentItem();
	igdeTreeItem * const selection = nativeItem ? nativeItem->GetOwner() : NULL;
	
	try{
		pOwner->SetSelection( selection );
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
		return 0;
	}
	
	return 1;
}

long igdeNativeFoxTreeList::onListRightMouseDown( FXObject*, FXSelector, void *data ){
	if( ! pOwner->GetEnabled() ){
		return 1;
	}
	
	const FXEvent &event = *( ( FXEvent* )data );
	
	igdeNativeFoxTreeItem * const nativeItem = ( igdeNativeFoxTreeItem* )
		pTreeList->getItemAt( event.win_x, event.win_y );
	if( nativeItem ){
		pTreeList->setCurrentItem( nativeItem, true );
	}
	
	pOwner->ShowContextMenu( decPoint( event.win_x, event.win_y ) );
	return 1;
}

long igdeNativeFoxTreeList::onListRightMouseUp( FXObject*, FXSelector, void* ){
	return 1;
}

long igdeNativeFoxTreeList::onListDoubleClicked( FXObject *sender, FXSelector selector, void *data ){
	if( ! pOwner->GetEnabled() ){
		return 0;
	}
	
	igdeNativeFoxTreeItem * const item = ( igdeNativeFoxTreeItem* )data;
	
	try{
		pOwner->NotifyDoubleClickItem( item->GetOwner() );
		
	}catch( const deException &e ){
		pOwner->GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( pOwner, e );
	}
	return 0;
}



long igdeNativeFoxTreeList::onResizerDrag( FXObject*, FXSelector, void *data ){
	const int distance = igdeNativeFoxResizer::SelCommandDraggedDistance( data );
	const int newHeight = getHeight() + distance;
	
	const int LINE_SPACING = 4;  // hard coded in FXTreeList
	const int itemHeight = LINE_SPACING + pTreeList->getFont()->getFontHeight();
	
	pOwner->SetRows( decMath::max( ( newHeight + itemHeight / 2 ) / itemHeight, 1 ) );
	
	return 0;
}



// Private Functions
//////////////////////

void igdeNativeFoxTreeList::pDropItemsNativeWidget( igdeTreeItem *parent ){
	igdeTreeItem *child = parent ? parent->GetFirstChild() : pOwner->GetFirstChild();
	while( child ){
		child->SetNativeWidget( NULL );
		pDropItemsNativeWidget( child );
		child = child->GetNext();
	}
}
