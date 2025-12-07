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
#include <stdlib.h>

#include "deoglShape.h"
#include "deoglShapeManager.h"

#include <dragengine/common/exceptions.h>



// Class deoglShapeManager
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShapeManager::deoglShapeManager(){
	pShapes = NULL;
	pShapeCount = 0;
	pShapeSize = 0;
}

deoglShapeManager::~deoglShapeManager(){
	pCleanUp();
}



// Management
///////////////

deoglShape *deoglShapeManager::GetShapeAt(int index) const{
	if(index < 0 || index >= pShapeCount){
		DETHROW(deeInvalidParam);
	}
	
	return pShapes[index];
}

void deoglShapeManager::AddShape(deoglShape *shape){
	if(!shape){
		DETHROW(deeInvalidParam);
	}
	
	if(pShapeCount == pShapeSize){
		int i, newSize = pShapeSize * 3 / 2 + 1;
		deoglShape **newArray = new deoglShape*[newSize];
		if(pShapes){
			for(i=0; i<pShapeSize; i++) newArray[i] = pShapes[i];
			delete [] pShapes;
		}
		pShapes = newArray;
		pShapeSize = newSize;
	}
	pShapes[pShapeCount] = shape;
	pShapeCount++;
}

void deoglShapeManager::RemoveAllShapes(){
	while(pShapeCount > 0){
		delete pShapes[pShapeCount - 1];
		pShapeCount--;
	}
}



// Private Functions
//////////////////////

void deoglShapeManager::pCleanUp(){
	RemoveAllShapes();
	if(pShapes){
		delete [] pShapes;
	}
}
