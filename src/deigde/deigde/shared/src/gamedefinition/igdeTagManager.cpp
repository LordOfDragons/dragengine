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

#include "igdeTagManager.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringSet.h>



// Class igdeTagManager
/////////////////////////

// Constructor, destructor
////////////////////////////

igdeTagManager::igdeTagManager(){
}

igdeTagManager::igdeTagManager(const igdeTagManager &tagManager){
	pTags = tagManager.pTags;
}

igdeTagManager::~igdeTagManager(){
}



// Management
///////////////

int igdeTagManager::GetTagCount() const{
	return pTags.GetCount();
}

const decString &igdeTagManager::GetTagAt(int index) const{
	return pTags.GetAt(index);
}

bool igdeTagManager::HasTag(const char *tag) const{
	return pTags.Has(tag);
}

int igdeTagManager::IndexOfTag(const char *tag) const{
	if(! tag){
		DETHROW(deeInvalidParam);
	}
	
	return pTags.IndexOf(tag);
}

void igdeTagManager::AddTag(const char *tag){
	if(! tag || ! tag[0]){
		DETHROW(deeInvalidParam);
	}
	
	if(! pTags.Has(tag)){
		pTags.Add(tag);
	}
}

void igdeTagManager::RemoveTag(const char *tag){
	const int index = pTags.IndexOf(tag);
	
	if(index != -1){
		pTags.RemoveFrom(index);
	}
}

void igdeTagManager::RemoveAllTags(){
	pTags.RemoveAll();
}



bool igdeTagManager::HasAllOf(const igdeTagManager &tags) const{
	const int tagCount = tags.GetTagCount();
	int i;
	
	if(tagCount == 0){
		return false;
	}
	
	for(i=0; i<tagCount; i++){
		if(! pTags.Has(tags.GetTagAt(i))){
			return false;
		}
	}
	
	return true;
}

bool igdeTagManager::HasAnyOf(const igdeTagManager &tags) const{
	const int tagCount = tags.GetTagCount();
	int i;
	
	for(i=0; i<tagCount; i++){
		if(pTags.Has(tags.GetTagAt(i))){
			return true;
		}
	}
	
	return false;
}

bool igdeTagManager::HasNoneOf(const igdeTagManager &tags) const{
	return ! HasAnyOf(tags);
}



void igdeTagManager::AddTagsFrom(const igdeTagManager &tags){
	const int tagCount = tags.GetTagCount();
	int i;
	
	for(i=0; i<tagCount; i++){
		AddTag(tags.GetTagAt(i));
	}
}

decStringSet igdeTagManager::ToSet() const{
	decStringSet set;
	
	const int count = pTags.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		set.Add(pTags.GetAt(i));
	}
	
	return set;
}



// Operators
//////////////

igdeTagManager &igdeTagManager::operator=(const igdeTagManager &tags){
	pTags = tags.pTags;
	return *this;
}

igdeTagManager &igdeTagManager::operator=(const decStringSet &tags){
	const int count = tags.GetCount();
	int i;
	
	pTags.RemoveAll();
	for(i=0; i<count; i++){
		pTags.Add(tags.GetAt(i));
	}
	
	return *this;
}

igdeTagManager::operator decStringSet() const{
	return ToSet();
}
