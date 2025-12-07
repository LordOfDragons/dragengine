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

#ifndef _DEOALRAYCACHERAYHIT_H_
#define _DEOALRAYCACHERAYHIT_H_

#include <dragengine/common/math/decMath.h>



/**
 * \brief Model ray cache ray hit.
 */
class deoalRayCacheRayHit{
	float pDistance;
	int pFaceIndex;
	bool pForwardFacing;
	decVector pPoint;
	decVector pNormal;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create model ray cache ray hit. */
	deoalRayCacheRayHit();
	
	/** \brief Clean up model ray cache ray hit. */
	~deoalRayCacheRayHit();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Distance along ray to hit point. */
	inline float GetDistance() const{ return pDistance; }
	
	/** \brief Set distance along ray to hit point. */
	void SetDistance(float distance);
	
	/** \brief Index of hit face. */
	inline int GetFaceIndex() const{ return pFaceIndex; }
	
	/** \brief Set index of hit face. */
	void SetFaceIndex(int faceIndex);
	
	/** \brief Forward facing. */
	inline bool GetForwardFacing() const{ return pForwardFacing; }
	
	/** \brief Set forward facing. */
	void SetForwardFacing(bool forwardFacing);
	
	/** \brief Hit point. */
	inline const decVector &GetPoint() const{ return pPoint; }
	
	/** \brief Set hit point. */
	void SetPoint(const decVector &point);
	
	/** \brief Normal. */
	inline const decVector &GetNormal() const{ return pNormal; }
	
	/** \brief Set normal. */
	void SetNormal(const decVector &normal);
	/*@}*/
};

#endif
