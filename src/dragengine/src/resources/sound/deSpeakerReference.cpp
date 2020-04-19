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

#include "deSpeaker.h"
#include "deSpeakerReference.h"



// Class deSpeakerReference
/////////////////////////////

// Constructor, destructor
////////////////////////////

deSpeakerReference::deSpeakerReference(){
}

deSpeakerReference::deSpeakerReference( deSpeaker *object ) :
deObjectReference( object ){
}

deSpeakerReference::deSpeakerReference( const deSpeakerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deSpeakerReference::TakeOver( deSpeaker *object ){
	deObjectReference::TakeOver( object );
}

bool deSpeakerReference::operator!() const{
	return deObjectReference::operator!();
}

deSpeakerReference::operator bool() const{
	return deObjectReference::operator bool();
}

deSpeakerReference::operator deSpeaker *() const{
	return ( deSpeaker* )operator deObject *();
}

deSpeakerReference::operator deSpeaker &() const{
	return ( deSpeaker& )operator deObject &();
}

deSpeaker *deSpeakerReference::operator->() const{
	return ( deSpeaker* )deObjectReference::operator->();
}

deSpeakerReference &deSpeakerReference::operator=( deSpeaker *object ){
	deObjectReference::operator=( object );
	return *this;
}

deSpeakerReference &deSpeakerReference::operator=( const deSpeakerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deSpeakerReference::operator==( deSpeaker *object ) const{
	return deObjectReference::operator==( object );
}

bool deSpeakerReference::operator!=( deSpeaker *object ) const{
	return deObjectReference::operator!=( object );
}

bool deSpeakerReference::operator==( const deSpeakerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deSpeakerReference::operator!=( const deSpeakerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
