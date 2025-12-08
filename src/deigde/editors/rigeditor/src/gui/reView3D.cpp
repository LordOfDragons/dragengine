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

#include "reView3D.h"
#include "reWindowMain.h"
#include "../configuration/reConfiguration.h"
#include "../collisions/reCLSelect.h"
#include "../collisions/reCLApplyPush.h"
#include "../collisions/reCLClosestElement.h"
#include "../rig/reRig.h"
#include "../rig/reCamera.h"
#include "../rig/bone/reRigBone.h"
#include "../rig/constraint/reTemporaryConstraint.h"
#include "../rig/shape/reRigShape.h"
#include "../rig/shape/reRigShapeList.h"
#include "../rig/shape/reSelectionShapes.h"
#include "../undosys/gui/shape/reUMoveShape.h"
#include "../undosys/gui/shape/reURotateShape.h"
#include "../undosys/gui/reBaseUndoScale.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/event/igdeMouseCameraListener.h>
#include <deigde/gui/event/igdeMouseDragListener.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/camera/deCameraManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/systems/modules/physics/deBasePhysicsWorld.h>



// Definitions
////////////////

#define RE_DRAG_MOVE	(1.0f / 50.0f)



namespace {

class cCameraInteraction : public igdeMouseCameraListener {
	reView3D &pView;
	
public:
	cCameraInteraction(reView3D &view) : pView(view){}
	
public:
	virtual igdeMouseCameraListener::eInteraction ChooseInteraction(){
		if(!pView.GetRig() || pView.GetRig()->GetCamera()->GetAttachToBone()){
			return eiNone;
		}
		return igdeMouseCameraListener::ChooseInteraction();
	}
	
	virtual void OnCameraChanged(){
		if(!pView.GetRig()){
			return;
		}
		
		reCamera &camera = *pView.GetRig()->GetCamera();
		camera.SetFreePosition(camera.GetPosition());
		camera.SetFreeOrientation(camera.GetOrientation());
		camera.SetFreeDistance(camera.GetDistance());
		
// 		pView.GetRig()->NotifyCameraChanged();
		pView.GetRig()->NotifyCameraViewChanged();
	}
};



class cBaseInteraction : public igdeMouseDragListener {
protected:
	reView3D &pView;
	
public:
	cBaseInteraction(reView3D &view) : pView(view){}
	
	decVector ViewDirection() const{
		if(!pView.GetRig()){
			return decVector(0.0f, 0.0f, 1.0f);
		}
		
		const decPoint &dragOrigin = GetDragPosition();
		const decPoint viewSize(pView.GetRenderAreaSize());
		return pView.GetRig()->GetCamera()->GetDirectionFor(
			viewSize.x, viewSize.y, dragOrigin.x, dragOrigin.y);
	}
	
	void TestCollisionWith(deBaseScriptingCollider &listener, const decDVector &position,
	const decVector &direction, decLayerMask &layerMask){
		if(!pView.GetRig()){
			return;
		}
		
		deBasePhysicsWorld * const phyWorld = pView.GetRig()->GetEngineWorld()->GetPeerPhysics();
		if(!phyWorld){
			return;
		}
		
		phyWorld->RayHits(position, direction, &listener, layerMask);
	}
	
	virtual bool OnDragBegin(){
		return GetDragState() == edsLeft && pView.GetRig() && pView.GetCanRender()
			&& OnDragBegin(*pView.GetRig());
	}
	
	virtual bool OnDragBegin(reRig &rig) = 0;
};



class cSimulationInteraction : public cBaseInteraction {
	reTemporaryConstraint *pConstraint;
	float pGrabDistance;
	
public:
	cSimulationInteraction(reView3D &view) : cBaseInteraction(view), pConstraint(NULL){
	}
	
	virtual ~cSimulationInteraction(){
		if(pConstraint){
			delete pConstraint;
		}
	}
	
	bool OnDragBegin(reRig &rig) override{
		if(!rig.GetSimulationRunning()){
			return false;
		}
		
		// no interaction while camera is attached to a bone as this would
		// cause serious problems with the physics
		reCamera &camera = *rig.GetCamera();
		if(camera.GetAttachToBone()){
			return false;
		}
		
		const decDVector position(camera.GetViewMatrix().GetPosition());
		const decVector view(ViewDirection());
		decLayerMask layerMask;
		
		const float rayLength = 200.0f;
		const float pushStrength = 100.0f; // velocity, make this a variable somehow
		
		// apply push if shift is pressed. no interaction starts in this case
		if(GetShiftOrigin()){
			reCLApplyPush applyPush(&rig);
			applyPush.SetCollider(rig.GetEngineSimulationCollider());
			applyPush.SetRay(position, view * rayLength);
			applyPush.SetPush(view * pushStrength);
			
			layerMask.SetBit(reRig::eclmSimulation);
			
			TestCollisionWith(applyPush, applyPush.GetRayPosition(),
				applyPush.GetRayDirection().ToVector(), layerMask);
			return false;
		}
		
		// apply regular interaction by grabbing object
		const decVector rayDirection(view * rayLength);
		reCLClosestElement closestElement(rig);
		
		closestElement.SetTestSimRig(true);
		closestElement.SetTestBones(false);
		closestElement.SetTestShapes(false);
		closestElement.SetTestConstraints(false);
		closestElement.SetTestPushes(false);
		
		layerMask.SetBit(reRig::eclmSimulation);
		
		TestCollisionWith(closestElement, position, rayDirection, layerMask);
		
		if(!closestElement.GetHasHit()){
			return false; // nothing to grab
		}
		
		// clicked on something we can grab
		if(pConstraint){
			delete pConstraint;
			pConstraint = NULL;
		}
		
		pGrabDistance = rayLength * closestElement.GetHitDistance();
		pConstraint = new reTemporaryConstraint(&rig, closestElement.GetHitSimBone(),
			position + decDVector(view * pGrabDistance), decQuaternion());
		return true;
	}
	
	virtual void OnDragUpdate(){
		if(!pConstraint){
			return;
		}
		
		pConstraint->SetPosition(pView.GetRig()->GetCamera()->GetViewMatrix().GetPosition()
			+ decDVector(ViewDirection() * pGrabDistance));
	}
	
	virtual void OnDragFinish(bool cancelled){
		if(pConstraint){
			delete pConstraint;
			pConstraint = NULL;
		}
	}
};



class cSelectInteraction : public cBaseInteraction {
public:
	cSelectInteraction(reView3D &view) : cBaseInteraction(view){}
	
	bool OnDragBegin(reRig &rig) override{
		if(rig.GetSimulationRunning() || rig.GetWorkMode() != reRig::ewmSelect){
			return false;
		}
		
		reCLSelect clSelect(&rig);
		clSelect.SetToggleSelection(GetShiftOrigin());
		
		decLayerMask layerMask;
		
		switch(rig.GetElementMode()){
		case reRig::eemBone:
			layerMask.SetBit(reRig::eclmBones);
			clSelect.SetCanSelectBones(true);
			break;
			
		case reRig::eemShape:
			layerMask.SetBit(reRig::eclmShapes);
			clSelect.SetCanSelectShapes(true);
			break;
			
		case reRig::eemConstraint:
			layerMask.SetBit(reRig::eclmConstraints);
			clSelect.SetCanSelectConstraints(true);
			break;
			
		default:
			layerMask.SetBit(reRig::eclmPushes);
			clSelect.SetCanSelectPushes(true);
		}
		
		clSelect.Reset();
		TestCollisionWith(clSelect, rig.GetCamera()->GetViewMatrix().GetPosition(),
			ViewDirection() * 50.0f, layerMask);
		clSelect.RunAction();
		return true;
	}
	
	virtual void OnDragUpdate(){
	}
	
	virtual void OnDragFinish(bool cancelled){
	}
};



class cMoveInteraction : public cBaseInteraction {
private:
	igdeUndo::Ref pUndo;
	decMatrix pViewMatrix;
	decMatrix pRotationMatrix;
	decMatrix pInvRotationMatrix;
	
public:
	cMoveInteraction(reView3D &view) : cBaseInteraction(view){}
	
	bool OnDragBegin(reRig &rig) override{
		if(rig.GetSimulationRunning() || rig.GetWorkMode() != reRig::ewmMove){
			return false;
		}
		
		pRotationMatrix.SetIdentity();
		pUndo = NULL;
		
		switch(rig.GetElementMode()){
		case reRig::eemBone:
			return false;
			
		case reRig::eemShape:{
			const reSelectionShapes &selection = *rig.GetSelectionShapes();
			const reRigShape * const activeShape = selection.GetActiveShape();
			
			reRigShapeList list;
			selection.AddVisibleShapesTo(list);
			if(list.GetShapeCount() == 0){
				return false;
			}
			
			pUndo.TakeOver(new reUMoveShape(list));
			
			if(activeShape){
				pRotationMatrix.SetRotation(activeShape->GetOrientation() * DEG2RAD);
				
				const reRigBone * const bone = activeShape->GetRigBone();
				if(bone){
					pRotationMatrix *= bone->GetPoseMatrix().GetRotationMatrix();
				}
			}
			}break;
			
		default:
			return false;
		}
		
		pInvRotationMatrix = pRotationMatrix.Invert();
		pViewMatrix = rig.GetCamera()->GetViewMatrix().GetRotationMatrix();
		return true;
	}
	
	virtual void OnDragUpdate(){
		if(!pUndo || !pView.GetRig()){
			return;
		}
		
		// determine new movement vector
		const reConfiguration &configuration = pView.GetWindowMain().GetConfiguration();
		float sensitivity = configuration.GetSensitivity();
		const decPoint distance(GetDragDistance());
		reRig &rig = *pView.GetRig();
		
		decVector vector(
			pViewMatrix.TransformRight() * (RE_DRAG_MOVE * (float)distance.x * sensitivity)
			+ pViewMatrix.TransformUp() * (RE_DRAG_MOVE * (float)distance.y * sensitivity));
		
		if(rig.GetUseLocal()){
			vector = pInvRotationMatrix * vector;
		}
		
		if(rig.GetLockAxisX()){
			vector.x = 0.0f;
		}
		if(rig.GetLockAxisY()){
			vector.y = 0.0f;
		}
		if(rig.GetLockAxisZ()){
			vector.z = 0.0f;
		}
		
		if(rig.GetUseLocal()){
			vector = pRotationMatrix * vector;
		}
		
		if(configuration.GetSnapToGrid() != GetShiftNow()){
			vector.Snap(configuration.GetGridSize());
		}
		
		// update undo object and redo the movement
		reUMoveShape &undo = (reUMoveShape&)(igdeUndo&)pUndo;
		undo.SetDistance(vector);
		undo.ProgressiveRedo();
	}
	
	virtual void OnDragFinish(bool cancelled){
		if(!pUndo){
			return;
		}
		
		if(cancelled || !pView.GetRig()){
			pUndo = NULL;
			return;
		}
		
		reUMoveShape &undo = (reUMoveShape&)(igdeUndo&)pUndo;
		if(undo.GetDistance().Length() > 1e-5f){
			pView.GetRig()->GetUndoSystem()->Add(pUndo);
		}
		pUndo = NULL;
	}
};



class cScaleInteraction : public cBaseInteraction {
public:
	cScaleInteraction(reView3D &view) : cBaseInteraction(view){}
	
	bool OnDragBegin(reRig &rig) override{
		if(rig.GetSimulationRunning() || rig.GetWorkMode() != reRig::ewmScale){
			return false;
		}
		
		// TODO
		
		return false;
	}
	
	virtual void OnDragUpdate(){
	}
	
	virtual void OnDragFinish(bool cancelled){
	}
};



class cRotateInteraction : public cBaseInteraction {
private:
	igdeUndo::Ref pUndo;
	decMatrix pRotationMatrix;
	decMatrix pInvRotationMatrix;
	float pNullAngle;
	
public:
	cRotateInteraction(reView3D &view) : cBaseInteraction(view), pNullAngle(0.0f){}
	
	bool OnDragBegin(reRig &rig) override{
		if(rig.GetSimulationRunning() || rig.GetWorkMode() != reRig::ewmRotate){
			return false;
		}
		
		const reSelectionShapes &selectionShapes = *rig.GetSelectionShapes();
		const reRigShape * const activeShape = selectionShapes.GetActiveShape();
		
		pUndo = NULL;
		
		// determine rotation axis
		pRotationMatrix.SetIdentity();
		
		switch(rig.GetElementMode()){
		case reRig::eemBone:
			break;
			
		case reRig::eemShape:
			if(activeShape){
				pRotationMatrix.SetRotation(activeShape->GetOrientation() * DEG2RAD);
				
				const reRigBone * bone = activeShape->GetRigBone();
				if(bone){
					pRotationMatrix *= bone->GetPoseMatrix().GetRotationMatrix();
				}
			}
			break;
			
		default:
			break;
		}
		
		pInvRotationMatrix = pRotationMatrix.Invert();
		
		decVector axis;
		
		if(rig.GetUseLocal()){
			axis = pInvRotationMatrix * ViewDirection();
			
		}else{
			axis = ViewDirection();
		}
		
		if(rig.GetLockAxisX()){
			axis.x = 0.0f;
		}
		if(rig.GetLockAxisY()){
			axis.y = 0.0f;
		}
		if(rig.GetLockAxisZ()){
			axis.z = 0.0f;
		}
		if(rig.GetUseLocal()){
			axis = pRotationMatrix * axis;
		}
		
		decDVector center;
		
		if(axis.Length() > 1e-5f){
			axis.Normalize();
			center.SetZero();
			
			switch(rig.GetElementMode()){
			case reRig::eemBone:
				break;
				
			case reRig::eemShape:{
				reRigShapeList list;
				selectionShapes.AddVisibleShapesTo(list);
				
				const int shapeCount = list.GetShapeCount();
				if(shapeCount == 0){
					return false;
				}
				
				center.SetZero();
				int i;
				for(i=0; i<shapeCount; i++){
					const reRigShape &shape = *list.GetShapeAt(i);
					const reRigBone * const bone = shape.GetRigBone();
					
					if(bone){
						center += bone->GetPoseMatrix() * shape.GetPosition();
						
					}else{
						center += shape.GetPosition();
					}
				}
				center /= (float)shapeCount;
				
				pUndo.TakeOver(new reURotateShape(list));
				((reURotateShape&)(igdeUndo&)pUndo).SetModifyPosition(shapeCount > 1);
				}break;
				
			default:
				break;
			}
		}
		
		if(!pUndo){
			return false;
		}
		
		const decPoint scrDir(GetDragOrigin() - (pView.GetRenderAreaSize() / 2));
		pNullAngle = atan2f((float)-scrDir.y, (float)scrDir.x);
		
		reURotateShape &undo = (reURotateShape&)(igdeUndo&)pUndo;
		undo.SetCenterPosition(center);
		undo.SetAxis(axis);
		
		return true;
	}
	
	virtual void OnDragUpdate(){
		if(!pUndo || !pView.GetRig()){
			return;
		}
		
		// determine new rotation
		const reConfiguration &configuration = pView.GetWindowMain().GetConfiguration();
		const decPoint scrDir(GetDragPosition() - (pView.GetRenderAreaSize() / 2));
		float angle = atan2f((float)-scrDir.y, (float)scrDir.x) - pNullAngle;
		
		if(GetShiftNow()){
			const float grid = configuration.GetRotSnapAngle() * DEG2RAD;
			angle = grid * ceilf(angle / grid - 0.5f);
		}
		
		// update undo object and redo the movement
		reURotateShape &undo = (reURotateShape&)(igdeUndo&)pUndo;
		undo.SetAngle(angle);
		undo.SetModifyPosition(!GetControlNow());
		undo.ProgressiveRedo();
	}
	
	virtual void OnDragFinish(bool cancelled){
		if(!pUndo){
			return;
		}
		
		if(cancelled || !pView.GetRig()){
			pUndo = NULL;
			return;
		}
		
		reURotateShape &undo = (reURotateShape&)(igdeUndo&)pUndo;
		if(undo.GetAngle() > 1e-5){
			pView.GetRig()->GetUndoSystem()->Add(pUndo);
		}
		pUndo = NULL;
	}
};

}



// Class reView3D
///////////////////

// Constructor, destructor
////////////////////////////

reView3D::reView3D(reWindowMain &windowMain) :
igdeViewRenderWindow(windowMain.GetEnvironment()),
pWindowMain(windowMain)
{
	pCameraInteraction.TakeOver(new cCameraInteraction(*this));
	pSimulationInteraction.TakeOver(new cSimulationInteraction(*this));
	pSelectInteraction.TakeOver(new cSelectInteraction(*this));
	pMoveInteraction.TakeOver(new cMoveInteraction(*this));
	pScaleInteraction.TakeOver(new cScaleInteraction(*this));
	pRotateInteraction.TakeOver(new cRotateInteraction(*this));
	
	AddListener(pCameraInteraction);
	AddListener(pSimulationInteraction);
	AddListener(pSelectInteraction);
	AddListener(pMoveInteraction);
	AddListener(pScaleInteraction);
	AddListener(pRotateInteraction);
}

reView3D::~reView3D(){
	SetRig(NULL);
}



// Management
///////////////

void reView3D::ResetView(){
}

void reView3D::SetRig(reRig *rig){
	if(rig == pRig){
		return;
	}
	
	pCameraInteraction->SetCamera(NULL);
	pSimulationInteraction->Cancel();
	pSelectInteraction->Cancel();
	pMoveInteraction->Cancel();
	pScaleInteraction->Cancel();
	pRotateInteraction->Cancel();
	
	SetRenderWorld(NULL);
	pRig = rig;
	
	if(rig){
		if(rig->GetCamera()){
			SetRenderWorld(rig->GetCamera()->GetEngineCamera());
			pCameraInteraction->SetCamera(rig->GetCamera());
		}
	}
}

void reView3D::OnFrameUpdate(float elapsed){
	igdeViewRenderWindow::OnFrameUpdate(elapsed);
	
	if(pRig){
		pRig->UpdateWorld(elapsed);
	}
}

void reView3D::CreateCanvas(){
	igdeViewRenderWindow::CreateCanvas();
	
	if(pRig){
		SetRenderWorld(pRig->GetCamera()->GetEngineCamera());
	}
}
