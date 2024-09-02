/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
