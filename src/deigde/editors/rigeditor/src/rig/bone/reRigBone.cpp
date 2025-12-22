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

#include "reRigBone.h"
#include "../reRig.h"
#include "../shape/reRigShape.h"
#include "../constraint/reRigConstraint.h"

#include <deigde/gui/igdeShapeBuilder.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWCoordSysArrows.h>

#include <dragengine/deEngine.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/exceptions.h>



// Class reRigBone
////////////////////

// Constructor, destructor
////////////////////////////

reRigBone::reRigBone(deEngine *engine) :
pIKLimitsLower(TWO_PI, TWO_PI, TWO_PI),
pIKLimitsUpper(0.0f, 0.0f, 0.0f),
pIKResistance(0.0f, 0.0f, 0.0f),
pIKLocked{false, false, false}
{
	DEASSERT_NOTNULL(engine)
	
	pEngine = engine;
	pRig = nullptr;
	pOrder = -1;
	
	pDebugDrawer = nullptr;
	pDDSBone = nullptr;
	pDDSCmp = nullptr;
	pDDSCoordSys = nullptr;
	pCollider = nullptr;
	
	pParentBone = nullptr;
	
	pDynamic = false;
	pMass = 1.0f;
	
	pDirtyMatrix = true;
	pVisited = false;
	
	pSelected = false;
	pActive = false;
	
	pName = "Bone";
	
	try{
		pCollider = engine->GetColliderManager()->CreateColliderVolume();
		pCollider->SetEnabled(true);
		pCollider->SetResponseType(deCollider::ertKinematic);
		pCollider->SetUseLocalGravity(true);
		
		decLayerMask layerMask;
		layerMask.SetBit(reRig::eclmBones);
		
		pCollider->SetCollisionFilter(decCollisionFilter(layerMask));
		
		// create debug drawer and shapes
		pDebugDrawer = engine->GetDebugDrawerManager()->CreateDebugDrawer();
		pDebugDrawer->SetXRay(true);
		
		pDDSBone = new igdeWDebugDrawerShape;
		pDDSBone->SetParentDebugDrawer(pDebugDrawer);
		
		pDDSCmp = new igdeWDebugDrawerShape;
		pDDSCmp->SetVisible(false);
		pDDSCmp->SetParentDebugDrawer(pDebugDrawer);
		
		pDDSCoordSys = new igdeWCoordSysArrows;
		pDDSCoordSys->SetVisible(false);
		pDDSCoordSys->SetParentDebugDrawer(pDebugDrawer);
		
		pUpdateDDSBoneShape();
		pUpdateDDSCmpShape();
		
		pUpdateDDSBoneColor();
		pUpdateDDSCmpColor();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

reRigBone::~reRigBone(){
	pCleanUp();
}



// Management
///////////////

void reRigBone::SetName(const char *name){
	pName = name;
	if(pRig){
		pRig->NotifyAllBoneChanged(this);
	}
}

void reRigBone::SetRig(reRig *rig){
	if(rig == pRig){
		return;
	}
	
	if(pRig){
		pRig->GetEngineWorld()->RemoveDebugDrawer(pDebugDrawer);
		pRig->GetEngineWorld()->RemoveCollider(pCollider);
	}
	
	pRig = rig;
	
	if(rig){
		rig->GetEngineWorld()->AddCollider(pCollider);
		rig->GetEngineWorld()->AddDebugDrawer(pDebugDrawer);
	}
	
	pShapes.Visit([&](reRigShape &s){
		s.SetRig(rig);
	});
	
	pConstraints.Visit([&](reRigConstraint &c){
		c.SetRig(rig);
	});
	
	pUpdateColliderShape();
}

void reRigBone::SetOrder(int order){
	pOrder = order;
}

void reRigBone::SetParentBone(reRigBone *bone){
	if(bone == pParentBone){
		return;
	}
	
	pParentBone = bone;
	
	if(pRig){
		pRig->UpdateBoneMatrices();
		
		pRig->NotifyAllBoneChanged(this);
		pRig->NotifyAllStructureChanged();
	}
	
	NotifyPoseChanged();
}

void reRigBone::SetPosition(const decVector &position){
	if(position.IsEqualTo(pPosition)){
		return;
	}
	
	pPosition = position;
	
	if(pRig){
		pRig->UpdateBoneMatrices();
		pRig->NotifyAllBoneChanged(this);
	}
	
	NotifyPoseChanged();
}

void reRigBone::SetOrientation(const decVector &orientation){
	if(orientation.IsEqualTo(pOrientation)){
		return;
	}
	
	pOrientation = orientation;
	
	if(pRig){
		pRig->UpdateBoneMatrices();
		pRig->NotifyAllBoneChanged(this);
	}
	
	NotifyPoseChanged();
}

void reRigBone::SetCentralMassPoint(const decVector &cmp){
	if(cmp.IsEqualTo(pCMP)){
		return;
	}
	
	pCMP = cmp;
	
	if(pRig){
		pRig->NotifyAllBoneChanged(this);
	}
	
	pDDSCmp->SetPosition(pCMP);
	pRepositionDDShapes();
	
	NotifyPoseChanged();
}



void reRigBone::UpdateMatrices(){
	if(pParentBone){
		pParentBone->UpdateMatrices();
	}
	
	if(pDirtyMatrix){
		decMatrix matrix = decMatrix::CreateRT(pOrientation * DEG2RAD, pPosition);
		if(pParentBone){
			matrix *= pParentBone->GetMatrix();
		}
		SetMatrices(matrix);
		pDirtyMatrix = false;
	}
}

void reRigBone::SetDirtyMatrix(bool dirty){
	pDirtyMatrix = dirty;
}

void reRigBone::SetMatrices(const decMatrix &matrix){
	pMatrix = matrix;
	pInverseMatrix = matrix.Invert();
	
	//pRepositionDDShapes(); // works on pose matrix so this has no effect
	pUpdateColliderShape();
}

void reRigBone::SetDynamic(bool dynamic){
	if(dynamic == pDynamic){
		return;
	}
	
	pDynamic = dynamic;
	
	if(pRig){
		pRig->NotifyAllBoneChanged(this);
	}
}

void reRigBone::SetMass(float mass){
	if(mass <= 0.0f){
		mass = 0.0f;
	}
	
	if(fabsf(mass - pMass) > FLOAT_SAFE_EPSILON){
		pMass = mass;
		
		if(pRig){
			pRig->NotifyAllBoneChanged(this);
		}
	}
}

void reRigBone::SetSelected(bool selected){
	if(selected == pSelected){
		return;
	}
	
	pSelected = selected;
	
	pDDSCoordSys->SetVisible(selected);
	
	pUpdateDDSCmpColor();
	pUpdateDDSBoneColor();
	ShowStateChanged();
}

void reRigBone::SetActive(bool active){
	if(active == pActive){
		return;
	}
	
	pActive = active;
	
	pUpdateDDSCmpColor();
	pUpdateDDSBoneColor();
}

void reRigBone::ShowStateChanged(){
	if(pRig){
		pDebugDrawer->SetVisible(pRig->GetShowBones());
	}
	
	pShapes.Visit([&](reRigShape &s){
		s.ShowStateChanged();
	});
	
	pConstraints.Visit([&](reRigConstraint &c){
		c.ShowStateChanged();
	});
}

void reRigBone::SetVisited(bool visited){
	pVisited = visited;
}



void reRigBone::SetPoseFromRest(){
	if(pRig){
		pPoseMatrix = decDMatrix(pMatrix) * pRig->GetPoseMatrix();
		
	}else{
		pPoseMatrix = decDMatrix(pMatrix);
	}
	
	pInversePoseMatrix = pPoseMatrix.Invert();
	NotifyPoseChanged();
	
	pRepositionDDShapes();
	pUpdateColliderShape();
}

void reRigBone::UpdateFromPose(){
	const decDMatrix oldPose = pPoseMatrix;
	deComponent *engComponent = nullptr;
	
	if(pRig){
		engComponent = pRig->GetEngineComponent();
	}
	
	if(engComponent && pOrder != -1){
		engComponent->PrepareBones();
		pPoseMatrix = decDMatrix(engComponent->GetBoneAt(pOrder).GetMatrix()) * pRig->GetPoseMatrix();
		
	}else if(pRig){
		pPoseMatrix = decDMatrix(pMatrix) * pRig->GetPoseMatrix();
		
	}else{
		pPoseMatrix = decDMatrix(pMatrix);
	}
	
	if(!pPoseMatrix.IsEqualTo(oldPose)){
		pInversePoseMatrix = pPoseMatrix.Invert();
		NotifyPoseChanged();
		
		pRepositionDDShapes();
		pUpdateColliderShape();
	}
}

void reRigBone::NotifyPoseChanged(){
	pShapes.Visit([&](reRigShape &s){
		s.InvalidatePosition();
	});
	
	pConstraints.Visit([&](reRigConstraint &c){
		c.InvalidatePositions();
	});
}

void reRigBone::UpdateShapesAndConstraints(){
	pShapes.Visit([&](reRigShape &s){
		s.Update();
	});
	
	pConstraints.Visit([&](reRigConstraint &c){
		c.Update();
	});
}



bool reRigBone::CanHaveParent(reRigBone *bone){
	// no parent is always good
	if(!bone){
		return true;
	}
	
	// ourself as parent is not allowed
	if(bone == this){
		return false;
	}
	
	// if there is no rig we can have no parent at all
	if(!pRig){
		return false;
	}
	
	// set all bones to not visited
	pRig->SetAllBonesVisited(false);
	
	// mark ourself visited
	pVisited = true;
	
	// visit all bones along the chain starting with the new bone
	while(bone){
		// if the bone is already marked we have a loop here
		if(bone->GetVisited()){
			return false;
		}
		
		// otherwise mark the bone and walk to its parent
		bone->SetVisited(true);
		bone = bone->GetParentBone();
	}
	
	// everything seems fine
	return true;
}



// Shapes
//////////

reRigShape *reRigBone::GetShapeWith(deColliderVolume *collider) const{
	DEASSERT_NOTNULL(collider)
	
	return pShapes.FindOrDefault([&](reRigShape *shape){
		return shape->GetCollider() == collider;
	});
}

void reRigBone::AddShape(reRigShape *shape){
	DEASSERT_NOTNULL(shape)
	
	pShapes.Add(shape);
	shape->SetRig(pRig);
	shape->SetRigBone(this);
	
	if(pRig){
		pRig->NotifyAllStructureChanged();
	}
}

void reRigBone::RemoveShape(reRigShape *shape){
	const reRigShape::Ref guard(shape);
	
	pShapes.Remove(shape);
	shape->SetRigBone(nullptr);
	shape->SetRig(nullptr);
	
	if(pRig){
		pRig->NotifyAllStructureChanged();
	}
}

void reRigBone::RemoveAllShapes(){
	pShapes.Visit([&](reRigShape &s){
		s.SetRigBone(nullptr);
		s.SetRig(nullptr);
	});
	pShapes.RemoveAll();
	
	if(pRig){
		pRig->NotifyAllStructureChanged();
	}
}



// Inverse kinematics
///////////////////////

void reRigBone::SetIKLimitsLower(const decVector &lower){
	if(lower.IsEqualTo(pIKLimitsLower)){
		return;
	}
	
	pIKLimitsLower = lower;
	
	if(pRig){
		pRig->NotifyAllBoneChanged(this);
	}
}

void reRigBone::SetIKLimitsUpper(const decVector &upper){
	if(upper.IsEqualTo(pIKLimitsUpper)){
		return;
	}
	
	pIKLimitsUpper = upper;
	
	if(pRig){
		pRig->NotifyAllBoneChanged(this);
	}
}

void reRigBone::SetIKResistance(const decVector &resistance){
	if(resistance.IsEqualTo(pIKResistance)){
		return;
	}
	
	pIKResistance = resistance;
	
	if(pRig){
		pRig->NotifyAllBoneChanged(this);
	}
}

void reRigBone::SetIKLockedX(bool locked){
	if(locked == pIKLocked[0]){
		return;
	}
	
	pIKLocked[0] = locked;
	
	if(pRig){
		pRig->NotifyAllBoneChanged(this);
	}
}

void reRigBone::SetIKLockedY(bool locked){
	if(locked == pIKLocked[1]){
		return;
	}
	
	pIKLocked[1] = locked;
	
	if(pRig){
		pRig->NotifyAllBoneChanged(this);
	}
}

void reRigBone::SetIKLockedZ(bool locked){
	if(locked == pIKLocked[2]){
		return;
	}
	
	pIKLocked[2] = locked;
	
	if(pRig){
		pRig->NotifyAllBoneChanged(this);
	}
}

void reRigBone::SetIKLocked(int axis, bool locked){
	if(locked == pIKLocked[axis]){
		return;
	}
	
	pIKLocked[axis] = locked;
	
	if(pRig){
		pRig->NotifyAllBoneChanged(this);
	}
}



// Constraints
////////////////

reRigConstraint *reRigBone::GetConstraintWith(deColliderVolume *collider) const{
	DEASSERT_NOTNULL(collider)
	
	return pConstraints.FindOrDefault([&](const reRigConstraint &c){
		return c.GetCollider() == collider;
	});
}

void reRigBone::AddConstraint(reRigConstraint *constraint){
	DEASSERT_FALSE(pConstraints.Has(constraint))
	
	pConstraints.Add(constraint);
	constraint->SetRig(pRig);
	constraint->SetRigBone(this);
	
	if(pRig){
		pRig->NotifyAllStructureChanged();
	}
}

void reRigBone::RemoveConstraint(reRigConstraint *constraint){
	DEASSERT_FALSE(pConstraints.Has(constraint))
	
	constraint->SetRigBone(nullptr);
	constraint->SetRig(nullptr);
	pConstraints.Remove(constraint);
	
	if(pRig){
		pRig->NotifyAllStructureChanged();
	}
}

void reRigBone::RemoveAllConstraints(){
	pConstraints.Visit([&](reRigConstraint &c){
		c.SetRigBone(nullptr);
		c.SetRig(nullptr);
	});
	pConstraints.RemoveAll();
	
	if(pRig){
		pRig->NotifyAllStructureChanged();
	}
}



// Private Functions
//////////////////////

void reRigBone::pCleanUp(){
	SetRig(nullptr);
	RemoveAllConstraints();
	RemoveAllShapes();
	
	if(pDDSCmp){
		delete pDDSCmp;
	}
	if(pDDSCoordSys){
		delete pDDSCoordSys;
	}
	if(pDDSBone){
		delete pDDSBone;
	}
}



void reRigBone::pUpdateDDSCmpShape(){
	igdeShapeBuilder builder;
	
	builder.CreateSphere(*pDDSCmp, decVector(), 0.01f);
}

void reRigBone::pUpdateDDSCmpColor(){
	pDDSCmp->SetVisible(pSelected);
	
	if(pActive){
		pDDSCmp->SetEdgeColor(decColor(1.0f, 0.5f, 0.0f, 1.0f));
		pDDSCmp->SetFillColor(decColor(1.0f, 0.5f, 0.0f, 0.1f));
		
	}else if(pSelected){
		pDDSCmp->SetEdgeColor(decColor(0.6f, 1.0f, 1.0f, 1.0f));
		pDDSCmp->SetFillColor(decColor(0.6f, 1.0f, 1.0f, 0.1f));
		
	}else{
		pDDSCmp->SetEdgeColor(decColor(0.3f, 0.7f, 1.0f, 1.0f));
		pDDSCmp->SetFillColor(decColor(0.3f, 0.7f, 1.0f, 0.1f));
	}
}

void reRigBone::pUpdateDDSBoneShape(){
	igdeShapeBuilder builder;
	
	builder.CreateArrow(*pDDSBone, decVector(), decVector(0.0, 0.0, 0.1f), 0.01f, 0.03f, 0.001f);
}

void reRigBone::pUpdateDDSBoneColor(){
	if(pActive){
		pDDSBone->SetEdgeColor(decColor(1.0f, 0.5f, 0.0f, 1.0f));
		pDDSBone->SetFillColor(decColor(1.0f, 0.5f, 0.0f, 0.1f));
		
	}else if(pSelected){
		pDDSBone->SetEdgeColor(decColor(0.6f, 1.0f, 1.0f, 1.0f));
		pDDSBone->SetFillColor(decColor(0.6f, 1.0f, 1.0f, 0.1f));
		
	}else{
		pDDSBone->SetEdgeColor(decColor(0.3f, 0.7f, 1.0f, 1.0f));
		pDDSBone->SetFillColor(decColor(0.3f, 0.7f, 1.0f, 0.1f));
	}
}

void reRigBone::pRepositionDDShapes(){
	pDebugDrawer->SetPosition(pPoseMatrix.GetPosition());
	pDebugDrawer->SetOrientation(pPoseMatrix.ToQuaternion());
}



void reRigBone::pUpdateColliderShape(){
	if(!pCollider){
		return;
	}
	
	decVector halfExtends(0.02f, 0.02f, 0.06f);
	decShapeBox *box = nullptr;
	decShapeList shapeList;
	
	pCollider->SetPosition(pPoseMatrix * decVector(0.0f, 0.0f, halfExtends.z));
	pCollider->SetOrientation(pPoseMatrix.ToQuaternion());
	
	try{
		box = new decShapeBox(halfExtends);
		shapeList.Add(box);
		
	}catch(const deException &){
		if(box){
			delete box;
		}
	}
	
	pCollider->SetShapes(shapeList);
}
