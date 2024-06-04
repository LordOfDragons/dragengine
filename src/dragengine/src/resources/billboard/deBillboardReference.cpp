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

#include "deBillboard.h"
#include "deBillboardReference.h"



// Class deBillboardReference
///////////////////////////////

// Constructor, destructor
////////////////////////////

deBillboardReference::deBillboardReference(){
}

deBillboardReference::deBillboardReference( deBillboard *object ) :
deObjectReference( object ){
}

deBillboardReference::deBillboardReference( const deBillboardReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deBillboardReference::TakeOver( deBillboard *object ){
	deObjectReference::TakeOver( object );
}

bool deBillboardReference::operator!() const{
	return deObjectReference::operator!();
}

deBillboardReference::operator bool() const{
	return deObjectReference::operator bool();
}

deBillboardReference::operator deBillboard *() const{
	return ( deBillboard* )operator deObject *();
}

deBillboardReference::operator deBillboard &() const{
	return ( deBillboard& )operator deObject &();
}

deBillboard *deBillboardReference::operator->() const{
	return ( deBillboard* )deObjectReference::operator->();
}

deBillboardReference &deBillboardReference::operator=( deBillboard *object ){
	deObjectReference::operator=( object );
	return *this;
}

deBillboardReference &deBillboardReference::operator=( const deBillboardReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deBillboardReference::operator==( deBillboard *object ) const{
	return deObjectReference::operator==( object );
}

bool deBillboardReference::operator!=( deBillboard *object ) const{
	return deObjectReference::operator!=( object );
}

bool deBillboardReference::operator==( const deBillboardReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deBillboardReference::operator!=( const deBillboardReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
