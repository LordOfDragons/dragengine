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

#ifndef _DESYNSYNTHESIZEREFFECTSTRETCH_H_
#define _DESYNSYNTHESIZEREFFECTSTRETCH_H_

#include "desynSynthesizerEffect.h"

class deSynthesizerEffectStretch;


/**
 * \brief Synthesizer effect time and pitch stretch.
 */
class desynSynthesizerEffectStretch : public desynSynthesizerEffect{
private:
	const float pMinTime;
	const float pMaxTime;
	const float pRangeTime;
	
	const float pMinPitch;
	const float pMaxPitch;
	const float pRangePitch;
	
	const desynSynthesizerTarget pTargetTime;
	const desynSynthesizerTarget pTargetPitch;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer effect. */
	desynSynthesizerEffectStretch( const desynSynthesizer &synthesizer, int firstLink,
		desynSynthesizerSource &source, const deSynthesizerEffectStretch &effect );
	
	/** \brief Clean up synthesizer effect. */
	virtual ~desynSynthesizerEffectStretch();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Time stretch target. */
	inline const desynSynthesizerTarget &GetTargetTime() const{ return pTargetTime; }
	
	/** \brief Pitch shift target. */
	inline const desynSynthesizerTarget &GetTargetPitch() const{ return pTargetPitch; }
	
	
	
	/** \brief Time stretch. */
	float GetTimeStretch( const desynSynthesizerInstance &instance, int sample ) const;
	
	/** \brief Pitch shift. */
	float GetPitchShift( const desynSynthesizerInstance &instance, int sample ) const;
	
	
	
	/**
	 * \brief State data size.
	 * \details Store state data position and return required state data size. Default implementation
	 *          stores the offset and returns 0.
	 */
	virtual int StateDataSize( int offset );
	
	/** \brief Init state data. */
	virtual void InitStateData( char *stateData );
	
	/** \brief Clean up state data. */
	virtual void CleanUpStateData( char *stateData );
	
	/**
	 * \brief Generate sound.
	 * \param[in,out] stateData State at start of buffer. Update with state at start of next buffer.
	 * \param[out] buffer Buffer to store samples in.
	 * \param[in] samples Number of samples to produce.
	 */
	virtual void GenerateSound( const desynSynthesizerInstance &instance, char *stateData,
		float *buffer, int samples, float curveOffset, float curveFactor );
	
	/**
	 * \brief Skip sound.
	 * \param[in,out] stateData State at start of skipping. Update with state after skipping.
	 * \param[in] samples Number of samples to skip.
	 */
	virtual void SkipSound( const desynSynthesizerInstance &instance, char *stateData,
		int samples, float curveOffset, float curveFactor );
	/*@}*/
};

#endif
