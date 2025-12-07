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

#include <stdlib.h>
#include <string.h>
#include "dePatternList.h"
#include "../common/exceptions.h"



// Class dePatternList
////////////////////////

// Constructor, destructor
////////////////////////////

dePatternList::dePatternList(){
}

dePatternList::~dePatternList(){
}




// Management
///////////////

int dePatternList::GetPatternCount() const {
	return pPatterns.GetCount();
}

const decString &dePatternList::GetPatternAt(int index) const{
	return pPatterns.GetAt(index);
}

int dePatternList::IndexOfPattern(const char *pattern) const{
	return pPatterns.IndexOf(pattern);
}

bool dePatternList::HasPattern(const char *pattern) const{
	return pPatterns.Has(pattern);
}

void dePatternList::AddPattern(const char *pattern){
	if(pPatterns.Has(pattern)){
		DETHROW(deeInvalidParam);
	}
	pPatterns.Add(pattern);
}

void dePatternList::RemovePattern(const char *pattern){
	const int index = pPatterns.IndexOf(pattern);
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	pPatterns.RemoveFrom(index);
}

void dePatternList::RemoveAllPatterns(){
	pPatterns.RemoveAll();
}



void dePatternList::SortPatternByLength(){
	const int count = pPatterns.GetCount();
	int p = 1;
	
	while(p < count){
		const int length1 = pPatterns[p - 1].GetLength();
		const int length2 = pPatterns[p].GetLength();
		
		if(length2 > length1){
			pPatterns.Move(p - 1, p);
			
			if(p > 1){
				p--;
				continue;
			}
		}
		
		p++;
	}
}

bool dePatternList::PathMatches(const char *path) const{
	if(! path){
		DETHROW(deeInvalidParam);
	}

	const int pathLength = (int)strlen(path);
	const int count = pPatterns.GetCount();
	int p;
	
	for(p=0; p<count; p++){
		const int patternLength = pPatterns[p].GetLength();
		
		if(patternLength < pathLength){
			if(strcmp(path + (pathLength - patternLength), pPatterns[p].GetString()) == 0){
				return true;
			}
		}
	}
	
	return false;
}

int dePatternList::IndexOfPatternMatchingPath(const char *path) const{
	if(! path){
		DETHROW(deeInvalidParam);
	}

	const int pathLength = (int)strlen(path);
	const int count = pPatterns.GetCount();
	int p;
	
	for(p=0; p<count; p++){
		const int patternLength = pPatterns[p].GetLength();
		
		if(patternLength < pathLength){
			if(strcmp(path + (pathLength - patternLength), pPatterns[p]) == 0){
				return p;
			}
		}
	}
	
	return -1;
}
