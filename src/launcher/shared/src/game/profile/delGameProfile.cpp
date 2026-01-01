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

#include "delGameProfile.h"
#include "delGPModule.h"
#include "delGPDisableModuleVersion.h"
#include "../../delLauncher.h"
#include "../../engine/delEngine.h"
#include "../../engine/delEngineInstance.h"
#include "../../engine/modules/delEngineModule.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/deModuleSystem.h>



// Class delGameProfile
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

delGameProfile::delGameProfile(const char *name) :
pName(name),
pReplaceRunArgs(false),
pFullScreen(true),
pWidth(1680),
pHeight(1050),
pValid(false){
}

delGameProfile::delGameProfile(const delGameProfile &profile){
	*this = profile;
	pName = profile.pName;
}

delGameProfile::~delGameProfile(){
}



// Management
///////////////

void delGameProfile::SetName(const char *name){
	pName = name;
}



void delGameProfile::SetModuleGraphic(const char *moduleName){
	pModuleGraphic = moduleName;
}

void delGameProfile::SetModuleInput(const char *moduleName){
	pModuleInput = moduleName;
}

void delGameProfile::SetModulePhysics(const char *moduleName){
	pModulePhysics = moduleName;
}

void delGameProfile::SetModuleAnimator(const char *moduleName){
	pModuleAnimator = moduleName;
}

void delGameProfile::SetModuleAI(const char *moduleName){
	pModuleAI = moduleName;
}

void delGameProfile::SetModuleCrashRecovery(const char *moduleName){
	pModuleCrashRecovery = moduleName;
}

void delGameProfile::SetModuleAudio(const char *moduleName){
	pModuleAudio = moduleName;
}

void delGameProfile::SetModuleSynthesizer(const char *moduleName){
	pModuleSynthesizer = moduleName;
}

void delGameProfile::SetModuleNetwork(const char *moduleName){
	pModuleNetwork = moduleName;
}

void delGameProfile::SetModuleVR(const char *moduleName){
	pModuleVR = moduleName;
}



void delGameProfile::SetModuleGraphicVersion(const char *moduleVersion){
	pModuleGraphicVersion = moduleVersion;
}

void delGameProfile::SetModuleInputVersion(const char *moduleVersion){
	pModuleInputVersion = moduleVersion;
}

void delGameProfile::SetModulePhysicsVersion(const char *moduleVersion){
	pModulePhysicsVersion = moduleVersion;
}

void delGameProfile::SetModuleAnimatorVersion(const char *moduleVersion){
	pModuleAnimatorVersion = moduleVersion;
}

void delGameProfile::SetModuleAIVersion(const char *moduleVersion){
	pModuleAIVersion = moduleVersion;
}

void delGameProfile::SetModuleCrashRecoveryVersion(const char *moduleVersion){
	pModuleCrashRecoveryVersion = moduleVersion;
}

void delGameProfile::SetModuleAudioVersion(const char *moduleVersion){
	pModuleAudioVersion = moduleVersion;
}

void delGameProfile::SetModuleSynthesizerVersion(const char *moduleVersion){
	pModuleSynthesizerVersion = moduleVersion;
}

void delGameProfile::SetModuleNetworkVersion(const char *moduleVersion){
	pModuleNetworkVersion = moduleVersion;
}

void delGameProfile::SetModuleVRVersion(const char *moduleVersion){
	pModuleVRVersion = moduleVersion;
}



void delGameProfile::SetRunArguments(const char *arguments){
	pRunArgs = arguments;
}

void delGameProfile::SetReplaceRunArguments(bool replaceRunArguments){
	pReplaceRunArgs = replaceRunArguments;
}

void delGameProfile::SetFullScreen(bool fullScreen){
	pFullScreen = fullScreen;
}

void delGameProfile::SetWidth(int width){
	pWidth = decMath::max(width, 100);
}

void delGameProfile::SetHeight(int height){
	pHeight = decMath::max(height, 75);
}



void delGameProfile::Verify(delLauncher &launcher){
	pValid = true;
	pValid &= VerifyModule(launcher, pModuleGraphic, pModuleGraphicVersion, deModuleSystem::emtGraphic);
	pValid &= VerifyModule(launcher, pModuleInput, pModuleInputVersion, deModuleSystem::emtInput);
	pValid &= VerifyModule(launcher, pModulePhysics, pModulePhysicsVersion, deModuleSystem::emtPhysics);
	pValid &= VerifyModule(launcher, pModuleAnimator, pModuleAnimatorVersion, deModuleSystem::emtAnimator);
	pValid &= VerifyModule(launcher, pModuleAI, pModuleAIVersion, deModuleSystem::emtAI);
	pValid &= VerifyModule(launcher, pModuleCrashRecovery, pModuleCrashRecoveryVersion, deModuleSystem::emtCrashRecovery);
	pValid &= VerifyModule(launcher, pModuleAudio, pModuleAudioVersion, deModuleSystem::emtAudio);
	pValid &= VerifyModule(launcher, pModuleSynthesizer, pModuleSynthesizerVersion, deModuleSystem::emtSynthesizer);
	pValid &= VerifyModule(launcher, pModuleNetwork, pModuleNetworkVersion, deModuleSystem::emtNetwork);
	pValid &= VerifyModule(launcher, pModuleVR, pModuleVRVersion, deModuleSystem::emtVR);
}

bool delGameProfile::VerifyModule(delLauncher &launcher, const char *moduleName,
const char *moduleVersion, int requiredType) const{
	const delEngineModuleList &moduleList = launcher.GetEngine().GetModules();
	delEngineModule *module;
	
	if(strlen(moduleVersion) == 0){
		module = moduleList.GetNamed (moduleName);
		
	}else{
		module = moduleList.GetNamed (moduleName, moduleVersion);
	}
	
	if(!module
	|| module->GetType() != requiredType
	|| module->GetStatus() != delEngineModule::emsReady){
		return false;
	}
	
	return true;
}

void delGameProfile::Activate(delLauncher &launcher, delEngineInstance &engineInstance) const{
	// disable module versions. this affects all modules
	const delEngineModuleList &engineModuleList = launcher.GetEngine().GetModules();
	const int moduleCount = pModules.GetCount();
	const int disableModuleVersionCount = pDisableModuleVersions.GetCount();
	int i;
	
	for(i=0; i<disableModuleVersionCount; i++){
		const delGPDisableModuleVersion &version = *pDisableModuleVersions.GetAt(i);
		delEngineModule * const module = engineModuleList.GetNamed (version.GetName(), version.GetVersion());
		if(module){
			engineInstance.EnableModule(module->GetName(), module->GetVersion(), false);
		}
	}
	
	// activate modules
	engineInstance.ActivateModule(pModuleCrashRecovery, pModuleCrashRecoveryVersion);
	engineInstance.ActivateModule(pModuleGraphic, pModuleGraphicVersion);
	engineInstance.ActivateModule(pModuleInput, pModuleInputVersion);
	engineInstance.ActivateModule(pModulePhysics, pModulePhysicsVersion);
	engineInstance.ActivateModule(pModuleAnimator, pModuleAnimatorVersion);
	engineInstance.ActivateModule(pModuleAI, pModuleAIVersion);
	engineInstance.ActivateModule(pModuleAudio, pModuleAudioVersion);
	engineInstance.ActivateModule(pModuleSynthesizer, pModuleSynthesizerVersion);
	engineInstance.ActivateModule(pModuleNetwork, pModuleNetworkVersion);
	engineInstance.ActivateModule(pModuleVR, pModuleVRVersion);
	
	// set module properties
	for(i=0; i<moduleCount; i++){
		delGPModule &module = *pModules.GetAt (i);
		delEngineModule * const engineModule = engineModuleList.GetNamed (module.GetName());
		if(engineModule){
			module.ApplyParameters(engineModule->GetVersion(), launcher, engineInstance);
		}
	}
}



// Operators
//////////////

delGameProfile &delGameProfile::operator=(const delGameProfile &profile){
	pModuleGraphic = profile.pModuleGraphic;
	pModuleInput = profile.pModuleInput;
	pModulePhysics = profile.pModulePhysics;
	pModuleAnimator = profile.pModuleAnimator;
	pModuleAI = profile.pModuleAI;
	pModuleCrashRecovery = profile.pModuleCrashRecovery;
	pModuleAudio = profile.pModuleAudio;
	pModuleSynthesizer = profile.pModuleSynthesizer;
	pModuleNetwork = profile.pModuleNetwork;
	pModuleVR = profile.pModuleVR;
	
	pModuleGraphicVersion = profile.pModuleGraphicVersion;
	pModuleInputVersion = profile.pModuleInputVersion;
	pModulePhysicsVersion = profile.pModulePhysicsVersion;
	pModuleAnimatorVersion = profile.pModuleAnimatorVersion;
	pModuleAIVersion = profile.pModuleAIVersion;
	pModuleCrashRecoveryVersion = profile.pModuleCrashRecoveryVersion;
	pModuleAudioVersion = profile.pModuleAudioVersion;
	pModuleSynthesizerVersion = profile.pModuleSynthesizerVersion;
	pModuleNetworkVersion = profile.pModuleNetworkVersion;
	pModuleVRVersion = profile.pModuleVRVersion;
	
	pDisableModuleVersions = profile.pDisableModuleVersions;
	
	pModules.RemoveAll();
	
	const delGPModuleList &moduleList = profile.GetModules();
	const int moduleCount = moduleList.GetCount();
	int i;
	for(i=0; i<moduleCount; i++){
		pModules.Add (delGPModule::Ref::New(*moduleList.GetAt (i)));
	}
	
	pRunArgs = profile.pRunArgs;
	pReplaceRunArgs = profile.pReplaceRunArgs;
	pFullScreen = profile.pFullScreen;
	pWidth = profile.pWidth;
	pHeight = profile.pHeight;
	
	pValid = profile.pValid;
	
	return *this;
}
