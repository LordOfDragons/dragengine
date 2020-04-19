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

#ifndef _DESKYLINK_H_
#define _DESKYLINK_H_

#include "../../common/math/decMath.h"
#include "../../common/curve/decCurveBezier.h"


// Definitions
////////////////

#define DEAL_THRESHOLD		CURVE_THRESHOLD


/**
 * \brief Sky link.
 * 
 * Links controller value to layer targets. Maps controller values using a bezier curve.
 * Using repeat values larger than 1 the curve is repeated along the Y axis.
 */
class deSkyLink{
private:
	int pController;
	decCurveBezier pCurve;
	int pRepeat;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create link. */
	deSkyLink();
	
	/** \brief Create copy of link. */
	deSkyLink( const deSkyLink &copy );
	
	/** \brief Clean up link. */
	~deSkyLink();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Controller or -1 if not set. */
	inline int GetController() const{ return pController; }
	
	/** \brief Controller is set. */
	inline bool HasController() const{ return pController != -1; }
	
	/**
	 * \brief Set controller or -1 to not set.
	 * \throws deeInvalidParam \em controller is less than -1.
	 */
	void SetController( int controller );
	
	/** \brief Curve. */
	inline decCurveBezier &GetCurve(){ return pCurve; }
	inline const decCurveBezier &GetCurve() const{ return pCurve; }
	
	/** \brief Repeat count of input value. */
	inline int GetRepeat() const{ return pRepeat; }
	
	/**
	 * \brief Set repeat count of input value.
	 * \throws deeInvalidParam \em repeat is less than 1.
	 */
	void SetRepeat( int repeat );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy link to this link. */
	deSkyLink &operator=( const deSkyLink &copy );
	/*@}*/
};

#endif
