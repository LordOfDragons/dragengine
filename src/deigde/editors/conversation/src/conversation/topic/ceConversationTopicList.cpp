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
