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

#include "igdeTextFieldListener.h"
#include "igdeTextFieldListenerReference.h"



// Class igdeTextFieldListenerReference
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTextFieldListenerReference::igdeTextFieldListenerReference(){
}

igdeTextFieldListenerReference::igdeTextFieldListenerReference( igdeTextFieldListener *listener ) :
deObjectReference( listener ){
}

igdeTextFieldListenerReference::igdeTextFieldListenerReference( const igdeTextFieldListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeTextFieldListenerReference::TakeOver( igdeTextFieldListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeTextFieldListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeTextFieldListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeTextFieldListenerReference::operator igdeTextFieldListener *() const{
	return ( igdeTextFieldListener* )operator deObject *();
}

igdeTextFieldListenerReference::operator igdeTextFieldListener &() const{
	return ( igdeTextFieldListener& )operator deObject &();
}

igdeTextFieldListener *igdeTextFieldListenerReference::operator->() const{
	return ( igdeTextFieldListener* )deObjectReference::operator->();
}

igdeTextFieldListenerReference &igdeTextFieldListenerReference::operator=( igdeTextFieldListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeTextFieldListenerReference &igdeTextFieldListenerReference::operator=( const igdeTextFieldListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeTextFieldListenerReference::operator==( igdeTextFieldListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeTextFieldListenerReference::operator!=( igdeTextFieldListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeTextFieldListenerReference::operator==( const igdeTextFieldListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeTextFieldListenerReference::operator!=( const igdeTextFieldListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
