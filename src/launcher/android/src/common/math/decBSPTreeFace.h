/* 
 * Drag[en]gine Android Launcher
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
#ifndef _DECBSPTREEFACE_H_
#define _DECBSPTREEFACE_H_

// includes
#include "decMath.h"



/**
 * @brief Generic BSP Tree Face.
 *
 * Stores a bsp tree face. The face has to be convex and can contain
 * more than 3 vertices. An optional user data parameter allows to
 * link external informations to a face.
 */
class decBSPTreeFace{
private:
	decVector *pVertices;
	short pVertexCount;
	short pVertexSize;
	void *pUserData;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new bsp tree face. */
	decBSPTreeFace();
	/** Cleans up new bsp tree face. */
	virtual ~decBSPTreeFace();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of vertices. */
	inline int GetVertexCount() const{ return pVertexCount; }
	/** Retrieves the vertex at the given position. */
	const decVector &GetVertexAt( int index ) const;
	/** Adds a vertex. */
	void AddVertex( const decVector &vertex );
	/** Removes all vertices. */
	void RemoveAllVertices();
	/** Retrieves the user data. */
	inline void *GetUserData() const{ return pUserData; }
	/** Sets the user data. */
	void SetUserData( void *userData );
	/*@}*/
};

// end of include only once
#endif
