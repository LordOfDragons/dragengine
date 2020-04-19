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

#include "ceUCCShotSetFovFrom.h"
#include "../../conversation/camerashot/ceCameraShot.h"

#include <dragengine/common/exceptions.h>



// Class ceUCCShotSetFovFrom
//////////////////////////////

// Constructor, destructor
////////////////////////////

ceUCCShotSetFovFrom::ceUCCShotSetFovFrom( ceCameraShot *cameraShot, float newFov ){
	if( ! cameraShot ) DETHROW( deeInvalidParam );
	
	pCameraShot = NULL;
	
	SetShortInfo( "Camera Shot Set Start Fov" );
	
	pOldFov = cameraShot->GetFovFrom();
	pNewFov = newFov;
	
	pCameraShot = cameraShot;
	cameraShot->AddReference();
}

ceUCCShotSetFovFrom::~ceUCCShotSetFovFrom(){
	if( pCameraShot ){
		pCameraShot->FreeReference();
	}
}



// Management
///////////////

void ceUCCShotSetFovFrom::Undo(){
	pCameraShot->SetFovFrom( pOldFov );
}

void ceUCCShotSetFovFrom::Redo(){
	pCameraShot->SetFovFrom( pNewFov );
}
