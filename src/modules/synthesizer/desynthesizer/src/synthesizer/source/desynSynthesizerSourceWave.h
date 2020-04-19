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

#ifndef _DESYNSYNTHESIZERSOURCEWAVE_H_
#define _DESYNSYNTHESIZERSOURCEWAVE_H_

#include <dragengine/resources/synthesizer/source/deSynthesizerSourceWave.h>

#include "desynSynthesizerSource.h"



/**
 * \brief Synthesizer source wave.
 */
class desynSynthesizerSourceWave : public desynSynthesizerSource{
private:
	deSynthesizerSourceWave::eWaveType pType;
	
	float pMinFrequency;
	float pMaxFrequency;
	float pFrequencyRange;
	
	desynSynthesizerTarget pTargetFrequency;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer source. */
	desynSynthesizerSourceWave( desynSynthesizer &synthesizer, int firstLink,
		const deSynthesizerSourceWave &source );
	
	/** \brief Clean up synthesizer source. */
	virtual ~desynSynthesizerSourceWave();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Wave type. */
	inline deSynthesizerSourceWave::eWaveType GetType() const{ return pType; }
	
	/** \brief Minimum play frequency in Hz. */
	inline float GetMinFrequency() const{ return pMinFrequency; }
	
	/** \brief Maximum frequency in Hz. */
	inline float GetMaxFrequency() const{ return pMaxFrequency; }
	
	/** \brief Play frequency target. */
	inline desynSynthesizerTarget &GetTargetFrequency(){ return pTargetFrequency; }
	inline const desynSynthesizerTarget &GetTargetFrequency() const{ return pTargetFrequency; }
	
	
	
	/** \brief Current frequency. */
	float GetFrequency( const desynSynthesizerInstance &instance, int sample ) const;
	
	
	
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
	
	/** \brief Generate sine wave. */
	void GenerateSineWave( const desynSynthesizerInstance &instance, char *stateData,
		float *buffer, int samples, float curveOffset, float curveFactor );
	
	/** \brief Generate square wave. */
	void GenerateSquareWave( const desynSynthesizerInstance &instance, char *stateData,
		float *buffer, int samples, float curveOffset, float curveFactor );
	
	/** \brief Generate saw tooth wave. */
	void GenerateSawToothWave( const desynSynthesizerInstance &instance, char *stateData,
		float *buffer, int samples, float curveOffset, float curveFactor );
	
	/** \brief Generate triangle wave. */
	void GenerateTriangleWave( const desynSynthesizerInstance &instance, char *stateData,
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
