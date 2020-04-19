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
