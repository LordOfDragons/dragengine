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
#include "../../texture/pixelbuffer/deoglPixelBuffer.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>

#define OGLHTS_MAX_MASK_TEXTURES 4

class deoglHTSCluster;
class deoglHTSTexture;
class deoglRenderThread;
class deoglRHeightTerrain;
class deoglTexture;
class deoglWorldCompute;

class deHeightTerrainSector;



/**
 * Render height terrain sector.
 */
class deoglRHTSector : public deObject{
public:
	/** Type holding strong reference. */
	typedef deTObjectReference<deoglRHTSector> Ref;
	
private:
	deoglRHeightTerrain &pHeightTerrain;
	int pIndex;
	
	decPoint pCoordinates;
	float pBaseHeight;
	float pScaling;
	
	deoglHTSTexture **pTextures;
	int pTextureCount;
	bool pValidTextures;
	bool pDirtyMaskTextures;
	bool pTexturesRequirePrepareForRender;
	
	deoglTexture *pMasks[ OGLHTS_MAX_MASK_TEXTURES ];
	deoglPixelBuffer::Ref pPixBufMasks[ OGLHTS_MAX_MASK_TEXTURES ];
	
	float *pHeights;
	float pMinHeight;
	float pMaxHeight;
	
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
	
protected:
	/** Clean up height terrain sector. */
	virtual ~deoglRHTSector();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Height terrain. */
	inline deoglRHeightTerrain &GetHeightTerrain() const{ return pHeightTerrain; }
	
	/** Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** Set index. */
	void SetIndex( int index );
	
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
	
	/** Add to world compute. */
	void AddToWorldCompute( deoglWorldCompute &worldCompute );
	
	/** Update world compute. */
	void UpdateWorldCompute( deoglWorldCompute &worldCompute );
	
	/** Remove from world compute. */
	void RemoveFromWorldCompute( deoglWorldCompute &worldCompute );
	
	/** Prepare for render. */
	void PrepareForRender();
	
	
	
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
	
	/** Texture requires prepare for render. */
	void TextureRequirePrepareForRender();
	
	
	
	/** Heights. */
	inline float *GetHeights() const{ return pHeights; }
	
	/** Minimum height. */
	inline float GetMinHeight() const{ return pMinHeight; }
	
	/** Maximum height. */
	inline float GetMaxHeight() const{ return pMaxHeight; }
	
	/** Height changed. */
	void HeightChanged( const deHeightTerrainSector &sector, const decPoint &from, const decPoint &to );
	
	/** Sector changed. */
	void SectorChanged( const deHeightTerrainSector &sector );
	
	
	
	/** Number of clusters. */
	inline int GetClusterCount() const{ return pClusterCount; }
	
	/** Cluster at location. */
	deoglHTSCluster &GetClusterAt( int x, int z ) const;
	deoglHTSCluster &GetClusterAt( const decPoint &coordinate ) const;
	
	/** List of clusters. */
	inline deoglHTSCluster *GetClusters() const{ return pClusters; }
	
	/** Clusters update world compute element textures. */
	void ClustersUpdateWorldComputeElementTextures();
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
	
	void pUpdateMaskTextures();
	void pCreateVBODataPoints1();
	void pCreateVBODataFaces();
	void pUpdateHeightMap();
};

#endif
