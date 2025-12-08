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

#include "reUBoneMirror.h"
#include "../../../rig/reRig.h"
#include "../../../rig/bone/reRigBone.h"
#include "../../../rig/bone/reSelectionBones.h"
#include "../../../rig/shape/reRigShape.h"
#include "../../../rig/shape/reRigShapeList.h"
#include "../../../rig/shape/reRigShapeSphere.h"
#include "../../../rig/shape/reRigShapeBox.h"
#include "../../../rig/shape/reRigShapeCylinder.h"
#include "../../../rig/shape/reRigShapeCapsule.h"
#include "../../../rig/constraint/reRigConstraint.h"
#include "../../../rig/constraint/reRigConstraintList.h"
#include "../../../rig/constraint/reRigConstraintDof.h"

#include <dragengine/common/exceptions.h>

// Class reUBoneMirror
////////////////////////

// Constructor, destructor
////////////////////////////

reUBoneMirror::reUBoneMirror(reRig *rig){
	const reSelectionBones &selection = *rig->GetSelectionBones();
	int b, boneCount = selection.GetBoneCount();
	int c, constraintCount;
	reRigBone *boneTarget;
	int s, shapeCount;
	
	pRig = NULL;
	
	pBones = NULL;
	pBoneCount = 0;
	
	SetShortInfo("Mirror Bones");
	
	try{
		if(boneCount > 0){
			// create bones array
			pBones = new sBone[boneCount];
			if(!pBones) DETHROW(deeOutOfMemory);
			
			// clear all entries just setting the bone
			while(pBoneCount < boneCount){
				sBone &ubone = pBones[pBoneCount];
				
				ubone.boneSource = selection.GetBoneAt(pBoneCount);
				ubone.boneSource->AddReference();
				
				ubone.boneTarget = NULL;
				ubone.oldMass = 0.0f;
				ubone.oldDynamic = false;
				ubone.oldShapes = NULL;
				ubone.oldConstraints = NULL;
				
				pBoneCount++;
			}
			
			// search for the mirrored bone and store the parameters if found
			for(b=0; b<pBoneCount; b++){
				sBone &ubone = pBones[b];
				
				boneTarget = pGetBoneWithMirroredName(rig, ubone.boneSource);
				
				if(boneTarget){
					ubone.boneTarget = boneTarget;
					
					ubone.oldCMP = boneTarget->GetCentralMassPoint();
					ubone.oldMass = boneTarget->GetMass();
					ubone.oldDynamic = boneTarget->GetDynamic();
					
					ubone.oldShapes = new reRigShapeList;
					if(!ubone.oldShapes) DETHROW(deeOutOfMemory);
					
					shapeCount = boneTarget->GetShapeCount();
					for(s=0; s<shapeCount; s++){
						ubone.oldShapes->AddShape(boneTarget->GetShapeAt(s));
					}
					
					ubone.oldConstraints = new reRigConstraintList;
					if(!ubone.oldConstraints) DETHROW(deeOutOfMemory);
					
					constraintCount = boneTarget->GetConstraintCount();
					for(c=0; c<constraintCount; c++){
						ubone.oldConstraints->AddConstraint(boneTarget->GetConstraintAt(c));
					}
					
					ubone.oldIKLimitsLower = boneTarget->GetIKLimitsLower();
					ubone.oldIKLimitsUpper = boneTarget->GetIKLimitsUpper();
					ubone.oldIKResistance = boneTarget->GetIKResistance();
					ubone.oldIKLocked[0] = boneTarget->GetIKLockedX();
					ubone.oldIKLocked[1] = boneTarget->GetIKLockedY();
					ubone.oldIKLocked[2] = boneTarget->GetIKLockedZ();
				}
			}
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
	
	pRig = rig;
	rig->AddReference();
}

reUBoneMirror::~reUBoneMirror(){
	pCleanUp();
}

// Undo and Redo operations
/////////////////////////////

void reUBoneMirror::Undo(){
	int c, constraintCount;
	reRigBone *boneTarget;
	int s, shapeCount;
	int b;
	
	for(b=0; b<pBoneCount; b++){
		sBone &ubone = pBones[b];
		boneTarget = ubone.boneTarget;
		
		if(boneTarget){
			boneTarget->SetCentralMassPoint(ubone.oldCMP);
			boneTarget->SetMass(ubone.oldMass);
			boneTarget->SetDynamic(ubone.oldDynamic);
			
			boneTarget->RemoveAllShapes();
			const reRigShapeList &oldShapes = *ubone.oldShapes;
			shapeCount = oldShapes.GetShapeCount();
			for(s=0; s<shapeCount; s++){
				boneTarget->AddShape(oldShapes.GetShapeAt(s));
			}
			
			boneTarget->RemoveAllConstraints();
			const reRigConstraintList &oldConstraints = *ubone.oldConstraints;
			constraintCount = oldConstraints.GetConstraintCount();
			for(c=0; c<constraintCount; c++){
				boneTarget->AddConstraint(oldConstraints.GetConstraintAt(c));
			}
			
			boneTarget->SetIKLimitsLower(ubone.oldIKLimitsLower);
			boneTarget->SetIKLimitsUpper(ubone.oldIKLimitsUpper);
			boneTarget->SetIKResistance(ubone.oldIKResistance);
			boneTarget->SetIKLockedX(ubone.oldIKLocked[0]);
			boneTarget->SetIKLockedY(ubone.oldIKLocked[1]);
			boneTarget->SetIKLockedZ(ubone.oldIKLocked[2]);
		}
	}
}

void reUBoneMirror::Redo(){
	reRigConstraint *constraint = NULL;
	reRigShape *shape = NULL;
	int c, constraintCount;
	int s, shapeCount;
	float exchange;
	float lowerOrg;
	float upperOrg;
	float lowerNew;
	float upperNew;
	int b;
	
	try{
		const decMatrix matrixMirrorScale(decMatrix::CreateScale(-1.0f, 1.0f, 1.0f));
		
		for(b=0; b<pBoneCount; b++){
			const sBone &ubone = pBones[b];
			if(!ubone.boneTarget){
				continue;
			}
			
			const reRigBone &boneSource = *ubone.boneSource;
			reRigBone &boneTarget = *ubone.boneTarget;
			
			const decMatrix matrixMirror(boneSource.GetMatrix()
				* matrixMirrorScale * boneTarget.GetInverseMatrix() );
			
			// mirror bone properties
			boneTarget.SetCentralMassPoint(matrixMirror * boneSource.GetCentralMassPoint());
			boneTarget.SetMass(boneSource.GetMass());
			boneTarget.SetDynamic(boneSource.GetDynamic());
			
			// mirror shapes. this is not correct yet and has to be fixed. the main problem
			// is that using mirroring the angles are incorrect in some situations.
			boneTarget.RemoveAllShapes();
			shapeCount = boneSource.GetShapeCount();
			for(s=0; s<shapeCount; s++){
				shape = boneSource.GetShapeAt(s)->Duplicate();
				
				const decMatrix matrixResult(decMatrix::CreateRT(
					shape->GetOrientation() * DEG2RAD, shape->GetPosition()) * matrixMirror);
				
				shape->SetPosition(matrixResult.GetPosition());
				
				shape->SetOrientation(decMatrix::CreateVU(matrixResult.TransformView(),
					matrixResult.TransformUp()).GetEulerAngles() * RAD2DEG);
				
				boneTarget.AddShape(shape);
				shape->FreeReference();
				shape = NULL;
			}
			
			// mirror constraints
			boneTarget.RemoveAllConstraints();
			constraintCount = boneSource.GetConstraintCount();
			for(c=0; c<constraintCount; c++){
				constraint = boneSource.GetConstraintAt(c)->Duplicate();
				
				// constraint position and orientation
				decMatrix matrixResult(decMatrix::CreateRT(
					constraint->GetOrientation() * DEG2RAD, constraint->GetPosition()) * matrixMirror);
				
				constraint->SetPosition(matrixResult.GetPosition());
				
				constraint->SetOrientation(decMatrix::CreateVU(matrixResult.TransformView(),
					matrixResult.TransformUp()).GetEulerAngles() * RAD2DEG);
				
				// matrix to mirror limits
				matrixResult *= decMatrix::CreateRT(constraint->GetOrientation() * DEG2RAD,
					constraint->GetPosition()).Invert();
				
				// dof linear x
				lowerOrg = constraint->GetDofLinearX().GetLowerLimit();
				upperOrg = constraint->GetDofLinearX().GetUpperLimit();
				
				if(upperOrg - lowerOrg < -1e-5f){
					lowerNew = lowerOrg;
					upperNew = lowerOrg;
					
				}else{
					lowerNew = (matrixResult * decVector(lowerOrg, 0.0f, 0.0f)).x;
					upperNew = (matrixResult * decVector(upperOrg, 0.0f, 0.0f)).x;
					
					if(upperNew < lowerNew){
						exchange = lowerNew;
						lowerNew = upperNew;
						upperNew = exchange;
					}
				}
				
				constraint->GetDofLinearX().SetLowerLimit(lowerNew);
				constraint->GetDofLinearX().SetUpperLimit(upperNew);
				
				// dof linear y
				lowerOrg = constraint->GetDofLinearY().GetLowerLimit();
				upperOrg = constraint->GetDofLinearY().GetUpperLimit();
				
				if(upperOrg - lowerOrg < -1e-5f){
					lowerNew = lowerOrg;
					upperNew = lowerOrg;
					
				}else{
					lowerNew = (matrixResult * decVector(0.0f, lowerOrg, 0.0f)).x;
					upperNew = (matrixResult * decVector(0.0f, upperOrg, 0.0f)).x;
					
					if(upperNew < lowerNew){
						exchange = lowerNew;
						lowerNew = upperNew;
						upperNew = exchange;
					}
				}
				
				constraint->GetDofLinearY().SetLowerLimit(lowerNew);
				constraint->GetDofLinearY().SetUpperLimit(upperNew);
				
				// dof linear z
				lowerOrg = constraint->GetDofLinearZ().GetLowerLimit();
				upperOrg = constraint->GetDofLinearZ().GetUpperLimit();
				
				if(upperOrg - lowerOrg < -1e-5f){
					lowerNew = lowerOrg;
					upperNew = lowerOrg;
					
				}else{
					lowerNew = (matrixResult * decVector(0.0f, 0.0f, lowerOrg)).x;
					upperNew = (matrixResult * decVector(0.0f, 0.0f, upperOrg)).x;
					
					if(upperNew < lowerNew){
						exchange = lowerNew;
						lowerNew = upperNew;
						upperNew = exchange;
					}
				}
				
				constraint->GetDofLinearZ().SetLowerLimit(lowerNew);
				constraint->GetDofLinearZ().SetUpperLimit(upperNew);
				
				// dof angular x
				lowerOrg = constraint->GetDofAngularX().GetLowerLimit();
				upperOrg = constraint->GetDofAngularX().GetUpperLimit();
				
				if(upperOrg - lowerOrg < -1e-5f){
					lowerNew = lowerOrg;
					upperNew = lowerOrg;
					
				}else{
					lowerNew = (decMatrix::CreateRotationX(lowerOrg * DEG2RAD) * matrixResult).GetEulerAngles().x * RAD2DEG;
					upperNew = (decMatrix::CreateRotationX(upperOrg * DEG2RAD) * matrixResult).GetEulerAngles().x * RAD2DEG;
					
					if(upperNew < lowerNew){
						exchange = lowerNew;
						lowerNew = upperNew;
						upperNew = exchange;
					}
				}
				
				constraint->GetDofAngularX().SetLowerLimit(lowerNew);
				constraint->GetDofAngularX().SetUpperLimit(upperNew);
				
				// dof angular y
				lowerOrg = constraint->GetDofAngularY().GetLowerLimit();
				upperOrg = constraint->GetDofAngularY().GetUpperLimit();
				
				if(upperOrg - lowerOrg < -1e-5f){
					lowerNew = lowerOrg;
					upperNew = lowerOrg;
					
				}else{
					lowerNew = (decMatrix::CreateRotationY(lowerOrg * DEG2RAD) * matrixResult).GetEulerAngles().y * RAD2DEG;
					upperNew = (decMatrix::CreateRotationY(upperOrg * DEG2RAD) * matrixResult).GetEulerAngles().y * RAD2DEG;
					
					if(upperNew < lowerNew){
						exchange = lowerNew;
						lowerNew = upperNew;
						upperNew = exchange;
					}
				}
				
				constraint->GetDofAngularY().SetLowerLimit(lowerNew);
				constraint->GetDofAngularY().SetUpperLimit(upperNew);
				
				// dof angular z
				lowerOrg = constraint->GetDofAngularZ().GetLowerLimit();
				upperOrg = constraint->GetDofAngularZ().GetUpperLimit();
				
				if(upperOrg - lowerOrg < -1e-5f){
					lowerNew = lowerOrg;
					upperNew = lowerOrg;
					
				}else{
					lowerNew = (decMatrix::CreateRotationZ(lowerOrg * DEG2RAD) * matrixResult).GetEulerAngles().z * RAD2DEG;
					upperNew = (decMatrix::CreateRotationZ(upperOrg * DEG2RAD) * matrixResult).GetEulerAngles().z * RAD2DEG;
					
					if(upperNew < lowerNew){
						exchange = lowerNew;
						lowerNew = upperNew;
						upperNew = exchange;
					}
				}
				
				constraint->GetDofAngularZ().SetLowerLimit(lowerNew);
				constraint->GetDofAngularZ().SetUpperLimit(upperNew);
				
				// mirror constraint bone
				reRigBone * const boneMirrored = pGetBoneWithMirroredName(pRig, constraint->GetConstraintBone());
				if(boneMirrored){
					constraint->SetConstraintBone(boneMirrored);
				}
				
				// add the mirrored constraint
				boneTarget.AddConstraint(constraint);
				constraint->FreeReference();
				constraint = NULL;
			}
			
			// mirror IK limits which is just copy
			boneTarget.SetIKLimitsLower(boneSource.GetIKLimitsLower());
			boneTarget.SetIKLimitsUpper(boneSource.GetIKLimitsUpper());
			boneTarget.SetIKResistance(boneSource.GetIKResistance());
			boneTarget.SetIKLockedX(boneSource.GetIKLockedX());
			boneTarget.SetIKLockedY(boneSource.GetIKLockedY());
			boneTarget.SetIKLockedZ(boneSource.GetIKLockedZ());
		}
		
	}catch(const deException &){
		if(shape) shape->FreeReference();
		if(constraint) constraint->FreeReference();
		throw;
	}
}

// Private Functions
//////////////////////

void reUBoneMirror::pCleanUp(){
	if(pBones){
		while(pBoneCount > 0){
			pBoneCount--;
			sBone &ubone = pBones[pBoneCount];
			
			if(ubone.oldConstraints) delete ubone.oldConstraints;
			if(ubone.oldShapes) delete ubone.oldShapes;
			if(ubone.boneTarget) ubone.boneTarget->FreeReference();
			if(ubone.boneSource) ubone.boneSource->FreeReference();
		}
		
		delete [] pBones;
	}
	

}

reRigBone *reUBoneMirror::pGetBoneWithMirroredName(reRig *rig, reRigBone *bone) const{
	const decString nameBone(bone->GetName().GetLower());
	decString findName;
	
	if(nameBone.EndsWith(".right")){
		findName = nameBone.GetMiddle(0, -6) + ".left";
		
	}else if(nameBone.EndsWith(".left")){
		findName = nameBone.GetMiddle(0, -2) + ".right";
		
	}else if(nameBone.EndsWith(".r")){
		findName = nameBone.GetMiddle(0, -2) + ".l";
		
	}else if(nameBone.EndsWith(".l")){
		findName = nameBone.GetMiddle(0, -2) + ".r";
		
	}else if(nameBone.FindString("right") != -1){
		findName = nameBone.GetReplacedString("right", "left");
		
	}else if(nameBone.FindString("left") != -1){
		findName = nameBone.GetReplacedString("left", "right");
		
	}else{
		return nullptr;
	}
	
	const int count = rig->GetBoneCount();
	int i;
	
	for(i=0; i<count; i++){
		reRigBone * const mirroredBone = rig->GetBoneAt(i);
		if(mirroredBone->GetName().GetLower() == findName){
			return mirroredBone;
		}
	}
	
	return nullptr;
}
