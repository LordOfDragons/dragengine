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

#include "ceActorPose.h"
#include "ceActorPoseList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceActorPose
//////////////////////

// Constructor, destructor
////////////////////////////

ceActorPoseList::ceActorPoseList(){
}

ceActorPoseList::~ceActorPoseList(){
}



// Management
///////////////

int ceActorPoseList::GetCount() const{
	return pGestures.GetCount();
}

ceActorPose *ceActorPoseList::GetAt(int index) const{
	return (ceActorPose*)pGestures.GetAt(index);
}

ceActorPose *ceActorPoseList::GetNamed(const char *name) const{
	const int count = pGestures.GetCount();
	ceActorPose *pose;
	int i;
	
	for(i=0; i<count; i++){
		pose = (ceActorPose*)pGestures.GetAt(i);
		
		if(pose->GetName().Equals(name)){
			return pose;
		}
	}
	
	return NULL;
}

int ceActorPoseList::IndexOf(ceActorPose *pose) const{
	return pGestures.IndexOf(pose);
}

int ceActorPoseList::IndexOfNamed(const char *name) const{
	const int count = pGestures.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((ceActorPose*)pGestures.GetAt(i))->GetName().Equals(name)){
			return i;
		}
	}
	
	return -1;
}

bool ceActorPoseList::Has(ceActorPose *pose) const{
	return pGestures.Has(pose);
}

bool ceActorPoseList::HasNamed(const char *name) const{
	const int count = pGestures.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((ceActorPose*)pGestures.GetAt(i))->GetName().Equals(name)){
			return true;
		}
	}
	
	return false;
}

void ceActorPoseList::Add(ceActorPose *pose){
	if(! pose || HasNamed(pose->GetName().GetString())) DETHROW(deeInvalidParam);
	
	pGestures.Add(pose);
}

void ceActorPoseList::Remove(ceActorPose *pose){
	pGestures.Remove(pose);
}

void ceActorPoseList::RemoveAll(){
	pGestures.RemoveAll();
}



ceActorPoseList &ceActorPoseList::operator=(const ceActorPoseList &list){
	pGestures = list.pGestures;
	return *this;
}
