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

#ifndef _DECCONVEXFACE_H_
#define _DECCONVEXFACE_H_

#include "decMath.h"

class decConvexFaceList;



/**
 * \brief Convex face defined by an ordered list of vertices and a normal.
 * 
 * The vertices are oriented clockwise around the normal. To assist in dealing
 * with convex face creation you can add the points in any order and use the
 * SortVertices function to sort the vertices in clockwise order around the
 * normal. Often faces are used to mark interesting locations in space. To
 * prevent having to create subclasses faces for this common task a marker
 * value is included which can be set to an integer value.
 */
class decConvexFace{
private:
	int *pVertices;
	int pVertexCount;
	decVector pNormal;
	int pMarker;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty convex face. */
	decConvexFace();
	
	/** \brief Create copy of convex face. */
	decConvexFace( const decConvexFace &face );
	
	/** \brief Clean up convex face. */
	virtual ~decConvexFace();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Face normal. */
	inline const decVector &GetNormal() const{ return pNormal; }
	
	/** \brief Set face normal. */
	void SetNormal( const decVector &normal );
	
	/** \brief Marker. */
	inline int GetMarker() const{ return pMarker; }
	
	/** \brief Set marker. */
	void SetMarker( int marker );
	
	
	
	/** \brief Number of vertices. */
	inline int GetVertexCount() const{ return pVertexCount; }
	
	/** \brief Vertex at index. */
	int GetVertexAt( int index ) const;
	
	/** \brief Set vertex at index. */
	void SetVertexAt( int index, int vertex );
	
	/** \brief Vertex is present. */
	bool HasVertex( int vertex ) const;
	
	/** \brief Index of vertex or -1 if absent. */
	int IndexOfVertex( int vertex ) const;
	
	/** \brief Add vertex. */
	void AddVertex( int vertex );
	
	/** \brief Insert vertex at index. */
	void InsertVertex( int index, int vertex );
	
	/** \brief Remove vertex from index. */
	void RemoveVertexFrom( int index );
	
	/** \brief Remove all vertices. */
	void RemoveAllVertices();
	
	
	
	/** \brief Calculate the center of the face. */
	decVector CalculateCenter( const decConvexFaceList &convexFaceList ) const;
	
	/** \brief Sort vertices in clockwise order around the face normal. */
	void SortVertices( const decConvexFaceList &convexFaceList );
	
	/** \brief Triangle is too small. */
	bool IsTooSmall( const decConvexFaceList &convexFaceList ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy face. */
	virtual decConvexFace &operator=( const decConvexFace &face );
	/*@}*/
};

#endif
