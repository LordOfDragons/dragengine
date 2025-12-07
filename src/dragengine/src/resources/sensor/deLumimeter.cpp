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
#include "deLumimeter.h"
#include "deLumimeterManager.h"
#include "../world/deWorld.h"
#include "../../deEngine.h"
#include "../../systems/modules/graphic/deBaseGraphicLumimeter.h"
#include "../../common/exceptions.h"



// Class deLumimeter
//////////////////////

// Constructor, destructor
////////////////////////////

deLumimeter::deLumimeter(deLumimeterManager *manager) : deResource(manager){
	pDirection.Set(0.0f, 0.0f, 1.0f);
	pConeInnerAngle = PI;
	pConeOuterAngle = PI;
	pConeExponent = 1.0f;
	
	pPeerGraphic = NULL;
	
	pParentWorld = NULL;
	pLLWorldNext = NULL;
	pLLWorldPrev = NULL;
}

deLumimeter::~deLumimeter(){
	if(pPeerGraphic) delete pPeerGraphic;
}



// Management
///////////////

void deLumimeter::SetPosition(const decDVector &position){
	if(! position.IsEqualTo(pPosition)){
		pPosition = position;
		
		if(pPeerGraphic) pPeerGraphic->PositionChanged();
	}
}

void deLumimeter::SetDirection(const decVector &direction){
	if(! direction.IsEqualTo(pDirection)){
		pDirection = direction;
		
		if(pPeerGraphic) pPeerGraphic->DirectionChanged();
	}
}

void deLumimeter::SetConeInnerAngle(float angle){
	if(angle < 0.0f) angle = 0.0f;
	if(angle > PI) angle = PI;
	
	if(fabs(angle - pConeInnerAngle) > 1e-5){
		pConeInnerAngle = angle;
		
		if(pPeerGraphic) pPeerGraphic->ConeParameterChanged();
	}
}

void deLumimeter::SetConeOuterAngle(float angle){
	if(angle < 0.0f) angle = 0.0f;
	if(angle > PI) angle = PI;
	
	if(fabs(angle - pConeOuterAngle) > 1e-5){
		pConeOuterAngle = angle;
		
		if(pPeerGraphic) pPeerGraphic->ConeParameterChanged();
	}
}

void deLumimeter::SetConeExponent(float exponent){
	if(exponent < 0.0f) exponent = 0.0f;
	
	if(fabs(exponent - pConeExponent) > 1e-5){
		pConeExponent = exponent;
		
		if(pPeerGraphic) pPeerGraphic->ConeParameterChanged();
	}
}



// Measuring
//////////////

float deLumimeter::MeasureLuminance(){
	if(pPeerGraphic){
		return pPeerGraphic->MeasureLuminance();
		
	}else{
		return 0.0f;
	}
}

decColor deLumimeter::MeasureColor(){
	if(pPeerGraphic){
		return pPeerGraphic->MeasureColor();
		
	}else{
		return decColor();
	}
}



// Linked List
////////////////

void deLumimeter::SetParentWorld(deWorld *world){
	pParentWorld = world;
}

void deLumimeter::SetLLWorldPrev(deLumimeter *lumimeter){
	pLLWorldPrev = lumimeter;
}

void deLumimeter::SetLLWorldNext(deLumimeter *lumimeter){
	pLLWorldNext = lumimeter;
}



// System Peers
/////////////////

void deLumimeter::SetPeerGraphic(deBaseGraphicLumimeter *peer){
	if(peer == pPeerGraphic){
		return;
	}
	
	if(pPeerGraphic){
		delete pPeerGraphic;
	}
	
	pPeerGraphic = peer;
}
