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

#ifndef _DEDAINAVMESHBUILDER_H_
#define _DEDAINAVMESHBUILDER_H_

#include <dragengine/common/collection/decPointerSet.h>

class dedaiNavMeshBuilderEdge;
class dedaiNavMeshBuilderFace;



/**
 * @brief Navigation Mesh Builder.
 */
class dedaiNavMeshBuilder{
private:
	decPointerSet pEdges;
	decPointerSet pFaces;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new navigation mesh builder. */
	dedaiNavMeshBuilder();
	/** Cleans up the navigation mesh builder. */
	~dedaiNavMeshBuilder();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of edges. */
	inline int GetEdgeCount() const{ return pEdges.GetCount(); }
	/** Retrieves an edge. */
	inline dedaiNavMeshBuilderEdge *GetEdgeAt(int index) const{ return (dedaiNavMeshBuilderEdge*)pEdges.GetAt(index); }
	/** Retrieves the matching edge creating it if not existing. */
	dedaiNavMeshBuilderEdge *GetEdgeWith(int vertex1, int vertex2);
	
	/** Retrieves the number of faces. */
	inline int GetFaceCount() const{ return pFaces.GetCount(); }
	/** Retrieves an face. */
	inline dedaiNavMeshBuilderFace *GetFaceAt(int index) const{ return (dedaiNavMeshBuilderFace*)pFaces.GetAt(index); }
	/** Adds a new face. */
	dedaiNavMeshBuilderFace *AddFace();
	
	/** Update indices. */
	void UpdateIndices();
	/** Clear. */
	void Clear();
	/*@}*/
};

#endif
