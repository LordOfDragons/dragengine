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

#include "deNetworkMessage.h"
#include "deNetworkMessageReference.h"



// Class deNetworkMessageReference
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deNetworkMessageReference::deNetworkMessageReference(){
}

deNetworkMessageReference::deNetworkMessageReference( deNetworkMessage *object ) :
deObjectReference( object ){
}

deNetworkMessageReference::deNetworkMessageReference( const deNetworkMessageReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void deNetworkMessageReference::TakeOver( deNetworkMessage *object ){
	deObjectReference::TakeOver( object );
}

bool deNetworkMessageReference::operator!() const{
	return deObjectReference::operator!();
}

deNetworkMessageReference::operator bool() const{
	return deObjectReference::operator bool();
}

deNetworkMessageReference::operator deNetworkMessage *() const{
	return ( deNetworkMessage* )operator deObject *();
}

deNetworkMessageReference::operator deNetworkMessage &() const{
	return ( deNetworkMessage& )operator deObject &();
}

deNetworkMessage *deNetworkMessageReference::operator->() const{
	return ( deNetworkMessage* )deObjectReference::operator->();
}

deNetworkMessageReference &deNetworkMessageReference::operator=( deNetworkMessage *object ){
	deObjectReference::operator=( object );
	return *this;
}

deNetworkMessageReference &deNetworkMessageReference::operator=( const deNetworkMessageReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool deNetworkMessageReference::operator==( deNetworkMessage *object ) const{
	return deObjectReference::operator==( object );
}

bool deNetworkMessageReference::operator!=( deNetworkMessage *object ) const{
	return deObjectReference::operator!=( object );
}

bool deNetworkMessageReference::operator==( const deNetworkMessageReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool deNetworkMessageReference::operator!=( const deNetworkMessageReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
