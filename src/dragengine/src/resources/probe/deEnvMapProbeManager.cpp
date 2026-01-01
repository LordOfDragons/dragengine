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
#include "deEnvMapProbe.h"
#include "deEnvMapProbeManager.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deGraphicSystem.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"



// Class deEnvMapProbeManager
///////////////////////////////

// Constructor, destructor
////////////////////////////

deEnvMapProbeManager::deEnvMapProbeManager(deEngine *engine) : deResourceManager(engine, ertEnvMapProbe){
	SetLoggingName("environment map probe");
}

deEnvMapProbeManager::~deEnvMapProbeManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deEnvMapProbeManager::GetEnvMapProbeCount() const{
	return pEnvMapProbes.GetCount();
}

deEnvMapProbe *deEnvMapProbeManager::GetRootEnvMapProbe() const{
	return (deEnvMapProbe*)pEnvMapProbes.GetRoot();
}

deEnvMapProbe::Ref deEnvMapProbeManager::CreateEnvMapProbe(){
	const deEnvMapProbe::Ref envMapProbe(deEnvMapProbe::Ref::New(this));
	GetGraphicSystem()->LoadEnvMapProbe(envMapProbe);
	pEnvMapProbes.Add(envMapProbe);
	return envMapProbe;
}



void deEnvMapProbeManager::ReleaseLeakingResources(){
	int count = GetEnvMapProbeCount();
	
	if(count > 0){
		LogWarnFormat("%i leaking environment map probes", count);
		pEnvMapProbes.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems support
////////////////////

void deEnvMapProbeManager::SystemGraphicLoad(){
	deEnvMapProbe *envMapProbe = (deEnvMapProbe*)pEnvMapProbes.GetRoot();
	deGraphicSystem &grasys = *GetGraphicSystem();
	
	while(envMapProbe){
		if(!envMapProbe->GetPeerGraphic()){
			grasys.LoadEnvMapProbe(envMapProbe);
		}
		
		envMapProbe = (deEnvMapProbe*)envMapProbe->GetLLManagerNext();
	}
}

void deEnvMapProbeManager::SystemGraphicUnload(){
	deEnvMapProbe *envMapProbe = (deEnvMapProbe*)pEnvMapProbes.GetRoot();
	
	while(envMapProbe){
		envMapProbe->SetPeerGraphic(NULL);
		envMapProbe = (deEnvMapProbe*)envMapProbe->GetLLManagerNext();
	}
}



void deEnvMapProbeManager::RemoveResource(deResource *resource){
	pEnvMapProbes.RemoveIfPresent(resource);
}
