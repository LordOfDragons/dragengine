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

#ifndef _DECCURVEBEZIER3D_H_
#define _DECCURVEBEZIER3D_H_

#include "../math/decMath.h"

class decCurveBezier3DPoint;


/**
 * \brief 3D Bezier Curve.
 */
class DE_DLL_EXPORT decCurveBezier3D{
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
