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

#include "ceUCGestureAdd.h"
#include "../../conversation/ceConversation.h"
#include "../../conversation/gesture/ceGesture.h"

#include <dragengine/common/exceptions.h>



// Class ceUCGestureAdd
/////////////////////////

// Constructor, destructor
////////////////////////////

ceUCGestureAdd::ceUCGestureAdd( ceConversation *conversation, ceGesture *gesture ){
	if( ! conversation || ! gesture ) DETHROW( deeInvalidParam );
	
	pConversation = NULL;
	pGesture = NULL;
	
	SetShortInfo( "Add Gesture" );
	
	pConversation = conversation;
	conversation->AddReference();
	
	pGesture = gesture;
	gesture->AddReference();
}

ceUCGestureAdd::~ceUCGestureAdd(){
	if( pGesture ){
		pGesture->FreeReference();
	}
	if( pConversation ){
		pConversation->FreeReference();
	}
}



// Management
///////////////

void ceUCGestureAdd::Undo(){
	pConversation->RemoveGesture( pGesture );
}

void ceUCGestureAdd::Redo(){
	pConversation->AddGesture( pGesture );
	pConversation->SetActiveGesture( pGesture );
}
