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

#include "ceUCGestureRemove.h"
#include "../../conversation/ceConversation.h"
#include "../../conversation/gesture/ceGesture.h"

#include <dragengine/common/exceptions.h>



// Class ceUCGestureRemove
////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCGestureRemove::ceUCGestureRemove( ceGesture *gesture ){
	if( ! gesture ) DETHROW( deeInvalidParam );
	
	ceConversation *conversation = gesture->GetConversation();
	if( ! conversation ) DETHROW( deeInvalidParam );
	
	pConversation = NULL;
	pGesture = NULL;
	
	SetShortInfo( "Remove Gesture" );
	
	pConversation = conversation;
	conversation->AddReference();
	
	pGesture = gesture;
	gesture->AddReference();
}

ceUCGestureRemove::~ceUCGestureRemove(){
}



// Management
///////////////

void ceUCGestureRemove::Undo(){
	pConversation->AddGesture( pGesture );
}

void ceUCGestureRemove::Redo(){
	pConversation->RemoveGesture( pGesture );
}
