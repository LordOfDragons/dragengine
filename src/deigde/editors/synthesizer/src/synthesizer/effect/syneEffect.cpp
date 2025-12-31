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

#include "syneEffect.h"
#include "syneEffectStretch.h"
#include "../syneSynthesizer.h"
#include "../link/syneLink.h"
#include "../source/syneSource.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/effect/deSynthesizerEffect.h>



// Definitions
////////////////

#define LOGSOURCE "Synthesizer Editor"



// Class syneEffect
///////////////////

// Constructor, destructor
////////////////////////////

syneEffect::syneEffect(deSynthesizerEffectVisitorIdentify::eEffectTypes type) :
pSynthesizer(nullptr),
pParentSource(nullptr),
pType(type),
pEnabled(true),
pStrength(1.0f){
}

syneEffect::syneEffect(const syneEffect &copy) :
pSynthesizer(nullptr),
pParentSource(nullptr),
pType(copy.pType),
pEnabled(copy.pEnabled),
pStrength(copy.pStrength),
pTargetStrength(copy.pTargetStrength){
}

syneEffect::~syneEffect(){
}



// Management
///////////////

syneSynthesizer *syneEffect::GetSynthesizer() const{
	return pParentSource ? pParentSource->GetSynthesizer() : nullptr;
}



void syneEffect::SetEngineEffect(deSynthesizerEffect *engEffect){
	pEngEffect = engEffect;
}

void syneEffect::InitEngineEffect(deSynthesizerEffect *engEffect) const{
	DEASSERT_NOTNULL(engEffect)
	
	syneSynthesizer * const synthesizer = GetSynthesizer();
	DEASSERT_NOTNULL(synthesizer)
	
	engEffect->SetEnabled(pEnabled);
	engEffect->SetStrength(pStrength);
	
	pTargetStrength->UpdateEngineTarget(*synthesizer, engEffect->GetTargetStrength());
}



void syneEffect::SetParentSource(syneSource *source){
	pParentSource = source;
}



void syneEffect::SetEnabled(bool enabled){
	if(enabled == pEnabled){
		return;
	}
	
	pEnabled = enabled;
	
	if(pEngEffect){
		pEngEffect->SetEnabled(enabled);
	}
	
	NotifyEffectChanged();
}

void syneEffect::SetStrength(float strength){
	if(fabsf(strength - pStrength) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pStrength = strength;
	
	if(pEngEffect){
		pEngEffect->SetStrength(strength);
	}
	
	NotifyEffectChanged();
}



void syneEffect::UpdateTargets(){
	syneSynthesizer * const synthesizer = GetSynthesizer();
	DEASSERT_NOTNULL(synthesizer)
	
	if(pEngEffect && synthesizer){
		pTargetStrength->UpdateEngineTarget(*synthesizer, pEngEffect->GetTargetStrength());
	}
}

int syneEffect::CountLinkUsage(syneLink *link) const{
	int usageCount = 0;
	
	if(pTargetStrength->GetLinks().Has(link)){
		usageCount++;
	}
	
	return usageCount;
}

void syneEffect::RemoveLinkFromTargets(syneLink *link){
	pTargetStrength->RemoveLink(link);
}

void syneEffect::RemoveLinksFromAllTargets(){
	pTargetStrength->RemoveAllLinks();
}



void syneEffect::ListLinks(syneLink::List &list){
	pTargetStrength->AddLinksToList(list);
}



void syneEffect::NotifyEffectChanged(){
	syneSynthesizer * const synthesizer = GetSynthesizer();
	if(!synthesizer){
		return;
	}
	
	UpdateTargets();
	
	if(pEngEffect){
		synthesizer->GetEngineSynthesizer()->NotifySourcesChanged();
	}
	
	synthesizer->NotifyEffectChanged(pParentSource, this);
}

void syneEffect::NotifyEngineOnlyEffectChanged(){
	syneSynthesizer * const synthesizer = GetSynthesizer();
	if(!synthesizer || !pEngEffect){
		return;
	}
	
	synthesizer->GetEngineSynthesizer()->NotifySourcesChanged();
}

void syneEffect::SynthesizerChanged(){
}

void syneEffect::SynthesizerDirectoryChanged(){
}



// Operators
//////////////

syneEffect &syneEffect::operator=(const syneEffect &copy){
	SetStrength(copy.pStrength);
	SetEnabled(copy.pEnabled);
	pTargetStrength = copy.pTargetStrength;
	NotifyEffectChanged();
	return *this;
}



// Helper
///////////

syneEffect::Ref syneEffect::CreateEffectFromType(deEngine *engine, deSynthesizerEffectVisitorIdentify::eEffectTypes type){
	switch(type){
	case deSynthesizerEffectVisitorIdentify::eetStretch:
		return syneEffectStretch::Ref::New();
		
	default:
		DETHROW(deeInvalidParam);
	}
}
