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

#include "deAnimation.h"
#include "deAnimationReference.h"



// Class deAnimationReference
///////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimationReference::deAnimationReference(){
}

deAnimationReference::deAnimationReference( deAnimation *object ) :
deObjectReference( object ){
}

deAnimationReference::deAnimationReference( const deAnimationReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deAnimationReference::TakeOver( deAnimation *object ){
	deObjectReference::TakeOver( object );
}

bool deAnimationReference::operator!() const{
	return deObjectReference::operator!();
}

deAnimationReference::operator bool() const{
	return deObjectReference::operator bool();
}

deAnimationReference::operator deAnimation *() const{
	return ( deAnimation* )operator deObject *();
}

deAnimationReference::operator deAnimation &() const{
	return ( deAnimation& )operator deObject &();
}

deAnimation *deAnimationReference::operator->() const{
	return ( deAnimation* )deObjectReference::operator->();
}

deAnimationReference &deAnimationReference::operator=( deAnimation *object ){
	deObjectReference::operator=( object );
	return *this;
}

deAnimationReference &deAnimationReference::operator=( const deAnimationReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deAnimationReference::operator==( deAnimation *object ) const{
	return deObjectReference::operator==( object );
}

bool deAnimationReference::operator!=( deAnimation *object ) const{
	return deObjectReference::operator!=( object );
}

bool deAnimationReference::operator==( const deAnimationReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deAnimationReference::operator!=( const deAnimationReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
