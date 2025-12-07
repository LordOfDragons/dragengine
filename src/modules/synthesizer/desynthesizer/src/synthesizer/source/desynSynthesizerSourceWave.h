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
	desynSynthesizerSourceWave(desynSynthesizer &synthesizer, int firstLink,
		const deSynthesizerSourceWave &source);
	
	/** \brief Clean up synthesizer source. */
	~desynSynthesizerSourceWave() override;
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
	float GetFrequency(const desynSynthesizerInstance &instance, int sample) const;
	
	
	
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
	
	/** \brief Generate sine wave. */
	void GenerateSineWave(const desynSynthesizerInstance &instance, char *stateData,
		float *buffer, int samples, float curveOffset, float curveFactor);
	
	/** \brief Generate square wave. */
	void GenerateSquareWave(const desynSynthesizerInstance &instance, char *stateData,
		float *buffer, int samples, float curveOffset, float curveFactor);
	
	/** \brief Generate saw tooth wave. */
	void GenerateSawToothWave(const desynSynthesizerInstance &instance, char *stateData,
		float *buffer, int samples, float curveOffset, float curveFactor);
	
	/** \brief Generate triangle wave. */
	void GenerateTriangleWave(const desynSynthesizerInstance &instance, char *stateData,
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
};

#endif
