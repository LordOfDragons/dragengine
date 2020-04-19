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

#ifndef _DECSHAPEHULL_H_
#define _DECSHAPEHULL_H_

#include "decShape.h"
#include "../math/decMath.h"


/**
 * \brief Analytic Hull Shape.
 * 
 * Defines an analytic, convex hull shape. Hull shapes are defined by a list of points and
 * an orientation. The final shape is the convex hull around these points. To use this shape
 * type create an instance then set the point count and fill in the points. Do not alter the
 * points after the shape has been added to a collider.
 */
class decShapeHull : public decShape{
private:
	decVector *pPoints;
	int pPointCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create hull shape. */
	decShapeHull();
	
	/** \brief Create hull shape. */
	decShapeHull( const decVector &position );
	
	/** \brief Create hull shape. */
	decShapeHull( const decVector &position, const decQuaternion &orientation );
	
	/** \brief Clean up hull shape. */
	virtual ~decShapeHull();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Point arrays. */
	inline decVector *GetPoints() const{ return pPoints; }
	
	/** \brief Number of points. */
	inline int GetPointCount() const{ return pPointCount; }
	
	/**
	 * \brief Set number of points.
	 * 
	 * Content is undefined if the number of points changed. If the number of
	 * points is the same the content stays the same.
	 * 
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void SetPointCount( int count );
	
	/**
	 * \brief Point at index.
	 * \throws deeOutOfBoundary \em count is less than 0 or greater than or equal to GetPointCount().
	 */
	const decVector &GetPointAt( int index ) const;
	
	/**
	 * \brief Set point at index.
	 * \throws deeOutOfBoundary \em count is less than 0 or greater than or equal to GetPointCount().
	 */
	void SetPointAt( int index, const decVector &point );
	
	
	
	/** \brief Create copy of shape. */
	virtual decShape *Copy() const;
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit shape. */
	virtual void Visit( decShapeVisitor &visitor );
	/*@}*/
};

#endif
