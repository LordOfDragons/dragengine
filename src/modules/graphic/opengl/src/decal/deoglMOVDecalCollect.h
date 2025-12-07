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

#ifndef _DEOGLMOVDECALCOLLECT_H_
#define _DEOGLMOVDECALCOLLECT_H_

#include <dragengine/common/math/decMath.h>

#include "../model/octree/deoglModelOctreeVisitor.h"

class deoglModelLOD;
class deoglTriangleSorter;



/**
 * Model octree visitor collecting triangles touching decal projection.
 * 
 * \note Before using the visitor you have to call deoglModelLOD::PrepareOctree()
 *       to ensure the octree is valid before visiting.
 */
class deoglMOVDecalCollect : public deoglModelOctreeVisitor{
private:
	const deoglModelLOD &pModelLOD;
	deoglTriangleSorter &pTriangleSorter;
	
	decVector pPlaneNormal[6];
	float pPlaneDistance[6];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create model octree visitor. */
	deoglMOVDecalCollect(const deoglModelLOD &modelLOD, deoglTriangleSorter &triangleSorter);
	
	/** Clean up model octree visitor. */
	virtual ~deoglMOVDecalCollect();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/**
	 * Visit model octree storing list in triangle sorter.
	 * 
	 * The matrix is the decal matrix (position and orientation) in model coordinate system.
	 * Faces are clipped against the range (size.x*-0.5, size.y*-0.5, -size.z) to
	 * (size.x*0.5, size.y*0.5, 0).
	 * 
	 * Faces are added to triangle sorter and sorted front to back.
	 */
	void CollectFaces(const decMatrix &matrix, const decVector &size);
	
	/** Visits an octree node. */
	virtual void VisitNode(deoglOctree *node, int intersection);
	/*@}*/
};

#endif
