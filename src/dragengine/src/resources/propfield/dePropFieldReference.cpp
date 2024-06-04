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

#include "dePropField.h"
#include "dePropFieldReference.h"



// Class dePropFieldReference
///////////////////////////////

// Constructor, destructor
////////////////////////////

dePropFieldReference::dePropFieldReference(){
}

dePropFieldReference::dePropFieldReference( dePropField *object ) :
deObjectReference( object ){
}

dePropFieldReference::dePropFieldReference( const dePropFieldReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void dePropFieldReference::TakeOver( dePropField *object ){
	deObjectReference::TakeOver( object );
}

bool dePropFieldReference::operator!() const{
	return deObjectReference::operator!();
}

dePropFieldReference::operator bool() const{
	return deObjectReference::operator bool();
}

dePropFieldReference::operator dePropField *() const{
	return ( dePropField* )operator deObject *();
}

dePropFieldReference::operator dePropField &() const{
	return ( dePropField& )operator deObject &();
}

dePropField *dePropFieldReference::operator->() const{
	return ( dePropField* )deObjectReference::operator->();
}

dePropFieldReference &dePropFieldReference::operator=( dePropField *object ){
	deObjectReference::operator=( object );
	return *this;
}

dePropFieldReference &dePropFieldReference::operator=( const dePropFieldReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool dePropFieldReference::operator==( dePropField *object ) const{
	return deObjectReference::operator==( object );
}

bool dePropFieldReference::operator!=( dePropField *object ) const{
	return deObjectReference::operator!=( object );
}

bool dePropFieldReference::operator==( const dePropFieldReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool dePropFieldReference::operator!=( const dePropFieldReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
