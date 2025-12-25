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

#include "seSource.h"
#include "seSourceSound.h"
#include "seSourceWave.h"
#include "seSourceSynthesizer.h"
#include "seSourceGroup.h"
#include "seSourceChain.h"
#include "../seSynthesizer.h"
#include "../link/seLink.h"
#include "../effect/seEffect.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSource.h>



// Definitions
////////////////

#define LOGSOURCE "Synthesizer Editor"



// Class seSource
///////////////////

// Constructor, destructor
////////////////////////////

seSource::seSource(deSynthesizerSourceVisitorIdentify::eSourceTypes type) :
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
pMaxPanning(0.0f)
{
	if(type < deSynthesizerSourceVisitorIdentify::estSound || type > deSynthesizerSourceVisitorIdentify::estGroup){
		DETHROW(deeInvalidParam);
	}
}

seSource::seSource(const seSource &copy) :
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
pTargetBlendFactor(copy.pTargetBlendFactor),
pTargetVolume(copy.pTargetVolume),
pTargetPanning(copy.pTargetPanning)
{
	copy.pEffects.Visit([&](const seEffect &e){
		const seEffect::Ref effect(e.CreateCopy());
		pEffects.Add(effect);
		effect->SetParentSource(this);
	});
}

seSource::~seSource(){
	SetSynthesizer(nullptr);
}



// Management
///////////////

seSynthesizer *seSource::GetSynthesizer() const{
	return pParentGroup ? pParentGroup->GetSynthesizer() : pSynthesizer;
}

void seSource::SetSynthesizer(seSynthesizer *synthesizer){
	if(synthesizer == pSynthesizer){
		return;
	}
	
	pEngSource = nullptr;
	pSynthesizer = synthesizer;
	SynthesizerChanged();
}



void seSource::SetEngineSource(deSynthesizerSource *engSource){
	pEngSource = engSource;
}

void seSource::InitEngineSource(deSynthesizerSource *engSource) const{
	if(!engSource){
		DETHROW(deeInvalidParam);
	}
	
	seSynthesizer * const synthesizer = GetSynthesizer();
	DEASSERT_NOTNULL(synthesizer);
	
	engSource->SetEnabled(pEnabled);
	engSource->SetMixMode(pMixMode);
	engSource->SetBlendFactor(pBlendFactor);
	engSource->SetMinVolume(pMinVolume);
	engSource->SetMaxVolume(pMaxVolume);
	engSource->SetMinPanning(pMinPanning);
	engSource->SetMaxPanning(pMaxPanning);
	
	pTargetBlendFactor.UpdateEngineTarget(*synthesizer, engSource->GetTargetBlendFactor());
	pTargetVolume.UpdateEngineTarget(*synthesizer, engSource->GetTargetVolume());
	pTargetPanning.UpdateEngineTarget(*synthesizer, engSource->GetTargetPanning());
	
	// effects
	pEffects.Visit([&](seEffect &effect){
		effect.SetEngineEffect(nullptr);
	});
	
	pEffects.Visit([&](seEffect &effect){
		const deSynthesizerEffect::Ref engEffect(effect.CreateEngineEffect());
		engSource->AddEffect(engEffect);
		effect.SetEngineEffect(engEffect);
	});
}



void seSource::SetParentGroup(seSourceGroup *group){
	pParentGroup = group;
}



void seSource::SetName(const char *name){
	if(pName == name){
		return;
	}
	
	pName = name;
	
	seSynthesizer * const synthesizer = GetSynthesizer();
	if(synthesizer){
		synthesizer->NotifySourceNameChanged(this);
	}
}

void seSource::SetEnabled(bool enabled){
	if(enabled == pEnabled){
		return;
	}
	
	pEnabled = enabled;
	
	if(pEngSource){
		pEngSource->SetEnabled(enabled);
	}
	
	NotifySourceChanged();
}

void seSource::SetMixMode(deSynthesizerSource::eMixModes mode){
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

void seSource::SetBlendFactor(float factor){
	if(fabsf(factor - pBlendFactor) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pBlendFactor = factor;
	
	if(pEngSource){
		pEngSource->SetBlendFactor(factor);
	}
	
	NotifySourceChanged();
}

void seSource::SetMinVolume(float volume){
	if(fabsf(volume - pMinVolume) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMinVolume = volume;
	
	if(pEngSource){
		pEngSource->SetMinVolume(volume);
	}
	
	NotifySourceChanged();
}

void seSource::SetMaxVolume(float volume){
	if(fabsf(volume - pMaxVolume) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMaxVolume = volume;
	
	if(pEngSource){
		pEngSource->SetMaxVolume(volume);
	}
	
	NotifySourceChanged();
}

void seSource::SetMinPanning(float panning){
	if(fabsf(panning - pMinPanning) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMinPanning = panning;
	
	if(pEngSource){
		pEngSource->SetMinPanning(panning);
	}
	
	NotifySourceChanged();
}

void seSource::SetMaxPanning(float panning){
	if(fabsf(panning - pMaxPanning) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMaxPanning = panning;
	
	if(pEngSource){
		pEngSource->SetMaxPanning(panning);
	}
	
	NotifySourceChanged();
}



void seSource::UpdateTargets(){
	seSynthesizer * const synthesizer = GetSynthesizer();
	if(pEngSource && synthesizer){
		pTargetBlendFactor.UpdateEngineTarget(*synthesizer, pEngSource->GetTargetBlendFactor());
		pTargetVolume.UpdateEngineTarget(*synthesizer, pEngSource->GetTargetVolume());
		pTargetPanning.UpdateEngineTarget(*synthesizer, pEngSource->GetTargetPanning());
	}
	
	const int effectCount = pEffects.GetCount();
	int i;
	for(i=0; i< effectCount; i++){
		pEffects.GetAt(i)->UpdateTargets();
	}
}

int seSource::CountLinkUsage(seLink *link) const{
	int usageCount = 0;
	
	if(pTargetBlendFactor.GetLinks().Has(link)){
		usageCount++;
	}
	if(pTargetVolume.GetLinks().Has(link)){
		usageCount++;
	}
	if(pTargetPanning.GetLinks().Has(link)){
		usageCount++;
	}
	
	pEffects.Visit([&](const seEffect &effect){
		usageCount += effect.CountLinkUsage(link);
	});
	
	return usageCount;
}

void seSource::RemoveLinkFromTargets(seLink *link){
	pTargetBlendFactor.RemoveLink(link);
	pTargetVolume.RemoveLink(link);
	pTargetPanning.RemoveLink(link);
	
	pEffects.Visit([&](seEffect &effect){
		effect.RemoveLinkFromTargets(link);
	});
}

void seSource::RemoveLinksFromAllTargets(){
	pTargetBlendFactor.RemoveAllLinks();
	pTargetVolume.RemoveAllLinks();
	pTargetPanning.RemoveAllLinks();
	
	pEffects.Visit([&](seEffect &effect){
		effect.RemoveLinksFromAllTargets();
	});
}



void seSource::ListLinks(seLink::List &list){
	pTargetBlendFactor.AddLinksToList(list);
	pTargetVolume.AddLinksToList(list);
	pTargetPanning.AddLinksToList(list);
	
	pEffects.Visit([&](seEffect &effect){
		effect.ListLinks(list);
	});
}



void seSource::NotifySourceChanged(){
	seSynthesizer * const synthesizer = GetSynthesizer();
	if(!synthesizer){
		return;
	}
	
	UpdateTargets();
	
	if(pEngSource){
		synthesizer->GetEngineSynthesizer()->NotifySourcesChanged();
	}
	
	synthesizer->NotifySourceChanged(this);
}

void seSource::NotifyEngineOnlySourceChanged(){
	seSynthesizer * const synthesizer = GetSynthesizer();
	if(!synthesizer || !pEngSource){
		return;
	}
	
	synthesizer->GetEngineSynthesizer()->NotifySourcesChanged();
}
void seSource::SynthesizerChanged(){
	pEffects.Visit([&](seEffect &e){
		e.SynthesizerChanged();
	});
}

void seSource::SynthesizerDirectoryChanged(){
	pEffects.Visit([&](seEffect &e){
		e.SynthesizerDirectoryChanged();
	});
}



// Effects
////////////

void seSource::SetActiveEffect(seEffect *effect){
	if(effect == pActiveEffect){
		return;
	}
	pActiveEffect = effect;
	seSynthesizer * const synthesizer = GetSynthesizer();
	if(synthesizer){
		synthesizer->NotifyActiveEffectChanged(this);
	}
}

void seSource::AddEffect(seEffect *effect){
	pEffects.Add(effect);
	
	effect->SetParentSource(this);
	
	seSynthesizer * const synthesizer = GetSynthesizer();
	if(synthesizer){
		synthesizer->RebuildSources();
		synthesizer->NotifyEffectStructureChanged(this);
	}
	
	if(!pActiveEffect){
		SetActiveEffect(effect);
	}
}

void seSource::InsertEffectAt(seEffect *effect, int index){
	pEffects.Insert(effect, index);
	
	effect->SetParentSource(this);
	
	seSynthesizer * const synthesizer = GetSynthesizer();
	if(synthesizer){
		synthesizer->RebuildSources();
		synthesizer->NotifyEffectStructureChanged(this);
	}
	
	if(!pActiveEffect){
		SetActiveEffect(effect);
	}
}

void seSource::MoveEffectTo(seEffect *effect, int index){
	pEffects.Move(effect, index);
	
	seSynthesizer * const synthesizer = GetSynthesizer();
	if(synthesizer){
		synthesizer->RebuildSources();
		synthesizer->NotifyEffectStructureChanged(this);
	}
}

void seSource::RemoveEffect(seEffect *effect){
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
	
	seSynthesizer * const synthesizer = GetSynthesizer();
	if(synthesizer){
		synthesizer->RebuildSources();
		synthesizer->NotifyEffectStructureChanged(this);
	}
}

void seSource::RemoveAllEffects(){
	SetActiveEffect(nullptr);
	
	pEffects.Visit([&](seEffect &effect){
		effect.SetParentSource(nullptr);
	});
	pEffects.RemoveAll();
	
	seSynthesizer * const synthesizer = GetSynthesizer();
	if(synthesizer){
		synthesizer->RebuildSources();
		synthesizer->NotifyEffectStructureChanged(this);
	}
}



// Operators
//////////////

seSource &seSource::operator=(const seSource &copy){
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
	copy.pEffects.Visit([&](const seEffect &e){
		AddEffect(e.CreateCopy());
	});
	
	NotifySourceChanged();
	
	return *this;
}



// Helper
///////////

seSource::Ref seSource::CreateSourceFromType(deEngine *engine, deSynthesizerSourceVisitorIdentify::eSourceTypes type){
	switch(type){
	case deSynthesizerSourceVisitorIdentify::estSound:
		return seSourceSound::Ref::New(engine);
		
	case deSynthesizerSourceVisitorIdentify::estWave:
		return seSourceWave::Ref::New();
		
	case deSynthesizerSourceVisitorIdentify::estChain:
		return seSourceChain::Ref::New(engine);
		
	case deSynthesizerSourceVisitorIdentify::estGroup:
		return seSourceGroup::Ref::New();
		
	case deSynthesizerSourceVisitorIdentify::estSynthesizer:
		return seSourceSynthesizer::Ref::New(engine);
		
	default:
		DETHROW(deeInvalidParam);
	}
}
