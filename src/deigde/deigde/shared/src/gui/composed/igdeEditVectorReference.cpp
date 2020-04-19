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

#include "igdeEditVector.h"
#include "igdeEditVectorReference.h"



// Class igdeEditVectorReference
//////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditVectorReference::igdeEditVectorReference(){
}

igdeEditVectorReference::igdeEditVectorReference( igdeEditVector *editVector ) :
deObjectReference( editVector ){
}

igdeEditVectorReference::igdeEditVectorReference( const igdeEditVectorReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditVectorReference::TakeOver( igdeEditVector *editVector ){
	deObjectReference::TakeOver( editVector );
}

bool igdeEditVectorReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditVectorReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditVectorReference::operator igdeEditVector *() const{
	return ( igdeEditVector* )operator deObject *();
}

igdeEditVectorReference::operator igdeEditVector &() const{
	return ( igdeEditVector& )operator deObject &();
}

igdeEditVector *igdeEditVectorReference::operator->() const{
	return ( igdeEditVector* )deObjectReference::operator->();
}

igdeEditVectorReference &igdeEditVectorReference::operator=( igdeEditVector *editVector ){
	deObjectReference::operator=( editVector );
	return *this;
}

igdeEditVectorReference &igdeEditVectorReference::operator=( const igdeEditVectorReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditVectorReference::operator==( igdeEditVector *editVector ) const{
	return deObjectReference::operator==( editVector );
}

bool igdeEditVectorReference::operator!=( igdeEditVector *editVector ) const{
	return deObjectReference::operator!=( editVector );
}

bool igdeEditVectorReference::operator==( const igdeEditVectorReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditVectorReference::operator!=( const igdeEditVectorReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
