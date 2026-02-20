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
#include "deTouchSensor.h"
#include "deTouchSensorManager.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/dePhysicsSystem.h"
#include "../../systems/deScriptingSystem.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"



// Class deTouchSensorManager
///////////////////////////////

// Constructor, destructor
////////////////////////////

deTouchSensorManager::deTouchSensorManager(deEngine *engine) : deResourceManager(engine, ertTouchSensor){
	SetLoggingName("touch sensor");
}

deTouchSensorManager::~deTouchSensorManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deTouchSensorManager::GetTouchSensorCount() const{
	return pTouchSensors.GetCount();
}

deTouchSensor *deTouchSensorManager::GetRootTouchSensor() const{
	return (deTouchSensor*)pTouchSensors.GetRoot();
}

deTouchSensor::Ref deTouchSensorManager::CreateTouchSensor(){
	const deTouchSensor::Ref touchSensor(deTouchSensor::Ref::New(this));
	GetPhysicsSystem()->LoadTouchSensor(touchSensor);
	GetScriptingSystem()->LoadTouchSensor(touchSensor);
	pTouchSensors.Add(touchSensor);
	return touchSensor;
}



void deTouchSensorManager::ReleaseLeakingResources(){
	int count = GetTouchSensorCount();
	
	if(count > 0){
		LogWarnFormat("%i leaking touch sensor", count);
		pTouchSensors.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems support
////////////////////

void deTouchSensorManager::SystemPhysicsLoad(){
	dePhysicsSystem &phySys = *GetPhysicsSystem();
	pTouchSensors.GetResources().Visit([&](deResource *res){
		deTouchSensor *touchSensor = static_cast<deTouchSensor*>(res);
		if(!touchSensor->GetPeerPhysics()){
			phySys.LoadTouchSensor(touchSensor);
		}
	});
}

void deTouchSensorManager::SystemPhysicsUnload(){
	pTouchSensors.GetResources().Visit([&](deResource *res){
		static_cast<deTouchSensor*>(res)->SetPeerPhysics(nullptr);
	});
}

void deTouchSensorManager::SystemScriptingLoad(){
	deScriptingSystem &scrSys = *GetScriptingSystem();
	pTouchSensors.GetResources().Visit([&](deResource *res){
		deTouchSensor *touchSensor = static_cast<deTouchSensor*>(res);
		if(!touchSensor->GetPeerScripting()){
			scrSys.LoadTouchSensor(touchSensor);
		}
	});
}

void deTouchSensorManager::SystemScriptingUnload(){
	pTouchSensors.GetResources().Visit([&](deResource *res){
		static_cast<deTouchSensor*>(res)->SetPeerScripting(nullptr);
	});
}



void deTouchSensorManager::RemoveResource(deResource *resource){
	pTouchSensors.RemoveIfPresent(resource);
}
