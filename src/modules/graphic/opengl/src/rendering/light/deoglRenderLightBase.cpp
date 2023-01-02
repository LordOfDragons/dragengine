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

#include "deoglRenderLight.h"
#include "deoglRenderLightBase.h"
#include "deoglRenderGI.h"
#include "../defren/deoglDeferredRendering.h"
#include "../plan/deoglRenderPlan.h"
#include "../plan/deoglRenderPlanLight.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../collidelist/deoglCollideListComponent.h"
#include "../../collidelist/deoglCollideListLight.h"
#include "../../component/deoglComponentSet.h"
#include "../../component/deoglRComponent.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../devmode/deoglDeveloperMode.h"
#include "../../framebuffer/deoglFramebuffer.h"
#include "../../framebuffer/deoglFramebufferManager.h"
#include "../../gi/deoglGI.h"
#include "../../gi/deoglGITraceRays.h"
#include "../../gi/deoglGIState.h"
#include "../../light/deoglRLight.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTDebug.h"
#include "../../renderthread/deoglRTTexture.h"
#include "../../renderthread/deoglRTDefaultTextures.h"
#include "../../renderthread/deoglRTFramebuffer.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../texture/cubemap/deoglCubeMap.h"
#include "../../texture/cubemap/deoglRenderableDepthCubeMap.h"
#include "../../texture/deoglTextureStageManager.h"
#include "../../texture/texture2d/deoglRenderableDepthTexture.h"
#include "../../texture/texture2d/deoglTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderLightBase
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderLightBase::deoglRenderLightBase( deoglRenderThread &renderThread ) : deoglRenderBase( renderThread ){
}

deoglRenderLightBase::~deoglRenderLightBase(){
}



// Management
///////////////

void deoglRenderLightBase::AddComponentsToColliderList( const deoglComponentSet &list ){
	const int count = list.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pColList.AddComponent( list.GetAt( i ) );
	}
}



decColor deoglRenderLightBase::TransformColor( const decMatrix &matrix, const decColor &color ) const{
	decVector vector = matrix * decVector( color.r, color.g, color.b );
	return decColor( vector.x, vector.y, vector.z );
}

void deoglRenderLightBase::RestoreFBO( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	const bool sssssEnable = renderThread.GetConfiguration().GetSSSSSEnable();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglPipelineState &state = renderThread.GetPipelineManager().GetState();
	
	if( sssssEnable ){
		defren.ActivateFBOColorTemp2( false, true );
		
	}else{
		defren.ActivateFBOColor( false, true );
	}
	
	state.StencilMask( 0 );
	state.StencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
	state.StencilFunc( GL_EQUAL, plan.GetStencilRefValue(), ~0 );
	
	tsmgr.EnableArrayTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
	
	if( renderThread.GetCapabilities().GetMaxDrawBuffers() >= 8 ){
		tsmgr.EnableArrayTexture( 1, *defren.GetTextureDiffuse(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 2, *defren.GetTextureNormal(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 3, *defren.GetTextureReflectivity(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 4, *defren.GetTextureRoughness(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 5, *defren.GetTextureAOSolidity(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 6, *defren.GetTextureSubSurface(), GetSamplerClampNearest() );
		
	}else{
		tsmgr.EnableArrayTexture( 1, *defren.GetTextureDiffuse(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 2, *defren.GetTextureNormal(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 3, *defren.GetTextureReflectivity(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 4, *renderThread.GetDefaultTextures().GetAO(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 5, *renderThread.GetDefaultTextures().GetAO(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 6, *renderThread.GetDefaultTextures().GetColor(), GetSamplerClampNearest() );
	}
	
	tsmgr.DisableStagesAbove( 6 );
}

void deoglRenderLightBase::RestoreDRTexturesSmooth(){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	
	if( renderThread.GetCapabilities().GetMaxDrawBuffers() >= 8 ){
		tsmgr.EnableArrayTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 1, *defren.GetTextureDiffuse(), GetSamplerClampLinear() );
		tsmgr.EnableArrayTexture( 2, *defren.GetTextureNormal(), GetSamplerClampLinear() );
		tsmgr.EnableArrayTexture( 3, *defren.GetTextureReflectivity(), GetSamplerClampLinear() );
		tsmgr.EnableArrayTexture( 4, *defren.GetTextureRoughness(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 5, *defren.GetTextureAOSolidity(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 6, *defren.GetTextureSubSurface(), GetSamplerClampNearest() );
		
	}else{
		tsmgr.EnableArrayTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
		tsmgr.EnableArrayTexture( 1, *defren.GetTextureDiffuse(), GetSamplerClampLinear() );
		tsmgr.EnableArrayTexture( 2, *defren.GetTextureNormal(), GetSamplerClampLinear() );
		tsmgr.EnableArrayTexture( 3, *defren.GetTextureReflectivity(), GetSamplerClampLinear() );
		tsmgr.EnableTexture( 4, *renderThread.GetDefaultTextures().GetAO(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 5, *renderThread.GetDefaultTextures().GetAO(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 6, *renderThread.GetDefaultTextures().GetColor(), GetSamplerClampNearest() );
	}
	
	tsmgr.DisableStagesAbove( 6 );
}

void deoglRenderLightBase::RestoreDRTextureDepthSmooth(){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	
	tsmgr.EnableArrayTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
	tsmgr.DisableStagesAbove( 0 );
}

void deoglRenderLightBase::RestoreFBOGITraceRays( deoglGIState &giState ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	
	deoglGITraceRays &giTraceRays = renderThread.GetGI().GetTraceRays();
	renderThread.GetFramebuffer().Activate( &giTraceRays.GetFBOLight() );
	
	SetViewport( giState.GetSampleImageSize() );
	
	tsmgr.EnableTexture( 0, giTraceRays.GetTexturePosition(), GetSamplerClampNearest() );
	tsmgr.EnableTexture( 1, giTraceRays.GetTextureDiffuse(), GetSamplerClampNearest() );
	tsmgr.EnableTexture( 2, giTraceRays.GetTextureNormal(), GetSamplerClampNearest() );
	tsmgr.EnableTexture( 3, giTraceRays.GetTextureReflectivity(), GetSamplerClampNearest() );
	tsmgr.DisableStagesAbove( 3 );
}



void deoglRenderLightBase::AddTopLevelDebugInfoSolid(){
}

void deoglRenderLightBase::AddTopLevelDebugInfoTransparent(){
}



// Protected Functions
////////////////////////

int deoglRenderLightBase::pPipelineModifiers( const deoglRenderPlanLight &planLight,
bool solid, bool hasAmbient ) const{
	int modifiers = 0;
	
	if( ! hasAmbient ){
		modifiers |= deoglLightPipelines::emNoAmbient;
	}
	
	if( ! solid ){
		modifiers |= deoglLightPipelines::emTransparent;
	}
	
	if( planLight.GetPlan().GetRenderStereo() ){
		modifiers |= deoglLightPipelines::emStereo;
	}
	
	if( planLight.GetLight()->GetCameraInside() ){
		// cull front faces, no shader depth test
		// 
		// TODO there is a potential problem here. if the back faces extend beyond the far
		//      clip plane the lighting is discard. there are two solutions for this problem
		//      
		//      1)
		//      use GL_ARB_depth_clamp (GL_DEPTH_CLAMP). this allows to clamp the depth to
		//      the valid depth range without discarding fragments beyond the far clip plane.
		//      this requires checking for the presence of this extension. this seems to be
		//      though present in 3.2 and newer.
		//      
		//      using this solution requires adding the parameter to deoglPipelineConfiguration
		//      as well as using an "inside-camera" modifier to switch on depth clammping.
		//      
		//      2)
		//      since depth testing is disabled in the shader in camera inside mode we could
		//      also set gl_Position.z = 0 in the vertex shader. this places the depth either
		//      at the center between near and far clip plane (not inverse depth) or at the
		//      near clip plane (inverse depth). in both cases all vertices depth value will
		//      end up at a constant value inside the depth range. and since the fragment
		//      shader depth test is disabled this has no influence.
		//      
		//      this solution requires an "inside-camera" modifier to switch on this mode as
		//      solution 1 but does not require an extension to be present. using this
		//      solution has also the advantage to disable the if-def out the depth test code
		//      in the fragment shader altogether. using 0 for pDepthCompare is then not
		//      necessary anymore.
		//      
		modifiers |= deoglLightPipelines::emCameraInside;
		
		if( ! planLight.GetPlan().GetFlipCulling() ){
			modifiers |= deoglLightPipelines::emFlipCullFace;
		}
		
	}else{
		// cull back faces, no depth test (opengl can not handle depth test with depth read)
		if( planLight.GetPlan().GetFlipCulling() ){
			modifiers |= deoglLightPipelines::emFlipCullFace;
		}
	}
	
	return modifiers;
}
