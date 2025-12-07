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
#ifndef _DEBPDECALMESHBUILDERFACE_H_
#define _DEBPDECALMESHBUILDERFACE_H_

// includes
#include "dragengine/common/math/decMath.h"



/**
 * @brief Bullet Decal Mesh Builder Face.
 * Face used by the mesh builder.
 */
class debpDecalMeshBuilderFace{
public:
	int pPoints[3];
	int pFaceIndex;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new decal mesh builder face. */
	debpDecalMeshBuilderFace();
	/** Cleans up the decal mesh builder face. */
	~debpDecalMeshBuilderFace();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the first point. */
	inline int GetPoint1() const{return pPoints[0];}
	/** Sets the first point. */
	void SetPoint1(int point);
	/** Retrieves the second point. */
	inline int GetPoint2() const{return pPoints[1];}
	/** Sets the second point. */
	void SetPoint2(int point);
	/** Retrieves the thirdpoint. */
	inline int GetPoint3() const{return pPoints[2];}
	/** Sets the third point. */
	void SetPoint3(int point);
	/** Retrieves the face index. */
	inline int GetFaceIndex() const{return pFaceIndex;}
	/** Sets the face index. */
	void SetFaceIndex(int faceIndex);
	/*@}*/
};

// end of include only once
#endif
