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

#include "gdeCategory.h"
#include "gdeCategoryList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeCategory
//////////////////////

// Constructor, destructor
////////////////////////////

gdeCategoryList::gdeCategoryList(){
}

gdeCategoryList::gdeCategoryList(const gdeCategoryList &list) :
pCategories(list.pCategories){
}

gdeCategoryList::~gdeCategoryList(){
}



// Management
///////////////

int gdeCategoryList::GetCount() const{
	return pCategories.GetCount();
}

gdeCategory *gdeCategoryList::GetAt(int index) const{
	return (gdeCategory*)pCategories.GetAt(index);
}

gdeCategory *gdeCategoryList::GetNamed(const char *name) const{
	const int count = pCategories.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		gdeCategory * const category = (gdeCategory*)pCategories.GetAt(i);
		
		if(category->GetName() == name){
			return category;
		}
	}
	
	return NULL;
}

gdeCategory *gdeCategoryList::GetWithPath(const char *path) const{
	const decStringList components(decString(path).Split('/'));
	const int count = components.GetCount();
	if(count == 0){
		return NULL;
	}
	
	gdeCategory *category = GetNamed(components.GetAt(0));
	if(! category){
		return NULL;
	}
	
	int i;
	for(i=1; i<count; i++){
		category = category->GetCategories().GetNamed(components.GetAt(i));
		if(! category){
			return NULL;
		}
	}
	
	return category;
}

int gdeCategoryList::IndexOf(gdeCategory *category) const{
	return pCategories.IndexOf(category);
}

int gdeCategoryList::IndexOfNamed(const char *name) const{
	const int count = pCategories.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((gdeCategory*)pCategories.GetAt(i))->GetName() == name){
			return i;
		}
	}
	
	return -1;
}

bool gdeCategoryList::Has(gdeCategory *category) const{
	return pCategories.Has(category);
}

bool gdeCategoryList::HasNamed(const char *name) const{
	const int count = pCategories.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((gdeCategory*)pCategories.GetAt(i))->GetName() == name){
			return true;
		}
	}
	
	return false;
}

void gdeCategoryList::Add(gdeCategory *category){
	if(! category || HasNamed(category->GetName())){
		DETHROW(deeInvalidParam);
	}
	
	pCategories.Add(category);
}

void gdeCategoryList::Remove(gdeCategory *category){
	pCategories.Remove(category);
}

void gdeCategoryList::RemoveAll(){
	pCategories.RemoveAll();
}



gdeCategoryList &gdeCategoryList::operator=(const gdeCategoryList &list){
	pCategories = list.pCategories;
	return *this;
}
