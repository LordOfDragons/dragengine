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
#include "deEnvMapProbe.h"
#include "deEnvMapProbeManager.h"
#include "../world/deWorld.h"
#include "../image/deImage.h"
#include "../../deEngine.h"
#include "../../systems/modules/graphic/deBaseGraphicEnvMapProbe.h"
#include "../../common/shape/decShape.h"
#include "../../common/exceptions.h"



// Class deEnvMapProbe
////////////////////////

// Constructor, destructor
////////////////////////////

deEnvMapProbe::deEnvMapProbe(deEnvMapProbeManager *manager) : deResource(manager){
	pScaling.Set(1.0f, 1.0f, 1.0f);
	pShapeReflection = NULL;
	pInfluenceBorderSize = 0.1f;
	pInfluencePriority = 0;
	
	pLayerMask.SetBit(0);
	
	pPeerGraphic = NULL;
	
	pParentWorld = NULL;
	pLLWorldPrev = NULL;
	pLLWorldNext = NULL;
}

deEnvMapProbe::~deEnvMapProbe(){
	if(pPeerGraphic){
		delete pPeerGraphic;
		pPeerGraphic = NULL;
	}
	
	if(pShapeReflection){
		delete pShapeReflection;
	}
}



// Management
///////////////

void deEnvMapProbe::SetPosition(const decDVector &position){
	if(! position.IsEqualTo(pPosition)){
		pPosition = position;
		
		if(pPeerGraphic){
			pPeerGraphic->PositionChanged();
		}
	}
}

void deEnvMapProbe::SetOrientation(const decQuaternion &orientation){
	if(! orientation.IsEqualTo(pOrientation)){
		pOrientation = orientation;
		
		if(pPeerGraphic){
			pPeerGraphic->OrientationChanged();
		}
	}
}

void deEnvMapProbe::SetScaling(const decVector &scaling){
	if(! scaling.IsEqualTo(pScaling)){
		pScaling = scaling;
		
		if(pPeerGraphic){
			pPeerGraphic->ScalingChanged();
		}
	}
}



void deEnvMapProbe::NotifyShapeListInfluenceChanged(){
	if(pPeerGraphic){
		pPeerGraphic->ShapeListInfluenceChanged();
	}
}



void deEnvMapProbe::SetShapeReflection(decShape *shape){
	if(shape != pShapeReflection){
		if(pShapeReflection){
			delete pShapeReflection;
		}
		
		pShapeReflection = shape;
		
		NotifyShapeReflectionChanged();
	}
}

void deEnvMapProbe::NotifyShapeReflectionChanged(){
	if(pPeerGraphic){
		pPeerGraphic->ShapeReflectionChanged();
	}
}



void deEnvMapProbe::SetInfluenceBorderSize(float borderSize){
	if(borderSize < 0.0f){
		borderSize = 0.0f;
	}
	
	if(fabsf(borderSize - pInfluenceBorderSize) > FLOAT_SAFE_EPSILON){
		pInfluenceBorderSize = borderSize;
		
		if(pPeerGraphic){
			pPeerGraphic->InfluenceBorderSizeChanged();
		}
	}
}

void deEnvMapProbe::SetInfluencePriority(int priority){
	if(priority < 0){
		priority = 0;
	}
	
	if(priority != pInfluencePriority){
		pInfluencePriority = priority;
		
		if(pPeerGraphic){
			pPeerGraphic->InfluencePriorityChanged();
		}
	}
}

void deEnvMapProbe::SetLayerMask(const decLayerMask &layerMask){
	if(layerMask == pLayerMask){
		return;
	}
	
	pLayerMask = layerMask;
	
	if(pPeerGraphic){
		pPeerGraphic->LayerMaskChanged();
	}
}



void deEnvMapProbe::SetImage(deImage *image){
	if(image == pImage){
		return;
	}
	
	pImage = image;
	
	if(pPeerGraphic){
		pPeerGraphic->ImageChanged();
	}
}



// System Peers
/////////////////

void deEnvMapProbe::SetPeerGraphic(deBaseGraphicEnvMapProbe *peer){
	if(peer != pPeerGraphic){
		if(pPeerGraphic){
			delete pPeerGraphic;
		}
		
		pPeerGraphic = peer;
	}
}



// Linked List
////////////////

void deEnvMapProbe::SetParentWorld(deWorld *world){
	pParentWorld = world;
}

void deEnvMapProbe::SetLLWorldPrev(deEnvMapProbe *instance){
	pLLWorldPrev = instance;
}

void deEnvMapProbe::SetLLWorldNext(deEnvMapProbe *instance){
	pLLWorldNext = instance;
}
