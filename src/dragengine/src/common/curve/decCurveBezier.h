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

#ifndef _DECCURVEBEZIER_H_
#define _DECCURVEBEZIER_H_

#include "../math/decMath.h"

class decCurveBezierPoint;


/**
 * \brief Bezier Curve.
 */
class DE_DLL_EXPORT decCurveBezier{
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
	decCurveBezier(const decCurveBezier &curve);
	
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
	const decCurveBezierPoint &GetPointAt(int position) const;
	
	/**
	 * \brief Index of the point with matching x coordinates or the point closest on the smaller side.
	 * 
	 * This method can be used to find the interval a point is located in by returning the most
	 * prior point. Retrieves -1 if there are no points satisfying this criteria.
	 */
	int FindPointPriorTo(float coordinate) const;
	
	/** \brief Add point returning the index where it has been added. */
	int AddPoint(const decCurveBezierPoint &point);
	
	/**
	 * \brief Remove point from the given position.
	 * \throws deeInvalidParam \em position is less than 0.
	 * \throws deeInvalidParam \em position is larger or equal to GetPointCount()-1.
	 */
	void RemovePointFrom(int position);
	
	/** \brief Remove all points. */
	void RemoveAllPoints();
	
	/** \brief Interpolation mode. */
	inline eInterpolationModes GetInterpolationMode() const{ return pInterpolationMode; }
	
	/**
	 * \brief Set interpolation mode.
	 * \throws deeInvalidParam \em mode is not a member of decCurveBezier::eInterpolationModes.
	 */
	void SetInterpolationMode(eInterpolationModes mode);
	
	
	
	/** \brief Set curve to linear mapping from (0,0) to (1,1). */
	void SetDefaultLinear();
	
	/** \brief Set curve to default bezier curve from (0,0) to (1,1). */
	void SetDefaultBezier();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Curves are equal. */
	bool operator==(const decCurveBezier &curve) const;
	
	/** \brief Curves are not equal. */
	bool operator!=(const decCurveBezier &curve) const;
	
	/** \brief Copy another curve to this curve. */
	decCurveBezier &operator=(const decCurveBezier &curve);
	/*@}*/
};

#endif
