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

#ifndef _DEOGLRENDERCANVAS_H_
#define _DEOGLRENDERCANVAS_H_

#include "deoglRenderBase.h"
#include "../canvas/render/deoglRCanvas.h"

class deoglRenderCanvasContext;
class deoglRCanvasText;
class deoglRCanvasVideoPlayer;
class deoglRCanvasRenderWorld;
class deoglRCanvasCanvasView;
class deoglRCanvasImage;
class deoglRCanvasPaint;
class deoglRenderTarget;



/**
 * Canvas renderer.
 */
class deoglRenderCanvas : public deoglRenderBase{
private:
	GLuint pVBOShapes;
	GLuint pVAOShapes;
	GLuint pActiveVAO;
	
	const deoglPipeline *pPipelineCanvasColor[ deoglRCanvas::BlendModeCount ];
	const deoglPipeline *pPipelineCanvasColorMask[ deoglRCanvas::BlendModeCount ];
	const deoglPipeline *pPipelineCanvasImage[ deoglRCanvas::BlendModeCount ];
	const deoglPipeline *pPipelineCanvasImageMask[ deoglRCanvas::BlendModeCount ];
	const deoglPipeline *pPipelineCanvasImageArray[deoglRCanvas::BlendModeCount];
	const deoglPipeline *pPipelineCanvasImageArrayMask[deoglRCanvas::BlendModeCount];
	const deoglPipeline *pPipelineCanvasRenderWorld[ deoglRCanvas::BlendModeCount ];
	const deoglPipeline *pPipelineCanvasRenderWorldMask[ deoglRCanvas::BlendModeCount ];
	
	deoglDebugInformation::Ref pDebugInfoCanvas;
	deoglDebugInformation::Ref pDebugInfoCanvasView;
	deoglDebugInformation::Ref pDebugInfoCanvasImage;
	deoglDebugInformation::Ref pDebugInfoCanvasPaint;
	deoglDebugInformation::Ref pDebugInfoCanvasRenderWorld;
	deoglDebugInformation::Ref pDebugInfoCanvasText;
	deoglDebugInformation::Ref pDebugInfoCanvasVideoPlayer;
	deoglDebugInformation::Ref pDebugInfoCanvasCanvasView;
	
	float pDebugTimeCanvasView;
	int pDebugCountCanvasView;
	float pDebugTimeCanvasImage;
	int pDebugCountCanvasImage;
	float pDebugTimeCanvasPaint;
	int pDebugCountCanvasPaint;
	float pDebugTimeCanvasRenderWorld;
	int pDebugCountCanvasRenderWorld;
	float pDebugTimeCanvasText;
	int pDebugCountCanvasText;
	float pDebugTimeCanvasVideoPlayer;
	int pDebugCountCanvasVideoPlayer;
	float pDebugTimeCanvasCanvasView;
	int pDebugCountCanvasCanvasView;
	
	deoglDebugInformation::Ref pDebugInfoPlanPrepare;
	deoglDebugInformation::Ref pDebugInfoPlanPrepareEarlyWorld;
	deoglDebugInformation::Ref pDebugInfoPlanPrepareFindContent;
	deoglDebugInformation::Ref pDebugInfoPlanPrepareBuildRTs;
	deoglDebugInformation::Ref pDebugInfoPlanPrepareSkyLightFindContent;
	deoglDebugInformation::Ref pDebugInfoPlanPrepareSkyLightBuildRT;
	deoglDebugInformation::Ref pDebugInfoPlanPrepareSkyLightGIFindContent;
	deoglDebugInformation::Ref pDebugInfoPlanPrepareSkyLightGIUpdateRenderTask;
	deoglDebugInformation::Ref pDebugInfoPlanPrepareWorld;
	deoglDebugInformation::Ref pDebugInfoPlanPrepareGIUpdate;
	deoglDebugInformation::Ref pDebugInfoPlanPrepareCulling;
	deoglDebugInformation::Ref pDebugInfoPlanPrepareEnvMaps;
	deoglDebugInformation::Ref pDebugInfoPlanPreparePrepareContent;
	deoglDebugInformation::Ref pDebugInfoPlanPrepareHTViewVBOs;
	deoglDebugInformation::Ref pDebugInfoPlanPrepareBuildPlan;
	deoglDebugInformation::Ref pDebugInfoPlanPrepareLights;
	deoglDebugInformation::Ref pDebugInfoPlanPrepareFinish;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create canvas renderer. */
	deoglRenderCanvas( deoglRenderThread &renderThread );
	
	/** Clean up canvas renderer. */
	virtual ~deoglRenderCanvas();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Set opengl states required for canvas rendering. */
	void Prepare( const deoglRenderCanvasContext &context );
	
	/** Draw canvas paint. */
	void DrawCanvasPaint( const deoglRenderCanvasContext &context, const deoglRCanvasPaint &canvas );
	
	/** Draw canvas image. */
	void DrawCanvasImage( const deoglRenderCanvasContext &context, const deoglRCanvasImage &canvas );
	
	/** Draw canvas canvas view. */
	void DrawCanvasCanvasView( const deoglRenderCanvasContext &context, const deoglRCanvasCanvasView &canvas );
	
	/** Draw canvas render world. */
	void DrawCanvasRenderWorld( const deoglRenderCanvasContext &context, const deoglRCanvasRenderWorld &canvas );
	
	/** Draw canvas video player. */
	void DrawCanvasVideoPlayer( const deoglRenderCanvasContext &context, const deoglRCanvasVideoPlayer &canvas );
	
	/** Draw canvas text. */
	void DrawCanvasText( const deoglRenderCanvasContext &context, const deoglRCanvasText &canvas );
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** Reset debug information canvas. */
	void DebugInfoCanvasReset();
	
	/** Update debug information canvas. */
	void DebugInfoCanvasUpdate();
	
	
	
	/** Clear all debug information prepare plan. */
	void ClearAllDebugInfoPlanPrepare( deoglRenderPlan &plan );
	
	void SampleDebugInfoPlanPrepare( deoglRenderPlan &plan );
	void SampleDebugInfoPlanPrepareEarlyWorld( deoglRenderPlan &plan );
	void SampleDebugInfoPlanPrepareFindContent( deoglRenderPlan &plan );
	void SampleDebugInfoPlanPrepareFindContent( deoglRenderPlan &plan, float elapsed );
	void SampleDebugInfoPlanPrepareBuildRTs( deoglRenderPlan &plan, float elapsed );
	void SampleDebugInfoPlanPrepareSkyLightFindContent( deoglRenderPlan &plan );
	void SampleDebugInfoPlanPrepareSkyLightFindContent( deoglRenderPlan &plan, float elapsed );
	void SampleDebugInfoPlanPrepareSkyLightBuildRT( deoglRenderPlan &plan, float elapsed );
	void SampleDebugInfoPlanPrepareSkyLightGIFindContent( deoglRenderPlan &plan );
	void SampleDebugInfoPlanPrepareSkyLightGIFindContent( deoglRenderPlan &plan, float elapsed );
	void SampleDebugInfoPlanPrepareSkyLightGIUpdateRenderTask( deoglRenderPlan &plan, float elapsed );
	void SampleDebugInfoPlanPrepareWorld( deoglRenderPlan &plan );
	void SampleDebugInfoPlanPrepareGIUpdate( deoglRenderPlan &plan );
	void SampleDebugInfoPlanPrepareCulling( deoglRenderPlan &plan );
	void SampleDebugInfoPlanPrepareEnvMaps( deoglRenderPlan &plan );
	void SampleDebugInfoPlanPreparePrepareContent( deoglRenderPlan &plan );
	void SampleDebugInfoPlanPrepareHTViewVBOs( deoglRenderPlan &plan );
	void SampleDebugInfoPlanPrepareBuildPlan( deoglRenderPlan &plan );
	void SampleDebugInfoPlanPrepareLights( deoglRenderPlan &plan );
	void SampleDebugInfoPlanPrepareFinish( deoglRenderPlan &plan );
	
	
	
	/** Add top level debug information in the right order. */
	virtual void AddTopLevelDebugInfo();
	
	/** Developer mode debug information changed. */
	virtual void DevModeDebugInfoChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pCreateShapesVAO();
	void pWorldRenderSize( int &width, int &height ) const;
	void pActivateVAOShapes();
	void pCreatePipelines(const deoglPipeline* (&pipelines)[deoglRCanvas::BlendModeCount],
		deoglPipelineConfiguration &config, const deoglShaderSources *sources,
		const deoglShaderDefines &defines);
};

#endif
