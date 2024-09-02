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

#include "deSynthesizer.h"
#include "deSynthesizerReference.h"



// Class deSynthesizerReference
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deSynthesizerReference::deSynthesizerReference(){
}

deSynthesizerReference::deSynthesizerReference( deSynthesizer *object ) :
deObjectReference( object ){
}

deSynthesizerReference::deSynthesizerReference( const deSynthesizerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deSynthesizerReference::TakeOver( deSynthesizer *object ){
	deObjectReference::TakeOver( object );
}

bool deSynthesizerReference::operator!() const{
	return deObjectReference::operator!();
}

deSynthesizerReference::operator bool() const{
	return deObjectReference::operator bool();
}

deSynthesizerReference::operator deSynthesizer *() const{
	return ( deSynthesizer* )operator deObject *();
}

deSynthesizerReference::operator deSynthesizer &() const{
	return ( deSynthesizer& )operator deObject &();
}

deSynthesizer *deSynthesizerReference::operator->() const{
	return ( deSynthesizer* )deObjectReference::operator->();
}

deSynthesizerReference &deSynthesizerReference::operator=( deSynthesizer *object ){
	deObjectReference::operator=( object );
	return *this;
}

deSynthesizerReference &deSynthesizerReference::operator=( const deSynthesizerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deSynthesizerReference::operator==( deSynthesizer *object ) const{
	return deObjectReference::operator==( object );
}

bool deSynthesizerReference::operator!=( deSynthesizer *object ) const{
	return deObjectReference::operator!=( object );
}

bool deSynthesizerReference::operator==( const deSynthesizerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deSynthesizerReference::operator!=( const deSynthesizerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
