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

#ifndef _DECCURVEBEZIER_H_
#define _DECCURVEBEZIER_H_

#include "../math/decMath.h"

class decCurveBezierPoint;


/**
 * \brief Bezier Curve.
 */
class decCurveBezier{
public:
	/** \brief Interpolation modes. */
	enum eInterpolationModes{
		/** \brief Constant interpolation. */
		eimConstant,
		
		/** \brief Linear interpolation. */
		eimLinear,
		
		/** \brief Bezier interpolation. */
		eimBezier
	};
	
	
	
private:
	decCurveBezierPoint *pPoints;
	int pPointCount;
	eInterpolationModes pInterpolationMode;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new bezier curve. */
	decCurveBezier();
	
	/** \brief Create copy of bezier curve. */
	decCurveBezier( const decCurveBezier &curve );
	
	/** \brief Clean up the bezier curve. */
	~decCurveBezier();
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
	const decCurveBezierPoint &GetPointAt( int position ) const;
	
	/**
	 * \brief Index of the point with matching x coordinates or the point closest on the smaller side.
	 * 
	 * This method can be used to find the interval a point is located in by returning the most
	 * prior point. Retrieves -1 if there are no points satisfying this criteria.
	 */
	int FindPointPriorTo( float coordinate ) const;
	
	/** \brief Add point returning the index where it has been added. */
	int AddPoint( const decCurveBezierPoint &point );
	
	/**
	 * \brief Remove point from the given position.
	 * \throws deeInvalidParam \em position is less than 0.
	 * \throws deeInvalidParam \em position is larger or equal to GetPointCount()-1.
	 */
	void RemovePointFrom( int position );
	
	/** \brief Remove all points. */
	void RemoveAllPoints();
	
	/** \brief Interpolation mode. */
	inline eInterpolationModes GetInterpolationMode() const{ return pInterpolationMode; }
	
	/**
	 * \brief Set interpolation mode.
	 * \throws deeInvalidParam \em mode is not a member of decCurveBezier::eInterpolationModes.
	 */
	void SetInterpolationMode( eInterpolationModes mode );
	
	
	
	/** \brief Set curve to linear mapping from (0,0) to (1,1). */
	void SetDefaultLinear();
	
	/** \brief Set curve to default bezier curve from (0,0) to (1,1). */
	void SetDefaultBezier();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Curves are equal. */
	bool operator==( const decCurveBezier &curve ) const;
	
	/** \brief Curves are not equal. */
	bool operator!=( const decCurveBezier &curve ) const;
	
	/** \brief Copy another curve to this curve. */
	decCurveBezier &operator=( const decCurveBezier &curve );
	/*@}*/
};

#endif
