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

#include "deNavigator.h"
#include "deNavigatorReference.h"



// Class deNavigatorReference
///////////////////////////////

// Constructor, destructor
////////////////////////////

deNavigatorReference::deNavigatorReference(){
}

deNavigatorReference::deNavigatorReference( deNavigator *object ) :
deObjectReference( object ){
}

deNavigatorReference::deNavigatorReference( const deNavigatorReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deNavigatorReference::TakeOver( deNavigator *object ){
	deObjectReference::TakeOver( object );
}

bool deNavigatorReference::operator!() const{
	return deObjectReference::operator!();
}

deNavigatorReference::operator bool() const{
	return deObjectReference::operator bool();
}

deNavigatorReference::operator deNavigator *() const{
	return ( deNavigator* )operator deObject *();
}

deNavigatorReference::operator deNavigator &() const{
	return ( deNavigator& )operator deObject &();
}

deNavigator *deNavigatorReference::operator->() const{
	return ( deNavigator* )deObjectReference::operator->();
}

deNavigatorReference &deNavigatorReference::operator=( deNavigator *object ){
	deObjectReference::operator=( object );
	return *this;
}

deNavigatorReference &deNavigatorReference::operator=( const deNavigatorReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deNavigatorReference::operator==( deNavigator *object ) const{
	return deObjectReference::operator==( object );
}

bool deNavigatorReference::operator!=( deNavigator *object ) const{
	return deObjectReference::operator!=( object );
}

bool deNavigatorReference::operator==( const deNavigatorReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deNavigatorReference::operator!=( const deNavigatorReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
