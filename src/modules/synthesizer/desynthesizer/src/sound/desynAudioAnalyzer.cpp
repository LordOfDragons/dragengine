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
	pOwner.AnalyzeLoop();
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
pSampleRate(44100),
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
	pFftIn = fftw_alloc_real(FFT_SIZE);
	pFftOut = fftw_alloc_complex(FFT_SIZE / 2 + 1);
	pFftPlan = fftw_plan_dft_r2c_1d(FFT_SIZE, pFftIn, pFftOut, FFTW_MEASURE);
	memset(pPrevMagnitude, 0, sizeof(pPrevMagnitude));
	pSampleBuffer.EnlargeCapacity(FFT_SIZE);
	
	const int bandCount = analyzer.GetFrequencyBandCount(); // always > 0
	pResBands.SetCount(bandCount, 0.0f);
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
		
		const float maxBandEnergy = pResBands.Inject(0.0f, [](float acc, float energy){
			return decMath::max(acc, energy);
		});
		
		const float normEnergy = maxBandEnergy > 0.0f ? 1.0f / maxBandEnergy : 0.0f;
		const int bandCount = pResBands.GetCount();
		pAnalyzer.GetFrequencyBands().VisitIndexed([&](int index, float &value){
			value = index < bandCount ? pResBands[index] * normEnergy : 0.0f;
		});
		
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
		pResBands.SetRangeAt(0, pResBands.GetCount(), 0.0f);
	}
	
	// update array size if configuration changed. does nothing if size is unchanged
	pResBands.SetCount(pAnalyzer.GetFrequencyBands().GetCount(), 0.0f);
}


void desynAudioAnalyzer::OnSamplesCaptured(const void *samples, int count){
	if(!pThreadRunning){
		return;
	}
	
	const deMutexGuard guard(pMutexInput);
	
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
		pSampleRate = fmt.sampleRate;
		
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

void desynAudioAnalyzer::pProcessFrame(const decTList<int16_t> &samples){
	const int sampleCount = samples.GetCount();
	if(sampleCount == 0){
		return;
	}
	
	const float norm = 1.0f / 32768.0f;
	
	// push new samples into the ring buffer, dropping oldest if full
	const int dropSampleCount = pSampleBuffer.GetCount() + sampleCount - FFT_SIZE;
	if(dropSampleCount > 0){
		pSampleBuffer.RemoveHead(dropSampleCount);
	}
	
	samples.Visit([&](int16_t sample){
		pSampleBuffer.Add((float)sample * norm);
	});
	
	// do not process until the ring buffer is fully populated
	if(pSampleBuffer.GetCount() < FFT_SIZE){
		return;
	}
	
	// compute time-domain metrics over the entire ring buffer (consistent with FFT window)
	double sumSq = 0.0;
	float peak = 0.0f;
	int zeroCrossings = 0;
	int prevSign = 0;
	
	pSampleBuffer.VisitIndexed([&](int i, float s){
		sumSq += (double)s * (double)s;
		peak = decMath::max(peak, fabsf(s));
		
		const int curSign = s >= 0.0f ? 1 : -1;
		if(i > 0 && curSign != prevSign){
			zeroCrossings++;
		}
		prevSign = curSign;
	});
	
	const float freqPerBin = (float)pSampleRate / (float)FFT_SIZE;
	
	// fill FFT input from ring buffer (oldest to newest) with Hann window
	for(int i=0; i<FFT_SIZE; i++){
		const float w = 0.5f * (1.0f - cosf(TWO_PI * (float)i / (float)(FFT_SIZE - 1)));
		pFftIn[i] = (double)(pSampleBuffer.GetAt(i) * w);
	}
	
	// FFT
	fftw_execute(pFftPlan);
	
	// first spectral loop: magnitudes, energy, centroid, flatness, flux.
	// covers all quantities that can be accumulated in a single forward pass
	float magnitude[HALF_FFT_SIZE];
	float totalEnergy = 0.0f;
	float weightedSum = 0.0f;
	double logSum = 0.0;
	int logCount = 0;
	float maxMag = 0.0f;
	float fluxSum = 0.0f;
	
	for(int k=0; k<HALF_FFT_SIZE; k++){
		const double re = pFftOut[k][0];
		const double im = pFftOut[k][1];
		magnitude[k] = (float)sqrt(re * re + im * im);
		
		totalEnergy += magnitude[k];
		
		weightedSum += (float)k * freqPerBin * magnitude[k];
		
		if(magnitude[k] > 0.0f){
			logSum += log((double)magnitude[k]);
			logCount++;
		}
		
		maxMag = decMath::max(maxMag, magnitude[k]);
		
		fluxSum += decMath::max(0.0f, magnitude[k] - pPrevMagnitude[k]);
	}
	memcpy(pPrevMagnitude, magnitude, sizeof(float) * HALF_FFT_SIZE);
	
	// derived scalars from the first pass
	const float centroid = totalEnergy > 0.0f ? weightedSum / totalEnergy : 0.0f;
	
	float flatness = 0.0f;
	if(logCount > 0 && totalEnergy > 0.0f){
		const float divisor = totalEnergy / (float)HALF_FFT_SIZE;
		if(divisor > 0.0f){
			const float geom = (float)exp(logSum / (double)logCount);
			flatness = decMath::clamp(geom / divisor, 0.0f, 1.0f);
		}
	}
	
	const float flux = maxMag > 0.0f ? fluxSum / ((float)HALF_FFT_SIZE * maxMag) : 0.0f;
	
	// second spectral loop: rolloff, peaks, frequency bands, pitch.
	// these quantities require the complete magnitude array accumulated in the first pass
	const float rolloffThreshold = 0.85f * totalEnergy;
	float rolloff = 0.0f;
	float cumulative = 0.0f;
	bool rolloffFound = false;
	
	pWorkPeaks.SetCountDiscard(0);
	
	const int bandCount = pWorkBands.GetCount();
	pWorkBands.SetRangeAt(0, bandCount, {});
	
	// logarithmic band mapping: bin k maps to band floor(log(k)/log(N-1)*bandCount).
	// bin 0 is DC and has no meaningful frequency, so it is skipped.
	const float logMaxBin = logf((float)(HALF_FFT_SIZE - 1));
	const float invLogBandScale = logMaxBin > 0.0f ? (float)bandCount / logMaxBin : 0.0f;
	
	const int lo = decMath::max(1, (int)(80.0f / freqPerBin));
	const int hi = decMath::min(HALF_FFT_SIZE - 1, (int)(1000.0f / freqPerBin));
	float bestMag = 0.0f;
	int bestBin = 0;
	
	for(int k=0; k<HALF_FFT_SIZE; k++){
		// spectral rolloff (85 % energy threshold)
		if(!rolloffFound){
			cumulative += magnitude[k];
			if(cumulative >= rolloffThreshold){
				rolloff = (float)k * freqPerBin;
				rolloffFound = true;
			}
		}
		
		// spectral peaks (local maxima)
		if(k >= 1 && k < HALF_FFT_SIZE - 1){
			if(magnitude[k] > magnitude[k - 1] && magnitude[k] > magnitude[k + 1]){
				pWorkPeaks.Add({
					.frequency = (float)k * freqPerBin,
					.magnitude = magnitude[k]
				});
			}
		}
		
		// frequency bands (logarithmic spacing)
		if(k > 0){
			auto &band = pWorkBands[decMath::min((int)(logf((float)k) * invLogBandScale), bandCount - 1)];
			band.energy += magnitude[k];
			band.count++;
		}
		
		// pitch (highest magnitude bin in the 80-1000 Hz range)
		if(k >= lo && k <= hi && magnitude[k] > bestMag){
			bestMag = magnitude[k];
			bestBin = k;
		}
	}
	
	// average band energy by bin count so all bands are comparable regardless of width
	pWorkBands.Visit([&](Band &band){
		if(band.count > 0){
			band.energy /= (float)band.count;
		}
	});
	
	const float pitchHz = bestMag > 0.0f ? (float)bestBin * freqPerBin : 0.0f;
	
	// accumulate results under mutex
	const deMutexGuard guard(pMutexResults);
	
	pResFrameCount++;
	pResSampleCount += FFT_SIZE;
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
			pResBands[index] += band.energy;
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
}
