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

#ifndef _DEDAISPACEMESH_H_
#define _DEDAISPACEMESH_H_

#include <dragengine/common/collection/decTList.h>
#include <dragengine/common/math/decMath.h>

class dedaiSpace;
class dedaiSpaceMeshEdge;
class dedaiSpaceMeshCorner;
class dedaiSpaceMeshFace;
class dedaiSpaceMeshLink;

class dedaiConvexFaceList;
class decConvexVolumeList;



/**
 * Navigation Mesh.
 */
class dedaiSpaceMesh{
private:
	dedaiSpace &pSpace;
	
	decTList<decVector> pVertices;
	int pStaticVertexCount;
	int pBlockerBaseVertex;
	
	decTList<dedaiSpaceMeshEdge> pEdges;
	int pStaticEdgeCount;
	int pBlockerBaseEdge;
	
	decTList<dedaiSpaceMeshCorner> pCorners;
	int pStaticCornerCount;
	int pBlockerBaseCorner;
	
	decTList<dedaiSpaceMeshFace> pFaces;
	int pStaticFaceCount;
	int pBlockerBaseFace;
	
	decTList<dedaiSpaceMeshLink> pLinks;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create space mesh. */
	explicit dedaiSpaceMesh(dedaiSpace &space);
	
	/** Clean up space mesh. */
	~dedaiSpaceMesh();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Parent space. */
	inline dedaiSpace &GetSpace() const{ return pSpace; }
	
	
	
	/** Vertices. */
	inline decTList<decVector> &GetVertices(){ return pVertices; }
	inline const decTList<decVector> &GetVertices() const{ return pVertices; }
	
	/** Number of static vertices. */
	inline int GetStaticVertexCount() const{ return pStaticVertexCount; }
	
	
	
	/** Edges. */
	inline decTList<dedaiSpaceMeshEdge> &GetEdges(){ return pEdges; }
	inline const decTList<dedaiSpaceMeshEdge> &GetEdges() const{ return pEdges; }
	
	/** Number of static edges. */
	inline int GetStaticEdgeCount() const{ return pStaticEdgeCount; }
	
	/** Index of the edge matching the given set of vertices or -1 if not found. */
	int IndexOfEdgeMatching(unsigned short vertex1, unsigned short vertex2) const;
	
	/** Add edge returning the edge index. */
	void AddEdge(unsigned short vertex1, unsigned short vertex2);
	
	
	
	/** Corners. */
	inline decTList<dedaiSpaceMeshCorner> &GetCorners(){ return pCorners; }
	inline const decTList<dedaiSpaceMeshCorner> &GetCorners() const{ return pCorners; }
	
	/** Number of static corners. */
	inline int GetStaticCornerCount() const{ return pStaticCornerCount; }
	
	/** Add corner. */
	void AddCorner(unsigned short vertex, unsigned short edge, unsigned short type);
	
	
	
	/** Faces. */
	inline decTList<dedaiSpaceMeshFace> &GetFaces(){ return pFaces; }
	inline const decTList<dedaiSpaceMeshFace> &GetFaces() const{ return pFaces; }
	
	/** Add face. */
	void AddFace();
	
	/** Number of static faces. */
	inline int GetStaticFaceCount() const{ return pStaticFaceCount; }
	
	/** Face closest to a position or -1 if not found. */
	dedaiSpaceMeshFace *GetFaceClosestTo(const decVector &position, float &distance) const;
	
	
	
	/**
	 * Nearest position inside radius.
	 * \details Sets nearest position and returns face if found otherwise returns \em NULL.
	 */
	dedaiSpaceMeshFace *NearestPoint(const decVector &point, float radius,
	decVector &nearestPosition, float &nearestDistSquared) const;
	
	
	
	/** Links. */
	inline const decTList<dedaiSpaceMeshLink> &GetLinks() const{ return pLinks; }
	
	/** Link matching foreign link parameters or \em NULL if absent. */
	dedaiSpaceMeshLink &GetLinkWith(const dedaiSpaceMeshLink &foreignLink);
	
	/** Add link calculating the transform matrix. */
	void AddLink(dedaiSpaceMesh *mesh, unsigned short face, unsigned short corner);
	
	/** Add link with the provided transform matrix. */
	void AddLink(dedaiSpaceMesh *mesh, unsigned short face, unsigned short corner, const decMatrix &transform);
	
	/** Link with parameters is present. */
	bool HasLinkWith(dedaiSpaceMesh *mesh, unsigned short face, unsigned short corner) const;
	
	/** Remove link for corner. */
	void RemoveLinkWith(const dedaiSpaceMeshLink &foreignLink);
	
	/** \brief Remove all links. */
	void RemoveAllLinks();
	
	
	/** Init from navigation mesh from the parent navigation space. */
	void InitFromSpace();
	
	/** Link to other navigation meshes if possible. */
	void LinkToOtherMeshes();
	
	/** Update blocking. */
	void UpdateBlocking();
	
	/** Clear space mesh. */
	void Clear();
	
	
	
	/** Update the shape of the debug drawer space object if existing. */
	void UpdateDDSSpaceShape();
	/*@}*/
	
	
	
private:
	void pInitFromNavSpace();
	void pInitFromHTNavSpace();
	
	void pInitConvexFaceListFromFace(dedaiConvexFaceList &list, const dedaiSpaceMeshFace &face) const;
	bool pMatchesConvexFaceListMeshFace(dedaiConvexFaceList &list, const dedaiSpaceMeshFace &face) const;
	void pOptimizeBlockedFaces(dedaiConvexFaceList &list, int initialVertexCount) const;
	void pDisableFace(int faceIndex);
	void pAddConvexFaces(const dedaiConvexFaceList &list, const dedaiSpaceMeshFace &sourceFace);
	void pLinkToMesh(dedaiSpaceMesh *mesh, float snapDistance, float snapAngle);
	void pSplitEdge(int edgeIndex, const decVector &splitVertex);
	
	void pVerifyInvariants() const;
	void pDebugPrint() const;
};

#endif
