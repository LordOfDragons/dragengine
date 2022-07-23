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

#ifndef _DECCURVEBEZIEREVALUATOR_H_
#define _DECCURVEBEZIEREVALUATOR_H_

#include "../math/decMath.h"

class decCurveBezier;


/**
 * \brief Bezier Curve Evaluator.
 */
class DE_DLL_EXPORT decCurveBezierEvaluator{
private:
	const decCurveBezier &pCurve;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new bezier curve evaluator. */
	decCurveBezierEvaluator( const decCurveBezier &curve );
	
	/** \brief Clean up the bezier curve evaluator. */
	~decCurveBezierEvaluator();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Bezier curve. */
	inline const decCurveBezier &GetCurve() const{ return pCurve; }
	
	/** \brief Evaluate curve at the given position. */
	float EvaluateAt( float x ) const;
	
	/** \brief Test if a value is inside the valid range. */
	inline bool ValidValue( double value ) const{ return value >= -0.000001 && value <= 1.000001; }
	
	/** \brief Test if a value is outside the valid range. */
	inline bool InvalidValue( double value ) const{ return value < -0.000001 || value > 1.000001; }
	/*@}*/
};

#endif
