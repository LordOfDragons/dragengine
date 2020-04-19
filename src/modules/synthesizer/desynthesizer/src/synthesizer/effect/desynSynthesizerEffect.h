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

#ifndef _DESYNSYNTHESIZEREFFECT_H_
#define _DESYNSYNTHESIZEREFFECT_H_

#include "../desynSynthesizerTarget.h"

#include <dragengine/resources/synthesizer/effect/deSynthesizerEffect.h>

class deDESynthesizer;
class desynSynthesizer;
class desynSynthesizerInstance;
class desynSynthesizerSource;



/**
 * \brief Synthesizer effect.
 */
class desynSynthesizerEffect{
private:
	const desynSynthesizer &pSynthesizer;
	desynSynthesizerSource &pSource;
	
	bool pDisabled;
	int pStateDataOffset;
	
	const desynSynthesizerTarget pTargetStrength;
	
	desynSynthesizerEffect *pPreviousEffect;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer effect. */
	desynSynthesizerEffect( const desynSynthesizer &synthesizer, int firstLink,
		desynSynthesizerSource &source, const deSynthesizerEffect &effect );
	
	/** \brief Clean up synthesizer effect. */
	virtual ~desynSynthesizerEffect();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief OpenAL module. */
	deDESynthesizer &GetModule() const;
	
	/** \brief Parent synthesizer. */
	inline const desynSynthesizer &GetSynthesizer() const{ return pSynthesizer; }
	
	/** \brief Parent source. */
	inline desynSynthesizerSource &GetSource() const{ return pSource; }
	
	
	
	/** \brief Effect is disabled. */
	inline bool GetDisabled() const{ return pDisabled; }
	
	/** \brief Set if effect is disabled. */
	void SetDisabled( bool disabled );
	
	/** \brief Offset in bytes to state data in synthesizer instances. */
	inline int GetStateDataOffset() const{ return pStateDataOffset; }
	
	/** \brief Set offset in bytes to state data in synthesizer instances. */
	void SetStateDataOffset( int offset );
	
	
	
	/** \brief Nearest curve evaluate position. */
	inline int NearestCurveEvalPosition( int indexSample, float curveOffset, float curveFactor ) const{
		return ( int )( curveOffset + curveFactor * ( float )indexSample );
	}
	
	/** \brief Strength target. */
	inline const desynSynthesizerTarget &GetTargetStrength() const{ return pTargetStrength; }
	
	
	
	/** \brief Previous effect or \em NULL if this is the first effect in the source. */
	inline desynSynthesizerEffect *GetPreviousEffect() const{ return pPreviousEffect; }
	
	/** \brief Set previous effect or \em NULL if this is the first effect in the source. */
	void SetPreviousEffect( desynSynthesizerEffect *effect );
	
	
	
	
	/** \brief Strength. */
	float GetStrength( const desynSynthesizerInstance &instance, int sample ) const;
	
	
	
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
