/* 
 * Drag[en]gine OpenGL Graphic Module
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
#ifndef _DEOGLCONVEXFACECLIPPER_H_
#define _DEOGLCONVEXFACECLIPPER_H_

// includes
#include <dragengine/common/math/decMath.h>



/**
 * Convex Face Clipper.
 *
 * Stores a convex face and allows to clip it with various planes.
 */
class deoglConvexFaceClipper{
private:
	decDVector *pVertices;
	int pVertexCount;
	decDVector pNormal;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates an empty convex volume face. */
	deoglConvexFaceClipper();
	/** Cleans up the convex volume face. */
	~deoglConvexFaceClipper();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the face normal. */
	inline const decDVector &GetNormal() const{ return pNormal; }
	/** Sets the face normal. */
	void SetNormal( const decDVector &normal );
	
	/** Retrieves the number of vertices. */
	inline int GetVertexCount() const{ return pVertexCount; }
	/** Retrieves the the vertex at the given position. */
	const decDVector &GetVertexAt( int position ) const;
	/** Determines if the given vertex exists. */
	bool HasVertex( const decDVector &vertex ) const;
	/** Retrieves the index of the given vertex or -1 if not found. */
	int IndexOfVertex( const decDVector &vertex ) const;
	/** Adds a vertex. */
	void AddVertex( const decDVector &vertex );
	/** Removes all vertices. */
	void RemoveAllVertices();
	
	/** Clips the face by the given plane. */
	void ClipByPlane( const decDVector &planeNormal, const decDVector &planePosition );
	/** Transforms the face using a matrix. */
	void Transform( const decDMatrix &matrix );
	/*@}*/
};

// end of include only once
#endif
