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

#include "igdeEditDirectoryListener.h"
#include "igdeEditDirectoryListenerReference.h"



// Class igdeEditDirectoryListenerReference
/////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditDirectoryListenerReference::igdeEditDirectoryListenerReference(){
}

igdeEditDirectoryListenerReference::igdeEditDirectoryListenerReference( igdeEditDirectoryListener *listener ) :
deObjectReference( listener ){
}

igdeEditDirectoryListenerReference::igdeEditDirectoryListenerReference( const igdeEditDirectoryListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeEditDirectoryListenerReference::TakeOver( igdeEditDirectoryListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeEditDirectoryListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeEditDirectoryListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeEditDirectoryListenerReference::operator igdeEditDirectoryListener *() const{
	return ( igdeEditDirectoryListener* )operator deObject *();
}

igdeEditDirectoryListenerReference::operator igdeEditDirectoryListener &() const{
	return ( igdeEditDirectoryListener& )operator deObject &();
}

igdeEditDirectoryListener *igdeEditDirectoryListenerReference::operator->() const{
	return ( igdeEditDirectoryListener* )deObjectReference::operator->();
}

igdeEditDirectoryListenerReference &igdeEditDirectoryListenerReference::operator=( igdeEditDirectoryListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeEditDirectoryListenerReference &igdeEditDirectoryListenerReference::operator=( const igdeEditDirectoryListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeEditDirectoryListenerReference::operator==( igdeEditDirectoryListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeEditDirectoryListenerReference::operator!=( igdeEditDirectoryListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeEditDirectoryListenerReference::operator==( const igdeEditDirectoryListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeEditDirectoryListenerReference::operator!=( const igdeEditDirectoryListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
