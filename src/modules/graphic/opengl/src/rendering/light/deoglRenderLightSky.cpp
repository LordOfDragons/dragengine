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
#include "deoglRLSVisitorCollectElements.h"
#include "../deoglRenderGeometry.h"
#include "../deoglRenderWorld.h"
#include "../debug/deoglRenderDebug.h"
#include "../defren/deoglDeferredRendering.h"
#include "../plan/deoglRenderPlan.h"
#include "../plan/deoglRenderPlanSkyLight.h"
#include "../task/deoglRenderTask.h"
#include "../task/deoglAddToRenderTask.h"
#include "../task/deoglRenderTaskShader.h"
#include "../lod/deoglLODCalculator.h"
#include "../deoglRenderOcclusion.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../collidelist/deoglCollideList.h"
#include "../../collidelist/deoglCollideListComponent.h"
#include "../../collidelist/deoglCollideListManager.h"
#include "../../component/deoglComponentLOD.h"
#include "../../component/deoglRComponent.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../debug/deoglDebugSaveTexture.h"
#include "../../debug/deoglDebugInformation.h"
#include "../../debug/deoglDebugSnapshot.h"
#include "../../debug/debugSnapshot.h"
#include "../../framebuffer/deoglFramebuffer.h"
#include "../../framebuffer/deoglFramebufferManager.h"
#include "../../light/deoglRLight.h"
#include "../../light/shader/deoglLightShader.h"
#include "../../light/volume/deoglLightVolume.h"
#include "../../model/deoglModelLOD.h"
#include "../../model/deoglRModel.h"
#include "../../occlusiontest/deoglOcclusionTest.h"
#include "../../occlusiontest/deoglOcclusionTracingState.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTDebug.h"
#include "../../renderthread/deoglRTDefaultTextures.h"
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
#include "../../texture/arraytexture/deoglArrayTexture.h"
#include "../../texture/arraytexture/deoglRenderableArrayTexture.h"
#include "../../texture/arraytexture/deoglRenderableArrayTextureManager.h"
#include "../../texture/cubemap/deoglCubeMap.h"
#include "../../texture/cubemap/deoglRenderableDepthCubeMap.h"
#include "../../texture/deoglTextureStageManager.h"
#include "../../texture/texture2d/deoglRenderableDepthTexture.h"
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
pShaderAO( NULL ),

pSolidShadowMap( NULL ),

pDebugInfoSolid( NULL ),
pDebugInfoTransparent( NULL ),

pDebugInfoSolidDetail( NULL ),
pDebugInfoSolidShadow( NULL ),
pDebugInfoSolidShadowElements( NULL ),
pDebugInfoSolidShadowOcclusion( NULL ),
pDebugInfoSolidShadowOcclusionStart( NULL ),
pDebugInfoSolidShadowOcclusionVBO( NULL ),
pDebugInfoSolidShadowOcclusionTest( NULL ),
pDebugInfoSolidShadowVBOs( NULL ),
pDebugInfoSolidShadowClear( NULL ),
pDebugInfoSolidShadowSplit( NULL ),
pDebugInfoSolidShadowSplitContent( NULL ),
pDebugInfoSolidShadowSplitLODLevels( NULL ),
pDebugInfoSolidShadowSplitClear( NULL ),
pDebugInfoSolidShadowSplitTask( NULL ),
pDebugInfoSolidShadowSplitRender( NULL ),
pDebugInfoSolidLight( NULL ),

pDebugInfoTransparentDetail( NULL ),
pDebugInfoTransparentLight( NULL )
{
// 	const deoglConfiguration &config = renderThread.GetConfiguration();
// 	deoglShaderManager &shamgr = renderThread.GetShader().GetShaderManager();
// 	bool useEncodeDepth = config.GetUseEncodeDepth();
// 	deoglShaderSources *sources;
// 	deoglShaderDefines defines;
	
	try{
// 		sources = shamgr.GetSourcesNamed( "DefRen AO Sky" );
// 		if( useEncodeDepth ){
// 			defines.AddDefine( "GEOM_ENCODED_DEPTH", "1" );
// 		}
// 		pShaderAO = shamgr.GetProgramWith( sources, defines );
// 		defines.RemoveAllDefines();
		
		pColList2 = new deoglCollideList;
		
		
		
		// debug information
		const decColor colorText( 1.0f, 1.0f, 1.0f, 1.0f );
		const decColor colorBgUp( 0.05f, 0.05f, 0.05f, 0.75f );
		const decColor colorBg( 0.0f, 0.0f, 0.0f, 0.75f );
		const decColor colorBgSub( 0.05f, 0.05f, 0.05f, 0.75f );
		const decColor colorBgSub2( 0.1f, 0.1f, 0.1f, 0.75f );
		const decColor colorBgSub3( 0.15f, 0.15f, 0.15f, 0.75f );
		
		pDebugInfoSolid = new deoglDebugInformation( "Sky", colorText, colorBgUp );
		pDebugInfoTransparent = new deoglDebugInformation( "Sky", colorText, colorBgUp );
		
		
		
		pDebugInfoSolidDetail = new deoglDebugInformation( "Light Sky Solid", colorText, colorBg );
		
		pDebugInfoSolidShadow = new deoglDebugInformation( "Shadow", colorText, colorBgSub );
		pDebugInfoSolidDetail->GetChildren().Add( pDebugInfoSolidShadow );
		
		pDebugInfoSolidShadowElements = new deoglDebugInformation( "Add Elements", colorText, colorBgSub2 );
		pDebugInfoSolidShadow->GetChildren().Add( pDebugInfoSolidShadowElements );
		
		pDebugInfoSolidShadowOcclusion = new deoglDebugInformation( "Occlusion", colorText, colorBgSub2 );
		pDebugInfoSolidShadow->GetChildren().Add( pDebugInfoSolidShadowOcclusion );
		
		pDebugInfoSolidShadowOcclusionStart = new deoglDebugInformation( "Start", colorText, colorBgSub3 );
		pDebugInfoSolidShadowOcclusion->GetChildren().Add( pDebugInfoSolidShadowOcclusionStart );
		
		pDebugInfoSolidShadowOcclusionVBO = new deoglDebugInformation( "VBO", colorText, colorBgSub3 );
		pDebugInfoSolidShadowOcclusion->GetChildren().Add( pDebugInfoSolidShadowOcclusionVBO );
		
		pDebugInfoSolidShadowOcclusionTest = new deoglDebugInformation( "Test", colorText, colorBgSub3 );
		pDebugInfoSolidShadowOcclusion->GetChildren().Add( pDebugInfoSolidShadowOcclusionTest );
		
		pDebugInfoSolidShadowVBOs = new deoglDebugInformation( "VBOs", colorText, colorBgSub2 );
		pDebugInfoSolidShadow->GetChildren().Add( pDebugInfoSolidShadowVBOs );
		
		pDebugInfoSolidShadowClear = new deoglDebugInformation( "Clear", colorText, colorBgSub2 );
		pDebugInfoSolidShadow->GetChildren().Add( pDebugInfoSolidShadowClear );
		
		pDebugInfoSolidShadowSplit = new deoglDebugInformation( "Splits", colorText, colorBgSub2 );
		pDebugInfoSolidShadow->GetChildren().Add( pDebugInfoSolidShadowSplit );
		
		pDebugInfoSolidShadowSplitContent = new deoglDebugInformation( "Add Elements", colorText, colorBgSub3 );
		pDebugInfoSolidShadowSplit->GetChildren().Add( pDebugInfoSolidShadowSplitContent );
		
		pDebugInfoSolidShadowSplitLODLevels = new deoglDebugInformation( "LOD Levels", colorText, colorBgSub3 );
		pDebugInfoSolidShadowSplit->GetChildren().Add( pDebugInfoSolidShadowSplitLODLevels );
		
		pDebugInfoSolidShadowSplitClear = new deoglDebugInformation( "Clear", colorText, colorBgSub3 );
		pDebugInfoSolidShadowSplit->GetChildren().Add( pDebugInfoSolidShadowSplitClear );
		
		pDebugInfoSolidShadowSplitTask = new deoglDebugInformation( "Task", colorText, colorBgSub3 );
		pDebugInfoSolidShadowSplit->GetChildren().Add( pDebugInfoSolidShadowSplitTask );
		
		pDebugInfoSolidShadowSplitRender = new deoglDebugInformation( "Render", colorText, colorBgSub3 );
		pDebugInfoSolidShadowSplit->GetChildren().Add( pDebugInfoSolidShadowSplitRender );
		
		pDebugInfoSolidLight = new deoglDebugInformation( "Light", colorText, colorBgSub );
		pDebugInfoSolidDetail->GetChildren().Add( pDebugInfoSolidLight );
		
		
		
		pDebugInfoTransparentDetail = new deoglDebugInformation( "Light Sky Transp", colorText, colorBg );
		
		pDebugInfoTransparentLight = new deoglDebugInformation( "Light", colorText, colorBgSub );
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

void deoglRenderLightSky::RenderLights( deoglRenderPlan &plan, bool solid ){
	if( ! solid ){
// 		return;
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
		RenderLight( plan, solid, *plan.GetSkyLightAt( i ) );
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

void deoglRenderLightSky::RenderLight( deoglRenderPlan &plan, bool solid,
deoglRenderPlanSkyLight &planSkyLight ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	//deoglShadowMapper &shadowMapper = renderThread.GetShadowMapper();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglLightShader *lightShader;
	
	const bool useShadow = planSkyLight.GetUseShadow();
	int shadowMapSize = plan.GetShadowSkySize();
	int passCount = 1;
	
	// for transparent shadows we cut down shadow map size a lot
	if( ! solid ){
		//shadowMapSize >>= 2;
		//if( shadowMapSize < 16 ) shadowMapSize = 16;
		// we are reusing the solid shadow map so no cutting down or the result is wrong
	}
	
	// render shadow map
#if 0
			if(plan.GetFBOTarget()){
				static int c = 0;
				const decDVector &p = plan.GetCameraPosition();
				decString s;
				s.Format("envmap_%g-%g-%g/c%d/skylight-enter/", p.x, p.y, p.z, c++);
				deoglDebugSnapshot snapshot( renderThread );
				//snapshot.SetEnableColor( true );
				snapshot.SetEnableStates( true );
				snapshot.SetName( s );
				snapshot.TakeSnapshot();
			}
#endif
	if( useShadow && solid ){
		passCount = planSkyLight.GetShadowLayerCount();
		RenderShadows( plan, planSkyLight );
		RestoreFBO( plan );
		DebugTimer2SampleCount( plan, *pDebugInfoSolidShadow, 1, true );
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
	deoglRSkyInstanceLayer &skyLayer = *planSkyLight.GetLayer();
	if( useShadow ){
		lightShader = skyLayer.GetShaderFor( deoglRSkyInstanceLayer::estSolid );
		
	}else{
		if( skyLayer.GetHasLightDirect() ){
			lightShader = skyLayer.GetShaderFor( deoglRSkyInstanceLayer::estNoShadow );
			
		}else{
			lightShader = skyLayer.GetShaderFor( deoglRSkyInstanceLayer::estAmbient );
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
	
	UpdateLightParamBlock( *lightShader, *spbLight, plan, planSkyLight );
	UpdateInstanceParamBlock( *lightShader, *spbInstance, plan, planSkyLight, shadowMapSize, passCount );
	
	GetRenderThread().GetRenderers().GetLight().GetLightPB()->Activate();
	spbLight->Activate();
	spbInstance->Activate();
	
	// set textures
	int target;
	
	target = lightShader->GetTextureTarget( deoglLightShader::ettShadow1SolidDepth );
	if( target != -1 ){
		tsmgr.EnableArrayTexture( target, *pSolidShadowMap->GetTexture(), GetSamplerShadowClampLinear() );
	}
	
	target = lightShader->GetTextureTarget( deoglLightShader::ettNoise );
	if( target != -1 ){
		tsmgr.EnableTexture( target, *renderThread.GetDefaultTextures().GetNoise2D(), GetSamplerRepeatNearest() );
	}
	
	target = lightShader->GetTextureTarget( deoglLightShader::ettLightDepth1 );
	if( target != -1 ){
		tsmgr.EnableArrayTexture( target, *pSolidShadowMap->GetTexture(), GetSamplerClampLinear() );
	}
	
	deoglOcclusionTracingState * const tracingState = plan.GetOcclusionTracingState();
	if( tracingState ){
		target = lightShader->GetTextureTarget( deoglLightShader::ettOTOcclusion );
		if( target != -1 ){
			tsmgr.EnableTexture( target, tracingState->GetTextureProbeOcclusion(), GetSamplerClampLinear() );
		}
		
		target = lightShader->GetTextureTarget( deoglLightShader::ettOTDistance );
		if( target != -1 ){
			tsmgr.EnableTexture( target, tracingState->GetTextureProbeDistance(), GetSamplerClampLinear() );
		}
	}
	
	// set the ao texture
	//tsmgr.EnableTexture( 5, *defren.GetTemporaryTexture(), deoglTextureStageManager::etfNearest, GL_CLAMP );
	
	// render quad
	defren.RenderFSQuadVAO();
	
	if( solid ){
		DebugTimer2SampleCount( plan, *pDebugInfoSolidLight, 1, true );
		
	}else{
		DebugTimer2SampleCount( plan, *pDebugInfoTransparentLight, 1, true );
	}
	// debug
	//ogl.GetRenderDebug().DisplayArrayTextureLayer( plan, shadowMapper.GetSolidDepthArrayTexture(), 0 );
	//ogl.GetRenderDebug().DisplayArrayTextureLayer( plan, shadowMapper.GetSolidDepthArrayTexture(), 1 );
#if 0
			if(plan.GetFBOTarget()){
				static int c = 0;
				const decDVector &p = plan.GetCameraPosition();
				decString s;
				s.Format("envmap_%g-%g-%g/c%d/skylight/", p.x, p.y, p.z, c++);
				deoglDebugSnapshot snapshot( renderThread );
				//snapshot.SetEnableColor( true );
				snapshot.SetEnableTemporary2( true );
				snapshot.SetEnableStates( true );
				snapshot.SetName( s );
				snapshot.TakeSnapshot();
			}
#endif
}



void deoglRenderLightSky::RenderShadows( deoglRenderPlan &plan, deoglRenderPlanSkyLight &planSkyLight ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglShadowMapper &shadowMapper = renderThread.GetShadowMapper();
	
	RenderShadowMap( plan, planSkyLight, shadowMapper );
	
	// activate stencil as we had to disable it for rendering the shadow maps
	OGL_CHECK( renderThread, glEnable( GL_STENCIL_TEST ) );
}

void deoglRenderLightSky::RenderShadowMap( deoglRenderPlan &plan,
deoglRenderPlanSkyLight &planSkyLight, deoglShadowMapper &shadowMapper ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglRWorld &world = *plan.GetWorld();
	deoglSPBlockUBO * const renderParamBlock = renderThread.GetRenderers().GetLight().GetShadowPB();
	const bool bugClearEntireArrTex = renderThread.GetCapabilities().GetClearEntireArrayTexture().Broken();
	deoglAddToRenderTask &addToRenderTask = renderThread.GetRenderers().GetLight().GetAddToRenderTask();
	deoglRenderTask &renderTask = renderThread.GetRenderers().GetLight().GetRenderTask();
	const decDVector &referencePosition = world.GetReferencePosition();
	const deoglConfiguration &config = renderThread.GetConfiguration();
	deoglRenderGeometry &rengeom = renderThread.GetRenderers().GetGeometry();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglRSkyInstanceLayer &skyLayer = *planSkyLight.GetLayer();
	deoglLODCalculator lodCalculator;
	float lodBoxWidth, lodBoxHeight;
	deoglRComponent *component;
	decMatrix matrixCamera;
	
	const decDMatrix &matCamInv = plan.GetInverseCameraMatrix();
	const decDVector &camPos = plan.GetCameraPosition();
	int c, componentCount;
	deoglDCollisionBox box;
	decVector position;
	int i;
	
	// limitations
	const int layerLimit = planSkyLight.GetShadowLayerCount();
	
//	DebugTimer3Sample( plan, *pDebugInfoSolidShadowElements, false );  // done in deoglRenderPlan
	
	// occlusion test the content to remove as much as possible
//#if 0
	DebugTimer4Reset( plan, false );
	
	/* NOTE
	 * in the collector test against all splits from smallest to largest. if an object is too small
	 * in screen space for a split and no previous split considers it large enough skip the object
	 * alltogether (if too small for split 0 it will be also too small for all larger splits).
	 * add the object to the collide list of the matching split if large enough for this split
	 * (with the correct lod that is).
	 * testing the occ map then requires only testing objects not too small
	 */
	deoglCollideList &collideList = planSkyLight.GetCollideList();
	deoglOcclusionTest &occtest = renderThread.GetOcclusionTest();
	occtest.RemoveAllInputData();
	
	componentCount = collideList.GetComponentCount();
	for( c=0; c<componentCount; c++ ){
		component = collideList.GetComponentAt( c )->GetComponent();
		component->SetRenderVisible( true );
		component->StartOcclusionTest( referencePosition );
	}
	DebugTimer4Sample( plan, *pDebugInfoSolidShadowOcclusionStart, false );
	
	occtest.UpdateVBO();
	DebugTimer4Sample( plan, *pDebugInfoSolidShadowOcclusionVBO, true );
	
	GetRenderThread().GetRenderers().GetOcclusion().RenderTestsSkyLayer(
		plan, skyLayer, collideList, planSkyLight.GetFrustumBoxMinExtend(),
		planSkyLight.GetFrustumBoxMaxExtend() );
	DebugTimer4Sample( plan, *pDebugInfoSolidShadowOcclusionTest, true );
	DebugTimer3Sample( plan, *pDebugInfoSolidShadowOcclusion, false );
//#endif
	
	// update component VBOs.
	UpdateComponentVBO( collideList );
	DebugTimer3Sample( plan, *pDebugInfoSolidShadowVBOs, true );
	
	// set up lod calculator
	lodCalculator.SetMaxPixelError( config.GetLODMaxPixelError() );
	lodCalculator.SetMaxErrorPerLevel( config.GetLODMaxErrorPerLevel() );
	
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
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	
	OGL_CHECK( renderThread, glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_TRUE ) );
	
	if( pglClipControl && defren.GetUseInverseDepth() ){
		pglClipControl( GL_LOWER_LEFT, GL_NEGATIVE_ONE_TO_ONE ); // reset, sky light uses linear depth
	}
	
	// get shadow map
	const int shadowMapSize = plan.GetShadowSkySize();
	const int layerCount = planSkyLight.GetShadowLayerCount();
	
	if( pSolidShadowMap && ( pSolidShadowMap->GetWidth() != shadowMapSize
			|| pSolidShadowMap->GetLayerCount() != layerCount ) ){
		pSolidShadowMap->SetInUse( false );
		pSolidShadowMap = NULL;
	}
	if( ! pSolidShadowMap ){
		pSolidShadowMap = renderThread.GetTexture().GetRenderableArrayTexture().GetTextureWith(
			shadowMapSize, shadowMapSize, layerCount, deoglPixelBuffer::epfDepth );
	}
	
	// clear shadow map
	shadowMapper.SetForeignSolidDepthArrayTexture( pSolidShadowMap->GetTexture() );
	const GLfloat clearDepth = ( GLfloat )1.0f; // sky light uses linear depth
	
	if( ! bugClearEntireArrTex ){
		shadowMapper.ActivateSolidArrayTexture( shadowMapSize, layerCount );
		OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
		DebugTimer3Sample( plan, *pDebugInfoSolidShadowClear, true );
	}
	
	// render all layers into the shadow map
	const int faceCountLimitNear = 100000;
	const int faceCountLimitFar = 100;
	const double faceCountLimitDistanceNear = 1.0;
	const double faceCountLimitDistanceFar = 51.0;
	const double faceCountLimitRange = faceCountLimitDistanceFar - faceCountLimitDistanceNear;
	const double faceCountLimitFactor = ( double )( faceCountLimitFar - faceCountLimitNear ) / faceCountLimitRange;
	
	DebugTimer4Reset( plan, true );
	
	for( i=0; i<layerCount; i++ ){
		deoglRenderPlanSkyLight::sShadowLayer &sl = planSkyLight.GetShadowLayerAt( i );
		
		if( i < layerLimit ){
			const int splitMask = 1 << i;
			
			// determine split content
			pColList2->Clear();
			
			componentCount = collideList.GetComponentCount();
			
			for( c=0; c<componentCount; c++ ){
				component = collideList.GetComponentAt( c )->GetComponent();
				
				if( ( component->GetSkyShadowSplitMask() & splitMask ) == splitMask ){
					pColList2->AddComponent( component );
				}
			}
			DebugTimer4Sample( plan, *pDebugInfoSolidShadowSplitContent, false );
			
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
			lodBoxWidth = sl.maxExtend.x - sl.minExtend.x;
			lodBoxHeight = sl.maxExtend.y - sl.minExtend.y;
			
			lodCalculator.SetComponentLODOrtho( *pColList2, lodBoxWidth, lodBoxHeight, shadowMapSize, shadowMapSize );
			DebugTimer4Sample( plan, *pDebugInfoSolidShadowSplitLODLevels, false );
			
			// setup matrix
			sl.matrix = matCL * decMatrix::CreateTranslation( -sl.minExtend ) * decMatrix::CreateScale( sl.scale );
			
			matrixCamera = decMatrix::CreateCamera( sl.position, matLig.TransformView(), matLig.TransformUp() )
				* decMatrix::CreateScale( sl.scale * 2.0f );
		}
		
		// activate shadow map. since objects mostly reside in only one shadow texture
		// we attach the individual layers instead of the entire texture. avoids the
		// need for a geometry program to be used.
		shadowMapper.ActivateSolidArrayTextureLayer( shadowMapSize, layerCount, i );
		if( bugClearEntireArrTex ){
			OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
			DebugTimer4Sample( plan, *pDebugInfoSolidShadowSplitClear, true );
		}
		
		if( i < layerLimit ){
		// render solid content. two different depth offsets for front and back faces are used. double sided always
		// counts as front facing. this way all can be rendered in one go
		renderParamBlock->MapBuffer();
		try{
			renderParamBlock->SetParameterDataMat4x3( deoglSkinShader::erutMatrixV, matrixCamera );
			renderParamBlock->SetParameterDataMat4x4( deoglSkinShader::erutMatrixP, decMatrix() );
			renderParamBlock->SetParameterDataMat4x4( deoglSkinShader::erutMatrixVP, matrixCamera );
			renderParamBlock->SetParameterDataMat3x3( deoglSkinShader::erutMatrixVn, matrixCamera.GetRotationMatrix().Invert() );
			renderParamBlock->SetParameterDataVec4( deoglSkinShader::erutDepthOffset, sl.zscale, sl.zoffset, -sl.zscale, -sl.zoffset );
			
		}catch( const deException & ){
			renderParamBlock->UnmapBuffer();
			throw;
		}
		renderParamBlock->UnmapBuffer();
		
		renderTask.Clear();
		renderTask.SetRenderParamBlock( renderParamBlock );
		
		addToRenderTask.Reset();
		addToRenderTask.SetSkinShaderType( deoglSkinTexture::estComponentShadowOrthogonal );
		addToRenderTask.SetSolid( true );
		addToRenderTask.SetNoShadowNone( true );
		addToRenderTask.SetForceDoubleSided( true );
		
		if( false ){
			const decDVector &fclcampos = plan.GetCameraPosition();
			const decDVector &fclcamview = plan.GetInverseCameraMatrix().TransformView();
			const double fclcamdist = fclcamview * fclcampos + faceCountLimitDistanceNear;
			double fclcompdist;
			int faceCountLimit;
			componentCount = pColList2->GetComponentCount();
			for( c=0; c<componentCount; c++ ){
				const deoglCollideListComponent &clcomponent = *pColList2->GetComponentAt( c );
				component = clcomponent.GetComponent();
				fclcompdist = fclcamview * component->GetMatrix().GetPosition() - fclcamdist;
				
				if( fclcompdist < 0.0 ){
					faceCountLimit = faceCountLimitNear;
					
				}else if( fclcompdist > faceCountLimitRange ){
					faceCountLimit = faceCountLimitFar;
					
				}else{
					faceCountLimit = faceCountLimitNear + ( int )( fclcompdist * faceCountLimitFactor );
				}
				
				if( ! component->GetModel() ){
					continue;
				}
				
				if( component->GetModel()->GetLODAt( clcomponent.GetLODLevel() ).GetFaceCount() <= faceCountLimit ){
					addToRenderTask.AddComponent( clcomponent );
				}
			}
			
		}else{
			addToRenderTask.AddComponents( *pColList2 );
		}
		
		if( renderThread.GetConfiguration().GetDebugSnapshot() == edbgsnapLightSkyShadowRenTask ){
			renderThread.GetLogger().LogInfoFormat( "RenderLightSky: shadow split %i", i );
			renderTask.DebugPrint( renderThread.GetLogger() );
		}
		DebugTimer4Sample( plan, *pDebugInfoSolidShadowSplitTask, true );
		
		renderTask.PrepareForRender( renderThread );
		rengeom.RenderTask( renderTask );
		
		addToRenderTask.SetForceDoubleSided( false );
		
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
		}
		DebugTimer4Sample( plan, *pDebugInfoSolidShadowSplitRender, true );
		DebugTimer3SampleCount( plan, *pDebugInfoSolidShadowSplit, 1, false );
	}
	
	shadowMapper.DropForeignArrayTextures();
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == edbgsnapLightSkySplits ){
		renderThread.GetConfiguration().SetDebugSnapshot( 0 );
	}
	if( renderThread.GetConfiguration().GetDebugSnapshot() == edbgsnapLightSkyShadowRenTask ){
		renderThread.GetConfiguration().SetDebugSnapshot( 0 );
	}
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == edbgsnapLightSkyShadowMap ){
		renderThread.GetDebug().GetDebugSaveTexture().SaveDepthArrayTexture( *( pSolidShadowMap->GetTexture() ), "sky_shadow", true );
		renderThread.GetConfiguration().SetDebugSnapshot( 0 );
	}
	
#if 0
	if(plan.GetFBOTarget()){
		static int c = 0;
		const decDVector &p = plan.GetCameraPosition();
		decString s;
		s.Format("envmap_%g-%g-%g_skyshadow-%d", p.x, p.y, p.z, c++);
		renderThread.GetDebug().GetDebugSaveTexture().SaveDepthArrayTexture(*pSolidShadowMap->GetTexture(), s, true);
	}
#endif
}



void deoglRenderLightSky::UpdateLightParamBlock( deoglLightShader &lightShader,
deoglSPBlockUBO &paramBlock, deoglRenderPlan &plan, deoglRenderPlanSkyLight &planSkyLight ){
	int target;
	
	// get light properties
	const deoglRSkyInstanceLayer &skyLayer = *planSkyLight.GetLayer();
	const float ambientIntensity = skyLayer.GetAmbientIntensity();
	const float lightIntensity = skyLayer.GetLightIntensity();
	decColor lightColor = skyLayer.GetLightColor();
	
	lightColor.r = powf( lightColor.r, OGL_RENDER_GAMMA );
	lightColor.g = powf( lightColor.g, OGL_RENDER_GAMMA );
	lightColor.b = powf( lightColor.b, OGL_RENDER_GAMMA );
	lightColor = TransformColor( plan.GetWorld()->GetLightColorMatrix(), lightColor );
	
	// set values
	paramBlock.MapBuffer();
	try{
		target = lightShader.GetLightUniformTarget( deoglLightShader::elutLightColor );
		if( target != -1 ){
			if( plan.GetOcclusionTracingState() ){
				paramBlock.SetParameterDataVec3( target, lightColor * ( lightIntensity + ambientIntensity ) );
				
			}else{
				paramBlock.SetParameterDataVec3( target, lightColor * lightIntensity );
			}
		}
		
		target = lightShader.GetLightUniformTarget( deoglLightShader::elutLightColorAmbient );
		if( target != -1 ){
			if( plan.GetOcclusionTracingState() ){
				// this one is a bit tricky. occlusion is the percentage of hits over a full
				// sphere. full light intensity is though usually acquired if half the sphere
				// is lit since the other half is typically underneath the surface. the occlusion
				// tracing stores the occlusion factor 1 for 50% occlusion hits and above.
				//paramBlock.SetParameterDataVec3( target, lightColor * ( lightIntensity + ambientIntensity ) * 0.5f );
				//paramBlock.SetParameterDataVec3( target, lightColor * ( lightIntensity + ambientIntensity ) );
				paramBlock.SetParameterDataVec3( target, lightColor * ambientIntensity );
				
			}else{
				paramBlock.SetParameterDataVec3( target, lightColor * ambientIntensity );
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
deoglSPBlockUBO &paramBlock, deoglRenderPlan &plan, deoglRenderPlanSkyLight &planSkyLight,
int shadowMapSize, int passCount ){
	// otherwise set shadow parameters
	const deoglRSkyInstanceLayer &skyLayer = *planSkyLight.GetLayer();
	float pixelSize, noiseScale;
	int target;
	
	// calculate matrices
	const deoglConfiguration &config = GetRenderThread().GetConfiguration();
	
	// set values
	decMatrix matrix[ 4 ];
	float layerBorder[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float scaleZ[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
	
	if( planSkyLight.GetUseShadow() ){
		// planSkyLight is only properly set up if shadow casting is used
		if( 0 < passCount ){
			const deoglRenderPlanSkyLight::sShadowLayer &sl = planSkyLight.GetShadowLayerAt( 0 );
			matrix[ 0 ] = sl.matrix;
			layerBorder[ 0 ] = sl.layerBorder;
			scaleZ[ 0 ] = sl.scale.z;
				if( fabsf( sl.scale.z ) < FLOAT_SAFE_EPSILON ){
					DETHROW( deeInvalidParam );
				}
		}
		if( 1 < passCount ){
			const deoglRenderPlanSkyLight::sShadowLayer &sl = planSkyLight.GetShadowLayerAt( 1 );
			matrix[ 1 ] = sl.matrix;
			layerBorder[ 1 ] = sl.layerBorder;
			scaleZ[ 1 ] = sl.scale.z;
				if( fabsf( sl.scale.z ) < FLOAT_SAFE_EPSILON ){
					DETHROW( deeInvalidParam );
				}
		}
		if( 2 < passCount ){
			const deoglRenderPlanSkyLight::sShadowLayer &sl = planSkyLight.GetShadowLayerAt( 2 );
			matrix[ 2 ] = sl.matrix;
			layerBorder[ 2 ] = sl.layerBorder;
			scaleZ[ 2 ] = sl.scale.z;
				if( fabsf( sl.scale.z ) < FLOAT_SAFE_EPSILON ){
					DETHROW( deeInvalidParam );
				}
		}
		if( 3 < passCount ){
			const deoglRenderPlanSkyLight::sShadowLayer &sl = planSkyLight.GetShadowLayerAt( 3 );
			matrix[ 3 ] = sl.matrix;
			layerBorder[ 3 ] = sl.layerBorder;
			scaleZ[ 3 ] = sl.scale.z;
				if( fabsf( sl.scale.z ) < FLOAT_SAFE_EPSILON ){
					DETHROW( deeInvalidParam );
				}
		}
	}
	
	paramBlock.MapBuffer();
	try{
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutLightView );
		if( target != -1 ){
			const decMatrix matrixL =
				decMatrix::CreateFromQuaternion( skyLayer.GetLightOrientation() )
				* skyLayer.GetMatrix() * plan.GetRefPosCameraMatrix();
			
			paramBlock.SetParameterDataVec3( target, matrixL.TransformView().Normalized() );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadowMatrix1 );
		if( target != -1 ){
			paramBlock.SetParameterDataMat4x4( target, matrix[ 0 ] );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadowMatrix2 );
		if( target != -1 ){
			paramBlock.SetParameterDataMat4x4( target, matrix[ 1 ] );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadowMatrix3 );
		if( target != -1 ){
			paramBlock.SetParameterDataMat4x4( target, matrix[ 2 ] );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadowMatrix4 );
		if( target != -1 ){
			paramBlock.SetParameterDataMat4x4( target, matrix[ 3 ] );
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
	pDebugInfoSolidShadowElements->Clear();
	pDebugInfoSolidShadowOcclusion->Clear();
	pDebugInfoSolidShadowOcclusionStart->Clear();
	pDebugInfoSolidShadowOcclusionVBO->Clear();
	pDebugInfoSolidShadowOcclusionTest->Clear();
	pDebugInfoSolidShadowVBOs->Clear();
	pDebugInfoSolidShadowClear->Clear();
	pDebugInfoSolidShadowSplit->Clear();
	pDebugInfoSolidShadowSplitContent->Clear();
	pDebugInfoSolidShadowSplitLODLevels->Clear();
	pDebugInfoSolidShadowSplitClear->Clear();
	pDebugInfoSolidShadowSplitTask->Clear();
	pDebugInfoSolidShadowSplitRender->Clear();
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
	
	if( pShaderAO ){
		pShaderAO->RemoveUsage();
	}
	
	if( pColList2 ){
		delete pColList2;
	}
	
	if( pDebugInfoSolid ){
		pDebugInfoSolid->FreeReference();
	}
	if( pDebugInfoTransparent ){
		pDebugInfoTransparent->FreeReference();
	}
	
	if( pDebugInfoSolidDetail ){
		pDebugInfoSolidDetail->FreeReference();
	}
	if( pDebugInfoSolidShadow ){
		pDebugInfoSolidShadow->FreeReference();
	}
	if( pDebugInfoSolidShadowElements ){
		pDebugInfoSolidShadowElements->FreeReference();
	}
	if( pDebugInfoSolidShadowOcclusion ){
		pDebugInfoSolidShadowOcclusion->FreeReference();
	}
	if( pDebugInfoSolidShadowOcclusionStart ){
		pDebugInfoSolidShadowOcclusionStart->FreeReference();
	}
	if( pDebugInfoSolidShadowOcclusionVBO ){
		pDebugInfoSolidShadowOcclusionVBO->FreeReference();
	}
	if( pDebugInfoSolidShadowOcclusionTest ){
		pDebugInfoSolidShadowOcclusionTest->FreeReference();
	}
	if( pDebugInfoSolidShadowVBOs ){
		pDebugInfoSolidShadowVBOs->FreeReference();
	}
	if( pDebugInfoSolidShadowClear ){
		pDebugInfoSolidShadowClear->FreeReference();
	}
	if( pDebugInfoSolidShadowSplit ){
		pDebugInfoSolidShadowSplit->FreeReference();
	}
	if( pDebugInfoSolidShadowSplitContent ){
		pDebugInfoSolidShadowSplitContent->FreeReference();
	}
	if( pDebugInfoSolidShadowSplitLODLevels ){
		pDebugInfoSolidShadowSplitLODLevels->FreeReference();
	}
	if( pDebugInfoSolidShadowSplitClear ){
		pDebugInfoSolidShadowSplitClear->FreeReference();
	}
	if( pDebugInfoSolidShadowSplitTask ){
		pDebugInfoSolidShadowSplitTask->FreeReference();
	}
	if( pDebugInfoSolidShadowSplitRender ){
		pDebugInfoSolidShadowSplitRender->FreeReference();
	}
	if( pDebugInfoSolidLight ){
		pDebugInfoSolidLight->FreeReference();
	}
	
	if( pDebugInfoTransparentDetail ){
		pDebugInfoTransparentDetail->FreeReference();
	}
	if( pDebugInfoTransparentLight ){
		pDebugInfoTransparentLight->FreeReference();
	}
}
