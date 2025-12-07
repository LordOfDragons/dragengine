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

#ifndef _DEDAIPATHFINDERFUNNEL_H_
#define _DEDAIPATHFINDERFUNNEL_H_

#include <dragengine/common/math/decMath.h>

class dedaiSpaceMesh;



/**
 * @brief Path Finder funnel.
 */
class dedaiPathFinderFunnel{
private:
	dedaiSpaceMesh *pMesh;
	decVector pOrigin;
	decVector pGoal;
	decVector pLeftCorner;
	decVector pRightCorner;
	decVector pLeftNormal;
	decVector pRightNormal;
	float pLeftDist;
	float pRightDist;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new path finder funnel. */
	dedaiPathFinderFunnel();
	/** Cleans up the path finder funnel. */
	~dedaiPathFinderFunnel();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the navigation mesh the funnel data is relative to. */
	inline dedaiSpaceMesh *GetMesh() const{ return pMesh; }
	/** Sets the navigation mesh the funnel data is relative to. */
	void SetMesh(dedaiSpaceMesh *mesh);
	/** Retrieves the origin. */
	inline const decVector &GetOrigin() const{ return pOrigin; }
	/** Sets the origin. */
	void SetOrigin(const decVector &origin);
	/** Retrieves the goal. */
	inline const decVector &GetGoal() const{ return pGoal; }
	/** Sets the goal. */
	void SetGoal(const decVector &goal);
	
	/** Retrieves the left corner. */
	inline const decVector &GetLeftCorner() const{ return pLeftCorner; }
	/** Sets the left corner. */
	void SetLeftCorner(const decVector &corner);
	/** Retrieves the left plane normal. */
	inline const decVector &GetLeftNormal() const{ return pLeftNormal; }
	/** Sets the left plane normal. */
	void SetLeftNormal(const decVector &normal);
	/** Retrieves the left plane distance. */
	inline float GetLeftDistance() const{ return pLeftDist; }
	
	/** Retrieves the right corner. */
	inline const decVector &GetRightCorner() const{ return pRightCorner; }
	/** Sets the right corner. */
	void SetRightCorner(const decVector &corner);
	/** Retrieves the right plane normal. */
	inline const decVector &GetRightNormal() const{ return pRightNormal; }
	/** Sets the right plane normal. */
	void SetRightNormal(const decVector &normal);
	/** Retrieves the right plane distance. */
	inline float GetRightDistance() const{ return pRightDist; }	
	
	/** Update left plane using the current origin and left normal. */
	void UpdateLeftPlane(const decVector &faceNormal);
	/** Update right plane using the current origin and right normal. */
	void UpdateRightPlane(const decVector &faceNormal);
	/** Transform funnel by a matrix. */
	void Transform(const decMatrix &matrix);
	/*@}*/
};

#endif
