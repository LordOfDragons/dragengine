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

#include "deSynthesizer.h"
#include "deSynthesizerManager.h"
#include "deSynthesizerLink.h"
#include "deSynthesizerController.h"
#include "source/deSynthesizerSource.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/synthesizer/deBaseSynthesizerSynthesizer.h"



// Class deSynthesizer
////////////////////////

// Constructor, destructor
////////////////////////////

deSynthesizer::deSynthesizer(deSynthesizerManager *manager) :
deResource(manager),
pChannelCount(1),
pSampleRate(44100),
pBytesPerSample(2),
pSampleCount(pSampleRate * 10),
pPeerSynthesizer(nullptr){
}

deSynthesizer::~deSynthesizer(){
	if(pPeerSynthesizer){
		delete pPeerSynthesizer;
		pPeerSynthesizer = nullptr;
	}
	
	RemoveAllSources();
	RemoveAllLinks();
	RemoveAllControllers();
}



// Management
///////////////

void deSynthesizer::SetChannelCount(int channelCount){
	if(channelCount < 1){
		DETHROW(deeInvalidParam);
	}
	
	if(channelCount == pChannelCount){
		return;
	}
	
	pChannelCount = channelCount;
	
	if(pPeerSynthesizer){
		pPeerSynthesizer->ParametersChanged();
	}
}

void deSynthesizer::SetSampleRate(int sampleRate){
	if(sampleRate < 1){
		DETHROW(deeInvalidParam);
	}
	
	if(sampleRate == pSampleRate){
		return;
	}
	
	pSampleRate = sampleRate;
	
	if(pPeerSynthesizer){
		pPeerSynthesizer->ParametersChanged();
	}
}

void deSynthesizer::SetBytesPerSample(int bytesPerSample){
	if(bytesPerSample < 1){
		DETHROW(deeInvalidParam);
	}
	
	if(bytesPerSample == pBytesPerSample){
		return;
	}
	
	pBytesPerSample = bytesPerSample;
	
	if(pPeerSynthesizer){
		pPeerSynthesizer->ParametersChanged();
	}
}

void deSynthesizer::SetSampleCount(int sampleCount){
	if(sampleCount < 1){
		DETHROW(deeInvalidParam);
	}
	
	if(sampleCount == pSampleCount){
		return;
	}
	
	pSampleCount = sampleCount;
	
	if(pPeerSynthesizer){
		pPeerSynthesizer->ParametersChanged();
	}
}



// Controllers
////////////////

int deSynthesizer::IndexOfControllerNamed(const char *name) const{
	const int count = pControllers.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((deSynthesizerController*)pControllers.GetAt(i))->GetName() == name){
			return i;
		}
	}
	
	return -1;
}

void deSynthesizer::AddController(deSynthesizerController *controller){
	DEASSERT_NOTNULL(controller)
	pControllers.AddOrThrow(controller);
	
	if(pPeerSynthesizer){
		pPeerSynthesizer->ControllersChanged();
	}
}

void deSynthesizer::RemoveController(deSynthesizerController *controller){
	pControllers.RemoveOrThrow(controller);
	
	if(pPeerSynthesizer){
		pPeerSynthesizer->ControllersChanged();
	}
}

void deSynthesizer::RemoveAllControllers(){
	if(pControllers.IsEmpty()){
		return;
	}
	
	pControllers.RemoveAll();
	
	if(pPeerSynthesizer){
		pPeerSynthesizer->ControllersChanged();
	}
}

void deSynthesizer::NotifyControllersChanged(){
	if(pPeerSynthesizer){
		pPeerSynthesizer->ControllersChanged();
	}
}



// Links
//////////

void deSynthesizer::AddLink(deSynthesizerLink *link){
	DEASSERT_NOTNULL(link)
	pLinks.AddOrThrow(link);
	
	if(pPeerSynthesizer){
		pPeerSynthesizer->LinksChanged();
	}
}

void deSynthesizer::RemoveLink(deSynthesizerLink *link){
	pLinks.RemoveOrThrow(link);
	
	if(pPeerSynthesizer){
		pPeerSynthesizer->LinksChanged();
	}
}

void deSynthesizer::RemoveAllLinks(){
	if(pLinks.IsEmpty()){
		return;
	}
	
	pLinks.RemoveAll();
	
	if(pPeerSynthesizer){
		pPeerSynthesizer->LinksChanged();
	}
}

void deSynthesizer::NotifyLinksChanged(){
	if(pPeerSynthesizer){
		pPeerSynthesizer->LinksChanged();
	}
}



// Sources
////////////

void deSynthesizer::AddSource(deSynthesizerSource *source){
	DEASSERT_NOTNULL(source)
	pSources.AddOrThrow(source);
	
	if(pPeerSynthesizer){
		pPeerSynthesizer->SourcesChanged();
	}
}

void deSynthesizer::RemoveSource(deSynthesizerSource *source){
	pSources.RemoveOrThrow(source);
	
	if(pPeerSynthesizer){
		pPeerSynthesizer->SourcesChanged();
	}
}

void deSynthesizer::RemoveAllSources(){
	pSources.RemoveAll();
	
	if(pPeerSynthesizer){
		pPeerSynthesizer->SourcesChanged();
	}
}

void deSynthesizer::NotifySourcesChanged(){
	if(pPeerSynthesizer){
		pPeerSynthesizer->SourcesChanged();
	}
}



// System Peers
/////////////////

void deSynthesizer::SetPeerSynthesizer(deBaseSynthesizerSynthesizer *peer){
	if(peer == pPeerSynthesizer){
		return;
	}
	
	if(pPeerSynthesizer){
		delete pPeerSynthesizer;
	}
	pPeerSynthesizer = peer;
}
