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

#ifndef _DEOGLRENDERREFLECTION_H_
#define _DEOGLRENDERREFLECTION_H_

#include "deoglRenderBase.h"

class deoglCollisionFrustum;
class deoglAddToRenderTask;
class deoglCollideList;
class deoglCubeMap;
class deoglEnvironmentMap;
class deoglFramebuffer;
class deoglRenderPlan;
class deoglRenderTask;
class deoglTexture;
class deoglArrayTexture;


/**
 * Reflection Renderer.
 */
class deoglRenderReflection : public deoglRenderBase{
private:
	const deoglPipeline *pPipelineCopyColor;
	const deoglPipeline *pPipelineCopyColorMipMap;
	const deoglPipeline *pPipelineCopyColorStereo;
	const deoglPipeline *pPipelineCopyColorMipMapStereo;
	const deoglPipeline *pPipelineMinMaxMipMapMin;
	const deoglPipeline *pPipelineMinMaxMipMapMax;
	const deoglPipeline *pPipelineMinMaxMipMapInitial;
	const deoglPipeline *pPipelineMinMaxMipMapDownsample;
	const deoglPipeline *pPipelineScreenSpace;
	const deoglPipeline *pPipelineScreenSpaceStereo;
	const deoglPipeline *pPipelineApplyReflections;
	const deoglPipeline *pPipelineApplyReflectionsStereo;
	
	const deoglPipeline *pPipelineCopyMaterial;
	const deoglPipeline *pPipelineEnvMapLightGI;
	const deoglPipeline *pPipelineEnvMapCopy;
	const deoglPipeline *pPipelineReflection;
	const deoglPipeline *pPipelineReflectionStereo;
	const deoglPipeline *pPipelineCubeMap2EquiMap;
	const deoglPipeline *pPipelineBuildEnvMap;
	const deoglPipeline *pPipelineEnvMapMask;
	
	deoglSPBSingleUse::Ref pRenderParamBlockSingleUse;
	deoglSPBlockUBO::Ref pRenderParamBlock;
	deoglRenderTask *pRenderTask;
	deoglAddToRenderTask *pAddToRenderTask;
	
	bool pUseEquiEnvMap;
	deoglCubeMap *pEnvMap;
	deoglTexture *pEnvMapEqui;
	deoglSPBSingleUse::Ref pEnvMapsParamBlockSingleUse;
	deoglSPBlockUBO::Ref pEnvMapsParamBlock;
	
	deoglEnvironmentMap *pDirectEnvMapActive; ///< weak reference
	deoglEnvironmentMap *pDirectEnvMapFading; ///< weak reference
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new renderer. */
	deoglRenderReflection(deoglRenderThread &renderThread);
	/** Cleans up the renderer. */
	virtual ~deoglRenderReflection();
	/*@}*/
	
	/** \name Rendering */
	/*@{*/
	/** Determines if equirectangular environment maps are used. */
	inline bool GetUseEquiEnvMap() const{ return pUseEquiEnvMap; }
	
	/** Retrieves the active direct environment map or NULL if not existing. */
	inline deoglEnvironmentMap *GetDirectEnvMapActive() const{ return pDirectEnvMapActive; }
	/** Retrieves the fading direct environment map or NULL if not existing. */
	inline deoglEnvironmentMap *GetDirectEnvMapFading() const{ return pDirectEnvMapFading; }
	
	/** Retrieves the environment cube map. */
	inline deoglCubeMap *GetEnvMap() const{ return pEnvMap; }
	/** Retrieves the environment equi map. */
	inline deoglTexture *GetEnvMapEqui() const{ return pEnvMapEqui; }
	/** Update environment map suitable for a render plan. */
	void UpdateEnvMap(deoglRenderPlan &plan);
	
	/** Convert cubemap into an equimap. */
	void ConvertCubeMap2EquiMap(deoglCubeMap &cubemap, deoglTexture *equimap);
	/** Render environment map mask from mask shapes into the alpha component the currently active framebuffer object. */
	void RenderEnvMapMask(deoglRenderPlan &plan, deoglEnvironmentMap &envmap, int cubeMapFace);
	
	/** Render reflections. */
	void RenderReflections(deoglRenderPlan &plan);
	
	/** Update environment map slots. */
	void UpdateEnvMapSlots(deoglRenderPlan &plan);
	/** Prepare render shader parameter block matching the environment map slots. */
	void UpdateRenderParameterBlock(deoglRenderPlan &plan);
	
	
	/** Render depth min-max mip-map texture for use with screen space reflections. */
	void RenderDepthMinMaxMipMap(deoglRenderPlan &plan);
	
	/** Copy color texture to temporary1 texture and create mip-map levels. */
	void CopyColorToTemporary1(deoglRenderPlan &plan);
	
	/** Copy material. */
	void CopyMaterial(deoglRenderPlan &plan, bool solid);
	
	/** Render GI lit environment maps. */
	void RenderGIEnvMaps(deoglRenderPlan &plan);
	
	/** Copy environment map. */
	void CopyEnvMap(deoglArrayTexture &source, deoglCubeMap &target);
	
	/** Render screen space reflections. */
	void RenderScreenSpace(deoglRenderPlan &plan);
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
