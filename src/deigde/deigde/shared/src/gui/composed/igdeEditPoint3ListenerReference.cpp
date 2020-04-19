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

#include "igdeEditPoint3Listener.h"
#include "igdeEditPoint3ListenerReference.h"



// Class igdeEditPoint3ListenerReference
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditPoint3ListenerReference::igdeEditPoint3ListenerReference(){
}

igdeEditPoint3ListenerReference::igdeEditPoint3ListenerReference( igdeEditPoint3Listener *listener ) :
deObjectReference( listener ){
}

igdeEditPoint3ListenerReference::igdeEditPoint3ListenerReference( const igdeEditPoint3ListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditPoint3ListenerReference::TakeOver( igdeEditPoint3Listener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeEditPoint3ListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditPoint3ListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditPoint3ListenerReference::operator igdeEditPoint3Listener *() const{
	return ( igdeEditPoint3Listener* )operator deObject *();
}

igdeEditPoint3ListenerReference::operator igdeEditPoint3Listener &() const{
	return ( igdeEditPoint3Listener& )operator deObject &();
}

igdeEditPoint3Listener *igdeEditPoint3ListenerReference::operator->() const{
	return ( igdeEditPoint3Listener* )deObjectReference::operator->();
}

igdeEditPoint3ListenerReference &igdeEditPoint3ListenerReference::operator=( igdeEditPoint3Listener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeEditPoint3ListenerReference &igdeEditPoint3ListenerReference::operator=( const igdeEditPoint3ListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditPoint3ListenerReference::operator==( igdeEditPoint3Listener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeEditPoint3ListenerReference::operator!=( igdeEditPoint3Listener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeEditPoint3ListenerReference::operator==( const igdeEditPoint3ListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditPoint3ListenerReference::operator!=( const igdeEditPoint3ListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
