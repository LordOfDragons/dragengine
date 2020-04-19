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

#include "ceUCCShotSetOffLookAtTo.h"
#include "../../conversation/camerashot/ceCameraShot.h"

#include <dragengine/common/exceptions.h>



// Class ceUCCShotSetOffLookAtTo
//////////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCCShotSetOffLookAtTo::ceUCCShotSetOffLookAtTo( ceCameraShot *cameraShot, const decVector &newPos ){
	if( ! cameraShot ) DETHROW( deeInvalidParam );
	
	pCameraShot = NULL;
	
	SetShortInfo( "Camera Shot Set End Look-At Position" );
	
	pOldPos = cameraShot->GetOffsetLookAtTo();
	pNewPos = newPos;
	
	pCameraShot = cameraShot;
	cameraShot->AddReference();
}

ceUCCShotSetOffLookAtTo::~ceUCCShotSetOffLookAtTo(){
	if( pCameraShot ){
		pCameraShot->FreeReference();
	}
}



// Management
///////////////

void ceUCCShotSetOffLookAtTo::Undo(){
	pCameraShot->SetOffsetLookAtTo( pOldPos );
}

void ceUCCShotSetOffLookAtTo::Redo(){
	pCameraShot->SetOffsetLookAtTo( pNewPos );
}
