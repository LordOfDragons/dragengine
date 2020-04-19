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

#include "igdeScrollBarListener.h"
#include "igdeScrollBarListenerReference.h"



// Class igdeScrollBarListenerReference
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeScrollBarListenerReference::igdeScrollBarListenerReference(){
}

igdeScrollBarListenerReference::igdeScrollBarListenerReference( igdeScrollBarListener *listener ) :
deObjectReference( listener ){
}

igdeScrollBarListenerReference::igdeScrollBarListenerReference( const igdeScrollBarListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeScrollBarListenerReference::TakeOver( igdeScrollBarListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeScrollBarListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeScrollBarListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeScrollBarListenerReference::operator igdeScrollBarListener *() const{
	return ( igdeScrollBarListener* )operator deObject *();
}

igdeScrollBarListenerReference::operator igdeScrollBarListener &() const{
	return ( igdeScrollBarListener& )operator deObject &();
}

igdeScrollBarListener *igdeScrollBarListenerReference::operator->() const{
	return ( igdeScrollBarListener* )deObjectReference::operator->();
}

igdeScrollBarListenerReference &igdeScrollBarListenerReference::operator=( igdeScrollBarListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeScrollBarListenerReference &igdeScrollBarListenerReference::operator=( const igdeScrollBarListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeScrollBarListenerReference::operator==( igdeScrollBarListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeScrollBarListenerReference::operator!=( igdeScrollBarListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeScrollBarListenerReference::operator==( const igdeScrollBarListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeScrollBarListenerReference::operator!=( const igdeScrollBarListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
