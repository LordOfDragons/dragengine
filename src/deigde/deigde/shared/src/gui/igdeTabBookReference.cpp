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

#include "igdeTabBook.h"
#include "igdeTabBookReference.h"



// Class igdeTabBookReference
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTabBookReference::igdeTabBookReference(){
}

igdeTabBookReference::igdeTabBookReference( igdeTabBook *tabbook ) :
deObjectReference( tabbook ){
}

igdeTabBookReference::igdeTabBookReference( const igdeTabBookReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeTabBookReference::TakeOver( igdeTabBook *tabbook ){
	deObjectReference::TakeOver( tabbook );
}

bool igdeTabBookReference::operator!() const{
	return deObjectReference::operator!();
}

igdeTabBookReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeTabBookReference::operator igdeTabBook *() const{
	return ( igdeTabBook* )operator deObject *();
}

igdeTabBookReference::operator igdeTabBook &() const{
	return ( igdeTabBook& )operator deObject &();
}

igdeTabBook *igdeTabBookReference::operator->() const{
	return ( igdeTabBook* )deObjectReference::operator->();
}

igdeTabBookReference &igdeTabBookReference::operator=( igdeTabBook *tabbook ){
	deObjectReference::operator=( tabbook );
	return *this;
}

igdeTabBookReference &igdeTabBookReference::operator=( const igdeTabBookReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeTabBookReference::operator==( igdeTabBook *tabbook ) const{
	return deObjectReference::operator==( tabbook );
}

bool igdeTabBookReference::operator!=( igdeTabBook *tabbook ) const{
	return deObjectReference::operator!=( tabbook );
}

bool igdeTabBookReference::operator==( const igdeTabBookReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeTabBookReference::operator!=( const igdeTabBookReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
