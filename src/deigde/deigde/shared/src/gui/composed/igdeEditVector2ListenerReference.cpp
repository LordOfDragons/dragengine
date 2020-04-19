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

#include "igdeEditVector2Listener.h"
#include "igdeEditVector2ListenerReference.h"



// Class igdeEditVector2ListenerReference
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditVector2ListenerReference::igdeEditVector2ListenerReference(){
}

igdeEditVector2ListenerReference::igdeEditVector2ListenerReference( igdeEditVector2Listener *listener ) :
deObjectReference( listener ){
}

igdeEditVector2ListenerReference::igdeEditVector2ListenerReference( const igdeEditVector2ListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditVector2ListenerReference::TakeOver( igdeEditVector2Listener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeEditVector2ListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditVector2ListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditVector2ListenerReference::operator igdeEditVector2Listener *() const{
	return ( igdeEditVector2Listener* )operator deObject *();
}

igdeEditVector2ListenerReference::operator igdeEditVector2Listener &() const{
	return ( igdeEditVector2Listener& )operator deObject &();
}

igdeEditVector2Listener *igdeEditVector2ListenerReference::operator->() const{
	return ( igdeEditVector2Listener* )deObjectReference::operator->();
}

igdeEditVector2ListenerReference &igdeEditVector2ListenerReference::operator=( igdeEditVector2Listener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeEditVector2ListenerReference &igdeEditVector2ListenerReference::operator=( const igdeEditVector2ListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditVector2ListenerReference::operator==( igdeEditVector2Listener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeEditVector2ListenerReference::operator!=( igdeEditVector2Listener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeEditVector2ListenerReference::operator==( const igdeEditVector2ListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditVector2ListenerReference::operator!=( const igdeEditVector2ListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
