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

#include "igdeComboBoxListener.h"
#include "igdeComboBoxListenerReference.h"



// Class igdeComboBoxListenerReference
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeComboBoxListenerReference::igdeComboBoxListenerReference(){
}

igdeComboBoxListenerReference::igdeComboBoxListenerReference( igdeComboBoxListener *listener ) :
deObjectReference( listener ){
}

igdeComboBoxListenerReference::igdeComboBoxListenerReference( const igdeComboBoxListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeComboBoxListenerReference::TakeOver( igdeComboBoxListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeComboBoxListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeComboBoxListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeComboBoxListenerReference::operator igdeComboBoxListener *() const{
	return ( igdeComboBoxListener* )operator deObject *();
}

igdeComboBoxListenerReference::operator igdeComboBoxListener &() const{
	return ( igdeComboBoxListener& )operator deObject &();
}

igdeComboBoxListener *igdeComboBoxListenerReference::operator->() const{
	return ( igdeComboBoxListener* )deObjectReference::operator->();
}

igdeComboBoxListenerReference &igdeComboBoxListenerReference::operator=( igdeComboBoxListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeComboBoxListenerReference &igdeComboBoxListenerReference::operator=( const igdeComboBoxListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeComboBoxListenerReference::operator==( igdeComboBoxListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeComboBoxListenerReference::operator!=( igdeComboBoxListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeComboBoxListenerReference::operator==( const igdeComboBoxListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeComboBoxListenerReference::operator!=( const igdeComboBoxListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
