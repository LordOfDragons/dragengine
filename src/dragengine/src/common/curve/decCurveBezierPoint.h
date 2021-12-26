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

#ifndef _DECCURVEBEZIERPOINT_H_
#define _DECCURVEBEZIERPOINT_H_

#include "../math/decMath.h"


/**
 * \brief Bezier Curve Point.
 */
class DE_DLL_EXPORT decCurveBezierPoint{
private:
	decVector2 pPoint;
	decVector2 pHandle1;
	decVector2 pHandle2;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new bezier curve point. */
	decCurveBezierPoint();
	
	/** \brief Create a new bezier curve point. */
	decCurveBezierPoint( const decVector2 &point );
	
	/** \brief Create a new bezier curve point. */
	decCurveBezierPoint( const decVector2 &point, const decVector2 &handle1, const decVector2 &handle2 );
	
	/** \brief Create a new bezier curve point. */
	decCurveBezierPoint( const decCurveBezierPoint &point );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Point. */
	inline const decVector2 &GetPoint() const{ return pPoint; }
	
	/** \brief First handle. */
	inline const decVector2 &GetHandle1() const{ return pHandle1; }
	
	/** \brief Second handle. */
	inline const decVector2 &GetHandle2() const{ return pHandle2; }
	
	/** \brief Set point. */
	void SetPoint( const decVector2 &point );
	
	/** \brief Set point. */
	void SetPoint( const decVector2 &point, const decVector2 &handle1, const decVector2 &handle2 );
	
	/** \brief Curve points are equal. */
	bool IsEqualTo( const decCurveBezierPoint &point, float threshold = VECTOR_THRESHOLD ) const;
	/*@}*/
};

#endif
