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

#include "deSynthesizerSource.h"
#include "deSynthesizerSourceVisitor.h"
#include "../deSynthesizer.h"
#include "../effect/deSynthesizerEffect.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// Class deSynthesizerSource
//////////////////////////////

// Constructor, destructor
////////////////////////////

deSynthesizerSource::deSynthesizerSource() :
pSynthesizer(NULL),
pMixMode(emmAdd),
pBlendFactor(1.0f),
pEnabled(true),
pMinVolume(1.0f),
pMaxVolume(1.0f),
pMinPanning(0.0f),
pMaxPanning(0.0f){
}

deSynthesizerSource::~deSynthesizerSource(){
}



// Management
///////////////

void deSynthesizerSource::SetSynthesizer(deSynthesizer *synthesizer){
	pSynthesizer = synthesizer;
}

void deSynthesizerSource::SetEnabled(bool enabled){
	pEnabled = enabled;
}

void deSynthesizerSource::SetMixMode(eMixModes mode){
	if(mode < emmBlend || mode > emmAdd){
		DETHROW(deeInvalidParam);
	}
	pMixMode = mode;
}

void deSynthesizerSource::SetBlendFactor(float factor){
	pBlendFactor = factor;
}

void deSynthesizerSource::SetMinVolume(float volume){
	pMinVolume = volume;
}

void deSynthesizerSource::SetMaxVolume(float volume){
	pMaxVolume = volume;
}

void deSynthesizerSource::SetMinPanning(float panning){
	pMinPanning = panning;
}

void deSynthesizerSource::SetMaxPanning(float panning){
	pMaxPanning = panning;
}



// Effects
////////////

int deSynthesizerSource::GetEffectCount() const{
	return pEffects.GetCount();
}

deSynthesizerEffect *deSynthesizerSource::GetEffectAt(int index) const{
	return (deSynthesizerEffect*)pEffects.GetAt(index);
}

void deSynthesizerSource::AddEffect(deSynthesizerEffect *effect){
	if(! effect){
		DETHROW(deeInvalidParam);
	}
	pEffects.Add(effect);
}

void deSynthesizerSource::RemoveEffect(deSynthesizerEffect *effect){
	pEffects.Remove(effect);
}

void deSynthesizerSource::RemoveAllEffects(){
	pEffects.RemoveAll();
}



// Visiting
/////////////

void deSynthesizerSource::Visit(deSynthesizerSourceVisitor &visitor){
	visitor.VisitSource(*this);
}
