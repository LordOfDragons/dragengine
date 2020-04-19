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

#ifndef _DESYNTHESIZERCONTROLLER_H_
#define _DESYNTHESIZERCONTROLLER_H_

#include "../../deObject.h"
#include "../../common/curve/decCurveBezier.h"
#include "../../common/math/decMath.h"
#include "../../common/string/decString.h"


/**
 * \brief Synthesizer controller.
 * 
 * Controllers provide the user with control over how the individual sounds of a synthesizer
 * produce the final sound. Controllers are written in a way that you can use them without
 * having to do scaling or transformation on your own. You can provide a minimum and maximum
 * value matching your setup. The controller takes care of transforming the current value
 * into the proper 0 to 1 range required for sounds to work with. The current value is
 * calculated using an value curve set by the user. The play position is used to sample the
 * curve producing the controller values over time. 
 */
class deSynthesizerController : public deObject{
private:
	decString pName;
	float pMinValue;
	float pMaxValue;
	bool pClamp;
	decCurveBezier pCurve;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create controller. */
	deSynthesizerController();
	
	/** \brief Create copy of controller. */
	deSynthesizerController( const deSynthesizerController &controller );
	
protected:
	/**
	 * \brief Clean up animator.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deSynthesizerController();
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
	
	/** \brief Clamp current value to range instead of wrapping around at the end points. */
	inline bool GetClamp() const{ return pClamp; }
	
	/** \brief Set if current value is clamped to range instead of wrapping around at the end points. */
	void SetClamp( bool clamp );
	
	/** \brief Value curve. */
	inline const decCurveBezier &GetCurve() const{ return pCurve; }
	
	/** \brief Set value curve. */
	void SetCurve( const decCurveBezier &curve );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy controller parameters. */
	deSynthesizerController &operator=( const deSynthesizerController &controller );
	/*@}*/
};

#endif
