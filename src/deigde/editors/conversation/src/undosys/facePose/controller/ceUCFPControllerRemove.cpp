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

#include "ceUCFPControllerRemove.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/facepose/ceFacePose.h"
#include "../../../utils/ceControllerValue.h"

#include <dragengine/common/exceptions.h>



// Class ceUCFPControllerRemove
//////////////////////////

// Constructor, destructor
////////////////////////////

ceUCFPControllerRemove::ceUCFPControllerRemove( ceFacePose *facePose, ceControllerValue *controller ){
	if( ! facePose || ! controller ){
		DETHROW( deeInvalidParam );
	}
	if( ! facePose->GetControllerList().Has( controller ) ){
		DETHROW( deeInvalidParam );
	}
	
	pFacePose = NULL;
	pController = NULL;
	
	SetShortInfo( "Remove Face Pose Controller" );
	
	pFacePose = facePose;
	facePose->AddReference();
	
	pController = controller;
	controller->AddReference();
}

ceUCFPControllerRemove::~ceUCFPControllerRemove(){
	if( pController ){
		pController->FreeReference();
	}
	if( pFacePose ){
		pFacePose->FreeReference();
	}
}



// Management
///////////////

void ceUCFPControllerRemove::Undo(){
	pFacePose->GetControllerList().Add( pController );
	pFacePose->NotifyControllersChanged();
}

void ceUCFPControllerRemove::Redo(){
	pFacePose->GetControllerList().Remove( pController );
	pFacePose->NotifyControllersChanged();
}
