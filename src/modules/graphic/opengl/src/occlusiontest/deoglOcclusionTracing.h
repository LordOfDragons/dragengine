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


// #define ENABLE_OCCTRACING


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
		eutpOcclusionMapSize, // int: size of occlusion map
		eutpDistanceMapSize, // int: size of distance map
		eutpOcclusionMapScale, // vec2: scale factor for occlusion map
		eutpDistanceMapScale, // vec2: scale factor for distance map
		eutpMaxProbeDistance, // float
		eutpDepthSharpness, // float
		eutpHysteresis, // float
		eutpProbeIndex, // ivec4[]: group of 4 probes to trace
		eutpProbePosition, // vec3[]: probe position and ray origin
		eutpRayDirection // vec3[]: ray direction
	};
	
private:
	deoglRenderThread &pRenderThread;
	
	decDVector pPosition;
	decVector pProbeSpacing;
	decVector pProbeSpacingInv;
	decPoint3 pProbeCount;
	decVector pProbeOrigin;
	int pStrideProbeCount;
	int pRaysPerProbe;
	int pMaxUpdateProbeCount;
	int pProbesPerLine;
	decPoint pSampleImageSize;
	int pOcclusionMapSize;
	int pDistanceMapSize;
	float pMaxProbeDistance;
	float pDepthSharpness;
	float pHysteresis;
	float pNormalBias;
	float pEnergyPreservation;
	
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
	
	int pSizeTexOcclusion;
	int pSizeTexDistance;
	deoglTexture pTexProbeOcclusion;
	deoglTexture pTexProbeDistance;
	deoglFramebuffer pFBOProbeOcclusion;
	deoglFramebuffer pFBOProbeDistance;
	
	decVector2 pOcclusionMapScale;
	decVector2 pDistanceMapScale;
	
	
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
	
	/** \brief Inverse probe spacing. */
	inline const decVector &GetProbeSpacingInverse() const{ return pProbeSpacingInv; }
	
	/** \brief Probe count. */
	inline const decPoint3 &GetProbeCount() const{ return pProbeCount; }
	
	/** \brief Probe origin. */
	inline const decVector &GetProbeOrigin() const{ return pProbeOrigin; }
	
	/** \brief Rays per probe. */
	inline int GetRaysPerProbe() const{ return pRaysPerProbe; }
	
	/** \brief Size of sample image. */
	inline const decPoint &GetSampleImageSize() const{ return pSampleImageSize; }
	
	/** \brief Size of occlusion map. */
	inline int GetOcclusionMapSize() const{ return pOcclusionMapSize; }
	
	/** \brief Size of distance map. */
	inline int GetDistanceMapSize() const{ return pDistanceMapSize; }
	
	/** \brief Occlusion map scale. */
	inline const decVector2 &GetOcclusionMapScale() const{ return pOcclusionMapScale; }
	
	/** \brief Distance map scale. */
	inline const decVector2 &GetDistanceMapScale() const{ return pDistanceMapScale; }
	
	/** \brief Normal bias. */
	inline float GetNormalBias() const{ return pNormalBias; }
	
	/** \brief Energey conversation. */
	inline float GetEnergyPreservation() const{ return pEnergyPreservation; }
	
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
	
	
	
	/** \brief Size of occlusion texture of one probe. */
	inline int GetSizeTexOcclusion() const{ return pSizeTexOcclusion; }
	
	/** \brief Size of distance texture of one probe. */
	inline int GetSizeTexDistance() const{ return pSizeTexDistance; }
	
	/** \brief Occlusion probe texture. */
	inline deoglTexture &GetTextureProbeOcclusion(){ return pTexProbeOcclusion; }
	inline const deoglTexture &GetTextureProbeOcclusion() const{ return pTexProbeOcclusion; }
	
	/** \brief Distance probe texture. */
	inline deoglTexture &GetTextureProbeDistance(){ return pTexProbeDistance; }
	inline const deoglTexture &GetTextureProbeDistance() const{ return pTexProbeDistance; }
	
	/** \brief Probe fbo occlusion. */
	inline deoglFramebuffer &GetFBOProbeOcclusion(){ return pFBOProbeOcclusion; }
	
	/** \brief Probe fbo distance. */
	inline deoglFramebuffer &GetFBOProbeDistance(){ return pFBOProbeDistance; }
	
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
