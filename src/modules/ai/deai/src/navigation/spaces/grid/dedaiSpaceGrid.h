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

#ifndef _DEDAISPACEGRID_H_
#define _DEDAISPACEGRID_H_

#include <dragengine/common/collection/decTList.h>
#include <dragengine/common/math/decMath.h>

class dedaiSpace;
class dedaiSpaceGridEdge;
class dedaiSpaceGridVertex;

class decConvexVolumeList;



/**
 * Space frid.
 */
class dedaiSpaceGrid{
private:
	dedaiSpace &pSpace;
	
	decTList<unsigned short> pVertexEdges;
	decTList<dedaiSpaceGridVertex> pVertices;
	decTList<dedaiSpaceGridEdge> pEdges;
	decTList<dedaiSpaceGridVertex*> pLinks;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create space grid. */
	dedaiSpaceGrid(dedaiSpace &space);
	
	/** Clean up space grid. */
	~dedaiSpaceGrid();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Parent space. */
	inline dedaiSpace &GetSpace() const{ return pSpace; }
	
	
	/** Vertex edges. */
	inline decTList<unsigned short> &GetVertexEdges(){ return pVertexEdges; }
	inline const decTList<unsigned short> &GetVertexEdges() const{ return pVertexEdges; }
	
	
	/** Vertices. */
	inline decTList<dedaiSpaceGridVertex> &GetVertices(){ return pVertices; }
	inline const decTList<dedaiSpaceGridVertex> &GetVertices() const{ return pVertices; }
	
	/** \brief Add vertex. */
	void AddVertex(const decVector &position);
	
	/** Vertex closest to a position or -1 if not found. */
	dedaiSpaceGridVertex *GetVertexClosestTo(const decVector &position, float &distance) const;
	
	/**
	 * Nearest position inside radius.
	 * \details Sets nearest position and returns edge if found otherwise returns \em NULL.
	 */
	dedaiSpaceGridEdge *NearestPoint(const decVector &point, float radius,
	decVector &nearestPosition, float &nearestDistSquared, float &nearestLambda) const;
	
	
	/** Edges. */
	inline decTList<dedaiSpaceGridEdge> &GetEdges(){ return pEdges; }
	inline const decTList<dedaiSpaceGridEdge> &GetEdges() const{ return pEdges; }
	
	/** Index of the edge matching the given set of vertices or -1 if not found. */
	int IndexOfEdgeMatching(unsigned short vertex1, unsigned short vertex2) const;
	
	/** Add edge returning the edge index. */
	void AddEdge(unsigned short vertex1, unsigned short vertex2);
	
	
	/** Links. */
	inline decTList<dedaiSpaceGridVertex*> &GetLinks(){ return pLinks; }
	inline const decTList<dedaiSpaceGridVertex*> &GetLinks() const{ return pLinks; }
	
	/** \brief Remove all links. */
	void RemoveAllLinks();
	
	
	/** Init from grid from the parent space. */
	void InitFromSpace();
	
	/** Link to other grids if possible. */
	void LinkToOtherGrids();
	
	/** Update blocking. */
	void UpdateBlocking();
	
	/** Clear grid. */
	void Clear();
	
	/** Update the shape of the debug drawer space object if existing. */
	void UpdateDDSSpaceShape();
	/*@}*/
	
	
	
private:
	void pInitFromNavSpace();
	void pInitFromHTNavSpace();
	
	void pDisableVertices(const decConvexVolumeList &list);
};

#endif
