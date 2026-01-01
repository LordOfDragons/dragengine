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

#include "syneUSourcePasteEffect.h"
#include "../../../synthesizer/effect/syneEffect.h"
#include "../../../synthesizer/source/syneSource.h"
#include "../../../synthesizer/link/syneLink.h"
#include "../../../synthesizer/syneSynthesizer.h"

#include <dragengine/common/exceptions.h>



// Class syneUSourcePasteEffect
///////////////////////////////

// Constructor, destructor
////////////////////////////

syneUSourcePasteEffect::syneUSourcePasteEffect(syneSource *source, const syneEffect::List &effects, int index) :

pIndex(index)
{
	if(!source || effects.IsEmpty()){
		DETHROW(deeInvalidParam);
	}
	
	effects.Visit([&](const syneEffect &effect){
		pEffects.Add(effect.CreateCopy());
	});
	
	pSource = source;
}

syneUSourcePasteEffect::~syneUSourcePasteEffect(){
}



// Management
///////////////

void syneUSourcePasteEffect::Undo(){
	syneSynthesizer * const synthesizer = pSource->GetSynthesizer();
	
	// remove effects
	pEffects.Visit([&](syneEffect *effect){
		pSource->RemoveEffect(effect);
	});
	
	// remove links added in a prior redo
	pRemoveLinkList.Visit([&](syneLink *link){
		synthesizer->RemoveLink(link);
	});
	pRemoveLinkList.RemoveAll();
	
	// remove controller added in a prior redo
	pRemoveControllerList.Visit([&](syneController *controller){
		synthesizer->RemoveController(controller);
	});
	pRemoveControllerList.RemoveAll();
}

void syneUSourcePasteEffect::Redo(){
	syneSynthesizer * const synthesizer = pSource->GetSynthesizer();
	
	pRemoveLinkList.RemoveAll();
	pRemoveControllerList.RemoveAll();
	
	pEffects.VisitIndexed([&](int i, syneEffect *effect){
		// check if links exist in the synthesizer. if not add them and mark them to remove during undo
		syneLink::List links;
		effect->ListLinks(links);
		
		links.Visit([&](syneLink *link){
			if(synthesizer->GetLinks().Has(link)){
				return;
			}
			
			syneController * const controller = link->GetController();
			if(controller && !synthesizer->GetControllers().Has(controller)){
				pRemoveControllerList.Add(controller);
				synthesizer->AddController(controller);
			}
			
			pRemoveLinkList.Add(link);
			synthesizer->AddLink(link);
		});
		
		// insert the source
		pSource->InsertEffectAt(effect, pIndex + i);
	});
}
