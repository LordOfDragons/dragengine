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

#include "syneLink.h"
#include "../syneSynthesizer.h"
#include "../controller/syneController.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/deSynthesizerLink.h>



// Class syneLink
/////////////////

// Constructor, destructor
////////////////////////////

syneLink::syneLink() :
pSynthesizer(nullptr),
pName("Link"),
pRepeat(1){
}

syneLink::syneLink(const syneLink &copy) :
pSynthesizer(nullptr),
pName(copy.pName),
pRepeat(copy.pRepeat),
pCurve(copy.pCurve)
{
	pController = copy.pController;
}

syneLink::~syneLink(){
	SetSynthesizer(nullptr);
}



// Management
///////////////

void syneLink::SetSynthesizer(syneSynthesizer *synthesizer){
	if(synthesizer == pSynthesizer){
		return;
	}
	
	if(pSynthesizer && pEngLink){
		pSynthesizer->GetEngineSynthesizer()->RemoveLink(pEngLink);
		pEngLink = nullptr;
	}
	
	pSynthesizer = synthesizer;
	
	if(synthesizer){
		pEngLink = deSynthesizerLink::Ref::New();
		synthesizer->GetEngineSynthesizer()->AddLink(pEngLink);
		
		UpdateController();
		
		pEngLink->SetRepeat(pRepeat);
		NotifyLinkChanged();
		
		UpdateCurve();
	}
}

void syneLink::SetName(const char *name){
	if(pName.Equals(name)){
		return;
	}
	
	pName = name;
	
	if(pSynthesizer){
		pSynthesizer->NotifyLinkNameChanged(this);
	}
}

void syneLink::SetController(syneController *controller){
	if(controller == pController){
		return;
	}
	pController = controller;
	UpdateController();
	
	if(pSynthesizer){
		pSynthesizer->NotifyLinkChanged(this);
	}
}

void syneLink::SetRepeat(int repeat){
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



void syneLink::NotifyLinkChanged(){
	if(pEngLink && pSynthesizer){
		pSynthesizer->GetEngineSynthesizer()->NotifyLinksChanged();
	}
}



void syneLink::UpdateController(){
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

void syneLink::UpdateCurve(){
	if(!pEngLink){
		return;
	}
	
	pEngLink->GetCurve() = pCurve;
	NotifyLinkChanged();
}



// Operators
//////////////

syneLink &syneLink::operator=(const syneLink &copy){
	SetName(copy.pName);
	SetController(copy.pController);
	SetRepeat(copy.pRepeat);
	pCurve = copy.pCurve;
	return *this;
}
