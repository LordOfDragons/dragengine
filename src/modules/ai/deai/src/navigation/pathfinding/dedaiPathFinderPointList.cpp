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
#include <string.h>

#include "dedaiPathFinderPointList.h"

#include <dragengine/common/exceptions.h>



// Class dedaiPathFinderPointList
///////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiPathFinderPointList::dedaiPathFinderPointList(){
	pPoints = NULL;
	pPointCount = 0;
	pPointSize = 0;
}

dedaiPathFinderPointList::~dedaiPathFinderPointList(){
	if(pPoints){
		delete [] pPoints;
	}
}



// Management
///////////////

const decVector &dedaiPathFinderPointList::GetFirst() const{
	if(pPointCount == 0){
		DETHROW(deeInvalidParam);
	}
	return pPoints[0];
}

const decVector &dedaiPathFinderPointList::GetAt(int index) const{
	if(index < 0 || index >= pPointCount){
		DETHROW(deeInvalidParam);
	}
	return pPoints[index];
}

void dedaiPathFinderPointList::Add(const decVector &point){
	if(pPointCount == pPointSize){
		const int newSize = pPointCount + 10;
		decVector *newArray = new decVector[newSize];
		if(pPoints){
			memcpy(newArray, pPoints, sizeof(decVector) * pPointCount);
			delete [] pPoints;
		}
		pPoints = newArray;
		pPointSize = newSize;
	}
	
	pPoints[pPointCount++] = point;
}

void dedaiPathFinderPointList::RemoveFirst(){
	if(pPointCount == 0){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	
	for(i=1; i<pPointCount; i++){
		pPoints[i - 1] = pPoints[i];
	}
	pPointCount--;
}

void dedaiPathFinderPointList::RemoveFirst(int count){
	if(count > 0){
		if(count > pPointCount){
			DETHROW(deeInvalidParam);
		}
		
		int i;
		
		for(i=count; i<pPointCount; i++){
			pPoints[i - count] = pPoints[i];
		}
		pPointCount -= count;
	}
}

void dedaiPathFinderPointList::RemoveFrom(int index){
	if(index < 0 || index >= pPointCount){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	
	for(i=index+1; i<pPointCount; i++){
		pPoints[i - 1] = pPoints[i];
	}
	pPointCount--;
}

void dedaiPathFinderPointList::RemoveAll(){
	pPointCount = 0;
}



void dedaiPathFinderPointList::Transform(const decMatrix &matrix){
	int i;
	
	for(i=0; i<pPointCount; i++){
		pPoints[i] = matrix * pPoints[i];
	}
}
