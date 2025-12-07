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

#include "deoalAudioThread.h"
#include "deoalATContext.h"
#include "deoalATLogger.h"
#include "../deoalBasics.h"
#include "../capabilities/deoalCapabilities.h"
#include "../component/deoalComponent.h"
#include "../extensions/deoalExtensions.h"

#include <dragengine/common/exceptions.h>



// Class deoalATContext
/////////////////////////

// Constructor, destructor
////////////////////////////

deoalATContext::deoalATContext(deoalAudioThread &audioThread) :
pAudioThread(audioThread),
pDevice(nullptr),
pContext(nullptr){
}

deoalATContext::~deoalATContext(){
}



// Management
///////////////

void deoalATContext::OpenDevice(){
	const decString &deviceName = pAudioThread.GetConfiguration().GetDeviceName();
	deoalATLogger &logger = pAudioThread.GetLogger();
	
	pScanForDevices();
	
	if(deviceName.IsEmpty()){
		logger.LogInfo("Open default device");
		pDevice = alcOpenDevice(nullptr);
		
	}else{
		logger.LogInfoFormat("Open device '%s' instead of default", deviceName.GetString());
		pDevice = alcOpenDevice(deviceName);
	}
	
	if(!pDevice){
		logger.LogErrorFormat("alcOpenDevice(%s) failed", deviceName.GetString());
		DETHROW(deeInvalidParam);
	}
}

void deoalATContext::CreateContext(){
	// create context. the attributes are hints. after creating it should be possible
	// to query the actually supported values. ALSoft seems to accept all values without
	// using them anyhow.
	deoalExtensions &extensions = pAudioThread.GetExtensions();
	deoalATLogger &logger = pAudioThread.GetLogger();
	
	int index = 0;
	ALCint attributes[9];
	
	attributes[index++] = ALC_MONO_SOURCES;
	attributes[index++] = 1024 * 8;
	
	attributes[index++] = ALC_STEREO_SOURCES;
	attributes[index++] = 1024 * 8;
	
	if(extensions.GetHasEFX()){
		attributes[index++] = ALC_MAX_AUXILIARY_SENDS;
		attributes[index++] = 1;
			// a send can have both an effect and a filter. to do environment simulation
			// it is enough to have a reverb effect and a low-pass filter. thus 1 send
			// is enough to simulate 1 indirect sound path
	}
	
	if(extensions.GetHasHRTF()){
		attributes[index++] = ALC_HRTF_SOFT;
		attributes[index++] = ALC_DONT_CARE_SOFT;
			// let driver choose when to use HRTF. we can force it with ALC_TRUE but then
			// it is possible the user is using a surround system where HRTF is not good.
			// we check later on though if HRTF is currently in use.
	}
	
	//ALC_FREQUENCY = 48000 // Hz
	//ALC_REFRESH = 50 // Hz => ignored by OpenALSoft
 
	
	attributes[index++] = ALC_INVALID;
	
	pContext = alcCreateContext(pDevice, attributes);
	if(!pContext){
		logger.LogError("alcCreateContext failed");
		DETHROW(deeInvalidParam);
	}
	
	// make the context current
	if(alcMakeContextCurrent(pContext) == AL_FALSE){
		logger.LogError("alcMakeContextCurrent failed");
		DETHROW(deeInvalidParam);
	}
}

void deoalATContext::LogContextInfo(){
	deoalATLogger &logger = pAudioThread.GetLogger();
	ALint frequency;
	ALint refresh;
	
	OAL_CHECK(pAudioThread, alcGetIntegerv(pDevice, ALC_FREQUENCY, 1, &frequency));
	OAL_CHECK(pAudioThread, alcGetIntegerv(pDevice, ALC_REFRESH, 1, &refresh));
	
	logger.LogInfoFormat("Context: Mixing Frequency = %i Hz", frequency);
	logger.LogInfoFormat("Context: Refresh Interval = %i Hz", refresh);
}

void deoalATContext::CleanUp(){
	if(pContext){
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(pContext);
		pContext = nullptr;
	}
	
	if(pDevice){
		alcCloseDevice(pDevice);
		pDevice = nullptr;
	}
}



// Private Functions
//////////////////////

void deoalATContext::pScanForDevices(){
	const ALCchar *defaultDevice = nullptr;
	const ALCchar *devices = nullptr;
	int position, len;
	
	// query for the information
	defaultDevice = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
	
	if(alcIsExtensionPresent(nullptr, "ALC_ENUMERATE_ALL_EXT") == AL_TRUE){
		defaultDevice = alcGetString(nullptr, ALC_DEFAULT_ALL_DEVICES_SPECIFIER);
		devices = alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER);
		
	}else if(alcIsExtensionPresent(nullptr, "ALC_ENUMERATION_EXT") == AL_TRUE){
		devices = alcGetString(nullptr, ALC_DEVICE_SPECIFIER);
	}
	
	// log devices
	deoalATLogger &logger = pAudioThread.GetLogger();
	
	if(devices) {
		logger.LogInfo("devices:");
		position = 0;
		while(devices[position]){
			len = (int)strlen(devices + position);
			if(len > 0){
				logger.LogInfoFormat("- %s", devices + position);
				position += len + 1;
			}
		}
		
	}else{
		logger.LogError("devices(all): < query failed >");
	}
	
	logger.LogInfoFormat("default device: '%s'", defaultDevice ? defaultDevice : "< query failed >");
}
