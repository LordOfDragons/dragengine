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

#ifndef _DEOGLRENDERLIGHTSPOT_H_
#define _DEOGLRENDERLIGHTSPOT_H_

#include "deoglRenderLightBase.h"
#include "../../collidelist/deoglCollideList.h"

class deoglLightShader;
class deoglRenderPlan;
class deoglRLight;
class deoglSPBlockUBO;
class deoglShaderProgram;
class deoglShadowMapper;
class deoglTexture;
class deoglRTRenderers;
class deoglRenderPlanMasked;



/**
 * \brief Render spot lights.
 */
class deoglRenderLightSpot : public deoglRenderLightBase{
public:
	struct sShadowDepthMaps{
		deoglTexture *shadow1Solid;
		deoglTexture *shadow1Transp;
		deoglTexture *shadow1TranspColor;
		deoglTexture *shadow2Solid;
		deoglTexture *shadow2Transp;
		deoglTexture *shadow2TranspColor;
		deoglTexture *shadow1Ambient;
		deoglTexture *shadow2Ambient;
		sShadowDepthMaps();
	};
	
	
	
private:
	deoglShaderProgram *pShaderShadowCopy;
	deoglShaderProgram *pShaderBoxBoundary1;
	deoglShaderProgram *pShaderBoxBoundary1Ambient;
	deoglShaderProgram *pShaderBoxBoundary2;
	deoglShaderProgram *pShaderOccMap;
	deoglCollideList pCollideList;
	
	
	
	deoglDebugInformation *pDebugInfoSolid;
	deoglDebugInformation *pDebugInfoTransparent;
	
	deoglDebugInformation *pDebugInfoSolidDetail;
	deoglDebugInformation *pDebugInfoSolidShadow;
	deoglDebugInformation *pDebugInfoSolidShadowClear;
	deoglDebugInformation *pDebugInfoSolidShadowClearTransp;
	deoglDebugInformation *pDebugInfoSolidShadowTask;
	deoglDebugInformation *pDebugInfoSolidShadowRender;
	deoglDebugInformation *pDebugInfoSolidShadowTranspTask;
	deoglDebugInformation *pDebugInfoSolidShadowTranspRender;
	deoglDebugInformation *pDebugInfoSolidLight;
	
	deoglDebugInformation *pDebugInfoTransparentDetail;
	deoglDebugInformation *pDebugInfoTransparentShadow;
	deoglDebugInformation *pDebugInfoTransparentShadowClear;
	deoglDebugInformation *pDebugInfoTransparentShadowClearTransp;
	deoglDebugInformation *pDebugInfoTransparentShadowTask;
	deoglDebugInformation *pDebugInfoTransparentShadowRender;
	deoglDebugInformation *pDebugInfoTransparentShadowTranspTask;
	deoglDebugInformation *pDebugInfoTransparentShadowTranspRender;
	deoglDebugInformation *pDebugInfoTransparentLight;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create renderer. */
	deoglRenderLightSpot( deoglRenderThread &renderThread, deoglRTRenderers &renderers );
	
	/** \brief Clean up renderer. */
	virtual ~deoglRenderLightSpot();
	/*@}*/
	
	
	
	/** \name Rendering */
	/*@{*/
	/** \brief Calculate box boundary for a spot light. */
	void CalculateBoxBoundary( deoglRenderPlan &plan, deoglRLight &light );
	
	/** \brief Render lights. */
	void RenderLights( deoglRenderPlan &plan, bool solid, deoglRenderPlanMasked *mask );
	
	/** \brief Render light. */
	void RenderLight( deoglRenderPlan &plan, bool solid, deoglRenderPlanMasked *mask,
		deoglRLight &light );
	
	/** \brief Activate textures for lighting. */
	void ActivateTextures( deoglRLight &light, deoglLightShader &shader,
		const sShadowDepthMaps &shadowDepthMaps );
	
	/** \brief Render shadow maps. */
	void RenderShadows( deoglRenderPlan &plan, bool solid, deoglRLight &light,
		const decDMatrix &matrixProjection, bool transparentStaticShadow,
		bool transparentDynamicShadow, bool refilterShadow );
	
	/** \brief Render shadow map. */
	void RenderShadowMap( deoglRenderPlan &plan, deoglRLight &light, const decDMatrix &matrixCamera,
		const decDMatrix &matrixProjection, deoglShadowMapper &shadowMapper,
		const deoglCollideList *clist1, const deoglCollideList *clist2, int solidShadowMapSize,
		int transpShadowMapSize, bool withTransparent, bool copyDepth, bool debugSolid );
	
	/** \brief Render ambient map. */
	void RenderAmbientMap( deoglRenderPlan &plan, deoglRLight &light, const decDMatrix &matrixCamera,
		const decDMatrix &matrixProjection, deoglShadowMapper &shadowMapper,
		const deoglCollideList *clist1, const deoglCollideList *clist2, int ambientMapSize );
	
	/** \brief Update light shader parameter block. */
	void UpdateLightParamBlock( deoglLightShader &lightShader, deoglSPBlockUBO &paramBlock,
		deoglRenderPlan &plan, deoglRLight &light );
	
	/** \brief Update instance shader parameter block. */
	void UpdateInstanceParamBlock( deoglLightShader &lightShader, deoglSPBlockUBO &paramBlock,
		deoglRenderPlan &plan, deoglRLight &light, sShadowDepthMaps &shadowDepthmaps,
		const decDMatrix &matrixLP );
	
	
	
	/** \brief Debug information solid lighting. */
	inline deoglDebugInformation *GetDebugInfoSolid() const{ return pDebugInfoSolid; }
	
	/** \brief Debug information transparent lighting. */
	inline deoglDebugInformation *GetDebugInfoTransparent() const{ return pDebugInfoTransparent; }
	
	
	
	/** \brief Reset debug information. */
	void ResetDebugInfo();
	
	/** \brief Add top level debug information in the right order. */
	virtual void AddTopLevelDebugInfoSolid();
	
	/** \brief Add top level debug information in the right order. */
	virtual void AddTopLevelDebugInfoTransparent();
	
	/** \brief Developer mode debug information changed. */
	virtual void DevModeDebugInfoChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
