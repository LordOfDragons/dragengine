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

#include "deoalSource.h"
#include "deoalSourceManager.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATLogger.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>



// #define DO_DEBUG 1

#ifdef DO_DEBUG
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif



// Class deoalSourceManager
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoalSourceManager::deoalSourceManager(deoalAudioThread &audioThread) :
pAudioThread(audioThread),
pCountBound(0),
pCountUnbound(0),
pEstimatedMaxBound(10000) // just some large number
{
}

deoalSourceManager::~deoalSourceManager(){
	const int count = pSources.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		delete (deoalSource*)pSources.GetAt(i);
	}
	pSources.RemoveAll();
}



// Management
///////////////

int deoalSourceManager::GetSourceCount() const{
	return pSources.GetCount();
}

deoalSource *deoalSourceManager::GetSourceAt(int index) const{
	return (deoalSource*)pSources.GetAt(index);
}



deoalSource *deoalSourceManager::BindSource(void *owner){
	if(! owner){
		DETHROW(deeInvalidParam);
	}
	
	// use next free unbound source
	deoalSource *source = pNextUnboundSource();
	if(source){
		DEBUG(pAudioThread.GetLogger().LogInfoFormat(
			"Bind previously unbound source %i to %p (%i,%i,%i)",
			pSources.IndexOf(source), owner, pSources.GetCount(), pCountBound, pCountUnbound));
		source->SetOwner(owner);
		pCountBound++;
		pCountUnbound--;
		return source;
	}
	
	// create a new source if possible
	source = pCreateNewSource();
	if(source){
		DEBUG(pAudioThread.GetLogger().LogInfoFormat(
			"Created new source %i bound to %p (%i,%i,%i)", pSources.IndexOf(source),
			owner, pSources.GetCount(), pCountBound, pCountUnbound));
		source->SetOwner(owner);
		pCountBound++;
		pCountUnbound--;
		return source;
	}
	
	// if no more sources can be created rebind the source with the smallest importance
	source = pBestRebindableSource();
	if(source){
		DEBUG(pAudioThread.GetLogger().LogInfoFormat(
			"Rebind previously bound source %i from %p to %p (%i,%i,%i)",
			pSources.IndexOf(source), source->GetOwner(), owner,
			pSources.GetCount(), pCountBound, pCountUnbound));
		source->Reset();
		source->SetOwner(owner);
		return source;
	}
	
	// this should never happen. if we end up here the hardware is broken
	DETHROW(deeInvalidAction);
}

void deoalSourceManager::UnbindSource(deoalSource *source){
	if(! source || ! source->GetOwner()){
		DETHROW(deeInvalidParam);
	}
	
	DEBUG(pAudioThread.GetLogger().LogInfoFormat(
		"Unbind source %i from %p (%i,%i,%i)", pSources.IndexOf(source),
		source->GetOwner(), pSources.GetCount(), pCountBound, pCountUnbound));
	
	source->Reset();
	
	pCountBound--;
	pCountUnbound++;
}



void deoalSourceManager::DebugOutput(decUnicodeString&){
}

void deoalSourceManager::PrintStatusLine(){
	pAudioThread.GetLogger().LogInfoFormat(
		"SourceManager: %i sources, %i bound, %i unbound",
		pSources.GetCount(), pCountBound, pCountUnbound);
}



// Private Functions
//////////////////////

deoalSource *deoalSourceManager::pNextUnboundSource() const{
	const int count = pSources.GetCount();
	deoalSource *source;
	int i;
	
	for(i=0; i<count; i++){
		source = (deoalSource*)pSources.GetAt(i);
		if(! source->GetOwner()){
			return source;
		}
	}
	
	return NULL;
}

deoalSource *deoalSourceManager::pBestRebindableSource() const{
	const int count = pSources.GetCount();
	deoalSource *bestSource = NULL;
	deoalSource *source;
	int i;
	
	for(i=0; i<count; i++){
		source = (deoalSource*)pSources.GetAt(i);
		if(! source->GetOwner()){
			continue;
		}
		if(! bestSource || source->GetImportance() < bestSource->GetImportance()){
			bestSource = source;
		}
	}
	
	return bestSource;
}

deoalSource *deoalSourceManager::pCreateNewSource(){
	// check if we are over our estimated maximum of active sources. this is only an estimate
	// and we might go higher but this is the first number where we ran into a problem
	if(pSources.GetCount() >= pEstimatedMaxBound){
		return NULL;
	}
	
	// try creating a new source
	deoalSource *source = NULL;
	
	try{
		source = new deoalSource(pAudioThread);
		pSources.Add(source);
		pCountUnbound++;
		
	}catch(const deException &){
		// failure usually means an AL_OUT_OF_MEMORY or similar has been thrown. this is the
		// case if the underlaying hardware can not create any more source. in this case
		// return NULL to rebind an existing source as fallback
		pAudioThread.GetLogger().LogWarnFormat(
			"SourceManager: Can not create any more sources (%i sources right now)",
			pSources.GetCount());
		if(source){
			delete source;
			source = NULL;
		}
		
		// keep track of the highest number of sources as an estimate of the maximum
		// number of sources we can use
		pEstimatedMaxBound = pSources.GetCount();
	}
	
	return source;
}
