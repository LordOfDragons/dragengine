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

#ifndef _DEDAIPATHFINDERNAVGRID_H_
#define _DEDAIPATHFINDERNAVGRID_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decPointerList.h>

class dedaiSpaceGridVertex;
class dedaiNavigator;
class dedaiWorld;



/**
 * @brief Path Finder for Navigation Grids.
 */
class dedaiPathFinderNavGrid{
private:
	dedaiWorld *pWorld;
	dedaiNavigator *pNavigator;
	decDVector pStartPoint;
	decDVector pEndPoint;
	
	dedaiSpaceGridVertex *pStartVertex;
	dedaiSpaceGridVertex *pEndVertex;
	
	decPointerList pListClosed;
	decPointerList pListOpen;
	decPointerList pPathVertices;
	
	decDVector *pPathPoints;
	int pPathPointCount;
	int pPathPointSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new path finder. */
	dedaiPathFinderNavGrid();
	/** Cleans up the path finder. */
	~dedaiPathFinderNavGrid();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the world to find the path in. */
	inline dedaiWorld *GetWorld() const{ return pWorld; }
	/** Sets the world to find the path in. */
	void SetWorld( dedaiWorld *world );
	/** Retrieves the navigator to get the type mappings from. */
	inline dedaiNavigator *GetNavigator() const{ return pNavigator; }
	/** Sets the navigator to get the type mappings from. */
	void SetNavigator( dedaiNavigator *navigator );
	/** Retrieves the start point. */
	inline const decDVector &GetStartPoint() const{ return pStartPoint; }
	/** Sets the start point. */
	void SetStartPoint( const decDVector &point );
	/** Retrieves the end point. */
	inline const decDVector &GetEndPoint() const{ return pEndPoint; }
	/** Sets the end point. */
	void SetEndPoint( const decDVector &point );
	
	/** Find path. */
	void FindPath();
	
	/** Retrieves the open list. */
	inline decPointerList &GetListOpen(){ return pListOpen; }
	inline const decPointerList &GetListOpen() const{ return pListOpen; }
	/** Retrieves the closed list. */
	inline decPointerList &GetListClosed(){ return pListClosed; }
	inline const decPointerList &GetListClosed() const{ return pListClosed; }
	/** Retrieves the vertices path. */
	inline decPointerList &GetPathVertices(){ return pPathVertices; }
	inline const decPointerList &GetPathVertices() const{ return pPathVertices; }
	
	/** Retrieves the number of path points. */
	inline int GetPathPointCount() const{ return pPathPointCount; }
	/** Retrieves the path points. */
	const decDVector *GetPathPoints() const{ return pPathPoints; }
	/** Adds a path point. */
	void AddPathPoint( const decDVector &point );
	/** Removes all path points. */
	void RemoveAllPathPoints();
	/*@}*/
	
private:
	void pClearLists();
	void pFindVertexPath();
};

#endif
