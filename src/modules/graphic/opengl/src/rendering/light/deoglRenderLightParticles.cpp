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
#include "deoglRenderLightParticles.h"
#include "../deoglRenderWorld.h"
#include "../defren/deoglDeferredRendering.h"
#include "../plan/deoglRenderPlan.h"
#include "../plan/deoglRenderPlanDebug.h"
#include "../../collidelist/deoglCollideList.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../debug/deoglDebugTraceGroup.h"
#include "../../light/shader/deoglLightShader.h"
#include "../../light/shader/deoglLightShaderManager.h"
#include "../../particle/deoglRParticleEmitter.h"
#include "../../particle/deoglRParticleEmitterInstance.h"
#include "../../particle/deoglRParticleEmitterInstanceType.h"
#include "../../particle/deoglRParticleEmitterType.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../renderthread/deoglRTTexture.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../shaders/deoglShaderCompiled.h"
#include "../../shaders/deoglShaderDefines.h"
#include "../../shaders/deoglShaderManager.h"
#include "../../shaders/deoglShaderProgram.h"
#include "../../shaders/deoglShaderSources.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../skin/channel/deoglSkinChannel.h"
#include "../../skin/deoglRSkin.h"
#include "../../skin/deoglSkinTexture.h"
#include "../../skin/state/deoglSkinState.h"
#include "../../texture/deoglTextureStageManager.h"
#include "../../vao/deoglVAO.h"

#include <dragengine/common/exceptions.h>



// Definitions
////////////////
#if 0
enum eSPLight{
	splMatrixMV,
	splMatrixProj,
	splPosTransform,
	splLightColor,
	splPosition,
	splSize
};
#endif



// Class deoglRenderLightParticles
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderLightParticles::deoglRenderLightParticles( deoglRenderThread &renderThread ) :
deoglRenderLightBase( renderThread ){
}

deoglRenderLightParticles::~deoglRenderLightParticles(){
	pCleanUp();
}



// Rendering
//////////////

void deoglRenderLightParticles::RenderLights( deoglRenderPlan &plan ){
	const deoglParticleEmitterInstanceList &instanceList = plan.GetCollideList().GetParticleEmitterList();
	const int instanceCount = instanceList.GetCount();
	
	if( instanceCount == 0 ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	int i;
	
	RestoreFBO( plan );
	
	// set states
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	SetCullMode( ! plan.GetFlipCulling() );
	OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
	OGL_CHECK( renderThread, glBlendFunc( GL_ONE, GL_ONE ) );
	
	OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
	
	// render
	for( i=0; i<instanceCount; i++ ){
		deoglRParticleEmitterInstance &instance = *instanceList.GetAt( i );
		
		if( instance.GetParticleCount() > 0 ){
			RenderParticleEmitterInstance( plan, instance );
		}
	}
	
	// clean up
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
}

void deoglRenderLightParticles::RenderParticleEmitterInstance( deoglRenderPlan &plan,
deoglRParticleEmitterInstance &emitterInstance ){
	const deoglRParticleEmitter &emitter = *emitterInstance.GetEmitter();
	const int typeCount = emitterInstance.GetTypeCount();
	int i;
	
	for( i=0; i<typeCount; i++ ){
		deoglRParticleEmitterInstanceType &itype = emitterInstance.GetTypeAt( i );
		deoglRParticleEmitterType &etype = emitter.GetTypeAt( i );
		
		if( etype.GetEmitLight() ){
			RenderParticleEmitterInstanceType( plan, emitterInstance, itype, etype );
		}
	}
}

void deoglRenderLightParticles::RenderParticleEmitterInstanceType( deoglRenderPlan &plan,
deoglRParticleEmitterInstance &instance, deoglRParticleEmitterInstanceType &itype, deoglRParticleEmitterType &etype ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "LightParticle.RenderParticleEmitterInstanceType" );
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglRParticleEmitter &emitter = *instance.GetEmitter();
	
	//deoglRenderPlanDebug * const planDebug = plan.GetDebug();
	
	// debug
	/*
	if( plan.GetRenderPassNumber() == 1 && planDebug ){
		planDebug->IncrementRenderedLights();
	}
	*/
	
	// set shader
	deoglLightShader * const lightShader = etype.GetShaderFor( plan.GetRenderStereo()
		? deoglRParticleEmitterType::estStereoNoShadow
		: deoglRParticleEmitterType::estNoShadow );
	if( ! lightShader ){
		DETHROW( deeInvalidParam );
	}
	
	renderThread.GetShader().ActivateShader( lightShader->GetShader() );
	
	// set program parameters
	deoglSPBlockUBO * const spbInstance = itype.GetLightInstanceParameterBlock();
	deoglSPBlockUBO * const spbLight = etype.GetLightParameterBlock();
	if( ! spbLight || ! spbInstance ){
		DETHROW( deeInvalidParam );
	}
	
	UpdateLightParamBlock( *lightShader, *spbLight, plan, emitter, etype );
	UpdateInstanceParamBlock( *lightShader, *spbInstance, plan, instance, itype );
	
	renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
	spbLight->Activate();
	spbInstance->Activate();
	
	// set textures
	int target = lightShader->GetTextureTarget( deoglLightShader::ettSamples );
	if( target != -1 ){
		tsmgr.EnableTexture( target, *etype.GetTextureSamples(),
			*renderThread.GetShader().GetTexSamplerConfig( deoglRTShader::etscClampLinear ) );
	}
	
	// render particles
	pglBindVertexArray( instance.GetVAO()->GetVAO() );
	
	/*
	if( plan.GetRenderStereo() && renderThread.GetChoices().GetRenderStereoVSLayer() ){
		// for some strange reason this code randomly fails to render properly.
		// renderdoc can not capture this behavior nor does any code review show
		// anything potentially wrong or dangerous. since a geometry shader is
		// anway used for particles this code can be ignored
		const GLint first[ 2 ] = { itype.GetFirstParticle(), itype.GetFirstParticle() };
		const GLsizei count[ 2 ] = { itype.GetParticleCount(), itype.GetParticleCount() };
		OGL_CHECK( renderThread, pglMultiDrawArrays( GL_POINTS, first, count, 2 ) );
		
	}else{
		*/
		OGL_CHECK( renderThread, glDrawArrays( GL_POINTS, itype.GetFirstParticle(), itype.GetParticleCount() ) );
// 	}
}



void deoglRenderLightParticles::UpdateLightParamBlock( deoglLightShader &lightShader, deoglSPBlockUBO &paramBlock,
deoglRenderPlan &plan, deoglRParticleEmitter &emitter, deoglRParticleEmitterType &type ){
	const float halfIntDistance = 0.15f; // a bit stronger than natural since particle lights are small in general
	const float range = 1.0f; // we use 1m as range since range is scaled per particle in the shader
	const float attenuationCoefficient = ( 1.0f / ( halfIntDistance * halfIntDistance ) - 2.0f ) / ( range * range );
	const float rangeNormInt = 1.0f / ( 1.0f + attenuationCoefficient * range * range );
	const float dampCoefficient = 1.0f / ( 1.0f - rangeNormInt );
	const float dampThreshold = -rangeNormInt * dampCoefficient;
	int target;
	
	paramBlock.MapBuffer();
	try{
		target = lightShader.GetLightUniformTarget( deoglLightShader::elutLightAttenuationCoefficient );
		if( target != -1 ){
			paramBlock.SetParameterDataFloat( target, attenuationCoefficient );
		}
		
		target = lightShader.GetLightUniformTarget( deoglLightShader::elutLightDampingCoefficient );
		if( target != -1 ){
			paramBlock.SetParameterDataFloat( target, dampCoefficient );
		}
		
		target = lightShader.GetLightUniformTarget( deoglLightShader::elutLightDampingThreshold );
		if( target != -1 ){
			paramBlock.SetParameterDataFloat( target, dampThreshold );
		}
		
	}catch( const deException & ){
		paramBlock.UnmapBuffer();
		throw;
	}
	paramBlock.UnmapBuffer();
}

void deoglRenderLightParticles::UpdateInstanceParamBlock( deoglLightShader &lightShader,
deoglSPBlockUBO &paramBlock, deoglRenderPlan &plan, deoglRParticleEmitterInstance &instance,
deoglRParticleEmitterInstanceType &type ){
	int target;
	
	// calculate matrices
	const decDMatrix matrixRefPos( decDMatrix::CreateTranslation( instance.GetReferencePosition() ) );
	const decDMatrix &matrixCamera = plan.GetCameraMatrix();
	
	const decDMatrix matrixMV( matrixRefPos * matrixCamera );
	
	const decDMatrix matrixCameraStereo( matrixCamera * plan.GetCameraStereoMatrix() );
	const decDMatrix matrixMVStereo( matrixRefPos * matrixCameraStereo );
	
	// set values
	paramBlock.MapBuffer();
	try{
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutMatrixMVP );
		if( target != -1 ){
			paramBlock.SetParameterDataArrayMat4x4( target, 0, plan.GetProjectionMatrix() );
			paramBlock.SetParameterDataArrayMat4x4( target, 1, plan.GetProjectionMatrixStereo() );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutMatrixMV );
		if( target != -1 ){
			paramBlock.SetParameterDataArrayMat4x3( target, 0, matrixMV );
			paramBlock.SetParameterDataArrayMat4x3( target, 1, matrixMVStereo );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutSamplesParams );
		if( target != -1 ){
			//const float width = 256.0f;
			//const float height = 4.0f;
			//paramBlock.SetParameterDataVec4( target, 255.0f / width, 0.5f / width, 1.0f / height, 0.5f / height );
			paramBlock.SetParameterDataVec4( target, 255.0f / 256.0f, 0.5f / 256.0f, 1.0f / 4.0f, 0.5f / 4.0f );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutBurstFactor );
		if( target != -1 ){
			paramBlock.SetParameterDataFloat( target, instance.GetBurstTime() );
		}
		
	}catch( const deException & ){
		paramBlock.UnmapBuffer();
		throw;
	}
	paramBlock.UnmapBuffer();
}



// Private Functions
//////////////////////

void deoglRenderLightParticles::pCleanUp(){
}
