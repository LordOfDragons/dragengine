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
#include "../../deEngine.h"
#include "../../common/file/decPath.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../common/file/decBaseFileWriter.h"
#include "../../common/exceptions.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deSynthesizerSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/synthesizer/deBaseSynthesizerSynthesizer.h"



// Class deSynthesizerManager
///////////////////////////////

// Constructor, destructor
////////////////////////////

deSynthesizerManager::deSynthesizerManager(deEngine *engine) :
deResourceManager(engine, ertSynthesizer){
	SetLoggingName("synthesizer");
}

deSynthesizerManager::~deSynthesizerManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deSynthesizerManager::GetSynthesizerCount() const{
	return pSynthesizers.GetCount();
}

deSynthesizer *deSynthesizerManager::GetRootSynthesizer() const{
	return (deSynthesizer*)pSynthesizers.GetRoot();
}

deSynthesizer::Ref deSynthesizerManager::CreateSynthesizer(){
	deSynthesizer::Ref synthesizer = NULL;
	
	try{
		synthesizer.TakeOver(new deSynthesizer(this));
		GetSynthesizerSystem()->LoadSynthesizer(synthesizer);
		
		pSynthesizers.Add(synthesizer);
		
	}catch(const deException &){
		throw;
	}
	
	return synthesizer;
}



void deSynthesizerManager::ReleaseLeakingResources(){
	const int count = GetSynthesizerCount();
	
	if(count > 0){
		LogWarnFormat("%i leaking synthesizers", count);
		pSynthesizers.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// systems support
////////////////////

void deSynthesizerManager::SystemSynthesizerLoad(){
	deSynthesizer::Ref synthesizer = (deSynthesizer*)pSynthesizers.GetRoot();
	deSynthesizerSystem &synthSys = *GetSynthesizerSystem();
	
	while(synthesizer){
		if(!synthesizer->GetPeerSynthesizer()){
			synthSys.LoadSynthesizer(synthesizer);
		}
		
		synthesizer = (deSynthesizer*)synthesizer->GetLLManagerNext();
	}
}

void deSynthesizerManager::SystemSynthesizerUnload(){
	deSynthesizer::Ref synthesizer = (deSynthesizer*)pSynthesizers.GetRoot();
	
	while(synthesizer){
		synthesizer->SetPeerSynthesizer(NULL);
		synthesizer = (deSynthesizer*)synthesizer->GetLLManagerNext();
	}
}



void deSynthesizerManager::RemoveResource(deResource *resource){
	pSynthesizers.RemoveIfPresent(resource);
}
