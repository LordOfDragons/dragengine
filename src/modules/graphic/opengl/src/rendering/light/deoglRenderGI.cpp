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
#include "deoglRenderGI.h"
#include "../deoglRenderWorld.h"
#include "../defren/deoglDeferredRendering.h"
#include "../plan/deoglRenderPlan.h"
#include "../task/deoglRenderTask.h"
#include "../task/deoglRenderTaskShader.h"
#include "../task/deoglRenderTaskTexture.h"
#include "../task/deoglAddToRenderTaskGIMaterial.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../component/deoglRComponent.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../debug/deoglDebugInformation.h"
#include "../../debug/deoglDebugSaveTexture.h"
#include "../../devmode/deoglDeveloperMode.h"
#include "../../framebuffer/deoglFramebuffer.h"
#include "../../framebuffer/deoglFramebufferManager.h"
#include "../../gi/deoglRayTraceField.h"
#include "../../gi/deoglGI.h"
#include "../../gi/deoglGIBVH.h"
#include "../../gi/deoglGIRays.h"
#include "../../gi/deoglGIState.h"
#include "../../gi/deoglGIMaterials.h"
#include "../../model/deoglModelLOD.h"
#include "../../model/deoglRModel.h"
#include "../../extensions/deoglExtensions.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../renderthread/deoglRTDebug.h"
#include "../../renderthread/deoglRTFramebuffer.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../renderthread/deoglRTTexture.h"
#include "../../renderthread/deoglRTBufferObject.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../shaders/deoglShaderCompiled.h"
#include "../../shaders/deoglShaderDefines.h"
#include "../../shaders/deoglShaderManager.h"
#include "../../shaders/deoglShaderProgram.h"
#include "../../shaders/deoglShaderSources.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../../shaders/paramblock/deoglSPBParameter.h"
#include "../../shapes/deoglShape.h"
#include "../../shapes/deoglShapeManager.h"
#include "../../texture/deoglTextureStageManager.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../../vao/deoglVAO.h"
#include "../../vbo/deoglSharedVBOBlock.h"
#include "../../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>


// Definitions
////////////////

enum eUBORenderLightParameters{
	euprlMatrix,
	euprlMatrixNormal,
	euprlProbeCount,
	euprlOcclusionMapSize,
	euprlProbeClamp,
	euprlDistanceMapSize,
	euprlProbeSpacing,
	euprlProbeSpacingInv,
	euprlNormalBias,
	euprlPositionClamp,
	euprlEnergyPreservation,
	euprlOcclusionMapScale,
	euprlDistanceMapScale,
	euprlGridCoordShift
};

enum eSPMaterial{
	espmQuadParams
};

enum eSPDebugProbe{
	spdpMatrixMVP,
	spdpGIGridCoordShift
};

enum eSPDebugProbeOffset{
	spdpoMatrixMVP,
	spdpoGIGridCoordShift
};



// Class deoglRenderGI
////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderGI::deoglRenderGI( deoglRenderThread &renderThread ) :
deoglRenderLightBase( renderThread ),

pShaderFieldTraceRays( NULL ),
pShaderTraceRays( NULL ),
pShaderUpdateProbeIrradiance( NULL ),
pShaderUpdateProbeDistance( NULL ),
pShaderMoveProbes( NULL ),
pShaderLight( NULL ),
pShaderDebugProbe( NULL ),
pShaderDebugProbeOffset( NULL ),

pRenderTask( NULL ),
pAddToRenderTask( NULL )
{
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	const deoglGI &gi = renderThread.GetGI();
	const deoglGIRays &rays = gi.GetRays();
	deoglShaderSources *sources;
	deoglShaderDefines defines;
	
	try{
		pCreateUBORenderLight();
		
		sources = shaderManager.GetSourcesNamed( "DefRen GI Field Trace Rays" );
		pShaderFieldTraceRays = shaderManager.GetProgramWith( sources, defines );
		
		// trace rays
		defines.AddDefine( "GI_PROBE_INDEX_COUNT", rays.GetProbeCount() / 4 );
		defines.AddDefine( "GI_PROBE_COUNT", rays.GetProbeCount() );
		defines.AddDefine( "GI_RAYS_PER_PROBE", rays.GetRaysPerProbe() );
		
		#ifdef GI_RENDERDOC_DEBUG
		defines.AddDefine( "GI_RENDERDOC_DEBUG", true );
		#endif
		
		sources = shaderManager.GetSourcesNamed( "DefRen GI Trace Rays" );
		pShaderTraceRays = shaderManager.GetProgramWith( sources, defines );
		
		// update probes
		sources = shaderManager.GetSourcesNamed( "DefRen GI Update Probes" );
		defines.AddDefine( "MAP_IRRADIANCE", "1" );
		pShaderUpdateProbeIrradiance = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveDefine( "MAP_IRRADIANCE" );
		
		defines.AddDefine( "MAP_DISTANCE", "1" );
		pShaderUpdateProbeDistance = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveDefine( "MAP_DISTANCE" );
		
		// move probes
		sources = shaderManager.GetSourcesNamed( "DefRen GI Move Probes" );
		pShaderMoveProbes = shaderManager.GetProgramWith( sources, defines );
		
		// debug
		sources = shaderManager.GetSourcesNamed( "DefRen GI Debug Probe" );
		pShaderDebugProbe = shaderManager.GetProgramWith( sources, defines );
		
		sources = shaderManager.GetSourcesNamed( "DefRen GI Debug Probe Offset" );
		pShaderDebugProbeOffset = shaderManager.GetProgramWith( sources, defines );
		
		// render light
		defines.RemoveAllDefines();
		sources = shaderManager.GetSourcesNamed( "DefRen Light GI" );
		pShaderLight = shaderManager.GetProgramWith( sources, defines );
		
		
		// render task for GI material
		pRenderTask = new deoglRenderTask;
		pAddToRenderTask = new deoglAddToRenderTaskGIMaterial( renderThread, *pRenderTask );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRenderGI::~deoglRenderGI(){
	pCleanUp();
}



// Rendering
//////////////

deoglGIState *deoglRenderGI::GetUpdateGIState( const deoglRenderPlan &plan ) const{
	if( plan.GetUseGIState() && ! plan.GetUseConstGIState() ){
		return plan.GetGIState();
	}
	return NULL;
}

deoglGIState * deoglRenderGI::GetRenderGIState( const deoglRenderPlan &plan ) const{
	if( plan.GetUseGIState() ){
		if( plan.GetUseConstGIState() ){
			return plan.GetUseConstGIState();
			
		}else{
			return plan.GetGIState();
		}
	}
	return NULL;
}

void deoglRenderGI::TraceRays( deoglRayTraceField &field ){
	// NOTE old FBO is restored because field FBO can be dropped after this call
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglFramebuffer * const oldfbo = renderThread.GetFramebuffer().GetActive();
	const deoglGIBVH &bvh = renderThread.GetGI().GetBVH();
	
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
	OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
	
	renderThread.GetFramebuffer().Activate( field.GetFBORays() );
	
	const deoglTexture &texRays = field.GetTextureRays();
	OGL_CHECK( renderThread, glViewport( 0, 0, texRays.GetWidth(), texRays.GetHeight() ) );
	OGL_CHECK( renderThread, glScissor( 0, 0, texRays.GetWidth(), texRays.GetHeight() ) );
	
	tsmgr.EnableTBO( 0, bvh.GetTBONodeBox().GetTBO(), GetSamplerClampNearest() );
	tsmgr.EnableTBO( 1, bvh.GetTBOIndex().GetTBO(), GetSamplerClampNearest() );
	tsmgr.EnableTBO( 2, bvh.GetTBOFace().GetTBO(), GetSamplerClampNearest() );
	tsmgr.EnableTBO( 3, bvh.GetTBOVertex().GetTBO(), GetSamplerClampNearest() );
	tsmgr.DisableStagesAbove( 3 );
	
	renderThread.GetShader().ActivateShader( pShaderFieldTraceRays );
	field.GetUBO().Activate();
	
	OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
	
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
	tsmgr.DisableAllStages();
	renderThread.GetFramebuffer().Activate( oldfbo );
}

void deoglRenderGI::TraceRays( deoglRenderPlan &plan ){
	deoglGIState * const giState = GetUpdateGIState( plan );
	if( ! giState ){
		DETHROW( deeInvalidParam );
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDebugInformation &debugInfo = *renderThread.GetRenderers().GetWorld().GetDebugInfo().infoGI;
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglGI &gi = renderThread.GetGI();
	deoglGIMaterials &materials = gi.GetMaterials();
	deoglGIRays &rays = gi.GetRays();
	deoglGIBVH &bvh = gi.GetBVH();
	
	if( debugInfo.GetVisible() ){
		GetDebugTimerAt( 0 ).Reset();
	}
	
	
	// prepare
	pRenderTask->Clear();
	pAddToRenderTask->Reset();
	
	bvh.Clear();
	bvh.FindComponents( *plan.GetWorld(), giState->GetPosition(), giState->GetDetectionBox() );
	bvh.AddStaticComponents( giState->GetPosition() );
	bvh.BuildBVH();
	//bvh.DebugPrint( giState.GetPosition() );
	
	materials.SetMaterialCount( pAddToRenderTask->GetMaterialMapCount() );
	
	giState->PrepareUBOState(); // has to be done here since it is shared
	
	
	// render materials
	RenderMaterials( plan );
	
	
	// set common states
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDepthFunc( GL_ALWAYS ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
	OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
	
	
	// trace dynamic rays
	// 
	// BVH node count can be 0. in this case pBVHInstanceRootNode is set to -1 instead
	// of the index of the root node. the scripts can deal with this. it is required
	// for the scripts to run even if the BVH is empty since the ray result pixels
	// have to be correctly initialized with the ray direction otherwise the probe
	// update shader fails to work correctly
	const decPoint &sampleImageSize = giState->GetSampleImageSize();
	
	renderThread.GetFramebuffer().Activate( &rays.GetFBORayResult() );
	
	OGL_CHECK( renderThread, glDepthMask( GL_TRUE ) );
	OGL_CHECK( renderThread, glViewport( 0, 0, sampleImageSize.x, sampleImageSize.y ) );
	OGL_CHECK( renderThread, glScissor( 0, 0, sampleImageSize.x, sampleImageSize.y ) );
	
	renderThread.GetShader().ActivateShader( pShaderTraceRays );
	gi.GetUBO().Activate();
	
	const GLfloat clearDepth = 1.0f;
	const GLfloat clearPosition[ 4 ] = { 0.0f, 0.0f, 0.0f, 10000.0f };
	const GLfloat clearNormal[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	const GLfloat clearDiffuse[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	const GLfloat clearLight[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	
	OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearPosition[ 0 ] ) );
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 1, &clearNormal[ 1 ] ) );
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 2, &clearDiffuse[ 2 ] ) );
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 3, &clearLight[ 3 ] ) );
	
	tsmgr.EnableTBO( 0, bvh.GetTBONodeBox().GetTBO(), GetSamplerClampNearest() );
	tsmgr.EnableTBO( 1, bvh.GetTBOIndex().GetTBO(), GetSamplerClampNearest() );
	tsmgr.EnableTBO( 2, bvh.GetTBOInstance().GetTBO(), GetSamplerClampNearest() );
	tsmgr.EnableTBO( 3, bvh.GetTBOMatrix().GetTBO(), GetSamplerClampNearest() );
	tsmgr.EnableTBO( 4, bvh.GetTBOFace().GetTBO(), GetSamplerClampNearest() );
	tsmgr.EnableTBO( 5, bvh.GetTBOVertex().GetTBO(), GetSamplerClampNearest() );
	tsmgr.EnableTBO( 6, bvh.GetTBOTexCoord().GetTBO(), GetSamplerClampNearest() );
	tsmgr.EnableTBO( 7, bvh.GetTBOMaterial().GetTBO(), GetSamplerClampNearest() );
	tsmgr.EnableTBO( 8, bvh.GetTBOMaterial2().GetTBO(), GetSamplerClampNearest() );
	
	tsmgr.EnableTexture( 9, materials.GetTextureDiffuse(), GetSamplerClampNearest() );
	tsmgr.EnableTexture( 10, materials.GetTextureReflectivity(), GetSamplerClampNearest() );
	tsmgr.EnableTexture( 11, materials.GetTextureEmissivity(), GetSamplerClampNearest() );
	
	tsmgr.DisableStagesAbove( 11 );
	
	#ifdef GI_RENDERDOC_DEBUG
		OGL_CHECK( renderThread, glViewport( 0, 0, 512, 256 ) );
		OGL_CHECK( renderThread, glScissor( 0, 0, 512, 256 ) );
	#endif
	OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
	
	
	// clean up
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
	tsmgr.DisableAllStages();
	
	if( debugInfo.GetVisible() ){
		if( renderThread.GetDebug().GetDeveloperMode().GetDebugInfoSync() ){
			glFinish();
		}
		debugInfo.IncrementElapsedTime( GetDebugTimerAt( 0 ).GetElapsedTime() );
	}
}

void deoglRenderGI::PrepareUBORenderLight( deoglRenderPlan &plan ){
	const deoglGIState * const giState = GetRenderGIState( plan );
	if( ! giState ){
		DETHROW( deeInvalidParam );
	}
	
	deoglSPBlockUBO &ubo = ( deoglSPBlockUBO& )( deObject& )pUBORenderLight;
	
	ubo.MapBuffer();
	try{
		const decDMatrix matrix( plan.GetInverseCameraMatrix()
			* decDMatrix::CreateTranslation( -( giState->GetPosition() + giState->GetProbeOrigin() ) ) );
		
		ubo.SetParameterDataMat4x3( euprlMatrix, matrix );
		ubo.SetParameterDataMat3x3( euprlMatrixNormal, matrix.GetRotationMatrix().QuickInvert() );
		
		ubo.SetParameterDataIVec3( euprlProbeCount, giState->GetProbeCount() );
		ubo.SetParameterDataIVec3( euprlProbeClamp, giState->GetGridCoordClamp() );
		ubo.SetParameterDataVec3( euprlProbeSpacing, giState->GetProbeSpacing() );
		ubo.SetParameterDataVec3( euprlProbeSpacingInv, giState->GetProbeSpacingInverse() );
		ubo.SetParameterDataVec3( euprlPositionClamp, giState->GetPositionClamp() );
		ubo.SetParameterDataIVec3( euprlGridCoordShift,
			giState->GetProbeCount() - giState->GetGridCoordShift() );
		
		ubo.SetParameterDataInt( euprlOcclusionMapSize, giState->GetIrradianceMapSize() );
		ubo.SetParameterDataVec2( euprlOcclusionMapScale, giState->GetIrradianceMapScale() );
		ubo.SetParameterDataInt( euprlDistanceMapSize, giState->GetDistanceMapSize() );
		ubo.SetParameterDataVec2( euprlDistanceMapScale, giState->GetDistanceMapScale() );
		ubo.SetParameterDataFloat( euprlNormalBias, giState->GetNormalBias() );
		ubo.SetParameterDataFloat( euprlEnergyPreservation, giState->GetEnergyPreservation() );
		
	}catch( const deException & ){
		ubo.UnmapBuffer();
		throw;
	}
	ubo.UnmapBuffer();
}

void deoglRenderGI::RenderMaterials( deoglRenderPlan &plan ){
	deoglGIState * const giState = GetUpdateGIState( plan );
	if( ! giState ){
		DETHROW( deeInvalidParam );
	}
	
	const int rtShaderCount = pRenderTask->GetShaderCount();
	if( rtShaderCount == 0 ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDebugInformation &debugInfo = *renderThread.GetRenderers().GetWorld().GetDebugInfo().infoGI;
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglGI &gi = renderThread.GetGI();
	deoglGIMaterials &materials = gi.GetMaterials();
	const int width = materials.GetTextureDiffuse().GetWidth();
	const int height = materials.GetTextureDiffuse().GetHeight();
	
	if( debugInfo.GetVisible() ){
		GetDebugTimerAt( 0 ).Reset();
	}
	
	renderThread.GetFramebuffer().Activate( &materials.GetFBOMaterial() );
	
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	OGL_CHECK( renderThread, glViewport( 0, 0, width, height ) );
	
	tsmgr.EnableTexture( 0, materials.GetTextureDiffuse(), GetSamplerRepeatLinear() );
	tsmgr.EnableTexture( 1, materials.GetTextureReflectivity(), GetSamplerRepeatLinear() );
	tsmgr.EnableTexture( 2, materials.GetTextureEmissivity(), GetSamplerRepeatLinear() );
	tsmgr.DisableStagesAbove( 2 );
	
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDepthFunc( GL_ALWAYS ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
	
	const int mapsPerRow = materials.GetMaterialsPerRow();
	const int rowsPerImage = materials.GetRowsPerImage();
	const float scaleU = 1.0f / ( float )mapsPerRow;
	const float scaleV = 1.0f / ( float )rowsPerImage;
	const float offsetScaleU = 2.0f / ( float )mapsPerRow;
	const float offsetScaleV = 2.0f / ( float )rowsPerImage;
	const float offsetBaseU = offsetScaleU * 0.5f - 1.0f;
	const float offsetBaseV = offsetScaleV * 0.5f - 1.0f;
	int i;
	
	for( i=0; i<rtShaderCount; i++ ){
		const deoglRenderTaskShader &rtShader = *pRenderTask->GetShaderAt( i );
		deoglShaderCompiled &shader = *rtShader.GetShader()->GetCompiled();
		
		renderThread.GetShader().ActivateShader( rtShader.GetShader() );
		
		deoglRenderTaskTexture *rtTexture = rtShader.GetRootTexture();
		
		while( rtTexture ){
			rtTexture->GetTUC()->Apply();
			
			const int matMapIndex = rtTexture->GetMaterialIndex();
			const decPoint matMapCoord( matMapIndex % mapsPerRow, matMapIndex / mapsPerRow );
			
			const float offsetU = offsetScaleU * ( float )matMapCoord.x + offsetBaseU;
			const float offsetV = offsetScaleV * ( float )matMapCoord.y + offsetBaseV;
			
			shader.SetParameterFloat( espmQuadParams, scaleU, scaleV, offsetU, offsetV );
			OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
			
			rtTexture = rtTexture->GetNextTexture();
		}
		
	}
	
	// clean up
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
	tsmgr.DisableAllStages();
	
	if( debugInfo.GetVisible() ){
		if( renderThread.GetDebug().GetDeveloperMode().GetDebugInfoSync() ){
			glFinish();
		}
		debugInfo.IncrementElapsedTime( GetDebugTimerAt( 0 ).GetElapsedTime() );
	}
}

void deoglRenderGI::UpdateProbes( deoglRenderPlan &plan ){
	deoglGIState * const giState = GetUpdateGIState( plan );
	if( ! giState ){
		DETHROW( deeInvalidParam );
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDebugInformation &debugInfo = *renderThread.GetRenderers().GetWorld().GetDebugInfo().infoGI;
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglGI &gi = renderThread.GetGI();
	deoglGIRays &rays = gi.GetRays();
	
	if( debugInfo.GetVisible() ){
		GetDebugTimerAt( 0 ).Reset();
	}
	
	
	// common
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDepthFunc( GL_ALWAYS ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
	OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
	OGL_CHECK( renderThread, glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
	
	OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
	
	tsmgr.EnableTexture( 0, rays.GetTexturePosition(), GetSamplerClampNearest() );
	tsmgr.EnableTexture( 1, rays.GetTextureNormal(), GetSamplerClampNearest() );
	tsmgr.EnableTexture( 2, rays.GetTextureLight(), GetSamplerClampNearest() );
	tsmgr.DisableStagesAbove( 2 );
	
	
	// update probes: irradiance map
	renderThread.GetFramebuffer().Activate( &giState->GetFBOProbeIrradiance() );
	
	OGL_CHECK( renderThread, glViewport( 0, 0, giState->GetTextureProbeIrradiance().GetWidth(),
		giState->GetTextureProbeIrradiance().GetHeight() ) );
	
	renderThread.GetShader().ActivateShader( pShaderUpdateProbeIrradiance );
	gi.GetUBO().Activate();
	
	OGL_CHECK( renderThread, pglDrawArraysInstanced( GL_TRIANGLE_FAN, 0, 4, giState->GetUpdateProbeCount() ) );
	
	
	// update probes: distance map
	renderThread.GetFramebuffer().Activate( &giState->GetFBOProbeDistance() );
	
	OGL_CHECK( renderThread, glViewport( 0, 0, giState->GetTextureProbeDistance().GetWidth(),
		giState->GetTextureProbeDistance().GetHeight() ) );
	
	renderThread.GetShader().ActivateShader( pShaderUpdateProbeDistance );
	gi.GetUBO().Activate();
	
	OGL_CHECK( renderThread, pglDrawArraysInstanced( GL_TRIANGLE_FAN, 0, 4, giState->GetUpdateProbeCount() ) );
	
	
	// clean up
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
	tsmgr.DisableAllStages();
	
	if( debugInfo.GetVisible() ){
		if( renderThread.GetDebug().GetDeveloperMode().GetDebugInfoSync() ){
			glFinish();
		}
		debugInfo.IncrementElapsedTime( GetDebugTimerAt( 0 ).GetElapsedTime() );
	}
}

void deoglRenderGI::MoveProbes( deoglRenderPlan &plan ){
	deoglGIState * const giState = GetUpdateGIState( plan );
	if( ! giState ){
		DETHROW( deeInvalidParam );
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDebugInformation &debugInfo = *renderThread.GetRenderers().GetWorld().GetDebugInfo().infoGI;
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglGI &gi = renderThread.GetGI();
	deoglGIRays &rays = gi.GetRays();
	
	if( debugInfo.GetVisible() ){
		GetDebugTimerAt( 0 ).Reset();
	}
	
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDepthFunc( GL_ALWAYS ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	//OGL_CHECK( renderThread, glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
	
	OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
	
	tsmgr.EnableTexture( 0, rays.GetTexturePosition(), GetSamplerClampNearest() );
	tsmgr.EnableTexture( 1, rays.GetTextureNormal(), GetSamplerClampNearest() );
	tsmgr.DisableStagesAbove( 2 );
	
	renderThread.GetFramebuffer().Activate( &giState->GetFBOProbeOffset() );
	
	OGL_CHECK( renderThread, glViewport( 0, 0, giState->GetTextureProbeOffset().GetWidth(),
		giState->GetTextureProbeOffset().GetHeight() ) );
	
	renderThread.GetShader().ActivateShader( pShaderMoveProbes );
	gi.GetUBO().Activate();
	
	OGL_CHECK( renderThread, pglDrawArraysInstanced( GL_TRIANGLE_FAN, 0, 4, giState->GetUpdateProbeCount() ) );
	
	// clean up
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
	tsmgr.DisableAllStages();
	
	if( debugInfo.GetVisible() ){
		if( renderThread.GetDebug().GetDeveloperMode().GetDebugInfoSync() ){
			glFinish();
		}
		debugInfo.IncrementElapsedTime( GetDebugTimerAt( 0 ).GetElapsedTime() );
	}
}

void deoglRenderGI::RenderLight( deoglRenderPlan &plan, bool solid ){
	deoglGIState * const giState = GetRenderGIState( plan );
	if( ! giState ){
		DETHROW( deeInvalidParam );
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDebugInformation &debugInfo = *renderThread.GetRenderers().GetWorld().GetDebugInfo().infoGI;
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	
	if( debugInfo.GetVisible() ){
		GetDebugTimerAt( 0 ).Reset();
	}
	
	RestoreFBO( plan );
	
	OGL_CHECK( renderThread, glViewport( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
	OGL_CHECK( renderThread, glScissor( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
	
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	
	OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
	if( solid ){
		OGL_CHECK( renderThread, glBlendFunc( GL_ONE, GL_ONE ) );
		
	}else{
		OGL_CHECK( renderThread, glBlendFunc( GL_SRC_ALPHA, GL_ONE ) );
	}
	
	renderThread.GetShader().ActivateShader( pShaderLight );
	renderThread.GetRenderers().GetLight().GetLightPB()->Activate();
	GetUBORenderLight().Activate();
	
	tsmgr.EnableTexture( 6, giState->GetTextureProbeIrradiance(), GetSamplerClampLinear() );
	tsmgr.EnableTexture( 7, giState->GetTextureProbeDistance(), GetSamplerClampLinear() );
	tsmgr.DisableStagesAbove( 7 );
	
	defren.RenderFSQuadVAO();
	
	// clean up
	if( debugInfo.GetVisible() ){
		if( renderThread.GetDebug().GetDeveloperMode().GetDebugInfoSync() ){
			glFinish();
		}
		debugInfo.IncrementElapsedTime( GetDebugTimerAt( 0 ).GetElapsedTime() );
	}
}

void deoglRenderGI::RenderDebugOverlay( deoglRenderPlan &plan ){
	deoglGIState * const giState = GetRenderGIState( plan );
	if( ! giState ){
		DETHROW( deeInvalidParam );
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglGI &gi = renderThread.GetGI();
	if( ! gi.GetDebugShowProbes() ){
		return;
	}
	
	const decPoint3 &probeCount = giState->GetProbeCount();
	const decDMatrix matrixC( decDMatrix::CreateTranslation( giState->GetPosition()
		+ decDVector( giState->GetProbeOrigin() ) ) * plan.GetCameraMatrix() );
	const decDMatrix matrixCP( matrixC * decDMatrix( plan.GetProjectionMatrix() ) );
	
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglShape &shapeSphere = *renderThread.GetBufferObject().GetShapeManager()
		.GetShapeAt( deoglRTBufferObject::esSphere );
	
	defren.ActivatePostProcessFBO( true );
	
	// probe
	shapeSphere.ActivateVAO();
	
	deoglShaderCompiled &shader = *pShaderDebugProbe->GetCompiled();
	shader.Activate();
	shader.SetParameterDMatrix4x4( spdpMatrixMVP, matrixCP );
	shader.SetParameterPoint3( spdpGIGridCoordShift, giState->GetProbeCount() - giState->GetGridCoordShift() );
	
	gi.GetUBO().Activate();
	
	tsmgr.EnableTexture( 0, giState->GetTextureProbeIrradiance(), GetSamplerClampLinear() );
	tsmgr.EnableTexture( 1, giState->GetTextureProbeDistance(), GetSamplerClampLinear() );
	tsmgr.DisableStagesAbove( 1 );
	
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_TRUE ) );
	OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDepthFunc( defren.GetDepthCompareFuncRegular() ) );
	OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
	SetCullMode( plan.GetFlipCulling() );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	
	OGL_CHECK( renderThread, pglDrawArraysInstanced( GL_TRIANGLES,
		shapeSphere.GetVBOBlock()->GetOffset() + shapeSphere.GetPointOffsetFaces(),
		shapeSphere.GetPointCountFaces(), probeCount.x * probeCount.y * probeCount.z ) );
	
	// offset
	OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
	
	deoglShaderCompiled &shader2 = *pShaderDebugProbeOffset->GetCompiled();
	shader2.Activate();
	shader2.SetParameterDMatrix4x4( spdpoMatrixMVP, matrixCP );
	shader2.SetParameterPoint3( spdpoGIGridCoordShift, giState->GetProbeCount() - giState->GetGridCoordShift() );
	
	gi.GetUBO().Activate();
	
	tsmgr.EnableTexture( 0, giState->GetTextureProbeOffset(), GetSamplerClampNearest() );
	tsmgr.DisableStagesAbove( 0 );
	
	//OGL_CHECK( renderThread, glDepthMask( GL_TRUE ) );
	OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
	
	OGL_CHECK( renderThread, pglDrawArraysInstanced( GL_LINES, 0, 2, probeCount.x * probeCount.y * probeCount.z ) );
	
	
	// clean up
	pglBindVertexArray( 0 );
	tsmgr.DisableStage( 0 );
}



// Private Functions
//////////////////////

void deoglRenderGI::pCleanUp(){
	if( pAddToRenderTask ){
		delete pAddToRenderTask;
	}
	if( pRenderTask ){
		delete pRenderTask;
	}
	
	if( pShaderDebugProbeOffset ){
		pShaderDebugProbeOffset->RemoveUsage();
	}
	if( pShaderDebugProbe ){
		pShaderDebugProbe->RemoveUsage();
	}
	if( pShaderLight ){
		pShaderLight->RemoveUsage();
	}
	if( pShaderFieldTraceRays ){
		pShaderFieldTraceRays->RemoveUsage();
	}
	if( pShaderTraceRays ){
		pShaderTraceRays->RemoveUsage();
	}
	if( pShaderUpdateProbeIrradiance ){
		pShaderUpdateProbeIrradiance->RemoveUsage();
	}
	if( pShaderMoveProbes ){
		pShaderMoveProbes->RemoveUsage();
	}
	if( pShaderUpdateProbeDistance ){
		pShaderUpdateProbeDistance->RemoveUsage();
	}
}

void deoglRenderGI::pCreateUBORenderLight(){
	deoglRenderThread &renderThread = GetRenderThread();
	pUBORenderLight.TakeOver( new deoglSPBlockUBO( renderThread ) );
	deoglSPBlockUBO &ubo = GetUBORenderLight();
	
	ubo.SetRowMajor( ! renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Broken() );
	ubo.SetParameterCount( euprlGridCoordShift + 1 );
	
	ubo.GetParameterAt( euprlMatrix ).SetAll( deoglSPBParameter::evtFloat, 4, 3, 1 ); // mat4x3
	ubo.GetParameterAt( euprlMatrixNormal ).SetAll( deoglSPBParameter::evtFloat, 3, 3, 1 ); // mat3
	ubo.GetParameterAt( euprlProbeCount ).SetAll( deoglSPBParameter::evtInt, 3, 1, 1 ); // ivec3
	ubo.GetParameterAt( euprlOcclusionMapSize ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo.GetParameterAt( euprlProbeClamp ).SetAll( deoglSPBParameter::evtInt, 3, 1, 1 ); // ivec3
	ubo.GetParameterAt( euprlDistanceMapSize ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
	ubo.GetParameterAt( euprlProbeSpacing ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
	ubo.GetParameterAt( euprlProbeSpacingInv ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
	ubo.GetParameterAt( euprlPositionClamp ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
	ubo.GetParameterAt( euprlNormalBias ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
	ubo.GetParameterAt( euprlOcclusionMapScale ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
	ubo.GetParameterAt( euprlDistanceMapScale ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
	ubo.GetParameterAt( euprlEnergyPreservation ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
	ubo.GetParameterAt( euprlGridCoordShift ).SetAll( deoglSPBParameter::evtInt, 3, 1, 1 ); // ivec3
	
	ubo.MapToStd140();
	ubo.SetBindingPoint( 1 );
}
