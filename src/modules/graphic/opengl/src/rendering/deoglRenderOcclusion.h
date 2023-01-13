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

#ifndef _DEOGLRENDEROCCLUSION_H_
#define _DEOGLRENDEROCCLUSION_H_

#include "deoglRenderBase.h"

class deoglAddToRenderTask;
class deoglCollideList;
class deoglCollisionFrustum;
class deoglCubeMap;
class deoglOcclusionMap;
class deoglOcclusionTest;
class deoglRenderPlan;
class deoglRenderPlanSkyLight;
class deoglRenderPlanMasked;
class deoglRenderTask;
class deoglRSkyInstanceLayer;
class deoglRSkyLayer;
class deoglPipeline;


/**
 * @brief Occlusion Renderer.
 */
class deoglRenderOcclusion : public deoglRenderBase{
private:
	enum ePipelineModifiers{
		epmStereo = 0x1,
		epmClipPlane = 0x2,
		epmOrtho = 0x4,
		epmSingle = 0x8
	};
	
	const deoglPipeline *pPipelinesOccMap[ epmSingle << 1 ];
	const deoglPipeline *pPipelinesOccQuery[ epmOrtho << 1 ];
	
	const deoglPipeline *pPipelineOccMapDownSample;
	const deoglPipeline *pPipelineOccMapDownSampleStereo;
	const deoglPipeline *pPipelineOccTest;
	const deoglPipeline *pPipelineOccTestDual;
	const deoglPipeline *pPipelineOccTestSun;
	const deoglPipeline *pPipelineOccTestStereo;
	const deoglPipeline *pPipelineOccTestDualStereo;
	const deoglPipeline *pPipelineOccTestSunStereo;
	const deoglPipeline *pPipelineOccTestTFB;
	const deoglPipeline *pPipelineOccTestTFBDual;
	const deoglPipeline *pPipelineOccTestTFBSun;
	const deoglPipeline *pPipelineOccMapCube;
	
	deoglSPBlockUBO::Ref pRenderParamBlock;
	deoglSPBlockUBO::Ref pOccMapFrustumParamBlock;
	deoglRenderTask *pRenderTask;
	deoglAddToRenderTask *pAddToRenderTask;
	
	GLuint pVBOFrustumPlanes;
	GLuint pVAOFrustumPlanes;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new renderer. */
	deoglRenderOcclusion( deoglRenderThread &renderThread );
	/** Cleans up the renderer. */
	virtual ~deoglRenderOcclusion();
	/*@}*/
	
	/** \name Rendering */
	/*@{*/
	/** Add basic defines for occlusion map shaders. */
	void AddOccMapDefines( deoglShaderDefines &defines );
	
	/** Render occlusion tests. */
	void RenderTestsCamera( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask );
	
	/** Render occlusion tests. */
	void RenderTestsSkyLayer( deoglRenderPlan &plan, deoglRenderPlanSkyLight &planSkyLigh );
	
	/** Shader to use for occlusion map rendering. */
	const deoglPipeline *GetRenderOcclusionMapRTS( const deoglRenderPlan &plan,
		const deoglRenderPlanMasked *mask, bool perspective, bool singleSided ) const;
	
	/** Render occlusion meshes into the occlusion map. */
	void RenderOcclusionMap( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask );
	void RenderOcclusionMap( deoglRenderPlan &plan, deoglRenderTask &renderTask );
	
	/** Render occlusion queries using active occlusion map. */
	void RenderOcclusionQueries( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask, bool perspective );
	
	/** Render occlusion tests. */
	void RenderOcclusionTests( deoglRenderPlan &plan, deoglOcclusionTest &occlusionTest,
		deoglOcclusionMap &occlusionMap, int baselevel, float clipNear,
		const decMatrix &matrixCamera, const decMatrix &matrixCameraStereo );
	
	/** Render occlusion tests with frustum check. */
	void RenderOcclusionTestsSun( deoglRenderPlan &plan, deoglOcclusionTest &occlusionTest,
		deoglOcclusionMap &occlusionMap, int baselevel, float clipNear,
		const decMatrix &matrixCamera, float clipNear2, const decMatrix &matrixCamera2,
		const decMatrix &matrixCamera2Stereo );
	
	/** Render occlusion tests using dual occlusion maps. */
	void RenderOcclusionTestsDual( deoglRenderPlan &plan, deoglOcclusionTest &occlusioNTest,
		deoglOcclusionMap &occlusionMap, int baselevel, float clipNear, const decMatrix &matrixCamera,
		deoglOcclusionMap &occlusionMap2, int baselevel2, float clipNear2, const decMatrix &matrixCamera2 );
	
	/** Debug render plan occlusion map. */
	void DebugOcclusionMap( deoglRenderPlan &plan );
	
	/** Render occlusion meshes into a cube map. The position is relative to the reference position. */
// 	void RenderOcclusionCubeMap( const deoglCollideList &collideList, deoglCubeMap *cubemap,
// 		const decDVector &position, float imageDistance, float viewDistance );
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
