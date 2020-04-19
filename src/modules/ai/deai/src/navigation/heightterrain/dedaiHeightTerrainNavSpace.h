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
	dedaiHeightTerrainNavSpace( dedaiHeightTerrainSector &sector,
		const deHeightTerrainNavSpace &navigationSpace );
	
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
	void HeightChanged( const decPoint &from, const decPoint &to );
	
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
