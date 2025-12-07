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

pSources(NULL),
pSourceCount(0),

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
	pCleanUp();
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
	return *((desynSynthesizerLink*)pLinks.GetAt(index));
}

void desynSynthesizer::AddLink(desynSynthesizerLink *link){
	pLinks.Add(link);
}



const desynSynthesizerSource &desynSynthesizer::GetSourceAt(int index) const{
	if(index < 0 || index >= pSourceCount){
		DETHROW(deeInvalidParam);
	}
	return *pSources[index];
}



void desynSynthesizer::Prepare(){
	if(!pDirtyContent){
		return;
	}
	
	deMutexGuard guard(pMutex);
	pClearSources();
	pClearLinks();
	
	pCreateLinks();
	pCreateSources();
	
	pDirtyContent = false;
}



void desynSynthesizer::InitStateData(char *stateData){
	int i;
	for(i=0; i<pSourceCount; i++){
		pSources[i]->InitStateData(stateData);
	}
}

void desynSynthesizer::CleanUpStateData(char *stateData){
	int i;
	for(i=0; i<pSourceCount; i++){
		pSources[i]->CleanUpStateData(stateData);
	}
}

void desynSynthesizer::GenerateSound(const desynSynthesizerInstance &instance,
char *stateData, float *buffer, int samples){
	if(pSilent){
		return;
	}
	
	deMutexGuard guard(pMutex);
	int i;
	for(i=0; i<pSourceCount; i++){
		pSources[i]->GenerateSound(instance, stateData, buffer, samples, 0.0f, 1.0f);
	}
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

void desynSynthesizer::pCleanUp(){
	deMutexGuard guard(pMutex);
	pClearSources();
	pClearLinks();
}



void desynSynthesizer::pClearLinks(){
	const int count = pLinks.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		delete (desynSynthesizerLink*)pLinks.GetAt(i);
	}
	
	pLinks.RemoveAll();
}

void desynSynthesizer::pCreateLinks(){
	pClearLinks();
	
	const int count = pSynthesizer.GetLinkCount();
	if(count == 0){
		return;
	}
	
	int i;
	for(i=0; i<count; i++){
		pLinks.Add(new desynSynthesizerLink(*pSynthesizer.GetLinkAt(i)));
	}
}

void desynSynthesizer::pCreateSources(){
	pSilent = true;
	pStateDataSize = 0;
	
	const int count = pSynthesizer.GetSourceCount();
	if(count == 0){
		return;
	}
	
	desynCreateSynthesizerSource createSource(*this, 0);
	pSources = new desynSynthesizerSource*[count];
	
	for(pSourceCount=0; pSourceCount<count; pSourceCount++){
		createSource.Reset();
		
		try{
			pSynthesizer.GetSourceAt(pSourceCount)->Visit(createSource);
			pSources[pSourceCount] = createSource.GetSource();
			
			if(!pSources[pSourceCount]->GetSilent()){
				pSilent = false;
			}
			
			pStateDataSize += pSources[pSourceCount]->StateDataSize(pStateDataSize);
			
		}catch(const deException &){
			if(createSource.GetSource()){
				delete createSource.GetSource();
			}
			throw;
		}
	}
}

void desynSynthesizer::pClearSources(){
	if(!pSources){
		return;
	}
	
	int i;
	for(i=0; i<pSourceCount; i++){
		delete pSources[i];
	}
	delete [] pSources;
	
	pSources = NULL;
	pSourceCount = 0;
	
	pStateDataSize = 0;
}
