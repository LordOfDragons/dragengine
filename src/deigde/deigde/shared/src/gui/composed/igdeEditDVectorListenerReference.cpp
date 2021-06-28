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

#include "igdeEditDVectorListener.h"
#include "igdeEditDVectorListenerReference.h"



// Class igdeEditDVectorListenerReference
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditDVectorListenerReference::igdeEditDVectorListenerReference(){
}

igdeEditDVectorListenerReference::igdeEditDVectorListenerReference( igdeEditDVectorListener *listener ) :
deObjectReference( listener ){
}

igdeEditDVectorListenerReference::igdeEditDVectorListenerReference( const igdeEditDVectorListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditDVectorListenerReference::TakeOver( igdeEditDVectorListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeEditDVectorListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditDVectorListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditDVectorListenerReference::operator igdeEditDVectorListener *() const{
	return ( igdeEditDVectorListener* )operator deObject *();
}

igdeEditDVectorListenerReference::operator igdeEditDVectorListener &() const{
	return ( igdeEditDVectorListener& )operator deObject &();
}

igdeEditDVectorListener *igdeEditDVectorListenerReference::operator->() const{
	return ( igdeEditDVectorListener* )deObjectReference::operator->();
}

igdeEditDVectorListenerReference &igdeEditDVectorListenerReference::operator=( igdeEditDVectorListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeEditDVectorListenerReference &igdeEditDVectorListenerReference::operator=( const igdeEditDVectorListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditDVectorListenerReference::operator==( igdeEditDVectorListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeEditDVectorListenerReference::operator!=( igdeEditDVectorListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeEditDVectorListenerReference::operator==( const igdeEditDVectorListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditDVectorListenerReference::operator!=( const igdeEditDVectorListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
