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

#include "deSound.h"
#include "deSoundReference.h"



// Class deSoundReference
///////////////////////////

// Constructor, destructor
////////////////////////////

deSoundReference::deSoundReference(){
}

deSoundReference::deSoundReference( deSound *object ) :
deObjectReference( object ){
}

deSoundReference::deSoundReference( const deSoundReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deSoundReference::TakeOver( deSound *object ){
	deObjectReference::TakeOver( object );
}

bool deSoundReference::operator!() const{
	return deObjectReference::operator!();
}

deSoundReference::operator bool() const{
	return deObjectReference::operator bool();
}

deSoundReference::operator deSound *() const{
	return ( deSound* )operator deObject *();
}

deSoundReference::operator deSound &() const{
	return ( deSound& )operator deObject &();
}

deSound *deSoundReference::operator->() const{
	return ( deSound* )deObjectReference::operator->();
}

deSoundReference &deSoundReference::operator=( deSound *object ){
	deObjectReference::operator=( object );
	return *this;
}

deSoundReference &deSoundReference::operator=( const deSoundReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deSoundReference::operator==( deSound *object ) const{
	return deObjectReference::operator==( object );
}

bool deSoundReference::operator!=( deSound *object ) const{
	return deObjectReference::operator!=( object );
}

bool deSoundReference::operator==( const deSoundReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deSoundReference::operator!=( const deSoundReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
