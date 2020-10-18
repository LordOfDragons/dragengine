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

#include "ceConversationTopic.h"
#include "ceConversationTopicList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceConversationTopic
//////////////////////////////

// Constructor, destructor
////////////////////////////

ceConversationTopicList::ceConversationTopicList(){
}

ceConversationTopicList::~ceConversationTopicList(){
}



// Management
///////////////

int ceConversationTopicList::GetCount() const{
	return pTopics.GetCount();
}

ceConversationTopic *ceConversationTopicList::GetAt( int index ) const{
	return ( ceConversationTopic* )pTopics.GetAt( index );
}

ceConversationTopic *ceConversationTopicList::GetWithID( const char *id ) const{
	const int count = pTopics.GetCount();
	ceConversationTopic *file;
	int i;
	
	for( i=0; i<count; i++ ){
		file = ( ceConversationTopic* )pTopics.GetAt( i );
		
		if( file->GetID().Equals( id ) ){
			return file;
		}
	}
	
	return NULL;
}

int ceConversationTopicList::IndexOf( ceConversationTopic *file ) const{
	return pTopics.IndexOf( file );
}

int ceConversationTopicList::IndexOfWithID( const char *id ) const{
	const int count = pTopics.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( ceConversationTopic* )pTopics.GetAt( i ) )->GetID().Equals( id ) ){
			return i;
		}
	}
	
	return -1;
}

bool ceConversationTopicList::Has( ceConversationTopic *file ) const{
	return pTopics.Has( file );
}

bool ceConversationTopicList::HasWithID( const char *id ) const{
	const int count = pTopics.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( ceConversationTopic* )pTopics.GetAt( i ) )->GetID().Equals( id ) ){
			return true;
		}
	}
	
	return false;
}

void ceConversationTopicList::Add( ceConversationTopic *file ){
	if( ! file || HasWithID( file->GetID().GetString() ) ) DETHROW( deeInvalidParam );
	
	pTopics.Add( file );
}

void ceConversationTopicList::Remove( ceConversationTopic *file ){
	pTopics.Remove( file );
}

void ceConversationTopicList::RemoveAll(){
	pTopics.RemoveAll();
}



ceConversationTopicList &ceConversationTopicList::operator=( const ceConversationTopicList &list ){
	pTopics = list.pTopics;
	return *this;
}

ceConversationTopicList &ceConversationTopicList::operator+=( const ceConversationTopicList &list ){
	pTopics += list.pTopics;
	return *this;
}
