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
