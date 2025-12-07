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

#include "deoalFilter.h"
#include "../audiothread/deoalAudioThread.h"
#include "../extensions/deoalExtensions.h"

#include <dragengine/common/exceptions.h>



// Class deoalFilter
//////////////////////

// Constructor, destructor
////////////////////////////

deoalFilter::deoalFilter(deoalAudioThread &audioThread) :
pAudioThread(audioThread),
pType(eftNull),
pGain(1.0f),
pGainLF(1.0f),
pGainHF(1.0f),
pFilter(0)
{
	if(!audioThread.GetExtensions().GetHasEFX()){
		DETHROW(deeInvalidParam);
	}
	
	OAL_CHECK(audioThread, palGenFilters(1, &pFilter));
}

deoalFilter::~deoalFilter(){
	if(pFilter){
		palDeleteFilters(1, &pFilter);
	}
}



// Management
///////////////

void deoalFilter::SetType(eFilterType type){
	if(type == pType){
		return;
	}
	
	pType = type;
	
	switch(type){
	case eftNull:
		OAL_CHECK(pAudioThread, palFilteri(pFilter, AL_FILTER_TYPE, AL_FILTER_NULL));
		break;
		
	case eftLowPass:
		OAL_CHECK(pAudioThread, palFilteri(pFilter, AL_FILTER_TYPE, AL_FILTER_LOWPASS));
		break;
		
	case eftHighPass:
		OAL_CHECK(pAudioThread, palFilteri(pFilter, AL_FILTER_TYPE, AL_FILTER_HIGHPASS));
		break;
		
	case eftBandPass:
		OAL_CHECK(pAudioThread, palFilteri(pFilter, AL_FILTER_TYPE, AL_FILTER_BANDPASS));
		break;
	}
}

void deoalFilter::SetGain(float gain){
	pGain = gain;
	
	switch(pType){
	case eftLowPass:
		OAL_CHECK(pAudioThread, palFilterf(pFilter, AL_LOWPASS_GAIN, gain));
		break;
		
	case eftHighPass:
		OAL_CHECK(pAudioThread, palFilterf(pFilter, AL_HIGHPASS_GAIN, gain));
		break;
		
	case eftBandPass:
		OAL_CHECK(pAudioThread, palFilterf(pFilter, AL_BANDPASS_GAIN, gain));
		break;
		
	default:
		break;
	}
}

void deoalFilter::SetGainLF(float gain){
	pGainLF = gain;
	
	switch(pType){
	case eftHighPass:
		OAL_CHECK(pAudioThread, palFilterf(pFilter, AL_HIGHPASS_GAINLF, gain));
		break;
		
	case eftBandPass:
		OAL_CHECK(pAudioThread, palFilterf(pFilter, AL_BANDPASS_GAINLF, gain));
		break;
		
	default:
		break;
	}
}

void deoalFilter::SetGainHF(float gain){
	pGainHF = gain;
	
	switch(pType){
	case eftLowPass:
		OAL_CHECK(pAudioThread, palFilterf(pFilter, AL_LOWPASS_GAINHF, gain));
		break;
		
	case eftBandPass:
		OAL_CHECK(pAudioThread, palFilterf(pFilter, AL_BANDPASS_GAINHF, gain));
		break;
		
	default:
		break;
	}
}

void deoalFilter::SetGains(float gain, float gainLF, float gainHF){
	pGain = gain;
	pGainLF = gainLF;
	pGainHF = gainHF;
	
	switch(pType){
	case eftLowPass:
		OAL_CHECK(pAudioThread, palFilterf(pFilter, AL_LOWPASS_GAIN, pGain));
		OAL_CHECK(pAudioThread, palFilterf(pFilter, AL_LOWPASS_GAINHF, pGainHF));
		break;
		
	case eftHighPass:
		OAL_CHECK(pAudioThread, palFilterf(pFilter, AL_HIGHPASS_GAIN, pGain));
		OAL_CHECK(pAudioThread, palFilterf(pFilter, AL_HIGHPASS_GAINLF, pGainLF));
		break;
		
	case eftBandPass:
		OAL_CHECK(pAudioThread, palFilterf(pFilter, AL_BANDPASS_GAIN, pGain));
		OAL_CHECK(pAudioThread, palFilterf(pFilter, AL_BANDPASS_GAINLF, pGainLF));
		OAL_CHECK(pAudioThread, palFilterf(pFilter, AL_BANDPASS_GAINHF, pGainHF));
		break;
		
	default:
		break;
	}
}
