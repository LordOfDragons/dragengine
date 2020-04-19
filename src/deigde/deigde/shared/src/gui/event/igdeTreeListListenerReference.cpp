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

#include "igdeTreeListListener.h"
#include "igdeTreeListListenerReference.h"



// Class igdeTreeListListenerReference
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTreeListListenerReference::igdeTreeListListenerReference(){
}

igdeTreeListListenerReference::igdeTreeListListenerReference( igdeTreeListListener *listener ) :
deObjectReference( listener ){
}

igdeTreeListListenerReference::igdeTreeListListenerReference( const igdeTreeListListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeTreeListListenerReference::TakeOver( igdeTreeListListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeTreeListListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeTreeListListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeTreeListListenerReference::operator igdeTreeListListener *() const{
	return ( igdeTreeListListener* )operator deObject *();
}

igdeTreeListListenerReference::operator igdeTreeListListener &() const{
	return ( igdeTreeListListener& )operator deObject &();
}

igdeTreeListListener *igdeTreeListListenerReference::operator->() const{
	return ( igdeTreeListListener* )deObjectReference::operator->();
}

igdeTreeListListenerReference &igdeTreeListListenerReference::operator=( igdeTreeListListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeTreeListListenerReference &igdeTreeListListenerReference::operator=( const igdeTreeListListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeTreeListListenerReference::operator==( igdeTreeListListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeTreeListListenerReference::operator!=( igdeTreeListListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeTreeListListenerReference::operator==( const igdeTreeListListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeTreeListListenerReference::operator!=( const igdeTreeListListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
