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

#include "deResource.h"
#include "deResourceReference.h"



// Class deResourceReference
//////////////////////////////

// Constructor, destructor
////////////////////////////

deResourceReference::deResourceReference(){
}

deResourceReference::deResourceReference( deResource *object ) :
deObjectReference( object ){
}

deResourceReference::deResourceReference( const deResourceReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deResourceReference::TakeOver( deResource *object ){
	deObjectReference::TakeOver( object );
}

bool deResourceReference::operator!() const{
	return deObjectReference::operator!();
}

deResourceReference::operator bool() const{
	return deObjectReference::operator bool();
}

deResourceReference::operator deResource *() const{
	return ( deResource* )operator deObject *();
}

deResourceReference::operator deResource &() const{
	return ( deResource& )operator deObject &();
}

deResource *deResourceReference::operator->() const{
	return ( deResource* )deObjectReference::operator->();
}

deResourceReference &deResourceReference::operator=( deResource *object ){
	deObjectReference::operator=( object );
	return *this;
}

deResourceReference &deResourceReference::operator=( const deResourceReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deResourceReference::operator==( deResource *object ) const{
	return deObjectReference::operator==( object );
}

bool deResourceReference::operator!=( deResource *object ) const{
	return deObjectReference::operator!=( object );
}

bool deResourceReference::operator==( const deResourceReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deResourceReference::operator!=( const deResourceReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
