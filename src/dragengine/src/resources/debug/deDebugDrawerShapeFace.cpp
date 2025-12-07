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

#include "deDebugDrawerShapeFace.h"
#include "../../common/exceptions.h"



// Class deDebugDrawerShapeFace
/////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deDebugDrawerShapeFace::deDebugDrawerShapeFace() :
pVertices(NULL),
pVertexCount(0),
pNormal(0.0f, 0.0f, 1.0f){
}

deDebugDrawerShapeFace::~deDebugDrawerShapeFace(){
	if(pVertices){
		delete [] pVertices;
	}
}



// Management
///////////////

void deDebugDrawerShapeFace::SetNormal(const decVector &normal){
	pNormal = normal;
	pNormal.Normalize();
}

void deDebugDrawerShapeFace::CalculateNormal(){
	if(pVertexCount < 3){
		return;
	}
	pNormal = ((pVertices[1] - pVertices[0]) % (pVertices[2] - pVertices[1])).Normalized();
}

const decVector &deDebugDrawerShapeFace::GetVertexAt(int index) const{
	if(index < 0 || index >= pVertexCount){
		DETHROW(deeInvalidParam);
	}
	return pVertices[index];
}

void deDebugDrawerShapeFace::SetVertexAt(int index, const decVector &vertex){
	if(index < 0 || index >= pVertexCount){
		DETHROW(deeInvalidParam);
	}
	pVertices[index] = vertex;
}

void deDebugDrawerShapeFace::AddVertex(const decVector &vertex){
	decVector * const newArray = new decVector[pVertexCount + 1];
	
	if(pVertices){
		memcpy(newArray, pVertices, sizeof(decVector) * pVertexCount);
		delete [] pVertices;
	}
	
	pVertices = newArray;
	pVertices[pVertexCount] = vertex;
	pVertexCount++;
}

void deDebugDrawerShapeFace::RemoveAllVertices(){
	if(pVertices){
		delete [] pVertices;
		pVertices = NULL;
	}
	pVertexCount = 0;
}


	
// Helper Functions
/////////////////////

void deDebugDrawerShapeFace::MakeCoplanarTo(const decVector &position){
	int i;
	for(i=0; i<pVertexCount; i++){
		pVertices[i] += pNormal * (pNormal * (position - pVertices[i]));
	}
}
