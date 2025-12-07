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
#include "../texture/texture2d/deoglTexture.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../vao/deoglVAO.h"

#include <dragengine/common/exceptions.h>


#define USE_ASYNC_PIPELINE_GET_PROGRAM 1


// Class deoglRenderBase::cPipelineGetProgramListener
///////////////////////////////////////////////////////

deoglRenderBase::cPipelineGetProgramListener::cPipelineGetProgramListener(
deoglRenderThread &renderThread, const deoglPipeline *&pipeline,
const deoglPipelineConfiguration &config, bool assignRTSIndex) :
pRenderThread(renderThread),
pConfig(config),
pPipeline(pipeline),
pAssignRTSIndex(assignRTSIndex){
}

void deoglRenderBase::cPipelineGetProgramListener::GetProgramFinished(
const deoglShaderProgram *program){
	if(!program){
		anyPipelineShaderFailed = true;
		return;
	}
	
	pConfig.SetShader(program);
	pPipeline = pRenderThread.GetPipelineManager().GetWith(pConfig, pAssignRTSIndex);
}



// Class deoglRenderBase
//////////////////////////

bool deoglRenderBase::anyPipelineShaderFailed = false;


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
	
	defines.SetDefines( "SHARED_SPB" );
	
	if( renderThread.GetChoices().GetSharedSPBUseSSBO() ){
		defines.SetDefines( "SHARED_SPB_USE_SSBO" );
		
		if( bo.GetLayoutOccMeshInstanceSSBO()->GetOffsetPadding() >= 16 ){
			defines.SetDefine( "SHARED_SPB_PADDING", bo.GetLayoutOccMeshInstanceSSBO()->GetOffsetPadding() / 16 );
		}
		
	}else{
		// NOTE UBO requires array size to be constant, SSBO does not
		if( bo.GetLayoutOccMeshInstanceUBO()->GetElementCount() > 0 ){
			defines.SetDefine( "SHARED_SPB_ARRAY_SIZE", bo.GetLayoutOccMeshInstanceUBO()->GetElementCount() );
		}
		
		if( bo.GetLayoutOccMeshInstanceUBO()->GetOffsetPadding() >= 16 ){
			defines.SetDefine( "SHARED_SPB_PADDING", bo.GetLayoutOccMeshInstanceUBO()->GetOffsetPadding() / 16 );
		}
	}
	
	if( renderThread.GetChoices().GetUseComputeRenderTask() ){
		defines.SetDefines( "SPB_SSBO_INSTANCE_ARRAY" );
		
	}else{
		if( bo.GetInstanceArraySizeUBO() > 0 ){
			defines.SetDefine( "SPB_INSTANCE_ARRAY_SIZE", bo.GetInstanceArraySizeUBO() );
		}
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
	SetViewport( 0, 0, width, height );
}

void deoglRenderBase::SetViewport( const decPoint &point ) const{
	SetViewport( point.x, point.y );
}

void deoglRenderBase::SetViewport( const decPoint3 &point ) const{
	SetViewport( point.x, point.y );
}

void deoglRenderBase::SetViewport( int x, int y, int width, int height ) const{
	OGL_CHECK( pRenderThread, glViewport( x, y, width, height ) );
	OGL_CHECK( pRenderThread, glScissor( x, y, width, height ) );
}

void deoglRenderBase::SetViewport( const decPoint &offset, const decPoint &size ) const{
	SetViewport( offset.x, offset.y, size.x, size.y );
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

void deoglRenderBase::ClearRenderDocDebugTexture(){
	if(pRenderThread.GetChoices().GetRenderDocDebugFlags() != 0){
		deoglTexture &texture = *pRenderThread.GetDeferredRendering().GetTexRenderDocDebug();
		const deoglPixelBuffer::Ref pbuf(deoglPixelBuffer::Ref::NewWith(deoglPixelBuffer::epfFloat4, texture.GetWidth(), texture.GetHeight(), 1));
		pbuf->SetToFloatColor(0.0f, 0.0f, 0.0f, 0.0f);
		texture.SetPixels(pbuf);
		//OGL_CHECK(pRenderThread, pglMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT));
	}
}



// Protected Functions
////////////////////////

void deoglRenderBase::pAsyncGetPipeline(const deoglPipeline *&pipeline,
deoglPipelineConfiguration &config, const char *sources,
const deoglShaderDefines &defines, bool assignRTSIndex){
#ifdef USE_ASYNC_PIPELINE_GET_PROGRAM
	deoglShaderManager &sm = pRenderThread.GetShader().GetShaderManager();
	sm.GetProgramWithAsync(sm.GetSourcesNamed(sources), defines,
		new cPipelineGetProgramListener(pRenderThread, pipeline, config, assignRTSIndex));
	
#else
	config.SetShader(pRenderThread, sources, defines);
	pipeline = pRenderThread.GetPipelineManager().GetWith(config,
		config.GetSPBInstanceIndexBase() != -1);
#endif
}

void deoglRenderBase::pAsyncGetPipeline(const deoglPipeline *&pipeline,
deoglPipelineConfiguration &config, const deoglShaderSources *sources,
const deoglShaderDefines &defines, bool assignRTSIndex){
#ifdef USE_ASYNC_PIPELINE_GET_PROGRAM
	deoglShaderManager &sm = pRenderThread.GetShader().GetShaderManager();
	sm.GetProgramWithAsync(sources, defines,
		new cPipelineGetProgramListener(pRenderThread, pipeline, config, assignRTSIndex));
	
#else
	config.SetShader(pRenderThread, sources, defines);
	pipeline = pRenderThread.GetPipelineManager().GetWith(config,
		config.GetSPBInstanceIndexBase() != -1);
#endif
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
