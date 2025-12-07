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

#ifndef _DEOGLDECALMESHBUILDERFACE_H_
#define _DEOGLDECALMESHBUILDERFACE_H_

#include <dragengine/common/math/decMath.h>



/**
 * OpenGL decal mesh builder face.
 */
class deoglDecalMeshBuilderFace{
public:
	int pPoints[3];
	decVector pNormal;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create decal mesh builder face. */
	deoglDecalMeshBuilderFace();
	
	/** Clean up decal mesh builder face. */
	~deoglDecalMeshBuilderFace();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Point by index. */
	int GetPointAt(int index) const;
	
	/** First point. */
	inline int GetPoint1() const{return pPoints[0];}
	
	/** Set first point. */
	void SetPoint1(int point);
	
	/** Second point. */
	inline int GetPoint2() const{return pPoints[1];}
	
	/** Set second point. */
	void SetPoint2(int point);
	
	/** Third point. */
	inline int GetPoint3() const{return pPoints[2];}
	
	/** Set third point. */
	void SetPoint3(int point);
	
	/** Face normal. */
	inline const decVector &GetFaceNormal() const{return pNormal;}
	
	/** Set face normal. */
	void SetFaceNormal(const decVector &normal);
	/*@}*/
};

// end of include only once
#endif
