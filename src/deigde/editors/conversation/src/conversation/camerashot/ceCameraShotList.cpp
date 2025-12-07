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

#include "ceCameraShot.h"
#include "ceCameraShotList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceCameraShot
///////////////////////

// Constructor, destructor
////////////////////////////

ceCameraShotList::ceCameraShotList(){
}

ceCameraShotList::~ceCameraShotList(){
}



// Management
///////////////

int ceCameraShotList::GetCount() const{
	return pShots.GetCount();
}

ceCameraShot *ceCameraShotList::GetAt(int index) const{
	return (ceCameraShot*)pShots.GetAt(index);
}

ceCameraShot *ceCameraShotList::GetNamed(const char *name) const{
	const int count = pShots.GetCount();
	ceCameraShot *cameraShot;
	int i;
	
	for(i=0; i<count; i++){
		cameraShot = (ceCameraShot*)pShots.GetAt(i);
		
		if(cameraShot->GetName().Equals(name)){
			return cameraShot;
		}
	}
	
	return NULL;
}

int ceCameraShotList::IndexOf(ceCameraShot *cameraShot) const{
	return pShots.IndexOf(cameraShot);
}

int ceCameraShotList::IndexOfNamed(const char *name) const{
	const int count = pShots.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((ceCameraShot*)pShots.GetAt(i))->GetName().Equals(name)){
			return i;
		}
	}
	
	return -1;
}

bool ceCameraShotList::Has(ceCameraShot *cameraShot) const{
	return pShots.Has(cameraShot);
}

bool ceCameraShotList::HasNamed(const char *name) const{
	const int count = pShots.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((ceCameraShot*)pShots.GetAt(i))->GetName().Equals(name)){
			return true;
		}
	}
	
	return false;
}

void ceCameraShotList::Add(ceCameraShot *cameraShot){
	if(!cameraShot || HasNamed(cameraShot->GetName().GetString())) DETHROW(deeInvalidParam);
	
	pShots.Add(cameraShot);
}

void ceCameraShotList::Remove(ceCameraShot *cameraShot){
	pShots.Remove(cameraShot);
}

void ceCameraShotList::RemoveAll(){
	pShots.RemoveAll();
}



ceCameraShotList &ceCameraShotList::operator=(const ceCameraShotList &list){
	pShots = list.pShots;
	return *this;
}

ceCameraShotList &ceCameraShotList::operator+=(const ceCameraShotList &list){
	pShots += list.pShots;
	return *this;
}
