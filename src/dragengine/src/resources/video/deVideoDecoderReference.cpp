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

#include "deVideoDecoder.h"
#include "deVideoDecoderReference.h"



// Class deVideoDecoderReference
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deVideoDecoderReference::deVideoDecoderReference(){
}

deVideoDecoderReference::deVideoDecoderReference( deVideoDecoder *object ) :
deObjectReference( object ){
}

deVideoDecoderReference::deVideoDecoderReference( const deVideoDecoderReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deVideoDecoderReference::TakeOver( deVideoDecoder *object ){
	deObjectReference::TakeOver( object );
}

bool deVideoDecoderReference::operator!() const{
	return deObjectReference::operator!();
}

deVideoDecoderReference::operator bool() const{
	return deObjectReference::operator bool();
}

deVideoDecoderReference::operator deVideoDecoder *() const{
	return ( deVideoDecoder* )operator deObject *();
}

deVideoDecoderReference::operator deVideoDecoder &() const{
	return ( deVideoDecoder& )operator deObject &();
}

deVideoDecoder *deVideoDecoderReference::operator->() const{
	return ( deVideoDecoder* )deObjectReference::operator->();
}

deVideoDecoderReference &deVideoDecoderReference::operator=( deVideoDecoder *object ){
	deObjectReference::operator=( object );
	return *this;
}

deVideoDecoderReference &deVideoDecoderReference::operator=( const deVideoDecoderReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deVideoDecoderReference::operator==( deVideoDecoder *object ) const{
	return deObjectReference::operator==( object );
}

bool deVideoDecoderReference::operator!=( deVideoDecoder *object ) const{
	return deObjectReference::operator!=( object );
}

bool deVideoDecoderReference::operator==( const deVideoDecoderReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deVideoDecoderReference::operator!=( const deVideoDecoderReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
