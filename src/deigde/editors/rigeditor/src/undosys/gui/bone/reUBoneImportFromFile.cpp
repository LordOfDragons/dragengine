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

#include "reUBoneImportFromFile.h"
#include "../../../rig/reRig.h"
#include "../../../rig/bone/reRigBone.h"
#include "../../../rig/bone/reSelectionBones.h"
#include "../../../rig/shape/reRigShape.h"
#include "../../../rig/shape/reRigShapeSphere.h"
#include "../../../rig/shape/reRigShapeBox.h"
#include "../../../rig/shape/reRigShapeCylinder.h"
#include "../../../rig/shape/reRigShapeCapsule.h"
#include "../../../rig/constraint/reRigConstraint.h"

#include <dragengine/common/exceptions.h>



// Class reUBoneImportFromFile
////////////////////////////////

// Constructor, destructor
////////////////////////////

reUBoneImportFromFile::reUBoneImportFromFile(reRig *rig, reRig *importedRig){
	if(!rig || !importedRig){
		DETHROW(deeInvalidParam);
	}
	
	pScale = 1.0f;
	pImportBoneProperties = true;
	pImportShapes = true;
	pImportConstraints = true;
	
	SetShortInfo("Import Bones");
	
	rig->GetSelectionBones()->GetBones().Visit([&](reRigBone *bone){
		reRigBone * const importBone = importedRig->GetBoneNamed(bone->GetName());
		
		if(importBone){
			const cBone::Ref ubone(cBone::Ref::New());
			
			ubone->bone = bone;
			ubone->importBone = importBone;
			
			ubone->oldCMP = bone->GetCentralMassPoint();
			ubone->oldMass = bone->GetMass();
			ubone->oldDynamic = bone->GetDynamic();
			ubone->oldIKLimitsLower = bone->GetIKLimitsLower();
			ubone->oldIKLimitsUpper = bone->GetIKLimitsUpper();
			ubone->oldIKResistance = bone->GetIKResistance();
			ubone->oldIKLocked[0] = bone->GetIKLockedX();
			ubone->oldIKLocked[1] = bone->GetIKLockedY();
			ubone->oldIKLocked[2] = bone->GetIKLockedZ();
			ubone->oldShapes = bone->GetShapes();
			ubone->oldConstraints = bone->GetConstraints();
			
			pBones.Add(ubone);
		}
	});
	
	pRig = rig;
}

reUBoneImportFromFile::~reUBoneImportFromFile(){
}



// Undo and Redo operations
/////////////////////////////

void reUBoneImportFromFile::SetScale(float scale){
	pScale = scale;
}

void reUBoneImportFromFile::SetImportBoneProperties(bool import){
	pImportBoneProperties = import;
}

void reUBoneImportFromFile::SetImportShapes(bool import){
	pImportShapes = import;
}

void reUBoneImportFromFile::SetImportConstraints(bool import){
	pImportConstraints = import;
}



void reUBoneImportFromFile::Undo(){
	pBones.Visit([&](const cBone &ubone){
		reRigBone &bone = ubone.bone;
		
		if(pImportBoneProperties){
			bone.SetCentralMassPoint(ubone.oldCMP);
			bone.SetMass(ubone.oldMass);
			bone.SetDynamic(ubone.oldDynamic);
			bone.SetIKLimitsLower(ubone.oldIKLimitsLower);
			bone.SetIKLimitsUpper(ubone.oldIKLimitsUpper);
			bone.SetIKResistance(ubone.oldIKResistance);
			bone.SetIKLockedX(ubone.oldIKLocked[0]);
			bone.SetIKLockedY(ubone.oldIKLocked[1]);
			bone.SetIKLockedZ(ubone.oldIKLocked[2]);
		}
		
		if(pImportShapes){
			bone.RemoveAllShapes();
			ubone.oldShapes.Visit([&](reRigShape *s){
				bone.AddShape(s);
			});
		}
		
		if(pImportConstraints){
			bone.RemoveAllConstraints();
			ubone.oldConstraints.Visit([&](reRigConstraint *c){
				bone.AddConstraint(c);
			});
		}
	});
}

void reUBoneImportFromFile::Redo(){
	pBones.Visit([&](const cBone &ubone){
		const reRigBone * const importBone = ubone.importBone;
		reRigBone &bone = ubone.bone;
		
		if(pImportBoneProperties){
			bone.SetCentralMassPoint(importBone->GetCentralMassPoint() * pScale);
			bone.SetMass(importBone->GetMass());
			bone.SetDynamic(importBone->GetDynamic());
			bone.SetIKLimitsLower(importBone->GetIKLimitsLower());
			bone.SetIKLimitsUpper(importBone->GetIKLimitsUpper());
			bone.SetIKResistance(importBone->GetIKResistance());
			bone.SetIKLockedX(importBone->GetIKLockedX());
			bone.SetIKLockedY(importBone->GetIKLockedY());
			bone.SetIKLockedZ(importBone->GetIKLockedZ());
		}
		
		if(pImportShapes){
			bone.RemoveAllShapes();
			importBone->GetShapes().Visit([&](const reRigShape &s){
				const reRigShape::Ref shape(s.Duplicate());
				shape->Scale(pScale);
				bone.AddShape(shape);
			});
		}
		
		if(pImportConstraints){
			bone.RemoveAllConstraints();
			importBone->GetConstraints().Visit([&](const reRigConstraint &c){
				const reRigConstraint::Ref constraint(c.Duplicate());
				constraint->Scale(pScale);
				
				if(constraint->GetConstraintBone()){
					constraint->SetConstraintBone(pRig->GetBoneNamed(constraint->GetConstraintBone()->GetName()));
				}
				
				bone.AddConstraint(constraint);
			});
		}
	});
}
