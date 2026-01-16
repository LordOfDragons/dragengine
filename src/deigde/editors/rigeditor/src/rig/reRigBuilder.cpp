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

#include "reRig.h"
#include "reRigBuilder.h"
#include "bone/reRigBone.h"
#include "shape/reRigShape.h"
#include "constraint/reRigConstraint.h"

#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/rig/deRigConstraint.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/exceptions.h>



// Class reRigBuilder
///////////////////////

// Constructor, destructor
////////////////////////////

reRigBuilder::reRigBuilder(reRig *rig){
	if(!rig){
		DETHROW(deeInvalidParam);
	}
	pRig = rig;
}

reRigBuilder::~reRigBuilder(){
}



// Management
///////////////

void reRigBuilder::BuildRig(deRig *engRig){
	if(!engRig) DETHROW(deeInvalidParam);
	decStringList shapeProperties;
	
	// TODO: constraints are wrong if the rig changes while a pose different
	// than the rest pose is used. most probably some incorrect bone position
	// is used to initialize the rig. all works well when the bones are in
	// rest position and then are posed for simulation. just changes while
	// in pose mode cranks up the build rig. One solution would be to put
	// all bones in rest mode for building the rig and then placing them
	// back in pose mode afterwards.
	pRig->GetBones().Visit([&](reRigBone &bone){
		bone.SetPoseFromRest();
	});
	
	// parameters
	engRig->SetCentralMassPoint(pRig->GetCentralMassPoint());
	engRig->SetModelCollision(pRig->GetModelCollision());
	
	// add bones. we have to respect the order of the bones to create
	// a valid engine rig.
	const int boneCount = pRig->GetBones().GetCount();
	int i;
	for(i=0; i<boneCount; i++){
		BuildRigBone(engRig, pRig->GetBones().FindOrDefault([&](const reRigBone &b){
			return b.GetOrder() == i;
		}));
	}
	
	// add shapes
	decShape::List shapes;
	pRig->GetShapes().Visit([&](reRigShape &rigShape){
		shapes.Add(rigShape.CreateShape());
		
		shapeProperties.Add(rigShape.GetProperty());
	});
	
	// root bone
	if(pRig->GetRootBone()){
		engRig->SetRootBone(pRig->GetRootBone()->GetOrder());
		
	}else{
		engRig->SetRootBone(-1);
	}
	
	engRig->SetShapes(shapes);
	engRig->SetShapeProperties(shapeProperties);
}

void reRigBuilder::BuildRigBone(deRig *engRig, reRigBone *rigBone){
	if(!engRig || !rigBone) DETHROW(deeInvalidParam);
	
	auto engBone = deRigBone::Ref::New(rigBone->GetName());
	decStringList shapeProperties;
	decShape::List shapes;
	
	try{
		// set options
		if(rigBone->GetParentBone()){
			engBone->SetParent(rigBone->GetParentBone()->GetOrder());
			
		}else{
			engBone->SetParent(-1);
		}
		
		engBone->SetPosition(rigBone->GetPosition());
		engBone->SetRotation(rigBone->GetOrientation() * DEG2RAD);
		engBone->SetCentralMassPoint(rigBone->GetCentralMassPoint());
		engBone->SetMatrices(rigBone->GetMatrix());
		engBone->SetDynamic(rigBone->GetDynamic());
		engBone->SetMass(rigBone->GetMass());
		
		// ik limits
		engBone->SetIKLimits(rigBone->GetIKLimitsLower() * DEG2RAD, rigBone->GetIKLimitsUpper() * DEG2RAD);
		engBone->SetIKResistance(rigBone->GetIKResistance());
		engBone->SetIKLockedX(rigBone->GetIKLockedX());
		engBone->SetIKLockedY(rigBone->GetIKLockedY());
		engBone->SetIKLockedZ(rigBone->GetIKLockedZ());
		
		// add shapes
		rigBone->GetShapes().Visit([&](reRigShape &rigShape){
			shapes.Add(rigShape.CreateShape());
			shapeProperties.Add(rigShape.GetProperty());
		});
		
		// add constraints
		rigBone->GetConstraints().Visit([&](reRigConstraint &rigConstraint){
			engBone->AddConstraint(rigConstraint.BuildEngineRigConstraint());
		});
		
	}catch(const deException &){
		engBone.Clear();
	}
	
	engBone->SetShapes(shapes);
	engBone->SetShapeProperties(shapeProperties);
	
	if(engBone){
		engRig->AddBone(std::move(engBone));
	}
}
