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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "seEffect.h"
#include "seEffectStretch.h"
#include "../seSynthesizer.h"
#include "../link/seLink.h"
#include "../source/seSource.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/effect/deSynthesizerEffect.h>



// Definitions
////////////////

#define LOGSOURCE "Synthesizer Editor"



// Class seEffect
///////////////////

// Constructor, destructor
////////////////////////////

seEffect::seEffect(deSynthesizerEffectVisitorIdentify::eEffectTypes type) :
pSynthesizer(nullptr),
pParentSource(nullptr),
pType(type),
pEnabled(true),
pStrength(1.0f){
}

seEffect::seEffect(const seEffect &copy) :
pSynthesizer(nullptr),
pParentSource(nullptr),
pType(copy.pType),
pEnabled(copy.pEnabled),
pStrength(copy.pStrength),
pTargetStrength(copy.pTargetStrength){
}

seEffect::~seEffect(){
}



// Management
///////////////

seSynthesizer *seEffect::GetSynthesizer() const{
	return pParentSource ? pParentSource->GetSynthesizer() : nullptr;
}



void seEffect::SetEngineEffect(deSynthesizerEffect *engEffect){
	pEngEffect = engEffect;
}

void seEffect::InitEngineEffect(deSynthesizerEffect *engEffect) const{
	DEASSERT_NOTNULL(engEffect)
	
	seSynthesizer * const synthesizer = GetSynthesizer();
	DEASSERT_NOTNULL(synthesizer)
	
	engEffect->SetEnabled(pEnabled);
	engEffect->SetStrength(pStrength);
	
	pTargetStrength->UpdateEngineTarget(*synthesizer, engEffect->GetTargetStrength());
}



void seEffect::SetParentSource(seSource *source){
	pParentSource = source;
}



void seEffect::SetEnabled(bool enabled){
	if(enabled == pEnabled){
		return;
	}
	
	pEnabled = enabled;
	
	if(pEngEffect){
		pEngEffect->SetEnabled(enabled);
	}
	
	NotifyEffectChanged();
}

void seEffect::SetStrength(float strength){
	if(fabsf(strength - pStrength) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pStrength = strength;
	
	if(pEngEffect){
		pEngEffect->SetStrength(strength);
	}
	
	NotifyEffectChanged();
}



void seEffect::UpdateTargets(){
	seSynthesizer * const synthesizer = GetSynthesizer();
	DEASSERT_NOTNULL(synthesizer)
	
	if(pEngEffect && synthesizer){
		pTargetStrength->UpdateEngineTarget(*synthesizer, pEngEffect->GetTargetStrength());
	}
}

int seEffect::CountLinkUsage(seLink *link) const{
	int usageCount = 0;
	
	if(pTargetStrength->GetLinks().Has(link)){
		usageCount++;
	}
	
	return usageCount;
}

void seEffect::RemoveLinkFromTargets(seLink *link){
	pTargetStrength->RemoveLink(link);
}

void seEffect::RemoveLinksFromAllTargets(){
	pTargetStrength->RemoveAllLinks();
}



void seEffect::ListLinks(seLink::List &list){
	pTargetStrength->AddLinksToList(list);
}



void seEffect::NotifyEffectChanged(){
	seSynthesizer * const synthesizer = GetSynthesizer();
	if(!synthesizer){
		return;
	}
	
	UpdateTargets();
	
	if(pEngEffect){
		synthesizer->GetEngineSynthesizer()->NotifySourcesChanged();
	}
	
	synthesizer->NotifyEffectChanged(pParentSource, this);
}

void seEffect::NotifyEngineOnlyEffectChanged(){
	seSynthesizer * const synthesizer = GetSynthesizer();
	if(!synthesizer || !pEngEffect){
		return;
	}
	
	synthesizer->GetEngineSynthesizer()->NotifySourcesChanged();
}

void seEffect::SynthesizerChanged(){
}

void seEffect::SynthesizerDirectoryChanged(){
}



// Operators
//////////////

seEffect &seEffect::operator=(const seEffect &copy){
	SetStrength(copy.pStrength);
	SetEnabled(copy.pEnabled);
	pTargetStrength = copy.pTargetStrength;
	NotifyEffectChanged();
	return *this;
}



// Helper
///////////

seEffect::Ref seEffect::CreateEffectFromType(deEngine *engine, deSynthesizerEffectVisitorIdentify::eEffectTypes type){
	switch(type){
	case deSynthesizerEffectVisitorIdentify::eetStretch:
		return seEffectStretch::Ref::New();
		
	default:
		DETHROW(deeInvalidParam);
	}
}
