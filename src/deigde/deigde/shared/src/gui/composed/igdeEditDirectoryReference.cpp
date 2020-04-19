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

#include "igdeEditDirectory.h"
#include "igdeEditDirectoryReference.h"



// Class igdeEditDirectoryReference
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditDirectoryReference::igdeEditDirectoryReference(){
}

igdeEditDirectoryReference::igdeEditDirectoryReference( igdeEditDirectory *editEditDirectory ) :
deObjectReference( editEditDirectory ){
}

igdeEditDirectoryReference::igdeEditDirectoryReference( const igdeEditDirectoryReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditDirectoryReference::TakeOver( igdeEditDirectory *editEditDirectory ){
	deObjectReference::TakeOver( editEditDirectory );
}

bool igdeEditDirectoryReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditDirectoryReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditDirectoryReference::operator igdeEditDirectory *() const{
	return ( igdeEditDirectory* )operator deObject *();
}

igdeEditDirectoryReference::operator igdeEditDirectory &() const{
	return ( igdeEditDirectory& )operator deObject &();
}

igdeEditDirectory *igdeEditDirectoryReference::operator->() const{
	return ( igdeEditDirectory* )deObjectReference::operator->();
}

igdeEditDirectoryReference &igdeEditDirectoryReference::operator=( igdeEditDirectory *editEditDirectory ){
	deObjectReference::operator=( editEditDirectory );
	return *this;
}

igdeEditDirectoryReference &igdeEditDirectoryReference::operator=( const igdeEditDirectoryReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditDirectoryReference::operator==( igdeEditDirectory *editEditDirectory ) const{
	return deObjectReference::operator==( editEditDirectory );
}

bool igdeEditDirectoryReference::operator!=( igdeEditDirectory *editEditDirectory ) const{
	return deObjectReference::operator!=( editEditDirectory );
}

bool igdeEditDirectoryReference::operator==( const igdeEditDirectoryReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditDirectoryReference::operator!=( const igdeEditDirectoryReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
