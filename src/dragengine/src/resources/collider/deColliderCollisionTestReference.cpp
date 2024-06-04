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

#include "deColliderCollisionTest.h"
#include "deColliderCollisionTestReference.h"



// Class deColliderCollisionTestReference
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deColliderCollisionTestReference::deColliderCollisionTestReference(){
}

deColliderCollisionTestReference::deColliderCollisionTestReference( deColliderCollisionTest *object ) :
deObjectReference( object ){
}

deColliderCollisionTestReference::deColliderCollisionTestReference( const deColliderCollisionTestReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deColliderCollisionTestReference::TakeOver( deColliderCollisionTest *object ){
	deObjectReference::TakeOver( object );
}

bool deColliderCollisionTestReference::operator!() const{
	return deObjectReference::operator!();
}

deColliderCollisionTestReference::operator bool() const{
	return deObjectReference::operator bool();
}

deColliderCollisionTestReference::operator deColliderCollisionTest *() const{
	return ( deColliderCollisionTest* )operator deObject *();
}

deColliderCollisionTestReference::operator deColliderCollisionTest &() const{
	return ( deColliderCollisionTest& )operator deObject &();
}

deColliderCollisionTest *deColliderCollisionTestReference::operator->() const{
	return ( deColliderCollisionTest* )deObjectReference::operator->();
}

deColliderCollisionTestReference &deColliderCollisionTestReference::operator=( deColliderCollisionTest *object ){
	deObjectReference::operator=( object );
	return *this;
}

deColliderCollisionTestReference &deColliderCollisionTestReference::operator=( const deColliderCollisionTestReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deColliderCollisionTestReference::operator==( deColliderCollisionTest *object ) const{
	return deObjectReference::operator==( object );
}

bool deColliderCollisionTestReference::operator!=( deColliderCollisionTest *object ) const{
	return deObjectReference::operator!=( object );
}

bool deColliderCollisionTestReference::operator==( const deColliderCollisionTestReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deColliderCollisionTestReference::operator!=( const deColliderCollisionTestReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
