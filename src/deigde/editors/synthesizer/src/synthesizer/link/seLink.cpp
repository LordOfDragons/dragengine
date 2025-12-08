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

#include "seLink.h"
#include "../seSynthesizer.h"
#include "../controller/seController.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/deSynthesizerLink.h>



// Class seLink
/////////////////

// Constructor, destructor
////////////////////////////

seLink::seLink() :
pSynthesizer(NULL),
pEngLink(NULL),
pName("Link"),
pController(NULL),
pRepeat(1){
}

seLink::seLink(const seLink &copy) :
pSynthesizer(NULL),
pEngLink(NULL),
pName(copy.pName),
pController(NULL),
pRepeat(copy.pRepeat),
pCurve(copy.pCurve)
{
	pController = copy.pController;
	if(pController){
	}
}

seLink::~seLink(){
	SetSynthesizer(NULL);
	
}



// Management
///////////////

void seLink::SetSynthesizer(seSynthesizer *synthesizer){
	if(synthesizer == pSynthesizer){
		return;
	}
	
	if(pSynthesizer && pEngLink){
		pSynthesizer->GetEngineSynthesizer()->RemoveLink(pEngLink);
		pEngLink = NULL;
	}
	
	pSynthesizer = synthesizer;
	
	if(synthesizer){
		deSynthesizerLink *link = NULL;
		
		try{
			link = new deSynthesizerLink;
			synthesizer->GetEngineSynthesizer()->AddLink(link);
			pEngLink = link;
			
		}catch(const deException &){
			if(link){
				link->FreeReference();
			}
			throw;
		}
		
		UpdateController();
		
		pEngLink->SetRepeat(pRepeat);
		NotifyLinkChanged();
		
		UpdateCurve();
	}
}

void seLink::SetName(const char *name){
	if(pName.Equals(name)){
		return;
	}
	
	pName = name;
	
	if(pSynthesizer){
		pSynthesizer->NotifyLinkNameChanged(this);
	}
}

void seLink::SetController(seController *controller){
	if(controller == pController){
		return;
	}
	
	
	pController = controller;
	
	UpdateController();
	
	if(pSynthesizer){
		pSynthesizer->NotifyLinkChanged(this);
	}
}

void seLink::SetRepeat(int repeat){
	if(repeat < 1){
		DETHROW(deeInvalidParam);
	}
	
	if(repeat == pRepeat){
		return;
	}
	
	pRepeat = repeat;
	
	if(pEngLink){
		pEngLink->SetRepeat(pRepeat);
		NotifyLinkChanged();
	}
	
	if(pSynthesizer){
		pSynthesizer->NotifyLinkChanged(this);
	}
}



void seLink::NotifyLinkChanged(){
	if(pEngLink && pSynthesizer){
		pSynthesizer->GetEngineSynthesizer()->NotifyLinksChanged();
	}
}



void seLink::UpdateController(){
	if(!pEngLink){
		return;
	}
	
	deSynthesizer *engSynthesizer = pSynthesizer->GetEngineSynthesizer();
	int indexController = -1;
	
	if(pController && engSynthesizer){
		indexController = pController->GetEngineControllerIndex();
	}
	
	pEngLink->SetController(indexController);
	
	NotifyLinkChanged();
}

void seLink::UpdateCurve(){
	if(!pEngLink){
		return;
	}
	
	pEngLink->GetCurve() = pCurve;
	NotifyLinkChanged();
}



// Operators
//////////////

seLink &seLink::operator=(const seLink &copy){
	SetName(copy.pName);
	SetController(copy.pController);
	SetRepeat(copy.pRepeat);
	pCurve = copy.pCurve;
	return *this;
}
