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

#ifndef _DESYNSYNTHESIZERSOURCEGROUP_H_
#define _DESYNSYNTHESIZERSOURCEGROUP_H_

#include "desynSynthesizerSource.h"
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceGroup.h>

class deSynthesizerSourceGroup;
class desynSynthesizerSource;



/**
 * \brief Synthesizer source group.
 */
class desynSynthesizerSourceGroup : public desynSynthesizerSource{
private:
	desynSynthesizerSource **pSources;
	int pSourceCount;
	
	deSynthesizerSourceGroup::eApplicationTypes pApplicationType;
	float pSelectRange;
	
	desynSynthesizerTarget pTargetSelect;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer source. */
	desynSynthesizerSourceGroup(desynSynthesizer &synthesizer, int firstLink,
		const deSynthesizerSourceGroup &source);
	
	/** \brief Clean up synthesizer source. */
	~desynSynthesizerSourceGroup() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Select target. */
	inline desynSynthesizerTarget &GetTargetSelect(){ return pTargetSelect; }
	inline const desynSynthesizerTarget &GetTargetSelect() const{ return pTargetSelect; }
	
	
	
	/** \brief Select value. */
	float GetSelect(const desynSynthesizerInstance &instance, int sample) const;
	
	
	
	/**
	 * \brief State data size of the source itself.
	 * \details Store state data position and return required state data size. Default implementation
	 *          stores the offset and returns 0.
	 */
	int StateDataSizeSource(int offset) override;
	
	/** \brief Init state data of source itself. */
	void InitStateDataSource(char *stateData) override;
	
	/**
	 * \brief Generate sound using source.
	 * \details Actual sound generation implementation of the source. Used to split general sound
	 *          processing of the source from the actual source specific sound generation.
	 * \param[in,out] stateData State at start of buffer. Update with state at start of next buffer.
	 * \param[out] buffer Buffer to store samples in.
	 * \param[in] samples Number of samples to produce.
	 */
	void GenerateSourceSound(const desynSynthesizerInstance &instance, char *stateData,
		float *buffer, int samples, float curveOffset, float curveFactor) override;
	
	/** \brief Generate sound using all mode. */
	void GenerateSoundAll(const desynSynthesizerInstance &instance, char *stateData,
		float *buffer, int samples, float curveOffset, float curveFactor);
	
	/** \brief Generate sound using select mode. */
	void GenerateSoundSelect(const desynSynthesizerInstance &instance, char *stateData,
		float *buffer, int samples, float curveOffset, float curveFactor);
	
	/** \brief Generate sound using solo mode. */
	void GenerateSoundSolo(const desynSynthesizerInstance &instance, char *stateData,
		float *buffer, int samples, float curveOffset, float curveFactor);
	
	/**
	 * \brief Skip sound.
	 * \details Actual skip implementation of the source. Used to split general skip processing of
	 *          source from the actual source specific skip processing and allow effect processing.
	 * \param[in,out] stateData State at start of skipping. Update with state after skipping.
	 * \param[in] samples Number of samples to skip.
	 */
	void SkipSourceSound(const desynSynthesizerInstance &instance, char *stateData,
		int samples, float curveOffset, float curveFactor) override;
	/*@}*/
	
	
	
private:
	void pCreateSources(desynSynthesizer &synthesizer, int firstLink, const deSynthesizerSourceGroup &source);
	void pClearSources();
};

#endif
