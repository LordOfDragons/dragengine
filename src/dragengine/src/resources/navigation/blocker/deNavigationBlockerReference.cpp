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

#include "deNavigationBlocker.h"
#include "deNavigationBlockerReference.h"



// Class deNavigationBlockerReference
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deNavigationBlockerReference::deNavigationBlockerReference(){
}

deNavigationBlockerReference::deNavigationBlockerReference( deNavigationBlocker *object ) :
deObjectReference( object ){
}

deNavigationBlockerReference::deNavigationBlockerReference( const deNavigationBlockerReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deNavigationBlockerReference::TakeOver( deNavigationBlocker *object ){
	deObjectReference::TakeOver( object );
}

bool deNavigationBlockerReference::operator!() const{
	return deObjectReference::operator!();
}

deNavigationBlockerReference::operator bool() const{
	return deObjectReference::operator bool();
}

deNavigationBlockerReference::operator deNavigationBlocker *() const{
	return ( deNavigationBlocker* )operator deObject *();
}

deNavigationBlockerReference::operator deNavigationBlocker &() const{
	return ( deNavigationBlocker& )operator deObject &();
}

deNavigationBlocker *deNavigationBlockerReference::operator->() const{
	return ( deNavigationBlocker* )deObjectReference::operator->();
}

deNavigationBlockerReference &deNavigationBlockerReference::operator=( deNavigationBlocker *object ){
	deObjectReference::operator=( object );
	return *this;
}

deNavigationBlockerReference &deNavigationBlockerReference::operator=( const deNavigationBlockerReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deNavigationBlockerReference::operator==( deNavigationBlocker *object ) const{
	return deObjectReference::operator==( object );
}

bool deNavigationBlockerReference::operator!=( deNavigationBlocker *object ) const{
	return deObjectReference::operator!=( object );
}

bool deNavigationBlockerReference::operator==( const deNavigationBlockerReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deNavigationBlockerReference::operator!=( const deNavigationBlockerReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
