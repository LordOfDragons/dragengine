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

#ifndef _DEOGLMOVCLOSESTFACE_H_
#define _DEOGLMOVCLOSESTFACE_H_

#include <dragengine/common/math/decMath.h>

#include "../deoglModelOctreeVisitor.h"

class deoglModelLOD;


/**
 * Model octree visitor retrieving the closest face to a point.
 * 
 * Visits the octree with a box enclosing a maximum search radius. The result
 * is the index of the face or -1 if no face could be found with a distance
 * less then or equal to the search radius.
 * 
 * \note Before using the visitor you have to call deoglModelLOD::PrepareOctree()
 *       to ensure the octree is valid before visiting.
 */
class deoglMOVClosestFace : public deoglModelOctreeVisitor{
private:
	const deoglModelLOD &pModelLOD;
	decVector pTestPoint;
	float pMaxRadius;
	
	int pFaceIndex;
	float pFaceDistanceSquared;
	
	decVector pTestBoxMin;
	decVector pTestBoxMax;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new model octree visitor. */
	deoglMOVClosestFace(const deoglModelLOD &modelLOD);
	/** Cleans up the model octree visitor. */
	virtual ~deoglMOVClosestFace();
	/*@}*/
	
	/** \name Visiting */
	/*@{*/
	/** Retrieves the test point. */
	inline const decVector &GetTestPoint() const{return pTestPoint;}
	/** Retrieves the maximum search distance. */
	inline float GetMaxRadius() const{return pMaxRadius;}
	/** Sets the test point and maximum search radius. */
	void SetTestPoint(const decVector &testPoint, float maxRadius);
	
	/**
	 * Resets the visitor.
	 * \details Sets the found face index to -1.
	 */
	void Reset();
	
	/** Retrieves the face index or -1 if not found. */
	inline int GetFaceIndex() const{return pFaceIndex;}
	
	/** Convenience method to visit stored model lod returning the found face or -1 if not found. */
	int FindClosestFace(const decVector &testPoint, float maxRadius);
	
	/** Visits an octree node. */
	virtual void VisitNode(deoglOctree *node, int intersection);
	/*@}*/
};

#endif
