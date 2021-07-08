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

#ifndef _DEOGLRENDERLIGHTPOINT_H_
#define _DEOGLRENDERLIGHTPOINT_H_

#include "deoglRenderLightBase.h"
#include "../../collidelist/deoglCollideList.h"

class deoglCubeMap;
class deoglLightShader;
class deoglRenderPlan;
class deoglRenderPlanLight;
class deoglRLight;
class deoglSPBlockUBO;
class deoglShaderProgram;
class deoglShadowMapper;
class deoglRTRenderers;
class deoglRenderPlanMasked;


/**
 * \brief Render point lights.
 */
class deoglRenderLightPoint : public deoglRenderLightBase{
public:
	struct sShadowDepthMaps{
		deoglCubeMap *shadow1Solid;
		deoglCubeMap *shadow1Transp;
		deoglCubeMap *shadow1TranspColor;
		deoglCubeMap *shadow2Solid;
		deoglCubeMap *shadow2Transp;
		deoglCubeMap *shadow2TranspColor;
		deoglCubeMap *shadow1Ambient;
		deoglCubeMap *shadow2Ambient;
		
		inline sShadowDepthMaps() :
		shadow1Solid( NULL ),
		shadow1Transp( NULL ),
		shadow2Solid( NULL ),
		shadow2Transp( NULL ){}
	};
	
	
	
private:
	deoglShaderProgram *pShaderLight;
	deoglShaderProgram *pShaderLightShadow;
	deoglShaderProgram *pShaderLightShadowTransp;
	
	deoglShaderProgram *pShaderShadowSolid;
	deoglShaderProgram *pShaderShadowHoles;
	deoglShaderProgram *pShaderShadowTransp;
	
	deoglShaderProgram *pShaderBoxBoundary1;
	deoglShaderProgram *pShaderBoxBoundary1Ambient;
	deoglShaderProgram *pShaderBoxBoundary2;
	deoglShaderProgram *pShaderOccMap;
	deoglShaderProgram *pShaderOccMapCube;
	
	int pCubeFaces[ 6 ];
	
	deoglCollideList pCollideList;
	
	
	
	deoglDebugInformation *pDebugInfoSolid;
	deoglDebugInformation *pDebugInfoTransparent;
	
	deoglDebugInformation *pDebugInfoSolidDetail;
	deoglDebugInformation *pDebugInfoSolidShadow;
	deoglDebugInformation *pDebugInfoSolidShadowClear;
	deoglDebugInformation *pDebugInfoSolidShadowClearTransp;
	deoglDebugInformation *pDebugInfoSolidShadowFace;
	deoglDebugInformation *pDebugInfoSolidShadowFaceClear;
	deoglDebugInformation *pDebugInfoSolidShadowFaceTask;
	deoglDebugInformation *pDebugInfoSolidShadowFaceRender;
	deoglDebugInformation *pDebugInfoSolidShadowFaceTranspClear;
	deoglDebugInformation *pDebugInfoSolidShadowFaceTranspTask;
	deoglDebugInformation *pDebugInfoSolidShadowFaceTranspRender;
	deoglDebugInformation *pDebugInfoSolidLight;
	
	deoglDebugInformation *pDebugInfoTransparentDetail;
	deoglDebugInformation *pDebugInfoTransparentShadow;
	deoglDebugInformation *pDebugInfoTransparentShadowClear;
	deoglDebugInformation *pDebugInfoTransparentShadowClearTransp;
	deoglDebugInformation *pDebugInfoTransparentShadowFace;
	deoglDebugInformation *pDebugInfoTransparentShadowFaceClear;
	deoglDebugInformation *pDebugInfoTransparentShadowFaceTask;
	deoglDebugInformation *pDebugInfoTransparentShadowFaceRender;
	deoglDebugInformation *pDebugInfoTransparentShadowFaceTranspClear;
	deoglDebugInformation *pDebugInfoTransparentShadowFaceTranspTask;
	deoglDebugInformation *pDebugInfoTransparentShadowFaceTranspRender;
	deoglDebugInformation *pDebugInfoTransparentLight;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create renderer. */
	deoglRenderLightPoint( deoglRenderThread &renderThread, deoglRTRenderers &renderers );
	
	/** \brief Clean up renderer. */
	virtual ~deoglRenderLightPoint();
	/*@}*/
	
	
	
	/** \name Rendering */
	/*@{*/
	/** \brief Calculate box boundary for a point light. */
	void CalculateBoxBoundary( deoglRenderPlanLight &planLight );
	
	/** \brief Renders lights. */
	void RenderLights( deoglRenderPlan &plan, bool solid, const deoglRenderPlanMasked *mask );
	
	/** \brief Render point light. */
	void RenderLight( deoglRenderPlanLight &planLight, bool solid, const deoglRenderPlanMasked *mask );
	
	/** \brief Activate textures for lighting. */
	void ActivateTextures( deoglRenderPlanLight &planLight, deoglLightShader &shader,
		const sShadowDepthMaps &shadowDepthMaps );
	
	/** \brief Render shadows for a point light. */
	void RenderShadows( deoglRenderPlanLight &planLight, bool solid,
		const decDMatrix &matrixProjection, bool transparentStaticShadow,
		bool transparentDynamicShadow, bool refilterShadow );
	
	/** \brief Clear cube map. */
	void ClearCubeMap( deoglShadowMapper &shadowMapper, int shadowMapSize );
	
	/** \brief Render shadow cube map. */
	void RenderShadowMaps( deoglRenderPlanLight &planLight, const decDMatrix &matrixProjection,
		deoglShadowMapper &shadowMapper, const deoglCollideList *clist1, const deoglCollideList *clist2,
		int solidShadowMapSize, int transpShadowMapSize, bool withTransparent, float shadowScale,
		float shadowOffset, bool debugSolid );
	
	/** \brief Render ambient map. */
	void RenderAmbientMap( deoglRenderPlanLight &planLight, const decDMatrix &matrixProjection,
		deoglShadowMapper &shadowMapper, const deoglCollideList *clist1, const deoglCollideList *clist2,
		int ambientMapSize, float shadowScale, float shadowOffset );
	
	/** \brief Update light shader parameter block. */
	void UpdateLightParamBlock( deoglLightShader &lightShader, deoglSPBlockUBO &paramBlock,
		deoglRenderPlanLight &planLight );
	
	/** \brief Update instance shader parameter block. */
	void UpdateInstanceParamBlock( deoglLightShader &lightShader, deoglSPBlockUBO &paramBlock,
		deoglRenderPlanLight &planLight, sShadowDepthMaps &shadowDepthMaps );
	
	
	
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
