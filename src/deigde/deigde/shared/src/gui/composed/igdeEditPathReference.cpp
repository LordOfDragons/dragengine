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

#include "igdeEditPath.h"
#include "igdeEditPathReference.h"



// Class igdeEditPathReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditPathReference::igdeEditPathReference(){
}

igdeEditPathReference::igdeEditPathReference( igdeEditPath *editEditPath ) :
deObjectReference( editEditPath ){
}

igdeEditPathReference::igdeEditPathReference( const igdeEditPathReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditPathReference::TakeOver( igdeEditPath *editEditPath ){
	deObjectReference::TakeOver( editEditPath );
}

bool igdeEditPathReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditPathReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditPathReference::operator igdeEditPath *() const{
	return ( igdeEditPath* )operator deObject *();
}

igdeEditPathReference::operator igdeEditPath &() const{
	return ( igdeEditPath& )operator deObject &();
}

igdeEditPath *igdeEditPathReference::operator->() const{
	return ( igdeEditPath* )deObjectReference::operator->();
}

igdeEditPathReference &igdeEditPathReference::operator=( igdeEditPath *editEditPath ){
	deObjectReference::operator=( editEditPath );
	return *this;
}

igdeEditPathReference &igdeEditPathReference::operator=( const igdeEditPathReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditPathReference::operator==( igdeEditPath *editEditPath ) const{
	return deObjectReference::operator==( editEditPath );
}

bool igdeEditPathReference::operator!=( igdeEditPath *editEditPath ) const{
	return deObjectReference::operator!=( editEditPath );
}

bool igdeEditPathReference::operator==( const igdeEditPathReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditPathReference::operator!=( const igdeEditPathReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
