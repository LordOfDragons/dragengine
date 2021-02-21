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

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "deoglRenderGI.h"
#include "defren/deoglDeferredRendering.h"
#include "plan/deoglRenderPlan.h"
#include "../component/deoglRComponent.h"
#include "../configuration/deoglConfiguration.h"
#include "../debug/deoglDebugSaveTexture.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../framebuffer/deoglFramebufferManager.h"
#include "../gi/deoglRayTraceField.h"
#include "../gi/deoglGI.h"
#include "../model/deoglModelLOD.h"
#include "../model/deoglRModel.h"
#include "../extensions/deoglExtensions.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTChoices.h"
#include "../renderthread/deoglRTDebug.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTTexture.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../renderthread/deoglRTLogger.h"
#include "../shaders/deoglShaderCompiled.h"
#include "../shaders/deoglShaderDefines.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/deoglShaderSources.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../shaders/paramblock/deoglSPBParameter.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../vao/deoglVAO.h"

#include <dragengine/common/exceptions.h>


// Definitions
////////////////



// Class deoglRenderGI
////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderGI::deoglRenderGI( deoglRenderThread &renderThread ) :
deoglRenderBase( renderThread ),

pShaderTraceRays( NULL )
{
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	deoglShaderSources *sources;
	deoglShaderDefines defines;
	
	try{
		sources = shaderManager.GetSourcesNamed( "DefRen GI Trace Rays" );
		pShaderTraceRays = shaderManager.GetProgramWith( sources, defines );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRenderGI::~deoglRenderGI(){
	pCleanUp();
}



// Rendering
//////////////

void deoglRenderGI::TraceRays( deoglRayTraceField &field ){
	// NOTE old FBO is restored because field FBO can be dropped after this call
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglFramebuffer * const oldfbo = renderThread.GetFramebuffer().GetActive();
	deoglGI &gi = renderThread.GetGI();
	
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
	OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
	
	renderThread.GetFramebuffer().Activate( field.GetFBORays() );
	
	const deoglTexture &texRays = field.GetTextureRays();
	OGL_CHECK( renderThread, glViewport( 0, 0, texRays.GetWidth(), texRays.GetHeight() ) );
	OGL_CHECK( renderThread, glScissor( 0, 0, texRays.GetWidth(), texRays.GetHeight() ) );
	
	tsmgr.EnableTBO( 0, gi.GetTBONodeBox().GetTBO(), GetSamplerClampNearest() );
	tsmgr.EnableTBO( 1, gi.GetTBOIndex().GetTBO(), GetSamplerClampNearest() );
	tsmgr.EnableTBO( 2, gi.GetTBOFace().GetTBO(), GetSamplerClampNearest() );
	tsmgr.EnableTBO( 3, gi.GetTBOVertex().GetTBO(), GetSamplerClampNearest() );
	tsmgr.DisableStagesAbove( 3 );
	
	renderThread.GetShader().ActivateShader( pShaderTraceRays );
	field.GetUBO().Activate();
	
	OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
	
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
	tsmgr.DisableAllStages();
	renderThread.GetFramebuffer().Activate( oldfbo );
}



// Private Functions
//////////////////////

void deoglRenderGI::pCleanUp(){
	if( pShaderTraceRays ){
		pShaderTraceRays->RemoveUsage();
	}
}
