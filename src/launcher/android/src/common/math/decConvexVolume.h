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

// include only once
#ifndef _DECCONVEXVOLUME_H_
#define _DECCONVEXVOLUME_H_

// includes
#include "decMath.h"

// definitions
class decConvexVolumeFace;



/**
 * @brief Convex Volume.
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
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates an empty convex volume. */
	decConvexVolume();
	/** Cleans up the convex volume. */
	virtual ~decConvexVolume();
	/*@}*/
	
	/** @name Vertex Management */
	/*@{*/
	/** Retrieves the number of vertices. */
	inline int GetVertexCount() const{ return pVertexCount; }
	/** Retrieves the vertex at the given position. */
	const decVector &GetVertexAt( int position ) const;
	/** Determines if the given vertex exists. */
	bool HasVertex( const decVector &vertex ) const;
	/** Retrieves the index of the given vertex or -1 if not found. */
	int IndexOfVertex( const decVector &vertex ) const;
	/** Adds a vertex. */
	void AddVertex( const decVector &vertex );
	/** Removes the given vertex. */
	void RemoveVertex( int index );
	/** Removes all vertices. */
	void RemoveAllVertices();
	/*@}*/
	
	/** @name Face Management */
	/*@{*/
	/** Retrieves the number of faces. */
	inline int GetFaceCount() const{ return pFaceCount; }
	/** Retrieves the face at the given index. */
	decConvexVolumeFace *GetFaceAt( int index ) const;
	/** Determines if the given face exists. */
	bool HasFace( decConvexVolumeFace *face ) const;
	/** Retrieves the index of the given face or -1 if not found. */
	int IndexOfFace( decConvexVolumeFace *face ) const;
	/** Adds a convex face. */
	void AddFace( decConvexVolumeFace *face );
	/** Removes the given face. */
	void RemoveFace( decConvexVolumeFace *face );
	/** Removes all faces. */
	void RemoveAllFaces();
	/*@}*/
	
	/** @name Operations */
	/*@{*/
	/** Clears the volume of all vertices and faces. */
	void SetEmpty();
	/** Sets the volume to a cube with the given dimensions. */
	void SetToCube( const decVector &halfSize );
	/** Moves the volume by the given amount. */
	void Move( const decVector &direction );
	/*@}*/
};

// end of include only once
#endif
