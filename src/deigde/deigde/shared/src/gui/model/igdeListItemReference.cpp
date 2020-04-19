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

#include "igdeListItem.h"
#include "igdeListItemReference.h"



// Class igdeListItemReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeListItemReference::igdeListItemReference(){
}

igdeListItemReference::igdeListItemReference( igdeListItem *listItem ) :
deObjectReference( listItem ){
}

igdeListItemReference::igdeListItemReference( const igdeListItemReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeListItemReference::TakeOver( igdeListItem *listItem ){
	deObjectReference::TakeOver( listItem );
}

bool igdeListItemReference::operator!() const{
	return deObjectReference::operator!();
}

igdeListItemReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeListItemReference::operator igdeListItem *() const{
	return ( igdeListItem* )operator deObject *();
}

igdeListItemReference::operator igdeListItem &() const{
	return ( igdeListItem& )operator deObject &();
}

igdeListItem *igdeListItemReference::operator->() const{
	return ( igdeListItem* )deObjectReference::operator->();
}

igdeListItemReference &igdeListItemReference::operator=( igdeListItem *listItem ){
	deObjectReference::operator=( listItem );
	return *this;
}

igdeListItemReference &igdeListItemReference::operator=( const igdeListItemReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeListItemReference::operator==( igdeListItem *listItem ) const{
	return deObjectReference::operator==( listItem );
}

bool igdeListItemReference::operator!=( igdeListItem *listItem ) const{
	return deObjectReference::operator!=( listItem );
}

bool igdeListItemReference::operator==( const igdeListItemReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeListItemReference::operator!=( const igdeListItemReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
