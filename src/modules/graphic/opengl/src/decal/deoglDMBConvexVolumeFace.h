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

#ifndef _DEOGLDMBCONVEXVOLUMEFACE_H_
#define _DEOGLDMBCONVEXVOLUMEFACE_H_

#include <dragengine/common/math/decConvexVolumeFace.h>
#include <dragengine/common/math/decMath.h>



/**
 * OpenGL decal mesh builder convex volume face.
 * 
 * Spezialized version of the convex volume face used by the decal mesh builder.
 */
class deoglDMBConvexVolumeFace : public decConvexVolumeFace{
public:
	bool pDecalFace;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create decal mesh builder convex volume face. */
	deoglDMBConvexVolumeFace();
	
	/** Clean up decal mesh builder convex volume face. */
	virtual ~deoglDMBConvexVolumeFace();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Decal face. */
	inline bool GetDecalFace() const{ return pDecalFace; }
	
	/** Set if decal face. */
	void SetDecalFace(bool decalFace);
	/*@}*/
};

#endif
