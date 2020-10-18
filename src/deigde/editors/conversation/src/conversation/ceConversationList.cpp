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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ceConversation.h"
#include "ceConversationList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceConversation
/////////////////////////

// Constructor, destructor
////////////////////////////

ceConversationList::ceConversationList(){
}

ceConversationList::ceConversationList( const ceConversationList &list ) :
pConversations( list.pConversations ){
}

ceConversationList::~ceConversationList(){
}



// Management
///////////////

int ceConversationList::GetCount() const{
	return pConversations.GetCount();
}

ceConversation *ceConversationList::GetAt( int index ) const{
	return ( ceConversation* )pConversations.GetAt( index );
}

int ceConversationList::IndexOf( ceConversation *target ) const{
	return pConversations.IndexOf( target );
}

bool ceConversationList::Has( ceConversation *target ) const{
	return pConversations.Has( target );
}

void ceConversationList::Add( ceConversation *target ){
	if( Has( target ) ){
		DETHROW( deeInvalidParam );
	}
	pConversations.Add( target );
}

void ceConversationList::Remove( ceConversation *target ){
	pConversations.Remove( target );
}

void ceConversationList::RemoveAll(){
	pConversations.RemoveAll();
}

ceConversationList &ceConversationList::operator=( const ceConversationList &list ){
	pConversations = list.pConversations;
	return *this;
}
