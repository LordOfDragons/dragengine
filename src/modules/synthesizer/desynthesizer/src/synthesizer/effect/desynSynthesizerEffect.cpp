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

#include "desynSynthesizerEffect.h"
#include "../desynSynthesizer.h"
#include "../desynSynthesizerInstance.h"
#include "../source/desynSynthesizerSource.h"
#include "../../deDESynthesizer.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/synthesizer/effect/deSynthesizerEffect.h>



// Class desynSynthesizerEffect
/////////////////////////////////

// Constructor, destructor
////////////////////////////

desynSynthesizerEffect::desynSynthesizerEffect(const desynSynthesizer &synthesizer,
int firstLink, desynSynthesizerSource &source, const deSynthesizerEffect &effect) :
pSynthesizer(synthesizer),
pSource(source),
pDisabled(true),
pStateDataOffset(0),

pTargetStrength(synthesizer, firstLink, effect.GetTargetStrength()),

pPreviousEffect(NULL){
}

desynSynthesizerEffect::~desynSynthesizerEffect(){
}



// Management
///////////////

deDESynthesizer &desynSynthesizerEffect::GetModule() const{
	return pSynthesizer.GetModule();
}

void desynSynthesizerEffect::SetDisabled(bool disabled){
	pDisabled = disabled;
}

void desynSynthesizerEffect::SetStateDataOffset(int offset){
	pStateDataOffset = decMath::max(offset, 0);
}



void desynSynthesizerEffect::SetPreviousEffect(desynSynthesizerEffect *effect){
	pPreviousEffect = effect;
}



float desynSynthesizerEffect::GetStrength(const desynSynthesizerInstance &instance, int sample) const{
	return pTargetStrength.GetValue(instance, sample, 1.0f);
}



int desynSynthesizerEffect::StateDataSize(int offset){
	pStateDataOffset = offset;
	return 0;
}

void desynSynthesizerEffect::InitStateData(char *stateData){
}

void desynSynthesizerEffect::CleanUpStateData(char *stateData){
}



void desynSynthesizerEffect::GenerateSound(const desynSynthesizerInstance &instance,
char *stateData, float *buffer, int samples, float curveOffset, float curveFactor){
	if(pPreviousEffect){
		pPreviousEffect->GenerateSound(instance, stateData, buffer, samples, curveOffset, curveFactor);
		
	}else{
		pSource.GenerateSourceSound(instance, stateData, buffer, samples, curveOffset, curveFactor);
	}
}

void desynSynthesizerEffect::SkipSound(const desynSynthesizerInstance &instance,
char *stateData, int samples, float curveOffset, float curveFactor){
	if(pPreviousEffect){
		pPreviousEffect->SkipSound(instance, stateData, samples, curveOffset, curveFactor);
		
	}else{
		pSource.SkipSourceSound(instance, stateData, samples, curveOffset, curveFactor);
	}
}
