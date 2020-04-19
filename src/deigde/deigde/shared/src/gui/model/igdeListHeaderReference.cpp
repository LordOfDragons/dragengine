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

#include "igdeListHeader.h"
#include "igdeListHeaderReference.h"



// Class igdeListHeaderReference
//////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeListHeaderReference::igdeListHeaderReference(){
}

igdeListHeaderReference::igdeListHeaderReference( igdeListHeader *header ) :
deObjectReference( header ){
}

igdeListHeaderReference::igdeListHeaderReference( const igdeListHeaderReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeListHeaderReference::TakeOver( igdeListHeader *header ){
	deObjectReference::TakeOver( header );
}

bool igdeListHeaderReference::operator!() const{
	return deObjectReference::operator!();
}

igdeListHeaderReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeListHeaderReference::operator igdeListHeader *() const{
	return ( igdeListHeader* )operator deObject *();
}

igdeListHeaderReference::operator igdeListHeader &() const{
	return ( igdeListHeader& )operator deObject &();
}

igdeListHeader *igdeListHeaderReference::operator->() const{
	return ( igdeListHeader* )deObjectReference::operator->();
}

igdeListHeaderReference &igdeListHeaderReference::operator=( igdeListHeader *header ){
	deObjectReference::operator=( header );
	return *this;
}

igdeListHeaderReference &igdeListHeaderReference::operator=( const igdeListHeaderReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeListHeaderReference::operator==( igdeListHeader *header ) const{
	return deObjectReference::operator==( header );
}

bool igdeListHeaderReference::operator!=( igdeListHeader *header ) const{
	return deObjectReference::operator!=( header );
}

bool igdeListHeaderReference::operator==( const igdeListHeaderReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeListHeaderReference::operator!=( const igdeListHeaderReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
