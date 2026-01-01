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

#include "gdeCategory.h"

#include <dragengine/common/exceptions.h>


// Class gdeCategory
//////////////////////

// Constructor, destructor
////////////////////////////

gdeCategory::gdeCategory() :
pParent(nullptr),
pHidden(false){
}

gdeCategory::gdeCategory(const char *name) :
pName(name),
pParent(nullptr),
pHidden(false){
}

gdeCategory::gdeCategory(const gdeCategory &category) :
pName(category.pName),
pDescription(category.pDescription),
pParent(nullptr),
pAutoCategorizePattern(category.pAutoCategorizePattern),
pHidden(category.pHidden)
{
	category.pCategories.Visit([&](const gdeCategory &c){
		gdeCategory::Ref sc = gdeCategory::Ref::New(c);
		sc->pParent = this;
		pCategories.Add(sc);
	});
}

gdeCategory::~gdeCategory(){
}



// Management
///////////////

void gdeCategory::SetName(const char *name){
	pName = name;
}

void gdeCategory::SetDescription(const char *description){
	pDescription = description;
}

void gdeCategory::SetParent(gdeCategory *parent){
	pParent = parent;
}

decString gdeCategory::GetPath() const{
	return pParent ? pParent->GetPath() + "/" + pName : pName;
}

void gdeCategory::SetAutoCategorizePattern(const decStringSet &patternList){
	pAutoCategorizePattern = patternList;
}

void gdeCategory::SetHidden(bool hidden){
	pHidden = hidden;
}



void gdeCategory::AddCategory(gdeCategory *category){
	if(!category || category->GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	pCategories.Add(category);
	category->SetParent(this);
}

void gdeCategory::RemoveCategory(gdeCategory *category){
	const gdeCategory:: Ref guard(category);
	pCategories.Remove(category);
	
	category->SetParent(nullptr);
}

void gdeCategory::RemoveAllCategories(){
	pCategories.Visit([&](gdeCategory &c){
		c.SetParent(nullptr);
	});
	pCategories.RemoveAll();
}


gdeCategory &gdeCategory::operator=(const gdeCategory &category){
	pName = category.pName;
	pDescription = category.pDescription;
	pAutoCategorizePattern = category.pAutoCategorizePattern;
	
	pCategories.RemoveAll();
	
	category.pCategories.Visit([&](const gdeCategory &c){
		gdeCategory::Ref sc = gdeCategory::Ref::New(c);
		sc->pParent = this;
		pCategories.Add(sc);
	});
	
	return *this;
}
