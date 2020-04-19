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

#include "deVideoAudioDecoder.h"
#include "deVideoAudioDecoderReference.h"



// Class deVideoAudioDecoderReference
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deVideoAudioDecoderReference::deVideoAudioDecoderReference(){
}

deVideoAudioDecoderReference::deVideoAudioDecoderReference( deVideoAudioDecoder *object ) :
deObjectReference( object ){
}

deVideoAudioDecoderReference::deVideoAudioDecoderReference( const deVideoAudioDecoderReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deVideoAudioDecoderReference::TakeOver( deVideoAudioDecoder *object ){
	deObjectReference::TakeOver( object );
}

bool deVideoAudioDecoderReference::operator!() const{
	return deObjectReference::operator!();
}

deVideoAudioDecoderReference::operator bool() const{
	return deObjectReference::operator bool();
}

deVideoAudioDecoderReference::operator deVideoAudioDecoder *() const{
	return ( deVideoAudioDecoder* )operator deObject *();
}

deVideoAudioDecoderReference::operator deVideoAudioDecoder &() const{
	return ( deVideoAudioDecoder& )operator deObject &();
}

deVideoAudioDecoder *deVideoAudioDecoderReference::operator->() const{
	return ( deVideoAudioDecoder* )deObjectReference::operator->();
}

deVideoAudioDecoderReference &deVideoAudioDecoderReference::operator=( deVideoAudioDecoder *object ){
	deObjectReference::operator=( object );
	return *this;
}

deVideoAudioDecoderReference &deVideoAudioDecoderReference::operator=( const deVideoAudioDecoderReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deVideoAudioDecoderReference::operator==( deVideoAudioDecoder *object ) const{
	return deObjectReference::operator==( object );
}

bool deVideoAudioDecoderReference::operator!=( deVideoAudioDecoder *object ) const{
	return deObjectReference::operator!=( object );
}

bool deVideoAudioDecoderReference::operator==( const deVideoAudioDecoderReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deVideoAudioDecoderReference::operator!=( const deVideoAudioDecoderReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
