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
class DE_DLL_EXPORT deDebugDrawerShapeFace{
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
