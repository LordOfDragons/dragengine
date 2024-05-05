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
