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

#include "igdeGDCategory.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>



// Class igdeGDCategory
/////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCategory::igdeGDCategory(const char *name) :
pName(name),
pHidden(false),
pParent(NULL)
{
	if(pName.IsEmpty()){
		DETHROW(deeInvalidParam);
	}
}

igdeGDCategory::~igdeGDCategory(){
	RemoveAllCategories();
}



// Management
///////////////

void igdeGDCategory::SetDescription(const char *description){
	pDescription = description;
}

void igdeGDCategory::SetParent(igdeGDCategory *parent){
	pParent = parent;
}

void igdeGDCategory::SetHidden(bool hidden){
	pHidden = hidden;
}

void igdeGDCategory::GetFullPath(decPath &path) const{
	if(pParent){
		pParent->GetFullPath(path);
		path.AddComponent(pName);
		
	}else{
		path.SetEmpty();
	}
}

decString igdeGDCategory::GetFullPathString() const{
	decPath path;
	GetFullPath(path);
	return path.GetPathUnix();
}

void igdeGDCategory::SetAutoCategorizePattern(const decStringSet &patternList){
	pAutoCategorizePattern = patternList;
}

igdeGDCategory *igdeGDCategory::AutoCategorize(const decString &path) const{
	igdeGDCategory *category = NULL;
	int longestMatch = 0;
	pAutoCategorize(path, longestMatch, category);
	return category;
}



// Children Categories
////////////////////////

int igdeGDCategory::GetCategoryCount() const{
	return pCategories.GetCount();
}

bool igdeGDCategory::HasCategory(igdeGDCategory *category) const{
	return pCategories.Has(category);
}

bool igdeGDCategory::HasCategoryNamed(const char *name) const{
	return IndexOfCategoryNamed(name) != -1;
}

int igdeGDCategory::IndexOfCategory(igdeGDCategory *category) const{
	return pCategories.IndexOf(category);
}

int igdeGDCategory::IndexOfCategoryNamed(const char *name) const{
	if(! name){
		DETHROW(deeInvalidParam);
	}
	const int count = pCategories.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((igdeGDCategory*)pCategories.GetAt(i))->GetName() == name){
			return i;
		}
	}
	
	return -1;
}

igdeGDCategory *igdeGDCategory::GetCategoryAt(int index) const{
	return (igdeGDCategory*)pCategories.GetAt(index);
}

igdeGDCategory *igdeGDCategory::GetCategoryNamed(const char *name) const{
	if(! name){
		DETHROW(deeInvalidParam);
	}
	const int count = pCategories.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		igdeGDCategory * const category = (igdeGDCategory*)pCategories.GetAt(i);
		if(category->GetName() == name){
			return category;
		}
	}
	
	return NULL;
}

igdeGDCategory *igdeGDCategory::GetCategoryWithPath(const decPath &path) const{
	const int count = path.GetComponentCount();
	igdeGDCategory *category = NULL;
	int i;
	
	for(i=0; i<count; i++){
		if(category){
			category = category->GetCategoryNamed(path.GetComponentAt(i));
			
		}else{
			category = GetCategoryNamed(path.GetComponentAt(i));
		}
		
		if(! category){
			break;
		}
	}
	
	return category;
}

void igdeGDCategory::AddCategory(igdeGDCategory *category){
	if(! category || HasCategoryNamed(category->GetName())){
		DETHROW(deeInvalidParam);
	}
	pCategories.Add(category);
	category->SetParent(this);
}

void igdeGDCategory::RemoveCategory(igdeGDCategory *category){
	const int index = pCategories.IndexOf(category);
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	category->SetParent(NULL);
	pCategories.RemoveFrom(index);
}

void igdeGDCategory::RemoveAllCategories(){
	while(pCategories.GetCount() > 0){
		const int index = pCategories.GetCount() - 1;
		((igdeGDCategory*)pCategories.GetAt(index))->SetParent(NULL);
		pCategories.RemoveFrom(index);
	}
}



void igdeGDCategory::UpdateWith(const igdeGDCategory &category){
	const int count = category.GetCategoryCount();
	int i;
	
	pDescription = category.GetDescription();
	pAutoCategorizePattern += category.pAutoCategorizePattern;
	pHidden = category.GetHidden();
	
	for(i=0; i<count; i++){
		const igdeGDCategory &childCategory = *category.GetCategoryAt(i);
		igdeGDCategory *matchingCategory = GetCategoryNamed(childCategory.GetName());
		
		if(! matchingCategory){
			const igdeGDCategory::Ref newCategory(igdeGDCategory::Ref::NewWith(
				childCategory.GetName()));
			pCategories.Add(newCategory);
			matchingCategory = newCategory;
			matchingCategory->SetParent(this);
		}
		
		matchingCategory->UpdateWith(childCategory);
	}
}



// Private Functions
//////////////////////

void igdeGDCategory::pAutoCategorize(const decString &path, int &longestMatch,
igdeGDCategory *&category) const{
	const int count = pCategories.GetCount();
	int i, j;
	
	for(i=0; i<count; i++){
		igdeGDCategory * const checkCategory = (igdeGDCategory*)pCategories.GetAt(i);
		const decStringSet &patternList = checkCategory->GetAutoCategorizePattern();
		const int patternCount = patternList.GetCount();
		
		for(j=0; j<patternCount; j++){
			const decString &pattern = patternList.GetAt(j);
			const int patternLen = pattern.GetLength();
			if(patternLen > longestMatch && path.MatchesPattern(pattern)){
				category = checkCategory;
				longestMatch = patternLen;
				break;
			}
		}
		
		checkCategory->pAutoCategorize(path, longestMatch, category);
	}
}
