/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLRHTSECTOR_H_
#define _DEOGLRHTSECTOR_H_

#include "../../deoglBasics.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>

#define OGLHTS_MAX_MASK_TEXTURES 4

class deoglHTSCluster;
class deoglHTSTexture;
class deoglPixelBuffer;
class deoglRenderThread;
class deoglRHeightTerrain;
class deoglTexture;

class deHeightTerrainSector;



/**
 * Render height terrain sector.
 */
class deoglRHTSector : public deObject{
private:
	deoglRHeightTerrain &pHeightTerrain;
	
	decPoint pCoordinates;
	float pBaseHeight;
	float pScaling;
	
	deoglHTSTexture **pTextures;
	int pTextureCount;
	bool pValidTextures;
	bool pDirtyTextures;
	
	deoglTexture *pMasks[ OGLHTS_MAX_MASK_TEXTURES ];
	deoglPixelBuffer *pPixBufMasks[ OGLHTS_MAX_MASK_TEXTURES ];
	
	float *pHeights;
	
	GLuint *pVBODataPoints1;
	int pVBODataPoints1Count;
	GLuint *pVBODataPoints2;
	int pVBODataPoints2Count;
	GLuint *pVBODataFaces;
	int pVBODataFacesCount;
	
	bool pDirtyPoints;
	
	deoglHTSCluster *pClusters;
	int pClusterCount;
	
	bool pValid;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create height terrain sector. */
	deoglRHTSector( deoglRHeightTerrain &heightTerrain, const deHeightTerrainSector &sector );
	
	/** Clean up height terrain sector. */
	virtual ~deoglRHTSector();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Height terrain. */
	inline deoglRHeightTerrain &GetHeightTerrain() const{ return pHeightTerrain; }
	
	/** Update vbo if required. */
	void UpdateVBO();
	
	/** Sector coordinates. */
	inline const decPoint &GetCoordinates() const{ return pCoordinates; }
	
	/** Base height. */
	inline float GetBaseHeight() const{ return pBaseHeight; }
	
	/** Scaling. */
	inline float GetScaling() const{ return pScaling; }
	
	/** Calculate world matrix. */
	decDMatrix CalcWorldMatrix() const;
	decDMatrix CalcWorldMatrix( const decDVector &referencePosition ) const;
	
	/** Calculate world position. */
	decDVector CalcWorldPosition() const;
	decDVector CalcWorldPosition( const decDVector &referencePosition ) const;
	
	
	
	/** Number of textures. */
	inline int GetTextureCount() const{ return pTextureCount; }
	
	/** Texture at index. */
	deoglHTSTexture &GetTextureAt( int index ) const;
	
	/** Terrain height map mask textures. */
	inline deoglTexture **GetMaskTextures(){ return pMasks; }
	
	/** Sector is valid. */
	inline bool GetValid() const{ return pValid; }
	
	/** Textures are valid. */
	inline bool GetValidTextures() const{ return pValidTextures; }
	
	
	
	/** Heights. */
	inline float *GetHeights() const{ return pHeights; }
	
	/** Height changed. */
	void HeightChanged( const deHeightTerrainSector &sector, const decPoint &from, const decPoint &to );
	
	/** Sector changed. */
	void SectorChanged( const deHeightTerrainSector &sector );
	
	
	
	/** Number of clusters. */
	inline int GetClusterCount() const{ return pClusterCount; }
	
	/** Cluster at location. */
	deoglHTSCluster &GetClusterAt( int x, int z ) const;
	
	/** List of clusters. */
	inline deoglHTSCluster *GetClusters() const{ return pClusters; }
	/*@}*/
	
private:
	void pCleanUp();
	
	void pCreateArrays( const deHeightTerrainSector &sector );
	void pCreateHeightMap( const deHeightTerrainSector &sector );
	
	void pSetTextureCount( int count );
	
	void pDropMaskPixelBuffers();
	
	void pSyncSector( const deHeightTerrainSector &sector );
	void pSyncTextures( const deHeightTerrainSector &sector );
	void pSyncMaskTextures( const deHeightTerrainSector &sector );
	void pSyncHeightMap( const deHeightTerrainSector &sector, const decPoint &from, const decPoint &to );
	
	void pUpdateTextures();
	void pUpdateMaskTextures();
	void pCalculateUVs();
	void pCalculateUVsPlanar( int textureIndex );
	void pCreateVBODataPoints1();
	void pCreateVBODataFaces();
	void pUpdateHeightMap();
};

#endif
