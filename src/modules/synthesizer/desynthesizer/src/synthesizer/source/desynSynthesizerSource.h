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

#ifndef _DESYNSYNTHESIZERSOURCE_H_
#define _DESYNSYNTHESIZERSOURCE_H_

#include "../desynSynthesizerTarget.h"

#include <dragengine/resources/synthesizer/source/deSynthesizerSource.h>

class deDESynthesizer;
class desynSynthesizer;
class desynSynthesizerInstance;
class desynSynthesizerEffect;



/**
 * \brief Synthesizer source.
 */
class desynSynthesizerSource{
public:
	/** \brief Generate buffer mono. */
	struct sGenerateBufferMono{
		float value;
	};
	
	/** \brief Generate buffer stereo. */
	struct sGenerateBufferStereo{
		float left;
		float right;
	};
	
	
	
private:
	const desynSynthesizer &pSynthesizer;
	
	bool pSilent;
	int pStateDataOffset;
	
	deSynthesizerSource::eMixModes pMixMode;
	
	float pMinVolume;
	float pMaxVolume;
	float pVolumeRange;
	
	float pMinPanning;
	float pMaxPanning;
	float pPanningRange;
	
	desynSynthesizerTarget pTargetBlendFactor;
	desynSynthesizerTarget pTargetVolume;
	desynSynthesizerTarget pTargetPanning;
	
	desynSynthesizerEffect **pEffects;
	int pEffectCount;
	desynSynthesizerEffect *pApplyEffect;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer source. */
	desynSynthesizerSource( desynSynthesizer &synthesizer, int firstLink,
		const deSynthesizerSource &source );
	
	/** \brief Clean up synthesizer source. */
	virtual ~desynSynthesizerSource();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief OpenAL module. */
	deDESynthesizer &GetModule() const;
	
	/** \brief Parent synthesizer. */
	inline const desynSynthesizer &GetSynthesizer() const{ return pSynthesizer; }
	
	
	
	/** \brief Source is silent. */
	inline bool GetSilent() const{ return pSilent; }
	
	/** \brief Set if source is silent. */
	void SetSilent( bool silent );
	
	/** \brief Offset in bytes to state data in synthesizer instances. */
	inline int GetStateDataOffset() const{ return pStateDataOffset; }
	
	/** \brief Set offset in bytes to state data in synthesizer instances. */
	void SetStateDataOffset( int offset );
	
	
	
	/** \brief Mix mode. */
	inline deSynthesizerSource::eMixModes GetMixMode() const{ return pMixMode; }
	
	
	
	/** \brief Blend factor target. */
	inline desynSynthesizerTarget &GetTargetBlendFactor(){ return pTargetBlendFactor; }
	inline const desynSynthesizerTarget &GetTargetBlendFactor() const{ return pTargetBlendFactor; }
	
	/** \brief Volume target. */
	inline desynSynthesizerTarget &GetTargetVolume(){ return pTargetVolume; }
	inline const desynSynthesizerTarget &GetTargetVolume() const{ return pTargetVolume; }
	
	/** \brief Panning target. */
	inline desynSynthesizerTarget &GetTargetPanning(){ return pTargetPanning; }
	inline const desynSynthesizerTarget &GetTargetPanning() const{ return pTargetPanning; }
	
	
	
	/** \brief Nearest curve evaluate position. */
	inline int NearestCurveEvalPosition( int indexSample, float curveOffset, float curveFactor ) const{
		return ( int )( curveOffset + curveFactor * ( float )indexSample );
	}
	
	/** \brief Blend factor. */
	float GetBlendFactor( const desynSynthesizerInstance &instance, int sample ) const;
	
	/** \brief Current volume. */
	float GetVolume( const desynSynthesizerInstance &instance, int sample ) const;
	
	/** \brief Current panning. */
	float GetPanning( const desynSynthesizerInstance &instance, int sample ) const;
	
	
	
	/**
	 * \brief State data size.
	 * \details Store state data position and return required state data size. Default implementation
	 *          stores the offset and returns 0.
	 */
	int StateDataSize( int offset );
	
	/**
	 * \brief State data size of the source itself.
	 * \details Store state data position and return required state data size. Default implementation
	 *          stores the offset and returns 0.
	 */
	virtual int StateDataSizeSource( int offset );
	
	/** \brief Init state data. */
	void InitStateData( char *stateData );
	
	/** \brief Init state data of source itself. */
	virtual void InitStateDataSource( char *stateData );
	
	/** \brief Clean up state data. */
	void CleanUpStateData( char *stateData );
	
	/** \brief Clean up state data of source itself. */
	virtual void CleanUpStateDataSource( char *stateData );
	
	
	
	/**
	 * \brief Generate sound, applies effects and applies the result to the existing sound.
	 * \param[in,out] stateData State at start of buffer. Update with state at start of next buffer.
	 * \param[out] buffer Buffer to store samples in.
	 * \param[in] samples Number of samples to produce.
	 */
	void GenerateSound( const desynSynthesizerInstance &instance, char *stateData,
		float *buffer, int samples, float curveOffset, float curveFactor );
	
	/**
	 * \brief Generate sound using source.
	 * \details Actual sound generation implementation of the source. Used to split general sound
	 *          processing of the source from the actual source specific sound generation and
	 *          allow effect processing.
	 * \param[in,out] stateData State at start of buffer. Update with state at start of next buffer.
	 * \param[out] buffer Buffer to store samples in.
	 * \param[in] samples Number of samples to produce.
	 */
	virtual void GenerateSourceSound( const desynSynthesizerInstance &instance, char *stateData,
		float *buffer, int samples, float curveOffset, float curveFactor ) = 0;
	
	/** \brief Generate silence. */
	void GenerateSilence( const desynSynthesizerInstance &instance, float *buffer, int samples );
	
	/** \brief Generate silence. */
	void GenerateSilence( const desynSynthesizerInstance &instance, float *buffer, int offset, int samples );
	
	/** \brief Apply silence. */
	void ApplySilence( const desynSynthesizerInstance &instance, float *buffer,
		int samples, float curveOffset, float curveFactor );
	
	/** \brief Apply generated sound to the output buffer using mixing and volume. */
	void ApplyGeneratedSound( const desynSynthesizerInstance &instance, float *outputBuffer,
		const float *generatedBuffer, int samples, float curveOffset, float curveFactor );
	
	/** \brief Apply generated mono sound to the output buffer using add mixing and volume. */
	void ApplyGeneratedSoundMonoAdd( const desynSynthesizerInstance &instance, float *outputBuffer,
		const float *generatedBuffer, int samples, float curveOffset, float curveFactor );
	
	/** \brief Apply generated mono sound to the output buffer using blend mixing and volume. */
	void ApplyGeneratedSoundMonoBlend( const desynSynthesizerInstance &instance, float *outputBuffer,
		const float *generatedBuffer, int samples, float curveOffset, float curveFactor );
	
	/** \brief Apply generated stereo sound to the output buffer using add mixing and volume. */
	void ApplyGeneratedSoundStereoAdd( const desynSynthesizerInstance &instance, float *outputBuffer,
		const float *generatedBuffer, int samples, float curveOffset, float curveFactor );
	
	/** \brief Apply generated stereo sound to the output buffer using blend mixing and volume. */
	void ApplyGeneratedSoundStereoBlend( const desynSynthesizerInstance &instance, float *outputBuffer,
		const float *generatedBuffer, int samples, float curveOffset, float curveFactor );
	
	/**
	 * \brief Skip sound.
	 * \param[in,out] stateData State at start of skipping. Update with state after skipping.
	 * \param[in] samples Number of samples to skip.
	 */
	void SkipSound( const desynSynthesizerInstance &instance, char *stateData,
		int samples, float curveOffset, float curveFactor);
	
	/**
	 * \brief Skip sound.
	 * \details Actual skip implementation of the source. Used to split general skip processing of
	 *          source from the actual source specific skip processing and allow effect processing.
	 * \param[in,out] stateData State at start of skipping. Update with state after skipping.
	 * \param[in] samples Number of samples to skip.
	 */
	virtual void SkipSourceSound( const desynSynthesizerInstance &instance, char *stateData,
		int samples, float curveOffset, float curveFactor ) = 0;
	/*@}*/
	
	
	
private:
	void pCreateEffects( desynSynthesizer &synthesizer, int firstLink, const deSynthesizerSource &source );
	void pFreeEffects();
};

#endif
