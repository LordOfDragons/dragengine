/* 
 * Drag[en]gine IGDE Conversation Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
