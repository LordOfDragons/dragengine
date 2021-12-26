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

#ifndef _DECCURVE_H_
#define _DECCURVE_H_

#include "../math/decMath.h"


/**
 * \brief 2D Curve.
 *
 * Defines a curve in 2 dimensions. The curve is defined using
 * a set of points. Points inserted are sorted using the X
 * coordinate. Points which are identical are merged with the
 * newly added point overwriting the old point. Two evaluation
 * methods are provided to evaluate the curve using either
 * constant or linear interpolation. Using constant
 * interpolation the value of a curve point corresponds to the
 * Y coordinate of the point right prior to it. Linear
 * interpolation slides the Y value gradually between points.
 * For cubic interpolation an additional class is used since
 * this requires storing point parameters for evaluation and
 * doing this for each evaluation of a point is slow.
 */
class DE_DLL_EXPORT decCurve2D{
private:
	decVector2 *pPoints;
	int pPointCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new curve. */
	decCurve2D();
	
	/** \brief Create copy of a curve. */
	decCurve2D( const decCurve2D &curve );
	
	/** \brief Clean up the curve. */
	~decCurve2D();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of points. */
	inline int GetPointCount() const{ return pPointCount; }
	
	/**
	 * \brief Point at position.
	 * \throws deeInvalidParam \em position is less than 0.
	 * \throws deeInvalidParam \em position is larger or equal to GetPointCount()-1.
	 */
	const decVector2 &GetPointAt( int position ) const;
	
	/**
	 * \brief Index of the point closest to an x coordinate inside a margin.
	 * 
	 * If no such point can be found -1 is returned.
	 */
	int IndexOfPointClosestTo( float coordinate, float threshold = CURVE_THRESHOLD ) const;
	
	/**
	 * \brief Index of the point with matching x coordinates or closest on the smaller side.
	 * 
	 * This method can be used to find the interval a point is located in by returning the
	 * most prior point. Retrieves -1 if there are no points satisfying this criteria.
	 */
	int IndexOfPointBefore( float coordinate ) const;
	
	/** \brief Add a point returning the index where it has been added. */
	int AddPoint( const decVector2 &point, float threshold = CURVE_THRESHOLD );
	
	/**
	 * \brief Remove a point.
	 * \throws deeInvalidParam \em position is less than 0.
	 * \throws deeInvalidParam \em position is larger or equal to GetPointCount()-1.
	 */
	void RemovePointFrom( int position );
	
	/** \brief Remove all points. */
	void RemoveAllPoints();
	
	/** \brief Clear the curve adding the points (0,0) and (1,1). */
	void SetDefaultCurve();
	
	/** \brief Offset the Y coordinate of all curve points. */
	void OffsetPointsBy( float offset );
	
	/** \brief Scale the Y coordinate of all curve points. */
	void ScalePointsBy( float scale );
	
	/** \brief Evaluate curve using constant interpolation. */
	float EvaluateConstant( float coordinate ) const;
	
	/** \brief Evaluate curve using linear interpolation. */
	float EvaluateLinear( float coordinate ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy another curve to this curve. */
	decCurve2D &operator=( const decCurve2D &curve );
	/*@}*/
};

#endif
