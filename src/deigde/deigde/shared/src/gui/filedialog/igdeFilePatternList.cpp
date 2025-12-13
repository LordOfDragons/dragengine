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
#include "igdeFilePattern.h"
#include "igdeFilePatternList.h"
#include <dragengine/common/exceptions.h>



// Class igdeFilePatternList
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeFilePatternList::igdeFilePatternList(){
	pFilePatterns = nullptr;
	pFilePatternCount = 0;
	pFilePatternSize = 0;
}

igdeFilePatternList::igdeFilePatternList(const igdeFilePatternList &list){
	pFilePatterns = nullptr;
	pFilePatternCount = 0;
	pFilePatternSize = 0;
	*this = list;
}

igdeFilePatternList::~igdeFilePatternList(){
	RemoveAllFilePatterns();
	if(pFilePatterns){
		delete [] pFilePatterns;
	}
}



// Management
///////////////

igdeFilePattern *igdeFilePatternList::GetFilePatternAt(int index) const{
	if(index < 0 || index >= pFilePatternCount) DETHROW(deeOutOfBoundary);
	
	return pFilePatterns[index];
}

int igdeFilePatternList::IndexOfFilePattern(igdeFilePattern *filePattern) const{
	if(!filePattern) DETHROW(deeInvalidParam);
	int f;
	
	for(f=0; f<pFilePatternCount; f++){
		if(filePattern == pFilePatterns[f]){
			return f;
		}
	}
	
	return -1;
}

bool igdeFilePatternList::HasFilePattern(igdeFilePattern *filePattern) const{
	if(!filePattern) DETHROW(deeInvalidParam);
	int f;
	
	for(f=0; f<pFilePatternCount; f++){
		if(filePattern == pFilePatterns[f]){
			return true;
		}
	}
	
	return false;
}

void igdeFilePatternList::AddFilePattern(igdeFilePattern *filePattern){
	if(HasFilePattern(filePattern)) DETHROW(deeInvalidParam);
	
	if(pFilePatternCount == pFilePatternSize){
		int newSize = pFilePatternSize * 3 / 2 + 1;
		igdeFilePattern **newArray = new igdeFilePattern*[newSize];
		if(!newArray) DETHROW(deeOutOfMemory);
		if(pFilePatterns){
			memcpy(newArray, pFilePatterns, sizeof(igdeFilePattern*) * pFilePatternSize);
			delete [] pFilePatterns;
		}
		pFilePatterns = newArray;
		pFilePatternSize = newSize;
	}
	
	pFilePatterns[pFilePatternCount] = filePattern;
	pFilePatternCount++;
}

void igdeFilePatternList::RemoveFilePattern(igdeFilePattern *filePattern){
	int i, index = IndexOfFilePattern(filePattern);
	if(index == -1) DETHROW(deeInvalidParam);
	
	for(i=index+1; i<pFilePatternCount; i++){
		pFilePatterns[i - 1] = pFilePatterns[i];
	}
	pFilePatternCount--;
	
	delete filePattern;
}

void igdeFilePatternList::RemoveAllFilePatterns(){
	while(pFilePatternCount > 0){
		pFilePatternCount--;
		delete pFilePatterns[pFilePatternCount];
	}
}



// Operators
//////////////

igdeFilePatternList &igdeFilePatternList::operator=(const igdeFilePatternList &list){
	const int count = list.GetFilePatternCount();
	igdeFilePattern *filePattern = nullptr;
	int i;
	
	RemoveAllFilePatterns();
	
	for(i=0; i<count; i++){
		try{
			filePattern = new igdeFilePattern(*list.GetFilePatternAt(i));
			AddFilePattern(filePattern);
			filePattern = nullptr;
			
		}catch(const deException &){
			if(filePattern){
				delete filePattern;
			}
			throw;
		}
	}
	
	return *this;
}
