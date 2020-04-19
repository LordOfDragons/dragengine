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

#include "igdeMouseCameraListener.h"
#include "igdeMouseCameraListenerReference.h"



// Class igdeMouseCameraListenerReference
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMouseCameraListenerReference::igdeMouseCameraListenerReference(){
}

igdeMouseCameraListenerReference::igdeMouseCameraListenerReference( igdeMouseCameraListener *listener ) :
deObjectReference( listener ){
}

igdeMouseCameraListenerReference::igdeMouseCameraListenerReference( const igdeMouseCameraListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeMouseCameraListenerReference::TakeOver( igdeMouseCameraListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeMouseCameraListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeMouseCameraListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeMouseCameraListenerReference::operator igdeMouseCameraListener *() const{
	return ( igdeMouseCameraListener* )operator deObject *();
}

igdeMouseCameraListenerReference::operator igdeMouseCameraListener &() const{
	return ( igdeMouseCameraListener& )operator deObject &();
}

igdeMouseCameraListener *igdeMouseCameraListenerReference::operator->() const{
	return ( igdeMouseCameraListener* )deObjectReference::operator->();
}

igdeMouseCameraListenerReference &igdeMouseCameraListenerReference::operator=( igdeMouseCameraListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeMouseCameraListenerReference &igdeMouseCameraListenerReference::operator=( const igdeMouseCameraListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeMouseCameraListenerReference::operator==( igdeMouseCameraListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeMouseCameraListenerReference::operator!=( igdeMouseCameraListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeMouseCameraListenerReference::operator==( const igdeMouseCameraListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeMouseCameraListenerReference::operator!=( const igdeMouseCameraListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
