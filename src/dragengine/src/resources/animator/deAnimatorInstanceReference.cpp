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

#include "deAnimatorInstance.h"
#include "deAnimatorInstanceReference.h"



// Class deAnimatorInstanceReference
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorInstanceReference::deAnimatorInstanceReference(){
}

deAnimatorInstanceReference::deAnimatorInstanceReference( deAnimatorInstance *object ) :
deObjectReference( object ){
}

deAnimatorInstanceReference::deAnimatorInstanceReference( const deAnimatorInstanceReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deAnimatorInstanceReference::TakeOver( deAnimatorInstance *object ){
	deObjectReference::TakeOver( object );
}

bool deAnimatorInstanceReference::operator!() const{
	return deObjectReference::operator!();
}

deAnimatorInstanceReference::operator bool() const{
	return deObjectReference::operator bool();
}

deAnimatorInstanceReference::operator deAnimatorInstance *() const{
	return ( deAnimatorInstance* )operator deObject *();
}

deAnimatorInstanceReference::operator deAnimatorInstance &() const{
	return ( deAnimatorInstance& )operator deObject &();
}

deAnimatorInstance *deAnimatorInstanceReference::operator->() const{
	return ( deAnimatorInstance* )deObjectReference::operator->();
}

deAnimatorInstanceReference &deAnimatorInstanceReference::operator=( deAnimatorInstance *object ){
	deObjectReference::operator=( object );
	return *this;
}

deAnimatorInstanceReference &deAnimatorInstanceReference::operator=( const deAnimatorInstanceReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deAnimatorInstanceReference::operator==( deAnimatorInstance *object ) const{
	return deObjectReference::operator==( object );
}

bool deAnimatorInstanceReference::operator!=( deAnimatorInstance *object ) const{
	return deObjectReference::operator!=( object );
}

bool deAnimatorInstanceReference::operator==( const deAnimatorInstanceReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deAnimatorInstanceReference::operator!=( const deAnimatorInstanceReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
