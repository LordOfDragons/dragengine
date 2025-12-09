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
pBonePoses(nullptr),
pBonePoseCount(0),
pFaceExpressions(nullptr),
pFaceExpressionCount(0)
{
	if(pDevice->GetBoneConfiguration() == deInputDevice::ebcHand){
		pBonePoses = new deInputDevicePose[deInputDevice::HandBoneCount * 2];
		pBonePoseCount = deInputDevice::HandBoneCount;
	}
	if(pDevice->GetSupportsFaceEyeExpressions() || pDevice->GetSupportsFaceMouthExpressions()){
		pFaceExpressions = new float[deInputDevice::FaceExpressionCount];
		pFaceExpressionCount = deInputDevice::FaceExpressionCount;
	}
}

dedsInputDevice::dedsInputDevice(deScriptingDragonScript &ds, deBaseVRModule &module, int deviceIndex) :
pDS(ds),
pDeviceSource(deInputEvent::esVR),
pDeviceIndex(deviceIndex),
pDevice(module.GetDeviceAt(deviceIndex)),
pBonePoses(nullptr),
pBonePoseCount(0),
pFaceExpressions(nullptr),
pFaceExpressionCount(0)
{
	if(pDevice->GetBoneConfiguration() == deInputDevice::ebcHand){
		pBonePoses = new deInputDevicePose[deInputDevice::HandBoneCount * 2];
		pBonePoseCount = deInputDevice::HandBoneCount;
	}
	if(pDevice->GetSupportsFaceEyeExpressions() || pDevice->GetSupportsFaceMouthExpressions()){
		pFaceExpressions = new float[deInputDevice::FaceExpressionCount];
		pFaceExpressionCount = deInputDevice::FaceExpressionCount;
	}
}

dedsInputDevice::~dedsInputDevice(){
	if(pFaceExpressions){
		delete [] pFaceExpressions;
	}
	if(pBonePoses){
		delete [] pBonePoses;
	}
}



// Management
///////////////

const deInputDevicePose &dedsInputDevice::GetBonePoseAt(int index, bool withController) const{
	if(index < 0 || index >= pBonePoseCount){
		static const deInputDevicePose defaultPose;
		return defaultPose;
	}
	
	if(withController){
		return pBonePoses[pBonePoseCount + index];
		
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
	int i;
	
	switch(pDeviceSource){
	case deInputEvent::esInput:{
		deBaseInputModule &module = *pDS.GetGameEngine()->GetInputSystem()->GetActiveModule();
		
		module.GetDevicePose(pDeviceIndex, pDevicePose);
		
		for(i=0; i<pBonePoseCount; i++){
			module.GetDeviceBonePose(pDeviceIndex, i, false, pBonePoses[i]);
		}
		for(i=0; i<pBonePoseCount; i++){
			module.GetDeviceBonePose(pDeviceIndex, i, true, pBonePoses[pBonePoseCount + i]);
		}
		
		for(i=0; i<pFaceExpressionCount; i++){
			pFaceExpressions[i] = module.GetDeviceFaceExpression(pDeviceIndex, i);
		}
		}break;
		
	case deInputEvent::esVR:{
		deBaseVRModule &module = *pDS.GetGameEngine()->GetVRSystem()->GetActiveModule();
		
		module.GetDevicePose(pDeviceIndex, pDevicePose);
		
		for(i=0; i<pBonePoseCount; i++){
			module.GetDeviceBonePose(pDeviceIndex, i, false, pBonePoses[i]);
		}
		for(i=0; i<pBonePoseCount; i++){
			module.GetDeviceBonePose(pDeviceIndex, i, true, pBonePoses[pBonePoseCount + i]);
		}
		
		for(i=0; i<pFaceExpressionCount; i++){
			pFaceExpressions[i] = module.GetDeviceFaceExpression(pDeviceIndex, i);
		}
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
		if(pBonePoses){
			delete [] pBonePoses;
			pBonePoses = nullptr;
			pBonePoseCount = 0;
		}
		
		if(device.pBonePoses){
			pBonePoses = new deInputDevicePose[device.pBonePoseCount * 2];
			pBonePoseCount = device.pBonePoseCount;
		}
	}
	
	if(pFaceExpressionCount != device.pFaceExpressionCount){
		if(pFaceExpressions){
			delete [] pFaceExpressions;
			pFaceExpressions = nullptr;
			pFaceExpressionCount = 0;
		}
		
		if(device.pFaceExpressions){
			pFaceExpressions = new float[device.pFaceExpressionCount];
			pFaceExpressionCount = device.pFaceExpressionCount;
		}
	}
}
