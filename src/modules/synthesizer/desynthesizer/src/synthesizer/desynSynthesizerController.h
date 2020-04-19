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
	inline float GetMinimumValue() const{ return pMinValue; }
	
	/** \brief Maximum value. */
	inline float GetMaximumValue() const{ return pMaxValue; }
	
	/** \brief Clamp current value to range instead of wrapping around at the end points. */
	inline bool GetClamp() const{ return pClamp; }
	
	/** \brief Value curve. */
	inline const desynSynthesizerCurve &GetCurve() const{ return pCurve; }
	
	
	
	/** \brief Sampled controller values. */
	float GetValue( int sample ) const;
	
	/**
	 * \brief Update controller value for input time.
	 * \details \em range is \em samples divided by sample rate.
	 */
	void UpdateValues( int samples, float time, float range );
	
	
	
	/** \brief Dirty. */
	inline bool GetDirty() const{ return pDirty; }
	
	/** \brief Set dirty. */
	void SetDirty( bool dirty );
	
	/** \brief Update from engine controller. */
	void Update( const deSynthesizerController &controller );
	/*@}*/
};

#endif
