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

#include "deHeightTerrainNavSpace.h"
#include "deHeightTerrainNavSpaceEdge.h"
#include "../../navigation/space/deNavigationSpaceFace.h"
#include "../../../common/exceptions.h"



// Class deHeightTerrainNavSpace
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deHeightTerrainNavSpace::deHeightTerrainNavSpace() :
pType(deNavigationSpace::estMesh),
pLayer(0),
pSnapDistance(0.001f),
pSnapAngle(DEG2RAD * 180.0f),
pCorners(NULL),
pCornerCount(0),
pEdges(NULL),
pEdgeCount(0),
pFaces(NULL),
pFaceCount(0){
}

deHeightTerrainNavSpace::~deHeightTerrainNavSpace(){
	if(pFaces){
		delete [] pFaces;
	}
	if(pEdges){
		delete [] pEdges;
	}
	if(pCorners){
		delete [] pCorners;
	}
}



// Management
///////////////

void deHeightTerrainNavSpace::SetType(deNavigationSpace::eSpaceTypes type){
	if(type < deNavigationSpace::estGrid || type > deNavigationSpace::estVolume){
		DETHROW(deeInvalidParam);
	}
	pType = type;
}

void deHeightTerrainNavSpace::SetLayer(int layer){
	pLayer = layer;
}



void deHeightTerrainNavSpace::SetSnapDistance(float distance){
	pSnapDistance = decMath::max(distance, 0.0f);
}

void deHeightTerrainNavSpace::SetSnapAngle(float angle){
	pSnapAngle = decMath::max(angle, 0.0f);
}



// Corners
////////////

void deHeightTerrainNavSpace::SetCornerCount(int count){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count == pCornerCount){
		return;
	}
	
	if(pCorners){
		delete [] pCorners;
	}
	pCorners = NULL;
	pCornerCount = 0;
	
	if(count > 0){
		pCorners = new unsigned int[count];
		pCornerCount = count;
	}
}

unsigned int deHeightTerrainNavSpace::GetCornerAt(int index) const{
	if(index < 0 || index >= pCornerCount){
		DETHROW(deeInvalidParam);
	}
	return pCorners[index];
}

void deHeightTerrainNavSpace::SetCornerAt(int index, unsigned int corner){
	if(index < 0 || index >= pCornerCount){
		DETHROW(deeInvalidParam);
	}
	pCorners[index] = corner;
}



// Edges
//////////

void deHeightTerrainNavSpace::SetEdgeCount(int count){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count == pEdgeCount){
		return;
	}
	
	if(pEdges){
		delete [] pEdges;
	}
	pEdges = NULL;
	pEdgeCount = 0;
	
	if(count > 0){
		pEdges = new deHeightTerrainNavSpaceEdge[count];
		pEdgeCount = count;
	}
}

deHeightTerrainNavSpaceEdge &deHeightTerrainNavSpace::GetEdgeAt(int index) const{
	if(index < 0 || index >= pEdgeCount){
		DETHROW(deeInvalidParam);
	}
	return pEdges[index];
}



// Faces
//////////

void deHeightTerrainNavSpace::SetFaceCount(int count){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count == pFaceCount){
		return;
	}
	
	if(pFaces){
		delete [] pFaces;
	}
	pFaces = NULL;
	pFaceCount = 0;
	
	if(count > 0){
		pFaces = new deNavigationSpaceFace[count];
		pFaceCount = count;
	}
}

deNavigationSpaceFace &deHeightTerrainNavSpace::GetFaceAt(int index) const{
	if(index < 0 || index >= pFaceCount){
		DETHROW(deeInvalidParam);
	}
	return pFaces[index];
}
