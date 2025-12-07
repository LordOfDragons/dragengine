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
#ifndef _DECCONVEXVOLUMEFACE_H_
#define _DECCONVEXVOLUMEFACE_H_

// includes
#include "decMath.h"

// definitions
class decConvexVolume;



/**
 * @brief Convex Volume Face.
 *
 * Defines the face of a convex volume. Such a face is also convex
 * and defined by an ordered list of vertices and a normal. The
 * vertices are oriented clockwise around the normal. To assist in
 * dealing with convex volume face creation you can add the points
 * in any order and use the SortVertices function to sort the
 * vertices in clockwise order around the normal. Often faces in a
 * convex volume are used to mark interesting locations in space.
 * To prevent having to create subclasses faces for this common task
 * a marker value is included in the default convex volume face class
 * which can be set to an integer value.
 */
class decConvexVolumeFace{
private:
	int *pVertices;
	int pVertexCount;
	decVector pNormal;
	int pMarker;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates an empty convex volume face. */
	decConvexVolumeFace();
	/** Cleans up the convex volume face. */
	virtual ~decConvexVolumeFace();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the face normal. */
	inline const decVector &GetNormal() const{return pNormal;}
	/** Sets the face normal. */
	void SetNormal(const decVector &normal);
	/** Retrieves the marker value. */
	inline int GetMarker() const{return pMarker;}
	/** Sets the marker value. */
	void SetMarker(int marker);
	
	/** Retrieves the number of vertices. */
	inline int GetVertexCount() const{return pVertexCount;}
	/** Retrieves the index of the vertex at the given position. */
	int GetVertexAt(int position) const;
	/** Determines if the given vertex exists. */
	bool HasVertex(int vertex) const;
	/** Retrieves the index of the given vertex or -1 if not found. */
	int IndexOfVertex(int vertex) const;
	/** Adds a vertex index. */
	void AddVertex(int vertex);
	/** Removes all vertices. */
	void RemoveAllVertices();
	
	/** Sorts the vertices in clockwise order around the face normal. */
	void SortVertices(const decConvexVolume &volume);
	/** Determines if the triangle is too small. */
	bool IsTooSmall(const decConvexVolume &volume) const;
	/*@}*/
};

// end of include only once
#endif
