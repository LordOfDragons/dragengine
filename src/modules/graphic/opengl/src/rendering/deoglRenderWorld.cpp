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

#include "deoglRenderDevMode.h"
#include "deoglRenderGeometry.h"
#include "deoglRenderGeometryPass.h"
#include "deoglRenderReflection.h"
#include "deoglRenderToneMap.h"
#include "deoglRenderTransparentPasses.h"
#include "deoglRenderTranspCounting.h"
#include "deoglRenderWorld.h"
#include "debug/deoglRenderDebugDrawer.h"
#include "defren/deoglDeferredRendering.h"
#include "light/deoglRenderLight.h"
#include "light/deoglRenderGI.h"
#include "plan/deoglRenderPlan.h"
#include "plan/deoglRenderPlanMasked.h"
#include "task/deoglAddToRenderTask.h"
#include "task/deoglAddToRenderTaskParticles.h"
#include "task/deoglRenderTask.h"
#include "task/deoglRenderTaskParticles.h"
#include "../debug/deoglDebugSnapshot.h"
#include "../devmode/deoglDeveloperMode.h"
#include "../effects/render/deoglREffect.h"
#include "../envmap/deoglEnvironmentMap.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../particle/deoglParticleSorter.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTDebug.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTTexture.h"
#include "../shaders/deoglShaderCompiled.h"
#include "../shaders/deoglShaderDefines.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/deoglShaderSources.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../skin/shader/deoglSkinShader.h"
#include "../sky/deoglRSky.h"
#include "../sky/deoglRSkyInstance.h"
#include "../sky/deoglRSkyLayer.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../texture/deoglTextureStageManager.h"
#include "../world/deoglRCamera.h"
#include "../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

enum eSPDepthOnly{
	spdoMatrixMVP,
	spdoMatrixMV,
	spdoMatrixDiffuse,
	spdoMaterialGamma,
	spdoClipPlane,
	spdoViewport,
	spdoPFMatrix
};

enum eSPTraCountMaxCount{
	sptcmcPosTransform,
	sptcmcTCTransform,
	sptcmcClampTC,
	sptcmcOffsets1,
	sptcmcOffsets2,
	sptcmcOffsets3,
	sptcmcOffsets4
};

enum eSPTraCountGetCount{
	sptcgcPosTransform,
	sptcgcThresholdTransform
};

enum eSPFinalize{
	spfinPosTransform,
	spfinTCTransform,
	spfinGamma,
	spfinBrightness,
	spfinContrast
};

enum eSPCopyDepth{
	spcdQuadParams
};

enum eSPDepthDownsample{
	spddsTCClamp,
	spddsMipMapLevel
};

//#define REAL_TRANSP_PARTICLES		1

#define DEBUG_SNAPSHOT_TRANSPARENCY_COUNTING 10
#define DEBUG_SNAPSHOT_TRANSPARENCY_PASSES   20



// #define DO_TIMING

#ifdef DO_TIMING
#include <dragengine/common/utils/decTimer.h>
static decTimer dtimer;

#define DEBUG_RESET_TIMER \
	dtimer.Reset();
#define DEBUG_PRINT_TIMER(what) \
	renderThread.GetLogger().LogInfoFormat( "World %s = %iys", what, \
		( int )( dtimer.GetElapsedTime() * 1000000.0 ) );\
	dtimer.Reset();
#else
#define DEBUG_RESET_TIMER
#define DEBUG_PRINT_TIMER(what)
#endif



// Class deoglRenderWorld
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderWorld::deoglRenderWorld( deoglRenderThread &renderThread ) :
deoglRenderBase( renderThread ),

pRenderPB( NULL ),
pRenderLuminancePB( NULL ),
pRenderCubePB( NULL ),
pRenderTask( NULL ),
pAddToRenderTask( NULL ),
pParticleSorter( NULL ),
pRenderTaskParticles( NULL ),
pAddToRenderTaskParticles( NULL ),

pDebugInfo( renderThread )
{
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	deoglShaderSources *sources;
	deoglShaderDefines defines;
	
	try{
		pRenderPB = deoglSkinShader::CreateSPBRender( renderThread, false );
		pRenderLuminancePB = deoglSkinShader::CreateSPBRender( renderThread, false );
		pRenderCubePB = deoglSkinShader::CreateSPBRender( renderThread, true );
		
		pRenderTask = new deoglRenderTask;
		pAddToRenderTask = new deoglAddToRenderTask( renderThread, *pRenderTask );
		
		pParticleSorter = new deoglParticleSorter;
		pRenderTaskParticles = new deoglRenderTaskParticles;
		pAddToRenderTaskParticles = new deoglAddToRenderTaskParticles( renderThread, pRenderTaskParticles );
		
		
		sources = shaderManager.GetSourcesNamed( "DefRen Finalize" );
		pShaderFinalize.TakeOver( shaderManager.GetProgramWith( sources, defines ) );
		
		
		DevModeDebugInfoChanged();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRenderWorld::~deoglRenderWorld(){
	pCleanUp();
}



// Rendering
//////////////

#define DO_QUICK_DEBUG 1

#ifdef DO_QUICK_DEBUG
#define QUICK_DEBUG_START( lower, upper )    if( renderThread.GetConfiguration().GetQuickDebug() > upper || renderThread.GetConfiguration().GetQuickDebug() < lower ){
#define QUICK_DEBUG_END                      }
#else
#define QUICK_DEBUG_START( lower, upper )
#define QUICK_DEBUG_END
#endif



//#define ENABLE_DEBUG_ENTER_EXIT 1
#ifdef OS_ANDROID
// 	#define ENABLE_DEBUG_ENTER_EXIT 1
#endif

#ifdef ENABLE_DEBUG_ENTER_EXIT
#define DBG_ENTER(x) GetRenderThread().GetLogger().LogInfo("RenderWorld." x ": ENTER");
#define DBG_ENTER_PARAM(x,pt,pv) GetRenderThread().GetLogger().LogInfoFormat("RenderWorld." x ": ENTER [" pt "]", pv);
#define DBG_ENTER_PARAM2(x,pt1,pv1,pt2,pv2) GetRenderThread().GetLogger().LogInfoFormat("RenderWorld." x ": ENTER [" pt1 "," pt2 "]", pv1, pv2);
#define DBG_ENTER_PARAM3(x,pt1,pv1,pt2,pv2,pt3,pv3) GetRenderThread().GetLogger().LogInfoFormat("RenderWorld." x ": ENTER [" pt1 "," pt2 "," pt3 "]", pv1, pv2, pv3);
#define DBG_EXIT(x) GetRenderThread().GetLogger().LogInfo("RenderWorld." x ": EXIT");
#else
#define DBG_ENTER(x)
#define DBG_ENTER_PARAM(x,pt,pv)
#define DBG_ENTER_PARAM2(x,pt1,pv1,pt2,pv2)
#define DBG_ENTER_PARAM3(x,pt1,pv1,pt2,pv2,pt3,pv3)
#define DBG_EXIT(x)
#endif


void deoglRenderWorld::RenderBlackScreen( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	
	defren.InitPostProcessTarget();
	defren.ActivatePostProcessFBO( true );
	
	const int viewportHeight = plan.GetViewportHeight();
	const int viewportWidth = plan.GetViewportWidth();
	const int viewportX = plan.GetViewportX();
	const int viewportY = plan.GetViewportY();
	
	OGL_CHECK( renderThread, glViewport( viewportX, viewportY, viewportWidth, viewportHeight ) );
	OGL_CHECK( renderThread, glScissor( viewportX, viewportY, viewportWidth, viewportHeight ) );
	
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	
	const GLfloat clearColor[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
	
	// swap so the caller finds the final image to work with in GetPostProcessTexture
	defren.SwapPostProcessTarget();
}

void deoglRenderWorld::RenderWorld( deoglRenderPlan &plan, deoglRenderPlanMasked *mask ){
DBG_ENTER_PARAM("RenderWorld", "%p", mask)
DEBUG_RESET_TIMER
//	bool maskedRendering = info->GetUseRenderMask();
//	deoglRenderableColorTexture *maskRenderTexture = info->GetMaskRenderTexture();
	
	const bool debugMainPass = ! mask;
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
//	deoglCollideList &colList = plan.GetCollideList();
	deoglDeveloperMode &devMode = renderThread.GetDebug().GetDeveloperMode();
	deoglRTRenderers &renderers = GetRenderThread().GetRenderers();
	deoglRWorld &world = *plan.GetWorld();
	deoglShaderCompiled *shader;
//	int c, componentCount;
	
	// debug if enabled in plan and this is the main pass
	if( debugMainPass && plan.GetDebugTiming() ){
		pDebugInfo.ClearAll();
		renderers.GetLight().ResetDebugInfo();
		DebugTimersReset( plan, true );
	}
	
	// prepare for rendering
	const bool disableLights = plan.GetDisableLights();
	
	if( disableLights ){
		renderers.GetGeometry().SetAmbient( decColor( 1.0, 1.0, 1.0 ) );
		
	}else{
		decColor ambient = world.GetAmbientLight();
		ambient.r = powf( ambient.r, OGL_RENDER_GAMMA );
		ambient.g = powf( ambient.g, OGL_RENDER_GAMMA );
		ambient.b = powf( ambient.b, OGL_RENDER_GAMMA );
		
		const decVector vector( world.GetLightColorMatrix() * decVector( ambient.r, ambient.g, ambient.b ) );
		renderers.GetGeometry().SetAmbient( decColor( vector.x, vector.y, vector.z ) );
	}
	
	// set up the render parameter shader parameter block
	PrepareRenderParamBlock( plan, mask );
	
	// this part is done only for the main render pass
	OGL_CHECK( renderThread, glViewport( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
	OGL_CHECK( renderThread, glScissor( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
	OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
	
	OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
	
	// set the stencil mask
	plan.SetRenderPassNumber( 1 );
	
	OGL_CHECK( renderThread, glEnable( GL_STENCIL_TEST ) );
	if( mask ){
		// if there is a mask set the stencil write mask to only affect the
		// bits of the render pass number and use the mask to filter
		plan.SetStencilRefValue( 0x03 ); // ( renderPassNumber << 1 ) | 0x01
		plan.SetStencilWriteMask( 0xfe );
		
	}else{
		// if there is no mask we set the mask bit to 0 and write to all stencil
		// bits. this way the mask bit is already cleared for the first masked
		// render level we encounter. no filtering for the mask is used
		plan.SetStencilRefValue( 0x02 ); // ( renderPassNumber << 1 ) | 0x00
		plan.SetStencilWriteMask( 0xff );
	}
	
	// render masked stuff. works before calling ClearScreen since it handles clearing the
	// depth buffer internally. after finishing rendering the mask passes we have to do
	// ClearScreen but protecting color otherwise the masks are overwritten
	if( mask ){
// 		ClearScreen( plan, false, true );
		
	}else{
		RenderMaskedPass( plan );
		DebugTimer2Sample( plan, *pDebugInfo.infoPassMasked, true );
		
// 		ClearScreen( plan, false, false );
	}
	
	if( debugMainPass ){
		DebugTimer2Sample( plan, *pDebugInfo.infoPrepare, true );
	}
	
	// prepare for one render turn with this camera
	plan.PrepareRenderOneTurn();
	
	// solid pass
	QUICK_DEBUG_START( 15, 19 )
	//if( ! plan->GetFBOTarget() )
	renderers.GetGeometryPass().RenderSolidGeometryPass( plan, mask );
	if( debugMainPass ){
		DebugTimer2Sample( plan, *pDebugInfo.infoSolidGeometry, true );
	}
	QUICK_DEBUG_END
	
	DBG_ENTER("RenderDepthMinMaxMipMap")
	renderers.GetReflection().RenderDepthMinMaxMipMap( plan );
	DBG_EXIT("RenderDepthMinMaxMipMap")
	DBG_ENTER("RenderReflections")
	renderers.GetReflection().RenderReflections( plan );
	DBG_EXIT("RenderReflections")
	if( debugMainPass ){
		DebugTimer2Sample( plan, *pDebugInfo.infoReflection, true );
	}
	
	QUICK_DEBUG_START( 13, 19 )
	// plan transparency. this affects lighting thus it comes earlier than RenderTransparentPasses
	if( plan.GetHasTransparency() ){
		#ifdef OS_ANDROID
		plan.PlanTransparency( plan.GetTransparencyLayerCount() );
		#else
		plan.PlanTransparency( renderers.GetTransparencyCounter().GetCount() );
		#endif
	}else{
		plan.PlanTransparency( 0 );
	}
	
	//if( ! plan->GetFBOTarget() )
	if( ! plan.GetDisableLights() ){
		if( ! mask && renderThread.GetConfiguration().GetDebugSnapshot() == 888 ){
			deoglDebugSnapshot snapshot( renderThread );
			snapshot.SetEnableDepth( true );
			snapshot.SetEnableColor( true );
			snapshot.SetEnableMaterialBuffers( true );
			snapshot.SetName( "light/" );
			snapshot.TakeSnapshot();
			renderThread.GetConfiguration().SetDebugSnapshot( 0 );
		}
		
		if( ! mask ){
			renderers.GetToneMap().LuminancePrepare( plan );
// 			renderers.GetGeometryPass().RenderLuminanceOnly( plan );
		}
		
		DBG_ENTER("RenderLights")
		renderers.GetLight().RenderLights( plan, true, mask );
		DBG_EXIT("RenderLights")
		DebugTimer2Sample( plan, *pDebugInfo.infoSolidGeometryLights, true );
#if 0
			if(plan.GetFBOTarget()){
				static int c = 0;
				const decDVector &p = plan.GetCameraPosition();
				decString s;
				s.Format("envmap_%g-%g-%g/c%d/lights-after/", p.x, p.y, p.z, c++);
				deoglDebugSnapshot snapshot( renderThread );
				snapshot.SetEnableColor( true );
				snapshot.SetEnableStates( true );
				snapshot.SetName( s );
				snapshot.TakeSnapshot();
			}
#endif
	}
	QUICK_DEBUG_END
	
	DBG_ENTER("RenderReflectionScreenSpace")
	renderers.GetReflection().RenderScreenSpace( plan );
	DBG_EXIT("RenderReflectionScreenSpace")
	if( debugMainPass ){
		DebugTimer2Sample( plan, *pDebugInfo.infoSSR, true );
	}
	
	// transparenc passes
	QUICK_DEBUG_START( 12, 19 )
	if( ! plan.GetFBOTarget() ){
		renderers.GetTransparentPasses().RenderTransparentPasses( plan, mask );
		if( debugMainPass ){
			DebugTimer2Sample( plan, *pDebugInfo.infoTransparent, true );
		}
	}
	QUICK_DEBUG_END
	
	// stop using stencil testing
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	
	// this happens only in the main pass
	if( ! mask ){
		// tone mapping
		if( disableLights || ! plan.GetUseToneMap() ){
			OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
			OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
			
			OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
			
			OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
			
			OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
			
			OGL_CHECK( renderThread, glViewport( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
			OGL_CHECK( renderThread, glScissor( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
			OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
			
			defren.ActivateFBOTemporary2( false );
			tsmgr.EnableTexture( 0, *defren.GetTextureColor(), GetSamplerClampNearest() );
			
			renderThread.GetShader().ActivateShader( pShaderFinalize );
			shader = pShaderFinalize->GetCompiled();
			
			shader->SetParameterFloat( spfinPosTransform, 1.0f, 1.0f, 0.0f, 0.0f );
			defren.SetShaderParamFSQuad( *shader, spfinTCTransform );
			shader->SetParameterFloat( spfinGamma, 1.0f, 1.0f, 1.0f, 1.0f );
			shader->SetParameterFloat( spfinBrightness, 0.0f, 0.0f, 0.0f, 0.0f );
			shader->SetParameterFloat( spfinContrast, 1.0f, 1.0f, 1.0f, 1.0f );
			
			defren.RenderFSQuadVAO();
			
		}else{
			//QUICK_DEBUG_START( 11, 19 )
			DBG_ENTER("RenderToneMap")
			renderers.GetToneMap().ToneMap( plan );
			DBG_EXIT("RenderToneMap")
			DebugTimer2Sample( plan, *pDebugInfo.infoToneMapping, true );
			//QUICK_DEBUG_END
		}
		
		// TODO due to the deferred rendering using transparency the depth buffer itself is
		// unusable. the depth is stored in the encoded ( or depth ) texture. the upcoming
		// shaders working with the depth values have to be modified to use the depth
		// texture instead of the depth unit. another solution would be to write a pass in
		// which only the depth texture is written to the depth populating the depth texture
		// ( and therefore the depth unit ) with the proper values. solution pending.
		
		// render shadeless objects
		/*
		QUICK_DEBUG_START( 10, 19 )
		pRenderShadelessPass( plan, mask );
		QUICK_DEBUG_END
		*/
		
		// post processing
		//if( ! disableLights && maskedRenderTexture ){
		//	plan->InitRenderTextures( defren.GetColorTexture(), defren.GetDiffuseTexture() );
		//	
		//}else{
			defren.InitPostProcessTarget();
		//}
		defren.ActivatePostProcessFBO( true );
		
		RenderAntiAliasingPass();
		
		if( plan.GetRenderDebugPass() ){
			RenderDebugDrawers( plan );
			renderers.GetLight().GetRenderGI().RenderDebugOverlay( plan );
			
			//pRenderDebugging( info );
			DebugTimer2Sample( plan, *pDebugInfo.infoDebugDrawers, true );
		}
		
		if( ! mask ){
			renderers.GetLight().GetRenderGI().MoveProbes( plan );
		}
		
		// effects
		RenderEffects( plan );
		DebugTimer2Sample( plan, *pDebugInfo.infoPostProcessing, true );
		
		// rendering time measuring finishes here since RenderDevMode uses it
		DebugTimer1Sample( plan, *pDebugInfo.infoWorld, true );
		
		// debug information
		if( plan.GetDebugTiming() && ! plan.GetFBOTarget() && devMode.GetEnabled() ){ // only in canvas rendering and main pass
			renderers.GetDevMode().RenderDevMode( plan );
		}
		
		// swap one last time so the caller finds the final image to work with in GetPostProcessTexture
		defren.SwapPostProcessTarget();
		DEBUG_PRINT_TIMER( "Total" )
	}
DBG_EXIT("RenderWorld")
}



void deoglRenderWorld::PrepareRenderParamBlock( deoglRenderPlan &plan, deoglRenderPlanMasked *mask ){
DBG_ENTER_PARAM("PrepareRenderParamBlock", "%p", mask)
	deoglRenderThread &renderThread = GetRenderThread();
	const decDMatrix matrixEnvMap( plan.GetRefPosCameraMatrix().GetRotationMatrix().Invert() );
	const decColor &ambient = renderThread.GetRenderers().GetGeometry().GetAmbient();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	const decDMatrix &matrixProjection = plan.GetProjectionMatrix();
	const decDMatrix &matrixCamera = plan.GetRefPosCameraMatrix();
	const deoglConfiguration &config = renderThread.GetConfiguration();
	decVector clipPlaneNormal( 0.0f, 0.0f, 1.0f );
	float clipPlaneDistance = 0.0f;
	float envMapLodLevel = 1.0f;
	
	// sharpness indicates the cone angle from 0 to 90 degrees. at 45 degrees a single cube map face is required
	// to be sampled. hence 0.5 sharpness has to pick the max lod level from the environment map. to determine
	// the lod level the log2 from the size times the double-sharpness can be used. to avoid a bad look at very
	// dull values the lod level is clamped to the max level minus one.
	deoglEnvironmentMap * const envmapSky = plan.GetWorld()->GetSkyEnvironmentMap();
	if( envmapSky ){
		//envMapLodLevel = ( float )envmapSky->GetMaxMipMapLevel() * 2.0f;
		//envMapLodLevel = ( float )( envmapSky->GetSize() - 1 ) * 2.0f;
		envMapLodLevel = ( float )envmapSky->GetSize() * 1.0f;
	}
	
	// clip plane if used
	if( mask && mask->GetUseClipPlane() ){
		clipPlaneNormal = mask->GetClipNormal();
		clipPlaneDistance = mask->GetClipDistance();
	}
	
	// particle light hack
	decColor particleLight = GetRenderThread().GetRenderers().GetGeometry().GetAmbient();
	
	const int skyCount = plan.GetSkyInstanceCount();
	int i;
	for( i=0; i<skyCount; i++ ){
		deoglRSky &sky = *plan.GetSkyInstanceAt( i )->GetRSky();
		const int layerCount = sky.GetLayerCount();
		int j;
		
		for( j=0; j<layerCount; j++ ){
			const deoglRSkyLayer &layer = sky.GetLayerAt( j );
			particleLight += layer.GetLightColor() * layer.GetLightIntensity();
		}
	}
	
	// fill the parameter block parameters with the found values. only used parameters are set
	pRenderPB->MapBuffer();
	try{
		pRenderPB->SetParameterDataVec4( deoglSkinShader::erutAmbient, ambient, 1.0f );
		pRenderPB->SetParameterDataMat4x3( deoglSkinShader::erutMatrixV, matrixCamera );
		pRenderPB->SetParameterDataMat4x4( deoglSkinShader::erutMatrixP, matrixProjection );
		pRenderPB->SetParameterDataMat4x4( deoglSkinShader::erutMatrixVP, matrixCamera * matrixProjection );
		pRenderPB->SetParameterDataMat3x3( deoglSkinShader::erutMatrixVn, matrixCamera.GetRotationMatrix().Invert() );
		pRenderPB->SetParameterDataMat3x3( deoglSkinShader::erutMatrixEnvMap, matrixEnvMap );
		
		pRenderPB->SetParameterDataFloat( deoglSkinShader::erutEnvMapLodLevel, envMapLodLevel );
		pRenderPB->SetParameterDataFloat( deoglSkinShader::erutNorRoughCorrStrength, config.GetNormalRoughnessCorrectionStrength() );
		
		pRenderPB->SetParameterDataBool( deoglSkinShader::erutSkinDoesReflections, ! config.GetSSREnable() );
		pRenderPB->SetParameterDataBool( deoglSkinShader::erutFlipCulling, plan.GetFlipCulling() );
		
		defren.SetShaderViewport( *pRenderPB, deoglSkinShader::erutViewport, true );
		pRenderPB->SetParameterDataVec4( deoglSkinShader::erutClipPlane, clipPlaneNormal, clipPlaneDistance );
		pRenderPB->SetParameterDataVec4( deoglSkinShader::erutScreenSpace,
			defren.GetScalingU(), defren.GetScalingV(), defren.GetPixelSizeU(), defren.GetPixelSizeV() );
		pRenderPB->SetParameterDataVec4( deoglSkinShader::erutDepthOffset, 0.0f, 0.0f, 0.0f, 0.0f );
		
		pRenderPB->SetParameterDataVec3( deoglSkinShader::erutParticleLightHack, particleLight );
		
		pRenderPB->SetParameterDataFloat( deoglSkinShader::erutBillboardZScale, tanf( plan.GetCameraFov() * 0.5f ) );
		
		if( plan.GetCamera() ){
			pRenderPB->SetParameterDataFloat( deoglSkinShader::erutCameraAdaptedIntensity,
				plan.GetCamera()->GetLastAverageLuminance() / config.GetHDRRSceneKey() );
			
		}else{
			pRenderPB->SetParameterDataFloat( deoglSkinShader::erutCameraAdaptedIntensity,
				plan.GetCameraAdaptedIntensity() );
		}
		
		const float znear = plan.GetCameraImageDistance();
		const float zfar = plan.GetCameraViewDistance();
		const float fadeRange = ( zfar - znear ) * 0.001f; // for example 1m on 1km
		pRenderPB->SetParameterDataVec3( deoglSkinShader::erutFadeRange, zfar - fadeRange, zfar, 1.0f / fadeRange );
		
	}catch( const deException & ){
		pRenderPB->UnmapBuffer();
		throw;
	}
	pRenderPB->UnmapBuffer();
	
	// luminance parameter block is the same except screen space differs
	pRenderLuminancePB->MapBuffer();
	try{
		pRenderLuminancePB->SetParameterDataVec4( deoglSkinShader::erutAmbient, ambient, 1.0f );
		pRenderLuminancePB->SetParameterDataMat4x3( deoglSkinShader::erutMatrixV, matrixCamera );
		pRenderLuminancePB->SetParameterDataMat4x4( deoglSkinShader::erutMatrixP, matrixProjection );
		pRenderLuminancePB->SetParameterDataMat4x4( deoglSkinShader::erutMatrixVP, matrixCamera * matrixProjection );
		pRenderLuminancePB->SetParameterDataMat3x3( deoglSkinShader::erutMatrixVn, matrixCamera.GetRotationMatrix().Invert() );
		pRenderLuminancePB->SetParameterDataMat3x3( deoglSkinShader::erutMatrixEnvMap, matrixEnvMap );
		
		pRenderLuminancePB->SetParameterDataFloat( deoglSkinShader::erutEnvMapLodLevel, envMapLodLevel );
		pRenderLuminancePB->SetParameterDataFloat( deoglSkinShader::erutNorRoughCorrStrength, 0.0f );
		
		pRenderLuminancePB->SetParameterDataBool( deoglSkinShader::erutSkinDoesReflections, true );
		pRenderLuminancePB->SetParameterDataBool( deoglSkinShader::erutFlipCulling, plan.GetFlipCulling() );
		
		const int width = defren.GetTextureLuminance()->GetWidth();
		const int height = defren.GetTextureLuminance()->GetHeight();
		pRenderLuminancePB->SetParameterDataVec4( deoglSkinShader::erutViewport,
			0.0f, 0.0f, 1.0f - 1.0f / ( float )width, 1.0f - 1.0f / ( float )height );
		
		defren.SetShaderViewport( *pRenderLuminancePB, deoglSkinShader::erutViewport, true );
		pRenderLuminancePB->SetParameterDataVec4( deoglSkinShader::erutClipPlane, clipPlaneNormal, clipPlaneDistance );
		pRenderLuminancePB->SetParameterDataVec4( deoglSkinShader::erutScreenSpace,
			1.0f, 1.0f, 1.0f / ( float )width, 1.0f / ( float )height );
		pRenderLuminancePB->SetParameterDataVec4( deoglSkinShader::erutDepthOffset, 0.0f, 0.0f, 0.0f, 0.0f );
		
		pRenderLuminancePB->SetParameterDataVec3( deoglSkinShader::erutParticleLightHack, decColor() );
		
		pRenderLuminancePB->SetParameterDataFloat( deoglSkinShader::erutBillboardZScale, tanf( plan.GetCameraFov() * 0.5f ) );
		
		const float znear = plan.GetCameraImageDistance();
		const float zfar = plan.GetCameraViewDistance();
		const float fadeRange = ( zfar - znear ) * 0.001f; // for example 1m on 1km
		pRenderLuminancePB->SetParameterDataVec3( deoglSkinShader::erutFadeRange, zfar - fadeRange, zfar, 1.0f / fadeRange );
		
	}catch( const deException & ){
		pRenderLuminancePB->UnmapBuffer();
		throw;
	}
	pRenderLuminancePB->UnmapBuffer();
DBG_EXIT("PrepareRenderParamBlock")
}



void deoglRenderWorld::RenderMaskedPass( deoglRenderPlan &plan ){
DBG_ENTER("RenderMaskedPass")
	const int maskedCount = plan.GetMaskedPlanCount();
	if( maskedCount == 0 ){
DBG_EXIT("RenderMaskedPass(early)")
		return;
	}
	
	// NOTE rendering masked passes is called in the main thread before clearing. this works
	//      since each rendered mask pass activates depth FBO only and clears it. while calling
	//      RenderWorld the rest is then cleared
	
	// render each masked object
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglRenderGeometry &rengeom = GetRenderThread().GetRenderers().GetGeometry();
	bool clearColor = plan.GetClearColor();
	int m;
	
	for( m=0; m<maskedCount; m++ ){
		deoglRenderPlanMasked * const maskedPlan = plan.GetMaskedPlanAt( m );
		
		// clear depth texture
		defren.ActivateFBODepth();
		
		OGL_CHECK( renderThread, glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ) );
		OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
		
		OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
		OGL_CHECK( renderThread, glDepthFunc( GL_ALWAYS ) );
		
		OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
		SetCullMode( plan.GetFlipCulling() );
		
		OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
		
		OGL_CHECK( renderThread, glEnable( GL_STENCIL_TEST ) );
		
		// clear the depth and stencil buffer
		OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
		
		OGL_CHECK( renderThread, glStencilMask( ~0 ) );
		OGL_CHECK( renderThread, pglClearBufferfi( GL_DEPTH_STENCIL, 0,
			defren.GetClearDepthValueRegular(), 0 ) );
		
		OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
		
		// render the mask
		OGL_CHECK( renderThread, glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE ) );
		OGL_CHECK( renderThread, glStencilMask( 0x01 ) );
		OGL_CHECK( renderThread, glStencilFunc( GL_ALWAYS, 0x01, 0x01 ) );
		
		// restore the render parameter shader parameter block
		if( m > 0 ){ // already prepared before the first mask
			PrepareRenderParamBlock( plan, NULL );
		}
		
		// render solid content
		pRenderTask->Clear();
		pRenderTask->SetRenderParamBlock( pRenderPB );
		
		pAddToRenderTask->Reset();
		pAddToRenderTask->SetSolid( true );
		pAddToRenderTask->SetSkinShaderType( deoglSkinTexture::estComponentDepth );
		pAddToRenderTask->SetNoRendered( false );
		
		pAddToRenderTask->AddComponentFaces( *maskedPlan->GetComponent(), maskedPlan->GetComponentTexture(), 0 );
		
		pRenderTask->PrepareForRender( renderThread );
		rengeom.RenderTask( *pRenderTask );
		
		// render the world using this mask
		deoglRenderPlan &maskedPlanPlan = *maskedPlan->GetPlan();
		maskedPlanPlan.SetClearColor( clearColor );
		RenderWorld( maskedPlanPlan, maskedPlan );
		//RenderFinalizeContext( maskedPlanPlan );  // this is wrong
		clearColor = maskedPlanPlan.GetClearColor();
	}
	
	plan.SetClearColor( clearColor );
	
	// restore the render parameter shader parameter block
	PrepareRenderParamBlock( plan, NULL );
	
	// the occlusion maps are trashed now. the best solution would be to use a set of occlusion
	// maps for each masked plan avoiding the trashing of the main occlusion maps
	
	//if( deoglSkinShader::REFLECTION_TEST_MODE == 2 ){
	//	ogl.GetRenderReflection().UpdateEnvMap( plan );
	//}
DBG_EXIT("RenderMaskedPass")
}



void deoglRenderWorld::RenderDebugDrawers( deoglRenderPlan &plan ){
DBG_ENTER("RenderDebugDrawers")
	deoglRenderThread &renderThread = GetRenderThread();
	deoglRenderDebugDrawer &rendd = GetRenderThread().GetRenderers().GetDebugDrawer();
	
	// attach diffuse texture as this is the output from the tone map pass
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	defren.ActivatePostProcessFBO( false );
	
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDepthFunc( GL_ALWAYS ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	
	rendd.RenderDebugDrawers( plan );
DBG_EXIT("RenderDebugDrawers")
}



void deoglRenderWorld::RenderEffects( deoglRenderPlan &plan ){
DBG_ENTER("RenderEffects")
	deoglRCamera * const camera = plan.GetCamera();
	if( ! camera ){
		return;
	}
	
	const int count = camera->GetEffectCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglREffect &effect = camera->GetEffectAt( i );
		
		if( effect.GetEnabled() ){
			effect.Render( plan );
		}
	}
DBG_EXIT("RenderEffects")
}

void deoglRenderWorld::RenderAntiAliasingPass(){
	// works but the results don't please me yet.
	/*
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager *tsMgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = *renderThread.GetDeferredRendering();
	
	// NOT REWORKED YET! DO NOT USE!!!
	
	// set attachments
	info->SwapRenderTextures();
	defren.AttachColor( info->GetPrimaryRenderTexture(), false, false ); // no depth, no stencil
	
	tsMgr->EnableTexture( 0, info->GetSecondaryRenderTexture(), GetSamplerClampNearest() );
	tsMgr->EnableDepth( 1, defren.GetDepthTexture() );
	tsMgr->EnableTexture( 2, defren.GetNormalTexture(), GetSamplerClampNearest() );
	
	// set states
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	
	// set program
	ogl.pSetProgram( espDrAntiAliasing );
	ogl.pSetProgramParam( espDrAntiAliasing_Options, defren.GetPixelSizeU(),
		defren.GetPixelSizeV(), 0.1f, 0.0f );
	
	// render quad
	defren.RenderFullScreenQuad();
	*/
}

void deoglRenderWorld::RenderFinalizeFBO( deoglRenderPlan &plan ){
DBG_ENTER("RenderFinalizeFBO")
	deoglRenderThread &renderThread = GetRenderThread();
	const bool copyDepth = plan.GetFBOTargetCopyDepth();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglShaderCompiled *shader;
	deoglTexSamplerConfig *sampler;
	
	const int viewportHeight = plan.GetViewportHeight();
	const int viewportWidth = plan.GetViewportWidth();
	const int viewportX = plan.GetViewportX();
	const int viewportY = plan.GetViewportY();
	
	renderThread.GetFramebuffer().Activate( plan.GetFBOTarget() );
	
	OGL_CHECK( renderThread, glViewport( viewportX, viewportY, viewportWidth, viewportHeight ) );
	OGL_CHECK( renderThread, glScissor( viewportX, viewportY, viewportWidth, viewportHeight ) );
	
	renderThread.GetTexture().GetStages().DisableAllStages();
	OGL_CHECK( renderThread, glViewport( viewportX, viewportY, viewportWidth, viewportHeight ) );
	OGL_CHECK( renderThread, glScissor( viewportX, viewportY, viewportWidth, viewportHeight ) );
	OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
	
	sampler = &GetSamplerClampLinear();
	
	tsmgr.EnableTexture( 0, *defren.GetPostProcessTexture(), *sampler );
	if( copyDepth ){
		tsmgr.EnableTexture( 1, *defren.GetDepthTexture1(), *sampler );
	}
	
	// set states
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	
	if( copyDepth ){
		OGL_CHECK( renderThread, glDepthMask( GL_TRUE ) );
		OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
		OGL_CHECK( renderThread, glDepthFunc( GL_ALWAYS ) );
		
	}else{
		OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
		OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	}
	
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	
	// set program and parameters
	if( copyDepth ){
		renderThread.GetShader().ActivateShader( pShaderFinalize );
		shader = pShaderFinalize->GetCompiled();
		
	}else{
		renderThread.GetShader().ActivateShader( pShaderFinalize );
		shader = pShaderFinalize->GetCompiled();
	}
	
	shader->SetParameterFloat( spfinPosTransform, 1.0f, 1.0f, 0.0f, 0.0f );
	shader->SetParameterFloat( spfinGamma, 1.0f, 1.0f, 1.0f, 1.0f );
	shader->SetParameterFloat( spfinBrightness, 0.0f, 0.0f, 0.0f, 0.0f );
	shader->SetParameterFloat( spfinContrast, 1.0f, 1.0f, 1.0f, 1.0f );
	
	if( plan.GetUpsideDown() ){
		defren.SetShaderParamFSQuad( *shader, spfinTCTransform );
		
	}else{
		defren.SetShaderParamFSQuadUpsideDown( *shader, spfinTCTransform );
	}
	
	defren.RenderFSQuadVAO();
	
	
	
	// revert to 2d mode
	tsmgr.DisableAllStages(); // deprecated
	OGL_CHECK( renderThread, glEnable( GL_BLEND ) ); // deprecated
	OGL_CHECK( renderThread, glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) ); // deprecated
	OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) ); // deprecated
	
	// dev mode debug image check
	renderThread.GetDebug().GetDeveloperMode().CheckDebugImageUse();
DBG_EXIT("RenderFinalizeFBO")
}

void deoglRenderWorld::RenderFinalizeContext( deoglRenderPlan &plan ){
DBG_ENTER("RenderFinalizeContext")
//	deoglRenderableColorTexture *maskRenderTexture = info->GetMaskRenderTexture();
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglConfiguration &config = renderThread.GetConfiguration();
	float gamma = 1.0f / ( OGL_RENDER_GAMMA * config.GetGammaCorrection() );
	float contrast = config.GetContrast();
	float brightness = config.GetBrightness();
	float pbn = brightness + ( 1.0f - contrast ) * 0.5f;
	deoglShaderCompiled *shader;
	deoglTexSamplerConfig *sampler;
	
	const int viewportHeight = plan.GetViewportHeight();
	const int viewportWidth = plan.GetViewportWidth();
	const int viewportX = plan.GetViewportX();
	const int viewportY = plan.GetViewportY();
	
	renderThread.GetTexture().GetStages().DisableAllStages();
	OGL_CHECK( renderThread, glViewport( viewportX, viewportY, viewportWidth, viewportHeight ) );
	OGL_CHECK( renderThread, glScissor( viewportX, viewportY, viewportWidth, viewportHeight ) );
	OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
	
	if( plan.GetUseUpscaling() ){
		sampler = &GetSamplerClampLinear();
		
	}else{
		sampler = &GetSamplerClampNearest();
	}
	
	tsmgr.EnableTexture( 0, *defren.GetPostProcessTexture(), *sampler );
	
	// set states
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
	OGL_CHECK( renderThread, glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
	
	// set program and parameters
	renderThread.GetShader().ActivateShader( pShaderFinalize );
	shader = pShaderFinalize->GetCompiled();
	
	shader->SetParameterFloat( spfinPosTransform, 1.0f, 1.0f, 0.0f, 0.0f );
	shader->SetParameterFloat( spfinGamma, gamma, gamma, gamma, 1.0f );
	shader->SetParameterFloat( spfinBrightness, pbn, pbn, pbn, 0.0f );
	shader->SetParameterFloat( spfinContrast, contrast, contrast, contrast, 1.0f );
	
	if( plan.GetUpsideDown() ){
		defren.SetShaderParamFSQuad( *shader, spfinTCTransform );
		
	}else{
		defren.SetShaderParamFSQuadUpsideDown( *shader, spfinTCTransform );
	}
	
	defren.RenderFSQuadVAO();
	
	
	
	// revert to 2d mode
	tsmgr.DisableAllStages(); // deprecated
	OGL_CHECK( renderThread, glEnable( GL_BLEND ) ); // deprecated
	OGL_CHECK( renderThread, glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) ); // deprecated
	OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) ); // deprecated
	
	// dev mode debug image check
	renderThread.GetDebug().GetDeveloperMode().CheckDebugImageUse();
DBG_EXIT("RenderFinalizeContext")
}



void deoglRenderWorld::AddTopLevelDebugInfo(){
	pDebugInfo.AddTopLevelDebugInfo();
}

void deoglRenderWorld::DevModeDebugInfoChanged(){
	pDebugInfo.DevModeDebugInfoChanged();
}



// Private Functions
//////////////////////

void deoglRenderWorld::pCleanUp(){
	if( pAddToRenderTaskParticles ){
		delete pAddToRenderTaskParticles;
	}
	if( pRenderTaskParticles ){
		delete pRenderTaskParticles;
	}
	if( pParticleSorter ){
		delete pParticleSorter;
	}
	if( pAddToRenderTask ){
		delete pAddToRenderTask;
	}
	if( pRenderTask ){
		delete pRenderTask;
	}
	if( pRenderCubePB ){
		pRenderCubePB->FreeReference();
	}
	if( pRenderLuminancePB ){
		pRenderLuminancePB->FreeReference();
	}
	if( pRenderPB ){
		pRenderPB->FreeReference();
	}
}
