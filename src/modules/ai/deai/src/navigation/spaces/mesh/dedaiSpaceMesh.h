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

#ifndef _DEDAISPACEMESH_H_
#define _DEDAISPACEMESH_H_

#include <dragengine/common/math/decMath.h>

class dedaiSpace;
class dedaiSpaceMeshEdge;
class dedaiSpaceMeshCorner;
class dedaiSpaceMeshFace;
class dedaiSpaceMeshLink;

class dedaiConvexFaceList;
class decConvexVolumeList;



/**
 * \brief Navigation Mesh.
 */
class dedaiSpaceMesh{
private:
	dedaiSpace &pSpace;
	
	decVector *pVertices;
	int pVertexCount;
	int pVertexSize;
	int pStaticVertexCount;
	int pBlockerBaseVertex;
	
	dedaiSpaceMeshEdge *pEdges;
	int pEdgeCount;
	int pEdgeSize;
	int pStaticEdgeCount;
	int pBlockerBaseEdge;
	
	dedaiSpaceMeshCorner *pCorners;
	int pCornerCount;
	int pCornerSize;
	int pStaticCornerCount;
	int pBlockerBaseCorner;
	
	dedaiSpaceMeshFace *pFaces;
	int pFaceCount;
	int pFaceSize;
	int pStaticFaceCount;
	int pBlockerBaseFace;
	
	dedaiSpaceMeshLink *pLinks;
	int pLinkCount;
	int pLinkSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create space mesh. */
	dedaiSpaceMesh( dedaiSpace &space );
	
	/** \brief Clean up space mesh. */
	~dedaiSpaceMesh();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent space. */
	inline dedaiSpace &GetSpace() const{ return pSpace; }
	
	
	
	/** \brief Number of vertices. */
	inline int GetVertexCount() const{ return pVertexCount; }
	
	/** \brief Array of vertices. */
	inline decVector *GetVertices() const{ return pVertices; }
	
	/** \brief Vertex by index. */
	const decVector &GetVertexAt( int index ) const;
	
	/** \brief Number of static vertices. */
	inline int GetStaticVertexCount() const{ return pStaticVertexCount; }
	
	/** \brief Add vertex. */
	void AddVertex( const decVector &position );
	
	/** \brief Index of the matching vertex or -1 if not found. */
	int IndexOfVertex( const decVector &vertex ) const;
	
	
	
	/** \brief Number of edges. */
	inline int GetEdgeCount() const{ return pEdgeCount; }
	
	/** \brief Number of static edges. */
	inline int GetStaticEdgeCount() const{ return pStaticEdgeCount; }
	
	/** \brief Array of edges. */
	inline dedaiSpaceMeshEdge *GetEdges() const{ return pEdges; }
	
	/** \brief Edge by index. */
	dedaiSpaceMeshEdge &GetEdgeAt( int index ) const;
	
	/** \brief Index of the edge matching the given set of vertices or -1 if not found. */
	int IndexOfEdgeMatching( unsigned short vertex1, unsigned short vertex2 ) const;
	
	/** \brief Add edge returning the edge index. */
	void AddEdge( unsigned short vertex1, unsigned short vertex2 );
	
	
	
	/** \brief Number of corners. */
	inline int GetCornerCount() const{ return pCornerCount; }
	
	/** \brief Array of corners. */
	inline dedaiSpaceMeshCorner *GetCorners() const{ return pCorners; }
	
	/** \brief Corner by index. */
	dedaiSpaceMeshCorner &GetCornerAt( int index ) const;
	
	/** \brief Number of static corners. */
	inline int GetStaticCornerCount() const{ return pStaticCornerCount; }
	
	/** \brief Add corner. */
	void AddCorner( unsigned short vertex, unsigned short edge, unsigned short type );
	
	
	
	/** \brief Number of faces. */
	inline int GetFaceCount() const{ return pFaceCount; }
	
	/** \brief Array of faces. */
	inline dedaiSpaceMeshFace *GetFaces() const{ return pFaces; }
	
	/** \brief Face by index. */
	dedaiSpaceMeshFace &GetFaceAt( int index ) const;
	
	/** \brief Number of static faces. */
	inline int GetStaticFaceCount() const{ return pStaticFaceCount; }
	
	/** \brief Face closest to a position or -1 if not found. */
	dedaiSpaceMeshFace *GetFaceClosestTo( const decVector &position, float &distance ) const;
	
	/** \brief Add face. */
	void AddFace();
	
	
	
	/**
	 * \brief Nearest position inside radius.
	 * \details Sets nearest position and returns face if found otherwise returns \em NULL.
	 */
	dedaiSpaceMeshFace *NearestPoint( const decVector &point, float radius,
	decVector &nearestPosition, float &nearestDistSquared ) const;
	
	
	
	/** \brief Number of links. */
	inline int GetLinkCount() const{ return pLinkCount; }
	
	/** \brief Array of links. */
	inline dedaiSpaceMeshLink *GetLinks() const{ return pLinks; }
	
	/** \brief Link by index. */
	dedaiSpaceMeshLink &GetLinkAt( int index ) const;
	
	/** \brief Link matching foreign link parameters or \em NULL if absent. */
	dedaiSpaceMeshLink &GetLinkWith( const dedaiSpaceMeshLink &foreignLink ) const;
	
	/** \brief Add link calculating the transform matrix. */
	void AddLink( dedaiSpaceMesh *mesh, unsigned short face, unsigned short corner );
	
	/** \brief Add link with the provided transform matrix. */
	void AddLink( dedaiSpaceMesh *mesh, unsigned short face, unsigned short corner, const decMatrix &transform );
	
	/** \brief Link with parameters is present. */
	bool HasLinkWith( dedaiSpaceMesh *mesh, unsigned short face, unsigned short corner ) const;
	
	/** \brief Remove link for corner. */
	void RemoveLinkWith( const dedaiSpaceMeshLink &foreignLink );
	
	/** \brief Remove all links. */
	void RemoveAllLinks();
	
	
	
	/** \brief Init from navigation mesh from the parent navigation space. */
	void InitFromSpace();
	
	/** \brief Link to other navigation meshes if possible. */
	void LinkToOtherMeshes();
	
	/** \brief Update blocking. */
	void UpdateBlocking();
	
	/** \brief Clear space mesh. */
	void Clear();
	
	
	
	/** \brief Update the shape of the debug drawer space object if existing. */
	void UpdateDDSSpaceShape();
	/*@}*/
	
	
	
private:
	void pInitFromNavSpace();
	void pInitFromHTNavSpace();
	
	void pInitConvexFaceListFromFace( dedaiConvexFaceList &list, const dedaiSpaceMeshFace &face ) const;
	bool pMatchesConvexFaceListMeshFace( dedaiConvexFaceList &list, const dedaiSpaceMeshFace &face ) const;
	void pOptimizeBlockedFaces( dedaiConvexFaceList &list, int initialVertexCount ) const;
	void pDisableFace( int faceIndex );
	void pAddConvexFaces( const dedaiConvexFaceList &list, const dedaiSpaceMeshFace &sourceFace );
	void pLinkToMesh( dedaiSpaceMesh *mesh, float snapDistance, float snapAngle );
	void pSplitEdge( int edgeIndex, const decVector &splitVertex );
	
	void pVerifyInvariants() const;
	void pDebugPrint() const;
};

#endif
