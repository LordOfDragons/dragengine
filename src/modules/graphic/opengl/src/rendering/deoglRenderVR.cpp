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
	deoglShaderDefines defines, commonDefines;
	deoglShaderSources *sources;
	
	try{
		renderThread.GetShader().SetCommonDefines( commonDefines );
		
		defines = commonDefines;
		sources = shaderManager.GetSourcesNamed( "VR Hidden Area" );
		pShaderHiddenArea = shaderManager.GetProgramWith( sources, defines );
		
		
		sources = shaderManager.GetSourcesNamed( "VR Hidden Area Stereo" );
		defines.SetDefine( "RENDER_TO_LAYER", 0 );
		pShaderHiddenAreaStereoLeft = shaderManager.GetProgramWith( sources, defines );
		
		defines.SetDefine( "RENDER_TO_LAYER", 1 );
		pShaderHiddenAreaStereoRight = shaderManager.GetProgramWith( sources, defines );
		
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

void deoglRenderVR::RenderHiddenArea( deoglRenderPlan &plan ){
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
	
	deoglRenderThread &renderThread = GetRenderThread();
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	
	if( model ){
		deoglModelLOD &lod = model->GetLODAt( 0 );
		lod.PrepareVBOBlock();
		const deoglSharedVBOBlock * const vboBlock = lod.GetVBOBlock();
		if( vboBlock ){
			pRenderHiddenArea( *vboBlock, pShaderHiddenArea );
		}
		
	}else{
		if( modelLeft ){
			deoglModelLOD &lod = modelLeft->GetLODAt( 0 );
			lod.PrepareVBOBlock();
			const deoglSharedVBOBlock * const vboBlock = lod.GetVBOBlock();
			if( vboBlock ){
				pRenderHiddenArea( *vboBlock, pShaderHiddenAreaStereoLeft );
			}
		}
		
		if( modelRight ){
			deoglModelLOD &lod = modelRight->GetLODAt( 0 );
			lod.PrepareVBOBlock();
			const deoglSharedVBOBlock * const vboBlock = lod.GetVBOBlock();
			if( vboBlock ){
				pRenderHiddenArea( *vboBlock, pShaderHiddenAreaStereoRight );
			}
		}
	}
	
	pglBindVertexArray( 0 );
	OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
}



// Private Functions
//////////////////////

void deoglRenderVR::pCleanUp(){
}

void deoglRenderVR::pRenderHiddenArea( const deoglSharedVBOBlock &vboBlock,deoglShaderProgram &shader ){
	deoglRenderThread &renderThread = GetRenderThread();
	renderThread.GetShader().ActivateShader( &shader );
	
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
