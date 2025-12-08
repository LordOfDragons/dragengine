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

#include "seUSourcePasteEffect.h"
#include "../../../synthesizer/effect/seEffect.h"
#include "../../../synthesizer/source/seSource.h"
#include "../../../synthesizer/link/seLink.h"
#include "../../../synthesizer/seSynthesizer.h"

#include <dragengine/common/exceptions.h>



// Class seUSourcePasteEffect
///////////////////////////////

// Constructor, destructor
////////////////////////////

seUSourcePasteEffect::seUSourcePasteEffect(seSource *source, const seEffectList &effectList, int index) :
pSource(NULL),
pIndex(index)
{
	const int effectCount = effectList.GetCount();
	
	if(!source || effectCount == 0){
		DETHROW(deeInvalidParam);
	}
	
	seEffect *effect = NULL;
	int i;
	
	try{
		for(i=0; i<effectCount; i++){
			effect = effectList.GetAt(i)->CreateCopy();
			pEffectList.Add(effect);
			effect->FreeReference();
			effect = NULL;
		}
		
	}catch(const deException &){
		throw;
	}
	
	pSource = source;
	source->AddReference();
}

seUSourcePasteEffect::~seUSourcePasteEffect(){
	pCleanUp();
}



// Management
///////////////

void seUSourcePasteEffect::Undo(){
	seSynthesizer * const synthesizer = pSource->GetSynthesizer();
	const int effectCount = pEffectList.GetCount();
	int i;
	
	// remove the effects
	for(i=0; i<effectCount; i++){
		pSource->RemoveEffect(pEffectList.GetAt(i));
	}
	
	// remove links added in a prior redo
	const int linkCount = pRemoveLinkList.GetCount();
	for(i=0; i<linkCount; i++){
		synthesizer->RemoveLink(pRemoveLinkList.GetAt(i));
	}
	pRemoveLinkList.RemoveAll();
	
	// remove controller added in a prior redo
	const int controllerCount = pRemoveControllerList.GetCount();
	for(i=0; i<controllerCount; i++){
		synthesizer->RemoveController(pRemoveControllerList.GetAt(i));
	}
	pRemoveControllerList.RemoveAll();
}

void seUSourcePasteEffect::Redo(){
	seSynthesizer * const synthesizer = pSource->GetSynthesizer();
	const int effectCount = pEffectList.GetCount();
	int i, j;
	
	pRemoveLinkList.RemoveAll();
	pRemoveControllerList.RemoveAll();
	
	for(i=0; i<effectCount; i++){
		seEffect * const effect = pEffectList.GetAt(i);
		
		// check if links exist in the synthesizer. if not add them and mark them to remove during undo
		seLinkList linkList;
		effect->ListLinks(linkList);
		
		const int linkCount = linkList.GetCount();
		for(j=0; j<linkCount; j++){
			seLink * const link = linkList.GetAt(j);
			
			if(!synthesizer->GetLinks().Has(link)){
				seController * const controller = link->GetController();
				if(controller && !synthesizer->GetControllers().Has(controller)){
					pRemoveControllerList.Add(controller);
					synthesizer->AddController(controller);
				}
				
				pRemoveLinkList.Add(link);
				synthesizer->AddLink(link);
			}
		}
		
		// insert the source
		pSource->InsertEffectAt(effect, pIndex + i);
	}
}



// Private Functions
//////////////////////

void seUSourcePasteEffect::pCleanUp(){
}
