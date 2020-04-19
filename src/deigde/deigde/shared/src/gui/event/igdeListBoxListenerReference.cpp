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

#include "igdeListBoxListener.h"
#include "igdeListBoxListenerReference.h"



// Class igdeListBoxListenerReference
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeListBoxListenerReference::igdeListBoxListenerReference(){
}

igdeListBoxListenerReference::igdeListBoxListenerReference( igdeListBoxListener *listener ) :
deObjectReference( listener ){
}

igdeListBoxListenerReference::igdeListBoxListenerReference( const igdeListBoxListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeListBoxListenerReference::TakeOver( igdeListBoxListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeListBoxListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeListBoxListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeListBoxListenerReference::operator igdeListBoxListener *() const{
	return ( igdeListBoxListener* )operator deObject *();
}

igdeListBoxListenerReference::operator igdeListBoxListener &() const{
	return ( igdeListBoxListener& )operator deObject &();
}

igdeListBoxListener *igdeListBoxListenerReference::operator->() const{
	return ( igdeListBoxListener* )deObjectReference::operator->();
}

igdeListBoxListenerReference &igdeListBoxListenerReference::operator=( igdeListBoxListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeListBoxListenerReference &igdeListBoxListenerReference::operator=( const igdeListBoxListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeListBoxListenerReference::operator==( igdeListBoxListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeListBoxListenerReference::operator!=( igdeListBoxListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeListBoxListenerReference::operator==( const igdeListBoxListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeListBoxListenerReference::operator!=( const igdeListBoxListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
