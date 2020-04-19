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

#include "igdeEditPoint3.h"
#include "igdeEditPoint3Reference.h"



// Class igdeEditPoint3Reference
//////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditPoint3Reference::igdeEditPoint3Reference(){
}

igdeEditPoint3Reference::igdeEditPoint3Reference( igdeEditPoint3 *editPoint3 ) :
deObjectReference( editPoint3 ){
}

igdeEditPoint3Reference::igdeEditPoint3Reference( const igdeEditPoint3Reference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditPoint3Reference::TakeOver( igdeEditPoint3 *editPoint3 ){
	deObjectReference::TakeOver( editPoint3 );
}

bool igdeEditPoint3Reference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditPoint3Reference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditPoint3Reference::operator igdeEditPoint3 *() const{
	return ( igdeEditPoint3* )operator deObject *();
}

igdeEditPoint3Reference::operator igdeEditPoint3 &() const{
	return ( igdeEditPoint3& )operator deObject &();
}

igdeEditPoint3 *igdeEditPoint3Reference::operator->() const{
	return ( igdeEditPoint3* )deObjectReference::operator->();
}

igdeEditPoint3Reference &igdeEditPoint3Reference::operator=( igdeEditPoint3 *editPoint3 ){
	deObjectReference::operator=( editPoint3 );
	return *this;
}

igdeEditPoint3Reference &igdeEditPoint3Reference::operator=( const igdeEditPoint3Reference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditPoint3Reference::operator==( igdeEditPoint3 *editPoint3 ) const{
	return deObjectReference::operator==( editPoint3 );
}

bool igdeEditPoint3Reference::operator!=( igdeEditPoint3 *editPoint3 ) const{
	return deObjectReference::operator!=( editPoint3 );
}

bool igdeEditPoint3Reference::operator==( const igdeEditPoint3Reference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditPoint3Reference::operator!=( const igdeEditPoint3Reference &reference ) const{
	return deObjectReference::operator!=( reference );
}
