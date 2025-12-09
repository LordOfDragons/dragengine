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

#include "deSynthesizer.h"
#include "deSynthesizerInstance.h"
#include "deSynthesizerInstanceManager.h"
#include "deSynthesizerController.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/audio/deBaseAudioSynthesizerInstance.h"
#include "../../systems/modules/synthesizer/deBaseSynthesizerSynthesizerInstance.h"



// Class deSynthesizerInstance
////////////////////////////////

// Constructor, destructor
////////////////////////////

deSynthesizerInstance::deSynthesizerInstance(deSynthesizerInstanceManager *manager) :
deResource(manager),

pSampleCount(0),

pPeerSynthesizer(NULL),
pPeerAudio(NULL){
}

deSynthesizerInstance::~deSynthesizerInstance(){
	pCleanUp();
}



// Management
///////////////

void deSynthesizerInstance::SetSynthesizer(deSynthesizer *synthesizer){
	if(pSynthesizer == synthesizer){
		return;
	}
	
	pSynthesizer = synthesizer;
	
	if(synthesizer){
		pSampleCount = synthesizer->GetSampleCount();
		
	}else{
		pSampleCount = 0;
	}
	
	pUpdateControllers();
	
	if(pPeerSynthesizer){
		pPeerSynthesizer->SynthesizerChanged();
	}
	if(pPeerAudio){
		pPeerAudio->SynthesizerChanged();
	}
}



void deSynthesizerInstance::SetSampleCount(int sampleCount){
	sampleCount = decMath::max(sampleCount, 0);
	
	if(sampleCount == pSampleCount){
		return;
	}
	
	pSampleCount = sampleCount;
	
	if(pPeerSynthesizer){
		pPeerSynthesizer->PlayTimeChanged();
	}
	if(pPeerAudio){
		pPeerAudio->PlayTimeChanged();
	}
}



int deSynthesizerInstance::GetControllerCount() const{
	return pControllers.GetCount();
}

deSynthesizerController *deSynthesizerInstance::GetControllerAt(int index) const{
	return (deSynthesizerController*)pControllers.GetAt(index);
}

int deSynthesizerInstance::IndexOfControllerNamed(const char *name) const{
	const int count = pControllers.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((deSynthesizerController*)pControllers.GetAt(i))->GetName() == name){
			return i;
		}
	}
	
	return -1;
}

void deSynthesizerInstance::NotifyControllerChangedAt(int index){
	if(index < 0 || index >= pControllers.GetCount()){
		DETHROW(deeInvalidParam);
	}
	
	if(pPeerSynthesizer){
		pPeerSynthesizer->ControllerChanged(index);
	}
}



void deSynthesizerInstance::Reset(){
	if(pPeerSynthesizer){
		return pPeerSynthesizer->Reset();
	}
}

void deSynthesizerInstance::GenerateSound(void *buffer, int bufferSize, int offset, int samples){
	if(!pPeerSynthesizer){
		DETHROW(deeInvalidParam);
	}
	
	pPeerSynthesizer->GenerateSound(buffer, bufferSize, offset, samples);
}



// System Peers
/////////////////

void deSynthesizerInstance::SetPeerSynthesizer(deBaseSynthesizerSynthesizerInstance *peer){
	if(peer == pPeerSynthesizer){
		return;
	}
	
	if(pPeerSynthesizer){
		delete pPeerSynthesizer;
	}
	
	pPeerSynthesizer = peer;
}

void deSynthesizerInstance::SetPeerAudio(deBaseAudioSynthesizerInstance *peer){
	if(peer == pPeerAudio){
		return;
	}
	
	if(pPeerAudio){
		delete pPeerAudio;
	}
	
	pPeerAudio = peer;
}



// Private function
/////////////////////

void deSynthesizerInstance::pCleanUp(){
	if(pPeerAudio){
		delete pPeerAudio;
		pPeerAudio = NULL;
	}
	if(pPeerSynthesizer){
		delete pPeerSynthesizer;
		pPeerSynthesizer = NULL;
	}
}

void deSynthesizerInstance::pUpdateControllers(){
	pControllers.RemoveAll();
	
	if(!pSynthesizer){
		return;
	}
	
	const int controllerCount = pSynthesizer->GetControllerCount();
	int i;
	
	for(i=0; i<controllerCount; i++){
		pControllers.Add(deSynthesizerController::Ref::NewWith(*pSynthesizer->GetControllerAt(i)));
	}
}
