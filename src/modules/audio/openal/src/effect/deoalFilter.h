/* 
 * Drag[en]gine OpenAL Audio Module
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
	deoalFilter( deoalAudioThread &audioThread );
	
	/** \brief Clean up openal filter. */
	~deoalFilter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief OpenAL module. */
	inline deoalAudioThread &GetAudioThread() const{ return pAudioThread; }
	
	/** \brief Filter. */
	inline ALuint GetFilter() const{ return pFilter; }
	
	
	
	/** \brief Set filter type. */
	void SetType( eFilterType type );
	
	/** \brief Set gain. */
	void SetGain( float gain );
	
	/**
	 * \brief Set low frequency gain relative to gain.
	 * 
	 * Gain of filtered lower frequencies relative to \em gain. For example if \em gain has a
	 * value of 0.5 and \em gainLow a value of 0.2 then the effective volume of
	 * low frequencies is 0.1 .
	 */
	void SetGainLF( float gain );
	
	/**
	 * \brief Set high frequency gain relative to gain.
	 * 
	 * Gain of filtered high frequencies relative to \em gain. For example if \em gain has a
	 * value of 0.5 and \em gainHigh a value of 0.2 then the effective volume of
	 * high frequencies is 0.1 .
	 */
	void SetGainHF( float gain );
	
	/**
	 * \brief Set gain values.
	 */
	void SetGains( float gain, float gainLF, float gainHF );
	/*@}*/
};

#endif
