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

#include "igdeAction.h"
#include "igdeActionReference.h"



// Class igdeActionReference
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeActionReference::igdeActionReference(){
}

igdeActionReference::igdeActionReference( igdeAction *action ) :
deObjectReference( action ){
}

igdeActionReference::igdeActionReference( const igdeActionReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeActionReference::TakeOver( igdeAction *action ){
	deObjectReference::TakeOver( action );
}

bool igdeActionReference::operator!() const{
	return deObjectReference::operator!();
}

igdeActionReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeActionReference::operator igdeAction *() const{
	return ( igdeAction* )operator deObject *();
}

igdeActionReference::operator igdeAction &() const{
	return ( igdeAction& )operator deObject &();
}

igdeAction *igdeActionReference::operator->() const{
	return ( igdeAction* )deObjectReference::operator->();
}

igdeActionReference &igdeActionReference::operator=( igdeAction *action ){
	deObjectReference::operator=( action );
	return *this;
}

igdeActionReference &igdeActionReference::operator=( const igdeActionReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeActionReference::operator==( igdeAction *action ) const{
	return deObjectReference::operator==( action );
}

bool igdeActionReference::operator!=( igdeAction *action ) const{
	return deObjectReference::operator!=( action );
}

bool igdeActionReference::operator==( const igdeActionReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeActionReference::operator!=( const igdeActionReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
