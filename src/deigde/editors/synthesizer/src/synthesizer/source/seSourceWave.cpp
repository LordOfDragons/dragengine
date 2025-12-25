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

#include "seSourceWave.h"
#include "../seSynthesizer.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSource.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceWave.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceVisitorIdentify.h>



// Class seSourceWave
////////////////////////////////////

// Constructor, destructor
////////////////////////////

seSourceWave::seSourceWave() :
seSource(deSynthesizerSourceVisitorIdentify::estWave),
pWaveType(deSynthesizerSourceWave::ewtSine),
pMinFrequency(440.0f),
pMaxFrequency(440.0f){
}

seSourceWave::seSourceWave(const seSourceWave &copy) :
seSource(copy),
pWaveType(copy.pWaveType),
pMinFrequency(copy.pMinFrequency),
pMaxFrequency(copy.pMaxFrequency),
pTargetFrequency(copy.pTargetFrequency){
}

seSourceWave::~seSourceWave(){
}



// Management
///////////////

void seSourceWave::SetWaveType(deSynthesizerSourceWave::eWaveType type){
	if(type < deSynthesizerSourceWave::ewtSine || type > deSynthesizerSourceWave::ewtTriangle){
		DETHROW(deeInvalidParam);
	}
	
	if(type == pWaveType){
		return;
	}
	
	pWaveType = type;
	
	if(GetEngineSource()){
		GetEngineSource().DynamicCast<deSynthesizerSourceWave>()->SetType(type);
		NotifySourceChanged();
	}
}

void seSourceWave::SetMinFrequency(float frequency){
	frequency = decMath::max(frequency, 0.0f);
	if(fabsf(frequency - pMinFrequency) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMinFrequency = frequency;
	
	if(GetEngineSource()){
		GetEngineSource().DynamicCast<deSynthesizerSourceWave>()->SetMinFrequency(frequency);
		NotifySourceChanged();
	}
}

void seSourceWave::SetMaxFrequency(float frequency){
	frequency = decMath::max(frequency, 0.0f);
	if(fabsf(frequency - pMaxFrequency) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	pMaxFrequency = frequency;
	
	if(GetEngineSource()){
		GetEngineSource().DynamicCast<deSynthesizerSourceWave>()->SetMaxFrequency(frequency);
		NotifySourceChanged();
	}
}



void seSourceWave::UpdateTargets(){
	seSource::UpdateTargets();
	
	deSynthesizerSourceWave * const source = GetEngineSource().DynamicCast<deSynthesizerSourceWave>();
	if(source){
		seSynthesizer * const synthesizer = GetSynthesizer();
		DEASSERT_NOTNULL(synthesizer);
		
		pTargetFrequency.UpdateEngineTarget(*synthesizer, source->GetTargetFrequency());
	}
}

int seSourceWave::CountLinkUsage(seLink *link) const{
	int usageCount = seSource::CountLinkUsage(link);
	
	if(pTargetFrequency.GetLinks().Has(link)){
		usageCount++;
	}
	
	return usageCount;
}

void seSourceWave::RemoveLinkFromTargets(seLink *link){
	seSource::RemoveLinkFromTargets(link);
	
	pTargetFrequency.RemoveLink(link);
	
	UpdateTargets();
}

void seSourceWave::RemoveLinksFromAllTargets(){
	seSource::RemoveLinksFromAllTargets();
	
	pTargetFrequency.RemoveAllLinks();
	
	UpdateTargets();
}



deSynthesizerSource::Ref seSourceWave::CreateEngineSource(){
	const deSynthesizerSourceWave::Ref engSource(deSynthesizerSourceWave::Ref::New());
	
	InitEngineSource(engSource);
	
	engSource->SetType(pWaveType);
	engSource->SetMinFrequency(pMinFrequency);
	engSource->SetMaxFrequency(pMaxFrequency);
	
	seSynthesizer * const synthesizer = GetSynthesizer();
	DEASSERT_NOTNULL(synthesizer);
	
	pTargetFrequency.UpdateEngineTarget(*synthesizer, engSource->GetTargetFrequency());
	
	return engSource;
}



seSource::Ref seSourceWave::CreateCopy() const{
	return seSourceWave::Ref::New(*this);
}

void seSourceWave::ListLinks(seLink::List &list){
	seSource::ListLinks(list);
	pTargetFrequency.AddLinksToList(list);
}



// Operators
//////////////

seSourceWave &seSourceWave::operator=(const seSourceWave &copy){
	SetWaveType(copy.pWaveType);
	SetMinFrequency(copy.pMinFrequency);
	SetMaxFrequency(copy.pMaxFrequency);
	pTargetFrequency = copy.pTargetFrequency;
	seSource::operator=(copy);
	return *this;
}
