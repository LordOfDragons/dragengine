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
#include "igdeGDSkyList.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDSky
////////////////////

// Constructor, destructor
////////////////////////////

igdeGDSkyList::igdeGDSkyList(){
}

igdeGDSkyList::~igdeGDSkyList(){
}



// Management
///////////////

int igdeGDSkyList::GetCount() const{
	return pSkies.GetCount();
}

igdeGDSky *igdeGDSkyList::GetAt(int index) const{
	return (igdeGDSky*)pSkies.GetAt(index);
}

igdeGDSky *igdeGDSkyList::GetWithPath(const char *path) const{
	if(! path){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pSkies.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		igdeGDSky * const sky = (igdeGDSky*)pSkies.GetAt(i);
		if(sky->GetPath().Equals(path)){
			return sky;
		}
	}
	
	return NULL;
}

igdeGDSky *igdeGDSkyList::GetWithName(const char *name) const{
	if(! name){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pSkies.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		igdeGDSky * const sky = (igdeGDSky*)pSkies.GetAt(i);
		if(sky->GetName().Equals(name)){
			return sky;
		}
	}
	
	return NULL;
}

int igdeGDSkyList::IndexOf(igdeGDSky *sky) const{
	return pSkies.IndexOf(sky);
}

int igdeGDSkyList::IndexOfWithPath(const char *path) const{
	if(! path){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pSkies.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((igdeGDSky*)pSkies.GetAt(i))->GetPath() == path){
			return i;
		}
	}
	
	return -1;
}

int igdeGDSkyList::IndexOfWithName(const char *name) const{
	if(! name){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pSkies.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((igdeGDSky*)pSkies.GetAt(i))->GetName() == name){
			return i;
		}
	}
	
	return -1;
}

int igdeGDSkyList::IndexOfWithPathOrName(const char *path, const char *name) const{
	if(! name){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pSkies.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const igdeGDSky &sky = *((igdeGDSky*)pSkies.GetAt(i));
		if(sky.GetPath() == path || sky.GetName() == name){
			return i;
		}
	}
	
	return -1;
}

bool igdeGDSkyList::Has(igdeGDSky *sky) const{
	return pSkies.Has(sky);
}

bool igdeGDSkyList::HasWithPath(const char *path) const{
	return IndexOfWithPath(path) != -1;
}

bool igdeGDSkyList::HasWithName(const char *name) const{
	return IndexOfWithName(name) != -1;
}

bool igdeGDSkyList::HasWithPathOrName(const char *path, const char *name) const{
	return IndexOfWithPathOrName(path, name) != -1;
}

void igdeGDSkyList::Add(igdeGDSky *sky){
	if(! sky || HasWithPathOrName(sky->GetPath(), sky->GetName())){
		DETHROW(deeInvalidParam);
	}
	pSkies.Add(sky);
}

void igdeGDSkyList::InsertAt(igdeGDSky *sky, int index){
	if(! sky){
		DETHROW(deeInvalidParam);
	}
	pSkies.Insert(sky, index);
}

void igdeGDSkyList::MoveTo(igdeGDSky *sky, int index){
	pSkies.Move(sky, index);
}

void igdeGDSkyList::Remove(igdeGDSky *sky){
	pSkies.Remove(sky);
}

void igdeGDSkyList::RemoveAll(){
	pSkies.RemoveAll();
}



igdeGDSkyList &igdeGDSkyList::operator=(const igdeGDSkyList &list){
	pSkies = list.pSkies;
	return *this;
}
