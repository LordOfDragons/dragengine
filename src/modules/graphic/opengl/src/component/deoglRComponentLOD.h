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

#ifndef _DEOGLRCOMPONENTLOD_H_
#define _DEOGLRCOMPONENTLOD_H_

#include "../deoglBasics.h"
#include "../memory/consumption/deoglMemoryConsumptionGPUUse.h"
#include "../rendering/task/config/deoglRenderTaskConfig.h"
#include "../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../skin/deoglSkinTexture.h"
#include "../skin/pipeline/deoglSkinTexturePipelines.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>

class deoglFramebuffer;
class deoglModelLOD;
class deoglRComponent;
class deoglTexture;
class deoglVAO;
class deoglVBOLayout;
class deoglSharedVBOBlock;
class deoglGIBVHDynamic;



/**
 * Render component LOD.
 * 
 * The VAO of the component contains the following bindings:
 * - input 0: vec3 position
 * - input 1: vec3 normal
 * - input 2: vec2 texture-coordinates
 * - input 3: vec3 tangent
 */
class deoglRComponentLOD : public deObject{
public:
	deoglRComponent &pComponent;
	const int pLODIndex;
	
	deoglSPBlockSSBO::Ref pVBO;
	deoglVAO *pVAO;
	deoglVBOLayout *pVBOLayout;
	const deoglSharedVBOBlock *pVBOBlock;
	
	oglMatrix3x4 *pWeights;
	
	oglVector3 *pPositions;
	oglVector3 *pRealNormals;
	oglVector3 *pNormals;
	oglVector3 *pTangents;
	oglVector3 *pFaceNormals;
	
	bool pDirtyModelWeights;
	bool pDirtyModelPositions;
	bool pDirtyModelNorTan;
	bool pDirtyDataWeights;
	bool pDirtyDataPositions;
	bool pDirtyDataNorTan;
	
	bool pDirtyVBO;
	bool pDirtyVAO;
	
	deoglMemoryConsumptionGPUUse pMemUse;
	
	deoglSPBlockSSBO::Ref pSSBOWeightMatrices;
	GLuint pVBOTransformVertices;
	GLuint pTBOTransformVertices;
	deoglTexture *pTexTransformNormTan;
	deoglFramebuffer *pFBOCalcNormalTangent;
	
	deoglGIBVHDynamic *pGIBVHDynamic;
	bool pDirtyGIBVHPositions;
	
	deoglRenderTaskConfig pRenderTaskConfigs[ 6 ];
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render component lod. */
	deoglRComponentLOD( deoglRComponent &component, int lodIndex );
	
	/** Clean up render component lod. */
	virtual ~deoglRComponentLOD();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Parent component. */
	inline deoglRComponent &GetComponent() const{ return pComponent; }
	
	/** LOD index. */
	inline int GetLODIndex() const{ return pLODIndex; }
	
	/** Get ModelLOD or NULL. */
	deoglModelLOD *GetModelLOD() const;
	
	/** Get ModelLOD or throws exception. */
	deoglModelLOD &GetModelLODRef() const;
	
	
	
	/** Weights for debuging purpose. */
	inline oglMatrix3x4 *GetWeights() const{ return pWeights; }
	
	/**
	 * Retrieves the face normals.
	 * \details Valid only after calling PrepareNormalsTangents and faces exist.
	 */
	inline oglVector3 *GetFaceNormals() const{ return pFaceNormals; }
	
	/**
	 * Retrieves the positions.
	 * \details Valid only after calling PreparePositions and positions exist.
	 */
	inline oglVector3 *GetPositions() const{ return pPositions; }
	
	/**
	 * Retrieves the real normals.
	 * \details Valid only after calling PrepareNormalsTangents and normals exist.
	 */
	inline oglVector3 *GetRealNormals() const{ return pRealNormals; }
	
	/**
	 * Retrieves the normals.
	 * \details Valid only after calling PrepareNormalsTangents and normals exist.
	 */
	inline oglVector3 *GetNormals() const{ return pNormals; }
	
	/**
	 * Retrieves the tangents.
	 * \details Valid only after calling PrepareNormalsTangents and tangents exist.
	 */
	inline oglVector3 *GetTangents() const{ return pTangents; }
	
	
	
	/** Prepare dynamic weights if dirty. */
	void PrepareWeights();
	
	/** Prepare dynamic positions if dirty. */
	void PreparePositions();
	
	/** Prepare dynamic normals and tangents if dirty. */
	void PrepareNormalsTangents();
	
	
	
	/** Point offset or 0 if not using a shared vao. */
	int GetPointOffset() const;
	
	/** Index offset or 0 if not using a shared vao. */
	int GetIndexOffset() const;
	
	/** VBO. */
	inline const deoglSPBlockSSBO::Ref &GetVBO() const{ return pVBO; }
	
	/** VAO. */
	inline deoglVAO *GetVAO() const{ return pVAO; }
	
	/** VAO to use. Can be dynamic VAO or static model VAO. */
	deoglVAO *GetUseVAO() const;
	
	/** Invalidate VAO. */
	void InvalidateVAO();
	
	/** Invalidate VBO. */
	void InvalidateVBO();
	
	/** Update the VBO if required. */
	void UpdateVBO();
	
	/** Free VBO if existing. */
	void FreeVBO();
	
	
	
	/** Update VBO on the CPU which is slow slow but accurate. */
	void UpdateVBOOnCPU();
	
	/** Write weight matrices to the SSBO. */
	void WriteWeightMatricesSSBO();
	
	/** Build vertex position set params. */
	void BuildVertexPositionSetParams();
	
	/** Update VBO on the GPU using the fast but approximate method. */
	void UpdateVBOOnGPUApproximate();
	
	/** Approximately transform vertices, normals and tangents on the GPU. */
	void GPUApproxTransformVNT();
	
	
	
	/** GI Dynamic BVH or NULL. */
	inline deoglGIBVHDynamic *GetGIBVHDynamic() const{ return pGIBVHDynamic; }
	
	/** Prepare GI Dynamic BVH if not build yet. */
	void PrepareGIDynamicBVH();
	
	/** Drop GI Dynamic BVH if present. */
	void DropGIDynamicBVH();
	
	
	
	/** Render task configuration or NULL. */
	const deoglRenderTaskConfig *GetRenderTaskConfig( deoglSkinTexturePipelines::eTypes type ) const;
	
	/** Update render task configuration. */
	void UpdateRenderTaskConfigurations();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pEnsureVBO();
	void pBuildVBO( const deoglModelLOD &modelLOD );
	void pWriteVBOData( const deoglModelLOD &modelLOD );
	void pUpdateVAO( deoglModelLOD &modelLOD );
	
	void pCalculateWeights( const deoglModelLOD &modelLOD );
	void pTransformVertices( const deoglModelLOD &modelLOD );
	void pCalculateNormalsAndTangents( const deoglModelLOD &modelLOD );
	
	void pPrepareVBOLayout( const deoglModelLOD &modelLOD );
	
	void pUpdateRenderTaskConfig( deoglRenderTaskConfig &config, deoglSkinTexturePipelines::eTypes type,
		int renderTaskFlags, int renderTaskFlagMask, bool shadow );
};

#endif
