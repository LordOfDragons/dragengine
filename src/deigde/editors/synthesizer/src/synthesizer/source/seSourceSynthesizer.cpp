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

#include "seSourceSynthesizer.h"
#include "../seSynthesizer.h"
#include "../controller/seController.h"
#include "../link/seLink.h"
#include "../../loadsave/seLoadSaveSystem.h"
#include "../../gui/seWindowMain.h"

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



// Class seSourceSynthesizer
//////////////////////////////

// Constructor, destructor
////////////////////////////

seSourceSynthesizer::seSourceSynthesizer(deEngine *engine) :
seSource(deSynthesizerSourceVisitorIdentify::estSynthesizer),
pEngine(engine){
}

seSourceSynthesizer::seSourceSynthesizer(const seSourceSynthesizer &copy) :
seSource(copy),
pEngine(copy.pEngine),
pPathSynthesizer(copy.pPathSynthesizer),
pChildSynthesizer(copy.pChildSynthesizer),
pControllerNames(copy.pControllerNames),
pConnections(copy.pConnections){
}

seSourceSynthesizer::~seSourceSynthesizer(){
}



// Management
///////////////

void seSourceSynthesizer::SetPathSynthesizer(const char *path){
	if(pPathSynthesizer == path){
		return;
	}
	
	pPathSynthesizer = path;
	UpdateChildSynthesizer();
	NotifySourceChanged();
}

void seSourceSynthesizer::UpdateChildSynthesizer(){
	deSynthesizerSourceSynthesizer * const source = GetEngineSource().DynamicCast<deSynthesizerSourceSynthesizer>();
	seSynthesizer::Ref synthesizer;
	
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
			synthesizer->GetControllers().Visit([&](const seController &controller){
				const deSynthesizerController::Ref engController(deSynthesizerController::Ref::New());
				engController->SetValueRange(controller.GetMinimumValue(), controller.GetMaximumValue());
				engController->SetClamp(controller.GetClamp());
				engController->SetCurve(controller.GetCurve());
				pChildSynthesizer->AddController(engController);
			});
			
			// add links
			synthesizer->GetLinks().Visit([&](const seLink &link){
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
			synthesizer->GetSources().Visit([&](seSource &child){
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



void seSourceSynthesizer::SetConnectionCount(int count){
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

void seSourceSynthesizer::SetControllerAt(int position, seController *controller){
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



deSynthesizerSource::Ref seSourceSynthesizer::CreateEngineSource(){
	const deSynthesizerSourceSynthesizer::Ref engSource(deSynthesizerSourceSynthesizer::Ref::New());
	
	InitEngineSource(engSource);
	
	engSource->SetSynthesizer(pChildSynthesizer);
	
	pUpdateConnections(*engSource);
	
	return engSource;
}



// Operators
//////////////

seSourceSynthesizer &seSourceSynthesizer::operator=(const seSourceSynthesizer &copy){
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
	
	seSource::operator=(copy);
	return *this;
}



seSource::Ref seSourceSynthesizer::CreateCopy() const{
	return seSourceSynthesizer::Ref::New(*this);
}

void seSourceSynthesizer::ListLinks(seLink::List &list){
	seSource::ListLinks(list);
}



void seSourceSynthesizer::SynthesizerChanged(){
	seSource::SynthesizerChanged();
	UpdateChildSynthesizer();
	NotifyEngineOnlySourceChanged();
}

void seSourceSynthesizer::SynthesizerDirectoryChanged(){
	seSource::SynthesizerDirectoryChanged();
	UpdateChildSynthesizer();
	NotifyEngineOnlySourceChanged();
}



// Private Functions
//////////////////////

void seSourceSynthesizer::pUpdateConnections(deSynthesizerSourceSynthesizer &source) const{
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
