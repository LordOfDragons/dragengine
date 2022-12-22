/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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
	deoglPipelineManager &pipelineManager = renderThread.GetPipelineManager();
	deoglPipelineConfiguration pipconf, pipconf2;
	deoglShaderDefines defines, commonDefines;
	deoglShaderSources *sources;
	
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
		pipconf.SetShader( renderThread, sources, defines );
		pipconf2.SetShader( renderThread, sources, defines );
		pPipelineHiddenAreaClearMask = pipelineManager.GetWith( pipconf );
		pPipelineHiddenAreaDepth = pipelineManager.GetWith( pipconf2 );
		
		// hidden area stereo left
		sources = shaderManager.GetSourcesNamed( "VR Hidden Area Stereo" );
		defines.SetDefine( "RENDER_TO_LAYER", 0 );
		pipconf.SetShader( renderThread, sources, defines );
		pipconf2.SetShader( renderThread, sources, defines );
		pPipelineHiddenAreaClearMaskStereoLeft = pipelineManager.GetWith( pipconf );
		pPipelineHiddenAreaDepthStereoLeft = pipelineManager.GetWith( pipconf2 );
		
		// hidden area stereo right
		defines.SetDefine( "RENDER_TO_LAYER", 1 );
		pipconf.SetShader( renderThread, sources, defines );
		pipconf2.SetShader( renderThread, sources, defines );
		pPipelineHiddenAreaClearMaskStereoRight = pipelineManager.GetWith( pipconf );
		pPipelineHiddenAreaDepthStereoRight = pipelineManager.GetWith( pipconf2 );
		
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
