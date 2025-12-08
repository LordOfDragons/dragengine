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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "reRigPush.h"
#include "../reRig.h"
#include "../bone/reRigBone.h"

#include <deigde/gui/igdeShapeBuilder.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>

#include <dragengine/deEngine.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/exceptions.h>



// Class reRigPush
////////////////////

// Constructor, destructor
////////////////////////////

reRigPush::reRigPush(deEngine *engine){
	if(!engine){
		DETHROW(deeInvalidParam);
	}
	
	pEngine = engine;
	pRig = NULL;
	
	pDebugDrawer = NULL;
	pDDSPush = NULL;
	pCollider = NULL;
	
	pType = reRigPush::eptSimple;
	pImpuls = 1.0f;
	pRayCount = 1;
	pConeAngle = 180.0f;
	
	pSelected = false;
	pActive = false;
	
	try{
		pCollider = engine->GetColliderManager()->CreateColliderVolume();
		pCollider->SetEnabled(true);
		pCollider->SetResponseType(deCollider::ertKinematic);
		pCollider->SetUseLocalGravity(true);
		
		decLayerMask layerMask;
		layerMask.SetBit(reRig::eclmPushes);
		
		pCollider->SetCollisionFilter(decCollisionFilter(layerMask));
		
		// create debug drawer and shapes
		pDebugDrawer = engine->GetDebugDrawerManager()->CreateDebugDrawer();
		pDebugDrawer->SetXRay(true);
		
		pDDSPush = new igdeWDebugDrawerShape;
		pUpdateDDPushGeometry();
		pDDSPush->SetVisible(true);
		pDDSPush->SetParentDebugDrawer(pDebugDrawer);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

reRigPush::reRigPush(const reRigPush &push){
	pEngine = push.pEngine;
	pRig = NULL;
	
	pDebugDrawer = NULL;
	pDDSPush = NULL;
	pCollider = NULL;
	
	pType = push.pType;
	pPosition = push.pPosition;
	pOrientation = push.pOrientation;
	pImpuls = push.pImpuls;
	pRayCount = push.pRayCount;
	pConeAngle = push.pConeAngle;
	
	pSelected = false;
	pActive = false;
	
	try{
		pCollider = pEngine->GetColliderManager()->CreateColliderVolume();
		pCollider->SetEnabled(true);
		pCollider->SetResponseType(deCollider::ertKinematic);
		pCollider->SetUseLocalGravity(true);
		
		decLayerMask layerMask;
		layerMask.SetBit(reRig::eclmPushes);
		
		pCollider->SetCollisionFilter(decCollisionFilter(layerMask));
		
		// create debug drawer and shapes
		pDebugDrawer = pEngine->GetDebugDrawerManager()->CreateDebugDrawer();
		pDebugDrawer->SetXRay(true);
		
		pDDSPush = new igdeWDebugDrawerShape;
		pUpdateDDPushGeometry();
		pDDSPush->SetVisible(true);
		pDDSPush->SetParentDebugDrawer(pDebugDrawer);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

reRigPush::~reRigPush(){
	pCleanUp();
}



// Management
///////////////

void reRigPush::SetRig(reRig *rig){
	if(pRig){
		pRig->GetEngineWorld()->RemoveDebugDrawer(pDebugDrawer);
		pRig->GetEngineWorld()->RemoveCollider(pCollider);
	}
	
	pRig = rig;
	
	if(rig){
		rig->GetEngineWorld()->AddCollider(pCollider);
		rig->GetEngineWorld()->AddDebugDrawer(pDebugDrawer);
	}
	
	pUpdateDDPush();
	pUpdateColliderShape();
}



void reRigPush::SetType(reRigPush::ePushTypes type){
	if(type == pType){
		return;
	}
	
	pType = type;
	
	if(pRig){
		pRig->NotifyPushChanged(this);
	}
}

void reRigPush::SetPosition(const decVector &position){
	if(position.IsEqualTo(pPosition)){
		return;
	}
	
	pPosition = position;
	
	if(pRig){
		pRig->NotifyPushChanged(this);
	}
	
	pDebugDrawer->SetPosition(position);
	pUpdateColliderShape();
}

void reRigPush::SetOrientation(const decVector &orientation){
	if(orientation.IsEqualTo(pOrientation)){
		return;
	}
	
	pOrientation = orientation;
	
	if(pRig){
		pRig->NotifyPushChanged(this);
	}
	
	pDebugDrawer->SetOrientation(decQuaternion::CreateFromEuler(orientation * DEG2RAD));
	pUpdateColliderShape();
}

void reRigPush::SetImpuls(float impuls){
	if(fabs(impuls - pImpuls) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pImpuls = impuls;
	
	if(pRig){
		pRig->NotifyPushChanged(this);
	}
}

void reRigPush::SetRayCount(int rayCount){
	if(rayCount < 1){
		DETHROW(deeInvalidParam);
	}
	
	if(rayCount != pRayCount){
		pRayCount = rayCount;
		
		if(pRig){
			pRig->NotifyPushChanged(this);
		}
	}
}

void reRigPush::SetConeAngle(float angle){
	if(fabsf(angle - pConeAngle) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pConeAngle = angle;
	
	if(pRig){
		pRig->NotifyPushChanged(this);
	}
}



void reRigPush::SetSelected(bool selected){
	if(selected == pSelected){
		return;
	}
	
	pSelected = selected;
	
	pUpdateDDPush();
}

void reRigPush::SetActive(bool active){
	if(active == pActive){
		return;
	}
	
	pActive = active;
	
	pUpdateDDPush();
}



void reRigPush::ShowStateChanged(){
	pUpdateDDPush();
	
	pCollider->SetEnabled(IsVisible());
}



bool reRigPush::IsVisible() const{
	if(pRig){
		return pRig->GetShowRigPushes();
		
	}else{
		return false;
	}
}



// Private Functions
//////////////////////

void reRigPush::pCleanUp(){
	SetRig(NULL);
	if(pDDSPush){
		delete pDDSPush;
	}
}

void reRigPush::pUpdateDDPush(){
	pDebugDrawer->SetVisible(IsVisible());
	
	if(pActive){
		pDDSPush->SetEdgeColor(decColor(0.0f, 0.8f, 0.0f, 1.0));
		pDDSPush->SetFillColor(decColor(0.0f, 0.8f, 0.0f, 0.25));
		
	}else if(pSelected){
		pDDSPush->SetEdgeColor(decColor(0.5f, 0.8f, 1.0f, 1.0));
		pDDSPush->SetFillColor(decColor(0.5f, 0.8f, 1.0f, 0.25));
		
	}else{
		pDDSPush->SetEdgeColor(decColor(0.0f, 0.5f, 0.0f, 1.0f));
		pDDSPush->SetFillColor(decColor(0.0f, 0.5f, 0.0f, 0.25f));
	}
}

void reRigPush::pUpdateDDPushGeometry(){
	igdeShapeBuilder builder;
	
	builder.CreateArrow(*pDDSPush, decVector(0.0f, 0.0f, -0.5f), decVector(), 0.05f, 0.08f, 0.01f);
}

void reRigPush::pUpdateColliderShape(){
	if(pCollider && pRig){
		decMatrix matrix = decMatrix::CreateRotation(pOrientation * DEG2RAD);
		decShapeBox *box = NULL;
		decVector arrowStart;
		decShapeList shapeList;
		
		arrowStart = pPosition - matrix.TransformView() * 0.5f;
		
		pCollider->SetPosition((arrowStart + pPosition) * 0.5f);
		pCollider->SetOrientation(matrix.ToQuaternion());
		pCollider->SetEnabled(IsVisible());
		
		try{
			box = new decShapeBox(decVector(0.05f, 0.05f, 0.5f));
			shapeList.Add(box);
			
		}catch(const deException &){
			if(box){
				delete box;
			}
		}
		
		pCollider->SetShapes(shapeList);
	}
}
