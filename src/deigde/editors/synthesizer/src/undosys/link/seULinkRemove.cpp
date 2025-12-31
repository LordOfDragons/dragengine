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

seULinkRemove::seULinkRemove(seLink *link){
	DEASSERT_NOTNULL(link)
	
	seSynthesizer * const synthesizer = link->GetSynthesizer();
	DEASSERT_NOTNULL(synthesizer)
	
	const int targetCount = synthesizer->CountLinkUsage(link);
	
	SetShortInfo("Remove Link");
	
	if(targetCount > 0){
		synthesizer->GetSources().Visit([&](const seSource::Ref &source){
			pAddTargetsForSource(link, targetCount, source);
		});
		
		DEASSERT_TRUE(pTargets.GetCount() == targetCount)
	}
	
	pSynthesizer = synthesizer;
	pLink = link;
}

seULinkRemove::~seULinkRemove(){
}



// Management
///////////////

void seULinkRemove::Undo(){
	pSynthesizer->AddLink(pLink);
	
	pTargets.Visit([&](const cTarget &t){
		t.target->AddLink(pLink);
		t.source->NotifySourceChanged();
	});
}

void seULinkRemove::Redo(){
	pTargets.Visit([&](const cTarget &t){
		t.target->RemoveLink(pLink);
		t.source->NotifySourceChanged();
	});
	
	pSynthesizer->RemoveLink(pLink);
}



// Private Functions
//////////////////////

void seULinkRemove::pAddTargetsForSource(seLink *link, int targetCount, const seSource::Ref &source){
	if(source->GetTargetBlendFactor()->GetLinks().Has(link)){
		pAddTarget(targetCount, source, source->GetTargetBlendFactor());
	}
	if(source->GetTargetPanning()->GetLinks().Has(link)){
		pAddTarget(targetCount, source, source->GetTargetPanning());
	}
	if(source->GetTargetVolume()->GetLinks().Has(link)){
		pAddTarget(targetCount, source, source->GetTargetVolume());
	}
	
	switch(source->GetType()){
	case deSynthesizerSourceVisitorIdentify::estSound:{
		seSourceSound &sound = source.DynamicCast<seSourceSound>();
		
		if(sound.GetTargetSpeed()->GetLinks().Has(link)){
			pAddTarget(targetCount, source, sound.GetTargetSpeed());
		}
		if(sound.GetTargetPlay()->GetLinks().Has(link)){
			pAddTarget(targetCount, source, sound.GetTargetPlay());
		}
		}break;
		
	case deSynthesizerSourceVisitorIdentify::estWave:{
		seSourceWave &wave = source.DynamicCast<seSourceWave>();
		
		if(wave.GetTargetFrequency()->GetLinks().Has(link)){
			pAddTarget(targetCount, source, wave.GetTargetFrequency());
		}
		}break;
		
	case deSynthesizerSourceVisitorIdentify::estGroup:{
		seSourceGroup &group = source.DynamicCast<seSourceGroup>();
		
		if(group.GetTargetSelect()->GetLinks().Has(link)){
			pAddTarget(targetCount, source, group.GetTargetSelect());
		}
		
		group.GetSources().Visit([&](const seSource::Ref &sub){
			pAddTargetsForSource(link, targetCount, sub);
		});
		}break;
		
	default:
		DETHROW(deeInvalidParam);
	}
}

void seULinkRemove::pAddTarget(int targetCount, seSource *source, seControllerTarget *target){
	DEASSERT_TRUE(pTargets.GetCount() < targetCount)
	const cTarget::Ref utarget(cTarget::Ref::New());
	utarget->source = source;
	utarget->target = target;
	pTargets.Add(utarget);
}
