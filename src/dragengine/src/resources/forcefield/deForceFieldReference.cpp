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

#include "deForceField.h"
#include "deForceFieldReference.h"



// Class deForceFieldReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

deForceFieldReference::deForceFieldReference(){
}

deForceFieldReference::deForceFieldReference( deForceField *object ) :
deObjectReference( object ){
}

deForceFieldReference::deForceFieldReference( const deForceFieldReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deForceFieldReference::TakeOver( deForceField *object ){
	deObjectReference::TakeOver( object );
}

bool deForceFieldReference::operator!() const{
	return deObjectReference::operator!();
}

deForceFieldReference::operator bool() const{
	return deObjectReference::operator bool();
}

deForceFieldReference::operator deForceField *() const{
	return ( deForceField* )operator deObject *();
}

deForceFieldReference::operator deForceField &() const{
	return ( deForceField& )operator deObject &();
}

deForceField *deForceFieldReference::operator->() const{
	return ( deForceField* )deObjectReference::operator->();
}

deForceFieldReference &deForceFieldReference::operator=( deForceField *object ){
	deObjectReference::operator=( object );
	return *this;
}

deForceFieldReference &deForceFieldReference::operator=( const deForceFieldReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deForceFieldReference::operator==( deForceField *object ) const{
	return deObjectReference::operator==( object );
}

bool deForceFieldReference::operator!=( deForceField *object ) const{
	return deObjectReference::operator!=( object );
}

bool deForceFieldReference::operator==( const deForceFieldReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deForceFieldReference::operator!=( const deForceFieldReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
