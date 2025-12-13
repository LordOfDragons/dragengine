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

#include "projTRProfile.h"
#include "projTRProfileList.h"

#include <dragengine/common/exceptions.h>



// Class projTRProfileList
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

projTRProfileList::projTRProfileList(){
}

projTRProfileList::~projTRProfileList(){
	RemoveAll();
}



// Management
///////////////

int projTRProfileList::GetCount() const{
	return pProfiles.GetCount();
}

projTRProfile *projTRProfileList::GetAt(int index) const{
	return (projTRProfile*)pProfiles.GetAt(index);
}

projTRProfile *projTRProfileList::GetNamed(const char *name) const{
	if(!name){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pProfiles.GetCount();
	projTRProfile *profile;
	int i;
	
	for(i=0; i<count; i++){
		profile = (projTRProfile*)pProfiles.GetAt(i);
		if(profile->GetName().Equals(name)){
			return profile;
		}
	}
	
	return nullptr;
}

bool projTRProfileList::Has(projTRProfile *profile) const{
	return pProfiles.Has(profile);
}

bool projTRProfileList::HasNamed(const char *name) const{
	return GetNamed(name) != nullptr;
}

int projTRProfileList::IndexOf(projTRProfile *profile) const{
	return pProfiles.IndexOf(profile);
}

int projTRProfileList::IndexOfNamed(const char *name) const{
	if(!name){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pProfiles.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((projTRProfile*)pProfiles.GetAt(i))->GetName().Equals(name)){
			return i;
		}
	}
	
	return -1;
}

void projTRProfileList::Add(projTRProfile *profile){
	if(!profile || HasNamed(profile->GetName().GetString())){
		DETHROW(deeInvalidParam);
	}
	
	pProfiles.Add(profile);
}

void projTRProfileList::Remove(projTRProfile *profile){
	const int index = IndexOf(profile);
	
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	pProfiles.RemoveFrom(index);
}

void projTRProfileList::RemoveAll(){
	pProfiles.RemoveAll();
}

void projTRProfileList::ValidateProfiles(deEngine &engine){
	const int count = pProfiles.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((projTRProfile*)pProfiles.GetAt(i))->Verify(engine);
	}
}
