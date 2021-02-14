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
		eutpGridProbeCount, // ivec3: count of probes in grid
		eutpProbesPerLine, // int: count of probes per line
		eutpGridProbeSpacing, // vec3: spacing of probes in grid
		eutpBVHInstanceRootNode, // int
		eutpOcclusionMapScale, // vec2: scale factor for occlusion map
		eutpDistanceMapScale, // vec2: scale factor for distance map
		eutpOcclusionMapSize, // int: size of occlusion map
		eutpDistanceMapSize, // int: size of distance map
		eutpMaxProbeDistance, // float
		eutpDepthSharpness, // float
		eutpProbeIndex, // ivec4[]: group of 4 probes to trace
		eutpProbePosition, // vec3[]: probe position and ray origin
		eutpRayDirection // vec3[]: ray direction
	};
	
private:
	deoglRenderThread &pRenderThread;
	
	decVector pProbeSpacing;
	decVector pProbeSpacingInv;
	decPoint3 pProbeCount;
	decVector pProbeOrigin;
	int pStrideProbeCount;
	int pRealProbeCount;
	int pMaxRaysPerProbe;
	int pRaysPerProbe;
	int pMaxUpdateProbeCount;
	int pProbesPerLine;
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
	
	decVector *pSphericalFibonacci;
	deoglSPBlockUBO *pUBOTracing;
	
	int pSizeTexOcclusion;
	int pSizeTexDistance;
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
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** \brief Probe spacing. */
	inline const decVector &GetProbeSpacing() const{ return pProbeSpacing; }
	
	/** \brief Inverse probe spacing. */
	inline const decVector &GetProbeSpacingInverse() const{ return pProbeSpacingInv; }
	
	/** \brief Probe count. */
	inline const decPoint3 &GetProbeCount() const{ return pProbeCount; }
	
	/** \brief Probe origin. */
	inline const decVector &GetProbeOrigin() const{ return pProbeOrigin; }
	
	/** \brief Probe stride. */
	inline int GetStrideProbeCount() const{ return pStrideProbeCount; }
	
	/** \brief Real probe count. */
	inline int GetRealProbeCount() const{ return pRealProbeCount; }
	
	/** \brief Maximum rays per probe. */
	inline int GetMaxRaysPerProbe() const{ return pMaxRaysPerProbe; }
	
	/** \brief Rays per probe. */
	inline int GetRaysPerProbe() const{ return pRaysPerProbe; }
	
	/** \brief Maximum probe update count. */
	inline int GetMaxUpdateProbeCount() const{ return pMaxUpdateProbeCount; }
	
	/** \brief Size of occlusion map. */
	inline int GetOcclusionMapSize() const{ return pOcclusionMapSize; }
	
	/** \brief Size of distance map. */
	inline int GetDistanceMapSize() const{ return pDistanceMapSize; }
	
	/** \brief Occlusion map scale. */
	inline const decVector2 &GetOcclusionMapScale() const{ return pOcclusionMapScale; }
	
	/** \brief Distance map scale. */
	inline const decVector2 &GetDistanceMapScale() const{ return pDistanceMapScale; }
	
	/** \brief Max probe distance. */
	inline float GetMaxProbeDistance() const{ return pMaxProbeDistance; }
	
	/** \brief Depth sharpness. */
	inline float GetDepthSharpness() const{ return pDepthSharpness; }
	
	/** \brief Hysteresis. */
	inline float GetHysteresis() const{ return pHysteresis; }
	
	/** \brief Normal bias. */
	inline float GetNormalBias() const{ return pNormalBias; }
	
	/** \brief Energey conversation. */
	inline float GetEnergyPreservation() const{ return pEnergyPreservation; }
	
	/** \brief Probes per line. */
	inline int GetProbesPerLine() const{ return pProbesPerLine; }
	
	
	
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
	
	
	
	/** \brief Spherical fibonacci constants. */
	inline const decVector *GetSphericalFibonacci() const{ return pSphericalFibonacci; }
	
	/** \brief UBO Tracing parameters preparing it if not created yet. */
	deoglSPBlockUBO &GetUBOTracing();
	
	
	
	/** \brief Size of occlusion texture of one probe. */
	inline int GetSizeTexOcclusion() const{ return pSizeTexOcclusion; }
	
	/** \brief Size of distance texture of one probe. */
	inline int GetSizeTexDistance() const{ return pSizeTexDistance; }
	
	/** \brief Prepare ray tracing information. */
	void PrepareRayTracing( deoglRWorld &world, const decDVector &position );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pFindComponents( deoglRWorld &world, const decDVector &position );
	void pAddOcclusionMeshes( const decDVector &position );
	void pAddOcclusionMesh( const decMatrix &matrix, deoglROcclusionMesh *occlusionMesh );
// 	bool pAddFace( const decVector &v1, const decVector &v2, const decVector &v3, bool doubleSided );
	void pWriteTBOs( const decDVector &position );
	void pInitSphericalFibonacci();
};

#endif
