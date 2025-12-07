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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "desynSynthesizerInstance.h"
#include "desynSynthesizer.h"
#include "desynSynthesizerController.h"
#include "source/desynSynthesizerSource.h"
#include "../deDESynthesizer.h"
#include "../buffer/desynSharedBuffer.h"
#include "../buffer/desynSharedBufferList.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/deSynthesizerInstance.h>
#include <dragengine/threading/deMutexGuard.h>



// Class desynSynthesizerInstance
///////////////////////////////////

// Constructor, destructor
////////////////////////////

desynSynthesizerInstance::desynSynthesizerInstance(deDESynthesizer &module, deSynthesizerInstance &instance) :
pModule(module),
pSynthesizerInstance(instance),

pSynthesizer(NULL),
pSynthesizerUpdateTracker(0),

pControllers(NULL),
pControllerCount(0),

pChannelCount(1),
pSampleRate(11025),
pBytesPerSample(1),
pSampleCount(0),
pSilent(true),
pInverseSampleRate(1.0f),
pGenerateSampleSize(1),

pBufferSampleCount(0),
pBufferCount(0),

pDirtySynthesizer(true),
pDirtyControllers(false),
pDirtyFormat(true),

pStateData(NULL),
pStateDataSize(0)
{
	SynthesizerChanged();
}

desynSynthesizerInstance::~desynSynthesizerInstance(){
	deMutexGuard guard(pMutex);
	pCleanUp();
}



// Management
///////////////

desynSynthesizerController &desynSynthesizerInstance::GetControllerAt(int index) const{
	if(index < 0 || index >= pControllerCount){
		DETHROW(deeInvalidParam);
	}
	return pControllers[index];
}



// Notifications
//////////////////

void desynSynthesizerInstance::SynthesizerChanged(){
	deMutexGuard guard(pMutex);
	pFreeStateData();
	pClearControllers();
	
	if(pSynthesizerInstance.GetSynthesizer()){
		pSynthesizer = (desynSynthesizer*)pSynthesizerInstance.GetSynthesizer()->GetPeerSynthesizer();
		
	}else{
		pSynthesizer = NULL;
	}
	
	pSynthesizerUpdateTracker = 0;
	pDirtySynthesizer = true;
}

void desynSynthesizerInstance::ControllerChanged(int index){
	deMutexGuard guard(pMutex);
	if(index < 0 || index >= pControllerCount){
		return;
	}
	GetControllerAt(index).SetDirty(true);
	pDirtyControllers = true;
}

void desynSynthesizerInstance::PlayTimeChanged(){
	deMutexGuard guard(pMutex);
	pDirtyFormat = true;
	pDirtyControllers = true;
}



// Generate sound
///////////////////

void desynSynthesizerInstance::Reset(){
	deMutexGuard guard(pMutex);
	pFreeStateData();
	if(pSynthesizer){
		pCreateStateData();
	}
}

void desynSynthesizerInstance::GenerateSound(void *buffer, int bufferSize, int offset, int samples){
	if(samples < 0 || offset < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(samples == 0){
		return;
	}
	
	deMutexGuard guard(pMutex);
	pPrepare();
	
	if(! buffer || pGenerateSampleSize * samples != bufferSize){
		DETHROW(deeInvalidParam);
	}
	
	if(pSilent || ! pSynthesizer){
		pGenerateSilence(buffer, samples);
	}
	
	desynSharedBuffer *sharedBuffer = NULL;
	
	try{
		sharedBuffer = pModule.GetSharedBufferList().ClaimBuffer(samples * pChannelCount);
		
		pUpdateControllerValues(samples, offset);
		pGenerateSound(sharedBuffer, buffer, samples);
		
		pModule.GetSharedBufferList().ReleaseBuffer(sharedBuffer);
		
	}catch(const deException &){
		if(sharedBuffer){
			pModule.GetSharedBufferList().ReleaseBuffer(sharedBuffer);
		}
		throw;
	}
}



// Private Functions
//////////////////////

void desynSynthesizerInstance::pCleanUp(){
	pClearControllers();
}



void desynSynthesizerInstance::pPrepare(){
	if(pSynthesizer){
		deMutexGuard guard(pSynthesizer->GetMutex());
		const unsigned int updateTracker = pSynthesizer->GetUpdateTracker();
		if(updateTracker != pSynthesizerUpdateTracker){
			pSynthesizerUpdateTracker = updateTracker;
			pDirtySynthesizer = true;
		}
	}
	
	if(pDirtySynthesizer){
		pClearControllers();
		
		pFreeStateData();
		if(pSynthesizer){
			pSynthesizer->Prepare();
			pCreateStateData();
		}
		
		pCreateControllers();
		
		pDirtySynthesizer = false;
		pDirtyControllers = false;
		pDirtyFormat = true;
	}
	
	if(pDirtyControllers){
		int i;
		for(i=0; i<pControllerCount; i++){
			if(pControllers[i].GetDirty()){
				pControllers[i].Update(*pSynthesizerInstance.GetControllerAt(i));
			}
		}
		pDirtyControllers = false;
	}
	
	if(pDirtyFormat){
		pUpdateFormat();
		pDirtyFormat = false;
	}
}

void desynSynthesizerInstance::pUpdateFormat(){
	// NOTE mutex guarded by caller
	
	if(pSynthesizer){
		deMutexGuard guard(pSynthesizer->GetMutex());
		pChannelCount = pSynthesizer->GetChannelCount();
		pSampleRate = pSynthesizer->GetSampleRate();
		pBytesPerSample = pSynthesizer->GetBytesPerSample();
		pSilent = pSynthesizer->GetSilent();
		
	}else{
		// just to keep the data small
		pChannelCount = 1;
		pSampleRate = 11025;
		pBytesPerSample = 1;
		pSilent = true;
	}
	
	pSampleCount = pSynthesizerInstance.GetSampleCount();
	
	// clamp to supported values
	pChannelCount = decMath::clamp(pChannelCount, 1, 2);
	pBytesPerSample = decMath::clamp(pBytesPerSample, 1, 2);
	
	// convenience values
	pInverseSampleRate = 1.0f / (float)pSampleRate;
	pGenerateSampleSize = pBytesPerSample * pChannelCount;
	
	// determine streaming parameters
	pBufferSampleCount = pSampleRate / 20; // 50ms slices
	pBufferCount = 2; // 100ms buffered
}



void desynSynthesizerInstance::pClearControllers(){
	if(! pControllers){
		return;
	}
	
	delete [] pControllers;
	pControllers = NULL;
	pControllerCount = 0;
}

void desynSynthesizerInstance::pCreateControllers(){
	pClearControllers();
	
	const int count = pSynthesizerInstance.GetControllerCount();
	if(count == 0){
		return;
	}
	
	pControllers = new desynSynthesizerController[count];
	
	for(pControllerCount=0; pControllerCount<count; pControllerCount++){
		pControllers[pControllerCount].Update(*pSynthesizerInstance.GetControllerAt(pControllerCount));
	}
	
	pDirtyControllers = false;
}



void desynSynthesizerInstance::pGenerateSilence(void *buffer, int samples){
	if(pBytesPerSample == 1){
		if(pChannelCount == 1){
			sSampleMono8 * const sbuf = (sSampleMono8*)buffer;
			int i;
			
			for(i=0; i<samples; i++){
				sbuf[i].value = SAMPLE8_ZERO;
			}
			
		}else if(pChannelCount == 2){
			sSampleStereo8 * const sbuf = (sSampleStereo8*)buffer;
			int i;
			
			for(i=0; i<samples; i++){
				sbuf[i].left = SAMPLE8_ZERO;
				sbuf[i].right = SAMPLE8_ZERO;
			}
			
		}else{
			memset(buffer, 0, samples * pGenerateSampleSize);
		}
		
	}else if(pBytesPerSample == 2){
		if(pChannelCount == 1){
			sSampleMono16 * const sbuf = (sSampleMono16*)buffer;
			int i;
			
			for(i=0; i<samples; i++){
				sbuf[i].value = SAMPLE16_ZERO;
			}
			
		}else if(pChannelCount == 2){
			sSampleStereo16 * const sbuf = (sSampleStereo16*)buffer;
			int i;
			
			for(i=0; i<samples; i++){
				sbuf[i].left = SAMPLE16_ZERO;
				sbuf[i].right = SAMPLE16_ZERO;
			}
			
		}else{
			memset(buffer, 0, samples * pGenerateSampleSize);
		}
		
	}else{
		memset(buffer, 0, samples * pGenerateSampleSize);
	}
}

void desynSynthesizerInstance::pGenerateSound(desynSharedBuffer *sharedBuffer, void *buffer, int samples){
	// ensure generate buffer has enough size and clear it to 0
	float * const sharedBufferData = sharedBuffer->GetBuffer();
	memset(sharedBufferData, 0, sizeof(float) * (samples * pChannelCount));
	
	// generate sound into generate buffer
	pSynthesizer->GenerateSound(*this, pStateData, sharedBufferData, samples);
	
	// down-sample and clamp generate buffer into final buffer
	if(pBytesPerSample == 1){
		if(pChannelCount == 1){
			const desynSynthesizerSource::sGenerateBufferMono * const sbuf =
				(const desynSynthesizerSource::sGenerateBufferMono*)sharedBufferData;
			sSampleMono8 * const dbuf = (sSampleMono8*)buffer;
			int i;
			
			for(i=0; i<samples; i++){
				dbuf[i].value = (Sample8)decMath::clamp(sbuf[i].value
					* SAMPLE8_RANGE_F, SAMPLE8_MIN_F, SAMPLE8_MAX_F );
			}
			
		}else if(pChannelCount == 2){
			const desynSynthesizerSource::sGenerateBufferStereo * const sbuf =
				(const desynSynthesizerSource::sGenerateBufferStereo*)sharedBufferData;
			sSampleStereo8 * const dbuf = (sSampleStereo8*)buffer;
			int i;
			
			for(i=0; i<samples; i++){
				dbuf[i].left = (Sample8)decMath::clamp(sbuf[i].left
					* SAMPLE8_RANGE_F, SAMPLE8_MIN_F, SAMPLE8_MAX_F );
				dbuf[i].right = (Sample8)decMath::clamp(sbuf[i].right
					* SAMPLE8_RANGE_F, SAMPLE8_MIN_F, SAMPLE8_MAX_F );
			}
		}
		
	}else if(pBytesPerSample == 2){
		if(pChannelCount == 1){
			const desynSynthesizerSource::sGenerateBufferMono * const sbuf =
				(const desynSynthesizerSource::sGenerateBufferMono*)sharedBufferData;
			sSampleMono16 * const dbuf = (sSampleMono16*)buffer;
			int i;
			
			for(i=0; i<samples; i++){
				dbuf[i].value = (Sample16)decMath::clamp(sbuf[i].value
					* SAMPLE16_RANGE_F, SAMPLE16_MIN_F, SAMPLE16_MAX_F );
			}
			
		}else if(pChannelCount == 2){
			const desynSynthesizerSource::sGenerateBufferStereo * const sbuf =
				(const desynSynthesizerSource::sGenerateBufferStereo*)sharedBufferData;
			sSampleStereo16 * const dbuf = (sSampleStereo16*)buffer;
			int i;
			
			for(i=0; i<samples; i++){
				dbuf[i].left = (Sample16)decMath::clamp(sbuf[i].left
					* SAMPLE16_RANGE_F, SAMPLE16_MIN_F, SAMPLE16_MAX_F );
				dbuf[i].right = (Sample16)decMath::clamp(sbuf[i].right
					* SAMPLE16_RANGE_F, SAMPLE16_MIN_F, SAMPLE16_MAX_F );
			}
		}
	}
}



void desynSynthesizerInstance::pUpdateControllerValues(int samples, int offset){
	const float time = pInverseSampleRate * (float)offset;
	const float range = pInverseSampleRate * (float)1.0f;
	int i;
	
	for(i=0; i<pControllerCount; i++){
		pControllers[i].UpdateValues(samples, time, range);
	}
}



void desynSynthesizerInstance::pCreateStateData(){
	if(pStateData){
		DETHROW(deeInvalidParam);
	}
	
	if(! pSynthesizer){
		return;
	}
	
	deMutexGuard guard(pSynthesizer->GetMutex());
	pStateDataSize = pSynthesizer->GetStateDataSize();
	if(pStateDataSize == 0){
		return;
	}
	
	pStateData = new char[pStateDataSize];
	pSynthesizer->InitStateData(pStateData);
}

void desynSynthesizerInstance::pFreeStateData(){
	if(! pStateData){
		return;
	}
	if(! pSynthesizer){
		DETHROW(deeInvalidParam);
	}
	
	deMutexGuard guard(pSynthesizer->GetMutex());
	pSynthesizer->CleanUpStateData(pStateData);
	delete [] pStateData;
	pStateData = NULL;
	pStateDataSize = 0;
}
