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

#include "desynSynthesizerEffectStretch.h"
#include "../desynSynthesizer.h"
#include "../desynSynthesizerInstance.h"
#include "../source/desynSynthesizerSource.h"
#include "../../deDESynthesizer.h"

#include <soundtouch/SoundTouch.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/effect/deSynthesizerEffectStretch.h>



// State data struct
//////////////////////

struct sStateData{
	soundtouch::SoundTouch *stouch;
	int blockSamples;
	float *inputBuffer;
};



// Class desynSynthesizerEffectStretch
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

desynSynthesizerEffectStretch::desynSynthesizerEffectStretch(const desynSynthesizer &synthesizer,
int firstLink, desynSynthesizerSource &source, const deSynthesizerEffectStretch &effect) :
desynSynthesizerEffect(synthesizer, firstLink, source, effect),

pMinTime(decMath::clamp(effect.GetMinTime(), -0.75f, 1.5f)),
pMaxTime(decMath::clamp(effect.GetMaxTime(), -0.75f, 1.5f)),
pRangeTime(pMaxTime - pMinTime),

pMinPitch(decMath::clamp(effect.GetMinPitch(), -0.75f, 1.5f)),
pMaxPitch(decMath::clamp(effect.GetMaxPitch(), -0.75f, 1.5f)),
pRangePitch(pMaxPitch - pMinPitch),

pTargetTime(synthesizer, firstLink, effect.GetTargetTime()),
pTargetPitch(synthesizer, firstLink, effect.GetTargetPitch())
{
	SetDisabled(! effect.GetEnabled());
	if(GetDisabled()){
		return;
	}
}

desynSynthesizerEffectStretch::~desynSynthesizerEffectStretch(){
}



// Management
///////////////

float desynSynthesizerEffectStretch::GetTimeStretch(const desynSynthesizerInstance &instance, int sample) const{
	return pMinTime + pRangeTime * pTargetTime.GetValue(instance, sample, 0.0f);
}

float desynSynthesizerEffectStretch::GetPitchShift(const desynSynthesizerInstance &instance, int sample) const{
	return pMinPitch + pRangePitch * pTargetPitch.GetValue(instance, sample, 0.0f);
}



int desynSynthesizerEffectStretch::StateDataSize(int offset){
	SetStateDataOffset(offset);
	return sizeof(sStateData);
}

void desynSynthesizerEffectStretch::InitStateData(char *stateData){
	sStateData& sdata = *((sStateData*)(stateData + GetStateDataOffset()));
	sdata.stouch = NULL;
	sdata.inputBuffer = NULL;
	
	const int sampleRate = GetSynthesizer().GetSynthesizer().GetSampleRate();
	const int channelCount = GetSynthesizer().GetSynthesizer().GetChannelCount();
	
	sdata.blockSamples = sampleRate / 100; // 10ms block size
	sdata.inputBuffer = new float[sdata.blockSamples * channelCount];
	
	sdata.stouch = new soundtouch::SoundTouch;
	sdata.stouch->setRate(1.0);
	sdata.stouch->setTempo(1.0);
	sdata.stouch->setPitch(1.0);
	sdata.stouch->setChannels(channelCount);
	sdata.stouch->setSampleRate(sampleRate);
	sdata.stouch->setSetting(SETTING_USE_AA_FILTER, 0); // 0(disabled,default), 1(enabled)
	sdata.stouch->setSetting(SETTING_USE_QUICKSEEK, 1); // 0(disabled), 1(enabled)
	//sdata.stouch->setSetting( SETTING_SEQUENCE_MS, ... );
	//sdata.stouch->setSetting( SETTING_SEEKWINDOW_MS, ... );
	//sdata.stouch->setSetting( SETTING_OVERLAP_MS, ... );
	
}

void desynSynthesizerEffectStretch::CleanUpStateData(char *stateData){
	sStateData& sdata = *((sStateData*)(stateData + GetStateDataOffset()));
	
	if(sdata.stouch){
		delete sdata.stouch;
		sdata.stouch = NULL;
	}
	
	if(sdata.inputBuffer){
		delete [] sdata.inputBuffer;
		sdata.inputBuffer = NULL;
	}
}



void desynSynthesizerEffectStretch::GenerateSound(const desynSynthesizerInstance &instance,
char *stateData, float *buffer, int samples, float curveOffset, float curveFactor){
// 	desynSynthesizerEffect::GenerateSound( instance, stateData, buffer, samples );
	
	desynSynthesizerEffect * const previousEffect = GetPreviousEffect();
	desynSynthesizerSource &source = GetSource();
	
// 	const int sampleRate = instance.GetSampleRate();
	const int channelCount = instance.GetChannelCount();
	
	sStateData& sdata = *((sStateData*)(stateData + GetStateDataOffset()));
	const float defBlockCurveFactor = curveFactor * ((float)sdata.blockSamples / (float)samples);
	const float defBlockCurveLast = curveOffset + curveFactor * (float)(samples - 1);
	float *bufferPtr = buffer;
	int offset = 0;
	
	while(offset < samples){
		const int curveEvalPos = NearestCurveEvalPosition(offset, curveOffset, curveFactor);
		const float pitchShift = GetPitchShift(instance, curveEvalPos);
		const float timeStretch = GetTimeStretch(instance, curveEvalPos);
		//GetModule().LogInfoFormat( "Stretch: offset=%i samples=%i pitch=%g stretch=%f\n", offset, samples, pitchShift, timeStretch );
		
		sdata.stouch->setPitch(1.0f + pitchShift);
		sdata.stouch->setTempo(1.0f + timeStretch);
		
		int remainingSamples = decMath::min(samples - offset, sdata.blockSamples);
		while(remainingSamples > 0){
			const int processedSamples = sdata.stouch->receiveSamples(bufferPtr, remainingSamples);
			//GetModule().LogInfoFormat( "Stretch: remaining=%i processed=%i\n", remainingSamples, processedSamples );
			bufferPtr += processedSamples * channelCount;
			offset += processedSamples;
			remainingSamples -= processedSamples;
			
			if(remainingSamples > 0){
				// due to the potential time stretching the curve offset and factor are different
				// for the block to sample in. the sampled block curve parameters are a sub-range
				// of the curve parameters used for the effect. thus the block to sample in can
				// not evaluate the curve outside the prepared range
				const float blockCurveOffset = (float)curveEvalPos;
				const float blockCurveFactor = decMath::min(defBlockCurveFactor, defBlockCurveLast - blockCurveOffset);
				
				if(previousEffect){
					previousEffect->GenerateSound(instance, stateData, sdata.inputBuffer,
						sdata.blockSamples, blockCurveOffset, blockCurveFactor);
					
				}else{
					source.GenerateSourceSound(instance, stateData, sdata.inputBuffer,
						sdata.blockSamples, blockCurveOffset, blockCurveFactor);
				}
				
				sdata.stouch->putSamples(sdata.inputBuffer, sdata.blockSamples);
			}
		}
	}
}

void desynSynthesizerEffectStretch::SkipSound(const desynSynthesizerInstance &instance,
char *stateData, int samples, float curveOffset, float curveFactor){
	// TODO IMPLEMENT!!!
	desynSynthesizerEffect::SkipSound(instance, stateData, samples, curveOffset, curveFactor);
}
