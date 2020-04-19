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

#include "igdeSliderListener.h"
#include "igdeSliderListenerReference.h"



// Class igdeSliderListenerReference
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeSliderListenerReference::igdeSliderListenerReference(){
}

igdeSliderListenerReference::igdeSliderListenerReference( igdeSliderListener *listener ) :
deObjectReference( listener ){
}

igdeSliderListenerReference::igdeSliderListenerReference( const igdeSliderListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeSliderListenerReference::TakeOver( igdeSliderListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeSliderListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeSliderListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeSliderListenerReference::operator igdeSliderListener *() const{
	return ( igdeSliderListener* )operator deObject *();
}

igdeSliderListenerReference::operator igdeSliderListener &() const{
	return ( igdeSliderListener& )operator deObject &();
}

igdeSliderListener *igdeSliderListenerReference::operator->() const{
	return ( igdeSliderListener* )deObjectReference::operator->();
}

igdeSliderListenerReference &igdeSliderListenerReference::operator=( igdeSliderListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeSliderListenerReference &igdeSliderListenerReference::operator=( const igdeSliderListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeSliderListenerReference::operator==( igdeSliderListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeSliderListenerReference::operator!=( igdeSliderListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeSliderListenerReference::operator==( const igdeSliderListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeSliderListenerReference::operator!=( const igdeSliderListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
