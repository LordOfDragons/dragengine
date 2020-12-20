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
 * \brief Canvas renderer.
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
	deoglDebugInformation *pDebugInfoPlanPrepareCollect;
	deoglDebugInformation *pDebugInfoPlanPrepareCulling;
	deoglDebugInformation *pDebugInfoPlanPrepareEnvMaps;
	deoglDebugInformation *pDebugInfoPlanPrepareHTViewVBOs;
	deoglDebugInformation *pDebugInfoPlanPrepareComponents;
	deoglDebugInformation *pDebugInfoPlanPrepareComponentsVBO;
	deoglDebugInformation *pDebugInfoPlanPrepareComponentsRenderables;
	deoglDebugInformation *pDebugInfoPlanPrepareSort;
	deoglDebugInformation *pDebugInfoPlanPrepareBuildPlan;
	deoglDebugInformation *pDebugInfoPlanPrepareLights;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create canvas renderer. */
	deoglRenderCanvas( deoglRenderThread &renderThread );
	
	/** \brief Clean up canvas renderer. */
	virtual ~deoglRenderCanvas();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Set opengl states required for canvas rendering. */
	void Prepare( const deoglRenderCanvasContext &context );
	
	/** \brief Draw canvas paint. */
	void DrawCanvasPaint( const deoglRenderCanvasContext &context, const deoglRCanvasPaint &canvas );
	
	/** \brief Draw canvas image. */
	void DrawCanvasImage( const deoglRenderCanvasContext &context, const deoglRCanvasImage &canvas );
	
	/** \brief Draw canvas canvas view. */
	void DrawCanvasCanvasView( const deoglRenderCanvasContext &context, const deoglRCanvasCanvasView &canvas );
	
	/** \brief Draw canvas render world. */
	void DrawCanvasRenderWorld( const deoglRenderCanvasContext &context, const deoglRCanvasRenderWorld &canvas );
	
	/** \brief Draw canvas video player. */
	void DrawCanvasVideoPlayer( const deoglRenderCanvasContext &context, const deoglRCanvasVideoPlayer &canvas );
	
	/** \brief Draw canvas text. */
	void DrawCanvasText( const deoglRenderCanvasContext &context, const deoglRCanvasText &canvas );
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** \brief Reset debug information canvas. */
	void DebugInfoCanvasReset();
	
	/** \brief Update debug information canvas. */
	void DebugInfoCanvasUpdate();
	
	
	
	/** \brief Clear all debug information prepare plan. */
	void ClearAllDebugInfoPlanPrepare( deoglRenderPlan &plan );
	
	/** \brief Sample debug information plan prepare. */
	void SampleDebugInfoPlanPrepare( deoglRenderPlan &plan );
	
	/** \brief Debug information plan prepare collect. */
	void SampleDebugInfoPlanPrepareCollect( deoglRenderPlan &plan );
	
	/** \brief Debug information plan prepare culling. */
	void SampleDebugInfoPlanPrepareCulling( deoglRenderPlan &plan );
	
	/** \brief Debug information plan prepare env-maps. */
	void SampleDebugInfoPlanPrepareEnvMaps( deoglRenderPlan &plan );
	
	/** \brief Debug information plan prepare ht-view vbos. */
	void SampleDebugInfoPlanPrepareHTViewVBOs( deoglRenderPlan &plan );
	
	/** \brief Debug information plan prepare components. */
	void SampleDebugInfoPlanPrepareComponents( deoglRenderPlan &plan );
	
	/** \brief Debug information plan prepare components vbo. */
	void SampleDebugInfoPlanPrepareComponentsVBO( deoglRenderPlan &plan );
	
	/** \brief Debug information plan prepare components renderables. */
	void SampleDebugInfoPlanPrepareComponentsRenderables( deoglRenderPlan &plan );
	
	/** \brief Debug information plan prepare sort. */
	void SampleDebugInfoPlanPrepareSort( deoglRenderPlan &plan );
	
	/** \brief Debug information plan prepare build plan. */
	void SampleDebugInfoPlanPrepareBuildPlan( deoglRenderPlan &plan );
	
	/** \brief Debug information plan prepare lights. */
	void SampleDebugInfoPlanPrepareLights( deoglRenderPlan &plan );
	
	
	
	/** \brief Add top level debug information in the right order. */
	virtual void AddTopLevelDebugInfo();
	
	/** \brief Developer mode debug information changed. */
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
