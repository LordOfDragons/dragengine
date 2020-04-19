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
#include <stdlib.h>
#include <string.h>

#include "gdeOCCamera.h"

#include <dragengine/common/exceptions.h>



// Class gdeOCCamera
//////////////////////

// Constructor, destructor
////////////////////////////

gdeOCCamera::gdeOCCamera() :
pFov( 90.0f ),
pFovRatio( 1.0f ),
pImageDistance( 0.01f ),
pViewDistance( 500.0f ){
}

gdeOCCamera::gdeOCCamera( const gdeOCCamera &camera ) :
pFov( camera.pFov ),
pFovRatio( camera.pFovRatio ),
pImageDistance( camera.pImageDistance ),
pViewDistance( camera.pViewDistance ),
pPosition( camera.pPosition ),
pRotation( camera.pRotation ),

pPropName( camera.pPropName ){
}

gdeOCCamera::~gdeOCCamera(){
}



// Management
///////////////

void gdeOCCamera::SetPosition( const decVector &position ){
	pPosition = position;
}

void gdeOCCamera::SetRotation( const decVector &orientation ){
	pRotation = orientation;
}

void gdeOCCamera::SetFov( float fov ){
	pFov = decMath::clamp( fov, 0.0f, 180.0f );
}

void gdeOCCamera::SetFovRatio( float ratio ){
	pFovRatio = decMath::max( ratio, 0.0f );
}

void gdeOCCamera::SetImageDistance( float distance ){
	pImageDistance = decMath::max( distance, 0.0f );
}

void gdeOCCamera::SetViewDistance( float distance ){
	pViewDistance = decMath::max( distance, 0.0f );
}

void gdeOCCamera::SetPropName( const char *propName ){
	pPropName = propName;
}
