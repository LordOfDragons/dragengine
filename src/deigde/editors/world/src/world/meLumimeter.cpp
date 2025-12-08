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
#include "meWorld.h"
#include "meLumimeter.h"
#include "object/meObject.h"
#include "dragengine/deEngine.h"
#include "dragengine/resources/world/deWorld.h"
#include "dragengine/resources/sensor/deLumimeter.h"
#include "dragengine/resources/sensor/deLumimeterManager.h"
#include "dragengine/common/exceptions.h"



// Class meLumimeter
//////////////////////

// Constructor, destructor
////////////////////////////

meLumimeter::meLumimeter(deEngine *engine){
	if(!engine) DETHROW(deeInvalidParam);
	
	pEngine = engine;
	pWorld = NULL;
	pLumimeter = NULL;
	pDDVolume = NULL;
	
	pDirection.Set(0.0f, 0.0f, 1.0f);
	pConeInnerAngle = 180.0f;
	pConeOuterAngle = 180.0f;
	pConeExponent = 1.0f;
	
	pTrackCamera = false;
	
	pHostObject = NULL;
	
	try{
		pLumimeter = engine->GetLumimeterManager()->CreateLumimeter();
		
		pLumimeter->SetPosition(pPosition);
		pLumimeter->SetDirection(pDirection);
		pLumimeter->SetConeInnerAngle(pConeInnerAngle * DEG2RAD);
		pLumimeter->SetConeOuterAngle(pConeOuterAngle * DEG2RAD);
		pLumimeter->SetConeExponent(pConeExponent);
		
		SetName("Lumimeter");
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

meLumimeter::~meLumimeter(){
	pCleanUp();
}



// Management
///////////////

void meLumimeter::SetWorld(meWorld *world){
	if(world == pWorld) return;
	
	if(pWorld){
		if(pDDVolume){
			//pWorld->GetDDLumimeters()->RemoveVolume( pDDVolume );
			pDDVolume = NULL;
		}
		pWorld->GetEngineWorld()->RemoveLumimeter(pLumimeter);
	}
	
	pWorld = world;
	
	if(world){
		pWorld->GetEngineWorld()->AddLumimeter(pLumimeter);
	}
	
	pUpdateDDVGeometry();
	pUpdateDDVolume();
}

void meLumimeter::SetName(const char *name){
	pName = name;
}

void meLumimeter::SetPosition(const decDVector &position){
	if(position.IsEqualTo(pPosition)){
		return;
	}
	
	pPosition = position;
	
	pLumimeter->SetPosition(pPosition);
	
	pUpdateDDVGeometry();
	pWorld->NotifyLumimeterChanged();
}

void meLumimeter::SetDirection(const decVector &direction){
	if(direction.IsEqualTo(pDirection)){
		return;
	}
	
	pDirection = direction;
	
	pLumimeter->SetDirection(pDirection);
	
	pUpdateDDVGeometry();
	pWorld->NotifyLumimeterChanged();
}

void meLumimeter::SetConeInnerAngle(float angle){
	if(fabs(angle - pConeInnerAngle) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pConeInnerAngle = angle;
	
	pLumimeter->SetConeInnerAngle(pConeInnerAngle * DEG2RAD);
	
	pUpdateDDVGeometry();
	pWorld->NotifyLumimeterChanged();
}

void meLumimeter::SetConeOuterAngle(float angle){
	if(fabs(angle - pConeOuterAngle) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pConeOuterAngle = angle;
	
	pLumimeter->SetConeOuterAngle(pConeOuterAngle * DEG2RAD);
	
	pUpdateDDVGeometry();
	pWorld->NotifyLumimeterChanged();
}

void meLumimeter::SetConeExponent(float exponent){
	if(fabs(exponent - pConeExponent) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pConeExponent = exponent;
	
	pLumimeter->SetConeExponent(pConeExponent);
	
	pWorld->NotifyLumimeterChanged();
}



void meLumimeter::SetTrackCamera(bool trackCamera){
	if(trackCamera == pTrackCamera){
		return;
	}
	
	pTrackCamera = trackCamera;
	
	pWorld->NotifyLumimeterChanged();
}



bool meLumimeter::HasHostObject() const{
	return pHostObject != NULL;
}

void meLumimeter::SetHostObject(meObject *object){
	if(object == pHostObject){
		return;
	}
	pHostObject = object;
	pWorld->NotifyLumimeterChanged();
}



// Measuring
//////////////

float meLumimeter::MeasureLuminance(){
	return pLumimeter ? pLumimeter->MeasureLuminance() : 0.0f;
}

decColor meLumimeter::MeasureColor(){
	return pLumimeter ? pLumimeter->MeasureColor() : decColor();
}



// Private Functions
//////////////////////

void meLumimeter::pCleanUp(){
	SetHostObject(NULL);
	SetWorld(NULL);
	
	if(pLumimeter) pLumimeter->FreeReference();
}

void meLumimeter::pUpdateDDVolume(){
	/*
	// check if we need a volume and create or destroy it if required
	if(pWorld && pVisible && (pCurrent || pSelected)){
		if(!pDDVolume){
			try{
				pDDVolume = new deDebugDrawerVolume;
				if(!pDDVolume) DETHROW(deeOutOfMemory);
				pWorld->GetDDObjects()->AddVolume(pDDVolume);
				
			}catch(const deException &){
				if(pDDVolume){
					delete pDDVolume;
					pDDVolume = NULL;
				}
				throw;
			}
			pUpdateDDVGeometry();
		}
		
	}else if(pDDVolume){
		pWorld->GetDDObjects()->RemoveVolume(pDDVolume);
		pDDVolume = NULL;
	}
	
	// update color if volume exists
	if(pDDVolume){
		if(pCurrent){
			pDDVolume->SetEdgeColor(decColor(1.0f, 0.5f, 0.0f, 1.0));
		}else if(pSelected){
			pDDVolume->SetEdgeColor(decColor(1.0f, 0.0f, 0.0f, 1.0));
		}else{
			pDDVolume->SetEdgeColor(decColor(0.0f, 0.0f, 0.0f, 0.0f));
		}
		pDDVolume->SetFillColor(decColor(0.0f, 0.0f, 0.0f, 0.0f));
	}
	*/
}

void meLumimeter::pUpdateDDVGeometry(){
	/*
	if(pDDVolume){
		decCollisionBox *colBox = NULL;
		
		try{
			colBox = new decCollisionBox(pPosition,
				pSize * 0.5f + decVector(0.01f, 0.01f, 0.01f),
				decMatrix::CreateRotation(pRotation).ToQuaternion());
			pDDVolume->SetVolume(colBox);
		}catch(const deException &){
			if(colBox) delete colBox;
		}
	}
	*/
}
