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

#ifndef _DEDAINAVMESHBUILDEREDGE_H_
#define _DEDAINAVMESHBUILDEREDGE_H_

class dedaiNavMeshBuilderFace;



/**
 * @brief Navigation Mesh Builder Edge.
 */
class dedaiNavMeshBuilderEdge{
private:
	int pIndex;
	int pVertex1;
	int pVertex2;
	dedaiNavMeshBuilderFace *pFace1;
	dedaiNavMeshBuilderFace *pFace2;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new navigation mesh builder edge. */
	dedaiNavMeshBuilderEdge();
	/** Creates a new navigation mesh builder edge. */
	dedaiNavMeshBuilderEdge( int vertex1, int vertex2 );
	/** Cleans up the navigation mesh builder edge. */
	~dedaiNavMeshBuilderEdge();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the index. */
	inline int GetIndex() const{ return pIndex; }
	/** Sets the index. */
	void SetIndex( int index );
	/** Retrieves the index of the first vertex. */
	inline int GetVertex1() const{ return pVertex1; }
	/** Sets the index of the first vertex. */
	void SetVertex1( int vertex );
	/** Retrieves the index of the second vertex. */
	inline int GetVertex2() const{ return pVertex2; }
	/** Sets the index of the second vertex. */
	void SetVertex2( int vertex );
	/** Retrieves the first face or NULL if there is none. */
	inline dedaiNavMeshBuilderFace *GetFace1() const{ return pFace1; }
	/** Sets the first face or NULL if there is none. */
	void SetFace1( dedaiNavMeshBuilderFace *face );
	/** Retrieves the second face or NULL if there is none. */
	inline dedaiNavMeshBuilderFace *GetFace2() const{ return pFace2; }
	/** Sets the second face or NULL if there is none. */
	void SetFace2( dedaiNavMeshBuilderFace *face );
	/*@}*/
};

#endif
