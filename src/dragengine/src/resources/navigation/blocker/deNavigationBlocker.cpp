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

#include "deNavigationBlocker.h"
#include "deNavigationBlockerManager.h"
#include "../space/deNavigationSpace.h"
#include "../../../systems/modules/ai/deBaseAINavigationBlocker.h"
#include "../../../common/exceptions.h"



// Class deNavigationBlocker
//////////////////////////////

// Constructor, destructor
////////////////////////////

deNavigationBlocker::deNavigationBlocker(deNavigationBlockerManager *manager) :
deResource(manager),
pBlockingPriority(0)
{
	pScaling.Set(1.0f, 1.0f, 1.0f);
	
	pLayer = 0;
	pSpaceType = deNavigationSpace::estGrid;
	pEnabled = true;
	
	pPeerAI = NULL;
	
	pParentWorld = NULL;
	pLLWorldPrev = NULL;
	pLLWorldNext = NULL;
}

deNavigationBlocker::~deNavigationBlocker(){
	if(pPeerAI){
		delete pPeerAI;
		pPeerAI = NULL;
	}
}



// Management
///////////////

void deNavigationBlocker::SetPosition(const decDVector &position){
	if(! position.IsEqualTo(pPosition)){
		pPosition = position;
		if(pPeerAI){
			pPeerAI->PositionChanged();
		}
	}
}

void deNavigationBlocker::SetOrientation(const decQuaternion &orientation){
	if(! orientation.IsEqualTo(pOrientation)){
		pOrientation = orientation;
		if(pPeerAI){
			pPeerAI->OrientationChanged();
		}
	}
}

void deNavigationBlocker::SetScaling(const decVector &scaling){
	if(! scaling.IsEqualTo(pScaling)){
		pScaling = scaling;
		if(pPeerAI){
			pPeerAI->ScalingChanged();
		}
	}
}

void deNavigationBlocker::SetLayer(int layer){
	if(layer != pLayer){
		pLayer = layer;
		if(pPeerAI){
			pPeerAI->LayerChanged();
		}
	}
}

void deNavigationBlocker::SetSpaceType(deNavigationSpace::eSpaceTypes spaceType){
	if(spaceType < deNavigationSpace::estGrid || spaceType > deNavigationSpace::estVolume){
		DETHROW(deeInvalidParam);
	}
	
	if(spaceType != pSpaceType){
		pSpaceType = spaceType;
		if(pPeerAI){
			pPeerAI->SpaceTypeChanged();
		}
	}
}

void deNavigationBlocker::SetBlockingPriority(int priority){
	if(priority == pBlockingPriority){
		return;
	}
	
	pBlockingPriority = priority;
	
	if(pPeerAI){
		pPeerAI->BlockingPriorityChanged();
	}
}



void deNavigationBlocker::SetEnabled(bool enabled){
	if(enabled == pEnabled){
		return;
	}
	
	pEnabled = enabled;
	
	if(pPeerAI){
		pPeerAI->EnabledChanged();
	}
}



void deNavigationBlocker::NotifyShapeListChanged(){
	if(pPeerAI){
		pPeerAI->ShapeChanged();
	}
}



// System Peers
/////////////////

void deNavigationBlocker::SetPeerAI(deBaseAINavigationBlocker *peer){
	if(pPeerAI){
		delete pPeerAI;
	}
	pPeerAI = peer;
}



// Linked List
////////////////

void deNavigationBlocker::SetParentWorld(deWorld *world){
	pParentWorld = world;
}

void deNavigationBlocker::SetLLWorldPrev(deNavigationBlocker *blocker){
	pLLWorldPrev = blocker;
}

void deNavigationBlocker::SetLLWorldNext(deNavigationBlocker *blocker){
	pLLWorldNext = blocker;
}
