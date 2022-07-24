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
class deoglSPBlockUBO;
class deoglTexture;
class deoglArrayTexture;


/**
 * @brief Reflection Renderer.
 */
class deoglRenderReflection : public deoglRenderBase{
private:
	deoglShaderProgramUsage pShaderCopyColor;
	deoglShaderProgramUsage pShaderCopyColorMipMap;
	deoglShaderProgramUsage pShaderCopyColorStereo;
	deoglShaderProgramUsage pShaderCopyColorMipMapStereo;
	deoglShaderProgramUsage pShaderMinMaxMipMapMin;
	deoglShaderProgramUsage pShaderMinMaxMipMapMax;
	deoglShaderProgramUsage pShaderMinMaxMipMapInitial;
	deoglShaderProgramUsage pShaderMinMaxMipMapDownsample;
	deoglShaderProgramUsage pShaderScreenSpace;
	deoglShaderProgramUsage pShaderApplyReflections;
	deoglShaderProgramUsage pShaderApplyReflectionsStereo;
	
	deoglShaderProgramUsage pShaderCopyMaterial;
	deoglShaderProgramUsage pShaderEnvMapLightGI;
	deoglShaderProgramUsage pShaderEnvMapCopy;
	deoglShaderProgramUsage pShaderReflection;
	deoglShaderProgramUsage pShaderCubeMap2EquiMap;
	deoglShaderProgramUsage pShaderBuildEnvMap;
	deoglShaderProgramUsage pShaderEnvMapMask;
	
	deoglSPBlockUBO *pRenderParamBlock;
	deoglRenderTask *pRenderTask;
	deoglAddToRenderTask *pAddToRenderTask;
	
	bool pUseEquiEnvMap;
	deoglCubeMap *pEnvMap;
	deoglTexture *pEnvMapEqui;
	deoglSPBlockUBO *pEnvMapsParamBlock;
	
	deoglEnvironmentMap *pDirectEnvMapActive; ///< weak reference
	deoglEnvironmentMap *pDirectEnvMapFading; ///< weak reference
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new renderer. */
	deoglRenderReflection( deoglRenderThread &renderThread );
	/** Cleans up the renderer. */
	virtual ~deoglRenderReflection();
	/*@}*/
	
	/** @name Rendering */
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
	void UpdateEnvMap( deoglRenderPlan &plan );
	
	/** Convert cubemap into an equimap. */
	void ConvertCubeMap2EquiMap( deoglCubeMap &cubemap, deoglTexture *equimap );
	/** Render environment map mask from mask shapes into the alpha component the currently active framebuffer object. */
	void RenderEnvMapMask( deoglRenderPlan &plan, deoglEnvironmentMap &envmap, int cubeMapFace );
	
	/** Render reflections. */
	void RenderReflections( deoglRenderPlan &plan );
	
	/** Update environment map slots. */
	void UpdateEnvMapSlots( deoglRenderPlan &plan );
	/** Prepare render shader parameter block matching the environment map slots. */
	void UpdateRenderParameterBlock( deoglRenderPlan &plan );
	
	
	/** Render depth min-max mip-map texture for use with screen space reflections. */
	void RenderDepthMinMaxMipMap( deoglRenderPlan &plan );
	
	/** Copy color texture to temporary1 texture and create mip-map levels. */
	void CopyColorToTemporary1( deoglRenderPlan &plan );
	
	/** Copy material. */
	void CopyMaterial( deoglRenderPlan &plan, bool solid );
	
	/** Render GI lit environment maps. */
	void RenderGIEnvMaps( deoglRenderPlan &plan );
	
	/** Copy environment map. */
	void CopyEnvMap( deoglArrayTexture &source, deoglCubeMap &target );
	
	/** Render screen space reflections. */
	void RenderScreenSpace( deoglRenderPlan &plan );
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
