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

#include "deoglRenderLightBase.h"
#include "../defren/deoglDeferredRendering.h"
#include "../plan/deoglRenderPlan.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../collidelist/deoglCollideListComponent.h"
#include "../../component/deoglComponentList.h"
#include "../../component/deoglRComponent.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../devmode/deoglDeveloperMode.h"
#include "../../framebuffer/deoglFramebuffer.h"
#include "../../framebuffer/deoglFramebufferManager.h"
#include "../../light/deoglRLight.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTDebug.h"
#include "../../renderthread/deoglRTTexture.h"
#include "../../renderthread/deoglRTDefaultTextures.h"
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

void deoglRenderLightBase::AddComponentsToColliderList( const deoglComponentList &list ){
	const int count = list.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pColList.AddComponent( list.GetAt( i ) );
	}
}

void deoglRenderLightBase::UpdateComponentVBO( const deoglCollideList &list ){
	const int count = list.GetComponentCount();
	int i;
	
	for( i=0; i <count; i++ ){
		list.GetComponentAt( i )->GetComponent()->UpdateVBO();
	}
}

void deoglRenderLightBase::UpdateComponentRenderables( deoglRenderPlan &plan,
const deoglCollideList &list ){
	const int count = list.GetComponentCount();
	int i;
	
	for( i=0; i <count; i++ ){
		list.GetComponentAt( i )->GetComponent()->UpdateRenderables( plan );
	}
}



decColor deoglRenderLightBase::TransformColor( const decMatrix &matrix, const decColor &color ) const{
	decVector vector = matrix * decVector( color.r, color.g, color.b );
	return decColor( vector.x, vector.y, vector.z );
}

void deoglRenderLightBase::RestoreFBO( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	const bool hasDepthCopy = renderThread.GetDebug().GetDeveloperMode().GetDebugEnableLightDepthStencil();
	const bool sssssEnable = renderThread.GetConfiguration().GetSSSSSEnable();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	
	if( sssssEnable ){
		defren.ActivateFBOColorTemp2( hasDepthCopy );
		
	}else{
		defren.ActivateFBOColor( hasDepthCopy );
	}
	
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	
	OGL_CHECK( renderThread, glDepthFunc( defren.GetDepthCompareFuncRegular() ) );
	if( pglClipControl && defren.GetUseInverseDepth() ){
		pglClipControl( GL_LOWER_LEFT, GL_ZERO_TO_ONE );
	}
	
	OGL_CHECK( renderThread, glEnable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glStencilMask( 0 ) );
	OGL_CHECK( renderThread, glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP ) );
	OGL_CHECK( renderThread, glStencilFunc( GL_EQUAL, plan.GetStencilRefValue(), ~0 ) );
	
	if( hasDepthCopy ){
		OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
		
		if( defren.GetUseEncodedDepth() ){
			tsmgr.EnableTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
			
		}else{
			tsmgr.EnableTexture( 0, *defren.GetDepthTexture2(), GetSamplerClampNearest() );
		}
		
	}else{
		OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
		tsmgr.EnableTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
	}
	
	if( renderThread.GetCapabilities().GetMaxDrawBuffers() >= 8 ){
		tsmgr.EnableTexture( 1, *defren.GetTextureDiffuse(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 2, *defren.GetTextureNormal(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 3, *defren.GetTextureReflectivity(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 4, *defren.GetTextureRoughness(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 5, *defren.GetTextureAOSolidity(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 6, *defren.GetTextureSubSurface(), GetSamplerClampNearest() );
		
	}else{
		tsmgr.EnableTexture( 1, *defren.GetTextureDiffuse(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 2, *defren.GetTextureNormal(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 3, *defren.GetTextureReflectivity(), GetSamplerClampNearest() );
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
		tsmgr.EnableTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 1, *defren.GetTextureDiffuse(), GetSamplerClampLinear() );
		tsmgr.EnableTexture( 2, *defren.GetTextureNormal(), GetSamplerClampLinear() );
		tsmgr.EnableTexture( 3, *defren.GetTextureReflectivity(), GetSamplerClampLinear() );
		tsmgr.EnableTexture( 4, *defren.GetTextureRoughness(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 5, *defren.GetTextureAOSolidity(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 6, *defren.GetTextureSubSurface(), GetSamplerClampNearest() );
		
	}else{
		tsmgr.EnableTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
		tsmgr.EnableTexture( 1, *defren.GetTextureDiffuse(), GetSamplerClampLinear() );
		tsmgr.EnableTexture( 2, *defren.GetTextureNormal(), GetSamplerClampLinear() );
		tsmgr.EnableTexture( 3, *defren.GetTextureReflectivity(), GetSamplerClampLinear() );
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
	
	tsmgr.EnableTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
	tsmgr.DisableStagesAbove( 0 );
}

void deoglRenderLightBase::RestoreFBO(){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	const bool sssssEnable = renderThread.GetConfiguration().GetSSSSSEnable();
	
	OGL_CHECK( renderThread, glViewport( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
	OGL_CHECK( renderThread, glScissor( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
	
	if( sssssEnable ){
		defren.ActivateFBOColorTemp2( true );
		
	}else{
		defren.ActivateFBOColor( true );
	}
}



void deoglRenderLightBase::AddTopLevelDebugInfoSolid(){
}

void deoglRenderLightBase::AddTopLevelDebugInfoTransparent(){
}
