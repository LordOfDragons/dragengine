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

#include "seULinkRemove.h"

#include "../../synthesizer/seSynthesizer.h"
#include "../../synthesizer/link/seLink.h"
#include "../../synthesizer/source/seSource.h"
#include "../../synthesizer/source/seSourceSound.h"
#include "../../synthesizer/source/seSourceWave.h"
#include "../../synthesizer/source/seSourceGroup.h"

#include <dragengine/resources/synthesizer/source/deSynthesizerSourceVisitorIdentify.h>

#include <dragengine/common/exceptions.h>



// Class seULinkRemove
/////////////////////////

// Constructor, destructor
////////////////////////////

seULinkRemove::seULinkRemove(seLink *link) :


pTargets(NULL),
pTargetCount(0)
{
	if(!link){
		DETHROW(deeInvalidParam);
	}
	
	seSynthesizer * const synthesizer = link->GetSynthesizer();
	if(!synthesizer){
		DETHROW(deeInvalidParam);
	}
	
	const int targetCount = synthesizer->CountLinkUsage(link);
	const int sourceCount = synthesizer->GetSources().GetCount();
	int i;
	
	SetShortInfo("Remove Link");
	
	if(targetCount > 0){
		try{
			pTargets = new sTarget[targetCount];
			
			for(i=0; i<sourceCount; i++){
				pAddTargetsForSource(link, targetCount, synthesizer->GetSources().GetAt(i));
			}
			
			if(pTargetCount != targetCount){
				DETHROW(deeInvalidParam);
			}
			
		}catch(const deException &){
			pCleanUp();
			throw;
		}
	}
	
	pSynthesizer = synthesizer;
	pLink = link;
}

seULinkRemove::~seULinkRemove(){
	pCleanUp();
}



// Management
///////////////

void seULinkRemove::Undo(){
	int i;
	
	pSynthesizer->AddLink(pLink);
	
	for(i=0; i<pTargetCount; i++){
		pTargets[i].target->AddLink(pLink);
		pTargets[i].source->NotifySourceChanged();
	}
}

void seULinkRemove::Redo(){
	int i;
	
	for(i=0; i<pTargetCount; i++){
		pTargets[i].target->RemoveLink(pLink);
		pTargets[i].source->NotifySourceChanged();
	}
	
	pSynthesizer->RemoveLink(pLink);
}



// Private Functions
//////////////////////

void seULinkRemove::pCleanUp(){
	int i;
	
	for(i=0; i<pTargetCount; i++){
		pTargets[i].source->FreeReference();
	}
}

void seULinkRemove::pAddTargetsForSource(seLink *link, int targetCount, seSource::Ref source){
	if(source->GetTargetBlendFactor().HasLink(link)){
		pAddTarget(targetCount, source, &source->GetTargetBlendFactor());
	}
	if(source->GetTargetPanning().HasLink(link)){
		pAddTarget(targetCount, source, &source->GetTargetPanning());
	}
	if(source->GetTargetVolume().HasLink(link)){
		pAddTarget(targetCount, source, &source->GetTargetVolume());
	}
	
	switch(source->GetType()){
	case deSynthesizerSourceVisitorIdentify::estSound:{
		seSourceSound * const sound = (seSourceSound*)source;
		
		if(sound->GetTargetSpeed().HasLink(link)){
			pAddTarget(targetCount, source, &sound->GetTargetSpeed());
		}
		if(sound->GetTargetPlay().HasLink(link)){
			pAddTarget(targetCount, source, &sound->GetTargetPlay());
		}
		}break;
		
	case deSynthesizerSourceVisitorIdentify::estWave:{
		seSourceWave * const wave = (seSourceWave*)source;
		
		if(wave->GetTargetFrequency().HasLink(link)){
			pAddTarget(targetCount, source, &wave->GetTargetFrequency());
		}
		}break;
		
	case deSynthesizerSourceVisitorIdentify::estGroup:{
		seSourceGroup * const group = (seSourceGroup*)source;
		
		if(group->GetTargetSelect().HasLink(link)){
			pAddTarget(targetCount, source, &group->GetTargetSelect());
		}
		
		const seSourceList &list = group->GetSources();
		const int count = list.GetCount();
		int i;
		
		for(i=0; i<count; i++){
			pAddTargetsForSource(link, targetCount, list.GetAt(i));
		}
		}break;
		
	default:
		DETHROW(deeInvalidParam);
	}
}

void seULinkRemove::pAddTarget(int targetCount, seSource::Ref source, seControllerTarget *target){
	if(pTargetCount >= targetCount){
		DETHROW(deeInvalidParam);
	}
	
	pTargets[pTargetCount].source = source;
	pTargets[pTargetCount].target = target;
	pTargetCount++;
}
