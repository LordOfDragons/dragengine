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

#include "igdeEditVector2.h"
#include "igdeEditVector2Reference.h"



// Class igdeEditVector2Reference
///////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditVector2Reference::igdeEditVector2Reference(){
}

igdeEditVector2Reference::igdeEditVector2Reference( igdeEditVector2 *editVector2 ) :
deObjectReference( editVector2 ){
}

igdeEditVector2Reference::igdeEditVector2Reference( const igdeEditVector2Reference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditVector2Reference::TakeOver( igdeEditVector2 *editVector2 ){
	deObjectReference::TakeOver( editVector2 );
}

bool igdeEditVector2Reference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditVector2Reference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditVector2Reference::operator igdeEditVector2 *() const{
	return ( igdeEditVector2* )operator deObject *();
}

igdeEditVector2Reference::operator igdeEditVector2 &() const{
	return ( igdeEditVector2& )operator deObject &();
}

igdeEditVector2 *igdeEditVector2Reference::operator->() const{
	return ( igdeEditVector2* )deObjectReference::operator->();
}

igdeEditVector2Reference &igdeEditVector2Reference::operator=( igdeEditVector2 *editVector2 ){
	deObjectReference::operator=( editVector2 );
	return *this;
}

igdeEditVector2Reference &igdeEditVector2Reference::operator=( const igdeEditVector2Reference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditVector2Reference::operator==( igdeEditVector2 *editVector2 ) const{
	return deObjectReference::operator==( editVector2 );
}

bool igdeEditVector2Reference::operator!=( igdeEditVector2 *editVector2 ) const{
	return deObjectReference::operator!=( editVector2 );
}

bool igdeEditVector2Reference::operator==( const igdeEditVector2Reference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditVector2Reference::operator!=( const igdeEditVector2Reference &reference ) const{
	return deObjectReference::operator!=( reference );
}
