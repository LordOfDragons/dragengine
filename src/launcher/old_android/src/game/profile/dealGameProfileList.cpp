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

#include "dealGameProfile.h"
#include "dealGameProfileList.h"

#include "../../common/exceptions.h"



// Class dealGameProfileList
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dealGameProfileList::dealGameProfileList(){
}

dealGameProfileList::~dealGameProfileList(){
	RemoveAll();
}



// Management
///////////////

int dealGameProfileList::GetCount() const{
	return pProfiles.GetCount();
}

dealGameProfile *dealGameProfileList::GetAt(int index) const{
	return (dealGameProfile*)pProfiles.GetAt(index);
}

dealGameProfile *dealGameProfileList::GetNamed(const char *name) const{
	if(! name){
		DETHROW(deeInvalidParam);
	}
	
	int i, count = pProfiles.GetCount();
	dealGameProfile *profile;
	
	for(i=0; i<count; i++){
		profile = (dealGameProfile*)pProfiles.GetAt(i);
		if(profile->GetName().Equals(name)){
			return profile;
		}
	}
	
	return NULL;
}

bool dealGameProfileList::Has(dealGameProfile *profile) const{
	return pProfiles.Has(profile);
}

bool dealGameProfileList::HasNamed(const char *name) const{
	return GetNamed(name) != NULL;
}

int dealGameProfileList::IndexOf(dealGameProfile *profile) const{
	return pProfiles.IndexOf(profile);
}

int dealGameProfileList::IndexOfNamed(const char *name) const{
	if(! name){
		DETHROW(deeInvalidParam);
	}
	
	int i, count = pProfiles.GetCount();
	
	for(i=0; i<count; i++){
		if(((dealGameProfile*)pProfiles.GetAt(i))->GetName().Equals(name)){
			return i;
		}
	}
	
	return -1;
}

void dealGameProfileList::Add(dealGameProfile *profile){
	if(! profile || HasNamed(profile->GetName())){
		DETHROW(deeInvalidParam);
	}
	
	pProfiles.Add(profile);
}

void dealGameProfileList::Remove(dealGameProfile *profile){
	int index = IndexOf(profile);
	
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	pProfiles.RemoveFrom(index);
}

void dealGameProfileList::RemoveAll(){
	pProfiles.RemoveAll();
}

void dealGameProfileList::ValidateProfiles(dealLauncher &launcher){
	int i, count = pProfiles.GetCount();
	
	for(i=0; i<count; i++){
		((dealGameProfile*)pProfiles.GetAt(i))->Verify(launcher);
	}
}
