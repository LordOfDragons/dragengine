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

#include "ceUCLookAtAdd.h"
#include "../../conversation/ceConversation.h"
#include "../../conversation/lookat/ceLookAt.h"

#include <dragengine/common/exceptions.h>



// Class ceUCLookAtAdd
////////////////////////

// Constructor, destructor
////////////////////////////

ceUCLookAtAdd::ceUCLookAtAdd( ceConversation *conversation, ceLookAt *lookat ){
	if( ! conversation || ! lookat ) DETHROW( deeInvalidParam );
	
	pConversation = NULL;
	pLookAt = NULL;
	
	SetShortInfo( "Add LookAt" );
	
	pConversation = conversation;
	conversation->AddReference();
	
	pLookAt = lookat;
	lookat->AddReference();
}

ceUCLookAtAdd::~ceUCLookAtAdd(){
	if( pLookAt ){
		pLookAt->FreeReference();
	}
	if( pConversation ){
		pConversation->FreeReference();
	}
}



// Management
///////////////

void ceUCLookAtAdd::Undo(){
	pConversation->RemoveLookAt( pLookAt );
}

void ceUCLookAtAdd::Redo(){
	pConversation->AddLookAt( pLookAt );
	pConversation->SetActiveLookAt( pLookAt );
}
