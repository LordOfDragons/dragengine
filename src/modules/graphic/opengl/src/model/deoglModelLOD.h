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

#ifndef _DEOGLMODELLOD_H_
#define _DEOGLMODELLOD_H_

#include "../deoglBasics.h"
#include "../vbo/deoglVBOLayout.h"

#include <dragengine/common/math/decMath.h>

class deoglRModel;
class deoglModelFace;
class deoglModelTexture;
class deoglModelOctree;
class deoglModelLODTexCoordSet;
class deoglSharedVBOBlock;
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
	int weight;
};

struct oglModelVertex{
	int position;
	int texcoord;
	int normal;
	int tangent;
};



/**
 * @brief Model LOD Mesh.
 */
class deoglModelLOD{
public:
	deoglRModel &pModel;
	const int pLODIndex;
	
	deoglModelTexture *pTextures;
	int pTextureCount;
	
	oglModelPosition *pPositions;
	int pPositionCount;
	
	decVector2 *pTexCoords;
	int pTexCoordCount;
	
	decVector *pNormals;
	int pNormalCount;
	
	decVector *pTangents;
	bool *pNegateTangents;
	int pTangentCount;
	
	oglModelWeight *pWeightsEntries;
	int pWeightsEntryCount;
	int *pWeightsCounts;
	int pWeightsCount;
	
	oglModelVertex *pVertices;
	int pVertexCount;
	
	deoglModelFace *pFaces;
	int pFaceCount;
	
	deoglModelLODTexCoordSet *pTexCoordSets;
	int pTexCoordSetCount;
	
	deoglSharedVBOBlock *pVBOBlock;
	deoglSharedVBOBlock *pVBOBlockPositionWeight;
	deoglSharedVBOBlock *pVBOBlockCalcNormalTangent;
	deoglSharedVBOBlock *pVBOBlockWriteSkinnedVBO;
	deoglSharedVBOBlock *pVBOBlockWithWeight;
	GLuint pIBO;
	deoglVBOLayout::eIndexTypes pIBOType;
	
	bool pDoubleSided;
	bool pDecal;
	
	deoglModelOctree *pOctree;
	
	float pMaxError;
	float pAvgError;
	
	deoglGIBVHLocal *pGIBVHLocal;
	
	
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new model lod. */
	deoglModelLOD( deoglRModel &model, int lodIndex, const deModel &engModel );
	/** Creates a new model lod from cache. */
	deoglModelLOD( deoglRModel &model, int lodIndex, decBaseFileReader &cacheReader );
	/** Cleans up the model lod. */
	~deoglModelLOD();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the parent model. */
	inline deoglRModel &GetModel() const{ return pModel; }
	/** Retrieves the lod index. */
	inline int GetLODIndex() const{ return pLODIndex; }
	
	
	
	/** Retrieves the VBO block. */
	deoglSharedVBOBlock *GetVBOBlock();
	
	/** Retrieves the position weight VBO block. */
	deoglSharedVBOBlock *GetVBOBlockPositionWeight();
	
	/** Retrieves the calculate normal tangent VBO block. */
	deoglSharedVBOBlock *GetVBOBlockCalcNormalTangent();
	
	/** Retrieves the write skinned vbo VBO block. */
	deoglSharedVBOBlock *GetVBOBlockWriteSkinnedVBO();
	
	/** Retrieves the vbo block with weight. */
	deoglSharedVBOBlock *GetVBOBlockWithWeight();
	
	/** Index buffer object. */
	GLuint GetIBO();
	
	/** Index buffer object data type. */
	inline deoglVBOLayout::eIndexTypes GetIBOType() const{ return pIBOType; }
	
	
	
	/** Number of textures. */
	inline int GetTextureCount() const{ return pTextureCount; }
	
	/** Texture at index. */
	const deoglModelTexture &GetTextureAt( int index ) const;
	
	/** Texture render task instance group. */
	deoglSharedSPBRTIGroupList &GetSharedSPBRTIGroupListAt( int texture ) const;
	
	/** Model has double sided textures. */
	inline bool GetDoubleSided() const{ return pDoubleSided; }
	
	/** Model has decal textures. */
	inline bool GetDecal() const{ return pDecal; }
	
	/** Retrieves the positions. */
	inline oglModelPosition *GetPositions() const{ return pPositions; }
	/** Retrieves the number of positions. */
	inline int GetPositionCount() const{ return pPositionCount; }
	
	/** Retrieves the texture coordinates. */
	inline decVector2 *GetTextureCoordinates() const{ return pTexCoords; }
	/** Retrieves the number of texture coordinates. */
	inline int GetTextureCoordinatesCount() const{ return pTexCoordCount; }
	
	/** Retrieves the normals. */
	inline decVector *GetNormals() const{ return pNormals; }
	/** Retrieves the normal count. */
	inline int GetNormalCount() const{ return pNormalCount; }
	
	/** Retrieves the tangents. */
	inline decVector *GetTangents() const{ return pTangents; }
	/** Retrieves the negate tangents. */
	inline bool *GetNegateTangents() const{ return pNegateTangents; }
	/** Retrieves the tangent count. */
	inline int GetTangentCount() const{ return pTangentCount; }
	
	/** Retrieves the weights entries. */
	inline oglModelWeight *GetWeightsEntries() const{ return pWeightsEntries; }
	/** Retrieves the number of weights entries. */
	inline int GetWeightsEntryCount() const{ return pWeightsEntryCount; }
	/** Retrieves the weights entries count list. */
	inline int *GetWeightsCounts() const{ return pWeightsCounts; }
	/** Retrieves the number of weights. */
	inline int GetWeightsCount() const{ return pWeightsCount; }
	
	/** Retrieves the vertices. */
	inline oglModelVertex *GetVertices() const{ return pVertices; }
	/** Retrieves the vertex count. */
	inline int GetVertexCount() const{ return pVertexCount; }
	
	/** Retrieves the faces. */
	inline deoglModelFace *GetFaces() const{ return pFaces; }
	/** Retrieves the number of faces. */
	inline int GetFaceCount() const{ return pFaceCount; }
	
	/** Retrieves the number of texture coordinate sets. */
	inline int GetTextureCoordinateSetCount() const{ return pTexCoordSetCount; }
	/** Retrieves the texture coordinate set at the given index. */
	const deoglModelLODTexCoordSet &GetTextureCoordSetAt( int index ) const;
	
	/** Octree or \em NULL if there is none. */
	inline deoglModelOctree *GetOctree() const{ return pOctree; }
	
	/** Prepare octree if not existing already. */
	void PrepareOctree();
	
	/** Retrieves the maximum error in meters compared to LOD 0. */
	inline float GetMaxError() const{ return pMaxError; }
	/** Retrieves the average error in meters compared to LOD 0. */
	inline float GetAvgError() const{ return pAvgError; }
	
	/** Load from cache file. */
	void LoadFromCache( decBaseFileReader &reader );
	/** Save to cache file. */
	void SaveToCache( decBaseFileWriter &writer );
	
	
	
	/** GI Local BVH or NULL. */
	inline deoglGIBVHLocal *GetGIBVHLocal() const{ return pGIBVHLocal; }
	
	/** Prepare GI Local BVH if not build yet. */
	void PrepareGILocalBVH();
	/*@}*/
	
private:
	void pCleanUp();
	void pCalcTangents();
	void pBuildArrays( const deModel &engModel );
	void pCalcErrorMetrics( const deModel &engModel );
	void pOptimizeVertexCache();
	void pWriteVBOData();
	void pWriteVBODataPositionWeight();
	void pWriteVBODataCalcNormalTangent();
	void pWriteVBODataWriteSkinnedVBO();
	void pWriteVBODataWithWeight();
};

#endif
