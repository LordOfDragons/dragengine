/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
class DE_DLL_EXPORT decShapeHull : public decShape{
private:
	decVector *pPoints;
	int pPointCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create hull shape. */
	decShapeHull();
	
	/** \brief Create hull shape. */
	decShapeHull(const decVector &position);
	
	/** \brief Create hull shape. */
	decShapeHull(const decVector &position, const decQuaternion &orientation);
	
	/** \brief Clean up hull shape. */
	virtual ~decShapeHull();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Point arrays. */
	inline decVector *GetPoints() const{return pPoints;}
	
	/** \brief Number of points. */
	inline int GetPointCount() const{return pPointCount;}
	
	/**
	 * \brief Set number of points.
	 * 
	 * Content is undefined if the number of points changed. If the number of
	 * points is the same the content stays the same.
	 * 
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void SetPointCount(int count);
	
	/**
	 * \brief Point at index.
	 * \throws deeOutOfBoundary \em count is less than 0 or greater than or equal to GetPointCount().
	 */
	const decVector &GetPointAt(int index) const;
	
	/**
	 * \brief Set point at index.
	 * \throws deeOutOfBoundary \em count is less than 0 or greater than or equal to GetPointCount().
	 */
	void SetPointAt(int index, const decVector &point);
	
	
	
	/** \brief Create copy of shape. */
	virtual decShape *Copy() const;
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit shape. */
	virtual void Visit(decShapeVisitor &visitor);
	/*@}*/
};

#endif
