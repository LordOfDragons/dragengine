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
	desynSynthesizerEffect(const desynSynthesizer &synthesizer, int firstLink,
		desynSynthesizerSource &source, const deSynthesizerEffect &effect);
	
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
	void SetDisabled(bool disabled);
	
	/** \brief Offset in bytes to state data in synthesizer instances. */
	inline int GetStateDataOffset() const{ return pStateDataOffset; }
	
	/** \brief Set offset in bytes to state data in synthesizer instances. */
	void SetStateDataOffset(int offset);
	
	
	
	/** \brief Nearest curve evaluate position. */
	inline int NearestCurveEvalPosition(int indexSample, float curveOffset, float curveFactor) const{
		return (int)(curveOffset + curveFactor * (float)indexSample);
	}
	
	/** \brief Strength target. */
	inline const desynSynthesizerTarget &GetTargetStrength() const{ return pTargetStrength; }
	
	
	
	/** \brief Previous effect or \em NULL if this is the first effect in the source. */
	inline desynSynthesizerEffect *GetPreviousEffect() const{ return pPreviousEffect; }
	
	/** \brief Set previous effect or \em NULL if this is the first effect in the source. */
	void SetPreviousEffect(desynSynthesizerEffect *effect);
	
	
	
	
	/** \brief Strength. */
	float GetStrength(const desynSynthesizerInstance &instance, int sample) const;
	
	
	
	/**
	 * \brief State data size.
	 * \details Store state data position and return required state data size. Default implementation
	 *          stores the offset and returns 0.
	 */
	virtual int StateDataSize(int offset);
	
	/** \brief Init state data. */
	virtual void InitStateData(char *stateData);
	
	/** \brief Clean up state data. */
	virtual void CleanUpStateData(char *stateData);
	
	/**
	 * \brief Generate sound.
	 * \param[in,out] stateData State at start of buffer. Update with state at start of next buffer.
	 * \param[out] buffer Buffer to store samples in.
	 * \param[in] samples Number of samples to produce.
	 */
	virtual void GenerateSound(const desynSynthesizerInstance &instance, char *stateData,
		float *buffer, int samples, float curveOffset, float curveFactor);
	
	/**
	 * \brief Skip sound.
	 * \param[in,out] stateData State at start of skipping. Update with state after skipping.
	 * \param[in] samples Number of samples to skip.
	 */
	virtual void SkipSound(const desynSynthesizerInstance &instance, char *stateData,
		int samples, float curveOffset, float curveFactor);
	/*@}*/
};

#endif
