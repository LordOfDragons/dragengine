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

#include "deoglRenderParticles.h"
#include "defren/deoglDeferredRendering.h"
#include "deoglRenderGeometry.h"
#include "plan/deoglRenderPlan.h"
#include "task/deoglRenderTaskParticles.h"
#include "task/deoglRenderTaskParticlesStep.h"
#include "../collidelist/deoglCollideList.h"
#include "../debug/deoglDebugTraceGroup.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTShader.h"
#include "../shaders/deoglShaderCompiled.h"
#include "../shaders/deoglShaderDefines.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/deoglShaderSources.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../texture/cubemap/deoglCubeMap.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../vao/deoglVAO.h"

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define FACTOR_EXPONENT		255.0f

enum eSPDepth{
	spdMatrixMV,
	spdMatrixProj,
	spdMaterialGamma,
	spdClipPlane,
	spdViewport,
	spdAttributes1,
	spdPosition
};

enum eSPRender{
	sprMatrixMV,
	sprMatrixProj,
	sprMaterialAmbient,
	sprMaterialDiffuse,
	sprMaterialSpecular,
	sprMaterialEmissive,
	sprMaterialGamma,
	sprViewport,
	sprDistortion,
	sprMatrixEnvMap,
	sprSampleParams,
	sprRibbonSheetCount
};




// Class deoglRenderParticles
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderParticles::deoglRenderParticles( deoglRenderThread &renderThread ) : deoglRenderBase( renderThread ){
}

deoglRenderParticles::~deoglRenderParticles(){
}



// Rendering
//////////////

void deoglRenderParticles::RenderTaskParticles( const deoglRenderTaskParticles &renderTask ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglSPBlockUBO * const renderParamBlock = renderTask.GetRenderParamBlock();
	const bool renderVSStereo = renderTask.GetRenderVSStereo();
	const int stepCount = renderTask.GetStepCount();
	const deoglVAO *curVAO = NULL;
	int i;
	
	if( stepCount == 0 ){
		return;
	}
	
	const deoglDebugTraceGroup debugTrace( renderThread, "Particles.RenderTaskParticles" );
	
	// conventional particles work with culling enabled. beams and ribbons though are tricky
	// to calculate so they easily end up backfacing. for these culling has to be disabled
	// to get them working. since conventional particles work with culling disabled too
	// culling is disabled for all particles to avoid any problems
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
// 	OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
	
	for( i=0; i<stepCount; i++ ){
		const deoglRenderTaskParticlesStep &renderTaskStep = *renderTask.GetStepAt( i );
		
		const int instanceCount = renderTaskStep.GetInstanceCount();
		if( instanceCount == 0 ){
			continue;
		}
		
		// activate shader
		renderThread.GetShader().ActivateShader( renderTaskStep.GetShader() );
		
		if( renderParamBlock ){
			renderParamBlock->Activate();
		}
		
		// set texture
		if( renderTaskStep.GetParameterBlockTexture() ){
			renderTaskStep.GetParameterBlockTexture()->Activate();
		}
		
		const deoglTexUnitsConfig * const tuc = renderTaskStep.GetTUC();
		if( tuc ){
			tuc->Apply();
			if( tuc->GetParameterBlock() ){
				tuc->GetParameterBlock()->Activate();
			}
		}
		
		// activate vao
		const deoglVAO * const vao = renderTaskStep.GetVAO();
		if( vao != curVAO ){
			pglBindVertexArray( vao->GetVAO() );
			curVAO = vao;
		}
		
		// set instance parameter block
		if( renderTaskStep.GetParameterBlockInstance() ){
			renderTaskStep.GetParameterBlockInstance()->Activate();
		}
		
		// render instances
		const GLenum indexGLType = vao->GetIndexGLType();
		const int indexSize = vao->GetIndexSize();
		
		//GetOgl().LogInfoFormat( "RenderTaskParticles: s=%i sha=%p tuc=%p vao=%i ic=%i",
		//	s, renderTaskStep.GetShader(), renderTaskStep.GetTUC(), vao->GetVAO(),
		//	renderTaskStep.GetIndexCount() );
		if( renderVSStereo ){
			const GLsizei count[ 2 ] = { renderTaskStep.GetIndexCount(), renderTaskStep.GetIndexCount() };
			const void * const indices[ 2 ] = {
				( void* )( intptr_t )( indexSize * renderTaskStep.GetFirstIndex() ),
				( void* )( intptr_t )( indexSize * renderTaskStep.GetFirstIndex() ) };
			OGL_CHECK( renderThread, pglMultiDrawElements( renderTaskStep.GetPrimitiveType(), count, indexGLType, indices, 2 ) );
			
		}else{
			OGL_CHECK( renderThread, glDrawElements( renderTaskStep.GetPrimitiveType(),
				renderTaskStep.GetIndexCount(), indexGLType,
				( GLvoid* )( intptr_t )( indexSize * renderTaskStep.GetFirstIndex() ) ) );
		}
	}
	
	pglBindVertexArray( 0 );
}
