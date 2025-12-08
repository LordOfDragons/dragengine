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
	if(!engine){
		DETHROW(deeInvalidParam);
	}
	
	pEngine = engine;
	pRig = NULL;
	pOrder = -1;
	
	pDebugDrawer = NULL;
	pDDSBone = NULL;
	pDDSCmp = NULL;
	pDDSCoordSys = NULL;
	pCollider = NULL;
	
	pParentBone = NULL;
	
	pDynamic = false;
	pMass = 1.0f;
	
	pDirtyMatrix = true;
	pVisited = false;
	
	pSelected = false;
	pActive = false;
	
	pName = "Bone";
	
	try{
		pCollider.TakeOver(engine->GetColliderManager()->CreateColliderVolume());
		pCollider->SetEnabled(true);
		pCollider->SetResponseType(deCollider::ertKinematic);
		pCollider->SetUseLocalGravity(true);
		
		decLayerMask layerMask;
		layerMask.SetBit(reRig::eclmBones);
		
		pCollider->SetCollisionFilter(decCollisionFilter(layerMask));
		
		// create debug drawer and shapes
		pDebugDrawer.TakeOver(engine->GetDebugDrawerManager()->CreateDebugDrawer());
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
	if(pRig == rig){
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
	
	int i;
	const int shapeCount = pShapes.GetShapeCount();
	for(i=0; i<shapeCount; i++){
		pShapes.GetShapeAt(i)->SetRig(rig);
	}
	
	const int constraintCount = pConstraints.GetConstraintCount();
	for(i=0; i<constraintCount; i++){
		pConstraints.GetConstraintAt(i)->SetRig(rig);
	}
	
	pUpdateColliderShape();
}

void reRigBone::SetOrder(int order){
	pOrder = order;
}

void reRigBone::SetParentBone(reRigBone *bone){
	if(pParentBone == bone){
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
	if(pDynamic == dynamic){
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
	if(pSelected == selected){
		return;
	}
	
	pSelected = selected;
	
	pDDSCoordSys->SetVisible(selected);
	
	pUpdateDDSCmpColor();
	pUpdateDDSBoneColor();
	ShowStateChanged();
}

void reRigBone::SetActive(bool active){
	if(pActive == active){
		return;
	}
	
	pActive = active;
	
	pUpdateDDSCmpColor();
	pUpdateDDSBoneColor();
}

void reRigBone::ShowStateChanged(){
	int i;
	
	if(pRig){
		pDebugDrawer->SetVisible(pRig->GetShowBones());
	}
	
	const int shapeCount = pShapes.GetShapeCount();
	for(i=0; i<shapeCount; i++){
		pShapes.GetShapeAt(i)->ShowStateChanged();
	}
	
	const int constraintCount = pConstraints.GetConstraintCount();
	for(i=0; i<constraintCount; i++){
		pConstraints.GetConstraintAt(i)->ShowStateChanged();
	}
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
	deComponent *engComponent = NULL;
	
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
	int i;
	
	const int shapeCount = pShapes.GetShapeCount();
	for(i=0; i<shapeCount; i++){
		pShapes.GetShapeAt(i)->InvalidatePosition();
	}
	
	const int constraintCount = pConstraints.GetConstraintCount();
	for(i=0; i<constraintCount; i++){
		pConstraints.GetConstraintAt(i)->InvalidatePositions();
	}
}

void reRigBone::UpdateShapesAndConstraints(){
	int i;
	
	const int shapeCount = pShapes.GetShapeCount();
	for(i=0; i<shapeCount; i++){
		pShapes.GetShapeAt(i)->Update();
	}
	
	const int constraintCount = pConstraints.GetConstraintCount();
	for(i=0; i<constraintCount; i++){
		pConstraints.GetConstraintAt(i)->Update();
	}
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

int reRigBone::GetShapeCount() const{
	return pShapes.GetShapeCount();
}

reRigShape *reRigBone::GetShapeAt(int index) const{
	return pShapes.GetShapeAt(index);
}

reRigShape *reRigBone::GetShapeWith(deColliderVolume *collider) const{
	if(!collider){
		DETHROW(deeInvalidParam);
	}
	
	const int shapeCount = pShapes.GetShapeCount();
	int i;
	
	for(i=0; i<shapeCount; i++){
		reRigShape * const shape = pShapes.GetShapeAt(i);
		
		if(collider == shape->GetCollider()){
			return shape;
		}
	}
	
	return NULL;
}

int reRigBone::IndexOfShape(reRigShape *shape) const{
	return pShapes.IndexOfShape(shape);
}

bool reRigBone::HasShape(reRigShape *shape) const{
	return pShapes.HasShape(shape);
}

void reRigBone::AddShape(reRigShape *shape){
	if(!shape || pShapes.HasShape(shape)){
		DETHROW(deeInvalidParam);
	}
	
	pShapes.AddShape(shape);
	shape->SetRig(pRig);
	shape->SetRigBone(this);
	
	if(pRig){
		pRig->NotifyAllStructureChanged();
	}
}

void reRigBone::RemoveShape(reRigShape *shape){
	if(!pShapes.HasShape(shape)){
		DETHROW(deeInvalidParam);
	}
	
	shape->SetRigBone(NULL);
	shape->SetRig(NULL);
	pShapes.RemoveShape(shape);
	
	if(pRig){
		pRig->NotifyAllStructureChanged();
	}
}

void reRigBone::RemoveAllShapes(){
	const int shapeCount = pShapes.GetShapeCount();
	int i;
	
	for(i=0; i<shapeCount; i++){
		reRigShape &shape = *pShapes.GetShapeAt(i);
		shape.SetRigBone(NULL);
		shape.SetRig(NULL);
	}
	pShapes.RemoveAllShapes();
	
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

int reRigBone::GetConstraintCount() const{
	return pConstraints.GetConstraintCount();
}

reRigConstraint *reRigBone::GetConstraintAt(int index) const{
	return pConstraints.GetConstraintAt(index);
}

reRigConstraint *reRigBone::GetConstraintWith(deColliderVolume *collider) const{
	if(!collider){
		DETHROW(deeInvalidParam);
	}
	
	const int constraintCount = pConstraints.GetConstraintCount();
	int i;
	
	for(i=0; i<constraintCount; i++){
		reRigConstraint * const constraint = pConstraints.GetConstraintAt(i);
		
		if(collider == constraint->GetCollider()){
			return constraint;
		}
	}
	
	return NULL;
}

int reRigBone::IndexOfConstraint(reRigConstraint *constraint) const{
	return pConstraints.IndexOfConstraint(constraint);
}

bool reRigBone::HasConstraint(reRigConstraint *constraint) const{
	return pConstraints.HasConstraint(constraint);
}

void reRigBone::AddConstraint(reRigConstraint *constraint){
	if(pConstraints.HasConstraint(constraint)){
		DETHROW(deeInvalidParam);
	}
	
	pConstraints.AddConstraint(constraint);
	constraint->SetRig(pRig);
	constraint->SetRigBone(this);
	
	if(pRig){
		pRig->NotifyAllStructureChanged();
	}
}

void reRigBone::RemoveConstraint(reRigConstraint *constraint){
	if(!pConstraints.HasConstraint(constraint)){
		DETHROW(deeInvalidParam);
	}
	
	constraint->SetRigBone(NULL);
	constraint->SetRig(NULL);
	pConstraints.RemoveConstraint(constraint);
	
	if(pRig){
		pRig->NotifyAllStructureChanged();
	}
}

void reRigBone::RemoveAllConstraints(){
	const int constraintCount = pConstraints.GetConstraintCount();
	int i;
	
	for(i=0; i<constraintCount; i++){
		reRigConstraint &constraint = *pConstraints.GetConstraintAt(i);
		constraint.SetRigBone(NULL);
		constraint.SetRig(NULL);
	}
	pConstraints.RemoveAllConstraints();
	
	if(pRig){
		pRig->NotifyAllStructureChanged();
	}
}



// Private Functions
//////////////////////

void reRigBone::pCleanUp(){
	SetRig(NULL);
	
	
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
	decShapeBox *box = NULL;
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
