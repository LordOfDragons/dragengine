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
