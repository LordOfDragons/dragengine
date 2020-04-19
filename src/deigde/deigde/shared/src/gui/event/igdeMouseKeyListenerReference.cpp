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

#include "igdeMouseKeyListener.h"
#include "igdeMouseKeyListenerReference.h"



// Class igdeMouseKeyListenerReference
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMouseKeyListenerReference::igdeMouseKeyListenerReference(){
}

igdeMouseKeyListenerReference::igdeMouseKeyListenerReference( igdeMouseKeyListener *listener ) :
deObjectReference( listener ){
}

igdeMouseKeyListenerReference::igdeMouseKeyListenerReference( const igdeMouseKeyListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeMouseKeyListenerReference::TakeOver( igdeMouseKeyListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeMouseKeyListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeMouseKeyListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeMouseKeyListenerReference::operator igdeMouseKeyListener *() const{
	return ( igdeMouseKeyListener* )operator deObject *();
}

igdeMouseKeyListenerReference::operator igdeMouseKeyListener &() const{
	return ( igdeMouseKeyListener& )operator deObject &();
}

igdeMouseKeyListener *igdeMouseKeyListenerReference::operator->() const{
	return ( igdeMouseKeyListener* )deObjectReference::operator->();
}

igdeMouseKeyListenerReference &igdeMouseKeyListenerReference::operator=( igdeMouseKeyListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeMouseKeyListenerReference &igdeMouseKeyListenerReference::operator=( const igdeMouseKeyListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeMouseKeyListenerReference::operator==( igdeMouseKeyListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeMouseKeyListenerReference::operator!=( igdeMouseKeyListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeMouseKeyListenerReference::operator==( const igdeMouseKeyListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeMouseKeyListenerReference::operator!=( const igdeMouseKeyListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
