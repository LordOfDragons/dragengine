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
