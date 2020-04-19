/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include "gdeUOCCameraSetFov.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/camera/gdeOCCamera.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCCameraSetFov
/////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCCameraSetFov::gdeUOCCameraSetFov( gdeObjectClass *objectClass, gdeOCCamera *camera, float newValue ) :
pObjectClass( NULL ),
pCamera( NULL )
{
	if( ! objectClass || ! camera ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Camera set fov" );
	
	pOldValue = camera->GetFov();
	pNewValue = newValue;
	
	pCamera = camera;
	camera->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCCameraSetFov::~gdeUOCCameraSetFov(){
	if( pCamera ){
		pCamera->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCCameraSetFov::Undo(){
	pCamera->SetFov( pOldValue );
	pObjectClass->NotifyCameraChanged( pCamera );
}

void gdeUOCCameraSetFov::Redo(){
	pCamera->SetFov( pNewValue );
	pObjectClass->NotifyCameraChanged( pCamera );
}
