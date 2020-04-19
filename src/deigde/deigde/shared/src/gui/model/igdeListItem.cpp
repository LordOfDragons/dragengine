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

#include "igdeListItem.h"

#include <dragengine/common/exceptions.h>



// Class igdeListItem
///////////////////////

// Constructor, destructor
////////////////////////////

igdeListItem::igdeListItem( const char *text ) :
pText( text ),
pData( NULL ),
pSelected( false ){
}

igdeListItem::igdeListItem( const char *text, igdeIcon *icon ) :
pText( text ),
pIcon( icon ),
pData( NULL ){
}

igdeListItem::igdeListItem( const char *text, igdeIcon *icon, const char *description ) :
pText( text ),
pDescription( description ),
pIcon( icon ),
pData( NULL ),
pSelected( false ){
}

igdeListItem::igdeListItem( const char *text, void *data ) :
pText( text ),
pData( data ),
pSelected( false ){
}

igdeListItem::igdeListItem( const char *text, igdeIcon *icon, void *data ) :
pText( text ),
pIcon( icon ),
pData( data ),
pSelected( false ){
}

igdeListItem::igdeListItem( const char *text, igdeIcon *icon, const char *description, void *data ) :
pText( text ),
pDescription( description ),
pIcon( icon ),
pData( data ),
pSelected( false ){
}

igdeListItem::igdeListItem( const igdeListItem &listItem ) :
pText( listItem.pText ),
pDescription( listItem.pDescription ),
pIcon( listItem.pIcon ),
pData( listItem.pData ),
pDetails( listItem.pDetails ),
pSelected( false ){
}

igdeListItem::~igdeListItem(){
}



// Management
///////////////

void igdeListItem::SetText( const char *text ){
	pText = text;
}

void igdeListItem::SetDescription( const char *description ){
	pDescription = description;
}

void igdeListItem::SetIcon( igdeIcon *icon ){
	pIcon = icon;
}

void igdeListItem::SetData( void *data ){
	pData = data;
}

void igdeListItem::SetSelected( bool selected ){
	pSelected = selected;
}



igdeListItem &igdeListItem::operator=( const igdeListItem &listItem ){
	pText = listItem.pText;
	pDescription = listItem.pDescription;
	pIcon = listItem.pIcon;
	pData = listItem.pData;
	pDetails = listItem.pDetails;
	return *this;
}
