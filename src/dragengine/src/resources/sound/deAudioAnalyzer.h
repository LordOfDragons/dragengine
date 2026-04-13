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
	
	
private:
	bool pUseAudioCapture;
	int pSpectralPeaksCount;
	int pFrequencyBandsCount;
	
	float pRMS;
	float pPeak;
	float pZeroCrossingRate;
	float pSpectralCentroid;
	float pSpectralFlatness;
	float pSpectralFlux;
	float pSpectralRolloff;
	decTList<float> pSpectralPeaks;
	decTList<float> pFrequencyBands;
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
	
	/** \brief Count of spectral peaks. */
	inline int GetSpectralPeaksCount() const{ return pSpectralPeaksCount; }
	
	/** \brief Set count of spectral peaks. */
	void SetSpectralPeaksCount(int count);
	
	/** \brief Count of frequency bands. */
	inline int GetFrequencyBandsCount() const{ return pFrequencyBandsCount; }
	
	/** \brief Set count of frequency bands. */
	void SetFrequencyBandsCount(int count);
	
	
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
	
	/** \brief Most prominent frequency peaks in Hz sorted by magnitude. */
	inline decTList<float> &GetSpectralPeaks(){ return pSpectralPeaks; }
	inline const decTList<float> &GetSpectralPeaks() const{ return pSpectralPeaks; }
	
	/** \brief Energy levels of equally-spaced frequency bands in range 0 to 1. */
	inline decTList<float> &GetFrequencyBands(){ return pFrequencyBands; }
	inline const decTList<float> &GetFrequencyBands() const{ return pFrequencyBands; }
	
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
