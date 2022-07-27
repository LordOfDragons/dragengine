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

#ifndef _DEOGLRENDERTONEMAP_H_
#define _DEOGLRENDERTONEMAP_H_

#include "deoglRenderBase.h"

#include <dragengine/common/math/decMath.h>

class deoglRenderPlan;
class deoglFramebuffer;
class deoglTexture;



/**
 * @brief OpenGL ToneMap Renderer.
 * Renderer for tone mapping related passes.
 */
class deoglRenderToneMap : public deoglRenderBase{
private:
	deoglFramebuffer *pFBOToneMapParams;
	deoglTexture *pTextureToneMapParams;
	
	deoglShaderProgramUsage pShaderColor2LogLum;
	deoglShaderProgramUsage pShaderColor2LogLumStereo;
	deoglShaderProgramUsage pShaderAvgLogLum;
	deoglShaderProgramUsage pShaderAvgLogLumStereo;
	deoglShaderProgramUsage pShaderParameters;
	deoglShaderProgramUsage pShaderBrightPass;
	deoglShaderProgramUsage pShaderBrightPassStereo;
	deoglShaderProgramUsage pShaderBloomReduce;
	deoglShaderProgramUsage pShaderBloomBlur;
	deoglShaderProgramUsage pShaderBloomBlurStereo;
	deoglShaderProgramUsage pShaderBloomAdd;
	deoglShaderProgramUsage pShaderToneMap;
	deoglShaderProgramUsage pShaderToneMapStereo;
	deoglShaderProgramUsage pShaderFinalize;
	deoglShaderProgramUsage pShaderFinalizeStereo;
	
	deoglShaderProgramUsage pShaderLumPrepare;
	deoglShaderProgramUsage pShaderLumPrepareStereo;
	
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new renderer. */
	deoglRenderToneMap( deoglRenderThread &renderThread );
	/** Cleans up the renderer. */
	virtual ~deoglRenderToneMap();
	/*@}*/
	
	/** @name Rendering */
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
