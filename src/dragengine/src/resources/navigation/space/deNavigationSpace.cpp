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

#include "deNavigationSpace.h"
#include "deNavigationSpaceManager.h"
#include "deNavigationSpaceCorner.h"
#include "deNavigationSpaceEdge.h"
#include "deNavigationSpaceFace.h"
#include "deNavigationSpaceWall.h"
#include "deNavigationSpaceRoom.h"
#include "../../../systems/modules/ai/deBaseAINavigationSpace.h"
#include "../../../common/exceptions.h"



// Class deNavigationSpace
////////////////////////////

// Constructor, destructor
////////////////////////////

deNavigationSpace::deNavigationSpace(deNavigationSpaceManager *manager) :
deResource(manager),

pType(estGrid),
pLayer(0),

pVertices(NULL),
pEdges(NULL),
pCorners(NULL),
pFaces(NULL),
pWalls(NULL),
pRooms(NULL),
pVertexCount(0),
pEdgeCount(0),
pCornerCount(0),
pFaceCount(0),
pWallCount(0),
pRoomCount(0),

pSnapDistance(0.001f),
pSnapAngle(180.0f * DEG2RAD),

pBlockingPriority(0),

pPeerAI(NULL),

pParentWorld(NULL),
pLLWorldPrev(NULL),
pLLWorldNext(NULL){
}

deNavigationSpace::~deNavigationSpace(){
	if(pPeerAI){
		delete pPeerAI;
		pPeerAI = NULL;
	}
	
	if(pRooms){
		delete [] pRooms;
	}
	if(pWalls){
		delete [] pWalls;
	}
	if(pFaces){
		delete [] pFaces;
	}
	if(pCorners){
		delete [] pCorners;
	}
	if(pEdges){
		delete [] pEdges;
	}
	if(pVertices){
		delete [] pVertices;
	}
}



// Management
///////////////

void deNavigationSpace::SetType(eSpaceTypes type){
	if(type < estGrid || type > estVolume){
		DETHROW(deeInvalidParam);
	}
	
	if(type != pType){
		pType = type;
		if(pPeerAI){
			pPeerAI->TypeChanged();
		}
	}
}

void deNavigationSpace::SetLayer(int layer){
	if(layer != pLayer){
		pLayer = layer;
		
		if(pPeerAI){
			pPeerAI->LayerChanged();
		}
	}
}

void deNavigationSpace::SetPosition(const decDVector &position){
	if(!position.IsEqualTo(pPosition)){
		pPosition = position;
		
		if(pPeerAI){
			pPeerAI->PositionChanged();
		}
	}
}

void deNavigationSpace::SetOrientation(const decQuaternion &orientation){
	if(!orientation.IsEqualTo(pOrientation)){
		pOrientation = orientation;
		
		if(pPeerAI){
			pPeerAI->OrientationChanged();
		}
	}
}

void deNavigationSpace::SetSnapDistance(float distance) {
	if(fabsf(distance - pSnapDistance) > FLOAT_SAFE_EPSILON){
		pSnapDistance = distance;
		
		if(pPeerAI){
			pPeerAI->SnappingChanged();
		}
	}
}

void deNavigationSpace::SetSnapAngle(float angle) {
	if(fabsf(angle - pSnapAngle) > FLOAT_SAFE_EPSILON){
		pSnapAngle = angle;
		
		if(pPeerAI){
			pPeerAI->SnappingChanged();
		}
	}
}

void deNavigationSpace::NotifyBlockerShapeListChanged(){
	if(pPeerAI){
		pPeerAI->BlockerShapeChanged();
	}
}

void deNavigationSpace::SetBlockingPriority(int priority){
	if(priority == pBlockingPriority){
		return;
	}
	
	pBlockingPriority = priority;
	
	if(pPeerAI){
		pPeerAI->BlockingPriorityChanged();
	}
}

void deNavigationSpace::NotifyLayoutChanged(){
	if(pPeerAI){
		pPeerAI->LayoutChanged();
	}
}

bool deNavigationSpace::Verify() const{
	return true;
}



// Vertices
/////////////

void deNavigationSpace::SetVertexCount(int count){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count != pVertexCount){
		if(pVertices){
			delete [] pVertices;
		}
		pVertices = NULL;
		pVertexCount = 0;
		
		if(count > 0){
			pVertices = new decVector[count];
			if(!pVertices){
				DETHROW(deeOutOfMemory);
			}
			pVertexCount = count;
		}
	}
}

const decVector &deNavigationSpace::GetVertexAt(int index) const{
	if(index < 0 || index >= pVertexCount){
		DETHROW(deeInvalidParam);
	}
	
	return pVertices[index];
}

void deNavigationSpace::SetVertexAt(int index, const decVector &vertex){
	if(index < 0 || index >= pVertexCount){
		DETHROW(deeInvalidParam);
	}
	
	pVertices[index] = vertex;
}



// Edges
//////////

void deNavigationSpace::SetEdgeCount(int count){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count != pEdgeCount){
		if(pEdges){
			delete [] pEdges;
		}
		pEdges = NULL;
		pEdgeCount = 0;
		
		if(count > 0){
			pEdges = new deNavigationSpaceEdge[count];
			if(!pEdges){
				DETHROW(deeOutOfMemory);
			}
			pEdgeCount = count;
		}
	}
}

deNavigationSpaceEdge &deNavigationSpace::GetEdgeAt(int index) const{
	if(index < 0 || index >= pEdgeCount){
		DETHROW(deeInvalidParam);
	}
	
	return pEdges[index];
}



// Corners
////////////

void deNavigationSpace::SetCornerCount(int count){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count != pCornerCount){
		if(pCorners){
			delete [] pCorners;
		}
		pCorners = NULL;
		pCornerCount = 0;
		
		if(count > 0){
			pCorners = new deNavigationSpaceCorner[count];
			if(!pCorners){
				DETHROW(deeOutOfMemory);
			}
			pCornerCount = count;
		}
	}
}

deNavigationSpaceCorner &deNavigationSpace::GetCornerAt(int index) const{
	if(index < 0 || index >= pCornerCount){
		DETHROW(deeInvalidParam);
	}
	
	return pCorners[index];
}



// Faces
//////////

void deNavigationSpace::SetFaceCount(int count){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count != pFaceCount){
		if(pFaces){
			delete [] pFaces;
		}
		pFaces = NULL;
		pFaceCount = 0;
		
		if(count > 0){
			pFaces = new deNavigationSpaceFace[count];
			if(!pFaces){
				DETHROW(deeOutOfMemory);
			}
			pFaceCount = count;
		}
	}
}

deNavigationSpaceFace &deNavigationSpace::GetFaceAt(int index) const{
	if(index < 0 || index >= pFaceCount){
		DETHROW(deeInvalidParam);
	}
	
	return pFaces[index];
}



// Walls
//////////

void deNavigationSpace::SetWallCount(int count){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count != pWallCount){
		if(pWalls){
			delete [] pWalls;
		}
		pWalls = NULL;
		pWallCount = 0;
		
		if(count > 0){
			pWalls = new deNavigationSpaceWall[count];
			if(!pWalls){
				DETHROW(deeOutOfMemory);
			}
			pWallCount = count;
		}
	}
}

deNavigationSpaceWall &deNavigationSpace::GetWallAt(int index) const{
	if(index < 0 || index >= pWallCount){
		DETHROW(deeInvalidParam);
	}
	
	return pWalls[index];
}



// Rooms
//////////

void deNavigationSpace::SetRoomCount(int count){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count != pRoomCount){
		if(pRooms){
			delete [] pRooms;
		}
		pRooms = NULL;
		pRoomCount = 0;
		
		if(count > 0){
			pRooms = new deNavigationSpaceRoom[count];
			if(!pRooms){
				DETHROW(deeOutOfMemory);
			}
			pRoomCount = count;
		}
	}
}

deNavigationSpaceRoom &deNavigationSpace::GetRoomAt(int index) const{
	if(index < 0 || index >= pRoomCount){
		DETHROW(deeInvalidParam);
	}
	
	return pRooms[index];
}



// System Peers
/////////////////

void deNavigationSpace::SetPeerAI(deBaseAINavigationSpace *peer){
	if(pPeerAI){
		delete pPeerAI;
	}
	pPeerAI = peer;
}



// Linked List
////////////////

void deNavigationSpace::SetParentWorld(deWorld *world){
	pParentWorld = world;
}

void deNavigationSpace::SetLLWorldPrev(deNavigationSpace *navspace){
	pLLWorldPrev = navspace;
}

void deNavigationSpace::SetLLWorldNext(deNavigationSpace *navspace){
	pLLWorldNext = navspace;
}
