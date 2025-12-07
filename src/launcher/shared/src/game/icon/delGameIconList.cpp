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

#include "delGameIcon.h"
#include "delGameIconList.h"

#include <dragengine/common/exceptions.h>


// Class delGameIconList
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

delGameIconList::delGameIconList(){
}

delGameIconList::~delGameIconList(){
	RemoveAll();
}



// Management
///////////////

int delGameIconList::GetCount() const{
	return pIcons.GetCount();
}

delGameIcon *delGameIconList::GetAt(int index) const{
	return (delGameIcon*)pIcons.GetAt(index);
}

delGameIcon *delGameIconList::GetWithSize(int size) const{
	const int count = pIcons.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		delGameIcon * const icon = (delGameIcon*)pIcons.GetAt(i);
		if(icon->GetSize() == size){
			return icon;
		}
	}
	
	return nullptr;
}

delGameIcon *delGameIconList::GetLargest(int size) const{
	const int count = pIcons.GetCount();
	delGameIcon *bestIcon = nullptr;
	int i;
	
	for(i=0; i<count; i++){
		delGameIcon * const icon = (delGameIcon*)pIcons.GetAt(i);
		if(icon->GetSize() > size){
			continue;
		}
		
		if(!bestIcon || icon->GetSize() > bestIcon->GetSize()){
			bestIcon = icon;
		}
	}
	
	return bestIcon;
}

delGameIcon *delGameIconList::GetSmallest(int size) const{
	const int count = pIcons.GetCount();
	delGameIcon *bestIcon = nullptr;
	int i;
	
	for(i=0; i<count; i++){
		delGameIcon * const icon = (delGameIcon*)pIcons.GetAt(i);
		if(icon->GetSize() < size){
			continue;
		}
		
		if(!bestIcon || icon->GetSize() < bestIcon->GetSize()){
			bestIcon = icon;
		}
	}
	
	return bestIcon;
}

delGameIcon *delGameIconList::GetLargest() const{
	const int count = pIcons.GetCount();
	delGameIcon *bestIcon = nullptr;
	int i;
	
	for(i=0; i<count; i++){
		delGameIcon * const icon = (delGameIcon*)pIcons.GetAt(i);
		if(!bestIcon || icon->GetSize() > bestIcon->GetSize()){
			bestIcon = icon;
		}
	}
	
	return bestIcon;
}

delGameIcon *delGameIconList::GetSmallest() const{
	const int count = pIcons.GetCount();
	delGameIcon *bestIcon = nullptr;
	int i;
	
	for(i=0; i<count; i++){
		delGameIcon * const icon = (delGameIcon*)pIcons.GetAt(i);
		if(!bestIcon || icon->GetSize() < bestIcon->GetSize()){
			bestIcon = icon;
		}
	}
	
	return bestIcon;
}

bool delGameIconList::Has(delGameIcon *icon) const{
	return pIcons.Has(icon);
}

bool delGameIconList::HasWithSize(int size) const{
	const int count = pIcons.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const delGameIcon * const icon = (delGameIcon*)pIcons.GetAt(i);
		if(icon->GetSize() == size){
			return true;
		}
	}
	
	return false;
}

int delGameIconList::IndexOf(delGameIcon *icon) const{
	return pIcons.IndexOf(icon);
}

int delGameIconList::IndexOfWithSize(int size) const{
	const int count = pIcons.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const delGameIcon * const icon = (delGameIcon*)pIcons.GetAt(i);
		if(icon->GetSize() == size){
			return i;
		}
	}
	
	return -1;
}

void delGameIconList::Add(delGameIcon *icon){
	if(!icon){
		DETHROW_INFO(deeNullPointer, "icon");
	}
	if(HasWithSize(icon->GetSize())){
		DETHROW_INFO(deeInvalidParam, "icon with size is present");
	}
	
	pIcons.Add(icon);
}

void delGameIconList::Remove(delGameIcon *icon){
	const int index = IndexOf(icon);
	if(index == -1){
		DETHROW_INFO(deeInvalidParam, "icon is absent");
	}
	
	pIcons.RemoveFrom(index);
}

void delGameIconList::RemoveAll(){
	pIcons.RemoveAll();
}
