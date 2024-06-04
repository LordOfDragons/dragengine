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

#ifndef _DEOGLRENDERTONEMAP_H_
#define _DEOGLRENDERTONEMAP_H_

#include "deoglRenderBase.h"

#include <dragengine/common/math/decMath.h>

class deoglRenderPlan;
class deoglFramebuffer;
class deoglTexture;



/**
 * OpenGL ToneMap Renderer.
 * Renderer for tone mapping related passes.
 */
class deoglRenderToneMap : public deoglRenderBase{
private:
	deoglFramebuffer *pFBOToneMapParams;
	deoglTexture *pTextureToneMapParams;
	
	const deoglPipeline *pPipelineColor2LogLum;
	const deoglPipeline *pPipelineColor2LogLumStereo;
	const deoglPipeline *pPipelineAvgLogLum;
	const deoglPipeline *pPipelineAvgLogLumStereo;
	const deoglPipeline *pPipelineParameters;
	const deoglPipeline *pPipelineParametersStereo;
	const deoglPipeline *pPipelineBrightPass;
	const deoglPipeline *pPipelineBrightPassStereo;
	const deoglPipeline *pPipelineBloomDownSample;
	const deoglPipeline *pPipelineBloomDownSampleStereo;
	const deoglPipeline *pPipelineBloomBlur;
	const deoglPipeline *pPipelineBloomBlurStereo;
	const deoglPipeline *pPipelineBloomAdd;
	const deoglPipeline *pPipelineBloomAddStereo;
	const deoglPipeline *pPipelineToneMap;
	const deoglPipeline *pPipelineToneMapStereo;
	const deoglPipeline *pPipelineToneMapCustom;
	const deoglPipeline *pPipelineToneMapCustomStereo;
	const deoglPipeline *pPipelineLdr;
	const deoglPipeline *pPipelineLdrStereo;
	
	const deoglPipeline *pPipelineLumPrepare;
	const deoglPipeline *pPipelineLumPrepareStereo;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new renderer. */
	deoglRenderToneMap( deoglRenderThread &renderThread );
	/** Cleans up the renderer. */
	virtual ~deoglRenderToneMap();
	/*@}*/
	
	/** \name Rendering */
	/*@{*/
	/** Prepare luminance texture from solid color and depth texture. */
	void LuminancePrepare( deoglRenderPlan &plan );
	
	/** Tone maps the render. */
	void ToneMap( deoglRenderPlan &plan );
	
	/** Calculates the scene key. */
	void CalculateSceneKey( deoglRenderPlan &plan );
	/** Renders the bloom pass. */
	void RenderBloomPass( deoglRenderPlan &plan, int &bloomWidth, int &bloomHeight );
	/** Renders the tone mapping pass. */
	void RenderToneMappingPass( deoglRenderPlan &plan, int bloomWidth, int bloomHeight );
	
	/** LDR render the screen. */
	void RenderLDR( deoglRenderPlan &plan );
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
