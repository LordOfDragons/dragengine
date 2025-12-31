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

#include "syneSource.h"
#include "syneSourceSound.h"
#include "syneSourceWave.h"
#include "syneSourceSynthesizer.h"
#include "syneSourceGroup.h"
#include "syneSourceChain.h"
#include "../syneSynthesizer.h"
#include "../link/syneLink.h"
#include "../effect/syneEffect.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSource.h>



// Definitions
////////////////

#define LOGSOURCE "Synthesizer Editor"



// Class syneSource
///////////////////

// Constructor, destructor
////////////////////////////

syneSource::syneSource(deSynthesizerSourceVisitorIdentify::eSourceTypes type) :
pSynthesizer(nullptr),
pParentGroup(nullptr),
pName("Source"),
pType(type),
pMixMode(deSynthesizerSource::emmAdd),
pBlendFactor(1.0f),
pEnabled(true),
pMinVolume(1.0f),
pMaxVolume(1.0f),
pMinPanning(0.0f),
pMaxPanning(0.0f),
pTargetBlendFactor(syneControllerTarget::Ref::New()),
pTargetVolume(syneControllerTarget::Ref::New()),
pTargetPanning(syneControllerTarget::Ref::New())
{
	if(type < deSynthesizerSourceVisitorIdentify::estSound || type > deSynthesizerSourceVisitorIdentify::estGroup){
		DETHROW(deeInvalidParam);
	}
}

syneSource::syneSource(const syneSource &copy) :
pSynthesizer(nullptr),
pParentGroup(nullptr),
pName(copy.pName),
pType(copy.pType),
pMixMode(copy.pMixMode),
pBlendFactor(copy.pBlendFactor),
pEnabled(copy.pEnabled),
pMinVolume(1.0f),
pMaxVolume(1.0f),
pMinPanning(0.0f),
pMaxPanning(0.0f),
pTargetBlendFactor(syneControllerTarget::Ref::New(copy.pTargetBlendFactor)),
pTargetVolume(syneControllerTarget::Ref::New(copy.pTargetVolume)),
pTargetPanning(syneControllerTarget::Ref::New(copy.pTargetPanning))
{
	copy.pEffects.Visit([&](const syneEffect &e){
		const syneEffect::Ref effect(e.CreateCopy());
		pEffects.Add(effect);
		effect->SetParentSource(this);
	});
}

syneSource::~syneSource(){
	SetSynthesizer(nullptr);
}



// Management
///////////////

syneSynthesizer *syneSource::GetSynthesizer() const{
	return pParentGroup ? pParentGroup->GetSynthesizer() : pSynthesizer;
}

void syneSource::SetSynthesizer(syneSynthesizer *synthesizer){
	if(synthesizer == pSynthesizer){
		return;
	}
	
	pEngSource = nullptr;
	pSynthesizer = synthesizer;
	SynthesizerChanged();
}



void syneSource::SetEngineSource(deSynthesizerSource *engSource){
	pEngSource = engSource;
}

void syneSource::InitEngineSource(deSynthesizerSource *engSource) const{
	if(!engSource){
		DETHROW(deeInvalidParam);
	}
	
	syneSynthesizer * const synthesizer = GetSynthesizer();
	DEASSERT_NOTNULL(synthesizer);
	
	engSource->SetEnabled(pEnabled);
	engSource->SetMixMode(pMixMode);
	engSource->SetBlendFactor(pBlendFactor);
	engSource->SetMinVolume(pMinVolume);
	engSource->SetMaxVolume(pMaxVolume);
	engSource->SetMinPanning(pMinPanning);
	engSource->SetMaxPanning(pMaxPanning);
	
	pTargetBlendFactor->UpdateEngineTarget(*synthesizer, engSource->GetTargetBlendFactor());
	pTargetVolume->UpdateEngineTarget(*synthesizer, engSource->GetTargetVolume());
	pTargetPanning->UpdateEngineTarget(*synthesizer, engSource->GetTargetPanning());
	
	// effects
	pEffects.Visit([&](syneEffect &effect){
		effect.SetEngineEffect(nullptr);
	});
	
	pEffects.Visit([&](syneEffect &effect){
		const deSynthesizerEffect::Ref engEffect(effect.CreateEngineEffect());
		engSource->AddEffect(engEffect);
		effect.SetEngineEffect(engEffect);
	});
}



void syneSource::SetParentGroup(syneSourceGroup *group){
	pParentGroup = group;
}



void syneSource::SetName(const char *name){
	if(pName == name){
		return;
	}
	
	pName = name;
	
	syneSynthesizer * const synthesizer = GetSynthesizer();
	if(synthesizer){
		synthesizer->NotifySourceNameChanged(this);
	}
}

void syneSource::SetEnabled(bool enabled){
	if(enabled == pEnabled){
		return;
	}
	
	pEnabled = enabled;
	
	if(pEngSource){
		pEngSource->SetEnabled(enabled);
	}
	
	NotifySourceChanged();
}

void syneSource::SetMixMode(deSynthesizerSource::eMixModes mode){
	if(mode < deSynthesizerSource::emmBlend || mode > deSynthesizerSource::deSynthesizerSource::emmAdd){
		DETHROW(deeInvalidParam);
	}
	
	if(mode == pMixMode){
		return;
	}
	
	pMixMode = mode;
	
	if(pEngSource){
		pEngSource->SetMixMode(mode);
	}
	
	NotifySourceChanged();
}

void syneSource::SetBlendFactor(float factor){
	if(fabsf(factor - pBlendFactor) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pBlendFactor = factor;
	
	if(pEngSource){
		pEngSource->SetBlendFactor(factor);
	}
	
	NotifySourceChanged();
}

void syneSource::SetMinVolume(float volume){
	if(fabsf(volume - pMinVolume) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMinVolume = volume;
	
	if(pEngSource){
		pEngSource->SetMinVolume(volume);
	}
	
	NotifySourceChanged();
}

void syneSource::SetMaxVolume(float volume){
	if(fabsf(volume - pMaxVolume) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMaxVolume = volume;
	
	if(pEngSource){
		pEngSource->SetMaxVolume(volume);
	}
	
	NotifySourceChanged();
}

void syneSource::SetMinPanning(float panning){
	if(fabsf(panning - pMinPanning) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMinPanning = panning;
	
	if(pEngSource){
		pEngSource->SetMinPanning(panning);
	}
	
	NotifySourceChanged();
}

void syneSource::SetMaxPanning(float panning){
	if(fabsf(panning - pMaxPanning) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMaxPanning = panning;
	
	if(pEngSource){
		pEngSource->SetMaxPanning(panning);
	}
	
	NotifySourceChanged();
}



void syneSource::UpdateTargets(){
	syneSynthesizer * const synthesizer = GetSynthesizer();
	if(pEngSource && synthesizer){
		pTargetBlendFactor->UpdateEngineTarget(*synthesizer, pEngSource->GetTargetBlendFactor());
		pTargetVolume->UpdateEngineTarget(*synthesizer, pEngSource->GetTargetVolume());
		pTargetPanning->UpdateEngineTarget(*synthesizer, pEngSource->GetTargetPanning());
	}
	
	const int effectCount = pEffects.GetCount();
	int i;
	for(i=0; i< effectCount; i++){
		pEffects.GetAt(i)->UpdateTargets();
	}
}

int syneSource::CountLinkUsage(syneLink *link) const{
	int usageCount = 0;
	
	if(pTargetBlendFactor->GetLinks().Has(link)){
		usageCount++;
	}
	if(pTargetVolume->GetLinks().Has(link)){
		usageCount++;
	}
	if(pTargetPanning->GetLinks().Has(link)){
		usageCount++;
	}
	
	pEffects.Visit([&](const syneEffect &effect){
		usageCount += effect.CountLinkUsage(link);
	});
	
	return usageCount;
}

void syneSource::RemoveLinkFromTargets(syneLink *link){
	pTargetBlendFactor->RemoveLink(link);
	pTargetVolume->RemoveLink(link);
	pTargetPanning->RemoveLink(link);
	
	pEffects.Visit([&](syneEffect &effect){
		effect.RemoveLinkFromTargets(link);
	});
}

void syneSource::RemoveLinksFromAllTargets(){
	pTargetBlendFactor->RemoveAllLinks();
	pTargetVolume->RemoveAllLinks();
	pTargetPanning->RemoveAllLinks();
	
	pEffects.Visit([&](syneEffect &effect){
		effect.RemoveLinksFromAllTargets();
	});
}



void syneSource::ListLinks(syneLink::List &list){
	pTargetBlendFactor->AddLinksToList(list);
	pTargetVolume->AddLinksToList(list);
	pTargetPanning->AddLinksToList(list);
	
	pEffects.Visit([&](syneEffect &effect){
		effect.ListLinks(list);
	});
}



void syneSource::NotifySourceChanged(){
	syneSynthesizer * const synthesizer = GetSynthesizer();
	if(!synthesizer){
		return;
	}
	
	UpdateTargets();
	
	if(pEngSource){
		synthesizer->GetEngineSynthesizer()->NotifySourcesChanged();
	}
	
	synthesizer->NotifySourceChanged(this);
}

void syneSource::NotifyEngineOnlySourceChanged(){
	syneSynthesizer * const synthesizer = GetSynthesizer();
	if(!synthesizer || !pEngSource){
		return;
	}
	
	synthesizer->GetEngineSynthesizer()->NotifySourcesChanged();
}
void syneSource::SynthesizerChanged(){
	pEffects.Visit([&](syneEffect &e){
		e.SynthesizerChanged();
	});
}

void syneSource::SynthesizerDirectoryChanged(){
	pEffects.Visit([&](syneEffect &e){
		e.SynthesizerDirectoryChanged();
	});
}



// Effects
////////////

void syneSource::SetActiveEffect(syneEffect *effect){
	if(effect == pActiveEffect){
		return;
	}
	pActiveEffect = effect;
	syneSynthesizer * const synthesizer = GetSynthesizer();
	if(synthesizer){
		synthesizer->NotifyActiveEffectChanged(this);
	}
}

void syneSource::AddEffect(syneEffect *effect){
	pEffects.Add(effect);
	
	effect->SetParentSource(this);
	
	syneSynthesizer * const synthesizer = GetSynthesizer();
	if(synthesizer){
		synthesizer->RebuildSources();
		synthesizer->NotifyEffectStructureChanged(this);
	}
	
	if(!pActiveEffect){
		SetActiveEffect(effect);
	}
}

void syneSource::InsertEffectAt(syneEffect *effect, int index){
	pEffects.Insert(effect, index);
	
	effect->SetParentSource(this);
	
	syneSynthesizer * const synthesizer = GetSynthesizer();
	if(synthesizer){
		synthesizer->RebuildSources();
		synthesizer->NotifyEffectStructureChanged(this);
	}
	
	if(!pActiveEffect){
		SetActiveEffect(effect);
	}
}

void syneSource::MoveEffectTo(syneEffect *effect, int index){
	pEffects.Move(effect, index);
	
	syneSynthesizer * const synthesizer = GetSynthesizer();
	if(synthesizer){
		synthesizer->RebuildSources();
		synthesizer->NotifyEffectStructureChanged(this);
	}
}

void syneSource::RemoveEffect(syneEffect *effect){
	const int index = pEffects.IndexOf(effect);
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	if(effect == pActiveEffect){
		if(pEffects.GetCount() == 1){
			SetActiveEffect(nullptr);
			
		}else if(index < pEffects.GetCount() - 1){
			SetActiveEffect(pEffects.GetAt(index + 1));
			
		}else{
			SetActiveEffect(pEffects.GetAt(index - 1));
		}
	}
	
	effect->SetParentSource(nullptr);
	
	pEffects.Remove(effect);
	
	syneSynthesizer * const synthesizer = GetSynthesizer();
	if(synthesizer){
		synthesizer->RebuildSources();
		synthesizer->NotifyEffectStructureChanged(this);
	}
}

void syneSource::RemoveAllEffects(){
	SetActiveEffect(nullptr);
	
	pEffects.Visit([&](syneEffect &effect){
		effect.SetParentSource(nullptr);
	});
	pEffects.RemoveAll();
	
	syneSynthesizer * const synthesizer = GetSynthesizer();
	if(synthesizer){
		synthesizer->RebuildSources();
		synthesizer->NotifyEffectStructureChanged(this);
	}
}



// Operators
//////////////

syneSource &syneSource::operator=(const syneSource &copy){
	SetName(copy.pName);
	SetMixMode(copy.pMixMode);
	SetBlendFactor(copy.pBlendFactor);
	SetEnabled(copy.pEnabled);
	SetMinVolume(copy.pMinVolume);
	SetMaxVolume(copy.pMaxVolume);
	SetMinPanning(copy.pMinPanning);
	SetMaxPanning(copy.pMaxPanning);
	pTargetBlendFactor = copy.pTargetBlendFactor;
	pTargetVolume = copy.pTargetVolume;
	pTargetPanning = copy.pTargetPanning;
	
	RemoveAllEffects();
	copy.pEffects.Visit([&](const syneEffect &e){
		AddEffect(e.CreateCopy());
	});
	
	NotifySourceChanged();
	
	return *this;
}



// Helper
///////////

syneSource::Ref syneSource::CreateSourceFromType(deEngine *engine, deSynthesizerSourceVisitorIdentify::eSourceTypes type){
	switch(type){
	case deSynthesizerSourceVisitorIdentify::estSound:
		return syneSourceSound::Ref::New(engine);
		
	case deSynthesizerSourceVisitorIdentify::estWave:
		return syneSourceWave::Ref::New();
		
	case deSynthesizerSourceVisitorIdentify::estChain:
		return syneSourceChain::Ref::New(engine);
		
	case deSynthesizerSourceVisitorIdentify::estGroup:
		return syneSourceGroup::Ref::New();
		
	case deSynthesizerSourceVisitorIdentify::estSynthesizer:
		return syneSourceSynthesizer::Ref::New(engine);
		
	default:
		DETHROW(deeInvalidParam);
	}
}
