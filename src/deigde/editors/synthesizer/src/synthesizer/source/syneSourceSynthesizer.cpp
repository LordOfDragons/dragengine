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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "syneSourceSynthesizer.h"
#include "../syneSynthesizer.h"
#include "../controller/syneController.h"
#include "../link/syneLink.h"
#include "../../loadsave/syneLoadSaveSystem.h"
#include "../../gui/syneWindowMain.h"

#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/deSynthesizerManager.h>
#include <dragengine/resources/synthesizer/deSynthesizerLink.h>
#include <dragengine/resources/synthesizer/deSynthesizerController.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSource.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceSynthesizer.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceVisitorIdentify.h>
#include <dragengine/common/exceptions.h>



// Class syneSourceSynthesizer
//////////////////////////////

// Constructor, destructor
////////////////////////////

syneSourceSynthesizer::syneSourceSynthesizer(deEngine *engine) :
syneSource(deSynthesizerSourceVisitorIdentify::estSynthesizer),
pEngine(engine){
}

syneSourceSynthesizer::syneSourceSynthesizer(const syneSourceSynthesizer &copy) :
syneSource(copy),
pEngine(copy.pEngine),
pPathSynthesizer(copy.pPathSynthesizer),
pChildSynthesizer(copy.pChildSynthesizer),
pControllerNames(copy.pControllerNames),
pConnections(copy.pConnections){
}

syneSourceSynthesizer::~syneSourceSynthesizer(){
}



// Management
///////////////

void syneSourceSynthesizer::SetPathSynthesizer(const char *path){
	if(pPathSynthesizer == path){
		return;
	}
	
	pPathSynthesizer = path;
	UpdateChildSynthesizer();
	NotifySourceChanged();
}

void syneSourceSynthesizer::UpdateChildSynthesizer(){
	deSynthesizerSourceSynthesizer * const source = GetEngineSource().DynamicCast<deSynthesizerSourceSynthesizer>();
	syneSynthesizer::Ref synthesizer;
	
	pControllerNames.RemoveAll();
	pChildSynthesizer = nullptr;
	
	// try to load the synthesizer
	if(!pPathSynthesizer.IsEmpty() && GetSynthesizer()){
		const decString &basePath = GetSynthesizer()->GetDirectoryPath();
		
		try{
			// load from file
			synthesizer = GetSynthesizer()->GetLoadSaveSystem().LoadSynthesizer(pPathSynthesizer, basePath);
			
			// create synthesizer
			pChildSynthesizer = pEngine->GetSynthesizerManager()->CreateSynthesizer();
			
			// add controllers
			synthesizer->GetControllers().Visit([&](const syneController &controller){
				const deSynthesizerController::Ref engController(deSynthesizerController::Ref::New());
				engController->SetValueRange(controller.GetMinimumValue(), controller.GetMaximumValue());
				engController->SetClamp(controller.GetClamp());
				engController->SetCurve(controller.GetCurve());
				pChildSynthesizer->AddController(engController);
			});
			
			// add links
			synthesizer->GetLinks().Visit([&](const syneLink &link){
				const deSynthesizerLink::Ref engLink(deSynthesizerLink::Ref::New());
				
				if(link.GetController()){
					engLink->SetController(synthesizer->GetControllers().IndexOf(link.GetController()));
					
				}else{
					engLink->SetController(-1);
				}
				
				engLink->GetCurve() = link.GetCurve();
				pChildSynthesizer->AddLink(engLink);
			});
			
			// add sources
			synthesizer->GetSources().Visit([&](syneSource &child){
				pChildSynthesizer->AddSource(child.CreateEngineSource());
			});
			
			// update the controller name list
			const int controllerCount = synthesizer->GetControllers().GetCount();
			int i;
			for(i=0; i<controllerCount; i++){
				pControllerNames.Add(synthesizer->GetControllers().GetAt(i)->GetName());
			}
			
		}catch(const deException &){
			GetSynthesizer()->GetEnvironment()->GetLogger()->LogInfoFormat("Synthesizer Editor",
				"Failed to load child synthesizer '%s' (base directory '%s')", pPathSynthesizer.GetString(), basePath.GetString());
		}
	}
	
	// if the engine source exists and the sub synthesizer too assign it
	if(source){
		source->SetSynthesizer(pChildSynthesizer);
		pUpdateConnections(*source);
	}
}



void syneSourceSynthesizer::SetConnectionCount(int count){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count == pConnections.GetCount()){
		return;
	}

	deSynthesizerSourceSynthesizer *source = GetEngineSource().DynamicCast<deSynthesizerSourceSynthesizer>();
	ConnectionList connections;
	
	int i;
	for(i=0; i<count; i++){
		if(i < pConnections.GetCount()){
			connections.Add(pConnections[i]);
			
		}else{
			connections.Add(nullptr);
		}
	}
	
	pConnections = std::move(connections);
	
	if(source){
		pUpdateConnections(*source);
	}
	
	NotifySourceChanged();
}

void syneSourceSynthesizer::SetControllerAt(int position, syneController *controller){
	if(controller == pConnections.GetAt(position)){
		return;
	}
	
	pConnections.SetAt(position, controller);
	
	deSynthesizerSourceSynthesizer * const source = GetEngineSource().DynamicCast<deSynthesizerSourceSynthesizer>();
	if(source){
		pUpdateConnections(*source);
	}
	
	NotifySourceChanged();
}



deSynthesizerSource::Ref syneSourceSynthesizer::CreateEngineSource(){
	const deSynthesizerSourceSynthesizer::Ref engSource(deSynthesizerSourceSynthesizer::Ref::New());
	
	InitEngineSource(engSource);
	
	engSource->SetSynthesizer(pChildSynthesizer);
	
	pUpdateConnections(*engSource);
	
	return engSource;
}



// Operators
//////////////

syneSourceSynthesizer &syneSourceSynthesizer::operator=(const syneSourceSynthesizer &copy){
	SetPathSynthesizer(copy.pPathSynthesizer);
	
	if(pChildSynthesizer){
		pChildSynthesizer = nullptr;
	}
	pChildSynthesizer = copy.pChildSynthesizer;
	pControllerNames = copy.pControllerNames;
	pConnections = copy.pConnections;
	
	deSynthesizerSourceSynthesizer * const source = GetEngineSource().DynamicCast<deSynthesizerSourceSynthesizer>();
	if(source){
		pUpdateConnections(*source);
	}
	
	syneSource::operator=(copy);
	return *this;
}



syneSource::Ref syneSourceSynthesizer::CreateCopy() const{
	return syneSourceSynthesizer::Ref::New(*this);
}

void syneSourceSynthesizer::ListLinks(syneLink::List &list){
	syneSource::ListLinks(list);
}



void syneSourceSynthesizer::SynthesizerChanged(){
	syneSource::SynthesizerChanged();
	UpdateChildSynthesizer();
	NotifyEngineOnlySourceChanged();
}

void syneSourceSynthesizer::SynthesizerDirectoryChanged(){
	syneSource::SynthesizerDirectoryChanged();
	UpdateChildSynthesizer();
	NotifyEngineOnlySourceChanged();
}



// Private Functions
//////////////////////

void syneSourceSynthesizer::pUpdateConnections(deSynthesizerSourceSynthesizer &source) const{
	const int connectionCount = source.GetConnectionCount();
	int i;
	
	for(i=0; i<connectionCount; i++){
		int engController = -1;
		
		if(i < pConnections.GetCount() && pConnections.GetAt(i)){
			engController = pConnections.GetAt(i)->GetEngineControllerIndex();
		}
		
		source.SetConnectionAt(i, engController);
	}
}
