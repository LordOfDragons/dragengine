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

#include "igdeEditDVector.h"
#include "igdeEditDVectorReference.h"



// Class igdeEditDVectorReference
///////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditDVectorReference::igdeEditDVectorReference(){
}

igdeEditDVectorReference::igdeEditDVectorReference( igdeEditDVector *editDVector ) :
deObjectReference( editDVector ){
}

igdeEditDVectorReference::igdeEditDVectorReference( const igdeEditDVectorReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditDVectorReference::TakeOver( igdeEditDVector *editDVector ){
	deObjectReference::TakeOver( editDVector );
}

bool igdeEditDVectorReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditDVectorReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditDVectorReference::operator igdeEditDVector *() const{
	return ( igdeEditDVector* )operator deObject *();
}

igdeEditDVectorReference::operator igdeEditDVector &() const{
	return ( igdeEditDVector& )operator deObject &();
}

igdeEditDVector *igdeEditDVectorReference::operator->() const{
	return ( igdeEditDVector* )deObjectReference::operator->();
}

igdeEditDVectorReference &igdeEditDVectorReference::operator=( igdeEditDVector *editDVector ){
	deObjectReference::operator=( editDVector );
	return *this;
}

igdeEditDVectorReference &igdeEditDVectorReference::operator=( const igdeEditDVectorReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditDVectorReference::operator==( igdeEditDVector *editDVector ) const{
	return deObjectReference::operator==( editDVector );
}

bool igdeEditDVectorReference::operator!=( igdeEditDVector *editDVector ) const{
	return deObjectReference::operator!=( editDVector );
}

bool igdeEditDVectorReference::operator==( const igdeEditDVectorReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditDVectorReference::operator!=( const igdeEditDVectorReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
