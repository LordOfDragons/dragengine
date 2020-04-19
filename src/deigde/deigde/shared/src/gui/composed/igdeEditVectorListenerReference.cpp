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

#include "igdeEditVectorListener.h"
#include "igdeEditVectorListenerReference.h"



// Class igdeEditVectorListenerReference
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditVectorListenerReference::igdeEditVectorListenerReference(){
}

igdeEditVectorListenerReference::igdeEditVectorListenerReference( igdeEditVectorListener *listener ) :
deObjectReference( listener ){
}

igdeEditVectorListenerReference::igdeEditVectorListenerReference( const igdeEditVectorListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditVectorListenerReference::TakeOver( igdeEditVectorListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeEditVectorListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditVectorListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditVectorListenerReference::operator igdeEditVectorListener *() const{
	return ( igdeEditVectorListener* )operator deObject *();
}

igdeEditVectorListenerReference::operator igdeEditVectorListener &() const{
	return ( igdeEditVectorListener& )operator deObject &();
}

igdeEditVectorListener *igdeEditVectorListenerReference::operator->() const{
	return ( igdeEditVectorListener* )deObjectReference::operator->();
}

igdeEditVectorListenerReference &igdeEditVectorListenerReference::operator=( igdeEditVectorListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeEditVectorListenerReference &igdeEditVectorListenerReference::operator=( const igdeEditVectorListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditVectorListenerReference::operator==( igdeEditVectorListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeEditVectorListenerReference::operator!=( igdeEditVectorListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeEditVectorListenerReference::operator==( const igdeEditVectorListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditVectorListenerReference::operator!=( const igdeEditVectorListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
