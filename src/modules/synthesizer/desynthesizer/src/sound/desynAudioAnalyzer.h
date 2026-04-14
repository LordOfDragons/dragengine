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

#ifndef _DESYNAUDIOANALYZER_H_
#define _DESYNAUDIOANALYZER_H_

#include <stdint.h>
#include <fftw3.h>

#include <dragengine/common/collection/decTList.h>
#include <dragengine/resources/sound/deAudioCaptureListener.h>
#include <dragengine/systems/deAudioSystem.h>
#include <dragengine/systems/modules/synthesizer/deBaseSynthesizerAudioAnalyzer.h>
#include <dragengine/threading/deThread.h>
#include <dragengine/threading/deMutex.h>
#include <dragengine/threading/deSemaphore.h>

class deDESynthesizer;
class deAudioAnalyzer;


/**
 * \brief Synthesizer module audio analyzer peer.
 */
class desynAudioAnalyzer : public deBaseSynthesizerAudioAnalyzer{
public:
	static constexpr int FFT_SIZE = 1024;
	static constexpr int HALF_FFT_SIZE = FFT_SIZE / 2;
	
	
private:
	class CaptureAudio : public deAudioCaptureListener{
	private:
		desynAudioAnalyzer &pOwner;
		
	protected:
		~CaptureAudio() override;
		
	public:
		explicit CaptureAudio(desynAudioAnalyzer &owner);
		
		void StateChanged() override;
		void SamplesCaptured(const void *samples, int count) override;
	};
	
	class AnalyzeThread : public deThread{
	private:
		desynAudioAnalyzer &pOwner;
		
	public:
		explicit AnalyzeThread(desynAudioAnalyzer &owner);
		~AnalyzeThread() override;
		
		void Run() override;
	};
	
	struct Peak{
		float freq = 0.0f;
		float mag = 0.0f;
	};
	
	struct Band{
		float energy = 0.0f;
		int count = 0;
	};
	
	
	deDESynthesizer &pModule;
	deAudioAnalyzer &pAnalyzer;
	
	deTObjectReference<CaptureAudio> pListener;
	AnalyzeThread pThread;
	
	deMutex pMutexInput;
	deSemaphore pSemaphore;
	decTList<int16_t> pInputSamples;
	deAudioSystem::AudioCaptureFormat pInputFormat;
	
	bool pStopThread, pThreadRunning;
	
	int pSampleRate;
	
	double *pFftIn;
	fftw_complex *pFftOut;
	fftw_plan pFftPlan;
	
	float pPrevMagnitude[FFT_SIZE / 2];
	
	deMutex pMutexResults;
	
	decTList<Peak> pWorkPeaks;
	decTList<Band> pWorkBands;
	
	int pResFrameCount, pResSampleCount, pResZeroCrossings;
	float pResPeak, pResPeakMag;
	double pResSumSq, pResCentroid, pResFlatness, pResFlux, pResRolloff, pResPitch;
	decTList<Peak> pResSpectralPeaks;
	decTList<float> pResBands;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	desynAudioAnalyzer(deDESynthesizer &module, deAudioAnalyzer &analyzer);
	~desynAudioAnalyzer() override;
	/*@}*/
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Configuration changed. */
	void ConfigurationChanged() override;
	
	/** \brief Push results into the deAudioAnalyzer resource. Called on the engine thread. */
	void UpdateResults() override;
	/*@}*/
	
	
	/** \name Internal use by Listener and Thread */
	/*@{*/
	/** \brief Copy samples into the input buffer and signal the analysis thread. */
	void OnSamplesCaptured(const void *samples, int count);
	
	/** \brief Update sample rate from current capture format. */
	void OnStateChanged();
	
	/** \brief Thread entry point; returns when pStopThread is set. */
	void AnalyzeLoop();
	/*@}*/
	
	
private:
	void pProcessFrame(const decTList<int16_t> &samples);
};

#endif
