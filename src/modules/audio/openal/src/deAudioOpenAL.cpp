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

#include "deAudioOpenAL.h"
#include "deoalCommandExecuter.h"
#include "audiothread/deoalAudioThread.h"
#include "component/deoalComponent.h"
#include "configuration/deoalConfiguration.h"
#include "configuration/deoalLSConfiguration.h"
#include "devmode/deoalDevMode.h"
#include "parameters/deoalParameter.h"
#include "parameters/deoalParameterList.h"
#include "parameters/deoalPEnableEFX.h"
#include "parameters/deoalPAuralizationMode.h"
#include "parameters/deoalPAuralizationQuality.h"
#include "parameters/deoalPMaxEnvSlots.h"
#include "parameters/deoalPLogLevel.h"
#include "microphone/deoalMicrophone.h"
#include "model/deoalModel.h"
#include "skin/deoalSkin.h"
#include "sound/deoalSound.h"
#include "soundLevelMeter/deoalSoundLevelMeter.h"
#include "speaker/deoalSpeaker.h"
#include "synthesizer/deoalSynthesizerInstance.h"
#include "video/deoalVideoPlayer.h"
#include "world/deoalWorld.h"

#include <dragengine/deEngine.h>
#include <dragengine/dragengine_configuration.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/systems/modules/deModuleParameter.h>
#include <dragengine/systems/modules/audio/deBaseAudioDecal.h>

#ifdef OS_ANDROID
#include <dragengine/app/deOSAndroid.h>
#include <android/native_activity.h>
#endif


#ifndef WITH_INTERNAL_MODULE
#ifdef __cplusplus
extern "C" {
#endif
#ifdef OS_UNIX
__attribute__ ((visibility ("default")))
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *OpenALCreateModule(deLoadableModule *loadableModule);
#ifdef  __cplusplus
}
#endif
#endif

deBaseModule *OpenALCreateModule(deLoadableModule *loadableModule){
	try{
		return new deAudioOpenAL(*loadableModule);
	}catch(const deException &){
	}
	return nullptr;
}


// Class deAudioOpenAL
////////////////////////

// Constructor, destructor
////////////////////////////

deAudioOpenAL::deAudioOpenAL(deLoadableModule &loadableModule) :
deBaseAudioModule(loadableModule),

pAudioThread(NULL),

pConfiguration(NULL),
pCommandExecuter(NULL),
pParameters(NULL),

pDevMode(NULL),

pActiveMicrophone(NULL)
{
	try{
		pCommandExecuter = new deoalCommandExecuter(*this);
		pConfiguration = new deoalConfiguration;
		
		pParameters = new deoalParameterList;
		pParameters->AddParameter(new deoalPEnableEFX(*this));
		pParameters->AddParameter(new deoalPAuralizationMode(*this));
		pParameters->AddParameter(new deoalPAuralizationQuality(*this));
		pParameters->AddParameter(new deoalPMaxEnvSlots(*this));
		pParameters->AddParameter(new deoalPLogLevel(*this));
		
	}catch(const deException &e){
		LogException(e);
		throw;
	}
}

deAudioOpenAL::~deAudioOpenAL(){
	CleanUp();
	
	if(pConfiguration){
		delete pConfiguration;
	}
	if(pCommandExecuter){
		delete pCommandExecuter;
	}
	if(pParameters){
		delete pParameters;
	}
}



// Management
///////////////

bool deAudioOpenAL::HasAudioThread() const{
	return pAudioThread != NULL;
}



deoalWorld *deAudioOpenAL::GetActiveWorld() const{
	if(!pActiveMicrophone){
		return NULL;
	}
	return pActiveMicrophone->GetParentWorld();
}



bool deAudioOpenAL::Init(deMicrophone *activeMic){
	#ifdef OAL_THREAD_CHECK
	LogWarn("OpenAL calls only in audio thread check enabled. Disable for production builds.");
	#endif
	#ifdef OAL_CHECKCOMMANDS
	LogWarn("OpenAL command failure check enabled. Disable for production builds.");
	#endif
	
	try{
		deoalLSConfiguration(*this, *pConfiguration).LoadConfig();
		
		pAudioThread = new deoalAudioThread(*this);
		pAudioThread->Init();
		
		SetActiveMicrophone(activeMic);
		
		pDevMode = new deoalDevMode(*this);
		
	}catch(const deException &e){
		LogException(e);
		return false;
	}
	
	return true;
}

void deAudioOpenAL::CleanUp(){
	if(pConfiguration){
		deoalLSConfiguration(*this, *pConfiguration).SaveConfig();
	}
	
	if(pAudioThread){
		pAudioThread->CleanUp();
		delete pAudioThread;
		pAudioThread = NULL;
	}
	
	if(pDevMode){
		delete pDevMode;
		pDevMode = NULL;
	}
}

void deAudioOpenAL::ProcessAudio(){
	// wait for audio to finish. if done asynchronously uses time history to judge if audio is
	// finished soon enough to wait for this event or to skip synchronization and running
	// another game frame update. this method returns only true if the main thread is allowed
	// to modify synchronization data. in all other situations false is returned
	if(!pAudioThread->MainThreadWaitFinishAudio()){
		return; // enough time left to run another game frame update
	}
	
	// retain resource data
	pAudioThread->RetainResourceData();
	
	// finalize asynchronously loaded resources
	pAudioThread->FinalizeAsyncResLoading();
	
	// synchronize active microphone parent world. this is done in the main thread and is
	// a synchronization point
	if(pActiveMicrophone){
		pAudioThread->SetActiveMicrophone(pActiveMicrophone->GetAMicrophone());
		
		if(pActiveMicrophone->GetParentWorld()){
			pActiveMicrophone->GetParentWorld()->Synchronize();
			
		}else{
			pActiveMicrophone->Synchronize();
		}
		
	}else{
		pAudioThread->SetActiveMicrophone(NULL);
	}
	
	// synchronize audio thread and trigger next audio cycle
	pAudioThread->Synchronize();
}

void deAudioOpenAL::SetActiveMicrophone(deMicrophone *microphone){
	deoalMicrophone *oalMicrophone = NULL;
	if(microphone){
		oalMicrophone = (deoalMicrophone*)microphone->GetPeerAudio();
	}
	
	if(pActiveMicrophone){
		pActiveMicrophone->SetActive(false);
	}
	
	pActiveMicrophone = oalMicrophone;
	
	if(oalMicrophone){
		oalMicrophone->SetActive(true);
	}
}

int deAudioOpenAL::GetFPSRate(){
	return pAudioThread->GetFPSRate();
}



// Audio Management
/////////////////////

deBaseAudioWorld *deAudioOpenAL::CreateWorld(deWorld *world){
	return new deoalWorld(*this, *world);
}

deBaseAudioSound *deAudioOpenAL::CreateSound(deSound *sound){
	return new deoalSound(*this, *sound);
}

deBaseAudioSpeaker *deAudioOpenAL::CreateSpeaker(deSpeaker *speaker){
	return new deoalSpeaker(*this, *speaker);
}

deBaseAudioMicrophone *deAudioOpenAL::CreateMicrophone(deMicrophone *microphone){
	return new deoalMicrophone(*this, *microphone);
}

deBaseAudioComponent *deAudioOpenAL::CreateComponent(deComponent *component){
	return new deoalComponent(*this, *component);
}

deBaseAudioModel *deAudioOpenAL::CreateModel(deModel *model){
	return new deoalModel(*this, *model);
}

deBaseAudioSkin *deAudioOpenAL::CreateSkin(deSkin *skin){
	return new deoalSkin(*this, *skin);
}

deBaseAudioDecal *deAudioOpenAL::CreateDecal(deDecal */*decal*/){
	return new deBaseAudioDecal;
}

deBaseAudioSoundLevelMeter *deAudioOpenAL::CreateSoundLevelMeter(deSoundLevelMeter *meter){
	return new deoalSoundLevelMeter(*this, *meter);
}

deBaseAudioVideoPlayer *deAudioOpenAL::CreateVideoPlayer(deVideoPlayer *videoPlayer){
	return new deoalVideoPlayer(*this, *videoPlayer);
}

deBaseAudioSynthesizerInstance *deAudioOpenAL::CreateSynthesizerInstance(deSynthesizerInstance *instance){
	return new deoalSynthesizerInstance(*this, *instance);
}

deBaseAudioHeightTerrain *deAudioOpenAL::CreateHeightTerrain(deHeightTerrain &heightTerrain){
	return nullptr;
}



// Notifications
//////////////////

void deAudioOpenAL::TexturePropertyMapChanged(){
}



// Parameters
///////////////

int deAudioOpenAL::GetParameterCount() const{
	return pParameters->GetParameterCount();
}

void deAudioOpenAL::GetParameterInfo(int index, deModuleParameter &info) const{
	info = pParameters->GetParameterAt(index);
}

int deAudioOpenAL::IndexOfParameterNamed(const char *name) const{
	return pParameters->IndexOfParameterNamed(name);
}

decString deAudioOpenAL::GetParameterValue(const char *name) const{
	return pParameters->GetParameterNamed(name).GetParameterValue();
}

void deAudioOpenAL::SetParameterValue(const char *name, const char *value){
	pParameters->GetParameterNamed(name).SetParameterValue(value);
}

void deAudioOpenAL::SendCommand(const decUnicodeArgumentList &command, decUnicodeString &answer){
	if(pCommandExecuter){
		pCommandExecuter->ExecuteCommand(command, answer);
		
	}else{
		answer.SetFromUTF8("Internal Error!");
	}
}

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

#ifndef MODULE_VERSION
#include "module_version.h"
#endif

class deoalModuleInternal : public deInternalModule{
public:
	typedef deTObjectReference<deoalModuleInternal> Ref;
	
	deoalModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("OpenAL");
		SetDescription("Ouputs audio using the OpenAL library.");
		SetAuthor("DragonDreams GmbH (info@dragondreams.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtAudio);
		SetDirectoryName("openal");
		SetPriority(1);
		SetDefaultLoggingName();
	}
	
	void CreateModule() override{
		SetModule(OpenALCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deTObjectReference<deInternalModule> deoalRegisterInternalModule(deModuleSystem *system){
	return deoalModuleInternal::Ref::NewWith(system);
}
#endif
