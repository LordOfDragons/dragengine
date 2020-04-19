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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cePlaybackActionStackEntry.h"
#include "../action/ceConversationAction.h"
#include "../action/ceConversationActionList.h"
#include "../topic/ceConversationTopic.h"
#include "../condition/ceConversationCondition.h"

#include <dragengine/common/exceptions.h>



// Class cePlaybackActionStackEntry
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

cePlaybackActionStackEntry::cePlaybackActionStackEntry() :
pParentTopic( NULL ),
pParentList( NULL ),

pLoopCondition( NULL ),
pLooping( false ),

pNextIndex( 0 ){
}

cePlaybackActionStackEntry::~cePlaybackActionStackEntry(){
	Clear();
}



// Management
///////////////

void cePlaybackActionStackEntry::SetParentTopic( ceConversationTopic *topic ){
	if( topic == pParentTopic ){
		return;
	}
	
	if( pParentTopic ){
		pParentTopic->FreeReference();
	}
	
	pParentTopic = topic;
	
	if( topic ){
		topic->AddReference();
	}
}

void cePlaybackActionStackEntry::SetParentAction( ceConversationAction *action ){
	pParentAction = action;
}

void cePlaybackActionStackEntry::SetParentList( const ceConversationActionList *list ){
	pParentList = list;
}



void cePlaybackActionStackEntry::SetLoopCondition( ceConversationCondition *condition ){
	if( condition == pLoopCondition ){
		return;
	}
	
	if( pLoopCondition ){
		pLoopCondition->FreeReference();
	}
	
	pLoopCondition = condition;
	
	if( condition ){
		condition->AddReference();
	}
}

void cePlaybackActionStackEntry::SetLooping( bool looping ){
	pLooping = looping;
}



void cePlaybackActionStackEntry::SetNextIndex( int index ){
	if( ! pParentList ){
		pNextIndex = 0;
		pNextAction = NULL;
		return;
	}
	
	if( index < 0 || index > pParentList->GetCount() ){
		DETHROW( deeInvalidParam );
	}
	
	pNextIndex = index;
	
	if( pNextIndex < pParentList->GetCount() ){
		pNextAction = pParentList->GetAt( pNextIndex );
		
	}else{
		pNextAction = NULL;
	}
}

void cePlaybackActionStackEntry::AdvanceIndex(){
	if( ! pParentList ){
		pNextIndex = 0;
		pNextAction = NULL;
		return;
	}
	
	const int nextIndex = pNextIndex + 1;
	
	if( nextIndex < pParentList->GetCount() ){
		SetNextIndex( nextIndex );
		
	}else if( pLooping ){
		SetNextIndex( 0 );
		
	}else{
		SetNextIndex( pParentList->GetCount() );
	}
}

bool cePlaybackActionStackEntry::HasNextAction() const{
	return pNextAction;
}

void cePlaybackActionStackEntry::Clear(){
	pNextAction = NULL;
	pNextIndex = 0;
	
	SetLoopCondition( NULL );
	SetLooping( false );
	
	SetParentList( NULL );
	SetParentAction( NULL );
	SetParentTopic( NULL );
}

void cePlaybackActionStackEntry::ForwardLast(){
	if( ! pParentList ){
		pNextIndex = 0;
		pNextAction = NULL;
		return;
	}
	
	if( pParentList->GetCount() == 0 ){
		SetNextIndex( 0 );
		
	}else{
		SetNextIndex( pParentList->GetCount() - 1 );
	}
}

void cePlaybackActionStackEntry::ForwardEnd(){
	if( ! pParentList ){
		pNextIndex = 0;
		pNextAction = NULL;
		return;
	}
	
	SetNextIndex( pParentList->GetCount() );
}

void cePlaybackActionStackEntry::CancelLooping(){
	SetLooping( false );
	SetLoopCondition( NULL );
	ForwardEnd();
}
