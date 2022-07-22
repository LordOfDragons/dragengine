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
class deoglRenderPlanLight;
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
	
	struct sShadowParams{
		decDVector lightPosition;
		decVector lightView;
		decDMatrix matrixCamera;
		decDMatrix matrixProjection;
		bool solid;
		bool transparentStaticShadow;
		bool transparentDynamicShadow;
		const deoglCollideList *collideList1;
		const deoglCollideList *collideList2;
		int solidShadowMapSize;
		int transpShadowMapSize;
		int ambientMapSize;
		bool withTransparent;
		
		/**
		 * Maximum pixel error. If the rendered mesh and the mesh used for shadow calculation
		 * differ shadow artifacts can happen. By definition lod meshes should be conservative
		 * which means they should not stick outside the silhouette if possible. This has the
		 * following consequences. If the render mesh has lower lod level than the shadow mesh
		 * shadow artifacts appear since the rendered mesh faces are inside the higher lod mesh.
		 * This also means if the shadow mesh is using a higher lod level than the rendered
		 * mesh shadow artifacts should not appear. Increasing the lod level though makes the
		 * silhouette of the cast shadow more edgy but also makes rendering the shadow maps
		 * faster. It's difficult to find a good solution for this.
		 */
		int lodMaxPixelError;
		int lodOffset;
		int lodFovX;
		int lodFovY;
		
		sShadowParams();
	};
	
	
private:
	deoglShaderProgram *pShaderShadowCopy;
	deoglShaderProgram *pShaderBoxBoundary1;
	deoglShaderProgram *pShaderBoxBoundary1Ambient;
	deoglShaderProgram *pShaderBoxBoundary2;
	deoglShaderProgram *pShaderOccMap;
	deoglCollideList pCollideList;
	
	
	
	deoglDebugInformation::Ref pDebugInfoSolid;
	deoglDebugInformation::Ref pDebugInfoTransparent;
	
	deoglDebugInformation::Ref pDebugInfoSolidDetail;
	deoglDebugInformation::Ref pDebugInfoSolidShadow;
	deoglDebugInformation::Ref pDebugInfoSolidShadowClear;
	deoglDebugInformation::Ref pDebugInfoSolidShadowClearTransp;
	deoglDebugInformation::Ref pDebugInfoSolidShadowTask;
	deoglDebugInformation::Ref pDebugInfoSolidShadowRender;
	deoglDebugInformation::Ref pDebugInfoSolidShadowTranspTask;
	deoglDebugInformation::Ref pDebugInfoSolidShadowTranspRender;
	deoglDebugInformation::Ref pDebugInfoSolidLight;
	
	deoglDebugInformation::Ref pDebugInfoTransparentDetail;
	deoglDebugInformation::Ref pDebugInfoTransparentShadow;
	deoglDebugInformation::Ref pDebugInfoTransparentShadowClear;
	deoglDebugInformation::Ref pDebugInfoTransparentShadowClearTransp;
	deoglDebugInformation::Ref pDebugInfoTransparentShadowTask;
	deoglDebugInformation::Ref pDebugInfoTransparentShadowRender;
	deoglDebugInformation::Ref pDebugInfoTransparentShadowTranspTask;
	deoglDebugInformation::Ref pDebugInfoTransparentShadowTranspRender;
	deoglDebugInformation::Ref pDebugInfoTransparentLight;
	
	
	
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
	void CalculateBoxBoundary( deoglRenderPlanLight &planLight );
	
	/** \brief Render lights. */
	void RenderLights( deoglRenderPlan &plan, bool solid, const deoglRenderPlanMasked *mask );
	
	/** \brief Render light. */
	void RenderLight( deoglRenderPlanLight &planLight, bool solid, const deoglRenderPlanMasked *mask );
	
	/** \brief Activate textures for lighting. */
	void ActivateTextures( deoglRenderPlanLight &planLight, deoglLightShader &shader,
		const sShadowDepthMaps &shadowDepthMaps );
	
	/** \brief Render shadow maps. */
	void RenderShadows( deoglRenderPlanLight &planLight, sShadowParams &shadowParams );
	
	/** \brief Render shadow map. */
	void RenderShadowMap( deoglRenderPlanLight &planLight,
		deoglShadowMapper &shadowMapper, const sShadowParams &shadowParams );
	
	/** \brief Render ambient map. */
	void RenderAmbientMap( deoglRenderPlanLight &planLight,
		deoglShadowMapper &shadowMapper, const sShadowParams &shadowParams );
	
	/** \brief Update light shader parameter block. */
	void UpdateLightParamBlock( deoglLightShader &lightShader, deoglSPBlockUBO &paramBlock,
		deoglRenderPlanLight &planLight );
	
	/** \brief Update instance shader parameter block. */
	void UpdateInstanceParamBlock( deoglLightShader &lightShader, deoglSPBlockUBO &paramBlock,
		deoglRenderPlan &plan, const deoglCollideListLight &cllight,
		sShadowDepthMaps &shadowDepthmaps, const decDMatrix &matrixLP );
	
	
	
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
