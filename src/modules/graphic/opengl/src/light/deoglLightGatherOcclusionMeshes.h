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

#ifndef _DEOGLLIGHTGATHEROCCLUSIONMESHES_H_
#define _DEOGLLIGHTGATHEROCCLUSIONMESHES_H_

#include "../world/deoglWorldOctreeVisitor.h"
#include "../triangles/deoglTriangleSorter.h"

class deoglRLight;
class deoglRComponent;



/**
 * Light Gather Occlusion Meshes Visitor.
 * Visits components in the world containing occlusion meshes adding them to a triangle sorter
 * for later cropping light volumes with it.
 */
class deoglLightGatherOcclusionMeshes : public deoglWorldOctreeVisitor{
private:
	deoglRLight &pLight;
	deoglTriangleSorter pTriangleSorter;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new visitor. */
	deoglLightGatherOcclusionMeshes(deoglRLight &light);
	/** Cleans up the visitor. */
	virtual ~deoglLightGatherOcclusionMeshes();
	/*@}*/
	
	/** \name Visiting */
	/*@{*/
	/** Retrieves the triangle sorter. */
	inline deoglTriangleSorter &GetTriangleSorter(){ return pTriangleSorter; }
	inline const deoglTriangleSorter &GetTriangleSorter() const{ return pTriangleSorter; }
	
	/** Reset the visitor clearing the triangle sorter. */
	void Reset();
	
	/** Visits an octree node. */
	virtual void VisitNode(deoglDOctree *node, int intersection);
	
	/** Add occlusion mesh to triangle sorter. */
	void AddOcclusionMesh(deoglRComponent &component);
	/*@}*/
};

#endif
