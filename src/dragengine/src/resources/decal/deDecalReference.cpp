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

#include "deDecal.h"
#include "deDecalReference.h"



// Class deDecalReference
///////////////////////////

// Constructor, destructor
////////////////////////////

deDecalReference::deDecalReference(){
}

deDecalReference::deDecalReference( deDecal *object ) :
deObjectReference( object ){
}

deDecalReference::deDecalReference( const deDecalReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deDecalReference::TakeOver( deDecal *object ){
	deObjectReference::TakeOver( object );
}

bool deDecalReference::operator!() const{
	return deObjectReference::operator!();
}

deDecalReference::operator bool() const{
	return deObjectReference::operator bool();
}

deDecalReference::operator deDecal *() const{
	return ( deDecal* )operator deObject *();
}

deDecalReference::operator deDecal &() const{
	return ( deDecal& )operator deObject &();
}

deDecal *deDecalReference::operator->() const{
	return ( deDecal* )deObjectReference::operator->();
}

deDecalReference &deDecalReference::operator=( deDecal *object ){
	deObjectReference::operator=( object );
	return *this;
}

deDecalReference &deDecalReference::operator=( const deDecalReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deDecalReference::operator==( deDecal *object ) const{
	return deObjectReference::operator==( object );
}

bool deDecalReference::operator!=( deDecal *object ) const{
	return deObjectReference::operator!=( object );
}

bool deDecalReference::operator==( const deDecalReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deDecalReference::operator!=( const deDecalReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
