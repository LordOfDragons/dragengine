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

#include "deAnimator.h"
#include "deAnimatorReference.h"



// Class deAnimatorReference
//////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorReference::deAnimatorReference(){
}

deAnimatorReference::deAnimatorReference( deAnimator *object ) :
deObjectReference( object ){
}

deAnimatorReference::deAnimatorReference( const deAnimatorReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deAnimatorReference::TakeOver( deAnimator *object ){
	deObjectReference::TakeOver( object );
}

bool deAnimatorReference::operator!() const{
	return deObjectReference::operator!();
}

deAnimatorReference::operator bool() const{
	return deObjectReference::operator bool();
}

deAnimatorReference::operator deAnimator *() const{
	return ( deAnimator* )operator deObject *();
}

deAnimatorReference::operator deAnimator &() const{
	return ( deAnimator& )operator deObject &();
}

deAnimator *deAnimatorReference::operator->() const{
	return ( deAnimator* )deObjectReference::operator->();
}

deAnimatorReference &deAnimatorReference::operator=( deAnimator *object ){
	deObjectReference::operator=( object );
	return *this;
}

deAnimatorReference &deAnimatorReference::operator=( const deAnimatorReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deAnimatorReference::operator==( deAnimator *object ) const{
	return deObjectReference::operator==( object );
}

bool deAnimatorReference::operator!=( deAnimator *object ) const{
	return deObjectReference::operator!=( object );
}

bool deAnimatorReference::operator==( const deAnimatorReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deAnimatorReference::operator!=( const deAnimatorReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
