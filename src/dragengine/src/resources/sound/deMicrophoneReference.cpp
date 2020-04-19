/* 
 * Drag[en]gine Game Engine
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

#include "deMicrophone.h"
#include "deMicrophoneReference.h"



// Class deMicrophoneReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

deMicrophoneReference::deMicrophoneReference(){
}

deMicrophoneReference::deMicrophoneReference( deMicrophone *object ) :
deObjectReference( object ){
}

deMicrophoneReference::deMicrophoneReference( const deMicrophoneReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deMicrophoneReference::TakeOver( deMicrophone *object ){
	deObjectReference::TakeOver( object );
}

bool deMicrophoneReference::operator!() const{
	return deObjectReference::operator!();
}

deMicrophoneReference::operator bool() const{
	return deObjectReference::operator bool();
}

deMicrophoneReference::operator deMicrophone *() const{
	return ( deMicrophone* )operator deObject *();
}

deMicrophoneReference::operator deMicrophone &() const{
	return ( deMicrophone& )operator deObject &();
}

deMicrophone *deMicrophoneReference::operator->() const{
	return ( deMicrophone* )deObjectReference::operator->();
}

deMicrophoneReference &deMicrophoneReference::operator=( deMicrophone *object ){
	deObjectReference::operator=( object );
	return *this;
}

deMicrophoneReference &deMicrophoneReference::operator=( const deMicrophoneReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deMicrophoneReference::operator==( deMicrophone *object ) const{
	return deObjectReference::operator==( object );
}

bool deMicrophoneReference::operator!=( deMicrophone *object ) const{
	return deObjectReference::operator!=( object );
}

bool deMicrophoneReference::operator==( const deMicrophoneReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deMicrophoneReference::operator!=( const deMicrophoneReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
