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
pPropName( camera.pPropName ),
pPropPosition(camera.pPropPosition),
pPropRotation(camera.pPropRotation){
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

void gdeOCCamera::SetPropName(const char *property){
	pPropName = property;
}

void gdeOCCamera::SetPropPosition(const char *property){
	pPropPosition = property;
}

void gdeOCCamera::SetPropRotation(const char *property){
	pPropRotation = property;
}
