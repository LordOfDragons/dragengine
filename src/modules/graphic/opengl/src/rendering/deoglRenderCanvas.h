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

#ifndef _DEOGLRENDERCANVAS_H_
#define _DEOGLRENDERCANVAS_H_

#include "deoglRenderBase.h"

class deoglRenderCanvasContext;
class deoglRCanvasText;
class deoglRCanvasVideoPlayer;
class deoglRCanvasRenderWorld;
class deoglRCanvasCanvasView;
class deoglRCanvasImage;
class deoglRCanvasPaint;
class deoglShaderProgram;
class deoglRenderTarget;



/**
 * Canvas renderer.
 */
class deoglRenderCanvas : public deoglRenderBase{
private:
	GLuint pVBOShapes;
	GLuint pVAOShapes;
	GLuint pActiveVAO;
	
	deoglShaderProgram *pShaderCanvasColor;
	deoglShaderProgram *pShaderCanvasColorMask;
	deoglShaderProgram *pShaderCanvasImage;
	deoglShaderProgram *pShaderCanvasImageMask;
	
	GLenum pBlendSrc;
	GLenum pBlendDest;
	
	deoglDebugInformation *pDebugInfoCanvas;
	deoglDebugInformation *pDebugInfoCanvasView;
	deoglDebugInformation *pDebugInfoCanvasImage;
	deoglDebugInformation *pDebugInfoCanvasPaint;
	deoglDebugInformation *pDebugInfoCanvasRenderWorld;
	deoglDebugInformation *pDebugInfoCanvasText;
	deoglDebugInformation *pDebugInfoCanvasVideoPlayer;
	deoglDebugInformation *pDebugInfoCanvasCanvasView;
	
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
	
	deoglDebugInformation *pDebugInfoPlanPrepare;
	deoglDebugInformation *pDebugInfoPlanPrepareWorld;
	deoglDebugInformation *pDebugInfoPlanPrepareCollect;
	deoglDebugInformation *pDebugInfoPlanPrepareCulling;
	deoglDebugInformation *pDebugInfoPlanPrepareEnvMaps;
	deoglDebugInformation *pDebugInfoPlanPrepareHTViewVBOs;
	deoglDebugInformation *pDebugInfoPlanPrepareComponents;
	deoglDebugInformation *pDebugInfoPlanPrepareSort;
	deoglDebugInformation *pDebugInfoPlanPrepareBuildPlan;
	deoglDebugInformation *pDebugInfoPlanPrepareLights;
	deoglDebugInformation *pDebugInfoPlanPrepareGIUpdate;
	deoglDebugInformation *pDebugInfoPlanPrepareGISkyShadowRenderTask;
	
	
	
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
	void SampleDebugInfoPlanPrepareWorld( deoglRenderPlan &plan );
	void SampleDebugInfoPlanPrepareCollect( deoglRenderPlan &plan );
	void SampleDebugInfoPlanPrepareCulling( deoglRenderPlan &plan );
	void SampleDebugInfoPlanPrepareEnvMaps( deoglRenderPlan &plan );
	void SampleDebugInfoPlanPrepareHTViewVBOs( deoglRenderPlan &plan );
	void SampleDebugInfoPlanPrepareComponents( deoglRenderPlan &plan );
	void SampleDebugInfoPlanPrepareSort( deoglRenderPlan &plan );
	void SampleDebugInfoPlanPrepareBuildPlan( deoglRenderPlan &plan );
	void SampleDebugInfoPlanPrepareLights( deoglRenderPlan &plan );
	void SampleDebugInfoPlanPrepareGIUpdate( deoglRenderPlan &plan );
	
	void ResetDebugInfoTimerGI( deoglRenderPlan &plan );
	void SampleDebugInfoPlanPrepareGISkyShadowRenderTask( deoglRenderPlan &plan );
	
	
	
	/** Add top level debug information in the right order. */
	virtual void AddTopLevelDebugInfo();
	
	/** Developer mode debug information changed. */
	virtual void DevModeDebugInfoChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pCreateShapesVAO();
	void pWorldRenderSize( int &width, int &height ) const;
	void pSetBlendMode( GLenum blendSrc, GLenum blendDest );
	void pSetBlendModeForce( GLenum blendSrc, GLenum blendDest );
	void pActivateVAOShapes();
};

#endif
