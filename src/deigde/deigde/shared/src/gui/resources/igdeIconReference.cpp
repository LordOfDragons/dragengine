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

#include "igdeIcon.h"
#include "igdeIconReference.h"



// Class igdeIconReference
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeIconReference::igdeIconReference(){
}

igdeIconReference::igdeIconReference( igdeIcon *icon ) :
deObjectReference( icon ){
}

igdeIconReference::igdeIconReference( const igdeIconReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeIconReference::TakeOver( igdeIcon *icon ){
	deObjectReference::TakeOver( icon );
}

bool igdeIconReference::operator!() const{
	return deObjectReference::operator!();
}

igdeIconReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeIconReference::operator igdeIcon *() const{
	return ( igdeIcon* )operator deObject *();
}

igdeIconReference::operator igdeIcon &() const{
	return ( igdeIcon& )operator deObject &();
}

igdeIcon *igdeIconReference::operator->() const{
	return ( igdeIcon* )deObjectReference::operator->();
}

igdeIconReference &igdeIconReference::operator=( igdeIcon *icon ){
	deObjectReference::operator=( icon );
	return *this;
}

igdeIconReference &igdeIconReference::operator=( const igdeIconReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeIconReference::operator==( igdeIcon *icon ) const{
	return deObjectReference::operator==( icon );
}

bool igdeIconReference::operator!=( igdeIcon *icon ) const{
	return deObjectReference::operator!=( icon );
}

bool igdeIconReference::operator==( const igdeIconReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeIconReference::operator!=( const igdeIconReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
