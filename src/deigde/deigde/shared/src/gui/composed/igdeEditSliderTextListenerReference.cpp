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

#include "igdeEditSliderTextListener.h"
#include "igdeEditSliderTextListenerReference.h"



// Class igdeEditSliderTextListenerReference
//////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditSliderTextListenerReference::igdeEditSliderTextListenerReference(){
}

igdeEditSliderTextListenerReference::igdeEditSliderTextListenerReference( igdeEditSliderTextListener *listener ) :
deObjectReference( listener ){
}

igdeEditSliderTextListenerReference::igdeEditSliderTextListenerReference( const igdeEditSliderTextListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditSliderTextListenerReference::TakeOver( igdeEditSliderTextListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeEditSliderTextListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditSliderTextListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditSliderTextListenerReference::operator igdeEditSliderTextListener *() const{
	return ( igdeEditSliderTextListener* )operator deObject *();
}

igdeEditSliderTextListenerReference::operator igdeEditSliderTextListener &() const{
	return ( igdeEditSliderTextListener& )operator deObject &();
}

igdeEditSliderTextListener *igdeEditSliderTextListenerReference::operator->() const{
	return ( igdeEditSliderTextListener* )deObjectReference::operator->();
}

igdeEditSliderTextListenerReference &igdeEditSliderTextListenerReference::operator=( igdeEditSliderTextListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeEditSliderTextListenerReference &igdeEditSliderTextListenerReference::operator=( const igdeEditSliderTextListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditSliderTextListenerReference::operator==( igdeEditSliderTextListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeEditSliderTextListenerReference::operator!=( igdeEditSliderTextListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeEditSliderTextListenerReference::operator==( const igdeEditSliderTextListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditSliderTextListenerReference::operator!=( const igdeEditSliderTextListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
