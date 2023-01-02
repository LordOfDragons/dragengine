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
#include "../deoglRenderWorld.h"
#include "../defren/deoglDeferredRendering.h"
#include "../lod/deoglLODCalculator.h"
#include "../plan/deoglRenderPlanDebug.h"
#include "../plan/deoglRenderPlan.h"
#include "../plan/deoglRenderPlanLight.h"
#include "../task/deoglAddToRenderTask.h"
#include "../task/deoglRenderTask.h"
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
#include "../../debug/deoglDebugTraceGroup.h"
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
#include "../../renderthread/deoglRTChoices.h"
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
#define LOG_SIZE_CHANGE(s,o,n) GetRenderThread().GetLogger().LogInfoFormat(\
	"Point.%s: Size changed from %d to %d (light=%p fbotarget=%p)", \
	s, o, n->GetSize(), &light, plan.GetFBOTarget());
#else
#define LOG_SIZE_CHANGE(s,o,n)
#endif



// Structs
////////////

deoglRenderLightPoint::sShadowDepthMaps::sShadowDepthMaps() :
shadow1Solid( nullptr ),
shadow1Transp( nullptr ),
shadow2Solid( nullptr ),
shadow2Transp( nullptr ){
}

deoglRenderLightPoint::sShadowParams::sShadowParams() :
solid( false ),
transparentStaticShadow( false ),
transparentDynamicShadow( false ),
collideList1( nullptr ),
collideList2( nullptr ),
solidShadowMapSize( 0 ),
transpShadowMapSize( 0 ),
ambientMapSize( 0 ),
withTransparent( false ),
shadowScale( 1.0f ),
shadowOffset( 0.0f ),
lodMaxPixelError( 1 ),
lodOffset( 0 ){
}


// Class deoglRenderLightPoint
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderLightPoint::deoglRenderLightPoint( deoglRenderThread &renderThread,
deoglRTRenderers &renderers ) :
deoglRenderLightBase( renderThread )
{
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	deoglPipelineManager &pipelineManager = renderThread.GetPipelineManager();
	const bool useInverseDepth = renderThread.GetChoices().GetUseInverseDepth();
	const bool renderCubeGS = renderThread.GetChoices().GetRenderCubeGS();
	const float smOffsetScale = renderThread.GetConfiguration().GetShadowMapOffsetScale();
	const float smOffsetBias = renderThread.GetConfiguration().GetShadowMapOffsetBias();
	deoglPipelineConfiguration pipconf;
	deoglShaderSources *sources;
	deoglShaderDefines defines;
	
	try{
		// box boundary
		pipconf.Reset();
		pipconf.SetMasks( true, true, true, true, false );
		
		sources = shaderManager.GetSourcesNamed( "DefRen Light BoxBoundary" );
		
		defines.SetDefines( "DEPTH_INPUT" );
		defines.SetDefines( "DEPTH_CUBEMAP" );
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineBoxBoundary1 = pipelineManager.GetWith( pipconf );
		
		defines.SetDefines( "AMBIENT_MAP" );
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineBoxBoundary1Ambient = pipelineManager.GetWith( pipconf );
		defines.RemoveAllDefines();
		
		pipconf.SetShader( renderThread, sources, defines );
		pPipelineBoxBoundary2 = pipelineManager.GetWith( pipconf );
		defines.RemoveAllDefines();
		
		
		
		// occlusion map
		pipconf.Reset();
		pipconf.SetMasks( false, false, false, false, true );
		pipconf.EnableDepthTestLessEqual();
		pipconf.SetClipControl( useInverseDepth );
		pipconf.EnableCulling( false );
		pipconf.EnablePolygonOffset( useInverseDepth ? -smOffsetScale : smOffsetScale, -smOffsetBias );
		
		renderers.GetOcclusion().AddOccMapDefines( defines );
		defines.SetDefines( "DEPTH_DISTANCE" );
		pipconf.SetShader( renderThread, "DefRen Occlusion OccMap", defines );
		pipconf.SetSPBInstanceIndexBase( 0 );
		pPipelineOccMap = pipelineManager.GetWith( pipconf, true );
		defines.RemoveAllDefines();
		
		// occlusion map cube
		if( renderCubeGS ){
			renderThread.GetShader().SetCommonDefines( defines );
			renderers.GetOcclusion().AddOccMapDefines( defines );
			defines.SetDefines( "DEPTH_DISTANCE", "GS_RENDER_CUBE", "GS_RENDER_CUBE_CULLING" );
			
			pipconf.SetShader( renderThread, "DefRen Occlusion OccMap Cube", defines );
			pipconf.SetSPBInstanceIndexBase( 0 );
			pPipelineOccMapCube = pipelineManager.GetWith( pipconf, true );
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
		
		pDebugInfoSolid.TakeOver( new deoglDebugInformation( "Point", colorText, colorBgUp ) );
		pDebugInfoTransparent.TakeOver( new deoglDebugInformation( "Point", colorText, colorBgUp ) );
		
		
		
		pDebugInfoSolidDetail.TakeOver( new deoglDebugInformation( "Light Point Solid", colorText, colorBg ) );
		
		pDebugInfoSolidShadow.TakeOver( new deoglDebugInformation( "Shadow", colorText, colorBgSub ) );
		pDebugInfoSolidDetail->GetChildren().Add( pDebugInfoSolidShadow );
		
		pDebugInfoSolidShadowClear.TakeOver( new deoglDebugInformation( "Clear", colorText, colorBgSub2 ) );
		pDebugInfoSolidShadow->GetChildren().Add( pDebugInfoSolidShadowClear );
		
		pDebugInfoSolidShadowClearTransp.TakeOver( new deoglDebugInformation( "Clear Transp", colorText, colorBgSub2 ) );
		pDebugInfoSolidShadow->GetChildren().Add( pDebugInfoSolidShadowClearTransp );
		
		pDebugInfoSolidShadowFace.TakeOver( new deoglDebugInformation( "Face", colorText, colorBgSub2 ) );
		pDebugInfoSolidShadow->GetChildren().Add( pDebugInfoSolidShadowFace );
		
		pDebugInfoSolidShadowFaceClear.TakeOver( new deoglDebugInformation( "Clear", colorText, colorBgSub3 ) );
		pDebugInfoSolidShadowFace->GetChildren().Add( pDebugInfoSolidShadowFaceClear );
		
		pDebugInfoSolidShadowFaceTask.TakeOver( new deoglDebugInformation( "Task", colorText, colorBgSub3 ) );
		pDebugInfoSolidShadowFace->GetChildren().Add( pDebugInfoSolidShadowFaceTask );
		
		pDebugInfoSolidShadowFaceRender.TakeOver( new deoglDebugInformation( "Render", colorText, colorBgSub3 ) );
		pDebugInfoSolidShadowFace->GetChildren().Add( pDebugInfoSolidShadowFaceRender );
		
		pDebugInfoSolidShadowFaceTranspClear.TakeOver( new deoglDebugInformation( "Transp Clear", colorText, colorBgSub3 ) );
		pDebugInfoSolidShadowFace->GetChildren().Add( pDebugInfoSolidShadowFaceTranspClear );
		
		pDebugInfoSolidShadowFaceTranspTask.TakeOver( new deoglDebugInformation( "Transp Task", colorText, colorBgSub3 ) );
		pDebugInfoSolidShadowFace->GetChildren().Add( pDebugInfoSolidShadowFaceTranspTask );
		
		pDebugInfoSolidShadowFaceTranspRender.TakeOver( new deoglDebugInformation( "Transp Render", colorText, colorBgSub3 ) );
		pDebugInfoSolidShadowFace->GetChildren().Add( pDebugInfoSolidShadowFaceTranspRender );
		
		pDebugInfoSolidLight.TakeOver( new deoglDebugInformation( "Light", colorText, colorBgSub ) );
		pDebugInfoSolidDetail->GetChildren().Add( pDebugInfoSolidLight );
		
		
		
		pDebugInfoTransparentDetail.TakeOver( new deoglDebugInformation( "Light Point Transp", colorText, colorBg ) );
		
		pDebugInfoTransparentShadow.TakeOver( new deoglDebugInformation( "Shadow", colorText, colorBgSub ) );
		pDebugInfoTransparentDetail->GetChildren().Add( pDebugInfoTransparentShadow );
		
		pDebugInfoTransparentShadowClear.TakeOver( new deoglDebugInformation( "Clear", colorText, colorBgSub2 ) );
		pDebugInfoTransparentShadow->GetChildren().Add( pDebugInfoTransparentShadowClear );
		
		pDebugInfoTransparentShadowClearTransp.TakeOver( new deoglDebugInformation( "Clear Transp", colorText, colorBgSub2 ) );
		pDebugInfoTransparentShadow->GetChildren().Add( pDebugInfoTransparentShadowClearTransp );
		
		pDebugInfoTransparentShadowFace.TakeOver( new deoglDebugInformation( "Face", colorText, colorBgSub2 ) );
		pDebugInfoTransparentShadow->GetChildren().Add( pDebugInfoTransparentShadowFace );
		
		pDebugInfoTransparentShadowFaceClear.TakeOver( new deoglDebugInformation( "Clear", colorText, colorBgSub3 ) );
		pDebugInfoTransparentShadowFace->GetChildren().Add( pDebugInfoTransparentShadowFaceClear );
		
		pDebugInfoTransparentShadowFaceTask.TakeOver( new deoglDebugInformation( "Task", colorText, colorBgSub3 ) );
		pDebugInfoTransparentShadowFace->GetChildren().Add( pDebugInfoTransparentShadowFaceTask );
		
		pDebugInfoTransparentShadowFaceRender.TakeOver( new deoglDebugInformation( "Render", colorText, colorBgSub3 ) );
		pDebugInfoTransparentShadowFace->GetChildren().Add( pDebugInfoTransparentShadowFaceRender );
		
		pDebugInfoTransparentShadowFaceTranspClear.TakeOver( new deoglDebugInformation( "Transp Clear", colorText, colorBgSub3 ) );
		pDebugInfoTransparentShadowFace->GetChildren().Add( pDebugInfoTransparentShadowFaceTranspClear );
		
		pDebugInfoTransparentShadowFaceTranspTask.TakeOver( new deoglDebugInformation( "Transp Task", colorText, colorBgSub3 ) );
		pDebugInfoTransparentShadowFace->GetChildren().Add( pDebugInfoTransparentShadowFaceTranspTask );
		
		pDebugInfoTransparentShadowFaceTranspRender.TakeOver( new deoglDebugInformation( "Transp Render", colorText, colorBgSub3 ) );
		pDebugInfoTransparentShadowFace->GetChildren().Add( pDebugInfoTransparentShadowFaceTranspRender );
		
		pDebugInfoTransparentLight.TakeOver( new deoglDebugInformation( "Light", colorText, colorBgSub ) );
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
	const bool useAmbient = planLight.GetUseAmbient();
	
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
	
	// first down sampling step with implicit position calculation from the shadow map
	deoglShaderCompiled *shader;
	
	if( useAmbient && scambient.GetStaticCubeMap() ){
		pPipelineBoxBoundary1Ambient->Activate();
		shader = pPipelineBoxBoundary1Ambient->GetGlShader()->GetCompiled();
		
	}else{
		pPipelineBoxBoundary1->Activate();
		shader = pPipelineBoxBoundary1->GetGlShader()->GetCompiled();
	}
	
	OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
	SetViewport( size, size );
	
	renderThread.GetFramebuffer().Activate( boundaryMap.GetFBOAt( mipMapLevel ) );
	
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
	if( useAmbient && scambient.GetStaticCubeMap() ){
		tsmgr.EnableCubeMap( 3, *scambient.GetStaticCubeMap(), GetSamplerClampNearest() );
	}
	
	OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
	//ogl.LogInfoFormat( "Point BoxBoundary %p Step1 size(%i) mipMaplevel(%i)
	//	depthScale(%f) depthOffset(%f)", &light, size, mipMapLevel, depthScale, depthOffset );
	
	// down sampling to 1x1 using mip map levels
	pPipelineBoxBoundary2->Activate();
	
	pPipelineBoxBoundary2->GetGlShader()->GetCompiled()->SetParameterFloat( spbbQuadParams, 1.0f, 1.0f, 0.0f, 0.0f );
	
	tsmgr.EnableTexture( 0, *boundaryMap.GetTextureMin(), GetSamplerClampNearest() );
	tsmgr.EnableTexture( 1, *boundaryMap.GetTextureMax(), GetSamplerClampNearest() );
	tsmgr.DisableStagesAbove( 1 );
	
	while( size > 1 ){
		size >>= 1;
		mipMapLevel++;
		
		SetViewport( size, size );
		
		renderThread.GetFramebuffer().Activate( boundaryMap.GetFBOAt( mipMapLevel ) );
		
		pPipelineBoxBoundary2->GetGlShader()->GetCompiled()->SetParameterInt( spbbMipMapLevel, mipMapLevel - 1 );
		
		OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
		//ogl.LogInfoFormat( "BoxBoundary %p Step2 size(%i) mipMaplevel(%i)", &light, size, mipMapLevel );
	}
	
	pglBindVertexArray( 0 );
	
	// fetch the result of the 1x1 lod level
	decVector boundaryMin, boundaryMax;
	
	boundaryMap.GetResult( boundaryMin, boundaryMax );
	
	boundaryMin -= decVector( 0.01f, 0.01f, 0.01f ); // just to make sure the box is not too small
	boundaryMax += decVector( 0.01f, 0.01f, 0.01f ); // just to make sure the box is not too small
	light.SetLightVolumeCropBox( new decShapeBox(
		( boundaryMax - boundaryMin ) * 0.5f, ( boundaryMin + boundaryMax ) * 0.5f ) );
	
	/*{
	const decDVector dlp( light.GetMatrix().GetPosition() );
	GetRenderThread().GetLogger().LogInfoFormat( "Point BoxBoundary %p (%g,%g,%g) min(%g,%g,%g) max(%g,%g,%g)",
		&light, dlp.x, dlp.y, dlp.z, boundaryMin.x, boundaryMin.y, boundaryMin.z,
		boundaryMax.x, boundaryMax.y, boundaryMax.z );
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
	const deoglDebugTraceGroup debugTrace( renderThread, "LightPoint.RenderLight" );
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglShadowCaster &shadowCaster = *light.GetShadowCaster();
	deoglSCTransparent &sctransp = shadowCaster.GetTransparent();
	deoglSCAmbient &scambient = shadowCaster.GetAmbient();
	deoglRenderPlanDebug * const planDebug = plan.GetDebug();
	deoglSCSolid &scsolid = shadowCaster.GetSolid();
	deoglShadowCaster::eShadowTypes shadowType = shadowCaster.GetShadowType();
	const bool useTemporary = planLight.GetUseShadowTemporary();
	const bool useAmbient = planLight.GetUseAmbient();
	const bool useShadow = planLight.GetUseShadow();
	
	if( useShadow && useTemporary ){
		shadowType = deoglShadowCaster::estDynamicOnly;
	}
	
	// determine if we need transparent shadow casting
	sShadowParams shadowParams;
	shadowParams.solid = solid;
	
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
				
				shadowParams.transparentStaticShadow = true;
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
				
				shadowParams.transparentDynamicShadow = true;
				break;
			}
		//}
	}
	
	// debug
	if( solid && plan.GetRenderPassNumber() == 1 && planDebug ){
		planDebug->IncrementRenderedLights();
	}
	
	// calculate light volume matrices
	shadowParams.matrixProjection = defren.CreateFrustumDMatrix( 1, 1, DEG2RAD * 90.0f,
		1.0f, OGL_REN_LIG_ZNEAR, light.GetRange() );
	
	deoglCubeMap *texSolidDepth1 = NULL;
	deoglCubeMap *texSolidDepth2 = NULL;
	deoglCubeMap *texTranspDepth1 = NULL;
	deoglCubeMap *texTranspColor1 = NULL;
	deoglCubeMap *texTranspDepth2 = NULL;
	deoglCubeMap *texTranspColor2 = NULL;
	deoglCubeMap *texAmbient1 = NULL;
	deoglCubeMap *texAmbient2 = NULL;
	
	if( useShadow ){
		DebugTimer2Reset( plan, true );
		
		RenderShadows( planLight, shadowParams );
		
		// switch back to our framebuffer
		RestoreFBO( plan );
		SetViewport( plan  );
		
		// determine shadow maps to use
		switch( shadowType ){
		case deoglShadowCaster::estStaticOnly:
			texSolidDepth1 = scsolid.GetStaticCubeMap();
			if( shadowParams.transparentStaticShadow ){
				texTranspDepth1 = sctransp.GetStaticShadowCubeMap();
				texTranspColor1 = sctransp.GetStaticColorCubeMap();
			}
			if( useAmbient ){
				texAmbient1 = scambient.GetStaticCubeMap();
			}
			break;
			
		case deoglShadowCaster::estDynamicOnly:
			if( useTemporary ){
				texSolidDepth1 = scsolid.GetTemporaryCubeMap()->GetCubeMap();
				if( shadowParams.transparentDynamicShadow ){
					texTranspDepth1 = sctransp.GetTemporaryShadowCubeMap()->GetCubeMap();
					texTranspColor1 = sctransp.GetTemporaryColorCubeMap()->GetCubeMap();
				}
				if( useAmbient ){
					texAmbient1 = scambient.GetTemporaryCubeMap()->GetCubeMap();
				}
				
			}else{
				texSolidDepth1 = scsolid.GetDynamicCubeMap();
				if( shadowParams.transparentDynamicShadow ){
					texTranspDepth1 = sctransp.GetDynamicShadowCubeMap();
					texTranspColor1 = sctransp.GetDynamicColorCubeMap();
				}
				if( useAmbient ){
					texAmbient1 = scambient.GetDynamicCubeMap();
				}
			}
			break;
			
		case deoglShadowCaster::estStaticAndDynamic:
			texSolidDepth1 = scsolid.GetStaticCubeMap();
			if( shadowParams.transparentStaticShadow ){
				texTranspDepth1 = sctransp.GetStaticShadowCubeMap();
				texTranspColor1 = sctransp.GetStaticColorCubeMap();
			}
			if( useAmbient ){
				texAmbient1 = scambient.GetStaticCubeMap();
			}
			
			texSolidDepth2 = scsolid.GetDynamicCubeMap();
			if( shadowParams.transparentDynamicShadow ){
				if( texTranspDepth1 ){
					texTranspDepth2 = sctransp.GetDynamicShadowCubeMap();
					
				}else{
					texTranspDepth1 = sctransp.GetDynamicShadowCubeMap();
				}
				
				if( texTranspColor1 ){
					texTranspColor2 = sctransp.GetDynamicColorCubeMap();
					
				}else{
					texTranspColor1 = sctransp.GetDynamicColorCubeMap();
				}
			}
			if( useAmbient ){
				texAmbient2 = scambient.GetDynamicCubeMap();
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
		
	}else{
		if( giState ){
			// gi state lighting changes FBO and other parameters
			RestoreFBO( plan );
			SetViewport( plan );
		}
	}
	
DEBUG_RESET_TIMER
	// set pipeline
	const int pipmods = pPipelineModifiers ( planLight, solid, texAmbient1 );
	const deoglLightPipeline *pipeline;
	
	if( useShadow ){
		if( texSolidDepth2 ){
			if( texTranspDepth2 ){
				pipeline = &light.GetPipelines().GetWithRef( deoglLightPipelines::etSolid2Transp2, pipmods );
				
			}else if( texTranspDepth1 ){
				pipeline = &light.GetPipelines().GetWithRef( deoglLightPipelines::etSolid2Transp1, pipmods );
				
			}else{
				pipeline = &light.GetPipelines().GetWithRef( deoglLightPipelines::etSolid2, pipmods );
			}
			
		}else{
			if( texTranspDepth1 ){
				pipeline = &light.GetPipelines().GetWithRef( deoglLightPipelines::etSolid1Transp1, pipmods );
				
			}else{
				pipeline = &light.GetPipelines().GetWithRef( deoglLightPipelines::etSolid1, pipmods );
			}
		}
		
	}else{
		pipeline = &light.GetPipelines().GetWithRef( deoglLightPipelines::etNoShadow, pipmods );
	}
	
	// set program parameters
	deoglSPBlockUBO * const spbInstance = light.GetInstanceParameterBlock();
	deoglSPBlockUBO * const spbLight = light.GetLightParameterBlock();
	DEASSERT_NOTNULL( spbInstance )
	DEASSERT_NOTNULL( spbLight )
	
	deoglLightShader &lightShader = pipeline->GetShader();
	UpdateLightParamBlock( lightShader, *spbLight, planLight );
	
	sShadowDepthMaps shadowDepthMaps;
	shadowDepthMaps.shadow1Solid = texSolidDepth1;
	shadowDepthMaps.shadow1Transp = texTranspDepth1;
	shadowDepthMaps.shadow1TranspColor = texTranspColor1;
	shadowDepthMaps.shadow2Solid = texSolidDepth2;
	shadowDepthMaps.shadow2Transp = texTranspDepth2;
	shadowDepthMaps.shadow2TranspColor = texTranspColor2;
	shadowDepthMaps.shadow1Ambient = texAmbient1;
	shadowDepthMaps.shadow2Ambient = texAmbient2;
	UpdateInstanceParamBlock( lightShader, *spbInstance, planLight, shadowDepthMaps );
	
	// light geometry
	if( lightGeometry ){
		pipeline->GetPipeline()->Activate();
		renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
		spbLight->Activate();
		spbInstance->Activate();
		
		ActivateTextures( planLight, lightShader, shadowDepthMaps );
		
		pglBindVertexArray( light.GetLightVolume()->GetVAO() );
		if( plan.GetRenderStereo() && renderThread.GetChoices().GetRenderStereoVSLayer() ){
			const GLint first[ 2 ] = { 0, 0 };
			const GLsizei count[ 2 ] = { light.GetLightVolume()->GetPointCount(), light.GetLightVolume()->GetPointCount() };
			OGL_CHECK( renderThread, pglMultiDrawArrays( GL_TRIANGLES, first, count, 2 ) );
			
		}else{
			OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLES, 0, light.GetLightVolume()->GetPointCount() ) );
		}
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
		
		pipeline = nullptr;
		if( useShadow ){
			if( texSolidDepth2 ){
				pipeline = light.GetPipelines().GetWith( deoglLightPipelines::etGIRaySolid2, 0 );
				
			}else{
				pipeline = light.GetPipelines().GetWith( deoglLightPipelines::etGIRaySolid1, 0 );
			}
			
		}else{
			pipeline = light.GetPipelines().GetWith( deoglLightPipelines::etGIRayNoShadow, 0 );
		}
		
		if( pipeline ){
			pipeline->GetPipeline()->Activate();
			
			// WARNING always non-stereo!
			renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
			
			spbLight->Activate();
			spbInstance->Activate();
			
			ActivateTextures( planLight, pipeline->GetShader(), shadowDepthMaps );
			
			RenderFullScreenQuadVAO();
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
			deoglRenderTarget * const target2 = light.GetLightCanvas()->GetRenderTarget();
			if( target2 ){
				tuc.EnableTexture( target2->GetTexture(), &GetSamplerClampLinear() );
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

void deoglRenderLightPoint::RenderShadows( deoglRenderPlanLight &planLight, sShadowParams &shadowParams ){
	deoglRenderPlan &plan = planLight.GetPlan();
	deoglRLight &light = *planLight.GetLight()->GetLight();
	deoglRenderThread &renderThread = GetRenderThread();
	deoglShadowMapper &shadowMapper = renderThread.GetShadowMapper();
	deoglShadowCaster &shadowCaster = *light.GetShadowCaster();
	deoglShadowCaster::eShadowTypes shadowType = shadowCaster.GetShadowType();
	deoglSCTransparent &sctransp = shadowCaster.GetTransparent();
	deoglSCAmbient &scambient = shadowCaster.GetAmbient();
	deoglSCSolid &scsolid = shadowCaster.GetSolid();
	const bool refilterShadow = planLight.GetRefilterShadows();
	const bool useTemporary = planLight.GetUseShadowTemporary();
	const bool useAmbient = planLight.GetUseAmbient();
	bool updateBoxBoundary = false;
	
	// get map sizes as calculated by render plan
	const int staticShadowMapSize = planLight.GetShadowSizeStatic();
	const int staticTranspShadowMapSize = planLight.GetTranspShadowSizeStatic();
	const int staticAmbientMapSize = planLight.GetAmbientShadowSizeStatic();
	
	const int dynamicShadowMapSize = planLight.GetShadowSizeDynamic();
	const int dynamicTranspShadowMapSize = planLight.GetTranspShadowSizeDynamic();
	const int dynamicAmbientMapSize = planLight.GetAmbientShadowSizeDynamic();
	
	// if layer mask restriction is used dynamic only shadows have to be used to filter properly
	if( useTemporary ){
		shadowType = deoglShadowCaster::estDynamicOnly;
	}
	
	// static shadow map
	if( shadowType == deoglShadowCaster::estStaticOnly
	||  shadowType == deoglShadowCaster::estStaticAndDynamic ){
		// check if an update is required
		bool requiresUpdate = false;
		
		if( scsolid.GetStaticCubeMap() ){
			if( staticShadowMapSize != scsolid.GetStaticCubeMap()->GetSize() ){
				LOG_SIZE_CHANGE("scsolid.static", staticShadowMapSize, scsolid.GetStaticCubeMap())
				scsolid.DropStatic();
				requiresUpdate = true;
			}
			
		}else{
			requiresUpdate = true;
		}
		
		if( useAmbient ){
			if( scambient.GetStaticCubeMap() ){
				if( scambient.GetStaticCubeMap()->GetSize() != staticAmbientMapSize ){
					LOG_SIZE_CHANGE("scambient.static", staticAmbientMapSize, scambient.GetStaticCubeMap())
					scambient.DropStatic();
					requiresUpdate = true;
				}
				
			}else{
				requiresUpdate = true;
			}
		}
		
		if( shadowParams.transparentStaticShadow ){
			if( sctransp.GetStaticShadowCubeMap() ){
				if( staticTranspShadowMapSize != sctransp.GetStaticShadowCubeMap()->GetSize() ){
					LOG_SIZE_CHANGE("sctransp.static", staticTranspShadowMapSize, sctransp.GetStaticShadowCubeMap())
					sctransp.DropStatic();
					requiresUpdate = true;
				}
				
			}else{
				requiresUpdate = true;
			}
			
			if( sctransp.GetStaticColorCubeMap() ){
				if( staticTranspShadowMapSize != sctransp.GetStaticColorCubeMap()->GetSize() ){
					LOG_SIZE_CHANGE("sctransp.staticColor", staticTranspShadowMapSize, sctransp.GetStaticColorCubeMap())
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
			if( shadowParams.transparentStaticShadow ){
				shadowMapper.SetForeignTransparentDepthCubeMap( sctransp.ObtainStaticShadowCubeMapWithSize( staticTranspShadowMapSize ) );
				shadowMapper.SetForeignTransparentColorCubeMap( sctransp.ObtainStaticColorCubeMapWithSize( staticTranspShadowMapSize ) );
			}
			
			shadowParams.collideList1 = light.GetStaticCollideList();
			shadowParams.collideList2 = nullptr;
			shadowParams.solidShadowMapSize = staticShadowMapSize;
			shadowParams.transpShadowMapSize = staticTranspShadowMapSize;
			shadowParams.ambientMapSize = staticAmbientMapSize;
			shadowParams.withTransparent = shadowParams.transparentStaticShadow;
			shadowParams.shadowScale = shadowCaster.GetStaticScale();
			shadowParams.shadowOffset = shadowCaster.GetStaticOffset();
			
			shadowParams.lodMaxPixelError = 2;
			shadowParams.lodOffset = 0;
			
			RenderShadowMaps( planLight, shadowMapper, shadowParams );
			
			shadowMapper.DropForeignCubeMaps();
			shadowMapper.DropForeignTextures();
			
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
			if( useAmbient ){
				shadowMapper.SetForeignAmbientCubeMap( scambient.ObtainStaticCubeMapWithSize( staticAmbientMapSize ) );
				
				RenderAmbientMap( planLight, shadowMapper, shadowParams );
				
				shadowMapper.DropForeignAmbientCubeMaps();
				shadowMapper.DropForeignAmbientTextures();
			}
			
			if( ! light.GetLightVolumeCropBox() ){
				updateBoxBoundary = true;
			}
DEBUG_PRINT_TIMER( "Shadow Static" );
		}
		
		scsolid.ResetLastUseStatic();
		if( shadowParams.transparentStaticShadow ){
			sctransp.ResetLastUseStatic();
		}
		if( useAmbient ){
			scambient.ResetLastUseStatic();
		}
		
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
		
		// drop shadow map if size changed
		if( ! useTemporary ){
			if( scsolid.GetDynamicCubeMap() && scsolid.GetDynamicCubeMap()->GetSize() != dynamicShadowMapSize ){
				LOG_SIZE_CHANGE("scsolid.dynamic", dynamicShadowMapSize, scsolid.GetDynamicCubeMap())
				scsolid.DropDynamic();
			}
			if( useAmbient && scambient.GetDynamicCubeMap() && scambient.GetDynamicCubeMap()->GetSize() != dynamicAmbientMapSize ){
				LOG_SIZE_CHANGE("scambient.dynamic", dynamicAmbientMapSize, scambient.GetDynamicCubeMap())
				scambient.DropDynamic();
			}
			if( shadowParams.transparentDynamicShadow && sctransp.GetDynamicShadowCubeMap()
			&& sctransp.GetDynamicShadowCubeMap()->GetSize() != dynamicTranspShadowMapSize ){
				LOG_SIZE_CHANGE("sctransp.dynamic", dynamicTranspShadowMapSize, sctransp.GetDynamicShadowCubeMap())
				sctransp.DropDynamic();
			}
		}
		
		// render shadow map if dirty. the dirty flag is reset each frame update. this ensures
		// dynamic shadow maps are rendered once per frame update
		bool requiresUpdate = false;
		
		if( useTemporary ){
			requiresUpdate = ! scsolid.GetTemporaryCubeMap();
			if( useAmbient ){
				requiresUpdate |= ! scambient.GetTemporaryCubeMap();
			}
			if( shadowParams.transparentDynamicShadow ){
				requiresUpdate |= ! sctransp.GetTemporaryShadowCubeMap()
					|| ! sctransp.GetTemporaryColorCubeMap();
			}
			
		}else{
			requiresUpdate = ! scsolid.GetDynamicCubeMap() || scsolid.GetDirtyDynamic();
			if( useAmbient ){
				requiresUpdate |= ! scambient.GetDynamicCubeMap() || scambient.GetDirtyDynamic();
			}
			if( shadowParams.transparentDynamicShadow ){
				requiresUpdate |= ! sctransp.GetDynamicShadowCubeMap()
					|| ! sctransp.GetDynamicColorCubeMap() || sctransp.GetDirtyDynamic();
			}
		}
		
		
		if( requiresUpdate ){
DEBUG_RESET_TIMER
			if( useTemporary ){
				shadowMapper.SetForeignSolidDepthCubeMap(
					scsolid.ObtainTemporaryCubeMapWithSize( dynamicShadowMapSize )->GetCubeMap() );
				
				if( shadowParams.transparentDynamicShadow ){
					shadowMapper.SetForeignTransparentDepthCubeMap( sctransp.
						ObtainTemporaryShadowCubeMapWithSize( dynamicTranspShadowMapSize )->GetCubeMap() );
					shadowMapper.SetForeignTransparentColorCubeMap( sctransp.
						ObtainTemporaryColorCubeMapWithSize( dynamicTranspShadowMapSize )->GetCubeMap() );
				}
				
			}else{
				shadowMapper.SetForeignSolidDepthCubeMap(
					scsolid.ObtainDynamicCubeMapWithSize( dynamicShadowMapSize ) );
				
				if( shadowParams.transparentDynamicShadow ){
					shadowMapper.SetForeignTransparentDepthCubeMap( sctransp.
						ObtainDynamicShadowCubeMapWithSize( dynamicTranspShadowMapSize ) );
					shadowMapper.SetForeignTransparentColorCubeMap( sctransp.
						ObtainDynamicColorCubeMapWithSize( dynamicTranspShadowMapSize ) );
				}
			}
			
			shadowParams.collideList1 = clist1;
			shadowParams.collideList2 = clist2;
			shadowParams.solidShadowMapSize = dynamicShadowMapSize;
			shadowParams.transpShadowMapSize = dynamicTranspShadowMapSize;
			shadowParams.ambientMapSize = dynamicAmbientMapSize;
			shadowParams.withTransparent = shadowParams.transparentDynamicShadow;
			shadowParams.shadowScale = shadowCaster.GetStaticScale();
			shadowParams.shadowOffset = shadowCaster.GetStaticOffset();
			
			shadowParams.lodMaxPixelError = 2;
			shadowParams.lodOffset = 0;
			
			RenderShadowMaps( planLight, shadowMapper, shadowParams );
			
			shadowMapper.DropForeignCubeMaps();
			shadowMapper.DropForeignTextures();
			
			if( ! useTemporary ){
				scsolid.SetDirtyDynamic( false );
				if( shadowParams.transparentDynamicShadow ){
					sctransp.SetDirtyDynamic( false );
				}
			}
DEBUG_PRINT_TIMER( "Shadow Dynamic Transparent" );
			
			// ambient map
			if( useAmbient ){
				if( useTemporary ){
					shadowMapper.SetForeignAmbientCubeMap(
						scambient.ObtainTemporaryCubeMapWithSize( dynamicAmbientMapSize )->GetCubeMap() );
					
				}else{
					shadowMapper.SetForeignAmbientCubeMap( scambient.ObtainDynamicCubeMapWithSize( dynamicAmbientMapSize ) );
				}
				
				RenderAmbientMap( planLight, shadowMapper, shadowParams );
				
				shadowMapper.DropForeignAmbientCubeMaps();
				shadowMapper.DropForeignAmbientTextures();
				
				if( ! useTemporary ){
					scambient.SetDirtyDynamic( false );
				}
			}
		}
		
		if( ! useTemporary ){
			scsolid.ResetLastUseDynamic();
			if( shadowParams.transparentStaticShadow ){
				sctransp.ResetLastUseDynamic();
			}
			if( useAmbient ){
				scambient.ResetLastUseDynamic();
			}
		}
	}
	
	// update box boundary if required
	if( updateBoxBoundary ){
DEBUG_RESET_TIMER
		CalculateBoxBoundary( planLight );
DEBUG_PRINT_TIMER( "Boundary Box" );
	}
}



bool HACK1 = false;

void deoglRenderLightPoint::RenderShadowMaps( deoglRenderPlanLight &planLight,
deoglShadowMapper &shadowMapper, const sShadowParams &shadowParams ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "LightPoint.RenderShadowMaps" );
	deoglAddToRenderTask &addToRenderTask = renderThread.GetRenderers().GetLight().GetAddToRenderTask();
	deoglRenderTask &renderTask = renderThread.GetRenderers().GetLight().GetRenderTask();
	deoglRenderGeometry &rengeom = renderThread.GetRenderers().GetGeometry();
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
	const float smOffsetBias = config.GetDistShadowBias() * shadowParams.shadowScale;
	
	const decVector lightPosition( light.GetMatrix().GetPosition() - plan.GetWorld()->GetReferencePosition() );
	int cmf;
	
	// calculate object render cube face special parameter. optimizes rendering by skipping
	// object faces on cube map faces they are not visible on
	const decDVector cubePosition( light.GetMatrix().GetPosition() );
	if( shadowParams.collideList1 ){
		shadowParams.collideList1->UpdateCubeFaceMasks( cubePosition );
	}
	if( shadowParams.collideList2 ){
		shadowParams.collideList2->UpdateCubeFaceMasks( cubePosition );
	}
	
	// calculate lod level to use
	deoglLODCalculator lodCalculator;
	lodCalculator.SetMaxPixelError( shadowParams.lodMaxPixelError );
	lodCalculator.SetLodOffset( shadowParams.lodOffset );
	
	if( shadowParams.collideList1 ){
		lodCalculator.SetComponentLODOmniDir( *shadowParams.collideList1, lightPosition, shadowParams.solidShadowMapSize );
	}
	if( shadowParams.collideList2 ){
		lodCalculator.SetComponentLODOmniDir( *shadowParams.collideList2, lightPosition, shadowParams.solidShadowMapSize );
	}
	
	// configuration
	const bool bugClearEntireCubeMap = renderThread.GetCapabilities().GetClearEntireCubeMap().Broken();
	const bool useGSRenderCube = renderThread.GetExtensions().SupportsGeometryShader()
		&& ! bugClearEntireCubeMap;
	
	// setup render parameters
	deoglSPBlockUBO * const renderParamBlock = renderThread.GetRenderers().GetLight().GetShadowPB();
	
	if( useGSRenderCube ){
		// NOTE Y axis is flipped compared to opengl. pCubeFaces takes care of this
		renderParamBlock->MapBuffer();
		try{
			renderParamBlock->SetParameterDataVec2( deoglSkinShader::erutDepthTransform,
				shadowParams.shadowScale, shadowParams.shadowOffset );
			renderParamBlock->SetParameterDataVec4( deoglSkinShader::erutDepthOffset,
				smOffsetScale, smOffsetBias, -smOffsetScale, -smOffsetBias );
			renderParamBlock->SetParameterDataMat4x4( deoglSkinShader::erutMatrixP,
				shadowParams.matrixProjection );
			
			for( cmf=0; cmf<6; cmf++ ){
				deoglCubeMap::CreateMatrixForFace( matrixCamera, lightPosition, pCubeFaces[ cmf ] );
				
				renderParamBlock->SetParameterDataArrayMat4x3(
					deoglSkinShader::erutMatrixV, cmf, matrixCamera );
				renderParamBlock->SetParameterDataArrayMat4x4(
					deoglSkinShader::erutMatrixVP, cmf, matrixCamera * shadowParams.matrixProjection );
				renderParamBlock->SetParameterDataArrayMat3x3(
					deoglSkinShader::erutMatrixVn, cmf, matrixCamera.GetRotationMatrix().Invert() );
			}
			
			renderParamBlock->SetParameterDataBVec4( deoglSkinShader::erutConditions1, false, false, false, false );
			
		}catch( const deException & ){
			renderParamBlock->UnmapBuffer();
			throw;
		}
		renderParamBlock->UnmapBuffer();
	}
	
	addToRenderTask.Reset();
	addToRenderTask.SetSolid( true );
	addToRenderTask.SetNoShadowNone( true );
	
	if( useGSRenderCube ){
		addToRenderTask.SetUseSpecialParamBlock( true );
		addToRenderTask.SetSkinPipelineType( deoglSkinTexturePipelines::etShadowDistanceCube );
		
	}else{
		addToRenderTask.SetSkinPipelineType( deoglSkinTexturePipelines::etShadowDistance );
	}
	
	const GLfloat clearColor[ 4 ] = { 1.0f, 0.0f, 0.0f, 0.0f };
	const GLfloat clearDepth = ( GLfloat )1.0f; // point light uses linear depth
	
	// render the solid shadow cube map
	if( ! bugClearEntireCubeMap ){
		pPipelineClearBuffers->Activate();
		shadowMapper.ActivateSolidCubeMap( shadowParams.solidShadowMapSize );
		OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
		
		if( shadowParams.solid ){
			DebugTimer3Sample( plan, *pDebugInfoSolidShadowClear, true );
			
		}else{
			DebugTimer3Sample( plan, *pDebugInfoTransparentShadowClear, true );
		}
	}
	
	if( useGSRenderCube ){
		// cube map activate already by clear
		DebugTimer4Reset( plan, false );
		
		renderTask.Clear();
		renderTask.SetRenderParamBlock( renderParamBlock );
		renderTask.SetUseSPBInstanceFlags( true );
		
		addToRenderTask.SetSolid( true );
		addToRenderTask.SetForceDoubleSided( true );
		if( shadowParams.collideList1 ){
			addToRenderTask.AddComponents( *shadowParams.collideList1 );
		}
		if( shadowParams.collideList2 ){
			addToRenderTask.AddComponents( *shadowParams.collideList2 );
		}
		
		addToRenderTask.SetForceDoubleSided( false );
		
		if( shadowParams.solid ){
			DebugTimer4Sample( plan, *pDebugInfoSolidShadowFaceTask, true );
			
		}else{
			DebugTimer4Sample( plan, *pDebugInfoTransparentShadowFaceTask, true );
		}
		
		renderTask.PrepareForRender();
// 			renderThread.GetLogger().LogInfo( "RenderLightPoint: render task" );
// 			renderTask.DebugPrint( renderThread.GetLogger() );
		rengeom.RenderTask( renderTask );
		
		if( shadowParams.solid ){
			DebugTimer4Sample( plan, *pDebugInfoSolidShadowFaceRender, true );
			DebugTimer3SampleCount( plan, *pDebugInfoSolidShadowFace, 1, false );
			
		}else{
			DebugTimer4Sample( plan, *pDebugInfoTransparentShadowFaceRender, true );
			DebugTimer3SampleCount( plan, *pDebugInfoTransparentShadowFace, 1, false );
		}
		
	}else{
		for( cmf=0; cmf<6; cmf++ ){
			DebugTimer4Reset( plan, false );
			deoglCubeMap::CreateMatrixForFace( matrixCamera, lightPosition, cmf );
			
			renderParamBlock->MapBuffer();
			try{
				renderParamBlock->SetParameterDataVec2( deoglSkinShader::erutDepthTransform,
					shadowParams.shadowScale, shadowParams.shadowOffset );
				renderParamBlock->SetParameterDataVec4( deoglSkinShader::erutDepthOffset,
					smOffsetScale, smOffsetBias, -smOffsetScale, -smOffsetBias );
				renderParamBlock->SetParameterDataMat4x4( deoglSkinShader::erutMatrixP,
					shadowParams.matrixProjection );
				
				renderParamBlock->SetParameterDataMat4x3(
					deoglSkinShader::erutMatrixV, matrixCamera );
				renderParamBlock->SetParameterDataMat4x4(
					deoglSkinShader::erutMatrixVP, matrixCamera * shadowParams.matrixProjection );
				renderParamBlock->SetParameterDataMat3x3(
					deoglSkinShader::erutMatrixVn, matrixCamera.GetRotationMatrix().Invert() );
				
				renderParamBlock->SetParameterDataBVec4( deoglSkinShader::erutConditions1, false, false, false, false );
				
			}catch( const deException & ){
				renderParamBlock->UnmapBuffer();
				throw;
			}
			renderParamBlock->UnmapBuffer();
			
			shadowMapper.ActivateSolidCubeMapFace( shadowParams.solidShadowMapSize, pCubeFaces[ cmf ] );
			
			if( bugClearEntireCubeMap ){
				pPipelineClearBuffers->Activate();
				OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
				OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
				
				if( shadowParams.solid ){
					DebugTimer4Sample( plan, *pDebugInfoSolidShadowFaceClear, true );
					
				}else{
					DebugTimer4Sample( plan, *pDebugInfoTransparentShadowFaceClear, true );
				}
			}
			
			renderTask.Clear();
			renderTask.SetRenderParamBlock( renderParamBlock );
			
			addToRenderTask.SetSolid( true );
			addToRenderTask.SetFilterCubeFace( cmf );
			addToRenderTask.SetForceDoubleSided( true );
			
			if( shadowParams.collideList1 ){
				addToRenderTask.AddComponents( *shadowParams.collideList1 );
			}
			if( shadowParams.collideList2 ){
				addToRenderTask.AddComponents( *shadowParams.collideList2 );
			}
			
			addToRenderTask.SetForceDoubleSided( false );
			
			if( shadowParams.solid ){
				DebugTimer4Sample( plan, *pDebugInfoSolidShadowFaceTask, true );
				
			}else{
				DebugTimer4Sample( plan, *pDebugInfoTransparentShadowFaceTask, true );
			}
			
			renderTask.PrepareForRender();
			rengeom.RenderTask( renderTask );
			
			if( shadowParams.solid ){
				DebugTimer4Sample( plan, *pDebugInfoSolidShadowFaceRender, true );
				DebugTimer3SampleCount( plan, *pDebugInfoSolidShadowFace, 1, false );
				
			}else{
				DebugTimer4Sample( plan, *pDebugInfoTransparentShadowFaceRender, true );
				DebugTimer3SampleCount( plan, *pDebugInfoTransparentShadowFace, 1, false );
			}
		}
	}
	
	// render the transparent shadow cube maps if required
	if( shadowParams.withTransparent ){
		addToRenderTask.SetSolid( false );
		
		if( ! bugClearEntireCubeMap ){
			pPipelineClearBuffers->Activate();
			shadowMapper.ActivateTransparentCubeMap( shadowParams.transpShadowMapSize );
			OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
			OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
			
			if( shadowParams.solid ){
				DebugTimer3Sample( plan, *pDebugInfoSolidShadowClearTransp, true );
				
			}else{
				DebugTimer3Sample( plan, *pDebugInfoTransparentShadowClearTransp, true );
			}
		}
		
		if( useGSRenderCube ){
			// cube map activate already by clear
			DebugTimer4Reset( plan, false );
			
			renderTask.Clear();
			renderTask.SetRenderParamBlock( renderParamBlock );
			
			if( shadowParams.collideList1 ){
				addToRenderTask.AddComponents( *shadowParams.collideList1 );
			}
			if( shadowParams.collideList2 ){
				addToRenderTask.AddComponents( *shadowParams.collideList2 );
			}
			
			if( shadowParams.solid ){
				DebugTimer4Sample( plan, *pDebugInfoSolidShadowFaceTranspTask, true );
				
			}else{
				DebugTimer4Sample( plan, *pDebugInfoTransparentShadowFaceTranspTask, true );
			}
			
			renderTask.PrepareForRender();
			rengeom.RenderTask( renderTask );
			
			if( shadowParams.solid ){
				DebugTimer4Sample( plan, *pDebugInfoSolidShadowFaceTranspRender, true );
				DebugTimer3Sample( plan, *pDebugInfoSolidShadowFace, false ); // because we added 1 already during solid
				
			}else{
				DebugTimer4Sample( plan, *pDebugInfoTransparentShadowFaceTranspRender, true );
				DebugTimer3Sample( plan, *pDebugInfoTransparentShadowFace, false ); // because we added 1 already during solid
			}
			
		}else{
			for( cmf=0; cmf<6; cmf++ ){
				DebugTimer4Reset( plan, false );
				deoglCubeMap::CreateMatrixForFace( matrixCamera, lightPosition, cmf );
				
				renderParamBlock->MapBuffer();
				try{
					renderParamBlock->SetParameterDataVec2( deoglSkinShader::erutDepthTransform,
						shadowParams.shadowScale, shadowParams.shadowOffset );
					renderParamBlock->SetParameterDataVec4( deoglSkinShader::erutDepthOffset,
						smOffsetScale, smOffsetBias, -smOffsetScale, -smOffsetBias );
					renderParamBlock->SetParameterDataMat4x4( deoglSkinShader::erutMatrixP,
						shadowParams.matrixProjection );
						
					renderParamBlock->SetParameterDataMat4x3(
						deoglSkinShader::erutMatrixV, matrixCamera );
					renderParamBlock->SetParameterDataMat4x4(
						deoglSkinShader::erutMatrixVP, matrixCamera * shadowParams.matrixProjection );
					
					renderParamBlock->SetParameterDataBVec4( deoglSkinShader::erutConditions1, false, false, false, false );
					
				}catch( const deException & ){
					renderParamBlock->UnmapBuffer();
					throw;
				}
				renderParamBlock->UnmapBuffer();
				
				shadowMapper.ActivateTransparentCubeMapFace( shadowParams.transpShadowMapSize, pCubeFaces[ cmf ] );
				if( bugClearEntireCubeMap ){
					pPipelineClearBuffers->Activate();
					OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
					OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
					
					if( shadowParams.solid ){
						DebugTimer4Sample( plan, *pDebugInfoSolidShadowFaceTranspClear, true );
						
					}else{
						DebugTimer4Sample( plan, *pDebugInfoTransparentShadowFaceTranspClear, true );
					}
				}
				
				renderTask.Clear();
				renderTask.SetRenderParamBlock( renderParamBlock );
				
				if( shadowParams.collideList1 ){
					addToRenderTask.AddComponents( *shadowParams.collideList1 );
				}
				if( shadowParams.collideList2 ){
					addToRenderTask.AddComponents( *shadowParams.collideList2 );
				}
				
				if( shadowParams.solid ){
					DebugTimer4Sample( plan, *pDebugInfoSolidShadowFaceTranspTask, true );
					
				}else{
					DebugTimer4Sample( plan, *pDebugInfoTransparentShadowFaceTranspTask, true );
				}
				
				renderTask.PrepareForRender();
				rengeom.RenderTask( renderTask );
				
				if( shadowParams.solid ){
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
deoglShadowMapper &shadowMapper, const sShadowParams &shadowParams ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "LightPoint.RenderAmbientMap" );
	deoglAddToRenderTask &addToRenderTask = renderThread.GetRenderers().GetLight().GetAddToRenderTask();
	deoglRenderTask &renderTask = renderThread.GetRenderers().GetLight().GetRenderTask();
	deoglRenderGeometry &rengeom = renderThread.GetRenderers().GetGeometry();
	deoglRLight &light = *planLight.GetLight()->GetLight();
	deoglRenderPlan &plan = planLight.GetPlan();
//	const float smOffsetScale = config.GetDistShadowScale(); //config.GetShadowMapOffsetScale();
	//const float smOffsetBias = config.GetDistShadowBias() / ( float )( ( 1 << 24 ) - 1 ); // config.GetShadowMapOffsetBias();
//	const float smOffsetBias = config.GetDistShadowBias() / ( float )( ( 1 << 16 ) - 1 ); // config.GetShadowMapOffsetBias();
	decMatrix matrixCamera;
	
	const decVector lightPosition = ( light.GetMatrix().GetPosition() - plan.GetWorld()->GetReferencePosition() ).ToVector();
	int cmf;
	
	// the component VBOs have been update already by RenderShadowMaps. also the cube face
	// visibility has been calculated and the special parameter block updated.
	//UpdateComponentVBO( clist );
	
	// configuration
	const bool bugClearEntireCubeMap = renderThread.GetCapabilities().GetClearEntireCubeMap().Broken();
	const bool useGSRenderCube = renderThread.GetChoices().GetRenderCubeGS();
	
	// setup render parameters
	deoglSPBlockUBO * const renderParamBlock =
		renderThread.GetRenderers().GetLight().GetOccMapPB();
	
	if( useGSRenderCube ){
		// NOTE Y axis is flipped compared to opengl. pCubeFaces takes care of this
		renderParamBlock->MapBuffer();
		try{
			for( cmf=0; cmf<6; cmf++ ){
				deoglCubeMap::CreateMatrixForFace( matrixCamera, lightPosition, pCubeFaces[ cmf ] );
				
			renderParamBlock->SetParameterDataVec2( 3, shadowParams.shadowScale, shadowParams.shadowOffset );
			//renderParamBlock->SetParameterDataVec4( deoglSkinShader::erutDepthOffset,
			// 	smOffsetScale, smOffsetBias, -smOffsetScale, -smOffsetBias );
			
			renderParamBlock->SetParameterDataArrayMat4x3( 1, cmf, matrixCamera );
				renderParamBlock->SetParameterDataArrayMat4x4( 0, cmf, matrixCamera * shadowParams.matrixProjection );
			}
			
		}catch( const deException & ){
			renderParamBlock->UnmapBuffer();
			throw;
		}
		renderParamBlock->UnmapBuffer();
		
		// object render cube face special parameter have been already updated by RenderShadowMaps
	}
	
	addToRenderTask.Reset();
	addToRenderTask.SetSolid( true );
	addToRenderTask.SetNoShadowNone( true );
	
	if( useGSRenderCube ){
		addToRenderTask.SetUseSpecialParamBlock( true );
		addToRenderTask.SetEnforcePipeline( pPipelineOccMapCube );
		
	}else{
		addToRenderTask.SetEnforcePipeline( pPipelineOccMap );
	}
	
	// clear
	const GLfloat clearDepth = ( GLfloat )1.0f; // point light uses linear depth
	
	if( ! bugClearEntireCubeMap ){
		pPipelineClearBuffers->Activate();
		shadowMapper.ActivateAmbientCubeMap( shadowParams.ambientMapSize );
		OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
	}
	
	// render the solid shadow cube map
	if( useGSRenderCube ){
		// cube map activate already by clear
		renderTask.Clear();
		renderTask.SetRenderParamBlock( renderParamBlock );
		renderTask.SetUseSPBInstanceFlags( true );
		
		if( shadowParams.collideList1 ){
			addToRenderTask.AddOcclusionMeshes( *shadowParams.collideList1, false );
		}
		if( shadowParams.collideList2 ){
			addToRenderTask.AddOcclusionMeshes( *shadowParams.collideList2, false );
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
				
				renderParamBlock->SetParameterDataVec2( 3, shadowParams.shadowScale, shadowParams.shadowOffset );
				//renderParamBlock->SetParameterDataVec4( deoglSkinShader::erutDepthOffset,
				// 	smOffsetScale, smOffsetBias, -smOffsetScale, -smOffsetBias );
				
				renderParamBlock->SetParameterDataMat4x4( 0, matrixCamera * shadowParams.matrixProjection );
				renderParamBlock->SetParameterDataMat4x3( 1, matrixCamera );
				
			}catch( const deException & ){
				renderParamBlock->UnmapBuffer();
				throw;
			}
			renderParamBlock->UnmapBuffer();
			
			shadowMapper.ActivateAmbientCubeMapFace( shadowParams.ambientMapSize, pCubeFaces[ cmf ] );
			
			if( bugClearEntireCubeMap ){
				pPipelineClearBuffers->Activate();
				OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
			}
			
			renderTask.Clear();
			renderTask.SetRenderParamBlock( renderParamBlock );
			
			addToRenderTask.SetFilterCubeFace( cmf );
			if( shadowParams.collideList2 ){
				addToRenderTask.AddOcclusionMeshes( *shadowParams.collideList1, false );
			}
			if( shadowParams.collideList2 ){
				addToRenderTask.AddOcclusionMeshes( *shadowParams.collideList2, false );
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
	const deoglConfiguration &config = GetRenderThread().GetConfiguration();
	deoglRLight &light = *planLight.GetLight()->GetLight();
	const deoglRenderPlan &plan = planLight.GetPlan();
	
	const bool isDepthCompareLEqual = GetRenderThread().GetChoices().GetDepthCompareFuncRegular() == GL_LEQUAL;
	const bool isCameraInside = planLight.GetLight()->GetCameraInside();
	const decDMatrix &matrixLight = light.GetMatrix();
	const decDVector &lightPosition = matrixLight.GetPosition();
	const deoglShadowCaster &shadowCaster = *light.GetShadowCaster();
	
	float noiseScale;
	int target;
	
	// calculate matrices
	const decDMatrix &matrixCamera = plan.GetCameraMatrix();
	
	const decDMatrix matrixMV( matrixLight * matrixCamera );
	const decDMatrix matrixMVP( matrixMV * plan.GetProjectionMatrix() );
	
	const decDMatrix matrixCameraStereo( matrixCamera * plan.GetCameraStereoMatrix() );
	const decDMatrix &matrixProjectionStereo = plan.GetProjectionMatrixStereo();
	const decDMatrix matrixMVStereo( matrixLight * matrixCameraStereo );
	const decDMatrix matrixMVPStereo( matrixMVStereo * matrixProjectionStereo );
	
	decDMatrix matrixShadow( ( decDMatrix::CreateTranslation( lightPosition ) * matrixCamera ).QuickInvert() );
	matrixShadow.a21 = -matrixShadow.a21;
	matrixShadow.a22 = -matrixShadow.a22;
	matrixShadow.a23 = -matrixShadow.a23;
	matrixShadow.a24 = -matrixShadow.a24;
	
	decDMatrix matrixShadowStereo( ( decDMatrix::CreateTranslation( lightPosition ) * matrixCameraStereo ).QuickInvert() );
	matrixShadowStereo.a21 = -matrixShadowStereo.a21;
	matrixShadowStereo.a22 = -matrixShadowStereo.a22;
	matrixShadowStereo.a23 = -matrixShadowStereo.a23;
	matrixShadowStereo.a24 = -matrixShadowStereo.a24;
	
	// set values
	paramBlock.MapBuffer();
	try{
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutMatrixMVP );
		if( target != -1 ){
			paramBlock.SetParameterDataArrayMat4x4( target, 0, matrixMVP );
			paramBlock.SetParameterDataArrayMat4x4( target, 1, matrixMVPStereo );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutMatrixMV );
		if( target != -1 ){
			paramBlock.SetParameterDataArrayMat4x3( target, 0, matrixMV );
			paramBlock.SetParameterDataArrayMat4x3( target, 1, matrixMVStereo );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutLightPosition );
		if( target != -1 ){
			paramBlock.SetParameterDataArrayVec3( target, 0, matrixMV.GetPosition() );
			paramBlock.SetParameterDataArrayVec3( target, 1, matrixMVStereo.GetPosition() );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutDepthCompare );
		if( target != -1 ){
			paramBlock.SetParameterDataFloat( target, isCameraInside ? 0.0f : ( isDepthCompareLEqual ? 1.0f : -1.0f ) );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadowMatrix1 );
		if( target != -1 ){
			paramBlock.SetParameterDataArrayMat4x4( target, 0, matrixShadow );
			paramBlock.SetParameterDataArrayMat4x4( target, 1, matrixShadowStereo );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadowMatrix2 );
		if( target != -1 ){
			paramBlock.SetParameterDataArrayMat4x4( target, 0, matrixShadow );
			paramBlock.SetParameterDataArrayMat4x4( target, 1, matrixShadowStereo );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutLightImageOmniMatrix );
		if( target != -1 ){
			decVector rotate;
			if( light.GetUseSkinTexture() ){
				rotate = light.GetUseSkinTexture()->GetOmniDirRotate() * TWO_PI;
			}
			const decMatrix matrixRotate( decMatrix::CreateRotation( rotate ) );
			paramBlock.SetParameterDataArrayMat4x3( target, 0, matrixRotate.QuickMultiply( matrixMV ).QuickInvert() );
			paramBlock.SetParameterDataArrayMat4x3( target, 1, matrixRotate.QuickMultiply( matrixMVStereo ).QuickInvert() );
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
			const float scale = 1.0f / shadowCaster.GetStaticScale();
			
			paramBlock.SetParameterDataVec2( target, scale, scale * -shadowCaster.GetStaticOffset() );
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
}
