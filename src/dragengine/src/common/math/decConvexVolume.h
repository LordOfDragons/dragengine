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
class DE_DLL_EXPORT decConvexVolume{
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
	inline int GetVertexCount() const{return pVertexCount;}
	
	/** \brief Vertex at the given position. */
	const decVector &GetVertexAt(int position) const;
	
	/** \brief Given vertex exists. */
	bool HasVertex(const decVector &vertex) const;
	
	/** \brief Index of the given vertex or -1 if not found. */
	int IndexOfVertex(const decVector &vertex) const;
	
	/** \brief Adds a vertex. */
	void AddVertex(const decVector &vertex);
	
	/** \brief Removes the given vertex. */
	void RemoveVertex(int index);
	
	/** \brief Removes all vertices. */
	void RemoveAllVertices();
	/*@}*/
	
	
	
	/** \name Face Management */
	/*@{*/
	/** \brief Number of faces. */
	inline int GetFaceCount() const{return pFaceCount;}
	
	/** \brief Face at the given index. */
	decConvexVolumeFace *GetFaceAt(int index) const;
	
	/** \brief Given face exists. */
	bool HasFace(decConvexVolumeFace *face) const;
	
	/** \brief Index of the given face or -1 if not found. */
	int IndexOfFace(decConvexVolumeFace *face) const;
	
	/** \brief Adds a convex face. */
	void AddFace(decConvexVolumeFace *face);
	
	/** \brief Removes the given face. */
	void RemoveFace(decConvexVolumeFace *face);
	
	/** \brief Removes all faces. */
	void RemoveAllFaces();
	/*@}*/
	
	
	
	/** \name Operations */
	/*@{*/
	/** \brief Clears the volume of all vertices and faces. */
	void SetEmpty();
	
	/** \brief Set volume to a cube with the given dimensions. */
	void SetToCube(const decVector &halfSize);
	
	/** \brief Moves the volume by the given amount. */
	void Move(const decVector &direction);
	/*@}*/
};

#endif
