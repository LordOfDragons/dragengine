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
	
	deoglTexture *pMasks[OGLHTS_MAX_MASK_TEXTURES];
	deoglPixelBuffer::Ref pPixBufMasks[OGLHTS_MAX_MASK_TEXTURES];
	
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
	deoglRHTSector(deoglRHeightTerrain &heightTerrain, const deHeightTerrainSector &sector);
	
protected:
	/** Clean up height terrain sector. */
	virtual ~deoglRHTSector();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Height terrain. */
	inline deoglRHeightTerrain &GetHeightTerrain() const{return pHeightTerrain;}
	
	/** Index. */
	inline int GetIndex() const{return pIndex;}
	
	/** Set index. */
	void SetIndex(int index);
	
	/** Sector coordinates. */
	inline const decPoint &GetCoordinates() const{return pCoordinates;}
	
	/** Base height. */
	inline float GetBaseHeight() const{return pBaseHeight;}
	
	/** Scaling. */
	inline float GetScaling() const{return pScaling;}
	
	/** Calculate world matrix. */
	decDMatrix CalcWorldMatrix() const;
	decDMatrix CalcWorldMatrix(const decDVector &referencePosition) const;
	
	/** Calculate world position. */
	decDVector CalcWorldPosition() const;
	decDVector CalcWorldPosition(const decDVector &referencePosition) const;
	
	/** Add to world compute. */
	void AddToWorldCompute(deoglWorldCompute &worldCompute);
	
	/** Update world compute. */
	void UpdateWorldCompute();
	
	/** Remove from world compute. */
	void RemoveFromWorldCompute();
	
	/** Prepare for render. */
	void PrepareForRender();
	
	
	
	/** Number of textures. */
	inline int GetTextureCount() const{return pTextureCount;}
	
	/** Texture at index. */
	deoglHTSTexture &GetTextureAt(int index) const;
	
	/** Terrain height map mask textures. */
	inline deoglTexture **GetMaskTextures(){return pMasks;}
	
	/** Sector is valid. */
	inline bool GetValid() const{return pValid;}
	
	/** Textures are valid. */
	inline bool GetValidTextures() const{return pValidTextures;}
	
	/** Texture requires prepare for render. */
	void TextureRequirePrepareForRender();
	
	
	
	/** Heights. */
	inline float *GetHeights() const{return pHeights;}
	
	/** Minimum height. */
	inline float GetMinHeight() const{return pMinHeight;}
	
	/** Maximum height. */
	inline float GetMaxHeight() const{return pMaxHeight;}
	
	/** Height changed. */
	void HeightChanged(const deHeightTerrainSector &sector, const decPoint &from, const decPoint &to);
	
	/** Sector changed. */
	void SectorChanged(const deHeightTerrainSector &sector);
	
	
	
	/** Number of clusters. */
	inline int GetClusterCount() const{return pClusterCount;}
	
	/** Cluster at location. */
	deoglHTSCluster &GetClusterAt(int x, int z) const;
	deoglHTSCluster &GetClusterAt(const decPoint &coordinate) const;
	
	/** List of clusters. */
	inline deoglHTSCluster *GetClusters() const{return pClusters;}
	
	/** Clusters update world compute element textures. */
	void ClustersUpdateWorldComputeElementTextures();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pCreateArrays(const deHeightTerrainSector &sector);
	void pCreateHeightMap(const deHeightTerrainSector &sector);
	
	void pSetTextureCount(int count);
	
	void pDropMaskPixelBuffers();
	
	void pSyncSector(const deHeightTerrainSector &sector);
	void pSyncTextures(const deHeightTerrainSector &sector);
	void pSyncMaskTextures(const deHeightTerrainSector &sector);
	void pSyncHeightMap(const deHeightTerrainSector &sector, const decPoint &from, const decPoint &to);
	
	void pUpdateMaskTextures();
	void pCreateVBODataPoints1();
	void pCreateVBODataFaces();
	void pUpdateHeightMap();
};

#endif
