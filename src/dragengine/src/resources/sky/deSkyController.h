/* 
 * Drag[en]gine Game Engine
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

#ifndef _DESKYCONTROLLER_H_
#define _DESKYCONTROLLER_H_

#include "../../common/math/decMath.h"
#include "../../common/string/decString.h"


/**
 * \brief Sky controller.
 * 
 * Controllers provide the user with control over how the individual layers of a sky.
 * Controllers are written in a way that you can use them without having to do
 * scaling or transformation on your own. You can provide a minimum and maximum
 * value matching your setup. The controller takes care of transforming the current
 * value into the proper 0 to 1 range required for sounds to work with.
 */
class deSkyController{
private:
	decString pName;
	float pMinValue;
	float pMaxValue;
	float pCurValue;
	bool pClamp;
	bool pFrozen;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create controller. */
	deSkyController();
	
	/** \brief Create copy of controller. */
	deSkyController( const deSkyController &controller );
	
	/** \brief Clean up animator. */
	~deSkyController();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Minimum value. */
	inline float GetMinimumValue() const{ return pMinValue; }
	
	/** \brief Maximum value. */
	inline float GetMaximumValue() const{ return pMaxValue; }
	
	/** \brief Set value range. */
	void SetValueRange( float minValue, float maxValue );
	
	/** \brief Current value. */
	inline float GetCurrentValue() const{ return pCurValue; }
	
	/** \brief Set current value. */
	void SetCurrentValue( float value );
	
	/** \brief Increment current value. */
	void IncrementCurrentValue( float incrementBy );
	
	/** \brief Clamp current value to range instead of wrapping around at the end points. */
	inline bool GetClamp() const{ return pClamp; }
	
	/** \brief Set if current value is clamped to range instead of wrapping around at the end points. */
	void SetClamp( bool clamp );
	
	/** \brief Controller is frozen. */
	inline bool GetFrozen() const{ return pFrozen; }
	
	/** \brief Set if controller is frozen. */
	void SetFrozen( bool frozen );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy controller to this controller. */
	deSkyController &operator=( const deSkyController &controller );
	/*@}*/
	
	
	
private:
	void pUpdateValue();
};

#endif
