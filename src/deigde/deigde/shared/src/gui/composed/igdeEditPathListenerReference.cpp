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

#include "igdeEditPathListener.h"
#include "igdeEditPathListenerReference.h"



// Class igdeEditPathListenerReference
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditPathListenerReference::igdeEditPathListenerReference(){
}

igdeEditPathListenerReference::igdeEditPathListenerReference( igdeEditPathListener *listener ) :
deObjectReference( listener ){
}

igdeEditPathListenerReference::igdeEditPathListenerReference( const igdeEditPathListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditPathListenerReference::TakeOver( igdeEditPathListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeEditPathListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditPathListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditPathListenerReference::operator igdeEditPathListener *() const{
	return ( igdeEditPathListener* )operator deObject *();
}

igdeEditPathListenerReference::operator igdeEditPathListener &() const{
	return ( igdeEditPathListener& )operator deObject &();
}

igdeEditPathListener *igdeEditPathListenerReference::operator->() const{
	return ( igdeEditPathListener* )deObjectReference::operator->();
}

igdeEditPathListenerReference &igdeEditPathListenerReference::operator=( igdeEditPathListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeEditPathListenerReference &igdeEditPathListenerReference::operator=( const igdeEditPathListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditPathListenerReference::operator==( igdeEditPathListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeEditPathListenerReference::operator!=( igdeEditPathListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeEditPathListenerReference::operator==( const igdeEditPathListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditPathListenerReference::operator!=( const igdeEditPathListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
