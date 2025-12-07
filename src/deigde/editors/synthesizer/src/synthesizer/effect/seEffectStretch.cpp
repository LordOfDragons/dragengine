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
		((deSynthesizerEffectStretch*)GetEngineEffect())->SetMinTime(time);
		NotifyEffectChanged();
	}
}

void seEffectStretch::SetMaxTime(float time){
	if(fabsf(time - pMaxTime) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMaxTime = time;
	
	if(GetEngineEffect()){
		((deSynthesizerEffectStretch*)GetEngineEffect())->SetMaxTime(time);
		NotifyEffectChanged();
	}
}

void seEffectStretch::SetMinPitch(float shift){
	if(fabsf(shift - pMinPitch) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMinPitch = shift;
	
	if(GetEngineEffect()){
		((deSynthesizerEffectStretch*)GetEngineEffect())->SetMinPitch(shift);
		NotifyEffectChanged();
	}
}

void seEffectStretch::SetMaxPitch(float shift){
	if(fabsf(shift - pMaxPitch) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMaxPitch = shift;
	
	if(GetEngineEffect()){
		((deSynthesizerEffectStretch*)GetEngineEffect())->SetMaxPitch(shift);
		NotifyEffectChanged();
	}
}



void seEffectStretch::UpdateTargets(){
	seEffect::UpdateTargets();
	
	deSynthesizerEffectStretch * const effect = (deSynthesizerEffectStretch*)GetEngineEffect();
	if(effect){
		pTargetTime.UpdateEngineTarget(GetSynthesizer(), effect->GetTargetTime());
		pTargetPitch.UpdateEngineTarget(GetSynthesizer(), effect->GetTargetPitch());
	}
}

int seEffectStretch::CountLinkUsage(seLink *link) const{
	int usageCount = seEffect::CountLinkUsage(link);
	
	if(pTargetTime.HasLink(link)){
		usageCount++;
	}
	if(pTargetPitch.HasLink(link)){
		usageCount++;
	}
	
	return usageCount;
}

void seEffectStretch::RemoveLinkFromTargets(seLink *link){
	seEffect::RemoveLinkFromTargets(link);
	
	if(pTargetTime.HasLink(link)){
		pTargetTime.RemoveLink(link);
	}
	if(pTargetPitch.HasLink(link)){
		pTargetPitch.RemoveLink(link);
	}
	
	UpdateTargets();
}

void seEffectStretch::RemoveLinksFromAllTargets(){
	seEffect::RemoveLinksFromAllTargets();
	
	pTargetTime.RemoveAllLinks();
	pTargetPitch.RemoveAllLinks();
	
	UpdateTargets();
}



deSynthesizerEffect *seEffectStretch::CreateEngineEffect(){
	deSynthesizerEffectStretch *engEffect = NULL;
	
	try{
		// create effect
		engEffect = new deSynthesizerEffectStretch;
		
		// init effect
		InitEngineEffect(engEffect);
		
		engEffect->SetMinTime(pMinTime);
		engEffect->SetMaxTime(pMaxTime);
		engEffect->SetMinPitch(pMinPitch);
		engEffect->SetMaxPitch(pMaxPitch);
		
		pTargetTime.UpdateEngineTarget(GetSynthesizer(), engEffect->GetTargetTime());
		pTargetPitch.UpdateEngineTarget(GetSynthesizer(), engEffect->GetTargetPitch());
		
	}catch(const deException &){
		if(engEffect){
			engEffect->FreeReference();
		}
		throw;
	}
	
	// finished
	return engEffect;
}



seEffect *seEffectStretch::CreateCopy() const{
	return new seEffectStretch(*this);
}

void seEffectStretch::ListLinks(seLinkList &list){
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
