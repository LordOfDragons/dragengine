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
#include <stdio.h>
#include "reRigShape.h"
#include "reRigShapeList.h"
#include "dragengine/common/exceptions.h"



// Class reRigShapeList
/////////////////////////

// Constructor, destructor
////////////////////////////

reRigShapeList::reRigShapeList(){
	pShapes = NULL;
	pShapeCount = 0;
	pShapeSize = 0;
}

reRigShapeList::~reRigShapeList(){
	RemoveAllShapes();
	if(pShapes) delete [] pShapes;
}



// Management
///////////////

reRigShape *reRigShapeList::GetShapeAt(int index) const{
	if(index < 0 || index >= pShapeCount) DETHROW(deeInvalidParam);
	
	return pShapes[index];
}

int reRigShapeList::IndexOfShape(reRigShape::Ref shape) const{
	if(!shape) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pShapeCount; i++){
		if(shape == pShapes[i]) return i;
	}
	
	return -1;
}

bool reRigShapeList::HasShape(reRigShape::Ref shape) const{
	if(!shape) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pShapeCount; i++){
		if(shape == pShapes[i]) return true;
	}
	
	return false;
}

void reRigShapeList::AddShape(reRigShape::Ref shape){
	if(HasShape(shape)) DETHROW(deeInvalidParam);
	
	if(pShapeCount == pShapeSize){
		int newSize = pShapeSize * 3 / 2 + 1;
		reRigShape **newArray = new reRigShape*[newSize];
		if(pShapes){
			memcpy(newArray, pShapes, sizeof(reRigShape*) * pShapeSize);
			delete [] pShapes;
		}
		pShapes = newArray;
		pShapeSize = newSize;
	}
	
	pShapes[pShapeCount] = shape;
	pShapeCount++;
}

void reRigShapeList::RemoveShape(reRigShape::Ref shape){
	int i, index = IndexOfShape(shape);
	if(index == -1) DETHROW(deeInvalidParam);
	
	for(i=index+1; i<pShapeCount; i++){
		pShapes[i - 1] = pShapes[i];
	}
	pShapeCount--;
}

void reRigShapeList::RemoveAllShapes(){
	while(pShapeCount > 0){
		pShapeCount--;
		pShapes[pShapeCount]->FreeReference();
	}
}
