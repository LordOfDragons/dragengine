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

#include "igdeEditPropertyValueListener.h"
#include "igdeEditPropertyValueListenerReference.h"



// Class igdeEditPropertyValueListenerReference
/////////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditPropertyValueListenerReference::igdeEditPropertyValueListenerReference(){
}

igdeEditPropertyValueListenerReference::igdeEditPropertyValueListenerReference( igdeEditPropertyValueListener *listener ) :
deObjectReference( listener ){
}

igdeEditPropertyValueListenerReference::igdeEditPropertyValueListenerReference( const igdeEditPropertyValueListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditPropertyValueListenerReference::TakeOver( igdeEditPropertyValueListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeEditPropertyValueListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditPropertyValueListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditPropertyValueListenerReference::operator igdeEditPropertyValueListener *() const{
	return ( igdeEditPropertyValueListener* )operator deObject *();
}

igdeEditPropertyValueListenerReference::operator igdeEditPropertyValueListener &() const{
	return ( igdeEditPropertyValueListener& )operator deObject &();
}

igdeEditPropertyValueListener *igdeEditPropertyValueListenerReference::operator->() const{
	return ( igdeEditPropertyValueListener* )deObjectReference::operator->();
}

igdeEditPropertyValueListenerReference &igdeEditPropertyValueListenerReference::operator=( igdeEditPropertyValueListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeEditPropertyValueListenerReference &igdeEditPropertyValueListenerReference::operator=( const igdeEditPropertyValueListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditPropertyValueListenerReference::operator==( igdeEditPropertyValueListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeEditPropertyValueListenerReference::operator!=( igdeEditPropertyValueListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeEditPropertyValueListenerReference::operator==( const igdeEditPropertyValueListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditPropertyValueListenerReference::operator!=( const igdeEditPropertyValueListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
