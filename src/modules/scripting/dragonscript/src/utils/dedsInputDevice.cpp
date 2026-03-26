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

#include <libdscript/libdscript.h>

#include "dedsInputDevice.h"
#include "../deScriptingDragonScript.h"

#include <dragengine/deEngine.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/deVRSystem.h>
#include <dragengine/systems/modules/input/deBaseInputModule.h>
#include <dragengine/systems/modules/vr/deBaseVRModule.h>


// Class dedsInputDevice
//////////////////////////

// Constructor, destructor
////////////////////////////

dedsInputDevice::dedsInputDevice(deScriptingDragonScript &ds, deBaseInputModule &module, int deviceIndex) :
pDS(ds),
pDeviceSource(deInputEvent::esInput),
pDeviceIndex(deviceIndex),
pDevice(module.GetDeviceAt(deviceIndex)),
pBonePoseCount(0),
pFaceExpressionCount(0)
{
	switch(pDevice->GetBoneConfiguration()){
	case deInputDevice::ebcHand:
		pBonePoses.AddRange(deInputDevice::HandBoneCount, {});
		pBonePosesWithController.AddRange(deInputDevice::HandBoneCount, {});
		pBonePoseCount = deInputDevice::HandBoneCount;
		break;
		
	case deInputDevice::ebcUpperBody:
		pBonePoses.AddRange(deInputDevice::UpperBodyBoneCount, {});
		pBonePoseCount = deInputDevice::UpperBodyBoneCount;
		break;
		
	case deInputDevice::ebcFullBody:
		pBonePoses.AddRange(deInputDevice::FullBodyBoneCount, {});
		pBonePoseCount = deInputDevice::FullBodyBoneCount;
		break;
		
	default:
		break;
	}
	
	if(pDevice->GetSupportsFaceEyeExpressions() || pDevice->GetSupportsFaceMouthExpressions()){
		pFaceExpressions.AddRange(deInputDevice::FaceExpressionCount, {});
		pFaceExpressionCount = deInputDevice::FaceExpressionCount;
	}
}

dedsInputDevice::dedsInputDevice(deScriptingDragonScript &ds, deBaseVRModule &module, int deviceIndex) :
pDS(ds),
pDeviceSource(deInputEvent::esVR),
pDeviceIndex(deviceIndex),
pDevice(module.GetDeviceAt(deviceIndex)),
pBonePoseCount(0),
pFaceExpressionCount(0)
{
	switch(pDevice->GetBoneConfiguration()){
	case deInputDevice::ebcHand:
		pBonePoses.AddRange(deInputDevice::HandBoneCount, {});
		pBonePosesWithController.AddRange(deInputDevice::HandBoneCount, {});
		pBonePoseCount = deInputDevice::HandBoneCount;
		break;
		
	case deInputDevice::ebcUpperBody:
		pBonePoses.AddRange(deInputDevice::UpperBodyBoneCount, {});
		pBonePoseCount = deInputDevice::UpperBodyBoneCount;
		break;
		
	case deInputDevice::ebcFullBody:
		pBonePoses.AddRange(deInputDevice::FullBodyBoneCount, {});
		pBonePoseCount = deInputDevice::FullBodyBoneCount;
		break;
		
	default:
		break;
	}
	
	if(pDevice->GetSupportsFaceEyeExpressions() || pDevice->GetSupportsFaceMouthExpressions()){
		pFaceExpressions.AddRange(deInputDevice::FaceExpressionCount, {});
		pFaceExpressionCount = deInputDevice::FaceExpressionCount;
	}
}

dedsInputDevice::~dedsInputDevice() = default;



// Management
///////////////

const deInputDevicePose &dedsInputDevice::GetBonePoseAt(int index, bool withController) const{
	if(index < 0 || index >= pBonePoseCount){
		static const deInputDevicePose defaultPose;
		return defaultPose;
	}
	
	if(withController && pBonePosesWithController.IsNotEmpty()){
		return pBonePosesWithController[index];
		
	}else{
		return pBonePoses[index];
	}
}

float dedsInputDevice::GetFaceExpressionAt(int index) const{
	if(index < 0 || index >= pFaceExpressionCount){
		return 0.0f;
	}
	
	return pFaceExpressions[index];
}

void dedsInputDevice::OnFrameUpdate(){
	switch(pDeviceSource){
	case deInputEvent::esInput:{
		deBaseInputModule &module = *pDS.GetGameEngine()->GetInputSystem()->GetActiveModule();
		module.GetDevicePose(pDeviceIndex, pDevicePose);
		
		pBonePoses.VisitIndexed([&](int index, deInputDevicePose &pose){
			module.GetDeviceBonePose(pDeviceIndex, index, false, pose);
		});
		pBonePosesWithController.VisitIndexed([&](int index, deInputDevicePose &pose){
			module.GetDeviceBonePose(pDeviceIndex, index, true, pose);
		});
		pFaceExpressions.VisitIndexed([&](int index, float &expression){
			expression = module.GetDeviceFaceExpression(pDeviceIndex, index);
		});
		}break;
		
	case deInputEvent::esVR:{
		deBaseVRModule &module = *pDS.GetGameEngine()->GetVRSystem()->GetActiveModule();
		module.GetDevicePose(pDeviceIndex, pDevicePose);
		
		pBonePoses.VisitIndexed([&](int index, deInputDevicePose &pose){
			module.GetDeviceBonePose(pDeviceIndex, index, false, pose);
		});
		pBonePosesWithController.VisitIndexed([&](int index, deInputDevicePose &pose){
			module.GetDeviceBonePose(pDeviceIndex, index, true, pose);
		});
		pFaceExpressions.VisitIndexed([&](int index, float &expression){
			expression = module.GetDeviceFaceExpression(pDeviceIndex, index);
		});
		}break;
	}
}

void dedsInputDevice::Update(const dedsInputDevice &device){
	if(device.pDeviceSource != pDeviceSource){
		DETHROW_INFO(deeInvalidParam, "device source differs");
	}
	if(device.pDevice->GetID() != pDevice->GetID()){
		DETHROW_INFO(deeInvalidParam, "device id differs");
	}
	
	pDeviceIndex = device.pDeviceIndex;
	pDevice = device.pDevice;
	
	if(pBonePoseCount != device.pBonePoseCount){
		pBonePoses = device.pBonePoses;
		pBonePosesWithController = device.pBonePosesWithController;
		pBonePoseCount = device.pBonePoseCount;
	}
	
	if(pFaceExpressionCount != device.pFaceExpressionCount){
		pFaceExpressions = device.pFaceExpressions;
		pFaceExpressionCount = device.pFaceExpressionCount;
	}
}
