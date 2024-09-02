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

#include "deConnection.h"
#include "deConnectionReference.h"



// Class deConnectionReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

deConnectionReference::deConnectionReference(){
}

deConnectionReference::deConnectionReference( deConnection *object ) :
deObjectReference( object ){
}

deConnectionReference::deConnectionReference( const deConnectionReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deConnectionReference::TakeOver( deConnection *object ){
	deObjectReference::TakeOver( object );
}

bool deConnectionReference::operator!() const{
	return deObjectReference::operator!();
}

deConnectionReference::operator bool() const{
	return deObjectReference::operator bool();
}

deConnectionReference::operator deConnection *() const{
	return ( deConnection* )operator deObject *();
}

deConnectionReference::operator deConnection &() const{
	return ( deConnection& )operator deObject &();
}

deConnection *deConnectionReference::operator->() const{
	return ( deConnection* )deObjectReference::operator->();
}

deConnectionReference &deConnectionReference::operator=( deConnection *object ){
	deObjectReference::operator=( object );
	return *this;
}

deConnectionReference &deConnectionReference::operator=( const deConnectionReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deConnectionReference::operator==( deConnection *object ) const{
	return deObjectReference::operator==( object );
}

bool deConnectionReference::operator!=( deConnection *object ) const{
	return deObjectReference::operator!=( object );
}

bool deConnectionReference::operator==( const deConnectionReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deConnectionReference::operator!=( const deConnectionReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
