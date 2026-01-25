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

#ifndef _DEOGLMODELLOD_H_
#define _DEOGLMODELLOD_H_

#include "../deoglBasics.h"
#include "../vbo/deoglVBOLayout.h"
#include "../vbo/deoglSharedVBOBlock.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decTList.h>
#include <dragengine/common/collection/decTUniqueList.h>

class deoglRModel;
class deoglModelFace;
class deoglModelTexture;
class deoglModelOctree;
class deoglModelLODTexCoordSet;
class deoglModelLODVertPosSet;
class deoglSharedSPB;
class deoglSharedSPBRTIGroupList;
class deoglGIBVHLocal;

class deGraphicOpenGl;
class deModel;
class deModelLOD;
class decBaseFileWriter;
class decBaseFileReader;



struct oglModelWeight{
	int bone;
	float weight;
};

struct oglModelPosition{
	decVector position;
	decVector normal;
	int weights;
};

struct oglModelVertex{
	int position;
	int texcoord;
	int normal;
	int tangent;
};



/**
 * Model LOD Mesh.
 */
class deoglModelLOD{
public:
	/** \brief Type holding unique reference. */
	using Ref = deTUniqueReference<deoglModelLOD>;
	
	deoglRModel &pModel;
	const int pLODIndex;
	
	decTUniqueList<deoglModelTexture> pTextures;
	decTList<oglModelPosition> pPositions;
	decTList<decVector2> pTexCoords;
	decTList<decVector> pNormals;
	decTList<decVector> pTangents;
	decTList<bool> pNegateTangents;
	decTList<oglModelWeight> pWeightsEntries;
	decTList<int> pWeightsCounts;
	decTList<oglModelVertex> pVertices;
	decTList<deoglModelFace> pFaces;
	decTList<deoglModelLODTexCoordSet> pTexCoordSets;
	decTList<deoglModelLODVertPosSet> pVertPosSets;
	int pVertPosSetPosCount;
	
	deoglSharedVBOBlock::Ref pVBOBlock;
	deoglSharedVBOBlock::Ref pVBOBlockPositionWeight;
	deoglSharedVBOBlock::Ref pVBOBlockCalcNormalTangent;
	deoglSharedVBOBlock::Ref pVBOBlockWriteSkinnedVBO;
	deoglSharedVBOBlock::Ref pVBOBlockWithWeight;
	deoglSharedVBOBlock::Ref pVBOBlockVertPosSet;
	GLuint pIBO;
	deoglVBOLayout::eIndexTypes pIBOType;
	
	bool pDoubleSided;
	bool pDecal;
	
	deoglModelOctree *pOctree;
	
	float pMaxError;
	float pAvgError;
	
	deoglGIBVHLocal *pGIBVHLocal;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new model lod. */
	deoglModelLOD(deoglRModel &model, int lodIndex, const deModel &engModel);
	/** Creates a new model lod from cache. */
	deoglModelLOD(deoglRModel &model, int lodIndex, decBaseFileReader &cacheReader);
	/** Cleans up the model lod. */
	~deoglModelLOD();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the parent model. */
	inline deoglRModel &GetModel() const{ return pModel; }
	/** Retrieves the lod index. */
	inline int GetLODIndex() const{ return pLODIndex; }
	
	
	
	/** Prepare VBO block. */
	void PrepareVBOBlock();
	// void PrepareVBOBlockPositionWeight();
	// void PrepareVBOBlockCalcNormalTangent();
	// void PrepareVBOBlockWriteSkinnedVBO();
	void PrepareVBOBlockWithWeight();
	void PrepareVBOBlockVertPosSet();
	
	/** VBO block. */
	inline const deoglSharedVBOBlock::Ref &GetVBOBlock() const{ return pVBOBlock; }
	// inline const deoglSharedVBOBlock::Ref &GetVBOBlockPositionWeight() const{ return pVBOBlockPositionWeight; }
	// inline const deoglSharedVBOBlock::Ref &GetVBOBlockCalcNormalTangent() const{ return pVBOBlockCalcNormalTangent; }
	// inline const deoglSharedVBOBlock::Ref &GetVBOBlockWriteSkinnedVBO() const{ return pVBOBlockWriteSkinnedVBO; }
	inline const deoglSharedVBOBlock::Ref &GetVBOBlockWithWeight() const{ return pVBOBlockWithWeight; }
	inline const deoglSharedVBOBlock::Ref &GetVBOBlockVertPosSet() const{ return pVBOBlockVertPosSet; }
	
	/** Index buffer object. */
	GLuint GetIBO();
	
	/** Index buffer object data type. */
	inline deoglVBOLayout::eIndexTypes GetIBOType() const{ return pIBOType; }
	
	
	
	/** Textures. */
	inline const decTUniqueList<deoglModelTexture> &GetTextures() const{ return pTextures; }
	
	/** Number of textures. */
	inline int GetTextureCount() const{ return pTextures.GetCount(); }
	
	/** Texture at index. */
	deoglModelTexture &GetTextureAt(int index);
	const deoglModelTexture &GetTextureAt(int index) const;
	
	/** Texture render task instance group. */
	deoglSharedSPBRTIGroupList &GetSharedSPBRTIGroupListAt(int texture) const;
	
	/** Model has double sided textures. */
	inline bool GetDoubleSided() const{ return pDoubleSided; }
	
	/** Model has decal textures. */
	inline bool GetDecal() const{ return pDecal; }
	
	/** Positions. */
	inline const decTList<oglModelPosition> &GetPositions() const{ return pPositions; }
	
	/** Texture coordinates. */
	inline const decTList<decVector2> &GetTextureCoordinates() const{ return pTexCoords; }
	
	/** Normals. */
	inline const decTList<decVector> &GetNormals() const{ return pNormals; }
	
	/** Tangents. */
	inline const decTList<decVector> &GetTangents() const{ return pTangents; }
	
	/** Negate tangents. */
	inline const decTList<bool> &GetNegateTangents() const{ return pNegateTangents; }
	
	/** Weights entries. */
	inline const decTList<oglModelWeight> &GetWeightsEntries() const{ return pWeightsEntries; }
	
	/** Weights entries count list. */
	inline const decTList<int> &GetWeightsCounts() const{ return pWeightsCounts; }
	
	/** Vertices. */
	inline const decTList<oglModelVertex> &GetVertices() const{ return pVertices; }
	
	/** Faces. */
	inline const decTList<deoglModelFace> &GetFaces() const{ return pFaces; }
	
	/** Texture coordinate sets. */
	inline const decTList<deoglModelLODTexCoordSet> &GetTextureCoordSets() const{ return pTexCoordSets; }
	
	/** Vertex position sets. */
	inline const decTList<deoglModelLODVertPosSet> &GetVertexPositionSets() const{ return pVertPosSets; }
	
	/** Octree or \em NULL if there is none. */
	inline deoglModelOctree *GetOctree() const{ return pOctree; }
	
	/** Prepare octree if not existing already. */
	void PrepareOctree();
	
	/** Retrieves the maximum error in meters compared to LOD 0. */
	inline float GetMaxError() const{ return pMaxError; }
	/** Retrieves the average error in meters compared to LOD 0. */
	inline float GetAvgError() const{ return pAvgError; }
	
	/** Load from cache file. */
	void LoadFromCache(decBaseFileReader &reader);
	/** Save to cache file. */
	void SaveToCache(decBaseFileWriter &writer);
	
	
	
	/** GI Local BVH or NULL. */
	inline deoglGIBVHLocal *GetGIBVHLocal() const{ return pGIBVHLocal; }
	
	/** Prepare GI Local BVH if not build yet. */
	void PrepareGILocalBVH();
	/*@}*/
	
private:
	void pCleanUp();
	void pCalcTangents();
	void pBuildArrays(const deModel &engModel);
	void pCalcErrorMetrics(const deModel &engModel);
	void pOptimizeVertexCache();
	void pWriteVBOData();
	// void pWriteVBODataPositionWeight();
	// void pWriteVBODataCalcNormalTangent();
	// void pWriteVBODataWriteSkinnedVBO();
	void pWriteVBODataWithWeight();
	void pWriteVBOBlockVertPosSet();
};

#endif
