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

#include "projTRProfile.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/systems/deCrashRecoverySystem.h>
#include <dragengine/systems/deGraphicSystem.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/dePhysicsSystem.h>
#include <dragengine/systems/deAnimatorSystem.h>
#include <dragengine/systems/deAISystem.h>
#include <dragengine/systems/deAudioSystem.h>
#include <dragengine/systems/deSynthesizerSystem.h>
#include <dragengine/systems/deVRSystem.h>
#include <dragengine/systems/deNetworkSystem.h>
#include <dragengine/systems/modules/deBaseModule.h>
#include <dragengine/systems/modules/deLoadableModule.h>



// Class projTRProfile
////////////////////////

// Constructors and Destructors
/////////////////////////////////

projTRProfile::projTRProfile() :
pReplaceRunArgs(false),
pWidth(800),
pHeight(600),
pFullScreen(false),

pValid(false){
}

projTRProfile::~projTRProfile(){
}



// Management
///////////////

void projTRProfile::SetName(const char *name){
	pName = name;
}

void projTRProfile::SetModuleGraphic(const char *moduleName){
	pModuleGraphic = moduleName;
}

void projTRProfile::SetModuleInput(const char *moduleName){
	pModuleInput = moduleName;
}

void projTRProfile::SetModulePhysics(const char *moduleName){
	pModulePhysics = moduleName;
}

void projTRProfile::SetModuleAnimator(const char *moduleName){
	pModuleAnimator = moduleName;
}

void projTRProfile::SetModuleAI(const char *moduleName){
	pModuleAI = moduleName;
}

void projTRProfile::SetModuleCrashRecovery(const char *moduleName){
	pModuleCrashRecovery = moduleName;
}

void projTRProfile::SetModuleAudio(const char *moduleName){
	pModuleAudio = moduleName;
}

void projTRProfile::SetModuleSynthesizer(const char *moduleName){
	pModuleSynthesizer = moduleName;
}

void projTRProfile::SetModuleNetwork(const char *moduleName){
	pModuleNetwork = moduleName;
}

void projTRProfile::SetModuleVR(const char *moduleName){
	pModuleVR = moduleName;
}



void projTRProfile::SetRunArguments(const char *arguments){
	pRunArgs = arguments;
}

void projTRProfile::SetReplaceRunArguments(bool replaceRunArguments){
	pReplaceRunArgs = replaceRunArguments;
}

void projTRProfile::SetWidth(int width){
	if(width < 1){
		DETHROW(deeInvalidParam);
	}
	pWidth = width;
}

void projTRProfile::SetHeight(int height){
	if(height < 1){
		DETHROW(deeInvalidParam);
	}
	pHeight = height;
}

void projTRProfile::SetFullScreen(bool fullScreen){
	pFullScreen = fullScreen;
}



void projTRProfile::Verify(deEngine &engine){
	pValid = true;
	pValid &= VerifyModule(engine, pModuleGraphic, deModuleSystem::emtGraphic);
	pValid &= VerifyModule(engine, pModuleInput, deModuleSystem::emtInput);
	pValid &= VerifyModule(engine, pModulePhysics, deModuleSystem::emtPhysics);
	pValid &= VerifyModule(engine, pModuleAnimator, deModuleSystem::emtAnimator);
	pValid &= VerifyModule(engine, pModuleAI, deModuleSystem::emtAI);
	pValid &= VerifyModule(engine, pModuleCrashRecovery, deModuleSystem::emtCrashRecovery);
	pValid &= VerifyModule(engine, pModuleAudio, deModuleSystem::emtAudio);
	pValid &= VerifyModule(engine, pModuleSynthesizer, deModuleSystem::emtSynthesizer);
	pValid &= VerifyModule(engine, pModuleNetwork, deModuleSystem::emtNetwork);
	pValid &= VerifyModule(engine, pModuleVR, deModuleSystem::emtVR);
}

bool projTRProfile::VerifyModule(deEngine &engine, const char *moduleName,
deModuleSystem::eModuleTypes requiredType) const{
	deLoadableModule * const module = engine.GetModuleSystem()->GetModuleNamed(moduleName);
	return module
		&& module->GetType() == requiredType
		&& module->IsLoaded()
		&& module->GetEnabled();
}
