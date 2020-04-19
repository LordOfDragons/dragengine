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

#include "igdeTriggerListener.h"
#include "igdeTriggerListenerReference.h"



// Class igdeTriggerListenerReference
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTriggerListenerReference::igdeTriggerListenerReference(){
}

igdeTriggerListenerReference::igdeTriggerListenerReference( igdeTriggerListener *listener ) :
deObjectReference( listener ){
}

igdeTriggerListenerReference::igdeTriggerListenerReference( const igdeTriggerListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeTriggerListenerReference::TakeOver( igdeTriggerListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeTriggerListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeTriggerListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeTriggerListenerReference::operator igdeTriggerListener *() const{
	return ( igdeTriggerListener* )operator deObject *();
}

igdeTriggerListenerReference::operator igdeTriggerListener &() const{
	return ( igdeTriggerListener& )operator deObject &();
}

igdeTriggerListener *igdeTriggerListenerReference::operator->() const{
	return ( igdeTriggerListener* )deObjectReference::operator->();
}

igdeTriggerListenerReference &igdeTriggerListenerReference::operator=( igdeTriggerListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeTriggerListenerReference &igdeTriggerListenerReference::operator=( const igdeTriggerListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeTriggerListenerReference::operator==( igdeTriggerListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeTriggerListenerReference::operator!=( igdeTriggerListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeTriggerListenerReference::operator==( const igdeTriggerListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeTriggerListenerReference::operator!=( const igdeTriggerListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
