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

#include "deSoundDecoder.h"
#include "deSoundDecoderReference.h"



// Class deSoundDecoderReference
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deSoundDecoderReference::deSoundDecoderReference(){
}

deSoundDecoderReference::deSoundDecoderReference( deSoundDecoder *object ) :
deObjectReference( object ){
}

deSoundDecoderReference::deSoundDecoderReference( const deSoundDecoderReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deSoundDecoderReference::TakeOver( deSoundDecoder *object ){
	deObjectReference::TakeOver( object );
}

bool deSoundDecoderReference::operator!() const{
	return deObjectReference::operator!();
}

deSoundDecoderReference::operator bool() const{
	return deObjectReference::operator bool();
}

deSoundDecoderReference::operator deSoundDecoder *() const{
	return ( deSoundDecoder* )operator deObject *();
}

deSoundDecoderReference::operator deSoundDecoder &() const{
	return ( deSoundDecoder& )operator deObject &();
}

deSoundDecoder *deSoundDecoderReference::operator->() const{
	return ( deSoundDecoder* )deObjectReference::operator->();
}

deSoundDecoderReference &deSoundDecoderReference::operator=( deSoundDecoder *object ){
	deObjectReference::operator=( object );
	return *this;
}

deSoundDecoderReference &deSoundDecoderReference::operator=( const deSoundDecoderReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deSoundDecoderReference::operator==( deSoundDecoder *object ) const{
	return deObjectReference::operator==( object );
}

bool deSoundDecoderReference::operator!=( deSoundDecoder *object ) const{
	return deObjectReference::operator!=( object );
}

bool deSoundDecoderReference::operator==( const deSoundDecoderReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deSoundDecoderReference::operator!=( const deSoundDecoderReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
