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
#include "dragengine/resources/synthesizer/deSynthesizerLink.h"
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
pEngine(engine),
pChildSynthesizer(NULL),
pControllerNames(NULL),
pControllerNameCount(0),
pConnections(NULL),
pConnectionCount(0){
}

seSourceSynthesizer::seSourceSynthesizer(const seSourceSynthesizer &copy) :
seSource(copy),
pEngine(copy.pEngine),
pPathSynthesizer(copy.pPathSynthesizer),
pChildSynthesizer(copy.pChildSynthesizer),
pControllerNames(NULL),
pControllerNameCount(0),
pConnections(NULL),
pConnectionCount(0)
{
	if(pChildSynthesizer){
		pChildSynthesizer->AddReference();
	}
	
	const int controllerNamesCount = copy.pControllerNameCount;
	if(controllerNamesCount > 0){
		pControllerNames = new decString[controllerNamesCount];
		pControllerNameCount = controllerNamesCount;
		
		int i;
		for(i=0; i<controllerNamesCount; i++){
			pControllerNames[i] = copy.pControllerNames[i];
		}
	}
	
	const int connectionCount = copy.pConnectionCount;
	if(connectionCount > 0){
		pConnections = new seController*[connectionCount];
		pConnectionCount = connectionCount;
		
		int i;
		for(i=0; i<connectionCount; i++){
			pConnections[i] = copy.pConnections[i];
			if(pConnections[i]){
				pConnections[i]->AddReference();
			}
		}
	}
}

seSourceSynthesizer::~seSourceSynthesizer(){
	if(pConnections){
		while(pConnectionCount > 0){
			pConnectionCount--;
			
			if(pConnections[pConnectionCount]){
				pConnections[pConnectionCount]->FreeReference();
			}
		}
		
		delete [] pConnections;
	}
	
	if(pControllerNames){
		delete [] pControllerNames;
	}
	
	if(pChildSynthesizer){
		pChildSynthesizer->FreeReference();
	}
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
	deSynthesizerSourceSynthesizer * const source = (deSynthesizerSourceSynthesizer*)GetEngineSource();
	deSynthesizerController *engController = NULL;
	deSynthesizerLink *engLink = NULL;
	deSynthesizerSource *engSource = NULL;
	seSynthesizer *synthesizer = NULL;
	int c, l, s;
	
	// free controller name list
	if(pControllerNames){
		delete [] pControllerNames;
		pControllerNames = NULL;
		pControllerNameCount = 0;
	}
	
	// release the sub synthesizer
	if(pChildSynthesizer){
		pChildSynthesizer->FreeReference();
		pChildSynthesizer = NULL;
	}
	
	// try to load the synthesizer
	if(!pPathSynthesizer.IsEmpty() && GetSynthesizer()){
		const decString &basePath = GetSynthesizer()->GetDirectoryPath();
		
		try{
			// load from file
			synthesizer = GetSynthesizer()->GetLoadSaveSystem().LoadSynthesizer(pPathSynthesizer, basePath);
			
			// create synthesizer
			pChildSynthesizer = pEngine->GetSynthesizerManager()->CreateSynthesizer();
			
			// add controllers
			const int controllerCount = synthesizer->GetControllers().GetCount();
			for(c=0; c<controllerCount; c++){
				const seController &controller = *synthesizer->GetControllers().GetAt(c);
				
				engController = new deSynthesizerController;
				engController->SetValueRange(controller.GetMinimumValue(), controller.GetMaximumValue());
				engController->SetClamp(controller.GetClamp());
				engController->SetCurve(controller.GetCurve());
				
				pChildSynthesizer->AddController(engController);
				engController = NULL;
			}
			
			// add links
			const int linkCount = synthesizer->GetLinks().GetCount();
			for(l=0; l<linkCount; l++){
				const seLink &link = *synthesizer->GetLinks().GetAt(l);
				
				engLink = new deSynthesizerLink;
				
				if(link.GetController()){
					engLink->SetController(synthesizer->GetControllers().IndexOf(link.GetController()));
					
				}else{
					engLink->SetController(-1);
				}
				
				engLink->GetCurve() = link.GetCurve();
				
				pChildSynthesizer->AddLink(engLink);
				engLink = NULL;
			}
			
			// add sources
			const int sourceCount = synthesizer->GetSources().GetCount();
			for(s=0; s<sourceCount; s++){
				engSource = synthesizer->GetSources().GetAt(s)->CreateEngineSource();
				
				pChildSynthesizer->AddSource(engSource);
				engSource->FreeReference();
				engSource = NULL;
			}
			
			// update the controller name list
			if(controllerCount > 0){
				pControllerNames = new decString[controllerCount];
				pControllerNameCount = controllerCount;
				for(c=0; c<controllerCount; c++){
					pControllerNames[c] = synthesizer->GetControllers().GetAt(c)->GetName();
				}
			}
			
			// free the loaded synthesizer as it is no more needed
			synthesizer->FreeReference();
			
		}catch(const deException &){
			GetSynthesizer()->GetEnvironment()->GetLogger()->LogInfoFormat("Synthesizer Editor",
				"Failed to load child synthesizer '%s' (base directory '%s')", pPathSynthesizer.GetString(), basePath.GetString());
			
			if(engSource){
				engSource->FreeReference();
			}
			if(engLink){
				engLink->FreeReference();
			}
			if(engController){
				engController->FreeReference();
			}
			if(synthesizer){
				synthesizer->FreeReference();
			}
		}
	}
	
	// if the engine source exists and the sub synthesizer too assign it
	if(source){
		source->SetSynthesizer(pChildSynthesizer);
		pUpdateConnections(*source);
	}
}



const decString &seSourceSynthesizer::GetControllerNameAt(int position) const{
	if(position < 0 || position >= pControllerNameCount){
		DETHROW(deeInvalidParam);
	}
	return pControllerNames[position];
}



void seSourceSynthesizer::SetConnectionCount(int count){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count == pConnectionCount){
		return;
	}
	
	deSynthesizerSourceSynthesizer *source = (deSynthesizerSourceSynthesizer*)GetEngineSource();
	seController **newArray = NULL;
	int c;
	
	if(count > 0){
		newArray = new seController*[count];
	}
	
	if(count > pConnectionCount){
		for(c=0; c<pConnectionCount; c++){
			newArray[c] = pConnections[c];
		}
		for(c=pConnectionCount; c<count; c++){
			newArray[c] = NULL;
		}
		
	}else{
		for(c=0; c<count; c++){
			newArray[c] = pConnections[c];
		}
		for(c=count; c<pConnectionCount; c++){
			if(pConnections[c]){
				pConnections[c]->FreeReference();
			}
		}
	}
	
	if(pConnections){
		delete [] pConnections;
	}
	
	pConnections = newArray;
	pConnectionCount = count;
	
	if(source){
		pUpdateConnections(*source);
	}
	
	NotifySourceChanged();
}

seController *seSourceSynthesizer::GetControllerAt(int position) const{
	if(position < 0 || position >= pConnectionCount){
		DETHROW(deeInvalidParam);
	}
	return pConnections[position];
}

void seSourceSynthesizer::SetControllerAt(int position, seController *controller){
	if(position < 0 || position >= pConnectionCount){
		DETHROW(deeInvalidParam);
	}
	
	if(controller == pConnections[position]){
		return;
	}
	
	deSynthesizerSourceSynthesizer *source = (deSynthesizerSourceSynthesizer*)GetEngineSource();
	
	if(pConnections[position]){
		pConnections[position]->FreeReference();
	}
	
	pConnections[position] = controller;
	
	if(controller){
		controller->AddReference();
	}
	
	if(source){
		pUpdateConnections(*source);
	}
	
	NotifySourceChanged();
}



deSynthesizerSource *seSourceSynthesizer::CreateEngineSource(){
	deSynthesizerSourceSynthesizer *engSource = NULL;
	
	try{
		engSource = new deSynthesizerSourceSynthesizer;
		
		InitEngineSource(engSource);
		
		engSource->SetSynthesizer(pChildSynthesizer);
		
		pUpdateConnections(*engSource);
		
	}catch(const deException &){
		if(engSource){
			engSource->FreeReference();
		}
		throw;
	}
	
	return engSource;
}



// Operators
//////////////

seSourceSynthesizer &seSourceSynthesizer::operator=(const seSourceSynthesizer &copy){
	SetPathSynthesizer(copy.pPathSynthesizer);
	
	if(pChildSynthesizer){
		pChildSynthesizer->FreeReference();
		pChildSynthesizer = NULL;
	}
	pChildSynthesizer = copy.pChildSynthesizer;
	if(pChildSynthesizer){
		pChildSynthesizer->AddReference();
	}
	
	if(pControllerNames){
		delete [] pControllerNames;
		pControllerNames = NULL;
		pControllerNameCount = 0;
	}
	
	if(pConnections){
		int i;
		for(i=0; i<pConnectionCount; i++){
			if(pConnections[i]){
				pConnections[i]->FreeReference();
			}
		}
		delete [] pConnections;
		pConnections = NULL;
		pConnectionCount = 0;
	}
	
	const int controllerNamesCount = copy.pControllerNameCount;
	if(controllerNamesCount > 0){
		pControllerNames = new decString[controllerNamesCount];
		pControllerNameCount = controllerNamesCount;
		
		int i;
		for(i=0; i<controllerNamesCount; i++){
			pControllerNames[i] = copy.pControllerNames[i];
		}
	}
	
	const int connectionCount = copy.pConnectionCount;
	if(connectionCount > 0){
		pConnections = new seController*[connectionCount];
		pConnectionCount = connectionCount;
		
		int i;
		for(i=0; i<connectionCount; i++){
			pConnections[i] = copy.pConnections[i];
			if(pConnections[i]){
				pConnections[i]->AddReference();
			}
		}
	}
	
	deSynthesizerSourceSynthesizer * const source = (deSynthesizerSourceSynthesizer*)GetEngineSource();
	if(source){
		pUpdateConnections(*source);
	}
	
	seSource::operator=(copy);
	return *this;
}



seSource *seSourceSynthesizer::CreateCopy() const{
	return new seSourceSynthesizer(*this);
}

void seSourceSynthesizer::ListLinks(seLinkList &list){
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
		
		if(i < pConnectionCount && pConnections[i]){
			engController = pConnections[i]->GetEngineControllerIndex();
		}
		
		source.SetConnectionAt(i, engController);
	}
}
