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

#include "meDecal.h"
#include "meDecalList.h"

#include <dragengine/common/exceptions.h>



// Class meDecalList
//////////////////////

// Constructor, destructor
////////////////////////////

meDecalList::meDecalList(){
}

meDecalList::~meDecalList(){
}



// Management
///////////////

int meDecalList::GetCount() const{
	return pObjects.GetCount();
}

meDecal *meDecalList::GetAt(int position) const{
	return (meDecal*)pObjects.GetAt(position);
}

int meDecalList::IndexOf(meDecal *decal) const{
	return pObjects.IndexOf(decal);
}

bool meDecalList::Has(meDecal *decal) const{
	return pObjects.Has(decal);
}

void meDecalList::Add(meDecal *decal){
	if(! decal){
		DETHROW(deeInvalidParam);
	}
	
	pObjects.Add(decal);
}

void meDecalList::AddIfAbsent(meDecal *decal){
	if(! decal){
		DETHROW(deeInvalidParam);
	}
	
	pObjects.AddIfAbsent(decal);
}

void meDecalList::Remove(meDecal *decal){
	if(! decal){
		DETHROW(deeInvalidParam);
	}
	
	pObjects.Remove(decal);
}

void meDecalList::RemoveIfPresent(meDecal *decal){
	if(! decal){
		DETHROW(deeInvalidParam);
	}
	
	pObjects.RemoveIfPresent(decal);
}

void meDecalList::RemoveAll(){
	pObjects.RemoveAll();
}



meDecalList &meDecalList::operator=(const meDecalList &list){
	pObjects = list.pObjects;
	return *this;
}
