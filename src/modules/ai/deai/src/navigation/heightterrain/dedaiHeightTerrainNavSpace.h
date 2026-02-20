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
#include <dragengine/common/collection/decTList.h>

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
	using Ref = deTObjectReference<dedaiHeightTerrainNavSpace>;
	
	
	struct sEdge{
		unsigned short vertex1, vertex2;
	};
	
	
	
private:
	dedaiHeightTerrainSector &pSector;
	const deHeightTerrainNavSpace &pNavigationSpace;
	
	decTList<unsigned int> pVertices;
	decTList<sEdge> pEdges;
	decTList<unsigned short> pCorners;
	decTList<deNavigationSpaceFace> pFaces;
	
	dedaiSpace *pSpace;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	dedaiHeightTerrainNavSpace(dedaiHeightTerrainSector &sector,
		const deHeightTerrainNavSpace &navigationSpace);
	
protected:
	/** \brief Clean up peer. */
	~dedaiHeightTerrainNavSpace() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Height terrain sector. */
	inline dedaiHeightTerrainSector &GetSector() const{ return pSector; }
	
	/** \brief Navigation space. */
	inline const deHeightTerrainNavSpace &GetNavigationSpace() const{ return pNavigationSpace; }
	
	
	
	/** \brief Vertices in grid coordinates. */
	inline const decTList<unsigned int> &GetVertices() const{ return pVertices; }
	
	/** \brief Edges mapping to grid vertices. */
	inline const decTList<sEdge> &GetEdges() const{ return pEdges; }
	
	/** \brief Corners mapping to grid vertices. */
	inline const decTList<unsigned short> &GetCorners() const{ return pCorners; }
	
	/** \brief Faces. */
	inline const decTList<deNavigationSpaceFace> &GetFaces() const{ return pFaces; }
	
	
	
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
