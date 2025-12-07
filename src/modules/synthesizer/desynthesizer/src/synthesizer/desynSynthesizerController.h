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

#ifndef _DESYNSYNTHESIZERCONTROLLER_H_
#define _DESYNSYNTHESIZERCONTROLLER_H_

#include "desynSynthesizerCurve.h"

class deSynthesizerController;



/**
 * \brief Synthesizer instance controller.
 */
class desynSynthesizerController{
private:
	float pMinValue;
	float pMaxValue;
	bool pClamp;
	desynSynthesizerCurve pCurve;
	bool pDirty;
	
	float *pValues;
	int pValueSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer source. */
	desynSynthesizerController();
	
	/** \brief Clean up synthesizer source. */
	virtual ~desynSynthesizerController();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Minimum value. */
	inline float GetMinimumValue() const{return pMinValue;}
	
	/** \brief Maximum value. */
	inline float GetMaximumValue() const{return pMaxValue;}
	
	/** \brief Clamp current value to range instead of wrapping around at the end points. */
	inline bool GetClamp() const{return pClamp;}
	
	/** \brief Value curve. */
	inline const desynSynthesizerCurve &GetCurve() const{return pCurve;}
	
	
	
	/** \brief Sampled controller values. */
	float GetValue(int sample) const;
	
	/**
	 * \brief Update controller value for input time.
	 * \details \em range is \em samples divided by sample rate.
	 */
	void UpdateValues(int samples, float time, float range);
	
	
	
	/** \brief Dirty. */
	inline bool GetDirty() const{return pDirty;}
	
	/** \brief Set dirty. */
	void SetDirty(bool dirty);
	
	/** \brief Update from engine controller. */
	void Update(const deSynthesizerController &controller);
	/*@}*/
};

#endif
