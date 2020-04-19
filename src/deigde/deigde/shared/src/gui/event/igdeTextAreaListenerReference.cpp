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

#include "igdeTextAreaListener.h"
#include "igdeTextAreaListenerReference.h"



// Class igdeTextAreaListenerReference
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTextAreaListenerReference::igdeTextAreaListenerReference(){
}

igdeTextAreaListenerReference::igdeTextAreaListenerReference( igdeTextAreaListener *listener ) :
deObjectReference( listener ){
}

igdeTextAreaListenerReference::igdeTextAreaListenerReference( const igdeTextAreaListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeTextAreaListenerReference::TakeOver( igdeTextAreaListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeTextAreaListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeTextAreaListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeTextAreaListenerReference::operator igdeTextAreaListener *() const{
	return ( igdeTextAreaListener* )operator deObject *();
}

igdeTextAreaListenerReference::operator igdeTextAreaListener &() const{
	return ( igdeTextAreaListener& )operator deObject &();
}

igdeTextAreaListener *igdeTextAreaListenerReference::operator->() const{
	return ( igdeTextAreaListener* )deObjectReference::operator->();
}

igdeTextAreaListenerReference &igdeTextAreaListenerReference::operator=( igdeTextAreaListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeTextAreaListenerReference &igdeTextAreaListenerReference::operator=( const igdeTextAreaListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeTextAreaListenerReference::operator==( igdeTextAreaListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeTextAreaListenerReference::operator!=( igdeTextAreaListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeTextAreaListenerReference::operator==( const igdeTextAreaListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeTextAreaListenerReference::operator!=( const igdeTextAreaListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
