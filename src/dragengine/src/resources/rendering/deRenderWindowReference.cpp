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

#include "deRenderWindow.h"
#include "deRenderWindowReference.h"



// Class deRenderWindowReference
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deRenderWindowReference::deRenderWindowReference(){
}

deRenderWindowReference::deRenderWindowReference( deRenderWindow *object ) :
deObjectReference( object ){
}

deRenderWindowReference::deRenderWindowReference( const deRenderWindowReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deRenderWindowReference::TakeOver( deRenderWindow *object ){
	deObjectReference::TakeOver( object );
}

bool deRenderWindowReference::operator!() const{
	return deObjectReference::operator!();
}

deRenderWindowReference::operator bool() const{
	return deObjectReference::operator bool();
}

deRenderWindowReference::operator deRenderWindow *() const{
	return ( deRenderWindow* )operator deObject *();
}

deRenderWindowReference::operator deRenderWindow &() const{
	return ( deRenderWindow& )operator deObject &();
}

deRenderWindow *deRenderWindowReference::operator->() const{
	return ( deRenderWindow* )deObjectReference::operator->();
}

deRenderWindowReference &deRenderWindowReference::operator=( deRenderWindow *object ){
	deObjectReference::operator=( object );
	return *this;
}

deRenderWindowReference &deRenderWindowReference::operator=( const deRenderWindowReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deRenderWindowReference::operator==( deRenderWindow *object ) const{
	return deObjectReference::operator==( object );
}

bool deRenderWindowReference::operator!=( deRenderWindow *object ) const{
	return deObjectReference::operator!=( object );
}

bool deRenderWindowReference::operator==( const deRenderWindowReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deRenderWindowReference::operator!=( const deRenderWindowReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
