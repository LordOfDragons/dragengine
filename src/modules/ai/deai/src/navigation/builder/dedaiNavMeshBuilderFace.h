/* 
 * Drag[en]gine AI Module
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

#ifndef _DEDAINAVMESHBUILDERFACE_H_
#define _DEDAINAVMESHBUILDERFACE_H_

#include <dragengine/common/collection/decIntSet.h>
#include <dragengine/common/collection/decPointerSet.h>



/**
 * @brief Navigation Mesh Builder Face.
 */
class dedaiNavMeshBuilderFace{
private:
	int pIndex;
	decIntSet pVertices;
	decPointerSet pEdges;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new navigation mesh builder face. */
	dedaiNavMeshBuilderFace();
	/** Cleans up the navigation mesh builder face. */
	~dedaiNavMeshBuilderFace();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the index. */
	inline int GetIndex() const{ return pIndex; }
	/** Sets the index. */
	void SetIndex( int index );
	/** Retrieves the vertex list. */
	inline decIntSet &GetVertices(){ return pVertices; }
	inline const decIntSet &GetVertices() const{ return pVertices; }
	/** Retrieves the edges list. */
	inline decPointerSet &GetEdges(){ return pEdges; }
	inline const decPointerSet &GetEdges() const{ return pEdges; }
	/*@}*/
};

#endif
