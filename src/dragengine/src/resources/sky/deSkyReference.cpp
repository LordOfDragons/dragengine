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

#include "deSky.h"
#include "deSkyReference.h"



// Class deSkyReference
/////////////////////////

// Constructor, destructor
////////////////////////////

deSkyReference::deSkyReference(){
}

deSkyReference::deSkyReference( deSky *object ) :
deObjectReference( object ){
}

deSkyReference::deSkyReference( const deSkyReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deSkyReference::TakeOver( deSky *object ){
	deObjectReference::TakeOver( object );
}

bool deSkyReference::operator!() const{
	return deObjectReference::operator!();
}

deSkyReference::operator bool() const{
	return deObjectReference::operator bool();
}

deSkyReference::operator deSky *() const{
	return ( deSky* )operator deObject *();
}

deSkyReference::operator deSky &() const{
	return ( deSky& )operator deObject &();
}

deSky *deSkyReference::operator->() const{
	return ( deSky* )deObjectReference::operator->();
}

deSkyReference &deSkyReference::operator=( deSky *object ){
	deObjectReference::operator=( object );
	return *this;
}

deSkyReference &deSkyReference::operator=( const deSkyReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deSkyReference::operator==( deSky *object ) const{
	return deObjectReference::operator==( object );
}

bool deSkyReference::operator!=( deSky *object ) const{
	return deObjectReference::operator!=( object );
}

bool deSkyReference::operator==( const deSkyReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deSkyReference::operator!=( const deSkyReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
