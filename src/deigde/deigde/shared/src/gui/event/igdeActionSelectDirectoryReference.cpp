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

#include "igdeActionSelectDirectory.h"
#include "igdeActionSelectDirectoryReference.h"



// Class igdeActionSelectDirectoryReference
/////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeActionSelectDirectoryReference::igdeActionSelectDirectoryReference(){
}

igdeActionSelectDirectoryReference::igdeActionSelectDirectoryReference( igdeActionSelectDirectory *action ) :
deObjectReference( action ){
}

igdeActionSelectDirectoryReference::igdeActionSelectDirectoryReference( const igdeActionSelectDirectoryReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeActionSelectDirectoryReference::TakeOver( igdeActionSelectDirectory *action ){
	deObjectReference::TakeOver( action );
}

bool igdeActionSelectDirectoryReference::operator!() const{
	return deObjectReference::operator!();
}

igdeActionSelectDirectoryReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeActionSelectDirectoryReference::operator igdeActionSelectDirectory *() const{
	return ( igdeActionSelectDirectory* )operator deObject *();
}

igdeActionSelectDirectoryReference::operator igdeActionSelectDirectory &() const{
	return ( igdeActionSelectDirectory& )operator deObject &();
}

igdeActionSelectDirectory *igdeActionSelectDirectoryReference::operator->() const{
	return ( igdeActionSelectDirectory* )deObjectReference::operator->();
}

igdeActionSelectDirectoryReference &igdeActionSelectDirectoryReference::operator=( igdeActionSelectDirectory *action ){
	deObjectReference::operator=( action );
	return *this;
}

igdeActionSelectDirectoryReference &igdeActionSelectDirectoryReference::operator=( const igdeActionSelectDirectoryReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeActionSelectDirectoryReference::operator==( igdeActionSelectDirectory *action ) const{
	return deObjectReference::operator==( action );
}

bool igdeActionSelectDirectoryReference::operator!=( igdeActionSelectDirectory *action ) const{
	return deObjectReference::operator!=( action );
}

bool igdeActionSelectDirectoryReference::operator==( const igdeActionSelectDirectoryReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeActionSelectDirectoryReference::operator!=( const igdeActionSelectDirectoryReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
