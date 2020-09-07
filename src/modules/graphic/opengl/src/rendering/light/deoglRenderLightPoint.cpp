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
#include "../deoglRenderGeometry.h"
#include "../deoglRenderOcclusion.h"
#include "../defren/deoglDeferredRendering.h"
#include "../plan/deoglRenderPlanDebug.h"
#include "../plan/deoglRenderPlan.h"
#include "../task/deoglAddToRenderTask.h"
#include "../task/deoglRenderTask.h"
#include "../task/deoglRenderTaskShader.h"
#include "../../canvas/render/deoglRCanvasView.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../capabilities/deoglCapsTextureFormat.h"
#include "../../collidelist/deoglCollideListComponent.h"
#include "../../collidelist/deoglCollideList.h"
#include "../../collidelist/deoglCollideListManager.h"
#include "../../component/deoglRComponent.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../debug/deoglDebugInformation.h"
#include "../../debug/deoglDebugSaveTexture.h"
#include "../../devmode/deoglDeveloperMode.h"
#include "../../extensions/deoglExtensions.h"
#include "../../framebuffer/deoglFramebuffer.h"
#include "../../framebuffer/deoglFramebufferManager.h"
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
pDebugInfoSolidShadowVBOs( NULL ),
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
pDebugInfoTransparentShadowVBOs( NULL ),
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
		const decColor colorBg( 0.0f, 0.0f, 0.0f, 0.75f );
		const decColor colorBgSub( 0.05f, 0.05f, 0.05f, 0.75f );
		const decColor colorBgSub2( 0.1f, 0.1f, 0.1f, 0.75f );
		const decColor colorBgSub3( 0.15f, 0.15f, 0.15f, 0.75f );
		
		pDebugInfoSolid = new deoglDebugInformation( "Point", colorText, colorBgUp );
		pDebugInfoTransparent = new deoglDebugInformation( "Point", colorText, colorBgUp );
		
		
		
		pDebugInfoSolidDetail = new deoglDebugInformation( "Light Point Solid", colorText, colorBg );
		
		pDebugInfoSolidShadow = new deoglDebugInformation( "Shadow", colorText, colorBgSub );
		pDebugInfoSolidDetail->GetChildren().Add( pDebugInfoSolidShadow );
		
		pDebugInfoSolidShadowVBOs = new deoglDebugInformation( "VBOs", colorText, colorBgSub2 );
		pDebugInfoSolidShadow->GetChildren().Add( pDebugInfoSolidShadowVBOs );
		
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
		
		pDebugInfoTransparentShadowVBOs = new deoglDebugInformation( "VBOs", colorText, colorBgSub2 );
		pDebugInfoTransparentShadow->GetChildren().Add( pDebugInfoTransparentShadowVBOs );
		
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

void deoglRenderLightPoint::CalculateBoxBoundary( deoglRLight &light ){
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



void deoglRenderLightPoint::RenderLights( deoglRenderPlan &plan, bool solid ){
DEBUG_RESET_TIMER_TOTAL
	deoglCollideList &clist = plan.GetCollideList();
	const int lightCount = clist.GetLightCount();
	int i;
	
	DebugTimersReset( plan, false );
	
	// DEBUG DEBUG this should NOT be needed
	RestoreFBO( plan );
	// DEBUG DEBUG this should NOT be needed
	
	for( i=0; i<lightCount; i++ ){
		deoglRLight &light = *clist.GetLightAt( i );
		if( light.GetLightType() != deLight::eltPoint ){
			continue;
		}
		
		if( light.IsHiddenByOccQuery() ){
			light.SetVisible( false );
			
		}else{
			RenderLight( plan, solid, light );
		}
	}
	
	// clean up job
	if( solid ){
		DebugTimer1Sample( plan, *pDebugInfoSolid, true );
		
	}else{
		DebugTimer1Sample( plan, *pDebugInfoTransparent, true );
	}
DEBUG_PRINT_TIMER_TOTAL
}



void deoglRenderLightPoint::RenderLight( deoglRenderPlan &plan, bool solid, deoglRLight &light ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglConfiguration &config = renderThread.GetConfiguration();
	const bool useShadowCubeEncodeDepth = config.GetUseShadowCubeEncodeDepth();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglShadowMapper &shadowMapper = renderThread.GetShadowMapper();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglShadowCaster &shadowCaster = *light.GetShadowCaster();
	deoglSCTransparent &sctransp = shadowCaster.GetTransparent();
	deoglSCAmbient &scambient = shadowCaster.GetAmbient();
	deoglRenderPlanDebug * const planDebug = plan.GetDebug();
	deoglSCSolid &scsolid = shadowCaster.GetSolid();
	deoglShadowCaster::eShadowTypes shadowType = shadowCaster.GetShadowType();
	bool useShadow = renderThread.GetConfiguration().GetDebugUseShadow();
	
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
	
	// if layer mask restriction is used dynamic only shadows have to be used to filter properly
	if( useShadow && plan.GetUseLayerMask() ){
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
	deoglCubeMap *texAmbient = NULL;
	
	if( useShadow ){
		RenderShadows( plan, solid, light, matrixLP,
			transparentStaticShadow, transparentDynamicShadow );
		
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
			texAmbient = scambient.GetStaticCubeMap();
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
			break;
			
		case deoglShadowCaster::estStaticAndDynamic:
			texSolidDepth1 = scsolid.GetStaticCubeMap();
			texAmbient = scambient.GetStaticCubeMap();
			
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
			break;
			
		default:
			break;
		}
		
		if( solid ){
			DebugTimer2SampleCount( plan, *pDebugInfoSolidShadow, 1, true );
			
		}else{
			DebugTimer2SampleCount( plan, *pDebugInfoTransparentShadow, 1, true );
		}
		
// 	}else{
// 		// DEBUG DEBUG this should NOT be needed
// 		RestoreFBO( plan );
// 		// DEBUG DEBUG this should NOT be needed
	}
	
DEBUG_RESET_TIMER
	// set states
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
	
	if( light.GetCameraInside() ){ // cull front faces, no depth test
		OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
		SetCullMode( ! plan.GetFlipCulling() );
		
	}else{ // cull back faces, depth test
		OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
		OGL_CHECK( renderThread, glDepthFunc( GL_LEQUAL ) ); // point light uses linear depth
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
				if( texAmbient ){
					lightShader = light.GetShaderFor( deoglRLight::estSolid1Transp1 );
					
				}else{
					lightShader = light.GetShaderFor( deoglRLight::estSolid1Transp1NoAmbient );
				}
				
			}else{
				if( texAmbient ){
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
	
	renderThread.GetShader().ActivateShader( lightShader->GetShader() );
	
	// set program parameters
	deoglSPBlockUBO * const spbInstance = light.GetInstanceParameterBlock();
	deoglSPBlockUBO * const spbLight = light.GetLightParameterBlock();
	
	if( ! spbLight || ! spbInstance ){
		DETHROW( deeInvalidParam );
	}
	
	UpdateLightParamBlock( *lightShader, *spbLight, plan, light );
	
	sShadowDepthMaps shadowDepthmaps;
	shadowDepthmaps.shadow1Solid = texSolidDepth1;
	shadowDepthmaps.shadow1Transp = texTranspDepth1;
	shadowDepthmaps.shadow2Solid = texSolidDepth2;
	shadowDepthmaps.shadowAmbient = texAmbient;
	UpdateInstanceParamBlock( *lightShader, *spbInstance, plan, light, shadowDepthmaps );
	
	GetRenderThread().GetRenderers().GetLight().GetLightPB()->Activate();
	spbLight->Activate();
	spbInstance->Activate();
	
	// set textures
	int target;
	
	target = lightShader->GetTextureTarget( deoglLightShader::ettColorCubemap );
	if( target != -1 ){
		deoglTexUnitConfig tuc;
		
		if( light.GetLightCanvas() ){
			// TODO can not be used as cube-map for the time being
			
		}else if( light.GetUseSkinTexture() ){
			tuc.EnableCubeMapFromChannel( renderThread, *light.GetUseSkinTexture(),
				deoglSkinChannel::ectColorOmnidirCube, light.GetSkinState(),
				light.GetDynamicSkin(), renderThread.GetDefaultTextures().GetEnvMap() );
		}
		
		if( tuc.IsEnabled() ){
			tuc.Apply( renderThread, target );
			
		}else{
			tsmgr.EnableCubeMap( target, *renderThread.GetDefaultTextures().GetEnvMap(),
				GetSamplerClampLinear() );
		}
	}
	
	target = lightShader->GetTextureTarget( deoglLightShader::ettColorEquirect );
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
				light.GetDynamicSkin(), renderThread.GetDefaultTextures().GetColor() );
		}
		
		if( tuc.IsEnabled() ){
			tuc.Apply( renderThread, target );
			
		}else{
			tsmgr.EnableTexture( target, *renderThread.GetDefaultTextures().GetColor(),
				GetSamplerClampLinear() );
		}
	}
	
	target = lightShader->GetTextureTarget( deoglLightShader::ettNoise );
	if( target != -1 ){
		tsmgr.EnableTexture( target, *renderThread.GetDefaultTextures().GetNoise2D(), GetSamplerRepeatNearest() );
	}
	
	target = lightShader->GetTextureTarget( deoglLightShader::ettShadow1SolidDepth );
	if( target != -1 ){
		if( texSolidDepth1->GetFormat()->GetIsDepth() ){
			tsmgr.EnableCubeMap( target, *texSolidDepth1, GetSamplerShadowClampLinear() );
			
		}else{
			tsmgr.EnableCubeMap( target, *texSolidDepth1, GetSamplerClampLinear() );
		}
	}
	
	target = lightShader->GetTextureTarget( deoglLightShader::ettShadow1TransparentDepth );
	if( target != -1 ){
		if( texTranspDepth1->GetFormat()->GetIsDepth() ){
			tsmgr.EnableCubeMap( target, *texTranspDepth1, GetSamplerShadowClampLinear() );
			
		}else{
			tsmgr.EnableCubeMap( target, *texTranspDepth1, GetSamplerClampLinear() );
		}
	}
	
	target = lightShader->GetTextureTarget( deoglLightShader::ettShadow1TransparentColor );
	if( target != -1 ){
		tsmgr.EnableCubeMap( target, *texTranspColor1, GetSamplerClampLinear() );
	}
	
	target = lightShader->GetTextureTarget( deoglLightShader::ettShadow2SolidDepth );
	if( target != -1 ){
		if( texSolidDepth2->GetFormat()->GetIsDepth() ){
			tsmgr.EnableCubeMap( target, *texSolidDepth2, GetSamplerShadowClampLinear() );
			
		}else{
			tsmgr.EnableCubeMap( target, *texSolidDepth2, GetSamplerClampLinear() );
		}
	}
	
	target = lightShader->GetTextureTarget( deoglLightShader::ettShadow2TransparentDepth );
	if( target != -1 ){
		if( texTranspDepth2->GetFormat()->GetIsDepth() ){
			tsmgr.EnableCubeMap( target, *texTranspDepth2, GetSamplerShadowClampLinear() );
			
		}else{
			tsmgr.EnableCubeMap( target, *texTranspDepth2, GetSamplerClampLinear() );
		}
	}
	
	target = lightShader->GetTextureTarget( deoglLightShader::ettShadow2TransparentColor );
	if( target != -1 ){
		tsmgr.EnableCubeMap( target, *texTranspColor2, GetSamplerClampLinear() );
	}
	
	target = lightShader->GetTextureTarget( deoglLightShader::ettShadowAmbient );
	if( target != -1 ){
		tsmgr.EnableCubeMap( target, *texAmbient, GetSamplerShadowClampLinear() );
	}
	
	target = lightShader->GetTextureTarget( deoglLightShader::ettLightDepth1 );
	if( target != -1 ){
		tsmgr.EnableCubeMap( target, *texSolidDepth1, GetSamplerClampLinear() );
	}
	
	target = lightShader->GetTextureTarget( deoglLightShader::ettLightDepth2 );
	if( target != -1 ){
		tsmgr.EnableCubeMap( target, *texSolidDepth2, GetSamplerClampLinear() );
	}
	
	// render the light
	pglBindVertexArray( light.GetLightVolume()->GetVAO() );
	OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLES, 0, light.GetLightVolume()->GetPointCount() ) );
	pglBindVertexArray( 0 );
	
	if( solid ){
		DebugTimer2SampleCount( plan, *pDebugInfoSolidLight, 1, true );
		
	}else{
		DebugTimer2SampleCount( plan, *pDebugInfoTransparentLight, 1, true );
	}
DEBUG_PRINT_TIMER( "Render" );
}

void deoglRenderLightPoint::RenderShadows( deoglRenderPlan &plan, bool solid, deoglRLight &light,
const decDMatrix &matrixProjection, bool transparentStaticShadow, bool transparentDynamicShadow ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglShadowMapper &shadowMapper = renderThread.GetShadowMapper();
	deoglShadowCaster &shadowCaster = *light.GetShadowCaster();
	deoglShadowCaster::eShadowTypes shadowType = shadowCaster.GetShadowType();
	deoglSCTransparent &sctransp = shadowCaster.GetTransparent();
	deoglSCAmbient &scambient = shadowCaster.GetAmbient();
	deoglSCSolid &scsolid = shadowCaster.GetSolid();
	bool updateBoxBoundary = false;
	
	// get map sizes as calculated by render plan
	const int staticShadowMapSize = scsolid.GetPlanStaticSize();
	const int dynamicShadowMapSize = solid ?
		scsolid.GetPlanDynamicSize() : scsolid.GetPlanTransparentSize();
	
	const int staticTranspShadowMapSize = sctransp.GetPlanStaticSize();
	const int dynamicTranspShadowMapSize = solid ?
		sctransp.GetPlanDynamicSize() : sctransp.GetPlanTransparentSize();
	
	const int ambientMapSize = scambient.GetPlanDynamicSize();
	
	// if layer mask restriction is used dynamic only shadows have to be used to filter properly
	if( plan.GetUseLayerMask() ){
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
			if( scsolid.GetStaticCubeMap()->GetSize() < staticShadowMapSize ){
				scsolid.DropStatic();
				requiresUpdate = true;
			}
			if( scambient.GetStaticCubeMap()->GetSize() < ambientMapSize ){
				scambient.DropStatic();
				requiresUpdate = true;
			}
			
		}else{
			requiresUpdate = true;
		}
		
		if( transparentStaticShadow ){
			if( sctransp.GetStaticShadowCubeMap() ){
				if( sctransp.GetStaticShadowCubeMap()->GetSize() < staticTranspShadowMapSize ){
					sctransp.DropStatic();
					requiresUpdate = true;
				}
				
			}else{
				requiresUpdate = true;
			}
			
			if( sctransp.GetStaticColorCubeMap() ){
				if( sctransp.GetStaticColorCubeMap()->GetSize() < staticTranspShadowMapSize ){
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
			
			RenderShadowMaps( plan, light, matrixProjection, shadowMapper, light.GetStaticCollideList(), NULL,
				staticShadowMapSize, staticTranspShadowMapSize, transparentStaticShadow,
				shadowCaster.GetStaticScale(), shadowCaster.GetStaticOffset(), solid );
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
			shadowMapper.DropForeignCubeMaps();
			shadowMapper.DropForeignTextures();
			
			shadowMapper.SetForeignSolidDepthCubeMap( scambient.ObtainStaticCubeMapWithSize( ambientMapSize ) );
			RenderAmbientMap( plan, light, matrixProjection, shadowMapper, *light.GetStaticCollideList(),
				ambientMapSize, shadowCaster.GetStaticScale(), shadowCaster.GetStaticOffset() );
			shadowMapper.DropForeignCubeMaps();
			
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
	}
	
	// dynamic shadow map with transparency if required
	if( shadowType == deoglShadowCaster::estDynamicOnly
	||  shadowType == deoglShadowCaster::estStaticAndDynamic ){
		const deoglCollideList *clist1 = NULL;
		const deoglCollideList *clist2 = NULL;
		
		if( plan.GetUseLayerMask() ){
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
			RenderShadowMaps( plan, light, matrixProjection, shadowMapper, clist1, clist2,
				dynamicShadowMapSize, dynamicTranspShadowMapSize, transparentDynamicShadow,
				shadowCaster.GetStaticScale(), shadowCaster.GetStaticOffset(), solid );
DEBUG_PRINT_TIMER( "Shadow Dynamic Solid" );
			
		}else if( ! scsolid.GetDynamicCubeMap() ){
DEBUG_RESET_TIMER
			shadowMapper.SetForeignSolidDepthCubeMap( scsolid.ObtainDynamicCubeMapWithSize(
				scsolid.GetPlanTransparentSize() )->GetCubeMap() );
			
			if( transparentDynamicShadow ){
				shadowMapper.SetForeignTransparentDepthCubeMap( sctransp.
					ObtainDynamicShadowCubeMapWithSize( dynamicTranspShadowMapSize )->GetCubeMap() );
				shadowMapper.SetForeignTransparentColorCubeMap( sctransp.
					ObtainDynamicColorCubeMapWithSize( dynamicTranspShadowMapSize )->GetCubeMap() );
			}
			
			RenderShadowMaps( plan, light, matrixProjection, shadowMapper, clist1, clist2,
				dynamicShadowMapSize, dynamicTranspShadowMapSize, transparentDynamicShadow,
				shadowCaster.GetStaticScale(), shadowCaster.GetStaticOffset(), solid );
			
			shadowMapper.DropForeignCubeMaps();
			shadowMapper.DropForeignTextures();
DEBUG_PRINT_TIMER( "Shadow Dynamic Transparent" );
		}
	}
	
	// update box boundary if required
	if( updateBoxBoundary ){
DEBUG_RESET_TIMER
		CalculateBoxBoundary( light );
DEBUG_PRINT_TIMER( "Boundary Box" );
	}
	
	// activate stencil as we had to disable it for rendering the shadow maps
	OGL_CHECK( renderThread, glEnable( GL_STENCIL_TEST ) );
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

void deoglRenderLightPoint::RenderShadowMaps( deoglRenderPlan &plan, deoglRLight &light,
const decDMatrix &matrixProjection, deoglShadowMapper &shadowMapper,
const deoglCollideList *clist1, const deoglCollideList *clist2, int solidShadowMapSize,
int transpShadowMapSize, bool withTransparent, float shadowScale, float shadowOffset,
bool debugSolid ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglAddToRenderTask &addToRenderTask = renderThread.GetRenderers().GetLight().GetAddToRenderTask();
	deoglRenderTask &renderTask = renderThread.GetRenderers().GetLight().GetRenderTask();
	deoglRenderGeometry &rengeom = renderThread.GetRenderers().GetGeometry();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	const deoglConfiguration &config = renderThread.GetConfiguration();
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
	
	// update component VBOs
	if( clist1 ){
		UpdateComponentVBO( *clist1 );
	}
	if( clist2 ){
		UpdateComponentVBO( *clist2 );
	}
	
	if( debugSolid ){
		DebugTimer3Sample( plan, *pDebugInfoSolidShadowVBOs, true );
		
	}else{
		DebugTimer3Sample( plan, *pDebugInfoTransparentShadowVBOs, true );
	}
	
	// calculate object render cube face special parameter. optimizes rendering by skipping
	// object faces on cube map faces they are not visible on
	const decDVector cubePosition( light.GetMatrix().GetPosition() );
	int i;
	
	if( clist1 ){
		const int componentCount = clist1->GetComponentCount();
		for( i=0; i<componentCount; i++ ){
			deoglRComponent &component = *clist1->GetComponentAt( i )->GetComponent();
			component.UpdateCubeFaceVisibility( cubePosition );
			if( deoglSkinShader::USE_SHARED_SPB ){
				component.SetSpecialFlagsFromFaceVisibility();
			}
		}
	}
	
	if( clist2 ){
		const int componentCount = clist2->GetComponentCount();
		for( i=0; i<componentCount; i++ ){
			deoglRComponent &component = *clist2->GetComponentAt( i )->GetComponent();
			component.UpdateCubeFaceVisibility( cubePosition );
			if( deoglSkinShader::USE_SHARED_SPB ){
				component.SetSpecialFlagsFromFaceVisibility();
			}
		}
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
		
		// update object render cube face special parameter. optimizes rendering by skipping
		// object faces on cube map faces they are not visible on
		int i;
		
		if( ! deoglSkinShader::USE_SHARED_SPB ){
			if( clist1 ){
				const int componentCount = clist1->GetComponentCount();
				for( i=0; i<componentCount; i++ ){
					clist1->GetComponentAt( i )->GetComponent()->UpdateSpecialSPBCubeRender();
				}
			}
			
			if( clist2 ){
				const int componentCount = clist2->GetComponentCount();
				for( i=0; i<componentCount; i++ ){
					clist2->GetComponentAt( i )->GetComponent()->UpdateSpecialSPBCubeRender();
				}
			}
		}
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
	addToRenderTask.SetForceDoubleSided( true );
	
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
		
		addToRenderTask.SetSolid( true );
		addToRenderTask.SetForceDoubleSided( true );
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
		
		renderTask.PrepareForRender( renderThread );
// 			renderThread.GetLogger().LogInfo( "RenderLightPoint: render task" );
// 			renderTask.DebugPrint( renderThread.GetLogger() );
		rengeom.RenderTask( renderTask );
		
		addToRenderTask.SetForceDoubleSided( false );
		
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
			
			addToRenderTask.SetSolid( true );
			addToRenderTask.SetForceDoubleSided( true );
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
			
			renderTask.PrepareForRender( renderThread );
			rengeom.RenderTask( renderTask );
			
			addToRenderTask.SetForceDoubleSided( false );
			
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
			
			renderTask.PrepareForRender( renderThread );
			rengeom.RenderTask( renderTask );
			
			if( debugSolid ){
				DebugTimer4Sample( plan, *pDebugInfoSolidShadowFaceTranspRender, true );
				DebugTimer3Sample( plan, *pDebugInfoSolidShadowFace, false ); // because we added 1 already during solid
				
			}else{
				DebugTimer4Sample( plan, *pDebugInfoTransparentShadowFaceTranspRender, true );
				DebugTimer3Sample( plan, *pDebugInfoTransparentShadowFace, true ); // because we added 1 already during solid
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
				
				renderTask.PrepareForRender( renderThread );
				rengeom.RenderTask( renderTask );
				
				if( debugSolid ){
					DebugTimer4Sample( plan, *pDebugInfoSolidShadowFaceTranspRender, true );
					DebugTimer3Sample( plan, *pDebugInfoSolidShadowFace, false ); // because we added 1 already during solid
					
				}else{
					DebugTimer4Sample( plan, *pDebugInfoTransparentShadowFaceTranspRender, true );
					DebugTimer3Sample( plan, *pDebugInfoTransparentShadowFace, true ); // because we added 1 already during solid
				}
			}
		}
	}
}

void deoglRenderLightPoint::RenderAmbientMap( deoglRenderPlan &plan, deoglRLight &light,
const decDMatrix &matrixProjection, deoglShadowMapper &shadowMapper,
const deoglCollideList &clist, int ambientMapSize, float shadowScale, float shadowOffset ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglAddToRenderTask &addToRenderTask = renderThread.GetRenderers().GetLight().GetAddToRenderTask();
	deoglRenderTask &renderTask = renderThread.GetRenderers().GetLight().GetRenderTask();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglRenderGeometry &rengeom = renderThread.GetRenderers().GetGeometry();
	const deoglConfiguration &config = renderThread.GetConfiguration();
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
		addToRenderTask.SetEnforceShader( pShaderOccMapCube );
		
	}else{
		addToRenderTask.SetEnforceShader( pShaderOccMap );
	}
	
	// clear
	const GLfloat clearColor[ 4 ] = { 1.0f, 0.0f, 0.0f, 0.0f };
	const GLfloat clearDepth = ( GLfloat )1.0f; // point light uses linear depth
	
	if( ! bugClearEntireCubeMap ){
		shadowMapper.ActivateSolidCubeMap( ambientMapSize );
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
		
		addToRenderTask.AddOcclusionMeshes( clist );
		
		renderTask.PrepareForRender( renderThread );
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
			
			shadowMapper.ActivateSolidCubeMapFace( ambientMapSize, pCubeFaces[ cmf ] );
			
			if( bugClearEntireCubeMap ){
				OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
				if( useShadowCubeEncodeDepth ){
					OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
				}
			}
			
			renderTask.Clear();
			renderTask.SetRenderParamBlock( renderParamBlock );
			
			addToRenderTask.SetFilterCubeFace( cmf );
			addToRenderTask.AddOcclusionMeshes( clist );
			
			renderTask.PrepareForRender( renderThread );
			rengeom.RenderTask( renderTask );
		}
	}
}



void deoglRenderLightPoint::UpdateLightParamBlock( deoglLightShader &lightShader, deoglSPBlockUBO &paramBlock,
deoglRenderPlan &plan, deoglRLight &light ){
	int target;
	
	// get light properties
	decColor lightColor = light.GetColor();
	
	lightColor.r = powf( lightColor.r, OGL_RENDER_GAMMA );
	lightColor.g = powf( lightColor.g, OGL_RENDER_GAMMA );
	lightColor.b = powf( lightColor.b, OGL_RENDER_GAMMA );
	lightColor = TransformColor( plan.GetWorld()->GetLightColorMatrix(), lightColor );
	
	float lightImageGamma = OGL_RENDER_GAMMA;
	
	// set values
	paramBlock.MapBuffer();
	try{
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
			paramBlock.SetParameterDataFloat( target, light.GetAmbientRatio() );
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
			paramBlock.SetParameterDataFloat( target, lightImageGamma );
		}
		
	}catch( const deException & ){
		paramBlock.UnmapBuffer();
		throw;
	}
	paramBlock.UnmapBuffer();
}

void deoglRenderLightPoint::UpdateInstanceParamBlock( deoglLightShader &lightShader,
deoglSPBlockUBO &paramBlock, deoglRenderPlan &plan, deoglRLight &light,
sShadowDepthMaps &shadowDepthMaps ){
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
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadowMatrix1 );
		if( target != -1 ){
			paramBlock.SetParameterDataMat4x4( target, matrixShadow );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadowMatrix2 );
		if( target != -1 ){
			paramBlock.SetParameterDataMat4x4( target, matrixShadow );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutLightImageMatrix );
		if( target != -1 ){
			paramBlock.SetParameterDataMat4x3( target, matrixMV.QuickInvert() );
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
	pDebugInfoSolidShadowVBOs->Clear();
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
	pDebugInfoTransparentShadowVBOs->Clear();
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
	if( pDebugInfoSolidShadowVBOs ){
		pDebugInfoSolidShadowVBOs->FreeReference();
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
	if( pDebugInfoTransparentShadowVBOs ){
		pDebugInfoTransparentShadowVBOs->FreeReference();
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
