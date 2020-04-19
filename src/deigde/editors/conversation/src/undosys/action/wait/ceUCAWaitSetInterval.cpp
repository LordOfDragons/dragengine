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
#include <string.h>
#include <stdlib.h>

#include "ceUCAWaitSetInterval.h"
#include "../../../conversation/action/ceCAWait.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <dragengine/common/exceptions.h>



// Class ceUCAWaitSetInterval
///////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCAWaitSetInterval::ceUCAWaitSetInterval( ceConversationTopic *topic, ceCAWait *wait, float newInterval ){
	if( ! topic || ! wait ) DETHROW( deeInvalidParam );
	
	pTopic = NULL;
	pWait = NULL;
	
	SetShortInfo( "Action Wait Set Interval" );
	
	pTopic = topic;
	topic->AddReference();
	
	pWait = wait;
	wait->AddReference();
	
	pOldInterval = wait->GetInterval();
	pNewInterval = newInterval;
}

ceUCAWaitSetInterval::~ceUCAWaitSetInterval(){
	if( pWait ){
		pWait->FreeReference();
	}
	if( pTopic ){
		pTopic->FreeReference();
	}
}



// Management
///////////////

void ceUCAWaitSetInterval::Undo(){
	pWait->SetInterval( pOldInterval );
	pTopic->NotifyActionStructureChanged( pWait );
}

void ceUCAWaitSetInterval::Redo(){
	pWait->SetInterval( pNewInterval );
	pTopic->NotifyActionStructureChanged( pWait );
}
