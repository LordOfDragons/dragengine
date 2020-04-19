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
#ifndef _DEOGLEDGEFINDEREDGE_H_
#define _DEOGLEDGEFINDEREDGE_H_

// includes
#include "dragengine/common/math/decMath.h"

// definitions

// predefintions



// opengl edge finder edge
//
// for 10k edges this uses 200KB of RAM
class deoglEdgeFinderEdge{
private:
	int pVertex[ 2 ];
	int pFace[ 2 ];
	bool pCrease;
public:
	// constructor, destructor
	deoglEdgeFinderEdge();
	~deoglEdgeFinderEdge();
	// management
	inline int GetVertex1() const{ return pVertex[ 0 ]; }
	void SetVertex1( int index );
	inline int GetVertex2() const{ return pVertex[ 1 ]; }
	void SetVertex2( int index );
	inline int GetFace1() const{ return pFace[ 0 ]; }
	void SetFace1( int index );
	inline int GetFace2() const{ return pFace[ 1 ]; }
	void SetFace2( int index );
	inline bool GetCrease() const{ return pCrease; }
	void SetCrease( bool crease );
	bool Matches( int vertex1, int vertex2 ) const;
	void Clear( int vertex1, int vertex2, int face1 );
};

// end of include only once
#endif
