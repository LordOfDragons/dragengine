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

#include "ceGesture.h"
#include "ceGestureList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceGesture
////////////////////

// Constructor, destructor
////////////////////////////

ceGestureList::ceGestureList(){
}

ceGestureList::~ceGestureList(){
}



// Management
///////////////

int ceGestureList::GetCount() const{
	return pGestures.GetCount();
}

ceGesture *ceGestureList::GetAt(int index) const{
	return (ceGesture*)pGestures.GetAt(index);
}

ceGesture *ceGestureList::GetNamed(const char *name) const{
	const int count = pGestures.GetCount();
	ceGesture *gesture;
	int i;
	
	for(i=0; i<count; i++){
		gesture = (ceGesture*)pGestures.GetAt(i);
		
		if(gesture->GetName().Equals(name)){
			return gesture;
		}
	}
	
	return NULL;
}

int ceGestureList::IndexOf(ceGesture *gesture) const{
	return pGestures.IndexOf(gesture);
}

int ceGestureList::IndexOfNamed(const char *name) const{
	const int count = pGestures.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((ceGesture*)pGestures.GetAt(i))->GetName().Equals(name)){
			return i;
		}
	}
	
	return -1;
}

bool ceGestureList::Has(ceGesture *gesture) const{
	return pGestures.Has(gesture);
}

bool ceGestureList::HasNamed(const char *name) const{
	const int count = pGestures.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((ceGesture*)pGestures.GetAt(i))->GetName().Equals(name)){
			return true;
		}
	}
	
	return false;
}

void ceGestureList::Add(ceGesture *gesture){
	if(!gesture || HasNamed(gesture->GetName().GetString())) DETHROW(deeInvalidParam);
	
	pGestures.Add(gesture);
}

void ceGestureList::Remove(ceGesture *gesture){
	pGestures.Remove(gesture);
}

void ceGestureList::RemoveAll(){
	pGestures.RemoveAll();
}



ceGestureList &ceGestureList::operator=(const ceGestureList &list){
	pGestures = list.pGestures;
	return *this;
}

ceGestureList &ceGestureList::operator+=(const ceGestureList &list){
	pGestures += list.pGestures;
	return *this;
}
