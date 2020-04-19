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

#ifndef _DECCONVEXVOLUME_H_
#define _DECCONVEXVOLUME_H_

#include "decMath.h"

// definitions
class decConvexVolumeFace;


/**
 * \brief Convex Volume.
 *
 * Defines a convex volume. Such volumes are composed of convex
 * faces of arbitrary vertex count. Convex volumes can be split
 * up using planes.
 */
class decConvexVolume{
private:
	decVector *pVertices;
	int pVertexCount;
	int pVertexSize;
	decConvexVolumeFace **pFaces;
	int pFaceCount;
	int pFaceSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates an empty convex volume. */
	decConvexVolume();
	
	/** \brief Clean up convex volume. */
	virtual ~decConvexVolume();
	/*@}*/
	
	
	
	/** \name Vertex Management */
	/*@{*/
	/** \brief Number of vertices. */
	inline int GetVertexCount() const{ return pVertexCount; }
	
	/** \brief Vertex at the given position. */
	const decVector &GetVertexAt( int position ) const;
	
	/** \brief Given vertex exists. */
	bool HasVertex( const decVector &vertex ) const;
	
	/** \brief Index of the given vertex or -1 if not found. */
	int IndexOfVertex( const decVector &vertex ) const;
	
	/** \brief Adds a vertex. */
	void AddVertex( const decVector &vertex );
	
	/** \brief Removes the given vertex. */
	void RemoveVertex( int index );
	
	/** \brief Removes all vertices. */
	void RemoveAllVertices();
	/*@}*/
	
	
	
	/** \name Face Management */
	/*@{*/
	/** \brief Number of faces. */
	inline int GetFaceCount() const{ return pFaceCount; }
	
	/** \brief Face at the given index. */
	decConvexVolumeFace *GetFaceAt( int index ) const;
	
	/** \brief Given face exists. */
	bool HasFace( decConvexVolumeFace *face ) const;
	
	/** \brief Index of the given face or -1 if not found. */
	int IndexOfFace( decConvexVolumeFace *face ) const;
	
	/** \brief Adds a convex face. */
	void AddFace( decConvexVolumeFace *face );
	
	/** \brief Removes the given face. */
	void RemoveFace( decConvexVolumeFace *face );
	
	/** \brief Removes all faces. */
	void RemoveAllFaces();
	/*@}*/
	
	
	
	/** \name Operations */
	/*@{*/
	/** \brief Clears the volume of all vertices and faces. */
	void SetEmpty();
	
	/** \brief Set volume to a cube with the given dimensions. */
	void SetToCube( const decVector &halfSize );
	
	/** \brief Moves the volume by the given amount. */
	void Move( const decVector &direction );
	/*@}*/
};

#endif
