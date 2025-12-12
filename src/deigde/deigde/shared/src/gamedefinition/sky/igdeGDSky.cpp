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

#include "igdeGDSky.h"
#include "igdeGDSkyController.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>



// Class igdeGDSky
////////////////////

// Constructor, destructor
////////////////////////////

igdeGDSky::igdeGDSky(const char *path, const char *name) :
pPath(path),
pName(name){
}

igdeGDSky::igdeGDSky(const igdeGDSky &sky) :
pPath(sky.pPath),
pName(sky.pName),
pDescription(sky.pDescription),
pCategory(sky.pCategory)
{
	const int controllerCount = sky.GetControllerCount();
	int i;
	
	for(i=0; i<controllerCount; i++){
		pControllers.Add(igdeGDSkyController::Ref::New(*sky.GetControllerAt(i)));
	}
}

igdeGDSky::~igdeGDSky(){
}



// Management
///////////////

void igdeGDSky::SetPath(const char *path){
	pPath = path;
}

void igdeGDSky::SetName(const char *name){
	pName = name;
}

void igdeGDSky::SetDescription(const char *description){
	pDescription = description;
}

void igdeGDSky::SetCategory(const char *category){
	pCategory = category;
}

void igdeGDSky::SetPreviewImage(deImage *image){
	pPreviewImage = image;
}



// Controllers
////////////////

int igdeGDSky::GetControllerCount() const{
	return pControllers.GetCount();
}

igdeGDSkyController *igdeGDSky::GetControllerAt(int index) const{
	return (igdeGDSkyController*)pControllers.GetAt(index);
}

void igdeGDSky::AddController(igdeGDSkyController *controller){
	if(!controller){
		DETHROW(deeInvalidParam);
	}
	pControllers.Add(controller);
}

void igdeGDSky::RemoveAllControllers(){
	pControllers.RemoveAll();
}
