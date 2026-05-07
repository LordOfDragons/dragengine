/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "deAudioAnalyzer.h"
#include "deAudioAnalyzerManager.h"
#include "../../deEngine.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deSynthesizerSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/synthesizer/deBaseSynthesizerAudioAnalyzer.h"
#include "../../common/exceptions.h"


// Class deAudioAnalyzerManager
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deAudioAnalyzerManager::deAudioAnalyzerManager(deEngine *engine) :
deResourceManager(engine, ertAudioAnalyzer)
{
	SetLoggingName("audioAnalyzer");
}

deAudioAnalyzerManager::~deAudioAnalyzerManager(){
	ReleaseLeakingResources();
}


// Management
///////////////

int deAudioAnalyzerManager::GetAudioAnalyzerCount() const{
	return pAnalyzers.GetCount();
}

deAudioAnalyzer *deAudioAnalyzerManager::GetRootAudioAnalyzer() const{
	return static_cast<deAudioAnalyzer*>(pAnalyzers.GetRoot());
}

deAudioAnalyzer::Ref deAudioAnalyzerManager::CreateAudioAnalyzer(){
	auto analyzer = deAudioAnalyzer::Ref::New(this);
	GetSynthesizerSystem()->LoadAudioAnalyzer(analyzer);
	pAnalyzers.Add(analyzer);
	return analyzer;
}


void deAudioAnalyzerManager::ReleaseLeakingResources(){
	const int count = GetAudioAnalyzerCount();
	if(count > 0){
		LogWarnFormat("%d leaking audio analyzers", count);
		pAnalyzers.RemoveAll(); // we do not delete them to avoid crashes. better leak than crash
	}
}


// Systems Support
////////////////////

void deAudioAnalyzerManager::SystemSynthesizerLoad(){
	deSynthesizerSystem &synthSys = *GetSynthesizerSystem();
	pAnalyzers.GetResources().Visit([&](deResource *res){
		deAudioAnalyzer *analyzer = static_cast<deAudioAnalyzer*>(res);
		if(!analyzer->GetPeerSynthesizer()){
			synthSys.LoadAudioAnalyzer(analyzer);
		}
	});
}

void deAudioAnalyzerManager::SystemSynthesizerUnload(){
	pAnalyzers.GetResources().Visit([](deResource *res){
		static_cast<deAudioAnalyzer*>(res)->SetPeerSynthesizer({});
	});
}


void deAudioAnalyzerManager::RemoveResource(deResource *resource){
	pAnalyzers.RemoveIfPresent(resource);
}
