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

#ifndef _DEOGLVISCOLLECTOCCMESHES_H_
#define _DEOGLVISCOLLECTOCCMESHES_H_

#include <dragengine/common/math/decMath.h>

#include "../../world/deoglWorldOctreeVisitor.h"

class deoglTriangleSorter;
class deoglRComponent;



/**
 * Visibility Collect Occlusion Meshes Visitor.
 * Visits components in the world containing occlusion meshes adding them to a triangle sorter
 * for later cropping convex volume lists with it.
 */
class deoglVisCollectOccMeshes : public deoglWorldOctreeVisitor{
private:
	decDVector pVisitMinExtend;
	decDVector pVisitMaxExtend;
	decDMatrix pMatrixInvHull;
	deoglTriangleSorter *pTriangleSorter;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new visitor. */
	deoglVisCollectOccMeshes();
	/** Cleans up the visitor. */
	virtual ~deoglVisCollectOccMeshes();
	/*@}*/
	
	/** \name Visiting */
	/*@{*/
	/** Retrieves the minimum extend of the box used to visit the occlusion meshes. */
	inline const decDVector &GetVisitMinExtend() const{return pVisitMinExtend;}
	/** Retrieves the maximum extend of the box used to visit the occlusion meshes. */
	inline const decDVector &GetVisitMaxExtend() const{return pVisitMaxExtend;}
	/** Sets the extends of the box used to visit the occlusion meshes. */
	void SetVisitExtends(const decDVector &minExtend, const decDVector &maxExtend);
	/** Retrieves the inverse matrix hull to transfor the occlusion meshes into the hull space. */
	inline const decDMatrix &GetMatrixInvHull() const{return pMatrixInvHull;}
	/** Sets the inverse matrix hull to transfor the occlusion meshes into the hull space. */
	void SetMatrixInvHull(const decDMatrix &matrixInvHull);
	
	/** Retrieves the triangle sorter. */
	inline deoglTriangleSorter *GetTriangleSorter() const{return pTriangleSorter;}
	
	/** Reset the visitor clearing the triangle sorter. */
	void Reset();
	
	/** Visits an octree node. */
	virtual void VisitNode(deoglDOctree *node, int intersection);
	
	/** Add occlusion mesh to triangle sorter. */
	void AddOcclusionMesh(deoglRComponent &component);
	/*@}*/
};

#endif
