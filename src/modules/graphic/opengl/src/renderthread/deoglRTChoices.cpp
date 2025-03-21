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

#include <dragengine/dragengine_configuration.h>

#include "deoglRenderThread.h"
#include "deoglRTChoices.h"
#include "deoglRTLogger.h"
#include "../gi/deoglGI.h"
#include "../capabilities/deoglCapabilities.h"
#include "../extensions/deoglExtensions.h"

#include <dragengine/common/exceptions.h>



// Class deoglRTChoices
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTChoices::deoglRTChoices( deoglRenderThread &renderThread ){
	const deoglConfiguration &conf = renderThread.GetConfiguration();
	const deoglCapabilities &caps = renderThread.GetCapabilities();
	const deoglExtensions &ext = renderThread.GetExtensions();
	
	#define HAS_EXT(e) ext.GetHasExtension(deoglExtensions::e)
	#define HAS_EXT_OR_GLES(e,v) (ext.GetHasExtension(deoglExtensions::e) || ext.GetGLESVersion() >= deoglExtensions:: v)
	
	// base-vertex allows to render Shared-VBO instances without needing to pre-offset indices
	// using the first point of a VBO block. using base-vertex reduces the amount of VBO required.
	// 
	// NOTE shared-SPB is not affected by this since it uses instancing.
	pSharedVBOUseBaseVertex = HAS_EXT_OR_GLES(ext_ARB_draw_elements_base_vertex, evgles3p2);
	//pSharedVBOUseBaseVertex = false;
	
	// Use SSBO for rendering if enough SSBO blocks are allowed.
	// NOTE: no geometry shader is currently using SSBO. for this reason it is not required
	//       for the hardware to support SSBOs in geometry shaders
	pUseSSBORender = HAS_EXT_OR_GLES(ext_ARB_shader_storage_buffer_object, evgles3p1)
		&& caps.GetSSBOMaxBlocksVertex() >= 4
		//&& caps.GetSSBOMaxBlocksGeometry() >= 4
		&& caps.GetSSBOMaxBlocksFragment() >= 4;
	
	// Using SSBOs allows to use a larger number of parameter block per shared SPB than using UBO
	pSharedSPBUseSSBO = pUseSSBORender;
	
	// use global shared SPB lists for SSBO. for UBO it is not favorable
	pGlobalSharedSPBLists = pSharedSPBUseSSBO;
	
	// render real transparent particles
	pRealTransparentParticles = false;
	
	// GI move using probes using ray cache instead of all rays
	#ifdef GI_USE_RAY_CACHE
	pGIMoveUsingCache = true;
	#else
	pGIMoveUsingCache = false;
	#endif
	
	// use render stereo rendering for VR
	pVRRenderStereo = true;
	
	// use layer in vertex shaders for stereo rendering. requires these extensions (% coverage):
	// - ARB_shader_viewport_layer_array (45%) or AMD_vertex_shader_layer (61%): gl_Layer in vertex shader
	// - ARB_multi_draw_indirect (73%): glMultiDrawElementsIndirect
	// - ARB_shader_draw_parameters (67%): gl_DrawID in vertex shader
	//   ^== this requires 4.6 core or it will not work
	pRenderStereoVSLayer =
		( HAS_EXT( ext_ARB_shader_viewport_layer_array ) || HAS_EXT( ext_AMD_vertex_shader_layer ) )
		&& HAS_EXT( ext_ARB_multi_draw_indirect )
		&& HAS_EXT( ext_ARB_draw_elements_base_vertex )
		&& HAS_EXT( ext_ARB_shader_draw_parameters );
	
	// use layer in vertex shaders for fullscreen quad rendering. requires these extensions (% coverage):
	// - ARB_shader_viewport_layer_array (45%) or AMD_vertex_shader_layer (61%): gl_Layer in vertex shader
	// - ARB_shader_draw_parameters (67%): gl_DrawID in vertex shader
	//   ^== this requires 4.6 core or it will not work
	pRenderFSQuadStereoVSLayer =
		( HAS_EXT( ext_ARB_shader_viewport_layer_array ) || HAS_EXT( ext_AMD_vertex_shader_layer ) )
		&& HAS_EXT( ext_ARB_shader_draw_parameters );
	
	pGPUTransformVertices = egputvApproximate;
	
	// inverse depth
	pUseInverseDepth = conf.GetUseInverseDepth();
	
	if( ! caps.GetFormats().GetUseFBOTex2DFormatFor( deoglCapsFmtSupport::eutfDepthF_Stencil )
	||  ! caps.GetFormats().GetUseFBOTex2DFormatFor( deoglCapsFmtSupport::eutfDepthF )
	||  ! caps.GetFormats().GetUseFBOTexCubeFormatFor( deoglCapsFmtSupport::eutfDepthF_Stencil )
	||  ! caps.GetFormats().GetUseFBOTexCubeFormatFor( deoglCapsFmtSupport::eutfDepthF )
	||  ! pglClipControl ){
		pUseInverseDepth = false; // not supported
	}
	
	if( pUseInverseDepth ){
		pDepthCompareFuncRegular = GL_GEQUAL;
		pDepthCompareFuncReversed = GL_LEQUAL;
		pClearDepthValueRegular = ( GLfloat )0.0f;
		pClearDepthValueReversed = ( GLfloat )1.0f;
		
	}else{
		pDepthCompareFuncRegular = GL_LEQUAL;
		pDepthCompareFuncReversed = GL_GEQUAL;
		pClearDepthValueRegular = ( GLfloat )1.0f;
		pClearDepthValueReversed = ( GLfloat )0.0f;
	}
	
	pUseDirectStateAccess = HAS_EXT( ext_ARB_direct_state_access );

	// temporary until working properly
	if( /* HASEXT( ext_ARB_shader_atomic_counter_ops ) && */ pUseSSBORender ){
		pUseComputeRenderTask = true;
		
	}else{
		pUseComputeRenderTask = false;
	}
	
	#undef HASEXT
	
	
	
	// log choices
	deoglRTLogger &l = renderThread.GetLogger();
	
	l.LogInfo( "Render Thread Choices:" );
	l.LogInfoFormat( "- Use SSBO for Rendering: %s", pUseSSBORender ? "Yes" : "No" );
	l.LogInfoFormat( "- Shared VBO Use Base Vertex: %s", pSharedVBOUseBaseVertex ? "Yes" : "No" );
	l.LogInfoFormat( "- Shared SPB Use SSBO: %s", pSharedSPBUseSSBO ? "Yes" : "No" );
	l.LogInfoFormat( "- Global Shared SPB Lists: %s", pGlobalSharedSPBLists ? "Yes" : "No" );
	l.LogInfoFormat( "- Real Transparent Particles: %s", pRealTransparentParticles ? "Yes" : "No" );
	
	switch( pGPUTransformVertices ){
	case egputvAccurate:
		l.LogInfo( "- GPU Transform Vertices: Accurate" );
		break;
		
	case egputvApproximate:
		l.LogInfo( "- GPU Transform Vertices: Approximate" );
		break;
		
	case egputvNone:
		l.LogInfo( "- GPU Transform Vertices: None" );
		break;
	}
	
	l.LogInfoFormat( "- GI Move Using Cache: %s", pGIMoveUsingCache ? "Yes" : "No" );
	l.LogInfoFormat( "- VR Render Stereo: %s", pVRRenderStereo ? "Yes" : "No" );
	l.LogInfoFormat( "- Render Stereo Vertex Shader Layer: %s", pRenderStereoVSLayer ? "Yes" : "No" );
	l.LogInfoFormat( "- Render Fullscreen Quad Stereo Vertex Shader Layer: %s", pRenderFSQuadStereoVSLayer ? "Yes" : "No" );
	l.LogInfoFormat( "- Use Inverse Depth: %s", pUseInverseDepth ? "Yes" : "No" );
	l.LogInfoFormat( "- Use Direct State Access: %s", pUseDirectStateAccess ? "Yes" : "No" );

	#ifdef OS_W32
		l.LogInfo( "- Windows: Force disable DSA on SSBO (driver misbehavior)" );
	#endif
	
	pRenderDocDebugFlags = 0;
	//pRenderDocDebugFlags = RenderDocDebugFlagGI;
}

deoglRTChoices::~deoglRTChoices(){
}
