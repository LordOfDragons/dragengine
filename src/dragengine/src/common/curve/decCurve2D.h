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
	decCurve2D(const decCurve2D &curve);
	
	/** \brief Clean up the curve. */
	~decCurve2D();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of points. */
	inline int GetPointCount() const{return pPointCount;}
	
	/**
	 * \brief Point at position.
	 * \throws deeInvalidParam \em position is less than 0.
	 * \throws deeInvalidParam \em position is larger or equal to GetPointCount()-1.
	 */
	const decVector2 &GetPointAt(int position) const;
	
	/**
	 * \brief Index of the point closest to an x coordinate inside a margin.
	 * 
	 * If no such point can be found -1 is returned.
	 */
	int IndexOfPointClosestTo(float coordinate, float threshold = CURVE_THRESHOLD) const;
	
	/**
	 * \brief Index of the point with matching x coordinates or closest on the smaller side.
	 * 
	 * This method can be used to find the interval a point is located in by returning the
	 * most prior point. Retrieves -1 if there are no points satisfying this criteria.
	 */
	int IndexOfPointBefore(float coordinate) const;
	
	/** \brief Add a point returning the index where it has been added. */
	int AddPoint(const decVector2 &point, float threshold = CURVE_THRESHOLD);
	
	/**
	 * \brief Remove a point.
	 * \throws deeInvalidParam \em position is less than 0.
	 * \throws deeInvalidParam \em position is larger or equal to GetPointCount()-1.
	 */
	void RemovePointFrom(int position);
	
	/** \brief Remove all points. */
	void RemoveAllPoints();
	
	/** \brief Clear the curve adding the points (0,0) and (1,1). */
	void SetDefaultCurve();
	
	/** \brief Offset the Y coordinate of all curve points. */
	void OffsetPointsBy(float offset);
	
	/** \brief Scale the Y coordinate of all curve points. */
	void ScalePointsBy(float scale);
	
	/** \brief Evaluate curve using constant interpolation. */
	float EvaluateConstant(float coordinate) const;
	
	/** \brief Evaluate curve using linear interpolation. */
	float EvaluateLinear(float coordinate) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy another curve to this curve. */
	decCurve2D &operator=(const decCurve2D &curve);
	/*@}*/
};

#endif
