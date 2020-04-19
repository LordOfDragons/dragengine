/* 
 * Drag[en]gine Synthesizer Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
