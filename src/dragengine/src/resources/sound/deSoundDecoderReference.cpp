/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
