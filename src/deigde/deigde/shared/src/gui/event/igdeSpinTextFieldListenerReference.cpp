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

#include "igdeSpinTextFieldListener.h"
#include "igdeSpinTextFieldListenerReference.h"



// Class igdeSpinTextFieldListenerReference
/////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeSpinTextFieldListenerReference::igdeSpinTextFieldListenerReference(){
}

igdeSpinTextFieldListenerReference::igdeSpinTextFieldListenerReference( igdeSpinTextFieldListener *listener ) :
deObjectReference( listener ){
}

igdeSpinTextFieldListenerReference::igdeSpinTextFieldListenerReference( const igdeSpinTextFieldListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeSpinTextFieldListenerReference::TakeOver( igdeSpinTextFieldListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeSpinTextFieldListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeSpinTextFieldListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeSpinTextFieldListenerReference::operator igdeSpinTextFieldListener *() const{
	return ( igdeSpinTextFieldListener* )operator deObject *();
}

igdeSpinTextFieldListenerReference::operator igdeSpinTextFieldListener &() const{
	return ( igdeSpinTextFieldListener& )operator deObject &();
}

igdeSpinTextFieldListener *igdeSpinTextFieldListenerReference::operator->() const{
	return ( igdeSpinTextFieldListener* )deObjectReference::operator->();
}

igdeSpinTextFieldListenerReference &igdeSpinTextFieldListenerReference::operator=( igdeSpinTextFieldListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeSpinTextFieldListenerReference &igdeSpinTextFieldListenerReference::operator=( const igdeSpinTextFieldListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeSpinTextFieldListenerReference::operator==( igdeSpinTextFieldListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeSpinTextFieldListenerReference::operator!=( igdeSpinTextFieldListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeSpinTextFieldListenerReference::operator==( const igdeSpinTextFieldListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeSpinTextFieldListenerReference::operator!=( const igdeSpinTextFieldListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
