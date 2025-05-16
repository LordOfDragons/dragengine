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

#include "dedsVRPlaceholder.h"
#include "deScriptingDragonScript.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputDevicePose.h>
#include <dragengine/resources/camera/deCameraManager.h>
#include <dragengine/resources/component/deComponentManager.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/sky/deSky.h>
#include <dragengine/resources/sky/deSkyManager.h>
#include <dragengine/resources/sky/deSkyLayer.h>
#include <dragengine/resources/sky/deSkyInstance.h>
#include <dragengine/resources/sky/deSkyInstanceManager.h>
#include <dragengine/resources/world/deWorldManager.h>
#include <dragengine/systems/deGraphicSystem.h>
#include <dragengine/systems/deVRSystem.h>


// Class dedsVRPlaceholder
////////////////////////////

// Constructor
////////////////

dedsVRPlaceholder::dedsVRPlaceholder(deScriptingDragonScript &ds) :
pDS(ds),
pDeviceIndexHmd(-1),
pEnabled(false),
pVRWorking(true)
{
	try{
		pCreateWorld();
		
		deVRSystem &vrsys = *ds.GetGameEngine()->GetVRSystem();
		DEASSERT_TRUE(vrsys.RuntimeUsable())
		vrsys.StartRuntime();
		pFindHmdDevice();
		
		SetEnabled(true);
		Update();
		
	}catch(const deException &e){
		pVRWorking = false;
		pDS.LogException(e);
	}
}

dedsVRPlaceholder::~dedsVRPlaceholder(){
	pCleanUp();
}



// Management
///////////////

void dedsVRPlaceholder::Update(){
	if(!pEnabled || !pVRWorking){
		return;
	}
	
	if(pDeviceIndexHmd != -1){
		deBaseVRModule &module = *pDS.GetGameEngine()->GetVRSystem()->GetActiveModule();
		deInputDevicePose pose;
		module.GetDevicePose(pDeviceIndexHmd, pose);
		
		pCamera->SetPosition(pose.GetPosition());
		pCamera->SetOrientation(pose.GetOrientation());
	}
	
	pWorld->Update(pDS.GetGameEngine()->GetElapsedTime());
}

void dedsVRPlaceholder::SetEnabled(bool enabled){
	if(pEnabled == enabled){
		return;
	}
	
	pEnabled = enabled;
	
	if(!pVRWorking){
		pDS.GetGameEngine()->GetVRSystem()->SetCamera(pGameCamera);
		return;
	}
	
	pDS.GetGameEngine()->GetVRSystem()->SetCamera(enabled ? pCamera : pGameCamera);
}

void dedsVRPlaceholder::SetGameCamera(deCamera *camera){
	pGameCamera = camera;
	if(!pEnabled || !pVRWorking){
		pDS.GetGameEngine()->GetVRSystem()->SetCamera(camera);
	}
}

void dedsVRPlaceholder::EventReceived(const deInputEvent &event){
	if(!pVRWorking){
		return;
	}
	
	if(event.GetSource() != deInputEvent::esVR){
		return;
	}
	
	switch(event.GetType()){
	case deInputEvent::eeDevicesAttachedDetached:
		pFindHmdDevice();
		break;
		
	default:
		break;
	}
}


// Private Functions
//////////////////////

void dedsVRPlaceholder::pCreateWorld(){
	pDS.LogInfo("Create VR Placeholder");
	deEngine &engine = *pDS.GetGameEngine();
	
	pWorld.TakeOver(engine.GetWorldManager()->CreateWorld());
	pWorld->SetDisableLights(true);
	
	const deModel::Ref model(deModel::Ref::New(engine.GetModelManager()->LoadModel(
		"/shareddata/models/vrplaceholder/vrplaceholder.demodel", "/")));
	
	const deSkin::Ref skin(deSkin::Ref::New(engine.GetSkinManager()->LoadSkin(
		"/shareddata/models/vrplaceholder/vrplaceholder.deskin", "/")));
	
	
	const deSky::Ref sky(deSky::Ref::New(engine.GetSkyManager()->CreateSky()));
	sky->SetBgColor(decColor(0.0f, 0.5f, 1.0f));
	sky->SetLayerCount(1);
	
	deSkyLayer &skyLayer = sky->GetLayerAt(0);
	skyLayer.SetAmbientIntensity(1.0f);
	skyLayer.SetLightIntensity(0.0f);
	
	const deSkyInstance::Ref skyInstance(deSkyInstance::Ref::New(
		engine.GetSkyInstanceManager()->CreateSkyInstance()));
	skyInstance->SetSky(sky);
	pWorld->AddSky(skyInstance);
	
	
	pComponent.TakeOver(engine.GetComponentManager()->CreateComponent(model, skin));
	pWorld->AddComponent(pComponent);
	
	pCamera.TakeOver(engine.GetCameraManager()->CreateCamera());
	pCamera->SetEnableGI(false);
	pCamera->SetEnableHDRR(false);
	pWorld->AddCamera(pCamera);
	
	pWorld->Update(0.0f);
}

void dedsVRPlaceholder::pCleanUp(){
	pDS.LogInfo("Clean up VR Placeholder" );
	
	deVRSystem &vrsys = *pDS.GetGameEngine()->GetVRSystem();
	try{
		vrsys.StopRuntime();
		
	}catch(const deException &e){
		pDS.LogException(e);
	}
	
	if(pCamera && pWorld && pCamera->GetParentWorld() == pWorld){
		pWorld->RemoveCamera(pCamera);
	}
	
	pCamera = nullptr;
	pComponent = nullptr;
	pWorld = nullptr;
}

void dedsVRPlaceholder::pFindHmdDevice(){
	deBaseVRModule &module = *pDS.GetGameEngine()->GetVRSystem()->GetActiveModule();
	const int count = module.GetDeviceCount();
	int i;
	
	pDeviceIndexHmd = -1;
	
	for(i=0; i<count; i++){
		const deInputDevice::Ref device(deInputDevice::Ref::New(module.GetDeviceAt(i)));
		if(device->GetType() == deInputDevice::eDeviceTypes::edtVRHMD){
			pDeviceIndexHmd = i;
			return;
		}
	}
}
