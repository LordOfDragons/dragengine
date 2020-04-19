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
	inline dedaiNavMeshBuilderEdge *GetEdgeAt( int index ) const{ return ( dedaiNavMeshBuilderEdge* )pEdges.GetAt( index ); }
	/** Retrieves the matching edge creating it if not existing. */
	dedaiNavMeshBuilderEdge *GetEdgeWith( int vertex1, int vertex2 );
	
	/** Retrieves the number of faces. */
	inline int GetFaceCount() const{ return pFaces.GetCount(); }
	/** Retrieves an face. */
	inline dedaiNavMeshBuilderFace *GetFaceAt( int index ) const{ return ( dedaiNavMeshBuilderFace* )pFaces.GetAt( index ); }
	/** Adds a new face. */
	dedaiNavMeshBuilderFace *AddFace();
	
	/** Update indices. */
	void UpdateIndices();
	/** Clear. */
	void Clear();
	/*@}*/
};

#endif
