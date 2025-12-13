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

#include "projProfile.h"
#include "projProfileList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class projProfile
///////////////////////

// Constructor, destructor
////////////////////////////

projProfileList::projProfileList(){
}

projProfileList::projProfileList(const projProfileList &copy) :
pProfiles(copy.pProfiles){
}

projProfileList::~projProfileList(){
}



// Management
///////////////

int projProfileList::GetCount() const{
	return pProfiles.GetCount();
}

projProfile *projProfileList::GetAt(int index) const{
	return (projProfile*)pProfiles.GetAt(index);
}

projProfile *projProfileList::GetNamed(const char *name) const{
	const int count = pProfiles.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		projProfile * const profile = (projProfile*)pProfiles.GetAt(i);
		
		if(profile->GetName() == name){
			return profile;
		}
	}
	
	return nullptr;
}

int projProfileList::IndexOf(projProfile *profile) const{
	return pProfiles.IndexOf(profile);
}

int projProfileList::IndexOfNamed(const char *name) const{
	const int count = pProfiles.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((projProfile*)pProfiles.GetAt(i))->GetName() == name){
			return i;
		}
	}
	
	return -1;
}

bool projProfileList::Has(projProfile *profile) const{
	return pProfiles.Has(profile);
}

bool projProfileList::HasNamed(const char *name) const{
	const int count = pProfiles.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((projProfile*)pProfiles.GetAt(i))->GetName() == name){
			return true;
		}
	}
	
	return false;
}

void projProfileList::Add(projProfile *profile){
	if(!profile || HasNamed(profile->GetName())){
		DETHROW(deeInvalidParam);
	}
	
	pProfiles.Add(profile);
}

void projProfileList::Remove(projProfile *profile){
	pProfiles.Remove(profile);
}

void projProfileList::RemoveAll(){
	pProfiles.RemoveAll();
}



projProfileList &projProfileList::operator=(const projProfileList &list){
	pProfiles = list.pProfiles;
	return *this;
}
