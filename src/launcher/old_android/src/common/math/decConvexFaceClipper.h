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
#ifndef _DECCONVEXFACECLIPPER_H_
#define _DECCONVEXFACECLIPPER_H_

// includes
#include "decMath.h"



/**
 * @brief Convex Face Clipper.
 *
 * Stores a convex face and allows to clip it with various planes.
 */
class decConvexFaceClipper{
private:
	decDVector *pVertices;
	int pVertexCount;
	decDVector pNormal;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates an empty convex volume face. */
	decConvexFaceClipper();
	/** Cleans up the convex volume face. */
	~decConvexFaceClipper();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the face normal. */
	inline const decDVector &GetNormal() const{return pNormal;}
	/** Sets the face normal. */
	void SetNormal(const decDVector &normal);
	
	/** Retrieves the number of vertices. */
	inline int GetVertexCount() const{return pVertexCount;}
	/** Retrieves the the vertex at the given position. */
	const decDVector &GetVertexAt(int position) const;
	/** Determines if the given vertex exists. */
	bool HasVertex(const decDVector &vertex) const;
	/** Retrieves the index of the given vertex or -1 if not found. */
	int IndexOfVertex(const decDVector &vertex) const;
	/** Adds a vertex. */
	void AddVertex(const decDVector &vertex);
	/** Removes all vertices. */
	void RemoveAllVertices();
	
	/** Clips the face by the given plane. */
	void ClipByPlane(const decDVector &planeNormal, const decDVector &planePosition);
	/** Transforms the face using a matrix. */
	void Transform(const decDMatrix &matrix);
	/*@}*/
};

// end of include only once
#endif
