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

#include "deNetworkValue.h"
#include "deNetworkValueReference.h"



// Class deNetworkValueReference
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deNetworkValueReference::deNetworkValueReference(){
}

deNetworkValueReference::deNetworkValueReference( deNetworkValue *object ) :
deObjectReference( object ){
}

deNetworkValueReference::deNetworkValueReference( const deNetworkValueReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deNetworkValueReference::TakeOver( deNetworkValue *object ){
	deObjectReference::TakeOver( object );
}

bool deNetworkValueReference::operator!() const{
	return deObjectReference::operator!();
}

deNetworkValueReference::operator bool() const{
	return deObjectReference::operator bool();
}

deNetworkValueReference::operator deNetworkValue *() const{
	return ( deNetworkValue* )operator deObject *();
}

deNetworkValueReference::operator deNetworkValue &() const{
	return ( deNetworkValue& )operator deObject &();
}

deNetworkValue *deNetworkValueReference::operator->() const{
	return ( deNetworkValue* )deObjectReference::operator->();
}

deNetworkValueReference &deNetworkValueReference::operator=( deNetworkValue *object ){
	deObjectReference::operator=( object );
	return *this;
}

deNetworkValueReference &deNetworkValueReference::operator=( const deNetworkValueReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deNetworkValueReference::operator==( deNetworkValue *object ) const{
	return deObjectReference::operator==( object );
}

bool deNetworkValueReference::operator!=( deNetworkValue *object ) const{
	return deObjectReference::operator!=( object );
}

bool deNetworkValueReference::operator==( const deNetworkValueReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deNetworkValueReference::operator!=( const deNetworkValueReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
