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

#include "deLight.h"
#include "deLightReference.h"



// Class deLightReference
///////////////////////////

// Constructor, destructor
////////////////////////////

deLightReference::deLightReference(){
}

deLightReference::deLightReference( deLight *object ) :
deObjectReference( object ){
}

deLightReference::deLightReference( const deLightReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deLightReference::TakeOver( deLight *object ){
	deObjectReference::TakeOver( object );
}

bool deLightReference::operator!() const{
	return deObjectReference::operator!();
}

deLightReference::operator bool() const{
	return deObjectReference::operator bool();
}

deLightReference::operator deLight *() const{
	return ( deLight* )operator deObject *();
}

deLightReference::operator deLight &() const{
	return ( deLight& )operator deObject &();
}

deLight *deLightReference::operator->() const{
	return ( deLight* )deObjectReference::operator->();
}

deLightReference &deLightReference::operator=( deLight *object ){
	deObjectReference::operator=( object );
	return *this;
}

deLightReference &deLightReference::operator=( const deLightReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deLightReference::operator==( deLight *object ) const{
	return deObjectReference::operator==( object );
}

bool deLightReference::operator!=( deLight *object ) const{
	return deObjectReference::operator!=( object );
}

bool deLightReference::operator==( const deLightReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deLightReference::operator!=( const deLightReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
