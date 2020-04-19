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

#include "igdeViewCurveBezierListener.h"
#include "igdeViewCurveBezierListenerReference.h"



// Class igdeViewCurveBezierListenerReference
///////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeViewCurveBezierListenerReference::igdeViewCurveBezierListenerReference(){
}

igdeViewCurveBezierListenerReference::igdeViewCurveBezierListenerReference( igdeViewCurveBezierListener *listener ) :
deObjectReference( listener ){
}

igdeViewCurveBezierListenerReference::igdeViewCurveBezierListenerReference( const igdeViewCurveBezierListenerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeViewCurveBezierListenerReference::TakeOver( igdeViewCurveBezierListener *listener ){
	deObjectReference::TakeOver( listener );
}

bool igdeViewCurveBezierListenerReference::operator!() const{
	return deObjectReference::operator!();
}

igdeViewCurveBezierListenerReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeViewCurveBezierListenerReference::operator igdeViewCurveBezierListener *() const{
	return ( igdeViewCurveBezierListener* )operator deObject *();
}

igdeViewCurveBezierListenerReference::operator igdeViewCurveBezierListener &() const{
	return ( igdeViewCurveBezierListener& )operator deObject &();
}

igdeViewCurveBezierListener *igdeViewCurveBezierListenerReference::operator->() const{
	return ( igdeViewCurveBezierListener* )deObjectReference::operator->();
}

igdeViewCurveBezierListenerReference &igdeViewCurveBezierListenerReference::operator=( igdeViewCurveBezierListener *listener ){
	deObjectReference::operator=( listener );
	return *this;
}

igdeViewCurveBezierListenerReference &igdeViewCurveBezierListenerReference::operator=( const igdeViewCurveBezierListenerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeViewCurveBezierListenerReference::operator==( igdeViewCurveBezierListener *listener ) const{
	return deObjectReference::operator==( listener );
}

bool igdeViewCurveBezierListenerReference::operator!=( igdeViewCurveBezierListener *listener ) const{
	return deObjectReference::operator!=( listener );
}

bool igdeViewCurveBezierListenerReference::operator==( const igdeViewCurveBezierListenerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeViewCurveBezierListenerReference::operator!=( const igdeViewCurveBezierListenerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
