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
#ifndef _DEOGLEDGEFINDER_H_
#define _DEOGLEDGEFINDER_H_

// includes
#include "dragengine/common/math/decMath.h"

// definitions

// predefintions
class deoglEdgeFinderVertex;
class deoglEdgeFinderEdge;
class deoglEdgeFinderFace;



// opengl edge finder
class deoglEdgeFinder{
private:
	deoglEdgeFinderVertex **pVertices;
	int pVertexCount, pVertexSize;
	deoglEdgeFinderEdge **pEdges;
	int pEdgeCount, pEdgeSize;
	deoglEdgeFinderFace **pFaces;
	int pFaceCount, pFaceSize;
public:
	// constructor, destructor
	deoglEdgeFinder();
	~deoglEdgeFinder();
	// management
	void Clear( int vertexCount );
	void CalculateEdges();
	// vertex management
	inline int GetVertexCount() const{ return pVertexCount; }
	deoglEdgeFinderVertex *GetVertexAt( int index ) const;
	// edge management
	inline int GetEdgeCount() const{ return pEdgeCount; }
	deoglEdgeFinderEdge *GetEdgeAt( int index ) const;
	// face management
	inline int GetFaceCount() const{ return pFaceCount; }
	deoglEdgeFinderFace *GetFaceAt( int index ) const;
	void AddFace( int vertex1, int vertex2, int vertex3 );
private:
	void pCleanUp();
	void pAddEdge( int vertex1, int vertex2, int face1 );
};

// end of include only once
#endif
