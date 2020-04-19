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

#include "igdeTreeItem.h"

#include <dragengine/common/exceptions.h>



// Class igdeTreeItem
///////////////////////

// Constructor, destructor
////////////////////////////

igdeTreeItem::igdeTreeItem( const char *text ) :
pText( text ),
pParent( NULL ),
pPrevious( NULL ),
pData( NULL ),
pSelected( false ),
pExpanded( false ),
pNativeWidget( NULL ){
}

igdeTreeItem::igdeTreeItem( const char *text, igdeIcon *icon ) :
pText( text ),
pIcon( icon ),
pParent( NULL ),
pPrevious( NULL ),
pData( NULL ),
pExpanded( false ),
pNativeWidget( NULL ){
}

igdeTreeItem::igdeTreeItem( const char *text, igdeIcon *icon, const char *description ) :
pText( text ),
pDescription( description ),
pIcon( icon ),
pParent( NULL ),
pPrevious( NULL ),
pData( NULL ),
pSelected( false ),
pExpanded( false ),
pNativeWidget( NULL ){
}

igdeTreeItem::igdeTreeItem( const char *text, void *data ) :
pText( text ),
pParent( NULL ),
pPrevious( NULL ),
pData( data ),
pSelected( false ),
pExpanded( false ),
pNativeWidget( NULL ){
}

igdeTreeItem::igdeTreeItem( const char *text, igdeIcon *icon, void *data ) :
pText( text ),
pIcon( icon ),
pParent( NULL ),
pPrevious( NULL ),
pData( data ),
pSelected( false ),
pExpanded( false ),
pNativeWidget( NULL ){
}

igdeTreeItem::igdeTreeItem( const char *text, igdeIcon *icon, const char *description, void *data ) :
pText( text ),
pDescription( description ),
pIcon( icon ),
pParent( NULL ),
pPrevious( NULL ),
pData( data ),
pSelected( false ),
pExpanded( false ),
pNativeWidget( NULL ){
}

igdeTreeItem::igdeTreeItem( const igdeTreeItem &treeItem ) :
pText( treeItem.pText ),
pDescription( treeItem.pDescription ),
pIcon( treeItem.pIcon ),
pParent( NULL ),
pPrevious( NULL ),
pData( treeItem.pData ),
pSelected( false ),
pExpanded( treeItem.pExpanded ),
pNativeWidget( NULL ){
}

igdeTreeItem::~igdeTreeItem(){
}



// Management
///////////////

void igdeTreeItem::SetText( const char *text ){
	pText = text;
}

void igdeTreeItem::SetDescription( const char *description ){
	pDescription = description;
}

void igdeTreeItem::SetIcon( igdeIcon *icon ){
	pIcon = icon;
}

void igdeTreeItem::SetData( void *data ){
	pData = data;
}

void igdeTreeItem::SetSelected( bool selected ){
	pSelected = selected;
}

void igdeTreeItem::SetExpanded( bool expanded ){
	pExpanded = expanded;
}

void igdeTreeItem::SetParent( igdeTreeItem *item ){
	pParent = item;
}

void igdeTreeItem::SetPrevious( igdeTreeItem *item ){
	pPrevious = item;
}

void igdeTreeItem::SetNext( igdeTreeItem *item ){
	pNext = item;
}

void igdeTreeItem::SetFirstChild( igdeTreeItem *item ){
	pFirstChild = item;
}

igdeTreeItem *igdeTreeItem::GetLastChild() const{
	igdeTreeItem *item = pFirstChild;
	igdeTreeItem *lastChild = NULL;
	while( item ){
		lastChild = item;
		item = item->pNext;
	}
	return lastChild;
}

int igdeTreeItem::GetChildrenCount() const{
	int count = 0;
	igdeTreeItem *item = pFirstChild;
	while( item ){
		count++;
		item = item->pNext;
	}
	return count;
}



igdeTreeItem &igdeTreeItem::operator=( const igdeTreeItem &treeItem ){
	pText = treeItem.pText;
	pDescription = treeItem.pDescription;
	pIcon = treeItem.pIcon;
	pData = treeItem.pData;
	pExpanded = treeItem.pExpanded;
	return *this;
}



void igdeTreeItem::SetNativeWidget( void *nativeWidget ){
	pNativeWidget = nativeWidget;
}
