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
#ifndef _DEOGLDEPTHTRIANGLELIST_H_
#define _DEOGLDEPTHTRIANGLELIST_H_

// includes
#include "../deoglBasics.h"

#include <dragengine/common/math/decMath.h>

// predefinitions



/**
 * @brief Depth Triangle List.
 *
 * Stores a list of depth triangles. The triangles are stored only with
 * there position and no texture information. Useful for shadow casting
 * algorithms which require a list of triangles potentially requiring
 * to split them. A split method is provided for adding a triangle using
 * the maximum edge length as split criteria. The triangles are kept in
 * a continuous list of vertiex-tripples so they can be copied directly
 * into a VBO if required. The winding of the triangles matches the
 * opengl triangle orientation.
 */
class deoglDepthTriangleList{
private:
	oglVector3 *pPoints;
	int pPointCount;
	int pPointSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new depth triangle list. */
	deoglDepthTriangleList();
	/** Cleans up the depth triangle list. */
	~deoglDepthTriangleList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of points. */
	inline int GetPointCount() const{ return pPointCount; }
	/** Retrieves the points. */
	inline oglVector3 *GetPoints() const{ return pPoints; }
	/** Adds a face with the given points. */
	void AddTriangle( const decVector &p1, const decVector &p2, const decVector &p3 );
	/**
	 * Adds a triangle using the given edge length as split criteria. If the
	 * maximum length of one of the triangle edges is larger than the
	 * thresold the triangle is split along this edge and the two new
	 * triangles send down this function recursively.
	 */
	void AddTriangle( const decVector &p1, const decVector &p2, const decVector &p3, float maxEdgeLength );
	/**
	 * Adds a triangle using a dynamic spherical splitting algorithm.
	 * The points of the triangle are first projected to the sphere
	 * with the radius of the triangle point furthest away. Then the
	 * triangle center is projected onto this sphere too testing the
	 * distance to the actual triangle center. If the difference is
	 * larger than the given threshold the triangle is split into
	 * tree child triangles send down this function recursively. For
	 * simplicity the sphere is assumed to be located at the origin
	 * with the given radius.
	 */
	void AddTriangleSpherical( const decVector &p1, const decVector &p2, const decVector &p3, float threshold );
	/** Removes all triangles. */
	void RemoveAllTriangles();
	/** Contracts the list to have the same size as there are elements. */
	void Contract();
	/*@}*/
	
private:
	/**
	 * Modified version of the AddTriangleSpherical call. Using brute
	 * force the length of each triangle point is calculated multiple
	 * times which is a waste of CPU time since the length of triangle
	 * points after the split is the same as before so handing the
	 * distance alongside spares us from calculating them repeatetly.
	 */
	void pAddTriangleSpherical( const decVector &p1, const decVector &pp1, const decVector &p2, const decVector &pp2,
	const decVector &p3, const decVector &pp3, float threshold );
};

// end of include only once
#endif
