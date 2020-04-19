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

#ifndef _DEANIMATORCONTROLLER_H_
#define _DEANIMATORCONTROLLER_H_

#include "../../../common/math/decMath.h"
#include "../../../common/string/decString.h"


/**
 * \brief Animator Controller Class.
 *
 * Animator controllers provide the user with control over how the
 * individual rules of an animator produce an animation. Controllers
 * are written in a way that you can use them without having to do
 * scaling or transformation on your own. You can provide a minimum
 * and maximum value matching your setup. The controller takes care
 * of transforming the current value into the proper 0 to 1 range
 * required for rules to work with. The current value is set by the
 * user and is clamped to the range specified before. The real value
 * is in the range 0 to 1 and is used by the rules. The real value
 * can only be read. Furthermore a controller can be frozen. If a
 * controller is frozen no values can be changed. This is useful to
 * prevent a controller from changing without having to change all
 * code path to account for the frozen state. In addition a controller
 * can also contain a matrix instead of a ranged value. This can be
 * useful for certain specialized rules which need a position or even
 * orientations to do their work.
 */
class deAnimatorController{
private:
	decString pName;
	float pMinValue;
	float pMaxValue;
	float pCurValue;
	bool pClamp;
	bool pFrozen;
	decVector pVector;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create unnamed animator controller with range 0 to 1 and value 0. */
	deAnimatorController();
	
	/** \brief Clean up animator. */
	~deAnimatorController();
	/*@}*/
	
	
	
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
	
	/** \brief Controller is frozen. */
	inline bool GetFrozen() const{ return pFrozen; }
	
	/** \brief Set if controller is frozen. */
	void SetFrozen( bool frozen );
	
	/** \brief Values passing range are clamped instead of wrapped around. */
	inline bool GetClamp() const{ return pClamp; }
	
	/** \brief Set if values passing range are clamped instead of wrapped around. */
	void SetClamp( bool clamp );
	
	/** \brief Vector. */
	inline const decVector &GetVector() const{ return pVector; }
	
	/** \brief Set vector. */
	void SetVector( const decVector &vector );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy controller parameters. */
	deAnimatorController &operator=( const deAnimatorController &controller );
	/*@}*/
	
	
	
private:
	void pUpdateValue();
};

#endif
