/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "deAudioAnalyzer.h"
#include "deAudioAnalyzerManager.h"
#include "../deResource.h"
#include "../deResourceManager.h"
#include "../../common/exceptions.h"
#include "../../common/math/decMath.h"
#include "../../systems/modules/synthesizer/deBaseSynthesizerAudioAnalyzer.h"


// Class deAudioAnalyzer
//////////////////////////

// Constructor, destructor
////////////////////////////

deAudioAnalyzer::deAudioAnalyzer(deAudioAnalyzerManager *manager) :
deResource(manager),
pUseAudioCapture(false),
pSpectralPeaksCount(3),
pFrequencyBandsCount(8),
pRMS(0.0f),
pPeak(0.0f),
pZeroCrossingRate(0.0f),
pSpectralCentroid(0.0f),
pSpectralFlatness(0.0f),
pSpectralFlux(0.0f),
pSpectralRolloff(0.0f),
pPitch(0.0f)
{
	pSpectralPeaks.SetCount(pSpectralPeaksCount, 0.0f);
	pFrequencyBands.SetCount(pFrequencyBandsCount, 0.0f);
}

deAudioAnalyzer::~deAudioAnalyzer() = default;


// Management
///////////////

void deAudioAnalyzer::SetUseAudioCapture(bool useAudioCapture){
	if(useAudioCapture == pUseAudioCapture){
		return;
	}
	
	pUseAudioCapture = useAudioCapture;
	
	if(pPeerSynthesizer){
		pPeerSynthesizer->ConfigurationChanged();
	}
}

void deAudioAnalyzer::SetSpectralPeaksCount(int count){
	DEASSERT_TRUE(count > 0)
	if(count == pSpectralPeaksCount){
		return;
	}
	
	pSpectralPeaksCount = count;
	pSpectralPeaks.SetCount(pSpectralPeaksCount, 0.0f);
	
	if(pPeerSynthesizer){
		pPeerSynthesizer->ConfigurationChanged();
	}
}

void deAudioAnalyzer::SetFrequencyBandsCount(int count){
	DEASSERT_TRUE(count > 0)
	
	pFrequencyBandsCount = count;
	pFrequencyBands.SetCount(pFrequencyBandsCount, 0.0f);
	
	if(pPeerSynthesizer){
		pPeerSynthesizer->ConfigurationChanged();
	}
}


void deAudioAnalyzer::UpdateResults(){
	if(pPeerSynthesizer){
		pPeerSynthesizer->UpdateResults();
	}
}


// Peer Setters
/////////////////

void deAudioAnalyzer::SetRMS(float rms){
	pRMS = rms;
}

void deAudioAnalyzer::SetPeak(float peak){
	pPeak = peak;
}

void deAudioAnalyzer::SetZeroCrossingRate(float zeroCrossingRate){
	pZeroCrossingRate = zeroCrossingRate;
}

void deAudioAnalyzer::SetSpectralCentroid(float spectralCentroid){
	pSpectralCentroid = spectralCentroid;
}

void deAudioAnalyzer::SetSpectralFlatness(float spectralFlatness){
	pSpectralFlatness = spectralFlatness;
}

void deAudioAnalyzer::SetSpectralFlux(float spectralFlux){
	pSpectralFlux = spectralFlux;
}

void deAudioAnalyzer::SetSpectralRolloff(float spectralRolloff){
	pSpectralRolloff = spectralRolloff;
}

void deAudioAnalyzer::SetPitch(float pitch){
	pPitch = pitch;
}


// System Peers
/////////////////

void deAudioAnalyzer::SetPeerSynthesizer(PeerSynthesizer &&peerSynthesizer){
	pPeerSynthesizer = std::move(peerSynthesizer);
}
