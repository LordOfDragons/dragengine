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

#include "deSkin.h"
#include "deSkinReference.h"



// Class deSkinReference
//////////////////////////

// Constructor, destructor
////////////////////////////

deSkinReference::deSkinReference(){
}

deSkinReference::deSkinReference( deSkin *object ) :
deObjectReference( object ){
}

deSkinReference::deSkinReference( const deSkinReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deSkinReference::TakeOver( deSkin *object ){
	deObjectReference::TakeOver( object );
}

bool deSkinReference::operator!() const{
	return deObjectReference::operator!();
}

deSkinReference::operator bool() const{
	return deObjectReference::operator bool();
}

deSkinReference::operator deSkin *() const{
	return ( deSkin* )operator deObject *();
}

deSkinReference::operator deSkin &() const{
	return ( deSkin& )operator deObject &();
}

deSkin *deSkinReference::operator->() const{
	return ( deSkin* )deObjectReference::operator->();
}

deSkinReference &deSkinReference::operator=( deSkin *object ){
	deObjectReference::operator=( object );
	return *this;
}

deSkinReference &deSkinReference::operator=( const deSkinReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deSkinReference::operator==( deSkin *object ) const{
	return deObjectReference::operator==( object );
}

bool deSkinReference::operator!=( deSkin *object ) const{
	return deObjectReference::operator!=( object );
}

bool deSkinReference::operator==( const deSkinReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deSkinReference::operator!=( const deSkinReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
