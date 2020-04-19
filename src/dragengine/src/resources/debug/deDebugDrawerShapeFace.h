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

#ifndef _DEDEBUGDRAWERSHAPEFACE_H_
#define _DEDEBUGDRAWERSHAPEFACE_H_

#include "../../common/math/decMath.h"


/**
 * \brief Debug drawer volume face.
 *
 * Describes a face in a debug drawer shape. These faces can have any
 * number of vertices where all vertices are considered to be coplanar.
 * The face normal is specified explicity to avoid troubles in cases
 * where vertices are very close together. This way an application or
 * module wishing for some debug drawing does not have to worry about
 * badly spaced vertices which makes debug code more simple. The face
 * is considered to be a triangle fan type face. Hence the first vertex
 * is the base vertex and the second vertex the first triangle base.
 * Every next vertex defines a triangle with the first vertex, the
 * last vertex and the newly specified vertex. The triangle fan is not
 * closesed automatically. For filled shapes at least 3 vertices are
 * required and for non filled shapes at least 2. If not enough
 * vertices are specified the face not drawn.
 */
class deDebugDrawerShapeFace{
private:
	decVector *pVertices;
	int pVertexCount;
	decVector pNormal;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create debug drawer shape face. */
	deDebugDrawerShapeFace();
	
	/** \brief Clean up debug drawer shape face. */
	~deDebugDrawerShapeFace();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Face normal. */
	inline const decVector &GetNormal() const{ return pNormal; }
	
	/** \brief Set face normal. */
	void SetNormal( const decVector &normal );
	
	/**
	 * \brief Calculate face normal from points.
	 * 
	 * Does nothing if the number of points is less than 3.
	 */
	void CalculateNormal();
	
	/** \brief Number of vertices. */
	inline int GetVertexCount() const{ return pVertexCount; }
	
	/** \brief Vertex at index. */
	const decVector &GetVertexAt( int index ) const;
	
	/** \brief Set vertex at index. */
	void SetVertexAt( int index, const decVector &vertex );
	
	/** \brief Add vertex. */
	void AddVertex( const decVector &vertex );
	
	/** \brief Remove all vertices. */
	void RemoveAllVertices();
	/*@}*/
	
	
	
	/** \name Helper Functions */
	/*@{*/
	/**
	 * \brief Turn face into a properly coplanar face.
	 * 
	 * All vertices are moved to be in the plane defined by the given vertex and the face normal.
	 */
	void MakeCoplanarTo( const decVector &position );
	/*@}*/
};

#endif
