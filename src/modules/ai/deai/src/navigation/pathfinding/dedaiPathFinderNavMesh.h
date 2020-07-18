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

#ifndef _DEDAIPATHFINDERNAVMESH_H_
#define _DEDAIPATHFINDERNAVMESH_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decPointerList.h>

class deDebugDrawerShape;
class dedaiSpaceMeshFace;
class dedaiNavigator;
class dedaiWorld;



/**
 * @brief Path Finder for Navigation Meshes.
 */
class dedaiPathFinderNavMesh{
private:
	dedaiWorld *pWorld;
	dedaiNavigator *pNavigator;
	decDVector pStartPoint;
	decDVector pEndPoint;
	
	dedaiSpaceMeshFace *pStartFace;
	dedaiSpaceMeshFace *pEndFace;
	
	decPointerList pListClosed;
	decPointerList pListOpen;
	decPointerList pPathFaces;
	
	decDVector *pPathPoints;
	int pPathPointCount;
	int pPathPointSize;
	
	deDebugDrawerShape *pDDSListOpen;
	deDebugDrawerShape *pDDSListClosed;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new path finder. */
	dedaiPathFinderNavMesh();
	/** Cleans up the path finder. */
	~dedaiPathFinderNavMesh();
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
	/** Retrieves the faces path. */
	inline decPointerList &GetPathFaces(){ return pPathFaces; }
	inline const decPointerList &GetPathFaces() const{ return pPathFaces; }
	
	/** Retrieves the number of path points. */
	inline int GetPathPointCount() const{ return pPathPointCount; }
	/** Retrieves the path points. */
	const decDVector *GetPathPoints() const{ return pPathPoints; }
	/** Adds a path point. */
	void AddPathPoint( const decDVector &point );
	/** Removes all path points. */
	void RemoveAllPathPoints();
	
	/** \brief Update a debug drawer with highlighting the faces along the path. */
	void UpdateDDSShapeFaces( deDebugDrawerShape &dds );
	
	/** \brief Sets the debug drawer shape to use to visualize the opened list or NULL to disable. */
	void SetDDSListOpen( deDebugDrawerShape *dds );
	/** \brief Sets the debug drawer shape to use to visualize the closed list or NULL to disable. */
	void SetDDSListClosed( deDebugDrawerShape *dds );
	/*@}*/
	
private:
	void pClearLists();
	void pFindFacePath();
	void pFindRealPath();
	int pFindEdgeLeadingToFace( const dedaiSpaceMeshFace &face, const dedaiSpaceMeshFace &targetFace ) const;
	void pUpdateDDSListOpen();
	void pUpdateDDSListClosed();
};

#endif
