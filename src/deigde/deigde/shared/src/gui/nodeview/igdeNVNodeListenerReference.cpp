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

#include "igdeNVNodeListener.h"
#include "igdeNVNodeListenerReference.h"



// Class igdeNVNodeListenerReference
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNVNodeListenerReference::igdeNVNodeListenerReference(){
}

igdeNVNodeListenerReference::igdeNVNodeListenerReference( igdeNVNodeListener *listener ) :
deObjectReference( listener ){
}

igdeNVNodeListenerReference::igdeNVNodeListenerReference( const igdeNVNodeListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeNVNodeListenerReference::TakeOver( igdeNVNodeListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeNVNodeListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeNVNodeListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeNVNodeListenerReference::operator igdeNVNodeListener *() const{
	return ( igdeNVNodeListener* )operator deObject *();
}

igdeNVNodeListenerReference::operator igdeNVNodeListener &() const{
	return ( igdeNVNodeListener& )operator deObject &();
}

igdeNVNodeListener *igdeNVNodeListenerReference::operator->() const{
	return ( igdeNVNodeListener* )deObjectReference::operator->();
}

igdeNVNodeListenerReference &igdeNVNodeListenerReference::operator=( igdeNVNodeListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeNVNodeListenerReference &igdeNVNodeListenerReference::operator=( const igdeNVNodeListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeNVNodeListenerReference::operator==( igdeNVNodeListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeNVNodeListenerReference::operator!=( igdeNVNodeListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeNVNodeListenerReference::operator==( const igdeNVNodeListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeNVNodeListenerReference::operator!=( const igdeNVNodeListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
