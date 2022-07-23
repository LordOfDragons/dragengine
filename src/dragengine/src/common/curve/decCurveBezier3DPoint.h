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

#ifndef _DECCURVEBEZIER3DPOINT_H_
#define _DECCURVEBEZIER3DPOINT_H_

#include "../math/decMath.h"


/**
 * \brief 3D Bezier Curve Point.
 */
class DE_DLL_EXPORT decCurveBezier3DPoint{
private:
	decVector pPoint;
	decVector pHandle1;
	decVector pHandle2;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new bezier curve point. */
	decCurveBezier3DPoint();
	
	/** \brief Create a new bezier curve point. */
	decCurveBezier3DPoint( const decVector &point );
	
	/** \brief Create a new bezier curve point. */
	decCurveBezier3DPoint( const decVector &point, const decVector &handle1, const decVector &handle2 );
	
	/** \brief Create a new bezier curve point. */
	decCurveBezier3DPoint( const decCurveBezier3DPoint &point );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Point. */
	inline const decVector &GetPoint() const{ return pPoint; }
	
	/** \brief First handle. */
	inline const decVector &GetHandle1() const{ return pHandle1; }
	
	/** \brief Second handle. */
	inline const decVector &GetHandle2() const{ return pHandle2; }
	
	/** \brief Set point. */
	void SetPoint( const decVector &point );
	
	/** \brief Set point. */
	void SetPoint( const decVector &point, const decVector &handle1, const decVector &handle2 );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Curve point is equal to another curve point. */
	bool operator==( const decCurveBezier3DPoint &point ) const;
	
	/** \brief Curve point is not equal to another curve point. */
	bool operator!=( const decCurveBezier3DPoint &point ) const;
	/*@}*/
};

#endif
