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
pSampleRate(16000),
pResolution(1024),
pFrequencyBandCount(8),
pLowestFrequency(20.0f),
pHighestFrequency(20000.0f),
pEnablePreEmphasis(true),
pPreEmphasisFactor(0.97f),
pEnableMelFiltering(false),
pMelFilterCount(40),
pNormalizeMelEnergies(true),
pRMS(0.0f),
pPeak(0.0f),
pZeroCrossingRate(0.0f),
pSpectralCentroid(0.0f),
pSpectralFlatness(0.0f),
pSpectralFlux(0.0f),
pSpectralRolloff(0.0f),
pPitch(0.0f)
{
	pFrequencyBands.SetCount(pFrequencyBandCount, {});
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

void deAudioAnalyzer::SetSampleRate(int sampleRate){
	DEASSERT_TRUE(sampleRate > 0)
	
	if(sampleRate == pSampleRate){
		return;
	}
	
	pSampleRate = sampleRate;
	
	if(pPeerSynthesizer){
		pPeerSynthesizer->ConfigurationChanged();
	}
}

void deAudioAnalyzer::SetResolution(int resolution){
	DEASSERT_TRUE(resolution >= 64)
	
	if(resolution == pResolution){
		return;
	}
	
	pResolution = resolution;
	
	if(pPeerSynthesizer){
		pPeerSynthesizer->ConfigurationChanged();
	}
}

void deAudioAnalyzer::SetFrequencyBandCount(int count){
	DEASSERT_TRUE(count > 0)
	
	if(count == pFrequencyBandCount){
		return;
	}
	
	pFrequencyBandCount = count;
	pFrequencyBands.SetCount(pFrequencyBandCount, {});
	
	if(pPeerSynthesizer){
		pPeerSynthesizer->ConfigurationChanged();
	}
}

void deAudioAnalyzer::SetFrequencyRange(float lowestFrequency, float highestFrequency){
	pLowestFrequency = decMath::max(lowestFrequency, 1.0f);
	pHighestFrequency = decMath::max(highestFrequency, pLowestFrequency + 1.0f);
	
	if(pPeerSynthesizer){
		pPeerSynthesizer->ConfigurationChanged();
	}
}

void deAudioAnalyzer::SetEnablePreEmphasis(bool enable){
	if(enable == pEnablePreEmphasis){
		return;
	}
	
	pEnablePreEmphasis = enable;
	
	if(pPeerSynthesizer){
		pPeerSynthesizer->ConfigurationChanged();
	}
}

void deAudioAnalyzer::SetPreEmphasisFactor(float factor){
	factor = decMath::clamp(factor, 0.0f, 1.0f);
	if(fabsf(factor - pPreEmphasisFactor) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pPreEmphasisFactor = factor;
	
	if(pPeerSynthesizer){
		pPeerSynthesizer->ConfigurationChanged();
	}
}

void deAudioAnalyzer::SetEnableMelFiltering(bool enable){
	if(enable == pEnableMelFiltering){
		return;
	}
	
	pEnableMelFiltering = enable;
	
	if(pPeerSynthesizer){
		pPeerSynthesizer->ConfigurationChanged();
	}
}

void deAudioAnalyzer::SetMelFilterCount(int count){
	DEASSERT_TRUE(count > 0)
	
	if(count == pMelFilterCount){
		return;
	}
	
	pMelFilterCount = count;
	
	if(pPeerSynthesizer){
		pPeerSynthesizer->ConfigurationChanged();
	}
}

void deAudioAnalyzer::SetNormalizeMelEnergies(bool normalize){
	if(normalize == pNormalizeMelEnergies){
		return;
	}
	
	pNormalizeMelEnergies = normalize;
	
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
