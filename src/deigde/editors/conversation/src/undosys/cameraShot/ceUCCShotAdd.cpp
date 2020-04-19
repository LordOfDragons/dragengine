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

#include "ceUCCShotAdd.h"
#include "../../conversation/ceConversation.h"
#include "../../conversation/camerashot/ceCameraShot.h"

#include <dragengine/common/exceptions.h>



// Class ceUCCShotAdd
///////////////////////

// Constructor, destructor
////////////////////////////

ceUCCShotAdd::ceUCCShotAdd( ceConversation *conversation, ceCameraShot *cameraShot ){
	if( ! conversation || ! cameraShot ) DETHROW( deeInvalidParam );
	
	pConversation = NULL;
	pCameraShot = NULL;
	
	SetShortInfo( "Add Camera Shot" );
	
	pConversation = conversation;
	conversation->AddReference();
	
	pCameraShot = cameraShot;
	cameraShot->AddReference();
}

ceUCCShotAdd::~ceUCCShotAdd(){
	if( pCameraShot ){
		pCameraShot->FreeReference();
	}
	if( pConversation ){
		pConversation->FreeReference();
	}
}



// Management
///////////////

void ceUCCShotAdd::Undo(){
	pConversation->RemoveCameraShot( pCameraShot );
}

void ceUCCShotAdd::Redo(){
	pConversation->AddCameraShot( pCameraShot );
	pConversation->SetActiveCameraShot( pCameraShot );
}
