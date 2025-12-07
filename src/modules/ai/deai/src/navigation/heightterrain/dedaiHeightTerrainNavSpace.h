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

#ifndef _DEDAIHEIGHTTERRAINNAVSPACE_H_
#define _DEDAIHEIGHTTERRAINNAVSPACE_H_

#include <dragengine/deObject.h>

class dedaiHeightTerrainSector;
class dedaiSpace;

class deHeightTerrainNavSpace;
class deNavigationSpaceFace;



/**
 * \brief Navigation space peer.
 */
class dedaiHeightTerrainNavSpace : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<dedaiHeightTerrainNavSpace> Ref;
	
	
	struct sEdge{
		unsigned short vertex1;
		unsigned short vertex2;
	};
	
	
	
private:
	dedaiHeightTerrainSector &pSector;
	const deHeightTerrainNavSpace &pNavigationSpace;
	
	unsigned int *pVertices;
	int pVertexCount;
	int pVertexSize;
	
	sEdge *pEdges;
	
	unsigned short *pCorners;
	int pCornerCount;
	int pCornerSize;
	
	deNavigationSpaceFace *pFaces;
	int pFaceCount;
	int pFaceSize;
	
	dedaiSpace *pSpace;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	dedaiHeightTerrainNavSpace(dedaiHeightTerrainSector &sector,
		const deHeightTerrainNavSpace &navigationSpace);
	
protected:
	/** \brief Clean up peer. */
	virtual ~dedaiHeightTerrainNavSpace();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Height terrain sector. */
	inline dedaiHeightTerrainSector &GetSector() const{ return pSector; }
	
	/** \brief Navigation space. */
	inline const deHeightTerrainNavSpace &GetNavigationSpace() const{ return pNavigationSpace; }
	
	
	
	/** \brief Vertices in grid coordinates. */
	inline const unsigned int *GetVertices() const{ return pVertices; }
	
	/** \brief Number of grid vertices. */
	inline int GetVertexCount() const{ return pVertexCount; }
	
	/** \brief Edges mapping to grid vertices. */
	inline const sEdge *GetEdges() const{ return pEdges; }
	
	/** \brief Corners mapping to grid vertices. */
	inline const unsigned short *GetCorners() const{ return pCorners; }
	
	/** \brief Number of corners. */
	inline int GetCornerCount() const{ return pCornerCount; }
	
	/** \brief Faces. */
	inline const deNavigationSpaceFace * const GetFaces() const{ return pFaces; }
	
	/** \brief Number of faces. */
	inline int GetFaceCount() const{ return pFaceCount; }
	
	
	
	/** \brief Space. */
	inline dedaiSpace *GetSpace() const{ return pSpace; }
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Parent world changed. */
	void ParentWorldChanged();
	
	/** \brief Sector heights changed. */
	void HeightChanged(const decPoint &from, const decPoint &to);
	
	/** \brief Layer changed. */
	void LayerChanged();
	
	/** \brief Type changed. */
	void TypeChanged();
	
	/** \brief Snapping parameters changed. */
	void SnappingChanged();
	
	/** \brief Layout changed. */
	void LayoutChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pUpdateData();
	void pUpdateCorners();
	void pUpdateEdges();
};

#endif
