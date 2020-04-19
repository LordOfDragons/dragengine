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
#ifndef _DEOGLEDGEFINDERFACE_H_
#define _DEOGLEDGEFINDERFACE_H_

// includes
#include "dragengine/common/math/decMath.h"

// definitions

// predefintions



// opengl edge finder face
//
// for 10k faces this uses 280KB of RAM
class deoglEdgeFinderFace{
private:
	int pVertex[ 3 ];
	int pEdge[ 3 ];
public:
	// constructor, destructor
	deoglEdgeFinderFace();
	~deoglEdgeFinderFace();
	// management
	inline int GetVertex1() const{ return pVertex[ 0 ]; }
	void SetVertex1( int index );
	inline int GetVertex2() const{ return pVertex[ 1 ]; }
	void SetVertex2( int index );
	inline int GetVertex3() const{ return pVertex[ 2 ]; }
	void SetVertex3( int index );
	int GetVertexAt( int vertex ) const;
	void SetVertexAt( int vertex, int index );
	inline int GetEdge1() const{ return pEdge[ 0 ]; }
	void SetEdge1( int index );
	inline int GetEdge2() const{ return pEdge[ 1 ]; }
	void SetEdge2( int index );
	inline int GetEdge3() const{ return pEdge[ 2 ]; }
	void SetEdge3( int index );
	int GetEdgeAt( int edge ) const;
	void SetEdgeAt( int edge, int index );
	void Clear( int vertex1, int vertex2, int vertex3 );
};

// end of include only once
#endif
