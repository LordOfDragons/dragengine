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
decTObjectOrderedSet<gdeCategory>(list){
}

gdeCategoryList::~gdeCategoryList(){
}


// Management
///////////////

gdeCategory *gdeCategoryList::GetNamed(const char *name) const{
	return FindOrDefault([&](const gdeCategory &c){
		return c.GetName() == name;
	});
}

gdeCategory *gdeCategoryList::GetWithPath(const char *path) const{
	const decStringList components(decString(path).Split('/'));
	if(components.IsEmpty()){
		return nullptr;
	}
	
	gdeCategory * const root = GetNamed(components.First());
	if(components.GetCount() == 1){
		return root;
	}
	
	return components.Inject(root, [&](gdeCategory *cat, const decString &comp){
		return cat ? cat->GetCategories().GetNamed(comp) : nullptr;
	}, 1);
}

void gdeCategoryList::Add(gdeCategory *category){
	DEASSERT_NOTNULL(category)
	DEASSERT_NULL(GetNamed(category->GetName()))
	
	decTObjectOrderedSet<gdeCategory>::Add(category);
}


gdeCategoryList &gdeCategoryList::operator=(const gdeCategoryList &list){
	decTObjectOrderedSet<gdeCategory>::operator=(list);
	return *this;
}
