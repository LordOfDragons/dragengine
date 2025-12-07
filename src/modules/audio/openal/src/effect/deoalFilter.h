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

#ifndef _DEOALFILTER_H_
#define _DEOALFILTER_H_

#include "../deoalBasics.h"



/**
 * \brief OpenAL filter object.
 */
class deoalFilter{
public:
	/** \brief Filter type. */
	enum eFilterType{
		/** \brief NULL filter. */
		eftNull,
		
		/**
		 * \brief Low-pass filter.
		 * 
		 * Parameters: \em gain, \em gainHF.
		 */
		eftLowPass,
		
		/**
		 * \brief High-pass filter.
		 * 
		 * Parameters: \em gain, \em gainLow.
		 */
		eftHighPass,
		
		/**
		 * \brief Band-pass filter.
		 * 
		 * The name band-pass filter is quite missleading. If \em gainHigh is less than
		 * \em gainLow then the filter behaves like a low-pass filter otherwise like a
		 * high-pass filter. A real band-pass filter looks different.
		 * 
		 * Parameters: \em gain, \em gainLow, \em gainHigh.
		 */
		eftBandPass
	};
	
	
	
private:
	deoalAudioThread &pAudioThread;
	
	eFilterType pType;
	float pGain;
	float pGainLF;
	float pGainHF;
	
	ALuint pFilter;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create openal filter. */
	deoalFilter(deoalAudioThread &audioThread);
	
	/** \brief Clean up openal filter. */
	~deoalFilter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief OpenAL module. */
	inline deoalAudioThread &GetAudioThread() const{return pAudioThread;}
	
	/** \brief Filter. */
	inline ALuint GetFilter() const{return pFilter;}
	
	
	
	/** \brief Set filter type. */
	void SetType(eFilterType type);
	
	/** \brief Set gain. */
	void SetGain(float gain);
	
	/**
	 * \brief Set low frequency gain relative to gain.
	 * 
	 * Gain of filtered lower frequencies relative to \em gain. For example if \em gain has a
	 * value of 0.5 and \em gainLow a value of 0.2 then the effective volume of
	 * low frequencies is 0.1 .
	 */
	void SetGainLF(float gain);
	
	/**
	 * \brief Set high frequency gain relative to gain.
	 * 
	 * Gain of filtered high frequencies relative to \em gain. For example if \em gain has a
	 * value of 0.5 and \em gainHigh a value of 0.2 then the effective volume of
	 * high frequencies is 0.1 .
	 */
	void SetGainHF(float gain);
	
	/**
	 * \brief Set gain values.
	 */
	void SetGains(float gain, float gainLF, float gainHF);
	/*@}*/
};

#endif
