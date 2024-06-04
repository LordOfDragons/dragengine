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

#ifndef _DESYNSYNTHESIZERSOURCECHAIN_H_
#define _DESYNSYNTHESIZERSOURCECHAIN_H_


#include "desynSynthesizerSource.h"
#include "../../desynBasics.h"

class desynSound;

class deSynthesizerSourceChain;


/**
 * \brief Synthesizer source sound.
 */
class desynSynthesizerSourceChain : public desynSynthesizerSource{
private:
	struct sSound{
		desynSound *sound;
		int sampleCount;
		int sampleRate;
		int lastSample;
		const sSampleMono8 *dataMono8;
		const sSampleMono16 *dataMono16;
		const sSampleStereo8 *dataStereo8;
		const sSampleStereo16 *dataStereo16;
		sSound();
		void Clear();
	};
	
	sSound *pSounds;
	int pSoundCount;
	
	float pMinSpeed;
	float pMaxSpeed;
	float pSpeedRange;
	float pSelectRange;
	
	desynSynthesizerTarget pTargetSpeed;
	desynSynthesizerTarget pTargetSelect;
	desynSynthesizerTarget pTargetPlay;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer source. */
	desynSynthesizerSourceChain( desynSynthesizer &synthesizer, int firstLink,
		const deSynthesizerSourceChain &source );
	
	/** \brief Clean up synthesizer source. */
	virtual ~desynSynthesizerSourceChain();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Play speed target. */
	inline desynSynthesizerTarget &GetTargetSpeed(){ return pTargetSpeed; }
	inline const desynSynthesizerTarget &GetTargetSpeed() const{ return pTargetSpeed; }
	
	/** \brief Select target. */
	inline desynSynthesizerTarget &GetTargetSelect(){ return pTargetSelect; }
	inline const desynSynthesizerTarget &GetTargetSelect() const{ return pTargetSelect; }
	
	/** \brief Play target. */
	inline desynSynthesizerTarget &GetTargetPlay(){ return pTargetPlay; }
	inline const desynSynthesizerTarget &GetTargetPlay() const{ return pTargetPlay; }
	
	
	
	/** \brief Speed target. */
	float GetSpeed( const desynSynthesizerInstance &instance, int sample ) const;
	
	/** \brief Select target. */
	int GetSelect( const desynSynthesizerInstance &instance, int sample ) const;
	
	/** \brief Play from target. */
	float GetPlay( const desynSynthesizerInstance &instance, int sample ) const;
	
	
	
	/**
	 * \brief State data size of the source itself.
	 * \details Store state data position and return required state data size. Default implementation
	 *          stores the offset and returns 0.
	 */
	virtual int StateDataSizeSource( int offset );
	
	/** \brief Init state data of source itself. */
	virtual void InitStateDataSource( char *stateData );
	
	/**
	 * \brief Generate sound using source.
	 * \details Actual sound generation implementation of the source. Used to split general sound
	 *          processing of the source from the actual source specific sound generation.
	 * \param[in,out] stateData State at start of buffer. Update with state at start of next buffer.
	 * \param[out] buffer Buffer to store samples in.
	 * \param[in] samples Number of samples to produce.
	 */
	virtual void GenerateSourceSound( const desynSynthesizerInstance &instance, char *stateData,
		float *buffer, int samples, float curveOffset, float curveFactor );
	
	/**
	 * \brief Generate mono sound.
	 * \param[out] buffer Buffer to store samples in.
	 * \param[in] offset Offset in samples to start producing sound at.
	 * \param[in] samples Number of samples to produce.
	 */
	void GenerateSoundMono( const desynSynthesizerInstance &instance, char *stateData,
		float *buffer, int samples, float curveOffset, float curveFactor );
	
	/**
	 * \brief Generate stereo sound.
	 * \param[out] buffer Buffer to store samples in.
	 * \param[in] offset Offset in samples to start producing sound at.
	 * \param[in] samples Number of samples to produce.
	 */
	void GenerateSoundStereo( const desynSynthesizerInstance &instance, char *stateData,
		float *buffer, int samples, float curveOffset, float curveFactor );
	
	/**
	 * \brief Skip sound.
	 * \details Actual skip implementation of the source. Used to split general skip processing of
	 *          source from the actual source specific skip processing and allow effect processing.
	 * \param[in,out] stateData State at start of skipping. Update with state after skipping.
	 * \param[in] samples Number of samples to skip.
	 */
	virtual void SkipSourceSound( const desynSynthesizerInstance &instance, char *stateData,
		int samples, float curveOffset, float curveFactor );
	/*@}*/
};

#endif
