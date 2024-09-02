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

#include "deFont.h"
#include "deFontReference.h"



// Class deFontReference
//////////////////////////

// Constructor, destructor
////////////////////////////

deFontReference::deFontReference(){
}

deFontReference::deFontReference( deFont *object ) :
deObjectReference( object ){
}

deFontReference::deFontReference( const deFontReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deFontReference::TakeOver( deFont *object ){
	deObjectReference::TakeOver( object );
}

bool deFontReference::operator!() const{
	return deObjectReference::operator!();
}

deFontReference::operator bool() const{
	return deObjectReference::operator bool();
}

deFontReference::operator deFont *() const{
	return ( deFont* )operator deObject *();
}

deFontReference::operator deFont &() const{
	return ( deFont& )operator deObject &();
}

deFont *deFontReference::operator->() const{
	return ( deFont* )deObjectReference::operator->();
}

deFontReference &deFontReference::operator=( deFont *object ){
	deObjectReference::operator=( object );
	return *this;
}

deFontReference &deFontReference::operator=( const deFontReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deFontReference::operator==( deFont *object ) const{
	return deObjectReference::operator==( object );
}

bool deFontReference::operator!=( deFont *object ) const{
	return deObjectReference::operator!=( object );
}

bool deFontReference::operator==( const deFontReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deFontReference::operator!=( const deFontReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
