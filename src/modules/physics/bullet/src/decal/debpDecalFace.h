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

// include only once
#ifndef _DEBPDECALFACE_H_
#define _DEBPDECALFACE_H_

// includes
#include "dragengine/common/math/decMath.h"



/**
 * @brief Bullet Decal Face.
 * Face of a decal.
 */
class debpDecalFace{
public:
	decVector pPoints[3];
	decVector pNormal;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new decal face. */
	debpDecalFace();
	/** Cleans up the decal face. */
	~debpDecalFace();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the face normal. */
	inline const decVector &GetNormal() const{return pNormal;}
	/** Sets the face normal. */
	void SetNormal(const decVector &normal);
	
	/** Retrieves the first point. */
	inline const decVector &GetPoint1() const{return pPoints[0];}
	/** Sets the first point. */
	void SetPoint1(const decVector &point);
	/** Retrieves the second point. */
	inline const decVector &GetPoint2() const{return pPoints[1];}
	/** Sets the second point. */
	void SetPoint2(const decVector &point);
	/** Retrieves the third point. */
	inline const decVector &GetPoint3() const{return pPoints[2];}
	/** Sets the third point. */
	void SetPoint3(const decVector &point);
	/*@}*/
};

// end of include only once
#endif
