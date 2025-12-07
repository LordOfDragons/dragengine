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
#include "decBSPTreeFace.h"
#include "../exceptions.h"



// Class decBSPTreeFace
/////////////////////////

// Constructor, destructor
////////////////////////////

decBSPTreeFace::decBSPTreeFace(){
	pVertices = NULL;
	pVertexCount = 0;
	pVertexSize = 0;
	pUserData = NULL;
	
	pVertices = new decVector[3];
	if(! pVertices) DETHROW(deeOutOfMemory);
	pVertexSize = 3;
}

decBSPTreeFace::~decBSPTreeFace(){
	if(pVertices) delete [] pVertices;
}



// Management
///////////////

const decVector &decBSPTreeFace::GetVertexAt(int index) const{
	if(index < 0 || index >= pVertexCount) DETHROW(deeInvalidParam);
	
	return pVertices[index];
}

void decBSPTreeFace::AddVertex(const decVector &vertex){
	if(pVertexCount == pVertexSize){
		int newSize = pVertexSize * 3 / 2 + 1;
		decVector *newArray = new decVector[newSize];
		if(pVertices){
			memcpy(newArray, pVertices, sizeof(decVector) * pVertexSize);
			delete [] pVertices;
		}
		pVertices = newArray;
		pVertexSize = newSize;
	}
	
	pVertices[pVertexCount] = vertex;
	pVertexCount++;
}

void decBSPTreeFace::RemoveAllVertices(){
	pVertexCount = 0;
}

void decBSPTreeFace::SetUserData(void *userData){
	pUserData = userData;
}
