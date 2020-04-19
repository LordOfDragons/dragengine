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

#include "gdeUOCCameraSetPropertyName.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/camera/gdeOCCamera.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCCameraSetPropertyName
/////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCCameraSetPropertyName::gdeUOCCameraSetPropertyName( gdeObjectClass *objectClass,
gdeOCCamera *camera, const char *newValue ) :
pObjectClass( NULL ),
pCamera( NULL )
{
	if( ! objectClass || ! camera ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Camera set property name" );
	
	pOldValue = camera->GetPropName();
	pNewValue = newValue;
	
	pCamera = camera;
	camera->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCCameraSetPropertyName::~gdeUOCCameraSetPropertyName(){
	if( pCamera ){
		pCamera->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCCameraSetPropertyName::Undo(){
	pCamera->SetPropName( pOldValue );
	pObjectClass->NotifyCameraChanged( pCamera );
}

void gdeUOCCameraSetPropertyName::Redo(){
	pCamera->SetPropName( pNewValue );
	pObjectClass->NotifyCameraChanged( pCamera );
}
