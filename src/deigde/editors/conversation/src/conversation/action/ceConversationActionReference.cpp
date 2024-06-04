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

#include "ceConversationAction.h"
#include "ceConversationActionReference.h"



// Class ceConversationActionReference
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

ceConversationActionReference::ceConversationActionReference(){
}

ceConversationActionReference::ceConversationActionReference( ceConversationAction *object ) :
deObjectReference( object ){
}

ceConversationActionReference::ceConversationActionReference( const ceConversationActionReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void ceConversationActionReference::TakeOver( ceConversationAction *object ){
	deObjectReference::TakeOver( object );
}

bool ceConversationActionReference::operator!() const{
	return deObjectReference::operator!();
}

ceConversationActionReference::operator bool() const{
	return deObjectReference::operator bool();
}

ceConversationActionReference::operator ceConversationAction *() const{
	return ( ceConversationAction* )operator deObject *();
}

ceConversationActionReference::operator ceConversationAction &() const{
	return ( ceConversationAction& )operator deObject &();
}

ceConversationAction *ceConversationActionReference::operator->() const{
	return ( ceConversationAction* )deObjectReference::operator->();
}

ceConversationActionReference &ceConversationActionReference::operator=( ceConversationAction *object ){
	deObjectReference::operator=( object );
	return *this;
}

ceConversationActionReference &ceConversationActionReference::operator=( const ceConversationActionReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool ceConversationActionReference::operator==( ceConversationAction *object ) const{
	return deObjectReference::operator==( object );
}

bool ceConversationActionReference::operator!=( ceConversationAction *object ) const{
	return deObjectReference::operator!=( object );
}

bool ceConversationActionReference::operator==( const ceConversationActionReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool ceConversationActionReference::operator!=( const ceConversationActionReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
