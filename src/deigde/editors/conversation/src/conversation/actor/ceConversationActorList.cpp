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

#include "ceConversationActor.h"
#include "ceConversationActorList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceConversationActor
///////////////////////////////

// Constructor, destructor
////////////////////////////

ceConversationActorList::ceConversationActorList(){
}

ceConversationActorList::~ceConversationActorList(){
}



// Management
///////////////

int ceConversationActorList::GetCount() const{
	return pActors.GetCount();
}

ceConversationActor *ceConversationActorList::GetAt( int index ) const{
	return ( ceConversationActor* )pActors.GetAt( index );
}

ceConversationActor *ceConversationActorList::GetWithID( const char *id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	ceConversationActor *actor;
	const int count = pActors.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		actor = ( ceConversationActor* )pActors.GetAt( i );
		
		if( actor->GetID().Equals( id ) ){
			return actor;
		}
	}
	
	return NULL;
}

ceConversationActor *ceConversationActorList::GetWithAliasID( const char *id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	ceConversationActor *actor;
	const int count = pActors.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		actor = ( ceConversationActor* )pActors.GetAt( i );
		
		if( ! actor->GetAliasID().IsEmpty() && actor->GetAliasID().Equals( id ) ){
			return actor;
		}
	}
	
	return NULL;
}

ceConversationActor *ceConversationActorList::GetWithIDOrAliasID( const char *id ) const{
	ceConversationActor *actor = GetWithAliasID( id );
	
	if( ! actor ){
		actor = GetWithID( id );
	}
	
	return actor;
}

int ceConversationActorList::IndexOf( ceConversationActor *actor ) const{
	return pActors.IndexOf( actor );
}

int ceConversationActorList::IndexWithID( const char *id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pActors.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const ceConversationActor &actor = *( ( ceConversationActor* )pActors.GetAt( i ) );
		
		if( actor.GetID().Equals( id ) ){
			return i;
		}
	}
	
	return -1;
}

int ceConversationActorList::IndexWithAliasID( const char *id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pActors.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const ceConversationActor &actor = *( ( ceConversationActor* )pActors.GetAt( i ) );
		
		if( ! actor.GetAliasID().IsEmpty() && actor.GetAliasID().Equals( id ) ){
			return i;
		}
	}
	
	return -1;
}

int ceConversationActorList::IndexWithIDOrAliasID( const char *id ) const{
	int index = IndexWithAliasID( id );
	
	if( index == -1 ){
		index = IndexWithID( id );
	}
	
	return index;
}

bool ceConversationActorList::Has( ceConversationActor *actor ) const{
	return pActors.Has( actor );
}

bool ceConversationActorList::HasWithID( const char *id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pActors.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const ceConversationActor &actor = *( ( ceConversationActor* )pActors.GetAt( i ) );
		
		if( actor.GetID().Equals( id ) ){
			return true;
		}
	}
	
	return false;
}

bool ceConversationActorList::HasWithAliasID( const char *id ) const{
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pActors.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const ceConversationActor &actor = *( ( ceConversationActor* )pActors.GetAt( i ) );
		
		if( ! actor.GetAliasID().IsEmpty() && actor.GetAliasID().Equals( id ) ){
			return true;
		}
	}
	
	return false;
}

bool ceConversationActorList::HasWithIDOrAliasID( const char *id ) const{
	return HasWithAliasID( id ) || HasWithID( id );
}

void ceConversationActorList::Add( ceConversationActor *actor ){
	if( ! actor ) DETHROW( deeInvalidParam );
	
	pActors.Add( actor );
}

void ceConversationActorList::InsertAt( ceConversationActor *actor, int index ){
	if( ! actor ) DETHROW( deeInvalidParam );
	
	pActors.Insert( actor, index );
}

void ceConversationActorList::MoveTo( ceConversationActor *actor, int index ){
	pActors.Move( actor, index );
}

void ceConversationActorList::Remove( ceConversationActor *actor ){
	pActors.Remove( actor );
}

void ceConversationActorList::RemoveAll(){
	pActors.RemoveAll();
}



ceConversationActorList &ceConversationActorList::operator=( const ceConversationActorList &list ){
	pActors = list.pActors;
	return *this;
}
