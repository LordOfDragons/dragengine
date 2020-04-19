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

#include "cePlaybackActionStack.h"
#include "cePlaybackActionStackEntry.h"
#include "../action/ceConversationAction.h"
#include "../action/ceConversationActionList.h"

#include <dragengine/common/exceptions.h>



// Class cePlaybackActionStack
////////////////////////////////

// Constructor, destructor
////////////////////////////

cePlaybackActionStack::cePlaybackActionStack(){
	pEntries = NULL;
	pEntryCount = 0;
	pEntrySize = 0;
	
	SetStackSize( 100 );
}

cePlaybackActionStack::~cePlaybackActionStack(){
	if( pEntries ){
		Clear();
		delete [] pEntries;
	}
}



// Management
///////////////

void cePlaybackActionStack::SetStackSize( int size ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	Clear();
	
	cePlaybackActionStackEntry *newArray = new cePlaybackActionStackEntry[ size ];
	if( pEntries ){
		delete [] pEntries;
	}
	pEntries = newArray;
	pEntrySize = size;
}

cePlaybackActionStackEntry &cePlaybackActionStack::GetTop() const{
	if( pEntryCount == 0 ){
		DETHROW( deeStackEmpty );
	}
	
	return pEntries[ pEntryCount - 1 ];
}

cePlaybackActionStackEntry &cePlaybackActionStack::GetAt( int position ) const{
	if( position < 0 || position >= pEntryCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pEntries[ pEntryCount - 1 - position ];
}

void cePlaybackActionStack::Push( ceConversationTopic *topic,
ceConversationAction *action, const ceConversationActionList *list, int index ){
	if( ( ! topic && ! action ) || ! list || index < 0 ){
		DETHROW( deeInvalidParam );
	}
	if( pEntryCount == pEntrySize ){
		DETHROW( deeStackOverflow );
	}
	
	pEntries[ pEntryCount ].SetParentTopic( topic );
	pEntries[ pEntryCount ].SetParentAction( action );
	pEntries[ pEntryCount ].SetParentList( list );
	pEntries[ pEntryCount ].SetNextIndex( index );
	pEntryCount++;
}

void cePlaybackActionStack::Pop(){
	if( pEntryCount == 0 ){
		DETHROW( deeStackEmpty );
	}
	
	pEntryCount--;
	pEntries[ pEntryCount ].Clear();
}

bool cePlaybackActionStack::HasNextAction() const{
	if( pEntryCount == 0 ){
		return NULL;
		
	}else{
		return pEntries[ pEntryCount - 1 ].HasNextAction();
	}
}

void cePlaybackActionStack::Clear(){
	while( pEntryCount > 0 ){
		pEntryCount--;
		pEntries[ pEntryCount ].Clear();
	}
}
