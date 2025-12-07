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

#include "reLSRig.h"
#include "reCreateRigShape.h"
#include "../rig/reRig.h"
#include "../rig/bone/reRigBone.h"
#include "../rig/constraint/reRigConstraint.h"
#include "../rig/constraint/reRigConstraintDof.h"
#include "../rig/shape/reRigShape.h"
#include "../gui/reWindowMain.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>

#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/rig/deRigBuilder.h>
#include <dragengine/resources/rig/deRigManager.h>
#include <dragengine/resources/rig/deRigConstraint.h>
#include <dragengine/systems/modules/rig/deBaseRigModule.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/deEngine.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/exceptions.h>



// Class reLSRig
//////////////////

// Constructor, destructor
////////////////////////////

reLSRig::reLSRig(deBaseRigModule *module) :
pModule(module)
{
	if(!module){
		DETHROW(deeInvalidParam);
	}
	
	const deLoadableModule &loadableModule = module->GetLoadableModule();
	const decStringList &patternList = loadableModule.GetPatternList();
	const int patternCount = patternList.GetCount();
	int i;
	
	pName = loadableModule.GetName();
	for(i=0; i<patternCount; i++){
		if(i > 0){
			pPattern.AppendCharacter(',');
		}
		pPattern.AppendCharacter('*');
		pPattern.Append(patternList.GetAt(i));
	}
}

reLSRig::~reLSRig(){
}



// Management
///////////////

void reLSRig::SetName(const char *name){
	pName = name;
}

void reLSRig::SetPattern(const char *pattern){
	pPattern = pattern;
}



// Loading and saving
///////////////////////

class cDirtyHackRigBuilder : public deRigBuilder{
public:
	cDirtyHackRigBuilder(){}
	virtual ~cDirtyHackRigBuilder(){}
	virtual void BuildRig(deRig *rig){}
};

void reLSRig::LoadRig(reRig *rig, decBaseFileReader *file){
	if(!rig || !file) DETHROW(deeInvalidParam);
	deEngine *engine = pModule->GetGameEngine();
	reCreateRigShape createRigShape(engine);
	reRigConstraint *constraint = NULL;
	reRigBone *rigBone = NULL;
	int c, constraintCount;
	deRig *engRig = NULL;
	int s, shapeCount;
	int b, boneCount;
	int index;
	
	cDirtyHackRigBuilder builder;
	
	// protect this area
	try{
		// try to load the rig. we do not load it fully since we are only interested
		// in retrieving the content of the file. don't try this at home kids :D
		engRig = engine->GetRigManager()->CreateRig("<load-rig-1>", builder);
		
		pModule->LoadRig(*file, *engRig);
		
		// now it's time to copy the information over
		boneCount = engRig->GetBoneCount();
		for(b=0; b<boneCount; b++){
			deRigBone &engRigBone = engRig->GetBoneAt(b);
			
			// create a new rig bone to take over the values
			rigBone = new reRigBone(engine);
			
			// copy over the values
			rigBone->SetName(engRigBone.GetName());
			rigBone->SetPosition(engRigBone.GetPosition());
			rigBone->SetOrientation(engRigBone.GetRotation() * RAD2DEG);
			rigBone->SetCentralMassPoint(engRigBone.GetCentralMassPoint());
			rigBone->SetDynamic(engRigBone.GetDynamic());
			rigBone->SetMass(engRigBone.GetMass());
			rigBone->SetIKLimitsLower(engRigBone.GetIKLimitsLower() * RAD2DEG);
			rigBone->SetIKLimitsUpper(engRigBone.GetIKLimitsUpper() * RAD2DEG);
			rigBone->SetIKResistance(engRigBone.GetIKResistance());
			rigBone->SetIKLockedX(engRigBone.GetIKLockedX());
			rigBone->SetIKLockedY(engRigBone.GetIKLockedY());
			rigBone->SetIKLockedZ(engRigBone.GetIKLockedZ());
			
			// create shapes
			shapeCount = engRigBone.GetShapes().GetCount();
			for(s=0; s<shapeCount; s++){
				engRigBone.GetShapes().GetAt(s)->Visit(createRigShape);
				if(createRigShape.GetRigShape()){
					reRigShape * const rigShape = createRigShape.GetRigShape();
					rigShape->SetProperty(engRigBone.GetShapeProperties().GetAt(s));
					rigBone->AddShape(rigShape);
				}
			}
			
			// add the bone
			rig->AddBone(rigBone);
			rigBone->FreeReference(); // since we hold a reference to it
			rigBone = NULL;
		}
		
		// we can create constraints only after we created all bones since we require
		// linking between bones which can be out of the natural order
		for(b=0; b<boneCount; b++){
			deRigBone &engRigBone = engRig->GetBoneAt(b);
			constraintCount = engRigBone.GetConstraintCount();
			
			for(c=0; c<constraintCount; c++){
				deRigConstraint &engConstraint = engRigBone.GetConstraintAt(c);
				
				constraint = new reRigConstraint(engine);
				
				constraint->SetPosition(engConstraint.GetReferencePosition());
				constraint->SetOrientation(decMatrix::CreateFromQuaternion(engConstraint.GetReferenceOrientation()).GetEulerAngles() * RAD2DEG);
				constraint->SetOffset(engConstraint.GetBoneOffset());
				
				constraint->GetDofLinearX().SetFromEngineDof(engConstraint.GetDofLinearX());
				constraint->GetDofLinearY().SetFromEngineDof(engConstraint.GetDofLinearY());
				constraint->GetDofLinearZ().SetFromEngineDof(engConstraint.GetDofLinearZ());
				constraint->GetDofAngularX().SetFromEngineDof(engConstraint.GetDofAngularX());
				constraint->GetDofAngularY().SetFromEngineDof(engConstraint.GetDofAngularY());
				constraint->GetDofAngularZ().SetFromEngineDof(engConstraint.GetDofAngularZ());
				
				constraint->SetLinearDamping(engConstraint.GetLinearDamping());
				constraint->SetAngularDamping(engConstraint.GetAngularDamping());
				constraint->SetSpringDamping(engConstraint.GetSpringDamping());
				
				constraint->SetIsRope(engConstraint.GetIsRope());
				constraint->SetBreakingThreshold(engConstraint.GetBreakingThreshold());
				
				if(engConstraint.GetParentBone() != -1){
					constraint->SetConstraintBone(rig->GetBoneAt(engConstraint.GetParentBone()));
				}
				
				rig->GetBoneAt(b)->AddConstraint(constraint);
				constraint->FreeReference(); // since we hold a reference to it
				constraint = NULL;
			}
		}
		
		// create shapes
		shapeCount = engRig->GetShapes().GetCount();
		for(s=0; s<shapeCount; s++){
			engRig->GetShapes().GetAt(s)->Visit(createRigShape);
			if(createRigShape.GetRigShape()){
				reRigShape * const rigShape = createRigShape.GetRigShape();
				rigShape->SetProperty(engRig->GetShapeProperties().GetAt(s));
				rig->AddShape(rigShape);
			}
		}
		
		// parent linking has to be resolved now. the rigBone variable is not used anymore
		// since it is protected with the try-catch and would backfire on us if used.
		for(b=0; b<boneCount; b++){
			index = engRig->GetBoneAt(b).GetParent();
			if(index != -1){
				if(index < 0 || index >= boneCount){
					DETHROW(deeInvalidParam);
				}
				
				rig->GetBoneAt(b)->SetParentBone(rig->GetBoneAt(index));
			}
		}
		
		// store away the physics parameters of the rig
		if(engRig->GetRootBone() != -1){
			rig->SetRootBone(rig->GetBoneAt(engRig->GetRootBone()));
			
		}else{
			rig->SetRootBone(NULL);
		}
		
		// time to release the rig resource
		engRig->FreeReference();
		
	}catch(const deException &){
		//e.PrintError();
		if(constraint) constraint->FreeReference();
		if(rigBone) rigBone->FreeReference();
		if(engRig) engRig->FreeReference();
		throw;
	}
}

void reLSRig::SaveRig(reRig *rig, decBaseFileWriter *file){
	if(!rig || !file) DETHROW(deeInvalidParam);
	
	// this is now another little hack. usually we would have to build a temporary
	// rig to save but the rig is already existing if nothing is wrong. we rebuild
	// the rig if not done already
	rig->Rebuild();
	
	// if nothing went wrong we have now a rig to save
	if(!rig->GetEngineRig()) DETHROW(deeInvalidParam);
	
	pModule->SaveRig(*file, *rig->GetEngineRig());
}
