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
#include "../../../rig/shape/reRigShapeSphere.h"
#include "../../../rig/shape/reRigShapeBox.h"
#include "../../../rig/shape/reRigShapeCylinder.h"
#include "../../../rig/shape/reRigShapeCapsule.h"
#include "../../../rig/constraint/reRigConstraint.h"
#include "../../../rig/constraint/reRigConstraintDof.h"

#include <dragengine/common/exceptions.h>



// Class reUBoneMirror
////////////////////////

// Constructor, destructor
////////////////////////////

reUBoneMirror::reUBoneMirror(reRig *rig){
	
	SetShortInfo("Mirror Bones");
	
	// create bones array
	rig->GetSelectionBones()->GetBones().Visit([&](reRigBone *bone){
		const cBone::Ref ubone(cBone::Ref::New());
		ubone->boneSource = bone;
		pBones.Add(ubone);
	});
	
	// search for the mirrored bone and store the parameters if found
	pBones.Visit([&](cBone &bone){
		reRigBone * const target = pGetBoneWithMirroredName(rig, bone.boneSource);
		
		if(target){
			bone.boneTarget = target;
			
			bone.oldCMP = target->GetCentralMassPoint();
			bone.oldMass = target->GetMass();
			bone.oldDynamic = target->GetDynamic();
			bone.oldShapes = target->GetShapes();
			bone.oldConstraints = target->GetConstraints();
			bone.oldIKLimitsLower = target->GetIKLimitsLower();
			bone.oldIKLimitsUpper = target->GetIKLimitsUpper();
			bone.oldIKResistance = target->GetIKResistance();
			bone.oldIKLocked[0] = target->GetIKLockedX();
			bone.oldIKLocked[1] = target->GetIKLockedY();
			bone.oldIKLocked[2] = target->GetIKLockedZ();
		}
	});
	
	pRig = rig;
}

reUBoneMirror::~reUBoneMirror(){
}



// Undo and Redo operations
/////////////////////////////

void reUBoneMirror::Undo(){
	pBones.Visit([&](cBone &bone){
		if(!bone.boneTarget){
			return;
		}
		
		reRigBone &target = bone.boneTarget;
		target.SetCentralMassPoint(bone.oldCMP);
		target.SetMass(bone.oldMass);
		target.SetDynamic(bone.oldDynamic);
		
		target.RemoveAllShapes();
		bone.oldShapes.Visit([&](reRigShape *shape){
			target.AddShape(shape);
		});
		
		target.RemoveAllConstraints();
		bone.oldConstraints.Visit([&](reRigConstraint *constraint){
			target.AddConstraint(constraint);
		});
		
		target.SetIKLimitsLower(bone.oldIKLimitsLower);
		target.SetIKLimitsUpper(bone.oldIKLimitsUpper);
		target.SetIKResistance(bone.oldIKResistance);
		target.SetIKLockedX(bone.oldIKLocked[0]);
		target.SetIKLockedY(bone.oldIKLocked[1]);
		target.SetIKLockedZ(bone.oldIKLocked[2]);
	});
}

void reUBoneMirror::Redo(){
	float exchange;
	float lowerOrg;
	float upperOrg;
	float lowerNew;
	float upperNew;
	
	const decMatrix matrixMirrorScale(decMatrix::CreateScale(-1.0f, 1.0f, 1.0f));
	
	pBones.Visit([&](cBone &bone){
		if(!bone.boneTarget){
			return;
		}
		
		const reRigBone &source = *bone.boneSource;
		reRigBone &target = *bone.boneTarget;
		
		const decMatrix matrixMirror(source.GetMatrix() * matrixMirrorScale * target.GetInverseMatrix());
		
		// mirror bone properties
		target.SetCentralMassPoint(matrixMirror * source.GetCentralMassPoint());
		target.SetMass(source.GetMass());
		target.SetDynamic(source.GetDynamic());
		
		// mirror shapes. this is not correct yet and has to be fixed. the main problem
		// is that using mirroring the angles are incorrect in some situations.
		target.RemoveAllShapes();
		source.GetShapes().Visit([&](const reRigShape &s){
			const reRigShape::Ref shape(s.Duplicate());
			
			const decMatrix matrixResult(decMatrix::CreateRT(
				shape->GetOrientation() * DEG2RAD, shape->GetPosition()) * matrixMirror);
			
			shape->SetPosition(matrixResult.GetPosition());
			
			shape->SetOrientation(decMatrix::CreateVU(matrixResult.TransformView(),
				matrixResult.TransformUp()).GetEulerAngles() * RAD2DEG);
			
			target.AddShape(shape);
		});
		
		// mirror constraints
		target.RemoveAllConstraints();
		source.GetConstraints().Visit([&](reRigConstraint &c){
			const reRigConstraint::Ref constraint(c.Duplicate());
			
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
			target.AddConstraint(constraint);
		});
		
		// mirror IK limits which is just copy
		target.SetIKLimitsLower(source.GetIKLimitsLower());
		target.SetIKLimitsUpper(source.GetIKLimitsUpper());
		target.SetIKResistance(source.GetIKResistance());
		target.SetIKLockedX(source.GetIKLockedX());
		target.SetIKLockedY(source.GetIKLockedY());
		target.SetIKLockedZ(source.GetIKLockedZ());
	});
}



// Private Functions
//////////////////////

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
	
	return rig->GetBones().FindOrDefault([&](reRigBone *b){
		return b->GetName().GetLower() == findName;
	});
	
	return nullptr;
}
