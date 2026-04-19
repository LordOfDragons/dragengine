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

#include "desynAudioAnalyzer.h"
#include "../deDESynthesizer.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/sound/deAudioAnalyzer.h>
#include <dragengine/systems/deAudioSystem.h>
#include <dragengine/threading/deMutexGuard.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>


// Class desynAudioAnalyzer::CaptureAudio
///////////////////////////////////////////

desynAudioAnalyzer::CaptureAudio::CaptureAudio(desynAudioAnalyzer &owner) :
pOwner(owner){
}

desynAudioAnalyzer::CaptureAudio::~CaptureAudio() = default;

void desynAudioAnalyzer::CaptureAudio::StateChanged(){
	pOwner.OnStateChanged();
}

void desynAudioAnalyzer::CaptureAudio::SamplesCaptured(const void *samples, int count){
	pOwner.OnSamplesCaptured(samples, count);
}


// Class desynAudioAnalyzer::AnalyzeThread
////////////////////////////////////////////

desynAudioAnalyzer::AnalyzeThread::AnalyzeThread(desynAudioAnalyzer &owner) :
pOwner(owner){
}

desynAudioAnalyzer::AnalyzeThread::~AnalyzeThread() = default;

void desynAudioAnalyzer::AnalyzeThread::Run(){
	try{
		pOwner.AnalyzeLoop();
		
	}catch(const deException &e){
		pOwner.GetModule().LogException(e);
	}
}


// Class desynAudioAnalyzer
/////////////////////////////

// Constructor, destructor
////////////////////////////

desynAudioAnalyzer::desynAudioAnalyzer(deDESynthesizer &module, deAudioAnalyzer &analyzer) :
pModule(module),
pAnalyzer(analyzer),
pListener(deTObjectReference<CaptureAudio>::New(*this)),
pThread(*this),
pStopThread(false),
pThreadRunning(false),
pCaptureSampleRate(48000), // will be updated from capture format
pMaxInputSamples(pCaptureSampleRate / 2), // 250ms
pDownsamplePhase(0.0),
pFftSize(analyzer.GetResolution()),
pHalfFftSize(analyzer.GetResolution() / 2),
pFftIn(nullptr),
pFftOut(nullptr),
pFftPlan(nullptr),
pResFrameCount(0),
pResSampleCount(0),
pResZeroCrossings(0),
pResPeak(0.0f),
pResPeakMag(0.0f),
pResSumSq(0.0),
pResCentroid(0.0),
pResFlatness(0.0),
pResFlux(0.0),
pResRolloff(0.0),
pResPitch(0.0)
{
	pConfig.sampleRate = pAnalyzer.GetSampleRate();
	pConfig.resolution = pAnalyzer.GetResolution();
	pConfig.lowestFrequency = pAnalyzer.GetLowestFrequency();
	pConfig.highestFrequency = pAnalyzer.GetHighestFrequency();
	pConfig.enablePreEmphasis = pAnalyzer.GetEnablePreEmphasis();
	pConfig.preEmphasisFactor = pAnalyzer.GetPreEmphasisFactor();
	pConfig.enableMelFiltering = pAnalyzer.GetEnableMelFiltering();
	pConfig.melFilterCount = pAnalyzer.GetMelFilterCount();
	pConfig.normalizeMelEnergies = pAnalyzer.GetNormalizeMelEnergies();
	pWorkConfig = pConfig;
	
	pRebuildFftPlan(pFftSize);
	
	const int bandCount = analyzer.GetFrequencyBandCount(); // always > 0
	pResBands.SetCount(bandCount, {});
	pWorkBands.SetCount(bandCount, {});
	
	auto &audSys = *module.GetGameEngine()->GetAudioSystem();
	audSys.GetAudioCaptureFormat(pInputFormat);
	audSys.AddAudioCaptureListener(pListener);
	OnStateChanged();
}

desynAudioAnalyzer::~desynAudioAnalyzer(){
	pModule.GetGameEngine()->GetAudioSystem()->RemoveAudioCaptureListener(pListener);
	
	pStopThread = true;
	pSemaphore.Signal();
	pThread.WaitForExit();
	
	if(pFftPlan){
		fftw_destroy_plan(pFftPlan);
	}
	if(pFftOut){
		fftw_free(pFftOut);
	}
	if(pFftIn){
		fftw_free(pFftIn);
	}
	fftw_cleanup();
}


// Notifications
//////////////////

void desynAudioAnalyzer::ConfigurationChanged(){
}

void desynAudioAnalyzer::UpdateResults(){
	const deMutexGuard guard(pMutexResults);
	
	if(pResFrameCount > 0){
		// time-domain: weighted by sample count so short and long batches contribute adequately
		pAnalyzer.SetRMS(sqrtf((float)(pResSumSq / (double)pResSampleCount)));
		pAnalyzer.SetPeak(pResPeak);
		pAnalyzer.SetZeroCrossingRate(pResSampleCount > 1
			? (float)pResZeroCrossings / (float)(pResSampleCount - 1) : 0.0f);
		
		// spectral: averaged over all frames
		const float invFrames = 1.0f / (float)pResFrameCount;
		pAnalyzer.SetSpectralCentroid((float)(pResCentroid * (double)invFrames));
		pAnalyzer.SetSpectralFlatness((float)(pResFlatness * (double)invFrames));
		pAnalyzer.SetSpectralFlux((float)(pResFlux * (double)invFrames));
		pAnalyzer.SetSpectralRolloff((float)(pResRolloff * (double)invFrames));
		pAnalyzer.SetPitch((float)(pResPitch * (double)invFrames));
		
		pResSpectralPeaks.Sort([](const deAudioAnalyzer::SpectralPeak &a, const deAudioAnalyzer::SpectralPeak &b){
			return DECompare(b.magnitude, a.magnitude);
		});
		auto &peaks = pAnalyzer.GetSpectralPeaks();
		peaks.SetCountDiscard(0);
		pResSpectralPeaks.Visit([&](const deAudioAnalyzer::SpectralPeak &peak){
			peaks.Add(peak);
		});
		
		const int bandCount = pResBands.GetCount();
		pAnalyzer.GetFrequencyBands().VisitIndexed([&](int index, deAudioAnalyzer::FrequencyBand &band){
			band = index < bandCount ? pResBands[index] : deAudioAnalyzer::FrequencyBand{};
		});
		
		/*
		const float maxBandEnergy = pResBands.Inject(0.0f, [](float acc, float energy){
			return decMath::max(acc, energy);
		});
		
		const float normEnergy = maxBandEnergy > 0.0f ? 1.0f / maxBandEnergy : 0.0f;
		const int bandCount = pResBands.GetCount();
		pAnalyzer.GetFrequencyBands().VisitIndexed([&](int index, float &value){
			value = index < bandCount ? pResBands[index] * normEnergy : 0.0f;
		});
		*/
		
		// reset accumulators
		pResFrameCount = 0;
		pResSampleCount = 0;
		pResSumSq = 0.0;
		pResPeak = 0.0f;
		pResZeroCrossings = 0;
		pResCentroid = 0.0;
		pResFlatness = 0.0;
		pResFlux = 0.0;
		pResRolloff = 0.0;
		pResPitch = 0.0;
		pResPeakMag = 0.0f;
		pResSpectralPeaks.SetCountDiscard(0);
		pResBands.SetRangeAt(0, pResBands.GetCount(), {});
	}
	
	// update configuration if configuration
	pConfig.sampleRate = pAnalyzer.GetSampleRate();
	pConfig.resolution = pAnalyzer.GetResolution();
	pConfig.lowestFrequency = pAnalyzer.GetLowestFrequency();
	pConfig.highestFrequency = pAnalyzer.GetHighestFrequency();
	pConfig.enablePreEmphasis = pAnalyzer.GetEnablePreEmphasis();
	pConfig.preEmphasisFactor = pAnalyzer.GetPreEmphasisFactor();
	pConfig.enableMelFiltering = pAnalyzer.GetEnableMelFiltering();
	pConfig.melFilterCount = pAnalyzer.GetMelFilterCount();
	pConfig.normalizeMelEnergies = pAnalyzer.GetNormalizeMelEnergies();
	
	pResBands.SetCount(pAnalyzer.GetFrequencyBands().GetCount(), {});
}


void desynAudioAnalyzer::OnSamplesCaptured(const void *samples, int count){
	if(!pThreadRunning){
		return;
	}
	
	const deMutexGuard guard(pMutexInput);
	
	const int dropSampleCount = pInputSamples.GetCount() + count - pMaxInputSamples;
	if(dropSampleCount > 0){
		if(dropSampleCount >= pInputSamples.GetCount()){
			pInputSamples.SetCountDiscard(0);
			
		}else{
			pInputSamples.RemoveHead(dropSampleCount);
		}
	}
	
	switch(pInputFormat.bitRate){
	case 16:{
		const int16_t * const s = reinterpret_cast<const int16_t*>(samples);
		for(int i=0; i<count; i++){
			pInputSamples.Add(s[i]);
		}
		}break;
		
	case 8:{
		const int8_t * const s = reinterpret_cast<const int8_t*>(samples);
		for(int i=0; i<count; i++){
			pInputSamples.Add((uint16_t)s[i] << 8);
		}
		}break;
	}
	
	pSemaphore.Signal();
}

void desynAudioAnalyzer::OnStateChanged(){
	const auto &audSys = *pModule.GetGameEngine()->GetAudioSystem();
	
	if(audSys.IsCapturingAudio()){
		if(pThreadRunning){
			return;
		}
		
		deAudioSystem::AudioCaptureFormat fmt;
		audSys.GetAudioCaptureFormat(fmt);
		pCaptureSampleRate = fmt.sampleRate;
		pMaxInputSamples = pCaptureSampleRate / 4; // 250ms
		pDownsamplePhase = 0.0;
		
		pStopThread = false;
		pThread.Start();
		pThreadRunning = true;
		pModule.LogInfoFormat(
			"AudioAnalyzer.OnStateChanged(): Start analyzing audio capture (rate=%dHz bitrate=%d)",
			fmt.sampleRate, fmt.bitRate);
		
	}else if(pThreadRunning){
		pThreadRunning = false;
		pStopThread = true;
		pSemaphore.Signal();
		pThread.WaitForExit();
		pModule.LogInfo("AudioAnalyzer.OnStateChanged(): Stop analyzing audio capture");
	}
}

void desynAudioAnalyzer::AnalyzeLoop(){
	pModule.LogInfo("AudioAnalyzer.AnalyzeLoop(): Enter");
	decTList<int16_t> localSamples;
	
	while(true){
		pSemaphore.Wait();
		
		{
		const deMutexGuard guard(pMutexInput);
		if(pStopThread){
			break;
		}
		
		localSamples.Swap(pInputSamples);
		pInputSamples.SetCountDiscard(0);
		}
		
		if(localSamples.IsNotEmpty()){
			pProcessFrame(localSamples);
			localSamples.SetCountDiscard(0);
		}
	}
	pModule.LogInfo("AudioAnalyzer.AnalyzeLoop(): Exit");
}



// Private helpers
///////////////////

void desynAudioAnalyzer::pRebuildFftPlan(int newSize){
	if(pFftPlan){
		fftw_destroy_plan(pFftPlan);
		pFftPlan = nullptr;
	}
	if(pFftOut){
		fftw_free(pFftOut);
		pFftOut = nullptr;
	}
	if(pFftIn){
		fftw_free(pFftIn);
		pFftIn = nullptr;
	}
	
	pFftSize = newSize;
	pHalfFftSize = newSize / 2;
	
	pFftIn = fftw_alloc_real(pFftSize);
	pFftOut = fftw_alloc_complex(pFftSize / 2 + 1);
	pFftPlan = fftw_plan_dft_r2c_1d(pFftSize, pFftIn, pFftOut, FFTW_MEASURE);
	
	pPrevMagnitudes.SetCount(pHalfFftSize, 0.0f);
	pMagnitudes.SetCount(pHalfFftSize, 0.0f);
	pEnergies.SetCount(pHalfFftSize, 0.0f);
	pSampleBuffer.SetCountDiscard(0);
	pSampleBuffer.EnlargeCapacity(pFftSize);
}

void desynAudioAnalyzer::pProcessFrame(const decTList<int16_t> &samples){
	// rebuild FFT plan if resolution changed
	const int resolution = pWorkConfig.resolution;
	if(resolution != pFftSize){
		pRebuildFftPlan(resolution);
	}
	
	const int sampleCount = samples.GetCount();
	if(sampleCount == 0){
		return;
	}
	
	const float norm = 1.0f / 32768.0f;
	
	// append new samples to the buffer at the target sample rate
	const int sampleRate = pWorkConfig.sampleRate;
	if(pCaptureSampleRate == sampleRate){
		samples.Visit([&](int16_t sample){
			pSampleBuffer.Add((float)sample * norm);
		});
		
	}else{
		// downsample to target sample rate using linear interpolation
		const double step = (double)pCaptureSampleRate / (double)sampleRate;
		while(pDownsamplePhase < (double)sampleCount){
			const int i = (int)pDownsamplePhase;
			const int j = decMath::min(i + 1, sampleCount - 1);
			const double frac = pDownsamplePhase - (double)i;
			const float s = (float)((double)samples[i] * (1.0 - frac) + (double)samples[j] * frac) * norm;
			pSampleBuffer.Add(s);
			pDownsamplePhase += step;
		}
		pDownsamplePhase -= (double)sampleCount;
	}
	
	// sliding window. process as long as there are enough samples in the buffer
	const int hopSize = pFftSize / 2;
	while(pSampleBuffer.GetCount() >= pFftSize){
		pAnalyzeWindow(hopSize);
	}
}

void desynAudioAnalyzer::pAnalyzeWindow(int hopSize){
	// time-domain metrics over the non-overlapping hop samples at the end of the window.
	// only the last hopSize samples are new. earlier samples were already covered in
	// prior calls to pAnalyzeWindow
	double sumSq = 0.0;
	float peak = 0.0f;
	int zeroCrossings = 0;
	int prevSign = pSampleBuffer[pFftSize - hopSize - 1] >= 0.0f ? 1 : -1;
	
	pSampleBuffer.Visit(pFftSize - hopSize, pFftSize, [&](float s){
		sumSq += (double)s * (double)s;
		peak = decMath::max(peak, fabsf(s));
		
		const int curSign = s >= 0.0f ? 1 : -1;
		if(curSign != prevSign){
			zeroCrossings++;
		}
		prevSign = curSign;
	});
	
	const float freqPerBin = (float)pWorkConfig.sampleRate / (float)pFftSize;
	
	// fill FFT input from the full window with Hann window and optional pre-emphasis
	const bool enablePreEmphasis = pWorkConfig.enablePreEmphasis;
	const float preEmphasisFactor = pWorkConfig.preEmphasisFactor;
	const float * const sampleBuffer = pSampleBuffer.GetArrayPointer();
	
	for(int i=0; i<pFftSize; i++){
		const float w = 0.5f * (1.0f - cosf(TWO_PI * (float)i / (float)(pFftSize - 1)));
		float s = sampleBuffer[i];
		if(enablePreEmphasis && i > 0){
			s -= preEmphasisFactor * sampleBuffer[i - 1];
		}
		pFftIn[i] = (double)(s * w);
	}
	
	// FFT
	fftw_execute(pFftPlan);
	
	// first spectral loop: magnitudes, energy, centroid, flatness, flux.
	// covers all quantities that can be accumulated in a single forward pass
	float * const prevMagnitudes = pPrevMagnitudes.GetArrayPointer();
	float * const magnitudes = pMagnitudes.GetArrayPointer();
	float * const energies = pEnergies.GetArrayPointer();
	const double fftScaleMagnitude = 1.0 / (double)pFftSize;
	const double fftScaleEnergy = 1.0 / (double)(pFftSize * pFftSize);
	float totalMagnitude = 0.0f;
	//float totalEnergy = 0.0f;
	float centroidSum = 0.0f;
	double logSum = 0.0;
	int logCount = 0;
	float maxMag = 0.0f;
	float fluxSum = 0.0f;
	
	for(int k=0; k<pHalfFftSize; k++){
		const double re = pFftOut[k][0];
		const double im = pFftOut[k][1];
		const double magSq = re * re + im * im;
		magnitudes[k] = (float)(sqrt(magSq) * fftScaleMagnitude);
		energies[k] = (float)(magSq * fftScaleEnergy);
		
		totalMagnitude += magnitudes[k];
		//totalEnergy += energies[k];
		
		centroidSum += (float)k * freqPerBin * magnitudes[k];
		
		if(magnitudes[k] > 0.0f){
			logSum += log((double)magnitudes[k]);
			logCount++;
		}
		
		maxMag = decMath::max(maxMag, magnitudes[k]);
		fluxSum += decMath::max(0.0f, magnitudes[k] - prevMagnitudes[k]);
	}
	memcpy(prevMagnitudes, magnitudes, sizeof(float) * pHalfFftSize);
	
	// derived scalars from the first pass
	const float centroid = totalMagnitude > 0.0f ? centroidSum / totalMagnitude : 0.0f;
	
	float flatness = 0.0f;
	if(logCount > 0 && totalMagnitude > 0.0f){
		const float divisor = totalMagnitude / (float)pHalfFftSize;
		if(divisor > 0.0f){
			const float geom = (float)exp(logSum / (double)logCount);
			flatness = decMath::clamp(geom / divisor, 0.0f, 1.0f);
		}
	}
	
	const float flux = maxMag > 0.0f ? fluxSum / ((float)pHalfFftSize * maxMag) : 0.0f;
	
	// second spectral loop: rolloff, peaks, frequency bands, pitch.
	// these quantities require the complete magnitude array accumulated in the first pass
	const float rolloffThreshold = 0.85f * totalMagnitude;
	float rolloff = 0.0f;
	float cumulative = 0.0f;
	bool rolloffFound = false;
	
	pWorkPeaks.SetCountDiscard(0);
	
	const int bandCount = pWorkBands.GetCount();
	pWorkBands.SetRangeAt(0, bandCount, {});
	
	const bool enableMelFiltering = pWorkConfig.enableMelFiltering;
	
	if(!enableMelFiltering){
		pWorkBands.Visit([&](Band &band){
			band.lowestFrequency = pWorkConfig.highestFrequency;
			band.highestFrequency = pWorkConfig.lowestFrequency;
		});
	}
	
	// logarithmic band mapping parameters (used only when mel filtering is disabled)
	const int freqLoBin = decMath::max(1, (int)(pWorkConfig.lowestFrequency / freqPerBin));
	const int freqHiBin = decMath::min(pHalfFftSize - 1, (int)(pWorkConfig.highestFrequency / freqPerBin));
	const float logBinRange = freqHiBin > freqLoBin ? logf((float)freqHiBin / (float)freqLoBin) : 1.0f;
	const float invLogBinRange = (float)bandCount / logBinRange;
	
	const int lo = decMath::max(1, (int)(80.0f / freqPerBin));
	const int hi = decMath::min(pHalfFftSize - 1, (int)(1000.0f / freqPerBin));
	float bestMag = 0.0f;
	int bestBin = 0;
	
	for(int k=0; k<pHalfFftSize; k++){
		// spectral rolloff (85 % energy threshold)
		if(!rolloffFound){
			cumulative += magnitudes[k];
			if(cumulative >= rolloffThreshold){
				rolloff = (float)k * freqPerBin;
				rolloffFound = true;
			}
		}
		
		// spectral peaks (local maxima)
		if(k >= 1 && k < pHalfFftSize - 1){
			if(magnitudes[k] > magnitudes[k - 1] && magnitudes[k] > magnitudes[k + 1]){
				pWorkPeaks.Add({
					.frequency = (float)k * freqPerBin,
					.magnitude = magnitudes[k]
				});
			}
		}
		
		// frequency bands: logarithmic spacing within capped frequency range
		// skipped when mel filtering is enabled
		if(!enableMelFiltering && k >= freqLoBin && k <= freqHiBin){
			const float logRel = logf((float)k / (float)freqLoBin);
			
			auto &band = pWorkBands[decMath::min((int)(logRel * invLogBinRange), bandCount - 1)];
			band.magnitude += magnitudes[k];
			band.energy += energies[k];
			band.count++;
			
			const float binFreq = (float)k * freqPerBin;
			band.lowestFrequency = decMath::min(band.lowestFrequency, binFreq);
			band.highestFrequency = decMath::max(band.highestFrequency, binFreq);
		}
		
		// pitch (highest magnitude bin in the 80-1000 Hz range)
		if(k >= lo && k <= hi && magnitudes[k] > bestMag){
			bestMag = magnitudes[k];
			bestBin = k;
		}
	}
	
	// mel filter bank: triangular filters equally spaced in mel scale.
	// energies are mapped into the output bands
	if(enableMelFiltering && bandCount > 0){
		const int melCount = pWorkConfig.melFilterCount;
		const float loFreq = pWorkConfig.lowestFrequency;
		const float hiFreq = pWorkConfig.highestFrequency;
		const float lowMel = 1125.0f * logf(1.0f + loFreq / 700.0f);
		const float highMel = 1125.0f * logf(1.0f + hiFreq / 700.0f);
		const float melRange = highMel - lowMel;
		const float melFactor = melRange / (float)(melCount + 1);
		
		// grow mel energy buffer if needed
		pMelEnergies.EnlargeCapacity(melCount);
		float * const melEnergies = pMelEnergies.GetArrayPointer();
		
		// compute mel filter center/boundary bin indices and filter energies
		for(int m=0; m<melCount; m++){
			// compute three consecutive mel-scale boundary points for this filter
			const float melLeft = lowMel + melFactor * (float)(m);
			const float melCenter = lowMel + melFactor * (float)(m + 1);
			const float melRight = lowMel + melFactor * (float)(m + 2);
			
			const float hzLeft = 700.0f * (expf(melLeft / 1125.0f) - 1.0f);
			const float hzCenter = 700.0f * (expf(melCenter / 1125.0f) - 1.0f);
			const float hzRight = 700.0f * (expf(melRight / 1125.0f) - 1.0f);
			
			const int binLeft = decMath::max(0, (int)(hzLeft / freqPerBin));
			const int binCenter = decMath::min(pHalfFftSize - 1, (int)(hzCenter / freqPerBin));
			const int binRight = decMath::min(pHalfFftSize - 1, (int)(hzRight / freqPerBin));
			
			float energy = 0.0f;
			float weightSum = 0.0f;
			
			for(int k=binLeft; k<=binRight; k++){
				float w;
				
				if(k <= binCenter){
					if(binCenter > binLeft){
						w = (float)(k - binLeft) / (float)(binCenter - binLeft);
						
					}else{
						w = 1.0f;
					}
					
				}else{
					if(binRight > binCenter){
						w = (float)(binRight - k) / (float)(binRight - binCenter);
						
					}else{
						w = 0.0f;
					}
				}
				
				energy += energies[k] * w;
				weightSum += w;
			}
			
			melEnergies[m] = energy;
			if(pWorkConfig.normalizeMelEnergies && weightSum > 0.0f){
				melEnergies[m] /= weightSum;
			}
		}
		
		// map mel filter energies into output bands
		const float melBandFactor = (float)melCount / (float)bandCount;
		
		for(int b=0; b<bandCount; b++){
			const int melFrom = (int)(melBandFactor * (float)b);
			const int melTo = (int)(melBandFactor * (float)(b + 1));
			
			float energy = 0.0f;
			int count = 0;
			for(int m=melFrom; m<melTo && m<melCount; m++){
				energy += melEnergies[m];
				count++;
			}
			
			// derive frequency range from the mel filters that map to this band
			const float melBoundLeft = lowMel + melRange * (float)melFrom / (float)(melCount + 1);
			const float melBoundRight = lowMel + melRange
				* (float)(decMath::min(melTo, melCount - 1) + 2) / (float)(melCount + 1);
			
			auto &band = pWorkBands[b];
			band.count = count;
			band.lowestFrequency = 700.0f * (expf(melBoundLeft / 1125.0f) - 1.0f);
			band.highestFrequency = 700.0f * (expf(melBoundRight / 1125.0f) - 1.0f);
			
			if(count > 0){
				if(pWorkConfig.normalizeMelEnergies){
					energy /= (float)count;
				}
				band.energy = energy;
				
			}else{
				band.energy = 0.0f;
			}
		}
		
		// apply DCT to log mel energies producing one coefficient per band.
		const float mfccLogEpsilon = 1e-10f;
		const float mfccFactor = (float)M_PI / (float)melCount;
		for(int b=0; b<bandCount; b++){
			float dct = 0.0f;
			for(int m=0; m<melCount; m++){
				dct += logf(decMath::max(melEnergies[m], mfccLogEpsilon))
					* cosf(mfccFactor * (float)b * ((float)m - 0.5f));
			}
			pWorkBands[b].mfcc = dct;
		}
	}
	
	const float pitchHz = bestMag > 0.0f ? (float)bestBin * freqPerBin : 0.0f;
	
	// advance the sliding window. this can be done before accumulating results since
	pSampleBuffer.RemoveHead(hopSize);
	
	// accumulate results under mutex
	const deMutexGuard guard(pMutexResults);
	
	pResFrameCount++;
	pResSampleCount += hopSize;
	pResSumSq += sumSq;
	pResPeak = decMath::max(pResPeak, peak);
	pResZeroCrossings += zeroCrossings;
	pResCentroid += centroid;
	pResFlatness += flatness;
	pResFlux += flux;
	pResRolloff += rolloff;
	pResPitch += pitchHz;
	
	// frequency band accumulation. skipped if count of bands changed mid flight
	const int resBandCount = pResBands.GetCount();
	if(resBandCount == pWorkBands.GetCount()){
		pWorkBands.VisitIndexed([&](int index, const Band &band){
			// average band magnitude and energy by bin count so all bands are comparable regardless of width
			float magnitude = band.magnitude;
			float energy = band.energy;
			
			if(band.count > 0){
				magnitude /= (float)band.count;
				if(!enableMelFiltering){
					energy /= (float)band.count;
				}
			}
			
			auto &resBand = pResBands[index];
			resBand.magnitude += magnitude;
			resBand.energy += energy;
			resBand.lowestFrequency = band.lowestFrequency;
			resBand.highestFrequency = band.highestFrequency;
			resBand.mfcc += band.mfcc;
		});
		
	}else{
		pWorkBands.SetCountDiscard(resBandCount);
	}
	
	// keep spectral peaks of frame with the strongest single peak
	if(pWorkPeaks.IsNotEmpty()){
		const float peakMag = pWorkPeaks.Inject(0.0f, [](float acc, const deAudioAnalyzer::SpectralPeak &sp){
			return decMath::max(acc, sp.magnitude);
		});
		
		if(peakMag > pResPeakMag){
			pResSpectralPeaks.Swap(pWorkPeaks);
			pResPeakMag = peakMag;
		}
	}
	
	// copy config. this is not required to be done in all hops but here we have the
	// mutex already locked so why not
	pWorkConfig = pConfig;
}
