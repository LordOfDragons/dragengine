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
#include "deoglRenderLightSky.h"
#include "deoglRenderGI.h"
#include "../deoglRenderGeometry.h"
#include "../deoglRenderWorld.h"
#include "../debug/deoglRenderDebug.h"
#include "../defren/deoglDeferredRendering.h"
#include "../plan/deoglRenderPlan.h"
#include "../plan/deoglRenderPlanSkyLight.h"
#include "../task/deoglRenderTask.h"
#include "../task/deoglAddToRenderTask.h"
#include "../task/deoglRenderTaskShader.h"
#include "../task/shared/deoglRenderTaskSharedShader.h"
#include "../lod/deoglLODCalculator.h"
#include "../deoglRenderOcclusion.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../collidelist/deoglCollideList.h"
#include "../../collidelist/deoglCollideListComponent.h"
#include "../../collidelist/deoglCollideListManager.h"
#include "../../collidelist/deoglCollideListHTSector.h"
#include "../../collidelist/deoglCollideListHTSCluster.h"
#include "../../collidelist/deoglCollideListPropField.h"
#include "../../collidelist/deoglCollideListPropFieldType.h"
#include "../../collidelist/deoglCollideListPropFieldCluster.h"
#include "../../component/deoglComponentLOD.h"
#include "../../component/deoglRComponent.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../debug/deoglDebugSaveTexture.h"
#include "../../debug/deoglDebugInformation.h"
#include "../../debug/deoglDebugSnapshot.h"
#include "../../debug/debugSnapshot.h"
#include "../../framebuffer/deoglFramebuffer.h"
#include "../../framebuffer/deoglFramebufferManager.h"
#include "../../gi/deoglGI.h"
#include "../../gi/deoglGIRayCache.h"
#include "../../gi/deoglGIState.h"
#include "../../light/deoglRLight.h"
#include "../../light/shader/deoglLightShader.h"
#include "../../light/volume/deoglLightVolume.h"
#include "../../model/deoglModelLOD.h"
#include "../../model/deoglRModel.h"
#include "../../occlusiontest/deoglOcclusionTest.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTDebug.h"
#include "../../renderthread/deoglRTDefaultTextures.h"
#include "../../renderthread/deoglRTFramebuffer.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../renderthread/deoglRTTexture.h"
#include "../../shaders/deoglShaderCompiled.h"
#include "../../shaders/deoglShaderDefines.h"
#include "../../shaders/deoglShaderManager.h"
#include "../../shaders/deoglShaderProgram.h"
#include "../../shaders/deoglShaderSources.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../shadow/deoglSCSolid.h"
#include "../../shadow/deoglSCTransparent.h"
#include "../../shadow/deoglShadowCaster.h"
#include "../../shadow/deoglShadowMapper.h"
#include "../../shapes/deoglShape.h"
#include "../../shapes/deoglShapeManager.h"
#include "../../skin/channel/deoglSkinChannel.h"
#include "../../skin/deoglSkinTexture.h"
#include "../../skin/shader/deoglSkinShader.h"
#include "../../skin/state/deoglSkinState.h"
#include "../../sky/deoglRSky.h"
#include "../../sky/deoglRSkyLayer.h"
#include "../../sky/deoglRSkyInstance.h"
#include "../../sky/deoglRSkyInstanceLayer.h"
#include "../../sky/deoglSkyLayerGICascade.h"
#include "../../texture/arraytexture/deoglArrayTexture.h"
#include "../../texture/arraytexture/deoglRenderableDepthArrayTexture.h"
#include "../../texture/arraytexture/deoglRenderableDepthArrayTextureManager.h"
#include "../../texture/cubemap/deoglCubeMap.h"
#include "../../texture/cubemap/deoglRenderableDepthCubeMap.h"
#include "../../texture/deoglTextureStageManager.h"
#include "../../texture/texture2d/deoglRenderableDepthTexture.h"
#include "../../texture/texture2d/deoglRenderableDepthTextureManager.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../utils/convexhull/deoglConvexHull2D.h"
#include "../../visitors/deoglVolumeShape.h"
#include "../../world/deoglRWorld.h"
#include "../../world/deoglWorldOctree.h"
#include "../../devmode/deoglDeveloperMode.h"
#include "../../utils/collision/deoglDCollisionFrustum.h"
#include "../../utils/collision/deoglDCollisionBox.h"
#include "../../utils/collision/deoglDCollisionSphere.h"
#include "../../utils/collision/deoglDCollisionVolume.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decConvexVolume.h>
#include <dragengine/common/math/decConvexVolumeFace.h>
#include <dragengine/common/math/decConvexVolumeList.h>



// Definitions
////////////////


enum eSPLight{
	splQuadParams,
	splPosTransform,
	splLightView,
	splLightColor,
	splAmbientColor,
	splLayerBorder,
	splShadowMatrix1,
	splShadowMatrix2,
	splShadowMatrix3,
	splShadowMatrix4,
	splShaMap1Params
};

enum eSPLightAmbient{
	splaQuadParams,
	splaPosTransform,
	splaLightView,
	splaAmbientColor
};



// Class deoglRenderLightSky
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderLightSky::deoglRenderLightSky( deoglRenderThread &renderThread ) :
deoglRenderLightBase( renderThread ),

pColList2( NULL ),

pSolidShadowMap( NULL )
{
// 	const deoglConfiguration &config = renderThread.GetConfiguration();
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
// 	bool useEncodeDepth = config.GetUseEncodeDepth();
	deoglShaderSources *sources;
	deoglShaderDefines defines;
	
	try{
// 		sources = shaderManager.GetSourcesNamed( "DefRen AO Sky" );
// 		if( useEncodeDepth ){
// 			defines.SetDefine( "GEOM_ENCODED_DEPTH", "1" );
// 		}
// 		pShaderAO = shaderManager.GetProgramWith( sources, defines );
// 		defines.RemoveAllDefines();
		
		sources = shaderManager.GetSourcesNamed( "DefRen Clear Depth" );
		pShaderClearDepth = shaderManager.GetProgramWith( sources, defines );
		
		sources = shaderManager.GetSourcesNamed( "DefRen Occlusion OccMap" );
		AddSharedSPBDefines( defines );
		defines.SetDefine( "WITH_SHADOWMAP", true );
		pShaderOccMesh = shaderManager.GetProgramWith( sources, defines );
		pShaderOccMesh->EnsureRTSShader();
		pShaderOccMesh->GetRTSShader()->SetSPBInstanceIndexBase( 0 );
		defines.RemoveAllDefines();
		
		
		
		pColList2 = new deoglCollideList;
		
		
		
		// debug information
		const decColor colorText( 1.0f, 1.0f, 1.0f, 1.0f );
		const decColor colorBgUp( 0.05f, 0.05f, 0.05f, 0.75f );
		const decColor colorBg( 0.0f, 0.0f, 0.25f, 0.75f );
		const decColor colorBgSub( 0.05f, 0.05f, 0.05f, 0.75f );
		const decColor colorBgSub2( 0.1f, 0.1f, 0.1f, 0.75f );
		const decColor colorBgSub3( 0.15f, 0.15f, 0.15f, 0.75f );
		
		pDebugInfoSolid.TakeOver( new deoglDebugInformation( "Sky", colorText, colorBgUp ) );
		pDebugInfoTransparent.TakeOver( new deoglDebugInformation( "Sky", colorText, colorBgUp ) );
		
		
		
		pDebugInfoSolidDetail.TakeOver( new deoglDebugInformation( "Light Sky Solid", colorText, colorBg ) );
		
		pDebugInfoSolidShadow.TakeOver( new deoglDebugInformation( "Shadow", colorText, colorBgSub ) );
		pDebugInfoSolidDetail->GetChildren().Add( pDebugInfoSolidShadow );
		
		pDebugInfoSolidShadowOcclusion.TakeOver( new deoglDebugInformation( "Occlusion", colorText, colorBgSub2 ) );
		pDebugInfoSolidShadow->GetChildren().Add( pDebugInfoSolidShadowOcclusion );
		
		pDebugInfoSolidShadowSplit.TakeOver( new deoglDebugInformation( "Splits", colorText, colorBgSub2 ) );
		pDebugInfoSolidShadow->GetChildren().Add( pDebugInfoSolidShadowSplit );
		
		pDebugInfoSolidShadowSplitContent.TakeOver( new deoglDebugInformation( "Add Elements", colorText, colorBgSub3 ) );
		pDebugInfoSolidShadowSplit->GetChildren().Add( pDebugInfoSolidShadowSplitContent );
		
		pDebugInfoSolidShadowSplitLODLevels.TakeOver( new deoglDebugInformation( "LOD Levels", colorText, colorBgSub3 ) );
		pDebugInfoSolidShadowSplit->GetChildren().Add( pDebugInfoSolidShadowSplitLODLevels );
		
		pDebugInfoSolidShadowSplitClear.TakeOver( new deoglDebugInformation( "Clear", colorText, colorBgSub3 ) );
		pDebugInfoSolidShadowSplit->GetChildren().Add( pDebugInfoSolidShadowSplitClear );
		
		pDebugInfoSolidShadowSplitTask.TakeOver( new deoglDebugInformation( "Task", colorText, colorBgSub3 ) );
		pDebugInfoSolidShadowSplit->GetChildren().Add( pDebugInfoSolidShadowSplitTask );
		
		pDebugInfoSolidShadowSplitRender.TakeOver( new deoglDebugInformation( "Render", colorText, colorBgSub3 ) );
		pDebugInfoSolidShadowSplit->GetChildren().Add( pDebugInfoSolidShadowSplitRender );
		
		pDebugInfoSolidShadowGI.TakeOver( new deoglDebugInformation( "Shadow GI", colorText, colorBgSub ) );
		pDebugInfoSolidDetail->GetChildren().Add( pDebugInfoSolidShadowGI );
		
		pDebugInfoSolidLight.TakeOver( new deoglDebugInformation( "Light", colorText, colorBgSub ) );
		pDebugInfoSolidDetail->GetChildren().Add( pDebugInfoSolidLight );
		
		
		
		pDebugInfoTransparentDetail.TakeOver( new deoglDebugInformation( "Light Sky Transp", colorText, colorBg ) );
		
		pDebugInfoTransparentLight.TakeOver( new deoglDebugInformation( "Light", colorText, colorBgSub ) );
		pDebugInfoTransparentDetail->GetChildren().Add( pDebugInfoTransparentLight );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRenderLightSky::~deoglRenderLightSky(){
	pCleanUp();
}



// Rendering
//////////////

void deoglRenderLightSky::RenderLights( deoglRenderPlan &plan, bool solid, const deoglRenderPlanMasked *mask ){
// 	if( ! solid ){
// 		return;
// 	}
	
	if( solid ){
		// this situation is annoying. to build sky shadow render tasks these actions are required:
		// - find sky light content (parallel task)
		// - render occlusion tests => requires view occlusion map to be rendered
		// - resolve occlusion tests and remove invisible components
		// - build render tasks (2 parallel tasks)
		// 
		// the problem here is the dependency on the view occlusion map to be rendered paired
		// with the time consumption of finding sky light content. due to this rendering the
		// occlusion tests can be done earliest at the end of the prepare render plan stage.
		// but to build render plans in parallel we need to first wait for the occlusion test
		// results to come in. by this time we are often close to rendering the sky light
		// and the effect is voided.
		// 
		// a solution would be to use glFenceSync() and periodically check if a render process
		// finished to start a following one as soon as possible. this is though quite complex
		// and possibly not helping. kept for later.
		// 
		// what is done here is starting the parellel tasks here. while rendering the GI shadow
		// is rendered first in the hope to hide the building of the render tasks there
		plan.SkyLightsStartBuildRT();
	}
	
	const int count = plan.GetSkyLightCount();
	if( count == 0 ){
		return;
	}
	
	DebugTimersReset( plan, false );
	
	//RenderAO( plan );
	RestoreFBO( plan );
	
	int i;
	for( i=0; i<count; i++ ){
		RenderLight( *plan.GetSkyLightAt( i ), solid, mask );
	}
	
	if( solid ){
		DebugTimer1Sample( plan, *pDebugInfoSolid, true );
		
	}else{
		DebugTimer1Sample( plan, *pDebugInfoTransparent, true );
	}
}



void deoglRenderLightSky::RenderAO( deoglRenderPlan &plan ){
#if 0
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
//	deoglRenderGeometry &rengeom = *ogl.GetRenderGeometry();
//	const decMatrix &cameraMatrix = plan.GetCameraMatrix();
	
	float znear = plan.GetCameraImageDistance();
	float zfar = plan.GetCameraViewDistance();
	float fov = plan.GetCameraFov();
	float fovRatio = plan.GetCameraFovRatio();
	float aspectRatio = plan.GetAspectRatio();
	
	// attach ao texture
	defren.ActivateFBOTemporary1( false );
	tsmgr.EnableTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
	
	// clear the ao texture to full bright
	OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
	OGL_CHECK( renderThread, glBlendFunc( GL_ONE, GL_ONE ) );
	OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	SetCullMode( ! plan.GetFlipCulling() );
	
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_FALSE, GL_FALSE ) );
	const GLfloat clearColor[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
	
	// set shader
	renderThread.GetShader().ActivateShader( pShaderAO );
	
	pShaderAO->GetCompiled()->SetParameterFloat( 0, defren.GetPixelSizeU(), defren.GetPixelSizeV(), 0.0f, 0.0f );
	
	// mapping (0,1-sv) => (-1,-1) and (su,1) => (1,1)
	// (x-c)*s = x*s-c*s
	float scaleU = 2.0f / defren.GetScalingU(); // s = 1/(su/2) = 2/su
	float scaleV = 2.0f / defren.GetScalingV(); // s = 1/(sv/2) = 2/sv
	float offsetU = -1.0f; // c = su/2 ; c*s = su/2 * 1/(su/2) = 1
	float offsetV = 1.0f - scaleV; // c = 1-sv/2 ; c*s = (1-sv/2) * 1/(sv/2) = 1/(sv/2)-1 = s-1
	
	// u=0; 0*(2/su) + (-1) = -1 (ok)
	// u=su; su*(2/su) + (-1) = 2-1 = 1 (ok)
	// v=1-sv; (1-sv)*(2/sv) + (1-2/sv) = 2/sv-2 + 1-2/sv = -1 (ok)
	// v=1; 1*(2/sv) + (1-2/sv) = 2/sv + 1-2/sv = 1 (ok)
	pShaderAO->GetCompiled()->SetParameterFloat( 1, scaleU, scaleV, offsetU, offsetV );
	
	float q = zfar / ( zfar - znear );
	pShaderAO->GetCompiled()->SetParameterVector4( 2, plan.GetDepthToPosition() );
	
	// render probes
//	rengeom.RenderSkyLOProbes( &plan, plan.GetCollideList(), plan.GetCameraMatrix() );
#endif
}

void deoglRenderLightSky::RenderLight( deoglRenderPlanSkyLight &plan, bool solid,
const deoglRenderPlanMasked *mask ){
	if( ! plan.GetUseLight() ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	//deoglShadowMapper &shadowMapper = renderThread.GetShadowMapper();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglLightShader *lightShader;
	
	const bool useShadow = plan.GetUseShadow();
	int shadowMapSize = plan.GetPlan().GetShadowSkySize();
	int passCount = 1;
	
	// render shadow map
	if( useShadow && solid ){
		passCount = plan.GetShadowLayerCount();
		RenderShadows( plan, solid, mask );
		RestoreFBO( plan.GetPlan() );
		DebugTimer2SampleCount( plan.GetPlan(), *pDebugInfoSolidShadow, 1, true );
	}
	
	// render light
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
	
	// set shader
	deoglRSkyInstanceLayer &skyLayer = *plan.GetLayer();
	if( useShadow ){
		lightShader = skyLayer.GetShaderFor( plan.GetPlan().GetRenderStereo()
			? deoglRSkyInstanceLayer::estStereoSolid
			: deoglRSkyInstanceLayer::estSolid );
		
	}else{
		if( skyLayer.GetHasLightDirect() ){
			lightShader = skyLayer.GetShaderFor( plan.GetPlan().GetRenderStereo()
				? deoglRSkyInstanceLayer::estStereoNoShadow
				: deoglRSkyInstanceLayer::estNoShadow );
			
		}else{
			lightShader = skyLayer.GetShaderFor( plan.GetPlan().GetRenderStereo()
				? deoglRSkyInstanceLayer::estStereoAmbient
				: deoglRSkyInstanceLayer::estAmbient );
		}
	}
	
	if( ! lightShader ){
		DETHROW( deeInvalidParam );
	}
	
	renderThread.GetShader().ActivateShader( lightShader->GetShader() );
	
	// set program parameters
	deoglSPBlockUBO * const spbInstance = skyLayer.GetInstanceParameterBlock();
	deoglSPBlockUBO * const spbLight = skyLayer.GetLightParameterBlock();
	
	if( ! spbLight || ! spbInstance ){
		DETHROW( deeInvalidParam );
	}
	
	UpdateLightParamBlock( *lightShader, *spbLight, plan );
	UpdateInstanceParamBlock( *lightShader, *spbInstance, plan, shadowMapSize, passCount );
	
	renderThread.GetRenderers().GetWorld().ActivateRenderPB( plan.GetPlan() );
	spbLight->Activate();
	spbInstance->Activate();
	
	// set textures
	int target;
	
	target = lightShader->GetTextureTarget( deoglLightShader::ettShadow1SolidDepth );
	if( target != -1 ){
		tsmgr.EnableArrayTexture( target, *pSolidShadowMap->GetArrayTexture(), GetSamplerShadowClampLinear() );
	}
	
	target = lightShader->GetTextureTarget( deoglLightShader::ettNoise );
	if( target != -1 ){
		tsmgr.EnableTexture( target, *renderThread.GetDefaultTextures().GetNoise2D(), GetSamplerRepeatNearest() );
	}
	
	target = lightShader->GetTextureTarget( deoglLightShader::ettLightDepth1 );
	if( target != -1 ){
		tsmgr.EnableArrayTexture( target, *pSolidShadowMap->GetArrayTexture(), GetSamplerClampLinear() );
	}
	
	// set the ao texture
	//tsmgr.EnableTexture( 5, *defren.GetTemporaryTexture(), deoglTextureStageManager::etfNearest, GL_CLAMP );
	
	// render quad
	defren.RenderFSQuadVAO();
	
	if( solid ){
		DebugTimer2SampleCount( plan.GetPlan(), *pDebugInfoSolidLight, 1, true );
		
	}else{
		DebugTimer2SampleCount( plan.GetPlan(), *pDebugInfoTransparentLight, 1, true );
	}
	
	// GI rays
	if( ! mask && solid ){
		deoglGIState * const giState = plan.GetPlan().GetUpdateGIState();
		if( giState ){
			const deoglSkyLayerGICascade * const slgc = plan.GetLayer()->GetGICascade( giState->GetSkyShadowCascade() );
			const deoglSCSolid * const scsolid = slgc ? &slgc->GetShadowCaster().GetSolid() : NULL;
			
			RestoreFBOGITraceRays( *giState );
			
			deoglTexture *texture1 = renderThread.GetDefaultTextures().GetShadowMap();
			deoglTexture *texture2 = texture1;
			lightShader = NULL;
			
			if( useShadow && scsolid ){
				if( scsolid->GetStaticMap() ){
					texture1 = scsolid->GetStaticMap();
					
					if( scsolid->GetDynamicMap() ){
						texture2 = scsolid->GetDynamicMap();
						lightShader = skyLayer.GetShaderFor( deoglRSkyInstanceLayer::estGIRaySolid2 );
						
					}else{
						lightShader = skyLayer.GetShaderFor( deoglRSkyInstanceLayer::estGIRaySolid1 );
					}
					
				}else if( scsolid->GetDynamicMap() ){
					texture1 = scsolid->GetDynamicMap();
					lightShader = skyLayer.GetShaderFor( deoglRSkyInstanceLayer::estGIRaySolid1 );
					
				}else{
					lightShader = skyLayer.GetShaderFor( deoglRSkyInstanceLayer::estGIRayNoShadow );
				}
				
			}else if( skyLayer.GetHasLightDirect() ){
				lightShader = skyLayer.GetShaderFor( deoglRSkyInstanceLayer::estGIRayNoShadow );
			}
			
			if( lightShader ){
				renderThread.GetShader().ActivateShader( lightShader->GetShader() );
				
				// WARNING always non-stereo!
				renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
				
				spbLight->Activate();
				spbInstance->Activate();
				
				target = lightShader->GetTextureTarget( deoglLightShader::ettShadow1SolidDepth );
				if( target != -1 ){
					tsmgr.EnableTexture( target, *texture1, GetSamplerShadowClampLinear() );
					
				}
				
				target = lightShader->GetTextureTarget( deoglLightShader::ettShadow2SolidDepth );
				if( target != -1 ){
					tsmgr.EnableTexture( target, *texture2, GetSamplerShadowClampLinear() );
				}
				
				target = lightShader->GetTextureTarget( deoglLightShader::ettNoise );
				if( target != -1 ){
					tsmgr.EnableTexture( target, *renderThread.GetDefaultTextures().GetNoise2D(), GetSamplerRepeatNearest() );
				}
				
				defren.RenderFSQuadVAO();
			}
		}
	}
}



void deoglRenderLightSky::RenderShadows( deoglRenderPlanSkyLight &plan, bool solid,
const deoglRenderPlanMasked *mask ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglShadowMapper &shadowMapper = renderThread.GetShadowMapper();
	
	if( solid && ! mask ){
		RenderGIShadows( plan, shadowMapper );
	}
	
	RenderShadowMap( plan, shadowMapper );
	
	// activate stencil as we had to disable it for rendering the shadow maps
	OGL_CHECK( renderThread, glEnable( GL_STENCIL_TEST ) );
}

// #define SKY_SHADOW_LAYERED_RENDERING 1
#define SKY_SHADOW_FILTERED 1
#define SKY_SHADOW_OCCMAP_VISIBLE 1
// #define SKY_SHADOW_DEBUG_TIME 1

#if defined SKY_SHADOW_LAYERED_RENDERING && defined SKY_SHADOW_FILTERED
	#undef SKY_SHADOW_FILTERED
#endif

#ifdef SKY_SHADOW_DEBUG_TIME
	#define SSDTPF(...) printf(__VA_ARGS__)
	#define SSDTLOG(...) renderThread.GetLogger().LogInfoFormat(__VA_ARGS__)
#else
	#define SSDTPF(...)
	#define SSDTLOG(...)
#endif

void deoglRenderLightSky::RenderShadowMap( deoglRenderPlanSkyLight &plan, deoglShadowMapper &shadowMapper ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglRWorld &world = *plan.GetPlan().GetWorld();
#ifdef SKY_SHADOW_LAYERED_RENDERING
	deoglSPBlockUBO * const renderParamBlock = renderThread.GetRenderers().GetLight().GetShadowCascadedPB();
#else
	deoglSPBlockUBO * const renderParamBlock = renderThread.GetRenderers().GetLight().GetShadowPB();
#endif
	const bool bugClearEntireArrTex = renderThread.GetCapabilities().GetClearEntireArrayTexture().Broken();
// 	deoglAddToRenderTask &addToRenderTask = renderThread.GetRenderers().GetLight().GetAddToRenderTask();
// 	deoglRenderTask &renderTask = renderThread.GetRenderers().GetLight().GetRenderTask();
	const decDVector &referencePosition = world.GetReferencePosition();
	deoglRenderGeometry &rengeom = renderThread.GetRenderers().GetGeometry();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglRSkyInstanceLayer &skyLayer = *plan.GetLayer();
	decMatrix matrixCamera;
	
	const decDMatrix &matCamInv = plan.GetPlan().GetInverseCameraMatrix();
	
	const decDVector &camPos = plan.GetPlan().GetCameraPosition();
	const bool clearBackFaceFragments = false; // problems. see GIShadow for the reason why
// 	int c, componentCount;
	deoglDCollisionBox box;
	decVector position;
	int i;
	
	// occlusion test the content to remove as much as possible. rendering the tests has
	// been done ahead of time in the render plan sky light to avoid stalling
#ifdef SKY_SHADOW_OCCMAP_VISIBLE
	// done by deoglRenderPlanSkyLight
// 	planSkyLight.GetOcclusionTest()->UpdateResults();
// 	collideList.RemoveCulledElements();
// 	DebugTimer3Sample( plan, *pDebugInfoSolidShadowOcclusion, false );
#endif
	
	// get light properties
	const decMatrix matLig = decMatrix::CreateRotation( 0.0f, PI, 0.0f ) * skyLayer.GetMatrix();
	const decMatrix matLigInv = matLig.Invert();
	const decMatrix matCL = matCamInv.GetRotationMatrix().ToMatrix() * matLigInv;
	
	// set opengl states
	OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDepthFunc( GL_LEQUAL ) ); // lequal, sky light uses linear depth
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
	SetCullMode( false );
	
	OGL_CHECK( renderThread, glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_TRUE ) );
	
	if( pglClipControl && defren.GetUseInverseDepth() ){
		pglClipControl( GL_LOWER_LEFT, GL_NEGATIVE_ONE_TO_ONE ); // reset, sky light uses linear depth
	}
	
	// set up stencil mask. this is used to mark back facing fragments (see after rendering)
	if( clearBackFaceFragments ){
		OGL_CHECK( renderThread, glEnable( GL_STENCIL_TEST ) );
		OGL_CHECK( renderThread, pglStencilOpSeparate( GL_FRONT, GL_KEEP, GL_KEEP, GL_ZERO ) );
		OGL_CHECK( renderThread, pglStencilOpSeparate( GL_BACK, GL_KEEP, GL_KEEP, GL_REPLACE ) );
		OGL_CHECK( renderThread, glStencilMask( 0xff ) );
		OGL_CHECK( renderThread, glStencilFunc( GL_ALWAYS, 0xff, 0xff ) );
		
	}else{
		OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	}
	
	// get shadow map
	const int shadowMapSize = plan.GetPlan().GetShadowSkySize();
	const int layerCount = plan.GetShadowLayerCount();
	
	if( pSolidShadowMap && ( pSolidShadowMap->GetWidth() != shadowMapSize
			|| pSolidShadowMap->GetLayerCount() != layerCount ) ){
		pSolidShadowMap->SetInUse( false );
		pSolidShadowMap = NULL;
	}
	if( ! pSolidShadowMap ){
		pSolidShadowMap = renderThread.GetTexture().GetRenderableDepthArrayTexture()
			.GetWith( shadowMapSize, shadowMapSize, layerCount, true, false );
	}
	
	// clear shadow map
	shadowMapper.SetForeignSolidDepthArrayTexture( pSolidShadowMap->GetArrayTexture() );
	
#ifdef SKY_SHADOW_LAYERED_RENDERING
	if( bugClearEntireArrTex ){
		for( i=0; i<layerCount; i++ ){
			shadowMapper.ActivateSolidArrayTextureLayer( shadowMapSize, layerCount, i, true );
			OGL_CHECK( renderThread, pglClearBufferfi( GL_DEPTH_STENCIL, 0, 1.0f, 0xff ) );
		}
		shadowMapper.ActivateSolidArrayTexture( shadowMapSize, layerCount, true );
		
	}else{
		shadowMapper.ActivateSolidArrayTexture( shadowMapSize, layerCount, true );
		OGL_CHECK( renderThread, pglClearBufferfi( GL_DEPTH_STENCIL, 0, 1.0f, 0xff ) );
	}
#else
	if( ! bugClearEntireArrTex ){
		shadowMapper.ActivateSolidArrayTexture( shadowMapSize, layerCount, true );
		if( clearBackFaceFragments ){
			OGL_CHECK( renderThread, pglClearBufferfi( GL_DEPTH_STENCIL, 0, 1.0f, 0xff ) );
			
		}else{
			const GLfloat clearDepth = 1.0f;
			OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
		}
	}
#endif
	
	// render all layers into the shadow map
	DebugTimer3Reset( plan.GetPlan(), true );
	DebugTimer4Reset( plan.GetPlan(), true );
	
#ifndef SKY_SHADOW_FILTERED
		#ifdef SKY_SHADOW_DEBUG_TIME
		decTimer timer;
		#endif
	// render task "all in one". calculating the optimal LOD to use would require using
	// SetComponentLODOrtho() on the lowest split the component is visible in. since this
	// is calculated on the GPU this can not be done here without expensive CPU calculations.
	// as a makeshift solution the LOD is calculated the same way as with the camera view
// 	deoglLODCalculator lodCalculator[ 4 ];
// 	float lodBoxWidth[ 4 ], lodBoxHeight[ 4 ];
// 	
// 	for( i=0; i<layerCount; i++ ){
// 		lodCalculator[ i ].SetMaxPixelError( config.GetLODMaxPixelError() );
// 		lodCalculator[ i ].SetMaxErrorPerLevel( config.GetLODMaxErrorPerLevel() );
// 		
// 		const deoglRenderPlanSkyLight::sShadowLayer &sl = planSkyLight.GetShadowLayerAt( i );
// 		lodBoxWidth[ i ] = sl.maxExtend.x - sl.minExtend.x;
// 		lodBoxHeight[ i ] = sl.maxExtend.y - sl.minExtend.y;
// 	}
	
	const int componentCount = collideList.GetComponentCount();
	for( i=0; i<componentCount; i++ ){
		deoglCollideListComponent &cl = *collideList.GetComponentAt( i );
		deoglRComponent &component = *cl.GetComponent();
// 		const int mask = component.GetSkyShadowSplitMask();
// 		
// 		if( mask ){
// 			int lowest;
// 			for( lowest=0; lowest<layerCount && ( mask & ( 1 << lowest ) ) == 0; lowest++ );
// 			lodCalculator[ lowest ].SetComponentLODOrtho( cl, lodBoxWidth[ lowest ],
// 				lodBoxHeight[ lowest ], shadowMapSize, shadowMapSize );
// 			
// 		}else{
// 			cl.SetLODLevel( component.GetLODCount() - 1 );
// 		}
		
		component.SetSpecialFlagsFromSkyShadowLayerMask( cl.GetCascadeMask() );
	}
	
	
	deoglLODCalculator lodCalculator;
	
	lodCalculator.SetMaxPixelError( config.GetLODMaxPixelError() );
	lodCalculator.SetMaxErrorPerLevel( config.GetLODMaxErrorPerLevel() );
	
	lodCalculator.SetComponentLODProjection( collideList, plan.GetCameraPosition(),
		plan.GetInverseCameraMatrix().TransformView(), plan.GetCameraFov(),
		plan.GetCameraFov() * plan.GetCameraFovRatio(), plan.GetViewportWidth(),
		plan.GetViewportHeight() );
			SSDT("SkyLight %d: Prepare %dys\n", i, (int)(timer.GetElapsedTime()*1e6f));
	
	
	renderTask.Clear();
	renderTask.SetRenderParamBlock( renderParamBlock );
	
	addToRenderTask.Reset();
#ifdef SKY_SHADOW_LAYERED_RENDERING
	renderTask.SetUseSPBInstanceFlags( true );
	addToRenderTask.SetUseSpecialParamBlock( true );
	addToRenderTask.SetSkinShaderType( deoglSkinTexture::estComponentShadowOrthogonalCascaded );
#else
	addToRenderTask.SetSkinShaderType( deoglSkinTexture::estComponentShadowOrthogonal );
#endif
	addToRenderTask.SetSolid( true );
	addToRenderTask.SetNoShadowNone( true );
	addToRenderTask.SetForceDoubleSided( true );
	addToRenderTask.AddComponents( collideList );
			SSDT("SkyLight %d: RTBuild %dys\n", i, (int)(timer.GetElapsedTime()*1e6f));
	
	renderTask.PrepareForRender( renderThread );
	DebugTimer4Sample( plan, *pDebugInfoSolidShadowSplitTask, true );
			SSDT("SkyLight %d: RTPrepare %dys (s=%d t=%d v=%d i=%d si=%d)\n", i,
				(int)(timer.GetElapsedTime()*1e6f), renderTask.GetShaderCount(),
				renderTask.GetTotalTextureCount(), renderTask.GetTotalVAOCount(),
				renderTask.GetTotalInstanceCount(), renderTask.GetTotalSubInstanceCount());
#endif
	
	// render layers
	
#ifdef SKY_SHADOW_LAYERED_RENDERING
	// render solid content. two different depth offsets for front and back faces are used. double sided always
	// counts as front facing. this way all can be rendered in one go
	renderParamBlock->MapBuffer();
	try{
		renderParamBlock->SetParameterDataMat4x4( deoglSkinShader::erutMatrixP, decMatrix() );
		
		for( i=0; i<layerCount; i++ ){
			deoglRenderPlanSkyLight::sShadowLayer &sl = planSkyLight.GetShadowLayerAt( i );
			
			// determine the shadow limits
			position.x = ( sl.minExtend.x + sl.maxExtend.x ) * 0.5f;
			position.y = ( sl.minExtend.y + sl.maxExtend.y ) * 0.5f;
			position.z = ( sl.minExtend.z + sl.maxExtend.z ) * 0.5f;
			//position.z = sl.minExtend.z;
			
			sl.position = ( camPos - referencePosition ).ToVector() + ( matLig * position );
			sl.scale.x = 1.0f / ( sl.maxExtend.x - sl.minExtend.x );
			sl.scale.y = 1.0f / ( sl.maxExtend.y - sl.minExtend.y );
			sl.scale.z = 1.0f / ( sl.maxExtend.z - sl.minExtend.z );
			
			// setup matrix
			sl.matrix = matCL * decMatrix::CreateTranslation( -sl.minExtend ) * decMatrix::CreateScale( sl.scale );
			
			matrixCamera = decMatrix::CreateCamera( sl.position, matLig.TransformView(), matLig.TransformUp() )
				* decMatrix::CreateScale( sl.scale * 2.0f );
			
			renderParamBlock->SetParameterDataArrayMat4x3( deoglSkinShader::erutMatrixV, i, matrixCamera );
			renderParamBlock->SetParameterDataArrayMat4x4( deoglSkinShader::erutMatrixVP, i, matrixCamera );
			renderParamBlock->SetParameterDataArrayMat3x3( deoglSkinShader::erutMatrixVn,
				i, matrixCamera.GetRotationMatrix().Invert() );
			renderParamBlock->SetParameterDataArrayVec4( deoglSkinShader::erutDepthOffset,
				i, sl.zscale, sl.zoffset * sl.scale.z, -sl.zscale, -sl.zoffset * sl.scale.z );
		}
		
	}catch( const deException & ){
		renderParamBlock->UnmapBuffer();
		throw;
	}
	renderParamBlock->UnmapBuffer();
			SSDT("SkyLight %d: ParamBlock %dys\n", i, (int)(timer.GetElapsedTime()*1e6f));
	
	OGL_CHECK( renderThread, pglStencilOpSeparate( GL_FRONT, GL_KEEP, GL_KEEP, GL_ZERO ) );
	OGL_CHECK( renderThread, pglStencilOpSeparate( GL_BACK, GL_KEEP, GL_KEEP, GL_REPLACE ) );
	OGL_CHECK( renderThread, glStencilFunc( GL_ALWAYS, 0xff, 0xff ) );
	
	rengeom.RenderTask( renderTask );
	
	// clear back facing fragments. back facing fragments cause a lot of troubles to shadow
	// casting. the idea here is to consider back facing fragments hit before any front facing
	// fragments to belong to the outer hull of a map. this is similar to a fragment blocking
	// the sky light right at the near end of the shadow map. by clearing back facing fragments
	// to 0 artifacts for back faces are eliminated altogether. this though won't help with
	// issues on front facing fragments but makes underground scenes safe
	OGL_CHECK( renderThread, glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP ) );
	OGL_CHECK( renderThread, glStencilFunc( GL_NOTEQUAL, 0, 0xff ) );
	
	renderThread.GetShader().ActivateShader( pShaderClearDepth );
	
	pShaderClearDepth->GetCompiled()->SetParameterFloat( 0, -1.0f );
	
	defren.RenderFSQuadVAO();
			SSDT("SkyLight %d: Render %dys\n", i, (int)(timer.GetElapsedTime()*1e6f));
	
	DebugTimer4Sample( plan, *pDebugInfoSolidShadowSplitRender, true );
	DebugTimer3SampleCount( plan, *pDebugInfoSolidShadowSplit, 1, false );
	
#else
	for( i=0; i<layerCount; i++ ){
		deoglRenderPlanSkyLight::sShadowLayer &sl = plan.GetShadowLayerAt( i );
			#ifdef SKY_SHADOW_DEBUG_TIME
			decTimer timer;
			#endif
		
#ifdef SKY_SHADOW_FILTERED
		if( i == 0 ){
			plan.WaitFinishedBuildRT1();
			
		}else if( i == 3 ){
			plan.WaitFinishedBuildRT2();
		}
		DebugTimer4Sample( plan.GetPlan(), *pDebugInfoSolidShadowSplitContent, false );
#endif
			SSDTPF("SkyLight %d: CollideList %dys\n", i, (int)(timer.GetElapsedTime()*1e6f));
		
		// determine the shadow limits
		position.x = ( sl.minExtend.x + sl.maxExtend.x ) * 0.5f;
		position.y = ( sl.minExtend.y + sl.maxExtend.y ) * 0.5f;
		position.z = ( sl.minExtend.z + sl.maxExtend.z ) * 0.5f;
		//position.z = sl.minExtend.z;
		
		sl.position = ( camPos - referencePosition ).ToVector() + ( matLig * position );
		sl.scale.x = 1.0f / ( sl.maxExtend.x - sl.minExtend.x );
		sl.scale.y = 1.0f / ( sl.maxExtend.y - sl.minExtend.y );
		sl.scale.z = 1.0f / ( sl.maxExtend.z - sl.minExtend.z );
		
		// calculate lod levels to use
#ifdef SKY_SHADOW_FILTERED
		const float lodBoxWidth = sl.maxExtend.x - sl.minExtend.x;
		const float lodBoxHeight = sl.maxExtend.y - sl.minExtend.y;
		
		deoglLODCalculator lodCalculator;
		lodCalculator.SetMaxPixelError( 2 );
		
		lodCalculator.SetComponentLODOrtho( *pColList2, lodBoxWidth, lodBoxHeight, shadowMapSize, shadowMapSize );
		DebugTimer4Sample( plan.GetPlan(), *pDebugInfoSolidShadowSplitLODLevels, false );
#endif
		
		// setup matrix
		sl.matrix = matCL * decMatrix::CreateTranslation( -sl.minExtend ) * decMatrix::CreateScale( sl.scale );
		
		matrixCamera = decMatrix::CreateCamera( sl.position, matLig.TransformView(), matLig.TransformUp() )
			* decMatrix::CreateScale( sl.scale * 2.0f );
		
		if( clearBackFaceFragments ){
			OGL_CHECK( renderThread, pglStencilOpSeparate( GL_FRONT, GL_KEEP, GL_KEEP, GL_ZERO ) );
			OGL_CHECK( renderThread, pglStencilOpSeparate( GL_BACK, GL_KEEP, GL_KEEP, GL_REPLACE ) );
			OGL_CHECK( renderThread, glStencilFunc( GL_ALWAYS, 0xff, 0xff ) );
		}
		
		// activate shadow map. since objects mostly reside in only one shadow texture
		// we attach the individual layers instead of the entire texture. avoids the
		// need for a geometry program to be used.
		shadowMapper.ActivateSolidArrayTextureLayer( shadowMapSize, layerCount, i, true );
		if( bugClearEntireArrTex ){
			OGL_CHECK( renderThread, pglClearBufferfi( GL_DEPTH_STENCIL, 0, 1.0f, 0xff ) );
			DebugTimer4Sample( plan.GetPlan(), *pDebugInfoSolidShadowSplitClear, true );
		}
			SSDTPF("SkyLight %d: Prepare %dys\n", i, (int)(timer.GetElapsedTime()*1e6f));
		
		// render solid content. two different depth offsets for front and back faces are used. double sided always
		// counts as front facing. this way all can be rendered in one go
		const float depthScale = sl.zscale;
		const float depthOffset = sl.zoffset * sl.scale.z;
		
		renderParamBlock->MapBuffer();
		try{
			renderParamBlock->SetParameterDataMat4x3( deoglSkinShader::erutMatrixV, matrixCamera );
			renderParamBlock->SetParameterDataMat4x4( deoglSkinShader::erutMatrixP, decMatrix() );
			renderParamBlock->SetParameterDataMat4x4( deoglSkinShader::erutMatrixVP, matrixCamera );
			renderParamBlock->SetParameterDataMat3x3( deoglSkinShader::erutMatrixVn,
				matrixCamera.GetRotationMatrix().Invert() );
			renderParamBlock->SetParameterDataVec4( deoglSkinShader::erutDepthOffset,
				depthScale, depthOffset, -depthScale, -depthOffset );
			
		}catch( const deException & ){
			renderParamBlock->UnmapBuffer();
			throw;
		}
		renderParamBlock->UnmapBuffer();
			SSDTPF("SkyLight %d: ParamBlock %dys\n", i, (int)(timer.GetElapsedTime()*1e6f));
		
#ifdef SKY_SHADOW_FILTERED
		sl.renderTask->SetRenderParamBlock( renderParamBlock );
		sl.renderTask->SetForceDoubleSided( true );
		
		if( renderThread.GetConfiguration().GetDebugSnapshot() == edbgsnapLightSkyShadowRenTask ){
			renderThread.GetLogger().LogInfoFormat( "RenderLightSky: shadow split %i", i );
			sl.renderTask->DebugPrint( renderThread.GetLogger() );
		}
		DebugTimer4Sample( plan.GetPlan(), *pDebugInfoSolidShadowSplitTask, true );
			SSDTPF("SkyLight %d: RTBuild %dys (s=%d t=%d v=%d i=%d si=%d)\n", i,
				(int)(timer.GetElapsedTime()*1e6f), sl.renderTask->GetShaderCount(),
				sl.renderTask->GetTotalTextureCount(), sl.renderTask->GetTotalVAOCount(),
				sl.renderTask->GetTotalInstanceCount(), sl.renderTask->GetTotalSubInstanceCount());
#endif
		rengeom.RenderTask( *sl.renderTask );
		
		// clear back facing fragments. back facing fragments cause a lot of troubles to shadow
		// casting. the idea here is to consider back facing fragments hit before any front facing
		// fragments to belong to the outer hull of a map. this is similar to a fragment blocking
		// the sky light right at the near end of the shadow map. by clearing back facing fragments
		// to 0 artifacts for back faces are eliminated altogether. this though won't help with
		// issues on front facing fragments but makes underground scenes safe
		if( clearBackFaceFragments ){
			OGL_CHECK( renderThread, glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP ) );
			OGL_CHECK( renderThread, glStencilFunc( GL_NOTEQUAL, 0, 0xff ) );
			
			renderThread.GetShader().ActivateShader( pShaderClearDepth );
			
			pShaderClearDepth->GetCompiled()->SetParameterFloat( 0, -1.0f );
			
			defren.RenderFSQuadVAO();
		}
			SSDTPF("SkyLight %d: Render %dys\n", i, (int)(timer.GetElapsedTime()*1e6f));
		
		// debug
		if( renderThread.GetConfiguration().GetDebugSnapshot() == edbgsnapLightSkySplits ){
			const decQuaternion rot = matLig.ToQuaternion();
			const float sx = ( sl.maxExtend.x - sl.minExtend.x ) * 0.5f;
			const float sy = ( sl.maxExtend.y - sl.minExtend.y ) * 0.5f;
			const float sz = ( sl.maxExtend.z - sl.minExtend.z ) * 0.5f;
			
			printf( "\n# split %i: fn=%g ff=%g lb=%g zs=%g zo=%g ex=(%g,%g,%g)(%g,%g,%g) pos=(%g,%g,%g) sca=(%g,%g,%g)\n",
				        i, sl.frustumNear, sl.frustumFar, sl.layerBorder, sl.zscale, sl.zoffset,
				sl.minExtend.x, sl.minExtend.y, sl.minExtend.z, sl.maxExtend.x, sl.maxExtend.y, sl.maxExtend.z,
				sl.position.x, sl.position.y, sl.position.z, sl.scale.x, sl.scale.y, sl.scale.z );
			
			printf( "meshSplit = bpy.data.meshes.new( 'split%i' )\n", i );
			printf( "objSplit = bpy.data.objects.new( 'split%i', meshSplit )\n", i );
			printf( "bpy.context.scene.objects.link( objSplit )\n" );
			printf( "bpy.context.scene.objects.active = objSplit\n" );
			printf( "vertices = []\n" );
			printf( "vertices.append( [%g,%g,%g] )\n", -sx, sy, -sz );
			printf( "vertices.append( [%g,%g,%g] )\n", sx, sy, -sz );
			printf( "vertices.append( [%g,%g,%g] )\n", sx, -sy, -sz );
			printf( "vertices.append( [%g,%g,%g] )\n", -sx, -sy, -sz );
			printf( "vertices.append( [%g,%g,%g] )\n", -sx, sy, sz );
			printf( "vertices.append( [%g,%g,%g] )\n", sx, sy, sz );
			printf( "vertices.append( [%g,%g,%g] )\n", sx, -sy, sz );
			printf( "vertices.append( [%g,%g,%g] )\n", -sx, -sy, sz );
			printf( "vertices = [ cposs( v, scale ) for v in vertices ]\n" );
			printf( "faces = []\n" );
			printf( "faces.append( [%i,%i,%i,%i] )\n", 1, 5, 6, 2 );
			printf( "faces.append( [%i,%i,%i,%i] )\n", 4, 0, 3, 7 );
			printf( "faces.append( [%i,%i,%i,%i] )\n", 4, 5, 1, 0 );
			printf( "faces.append( [%i,%i,%i,%i] )\n", 3, 2, 6, 7 );
			printf( "faces.append( [%i,%i,%i,%i] )\n", 5, 4, 7, 6 );
			printf( "faces.append( [%i,%i,%i,%i] )\n", 0, 1, 2, 3 );
			printf( "faces = [ f[::-1] for f in faces ]\n" );
			printf( "meshSplit.from_pydata( vertices, [], faces )\n" );
			printf( "meshSplit.update()\n" );
			printf( "objSplit.location = cposs( [%g,%g,%g], scale )\n", sl.position.x, sl.position.y, sl.position.z );
			printf( "objSplit.rotation_mode = 'QUATERNION'\n" );
			printf( "objSplit.rotation_quaternion = cquat( [%g,%g,%g,%g] )\n", rot.x, rot.y, rot.z, rot.w );
		}
		
		DebugTimer4Sample( plan.GetPlan(), *pDebugInfoSolidShadowSplitRender, true );
	}
#endif
	
	shadowMapper.DropForeignArrayTextures();
	DebugTimer3SampleCount( plan.GetPlan(), *pDebugInfoSolidShadowSplit, layerCount, true );
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == edbgsnapLightSkySplits ){
		renderThread.GetConfiguration().SetDebugSnapshot( 0 );
	}
	if( renderThread.GetConfiguration().GetDebugSnapshot() == edbgsnapLightSkyShadowRenTask ){
		renderThread.GetConfiguration().SetDebugSnapshot( 0 );
	}
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == edbgsnapLightSkyShadowMap ){
		renderThread.GetDebug().GetDebugSaveTexture().SaveDepthArrayTexture(
			*( pSolidShadowMap->GetArrayTexture() ), "sky_shadow", true );
		renderThread.GetConfiguration().SetDebugSnapshot( 0 );
	}
}

void deoglRenderLightSky::RenderGIShadows( deoglRenderPlanSkyLight &plan,
deoglShadowMapper &shadowMapper ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglGIState * const giState = plan.GetPlan().GetUpdateGIState();
	if( ! giState ){
		return;
	}
	
	DebugTimer4Reset( plan.GetPlan(), true );
	plan.WaitFinishedGIUpdateRT();
	
	deoglSkyLayerGICascade * const slgc = plan.GetLayer()->GetGICascade( giState->GetSkyShadowCascade() );
	if( ! slgc ){
		return;
	}
	
	// shadow layer
	const decMatrix matLig( decMatrix::CreateRotation( 0.0f, PI, 0.0f ) * plan.GetLayer()->GetMatrix() );
	
	deoglRenderPlanSkyLight::sShadowLayer &sl = plan.GetGIShadowLayer();
	
	const decDVector &worldRefPos = plan.GetPlan().GetWorld()->GetReferencePosition();
	const decVector position( ( sl.minExtend + sl.maxExtend ) * 0.5f );
	sl.position = ( slgc->GetPosition() - worldRefPos ).ToVector() + ( matLig * position );
	sl.scale.x = 1.0f / ( sl.maxExtend.x - sl.minExtend.x );
	sl.scale.y = 1.0f / ( sl.maxExtend.y - sl.minExtend.y );
	sl.scale.z = 1.0f / ( sl.maxExtend.z - sl.minExtend.z );
// 		renderThread.GetLogger().LogInfoFormat("ShadowMapGI: (%g,%g,%g)",
// 			sl.maxExtend.x - sl.minExtend.x, sl.maxExtend.y - sl.minExtend.y, sl.maxExtend.z - sl.minExtend.z);
	
	// prepare what is used by both
	deoglShadowCaster &shadowCaster = slgc->GetShadowCaster();
	deoglSCSolid &scsolid = shadowCaster.GetSolid();
	
	shadowCaster.SetShadowLayerCount( 1 );
	deoglShadowCaster::sShadowLayer &scsl = shadowCaster.GetShadowLayerAt( 0 );
	scsl.layerBorder = sl.layerBorder;
	scsl.scale = sl.scale;
	scsl.matrix = sl.matrix;
	
	// the calculation here is a bit complicated. the fragment position is transformed from
	// GI space to camera space to allow the same light shader code to light it. the GI shadow
	// map though is calculated relative to GI space. the GI and camera space are slightly
	// different from each other. for this reason we have to convert the position first from
	// camera spaces back to GI space. from there we can convert to light space
	// 
	// NOTE the rotation is not the problem but the position. the sl.position calculation
	//      above uses for camera plan->GetCameraPosition() instead of giState->GetPosition().
	//      this offset has to be corrected
	const decDMatrix matrixCamera( decDMatrix::CreateCamera( sl.position, matLig.TransformView(), matLig.TransformUp() ) );
	
	sl.matrix = ( plan.GetPlan().GetInverseCameraMatrix() * matrixCamera ).ToMatrix()
		* decMatrix::CreateTranslation( -sl.minExtend ) * decMatrix::CreateScale( sl.scale );
	
	const decMatrix matrixGI( matrixCamera.ToMatrix() * decMatrix::CreateScale( sl.scale * 2.0f ) );
			SSDTLOG("RenderGIShadowMap Prepare2 %d", (int)(timer.GetElapsedTime() * 1e6f));
	
	// the depth offset has to be scaled to be correct. this is required since the depth
	// offset from the configuration is in meters but the depth offset to apply in the
	// shader has to be in the range from 0 to 1
	// 
	// NOTE depth offset scaling is not working correctly. the shader uses dxdF/dydF for
	//      this which fails to work properly across different triangles. the correct
	//      solution requires using a geometry shader which calculates the triangle
	//      depth slope using the vZDepth calculated by the vertex shader. this are
	//      in the correct space and thus would allow to calculate the correct offset.
	//      
	//      actually a geometry shader would be required anyway to calculate and apply
	//      the depth offset without needing to manipulate the gl_FragDepth in the
	//      fragment shader. not touching gl_FragDepth is faster. using a geometry
	//      shader does make rendering more expensive but not touching gl_FragDepth
	//      should allow double-write speed to be used and should compensate
	const float depthOffset = sl.zoffset * sl.scale.z;
	const float depthScale = 0.0f; //sl.zscale;
	
	// static shadow map
	const int shadowMapSize = plan.GetGIShadowSize();
	
	if( plan.GetGIShadowUpdateStatic() ){
		// parameter block
		deoglSPBlockUBO * const renderParamBlock = renderThread.GetRenderers().GetLight().GetShadowPB();
		renderParamBlock->MapBuffer();
		try{
			renderParamBlock->SetParameterDataMat4x3( deoglSkinShader::erutMatrixV, matrixGI );
			renderParamBlock->SetParameterDataMat4x4( deoglSkinShader::erutMatrixP, decMatrix() );
			renderParamBlock->SetParameterDataMat4x4( deoglSkinShader::erutMatrixVP, matrixGI );
			renderParamBlock->SetParameterDataMat3x3( deoglSkinShader::erutMatrixVn,
				matrixGI.GetRotationMatrix().QuickInvert() );
			
			// for static shadow maps we use the clear depth trick. depth offsets though
			// interfere with this trick. the depth scale and front/back face specific offsets
			// can cause back faces to peek through. while no problem for the regular case
			// with depth clearing these peek through parts can cause major problems.
			// for this reason both front and back faces have to be equal. both should be
			// slightly pushing depth backwards. furthermore depth offset scaling has to
			// be disabled
			// 
			// NOTE we have a problem here. even if front and back faces share the same
			//      point in the VBO the GPU can add a slight error while calculating
			//      the interpolation. this can cause a stray pixel of back face popping
			//      up at the edge of geometry because the front face did not cover the
			//      exact same pixels as it should. this inconsistent behavior causes
			//      problems. for this reason this trick is disabled until a better idea
			renderParamBlock->SetParameterDataVec4( deoglSkinShader::erutDepthOffset,
				depthScale, depthOffset, -depthScale, -depthOffset ); // due to disabled
//				depthScale, depthOffset, depthScale, depthOffset );
			
		}catch( const deException & ){
			renderParamBlock->UnmapBuffer();
			throw;
		}
		renderParamBlock->UnmapBuffer();
				SSDTLOG("RenderGIShadowMap RenderParamBlock %d", (int)(timer.GetElapsedTime() * 1e6f));
		
		// render
		plan.GetGIRenderTaskStatic().SetRenderParamBlock( renderParamBlock );
		plan.GetGIRenderTaskStatic().SetForceDoubleSided( true );
		
		shadowMapper.SetForeignSolidDepthTexture( scsolid.ObtainStaticMapWithSize( shadowMapSize, true, false ) );
		RenderGIShadowMap( shadowMapper, plan.GetGIRenderTaskStatic(), shadowMapSize, false /*true*/ );
		shadowMapper.DropForeignTextures();
	}
	
	// dynamic shadow map. parameter block
	deoglSPBlockUBO * const renderParamBlock = renderThread.GetRenderers().GetLight().GetShadowPB();
	renderParamBlock->MapBuffer();
	try{
		// this is the same as above and could be shared
		renderParamBlock->SetParameterDataMat4x3( deoglSkinShader::erutMatrixV, matrixGI );
		renderParamBlock->SetParameterDataMat4x4( deoglSkinShader::erutMatrixP, decMatrix() );
		renderParamBlock->SetParameterDataMat4x4( deoglSkinShader::erutMatrixVP, matrixGI );
		renderParamBlock->SetParameterDataMat3x3( deoglSkinShader::erutMatrixVn, matrixGI.GetRotationMatrix().QuickInvert() );
		
		// this though is different and requires an own parameter block to be used.
		// in general only dynamic shadow map is rendered so the situation of this
		// parameter blockt to be updated twice (and stalling) is little
		renderParamBlock->SetParameterDataVec4( deoglSkinShader::erutDepthOffset,
			depthScale, depthOffset, -depthScale, -depthOffset );
		
	}catch( const deException & ){
		renderParamBlock->UnmapBuffer();
		throw;
	}
	renderParamBlock->UnmapBuffer();
			SSDTLOG("RenderGIShadowMap RenderParamBlock %d", (int)(timer.GetElapsedTime() * 1e6f));
	
	// render
	plan.GetGIRenderTaskDynamic().SetRenderParamBlock( renderParamBlock );
	plan.GetGIRenderTaskDynamic().SetForceDoubleSided( true );
	
	if( scsolid.GetDynamicMap() && scsolid.GetDynamicMap()->GetWidth() != shadowMapSize ){
		scsolid.DropDynamic();
	}
	
	shadowMapper.SetForeignSolidDepthTexture( scsolid.ObtainDynamicMapWithSize( shadowMapSize, true, false ) );
	RenderGIShadowMap( shadowMapper, plan.GetGIRenderTaskDynamic(), shadowMapSize, false );
	shadowMapper.DropForeignTextures();
	
	// clean up
	DebugTimer4Sample( plan.GetPlan(), *pDebugInfoSolidShadowGI, true );
}

void deoglRenderLightSky::RenderGIShadowMap( deoglShadowMapper &shadowMapper,
deoglRenderTask &renderTask, int shadowMapSize, bool clearBackFaceFragments ){
	deoglRenderThread &renderThread = GetRenderThread();
	
			#ifdef SKY_SHADOW_DEBUG_TIME
			decTimer timer;
			#endif
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	
	// opengl states
	OGL_CHECK( renderThread, glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_TRUE ) );
	OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDepthFunc( GL_LEQUAL ) ); // lequal, sky light uses linear depth
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( renderThread, glCullFace( GL_BACK ) );
	
	if( pglClipControl && defren.GetUseInverseDepth() ){
		pglClipControl( GL_LOWER_LEFT, GL_NEGATIVE_ONE_TO_ONE ); // reset, sky light uses linear depth
	}
	
	// set up stencil mask. this is used to mark back facing fragments (see after rendering)
	// front faces set 0x0 as stencil value and back faces 0xff
	if( clearBackFaceFragments ){
		OGL_CHECK( renderThread, glEnable( GL_STENCIL_TEST ) );
		OGL_CHECK( renderThread, pglStencilOpSeparate( GL_FRONT, GL_KEEP, GL_KEEP, GL_ZERO ) );
		OGL_CHECK( renderThread, pglStencilOpSeparate( GL_BACK, GL_KEEP, GL_KEEP, GL_REPLACE ) );
		OGL_CHECK( renderThread, glStencilMask( 0xff ) );
		OGL_CHECK( renderThread, glStencilFunc( GL_ALWAYS, 0xff, 0xff ) );
		
	}else{
		OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	}
	
	// get shadow map
			SSDTLOG("RenderGIShadowMap Prepare %d", (int)(timer.GetElapsedTime() * 1e6f));
	
	// clear shadow map
	shadowMapper.ActivateSolidTexture( shadowMapSize, false, true );
	
	if( clearBackFaceFragments ){
		OGL_CHECK( renderThread, pglClearBufferfi( GL_DEPTH_STENCIL, 0, 1.0f, 0xff ) );
		
	}else{
		const GLfloat clearDepth = 1.0f;
		OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
	}
			SSDTLOG("RenderGIShadowMap Clear %d", (int)(timer.GetElapsedTime() * 1e6f));
	
	// render shadow map. only solid content without holes is rendered at the highest lod level
	renderThread.GetRenderers().GetGeometry().RenderTask( renderTask );
			SSDTLOG("RenderGIShadowMap Render %d (s=%d t=%d v=%d i=%d si=%d)",
				(int)(timer.GetElapsedTime() * 1e6f), renderTask.GetShaderCount(), renderTask.GetTotalTextureCount(),
				renderTask.GetTotalVAOCount(), renderTask.GetTotalInstanceCount(), renderTask.GetTotalSubInstanceCount() );
	
	if( clearBackFaceFragments ){
		// clear back facing fragments. back facing fragments cause a lot of troubles to shadow
		// casting. the idea here is to consider back facing fragments hit before any front facing
		// fragments to belong to the outer hull of a map. this is similar to a fragment blocking
		// the sky light right at the near end of the shadow map. by clearing back facing fragments
		// to 0 artifacts for back faces are eliminated altogether. this though won't help with
		// issues on front facing fragments but makes underground scenes safe
		OGL_CHECK( renderThread, glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP ) );
		OGL_CHECK( renderThread, glStencilFunc( GL_NOTEQUAL, 0, 0xff ) );
		
		renderThread.GetShader().ActivateShader( pShaderClearDepth );
		
		pShaderClearDepth->GetCompiled()->SetParameterFloat( 0, -1.0f );
		
		defren.RenderFSQuadVAO();
				SSDTLOG("RenderGIShadowMap BackFaceClear %d", (int)(timer.GetElapsedTime() * 1e6f));
	}
}



void deoglRenderLightSky::UpdateLightParamBlock( deoglLightShader &lightShader,
deoglSPBlockUBO &paramBlock, deoglRenderPlanSkyLight &plan ){
	int target;
	
	// get light properties
	const deoglRSkyInstanceLayer &skyLayer = *plan.GetLayer();
	const float ambientIntensity = skyLayer.GetAmbientIntensity();
	const float lightIntensity = skyLayer.GetLightIntensity();
	decColor lightColor = skyLayer.GetLightColor();
	
	lightColor.r = powf( lightColor.r, OGL_RENDER_GAMMA );
	lightColor.g = powf( lightColor.g, OGL_RENDER_GAMMA );
	lightColor.b = powf( lightColor.b, OGL_RENDER_GAMMA );
	lightColor = TransformColor( plan.GetPlan().GetWorld()->GetLightColorMatrix(), lightColor );
	
	// set values
	paramBlock.MapBuffer();
	try{
		const bool hasGIState = plan.GetPlan().GetRenderGIState() != NULL;
		
		target = lightShader.GetLightUniformTarget( deoglLightShader::elutLightColor );
		if( target != -1 ){
			if( hasGIState ){
				paramBlock.SetParameterDataVec3( target, lightColor * ( lightIntensity + ambientIntensity ) );
				
			}else{
				paramBlock.SetParameterDataVec3( target, lightColor * lightIntensity );
				// this happens usually only for env-maps. better be dark than wrong lit
				//paramBlock.SetParameterDataVec3( target, lightColor * ( lightIntensity + ambientIntensity ) );
			}
		}
		
		target = lightShader.GetLightUniformTarget( deoglLightShader::elutLightColorAmbient );
		if( target != -1 ){
			if( hasGIState || plan.GetPlan().GetNoAmbientLight() ){
				paramBlock.SetParameterDataVec3( target, 0.0f, 0.0f, 0.0f );
				
			}else{
				paramBlock.SetParameterDataVec3( target, lightColor * ambientIntensity );
				// this happens usually only for env-maps. better be dark than wrong lit
				//paramBlock.SetParameterDataVec3( target, 0.0f, 0.0f, 0.0f );
			}
		}
		
		target = lightShader.GetLightUniformTarget( deoglLightShader::elutLightGIAmbientRatio );
		if( target != -1 ){
			if( plan.GetPlan().GetNoAmbientLight() ){
				paramBlock.SetParameterDataFloat( target, 0.0f );
				
			}else{
				paramBlock.SetParameterDataFloat( target,
					ambientIntensity / decMath::max( lightIntensity + ambientIntensity, 0.1f ) );
			}
		}
		
		// NOTE sky light is currently using a special handling which has to be replaced with the
		//      one used by point and spot lights. due to this pLightAmbientRatio can not be
		//      used in the shader since it is not defined here. to work with the new version the
		//      following changes have to be done (once the shader is up to it)
		//      
		//      - LightColor = lightColor * ( lightIntensity + ambientIntensity )
		//      - pLightAmbientRatio = ambientIntensity / ( lightIntensity + ambientIntensity )
		//      - pLightColorAmbient = not used anymore
		//      
		//      better would be to change the sky light definition to use ambient-ratio as
		//      lights do. in this case the solution becomes as simple as in the other lights:
		//      
		//      - LightColor = lightColor * lightIntensity
		//      - pLightAmbientRatio = lightAmbientRatio
		//      - pLightColorAmbient = not used anymore
		//      
		target = lightShader.GetLightUniformTarget( deoglLightShader::elutLightAmbientRatio );
		if( target != -1 ){
			paramBlock.SetParameterDataFloat( target, 0.0f );
		}
		
	}catch( const deException & ){
		paramBlock.UnmapBuffer();
		throw;
	}
	paramBlock.UnmapBuffer();
}

void deoglRenderLightSky::UpdateInstanceParamBlock( deoglLightShader &lightShader,
deoglSPBlockUBO &paramBlock, deoglRenderPlanSkyLight &plan, int shadowMapSize, int passCount ){
	const deoglConfiguration &config = GetRenderThread().GetConfiguration();
	const deoglRSkyInstanceLayer &skyLayer = *plan.GetLayer();
	float pixelSize, noiseScale;
	int target;
	
	// calculate matrices
	decMatrix matrixShadow[ 4 ], matrixShadowStereo[ 4 ], giMatrixShadow;
	float layerBorder[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float scaleZ[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
	
	const decDMatrix matrixLayer( decMatrix::CreateFromQuaternion(
		skyLayer.GetLightOrientation() ).QuickMultiply( skyLayer.GetMatrix() ) );
	const decDMatrix matrixLV( matrixLayer.QuickMultiply( plan.GetPlan().GetRefPosCameraMatrix() ) );
	
	// for VR rendering the shadow maps have been calculated against the left eye. when the right
	// eye is rendered the geometry has to be transformed into the left eye for the shadow maps
	// to work correctly.
	decMatrix transformShadow, transformShadowStereo;
	decDMatrix matrixLVStereo;
	
	if( plan.GetPlan().GetRenderStereo() ){
		// using stereo rendering the camera matrix is the left eye camera matrix. the stereo
		// matrix has then to transform from the right eye to the left eye. the stored plan
		// camera stereo matrix transforms from the left eye to the right eye
		matrixLVStereo = matrixLV.QuickMultiply( plan.GetPlan().GetCameraStereoMatrix() );
		transformShadowStereo = plan.GetPlan().GetCameraStereoInverseMatrix();
		
	}else{
		// using non-stereo rendering the situation is slightly different. if the left eye is
		// rendered we are in the correct eye already and do not modify the shadow matrix.
		// if the right eye is rendered the shadow matrix has to be transformed from right eye
		// to left eye. in contrary to stereo rendering this modification has to be applied to
		// the base shadow map not the stereo shadow map
		transformShadow = plan.GetPlan().GetCameraStereoInverseMatrix();
	}
	
	if( plan.GetUseShadow() ){
		// planSkyLight is only properly set up if shadow casting is used
		if( 0 < passCount ){
			const deoglRenderPlanSkyLight::sShadowLayer &sl = plan.GetShadowLayerAt( 0 );
			matrixShadow[ 0 ] = transformShadow * sl.matrix;
			matrixShadowStereo[ 0 ] = transformShadowStereo * sl.matrix;
			layerBorder[ 0 ] = sl.layerBorder;
			scaleZ[ 0 ] = sl.scale.z;
				if( fabsf( sl.scale.z ) < FLOAT_SAFE_EPSILON ){
					DETHROW( deeInvalidParam );
				}
		}
		if( 1 < passCount ){
			const deoglRenderPlanSkyLight::sShadowLayer &sl = plan.GetShadowLayerAt( 1 );
			matrixShadow[ 1 ] = transformShadow * sl.matrix;
			matrixShadowStereo[ 1 ] = transformShadowStereo * sl.matrix;
			layerBorder[ 1 ] = sl.layerBorder;
			scaleZ[ 1 ] = sl.scale.z;
				if( fabsf( sl.scale.z ) < FLOAT_SAFE_EPSILON ){
					DETHROW( deeInvalidParam );
				}
		}
		if( 2 < passCount ){
			const deoglRenderPlanSkyLight::sShadowLayer &sl = plan.GetShadowLayerAt( 2 );
			matrixShadow[ 2 ] = transformShadow * sl.matrix;
			matrixShadowStereo[ 2 ] = transformShadowStereo * sl.matrix;
			layerBorder[ 2 ] = sl.layerBorder;
			scaleZ[ 2 ] = sl.scale.z;
				if( fabsf( sl.scale.z ) < FLOAT_SAFE_EPSILON ){
					DETHROW( deeInvalidParam );
				}
		}
		if( 3 < passCount ){
			const deoglRenderPlanSkyLight::sShadowLayer &sl = plan.GetShadowLayerAt( 3 );
			matrixShadow[ 3 ] = transformShadow * sl.matrix;
			matrixShadowStereo[ 3 ] = transformShadowStereo * sl.matrix;
			layerBorder[ 3 ] = sl.layerBorder;
			scaleZ[ 3 ] = sl.scale.z;
				if( fabsf( sl.scale.z ) < FLOAT_SAFE_EPSILON ){
					DETHROW( deeInvalidParam );
				}
		}
		
		// only properly set up if plan has GI
		if( plan.GetPlan().GetRenderGIState() ){
			giMatrixShadow = plan.GetGIShadowLayer().matrix;
		}
	}
	
	paramBlock.MapBuffer();
	try{
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutLightView );
		if( target != -1 ){
			paramBlock.SetParameterDataArrayVec3( target, 0, matrixLV.TransformView().Normalized() );
			paramBlock.SetParameterDataArrayVec3( target, 1, matrixLVStereo.TransformView().Normalized() );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadowMatrix1 );
		if( target != -1 ){
			paramBlock.SetParameterDataArrayMat4x4( target, 0, matrixShadow[ 0 ] );
			paramBlock.SetParameterDataArrayMat4x4( target, 1, matrixShadowStereo[ 0 ] );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadowMatrix2 );
		if( target != -1 ){
			paramBlock.SetParameterDataArrayMat4x4( target, 0, matrixShadow[ 1 ] );
			paramBlock.SetParameterDataArrayMat4x4( target, 1, matrixShadowStereo[ 1 ] );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadowMatrix3 );
		if( target != -1 ){
			paramBlock.SetParameterDataArrayMat4x4( target, 0, matrixShadow[ 2 ] );
			paramBlock.SetParameterDataArrayMat4x4( target, 1, matrixShadowStereo[ 2 ] );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadowMatrix4 );
		if( target != -1 ){
			paramBlock.SetParameterDataArrayMat4x4( target, 0, matrixShadow[ 3 ] );
			paramBlock.SetParameterDataArrayMat4x4( target, 1, matrixShadowStereo[ 3 ] );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutDepthCompare );
		if( target != -1 ){
			paramBlock.SetParameterDataFloat( target, 0.0f );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutLayerBorder );
		if( target != -1 ){
			paramBlock.SetParameterDataVec4( target, layerBorder[ 0 ], layerBorder[ 1 ],
				layerBorder[ 2 ], layerBorder[ 3 ] );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadow1Solid );
		if( target != -1 ){
			pixelSize = 1.0f / ( float )shadowMapSize;
			noiseScale = config.GetShadowCubePCFSize() / ( float )shadowMapSize;
			
			paramBlock.SetParameterDataVec3( target, pixelSize, pixelSize, noiseScale );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadowDepthTransform );
		if( target != -1 ){
			paramBlock.SetParameterDataVec2( target, 1.0f / scaleZ[ 0 ], 1.0f / scaleZ[ 1 ] );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadowDepthTransform2 );
		if( target != -1 ){
			paramBlock.SetParameterDataVec2( target, 1.0f / scaleZ[ 2 ], 1.0f / scaleZ[ 3 ] );
		}
		
		// global illumination
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutGIShadowMatrix );
		if( target != -1 ){
			paramBlock.SetParameterDataMat4x4( target, giMatrixShadow );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutGIShadowParams );
		if( target != -1 ){
			const int giShadowMapSize = plan.GetGIShadowSize();
			pixelSize = 1.0f / ( float )giShadowMapSize;
			noiseScale = config.GetShadowCubePCFSize() / ( float )giShadowMapSize;
			
			paramBlock.SetParameterDataVec3( target, pixelSize, pixelSize, noiseScale );
		}
		
	}catch( const deException & ){
		paramBlock.UnmapBuffer();
		throw;
	}
	paramBlock.UnmapBuffer();
}



void deoglRenderLightSky::ResetDebugInfo(){
	pDebugInfoSolid->Clear();
	pDebugInfoTransparent->Clear();
	
	pDebugInfoSolidShadow->Clear();
	pDebugInfoSolidShadowOcclusion->Clear();
	pDebugInfoSolidShadowSplit->Clear();
	pDebugInfoSolidShadowSplitContent->Clear();
	pDebugInfoSolidShadowSplitLODLevels->Clear();
	pDebugInfoSolidShadowSplitClear->Clear();
	pDebugInfoSolidShadowSplitTask->Clear();
	pDebugInfoSolidShadowSplitRender->Clear();
	pDebugInfoSolidShadowGI->Clear();
	pDebugInfoSolidLight->Clear();
	
	pDebugInfoTransparentLight->Clear();
}

void deoglRenderLightSky::AddTopLevelDebugInfoSolid(){
	GetRenderThread().GetDebug().GetDebugInformationList().Add( pDebugInfoSolidDetail );
}

void deoglRenderLightSky::AddTopLevelDebugInfoTransparent(){
	GetRenderThread().GetDebug().GetDebugInformationList().Add( pDebugInfoTransparentDetail );
}

void deoglRenderLightSky::DevModeDebugInfoChanged(){
	const int details = GetRenderThread().GetDebug().GetDeveloperMode().GetDebugInfoDetails();
	const bool show = ( details & ( deoglDeveloperMode::edimLight | deoglDeveloperMode::edimLightSky ) )
		== ( deoglDeveloperMode::edimLight | deoglDeveloperMode::edimLightSky );
	
	pDebugInfoSolidDetail->SetVisible( show );
	//pDebugInfoSolidShadow->SetVisible( true );
	//pDebugInfoSolidShadowOcclusion->SetVisible( true );
	//pDebugInfoSolidShadowSplit->SetVisible( true );
	
	pDebugInfoTransparentDetail->SetVisible( show );
}



// Private Functions
//////////////////////

void deoglRenderLightSky::pCleanUp(){
	if( pSolidShadowMap ){
		pSolidShadowMap->SetInUse( false );
		pSolidShadowMap = NULL;
	}
	if( pSolidShadowMap ){
		pSolidShadowMap->SetInUse( false );
		pSolidShadowMap = NULL;
	}
	
	if( pColList2 ){
		delete pColList2;
	}
}
