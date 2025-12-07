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

#include <dragengine/common/math/decMath.h>

class dedaiSpace;
class dedaiSpaceGridEdge;
class dedaiSpaceGridVertex;

class decConvexVolumeList;



/**
 * \brief Space frid.
 */
class dedaiSpaceGrid{
private:
	dedaiSpace &pSpace;
	
	unsigned short *pVertexEdges;
	int pVertexEdgeCount;
	int pVertexEdgeSize;
	
	dedaiSpaceGridVertex *pVertices;
	int pVertexCount;
	int pVertexSize;
	
	dedaiSpaceGridEdge *pEdges;
	int pEdgeCount;
	int pEdgeSize;
	
	dedaiSpaceGridVertex **pLinks;
	int pLinkCount;
	int pLinkSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create space grid. */
	dedaiSpaceGrid(dedaiSpace &space);
	
	/** \brief Clean up space grid. */
	~dedaiSpaceGrid();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent space. */
	inline dedaiSpace &GetSpace() const{ return pSpace; }
	
	
	
	/** \brief Number of vertex edges. */
	inline int GetVertexEdgeCount() const{ return pVertexEdgeCount; }
	
	/** \brief Array of vertex edges. */
	inline unsigned short *GetVertexEdges() const{ return pVertexEdges; }
	
	/** \brief Vertex edge by index. */
	unsigned short GetVertexEdgeAt(int index) const;
	
	/** \brief Add vertex edge. */
	void AddVertexEdge(unsigned short edge);
	
	
	
	/** \brief Number of vertices. */
	inline int GetVertexCount() const{ return pVertexCount; }
	
	/** \brief Array of vertices. */
	inline dedaiSpaceGridVertex *GetVertices() const{ return pVertices; }
	
	/** \brief Vertex by index. */
	dedaiSpaceGridVertex &GetVertexAt(int index) const;
	
	/** \brief Add vertex. */
	void AddVertex(const decVector &position);
	
	/** \brief Vertex closest to a position or -1 if not found. */
	dedaiSpaceGridVertex *GetVertexClosestTo(const decVector &position, float &distance) const;
	
	
	
	/** \brief Number of edges. */
	inline int GetEdgeCount() const{ return pEdgeCount; }
	
	/** \brief Array of edges. */
	inline dedaiSpaceGridEdge *GetEdges() const{ return pEdges; }
	
	/** \brief Edge by index. */
	dedaiSpaceGridEdge &GetEdgeAt(int index) const;
	
	/** \brief Index of the edge matching the given set of vertices or -1 if not found. */
	int IndexOfEdgeMatching(unsigned short vertex1, unsigned short vertex2) const;
	
	/** \brief Add edge returning the edge index. */
	void AddEdge(unsigned short vertex1, unsigned short vertex2);
	
	
	
	/** \brief Number of links. */
	inline int GetLinkCount() const{ return pLinkCount; }
	
	/** \brief Array of links. */
	inline dedaiSpaceGridVertex **GetLinks() const{ return pLinks; }
	
	/** \brief Link by index. */
	dedaiSpaceGridVertex *GetLinkAt(int index) const;
	
	/** \brief Add link. */
	void AddLink(dedaiSpaceGridVertex *vertex);
	
	/** \brief Remove all links. */
	void RemoveAllLinks();
	
	
	
	/**
	 * \brief Nearest position inside radius.
	 * \details Sets nearest position and returns edge if found otherwise returns \em NULL.
	 */
	dedaiSpaceGridEdge *NearestPoint(const decVector &point, float radius,
	decVector &nearestPosition, float &nearestDistSquared, float &nearestLambda) const;
	
	
	
	/** \brief Init from grid from the parent space. */
	void InitFromSpace();
	
	/** \brief Link to other grids if possible. */
	void LinkToOtherGrids();
	
	/** \brief Update blocking. */
	void UpdateBlocking();
	
	/** \brief Clear grid. */
	void Clear();
	
	/** \brief Update the shape of the debug drawer space object if existing. */
	void UpdateDDSSpaceShape();
	/*@}*/
	
	
	
private:
	void pInitFromNavSpace();
	void pInitFromHTNavSpace();
	
	void pDisableVertices(const decConvexVolumeList &list);
};

#endif
