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

#include "deWorld.h"
#include "deWorldReference.h"



// Class deWorldReference
///////////////////////////

// Constructor, destructor
////////////////////////////

deWorldReference::deWorldReference(){
}

deWorldReference::deWorldReference( deWorld *object ) :
deObjectReference( object ){
}

deWorldReference::deWorldReference( const deWorldReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deWorldReference::TakeOver( deWorld *object ){
	deObjectReference::TakeOver( object );
}

bool deWorldReference::operator!() const{
	return deObjectReference::operator!();
}

deWorldReference::operator bool() const{
	return deObjectReference::operator bool();
}

deWorldReference::operator deWorld *() const{
	return ( deWorld* )operator deObject *();
}

deWorldReference::operator deWorld &() const{
	return ( deWorld& )operator deObject &();
}

deWorld *deWorldReference::operator->() const{
	return ( deWorld* )deObjectReference::operator->();
}

deWorldReference &deWorldReference::operator=( deWorld *object ){
	deObjectReference::operator=( object );
	return *this;
}

deWorldReference &deWorldReference::operator=( const deWorldReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deWorldReference::operator==( deWorld *object ) const{
	return deObjectReference::operator==( object );
}

bool deWorldReference::operator!=( deWorld *object ) const{
	return deObjectReference::operator!=( object );
}

bool deWorldReference::operator==( const deWorldReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deWorldReference::operator!=( const deWorldReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
