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

pSnapDistance(0.001f),
pSnapAngle(180.0f * DEG2RAD),

pBlockingPriority(0),

pPeerAI(nullptr),

pParentWorld(nullptr),
pLLWorld(this){
}

deNavigationSpace::~deNavigationSpace(){
	if(pPeerAI){
		delete pPeerAI;
		pPeerAI = nullptr;
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
