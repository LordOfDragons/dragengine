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

#include "deNetworkState.h"
#include "deNetworkStateReference.h"



// Class deNetworkStateReference
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deNetworkStateReference::deNetworkStateReference(){
}

deNetworkStateReference::deNetworkStateReference( deNetworkState *object ) :
deObjectReference( object ){
}

deNetworkStateReference::deNetworkStateReference( const deNetworkStateReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deNetworkStateReference::TakeOver( deNetworkState *object ){
	deObjectReference::TakeOver( object );
}

bool deNetworkStateReference::operator!() const{
	return deObjectReference::operator!();
}

deNetworkStateReference::operator bool() const{
	return deObjectReference::operator bool();
}

deNetworkStateReference::operator deNetworkState *() const{
	return ( deNetworkState* )operator deObject *();
}

deNetworkStateReference::operator deNetworkState &() const{
	return ( deNetworkState& )operator deObject &();
}

deNetworkState *deNetworkStateReference::operator->() const{
	return ( deNetworkState* )deObjectReference::operator->();
}

deNetworkStateReference &deNetworkStateReference::operator=( deNetworkState *object ){
	deObjectReference::operator=( object );
	return *this;
}

deNetworkStateReference &deNetworkStateReference::operator=( const deNetworkStateReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deNetworkStateReference::operator==( deNetworkState *object ) const{
	return deObjectReference::operator==( object );
}

bool deNetworkStateReference::operator!=( deNetworkState *object ) const{
	return deObjectReference::operator!=( object );
}

bool deNetworkStateReference::operator==( const deNetworkStateReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deNetworkStateReference::operator!=( const deNetworkStateReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
