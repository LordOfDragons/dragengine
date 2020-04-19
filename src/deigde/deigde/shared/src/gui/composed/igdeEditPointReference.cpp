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

#include "igdeEditPoint.h"
#include "igdeEditPointReference.h"



// Class igdeEditPointReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditPointReference::igdeEditPointReference(){
}

igdeEditPointReference::igdeEditPointReference( igdeEditPoint *editPoint ) :
deObjectReference( editPoint ){
}

igdeEditPointReference::igdeEditPointReference( const igdeEditPointReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditPointReference::TakeOver( igdeEditPoint *editPoint ){
	deObjectReference::TakeOver( editPoint );
}

bool igdeEditPointReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditPointReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditPointReference::operator igdeEditPoint *() const{
	return ( igdeEditPoint* )operator deObject *();
}

igdeEditPointReference::operator igdeEditPoint &() const{
	return ( igdeEditPoint& )operator deObject &();
}

igdeEditPoint *igdeEditPointReference::operator->() const{
	return ( igdeEditPoint* )deObjectReference::operator->();
}

igdeEditPointReference &igdeEditPointReference::operator=( igdeEditPoint *editPoint ){
	deObjectReference::operator=( editPoint );
	return *this;
}

igdeEditPointReference &igdeEditPointReference::operator=( const igdeEditPointReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditPointReference::operator==( igdeEditPoint *editPoint ) const{
	return deObjectReference::operator==( editPoint );
}

bool igdeEditPointReference::operator!=( igdeEditPoint *editPoint ) const{
	return deObjectReference::operator!=( editPoint );
}

bool igdeEditPointReference::operator==( const igdeEditPointReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditPointReference::operator!=( const igdeEditPointReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
