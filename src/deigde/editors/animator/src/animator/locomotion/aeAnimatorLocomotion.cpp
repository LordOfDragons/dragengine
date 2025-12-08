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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aeALOccupiedTest.h"
#include "aeAnimatorLocomotion.h"
#include "aeAnimatorLocomotionLeg.h"
#include "../aeAnimator.h"
#include "../aeSubAnimator.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/sensor/deTouchSensor.h>
#include <dragengine/resources/sensor/deTouchSensorManager.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/world/deWorldManager.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/physics/deBasePhysicsWorld.h>



// Definitions
////////////////

#define LOGSOURCE "Animator Editor"



// Class aeAnimatorLocomotion
///////////////////////////////

// Constructor, destructor
////////////////////////////

aeAnimatorLocomotion::aeAnimatorLocomotion(aeAnimator *animator) :
pAnimator(animator),
pColliderPosition(0.0f, 0.4f, 0.0f),
pColliderRadius(0.4f),
pTSGroundPosition(0.0f, 0.0f, 0.0f),
pTSGroundExtents(0.4f, 0.1f, 0.4f),
pTSDragonBodyPosition(0.0f, 0.8f, 0.0f),
pTSDragonBodySize(2.5f, 0.81f, 2.5f),

pGroundPlaneFront(0.2f),
pGroundPlaneBack(-0.2f),
pGroundPlaneLeft(-0.2f),
pGroundPlaneRight(0.2f),
pGroundPlaneRadius(0.1f),
pGroundPlaneOffsetFL(0.0f),
pGroundPlaneOffsetFR(0.0f),
pGroundPlaneOffsetBL(0.0f),
pGroundPlaneOffsetBR(0.0f),

pDragonColBodyPos(0.0f, 0.8f, 0.0f),
pDragonColBodyRot(0.0f, 0.0f, 0.0f),
pDragonColBodySize(0.8f, 0.8f, 2.0f),
pDragonColHeadPos(),
pDragonColHeadRot(),
pDragonColHeadSize(),
pDragonColHandsPos(0.0f, 0.45f, 0.5f),
pDragonColHandsRadius(0.0f),
pDragonColFeetPos(0.0f, 0.45f, -0.6f),
pDragonColFeetRadius(0.45f),
pDragonFootGround(0.2f),
pDragonFootFront(0.5f),
pDragonFootBack(-0.6f),
pDragonFootSpread(0.2f),
pDragonFootRadius(0.2f),

pLegs(NULL),
pLegCount(0),
pUseLegPairCount(1),
pLegBlendTime(0.0f),
pLegGCCollider(NULL),
pLegGCColliderRadius(0.05f),

pLocomotionType(eltNatural),

pLimitLookDown(-90.0f),
pLimitLookUp(90.0f),

pLimitLookRight(90.0f),
pLimitLookLeft(-90.0f),

pTurnLeftRight(0.0f),

pAdjustTimeTurnIP(1.0f),

pSpeedWalk(1.25f),
pSpeedRun(4.0f),

pPosition(),
pTurnVelocity(0.0f),
pTiltOffset(0.0f),
pTiltUpDown(0.0f),
pTiltLeftRight(0.0f),
pVelocityOrientation(0.0f),
pMovingSpeed(0.0f),
pRelativeMovingSpeed(0.0f),
pMovingDirection(0.0f),
pTouchingGround(false),

pMotionTime(0.0f),

pKeyForward(false),
pKeyBackward(false),
pKeyStepLeft(false),
pKeyStepRight(false),
pKeyInputDirection(0.0f),
pToggleCrouch(false),
pToggleRun(false),

pEnabled(false),
pUseFoGIK(false),

pIsMoving(false),
pIsTurningIP(false),
pTurnIP(0.0f)
{
	if(!animator){
		DETHROW(deeInvalidParam);
	}
	
	deEngine * const engine = animator->GetEngine();
	int i;
	
	pLookUpDown.SetAdjustRange(90.0f);
	pLookUpDown.SetAdjustTime(0.0f);
	
	pLookLeftRight.SetAdjustRange(90.0f);
	pLookLeftRight.SetAdjustTime(0.0f);
	
	pOrientation.SetAdjustRange(90.0f);
	pOrientation.SetAdjustTime(1.0f);
	
	pLinearVelocity.SetAdjustRange(pSpeedRun);
	pLinearVelocity.SetAdjustTime(0.5f);
	
	pStance.SetAdjustRange(1.0f);
	pStance.SetAdjustTime(0.5f);
	
	try{
		// setup the listeners
		pColliderListener.SetAnimatorLocomotion(this);
		pTouchSensorListener.SetAnimatorLocomotion(this);
		
		// create debug drawers
		pDebugDrawer = engine->GetDebugDrawerManager()->CreateDebugDrawer();
		pDebugDrawer->SetXRay(true);
		pDebugDrawer->SetVisible(false);
		animator->GetEngineWorld()->AddDebugDrawer(pDebugDrawer);
		
		pDDSCollider.SetParentDebugDrawer(pDebugDrawer);
		pDDSCollider.SetVisible(true);
		pDDSCollider.SetEdgeColor(decColor(0.0f, 0.75f, 0.75f, 0.25f));
		pDDSCollider.SetFillColor(decColor(0.0f, 0.75f, 0.75f, 0.1f));
		
		pDDSSensorGround.SetParentDebugDrawer(pDebugDrawer);
		pDDSSensorGround.SetVisible(true);
		
		pDDSDragonColBody.SetParentDebugDrawer(pDebugDrawer);
		pDDSDragonColBody.SetVisible(false);
		pDDSDragonColBody.SetEdgeColor(decColor(0.0f, 0.75f, 0.75f, 0.25f));
		pDDSDragonColBody.SetFillColor(decColor(0.0f, 0.75f, 0.75f, 0.1f));
		
		pDDSDragonColHead.SetParentDebugDrawer(pDebugDrawer);
		pDDSDragonColHead.SetVisible(false);
		pDDSDragonColHead.SetEdgeColor(decColor(0.0f, 0.75f, 0.75f, 0.25f));
		pDDSDragonColHead.SetFillColor(decColor(0.0f, 0.75f, 0.75f, 0.1f));
		
		pDDSDragonColHands.SetParentDebugDrawer(pDebugDrawer);
		pDDSDragonColHands.SetVisible(false);
		pDDSDragonColHands.SetEdgeColor(decColor(0.0f, 0.75f, 0.75f, 0.25f));
		pDDSDragonColHands.SetFillColor(decColor(0.0f, 0.75f, 0.75f, 0.1f));
		
		pDDSDragonColFeet.SetParentDebugDrawer(pDebugDrawer);
		pDDSDragonColFeet.SetVisible(false);
		pDDSDragonColFeet.SetEdgeColor(decColor(0.0f, 0.75f, 0.75f, 0.25f));
		pDDSDragonColFeet.SetFillColor(decColor(0.0f, 0.75f, 0.75f, 0.1f));
		
		for(i=0; i<4; i++){
			pDDSGroundPlane[i].SetParentDebugDrawer(pDebugDrawer);
			pDDSGroundPlane[i].SetVisible(true);
			pDDSGroundPlane[i].SetEdgeColor(decColor(0.0f, 0.75f, 0.75f, 0.25f));
			pDDSGroundPlane[i].SetFillColor(decColor(0.0f, 0.75f, 0.75f, 0.1f));
		}
		
		// create collider
		pCreateCollider();
		
		// create touch sensor
		pCreateTouchSensor();
		
		// create 4 legs
		pLegs = new aeAnimatorLocomotionLeg*[4];
		if(!pLegs) DETHROW(deeInvalidParam);
		
		for(pLegCount=0; pLegCount<4; pLegCount++){
			pLegs[pLegCount] = new aeAnimatorLocomotionLeg(this);
			if(!pLegs[pLegCount]) DETHROW(deeOutOfMemory);
			
			pLegs[pLegCount]->SetShapesVisible(pLegCount < 2);
		}
		
		// update the leg pair information
		SetUseLegPairCount(1);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

aeAnimatorLocomotion::~aeAnimatorLocomotion(){
	pCleanUp();
}



// Management
///////////////

void aeAnimatorLocomotion::Reset(){
	const double groundThreshold = 0.01f;
	int l;
	
	pMotionTime = 0.0f;
	
	pGroundPlaneOffsetFL = 0.0f;
	pGroundPlaneOffsetFR = 0.0f;
	pGroundPlaneOffsetBL = 0.0f;
	pGroundPlaneOffsetBR = 0.0f;
	
	SetPosition(decDVector(0.0, groundThreshold, 0.0));
	SetOrientation(0.0f);
	SetTiltOffset(0.0f);
	SetTiltUpDown(0.0f);
	SetTiltLeftRight(0.0f);
	pLinearVelocity.Reset();
	SetTurnVelocity(0.0f);
	SetVelocityOrientation(0.0f);
	SetMovingDirection(0.0f);
	SetMovingSpeed(0.0f);
	SetRelativeMovingSpeed(0.0f);
	pStance.Reset();
	
	pLookLeftRight.Reset();
	SetLookUpDown(0.0f);
	SetLookUpDownGoal(0.0f);
	
	SetKeyForward(false);
	SetKeyBackward(false);
	SetKeyStepLeft(false);
	SetKeyStepRight(false);
	SetKeyInputDirection(0.0f);
	SetToggleCrouch(false);
	SetToggleRun(false);
	
	pIsMoving = false;
	pIsTurningIP = false;
	pTurnIP = 0.0f;
	pTouchingGround = false;
	
	for(l=0; l<pLegCount; l++){
		pLegs[l]->Reset();
	}
}



deLogger *aeAnimatorLocomotion::GetLogger() const{
	return pAnimator->GetLogger();
}

void aeAnimatorLocomotion::SetColliderPosition(const decVector &position){
	pColliderPosition = position;
	UpdateCollider();
}

void aeAnimatorLocomotion::SetColliderRadius(float radius){
	pColliderRadius = radius;
	UpdateCollider();
}

void aeAnimatorLocomotion::UpdateCollider(){
	if(pCollider){
		decShape *shape = NULL;
		decShapeList shapeList;
		int i;
		
		// update shape of movement collider
		try{
			if(pUseLegPairCount == 1){
				shape = new decShapeSphere(pColliderRadius, pColliderPosition);
				if(!shape) DETHROW(deeOutOfMemory);
				shapeList.Add(shape);
				shape = NULL;
				
			}else{
				shape = new decShapeSphere(pColliderRadius, pColliderPosition);
				if(!shape) DETHROW(deeOutOfMemory);
				shapeList.Add(shape);
				shape = NULL;
				shape = new decShapeSphere(0.4f, decVector(0.0f, 0.8f, 0.6f));
				if(!shape) DETHROW(deeOutOfMemory);
				shapeList.Add(shape);
				shape = NULL;
				shape = new decShapeSphere(0.4f, decVector(0.0f, 0.8f, -0.6f));
				if(!shape) DETHROW(deeOutOfMemory);
				shapeList.Add(shape);
				shape = NULL;
				/*
				shape = new decShapeBox(pDragonColBodySize * 0.5f, pDragonColBodyPos, decMatrix::CreateRotation(pDragonColBodyRot * DEG2RAD).ToQuaternion());
				if(!shape) DETHROW(deeOutOfMemory);
				shapeList.AddShape(shape);
				shape = NULL;
				*/
			}
			
		}catch(const deException &){
			if(shape) delete shape;
			throw;
		}
		pCollider->SetShapes(shapeList);
		
		decShapeList shapeListTG;
		decShapeList shapeListDCB;
		// update shape of ground checker collider
		//pGroundCheckCollider->RemoveAllShapes();
		//pFootCollider->RemoveAllShapes();
		
		try{
			if(pUseLegPairCount == 1){
				/*
				shape = new decShapeSphere(pColliderRadius, pColliderPosition);
				if(!shape) DETHROW(deeOutOfMemory);
				pGroundCheckCollider->AddShape(shape);
				shape = NULL;
				*/
				/*
				shape = new decShapeSphere(pColliderRadius);
				if(!shape) DETHROW(deeOutOfMemory);
				pGroundCheckCollider->AddShape(shape);
				shape = NULL;
				*/
				shape = new decShapeBox(decVector(0.01f, 0.05f, 0.15f), decVector(0.0f, 0.05f, 0.0f));
				if(!shape) DETHROW(deeOutOfMemory);
				shapeListTG.Add(shape);
				shape = NULL;
				
			}else{
				/*
				shape = new decShapeSphere(pDragonColHandsRadius);
				if(!shape) DETHROW(deeOutOfMemory);
				pGroundCheckCollider->AddShape(shape);
				shape = NULL;
				*/
				/*
				shape = new decShapeSphere(pDragonFootRadius);
				if(!shape) DETHROW(deeOutOfMemory);
				pFootCollider->AddShape(shape);
				shape = NULL;
				*/
				
				/*
				shape = new decShapeBox(pDragonColBodySize * 0.5f, pDragonColBodyPos, decMatrix::CreateRotation(pDragonColBodyRot * DEG2RAD).ToQuaternion());
				if(!shape) DETHROW(deeOutOfMemory);
				pDragonColBody->AddShape(shape);
				shape = NULL;
				*/
				shape = new decShapeSphere(0.4f, decVector(0.0f, 0.8f, 0.6f));
				if(!shape) DETHROW(deeOutOfMemory);
				shapeListDCB.Add(shape);
				shape = NULL;
				shape = new decShapeSphere(0.4f, decVector(0.0f, 0.8f, -0.6f));
				if(!shape) DETHROW(deeOutOfMemory);
				shapeListDCB.Add(shape);
				shape = NULL;
				
				shape = new decShapeBox(decVector(0.06f, 0.05f, 0.15f), decVector(0.0f, 0.05f, 0.0f));
				if(!shape) DETHROW(deeOutOfMemory);
				shapeListTG.Add(shape);
				shape = NULL;
			}
			
		}catch(const deException &){
			if(shape) delete shape;
			throw;
		}
		pTGCollider->SetShapes(shapeListTG);
		pDragonColBody->SetShapes(shapeListDCB);
		
		// update shape of ground plane collider
		decShapeList shapeListGPC;
		
		try{
			shape = new decShapeSphere(pGroundPlaneRadius, decVector(0.0f, pGroundPlaneRadius, 0.0f));
			shapeListGPC.Add(shape);
			shape = NULL;
			
		}catch(const deException &){
			if(shape) delete shape;
			throw;
		}
		pGroundPlaneCollider->SetShapes(shapeListGPC);
		
		// update debug drawer
		pDDSCollider.RemoveAllShapes();
		pDDSDragonColBody.RemoveAllShapes();
		pDDSDragonColHead.RemoveAllShapes();
		pDDSDragonColHands.RemoveAllShapes();
		pDDSDragonColFeet.RemoveAllShapes();
		
		for(i=0; i<4; i++){
			pDDSGroundPlane[i].RemoveAllShapes();
		}
		
		try{
			if(pUseLegPairCount == 1){
				shape = new decShapeSphere(pColliderRadius, pColliderPosition);
				if(!shape) DETHROW(deeOutOfMemory);
				pDDSCollider.AddShape(shape);
				shape = NULL;
				
				shape = new decShapeSphere(pColliderRadius);
				if(!shape) DETHROW(deeOutOfMemory);
				pDDSDragonColHands.AddShape(shape);
				shape = NULL;
				
				shape = new decShapeSphere(pColliderRadius);
				if(!shape) DETHROW(deeOutOfMemory);
				pDDSDragonColFeet.AddShape(shape);
				shape = NULL;
				
			}else{
				shape = new decShapeSphere(pColliderRadius, pColliderPosition);
				if(!shape) DETHROW(deeOutOfMemory);
				pDDSCollider.AddShape(shape);
				shape = NULL;
				shape = new decShapeSphere(0.4f, decVector(0.0f, 0.8f, 0.6f));
				if(!shape) DETHROW(deeOutOfMemory);
				pDDSCollider.AddShape(shape);
				shape = NULL;
				shape = new decShapeSphere(0.4f, decVector(0.0f, 0.8f, -0.6f));
				if(!shape) DETHROW(deeOutOfMemory);
				pDDSCollider.AddShape(shape);
				shape = NULL;
				/*
				shape = new decShapeBox(pDragonColBodySize * 0.5f, pDragonColBodyPos, decMatrix::CreateRotation(pDragonColBodyRot * DEG2RAD).ToQuaternion());
				if(!shape) DETHROW(deeOutOfMemory);
				pDDSCollider.AddShape(shape);
				shape = NULL;
				*/
				
				shape = new decShapeSphere(0.4f, decVector(0.0f, 0.8f, 0.6f));
				if(!shape) DETHROW(deeOutOfMemory);
				pDDSDragonColBody.AddShape(shape);
				shape = NULL;
				shape = new decShapeSphere(0.4f, decVector(0.0f, 0.8f, -0.6f));
				if(!shape) DETHROW(deeOutOfMemory);
				pDDSDragonColBody.AddShape(shape);
				shape = NULL;
				/*
				shape = new decShapeBox(pDragonColBodySize * 0.5f, pDragonColBodyPos, decMatrix::CreateRotation(pDragonColBodyRot * DEG2RAD).ToQuaternion());
				if(!shape) DETHROW(deeOutOfMemory);
				pDDSDragonColBody.AddShape(shape);
				shape = NULL;
				*/
				
				shape = new decShapeSphere(pDragonColHandsRadius);
				if(!shape) DETHROW(deeOutOfMemory);
				pDDSDragonColHands.AddShape(shape);
				shape = NULL;
				
				shape = new decShapeSphere(pDragonColFeetRadius);
				if(!shape) DETHROW(deeOutOfMemory);
				pDDSDragonColFeet.AddShape(shape);
				shape = NULL;
			}
			
			for(i=0; i<4; i++){
				shape = new decShapeSphere(pGroundPlaneRadius, decVector(0.0f, pGroundPlaneRadius, 0.0f));
				if(!shape) DETHROW(deeOutOfMemory);
				pDDSGroundPlane[i].AddShape(shape);
				shape = NULL;
			}
			
		}catch(const deException &){
			if(shape) delete shape;
			throw;
		}
	}
}

void aeAnimatorLocomotion::UpdateTouchSensors(){
	// touch sensor shapes
	decShapeBox *shape = NULL;
	decShapeList list;
	
	try{
		shape = new decShapeBox(pTSGroundExtents * 0.5f, pTSGroundPosition);
		list.Add(shape);
		shape = NULL;
		/*
		shape = new decShapeBox(pTSDragonBodySize * 0.5f, pTSDragonBodyPosition);
		list.Add(shape);
		shape = NULL;
		*/
		pTouchSensor->SetShape(list);
		
	}catch(const deException &){
		if(shape){
			delete shape;
		}
		throw;
	}
	
	// debug drawer shapes
	pDDSSensorGround.RemoveAllShapes();
	
	try{
		shape = new decShapeBox(pTSGroundExtents * 0.5f, pTSGroundPosition);
		if(!shape) DETHROW(deeOutOfMemory);
		pDDSSensorGround.AddShape(shape);
		shape = NULL;
		
		shape = new decShapeBox(pTSDragonBodySize * 0.5f, pTSDragonBodyPosition);
		if(!shape) DETHROW(deeOutOfMemory);
		pDDSSensorGround.AddShape(shape);
		shape = NULL;
		
	}catch(const deException &){
		if(shape) delete shape;
		throw;
	}
}

void aeAnimatorLocomotion::UpdateTSColors(){
	if(pTouchSensor->IsEmpty()){
		pDDSSensorGround.SetEdgeColor(decColor(0.0f, 0.5f, 0.0f, 0.25f));
		pDDSSensorGround.SetFillColor(decColor(0.0f, 0.5f, 0.0f, 0.1f));
		
	}else{
		pDDSSensorGround.SetEdgeColor(decColor(0.5f, 0.0f, 0.0f, 0.25f));
		pDDSSensorGround.SetFillColor(decColor(0.5f, 0.0f, 0.0f, 0.1f));
	}
}



bool aeAnimatorLocomotion::GetShowShapes() const{
	return pDebugDrawer->GetVisible();
}

void aeAnimatorLocomotion::SetShowShapes(bool showShapes){
	pDebugDrawer->SetVisible(showShapes);
}



void aeAnimatorLocomotion::SetLocomotionType(eLocomotionTypes type){
	if(type == pLocomotionType){
		return;
	}
	
	pLocomotionType = type;
	
	pAnimator->NotifyLocomotionChanged();
	pAnimator->SetChanged(true);
}



aeAnimatorLocomotionLeg *aeAnimatorLocomotion::GetLegAt(int index) const{
	if(index < 0 || index >= pLegCount) DETHROW(deeInvalidParam);
	
	return pLegs[index];
}

void aeAnimatorLocomotion::SetLegBlendTime(float time){
	if(fabsf(time - pLegBlendTime) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pLegBlendTime = time;
	
	pAnimator->NotifyLocomotionChanged();
	pAnimator->SetChanged(true);
}

void aeAnimatorLocomotion::SetUseLegPairCount(int pairCount){
	if(pairCount < 1 || pairCount > 2){
		DETHROW(deeInvalidParam);
	}
	
	if(pairCount == pUseLegPairCount){
		return;
	}
	
	int l, legCount = pairCount * 2;
	
	pUseLegPairCount = pairCount;
	
	if(pairCount == 1){
		pTSGroundPosition.Set(0.0f, 0.0f, 0.0f);
		//pTSGroundExtents.Set( 0.4f, 0.1f, 0.4f );
		pTSGroundExtents.Set(0.4f, 0.5f, 0.8f);
		
		pDDSCollider.SetVisible(true);
		
		pDDSDragonColBody.SetVisible(false);
		pDDSDragonColHead.SetVisible(false);
		pDDSDragonColHands.SetVisible(false);
		pDDSDragonColFeet.SetVisible(false);
		
		pLegs[0]->SetVisBoneName("ikb.foot.l");
		pLegs[1]->SetVisBoneName("ikb.foot.r");
		
	}else{
		pTSGroundPosition.Set(0.0f, 0.0f, 0.0f);
		pTSGroundExtents.Set(0.6f, 0.8f, 2.0f);
		/*
		pDDSCollider.SetVisible(false);
		
		pDDSDragonColBody.SetVisible(true);
		pDDSDragonColHead.SetVisible(true);
		pDDSDragonColHands.SetVisible(true);
		pDDSDragonColFeet.SetVisible(true);
		*/
		pDDSCollider.SetVisible(true);
		
		pDDSDragonColBody.SetVisible(false);
		pDDSDragonColHead.SetVisible(false);
		pDDSDragonColHands.SetVisible(false);
		pDDSDragonColFeet.SetVisible(false);
		
		pLegs[0]->SetVisBoneName("ikb.hand.l");
		pLegs[1]->SetVisBoneName("ikb.hand.r");
		pLegs[2]->SetVisBoneName("ikb.foot.l");
		pLegs[3]->SetVisBoneName("ikb.foot.r");
	}
	
	for(l=0; l<pLegCount; l++){
		pLegs[l]->SetShapesVisible(l < legCount);
	}
	
	UpdateCollider();
	UpdateTouchSensors();
	
	// update the sub animator for ik purpose
	aeSubAnimator *subAnimator = pAnimator->GetSubAnimator();
	decVector minRot(-45.0f, -180.0f, -45.0f);
	decVector maxRot(0.0f, 180.0f, 45.0f);
	
	subAnimator->ClearAnimator();
	subAnimator->AddRuleSS();
	
	if(pairCount == 1){
		decVector localPosFoot(0.0f, 0.0f, 0.2f/*0.133f*/);
		
		subAnimator->AddRuleSS();
		subAnimator->EnableRule(1, false);
// 		subAnimator->AddRuleTG( minRot, maxRot, -1, -1, -1 );
// 		subAnimator->AddBoneToRule( 1, "ikb.foot.l" );
// 		subAnimator->EnableRule( 1, true );
		
		subAnimator->AddRuleSS();
		subAnimator->EnableRule(1, false);
// 		subAnimator->AddRuleTG( minRot, maxRot, -1, -1, -1 );
// 		subAnimator->AddBoneToRule( 2, "ikb.foot.r" );
// 		subAnimator->EnableRule( 2, true );
		
		subAnimator->AddRuleIK(localPosFoot, decVector(), "ikt.foot.l", -1);
		subAnimator->AddBoneToRule(3, "leg.u.l");
		subAnimator->AddBoneToRule(3, "foot.l");
		
		subAnimator->AddRuleIK(localPosFoot, decVector(), "ikt.foot.r", -1);
		subAnimator->AddBoneToRule(4, "leg.u.r");
		subAnimator->AddBoneToRule(4, "foot.r");
		
	}else{
		decVector localPosHand(0.0f, 0.0f, 0.1236f); //localPosHand(0.0f, 0.0f, 0.206f);
		decVector localPosFoot(0.0f, 0.0f, 0.078f); //localPosFoot(0.0f, 0.0f, 0.130f);
		
		subAnimator->AddRuleSS();
		subAnimator->EnableRule(1, false);
// 		subAnimator->AddRuleTG( minRot, maxRot, -1, -1, -1 );
// 		subAnimator->AddBoneToRule( 1, "ikb.hand.l" );
// 		
		subAnimator->AddRuleSS();
		subAnimator->EnableRule(1, false);
// 		subAnimator->AddRuleTG( minRot, maxRot, -1, -1, -1 );
// 		subAnimator->AddBoneToRule( 2, "ikb.hand.r" );
		
		subAnimator->AddRuleSS();
		subAnimator->EnableRule(1, false);
// 		subAnimator->AddRuleTG( minRot, maxRot, -1, -1, -1 );
// 		subAnimator->AddBoneToRule( 3, "ikb.foot.l" );
		
		subAnimator->AddRuleSS();
		subAnimator->EnableRule(1, false);
// 		subAnimator->AddRuleTG( minRot, maxRot, -1, -1, -1 );
// 		subAnimator->AddBoneToRule( 4, "ikb.foot.r" );
		
		subAnimator->AddRuleIK(localPosHand, decVector(), "ikt.hand.l", -1);
		subAnimator->AddBoneToRule(5, "shoulder.l");
		subAnimator->AddBoneToRule(5, "hand2.l");
		
		subAnimator->AddRuleIK(localPosHand, decVector(), "ikt.hand.r", -1);
		subAnimator->AddBoneToRule(6, "shoulder.r");
		subAnimator->AddBoneToRule(6, "hand2.r");
		
		subAnimator->AddRuleIK(localPosFoot, decVector(), "ikt.foot.l", -1);
		subAnimator->AddBoneToRule(7, "leg.u.l");
		subAnimator->AddBoneToRule(7, "foot2.l");
		
		subAnimator->AddRuleIK(localPosFoot, decVector(), "ikt.foot.r", -1);
		subAnimator->AddBoneToRule(8, "leg.u.r");
		subAnimator->AddBoneToRule(8, "foot2.r");
	}
	
	pAnimator->NotifyLocomotionChanged();
	pAnimator->SetChanged(true);
}



void aeAnimatorLocomotion::SetEnabled(bool enabled){
	if(enabled != pEnabled){
		pEnabled = enabled;
		
		if(pCollider){
			pCollider->SetEnabled(enabled);
			pCollider->SetPosition(pPosition);
			pCollider->SetLinearVelocity(decVector());
			pCollider->SetAngularVelocity(decVector());
		}
		
		UpdateTSColors();
	}
}

void aeAnimatorLocomotion::SetUseFoGIK(bool use){
	pUseFoGIK = use;
}



void aeAnimatorLocomotion::SetLimitLookDown(float degrees){
	if(fabsf(degrees - pLimitLookDown) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pLimitLookDown = degrees;
	
	SetLookUpDown(pLookUpDown.GetValue());
	SetLookUpDownGoal(pLookUpDown.GetGoal());
	
	pAnimator->NotifyLocomotionChanged();
	pAnimator->SetChanged(true);
}

void aeAnimatorLocomotion::SetLimitLookUp(float degrees){
	if(fabsf(degrees - pLimitLookUp) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pLimitLookUp = degrees;
	
	SetLookUpDown(pLookUpDown.GetValue());
	SetLookUpDownGoal(pLookUpDown.GetGoal());
	
	pAnimator->NotifyLocomotionChanged();
	pAnimator->SetChanged(true);
}

void aeAnimatorLocomotion::SetLookUpDown(float degrees){
	pLookUpDown.SetValue(decMath::clamp(degrees, pLimitLookDown, pLimitLookUp));
}

void aeAnimatorLocomotion::SetLookUpDownGoal(float degrees){
	pLookUpDown.SetGoal(decMath::clamp(degrees, pLimitLookDown, pLimitLookUp));
}



void aeAnimatorLocomotion::SetLimitLookLeft(float degrees){
	if(fabsf(degrees - pLimitLookLeft) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pLimitLookLeft = degrees;
	
	pAnimator->NotifyLocomotionChanged();
	pAnimator->SetChanged(true);
}

void aeAnimatorLocomotion::SetLimitLookRight(float degrees){
	if(fabsf(degrees - pLimitLookRight) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pLimitLookRight = degrees;
	
	pAnimator->NotifyLocomotionChanged();
	pAnimator->SetChanged(true);
}


void aeAnimatorLocomotion::SetAdjustTimeTurnIP(float adjustTime){
	if(fabsf(adjustTime - pAdjustTimeTurnIP) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pAdjustTimeTurnIP = decMath::max(adjustTime, 0.0f);
	
	pAnimator->NotifyLocomotionChanged();
	pAnimator->SetChanged(true);
}



void aeAnimatorLocomotion::SetWalkSpeed(float speed){
	if(fabsf(speed - pSpeedWalk) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pSpeedWalk = decMath::max(speed, 0.0f);
	
	pAnimator->NotifyLocomotionChanged();
	pAnimator->SetChanged(true);
}

void aeAnimatorLocomotion::SetRunSpeed(float speed){
	if(fabsf(speed - pSpeedRun) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pSpeedRun = decMath::max(speed, 0.0f);
	pLinearVelocity.SetAdjustRange(pSpeedRun);
	
	pAnimator->NotifyLocomotionChanged();
	pAnimator->SetChanged(true);
}



void aeAnimatorLocomotion::SetPosition(const decDVector &position){
	pPosition = position;
	
	pCollider->SetPosition(position);
	pTouchSensor->SetPosition(position);
	pDebugDrawer->SetPosition(position);
}

void aeAnimatorLocomotion::SetOrientation(float orientation){
	pOrientation.SetValue(decMath::normalize(orientation, 360.0f));
	
	const decMatrix matrix = decMatrix::CreateRotationY(pOrientation.GetValue() * DEG2RAD);
	pView = matrix.TransformView();
	pOrientQuat = matrix.ToQuaternion();
	
	pCollider->SetOrientation(pOrientQuat);
	pTouchSensor->SetOrientation(pOrientQuat);
	pDebugDrawer->SetOrientation(pOrientQuat);
}

void aeAnimatorLocomotion::SetTiltOffset(float offset){
	pTiltOffset = offset;
}

void aeAnimatorLocomotion::SetTiltUpDown(float tilt){
	pTiltUpDown = tilt;
}

void aeAnimatorLocomotion::SetTiltLeftRight(float tilt){
	pTiltLeftRight = tilt;
}

void aeAnimatorLocomotion::SetAngularVelocity(const decVector &velocity){
	pAngularVelocity = velocity;
}

void aeAnimatorLocomotion::SetTurnVelocity(float velocity){
	pTurnVelocity = velocity;
	SetAngularVelocity(decVector(0.0f, velocity, 0.0f));
}

void aeAnimatorLocomotion::SetVelocityOrientation(float orientation){
	pVelocityOrientation = decMath::normalize(orientation, -180.0f, 180.0f);
}

void aeAnimatorLocomotion::SetMovingSpeed(float speed){
	pMovingSpeed = decMath::max(speed, 0.0f);
}

void aeAnimatorLocomotion::SetRelativeMovingSpeed(float speed){
	pRelativeMovingSpeed = speed;
}

void aeAnimatorLocomotion::SetMovingDirection(float movingDirection){
	pMovingDirection = decMath::normalize(movingDirection, -180.0f, 180.0f);
}



void aeAnimatorLocomotion::SetKeyForward(bool pressed){
	pKeyForward = pressed;
}

void aeAnimatorLocomotion::SetKeyBackward(bool pressed){
	pKeyBackward = pressed;
}

void aeAnimatorLocomotion::SetKeyStepLeft(bool pressed){
	pKeyStepLeft = pressed;
}

void aeAnimatorLocomotion::SetKeyStepRight(bool pressed){
	pKeyStepRight = pressed;
}

void aeAnimatorLocomotion::SetKeyInputDirection(float direction){
	pKeyInputDirection = decMath::normalize(direction, -180.0f, 180.0f);;
}

void aeAnimatorLocomotion::SetToggleCrouch(bool toggled){
	pToggleCrouch = toggled;
}

void aeAnimatorLocomotion::SetToggleRun(bool toggled){
	pToggleRun = toggled;
}



void aeAnimatorLocomotion::Update(float elapsed){
	if(elapsed < 1e-5f){
		return;
	}
	
	pUpdateLooking(elapsed);
	pUpdateTurnLeftRight(elapsed);
	pUpdateIsMoving();
	pUpdateOrientation(elapsed);
	pUpdateLinearVelocity(elapsed);
	pUpdateStance(elapsed);
	
	// update collider
	if(pCollider){
		pCollider->SetLinearVelocity(pLinearVelocity.GetValue());
		
		if(pLegCount > 1){
			pCollider->SetAngularVelocity(pAngularVelocity);
		}
	}
	
	// update legs
	const int legCount = pUseLegPairCount * 2; // pLegCount
	int i;
	
	for(i=0; i<legCount; i++){
		pLegs[i]->Update(elapsed);
	}
	
	// now we can determine the real look up down which is altered by the body tilt
	// for multiple leg pair characters
// 	if( pUseLegPairCount > 1 ){
// 		SetLookUpDown( pLookUpDown.GetValue() - pTiltUpDown );
// 	}
}

void aeAnimatorLocomotion::PostPhysics(){
	decQuaternion orientation(pCollider->GetOrientation());
	decDVector position(pCollider->GetPosition());
	const float testDistance = 0.6f;
	const float gap = 0.01f;
	
	// test the how far above the ground the ai collider is located by moving the ai collider
	// along the gravity direction. a correction is only done if the ground is farther below
	// the ai collider than a small threshold. this way a collider can smoothly step down
	// slopes but can never step up especially not jump walls incorrectly. for climbing the
	// collision response code is responsible
	pGroundChecker.Reset();
	pTouchSensor->ColliderMoveHits(pCollider,
		decVector(0.0f, -testDistance, 0.0f), &pGroundChecker);
	pTouchingGround = pGroundChecker.HasCollision();
	
	if(pTouchingGround){
		const deCollisionInfo &colInfo = pGroundChecker.GetCollisionInfo();
		const float distance = colInfo.GetDistance() * testDistance - gap;
		
		if(distance >= -gap){
			position.y -= (double)distance;
			SetPosition(position);
		}
	}
	
	// once done the body tilt plane can be determined using 4 probes. the plane parameters are then stored
	// for the animator to use
	const decDMatrix matrix(decDMatrix::CreateFromQuaternion(orientation)
		* decDMatrix::CreateTranslation( position ) );
	const float tiltMoveDist = 1.0f; //5.0f;
	const float tiltGap = 0.3f; //0.5f;
	float newValue;
	
	pGroundPlaneOffsetFL = tiltGap;
	pGroundPlaneOffsetFR = tiltGap;
	pGroundPlaneOffsetBL = tiltGap;
	pGroundPlaneOffsetBR = tiltGap;
	
	pGroundChecker.Reset();
	pGroundPlaneCollider->SetPosition(matrix.Transform((double)pGroundPlaneLeft,
		(double)pGroundPlaneOffsetFL, (double)pGroundPlaneFront));
	pTouchSensor->ColliderMoveHits(pGroundPlaneCollider,
		decVector(0.0f, -tiltMoveDist, 0.0f), &pGroundChecker);
	if(pGroundChecker.HasCollision()){
		pGroundPlaneOffsetFL -= pGroundChecker.GetCollisionInfo().GetDistance() * tiltMoveDist;
	}
	
	pGroundChecker.Reset();
	pGroundPlaneCollider->SetPosition(matrix.Transform((double)pGroundPlaneRight,
		(double)pGroundPlaneOffsetFR, (double)pGroundPlaneFront));
	pTouchSensor->ColliderMoveHits(pGroundPlaneCollider,
		decVector(0.0f, -tiltMoveDist, 0.0f), &pGroundChecker);
	if(pGroundChecker.HasCollision()){
		pGroundPlaneOffsetFR -= pGroundChecker.GetCollisionInfo().GetDistance() * tiltMoveDist;
	}
	
	pGroundChecker.Reset();
	pGroundPlaneCollider->SetPosition(matrix.Transform((double)pGroundPlaneLeft,
		(double)pGroundPlaneOffsetBL, (double)pGroundPlaneBack));
	pTouchSensor->ColliderMoveHits(pGroundPlaneCollider,
		decVector(0.0f, -tiltMoveDist, 0.0f), &pGroundChecker);
	if(pGroundChecker.HasCollision()){
		pGroundPlaneOffsetBL -= pGroundChecker.GetCollisionInfo().GetDistance() * tiltMoveDist;
	}
	
	pGroundChecker.Reset();
	pGroundPlaneCollider->SetPosition(matrix.Transform((double)pGroundPlaneRight,
		(double)pGroundPlaneOffsetBR, (double)pGroundPlaneBack));
	pTouchSensor->ColliderMoveHits(pGroundPlaneCollider,
		decVector(0.0f, -tiltMoveDist, 0.0f), &pGroundChecker);
	if(pGroundChecker.HasCollision()){
		pGroundPlaneOffsetBR -= pGroundChecker.GetCollisionInfo().GetDistance() * tiltMoveDist;
	}
	
	// determine the offset relative to the collider. this determines how much the root bone has to be
	// moved up or down to provide a good distance to the ground for the feet-on-ground animator. This
	// can be done using different possibilities depending on how elaborate the model is. Here the
	// offset of the average of all probes is used. This way the animator is responsible to apply tilt
	// based motions
	/*
	newValue = pGroundPlaneOffsetFL;
	if(pGroundPlaneOffsetFR < newValue){
		newValue = pGroundPlaneOffsetFR;
	}
	if(pGroundPlaneOffsetBL < newValue){
		newValue = pGroundPlaneOffsetBL;
	}
	if(pGroundPlaneOffsetBR < newValue){
		newValue = pGroundPlaneOffsetBR;
	}*/
	newValue = (pGroundPlaneOffsetFL + pGroundPlaneOffsetFR
		+ pGroundPlaneOffsetBL + pGroundPlaneOffsetBR) * 0.25f;
	SetTiltOffset(pTiltOffset * 0.5f + newValue * 0.5f);
	
	// determine the up/down and left/right tilt. these are measured in degrees
	newValue = atanf((pGroundPlaneOffsetFL + pGroundPlaneOffsetFR
		- pGroundPlaneOffsetBL - pGroundPlaneOffsetBR) * 0.5f
			/ (pGroundPlaneFront - pGroundPlaneBack)) * RAD2DEG;
	SetTiltUpDown(pTiltUpDown * 0.5f + newValue * 0.5f);
	
	newValue = atanf((pGroundPlaneOffsetFL + pGroundPlaneOffsetBL
		- pGroundPlaneOffsetFR - pGroundPlaneOffsetBR) * 0.5f
			/ (pGroundPlaneRight - pGroundPlaneLeft)) * RAD2DEG;
	SetTiltLeftRight(pTiltLeftRight * 0.5f + newValue * 0.5f);
}

void aeAnimatorLocomotion::FeetOnGround(){
	aeSubAnimator &subAnimator = *pAnimator->GetSubAnimator();
	int l, legCount = pUseLegPairCount * 2;
	
	// determine the ik parameters for the post animator stage
	for(l=0; l<legCount; l++){
		pLegs[l]->PostUpdate();
	}
	
	// apply the feet on ground animator
	if(pUseFoGIK){
		subAnimator.Apply();
	}
}

void aeAnimatorLocomotion::PostUpdate(){
	int l, legCount = pUseLegPairCount * 2;
	
	// update debug drawes for visualizing the results
	if(pUseLegPairCount == 1){
		decDVector testpos1(0.0, (double)pColliderPosition.y, 0.3); // 0.4-0.5 for running
		decDVector testpos2(0.0, (double)pColliderPosition.y, -0.3);
		
		pDDSDragonColHands.SetPosition(testpos1);
		pDDSDragonColFeet.SetPosition(testpos2);
		
	}else{
		//decDVector testpos1( pDragonColHandsPos );
		//decDVector testpos2( pDragonColFeetPos );
		decDVector testpos1(0.0, (double)pColliderPosition.y, 0.3); // 0.4-0.5 for running
		decDVector testpos2(0.0, (double)pColliderPosition.y, -0.3);
		
		pDDSDragonColHands.SetPosition(testpos1);
		pDDSDragonColFeet.SetPosition(testpos2);
	}
	
	pDDSGroundPlane[0].SetPosition(decDVector((double)pGroundPlaneLeft,
		(double)pGroundPlaneOffsetFL, (double)pGroundPlaneFront));
	pDDSGroundPlane[1].SetPosition(decDVector((double)pGroundPlaneRight,
		(double)pGroundPlaneOffsetFR, (double)pGroundPlaneFront));
	pDDSGroundPlane[2].SetPosition(decDVector((double)pGroundPlaneLeft,
		(double)pGroundPlaneOffsetBL, (double)pGroundPlaneBack));
	pDDSGroundPlane[3].SetPosition(decDVector((double)pGroundPlaneRight,
		(double)pGroundPlaneOffsetBR, (double)pGroundPlaneBack));
	
	for(l=0; l<legCount; l++){
		pLegs[l]->UpdateDebugDrawers();
	}
}



void aeAnimatorLocomotion::OnColliderChanged(){
	SetPosition(pCollider->GetPosition());
}



// Private Functions
//////////////////////

void aeAnimatorLocomotion::pCleanUp(){
	int i;
	
	if(pLegs){
		while(pLegCount > 0){
			pLegCount--;
			delete pLegs[pLegCount];
		}
		
		delete [] pLegs;
	}
	
	for(i=0; i<4; i++){
		pDDSGroundPlane[i].SetParentDebugDrawer(NULL);
	}
	
	pDDSSensorGround.SetParentDebugDrawer(NULL);
	pDDSDragonColBody.SetParentDebugDrawer(NULL);
	pDDSDragonColHead.SetParentDebugDrawer(NULL);
	pDDSDragonColHands.SetParentDebugDrawer(NULL);
	pDDSDragonColFeet.SetParentDebugDrawer(NULL);
	pDDSCollider.SetParentDebugDrawer(NULL);
	
	if(pGroundPlaneCollider) pGroundPlaneCollider->FreeReference();
	if(pFootCollider) pFootCollider->FreeReference();
	if(pDragonColBody) pDragonColBody->FreeReference();
	if(pGroundCheckCollider) pGroundCheckCollider->FreeReference();
	if(pTGCollider) pTGCollider->FreeReference();
	
	if(pAnimator){
		deWorld *world = pAnimator->GetEngineWorld();
		
		if(pTouchSensor){
			world->RemoveTouchSensor(pTouchSensor);
		}
		
		if(pCollider){
			world->RemoveCollider(pCollider);
		}
		
		if(pDebugDrawer){
			world->RemoveDebugDrawer(pDebugDrawer);
		}
	}
}

void aeAnimatorLocomotion::pCreateCollider(){
	// NOTE: The collider component would use layer eclProxies.
	
	igdeEnvironment &environment = *pAnimator->GetEnvironment();
	deColliderManager &colmgr = *pAnimator->GetEngine()->GetColliderManager();
	deWorld &engWorld = *pAnimator->GetEngineWorld();
	decLayerMask layermask1;
	decLayerMask layermask2;
	
	//layermask.SetBit( aeAnimator::eclTerrain );
	//layermask.SetBit( aeAnimator::eclElements );
	layermask1.SetBit(aeAnimator::eclAI);
	
	layermask2.SetBit(aeAnimator::eclGround);
	
	// collider for movement
	pCollider = colmgr.CreateColliderVolume();
	
	pCollider->SetResponseType(deCollider::ertKinematic);
	pCollider->SetUseLocalGravity(true);
	pCollider->SetMass(50.0f);
	pCollider->SetEnabled(false);
	pCollider->SetCollisionFilter(decCollisionFilter(layermask1));
	environment.SetColliderDelegee(pCollider, &pColliderListener);
	engWorld.AddCollider(pCollider);
	
	// touch ground collider
	pTGCollider = colmgr.CreateColliderVolume();
	pTGCollider->SetResponseType(deCollider::ertKinematic);
	pTGCollider->SetUseLocalGravity(true);
	pTGCollider->SetEnabled(false);
	pTGCollider->SetCollisionFilter(decCollisionFilter(layermask2));
	
	// ground check collider
	/*
	pGroundCheckCollider = colmgr.CreateColliderVolume();
	pGroundCheckCollider->SetResponseType(deCollider::ertKinematic);
	pGroundCheckCollider->SetUseLocalGravity(true);
	pGroundCheckCollider->SetEnabled(false);
	pGroundCheckCollider->GetLayerMask() = layermask2;
	pGroundCheckCollider->NotifyLayerMaskChanged();
	*/
	pDragonColBody = colmgr.CreateColliderVolume();
	pDragonColBody->SetResponseType(deCollider::ertKinematic);
	pDragonColBody->SetUseLocalGravity(true);
	pDragonColBody->SetEnabled(false);
	pDragonColBody->SetCollisionFilter(decCollisionFilter(layermask2));
	
	// ground plane collider
	pGroundPlaneCollider = colmgr.CreateColliderVolume();
	pGroundPlaneCollider->SetResponseType(deCollider::ertKinematic);
	pGroundPlaneCollider->SetUseLocalGravity(true);
	pGroundPlaneCollider->SetEnabled(false);
	pGroundPlaneCollider->SetCollisionFilter(decCollisionFilter(layermask2));
	
	// foot collider
	/*
	pFootCollider = colmgr.CreateColliderVolume();
	pFootCollider->SetResponseType(deCollider::ertKinematic);
	pFootCollider->SetUseLocalGravity(true);
	pFootCollider->SetEnabled(false);
	pFootCollider->GetLayerMask() = layermask2;
	pFootCollider->NotifyLayerMaskChanged();
	*/
	// update the colliders
	UpdateCollider();
}

void aeAnimatorLocomotion::pCreateTouchSensor(){
	// create the touch sensor
	pTouchSensor = pAnimator->GetEngine()->GetTouchSensorManager()->CreateTouchSensor();
	
	// setup some important parameters
	
	// set the mask
	decLayerMask layerMask;
	//layerMask.SetBit( aeAnimator::eclTerrain );
	layerMask.SetBit(aeAnimator::eclGround);
	
	pTouchSensor->SetCollisionFilter(decCollisionFilter(layerMask, layerMask));
	
	// register ourself as delegee of this touch sensor
	pAnimator->GetEnvironment()->SetTouchSensorDelegee(pTouchSensor, &pTouchSensorListener);
	
	// add the touch sensor to the world
	pAnimator->GetEngineWorld()->AddTouchSensor(pTouchSensor);
	
	// update the touch sensor
	UpdateTouchSensors();
	UpdateTSColors();
}



void aeAnimatorLocomotion::pUpdateLooking(float elapsed){
	pLookLeftRight.Update(elapsed);
	pLookUpDown.Update(elapsed);
}

void aeAnimatorLocomotion::pUpdateTurnLeftRight(float elapsed){
	switch(pLocomotionType){
	case eltNatural:
	case eltFPS:
		break;
		
	case eltVehicle:
		if(pKeyStepLeft){
			pTurnLeftRight += 45.0f * elapsed;
		}
		if(pKeyStepRight){
			pTurnLeftRight -= 45.0f * elapsed;
		}
		break;
	}
}

void aeAnimatorLocomotion::pUpdateIsMoving(){
	bool isMoving = false;
	
	// process input
	isMoving = pKeyForward || pKeyBackward;
	if(pLocomotionType != eltVehicle){
		isMoving |= pKeyStepLeft || pKeyStepRight;
	}
	
	// apply to locomotion
	if(isMoving != pIsMoving){
		if(isMoving){
			pIsTurningIP = false;
			pTurnIP = 0.0f;
			
		}else{
			pAnimator->ResetControllersWith(eaTimeTurnIP);
		}
		
		pIsMoving = isMoving;
	}
}

void aeAnimatorLocomotion::pUpdateOrientation(float elapsed){
	float adjustOrientation = 0.0f;
	
	if(pIsMoving){
		pUpdateOrientationMoving(elapsed, adjustOrientation);
		
	}else{
		pUpdateOrientationNotMoving(elapsed, adjustOrientation);
	}
	
	pCheckLookingRangeViolation(adjustOrientation);
	
	//pOrientation.SetChangeSpeed( adjustOrientation / elapsed ); // problem: adjust change too small if adjusted
	// setChangeSpeed( getChangeSpeed + ( adjustOrientation - aoBefore ) / elapsed ); // clamp to avoid too large value
	
	SetOrientation(pOrientation.GetValue() + adjustOrientation);
	
	pLookLeftRight.SetValue(pLookLeftRight.GetValue() - adjustOrientation);
	if(pLocomotionType != eltVehicle){
		pLookLeftRight.SetGoal(pLookLeftRight.GetGoal() - adjustOrientation);
	}
	
	if(adjustOrientation > 0.0f){
		pTurnLeftRight = decMath::max(pTurnLeftRight - adjustOrientation, 0.0f);
		
	}else{
		pTurnLeftRight = decMath::min(pTurnLeftRight - adjustOrientation, 0.0f);
	}
	
	SetTurnVelocity(adjustOrientation / elapsed);
}

void aeAnimatorLocomotion::pUpdateOrientationMoving(float elapsed, float &adjustOrientation){
	// adjust value for no smoothing. this is locomotion type dependent
	switch(pLocomotionType){
	case eltNatural:
		adjustOrientation = pTurnLeftRight + pLookLeftRight.GetGoal() + pCalcMovingDirectionNatural();
		break;
		
	case eltFPS:
		adjustOrientation = pTurnLeftRight + pLookLeftRight.GetGoal();
		break;
		
	case eltVehicle:
		adjustOrientation = pTurnLeftRight;
		break;
	}
	
	// smooth value if required. this applies to all locomotion types
	const float oldOrientation = pOrientation.GetValue();
	pOrientation.SetGoal(oldOrientation + adjustOrientation);
	pOrientation.Update(elapsed);
	adjustOrientation = pOrientation.GetValue() - oldOrientation;
	pOrientation.SetValue(oldOrientation);
}

void aeAnimatorLocomotion::pUpdateOrientationNotMoving(float elapsed, float &adjustOrientation){
	// adjust orientation is the remaining turn left right
	adjustOrientation = pTurnLeftRight;
	
	const float oldOrientation = pOrientation.GetValue();
	pOrientation.SetGoal(oldOrientation + adjustOrientation);
	pOrientation.Update(elapsed);
	adjustOrientation = pOrientation.GetValue() - oldOrientation;
	pOrientation.SetValue(oldOrientation);
	
	// turn in place rotation if required
	if(pLocomotionType != eltVehicle){
		if(pLookLeftRight.GetGoal() > 70.0f){
			if(pIsTurningIP){
				if(pTurnIP < 0.0f){
					pTurnIP += 90.0f;
					pAnimator->InverseControllersWith(eaTimeTurnIP);
				}
				
			}else{
				pIsTurningIP = true;
				pTurnIP = 90.0f;
				pAnimator->ResetControllersWith(eaTimeTurnIP);
			}
			
		}else if(pLookLeftRight.GetGoal() < -70.0f){
			if(pIsTurningIP){
				if(pTurnIP > 0.0f){
					pTurnIP -= 90.0f;
					pAnimator->InverseControllersWith(eaTimeTurnIP);
				}
				
			}else{
				pIsTurningIP = true;
				pTurnIP = -90.0f;
				pAnimator->ResetControllersWith(eaTimeTurnIP);
			}
		}
		
		if(pIsTurningIP){
			if(pTurnIP > 0.0f){
				adjustOrientation = 90.0f * elapsed / pAdjustTimeTurnIP;
				
				if(adjustOrientation > pTurnIP){
					adjustOrientation = pTurnIP;
					pTurnIP = 0.0f;
					pIsTurningIP = false;
					
				}else{
					pTurnIP -= adjustOrientation;
				}
				
			}else{
				adjustOrientation = -90.0f * elapsed / pAdjustTimeTurnIP;
				
				if(adjustOrientation < pTurnIP){
					adjustOrientation = pTurnIP;
					pTurnIP = 0.0f;
					pIsTurningIP = false;
					
				}else{
					pTurnIP -= adjustOrientation;
				}
			}
		}
	}
}

float aeAnimatorLocomotion::pCalcMovingDirectionNatural() const{
	float movingDirection = 0.0f;
	
	if(pIsMoving){
		// process input
		if(pKeyStepLeft){
			if(pKeyForward || pKeyBackward){
				movingDirection = 45.0f;
				
			}else{
				movingDirection = 90.0f;
			}
			
		}else if(pKeyStepRight){
			if(pKeyForward || pKeyBackward){
				movingDirection = -45.0f;
				
			}else{
				movingDirection = -90.0f;
			}
		}
		
		if(pKeyBackward){
			movingDirection = -movingDirection;
		}
	}
	
	return movingDirection;
}

void aeAnimatorLocomotion::pCheckLookingRangeViolation(float &adjustOrientation){
	bool canTurn = true;
	
	if(pLocomotionType == eltVehicle){
		canTurn = false;
	}
	
	// if we can turn the body keep the looking always inside the limits.
	// if leaving adjust the adjustOrientation to satisfy the limits again.
	if(canTurn){
		if(pLookLeftRight.GetGoal() - adjustOrientation > 90.0f){
			adjustOrientation = pLookLeftRight.GetGoal() - 90.0f;
			
		}else if(pLookLeftRight.GetGoal() - adjustOrientation < -90.0f){
			adjustOrientation = pLookLeftRight.GetGoal() + 90.0f;
		}
		
	// if we can not turn clamp the looking to the limits
	}else{
		if(pLookLeftRight.GetValue() - adjustOrientation > pLimitLookRight){
			pLookLeftRight.SetValue(pLimitLookRight);
			pLookLeftRight.SetGoal(pLimitLookRight);
			
		}else if(pLookLeftRight.GetValue() - adjustOrientation < pLimitLookLeft){
			pLookLeftRight.SetValue(pLimitLookLeft);
			pLookLeftRight.SetGoal(pLimitLookLeft);
		}
	}
}

void aeAnimatorLocomotion::pUpdateLinearVelocity(float elapsed){
	// moving speed
	float speed = 0.0f;
	
	if(pIsMoving){
		if(pToggleRun){
			speed = pSpeedRun;
			
		}else{
			speed = pSpeedWalk;
		}
	}
	
	// linear velocity
	if(pIsMoving){
		float orientation = pOrientation.GetValue();
		
		switch(pLocomotionType){
		case eltNatural:
		case eltFPS:
			orientation += pLookLeftRight.GetValue();
			if(pKeyForward){
				if(pKeyStepLeft){
					orientation += 45.0f;
					
				}else if(pKeyStepRight){
					orientation -= 45.0f;
				}
				
			}else if(pKeyBackward){
				if(pKeyStepLeft){
					orientation += 135.0f;
					
				}else if(pKeyStepRight){
					orientation -= 135.0f;
					
				}else{
					orientation += 180.0f;
				}
				
			}else if(pKeyStepLeft){
				orientation += 90.0f;
				
			}else if(pKeyStepRight){
				orientation -= 90.0f;
			}
			break;
			
		case eltVehicle:
			if(pKeyBackward){
				orientation += 180.0f;
			}
			break;
		}
		
		SetVelocityOrientation(orientation);
	}
	
	pLinearVelocity.SetGoal(decMatrix::CreateRotationY(pVelocityOrientation * DEG2RAD).TransformView() * speed);
	pLinearVelocity.Update(elapsed);
	
	const decVector &linearVelocity = pLinearVelocity.GetValue();
	pMovingSpeed = linearVelocity.Length();
	pRelativeMovingSpeed = pMovingSpeed;
	
	if(pMovingSpeed > 0.001f){ // otherwise undefined orientation
		SetVelocityOrientation(-atan2f(linearVelocity.x, linearVelocity.z) * RAD2DEG);
	}
	
	SetMovingDirection(pVelocityOrientation - pOrientation.GetValue());
	
	switch(pLocomotionType){
	case eltNatural:
	case eltVehicle:
		if(fabs(pMovingDirection) > 90.0f){
			pRelativeMovingSpeed = -pMovingSpeed;
		}
		SetMovingDirection(0.0f);
		break;
		
	case eltFPS:
		break;
	}
}

void aeAnimatorLocomotion::pUpdateStance(float elapsed){
	pStance.SetGoal(pToggleCrouch ? 1.0f : 0.0f);
	pStance.Update(elapsed);
}
