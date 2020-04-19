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

#include "ceConversationAction.h"
#include "ceConversationActionList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceConversationAction
///////////////////////////////

// Constructor, destructor
////////////////////////////

ceConversationActionList::ceConversationActionList(){
}

ceConversationActionList::~ceConversationActionList(){
}



// Management
///////////////

int ceConversationActionList::GetCount() const{
	return pActions.GetCount();
}

ceConversationAction *ceConversationActionList::GetAt( int index ) const{
	return ( ceConversationAction* )pActions.GetAt( index );
}

int ceConversationActionList::IndexOf( ceConversationAction *action ) const{
	return pActions.IndexOf( action );
}

bool ceConversationActionList::Has( ceConversationAction *action ) const{
	return pActions.Has( action );
}

void ceConversationActionList::Add( ceConversationAction *action ){
	if( ! action ) DETHROW( deeInvalidParam );
	
	pActions.Add( action );
}

void ceConversationActionList::InsertAt( ceConversationAction *action, int index ){
	if( ! action ) DETHROW( deeInvalidParam );
	
	pActions.Insert( action, index );
}

void ceConversationActionList::MoveTo( ceConversationAction *action, int index ){
	pActions.Move( action, index );
}

void ceConversationActionList::Remove( ceConversationAction *action ){
	pActions.Remove( action );
}

void ceConversationActionList::RemoveAll(){
	pActions.RemoveAll();
}



ceConversationActionList &ceConversationActionList::operator=( const ceConversationActionList &list ){
	pActions = list.pActions;
	return *this;
}
