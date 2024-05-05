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

#include "deRig.h"
#include "deRigReference.h"



// Class deRigReference
/////////////////////////

// Constructor, destructor
////////////////////////////

deRigReference::deRigReference(){
}

deRigReference::deRigReference( deRig *object ) :
deObjectReference( object ){
}

deRigReference::deRigReference( const deRigReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deRigReference::TakeOver( deRig *object ){
	deObjectReference::TakeOver( object );
}

bool deRigReference::operator!() const{
	return deObjectReference::operator!();
}

deRigReference::operator bool() const{
	return deObjectReference::operator bool();
}

deRigReference::operator deRig *() const{
	return ( deRig* )operator deObject *();
}

deRigReference::operator deRig &() const{
	return ( deRig& )operator deObject &();
}

deRig *deRigReference::operator->() const{
	return ( deRig* )deObjectReference::operator->();
}

deRigReference &deRigReference::operator=( deRig *object ){
	deObjectReference::operator=( object );
	return *this;
}

deRigReference &deRigReference::operator=( const deRigReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deRigReference::operator==( deRig *object ) const{
	return deObjectReference::operator==( object );
}

bool deRigReference::operator==( const deRigReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deRigReference::operator!=( deRig *object ) const{
	return deObjectReference::operator!=( object );
}

bool deRigReference::operator!=( const deRigReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
