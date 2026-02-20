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

#include "desynSynthesizer.h"
#include "desynCreateSynthesizerSource.h"
#include "source/desynSynthesizerSource.h"
#include "desynSynthesizerLink.h"
#include "desynSynthesizerInstance.h"
#include "../deDESynthesizer.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSource.h>
#include <dragengine/threading/deMutexGuard.h>



// Class desynSynthesizer
///////////////////////////

// Constructor, destructor
////////////////////////////

desynSynthesizer::desynSynthesizer(deDESynthesizer &module, deSynthesizer &synthesizer) :
pModule(module),
pSynthesizer(synthesizer),

pSilent(true),
pStateDataSize(0),

pDirtyContent(true),

pChannelCount(synthesizer.GetChannelCount()),
pSampleRate(synthesizer.GetSampleRate()),
pBytesPerSample(synthesizer.GetBytesPerSample()),
pSampleCount(synthesizer.GetSampleCount()),

pUpdateTracker(0){
}

desynSynthesizer::~desynSynthesizer(){
	deMutexGuard guard(pMutex);
	pSources.RemoveAll();
	pLinks.RemoveAll();
}



// Management
///////////////

void desynSynthesizer::SetSilent(bool silent){
	pSilent = silent;
}

void desynSynthesizer::SetStateDataSize(int size){
	pStateDataSize = decMath::max(size, 0);
}



int desynSynthesizer::GetLinkCount() const{
	return pLinks.GetCount();
}

const desynSynthesizerLink &desynSynthesizer::GetLinkAt(int index) const{
	return *pLinks.GetAt(index);
}

void desynSynthesizer::AddLink(deTUniqueReference<desynSynthesizerLink> &&link){
	DEASSERT_NOTNULL(link)
	
	pLinks.Add(std::move(link));
}



const desynSynthesizerSource &desynSynthesizer::GetSourceAt(int index) const{
	return pSources.GetAt(index);
}



void desynSynthesizer::Prepare(){
	if(!pDirtyContent){
		return;
	}
	
	deMutexGuard guard(pMutex);
	pSources.RemoveAll();
	pLinks.RemoveAll();
	
	pSilent = true;
	pStateDataSize = 0;
	
	pCreateLinks();
	pCreateSources();
	
	pDirtyContent = false;
}



void desynSynthesizer::InitStateData(char *stateData){
	pSources.Visit([&](desynSynthesizerSource &s){
		s.InitStateData(stateData);
	});
}

void desynSynthesizer::CleanUpStateData(char *stateData){
	pSources.Visit([&](desynSynthesizerSource &s){
		s.CleanUpStateData(stateData);
	});
}

void desynSynthesizer::GenerateSound(const desynSynthesizerInstance &instance,
char *stateData, float *buffer, int samples){
	if(pSilent){
		return;
	}
	
	deMutexGuard guard(pMutex);
	pSources.Visit([&](desynSynthesizerSource &s){
		s.GenerateSound(instance, stateData, buffer, samples, 0.0f, 1.0f);
	});
}



// Notifications
//////////////////

void desynSynthesizer::ParametersChanged(){
	deMutexGuard guard(pMutex);
	pUpdateTracker++;
	pBytesPerSample = pSynthesizer.GetBytesPerSample();
	pChannelCount = pSynthesizer.GetChannelCount();
	pSampleRate = pSynthesizer.GetSampleRate();
	pSampleCount = pSynthesizer.GetSampleCount();
}

void desynSynthesizer::ControllersChanged(){
	deMutexGuard guard(pMutex);
	pUpdateTracker++;
}

void desynSynthesizer::LinksChanged(){
	if(pDirtyContent){
		return;
	}
	
	deMutexGuard guard(pMutex);
	pDirtyContent = true;
	pUpdateTracker++;
}

void desynSynthesizer::SourcesChanged(){
	if(pDirtyContent){
		return;
	}
	
	deMutexGuard guard(pMutex);
	pDirtyContent = true;
	pUpdateTracker++;
}



// Private Functions
//////////////////////

void desynSynthesizer::pCreateLinks(){
	pSynthesizer.GetLinks().Visit([&](const deSynthesizerLink &l){
		pLinks.Add(desynSynthesizerLink::Ref::New(l));
	});
}

void desynSynthesizer::pCreateSources(){
	if(pSynthesizer.GetSources().IsEmpty()){
		return;
	}
	
	desynCreateSynthesizerSource createSource(*this, 0);
	
	pSynthesizer.GetSources().Visit([&](deSynthesizerSource &s){
		createSource.Reset();
		
		s.Visit(createSource);
		pSources.Add(std::move(createSource.GetSource()));
		pStateDataSize += pSources.Last()->StateDataSize(pStateDataSize);
	});
	
	pSilent = pSources.AllMatching([](const desynSynthesizerSource &s){
		return s.GetSilent();
	});
}
