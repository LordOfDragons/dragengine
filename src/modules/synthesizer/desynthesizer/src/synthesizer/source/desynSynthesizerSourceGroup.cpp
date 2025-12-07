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

#include "desynSynthesizerSourceGroup.h"
#include "../desynCreateSynthesizerSource.h"
#include "../desynSynthesizerInstance.h"
#include "../../deDESynthesizer.h"
#include "../../buffer/desynSharedBuffer.h"
#include "../../buffer/desynSharedBufferList.h"

#include <dragengine/common/exceptions.h>



// Class desynSynthesizerSourceGroup
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

desynSynthesizerSourceGroup::desynSynthesizerSourceGroup(desynSynthesizer &synthesizer,
int firstLink, const deSynthesizerSourceGroup &source) :
desynSynthesizerSource(synthesizer, firstLink, source),
pSources(NULL),
pSourceCount(0),
pApplicationType(source.GetApplicationType()),
pSelectRange(0.0f),
pTargetSelect(synthesizer, firstLink, source.GetTargetSelect())
{
	SetSilent(!source.GetEnabled());
	if(GetSilent()){
		return;
	}
	
	try{
		pCreateSources(synthesizer, firstLink, source);
		pSelectRange = (float)(pSourceCount - 1);
		
	}catch(const deException &){
		pClearSources();
		throw;
	}
}

desynSynthesizerSourceGroup::~desynSynthesizerSourceGroup(){
	pClearSources();
}



// Management
///////////////

float desynSynthesizerSourceGroup::GetSelect(const desynSynthesizerInstance &instance, int sample) const{
	return pTargetSelect.GetValue(instance, sample, 0.0f) * pSelectRange;
}



int desynSynthesizerSourceGroup::StateDataSizeSource(int offset){
	int count = 0;
	
	if(pApplicationType != deSynthesizerSourceGroup::eatAll){
		count += pSourceCount * sizeof(desynSharedBuffer*);
	}
	
	int i;
	for(i=0; i<pSourceCount; i++){
		count += pSources[i]->StateDataSize(offset + count);
	}
	
	return count;
}

void desynSynthesizerSourceGroup::InitStateDataSource(char *stateData){
	int i;
	
	if(pApplicationType != deSynthesizerSourceGroup::eatAll){
		desynSharedBuffer ** const sdata = (desynSharedBuffer**)(stateData + GetStateDataOffset());
		for(i=0; i<pSourceCount; i++){
			sdata[i] = NULL;
		}
	}
	
	for(i=0; i<pSourceCount; i++){
		pSources[i]->InitStateData(stateData);
	}
}

void desynSynthesizerSourceGroup::GenerateSourceSound(const desynSynthesizerInstance &instance,
char *stateData, float *buffer, int samples, float curveOffset, float curveFactor){
	switch(pApplicationType){
	case deSynthesizerSourceGroup::eatAll:
		GenerateSoundAll(instance, stateData, buffer, samples, curveOffset, curveFactor);
		break;
		
	case deSynthesizerSourceGroup::eatSelect:
		GenerateSoundSelect(instance, stateData, buffer, samples, curveOffset, curveFactor);
		break;
		
	case deSynthesizerSourceGroup::eatSolo:
		GenerateSoundSolo(instance, stateData, buffer, samples, curveOffset, curveFactor);
		break;
	}
}

void desynSynthesizerSourceGroup::GenerateSoundAll(const desynSynthesizerInstance &instance,
char *stateData, float *buffer, int samples, float curveOffset, float curveFactor){
	int i;
	
	GenerateSilence(instance, buffer, samples);
	for(i=0; i<pSourceCount; i++){
		pSources[i]->GenerateSound(instance, stateData, buffer, samples, curveOffset, curveFactor);
	}
}

void desynSynthesizerSourceGroup::GenerateSoundSelect(const desynSynthesizerInstance &instance,
char *stateData, float *buffer, int samples, float curveOffset, float curveFactor){
	desynSharedBuffer ** const sdata = (desynSharedBuffer**)(stateData + GetStateDataOffset());
	desynSharedBufferList &sharedBufferList = GetModule().GetSharedBufferList();
	const int channelCount = instance.GetChannelCount();
	const int lastSource = pSourceCount - 1;
	float intpart;
	int i;
	
	try{
		if(channelCount == 1){
			sGenerateBufferMono * const dbuf = (sGenerateBufferMono*)buffer;
			
			for(i=0; i<samples; i++){
				const int curveEvalPos = NearestCurveEvalPosition(i, curveOffset, curveFactor);
				const float selectBlend = modff(GetSelect(instance, curveEvalPos), &intpart);
				const int selectFirst = decMath::clamp((int)intpart, 0, lastSource);
				const int selectSecond = decMath::min((selectFirst + 1), lastSource);
				
				if(!sdata[selectFirst]){
					sdata[selectFirst] = sharedBufferList.ClaimBuffer(samples);
					memset(sdata[selectFirst]->GetBuffer(), 0, sizeof(float) * samples);
					pSources[selectFirst]->GenerateSound(instance, stateData,
						sdata[selectFirst]->GetBuffer(), samples, curveOffset, curveFactor);
				}
				const sGenerateBufferMono &buffer1 = *((const sGenerateBufferMono *)sdata[selectFirst]->GetBuffer() + i);
				
				if(!sdata[selectSecond]){
					sdata[selectSecond] = sharedBufferList.ClaimBuffer(samples);
					memset(sdata[selectSecond]->GetBuffer(), 0, sizeof(float) * samples);
					pSources[selectSecond]->GenerateSound(instance, stateData,
						sdata[selectSecond]->GetBuffer(), samples, curveOffset, curveFactor);
				}
				const sGenerateBufferMono &buffer2 = *((const sGenerateBufferMono *)sdata[selectSecond]->GetBuffer() + i);
				
				dbuf[i].value = decMath::mix(buffer1.value, buffer2.value, selectBlend);
			}
			
		}else if(channelCount == 2){
			sGenerateBufferStereo * const dbuf = (sGenerateBufferStereo*)buffer;
			
			for(i=0; i<samples; i++){
				const int curveEvalPos = NearestCurveEvalPosition(i, curveOffset, curveFactor);
				const float selectBlend = modff(GetSelect(instance, curveEvalPos), &intpart);
				const int selectFirst = decMath::clamp((int)intpart, 0, lastSource);
				const int selectSecond = decMath::min((selectFirst + 1), lastSource);
				
				if(!sdata[selectFirst]){
					sdata[selectFirst] = sharedBufferList.ClaimBuffer(samples * 2);
					memset(sdata[selectFirst]->GetBuffer(), 0, sizeof(float) * samples * 2);
					pSources[selectFirst]->GenerateSound(instance, stateData,
						sdata[selectFirst]->GetBuffer(), samples, curveOffset, curveFactor);
				}
				const sGenerateBufferStereo &buffer1 = *((const sGenerateBufferStereo*)sdata[selectFirst]->GetBuffer() + i);
				
				if(!sdata[selectSecond]){
					sdata[selectSecond] = sharedBufferList.ClaimBuffer(samples * 2);
					memset(sdata[selectSecond]->GetBuffer(), 0, sizeof(float) * samples * 2);
					pSources[selectSecond]->GenerateSound(instance, stateData,
						sdata[selectSecond]->GetBuffer(), samples, curveOffset, curveFactor);
				}
				const sGenerateBufferStereo &buffer2 = *((const sGenerateBufferStereo*)sdata[selectSecond]->GetBuffer() + i);
				
				dbuf[i].left = decMath::mix(buffer1.left, buffer2.left, selectBlend);
				dbuf[i].right = decMath::mix(buffer1.right, buffer2.right, selectBlend);
			}
		}
		
		for(i=0; i<pSourceCount; i++){
			if(sdata[i]){
				sharedBufferList.ReleaseBuffer(sdata[i]);
				sdata[i] = NULL;
				
			}else{
				pSources[i]->SkipSound(instance, stateData, samples, curveOffset, curveFactor);
			}
		}
		
	}catch(const deException &){
		for(i=0; i<pSourceCount; i++){
			if(sdata[i]){
				sharedBufferList.ReleaseBuffer(sdata[i]);
				sdata[i] = NULL;
			}
		}
		throw;
	}
}

void desynSynthesizerSourceGroup::GenerateSoundSolo(const desynSynthesizerInstance &instance,
char *stateData, float *buffer, int samples, float curveOffset, float curveFactor){
	desynSharedBuffer ** const sdata = (desynSharedBuffer**)(stateData + GetStateDataOffset());
	desynSharedBufferList &sharedBufferList = GetModule().GetSharedBufferList();
	const int channelCount = instance.GetChannelCount();
	const int lastSource = pSourceCount - 1;
	float intpart;
	int i;
	
	try{
		if(channelCount == 1){
			sGenerateBufferMono * const dbuf = (sGenerateBufferMono*)buffer;
			
			for(i=0; i<samples; i++){
				const int curveEvalPos = NearestCurveEvalPosition(i, curveOffset, curveFactor);
				const float selectBlend = modff(GetSelect(instance, curveEvalPos), &intpart);
				const int selectSolo = decMath::clamp((selectBlend < 0.5f) ? (int)intpart : ((int)intpart + 1), 0, lastSource);
				
				if(!sdata[selectSolo]){
					sdata[selectSolo] = sharedBufferList.ClaimBuffer(samples);
					memset(sdata[selectSolo]->GetBuffer(), 0, sizeof(float) * samples);
					pSources[selectSolo]->GenerateSound(instance, stateData,
						sdata[selectSolo]->GetBuffer(), samples, curveOffset, curveFactor);
				}
				const sGenerateBufferMono &bufferSolo = *((const sGenerateBufferMono *)sdata[selectSolo]->GetBuffer() + i);
				
				dbuf[i].value = bufferSolo.value;
			}
			
		}else if(channelCount == 2){
			sGenerateBufferStereo * const dbuf = (sGenerateBufferStereo*)buffer;
			
			for(i=0; i<samples; i++){
				const int curveEvalPos = NearestCurveEvalPosition(i, curveOffset, curveFactor);
				const float selectBlend = modff(GetSelect(instance, curveEvalPos), &intpart);
				const int selectSolo = decMath::clamp((selectBlend < 0.5f) ? (int)intpart : ((int)intpart + 1), 0, lastSource);
				
				if(!sdata[selectSolo]){
					sdata[selectSolo] = sharedBufferList.ClaimBuffer(samples * 2);
					memset(sdata[selectSolo]->GetBuffer(), 0, sizeof(float) * samples * 2);
					pSources[selectSolo]->GenerateSound(instance, stateData,
						sdata[selectSolo]->GetBuffer(), samples, curveOffset, curveFactor);
				}
				const sGenerateBufferStereo &bufferSolo = *((const sGenerateBufferStereo*)sdata[selectSolo]->GetBuffer() + i);
				
				dbuf[i].left = bufferSolo.left;
				dbuf[i].right = bufferSolo.right;
			}
		}
		
		for(i=0; i<pSourceCount; i++){
			if(sdata[i]){
				sharedBufferList.ReleaseBuffer(sdata[i]);
				sdata[i] = NULL;
				
			}else{
				pSources[i]->SkipSound(instance, stateData, samples, curveOffset, curveFactor);
			}
		}
		
	}catch(const deException &){
		for(i=0; i<pSourceCount; i++){
			if(sdata[i]){
				sharedBufferList.ReleaseBuffer(sdata[i]);
				sdata[i] = NULL;
			}
		}
		throw;
	}
}

void desynSynthesizerSourceGroup::SkipSourceSound(const desynSynthesizerInstance &instance,
char *stateData, int samples, float curveOffset, float curveFactor){
	int i;
	for(i=0; i<pSourceCount; i++){
		pSources[i]->SkipSound(instance, stateData, samples, curveOffset, curveFactor);
	}
}



// Private Functions
//////////////////////

void desynSynthesizerSourceGroup::pCreateSources(desynSynthesizer &synthesizer,
int firstLink, const deSynthesizerSourceGroup &source){
	pClearSources();
	
	const int count = source.GetSourceCount();
	if(count == 0){
		SetSilent(true);
		return;
	}
	
	desynCreateSynthesizerSource createSource(synthesizer, firstLink);
	
	pSources = new desynSynthesizerSource*[count];
	
	for(pSourceCount=0; pSourceCount<count; pSourceCount++){
		createSource.Reset();
		
		try{
			source.GetSourceAt(pSourceCount)->Visit(createSource);
			pSources[pSourceCount] = createSource.GetSource();
			
		}catch(const deException &){
			if(createSource.GetSource()){
				delete createSource.GetSource();
			}
			throw;
		}
	}
}

void desynSynthesizerSourceGroup::pClearSources(){
	if(!pSources){
		return;
	}
	
	int i;
	for(i=0; i<pSourceCount; i++){
		delete pSources[i];
	}
	delete [] pSources;
	
	pSources = NULL;
	pSourceCount = 0;
}
