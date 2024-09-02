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

#include "ceConversationCondition.h"
#include "ceConversationConditionReference.h"



// Class ceConversationConditionReference
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

ceConversationConditionReference::ceConversationConditionReference(){
}

ceConversationConditionReference::ceConversationConditionReference( ceConversationCondition *object ) :
deObjectReference( object ){
}

ceConversationConditionReference::ceConversationConditionReference( const ceConversationConditionReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void ceConversationConditionReference::TakeOver( ceConversationCondition *object ){
	deObjectReference::TakeOver( object );
}

bool ceConversationConditionReference::operator!() const{
	return deObjectReference::operator!();
}

ceConversationConditionReference::operator bool() const{
	return deObjectReference::operator bool();
}

ceConversationConditionReference::operator ceConversationCondition *() const{
	return ( ceConversationCondition* )operator deObject *();
}

ceConversationConditionReference::operator ceConversationCondition &() const{
	return ( ceConversationCondition& )operator deObject &();
}

ceConversationCondition *ceConversationConditionReference::operator->() const{
	return ( ceConversationCondition* )deObjectReference::operator->();
}

ceConversationConditionReference &ceConversationConditionReference::operator=( ceConversationCondition *object ){
	deObjectReference::operator=( object );
	return *this;
}

ceConversationConditionReference &ceConversationConditionReference::operator=( const ceConversationConditionReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool ceConversationConditionReference::operator==( ceConversationCondition *object ) const{
	return deObjectReference::operator==( object );
}

bool ceConversationConditionReference::operator!=( ceConversationCondition *object ) const{
	return deObjectReference::operator!=( object );
}

bool ceConversationConditionReference::operator==( const ceConversationConditionReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool ceConversationConditionReference::operator!=( const ceConversationConditionReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
