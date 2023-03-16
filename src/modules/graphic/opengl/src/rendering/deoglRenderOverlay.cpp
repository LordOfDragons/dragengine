/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "deoglRenderCanvasContext.h"
#include "deoglRenderOverlay.h"
#include "../deoglResources.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTShader.h"
#include "../shaders/deoglShaderCompiled.h"
#include "../shaders/deoglShaderDefines.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/deoglShaderSources.h"
#include "../shaders/deoglShaderCompilingInfo.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/texture2d/deoglTexture.h"

#include <dragengine/common/exceptions.h>


// Definitions
////////////////

enum eSPImage{
	espiColor,
	espiPosTransform,
	espiTCTransform
};



// Class deoglRenderOverlay
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderOverlay::deoglRenderOverlay( deoglRenderThread &renderThread ) :
deoglRenderBase( renderThread )
{
	deoglPipelineManager &pipelineManager = renderThread.GetPipelineManager();
	deoglShaderDefines defines, commonDefines;
	deoglPipelineConfiguration pipconf;
	
	
	renderThread.GetShader().SetCommonDefines( commonDefines );
	
	pipconf.Reset();
	pipconf.SetMasks( true, true, true, true, false );
	pipconf.EnableBlendBlend();
	pipconf.SetEnableScissorTest( true );
	
	
	// image
	defines = commonDefines;
	pipconf.SetShader( renderThread, "2D Textured", defines );
	pPipelineImage = pipelineManager.GetWith( pipconf );
}

deoglRenderOverlay::~deoglRenderOverlay(){
}



// Rendering
//////////////

void deoglRenderOverlay::RenderOverlays( const deoglRenderCanvasContext &context ){
	RenderShaderCompilingInfo( context );
}

void deoglRenderOverlay::RenderShaderCompilingInfo( const deoglRenderCanvasContext &context ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglShaderCompilingInfo &scinfo = renderThread.GetShader().GetShaderCompilingInfo();
	if( ! scinfo.IsVisible() ){
		return;
	}
	
	// TODO
}
