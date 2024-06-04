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

#ifndef _DEOGLMODELLODVISITOR_H_
#define _DEOGLMODELLODVISITOR_H_

#include <dragengine/common/math/decMath.h>

#include "octree/deoglModelOctreeVisitor.h"

class deoglModelLOD;



/**
 * Model LOD Visitor.
 * Model visitor calculating the minimum distance of a vertex faces in a model. Only faces
 * inside a small box around the vertex are tested for speed reasons.
 */
class deoglModelLODVisitor : public deoglModelOctreeVisitor{
private:
	deoglModelLOD &pModelLOD;
	decVector pBoxMinExtend;
	decVector pBoxMaxExtend;
	decVector pLODPoint;
	
	float pMinLODDistance;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new model octree visitor. */
	deoglModelLODVisitor( deoglModelLOD &modelLOD );
	/** Cleans up the model octree visitor. */
	virtual ~deoglModelLODVisitor();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the box minimum extend. */
	inline const decVector &GetBoxMinExtend() const{ return pBoxMinExtend; }
	/** Retrieves the box maximum extend. */
	inline const decVector &GetBoxMaxExtend() const{ return pBoxMaxExtend; }
	/** Sets the box extends. */
	void SetBoxExtends( const decVector &minExtend, const decVector &maxExtend );
	/** Retrieves the lod point. */
	inline const decVector &GetLODPoint() const{ return pLODPoint; }
	/** Sets the lod point. */
	void SetLODPoint( const decVector &lodPoint );
	
	/** Retrieves the minimum lod distance. */
	inline float GetMinLODDistance() const{ return pMinLODDistance; }
	/** Sets the minimum lod distance. */
	void SetMinLODDistance( float distance );
	/*@}*/
	
	/** \name Visiting */
	/*@{*/
	/**
	 * Visits an octree node. The default implementation is to visit all
	 * world elements stored in the node.
	 */
	virtual void VisitNode( deoglOctree *node, int intersection );
	/*@}*/
};

#endif
