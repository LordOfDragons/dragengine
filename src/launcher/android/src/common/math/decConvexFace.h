/* 
 * Drag[en]gine Android Launcher
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
