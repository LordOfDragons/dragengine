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

#ifndef _DECCURVEBEZIER3D_H_
#define _DECCURVEBEZIER3D_H_

#include "../math/decMath.h"

class decCurveBezier3DPoint;


/**
 * \brief 3D Bezier Curve.
 */
class decCurveBezier3D{
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
	decCurveBezier3DPoint *pPoints;
	int pPointCount;
	eInterpolationModes pInterpolationMode;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create bezier curve. */
	decCurveBezier3D();
	
	/** \brief Create copy of bezier curve. */
	decCurveBezier3D( const decCurveBezier3D &copy );
	
	/** \brief Clean up bezier curve. */
	~decCurveBezier3D();
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
	decCurveBezier3DPoint &GetPointAt( int position );
	const decCurveBezier3DPoint &GetPointAt( int position ) const;
	
	/** \brief Add point. */
	void AddPoint( const decCurveBezier3DPoint &point );
	
	/**
	 * \brief Insert point.
	 * \throws deeInvalidParam \em position is less than 0.
	 * \throws deeInvalidParam \em position is larger or equal to GetPointCount().
	 */
	void InsertPoint( int position, const decCurveBezier3DPoint &point );
	
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
	 * \throws deeInvalidParam \em mode is not a member of decCurveBezier3D::eInterpolationModes.
	 */
	void SetInterpolationMode( eInterpolationModes mode );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/**
	 * \brief Point at position.
	 * \throws deeInvalidParam \em position is less than 0.
	 * \throws deeInvalidParam \em position is larger or equal to GetPointCount()-1.
	 */
	decCurveBezier3DPoint &operator[]( int position );
	const decCurveBezier3DPoint &operator[]( int position ) const;
	
	/** \brief Copy another curve to this curve. */
	decCurveBezier3D &operator=( const decCurveBezier3D &curve );
	
	/** \brief Curve is equal to another curve. */
	bool operator==( const decCurveBezier3D &curve ) const;
	/*@}*/
};

#endif
