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
#include "deoglRenderLightPoint.h"
#include "deoglRenderGI.h"
#include "../deoglRenderGeometry.h"
#include "../deoglRenderOcclusion.h"
#include "../defren/deoglDeferredRendering.h"
#include "../plan/deoglRenderPlanDebug.h"
#include "../plan/deoglRenderPlan.h"
#include "../plan/deoglRenderPlanLight.h"
#include "../task/deoglAddToRenderTask.h"
#include "../task/deoglRenderTask.h"
#include "../task/deoglRenderTaskShader.h"
#include "../task/shared/deoglRenderTaskSharedShader.h"
#include "../../canvas/render/deoglRCanvasView.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../capabilities/deoglCapsTextureFormat.h"
#include "../../collidelist/deoglCollideList.h"
#include "../../collidelist/deoglCollideListComponent.h"
#include "../../collidelist/deoglCollideListLight.h"
#include "../../collidelist/deoglCollideListManager.h"
#include "../../component/deoglRComponent.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../debug/deoglDebugInformation.h"
#include "../../debug/deoglDebugSaveTexture.h"
#include "../../devmode/deoglDeveloperMode.h"
#include "../../extensions/deoglExtensions.h"
#include "../../framebuffer/deoglFramebuffer.h"
#include "../../framebuffer/deoglFramebufferManager.h"
#include "../../gi/deoglGI.h"
#include "../../gi/deoglGIRayCache.h"
#include "../../gi/deoglGIState.h"
#include "../../light/deoglLightBoundaryMap.h"
#include "../../light/deoglRLight.h"
#include "../../light/shader/deoglLightShader.h"
#include "../../light/volume/deoglLightVolume.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTBufferObject.h"
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
#include "../../skin/deoglRSkin.h"
#include "../../skin/deoglSkinTexture.h"
#include "../../skin/shader/deoglSkinShader.h"
#include "../../skin/state/deoglSkinState.h"
#include "../../target/deoglRenderTarget.h"
#include "../../texture/deoglRImage.h"
#include "../../texture/cubemap/deoglCubeMap.h"
#include "../../texture/cubemap/deoglRenderableColorCubeMap.h"
#include "../../texture/cubemap/deoglRenderableDepthCubeMap.h"
#include "../../texture/deoglTextureStageManager.h"
#include "../../texture/texture2d/deoglRenderableDepthTexture.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../texture/texunitsconfig/deoglTexUnitConfig.h"
#include "../../vao/deoglVAO.h"
#include "../../visitors/deoglVolumeShape.h"
#include "../../video/deoglRVideoPlayer.h"
#include "../../world/deoglRWorld.h"
#include "../../world/deoglWorldOctree.h"
#include "../../utils/collision/deoglDCollisionFrustum.h"
#include "../../utils/collision/deoglDCollisionBox.h"
#include "../../utils/collision/deoglDCollisionSphere.h"
#include "../../utils/collision/deoglDCollisionVolume.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/math/decConvexVolume.h>
#include <dragengine/common/math/decConvexVolumeFace.h>
#include <dragengine/common/math/decConvexVolumeList.h>



// Definitions
////////////////

#define OGL_REN_LIG_ZNEAR		0.01
//#define OGL_REN_LIG_ZFAR		2000.0

// light model flags ( bits 3-4 )
// light model default = 0 ( 00|00 )
// light model particle = 1 ( 01|00 )

enum eSPLight{
	splLightPos,
	splLightColor,
	splLightColorAmbient,
	splLightAtten,
	splLightDamp,
	splPosTransform,
	splShadowMatrix,
	splShaStaParams,
	splShaDynParams,
	splShaOpts2,
	splMatrixMVP,
	splMatrixMV
};

enum eSPShadow{
	spsMatrixMVP,
	spsMatrixMV,
	spsMatrixDiffuse,
	spsMaterialGamma,
	spsOptions,
	spsMask
};

enum eSPBoxBoundary{
	spbbQuadParams,
	spbbPosTransform,
	spbbMipMapLevel,
	spbbMatrixRotation,
	spbbInitialMinValue,
	spbbInitialMaxValue,
	spbbClearMinValue,
	spbbClearMaxValue
};



// #define DO_TIMING

#ifdef DO_TIMING
#include <dragengine/common/utils/decTimer.h>
static decTimer dtimer;
static decTimer dtimerTotal;

#define DEBUG_RESET_TIMER \
	dtimer.Reset();
#define DEBUG_PRINT_TIMER(what) \
	GetRenderThread().GetLogger().LogInfoFormat( "PointLight %s = %iys", what, \
		( int )( dtimer.GetElapsedTime() * 1000000.0 ) ); \
	dtimer.Reset();

#define DEBUG_RESET_TIMER_TOTAL \
	dtimerTotal.Reset();
#define DEBUG_PRINT_TIMER_TOTAL \
	GetRenderThread().GetLogger().LogInfoFormat( "PointLight Total = %iys", \
		( int )( dtimerTotal.GetElapsedTime() * 1000000.0 ) ); \
	dtimerTotal.Reset();
// grep -F "PointLight Total" logs_delauncher/delauncherconsole.log | sed -e "s/[^0-9]*\([0-9]\+\)ys/\1/" | sort -n -u
#else
#define DEBUG_RESET_TIMER
#define DEBUG_PRINT_TIMER(what)
#define DEBUG_RESET_TIMER_TOTAL
#define DEBUG_PRINT_TIMER_TOTAL
#endif



// #define DO_LOG_SIZE_CHANGE 1

#ifdef DO_LOG_SIZE_CHANGE
#define LOG_SIZE_CHANGE(s,o,n,l,t) GetRenderThread().GetLogger().LogInfoFormat("Point.%s: Size changed from %d to %d (light=%p fbotarget=%p)", s, o, n, l, t);
#else
#define LOG_SIZE_CHANGE(s,o,n,l,t)
#endif



// Class deoglRenderLightPoint
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderLightPoint::deoglRenderLightPoint( deoglRenderThread &renderThread,
deoglRTRenderers &renderers ) :
deoglRenderLightBase( renderThread ),

pShaderLight( NULL ),
pShaderLightShadow( NULL ),
pShaderLightShadowTransp( NULL ),

pShaderShadowSolid( NULL ),
pShaderShadowHoles( NULL ),
pShaderShadowTransp( NULL ),

pShaderBoxBoundary1( NULL ),
pShaderBoxBoundary2( NULL ),
pShaderOccMap( NULL ),
pShaderOccMapCube( NULL ),

pDebugInfoSolid( NULL ),
pDebugInfoTransparent( NULL ),

pDebugInfoSolidDetail( NULL ),
pDebugInfoSolidShadow( NULL ),
pDebugInfoSolidShadowClear( NULL ),
pDebugInfoSolidShadowClearTransp( NULL ),
pDebugInfoSolidShadowFace( NULL ),
pDebugInfoSolidShadowFaceClear( NULL ),
pDebugInfoSolidShadowFaceTask( NULL ),
pDebugInfoSolidShadowFaceRender( NULL ),
pDebugInfoSolidShadowFaceTranspClear( NULL ),
pDebugInfoSolidShadowFaceTranspTask( NULL ),
pDebugInfoSolidShadowFaceTranspRender( NULL ),
pDebugInfoSolidLight( NULL ),

pDebugInfoTransparentDetail( NULL ),
pDebugInfoTransparentShadow( NULL ),
pDebugInfoTransparentShadowClear( NULL ),
pDebugInfoTransparentShadowClearTransp( NULL ),
pDebugInfoTransparentShadowFace( NULL ),
pDebugInfoTransparentShadowFaceClear( NULL ),
pDebugInfoTransparentShadowFaceTask( NULL ),
pDebugInfoTransparentShadowFaceRender( NULL ),
pDebugInfoTransparentShadowFaceTranspClear( NULL ),
pDebugInfoTransparentShadowFaceTranspTask( NULL ),
pDebugInfoTransparentShadowFaceTranspRender( NULL ),
pDebugInfoTransparentLight( NULL )
{
	const deoglConfiguration &config = renderThread.GetConfiguration();
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	bool useShadowCubeEncodeDepth = config.GetUseShadowCubeEncodeDepth();
	bool useEncodeDepth = config.GetUseEncodeDepth();
	deoglShaderSources *sources;
	deoglShaderDefines defines;
	
	try{
		sources = shaderManager.GetSourcesNamed( "DefRen Light Point" );
		if( useEncodeDepth ){
			defines.AddDefine( "GEOM_ENCODED_DEPTH", "1" );
		}
		pShaderLight = shaderManager.GetProgramWith( sources, defines );
		
		defines.AddDefine( "USE_SHADOW", "1" );
		if( useShadowCubeEncodeDepth ){
			defines.AddDefine( "USE_ENCODED_DEPTH", "1" );
			
		}else{
			defines.AddDefine( "USE_DEPTH_CUBE", "1" );
		}
		//defines.AddDefine( "PCF_9TAP", "1" );
		//defines.AddDefine( "PCF_VARTAP", "1" );
		defines.AddDefine( "PCF_NOISETAP", "1" );
		pShaderLightShadow = shaderManager.GetProgramWith( sources, defines );
		
		defines.AddDefine( "TRANSPARENT_SHADOW", "1" );
		pShaderLightShadowTransp = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		/*
		if( useEncodeDepth ){
			defines.AddDefine( "GEOM_ENCODED_DEPTH", "1" );
		}
		defines.AddDefine( "USE_SHADOW", "1" );
		defines.AddDefine( "USE_DEPTH_CUBE", "1" );
		defines.AddDefine( "PCF_9TAP", "1" );
		defines.AddDefine( "WITH_STATIC_LIGHT", "1" );
		pShaderLightStaticShadow = shamgr.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		*/
		
		//defines.AddDefine( "USE_SHADOW", "1" );
		//defines.AddDefine( "PCF_9TAP", "1" );
		//defines.AddDefine( "NO_AMBIENT", "1" );
		//pShaderLightShadowNoAmbient = shamgr.GetProgramWith( sources, defines );
		
		sources = shaderManager.GetSourcesNamed( "DefRen Shadow Point" );
		if( useShadowCubeEncodeDepth ){
			defines.AddDefine( "DEPTH_AS_COLOR", "1" );
		}
		pShaderShadowSolid = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		if( useShadowCubeEncodeDepth ){
			defines.AddDefine( "DEPTH_AS_COLOR", "1" );
		}
		defines.AddDefine( "DISCARD_HOLES", "1" );
		pShaderShadowHoles = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		if( useShadowCubeEncodeDepth ){
			defines.AddDefine( "DEPTH_AS_COLOR", "1" );
		}
		defines.AddDefine( "TRANSPARENT", "1" );
		pShaderShadowTransp = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		
		
		sources = shaderManager.GetSourcesNamed( "DefRen Light BoxBoundary" );
		
		if( config.GetUseShadowCubeEncodeDepth() ){
			defines.AddDefine( "DECODE_IN_DEPTH", "1" );
		}
		defines.AddDefine( "DEPTH_INPUT", "1" );
		defines.AddDefine( "DEPTH_CUBEMAP", "1" );
		pShaderBoxBoundary1 = shaderManager.GetProgramWith( sources, defines );
		
		defines.AddDefine( "AMBIENT_MAP", "1" );
		pShaderBoxBoundary1Ambient = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		if( config.GetUseShadowCubeEncodeDepth() ){
			defines.AddDefine( "DECODE_IN_DEPTH", "1" );
		}
		pShaderBoxBoundary2 = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		
		
		sources = shaderManager.GetSourcesNamed( "DefRen Occlusion OccMap" );
		renderers.GetOcclusion().AddOccMapDefines( defines );
		defines.AddDefine( "DEPTH_DISTANCE", "1" );
		pShaderOccMap = shaderManager.GetProgramWith( sources, defines );
		pShaderOccMap->EnsureRTSShader();
		pShaderOccMap->GetRTSShader()->SetSPBInstanceIndexBase( 0 );
		defines.RemoveAllDefines();
		
		
		
		const bool bugClearEntireCubeMap = renderThread.GetCapabilities().GetClearEntireCubeMap().Broken();
		const bool useGSRenderCube = renderThread.GetExtensions().SupportsGeometryShader()
			&& ! bugClearEntireCubeMap;
		
		if( useGSRenderCube ){
			sources = shaderManager.GetSourcesNamed( "DefRen Occlusion OccMap Cube" );
			renderers.GetOcclusion().AddOccMapDefines( defines );
			defines.AddDefine( "DEPTH_DISTANCE", "1" );
			
			defines.AddDefine( "GS_RENDER_CUBE", "1" );
			defines.AddDefine( "GS_RENDER_CUBE_CULLING", "1" );
			if( renderThread.GetExtensions().SupportsGSInstancing() ){
				defines.AddDefine( "GS_RENDER_CUBE_INSTANCING", "1" );
			}
			
			pShaderOccMapCube = shaderManager.GetProgramWith( sources, defines );
			pShaderOccMapCube->EnsureRTSShader();
			pShaderOccMapCube->GetRTSShader()->SetSPBInstanceIndexBase( 0 );
			defines.RemoveAllDefines();
		}
		
		
		
		pCubeFaces[ 0 ] = deoglCubeMap::efPositiveX;
		pCubeFaces[ 1 ] = deoglCubeMap::efNegativeX;
		pCubeFaces[ 2 ] = deoglCubeMap::efNegativeY; //deoglCubeMap::efPositiveY;
		pCubeFaces[ 3 ] = deoglCubeMap::efPositiveY; //deoglCubeMap::efNegativeY;
		pCubeFaces[ 4 ] = deoglCubeMap::efPositiveZ;
		pCubeFaces[ 5 ] = deoglCubeMap::efNegativeZ;
		
		
		
		// debug information
		const decColor colorText( 1.0f, 1.0f, 1.0f, 1.0f );
		const decColor colorBgUp( 0.05f, 0.05f, 0.05f, 0.75f );
		const decColor colorBg( 0.0f, 0.0f, 0.25f, 0.75f );
		const decColor colorBgSub( 0.05f, 0.05f, 0.05f, 0.75f );
		const decColor colorBgSub2( 0.1f, 0.1f, 0.1f, 0.75f );
		const decColor colorBgSub3( 0.15f, 0.15f, 0.15f, 0.75f );
		
		pDebugInfoSolid = new deoglDebugInformation( "Point", colorText, colorBgUp );
		pDebugInfoTransparent = new deoglDebugInformation( "Point", colorText, colorBgUp );
		
		
		
		pDebugInfoSolidDetail = new deoglDebugInformation( "Light Point Solid", colorText, colorBg );
		
		pDebugInfoSolidShadow = new deoglDebugInformation( "Shadow", colorText, colorBgSub );
		pDebugInfoSolidDetail->GetChildren().Add( pDebugInfoSolidShadow );
		
		pDebugInfoSolidShadowClear = new deoglDebugInformation( "Clear", colorText, colorBgSub2 );
		pDebugInfoSolidShadow->GetChildren().Add( pDebugInfoSolidShadowClear );
		
		pDebugInfoSolidShadowClearTransp = new deoglDebugInformation( "Clear Transp", colorText, colorBgSub2 );
		pDebugInfoSolidShadow->GetChildren().Add( pDebugInfoSolidShadowClearTransp );
		
		pDebugInfoSolidShadowFace = new deoglDebugInformation( "Face", colorText, colorBgSub2 );
		pDebugInfoSolidShadow->GetChildren().Add( pDebugInfoSolidShadowFace );
		
		pDebugInfoSolidShadowFaceClear = new deoglDebugInformation( "Clear", colorText, colorBgSub3 );
		pDebugInfoSolidShadowFace->GetChildren().Add( pDebugInfoSolidShadowFaceClear );
		
		pDebugInfoSolidShadowFaceTask = new deoglDebugInformation( "Task", colorText, colorBgSub3 );
		pDebugInfoSolidShadowFace->GetChildren().Add( pDebugInfoSolidShadowFaceTask );
		
		pDebugInfoSolidShadowFaceRender = new deoglDebugInformation( "Render", colorText, colorBgSub3 );
		pDebugInfoSolidShadowFace->GetChildren().Add( pDebugInfoSolidShadowFaceRender );
		
		pDebugInfoSolidShadowFaceTranspClear = new deoglDebugInformation( "Transp Clear", colorText, colorBgSub3 );
		pDebugInfoSolidShadowFace->GetChildren().Add( pDebugInfoSolidShadowFaceTranspClear );
		
		pDebugInfoSolidShadowFaceTranspTask = new deoglDebugInformation( "Transp Task", colorText, colorBgSub3 );
		pDebugInfoSolidShadowFace->GetChildren().Add( pDebugInfoSolidShadowFaceTranspTask );
		
		pDebugInfoSolidShadowFaceTranspRender = new deoglDebugInformation( "Transp Render", colorText, colorBgSub3 );
		pDebugInfoSolidShadowFace->GetChildren().Add( pDebugInfoSolidShadowFaceTranspRender );
		
		pDebugInfoSolidLight = new deoglDebugInformation( "Light", colorText, colorBgSub );
		pDebugInfoSolidDetail->GetChildren().Add( pDebugInfoSolidLight );
		
		
		
		pDebugInfoTransparentDetail = new deoglDebugInformation( "Light Point Transp", colorText, colorBg );
		
		pDebugInfoTransparentShadow = new deoglDebugInformation( "Shadow", colorText, colorBgSub );
		pDebugInfoTransparentDetail->GetChildren().Add( pDebugInfoTransparentShadow );
		
		pDebugInfoTransparentShadowClear = new deoglDebugInformation( "Clear", colorText, colorBgSub2 );
		pDebugInfoTransparentShadow->GetChildren().Add( pDebugInfoTransparentShadowClear );
		
		pDebugInfoTransparentShadowClearTransp = new deoglDebugInformation( "Clear Transp", colorText, colorBgSub2 );
		pDebugInfoTransparentShadow->GetChildren().Add( pDebugInfoTransparentShadowClearTransp );
		
		pDebugInfoTransparentShadowFace = new deoglDebugInformation( "Face", colorText, colorBgSub2 );
		pDebugInfoTransparentShadow->GetChildren().Add( pDebugInfoTransparentShadowFace );
		
		pDebugInfoTransparentShadowFaceClear = new deoglDebugInformation( "Clear", colorText, colorBgSub3 );
		pDebugInfoTransparentShadowFace->GetChildren().Add( pDebugInfoTransparentShadowFaceClear );
		
		pDebugInfoTransparentShadowFaceTask = new deoglDebugInformation( "Task", colorText, colorBgSub3 );
		pDebugInfoTransparentShadowFace->GetChildren().Add( pDebugInfoTransparentShadowFaceTask );
		
		pDebugInfoTransparentShadowFaceRender = new deoglDebugInformation( "Render", colorText, colorBgSub3 );
		pDebugInfoTransparentShadowFace->GetChildren().Add( pDebugInfoTransparentShadowFaceRender );
		
		pDebugInfoTransparentShadowFaceTranspClear = new deoglDebugInformation( "Transp Clear", colorText, colorBgSub3 );
		pDebugInfoTransparentShadowFace->GetChildren().Add( pDebugInfoTransparentShadowFaceTranspClear );
		
		pDebugInfoTransparentShadowFaceTranspTask = new deoglDebugInformation( "Transp Task", colorText, colorBgSub3 );
		pDebugInfoTransparentShadowFace->GetChildren().Add( pDebugInfoTransparentShadowFaceTranspTask );
		
		pDebugInfoTransparentShadowFaceTranspRender = new deoglDebugInformation( "Transp Render", colorText, colorBgSub3 );
		pDebugInfoTransparentShadowFace->GetChildren().Add( pDebugInfoTransparentShadowFaceTranspRender );
		
		pDebugInfoTransparentLight = new deoglDebugInformation( "Light", colorText, colorBgSub );
		pDebugInfoTransparentDetail->GetChildren().Add( pDebugInfoTransparentLight );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRenderLightPoint::~deoglRenderLightPoint(){
	pCleanUp();
}



// Rendering
//////////////

void deoglRenderLightPoint::CalculateBoxBoundary( deoglRenderPlanLight &planLight ){
	deoglRLight &light = *planLight.GetLight()->GetLight();
	deoglShadowCaster &shadowCaster = *light.GetShadowCaster();
	deoglSCSolid &scsolid = shadowCaster.GetSolid();
	deoglSCAmbient &scambient = shadowCaster.GetAmbient();
	
	if( ! scsolid.GetStaticCubeMap() ){
		DETHROW( deeInvalidParam );
		// or simply return the full box boundary instead?
	}
	
	deoglCubeMap &shadowmap = *scsolid.GetStaticCubeMap();
	int size = shadowmap.GetSize() >> 1;
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglLightBoundaryMap &boundaryMap = renderThread.GetLightBoundaryMap( size );
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	int mipMapLevel = boundaryMap.GetBaseLevel( size );
// 	int i;
	
	// calculate light matrix
	const float depthScale = 1.0f / shadowCaster.GetStaticScale(); // 1 / f1 = 1 / scale
	const float depthOffset = -shadowCaster.GetStaticOffset() / shadowCaster.GetStaticScale(); // -f2 / f1 = -offset / scale
	const float pixelStep = 2.0f / ( float )shadowmap.GetSize();
	
	decMatrix matrixLRot;
	matrixLRot.a21 = -matrixLRot.a21;
	matrixLRot.a22 = -matrixLRot.a22;
	matrixLRot.a23 = -matrixLRot.a23;
	matrixLRot.a24 = -matrixLRot.a24;
	
	OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
	
	// first down sampling step with implicit position calculation from the shadow map
	OGL_CHECK( renderThread, glViewport( 0, 0, size, size ) );
	OGL_CHECK( renderThread, glScissor( 0, 0, size, size ) );
	
	renderThread.GetFramebuffer().Activate( boundaryMap.GetFBOAt( mipMapLevel ) );
	
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	
	deoglShaderCompiled *shader;
	
	if( scambient.GetStaticCubeMap() ){
		renderThread.GetShader().ActivateShader( pShaderBoxBoundary1Ambient );
		shader = pShaderBoxBoundary1Ambient->GetCompiled();
		
	}else{
		renderThread.GetShader().ActivateShader( pShaderBoxBoundary1 );
		shader = pShaderBoxBoundary1->GetCompiled();
	}
	
	shader->SetParameterFloat( spbbQuadParams, 1.0f, 1.0f, 0.0f, 0.0f );
	shader->SetParameterFloat( spbbPosTransform, depthScale, depthOffset, pixelStep, 1.0f );
	shader->SetParameterMatrix3x3( spbbMatrixRotation, matrixLRot );
	shader->SetParameterFloat( spbbInitialMinValue, 0.0f, 0.0f, 0.0f );
	shader->SetParameterFloat( spbbInitialMaxValue, 0.0f, 0.0f, 0.0f );
	
	const float lightRange = light.GetRange();
	shader->SetParameterFloat( spbbClearMinValue, -lightRange, -lightRange, -lightRange );
	shader->SetParameterFloat( spbbClearMaxValue, lightRange, lightRange, lightRange );
	
	tsmgr.DisableAllStages();
	tsmgr.EnableCubeMap( 2, shadowmap, GetSamplerClampNearest() );
	if( scambient.GetStaticCubeMap() ){
		tsmgr.EnableCubeMap( 3, *scambient.GetStaticCubeMap(), GetSamplerClampNearest() );
	}
	
	OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
	//ogl.LogInfoFormat( "Point BoxBoundary %p Step1 size(%i) mipMaplevel(%i)
	//	depthScale(%f) depthOffset(%f)", &light, size, mipMapLevel, depthScale, depthOffset );
	
	// down sampling to 1x1 using mip map levels
	renderThread.GetShader().ActivateShader( pShaderBoxBoundary2 );
	
	pShaderBoxBoundary2->GetCompiled()->SetParameterFloat( spbbQuadParams, 1.0f, 1.0f, 0.0f, 0.0f );
	
	tsmgr.EnableTexture( 0, *boundaryMap.GetTextureMin(), GetSamplerClampNearest() );
	tsmgr.EnableTexture( 1, *boundaryMap.GetTextureMax(), GetSamplerClampNearest() );
	tsmgr.DisableStage( 2 );
	
	while( size > 1 ){
		size >>= 1;
		mipMapLevel++;
		
		OGL_CHECK( renderThread, glViewport( 0, 0, size, size ) );
		OGL_CHECK( renderThread, glScissor( 0, 0, size, size ) );
		
		renderThread.GetFramebuffer().Activate( boundaryMap.GetFBOAt( mipMapLevel ) );
		
		pShaderBoxBoundary2->GetCompiled()->SetParameterInt( spbbMipMapLevel, mipMapLevel - 1 );
		
		OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
		//ogl.LogInfoFormat( "BoxBoundary %p Step2 size(%i) mipMaplevel(%i)", &light, size, mipMapLevel );
	}
	
	pglBindVertexArray( 0 );
	
	// fetch the result of the 1x1 lod level
	decVector boundaryMin, boundaryMax;
	
	boundaryMap.GetResult( boundaryMin, boundaryMax );
	
	boundaryMin -= decVector( 0.01f, 0.01f, 0.01f ); // just to make sure the box is not too small
	boundaryMax += decVector( 0.01f, 0.01f, 0.01f ); // just to make sure the box is not too small
	
	const decVector cbExtends = ( boundaryMax - boundaryMin ) * 0.5f;
	const decVector cbPosition = ( boundaryMin + boundaryMax ) * 0.5f;
	
	light.SetLightVolumeCropBox( new decShapeBox( cbExtends, cbPosition ) );
	
	/*{
	const decDVector bmin( light.GetMatrix() * boundaryMin );
	const decDVector bmax( light.GetMatrix() * boundaryMax );
	GetRenderThread().GetLogger().LogInfoFormat( "Point BoxBoundary %p min(%g,%g,%g) max(%g,%g,%g)",
		&light, bmin.x, bmin.y, bmin.z, bmax.x, bmax.y, bmax.z );
	}*/
}



void deoglRenderLightPoint::RenderLights( deoglRenderPlan &plan, bool solid, const deoglRenderPlanMasked *mask ){
DEBUG_RESET_TIMER_TOTAL
	const int lightCount = plan.GetLightCount();
	int i;
	
	DebugTimersReset( plan, false );
	
	// DEBUG DEBUG this should NOT be needed
	RestoreFBO( plan );
	// DEBUG DEBUG this should NOT be needed
	
	for( i=0; i<lightCount; i++ ){
		deoglRenderPlanLight &planLight = *plan.GetLightAt( i );
		if( planLight.GetLight()->GetLight()->GetLightType() != deLight::eltPoint ){
			continue;
		}
		
		RenderLight( planLight, solid, mask );
	}
	
	// clean up job
	if( solid ){
		DebugTimer1Sample( plan, *pDebugInfoSolid, true );
		
	}else{
		DebugTimer1Sample( plan, *pDebugInfoTransparent, true );
	}
DEBUG_PRINT_TIMER_TOTAL
}



void deoglRenderLightPoint::RenderLight( deoglRenderPlanLight &planLight, bool solid,
const deoglRenderPlanMasked *mask ){
	// determine what needs to be rendered
	deoglCollideListLight &cllight = *planLight.GetLight();
	if( cllight.IsHiddenByOccQuery() ){
		cllight.SetCulled( true );
	}
	
	deoglRenderPlan &plan = planLight.GetPlan();
	const bool lightGeometry = ! cllight.GetCulled();
	deoglGIState * const giState = ! mask && solid ? plan.GetUpdateGIState() : NULL;
	
	if( ! lightGeometry && ! giState ){
		return;
	}
	
	// prepare for rendering
	deoglRLight &light = *cllight.GetLight();
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglConfiguration &config = renderThread.GetConfiguration();
	const bool useShadowCubeEncodeDepth = config.GetUseShadowCubeEncodeDepth();
	deoglShadowMapper &shadowMapper = renderThread.GetShadowMapper();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglShadowCaster &shadowCaster = *light.GetShadowCaster();
	deoglSCTransparent &sctransp = shadowCaster.GetTransparent();
	deoglSCAmbient &scambient = shadowCaster.GetAmbient();
	deoglRenderPlanDebug * const planDebug = plan.GetDebug();
	deoglSCSolid &scsolid = shadowCaster.GetSolid();
	deoglShadowCaster::eShadowTypes shadowType = shadowCaster.GetShadowType();
	bool useShadow = renderThread.GetConfiguration().GetShadowQuality() != deoglConfiguration::esqOff;
	
	// no shadows if the light wishes so
	if( ! light.GetCastShadows() ){
		useShadow = false;
	}
	
	// it can also happen that the light has nothing to cast shadows with
	// so in this case shadow casting is disabled too as it is faster
	if( shadowType == deoglShadowCaster::estNoShadows ){
		useShadow = false;
	}
	
	// for environment maps we use only static shadows
	/*
	if( useShadow && plan.GetFBOTarget() ){
		switch( shadowType ){
		case deoglShadowCaster::estStaticAndDynamic:
			shadowType = deoglShadowCaster::estStaticOnly;
			break;
			
		case deoglShadowCaster::estDynamicOnly:
			shadowType = deoglShadowCaster::estNoShadows;
			useShadow = false;
			break;
			
		default:
			break;
		}
	}
	*/
	
	// if layer mask restriction is used dynamic only shadows have to be used to filter properly.
	// the logic is this. lights filter scene elements to be included in their shadow maps by
	// matching the element "layer mask" against the "shadow layer mask". if the camera restricts
	// the layer mask this filtering stays correct if all bits of the "shadow layer mask" are
	// covered by the bits of the "camery layer mask".
	// 
	// as a side note it would be also possible for this rule to not apply if not all bits of
	// the "shadow layer mask" match the "camera layer mask". this requires or combining all
	// layer masks of all filtered scene elements. if this combined layer mask does match in
	// all bits the "camera layer mask" then this would be enough to still fullfil the
	// requirement to use the static shadow maps.
	// TODO check if this special filter check should be added or not
	const bool refilterShadow = plan.GetUseLayerMask() && ! light.StaticMatchesCamera( plan.GetLayerMask() );
	
	if( useShadow && refilterShadow ){
		shadowType = deoglShadowCaster::estDynamicOnly;
	}
	
	// determine if we need transparent shadow casting
	bool transparentStaticShadow = false;
	bool transparentDynamicShadow = false;
	
	if( useShadow ){
		//if( ! plan.GetTransparentShadow() ){
			const deoglCollideList &clistStatic = *light.GetStaticCollideList();
			const deoglCollideList &clistDynamic = *light.GetDynamicCollideList();
			int i, count;
			
			count = clistStatic.GetComponentCount();
			for( i=0; i<count; i++ ){
				const deoglRComponent &component = *clistStatic.GetComponentAt( i )->GetComponent();
				if( component.GetSolid() ){
					continue;
				}
				if( ! component.GetSkin() ){
					continue;
				}
				if( ! component.GetSkin()->GetCastTransparentShadow() ){
					continue;
				}
				
				transparentStaticShadow = true;
				break;
			}
			
			count = clistDynamic.GetComponentCount();
			for( i=0; i<count; i++ ){
				const deoglRComponent &component = *clistDynamic.GetComponentAt( i )->GetComponent();
				if( component.GetSolid() ){
					continue;
				}
				if( ! component.GetSkin() ){
					continue;
				}
				if( ! component.GetSkin()->GetCastTransparentShadow() ){
					continue;
				}
				
				transparentDynamicShadow = true;
				break;
			}
		//}
	}
	
	// debug
	if( solid && plan.GetRenderPassNumber() == 1 && planDebug ){
		planDebug->IncrementRenderedLights();
	}
	
	// calculate light volume matrices
	const decDMatrix matrixLP( defren.CreateFrustumDMatrix( 1, 1, DEG2RAD * 90.0f,
		1.0f, OGL_REN_LIG_ZNEAR, light.GetRange() ) );
	
	deoglCubeMap *texSolidDepth1 = NULL;
	deoglCubeMap *texSolidDepth2 = NULL;
	deoglCubeMap *texTranspDepth1 = NULL;
	deoglCubeMap *texTranspColor1 = NULL;
	deoglCubeMap *texTranspDepth2 = NULL;
	deoglCubeMap *texTranspColor2 = NULL;
	deoglCubeMap *texAmbient1 = NULL;
	deoglCubeMap *texAmbient2 = NULL;
	
	if( useShadow ){
		RenderShadows( planLight, solid, matrixLP, transparentStaticShadow,
			transparentDynamicShadow, refilterShadow );
		
		// switch back to our framebuffer
		OGL_CHECK( renderThread, glViewport( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
		OGL_CHECK( renderThread, glScissor( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
		RestoreFBO( plan );
		
		// determine shadow maps to use
		switch( shadowType ){
		case deoglShadowCaster::estStaticOnly:
			texSolidDepth1 = scsolid.GetStaticCubeMap();
			if( transparentStaticShadow ){
				texTranspDepth1 = sctransp.GetStaticShadowCubeMap();
				texTranspColor1 = sctransp.GetStaticColorCubeMap();
			}
			texAmbient1 = scambient.GetStaticCubeMap();
			break;
			
		case deoglShadowCaster::estDynamicOnly:
			if( scsolid.GetDynamicCubeMap() ){
				texSolidDepth1 = scsolid.GetDynamicCubeMap()->GetCubeMap();
				
			}else if( useShadowCubeEncodeDepth ){
				texSolidDepth1 = shadowMapper.GetSolidEncodedDepthCubeMap();
				
			}else{
				texSolidDepth1 = shadowMapper.GetSolidDepthCubeMap();
			}
			
			if( transparentDynamicShadow ){
				if( sctransp.GetDynamicShadowCubeMap() ){
					texTranspDepth1 = sctransp.GetDynamicShadowCubeMap()->GetCubeMap();
					texTranspColor1 = sctransp.GetDynamicColorCubeMap()->GetCubeMap();
					
				}else if( useShadowCubeEncodeDepth ){
					texTranspDepth1 = shadowMapper.GetTransparentEncodedDepthCubeMap();
					texTranspColor1 = shadowMapper.GetTransparentColorCubeMap();
					
				}else{
					texTranspDepth1 = shadowMapper.GetTransparentDepthCubeMap();
					texTranspColor1 = shadowMapper.GetTransparentColorCubeMap();
				}
			}
			
			if( scambient.GetDynamicCubeMap() ){
				texAmbient1 = scambient.GetDynamicCubeMap()->GetCubeMap();
				
			}else{
				texAmbient1 = shadowMapper.GetAmbientCubeMap();
			}
			break;
			
		case deoglShadowCaster::estStaticAndDynamic:
			texSolidDepth1 = scsolid.GetStaticCubeMap();
			texAmbient1 = scambient.GetStaticCubeMap();
			
			if( scsolid.GetDynamicCubeMap() ){
				texSolidDepth2 = scsolid.GetDynamicCubeMap()->GetCubeMap();
				
			}else if( useShadowCubeEncodeDepth ){
				texSolidDepth2 = shadowMapper.GetSolidEncodedDepthCubeMap();
				
			}else{
				texSolidDepth2 = shadowMapper.GetSolidDepthCubeMap();
			}
			
			if( transparentStaticShadow ){
				texTranspDepth1 = sctransp.GetStaticShadowCubeMap();
				texTranspColor1 = sctransp.GetStaticColorCubeMap();
			}
			
			if( transparentDynamicShadow ){
				deoglCubeMap *&depth = transparentStaticShadow ? texTranspDepth2 : texTranspDepth1;
				deoglCubeMap *&color = transparentStaticShadow ? texTranspColor2 : texTranspColor1;
				
				if( sctransp.GetDynamicShadowCubeMap() ){
					depth = sctransp.GetDynamicShadowCubeMap()->GetCubeMap();
					color = sctransp.GetDynamicColorCubeMap()->GetCubeMap();
					
				}else if( useShadowCubeEncodeDepth ){
					depth = shadowMapper.GetTransparentEncodedDepthCubeMap();
					color = shadowMapper.GetTransparentColorCubeMap();
					
				}else{
					depth = shadowMapper.GetTransparentDepthCubeMap();
					color = shadowMapper.GetTransparentColorCubeMap();
				}
			}
			
			if( scambient.GetDynamicCubeMap() ){
				texAmbient2 = scambient.GetDynamicCubeMap()->GetCubeMap();
				
			}else{
				texAmbient2 = shadowMapper.GetAmbientCubeMap();
			}
			break;
			
		default:
			break;
		}
		
		if( solid ){
			DebugTimer2SampleCount( plan, *pDebugInfoSolidShadow, 1, true );
			
		}else{
			DebugTimer2SampleCount( plan, *pDebugInfoTransparentShadow, 1, true );
		}
	}
	
	if( ! mask && solid ){
		// disable ambient if GI is used. uses faster shader but does not prevent
		// rendering ambient maps since they are used for more than shading
		texAmbient1 = NULL;
		texAmbient2 = NULL;
	}
	
DEBUG_RESET_TIMER
	// set states
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
	
	if( cllight.GetCameraInside() ){ // cull front faces, no depth test
		OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
		SetCullMode( ! plan.GetFlipCulling() );
		
	}else{ // cull back faces, depth test
		// opengl can not handle depth depth if depth texture is also assigned for sampling
		// even if not written to. use shader depth compare instead
		//OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
		SetCullMode( plan.GetFlipCulling() );
	}
	
	OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
	if( solid ){
		OGL_CHECK( renderThread, glBlendFunc( GL_ONE, GL_ONE ) );
		
	}else{
		OGL_CHECK( renderThread, glBlendFunc( GL_SRC_ALPHA, GL_ONE ) );
	}
	
	// set shader
	deoglLightShader *lightShader;
	
	if( useShadow ){
		if( texSolidDepth2 ){
			if( texTranspDepth2 ){
				lightShader = light.GetShaderFor( deoglRLight::estSolid2Transp2 );
				
			}else if( texTranspDepth1 ){
				lightShader = light.GetShaderFor( deoglRLight::estSolid2Transp1 );
				
			}else{
				lightShader = light.GetShaderFor( deoglRLight::estSolid2 );
			}
			
		}else{
			if( texTranspDepth1 ){
				if( texAmbient1 ){
					lightShader = light.GetShaderFor( deoglRLight::estSolid1Transp1 );
					
				}else{
					lightShader = light.GetShaderFor( deoglRLight::estSolid1Transp1NoAmbient );
				}
				
			}else{
				if( texAmbient1 ){
					lightShader = light.GetShaderFor( deoglRLight::estSolid1 );
					
				}else{
					lightShader = light.GetShaderFor( deoglRLight::estSolid1NoAmbient );
				}
			}
		}
		
	}else{
		lightShader = light.GetShaderFor( deoglRLight::estNoShadow );
	}
	
	if( ! lightShader ){
		DETHROW( deeInvalidParam );
	}
	
	// set program parameters
	deoglSPBlockUBO * const spbInstance = light.GetInstanceParameterBlock();
	deoglSPBlockUBO * const spbLight = light.GetLightParameterBlock();
	
	if( ! spbLight || ! spbInstance ){
		DETHROW( deeInvalidParam );
	}
	
	UpdateLightParamBlock( *lightShader, *spbLight, planLight );
	
	sShadowDepthMaps shadowDepthMaps;
	shadowDepthMaps.shadow1Solid = texSolidDepth1;
	shadowDepthMaps.shadow1Transp = texTranspDepth1;
	shadowDepthMaps.shadow1TranspColor = texTranspColor1;
	shadowDepthMaps.shadow2Solid = texSolidDepth2;
	shadowDepthMaps.shadow2Transp = texTranspDepth2;
	shadowDepthMaps.shadow2TranspColor = texTranspColor2;
	shadowDepthMaps.shadow1Ambient = texAmbient1;
	shadowDepthMaps.shadow2Ambient = texAmbient2;
	UpdateInstanceParamBlock( *lightShader, *spbInstance, planLight, shadowDepthMaps );
	
	// light geometry
	if( lightGeometry ){
		renderThread.GetShader().ActivateShader( lightShader->GetShader() );
		
		GetRenderThread().GetRenderers().GetLight().GetLightPB()->Activate();
		spbLight->Activate();
		spbInstance->Activate();
		
		ActivateTextures( planLight, *lightShader, shadowDepthMaps );
		
		pglBindVertexArray( light.GetLightVolume()->GetVAO() );
		OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLES, 0, light.GetLightVolume()->GetPointCount() ) );
		pglBindVertexArray( 0 );
		
		if( solid ){
			DebugTimer2SampleCount( plan, *pDebugInfoSolidLight, 1, true );
			
		}else{
			DebugTimer2SampleCount( plan, *pDebugInfoTransparentLight, 1, true );
		}
	}
	
	// GI rays
	if( giState ){
		RestoreFBOGITraceRays( *giState );
		
		lightShader = NULL;
		if( useShadow ){
			if( texSolidDepth2 ){
				lightShader = light.GetShaderFor( deoglRLight::estGIRaySolid2 );
				
			}else{
				lightShader = light.GetShaderFor( deoglRLight::estGIRaySolid1 );
			}
			
		}else{
			lightShader = light.GetShaderFor( deoglRLight::estGIRayNoShadow );
		}
		
		if( lightShader ){
			renderThread.GetShader().ActivateShader( lightShader->GetShader() );
			
			GetRenderThread().GetRenderers().GetLight().GetLightPB()->Activate();
			spbLight->Activate();
			spbInstance->Activate();
			
			ActivateTextures( planLight, *lightShader, shadowDepthMaps );
			
			defren.RenderFSQuadVAO();
		}
	}
DEBUG_PRINT_TIMER( "Render" );
}

void deoglRenderLightPoint::ActivateTextures( deoglRenderPlanLight &planLight, deoglLightShader &shader,
const sShadowDepthMaps &shadowDepthMaps ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglRTDefaultTextures &dt = renderThread.GetDefaultTextures();
	deoglRLight &light = *planLight.GetLight()->GetLight();
	int target;
	
	target = shader.GetTextureTarget( deoglLightShader::ettColorCubemap );
	if( target != -1 ){
		deoglTexUnitConfig tuc;
		
		if( light.GetLightCanvas() ){
			// TODO can not be used as cube-map for the time being
			
		}else if( light.GetUseSkinTexture() ){
			tuc.EnableCubeMapFromChannel( renderThread, *light.GetUseSkinTexture(),
				deoglSkinChannel::ectColorOmnidirCube, light.GetSkinState(),
				light.GetDynamicSkin(), dt.GetEnvMap() );
		}
		
		if( tuc.IsEnabled() ){
			tuc.Apply( renderThread, target );
			
		}else{
			tsmgr.EnableCubeMap( target, *dt.GetEnvMap(), GetSamplerClampLinear() );
		}
	}
	
	target = shader.GetTextureTarget( deoglLightShader::ettColorEquirect );
	if( target != -1 ){
		deoglTexUnitConfig tuc;
		
		if( light.GetLightCanvas() ){
			deoglRenderTarget * const target = light.GetLightCanvas()->GetRenderTarget();
			if( target ){
				tuc.EnableTexture( target->GetTexture(), &GetSamplerClampLinear() );
			}
			
		}else if( light.GetUseSkinTexture() ){
			tuc.EnableTextureFromChannel( renderThread, *light.GetUseSkinTexture(),
				deoglSkinChannel::ectColorOmnidirEquirect, light.GetSkinState(),
				light.GetDynamicSkin(), dt.GetColor() );
		}
		
		if( tuc.IsEnabled() ){
			tuc.Apply( renderThread, target );
			
		}else{
			tsmgr.EnableTexture( target, *dt.GetColor(), GetSamplerClampLinear() );
		}
	}
	
	target = shader.GetTextureTarget( deoglLightShader::ettNoise );
	if( target != -1 ){
		tsmgr.EnableTexture( target, *dt.GetNoise2D(), GetSamplerRepeatNearest() );
	}
	
	target = shader.GetTextureTarget( deoglLightShader::ettShadow1SolidDepth );
	if( target != -1 ){
		if( shadowDepthMaps.shadow1Solid ){
			if( shadowDepthMaps.shadow1Solid->GetFormat()->GetIsDepth() ){
				tsmgr.EnableCubeMap( target, *shadowDepthMaps.shadow1Solid, GetSamplerShadowClampLinear() );
				
			}else{
				tsmgr.EnableCubeMap( target, *shadowDepthMaps.shadow1Solid, GetSamplerClampLinear() );
			}
			
		}else{
			tsmgr.EnableCubeMap( target, *dt.GetShadowCube(), GetSamplerShadowClampLinear() );
		}
	}
	
	target = shader.GetTextureTarget( deoglLightShader::ettShadow1TransparentDepth );
	if( target != -1 ){
		if( shadowDepthMaps.shadow1Transp ){
			if( shadowDepthMaps.shadow1Transp->GetFormat()->GetIsDepth() ){
				tsmgr.EnableCubeMap( target, *shadowDepthMaps.shadow1Transp, GetSamplerShadowClampLinear() );
				
			}else{
				tsmgr.EnableCubeMap( target, *shadowDepthMaps.shadow1Transp, GetSamplerClampLinear() );
			}
			
		}else{
			tsmgr.EnableCubeMap( target, *dt.GetShadowCube(), GetSamplerShadowClampLinear() );
		}
	}
	
	target = shader.GetTextureTarget( deoglLightShader::ettShadow1TransparentColor );
	if( target != -1 ){
		tsmgr.EnableCubeMap( target, shadowDepthMaps.shadow1TranspColor
			? *shadowDepthMaps.shadow1TranspColor : *dt.GetShadowCubeColor(), GetSamplerClampLinear() );
	}
	
	target = shader.GetTextureTarget( deoglLightShader::ettShadow2SolidDepth );
	if( target != -1 ){
		if( shadowDepthMaps.shadow2Solid ){
			if( shadowDepthMaps.shadow2Solid->GetFormat()->GetIsDepth() ){
				tsmgr.EnableCubeMap( target, *shadowDepthMaps.shadow2Solid, GetSamplerShadowClampLinear() );
				
			}else{
				tsmgr.EnableCubeMap( target, *shadowDepthMaps.shadow2Solid, GetSamplerClampLinear() );
			}
			
		}else{
			tsmgr.EnableCubeMap( target, *dt.GetShadowCube(), GetSamplerShadowClampLinear() );
		}
	}
	
	target = shader.GetTextureTarget( deoglLightShader::ettShadow2TransparentDepth );
	if( target != -1 ){
		if( shadowDepthMaps.shadow2Transp ){
			if( shadowDepthMaps.shadow2Transp->GetFormat()->GetIsDepth() ){
				tsmgr.EnableCubeMap( target, *shadowDepthMaps.shadow2Transp, GetSamplerShadowClampLinear() );
				
			}else{
				tsmgr.EnableCubeMap( target, *shadowDepthMaps.shadow2Transp, GetSamplerClampLinear() );
			}
			
		}else{
			tsmgr.EnableCubeMap( target, *dt.GetShadowCube(), GetSamplerShadowClampLinear() );
		}
	}
	
	target = shader.GetTextureTarget( deoglLightShader::ettShadow2TransparentColor );
	if( target != -1 ){
		tsmgr.EnableCubeMap( target, shadowDepthMaps.shadow2TranspColor
			? *shadowDepthMaps.shadow2TranspColor: *dt.GetShadowCubeColor(), GetSamplerClampLinear() );
	}
	
	target = shader.GetTextureTarget( deoglLightShader::ettShadow1Ambient );
	if( target != -1 ){
		tsmgr.EnableCubeMap( target, shadowDepthMaps.shadow1Ambient
			? *shadowDepthMaps.shadow1Ambient : *dt.GetShadowCube(), GetSamplerShadowClampLinear() );
	}
	
	target = shader.GetTextureTarget( deoglLightShader::ettShadow2Ambient );
	if( target != -1 ){
		tsmgr.EnableCubeMap( target, shadowDepthMaps.shadow2Ambient
			? *shadowDepthMaps.shadow2Ambient : *dt.GetShadowCube(), GetSamplerShadowClampLinear() );
	}
	
	target = shader.GetTextureTarget( deoglLightShader::ettLightDepth1 );
	if( target != -1 ){
		tsmgr.EnableCubeMap( target, shadowDepthMaps.shadow1Solid
			? *shadowDepthMaps.shadow1Solid : *dt.GetShadowCube(), GetSamplerClampLinear() );
	}
	
	target = shader.GetTextureTarget( deoglLightShader::ettLightDepth2 );
	if( target != -1 ){
		tsmgr.EnableCubeMap( target, shadowDepthMaps.shadow2Solid
			? *shadowDepthMaps.shadow2Solid : *dt.GetShadowCube(), GetSamplerClampLinear() );
	}
}

void deoglRenderLightPoint::RenderShadows( deoglRenderPlanLight &planLight, bool solid,
const decDMatrix &matrixProjection, bool transparentStaticShadow,
bool transparentDynamicShadow, bool refilterShadow ){
	deoglRenderPlan &plan = planLight.GetPlan();
	deoglRLight &light = *planLight.GetLight()->GetLight();
	deoglRenderThread &renderThread = GetRenderThread();
	deoglShadowMapper &shadowMapper = renderThread.GetShadowMapper();
	deoglShadowCaster &shadowCaster = *light.GetShadowCaster();
	deoglShadowCaster::eShadowTypes shadowType = shadowCaster.GetShadowType();
	deoglSCTransparent &sctransp = shadowCaster.GetTransparent();
	deoglSCAmbient &scambient = shadowCaster.GetAmbient();
	deoglSCSolid &scsolid = shadowCaster.GetSolid();
	bool updateBoxBoundary = false;
	
	// get map sizes as calculated by render plan
	const int staticShadowMapSize = planLight.GetSolidShadowSizeStatic();
	const int dynamicShadowMapSize = solid ?
		planLight.GetSolidShadowSizeDynamic() : planLight.GetTranspShadowSizeDynamic();
	
	const int staticTranspShadowMapSize = planLight.GetTranspShadowSizeStatic();
	const int dynamicTranspShadowMapSize = solid ?
		planLight.GetTranspShadowSizeDynamic() : planLight.GetTranspShadowSizeTransp();
	
	const int staticAmbientMapSize = planLight.GetAmbientShadowSizeStatic();
	const int dynamicAmbientMapSize = solid ?
		planLight.GetAmbientShadowSizeDynamic() : planLight.GetAmbientShadowSizeTransp();
	
	// if layer mask restriction is used dynamic only shadows have to be used to filter properly
	if( refilterShadow ){
		shadowType = deoglShadowCaster::estDynamicOnly;
	}
	
	// static shadow map
	if( shadowType == deoglShadowCaster::estStaticOnly
	||  shadowType == deoglShadowCaster::estStaticAndDynamic ){
		bool requiresUpdate = false;
		
		// check if an update is required. updates can be required due to the following reasons:
		// 1) the texture does not exist
		// 2) the size is less than the requested size
		// the point 2 happens if an environment map has been rendered. in this case a small texture is
		// used for shadow casting. at the regular scene then the texture exists but obviously it is way
		// too small and needs to be recreated. a larger texture than required is not a problem.
		if( scsolid.GetStaticCubeMap() && scambient.GetStaticCubeMap() ){
			if( ( plan.GetFBOTarget() && staticShadowMapSize > scsolid.GetStaticCubeMap()->GetSize() )
			|| ( ! plan.GetFBOTarget() && staticShadowMapSize != scsolid.GetStaticCubeMap()->GetSize() ) ){
				LOG_SIZE_CHANGE("scsolid.static", staticShadowMapSize,
					scsolid.GetStaticCubeMap()->GetSize(), &light, plan.GetFBOTarget())
				scsolid.DropStatic();
				requiresUpdate = true;
			}
			if( ( plan.GetFBOTarget() && staticAmbientMapSize > scambient.GetStaticCubeMap()->GetSize() )
			|| ( ! plan.GetFBOTarget() && staticAmbientMapSize != scambient.GetStaticCubeMap()->GetSize() ) ){
				LOG_SIZE_CHANGE("scambient.static", staticAmbientMapSize,
					scambient.GetStaticCubeMap()->GetSize(), &light, plan.GetFBOTarget())
				scambient.DropStatic();
				requiresUpdate = true;
			}
			
		}else{
			requiresUpdate = true;
		}
		
		if( transparentStaticShadow ){
			if( sctransp.GetStaticShadowCubeMap() ){
				if( ( plan.GetFBOTarget() && staticTranspShadowMapSize > sctransp.GetStaticShadowCubeMap()->GetSize() )
				|| ( ! plan.GetFBOTarget() && staticTranspShadowMapSize != sctransp.GetStaticShadowCubeMap()->GetSize() ) ){
					LOG_SIZE_CHANGE("sctransp.static", staticTranspShadowMapSize,
						sctransp.GetStaticShadowCubeMap()->GetSize(), &light, plan.GetFBOTarget())
					sctransp.DropStatic();
					requiresUpdate = true;
				}
				
			}else{
				requiresUpdate = true;
			}
			
			if( sctransp.GetStaticColorCubeMap() ){
				if( ( plan.GetFBOTarget() && staticTranspShadowMapSize > sctransp.GetStaticColorCubeMap()->GetSize() )
				|| ( ! plan.GetFBOTarget() && staticTranspShadowMapSize != sctransp.GetStaticColorCubeMap()->GetSize() ) ){
					LOG_SIZE_CHANGE("sctransp.staticColor", staticTranspShadowMapSize,
						sctransp.GetStaticShadowCubeMap()->GetSize(), &light, plan.GetFBOTarget())
					sctransp.DropStatic();
					requiresUpdate = true;
				}
				
			}else{
				requiresUpdate = true;
			}
		}
		
		if( requiresUpdate ){
			//ogl.LogInfoFormat( "RenderLightPoint: light %p: render static shadow cube map.", light.GetLight() );
			
DEBUG_RESET_TIMER
			// TODO for static shadow maps using always lod level 0 is enough. lod levels are supposed to keep the
			// shape of the object as well as possible thus the shadow error is small. on a distance where the
			// rendered lod level can differ from the static shadow casting lod level this is hard to spot anyways
			shadowMapper.SetForeignSolidDepthCubeMap( scsolid.ObtainStaticCubeMapWithSize( staticShadowMapSize ) );
			if( transparentStaticShadow ){
				shadowMapper.SetForeignTransparentDepthCubeMap( sctransp.ObtainStaticShadowCubeMapWithSize( staticTranspShadowMapSize ) );
				shadowMapper.SetForeignTransparentColorCubeMap( sctransp.ObtainStaticColorCubeMapWithSize( staticTranspShadowMapSize ) );
			}
			
			RenderShadowMaps( planLight, matrixProjection, shadowMapper, light.GetStaticCollideList(), NULL,
				staticShadowMapSize, staticTranspShadowMapSize, transparentStaticShadow,
				shadowCaster.GetStaticScale(), shadowCaster.GetStaticOffset(), solid );
			
			shadowMapper.DropForeignCubeMaps();
			
			/*
			decString text;
			text.Format( "point_shadow_solid_depth_%p", light.GetLight() );
			renderThread.GetDebug().GetDebugSaveTexture().SaveCubeMap( *scsolid.GetShadowCubeMap(), text.GetString(), false, false );
			*/
			/*
			if( transparentStaticShadow ){
				text.Format( "point_shadow_transparent_depth_%p", light.GetLight() );
				renderThread.GetDebug().GetDebugSaveTexture().SaveCubeMap( *sctransp.GetShadowCubeMap(), text.GetString(), false, false );
				text.Format( "point_shadow_transparent_color_%p", light.GetLight() );
				renderThread.GetDebug().GetDebugSaveTexture().SaveCubeMap( *sctransp.GetColorCubeMap(), text.GetString(), false, false );
			}
			*/
			shadowMapper.SetForeignAmbientCubeMap( scambient.ObtainStaticCubeMapWithSize( staticAmbientMapSize ) );
			RenderAmbientMap( planLight, matrixProjection, shadowMapper, light.GetStaticCollideList(), NULL,
				staticAmbientMapSize, shadowCaster.GetStaticScale(), shadowCaster.GetStaticOffset() );
			shadowMapper.DropForeignAmbientCubeMaps();
			shadowMapper.DropForeignAmbientTextures();
			
			if( ! light.GetLightVolumeCropBox() ){
				updateBoxBoundary = true;
			}
DEBUG_PRINT_TIMER( "Shadow Static" );
		}
		
		scsolid.ResetLastUseStatic();
		if( transparentStaticShadow ){
			sctransp.ResetLastUseStatic();
		}
		scambient.ResetLastUseStatic();
		
		light.ShadowCasterRequiresPrepare();
	}
// 		renderThread.GetLogger().LogInfoFormat("PointLight: shadow=%d static=%d dynamic=%d ubb=%d lvcb=%p",
// 			shadowType, light.GetStaticCollideList()->GetComponentCount(),
// 			light.GetDynamicCollideList()->GetComponentCount(), updateBoxBoundary,
// 			light.GetLightVolumeCropBox());
	
	// dynamic shadow map with transparency if required
	if( shadowType == deoglShadowCaster::estDynamicOnly
	||  shadowType == deoglShadowCaster::estStaticAndDynamic ){
		const deoglCollideList *clist1 = NULL;
		const deoglCollideList *clist2 = NULL;
		
		if( refilterShadow ){
			const decLayerMask &layerMask = plan.GetLayerMask();
			pCollideList.Clear();
			
			clist1 = light.GetStaticCollideList();
			int i, count = clist1->GetComponentCount();
			for( i=0; i<count; i++ ){
				deoglRComponent * const component = clist1->GetComponentAt( i )->GetComponent();
				if( component->GetLayerMask().IsEmpty() || layerMask.Matches( component->GetLayerMask() ) ){
					pCollideList.AddComponent( component );
				}
			}
			
			clist1 = light.GetDynamicCollideList();
			count = clist1->GetComponentCount();
			for( i=0; i<count; i++ ){
				deoglRComponent * const component = clist1->GetComponentAt( i )->GetComponent();
				if( component->GetLayerMask().IsEmpty() || layerMask.Matches( component->GetLayerMask() ) ){
					pCollideList.AddComponent( component );
				}
			}
			
			clist1 = &pCollideList;
			
		}else if( shadowType == deoglShadowCaster::estDynamicOnly ){
			clist1 = light.GetStaticCollideList();
			clist2 = light.GetDynamicCollideList();
			
		}else{
			clist1 = light.GetDynamicCollideList();
		}
		
		/*
		// version 1. render using dynamic resolution first then copy to transparent resolution
		// this version requires a copy/downsample shader and has to be able to copy with
		// transparent resolution being any scale of 1/1, 1/2, 1/4 or more of the dynamic size
		if( ! scsolid.GetDynamicCubeMap() ){
			RenderShadowMaps( plan, light, matrixProjection, shadowMapper, clist1, clist2,
				dynamicShadowMapSize, dynamicTranspShadowMapSize, transparentDynamicShadow,
				shadowCaster.GetStaticScale(), shadowCaster.GetStaticOffset(), solid );
			
			if( plan.GetHasTransparency() ){
				// TODO downsample shadow map into scsolid dynamic
			}
		}
		*/
		
		// version 2. in solid case use dynamic resolution. in transparent case use transparent
		// resolution. if only one transparency layer is present use regular shadow mapper
		// rendering. if more than one transparency layer is present render the maps at layer 0
		if( solid ){
DEBUG_RESET_TIMER
			RenderShadowMaps( planLight, matrixProjection, shadowMapper, clist1, clist2,
				dynamicShadowMapSize, dynamicTranspShadowMapSize, transparentDynamicShadow,
				shadowCaster.GetStaticScale(), shadowCaster.GetStaticOffset(), solid );
DEBUG_PRINT_TIMER( "Shadow Dynamic Solid" );
			
		}else if( ! scsolid.GetDynamicCubeMap() ){
DEBUG_RESET_TIMER
			shadowMapper.SetForeignSolidDepthCubeMap( scsolid.ObtainDynamicCubeMapWithSize(
				planLight.GetTranspShadowSizeDynamic() )->GetCubeMap() );
			
			if( transparentDynamicShadow ){
				shadowMapper.SetForeignTransparentDepthCubeMap( sctransp.
					ObtainDynamicShadowCubeMapWithSize( dynamicTranspShadowMapSize )->GetCubeMap() );
				shadowMapper.SetForeignTransparentColorCubeMap( sctransp.
					ObtainDynamicColorCubeMapWithSize( dynamicTranspShadowMapSize )->GetCubeMap() );
			}
			
			RenderShadowMaps( planLight, matrixProjection, shadowMapper, clist1, clist2,
				dynamicShadowMapSize, dynamicTranspShadowMapSize, transparentDynamicShadow,
				shadowCaster.GetStaticScale(), shadowCaster.GetStaticOffset(), solid );
			
			shadowMapper.DropForeignCubeMaps();
			shadowMapper.DropForeignTextures();
DEBUG_PRINT_TIMER( "Shadow Dynamic Transparent" );
		}
		
		// ambient map
		if( solid ){
			RenderAmbientMap( planLight, matrixProjection, shadowMapper, clist1, clist2,
				dynamicAmbientMapSize, shadowCaster.GetStaticScale(), shadowCaster.GetStaticOffset() );
			
		}else if( ! scambient.GetDynamicCubeMap() ){
			shadowMapper.SetForeignAmbientCubeMap( scambient.ObtainDynamicCubeMapWithSize(
				dynamicAmbientMapSize )->GetCubeMap() );
			RenderAmbientMap( planLight, matrixProjection, shadowMapper, clist1, clist2,
				dynamicAmbientMapSize, shadowCaster.GetStaticScale(), shadowCaster.GetStaticOffset() );
			shadowMapper.DropForeignAmbientCubeMaps();
			shadowMapper.DropForeignAmbientTextures();
		}
	}
	
	// update box boundary if required
	if( updateBoxBoundary ){
DEBUG_RESET_TIMER
		CalculateBoxBoundary( planLight );
DEBUG_PRINT_TIMER( "Boundary Box" );
	}
}



void deoglRenderLightPoint::ClearCubeMap( deoglShadowMapper &shadowMapper, int shadowMapSize ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglConfiguration &config = renderThread.GetConfiguration();
	bool useShadowCubeEncodeDepth = config.GetUseShadowCubeEncodeDepth();
	int cmf;
	
	if( useShadowCubeEncodeDepth ){
		OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
		
	}else{
		OGL_CHECK( renderThread, glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ) );
	}
	
	OGL_CHECK( renderThread, glDepthMask( GL_TRUE ) );
	
	const GLfloat clearColor[ 4 ] = { 1.0f, 0.0f, 0.0f, 0.0f };
	const GLfloat clearDepth = ( GLfloat )1.0; // point light uses linear depth
	
	for( cmf=0; cmf<6; cmf++ ){
		shadowMapper.ActivateSolidCubeMapFace( shadowMapSize, pCubeFaces[ cmf ] );
		
		OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
		if( useShadowCubeEncodeDepth ){
			OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
		}
	}
}

bool HACK1 = false;

void deoglRenderLightPoint::RenderShadowMaps( deoglRenderPlanLight &planLight,
const decDMatrix &matrixProjection, deoglShadowMapper &shadowMapper, const deoglCollideList *clist1,
const deoglCollideList *clist2, int solidShadowMapSize, int transpShadowMapSize, bool withTransparent,
float shadowScale, float shadowOffset, bool debugSolid ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglAddToRenderTask &addToRenderTask = renderThread.GetRenderers().GetLight().GetAddToRenderTask();
	deoglRenderTask &renderTask = renderThread.GetRenderers().GetLight().GetRenderTask();
	deoglRenderGeometry &rengeom = renderThread.GetRenderers().GetGeometry();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	const deoglConfiguration &config = renderThread.GetConfiguration();
	deoglRLight &light = *planLight.GetLight()->GetLight();
	deoglRenderPlan &plan = planLight.GetPlan();
	decMatrix matrixCamera;
	
	DebugTimer3Reset( plan, false );
	
	// the problem with regular depth offset is that it is focused on the resolution of
	// the depth texture. while this is correct in general it has the problem that the
	// real distance depends on the view range of the texture. if the view range is large
	// the distance between possible depth values is reasonably large. but if the range
	// is small the distance between depth values gets very small. it is then so small
	// the difference becomes too small in real distances causing flickering to appear.
	// and if the offset is increased to counter this with large view range the real
	// offset turns way too large. thus the offset has to be chosen depending on the
	// resolution of the depth texture but using the desired real world distance shift
	// in relation to the view range as measurement. this guarantees the shift matches
	// the desired real world distance and thus is constant nullifying both problems.
	
	// Epsylon version
	const float smOffsetScale = config.GetDistShadowScale();
	const float smOffsetBias = config.GetDistShadowBias() * shadowScale;
	
	bool useShadowCubeEncodeDepth = config.GetUseShadowCubeEncodeDepth();
	const decVector lightPosition( light.GetMatrix().GetPosition()
		- plan.GetWorld()->GetReferencePosition() );
	int cmf;
	
	// calculate object render cube face special parameter. optimizes rendering by skipping
	// object faces on cube map faces they are not visible on
	const decDVector cubePosition( light.GetMatrix().GetPosition() );
	if( clist1 ){
		clist1->UpdateCubeFaceMasks( cubePosition );
	}
	if( clist2 ){
		clist2->UpdateCubeFaceMasks( cubePosition );
	}
	
	// configuration
	const bool bugClearEntireCubeMap = renderThread.GetCapabilities().GetClearEntireCubeMap().Broken();
	const bool useGSRenderCube = renderThread.GetExtensions().SupportsGeometryShader()
		&& ! bugClearEntireCubeMap;
	
	// setup render parameters
	deoglSPBlockUBO * const renderParamBlock = useGSRenderCube
		? renderThread.GetRenderers().GetLight().GetShadowCubePB()
		: renderThread.GetRenderers().GetLight().GetShadowPB();
	
	if( useGSRenderCube ){
		// NOTE Y axis is flipped compared to opengl. pCubeFaces takes care of this
		renderParamBlock->MapBuffer();
		try{
			renderParamBlock->SetParameterDataVec2( deoglSkinShader::erutDepthTransform,
				shadowScale, shadowOffset );
			renderParamBlock->SetParameterDataVec4( deoglSkinShader::erutDepthOffset,
				smOffsetScale, smOffsetBias, -smOffsetScale, -smOffsetBias );
			renderParamBlock->SetParameterDataMat4x4( deoglSkinShader::erutMatrixP,
				matrixProjection );
			
			for( cmf=0; cmf<6; cmf++ ){
				deoglCubeMap::CreateMatrixForFace( matrixCamera, lightPosition, pCubeFaces[ cmf ] );
				
				renderParamBlock->SetParameterDataArrayMat4x3(
					deoglSkinShader::erutMatrixV, cmf, matrixCamera );
				renderParamBlock->SetParameterDataArrayMat4x4(
					deoglSkinShader::erutMatrixVP, cmf, matrixCamera * matrixProjection );
				renderParamBlock->SetParameterDataArrayMat3x3(
					deoglSkinShader::erutMatrixVn, cmf, matrixCamera.GetRotationMatrix().Invert() );
			}
			
		}catch( const deException & ){
			renderParamBlock->UnmapBuffer();
			throw;
		}
		renderParamBlock->UnmapBuffer();
	}
	
	// set states
	OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDepthFunc( GL_LEQUAL ) ); // point light uses linear depth
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
	SetCullMode( false );
	
	if( useShadowCubeEncodeDepth ){
		OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
		
	}else{
		OGL_CHECK( renderThread, glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ) );
	}
	
	OGL_CHECK( renderThread, glDepthMask( GL_TRUE ) );
	
	if( pglClipControl && defren.GetUseInverseDepth() ){
		pglClipControl( GL_LOWER_LEFT,  GL_NEGATIVE_ONE_TO_ONE ); // reset, point light uses linear depth
	}
	
	addToRenderTask.Reset();
	addToRenderTask.SetSolid( true );
	addToRenderTask.SetNoShadowNone( true );
	
	if( useGSRenderCube ){
		addToRenderTask.SetUseSpecialParamBlock( true );
		addToRenderTask.SetSkinShaderType( deoglSkinTexture::estComponentShadowDistanceCube );
		
	}else{
		addToRenderTask.SetSkinShaderType( deoglSkinTexture::estComponentShadowDistance );
	}
	
	const GLfloat clearColor[ 4 ] = { 1.0f, 0.0f, 0.0f, 0.0f };
	const GLfloat clearDepth = ( GLfloat )1.0f; // point light uses linear depth
	
	// render the solid shadow cube map
	if( ! bugClearEntireCubeMap ){
		shadowMapper.ActivateSolidCubeMap( solidShadowMapSize );
		OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
		
		if( debugSolid ){
			DebugTimer3Sample( plan, *pDebugInfoSolidShadowClear, true );
			
		}else{
			DebugTimer3Sample( plan, *pDebugInfoTransparentShadowClear, true );
		}
	}
	
	if( useGSRenderCube ){
		// cube map activate already by clear
		
		renderTask.Clear();
		renderTask.SetRenderParamBlock( renderParamBlock );
		renderTask.SetUseSPBInstanceFlags( true );
		renderTask.SetForceDoubleSided( true );
		
		addToRenderTask.SetSolid( true );
		if( clist1 ){
			addToRenderTask.AddComponents( *clist1 );
		}
		if( clist2 ){
			addToRenderTask.AddComponents( *clist2 );
		}
		
		if( debugSolid ){
			DebugTimer4Sample( plan, *pDebugInfoSolidShadowFaceTask, true );
			
		}else{
			DebugTimer4Sample( plan, *pDebugInfoTransparentShadowFaceTask, true );
		}
		
		renderTask.PrepareForRender();
// 			renderThread.GetLogger().LogInfo( "RenderLightPoint: render task" );
// 			renderTask.DebugPrint( renderThread.GetLogger() );
		rengeom.RenderTask( renderTask );
		
		renderTask.SetForceDoubleSided( false );
		
		if( debugSolid ){
			DebugTimer4Sample( plan, *pDebugInfoSolidShadowFaceRender, true );
			DebugTimer3SampleCount( plan, *pDebugInfoSolidShadowFace, 1, false );
			
		}else{
			DebugTimer4Sample( plan, *pDebugInfoTransparentShadowFaceRender, true );
			DebugTimer3SampleCount( plan, *pDebugInfoTransparentShadowFace, 1, false );
		}
		
	}else{
		for( cmf=0; cmf<6; cmf++ ){
			deoglCubeMap::CreateMatrixForFace( matrixCamera, lightPosition, cmf );
			
			renderParamBlock->MapBuffer();
			try{
				renderParamBlock->SetParameterDataVec2( deoglSkinShader::erutDepthTransform,
					shadowScale, shadowOffset );
				renderParamBlock->SetParameterDataVec4( deoglSkinShader::erutDepthOffset,
					smOffsetScale, smOffsetBias, -smOffsetScale, -smOffsetBias );
				renderParamBlock->SetParameterDataMat4x4( deoglSkinShader::erutMatrixP,
					matrixProjection );
				
				renderParamBlock->SetParameterDataMat4x3(
					deoglSkinShader::erutMatrixV, matrixCamera );
				renderParamBlock->SetParameterDataMat4x4(
					deoglSkinShader::erutMatrixVP, matrixCamera * matrixProjection );
				renderParamBlock->SetParameterDataMat3x3(
					deoglSkinShader::erutMatrixVn, matrixCamera.GetRotationMatrix().Invert() );
				
			}catch( const deException & ){
				renderParamBlock->UnmapBuffer();
				throw;
			}
			renderParamBlock->UnmapBuffer();
			
			shadowMapper.ActivateSolidCubeMapFace( solidShadowMapSize, pCubeFaces[ cmf ] );
			
			if( bugClearEntireCubeMap ){
				OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
				OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
				
				if( debugSolid ){
					DebugTimer4Sample( plan, *pDebugInfoSolidShadowFaceClear, true );
					
				}else{
					DebugTimer4Sample( plan, *pDebugInfoTransparentShadowFaceClear, true );
				}
			}
			
			renderTask.Clear();
			renderTask.SetRenderParamBlock( renderParamBlock );
			renderTask.SetForceDoubleSided( true );
			
			addToRenderTask.SetSolid( true );
			addToRenderTask.SetFilterCubeFace( cmf );
			if( clist1 ){
				addToRenderTask.AddComponents( *clist1 );
			}
			if( clist2 ){
				addToRenderTask.AddComponents( *clist2 );
			}
			
			if( debugSolid ){
				DebugTimer4Sample( plan, *pDebugInfoSolidShadowFaceTask, true );
				
			}else{
				DebugTimer4Sample( plan, *pDebugInfoTransparentShadowFaceTask, true );
			}
			
			renderTask.PrepareForRender();
			rengeom.RenderTask( renderTask );
			
			renderTask.SetForceDoubleSided( false );
			
			if( debugSolid ){
				DebugTimer4Sample( plan, *pDebugInfoSolidShadowFaceRender, true );
				DebugTimer3SampleCount( plan, *pDebugInfoSolidShadowFace, 1, false );
				
			}else{
				DebugTimer4Sample( plan, *pDebugInfoTransparentShadowFaceRender, true );
				DebugTimer3SampleCount( plan, *pDebugInfoTransparentShadowFace, 1, false );
			}
		}
	}
	
	// render the transparent shadow cube maps if required
	if( withTransparent ){
		OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
		
		addToRenderTask.SetSolid( false );
		
		if( ! bugClearEntireCubeMap ){
			shadowMapper.ActivateTransparentCubeMap( transpShadowMapSize );
			OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
			OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
			
			if( debugSolid ){
				DebugTimer3Sample( plan, *pDebugInfoSolidShadowClearTransp, true );
				
			}else{
				DebugTimer3Sample( plan, *pDebugInfoTransparentShadowClearTransp, true );
			}
		}
		
		if( useGSRenderCube ){
			// cube map activate already by clear
			
			renderTask.Clear();
			renderTask.SetRenderParamBlock( renderParamBlock );
			
			if( clist1 ){
				addToRenderTask.AddComponents( *clist1 );
			}
			if( clist2 ){
				addToRenderTask.AddComponents( *clist2 );
			}
			
			if( debugSolid ){
				DebugTimer4Sample( plan, *pDebugInfoSolidShadowFaceTranspTask, true );
				
			}else{
				DebugTimer4Sample( plan, *pDebugInfoTransparentShadowFaceTranspTask, true );
			}
			
			renderTask.PrepareForRender();
			rengeom.RenderTask( renderTask );
			
			if( debugSolid ){
				DebugTimer4Sample( plan, *pDebugInfoSolidShadowFaceTranspRender, true );
				DebugTimer3Sample( plan, *pDebugInfoSolidShadowFace, false ); // because we added 1 already during solid
				
			}else{
				DebugTimer4Sample( plan, *pDebugInfoTransparentShadowFaceTranspRender, true );
				DebugTimer3Sample( plan, *pDebugInfoTransparentShadowFace, false ); // because we added 1 already during solid
			}
			
		}else{
			for( cmf=0; cmf<6; cmf++ ){
				deoglCubeMap::CreateMatrixForFace( matrixCamera, lightPosition, cmf );
				
				renderParamBlock->MapBuffer();
				try{
					renderParamBlock->SetParameterDataVec2( deoglSkinShader::erutDepthTransform,
						shadowScale, shadowOffset );
					renderParamBlock->SetParameterDataVec4( deoglSkinShader::erutDepthOffset,
						smOffsetScale, smOffsetBias, -smOffsetScale, -smOffsetBias );
					renderParamBlock->SetParameterDataMat4x4( deoglSkinShader::erutMatrixP,
						matrixProjection );
						
					renderParamBlock->SetParameterDataMat4x3(
						deoglSkinShader::erutMatrixV, matrixCamera );
					renderParamBlock->SetParameterDataMat4x4(
						deoglSkinShader::erutMatrixVP, matrixCamera * matrixProjection );
					
				}catch( const deException & ){
					renderParamBlock->UnmapBuffer();
					throw;
				}
				renderParamBlock->UnmapBuffer();
				
				shadowMapper.ActivateTransparentCubeMapFace( transpShadowMapSize, pCubeFaces[ cmf ] );
				if( bugClearEntireCubeMap ){
					OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
					OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
					
					if( debugSolid ){
						DebugTimer4Sample( plan, *pDebugInfoSolidShadowFaceTranspClear, true );
						
					}else{
						DebugTimer4Sample( plan, *pDebugInfoTransparentShadowFaceTranspClear, true );
					}
				}
				
				renderTask.Clear();
				renderTask.SetRenderParamBlock( renderParamBlock );
				
				if( clist1 ){
					addToRenderTask.AddComponents( *clist1 );
				}
				if( clist2 ){
					addToRenderTask.AddComponents( *clist2 );
				}
				
				if( debugSolid ){
					DebugTimer4Sample( plan, *pDebugInfoSolidShadowFaceTranspTask, true );
					
				}else{
					DebugTimer4Sample( plan, *pDebugInfoTransparentShadowFaceTranspTask, true );
				}
				
				renderTask.PrepareForRender();
				rengeom.RenderTask( renderTask );
				
				if( debugSolid ){
					DebugTimer4Sample( plan, *pDebugInfoSolidShadowFaceTranspRender, true );
					DebugTimer3Sample( plan, *pDebugInfoSolidShadowFace, false ); // because we added 1 already during solid
					
				}else{
					DebugTimer4Sample( plan, *pDebugInfoTransparentShadowFaceTranspRender, true );
					DebugTimer3Sample( plan, *pDebugInfoTransparentShadowFace, false ); // because we added 1 already during solid
				}
			}
		}
	}
}

void deoglRenderLightPoint::RenderAmbientMap( deoglRenderPlanLight &planLight,
const decDMatrix &matrixProjection, deoglShadowMapper &shadowMapper, const deoglCollideList *clist1,
const deoglCollideList *clist2, int ambientMapSize, float shadowScale, float shadowOffset ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglAddToRenderTask &addToRenderTask = renderThread.GetRenderers().GetLight().GetAddToRenderTask();
	deoglRenderTask &renderTask = renderThread.GetRenderers().GetLight().GetRenderTask();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglRenderGeometry &rengeom = renderThread.GetRenderers().GetGeometry();
	const deoglConfiguration &config = renderThread.GetConfiguration();
	deoglRLight &light = *planLight.GetLight()->GetLight();
	deoglRenderPlan &plan = planLight.GetPlan();
//	const float smOffsetScale = config.GetDistShadowScale(); //config.GetShadowMapOffsetScale();
	//const float smOffsetBias = config.GetDistShadowBias() / ( float )( ( 1 << 24 ) - 1 ); // config.GetShadowMapOffsetBias();
//	const float smOffsetBias = config.GetDistShadowBias() / ( float )( ( 1 << 16 ) - 1 ); // config.GetShadowMapOffsetBias();
	decMatrix matrixCamera;
	
	bool useShadowCubeEncodeDepth = config.GetUseShadowCubeEncodeDepth();
	const decVector lightPosition = ( light.GetMatrix().GetPosition() - plan.GetWorld()->GetReferencePosition() ).ToVector();
	int cmf;
	
	// the component VBOs have been update already by RenderShadowMaps. also the cube face
	// visibility has been calculated and the special parameter block updated.
	//UpdateComponentVBO( clist );
	
	// configuration
	const bool bugClearEntireCubeMap = renderThread.GetCapabilities().GetClearEntireCubeMap().Broken();
	const bool useGSRenderCube = renderThread.GetExtensions().SupportsGeometryShader()
		&& ! bugClearEntireCubeMap;
	
	// setup render parameters
	deoglSPBlockUBO * const renderParamBlock =
		renderThread.GetRenderers().GetLight().GetOccMapPB();
	
	if( useGSRenderCube ){
		// NOTE Y axis is flipped compared to opengl. pCubeFaces takes care of this
		renderParamBlock->MapBuffer();
		try{
			for( cmf=0; cmf<6; cmf++ ){
				deoglCubeMap::CreateMatrixForFace( matrixCamera, lightPosition, pCubeFaces[ cmf ] );
				
			renderParamBlock->SetParameterDataVec2( 3, shadowScale, shadowOffset );
			//renderParamBlock->SetParameterDataVec4( deoglSkinShader::erutDepthOffset,
			// 	smOffsetScale, smOffsetBias, -smOffsetScale, -smOffsetBias );
			
			renderParamBlock->SetParameterDataArrayMat4x3( 1, cmf, matrixCamera );
				renderParamBlock->SetParameterDataArrayMat4x4( 0, cmf, matrixCamera * matrixProjection );
			}
			
		}catch( const deException & ){
			renderParamBlock->UnmapBuffer();
			throw;
		}
		renderParamBlock->UnmapBuffer();
		
		// object render cube face special parameter have been already updated by RenderShadowMaps
	}
	
	// set states
	OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDepthFunc( GL_LEQUAL ) ); // point light uses linear depth
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
	SetCullMode( false );
	
	if( useShadowCubeEncodeDepth ){
		OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
		
	}else{
		OGL_CHECK( renderThread, glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ) );
	}
	
	OGL_CHECK( renderThread, glDepthMask( GL_TRUE ) );
	
	if( pglClipControl && defren.GetUseInverseDepth() ){
		pglClipControl( GL_LOWER_LEFT,  GL_NEGATIVE_ONE_TO_ONE ); // reset, point light uses linear depth
	}
	
	addToRenderTask.Reset();
	addToRenderTask.SetSolid( true );
	addToRenderTask.SetNoShadowNone( true );
	
	if( useGSRenderCube ){
		addToRenderTask.SetUseSpecialParamBlock( true );
		addToRenderTask.SetEnforceShader( pShaderOccMapCube->GetRTSShader() );
		
	}else{
		addToRenderTask.SetEnforceShader( pShaderOccMap->GetRTSShader() );
	}
	
	// clear
	const GLfloat clearColor[ 4 ] = { 1.0f, 0.0f, 0.0f, 0.0f };
	const GLfloat clearDepth = ( GLfloat )1.0f; // point light uses linear depth
	
	if( ! bugClearEntireCubeMap ){
		shadowMapper.ActivateAmbientCubeMap( ambientMapSize );
		OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
		if( useShadowCubeEncodeDepth ){
			OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
		}
	}
	
	// render the solid shadow cube map
	if( useGSRenderCube ){
		// cube map activate already by clear
		
		renderTask.Clear();
		renderTask.SetRenderParamBlock( renderParamBlock );
		renderTask.SetUseSPBInstanceFlags( true );
		
		if( clist1 ){
			addToRenderTask.AddOcclusionMeshes( *clist1 );
		}
		if( clist2 ){
			addToRenderTask.AddOcclusionMeshes( *clist2 );
		}
		
		renderTask.PrepareForRender();
		rengeom.RenderTask( renderTask );
		
		// PROBLEM: depth, geometry and shadow map rendering uses new shared-spb system.
		//          in this system special-param-block is no more per-component but
		//          per-render-task. this code here expects special-param blocks on
		//          components to be prepared by a previous shadow map call which is no
		//          more the case. requires updating occlusion-mesh in add-to-render-task
		//          to be using new shared-spb system too, then this works again
		
	}else{
		// render the solid shadow cube map
		for( cmf=0; cmf<6; cmf++ ){
			deoglCubeMap::CreateMatrixForFace( matrixCamera, lightPosition, cmf );
			
			renderParamBlock->MapBuffer();
			try{
				
				renderParamBlock->SetParameterDataVec2( 3, shadowScale, shadowOffset );
				//renderParamBlock->SetParameterDataVec4( deoglSkinShader::erutDepthOffset,
				// 	smOffsetScale, smOffsetBias, -smOffsetScale, -smOffsetBias );
				
				renderParamBlock->SetParameterDataMat4x4( 0, matrixCamera * matrixProjection );
				renderParamBlock->SetParameterDataMat4x3( 1, matrixCamera );
				
			}catch( const deException & ){
				renderParamBlock->UnmapBuffer();
				throw;
			}
			renderParamBlock->UnmapBuffer();
			
			shadowMapper.ActivateAmbientCubeMapFace( ambientMapSize, pCubeFaces[ cmf ] );
			
			if( bugClearEntireCubeMap ){
				OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
				if( useShadowCubeEncodeDepth ){
					OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
				}
			}
			
			renderTask.Clear();
			renderTask.SetRenderParamBlock( renderParamBlock );
			
			addToRenderTask.SetFilterCubeFace( cmf );
			if( clist1 ){
				addToRenderTask.AddOcclusionMeshes( *clist1 );
			}
			if( clist2 ){
				addToRenderTask.AddOcclusionMeshes( *clist2 );
			}
			
			renderTask.PrepareForRender();
			rengeom.RenderTask( renderTask );
		}
	}
}



void deoglRenderLightPoint::UpdateLightParamBlock( deoglLightShader &lightShader,
deoglSPBlockUBO &paramBlock, deoglRenderPlanLight &planLight ){
	const deoglRLight &light = *planLight.GetLight()->GetLight();
	deoglRenderPlan &plan = planLight.GetPlan();
	int target;
	
	// get light properties
	decColor lightColor = light.GetColor();
	
	lightColor.r = powf( lightColor.r, OGL_RENDER_GAMMA );
	lightColor.g = powf( lightColor.g, OGL_RENDER_GAMMA );
	lightColor.b = powf( lightColor.b, OGL_RENDER_GAMMA );
	lightColor = TransformColor( plan.GetWorld()->GetLightColorMatrix(), lightColor );
	
	float lightImageGamma = OGL_RENDER_GAMMA;
	
	if( light.GetUseSkinTexture() ){
		lightImageGamma = light.GetUseSkinTexture()->GetColorGamma();
	}
	
	// set values
	paramBlock.MapBuffer();
	try{
		const bool hasGIState = plan.GetRenderGIState() != NULL;
		
		target = lightShader.GetLightUniformTarget( deoglLightShader::elutLightColor );
		if( target != -1 ){
			paramBlock.SetParameterDataVec3( target, lightColor * light.GetIntensity() );
		}
		
		target = lightShader.GetLightUniformTarget( deoglLightShader::elutLightRange );
		if( target != -1 ){
			paramBlock.SetParameterDataFloat( target, light.GetRange() );
		}
		
		target = lightShader.GetLightUniformTarget( deoglLightShader::elutLightAmbientRatio );
		if( target != -1 ){
			if( hasGIState || plan.GetNoAmbientLight() ){
				paramBlock.SetParameterDataFloat( target, 0.0f );
				
			}else{
				paramBlock.SetParameterDataFloat( target, light.GetAmbientRatio() );
				// this happens usually only for env-maps. better be dark than wrong lit
				//paramBlock.SetParameterDataFloat( target, 0.0f );
			}
		}
		
		target = lightShader.GetLightUniformTarget( deoglLightShader::elutLightGIAmbientRatio );
		if( target != -1 ){
			if( plan.GetNoAmbientLight() ){
				paramBlock.SetParameterDataFloat( target, 0.0f );
				
			}else{
				paramBlock.SetParameterDataFloat( target, light.GetAmbientRatio() );
			}
		}
		
		target = lightShader.GetLightUniformTarget( deoglLightShader::elutLightAttenuationCoefficient );
		if( target != -1 ){
			paramBlock.SetParameterDataFloat( target, light.GetAttenuationCoefficient() );
		}
		
		target = lightShader.GetLightUniformTarget( deoglLightShader::elutLightDampingCoefficient );
		if( target != -1 ){
			paramBlock.SetParameterDataFloat( target, light.GetDampingCoefficient() );
		}
		
		target = lightShader.GetLightUniformTarget( deoglLightShader::elutLightDampingThreshold );
		if( target != -1 ){
			paramBlock.SetParameterDataFloat( target, light.GetDampingThreshold() );
		}
		
		target = lightShader.GetLightUniformTarget( deoglLightShader::elutLightImageGamma );
		if( target != -1 ){
			paramBlock.SetParameterDataFloat( target, decMath::max( lightImageGamma, 0.0f ) );
		}
		
	}catch( const deException & ){
		paramBlock.UnmapBuffer();
		throw;
	}
	paramBlock.UnmapBuffer();
}

void deoglRenderLightPoint::UpdateInstanceParamBlock( deoglLightShader &lightShader,
deoglSPBlockUBO &paramBlock, deoglRenderPlanLight &planLight, sShadowDepthMaps &shadowDepthMaps ){
	deoglRLight &light = *planLight.GetLight()->GetLight();
	deoglRenderPlan &plan = planLight.GetPlan();
	float noiseScale;
	int target;
	
	// calculate matrices
	const deoglConfiguration &config = GetRenderThread().GetConfiguration();
	deoglShadowCaster &shadowCaster = *light.GetShadowCaster();
	
	const decDMatrix matrixMV( light.GetMatrix() * plan.GetCameraMatrix() );
	const decDMatrix matrixMVP( matrixMV * plan.GetProjectionMatrix() );
	
	decDMatrix matrixShadow( ( decDMatrix::CreateTranslation( light.GetMatrix().GetPosition() )
		* plan.GetCameraMatrix() ).QuickInvert() );
	matrixShadow.a21 = -matrixShadow.a21;
	matrixShadow.a22 = -matrixShadow.a22;
	matrixShadow.a23 = -matrixShadow.a23;
	matrixShadow.a24 = -matrixShadow.a24;
	
	// set values
	paramBlock.MapBuffer();
	try{
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutMatrixMVP );
		if( target != -1 ){
			paramBlock.SetParameterDataMat4x4( target, matrixMVP );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutMatrixMV );
		if( target != -1 ){
			paramBlock.SetParameterDataMat4x3( target, matrixMV );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutLightPosition );
		if( target != -1 ){
			paramBlock.SetParameterDataVec3( target, matrixMV.GetPosition() );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutDepthCompare );
		if( target != -1 ){
			paramBlock.SetParameterDataFloat( target, planLight.GetLight()->GetCameraInside() ? 0.0f
				: ( GetRenderThread().GetDeferredRendering().GetDepthCompareFuncRegular() == GL_LEQUAL ? 1.0f : -1.0f ) );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadowMatrix1 );
		if( target != -1 ){
			paramBlock.SetParameterDataMat4x4( target, matrixShadow );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadowMatrix2 );
		if( target != -1 ){
			paramBlock.SetParameterDataMat4x4( target, matrixShadow );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutLightImageOmniMatrix );
		if( target != -1 ){
			decVector rotate;
			if( light.GetUseSkinTexture() ){
				rotate = light.GetUseSkinTexture()->GetOmniDirRotate() * TWO_PI;
			}
			paramBlock.SetParameterDataMat4x3( target, ( decMatrix::CreateRotation( rotate ) * matrixMV ).QuickInvert() );
		}
		
		/*
		float pcfSize = config.GetShadowCubePCFSize() / ( float )shadowMapSize;
		shader->SetParameterDMatrix4x4( splShadowMatrix, shadowMatrix );
		shader->SetParameterFloat( splShaStaParams, shadowCaster.GetStaticScale(), -shadowCaster.GetStaticOffset(), pcfSize );
		shader->SetParameterFloat( splShaDynParams, shadowCaster.GetDynamicScale(), -shadowCaster.GetDynamicOffset(), pcfSize );
		*/
		const float shadowScale = shadowCaster.GetStaticScale();
		const float shadowOffset = shadowCaster.GetStaticOffset();
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadow1Solid );
		if( target != -1 ){
			if( shadowDepthMaps.shadow1Solid ){
				noiseScale = config.GetShadowCubePCFSize() / ( float )shadowDepthMaps.shadow1Solid->GetSize();
				
			}else{
				noiseScale = config.GetShadowCubePCFSize() / 1024.0f;
			}
			
			paramBlock.SetParameterDataVec3( target, shadowScale, shadowOffset, noiseScale );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadow1Transparent );
		if( target != -1 ){
			if( shadowDepthMaps.shadow1Transp ){
				noiseScale = config.GetShadowCubePCFSize() / ( float )shadowDepthMaps.shadow1Transp->GetSize();
				
			}else{
				noiseScale = config.GetShadowCubePCFSize() / 1024.0f;
			}
			
			paramBlock.SetParameterDataVec3( target, shadowScale, shadowOffset, noiseScale );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadow2Solid );
		if( target != -1 ){
			if( shadowDepthMaps.shadow2Solid ){
				noiseScale = config.GetShadowCubePCFSize() / ( float )shadowDepthMaps.shadow2Solid->GetSize();
				
			}else{
				noiseScale = config.GetShadowCubePCFSize() / 1024.0f;
			}
			
			paramBlock.SetParameterDataVec3( target, shadowScale, shadowOffset, noiseScale );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadow2Transparent );
		if( target != -1 ){
			if( shadowDepthMaps.shadow2Transp ){
				noiseScale = config.GetShadowCubePCFSize() / ( float )shadowDepthMaps.shadow2Transp->GetSize();
				
			}else{
				noiseScale = config.GetShadowCubePCFSize() / 1024.0f;
			}
			
			paramBlock.SetParameterDataVec3( target, shadowScale, shadowOffset, noiseScale );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadowDepthTransform );
		if( target != -1 ){
			// depth = distance * depthScale + depthOffset;
			//   whereas:
			//     depthScale = GetStaticScale()
			//     depthOffset = GetStaticOffset()
			// 
			// distance = (depth - depthOffset) / depthScale
			// distance = depth / depthScale - depthOffset / depthScale
			// 
			// which yields:
			//   distance = depth * param1 + param2
			// 
			// with:
			//   param1 = 1 / GetStaticScale()
			//   param2 = -GetStaticOffset() / GetStaticScale()
			const deoglShadowCaster &shadowCaster = *light.GetShadowCaster();
			const float invDepthScale = 1.0f / shadowCaster.GetStaticScale();
			
			paramBlock.SetParameterDataVec2( target, invDepthScale,
				-shadowCaster.GetStaticOffset() * invDepthScale );
		}
		
	}catch( const deException & ){
		paramBlock.UnmapBuffer();
		throw;
	}
	paramBlock.UnmapBuffer();
}



void deoglRenderLightPoint::ResetDebugInfo(){
	pDebugInfoSolid->Clear();
	pDebugInfoTransparent->Clear();
	
	pDebugInfoSolidShadow->Clear();
	pDebugInfoSolidShadowClear->Clear();
	pDebugInfoSolidShadowClearTransp->Clear();
	pDebugInfoSolidShadowFace->Clear();
	pDebugInfoSolidShadowFaceClear->Clear();
	pDebugInfoSolidShadowFaceTask->Clear();
	pDebugInfoSolidShadowFaceRender->Clear();
	pDebugInfoSolidShadowFaceTranspClear->Clear();
	pDebugInfoSolidShadowFaceTranspTask->Clear();
	pDebugInfoSolidShadowFaceTranspRender->Clear();
	pDebugInfoSolidLight->Clear();
	
	pDebugInfoTransparentShadow->Clear();
	pDebugInfoTransparentShadowClear->Clear();
	pDebugInfoTransparentShadowClearTransp->Clear();
	pDebugInfoTransparentShadowFace->Clear();
	pDebugInfoTransparentShadowFaceClear->Clear();
	pDebugInfoTransparentShadowFaceTask->Clear();
	pDebugInfoTransparentShadowFaceRender->Clear();
	pDebugInfoTransparentShadowFaceTranspClear->Clear();
	pDebugInfoTransparentShadowFaceTranspTask->Clear();
	pDebugInfoTransparentShadowFaceTranspRender->Clear();
	pDebugInfoTransparentLight->Clear();
}

void deoglRenderLightPoint::AddTopLevelDebugInfoSolid(){
	GetRenderThread().GetDebug().GetDebugInformationList().Add( pDebugInfoSolidDetail );
}

void deoglRenderLightPoint::AddTopLevelDebugInfoTransparent(){
	GetRenderThread().GetDebug().GetDebugInformationList().Add( pDebugInfoTransparentDetail );
}

void deoglRenderLightPoint::DevModeDebugInfoChanged(){
	const int details = GetRenderThread().GetDebug().GetDeveloperMode().GetDebugInfoDetails();
	const bool show = ( details & ( deoglDeveloperMode::edimLight | deoglDeveloperMode::edimLightPoint ) )
		== ( deoglDeveloperMode::edimLight | deoglDeveloperMode::edimLightPoint );
	
	pDebugInfoSolidDetail->SetVisible( show );
	//pDebugInfoSolidShadow->SetVisible( true );
	
	pDebugInfoTransparentDetail->SetVisible( show );
}



// Private Functions
//////////////////////

void deoglRenderLightPoint::pCleanUp(){
	if( pShaderOccMapCube ){
		pShaderOccMapCube->RemoveUsage();
	}
	if( pShaderOccMap ){
		pShaderOccMap->RemoveUsage();
	}
	if( pShaderBoxBoundary2 ){
		pShaderBoxBoundary2->RemoveUsage();
	}
	if( pShaderBoxBoundary1Ambient ){
		pShaderBoxBoundary1Ambient->RemoveUsage();
	}
	if( pShaderBoxBoundary1 ){
		pShaderBoxBoundary1->RemoveUsage();
	}
	
	if( pShaderLight ){
		pShaderLight->RemoveUsage();
	}
	if( pShaderLightShadow ){
		pShaderLightShadow->RemoveUsage();
	}
	if( pShaderLightShadowTransp ){
		pShaderLightShadowTransp->RemoveUsage();
	}
	if( pShaderShadowSolid ){
		pShaderShadowSolid->RemoveUsage();
	}
	if( pShaderShadowHoles ){
		pShaderShadowHoles->RemoveUsage();
	}
	if( pShaderShadowTransp ){
		pShaderShadowTransp->RemoveUsage();
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
	if( pDebugInfoSolidShadowClear ){
		pDebugInfoSolidShadowClear->FreeReference();
	}
	if( pDebugInfoSolidShadowClearTransp ){
		pDebugInfoSolidShadowClearTransp->FreeReference();
	}
	if( pDebugInfoSolidShadowFace ){
		pDebugInfoSolidShadowFace->FreeReference();
	}
	if( pDebugInfoSolidShadowFaceClear ){
		pDebugInfoSolidShadowFaceClear->FreeReference();
	}
	if( pDebugInfoSolidShadowFaceTask ){
		pDebugInfoSolidShadowFaceTask->FreeReference();
	}
	if( pDebugInfoSolidShadowFaceRender ){
		pDebugInfoSolidShadowFaceRender->FreeReference();
	}
	if( pDebugInfoSolidShadowFaceTranspClear ){
		pDebugInfoSolidShadowFaceTranspClear->FreeReference();
	}
	if( pDebugInfoSolidShadowFaceTranspTask ){
		pDebugInfoSolidShadowFaceTranspTask->FreeReference();
	}
	if( pDebugInfoSolidShadowFaceTranspRender ){
		pDebugInfoSolidShadowFaceTranspRender->FreeReference();
	}
	if( pDebugInfoSolidLight ){
		pDebugInfoSolidLight->FreeReference();
	}
	
	if( pDebugInfoTransparentDetail ){
		pDebugInfoTransparentDetail->FreeReference();
	}
	if( pDebugInfoTransparentShadow ){
		pDebugInfoTransparentShadow->FreeReference();
	}
	if( pDebugInfoTransparentShadowClear ){
		pDebugInfoTransparentShadowClear->FreeReference();
	}
	if( pDebugInfoTransparentShadowClearTransp ){
		pDebugInfoTransparentShadowClearTransp->FreeReference();
	}
	if( pDebugInfoTransparentShadowFace ){
		pDebugInfoTransparentShadowFace->FreeReference();
	}
	if( pDebugInfoTransparentShadowFaceClear ){
		pDebugInfoTransparentShadowFaceClear->FreeReference();
	}
	if( pDebugInfoTransparentShadowFaceTask ){
		pDebugInfoTransparentShadowFaceTask->FreeReference();
	}
	if( pDebugInfoTransparentShadowFaceRender ){
		pDebugInfoTransparentShadowFaceRender->FreeReference();
	}
	if( pDebugInfoTransparentShadowFaceTranspClear ){
		pDebugInfoTransparentShadowFaceTranspClear->FreeReference();
	}
	if( pDebugInfoTransparentShadowFaceTranspTask ){
		pDebugInfoTransparentShadowFaceTranspTask->FreeReference();
	}
	if( pDebugInfoTransparentShadowFaceTranspRender ){
		pDebugInfoTransparentShadowFaceTranspRender->FreeReference();
	}
	if( pDebugInfoTransparentLight ){
		pDebugInfoTransparentLight->FreeReference();
	}
}
