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

#include "dearLink.h"
#include "dearAnimatorInstance.h"
#include "dearBoneState.h"
#include "dearVPSState.h"
#include "dearControllerStates.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decIntList.h>
#include <dragengine/resources/animator/deAnimatorLink.h>



// Class dearLink
///////////////////

// Constructors and Destructors
/////////////////////////////////

dearLink::dearLink(dearAnimatorInstance &instance, const deAnimatorLink &link,
	const decIntList &controllerMapping) :
pInstance(instance),
pLink(link),
pEvaluator(link.GetCurve()),
pBoneIndex(-1),
pVPSIndex(-1),
pWrapY(link.GetWrapY())
{
	const int controller = pLink.GetController();
	if(controller != -1){
		if(controller < controllerMapping.GetCount()){
			pLink.SetController(controllerMapping.GetAt(controller));
			
		}else{
			pLink.SetController(-1);
		}
	}
	
	if(! pLink.GetBone().IsEmpty()){
		pBoneIndex = instance.GetBoneStateList().IndexOfStateNamed(pLink.GetBone());
	}
	
	if(! pLink.GetVertexPositionSet().IsEmpty()){
		pVPSIndex = instance.GetVPSStateList().IndexOfStateNamed(pLink.GetVertexPositionSet());
	}
}

dearLink::~dearLink(){
}



// Management
///////////////

bool dearLink::HasController() const{
	return pLink.HasController();
}

int dearLink::GetController() const{
	return pLink.GetController();
}

bool dearLink::HasBone() const{
	return pBoneIndex != -1;
}

bool dearLink::HasVPS() const{
	return pVPSIndex != -1;
}



float dearLink::GetValue(float defaultValue) const{
	float value;
	
	if(pLink.HasController()){
		value = pInstance.GetControllerStates().GetValueAt(pLink.GetController());
		
	}else if(pBoneIndex != -1){
		const dearBoneState &state = *pInstance.GetBoneStateList().GetStateAt(pBoneIndex);
		
		switch(pLink.GetBoneParameter()){
		case deAnimatorLink::ebpPositionX:
			value = state.GetPosition().x;
			break;
			
		case deAnimatorLink::ebpPositionY:
			value = state.GetPosition().y;
			break;
			
		case deAnimatorLink::ebpPositionZ:
			value = state.GetPosition().z;
			break;
			
		case deAnimatorLink::ebpRotationX:
			value = state.GetOrientation().GetEulerAngles().x;
			break;
			
		case deAnimatorLink::ebpRotationY:
			value = state.GetOrientation().GetEulerAngles().y;
			break;
			
		case deAnimatorLink::ebpRotationZ:
			value = state.GetOrientation().GetEulerAngles().z;
			break;
			
		case deAnimatorLink::ebpScaleX:
			value = state.GetScale().x;
			break;
			
		case deAnimatorLink::ebpScaleY:
			value = state.GetScale().y;
			break;
			
		case deAnimatorLink::ebpScaleZ:
			value = state.GetScale().z;
			break;
			
		default:
			value = pLink.GetBoneMinimumValue();
		}
		
		value = decMath::linearStep(value, pLink.GetBoneMinimumValue(),  pLink.GetBoneMaximumValue());
		
	}else if(pVPSIndex != -1){
		value = decMath::linearStep(pInstance.GetVPSStateList().GetStateAt(pVPSIndex).GetWeight(),
			pLink.GetVertexPositionSetMinimumValue(), pLink.GetVertexPositionSetMaximumValue());
		
	}else{
		return defaultValue;
	}
	
	const int repeat = pLink.GetRepeat();
	if(repeat > 1){
		value *= (float)repeat;
		value -= floorf(value);
	}
	
	if(pEvaluator.GetCurve().GetPointCount() > 0){
		value = pEvaluator.EvaluateAt(value);
	}
	
	if(pWrapY){
		value -= floorf(value);
	}
	
	return value;
}

void dearLink::GetVector(decVector &vector) const{
	if(pLink.HasController()){
		vector = pInstance.GetControllerStates().GetVectorAt(pLink.GetController());
	}
}

void dearLink::GetQuaternion(decQuaternion &quaternion) const{
	if(pLink.HasController()){
		quaternion.SetFromEuler(pInstance.GetControllerStates().GetVectorAt(pLink.GetController()));
	}
}
