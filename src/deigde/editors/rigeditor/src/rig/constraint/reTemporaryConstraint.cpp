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

#include "reTemporaryConstraint.h"
#include "../reRig.h"
#include "../bone/reRigBone.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderConstraint.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentBone.h>

#include <dragengine/common/exceptions.h>



// Class reTemporaryConstraint
////////////////////////////////

// Constructor, destructor
////////////////////////////

reTemporaryConstraint::reTemporaryConstraint(reRig *rig, reRigBone *bone,
const decDVector &position, const decQuaternion &orientation){
	if(!rig) DETHROW(deeInvalidParam);
	
	decDMatrix matrix(decDMatrix::CreateWorld(position, orientation));
	deColliderComponent::Ref simcol = rig->GetEngineSimulationCollider();
	
	pPosition = position;
	pOrientation = orientation;
	
	pEngSimCollider = NULL;
	pEngConstraint = NULL;
	
	if(simcol){
		pEngConstraint = new deColliderConstraint;
		
		try{
			pEngConstraint->SetPosition2(position.ToVector());
			pEngConstraint->SetOrientation2(orientation);
			
			pEngConstraint->SetToBallJoint();
			
			if(bone){
				pEngConstraint->SetBone(bone->GetOrder());
				
				matrix *= decDMatrix(bone->GetInversePoseMatrix());
				
			}else{
				pEngConstraint->SetBone(-1);
				
				matrix *= decDMatrix(rig->GetPoseMatrix()).Invert();
			}
			
			pEngConstraint->SetPosition1(matrix.GetPosition().ToVector());
			pEngConstraint->SetOrientation1(matrix.ToQuaternion());
			
			simcol->AddConstraint(pEngConstraint);
			
		}catch(const deException &){
			if(pEngConstraint) delete pEngConstraint;
			throw;
		}
	}
	
	pEngSimCollider = simcol;
	if(simcol) simcol->AddReference();
}

reTemporaryConstraint::~reTemporaryConstraint(){
	if(pEngSimCollider){
		if(pEngConstraint){
			pEngSimCollider->RemoveConstraint(pEngConstraint);
		}
	}
}



// Management
///////////////

void reTemporaryConstraint::SetPosition(const decDVector &position){
	if(!position.IsEqualTo(pPosition)){
		pPosition = position;
		
		if(pEngConstraint){
			pEngConstraint->SetPosition2(pPosition.ToVector());
		}
		
		pNotifyConstraintChanged();
	}
}

void reTemporaryConstraint::SetOrientation(const decQuaternion &orientation){
	if(!orientation.IsEqualTo(pOrientation)){
		pOrientation = orientation;
		
		if(pEngConstraint){
			pEngConstraint->SetOrientation2(orientation);
		}
		
		pNotifyConstraintChanged();
	}
}



void reTemporaryConstraint::SetLinearLowerLimits(const decVector &lowerLimits){
	if(!lowerLimits.IsEqualTo(pLinearLowerLimits)){
		pLinearLowerLimits = lowerLimits;
		
		if(pEngConstraint){
			pEngConstraint->GetDofLinearX().SetLowerLimit(pLinearLowerLimits.x);
			pEngConstraint->GetDofLinearY().SetLowerLimit(pLinearLowerLimits.y);
			pEngConstraint->GetDofLinearZ().SetLowerLimit(pLinearLowerLimits.z);
		}
		
		pNotifyConstraintChanged();
	}
}

void reTemporaryConstraint::SetLinearUpperLimits(const decVector &upperLimits){
	if(!upperLimits.IsEqualTo(pLinearUpperLimits)){
		pLinearUpperLimits = upperLimits;
		
		if(pEngConstraint){
			pEngConstraint->GetDofLinearX().SetUpperLimit(pLinearUpperLimits.x);
			pEngConstraint->GetDofLinearY().SetUpperLimit(pLinearUpperLimits.y);
			pEngConstraint->GetDofLinearZ().SetUpperLimit(pLinearUpperLimits.z);
		}
		
		pNotifyConstraintChanged();
	}
}

void reTemporaryConstraint::SetAngularLowerLimits(const decVector &lowerLimits){
	if(!lowerLimits.IsEqualTo(pAngularLowerLimits)){
		pAngularLowerLimits = lowerLimits;
		
		if(pEngConstraint){
			pEngConstraint->GetDofAngularX().SetLowerLimit(pAngularLowerLimits.x * DEG2RAD);
			pEngConstraint->GetDofAngularY().SetLowerLimit(pAngularLowerLimits.y * DEG2RAD);
			pEngConstraint->GetDofAngularZ().SetLowerLimit(pAngularLowerLimits.z * DEG2RAD);
		}
		
		pNotifyConstraintChanged();
	}
}

void reTemporaryConstraint::SetAngularUpperLimits(const decVector &upperLimits){
	if(!upperLimits.IsEqualTo(pAngularUpperLimits)){
		pAngularUpperLimits = upperLimits;
		
		if(pEngConstraint){
			pEngConstraint->GetDofAngularX().SetUpperLimit(pAngularUpperLimits.x * DEG2RAD);
			pEngConstraint->GetDofAngularY().SetUpperLimit(pAngularUpperLimits.y * DEG2RAD);
			pEngConstraint->GetDofAngularZ().SetUpperLimit(pAngularUpperLimits.z * DEG2RAD);
		}
		
		pNotifyConstraintChanged();
	}
}



// Private Functions
//////////////////////

void reTemporaryConstraint::pNotifyConstraintChanged(){
	if(pEngSimCollider && pEngConstraint){
		pEngSimCollider->NotifyConstraintChanged(pEngSimCollider->IndexOfConstraint(pEngConstraint));
	}
}
