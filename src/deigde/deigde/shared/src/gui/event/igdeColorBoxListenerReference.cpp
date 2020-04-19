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

#include "igdeColorBoxListener.h"
#include "igdeColorBoxListenerReference.h"



// Class igdeColorBoxListenerReference
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeColorBoxListenerReference::igdeColorBoxListenerReference(){
}

igdeColorBoxListenerReference::igdeColorBoxListenerReference( igdeColorBoxListener *listener ) :
deObjectReference( listener ){
}

igdeColorBoxListenerReference::igdeColorBoxListenerReference( const igdeColorBoxListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeColorBoxListenerReference::TakeOver( igdeColorBoxListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeColorBoxListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeColorBoxListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeColorBoxListenerReference::operator igdeColorBoxListener *() const{
	return ( igdeColorBoxListener* )operator deObject *();
}

igdeColorBoxListenerReference::operator igdeColorBoxListener &() const{
	return ( igdeColorBoxListener& )operator deObject &();
}

igdeColorBoxListener *igdeColorBoxListenerReference::operator->() const{
	return ( igdeColorBoxListener* )deObjectReference::operator->();
}

igdeColorBoxListenerReference &igdeColorBoxListenerReference::operator=( igdeColorBoxListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeColorBoxListenerReference &igdeColorBoxListenerReference::operator=( const igdeColorBoxListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeColorBoxListenerReference::operator==( igdeColorBoxListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeColorBoxListenerReference::operator!=( igdeColorBoxListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeColorBoxListenerReference::operator==( const igdeColorBoxListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeColorBoxListenerReference::operator!=( const igdeColorBoxListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
