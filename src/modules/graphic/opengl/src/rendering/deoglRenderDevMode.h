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
