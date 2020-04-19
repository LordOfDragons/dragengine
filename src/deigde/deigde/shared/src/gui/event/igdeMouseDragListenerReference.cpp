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

#include "igdeMouseDragListener.h"
#include "igdeMouseDragListenerReference.h"



// Class igdeMouseDragListenerReference
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMouseDragListenerReference::igdeMouseDragListenerReference(){
}

igdeMouseDragListenerReference::igdeMouseDragListenerReference( igdeMouseDragListener *listener ) :
deObjectReference( listener ){
}

igdeMouseDragListenerReference::igdeMouseDragListenerReference( const igdeMouseDragListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeMouseDragListenerReference::TakeOver( igdeMouseDragListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeMouseDragListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeMouseDragListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeMouseDragListenerReference::operator igdeMouseDragListener *() const{
	return ( igdeMouseDragListener* )operator deObject *();
}

igdeMouseDragListenerReference::operator igdeMouseDragListener &() const{
	return ( igdeMouseDragListener& )operator deObject &();
}

igdeMouseDragListener *igdeMouseDragListenerReference::operator->() const{
	return ( igdeMouseDragListener* )deObjectReference::operator->();
}

igdeMouseDragListenerReference &igdeMouseDragListenerReference::operator=( igdeMouseDragListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeMouseDragListenerReference &igdeMouseDragListenerReference::operator=( const igdeMouseDragListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeMouseDragListenerReference::operator==( igdeMouseDragListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeMouseDragListenerReference::operator!=( igdeMouseDragListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeMouseDragListenerReference::operator==( const igdeMouseDragListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeMouseDragListenerReference::operator!=( const igdeMouseDragListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
