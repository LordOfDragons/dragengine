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
 * Occlusion Renderer.
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
	const deoglPipeline *pPipelineOccTestComputeRT;
	const deoglPipeline *pPipelineOccTestDual;
	const deoglPipeline *pPipelineOccTestSun;
	const deoglPipeline *pPipelineOccTestSunComputeRT;
	
	deoglSPBSingleUse::Ref pRenderParamBlockSingleUse;
	deoglSPBlockUBO::Ref pRenderParamBlock;
	deoglSPBSingleUse::Ref pOccMapFrustumParamBlockSingleUse;
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
	/** Render occlusion tests. */
	void RenderTestsCamera( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask );
	
	/** Render occlusion tests. */
	void RenderTestsSkyLayer( deoglRenderPlanSkyLight &planSkyLigh );
	
	/** Shader to use for occlusion map rendering. */
	const deoglPipeline *GetRenderOcclusionMapRTS( const deoglRenderPlan &plan,
		const deoglRenderPlanMasked *mask, bool perspective, bool singleSided ) const;
	
	/** Render occlusion meshes into the occlusion map. */
	void RenderOcclusionMap( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask );
	void RenderOcclusionMap( deoglRenderPlan &plan, deoglRenderTask *renderTask,
		deoglComputeRenderTask *computeRenderTask );
	
	/** Render occlusion queries using active occlusion map. */
	void RenderOcclusionQueries( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask, bool perspective );
	
	/** Render occlusion tests. */
	void RenderOcclusionTests( deoglRenderPlan &plan, deoglOcclusionTest &occlusionTest,
		deoglOcclusionMap &occlusionMap, int baselevel, float clipNear,
		const decMatrix &matrixCamera, const decMatrix &matrixCameraStereo );
	
	/** Render occlusion tests with frustum check. */
	void RenderOcclusionTestsSun( deoglRenderPlanSkyLight &planSkyLight, deoglOcclusionTest &occlusionTest,
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
