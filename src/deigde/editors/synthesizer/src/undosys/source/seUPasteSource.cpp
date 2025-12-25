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
#include <string.h>
#include <stdlib.h>

#include "seUPasteSource.h"
#include "../../synthesizer/seSynthesizer.h"
#include "../../synthesizer/source/seSource.h"
#include "../../synthesizer/link/seLink.h"

#include <dragengine/common/exceptions.h>



// Class seUPasteSource
/////////////////////////

// Constructor, destructor
////////////////////////////

seUPasteSource::seUPasteSource(seSynthesizer *synthesizer, const seSource::List &sources, int index) :

pIndex(index)
{
	if(!synthesizer || sources.IsEmpty()){
		DETHROW(deeInvalidParam);
	}
	
	sources.Visit([&](const seSource &source){
		pSources.Add(source.CreateCopy());
	});
	
	pSynthesizer = synthesizer;
}

seUPasteSource::~seUPasteSource(){
}



// Management
///////////////

void seUPasteSource::Undo(){
	// remove the sources
	pSources.Visit([&](seSource *source){
		pSynthesizer->RemoveSource(source);
	});
	
	// remove links added in a prior redo
	pRemoveLinkList.Visit([&](seLink *link){
		pSynthesizer->RemoveLink(link);
	});
	pRemoveLinkList.RemoveAll();
	
	// remove controller added in a prior redo
	pRemoveControllerList.Visit([&](seController *controller){
		pSynthesizer->RemoveController(controller);
	});
	pRemoveControllerList.RemoveAll();
}

void seUPasteSource::Redo(){
	pRemoveLinkList.RemoveAll();
	pRemoveControllerList.RemoveAll();
	
	pSources.VisitIndexed([&](int i, seSource *source){
		// check if links exist in the synthesizer. if not add them and mark them to remove during undo
		seLink::List links;
		source->ListLinks(links);
		
		links.Visit([&](seLink *link){
			if(pSynthesizer->GetLinks().Has(link)){
				return;
			}
			
			seController * const controller = link->GetController();
			if(controller && !pSynthesizer->GetControllers().Has(controller)){
				pRemoveControllerList.Add(controller);
				pSynthesizer->AddController(controller);
			}
			
			pRemoveLinkList.Add(link);
			pSynthesizer->AddLink(link);
		});
		
		// insert the source
		pSynthesizer->InsertSourceAt(source, pIndex + i);
	});
}
