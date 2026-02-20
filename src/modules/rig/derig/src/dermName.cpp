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

#include "dermName.h"

#include <dragengine/common/exceptions.h>


// Class dermName::List
/////////////////////////

bool dermName::List::HasName(const char *name){
	return HasMatching([&](const dermName &n){
		return n.GetName() == name;
	});
}

int dermName::List::IndexOfName(const char *name) const{
	return IndexOfMatching([&](const dermName &n){
		return n.GetName() == name;
	});
}

dermName &dermName::List::GetName(const char *name){
	dermName *found;
	DEASSERT_TRUE(Find(found, [&](const dermName &n){
		return n.GetName() == name;
	}));
	return *found;
}

const dermName &dermName::List::GetName(const char *name) const{
	return const_cast<dermName::List*>(this)->GetName(name);
}

int dermName::List::AddName(const char *name){
	const int index = IndexOfName(name);
	if(index != -1){
		return index;
	}
	
	Add({name, -1});
	return GetCount() - 1;
}


// Class dermName
///////////////////

// Constructor, destructor
////////////////////////////

dermName::dermName() :
pNumber(-1){
}

dermName::dermName(const char *name, int number) :
pName(name),
pNumber(number){
}

dermName::dermName(const dermName &name) :
pName(name.pName),
pNumber(name.pNumber){
}

dermName dermName::operator=(const dermName &name){
	pName = name.pName;
	pNumber = name.pNumber;
	return *this;
}

dermName::dermName(dermName &&name) noexcept :
pName(std::move(name.pName)),
pNumber(name.pNumber){
}

dermName dermName::operator=(dermName &&name) noexcept{
	pName = std::move(name.pName);
	pNumber = name.pNumber;
	return *this;
}

// Management
//////////////////

void dermName::SetNumber(int number){
	pNumber = number;
}
