/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
	inline dedaiWorld *GetWorld() const{return pWorld;}
	/** Sets the world to find the path in. */
	void SetWorld(dedaiWorld *world);
	/** Retrieves the navigator to get the type mappings from. */
	inline dedaiNavigator *GetNavigator() const{return pNavigator;}
	/** Sets the navigator to get the type mappings from. */
	void SetNavigator(dedaiNavigator *navigator);
	/** Retrieves the start point. */
	inline const decDVector &GetStartPoint() const{return pStartPoint;}
	/** Sets the start point. */
	void SetStartPoint(const decDVector &point);
	/** Retrieves the end point. */
	inline const decDVector &GetEndPoint() const{return pEndPoint;}
	/** Sets the end point. */
	void SetEndPoint(const decDVector &point);
	
	/** Find path. */
	void FindPath();
	
	/** Retrieves the open list. */
	inline decPointerList &GetListOpen(){return pListOpen;}
	inline const decPointerList &GetListOpen() const{return pListOpen;}
	/** Retrieves the closed list. */
	inline decPointerList &GetListClosed(){return pListClosed;}
	inline const decPointerList &GetListClosed() const{return pListClosed;}
	/** Retrieves the vertices path. */
	inline decPointerList &GetPathVertices(){return pPathVertices;}
	inline const decPointerList &GetPathVertices() const{return pPathVertices;}
	
	/** Retrieves the number of path points. */
	inline int GetPathPointCount() const{return pPathPointCount;}
	/** Retrieves the path points. */
	const decDVector *GetPathPoints() const{return pPathPoints;}
	/** Adds a path point. */
	void AddPathPoint(const decDVector &point);
	/** Removes all path points. */
	void RemoveAllPathPoints();
	/*@}*/
	
private:
	void pClearLists();
	void pFindVertexPath();
};

#endif
