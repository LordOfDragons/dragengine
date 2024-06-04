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

#include "deNavigationSpace.h"
#include "deNavigationSpaceReference.h"



// Class deNavigationSpaceReference
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deNavigationSpaceReference::deNavigationSpaceReference(){
}

deNavigationSpaceReference::deNavigationSpaceReference( deNavigationSpace *object ) :
deObjectReference( object ){
}

deNavigationSpaceReference::deNavigationSpaceReference( const deNavigationSpaceReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deNavigationSpaceReference::TakeOver( deNavigationSpace *object ){
	deObjectReference::TakeOver( object );
}

bool deNavigationSpaceReference::operator!() const{
	return deObjectReference::operator!();
}

deNavigationSpaceReference::operator bool() const{
	return deObjectReference::operator bool();
}

deNavigationSpaceReference::operator deNavigationSpace *() const{
	return ( deNavigationSpace* )operator deObject *();
}

deNavigationSpaceReference::operator deNavigationSpace &() const{
	return ( deNavigationSpace& )operator deObject &();
}

deNavigationSpace *deNavigationSpaceReference::operator->() const{
	return ( deNavigationSpace* )deObjectReference::operator->();
}

deNavigationSpaceReference &deNavigationSpaceReference::operator=( deNavigationSpace *object ){
	deObjectReference::operator=( object );
	return *this;
}

deNavigationSpaceReference &deNavigationSpaceReference::operator=( const deNavigationSpaceReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deNavigationSpaceReference::operator==( deNavigationSpace *object ) const{
	return deObjectReference::operator==( object );
}

bool deNavigationSpaceReference::operator!=( deNavigationSpace *object ) const{
	return deObjectReference::operator!=( object );
}

bool deNavigationSpaceReference::operator==( const deNavigationSpaceReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deNavigationSpaceReference::operator!=( const deNavigationSpaceReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
