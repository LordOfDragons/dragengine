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

#include "deoglRenderBase.h"
#include "plan/deoglRenderPlan.h"
#include "defren/deoglDeferredRendering.h"

#include "../configuration/deoglConfiguration.h"
#include "../capabilities/deoglCapabilities.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTDebug.h"
#include "../renderthread/deoglRTChoices.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../debug/deoglDebugInformation.h"
#include "../devmode/deoglDeveloperMode.h"
#include "../shaders/deoglShaderDefines.h"
#include "../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../vao/deoglVAO.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderBase
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderBase::deoglRenderBase( deoglRenderThread &renderThread ) :
pRenderThread( renderThread )
{
	deoglPipelineManager &pipelineManager = renderThread.GetPipelineManager();
	deoglPipelineConfiguration pipconf;
	
	pipconf.SetMasks( true, true, true, true, true );
	pPipelineClearBuffers = pipelineManager.GetWith( pipconf );
}

deoglRenderBase::~deoglRenderBase(){
}



// Management
///////////////

void deoglRenderBase::AddBasicDefines( deoglShaderDefines &defines ){
	if( pRenderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Broken() ){
		defines.SetDefines( "UBO_IDMATACCBUG" );
	}
	
	if( pRenderThread.GetCapabilities().GetUBODirectLinkDeadloop().Broken() ){
		defines.SetDefines( "BUG_UBO_DIRECT_LINK_DEAD_LOOP" );
	}
}

void deoglRenderBase::AddSharedSPBDefines( deoglShaderDefines &defines ){
	const deoglRenderThread &renderThread = GetRenderThread();
	const deoglRTBufferObject &bo = renderThread.GetBufferObject();
	decString value;
	
	defines.SetDefines( "SHARED_SPB" );
	
	if( renderThread.GetChoices().GetSharedSPBUseSSBO() ){
		defines.SetDefines( "SHARED_SPB_USE_SSBO" );
		
		if( bo.GetLayoutOccMeshInstanceSSBO()->GetOffsetPadding() >= 16 ){
			value.SetValue( bo.GetLayoutOccMeshInstanceSSBO()->GetOffsetPadding() / 16 );
			defines.SetDefine( "SHARED_SPB_PADDING", value );
		}
		
	}else{
		// NOTE UBO requires array size to be constant, SSBO does not
		if( bo.GetLayoutOccMeshInstanceUBO()->GetElementCount() > 0 ){
			value.SetValue( bo.GetLayoutOccMeshInstanceUBO()->GetElementCount() );
			defines.SetDefine( "SHARED_SPB_ARRAY_SIZE", value );
		}
		
		if( bo.GetLayoutOccMeshInstanceUBO()->GetOffsetPadding() >= 16 ){
			value.SetValue( bo.GetLayoutOccMeshInstanceUBO()->GetOffsetPadding() / 16 );
			defines.SetDefine( "SHARED_SPB_PADDING", value );
		}
	}
	
	if( bo.GetInstanceArraySizeUBO() > 0 ){
		value.SetValue( bo.GetInstanceArraySizeUBO() );
		defines.SetDefine( "SPB_INSTANCE_ARRAY_SIZE", value );
	}
}

deoglTexSamplerConfig &deoglRenderBase::GetSamplerClampNearest() const{
	return *pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampNearest );
}

deoglTexSamplerConfig &deoglRenderBase::GetSamplerClampLinear() const{
	return *pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampLinear );
}

deoglTexSamplerConfig &deoglRenderBase::GetSamplerClampNearestMipMap() const{
	return *pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampNearestMipMap );
}

deoglTexSamplerConfig &deoglRenderBase::GetSamplerClampLinearMipMap() const{
	return *pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampLinearMipMap );
}

deoglTexSamplerConfig &deoglRenderBase::GetSamplerClampLinearMipMapNearest() const{
	return *pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampLinearMipMapNearest );
}

deoglTexSamplerConfig &deoglRenderBase::GetSamplerRepeatNearest() const{
	return *pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscRepeatNearest );
}

deoglTexSamplerConfig &deoglRenderBase::GetSamplerRepeatLinear() const{
	return *pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscRepeatLinear );
}

deoglTexSamplerConfig &deoglRenderBase::GetSamplerRepeatLinearMipMap() const{
	return *pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscRepeatLinearMipMap );
}

deoglTexSamplerConfig &deoglRenderBase::GetSamplerShadowClampLinear() const{
	return *pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscShadowClampLinear );
}

deoglTexSamplerConfig &deoglRenderBase::GetSamplerShadowClampLinearInverse() const{
	if( pRenderThread.GetChoices().GetUseInverseDepth() ){
		return *pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscShadowClampLinearInverse );
		
	}else{
		return *pRenderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscShadowClampLinear );
	}
}



void deoglRenderBase::SetViewport( const deoglRenderPlan &plan ) const{
	SetViewport( plan.GetViewportWidth(), plan.GetViewportHeight() );
}

void deoglRenderBase::SetViewport( int width, int height ) const{
	OGL_CHECK( pRenderThread, glViewport( 0, 0, width, height ) );
	OGL_CHECK( pRenderThread, glScissor( 0, 0, width, height ) );
}

void deoglRenderBase::SetViewport( const decPoint &point ) const{
	SetViewport( point.x, point.y );
}

void deoglRenderBase::SetViewport( const decPoint3 &point ) const{
	SetViewport( point.x, point.y );
}


void deoglRenderBase::SetCullMode( bool renderBackFaces ) const{
	OGL_CHECK( GetRenderThread(), glCullFace( renderBackFaces ? GL_FRONT : GL_BACK ) );
}

void deoglRenderBase::RenderFullScreenQuad() const{
	OGL_CHECK( pRenderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
}

void deoglRenderBase::RenderFullScreenQuad( const deoglRenderPlan &plan ) const{
	if( plan.GetRenderStereo() ){
		OGL_CHECK( pRenderThread, glDrawArrays( GL_TRIANGLES, 0, 12 ) );
		
	}else{
		OGL_CHECK( pRenderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
	}
}

void deoglRenderBase::RenderFullScreenQuadVAO() const{
	OGL_CHECK( pRenderThread, pglBindVertexArray(
		pRenderThread.GetDeferredRendering().GetVAOFullScreenQuad()->GetVAO() ) );
	OGL_CHECK( pRenderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
	OGL_CHECK( pRenderThread, pglBindVertexArray( 0 ) );
}

void deoglRenderBase::RenderFullScreenQuadVAO( const deoglRenderPlan &plan ) const{
	RenderFullScreenQuadVAO( plan.GetRenderStereo() );
}

void deoglRenderBase::RenderFullScreenQuadVAO( bool useStereo ) const{
	OGL_CHECK( pRenderThread, pglBindVertexArray(
		pRenderThread.GetDeferredRendering().GetVAOFullScreenQuad()->GetVAO() ) );
	
	if( useStereo ){
		OGL_CHECK( pRenderThread, glDrawArrays( GL_TRIANGLES, 0, 12 ) );
		
	}else{
		OGL_CHECK( pRenderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
	}
	
	OGL_CHECK( pRenderThread, pglBindVertexArray( 0 ) );
}



// Debug
//////////

void deoglRenderBase::DebugTimersReset( const deoglRenderPlan &plan, bool waitGPU ) {
	if( ! plan.GetDebugTiming() ){
		return;
	}
	
	if( waitGPU && pRenderThread.GetDebug().GetDeveloperMode().GetDebugInfoSync() ){
		glFinish();
	}
	
	pDebugTimer[ 0 ].Reset();
	pDebugTimer[ 1 ].Reset();
	pDebugTimer[ 2 ].Reset();
	pDebugTimer[ 3 ].Reset();
}

void deoglRenderBase::DebugTimer1Reset( const deoglRenderPlan &plan, bool waitGPU ){
	pDebugTimerReset( pDebugTimer[ 0 ], plan, waitGPU );
}

void deoglRenderBase::DebugTimer1Sample( const deoglRenderPlan &plan,
deoglDebugInformation &debugInfo, bool waitGPU ){
	pDebugTimerSample( pDebugTimer[ 0 ], plan, debugInfo, waitGPU );
}

void deoglRenderBase::DebugTimer1SampleCount( const deoglRenderPlan &plan,
deoglDebugInformation &debugInfo, int count, bool waitGPU ){
	pDebugTimerSampleCount( pDebugTimer[ 0 ], plan, debugInfo, count, waitGPU );
}

void deoglRenderBase::DebugTimer2Reset( const deoglRenderPlan &plan, bool waitGPU ){
	pDebugTimerReset( pDebugTimer[ 1 ], plan, waitGPU );
}

void deoglRenderBase::DebugTimer2Sample( const deoglRenderPlan &plan,
deoglDebugInformation &debugInfo, bool waitGPU ){
	pDebugTimerSample( pDebugTimer[ 1 ], plan, debugInfo, waitGPU );
}

void deoglRenderBase::DebugTimer2SampleCount( const deoglRenderPlan &plan,
deoglDebugInformation &debugInfo, int count, bool waitGPU ){
	pDebugTimerSampleCount( pDebugTimer[ 1 ], plan, debugInfo, count, waitGPU );
}

void deoglRenderBase::DebugTimer3Reset( const deoglRenderPlan &plan, bool waitGPU ){
	pDebugTimerReset( pDebugTimer[ 2 ], plan, waitGPU );
}

void deoglRenderBase::DebugTimer3Sample( const deoglRenderPlan &plan,
deoglDebugInformation &debugInfo, bool waitGPU ){
	pDebugTimerSample( pDebugTimer[ 2 ], plan, debugInfo, waitGPU );
}

void deoglRenderBase::DebugTimer3SampleCount( const deoglRenderPlan &plan,
deoglDebugInformation &debugInfo, int count, bool waitGPU ){
	pDebugTimerSampleCount( pDebugTimer[ 2 ], plan, debugInfo, count, waitGPU );
}

void deoglRenderBase::DebugTimer4Reset( const deoglRenderPlan &plan, bool waitGPU ){
	pDebugTimerReset( pDebugTimer[ 3 ], plan, waitGPU );
}

void deoglRenderBase::DebugTimer4Sample( const deoglRenderPlan &plan,
deoglDebugInformation &debugInfo, bool waitGPU ){
	pDebugTimerSample( pDebugTimer[ 3 ], plan, debugInfo, waitGPU );
}

void deoglRenderBase::DebugTimer4SampleCount( const deoglRenderPlan &plan,
deoglDebugInformation &debugInfo, int count, bool waitGPU ){
	pDebugTimerSampleCount( pDebugTimer[ 3 ], plan, debugInfo, count, waitGPU );
}

void deoglRenderBase::DebugTimerIncrement( const deoglRenderPlan &plan,
deoglDebugInformation &debugInfo, float elapsed, int count ){
	if( ! plan.GetDebugTiming() || ! debugInfo.GetVisible() ){
		return;
	}
	
	debugInfo.IncrementElapsedTime( elapsed );
	if( count > 0 ){
		debugInfo.IncrementCounter( count );
	}
}



void deoglRenderBase::AddTopLevelDebugInfo(){
}

void deoglRenderBase::DevModeDebugInfoChanged(){
}



// Private Functions
//////////////////////

void deoglRenderBase::pDebugTimerReset( decTimer &timer, const deoglRenderPlan &plan, bool waitGPU ){
	if( ! plan.GetDebugTiming() ){
		return;
	}
	
	if( waitGPU && pRenderThread.GetDebug().GetDeveloperMode().GetDebugInfoSync() ){
		glFinish();
	}
	timer.Reset();
}

void deoglRenderBase::pDebugTimerSample( decTimer &timer, const deoglRenderPlan &plan,
deoglDebugInformation &debugInfo, bool waitGPU ){
	if( ! plan.GetDebugTiming() || ! debugInfo.GetVisible() ){
		return;
	}
	
	if( waitGPU && pRenderThread.GetDebug().GetDeveloperMode().GetDebugInfoSync() ){
		glFinish();
	}
	debugInfo.IncrementElapsedTime( timer.GetElapsedTime() );
}

void deoglRenderBase::pDebugTimerSampleCount( decTimer &timer, const deoglRenderPlan &plan,
deoglDebugInformation &debugInfo, int count, bool waitGPU ){
	if( ! plan.GetDebugTiming() || ! debugInfo.GetVisible() ){
		return;
	}
	
	if( waitGPU && pRenderThread.GetDebug().GetDeveloperMode().GetDebugInfoSync() ){
		glFinish();
	}
	debugInfo.IncrementElapsedTime( timer.GetElapsedTime() );
	debugInfo.IncrementCounter( count );
}
