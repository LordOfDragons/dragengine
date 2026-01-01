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

#include "syneEffectStretch.h"
#include "../syneSynthesizer.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/synthesizer/effect/deSynthesizerEffect.h>
#include <dragengine/resources/synthesizer/effect/deSynthesizerEffectStretch.h>
#include <dragengine/resources/synthesizer/effect/deSynthesizerEffectVisitorIdentify.h>



// Class syneEffectStretch
//////////////////////////

// Constructor, destructor
////////////////////////////

syneEffectStretch::syneEffectStretch() :
syneEffect(deSynthesizerEffectVisitorIdentify::eetStretch),
pMinTime(0.0f),
pMaxTime(0.0f),
pMinPitch(0.0f),
pMaxPitch(0.0f){
}

syneEffectStretch::syneEffectStretch(const syneEffectStretch &copy) :
syneEffect(copy),
pMinTime(copy.pMinTime),
pMaxTime(copy.pMaxTime),
pMinPitch(copy.pMinPitch),
pMaxPitch(copy.pMaxPitch),
pTargetTime(copy.pTargetTime),
pTargetPitch(copy.pTargetPitch){
}

syneEffectStretch::~syneEffectStretch(){
}



// Management
///////////////

void syneEffectStretch::SetMinTime(float time){
	if(fabsf(time - pMinTime) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMinTime = time;
	
	if(GetEngineEffect()){
		GetEngineEffect().DynamicCast<deSynthesizerEffectStretch>()->SetMinTime(time);
		NotifyEffectChanged();
	}
}

void syneEffectStretch::SetMaxTime(float time){
	if(fabsf(time - pMaxTime) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMaxTime = time;
	
	if(GetEngineEffect()){
		GetEngineEffect().DynamicCast<deSynthesizerEffectStretch>()->SetMaxTime(time);
		NotifyEffectChanged();
	}
}

void syneEffectStretch::SetMinPitch(float shift){
	if(fabsf(shift - pMinPitch) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMinPitch = shift;
	
	if(GetEngineEffect()){
		GetEngineEffect().DynamicCast<deSynthesizerEffectStretch>()->SetMinPitch(shift);
		NotifyEffectChanged();
	}
}

void syneEffectStretch::SetMaxPitch(float shift){
	if(fabsf(shift - pMaxPitch) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMaxPitch = shift;
	
	if(GetEngineEffect()){
		GetEngineEffect().DynamicCast<deSynthesizerEffectStretch>()->SetMaxPitch(shift);
		NotifyEffectChanged();
	}
}



void syneEffectStretch::UpdateTargets(){
	syneEffect::UpdateTargets();
	
	deSynthesizerEffectStretch * const effect = GetEngineEffect().DynamicCast<deSynthesizerEffectStretch>();
	if(effect){
		syneSynthesizer * const synthesizer = GetSynthesizer();
		DEASSERT_NOTNULL(synthesizer)
		
		pTargetTime->UpdateEngineTarget(*synthesizer, effect->GetTargetTime());
		pTargetPitch->UpdateEngineTarget(*synthesizer, effect->GetTargetPitch());
	}
}

int syneEffectStretch::CountLinkUsage(syneLink *link) const{
	int usageCount = syneEffect::CountLinkUsage(link);
	
	if(pTargetTime->GetLinks().Has(link)){
		usageCount++;
	}
	if(pTargetPitch->GetLinks().Has(link)){
		usageCount++;
	}
	
	return usageCount;
}

void syneEffectStretch::RemoveLinkFromTargets(syneLink *link){
	syneEffect::RemoveLinkFromTargets(link);
	
	pTargetTime->RemoveLink(link);
	pTargetPitch->RemoveLink(link);
	
	UpdateTargets();
}

void syneEffectStretch::RemoveLinksFromAllTargets(){
	syneEffect::RemoveLinksFromAllTargets();
	
	pTargetTime->RemoveAllLinks();
	pTargetPitch->RemoveAllLinks();
	
	UpdateTargets();
}



deSynthesizerEffect::Ref syneEffectStretch::CreateEngineEffect(){
	const deSynthesizerEffectStretch::Ref engEffect(deSynthesizerEffectStretch::Ref::New());
	
	InitEngineEffect(engEffect);
	
	engEffect->SetMinTime(pMinTime);
	engEffect->SetMaxTime(pMaxTime);
	engEffect->SetMinPitch(pMinPitch);
	engEffect->SetMaxPitch(pMaxPitch);
	
	syneSynthesizer * const synthesizer = GetSynthesizer();
	DEASSERT_NOTNULL(synthesizer)
	
	pTargetTime->UpdateEngineTarget(*synthesizer, engEffect->GetTargetTime());
	pTargetPitch->UpdateEngineTarget(*synthesizer, engEffect->GetTargetPitch());
	
	return engEffect;
}



syneEffect::Ref syneEffectStretch::CreateCopy() const{
	return syneEffectStretch::Ref::New(*this);
}

void syneEffectStretch::ListLinks(syneLink::List &list){
	syneEffect::ListLinks(list);
	pTargetTime->AddLinksToList(list);
	pTargetPitch->AddLinksToList(list);
}



// Operators
//////////////

syneEffectStretch &syneEffectStretch::operator=(const syneEffectStretch &copy){
	SetMinTime(copy.pMinTime);
	SetMaxTime(copy.pMaxTime);
	SetMinPitch(copy.pMinPitch);
	SetMaxPitch(copy.pMaxPitch);
	pTargetTime = copy.pTargetTime;
	pTargetPitch = copy.pTargetPitch;
	syneEffect::operator=(copy);
	return *this;
}
