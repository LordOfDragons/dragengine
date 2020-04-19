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
#ifndef _DEOGLEDGEFINDERVERTEX_H_
#define _DEOGLEDGEFINDERVERTEX_H_

// includes
#include "dragengine/common/math/decMath.h"

// definitions

// predefintions



// opengl edge finder vertex
//
// for 10k vertices this uses roughly 1MB of RAM
class deoglEdgeFinderVertex{
private:
	int *pEdges;
	int pEdgeCount, pEdgeSize;
	int *pFaces;
	int pFaceCount, pFaceSize;
public:
	// constructor, destructor
	deoglEdgeFinderVertex();
	~deoglEdgeFinderVertex();
	// management
	void Clear();
	// edges
	inline int GetEdgeCount() const{ return pEdgeCount; }
	int GetEdgeAt( int index ) const;
	bool HasEdge( int index ) const;
	void AddEdge( int index );
	void RemoveAllEdges();
	// faces
	inline int GetFaceCount() const{ return pFaceCount; }
	int GetFaceAt( int index ) const;
	bool HasFace( int index ) const;
	void AddFace( int index );
	void RemoveAllFaces();
private:
	void pCleanUp();
};

// end of include only once
#endif
