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

#ifndef _DEOGLRENDERDEVMODE_H_
#define _DEOGLRENDERDEVMODE_H_

#include "deoglRenderBase.h"

class deoglDebugInformationList;
class deoglDebugInformation;
class decString;


/**
 * Render developer mode information.
 */
class deoglRenderDevMode : public deoglRenderBase{
private:
	const deoglPipeline *pPipelineSolidColor2D;
	const deoglPipeline *pPipelineSolidColor3D;
	const deoglPipeline *pPipelineShape;
	const deoglPipeline *pPipelineShapeLine;
	const deoglPipeline *pPipelineVRDebugPanel;
	const deoglPipeline *pPipelineVRDebugPanelStereo;
	
	decVector2 pScalePosition;
	decVector2 pOffsetPosition;
	
	GLuint pVBOShapes;
	GLuint pVAOShapes;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new renderer. */
	deoglRenderDevMode( deoglRenderThread &renderThread );
	/** Cleans up the renderer. */
	~deoglRenderDevMode();
	/*@}*/
	
	/** \name Rendering */
	/*@{*/
	/** Renders the developer mode information. */
	void RenderDevMode( deoglRenderPlan &plan );
	
	/** Renders the visiblity of components in the view. */
	void RenderVisComponent( deoglRenderPlan &plan );
	/** Renders the visiblity of lights in the view. */
	void RenderVisLight( deoglRenderPlan &plan );
	/** Renders the component lod levels. */
	void RenderComponentLodLevels( deoglRenderPlan &plan );
	/** Highlight transparent objects. */
	void RenderHighlightTransparentObjects( deoglRenderPlan& plan );
	
	/** Renders the height terrain boxes. */
	void RenderHeightTerrainBoxes( deoglRenderPlan &plan );
	/** Renders the prop field information. */
	void RenderPropFieldInfo( deoglRenderPlan &plan );
	
	/** Renders light infos. */
	void RenderLightInfos( deoglRenderPlan &plan );
	
	/** Render environment map information. */
	void RenderEnvMapInfo( deoglRenderPlan &plan );
	
	/** Display overlay information stacked from top to bottom. */
	void RenderOverlayInfos( deoglRenderPlan &plan );
	/** Display occlusion map level. */
	void RenderOccMapLevel( deoglRenderPlan &plan );
	/** Renders the LOD levels of the height terrain surrounding the camera using an overlay image. */
	void RenderHeightTerrainLODLevels( deoglRenderPlan &plan, const decPoint &position, decPoint &size );
	/** Display the number of transparency levels as a dot bar. */
	void RenderTraspLevelCount( deoglRenderPlan &plan, const decPoint &position, decPoint &size );
	
	/** Display render plan debug information. */
	void RenderRenderPlanDebugInfo( deoglRenderPlan &plan, const decPoint &viewport,
		const decPoint &position, decPoint &size );
	
	/** Display memory information. */
	void RenderMemoryInfo( deoglRenderPlan &plan, const decPoint &viewport,
		const decPoint &position, decPoint &size );
	
	
	
	/** Show visible debug information. */
	void RenderDebugInformation( const decPoint &viewport, const decPoint &position,
		decPoint &size, bool forceSolid );
	
	/** Log visible debug information. */
	void LogDebugInformation();
	void LogDebugInformation( const deoglDebugInformationList &list, const decString &prefix );
	
	/** Layout visible debug information for rendering. */
	void LayoutDebugInformation( const decPoint &viewport, const decPoint &position, decPoint &size,
		const deoglDebugInformationList &list, int minWidth, int maxWidth, bool alignSidewards );
	
	void ChildMaxNameLen( const deoglDebugInformationList &list, int &maxNameWidth,
		bool &siblingsHaveElapsedTime, bool &siblingsHaveCounter ) const;
	
	/** Layout visible debug information for rendering. */
	void LayoutDebugInformation( const decPoint &viewport, int maxNameWidth,
		deoglDebugInformation &debugInformation, int minWidth, int maxWidth,
		bool siblingsHaveElapsedTime, bool siblingsHaveCounter );
	
	/** Show visible debug information. */
	void RenderDebugInformation( const decPoint &viewport, const decPoint &parentPosition,
		const deoglDebugInformation &debugInformation, bool forceSolid );
	/*@}*/
	
private:
	void pCleanUp();
	void pCreateShapesVAO();
};

#endif
