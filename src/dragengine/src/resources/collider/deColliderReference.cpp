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

#include "deCollider.h"
#include "deColliderReference.h"



// Class deColliderReference
//////////////////////////////

// Constructor, destructor
////////////////////////////

deColliderReference::deColliderReference(){
}

deColliderReference::deColliderReference( deCollider *object ) :
deObjectReference( object ){
}

deColliderReference::deColliderReference( const deColliderReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deColliderReference::TakeOver( deCollider *object ){
	deObjectReference::TakeOver( object );
}

bool deColliderReference::operator!() const{
	return deObjectReference::operator!();
}

deColliderReference::operator bool() const{
	return deObjectReference::operator bool();
}

deColliderReference::operator deCollider *() const{
	return ( deCollider* )operator deObject *();
}

deColliderReference::operator deCollider &() const{
	return ( deCollider& )operator deObject &();
}

deCollider *deColliderReference::operator->() const{
	return ( deCollider* )deObjectReference::operator->();
}

deColliderReference &deColliderReference::operator=( deCollider *object ){
	deObjectReference::operator=( object );
	return *this;
}

deColliderReference &deColliderReference::operator=( const deColliderReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deColliderReference::operator==( deCollider *object ) const{
	return deObjectReference::operator==( object );
}

bool deColliderReference::operator!=( deCollider *object ) const{
	return deObjectReference::operator!=( object );
}

bool deColliderReference::operator==( const deColliderReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deColliderReference::operator!=( const deColliderReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
