/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLOCCLUSIONTRACING_H_
#define _DEOGLOCCLUSIONTRACING_H_

#include "../collidelist/deoglCollideList.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../tbo/deoglDynamicTBOFloat32.h"
#include "../tbo/deoglDynamicTBOUInt32.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../utils/bvh/deoglBVH.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decTimer.h>

class deoglRenderThread;
class deoglDynamicTBO;
class deoglROcclusionMesh;
class deoglRWorld;


/**
 * \brief Occlusion tracing.
 * 
 * Stores occlusion meshes for shaders to apply ray tracing to.
 */
class deoglOcclusionTracing{
public:
	/** \brief Occlusion meshes. */
	struct sOccMesh{
		deoglROcclusionMesh *occlusionMesh;
		int indexNodes;
		int indexFaces;
		int indexVertices;
	};
	
	/** \brief Occlusion mesh instance. */
	struct sOccMeshInstance{
		decMatrix matrix;
		int indexMesh;
		int indexMatrix;
		int indexInstance;
	};
	
	/** \brief UBO Parameters. */
	enum eUBOTracingParameter{
		eutpSampleImageScale, // vec2: scale factor for sample image size
		eutpProbeCount, // int: count of probes to update
		eutpRaysPerProbe, // int: count of rays per probe
		eutpProbesPerLine, // int: count of probes per line
		eutpBVHInstanceRootNode, // int
		eutpGridProbeCount, // ivec3: count of probes in grid
		eutpGridProbeSpacing, // vec3: spacing of probes in grid
		eutpProbeIndex, // ivec4[]: group of 4 probes to trace
		eutpProbePosition, // vec3[]: probe position and ray origin
		eutpRayDirection // vec3[]: ray direction
	};
	
private:
	deoglRenderThread &pRenderThread;
	
	decDVector pPosition;
	decVector pProbeSpacing;
	decPoint3 pProbeCount;
	decVector pProbeOrigin;
	int pStrideProbeCount;
	int pRaysPerProbe;
	int pMaxUpdateProbeCount;
	int pProbesPerLine;
	decPoint pSampleImageSize;
	
	deoglCollideList pCollideList;
	
	sOccMesh *pOccMeshes;
	int pOccMeshCount;
	int pOccMeshSize;
	
	sOccMeshInstance *pOccMeshInstances;
	int pOccMeshInstanceCount;
	int pOccMeshInstanceSize;
	
	deoglBVH pBVHInstances;
	deoglBVH::sBuildPrimitive *pBVHBuildPrimitives;
	int pBVHBuildPrimitiveSize;
	
	deoglDynamicTBOFloat32 pTBONodeBox;
	deoglDynamicTBOUInt32 pTBOIndex;
	deoglDynamicTBOUInt32 pTBOInstance;
	deoglDynamicTBOFloat32 pTBOMatrix;
	deoglDynamicTBOUInt32 pTBOFace;
	deoglDynamicTBOFloat32 pTBOVertex;
	int pBVHInstanceRootNode;
	
	decTimer pTimerUpdateProbe;
	float pElapsedUpdateProbe;
	float pUpdateProbeInterval;
	int *pUpdateProbes;
	int pUpdateProbeCount;
	
	decVector pSphericalFibonacci[ 64 ];
	deoglSPBlockUBO *pUBOTracing;
	deoglTexture pTexRayOrigin;
	deoglTexture pTexRayDirection;
	deoglFramebuffer pFBORay;
	
	deoglTexture pTexProbeOcclusion;
	deoglTexture pTexProbeDistance;
	deoglFramebuffer pFBOProbe;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create occlusion tracing. */
	deoglOcclusionTracing( deoglRenderThread &renderThread );
	
	/** \brief Clean up occlusion tracing. */
	~deoglOcclusionTracing();
	/*@}*/
	
	
	
	/** @name Management */
	/*@{*/
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Probe spacing. */
	inline const decVector &GetProbeSpacing() const{ return pProbeSpacing; }
	
	/** \brief Probe count. */
	inline const decPoint3 &GetProbeCount() const{ return pProbeCount; }
	
	/** \brief Rays per probe. */
	inline int GetRaysPerProbe() const{ return pRaysPerProbe; }
	
	/** \brief Size of sample image. */
	inline const decPoint &GetSampleImageSize() const{ return pSampleImageSize; }
	
	/** \brief Probes per line. */
	inline int GetProbesPerLine() const{ return pProbesPerLine; }
	
	/** \brief World position closest to grid. */
	decDVector WorldClosestGrid( const decDVector &position ) const;
	
	/** \brief Grid coordinates for probe index. */
	decPoint3 ProbeIndex2GridCoord( int index ) const;
	
	/** \brief Probe index for grid coordinates. */
	int GridCoord2ProbeIndex( const decPoint3 &coord ) const;
	
	/** \brief Probe index for grid coordinates. */
	decVector Grid2Local( const decPoint3 &coord ) const;
	
	
	
	/** \brief TBO for BVH node boundaries. */
	inline const deoglDynamicTBOFloat32 &GetTBONodeBox() const{ return pTBONodeBox; }
	
	/** \brief TBO for BVH node indices. */
	inline const deoglDynamicTBOUInt32 &GetTBOIndex() const{ return pTBOIndex; }
	
	/** \brief TBO for instance data. */
	inline const deoglDynamicTBOUInt32 &GetTBOInstance() const{ return pTBOInstance; }
	
	/** \brief TBO for instance matrices. */
	inline const deoglDynamicTBOFloat32 &GetTBOMatrix() const{ return pTBOMatrix; }
	
	/** \brief TBO for mesh faces. */
	inline const deoglDynamicTBOUInt32 &GetTBOFace() const{ return pTBOFace; }
	
	/** \brief TBO for mesh vertices. */
	inline const deoglDynamicTBOFloat32 &GetTBOVertex() const{ return pTBOVertex; }
	
	/** \brief Index of bvh instance root node. */
	inline int GetBVHInstanceRootNode() const{ return pBVHInstanceRootNode; }
	
	/** \brief List of index of probes to update. */
	inline int *GetUpdateProbes() const{ return pUpdateProbes; }
	
	/** \brief Count of probes to update. */
	inline int GetUpdateProbeCount() const{ return pUpdateProbeCount; }
	
	
	
	/** \brief UBO Tracing parameters. */
	inline deoglSPBlockUBO *GetUBOTracing(){ return pUBOTracing; }
	
	/** \brief Ray origin texture. */
	inline const deoglTexture &GetTextureRayOrigin() const{ return pTexRayOrigin; }
	
	/** \brief Ray direction texture. */
	inline const deoglTexture &GetTextureRayDirection() const{ return pTexRayDirection; }
	
	/** \brief Ray FBO. */
	inline deoglFramebuffer &GetFBORay(){ return pFBORay; }
	
	
	
	/** \brief Occlusion probe texture. */
	inline const deoglTexture &GetTextureProbeOcclusion() const{ return pTexProbeOcclusion; }
	
	/** \brief Distance probe texture. */
	inline const deoglTexture &GetTextureProbeDistance() const{ return pTexProbeDistance; }
	
	/** \brief Probe fbo. */
	inline deoglFramebuffer &GetFBOProbe(){ return pFBOProbe; }
	
	/** \brief Update. */
	void Update( deoglRWorld &world, const decDVector &position );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pFindComponents( deoglRWorld &world );
	void pAddOcclusionMeshes();
	void pAddOcclusionMesh( const decMatrix &matrix, deoglROcclusionMesh *occlusionMesh );
// 	bool pAddFace( const decVector &v1, const decVector &v2, const decVector &v3, bool doubleSided );
	void pWriteTBOs();
	void pUpdatePosition( const decDVector &position );
	void pTraceProbes();
	void pFindProbesToUpdate();
	void pInitSphericalFibonacci();
	void pPrepareUBOState();
	void pPrepareRayTexturesAndFBO();
	void pPrepareProbeTexturesAndFBO();
};

#endif
