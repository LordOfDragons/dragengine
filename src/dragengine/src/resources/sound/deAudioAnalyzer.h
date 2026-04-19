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

#ifndef _DEAUDIOANALYZER_H_
#define _DEAUDIOANALYZER_H_

#include <stdint.h>

#include "../deResource.h"
#include "../../common/collection/decTList.h"
#include "../../deTUniqueReference.h"
#include "../../systems/modules/synthesizer/deBaseSynthesizerAudioAnalyzer.h"

class deAudioAnalyzerManager;


/**
 * \brief Audio analyzer resource.
 * 
 * Analyzes audio data providing time-domain and frequency-domain results.
 */
class DE_DLL_EXPORT deAudioAnalyzer : public deResource{
public:
	/** \brief Type holding strong reference. */
	using Ref = deTObjectReference<deAudioAnalyzer>;
	
	/** \brief Synthesizer module peer reference. */
	using PeerSynthesizer = deTUniqueReference<deBaseSynthesizerAudioAnalyzer>;
	
	
	/** \brief Spectral peak. */
	struct SpectralPeak{
		/** \brief Frequency of the peak in Hz. */
		float frequency = 0.0f;
		
		/** \brief Magnitude of the peak. */
		float magnitude = 0.0f;
	};
	
	/** \brief Frequency band. */
	struct FrequencyBand{
		/** \brief Lowest frequency of the band in Hz. */
		float lowestFrequency = 0.0f;
		
		/** \brief Highest frequency of the band in Hz. */
		float highestFrequency = 0.0f;
		
		/** \brief Magnitude of the band. */
		float magnitude = 0.0f;
		
		/** \brief Energy of the band. */
		float energy = 0.0f;
		
		/**
		 * \brief Mel Frequency Cepstral Coefficient.
		 * 
		 * Set to the DCT coefficient of the log mel energies corresponding to this band.
		 * Only valid if mel filtering is enabled. Otherwise 0.
		 */
		float mfcc = 0.0f;
	};
	
	
private:
	bool pUseAudioCapture;
	int pSampleRate;
	int pResolution;
	int pFrequencyBandCount;
	float pLowestFrequency, pHighestFrequency;
	bool pEnablePreEmphasis;
	float pPreEmphasisFactor;
	bool pEnableMelFiltering;
	int pMelFilterCount;
	bool pNormalizeMelEnergies;
	
	float pRMS;
	float pPeak;
	float pZeroCrossingRate;
	float pSpectralCentroid;
	float pSpectralFlatness;
	float pSpectralFlux;
	float pSpectralRolloff;
	decTList<SpectralPeak> pSpectralPeaks;
	decTList<FrequencyBand> pFrequencyBands;
	float pPitch;
	
	PeerSynthesizer pPeerSynthesizer;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create audio analyzer. */
	deAudioAnalyzer(deAudioAnalyzerManager *manager);
	
protected:
	/**
	 * \brief Clean up audio analyzer.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deAudioAnalyzer() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/**
	 * \brief Use audio capture as input for analysis.
	 * 
	 * Default is false.
	 */
	inline bool GetUseAudioCapture() const{ return pUseAudioCapture; }
	
	/**
	 * \brief Set use of audio capture as input for analysis.
	 */
	void SetUseAudioCapture(bool useAudioCapture);
	
	/**
	 * \brief Analysis sample rate in Hz.
	 * 
	 * Downsamples input from input sample rate to analysis sample rate.
	 * Default is 16000.
	 */
	inline int GetSampleRate() const{ return pSampleRate; }
	
	/**
	 * \brief Set analysis sample rate in Hz.
	 * 
	 * Downsamples input from input sample rate to analysis sample rate.
	 */
	void SetSampleRate(int sampleRate);
	
	/**
	 * \brief FFT resolution (window size in samples).
	 * 
	 * Default is 1024.
	 */
	inline int GetResolution() const{ return pResolution; }
	
	/** \brief Set FFT resolution. */
	void SetResolution(int resolution);
	
	/** \brief Count of frequency bands. */
	inline int GetFrequencyBandCount() const{ return pFrequencyBandCount; }
	
	/** \brief Set count of frequency bands. */
	void SetFrequencyBandCount(int count);
	
	/** \brief Lowest frequency for frequency bands in Hz. */
	inline float GetLowestFrequency() const{ return pLowestFrequency; }
	
	/** \brief Highest frequency for frequency bands in Hz. */
	inline float GetHighestFrequency() const{ return pHighestFrequency; }
	
	/** \brief Set frequency range for frequency bands in Hz. */
	void SetFrequencyRange(float lowestFrequency, float highestFrequency);
	
	/**
	 * \brief Apply pre-emphasis filter before FFT.
	 * 
	 * Default is true.
	 */
	inline bool GetEnablePreEmphasis() const{ return pEnablePreEmphasis; }
	
	/** \brief Set apply pre-emphasis filter before FFT. */
	void SetEnablePreEmphasis(bool enable);
	
	/**
	 * \brief Pre-emphasis filter coefficient.
	 * 
	 * Default is 0.97.
	 */
	inline float GetPreEmphasisFactor() const{ return pPreEmphasisFactor; }
	
	/** \brief Set pre-emphasis filter coefficient. */
	void SetPreEmphasisFactor(float factor);
	
	/**
	 * \brief Apply mel filter banks after FFT.
	 * 
	 * If enabled changes frequency bands from logarithmically spaced to mel-scale spaced
	 * triangular filters. Default is false.
	 */
	inline bool GetEnableMelFiltering() const{ return pEnableMelFiltering; }
	
	/**
	 * \brief Set apply mel filter banks after FFT.
	 * 
	 * If enabled changes frequency bands from logarithmically spaced to mel-scale spaced
	 * triangular filters.
	 */
	void SetEnableMelFiltering(bool enable);
	
	/**
	 * \brief Count of mel filter banks.
	 * 
	 * Default is 26.
	 */
	inline int GetMelFilterCount() const{ return pMelFilterCount; }
	
	/** \brief Set count of mel filter banks. */
	void SetMelFilterCount(int count);
	
	/**
	 * \brief Normalize mel filter bank energies.
	 * 
	 * Default is true.
	 */
	inline bool GetNormalizeMelEnergies() const{ return pNormalizeMelEnergies; }
	
	/** \brief Set normalize mel filter bank energies. */
	void SetNormalizeMelEnergies(bool normalize);
	
	/**
	 * \brief Update results.
	 * 
	 * Analysis is usually performed asynchronously by the synthesizer module. Call function to
	 * let the module update the results for later use.
	 */
	void UpdateResults();
	/*@}*/
	
	
	/** \name Time-Domain Results */
	/*@{*/
	/** \brief Root mean square amplitude in range 0 to 1. */
	inline float GetRMS() const{ return pRMS; }
	
	/**
	 * \brief Set root mean square amplitude.
	 * 
	 * Called by synthesizer module peer.
	 */
	void SetRMS(float rms);
	
	/** \brief Absolute peak amplitude in range 0 to 1. */
	inline float GetPeak() const{ return pPeak; }
	
	/**
	 * \brief Set absolute peak amplitude.
	 * 
	 * Called by synthesizer module peer.
	 */
	void SetPeak(float peak);
	
	/** \brief Zero-crossing rate in range 0 to 1. */
	inline float GetZeroCrossingRate() const{ return pZeroCrossingRate; }
	
	/**
	 * \brief Set zero-crossing rate.
	 * 
	 * Called by synthesizer module peer.
	 */
	void SetZeroCrossingRate(float zeroCrossingRate);
	/*@}*/
	
	
	/** \name Frequency-Domain Results */
	/*@{*/
	/** \brief Spectral centroid (center of mass of spectrum) in Hz. */
	inline float GetSpectralCentroid() const{ return pSpectralCentroid; }
	
	/**
	 * \brief Set spectral centroid in Hz.
	 * 
	 * Called by synthesizer module peer.
	 */
	void SetSpectralCentroid(float spectralCentroid);
	
	/** \brief Spectral flatness in range 0(tonal) to 1(noise-like). */
	inline float GetSpectralFlatness() const{ return pSpectralFlatness; }
	
	/**
	 * \brief Set spectral flatness.
	 * 
	 * Called by synthesizer module peer.
	 */
	void SetSpectralFlatness(float spectralFlatness);
	
	/** \brief Spectral flux measuring rate of spectrum change. */
	inline float GetSpectralFlux() const{ return pSpectralFlux; }
	
	/**
	 * \brief Set spectral flux.
	 * 
	 * Called by synthesizer module peer.
	 */
	void SetSpectralFlux(float spectralFlux);
	
	/** \brief Spectral roll-off frequency in Hz. */
	inline float GetSpectralRolloff() const{ return pSpectralRolloff; }
	
	/**
	 * \brief Set spectral roll-off frequency in Hz.
	 * 
	 * Called by synthesizer module peer.
	 */
	void SetSpectralRolloff(float spectralRolloff);
	
	/** \brief Spectral peak sorted from strongest to weakest. */
	inline decTList<SpectralPeak> &GetSpectralPeaks(){ return pSpectralPeaks; }
	inline const decTList<SpectralPeak> &GetSpectralPeaks() const{ return pSpectralPeaks; }
	
	/** \brief Energy levels of logarithmically spaced frequency bands. */
	inline decTList<FrequencyBand> &GetFrequencyBands(){ return pFrequencyBands; }
	inline const decTList<FrequencyBand> &GetFrequencyBands() const{ return pFrequencyBands; }
	
	/** \brief Estimated pitch (fundamental frequency) in Hz or 0 if not detected. */
	inline float GetPitch() const{ return pPitch; }
	
	/**
	 * \brief Set estimated pitch in Hz.
	 * 
	 * Called by synthesizer module peer.
	 */
	void SetPitch(float pitch);
	/*@}*/
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Synthesizer module peer or nullptr if not set. */
	inline const PeerSynthesizer &GetPeerSynthesizer() const{ return pPeerSynthesizer; }
	
	/** \brief Set synthesizer module peer. */
	void SetPeerSynthesizer(PeerSynthesizer &&peerSynthesizer);
	/*@}*/
};

#endif
