/* 
 * Drag[en]gine Game Engine
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

#ifndef _DECCONVEXVOLUMEFACE_H_
#define _DECCONVEXVOLUMEFACE_H_

#include "decMath.h"

// definitions
class decConvexVolume;


/**
 * \brief Convex Volume Face.
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
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates an empty convex volume face. */
	decConvexVolumeFace();
	
	/** \brief Clean up convex volume face. */
	virtual ~decConvexVolumeFace();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Face normal. */
	inline const decVector &GetNormal() const{ return pNormal; }
	
	/** \brief Set face normal. */
	void SetNormal( const decVector &normal );
	
	/** \brief Marker value. */
	inline int GetMarker() const{ return pMarker; }
	
	/** \brief Set marker value. */
	void SetMarker( int marker );
	
	/** \brief Number of vertices. */
	inline int GetVertexCount() const{ return pVertexCount; }
	
	/** \brief Index of the vertex at the given position. */
	int GetVertexAt( int position ) const;
	
	/** \brief Given vertex exists. */
	bool HasVertex( int vertex ) const;
	
	/** \brief Index of the given vertex or -1 if not found. */
	int IndexOfVertex( int vertex ) const;
	
	/** \brief Adds a vertex index. */
	void AddVertex( int vertex );
	
	/** \brief Removes all vertices. */
	void RemoveAllVertices();
	
	/** \brief Sorts the vertices in clockwise order around the face normal. */
	void SortVertices( const decConvexVolume &volume );
	
	/** \brief Triangle is too small. */
	bool IsTooSmall( const decConvexVolume &volume ) const;
	/*@}*/
};

#endif
