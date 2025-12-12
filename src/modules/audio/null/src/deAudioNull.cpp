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

#include "deAudioNull.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/systems/deAudioSystem.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/sound/deSpeaker.h>



// Export Definition
#ifndef WITH_INTERNAL_MODULE
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *NullAudioCreateModule(deLoadableModule *loadableModule);
#ifdef  __cplusplus
}
#endif
#endif



// Entry Function
///////////////////

deBaseModule *NullAudioCreateModule(deLoadableModule *loadableModule){
	deBaseModule *module = NULL;
	
	try{
		module = new deAudioNull(*loadableModule);
		
	}catch(const deException &){
		return NULL;
	}
	
	return module;
}



// Class deAudioNull
//////////////////////

// Constructor, destructor
////////////////////////////

deAudioNull::deAudioNull(deLoadableModule &loadableModule) :
deBaseAudioModule(loadableModule){
}

deAudioNull::~deAudioNull(){
}



// Management
///////////////

bool deAudioNull::Init(deMicrophone *activeMic){
	return true;
}

void deAudioNull::CleanUp(){
}

void deAudioNull::ProcessAudio(){
	deMicrophone *activeMicrophone = GetGameEngine()->GetAudioSystem()->GetActiveMicrophone();
	deWorld *world = NULL;
	
	// stepping the speakers is required as game code can synchronize to sound finished playing.
	// for the time being we stop all speakers found playing. this is not correct but prevents
	// problems until proper code is written
	if(activeMicrophone){
		world = activeMicrophone->GetParentWorld();
	}
	
	if(world){
		deSpeaker *speaker = world->GetRootSpeaker();
		while(speaker){
			deSpeaker * const safeSpeaker = speaker;
			speaker = speaker->GetLLWorldNext();
			
			if(safeSpeaker->GetPlaying()){
				safeSpeaker->Stop();
			}
		}
	}
}

void deAudioNull::SetActiveMicrophone(deMicrophone *microphone){
}



// Audio Management
/////////////////////

deBaseAudioWorld *deAudioNull::CreateWorld(deWorld*){
	return NULL;
}

deBaseAudioSound *deAudioNull::CreateSound(deSound*){
	return NULL;
}

deBaseAudioSpeaker *deAudioNull::CreateSpeaker(deSpeaker*){
	return NULL;
}

deBaseAudioMicrophone *deAudioNull::CreateMicrophone(deMicrophone*){
	return NULL;
}

deBaseAudioComponent *deAudioNull::CreateComponent(deComponent*){
	return NULL;
}

deBaseAudioModel *deAudioNull::CreateModel(deModel*){
	return NULL;
}

deBaseAudioSkin *deAudioNull::CreateSkin(deSkin*){
	return NULL;
}

deBaseAudioDecal *deAudioNull::CreateDecal(deDecal*){
	return NULL;
}

deBaseAudioSoundLevelMeter *deAudioNull::CreateSoundLevelMeter(deSoundLevelMeter*){
	return NULL;
}

deBaseAudioVideoPlayer *deAudioNull::CreateVideoPlayer(deVideoPlayer*){
	return NULL;
}

deBaseAudioSynthesizerInstance *deAudioNull::CreateSynthesizerInstance(deSynthesizerInstance*){
	return NULL;
}

deBaseAudioHeightTerrain *deAudioNull::CreateHeightTerrain(deHeightTerrain&){
	return NULL;
}

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

#ifndef MODULE_VERSION
#include "module_version.h"
#endif

class deanModuleInternal : public deInternalModule{
public:
	typedef deTObjectReference<deanModuleInternal> Ref;
	
	deanModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("NullAudio");
		SetDescription("Outputs no audio.");
		SetAuthor("DragonDreams GmbH (info@dragondreams.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtAudio);
		SetDirectoryName("null");
		SetPriority(0);
		SetIsFallback(true);
		SetDefaultLoggingName();
	}
	
	void CreateModule() override{
		SetModule(NullAudioCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deTObjectReference<deInternalModule> deanRegisterInternalModule(deModuleSystem *system){
	return deanModuleInternal::Ref::NewWith(system);
}
#endif
