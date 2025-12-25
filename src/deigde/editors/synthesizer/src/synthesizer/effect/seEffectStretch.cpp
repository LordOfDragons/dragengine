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

#include "seEffectStretch.h"
#include "../seSynthesizer.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/synthesizer/effect/deSynthesizerEffect.h>
#include <dragengine/resources/synthesizer/effect/deSynthesizerEffectStretch.h>
#include <dragengine/resources/synthesizer/effect/deSynthesizerEffectVisitorIdentify.h>



// Class seEffectStretch
//////////////////////////

// Constructor, destructor
////////////////////////////

seEffectStretch::seEffectStretch() :
seEffect(deSynthesizerEffectVisitorIdentify::eetStretch),
pMinTime(0.0f),
pMaxTime(0.0f),
pMinPitch(0.0f),
pMaxPitch(0.0f){
}

seEffectStretch::seEffectStretch(const seEffectStretch &copy) :
seEffect(copy),
pMinTime(copy.pMinTime),
pMaxTime(copy.pMaxTime),
pMinPitch(copy.pMinPitch),
pMaxPitch(copy.pMaxPitch),
pTargetTime(copy.pTargetTime),
pTargetPitch(copy.pTargetPitch){
}

seEffectStretch::~seEffectStretch(){
}



// Management
///////////////

void seEffectStretch::SetMinTime(float time){
	if(fabsf(time - pMinTime) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMinTime = time;
	
	if(GetEngineEffect()){
		GetEngineEffect().DynamicCast<deSynthesizerEffectStretch>()->SetMinTime(time);
		NotifyEffectChanged();
	}
}

void seEffectStretch::SetMaxTime(float time){
	if(fabsf(time - pMaxTime) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMaxTime = time;
	
	if(GetEngineEffect()){
		GetEngineEffect().DynamicCast<deSynthesizerEffectStretch>()->SetMaxTime(time);
		NotifyEffectChanged();
	}
}

void seEffectStretch::SetMinPitch(float shift){
	if(fabsf(shift - pMinPitch) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMinPitch = shift;
	
	if(GetEngineEffect()){
		GetEngineEffect().DynamicCast<deSynthesizerEffectStretch>()->SetMinPitch(shift);
		NotifyEffectChanged();
	}
}

void seEffectStretch::SetMaxPitch(float shift){
	if(fabsf(shift - pMaxPitch) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMaxPitch = shift;
	
	if(GetEngineEffect()){
		GetEngineEffect().DynamicCast<deSynthesizerEffectStretch>()->SetMaxPitch(shift);
		NotifyEffectChanged();
	}
}



void seEffectStretch::UpdateTargets(){
	seEffect::UpdateTargets();
	
	deSynthesizerEffectStretch * const effect = GetEngineEffect().DynamicCast<deSynthesizerEffectStretch>();
	if(effect){
		seSynthesizer * const synthesizer = GetSynthesizer();
		DEASSERT_NOTNULL(synthesizer)
		
		pTargetTime.UpdateEngineTarget(*synthesizer, effect->GetTargetTime());
		pTargetPitch.UpdateEngineTarget(*synthesizer, effect->GetTargetPitch());
	}
}

int seEffectStretch::CountLinkUsage(seLink *link) const{
	int usageCount = seEffect::CountLinkUsage(link);
	
	if(pTargetTime.GetLinks().Has(link)){
		usageCount++;
	}
	if(pTargetPitch.GetLinks().Has(link)){
		usageCount++;
	}
	
	return usageCount;
}

void seEffectStretch::RemoveLinkFromTargets(seLink *link){
	seEffect::RemoveLinkFromTargets(link);
	
	pTargetTime.RemoveLink(link);
	pTargetPitch.RemoveLink(link);
	
	UpdateTargets();
}

void seEffectStretch::RemoveLinksFromAllTargets(){
	seEffect::RemoveLinksFromAllTargets();
	
	pTargetTime.RemoveAllLinks();
	pTargetPitch.RemoveAllLinks();
	
	UpdateTargets();
}



deSynthesizerEffect::Ref seEffectStretch::CreateEngineEffect(){
	const deSynthesizerEffectStretch::Ref engEffect(deSynthesizerEffectStretch::Ref::New());
	
	InitEngineEffect(engEffect);
	
	engEffect->SetMinTime(pMinTime);
	engEffect->SetMaxTime(pMaxTime);
	engEffect->SetMinPitch(pMinPitch);
	engEffect->SetMaxPitch(pMaxPitch);
	
	seSynthesizer * const synthesizer = GetSynthesizer();
	DEASSERT_NOTNULL(synthesizer)
	
	pTargetTime.UpdateEngineTarget(*synthesizer, engEffect->GetTargetTime());
	pTargetPitch.UpdateEngineTarget(*synthesizer, engEffect->GetTargetPitch());
	
	return engEffect;
}



seEffect::Ref seEffectStretch::CreateCopy() const{
	return seEffectStretch::Ref::New(*this);
}

void seEffectStretch::ListLinks(seLink::List &list){
	seEffect::ListLinks(list);
	pTargetTime.AddLinksToList(list);
	pTargetPitch.AddLinksToList(list);
}



// Operators
//////////////

seEffectStretch &seEffectStretch::operator=(const seEffectStretch &copy){
	SetMinTime(copy.pMinTime);
	SetMaxTime(copy.pMaxTime);
	SetMinPitch(copy.pMinPitch);
	SetMaxPitch(copy.pMaxPitch);
	pTargetTime = copy.pTargetTime;
	pTargetPitch = copy.pTargetPitch;
	seEffect::operator=(copy);
	return *this;
}
