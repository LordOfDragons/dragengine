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

#include "mePathFindTestType.h"
#include "mePathFindTestTypeList.h"

#include <dragengine/common/exceptions.h>



// Class mePathFindTestTypeList
/////////////////////////////////

// Constructor, destructor
////////////////////////////

mePathFindTestTypeList::mePathFindTestTypeList(){
}

mePathFindTestTypeList::~mePathFindTestTypeList(){
}



// Management
///////////////

int mePathFindTestTypeList::GetCount() const{
	return pTypes.GetCount();
}

mePathFindTestType *mePathFindTestTypeList::GetAt(int position) const{
	return (mePathFindTestType*)pTypes.GetAt(position);
}

mePathFindTestType *mePathFindTestTypeList::GetWith(int typeNumber) const{
	const int count = pTypes.GetCount();
	mePathFindTestType *type;
	int i;
	
	for(i=0; i<count; i++){
		type = (mePathFindTestType*)pTypes.GetAt(i);
		
		if(type->GetTypeNumber() == typeNumber){
			return type;
		}
	}
	
	return NULL;
}

int mePathFindTestTypeList::IndexOf(mePathFindTestType *type) const{
	return pTypes.IndexOf(type);
}

int mePathFindTestTypeList::IndexWith(int typeNumber) const{
	const int count = pTypes.GetCount();
	mePathFindTestType *type;
	int i;
	
	for(i=0; i<count; i++){
		type = (mePathFindTestType*)pTypes.GetAt(i);
		
		if(type->GetTypeNumber() == typeNumber){
			return i;
		}
	}
	
	return -1;
}

bool mePathFindTestTypeList::Has(mePathFindTestType *type) const{
	return pTypes.Has(type);
}

bool mePathFindTestTypeList::HasWith(int typeNumber) const{
	const int count = pTypes.GetCount();
	mePathFindTestType *type;
	int i;
	
	for(i=0; i<count; i++){
		type = (mePathFindTestType*)pTypes.GetAt(i);
		
		if(type->GetTypeNumber() == typeNumber){
			return true;
		}
	}
	
	return false;
}

void mePathFindTestTypeList::Add(mePathFindTestType *type){
	if(!type || HasWith(type->GetTypeNumber())){
		DETHROW(deeInvalidParam);
	}
	
	pTypes.Add(type);
}

void mePathFindTestTypeList::AddIfAbsent(mePathFindTestType *type){
	if(!type || HasWith(type->GetTypeNumber())){
		DETHROW(deeInvalidParam);
	}
	
	pTypes.AddIfAbsent(type);
}

void mePathFindTestTypeList::Remove(mePathFindTestType *type){
	pTypes.Remove(type);
}

void mePathFindTestTypeList::RemoveIfPresent(mePathFindTestType *type){
	pTypes.RemoveIfPresent(type);
}

void mePathFindTestTypeList::RemoveAll(){
	pTypes.RemoveAll();
}
