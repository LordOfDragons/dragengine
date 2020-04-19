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

#include "igdeActionExternOpen.h"
#include "igdeActionExternOpenReference.h"



// Class igdeActionExternOpenReference
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeActionExternOpenReference::igdeActionExternOpenReference(){
}

igdeActionExternOpenReference::igdeActionExternOpenReference( igdeActionExternOpen *action ) :
deObjectReference( action ){
}

igdeActionExternOpenReference::igdeActionExternOpenReference( const igdeActionExternOpenReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeActionExternOpenReference::TakeOver( igdeActionExternOpen *action ){
	deObjectReference::TakeOver( action );
}

bool igdeActionExternOpenReference::operator!() const{
	return deObjectReference::operator!();
}

igdeActionExternOpenReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeActionExternOpenReference::operator igdeActionExternOpen *() const{
	return ( igdeActionExternOpen* )operator deObject *();
}

igdeActionExternOpenReference::operator igdeActionExternOpen &() const{
	return ( igdeActionExternOpen& )operator deObject &();
}

igdeActionExternOpen *igdeActionExternOpenReference::operator->() const{
	return ( igdeActionExternOpen* )deObjectReference::operator->();
}

igdeActionExternOpenReference &igdeActionExternOpenReference::operator=( igdeActionExternOpen *action ){
	deObjectReference::operator=( action );
	return *this;
}

igdeActionExternOpenReference &igdeActionExternOpenReference::operator=( const igdeActionExternOpenReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeActionExternOpenReference::operator==( igdeActionExternOpen *action ) const{
	return deObjectReference::operator==( action );
}

bool igdeActionExternOpenReference::operator!=( igdeActionExternOpen *action ) const{
	return deObjectReference::operator!=( action );
}

bool igdeActionExternOpenReference::operator==( const igdeActionExternOpenReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeActionExternOpenReference::operator!=( const igdeActionExternOpenReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
