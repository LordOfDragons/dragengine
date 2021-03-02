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

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>

class deoglFramebuffer;
class deoglModelLOD;
class deoglRComponent;
class deoglTexture;
class deoglVAO;
class deoglVBOLayout;
class deoglSharedVBOBlock;


/**
 * \brief Render component LOD.
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
	
	deoglVBOpnt *pVBOData;
	int pVBOPointCount;
	int pVBOPointSize;
	
	GLuint pVBO;
	int pSolidFaceCount;
	int pFaceCount;
	deoglVAO *pVAO;
	deoglVBOLayout *pVBOLayout;
	const deoglSharedVBOBlock *pVBOBlock;
	
	oglMatrix3x4 *pWeights;
	
	oglVector *pPositions;
	oglVector *pRealNormals;
	oglVector *pNormals;
	oglVector *pTangents;
	oglVector *pFaceNormals;
	
	bool pDirtyModelWeights;
	bool pDirtyModelPositions;
	bool pDirtyModelNorTan;
	bool pDirtyDataWeights;
	bool pDirtyDataPositions;
	bool pDirtyDataNorTan;
	
	bool pDirtyVBO;
	bool pDirtyVAO;
	
	int pMemoryConsumptionGPU;
	
	GLuint pVBOWeightMatrices;
	GLuint pTBOWeightMatrices;
	GLuint pVBOTransformVertices;
	GLuint pTBOTransformVertices;
	deoglTexture *pTexTransformNormTan;
	deoglFramebuffer *pFBOCalcNormalTangent;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render component lod. */
	deoglRComponentLOD( deoglRComponent &component, int lodIndex );
	
	/** \brief Clean up render component lod. */
	virtual ~deoglRComponentLOD();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent component. */
	inline deoglRComponent &GetComponent() const{ return pComponent; }
	
	/** \brief LOD index. */
	inline int GetLODIndex() const{ return pLODIndex; }
	
	/** \brief Get ModelLOD or NULL. */
	deoglModelLOD *GetModelLOD() const;
	
	
	
	/** \brief Weights for debuging purpose. */
	inline oglMatrix3x4 *GetWeights() const{ return pWeights; }
	
	/**
	 * \brief Retrieves the face normals.
	 * \details Valid only after calling PrepareNormalsTangents and faces exist.
	 */
	inline oglVector *GetFaceNormals() const{ return pFaceNormals; }
	
	/**
	 * \brief Retrieves the positions.
	 * \details Valid only after calling PreparePositions and positions exist.
	 */
	inline oglVector *GetPositions() const{ return pPositions; }
	
	/**
	 * \brief Retrieves the real normals.
	 * \details Valid only after calling PrepareNormalsTangents and normals exist.
	 */
	inline oglVector *GetRealNormals() const{ return pRealNormals; }
	
	/**
	 * \brief Retrieves the normals.
	 * \details Valid only after calling PrepareNormalsTangents and normals exist.
	 */
	inline oglVector *GetNormals() const{ return pNormals; }
	
	/**
	 * \brief Retrieves the tangents.
	 * \details Valid only after calling PrepareNormalsTangents and tangents exist.
	 */
	inline oglVector *GetTangents() const{ return pTangents; }
	
	
	
	/** \brief Prepare dynamic weights if dirty. */
	void PrepareWeights();
	
	/** \brief Prepare dynamic positions if dirty. */
	void PreparePositions();
	
	/** \brief Prepare dynamic normals and tangents if dirty. */
	void PrepareNormalsTangents();
	
	
	
	/** \brief Number of points. */
	inline int GetPointCount() const{ return pVBOPointCount; }
	
	/** \brief VBO data in client memory. */
	inline deoglVBOpnt *GetVBOData() const{ return pVBOData; }
	
	/** \brief Point offset or 0 if not using a shared vao. */
	int GetPointOffset() const;
	
	/** \brief Index offset or 0 if not using a shared vao. */
	int GetIndexOffset() const;
	
	/** \brief VBO. */
	inline GLuint GetVBO() const{ return pVBO; }
	
	/** \brief VAO. */
	inline deoglVAO *GetVAO() const{ return pVAO; }
	
	/** \brief Invalidate VAO. */
	void InvalidateVAO();
	
	/** \brief Invalidate VBO. */
	void InvalidateVBO();
	
	/** \brief Update the VBO if required. */
	void UpdateVBO();
	
	/** \brief Free VBO if existing. */
	void FreeVBO();
	
	
	
	/** \brief Update VBO on the CPU which is slow slow but accurate. */
	void UpdateVBOOnCPU();
	
	
	
	/** \brief Update VBO on the GPU using the accurate but slower method. */
	void UpdateVBOOnGPUAccurate();
	
	/** \brief Write weight matrices to the TBO. */
	void WriteWeightMatricesTBO();
	
	/** \brief Transform vertices on the GPU. */
	void GPUTransformVertices();
	
	/** \brief Calculate normals and tangets on the GPU. */
	void GPUCalcNormalTangents();
	
	/** \brief Write final VBO from the GPU calculated data to be used by rendering. */
	void GPUWriteRenderVBO();
	
	
	
	/** \brief Update VBO on the GPU using the fast but approximate method. */
	void UpdateVBOOnGPUApproximate();
	
	/** \brief Approximately transform vertices, normals and tangents on the GPU. */
	void GPUApproxTransformVNT();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pBuildVBO( const deoglModelLOD &modelLOD );
	void pWriteVBOData( const deoglModelLOD &modelLOD );
	void pUpdateVAO( deoglModelLOD &modelLOD );
	
	void pCalculateWeights( const deoglModelLOD &modelLOD );
	void pTransformVertices( const deoglModelLOD &modelLOD );
	void pCalculateNormalsAndTangents( const deoglModelLOD &modelLOD );
	
	void pPrepareVBOLayout( const deoglModelLOD &modelLOD );
};

#endif
