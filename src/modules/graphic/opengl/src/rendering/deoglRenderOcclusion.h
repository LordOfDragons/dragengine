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

class deoglCollisionFrustum;
class deoglAddToRenderTask;
class deoglCollideList;
class deoglCubeMap;
class deoglOcclusionMap;
class deoglRenderPlan;
class deoglRenderTask;
class deoglSPBlockUBO;
class deoglShaderProgram;
class deoglRSkyLayer;
class deoglRSkyInstanceLayer;
class deoglOcclusionTracing;


/**
 * @brief Occlusion Renderer.
 */
class deoglRenderOcclusion : public deoglRenderBase{
private:
	deoglShaderProgram *pShaderOccMap;
	deoglShaderProgram *pShaderOccMapOrtho;
	deoglShaderProgram *pShaderOccMapDownSample;
	deoglShaderProgram *pShaderOccTest;
	deoglShaderProgram *pShaderOccTestDual;
	deoglShaderProgram *pShaderOccTestSun;
	deoglShaderProgram *pShaderOccTestTFB;
	deoglShaderProgram *pShaderOccTestTFBDual;
	deoglShaderProgram *pShaderOccTestTFBSun;
	deoglShaderProgram *pShaderOccMapCube;
	
	deoglShaderProgram *pShaderOccTracingGenRays;
	deoglShaderProgram *pShaderOccTracing;
	deoglShaderProgram *pShaderOccTracingDebug;
	
	deoglSPBlockUBO *pRenderParamBlock;
	deoglSPBlockUBO *pOccMapFrustumParamBlock;
	deoglRenderTask *pRenderTask;
	deoglAddToRenderTask *pAddToRenderTask;
	
	GLuint pVBOFrustumPlanes;
	GLuint pVAOFrustumPlanes;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new renderer. */
	deoglRenderOcclusion( deoglRenderThread &renderThread );
	/** Cleans up the renderer. */
	virtual ~deoglRenderOcclusion();
	/*@}*/
	
	/** @name Rendering */
	/*@{*/
	/** \brief Add basic defines for occlusion map shaders. */
	void AddOccMapDefines( deoglShaderDefines &defines );
	
	/** Render occlusion tests. */
	void RenderTestsCamera( deoglRenderPlan &plan );
	
	/** Render occlusion tests. */
	void RenderTestsSkyLayer( deoglRenderPlan &plan, deoglRSkyInstanceLayer &skyLayer,
		deoglCollideList &collideList, const decVector &minExtend, const decVector &maxExtend );
	
	/** Render occlusion meshes into the occlusion map. */
	void RenderOcclusionMap( deoglRenderPlan &plan );
	
	/** Render occlusion tests. */
	void RenderOcclusionTests( deoglOcclusionMap &occmap, int baselevel, float clipNear, const decMatrix &matrixCamera );
	
	/** Render occlusion tests with frustum check. */
	void RenderOcclusionTestsSun( deoglOcclusionMap &occmap, int baselevel, float clipNear, const decMatrix &matrixCamera,
		float clipNear2, const decMatrix &matrixCamera2, deoglRenderPlan &plan );
	/** Render occlusion tests using dual occlusion maps. */
	void RenderOcclusionTestsDual( deoglOcclusionMap &occmap, int baselevel, float clipNear, const decMatrix &matrixCamera,
		deoglOcclusionMap &occmap2, int baselevel2, float clipNear2, const decMatrix &matrixCamera2 );
	
	/** Debug render plan occlusion map. */
	void DebugOcclusionMap( deoglRenderPlan &plan );
	
	/** Render occlusion meshes into a cube map. The position is relative to the reference position. */
	void RenderOcclusionCubeMap( const deoglCollideList &collideList, deoglCubeMap *cubemap,
		const decDVector &position, float imageDistance, float viewDistance );
	
	/** \brief Sample occlusion trace probes. */
	void RenderOcclusionTraceProbes( deoglOcclusionTracing &tracing );
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
