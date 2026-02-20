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

#include "delGameIconList.h"

#include <dragengine/common/exceptions.h>


// Class delGameIconList
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

delGameIconList::delGameIconList(const delGameIconList &list) :
decTObjectOrderedSet<delGameIcon>(list){
}


// Management
///////////////

delGameIcon *delGameIconList::GetWithSize(int size) const{
	return FindOrDefault([&](const delGameIcon &icon){
		return icon.GetSize() == size;
	});
}

delGameIcon *delGameIconList::GetLargest(int size) const{
	delGameIcon *bestIcon = nullptr;
	
	Visit([&](delGameIcon *icon){
		if(icon->GetSize() > size){
			return;
		}
		
		if(!bestIcon || icon->GetSize() > bestIcon->GetSize()){
			bestIcon = icon;
		}
	});
	
	return bestIcon;
}

delGameIcon *delGameIconList::GetSmallest(int size) const{
	delGameIcon *bestIcon = nullptr;
	
	Visit([&](delGameIcon *icon){
		if(icon->GetSize() < size){
			return;
		}
		
		if(!bestIcon || icon->GetSize() < bestIcon->GetSize()){
			bestIcon = icon;
		}
	});
	
	return bestIcon;
}

delGameIcon *delGameIconList::GetLargest() const{
	delGameIcon *bestIcon = nullptr;
	
	Visit([&](delGameIcon *icon){
		if(!bestIcon || icon->GetSize() > bestIcon->GetSize()){
			bestIcon = icon;
		}
	});
	
	return bestIcon;
}

delGameIcon *delGameIconList::GetSmallest() const{
	delGameIcon *bestIcon = nullptr;
	
	Visit([&](delGameIcon *icon){
		if(!bestIcon || icon->GetSize() < bestIcon->GetSize()){
			bestIcon = icon;
		}
	});
	
	return bestIcon;
}

bool delGameIconList::HasWithSize(int size) const{
	return HasMatching([&](const delGameIcon &icon){
		return icon.GetSize() == size;
	});
}

int delGameIconList::IndexOfWithSize(int size) const{
	return IndexOfMatching([&](const delGameIcon &icon){
		return icon.GetSize() == size;
	});
}
