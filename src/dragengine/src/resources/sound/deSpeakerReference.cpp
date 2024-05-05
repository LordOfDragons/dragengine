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
