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

#include "gdeSkin.h"
#include "gdeSkinList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeSkin
//////////////////

// Constructor, destructor
////////////////////////////

gdeSkinList::gdeSkinList(){
}

gdeSkinList::gdeSkinList(const gdeSkinList &list) :
pSkins(list.pSkins){
}

gdeSkinList::~gdeSkinList(){
}



// Management
///////////////

int gdeSkinList::GetCount() const{
	return pSkins.GetCount();
}

gdeSkin *gdeSkinList::GetAt(int index) const{
	return (gdeSkin*)pSkins.GetAt(index);
}

gdeSkin *gdeSkinList::GetWithPath(const char *path) const{
	const int count = pSkins.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		gdeSkin * const skin = (gdeSkin*)pSkins.GetAt(i);
		
		if(skin->GetPath() == path){
			return skin;
		}
	}
	
	return NULL;
}

int gdeSkinList::IndexOf(gdeSkin *skin) const{
	return pSkins.IndexOf(skin);
}

int gdeSkinList::IndexOfWithPath(const char *path) const{
	const int count = pSkins.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((gdeSkin*)pSkins.GetAt(i))->GetPath() == path){
			return i;
		}
	}
	
	return -1;
}

bool gdeSkinList::Has(gdeSkin *skin) const{
	return pSkins.Has(skin);
}

bool gdeSkinList::HasWithPath(const char *path) const{
	const int count = pSkins.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((gdeSkin*)pSkins.GetAt(i))->GetPath() == path){
			return true;
		}
	}
	
	return false;
}

void gdeSkinList::Add(gdeSkin *skin){
	if(! skin || HasWithPath(skin->GetPath())){
		DETHROW(deeInvalidParam);
	}
	
	pSkins.Add(skin);
}

void gdeSkinList::Remove(gdeSkin *skin){
	pSkins.Remove(skin);
}

void gdeSkinList::RemoveAll(){
	pSkins.RemoveAll();
}



gdeSkinList &gdeSkinList::operator=(const gdeSkinList &list){
	pSkins = list.pSkins;
	return *this;
}
