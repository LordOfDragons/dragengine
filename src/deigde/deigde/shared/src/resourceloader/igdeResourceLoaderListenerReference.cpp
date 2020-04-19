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

#include "igdeResourceLoaderListener.h"
#include "igdeResourceLoaderListenerReference.h"



// Class igdeResourceLoaderListenerReference
//////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeResourceLoaderListenerReference::igdeResourceLoaderListenerReference(){
}

igdeResourceLoaderListenerReference::igdeResourceLoaderListenerReference( igdeResourceLoaderListener *listener ) :
deObjectReference( listener ){
}

igdeResourceLoaderListenerReference::igdeResourceLoaderListenerReference( const igdeResourceLoaderListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeResourceLoaderListenerReference::TakeOver( igdeResourceLoaderListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeResourceLoaderListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeResourceLoaderListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeResourceLoaderListenerReference::operator igdeResourceLoaderListener *() const{
	return ( igdeResourceLoaderListener* )operator deObject *();
}

igdeResourceLoaderListenerReference::operator igdeResourceLoaderListener &() const{
	return ( igdeResourceLoaderListener& )operator deObject &();
}

igdeResourceLoaderListener *igdeResourceLoaderListenerReference::operator->() const{
	return ( igdeResourceLoaderListener* )deObjectReference::operator->();
}

igdeResourceLoaderListenerReference &igdeResourceLoaderListenerReference::operator=( igdeResourceLoaderListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeResourceLoaderListenerReference &igdeResourceLoaderListenerReference::operator=( const igdeResourceLoaderListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeResourceLoaderListenerReference::operator==( igdeResourceLoaderListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeResourceLoaderListenerReference::operator!=( igdeResourceLoaderListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeResourceLoaderListenerReference::operator==( const igdeResourceLoaderListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeResourceLoaderListenerReference::operator!=( const igdeResourceLoaderListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
