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

#include "desynSynthesizerSourceWave.h"
#include "../desynSynthesizerInstance.h"
#include "../../deDESynthesizer.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceWave.h>



// Definitions
////////////////

#define PI2 (PI * 2.0f)

struct sStateData{
	float phase;
};



// Class desynSynthesizerSourceWave
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

desynSynthesizerSourceWave::desynSynthesizerSourceWave(desynSynthesizer &synthesizer,
int firstLink, const deSynthesizerSourceWave &source) :
desynSynthesizerSource(synthesizer, firstLink, source),
pType(source.GetType()),
pMinFrequency(source.GetMinFrequency()),
pMaxFrequency(source.GetMaxFrequency()),
pFrequencyRange(pMaxFrequency - pMinFrequency),
pTargetFrequency(synthesizer, firstLink, source.GetTargetFrequency())
{
	SetSilent(! source.GetEnabled());
	if(GetSilent()){
		return;
	}
}

desynSynthesizerSourceWave::~desynSynthesizerSourceWave(){
}



// Management
///////////////

float desynSynthesizerSourceWave::GetFrequency(const desynSynthesizerInstance &instance, int sample) const{
	return pMinFrequency + pFrequencyRange * pTargetFrequency.GetValue(instance, sample, 0.0f);
}



int desynSynthesizerSourceWave::StateDataSizeSource(int offset){
	return (int)sizeof(sStateData);
}

void desynSynthesizerSourceWave::InitStateDataSource(char *stateData){
	sStateData& sdata = *((sStateData*)(stateData + GetStateDataOffset()));
	sdata.phase = 0.0f;
}



void desynSynthesizerSourceWave::GenerateSourceSound(const desynSynthesizerInstance &instance,
char *stateData, float *buffer, int samples, float curveOffset, float curveFactor){
	switch(pType){
	case deSynthesizerSourceWave::ewtSine:
		GenerateSineWave(instance, stateData, buffer, samples, curveOffset, curveFactor);
		break;
		
	case deSynthesizerSourceWave::ewtSquare:
		GenerateSquareWave(instance, stateData, buffer, samples, curveOffset, curveFactor);
		break;
		
	case deSynthesizerSourceWave::ewtSawTooth:
		GenerateSawToothWave(instance, stateData, buffer, samples, curveOffset, curveFactor);
		break;
		
	case deSynthesizerSourceWave::ewtTriangle:
		GenerateTriangleWave(instance, stateData, buffer, samples, curveOffset, curveFactor);
		break;
	};
}

void desynSynthesizerSourceWave::GenerateSineWave(const desynSynthesizerInstance &instance,
char *stateData, float *buffer, int samples, float curveOffset, float curveFactor){
	sStateData& sdata = *((sStateData*)(stateData + GetStateDataOffset()));
	const int channelCount = instance.GetChannelCount();
	const float invSampleRate = instance.GetInverseSampleRate();
	int i;
	
	if(channelCount == 1){
		sGenerateBufferMono * const sbuf = (sGenerateBufferMono*)buffer;
		
		for(i=0; i<samples; i++){
			const int curveEvalPos = NearestCurveEvalPosition(i, curveOffset, curveFactor);
			sbuf[i].value = sinf(sdata.phase * PI2);
			sdata.phase += GetFrequency(instance, curveEvalPos) * invSampleRate;
		}
		
	}else if(channelCount == 2){
		sGenerateBufferStereo * const sbuf = (sGenerateBufferStereo*)buffer;
		
		for(i=0; i<samples; i++){
			const int curveEvalPos = NearestCurveEvalPosition(i, curveOffset, curveFactor);
			const float panning = GetPanning(instance, curveEvalPos);
			const float value = sinf(sdata.phase * PI2);
			
			sbuf[i].left = decMath::min(1.0f - panning, 1.0f) * value;
			sbuf[i].right = decMath::min(1.0f + panning, 1.0f) * value;
			
			sdata.phase += GetFrequency(instance, curveEvalPos) * invSampleRate;
		}
	}
	
	float intpart;
	sdata.phase = modff(sdata.phase, &intpart);
}

void desynSynthesizerSourceWave::GenerateSquareWave(const desynSynthesizerInstance &instance,
char *stateData, float *buffer, int samples, float curveOffset, float curveFactor){
	sStateData& sdata = *((sStateData*)(stateData + GetStateDataOffset()));
	const int channelCount = instance.GetChannelCount();
	const float invSampleRate = instance.GetInverseSampleRate();
	float intpart;
	int i;
	
	if(channelCount == 1){
		sGenerateBufferMono * const sbuf = (sGenerateBufferMono*)buffer;
		
		for(i=0; i<samples; i++){
			const int curveEvalPos = NearestCurveEvalPosition(i, curveOffset, curveFactor);
			
			if(sdata.phase < 0.5f){
				sbuf[i].value = 1.0f;
				
			}else{
				sbuf[i].value = -1.0f;
			}
			
			sdata.phase = modff(sdata.phase + GetFrequency(instance, curveEvalPos) * invSampleRate, &intpart);
		}
		
	}else if(channelCount == 2){
		sGenerateBufferStereo * const sbuf = (sGenerateBufferStereo*)buffer;
		
		for(i=0; i<samples; i++){
			const int curveEvalPos = NearestCurveEvalPosition(i, curveOffset, curveFactor);
			const float panning = GetPanning(instance, curveEvalPos);
			const float panLeft = decMath::min(1.0f - panning, 1.0f);
			const float panRight = decMath::min(1.0f + panning, 1.0f);
			
			if(sdata.phase < 0.5f){
				sbuf[i].left = panLeft;
				sbuf[i].right = panRight;
				
			}else{
				sbuf[i].left = -panLeft;
				sbuf[i].right = -panRight;
			}
			
			sdata.phase = modff(sdata.phase + GetFrequency(instance, curveEvalPos) * invSampleRate, &intpart);
		}
	}
}

void desynSynthesizerSourceWave::GenerateSawToothWave(const desynSynthesizerInstance &instance,
char *stateData, float *buffer, int samples, float curveOffset, float curveFactor){
	sStateData& sdata = *((sStateData*)(stateData + GetStateDataOffset()));
	const int channelCount = instance.GetChannelCount();
	const float invSampleRate = instance.GetInverseSampleRate();
	float intpart;
	int i;
	
	if(channelCount == 1){
		sGenerateBufferMono * const sbuf = (sGenerateBufferMono*)buffer;
		
		for(i=0; i<samples; i++){
			const int curveEvalPos = NearestCurveEvalPosition(i, curveOffset, curveFactor);
			sbuf[i].value = sdata.phase - 1.0f;
			sdata.phase = modff(sdata.phase + GetFrequency(instance, curveEvalPos) * invSampleRate, &intpart);
		}
		
	}else if(channelCount == 2){
		sGenerateBufferStereo * const sbuf = (sGenerateBufferStereo*)buffer;
		
		for(i=0; i<samples; i++){
			const int curveEvalPos = NearestCurveEvalPosition(i, curveOffset, curveFactor);
			const float panning = GetPanning(instance, curveEvalPos);
			const float value = sdata.phase - 1.0f;
			sbuf[i].left = decMath::min(1.0f - panning, 1.0f) * value;
			sbuf[i].right = decMath::min(1.0f + panning, 1.0f) * value;
			
			sdata.phase = modff(sdata.phase + GetFrequency(instance, curveEvalPos) * invSampleRate, &intpart);
		}
	}
}

void desynSynthesizerSourceWave::GenerateTriangleWave(const desynSynthesizerInstance &instance,
char *stateData, float *buffer, int samples, float curveOffset, float curveFactor){
	sStateData& sdata = *((sStateData*)(stateData + GetStateDataOffset()));
	const int channelCount = instance.GetChannelCount();
	const float invSampleRate = instance.GetInverseSampleRate();
	float intpart;
	int i;
	
	if(channelCount == 1){
		sGenerateBufferMono * const sbuf = (sGenerateBufferMono*)buffer;
		
		for(i=0; i<samples; i++){
			const int curveEvalPos = NearestCurveEvalPosition(i, curveOffset, curveFactor);
			const float fract = sdata.phase * 4.0f;
			
			if(fract < 1.0f){
				sbuf[i].value = fract;
				
			}else if(fract > 3.0f){
				sbuf[i].value = fract - 4.0f;
				
			}else{
				sbuf[i].value = 2.0f - fract;
			}
			
			sdata.phase = modff(sdata.phase + GetFrequency(instance, curveEvalPos) * invSampleRate, &intpart);
		}
		
	}else if(channelCount == 2){
		sGenerateBufferStereo * const sbuf = (sGenerateBufferStereo*)buffer;
		
		for(i=0; i<samples; i++){
			const int curveEvalPos = NearestCurveEvalPosition(i, curveOffset, curveFactor);
			const float panning = GetPanning(instance, curveEvalPos);
			
			const float fract = sdata.phase * 4.0f;
			float value;
			
			if(fract < 1.0f){
				value = fract;
				
			}else if(fract > 3.0f){
				value = fract - 4.0f;
				
			}else{
				value = 2.0f - fract;
			}
			
			sbuf[i].left = decMath::min(1.0f - panning, 1.0f) * value;
			sbuf[i].right = decMath::min(1.0f + panning, 1.0f) * value;
			
			sdata.phase = modff(sdata.phase + GetFrequency(instance, curveEvalPos) * invSampleRate, &intpart);
		}
	}
}

void desynSynthesizerSourceWave::SkipSourceSound(const desynSynthesizerInstance &instance,
char *stateData, int samples, float curveOffset, float curveFactor){
}
