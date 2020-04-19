/* 
 * Drag[en]gine IGDE
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
#include <stdlib.h>
#include <string.h>

#include "igdeGDCamera.h"

#include "dragengine/common/exceptions.h"



// Class igdeGDCamera
///////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCamera::igdeGDCamera(){
	pFov = 90.0f;
	pFovRatio = 1.0f;
	pImageDistance = 0.01f;
	pViewDistance = 500.0f;
	pPropName = "";
}

igdeGDCamera::igdeGDCamera( const igdeGDCamera &camera ){
	pFov = camera.GetFov();
	pFovRatio = camera.GetFovRatio();
	pImageDistance = camera.GetImageDistance();
	pViewDistance = camera.GetViewDistance();
	pPosition = camera.GetPosition();
	pOrientation = camera.GetOrientation();
	
	pPropName = camera.GetPropName();
}

igdeGDCamera::~igdeGDCamera(){
}



// Management
///////////////

void igdeGDCamera::SetPosition( const decVector &position ){
	pPosition = position;
}

void igdeGDCamera::SetOrientation( const decQuaternion &orientation ){
	pOrientation = orientation;
}

void igdeGDCamera::SetFov( float fov ){
	if( fov <= 0.0f || fov >= 180.0f ) DETHROW( deeInvalidParam );
	
	pFov = fov;
}

void igdeGDCamera::SetFovRatio( float ratio ){
	if( ratio <= 0.0 ) DETHROW( deeInvalidParam );
	
	pFovRatio = ratio;
}

void igdeGDCamera::SetImageDistance( float distance ){
	if( distance <= 0.0f ) DETHROW( deeInvalidParam );
	
	pImageDistance = distance;
}

void igdeGDCamera::SetViewDistance( float distance ){
	if( distance <= 0.0f ) DETHROW( deeInvalidParam );
	
	pViewDistance = distance;
}

void igdeGDCamera::SetPropName( const char *propName ){
	pPropName = propName;
}
