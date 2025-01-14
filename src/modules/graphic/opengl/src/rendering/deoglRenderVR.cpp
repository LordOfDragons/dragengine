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

#include <stdio.h>
#include <string.h>

#include "deoglRenderVR.h"
#include "defren/deoglDeferredRendering.h"
#include "plan/deoglRenderPlan.h"
#include "../model/deoglModelLOD.h"
#include "../model/deoglRModel.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTChoices.h"
#include "../renderthread/deoglRTDebug.h"
#include "../renderthread/deoglRTDefaultTextures.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTTexture.h"
#include "../shaders/deoglShaderCompiled.h"
#include "../shaders/deoglShaderDefines.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/deoglShaderSources.h"
#include "../vao/deoglVAO.h"
#include "../vbo/deoglSharedVBO.h"
#include "../vbo/deoglSharedVBOBlock.h"
#include "../vr/deoglVR.h"
#include "../world/deoglRCamera.h"

#include <dragengine/common/exceptions.h>


// Class deoglRenderVR
////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderVR::deoglRenderVR( deoglRenderThread &renderThread ) :
deoglRenderBase( renderThread )
{
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	const bool useInverseDepth = renderThread.GetChoices().GetUseInverseDepth();
	deoglPipelineConfiguration pipconf, pipconf2;
	deoglShaderDefines defines, commonDefines;
	const deoglShaderSources *sources;
	
	try{
		renderThread.GetShader().SetCommonDefines( commonDefines );
		
		// hidden area
		pipconf.Reset();
		pipconf.SetMasks( false, false, false, false, false );
		pipconf.EnableDepthTestAlways();
		pipconf.SetEnableScissorTest( true );
		pipconf.SetEnableStencilTest( true );
		pipconf.SetStencil( GL_ALWAYS, 0x0, 0x01, 0x01 );
		pipconf.SetStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
		
		pipconf2 = pipconf;
		pipconf2.SetDepthMask( true );
		pipconf2.SetDepthFunc( renderThread.GetChoices().GetDepthCompareFuncRegular() );
		pipconf2.SetEnableStencilTest( false );
		pipconf2.SetClipControl( useInverseDepth );
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "VR Hidden Area" );
		pAsyncGetPipeline(pPipelineHiddenAreaClearMask, pipconf, sources, defines);
		pAsyncGetPipeline(pPipelineHiddenAreaDepth, pipconf2, sources, defines);
		
		// hidden area stereo left
		sources = shaderManager.GetSourcesNamed( "VR Hidden Area Stereo" );
		defines.SetDefine( "RENDER_TO_LAYER", 0 );
		pAsyncGetPipeline(pPipelineHiddenAreaClearMaskStereoLeft, pipconf, sources, defines);
		pAsyncGetPipeline(pPipelineHiddenAreaDepthStereoLeft, pipconf2, sources, defines);
		
		// hidden area stereo right
		defines.SetDefine( "RENDER_TO_LAYER", 1 );
		pAsyncGetPipeline(pPipelineHiddenAreaClearMaskStereoRight, pipconf, sources, defines);
		pAsyncGetPipeline(pPipelineHiddenAreaDepthStereoRight, pipconf2, sources, defines);
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRenderVR::~deoglRenderVR(){
	pCleanUp();
}



// Rendering
//////////////

void deoglRenderVR::RenderHiddenArea( deoglRenderPlan &plan, bool clearMask ){
	if( ! plan.GetCamera() || ! plan.GetCamera()->GetVR() ){
		return;
	}
	
	const deoglVR &vr = *plan.GetCamera()->GetVR();
	deoglRModel *model = nullptr;
	deoglRModel *modelLeft = nullptr;
	deoglRModel *modelRight = nullptr;
	
	switch( plan.GetRenderVR() ){
	case deoglRenderPlan::ervrLeftEye:
		model = vr.GetLeftEye().GetHiddenMesh();
		break;
		
	case deoglRenderPlan::ervrRightEye:
		model = vr.GetRightEye().GetHiddenMesh();
		break;
		
	case deoglRenderPlan::ervrStereo:
		modelLeft = vr.GetLeftEye().GetHiddenMesh();
		modelRight = vr.GetRightEye().GetHiddenMesh();
		break;
		
	case deoglRenderPlan::ervrNone:
		return;
	}
	
	if( ! model && ! modelLeft && ! modelRight ){
		return;
	}
	
	if( model ){
		deoglModelLOD &lod = model->GetLODAt( 0 );
		lod.PrepareVBOBlock();
		const deoglSharedVBOBlock * const vboBlock = lod.GetVBOBlock();
		if( vboBlock ){
			( clearMask ? pPipelineHiddenAreaClearMask : pPipelineHiddenAreaDepth )->Activate();
			pRenderHiddenArea( *vboBlock );
		}
		
	}else{
		if( modelLeft ){
			deoglModelLOD &lod = modelLeft->GetLODAt( 0 );
			lod.PrepareVBOBlock();
			const deoglSharedVBOBlock * const vboBlock = lod.GetVBOBlock();
			if( vboBlock ){
				( clearMask ? pPipelineHiddenAreaClearMaskStereoLeft : pPipelineHiddenAreaDepthStereoLeft )->Activate();
				pRenderHiddenArea( *vboBlock );
			}
		}
		
		if( modelRight ){
			deoglModelLOD &lod = modelRight->GetLODAt( 0 );
			lod.PrepareVBOBlock();
			const deoglSharedVBOBlock * const vboBlock = lod.GetVBOBlock();
			if( vboBlock ){
				( clearMask ? pPipelineHiddenAreaClearMaskStereoRight : pPipelineHiddenAreaDepthStereoRight )->Activate();
				pRenderHiddenArea( *vboBlock );
			}
		}
	}
	
	pglBindVertexArray( 0 );
}



// Private Functions
//////////////////////

void deoglRenderVR::pCleanUp(){
}

void deoglRenderVR::pRenderHiddenArea( const deoglSharedVBOBlock &vboBlock ){
	deoglRenderThread &renderThread = GetRenderThread();
	
	const deoglVAO &vao = *vboBlock.GetVBO()->GetVAO();
	pglBindVertexArray( vao.GetVAO() );
	
	if( renderThread.GetChoices().GetSharedVBOUseBaseVertex() ){
		OGL_CHECK( renderThread, pglDrawElementsBaseVertex( GL_TRIANGLES,
			vboBlock.GetIndexCount(), vao.GetIndexGLType(),
			( GLvoid* )( intptr_t )( vao.GetIndexSize() * vboBlock.GetIndexOffset() ),
			vboBlock.GetOffset() ) );
		
	}else{
		OGL_CHECK( renderThread, glDrawElements( GL_TRIANGLES,
			vboBlock.GetIndexCount(), vao.GetIndexGLType(),
			( GLvoid* )( intptr_t )( vao.GetIndexSize() * vboBlock.GetIndexOffset() ) ) );
	}
}
