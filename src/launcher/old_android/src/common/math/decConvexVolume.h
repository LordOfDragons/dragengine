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
