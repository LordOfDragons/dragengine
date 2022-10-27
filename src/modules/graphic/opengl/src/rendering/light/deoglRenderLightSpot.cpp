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
#include "deoglRenderLightSpot.h"
#include "deoglRenderGI.h"
#include "../deoglRenderGeometry.h"
#include "../deoglRenderOcclusion.h"
#include "../deoglRenderWorld.h"
#include "../defren/deoglDeferredRendering.h"
#include "../lod/deoglLODCalculator.h"
#include "../plan/deoglRenderPlan.h"
#include "../plan/deoglRenderPlanDebug.h"
#include "../plan/deoglRenderPlanLight.h"
#include "../task/deoglRenderTask.h"
#include "../task/deoglAddToRenderTask.h"
#include "../task/deoglRenderTaskShader.h"
#include "../task/shared/deoglRenderTaskSharedShader.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../canvas/render/deoglRCanvasView.h"
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
#include "../../texture/cubemap/deoglCubeMap.h"
#include "../../texture/cubemap/deoglRenderableDepthCubeMap.h"
#include "../../texture/deoglRImage.h"
#include "../../texture/deoglTextureStageManager.h"
#include "../../texture/texture2d/deoglRenderableColorTexture.h"
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

enum eSPLight{
	splMatrixMVP,
	splMatrixMV,
	splPosTransform,
	splLightPos,
	splLightView,
	splLightUp,
	splLightRight,
	splLightColor,
	splLightColorAmbient,
	splLightOptions,
	splLightAtten,
	splShadowMatrix,
	splShaMap1Params,
	splShaMap2Params,
	splShaTraParams
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




/*
== NOTES ==
===========

specular highlights should be hidden by shadows but otherwise they can be seen
outside the light cone as they are reflections of the shining surface which
with spot lights is usually a kind of flat circular surface. this can though
not be implemented using 2-dimensional shadow maps as they can not cover up
to 180 degree around the light source. this is usually not done since the
shadow casting is important. one solution would be the allow shadows up to
180 degree and cutting it off. this though neglects the shadow map and allows
specular highlights in hidden places.

currently the specular highlights are dimmed using the shadow map and cancelled
outside the light cone. fastest solution without too much inaccuracy.
*/



/*
TODO: transparent shadow casting is not working. it causes an exception in the
shadow mapper missing the depth transparent texture for some unknown reason

TODO: with the transparent pass there is a problem with the shadow map size.
static shadow maps have to use the same size as the for the solid pass but the
dynamic shadow map is reduced in size. this entire shadow casting situation for
transparent passes has to be reworked anyways maybe using static shadow maps
in full resolution as with the solid pass case but using only light dynamic
light probes for the dynamic shadow casting.
*/



// Structs
////////////

deoglRenderLightSpot::sShadowDepthMaps::sShadowDepthMaps() :
shadow1Solid( nullptr ),
shadow1Transp( nullptr ),
shadow2Solid( nullptr ),
shadow2Transp( nullptr ),
shadow1Ambient( nullptr ),
shadow2Ambient( nullptr ){
}

deoglRenderLightSpot::sShadowParams::sShadowParams() :
solid( false ),
transparentStaticShadow( false ),
transparentDynamicShadow( false ),
collideList1( nullptr ),
collideList2( nullptr ),
solidShadowMapSize( 0 ),
transpShadowMapSize( 0 ),
ambientMapSize( 0 ),
withTransparent( false ),
lodMaxPixelError( 1 ),
lodOffset( 0 ),
lodFovX( 0.0f ),
lodFovY( 0.0f ){
}



// #define DO_LOG_SIZE_CHANGE 1

#ifdef DO_LOG_SIZE_CHANGE
#define LOG_SIZE_CHANGE(s,o,n) GetRenderThread().GetLogger().LogInfoFormat(\
	"Spot.%s: Size changed from %d to %d (light=%p fbotarget=%p)", \
	s, o, n->GetWidth(), &light, plan.GetFBOTarget());
#else
#define LOG_SIZE_CHANGE(s,o,n)
#endif



// Class deoglRenderLightSpot
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderLightSpot::deoglRenderLightSpot( deoglRenderThread &renderThread,
deoglRTRenderers &renderers ) :
deoglRenderLightBase( renderThread )
{
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	const deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglShaderSources *sources;
	deoglShaderDefines defines;
	
	try{
		sources = shaderManager.GetSourcesNamed( "DefRen Shadow Copy Texture2D" );
		pShaderShadowCopy = shaderManager.GetProgramWith( sources, defines );
		
		
		
		sources = shaderManager.GetSourcesNamed( "DefRen Light BoxBoundary" );
		
		if( defren.GetUseInverseDepth() ){
			defines.SetDefine( "SHADOW_INVERSE_DEPTH", "1" );
		}
		defines.SetDefine( "DEPTH_INPUT", "1" );
		pShaderBoxBoundary1 = shaderManager.GetProgramWith( sources, defines );
		
		defines.SetDefine( "AMBIENT_MAP", "1" );
		pShaderBoxBoundary1Ambient = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		pShaderBoxBoundary2 = shaderManager.GetProgramWith( sources, defines );
		defines.RemoveAllDefines();
		
		
		
		sources = shaderManager.GetSourcesNamed( "DefRen Occlusion OccMap" );
		renderers.GetOcclusion().AddOccMapDefines( defines );
		pShaderOccMap = shaderManager.GetProgramWith( sources, defines );
		pShaderOccMap->EnsureRTSShader();
		pShaderOccMap->GetRTSShader()->SetSPBInstanceIndexBase( 0 );
		defines.RemoveAllDefines();
		
		
		
		// debug information
		const decColor colorText( 1.0f, 1.0f, 1.0f, 1.0f );
		const decColor colorBgUp( 0.05f, 0.05f, 0.05f, 0.75f );
		const decColor colorBg( 0.0f, 0.0f, 0.25f, 0.75f );
		const decColor colorBgSub( 0.05f, 0.05f, 0.05f, 0.75f );
		const decColor colorBgSub2( 0.1f, 0.1f, 0.1f, 0.75f );
		const decColor colorBgSub3( 0.15f, 0.15f, 0.15f, 0.75f );
		
		pDebugInfoSolid.TakeOver( new deoglDebugInformation( "Spot", colorText, colorBgUp ) );
		pDebugInfoTransparent.TakeOver( new deoglDebugInformation( "Spot", colorText, colorBgUp ) );
		
		
		
		pDebugInfoSolidDetail.TakeOver( new deoglDebugInformation( "Light Spot Solid", colorText, colorBg ) );
		
		pDebugInfoSolidShadow.TakeOver( new deoglDebugInformation( "Shadow", colorText, colorBgSub ) );
		pDebugInfoSolidDetail->GetChildren().Add( pDebugInfoSolidShadow );
		
		pDebugInfoSolidShadowClear.TakeOver( new deoglDebugInformation( "Clear", colorText, colorBgSub2 ) );
		pDebugInfoSolidShadow->GetChildren().Add( pDebugInfoSolidShadowClear );
		
		pDebugInfoSolidShadowClearTransp.TakeOver( new deoglDebugInformation( "Clear Transp", colorText, colorBgSub2 ) );
		pDebugInfoSolidShadow->GetChildren().Add( pDebugInfoSolidShadowClearTransp );
		
		pDebugInfoSolidShadowTask.TakeOver( new deoglDebugInformation( "Task", colorText, colorBgSub3 ) );
		pDebugInfoSolidShadow->GetChildren().Add( pDebugInfoSolidShadowTask );
		
		pDebugInfoSolidShadowRender.TakeOver( new deoglDebugInformation( "Render", colorText, colorBgSub3 ) );
		pDebugInfoSolidShadow->GetChildren().Add( pDebugInfoSolidShadowRender );
		
		pDebugInfoSolidShadowTranspTask.TakeOver( new deoglDebugInformation( "Transp Task", colorText, colorBgSub3 ) );
		pDebugInfoSolidShadow->GetChildren().Add( pDebugInfoSolidShadowTranspTask );
		
		pDebugInfoSolidShadowTranspRender.TakeOver( new deoglDebugInformation( "Transp Render", colorText, colorBgSub3 ) );
		pDebugInfoSolidShadow->GetChildren().Add( pDebugInfoSolidShadowTranspRender );
		
		pDebugInfoSolidLight.TakeOver( new deoglDebugInformation( "Light", colorText, colorBgSub ) );
		pDebugInfoSolidDetail->GetChildren().Add( pDebugInfoSolidLight );
		
		
		
		pDebugInfoTransparentDetail.TakeOver( new deoglDebugInformation( "Light Spot Transp", colorText, colorBg ) );
		
		pDebugInfoTransparentShadow.TakeOver( new deoglDebugInformation( "Shadow", colorText, colorBgSub ) );
		pDebugInfoTransparentDetail->GetChildren().Add( pDebugInfoTransparentShadow );
		
		pDebugInfoTransparentShadowClear.TakeOver( new deoglDebugInformation( "Clear", colorText, colorBgSub2 ) );
		pDebugInfoTransparentShadow->GetChildren().Add( pDebugInfoTransparentShadowClear );
		
		pDebugInfoTransparentShadowClearTransp.TakeOver( new deoglDebugInformation( "Clear Transp", colorText, colorBgSub2 ) );
		pDebugInfoTransparentShadow->GetChildren().Add( pDebugInfoTransparentShadowClearTransp );
		
		pDebugInfoTransparentShadowTask.TakeOver( new deoglDebugInformation( "Task", colorText, colorBgSub3 ) );
		pDebugInfoTransparentShadow->GetChildren().Add( pDebugInfoTransparentShadowTask );
		
		pDebugInfoTransparentShadowRender.TakeOver( new deoglDebugInformation( "Render", colorText, colorBgSub3 ) );
		pDebugInfoTransparentShadow->GetChildren().Add( pDebugInfoTransparentShadowRender );
		
		pDebugInfoTransparentShadowTranspTask.TakeOver( new deoglDebugInformation( "Transp Task", colorText, colorBgSub3 ) );
		pDebugInfoTransparentShadow->GetChildren().Add( pDebugInfoTransparentShadowTranspTask );
		
		pDebugInfoTransparentShadowTranspRender.TakeOver( new deoglDebugInformation( "Transp Render", colorText, colorBgSub3 ) );
		pDebugInfoTransparentShadow->GetChildren().Add( pDebugInfoTransparentShadowTranspRender );
		
		pDebugInfoTransparentLight.TakeOver( new deoglDebugInformation( "Light", colorText, colorBgSub ) );
		pDebugInfoTransparentDetail->GetChildren().Add( pDebugInfoTransparentLight );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRenderLightSpot::~deoglRenderLightSpot(){
	pCleanUp();
}



// Rendering
//////////////

void deoglRenderLightSpot::CalculateBoxBoundary( deoglRenderPlanLight &planLight ) {
	deoglRLight &light = *planLight.GetLight()->GetLight();
	deoglShadowCaster &shadowCaster = *light.GetShadowCaster();
	deoglSCSolid &scsolid = shadowCaster.GetSolid();
	deoglSCAmbient &scambient = shadowCaster.GetAmbient();
	const bool useAmbient = planLight.GetUseAmbient();
	
	if( ! scsolid.GetStaticMap() ){
		DETHROW( deeInvalidParam );
		// or simply return the full box boundary instead?
	}
	
	deoglTexture &shadowmap = *scsolid.GetStaticMap();
	int size = shadowmap.GetWidth() >> 1;
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglLightBoundaryMap &boundaryMap = renderThread.GetLightBoundaryMap( size );
	//deoglShadowMapper &shadowMapper = *renderThread.GetShadowMapper();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	int mipMapLevel = boundaryMap.GetBaseLevel( size );
	
	// calculate light matrix
	const float znear = OGL_REN_LIG_ZNEAR;
	//const float zfar = light.GetRange();
	float fov;
	
	// this is not correct but at least not wrong either. correctly the y/x fov ratio
	// has to be used too to create the correct frustum. this solution here is sub
	// optimal but at least not wrong
	const float fovX = decMath::min( light.GetSpotAngle(), DEG2RAD * 179.0f );
	const float fovY = decMath::min( light.GetSpotAngle() * light.GetSpotRatio(), DEG2RAD * 179.0f );
	fov = decMath::max( fovX, fovY );
	
	decVector4 depth2Pos;
	
	if( defren.GetUseInverseDepth() ){
		depth2Pos.x = -znear;
		depth2Pos.y = 0.0f;
		
	}else{
		depth2Pos.x = znear;
		depth2Pos.y = 1.0f;
	}
	
	depth2Pos.w = depth2Pos.z = tanf( fov * 0.5f );
	
	const decDMatrix matrixLRot( light.GetMatrix().GetRotationMatrix() );
	
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
	
	if( useAmbient && scambient.GetStaticMap() ){
		renderThread.GetShader().ActivateShader( pShaderBoxBoundary1Ambient );
		shader = pShaderBoxBoundary1Ambient->GetCompiled();
		
	}else{
		renderThread.GetShader().ActivateShader( pShaderBoxBoundary1 );
		shader = pShaderBoxBoundary1->GetCompiled();
	}
	
	shader->SetParameterFloat( spbbQuadParams, 1.0f, 1.0f, 0.0f, 0.0f );
	shader->SetParameterVector4( spbbPosTransform, depth2Pos );
	shader->SetParameterDMatrix3x3( spbbMatrixRotation, matrixLRot );
	shader->SetParameterFloat( spbbInitialMinValue, 0.0f, 0.0f, 0.0f );
	shader->SetParameterFloat( spbbInitialMaxValue, 0.0f, 0.0f, 0.0f );
	
	const decDVector lightPosition( light.GetMatrix().GetPosition() );
	shader->SetParameterDVector3( spbbClearMinValue, light.GetFullMinExtend() - lightPosition );
	shader->SetParameterDVector3( spbbClearMaxValue, light.GetFullMaxExtend() - lightPosition );
	
	tsmgr.DisableAllStages();
	tsmgr.EnableTexture( 2, shadowmap, GetSamplerClampNearest() );
	if( useAmbient && scambient.GetStaticMap() ){
		tsmgr.EnableTexture( 3, *scambient.GetStaticMap(), GetSamplerClampNearest() );
	}
	
	OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
	//ogl.LogInfoFormat( "BoxBoundary %p Step1 size(%i) mipMaplevel(%i)", &light, size, mipMapLevel );
	
	// down sampling to 1x1 using mip map levels
	renderThread.GetShader().ActivateShader( pShaderBoxBoundary2 );
	
	pShaderBoxBoundary2->GetCompiled()->SetParameterFloat( spbbQuadParams, 1.0f, 1.0f, 0.0f, 0.0f );
	
	tsmgr.DisableStage( 2 );
	tsmgr.EnableTexture( 0, *boundaryMap.GetTextureMin(), GetSamplerClampNearest() );
	tsmgr.EnableTexture( 1, *boundaryMap.GetTextureMax(), GetSamplerClampNearest() );
	
	while( size > 1 ){
		size >>= 1;
		mipMapLevel++;
		
		OGL_CHECK( renderThread, glViewport( 0, 0, size, size ) );
		OGL_CHECK( renderThread, glScissor( 0, 0, size, size ) );
		
		renderThread.GetFramebuffer().Activate( boundaryMap.GetFBOAt( mipMapLevel ) );
		
		pShaderBoxBoundary2->GetCompiled()->SetParameterInt( spbbMipMapLevel, mipMapLevel - 1 );
		
		OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
		//ogl.LogInfoFormat( "BoxBoundary %p Step2 mipMapSize(%i) mipMaplevel(%i)", &light, mipMapSize, mipMapLevel );
	}
	
	pglBindVertexArray( 0 );
	
	// fetch the result of the 1x1 lod level
	decVector boundaryMin, boundaryMax;
	
	boundaryMap.GetResult( boundaryMin, boundaryMax );
	
	boundaryMin -= decVector( 0.01f, 0.01f, 0.01f ); // just to make sure the box is not too small
	boundaryMax += decVector( 0.01f, 0.01f, 0.01f ); // just to make sure the box is not too small
	
	const decVector cbExtends( ( boundaryMax - boundaryMin ) * 0.5f );
	const decVector cbPosition( ( boundaryMin + boundaryMax ) * 0.5f );
	
	light.SetLightVolumeCropBox( new decShapeBox( cbExtends, cbPosition ) );
	
	// DEBUG
	/*
	size = shadowmap.GetWidth() >> 1;
	mipMapLevel = boundaryMap.GetBaseLevel( size );
	renderThread.GetLogger().LogInfoFormat( "BoxBoundary %p min(%g,%g,%g) max(%g,%g,%g) level=%d size=%d",
		&light, boundaryMin.x, boundaryMin.y, boundaryMin.z,
		boundaryMax.x, boundaryMax.y, boundaryMax.z, mipMapLevel, size );
	*/
}



void deoglRenderLightSpot::RenderLights( deoglRenderPlan &plan, bool solid, const deoglRenderPlanMasked *mask ){
	const int count = plan.GetLightCount();
	int i;
	
	DebugTimersReset( plan, false );
	
	for( i=0; i<count; i++ ){
		deoglRenderPlanLight &light = *plan.GetLightAt( i );
		
		switch( light.GetLight()->GetLight()->GetLightType() ){
		case deLight::eltSpot:
		case deLight::eltProjector:
			RenderLight( light, solid, mask );
			break;
			
		default:
			break;
		}
	}
	
	if( solid ){
		DebugTimer1Sample( plan, *pDebugInfoSolid, true );
		
	}else{
		DebugTimer1Sample( plan, *pDebugInfoTransparent, true );
	}
}


#include "../../debug/deoglDebugStateSnapshot.h"
void deoglRenderLightSpot::RenderLight( deoglRenderPlanLight &planLight, bool solid,
const deoglRenderPlanMasked *mask ){
	// determine what needs to be rendered
	deoglCollideListLight &cllight = *planLight.GetLight();
	if( cllight.IsHiddenByOccQuery() ){
		cllight.SetCulled( true );
	}
	
	const bool lightGeometry = ! cllight.GetCulled();
	deoglRenderPlan &plan = planLight.GetPlan();
	deoglGIState * const giState = ! mask && solid ? plan.GetUpdateGIState() : NULL;
	
	if( ! lightGeometry && ! giState ){
		return;
	}
	
	// prepare for rendering
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "LightSpot.RenderLight" );
	deoglRLight &light = *cllight.GetLight();
	deoglShadowCaster &shadowCaster = *light.GetShadowCaster();
	deoglSCTransparent &sctransp = shadowCaster.GetTransparent();
	deoglSCSolid &scsolid = shadowCaster.GetSolid();
	deoglSCAmbient &scambient = shadowCaster.GetAmbient();
	deoglShadowMapper &shadowMapper = renderThread.GetShadowMapper();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglRenderPlanDebug * const planDebug = plan.GetDebug();
	deoglDCollisionBox colbox;
	int shadowType = shadowCaster.GetShadowType();
	deoglTexture *texSolidDepth1 = NULL;
	deoglTexture *texSolidDepth2 = NULL;
	deoglTexture *texTranspDepth1 = NULL;
	deoglTexture *texTranspColor1 = NULL;
	deoglTexture *texTranspDepth2 = NULL;
	deoglTexture *texTranspColor2 = NULL;
	deoglTexture *texAmbient1 = NULL;
	deoglTexture *texAmbient2 = NULL;
	
	const bool useTemporary = planLight.GetUseShadowTemporary();
	const bool useAmbient = planLight.GetUseAmbient();
	const bool useShadow = planLight.GetUseShadow();
	
	bool copyShadowMaps = false;
	
	if( useShadow && useTemporary ){
		shadowType = deoglShadowCaster::estDynamicOnly;
	}
	
	// determine if we need transparent shadow casting
	sShadowParams shadowParams;
	
	if( useShadow ){
		//if( ! plan.GetTransparentShadow() ){
			const deoglCollideList &clistStatic = *light.GetStaticCollideList();
			const deoglCollideList &clistDynamic = *light.GetDynamicCollideList();
			int i, count;
			
			count = clistStatic.GetComponentCount();
			for( i=0; i<count; i++ ){
				const deoglRComponent &component = *clistStatic.GetComponentAt( i )->GetComponent();
				
				if( ! component.GetSolid() && component.GetSkin() && component.GetSkin()->GetCastTransparentShadow() ){
					shadowParams.transparentStaticShadow = true;
					break;
				}
			}
			
			count = clistDynamic.GetComponentCount();
			for( i=0; i<count; i++ ){
				const deoglRComponent &component = *clistDynamic.GetComponentAt( i )->GetComponent();
				
				if( ! component.GetSolid() && component.GetSkin() && component.GetSkin()->GetCastTransparentShadow() ){
					shadowParams.transparentDynamicShadow = true;
					break;
				}
			}
		//}
	}
	
	// debug
	if( solid && plan.GetRenderPassNumber() == 1 && planDebug ){
		planDebug->IncrementRenderedLights();
	}
	
	// calculate light volume matrices
	shadowParams.matrixProjection = defren.CreateProjectionDMatrix( 1, 1,
		light.GetSpotAngle(), light.GetSpotRatio(), OGL_REN_LIG_ZNEAR, light.GetRange() );
	
	shadowParams.lodFovX = light.GetSpotAngle();
	shadowParams.lodFovY = light.GetSpotRatio();
	
	// render shadow map if required
	if( useShadow ){
		DebugTimer2Reset( plan, true );
		RenderShadows( planLight, shadowParams );
		
		OGL_CHECK( renderThread, glViewport( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
		OGL_CHECK( renderThread, glScissor( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
		RestoreFBO( plan );
		
		// determine shadow maps to use
		switch( shadowType ){
		case deoglShadowCaster::estStaticOnly:
			texSolidDepth1 = scsolid.GetStaticMap();
			if( shadowParams.transparentStaticShadow ){
				texTranspDepth1 = sctransp.GetStaticShadowMap();
				texTranspColor1 = sctransp.GetStaticColorMap();
			}
			if( useAmbient ){
				texAmbient1 = scambient.GetStaticMap();
			}
			break;
			
		case deoglShadowCaster::estDynamicOnly:
			if( useTemporary ){
				texSolidDepth1 = scsolid.GetTemporaryMap()->GetTexture();
				if( shadowParams.transparentDynamicShadow ){
					texTranspDepth1 = sctransp.GetTemporaryShadowMap()->GetTexture();
					texTranspColor1 = sctransp.GetTemporaryColorMap()->GetTexture();
				}
				if( useAmbient ){
					texAmbient1 = scambient.GetTemporaryMap()->GetTexture();
				}
				
			}else{
				texSolidDepth1 = scsolid.GetDynamicMap();
				if( shadowParams.transparentDynamicShadow ){
					texTranspDepth1 = sctransp.GetDynamicShadowMap();
					texTranspColor1 = sctransp.GetDynamicColorMap();
				}
				if( useAmbient ){
					texAmbient1 = scambient.GetDynamicMap();
				}
			}
			break;
			
		case deoglShadowCaster::estStaticAndDynamic:
			if( copyShadowMaps ){
				// WARNING! THIS CODE IS NOT WORKING! DISABLED!
				texSolidDepth1 = shadowMapper.GetSolidDepthTexture();
				if( shadowParams.transparentStaticShadow ){
					texTranspDepth1 = sctransp.GetStaticShadowMap();
					texTranspColor1 = sctransp.GetStaticColorMap();
				}
				break;
			}
			
			texSolidDepth1 = scsolid.GetStaticMap();
			if( shadowParams.transparentStaticShadow ){
				texTranspDepth1 = sctransp.GetStaticShadowMap();
				texTranspColor1 = sctransp.GetStaticColorMap();
			}
			if( useAmbient ){
				texAmbient1 = scambient.GetStaticMap();
			}
			
			texSolidDepth2 = scsolid.GetDynamicMap();
			if( shadowParams.transparentDynamicShadow ){
				if( texTranspDepth1 ){
					texTranspDepth2 = sctransp.GetDynamicShadowMap();
					
				}else{
					texTranspDepth1 = sctransp.GetDynamicShadowMap();
				}
				
				if( texTranspColor1 ){
					texTranspColor2 = sctransp.GetDynamicColorMap();
					
				}else{
					texTranspColor1 = sctransp.GetDynamicColorMap();
				}
			}
			if( useAmbient ){
				texAmbient2 = scambient.GetDynamicMap();
			}
			break;
		}
		
		/*
		if( renderThread.GetConfiguration().GetDebugSnapshot() == 91 ){
			if( texSolidDepth1 ){
				renderThread.GetDebug().GetDebugSaveTexture().SaveDepthTexture(
					*texSolidDepth1, "spot_texSolidDepth1", deoglDebugSaveTexture::edtDepthInverse );
			}
			if( texSolidDepth2 ){
				renderThread.GetDebug().GetDebugSaveTexture().SaveDepthTexture(
					*texSolidDepth2, "spot_texSolidDepth2", deoglDebugSaveTexture::edtDepthInverse );
			}
			if( texTranspDepth1 ){
				renderThread.GetDebug().GetDebugSaveTexture().SaveDepthTexture(
					*texTranspDepth1, "spot_texTranspDepth1", deoglDebugSaveTexture::edtDepthInverse );
			}
			if( texTranspColor1 ){
				renderThread.GetDebug().GetDebugSaveTexture().SaveTexture(
					*texTranspColor1, "spot_texTranspColor1" );
			}
			if( texTranspDepth2 ){
				renderThread.GetDebug().GetDebugSaveTexture().SaveDepthTexture(
					*texTranspDepth2, "spot_texTranspDepth2", deoglDebugSaveTexture::edtDepthInverse );
			}
			if( texTranspColor2 ){
				renderThread.GetDebug().GetDebugSaveTexture().SaveTexture(
					*texTranspColor2, "spot_texTranspColor2" );
			}
			if( texAmbient ){
				renderThread.GetDebug().GetDebugSaveTexture().SaveDepthTexture(
					*texAmbient, "spot_texAmbient", deoglDebugSaveTexture::edtDepthInverse );
			}
			renderThread.GetConfiguration().SetDebugSnapshot( 0 );
		}
		*/
		
		if( solid ){
			DebugTimer2SampleCount( plan, *pDebugInfoSolidShadow, 1, true );
			
		}else{
			DebugTimer2SampleCount( plan, *pDebugInfoTransparentShadow, 1, true );
		}
		
	}else{
		OGL_CHECK( renderThread, glDepthFunc( defren.GetDepthCompareFuncRegular() ) ); // spot light is using similar
		if( pglClipControl && defren.GetUseInverseDepth() ){
			pglClipControl( GL_LOWER_LEFT, GL_ZERO_TO_ONE );
		}
	}
	
	// set up the opengl states we need for all this
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
				if( texAmbient1 ){
					lightShader = light.GetShaderFor( plan.GetRenderStereo()
						? deoglRLight::estStereoSolid2Transp2
						: deoglRLight::estSolid2Transp2 );
					
				}else{
					lightShader = light.GetShaderFor( plan.GetRenderStereo()
						? deoglRLight::estStereoSolid2Transp2NoAmbient
						: deoglRLight::estSolid2Transp2NoAmbient );
				}
				
			}else if( texTranspDepth1 ){
				if( texAmbient1 ){
					lightShader = light.GetShaderFor( plan.GetRenderStereo()
						? deoglRLight::estStereoSolid2Transp1
						: deoglRLight::estSolid2Transp1 );
					
				}else{
					lightShader = light.GetShaderFor( plan.GetRenderStereo()
						? deoglRLight::estStereoSolid2Transp1NoAmbient
						: deoglRLight::estSolid2Transp1NoAmbient );
				}
				
			}else{
				if( texAmbient1 ){
					lightShader = light.GetShaderFor( plan.GetRenderStereo()
						? deoglRLight::estStereoSolid2
						: deoglRLight::estSolid2 );
					
				}else{
					lightShader = light.GetShaderFor( plan.GetRenderStereo()
						? deoglRLight::estStereoSolid2NoAmbient
						: deoglRLight::estSolid2NoAmbient );
				}
			}
			
		}else{
			if( texTranspDepth1 ){
				if( texAmbient1 ){
					lightShader = light.GetShaderFor( plan.GetRenderStereo()
						? deoglRLight::estStereoSolid1Transp1
						: deoglRLight::estSolid1Transp1 );
					
				}else{
					lightShader = light.GetShaderFor( plan.GetRenderStereo()
						? deoglRLight::estStereoSolid1Transp1NoAmbient
						: deoglRLight::estSolid1Transp1NoAmbient );
				}
				
			}else{
				if( texAmbient1 ){
					lightShader = light.GetShaderFor( deoglRLight::estSolid1 );
					
				}else{
					lightShader = light.GetShaderFor( plan.GetRenderStereo()
						? deoglRLight::estStereoSolid1NoAmbient
						: deoglRLight::estSolid1NoAmbient );
				}
			}
		}
		
	}else{
		lightShader = light.GetShaderFor( plan.GetRenderStereo()
			? deoglRLight::estStereoNoShadow
			: deoglRLight::estNoShadow );
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
	UpdateInstanceParamBlock( *lightShader, *spbInstance, plan, cllight, shadowDepthMaps, shadowParams.matrixProjection );
	
	// light geometry
	if( lightGeometry ){
		renderThread.GetShader().ActivateShader( lightShader->GetShader() );
		
		renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
		spbLight->Activate();
		spbInstance->Activate();
		
		ActivateTextures( planLight, *lightShader, shadowDepthMaps );
		
		// render the light
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
			
			// WARNING always non-stereo!
			renderThread.GetRenderers().GetWorld().GetRenderPB()->Activate();
			
			spbLight->Activate();
			spbInstance->Activate();
			
			ActivateTextures( planLight, *lightShader, shadowDepthMaps );
			
			defren.RenderFSQuadVAO();
		}
	}
}

void deoglRenderLightSpot::ActivateTextures( deoglRenderPlanLight &planLight,
deoglLightShader &shader, const sShadowDepthMaps &shadowDepthMaps ){
	deoglRLight &light = *planLight.GetLight()->GetLight();
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglRTDefaultTextures &dt = renderThread.GetDefaultTextures();
	deoglTexture &dtshadow = renderThread.GetDeferredRendering().GetUseInverseDepth()
		? *dt.GetShadowMapInverseDepth() : *dt.GetShadowMap();
	int target;
	
	target = shader.GetTextureTarget( deoglLightShader::ettColor );
	if( target != -1 ){
		deoglTexUnitConfig tuc;
		
		if( light.GetLightCanvas() ){
			deoglRenderTarget * const target2 = light.GetLightCanvas()->GetRenderTarget();
			if( target2 ){
				tuc.EnableTexture( target2->GetTexture(), &GetSamplerClampLinear() );
			}
			
		}else if( light.GetUseSkinTexture() ){
			tuc.EnableTextureFromChannel( renderThread, *light.GetUseSkinTexture(),
				deoglSkinChannel::ectColor, light.GetSkinState(), light.GetDynamicSkin(),
				dt.GetColor() );
		}
		
		if( tuc.IsEnabled() ){
			tuc.Apply( renderThread, target );
			
		}else{
			tsmgr.EnableTexture( target, *dt.GetColor(), GetSamplerClampLinear() );
		}
	}
	
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
		tsmgr.EnableTexture( target, shadowDepthMaps.shadow1Solid
			? *shadowDepthMaps.shadow1Solid : dtshadow, GetSamplerShadowClampLinearInverse() );
	}
	target = shader.GetTextureTarget( deoglLightShader::ettShadow1TransparentDepth );
	if( target != -1 ){
		tsmgr.EnableTexture( target, shadowDepthMaps.shadow1Transp
			? *shadowDepthMaps.shadow1Transp : dtshadow, GetSamplerShadowClampLinearInverse() );
	}
	target = shader.GetTextureTarget( deoglLightShader::ettShadow1TransparentColor );
	if( target != -1 ){
		tsmgr.EnableTexture( target, shadowDepthMaps.shadow1TranspColor
			? *shadowDepthMaps.shadow1TranspColor : *dt.GetShadowMapColor(), GetSamplerClampLinear() );
	}
	
	target = shader.GetTextureTarget( deoglLightShader::ettShadow2SolidDepth );
	if( target != -1 ){
		tsmgr.EnableTexture( target, shadowDepthMaps.shadow2Solid
			? *shadowDepthMaps.shadow2Solid : dtshadow, GetSamplerShadowClampLinearInverse() );
	}
	target = shader.GetTextureTarget( deoglLightShader::ettShadow2TransparentDepth );
	if( target != -1 ){
		tsmgr.EnableTexture( target, shadowDepthMaps.shadow2Transp
			? *shadowDepthMaps.shadow2Transp : dtshadow, GetSamplerShadowClampLinearInverse() );
	}
	target = shader.GetTextureTarget( deoglLightShader::ettShadow2TransparentColor );
	if( target != -1 ){
		tsmgr.EnableTexture( target, shadowDepthMaps.shadow2TranspColor
			? *shadowDepthMaps.shadow2TranspColor : *dt.GetShadowMapColor(), GetSamplerClampLinear() );
	}
	
	target = shader.GetTextureTarget( deoglLightShader::ettShadow1Ambient );
	if( target != -1 ){
		tsmgr.EnableTexture( target, shadowDepthMaps.shadow1Ambient
			? *shadowDepthMaps.shadow1Ambient : dtshadow, GetSamplerShadowClampLinearInverse() );
	}
	
	target = shader.GetTextureTarget( deoglLightShader::ettShadow2Ambient );
	if( target != -1 ){
		tsmgr.EnableTexture( target, shadowDepthMaps.shadow2Ambient
			? *shadowDepthMaps.shadow2Ambient : dtshadow, GetSamplerShadowClampLinearInverse() );
	}
	
	target = shader.GetTextureTarget( deoglLightShader::ettLightDepth1 );
	if( target != -1 ){
		tsmgr.EnableTexture( target, shadowDepthMaps.shadow1Solid
			? *shadowDepthMaps.shadow1Solid : *dt.GetShadowMap(), GetSamplerClampLinear() );
	}
	target = shader.GetTextureTarget( deoglLightShader::ettLightDepth2 );
	if( target != -1 ){
		tsmgr.EnableTexture( target, shadowDepthMaps.shadow2Solid
			? *shadowDepthMaps.shadow2Solid : *dt.GetShadowMap(), GetSamplerClampLinear() );
	}
}



void deoglRenderLightSpot::RenderShadows( deoglRenderPlanLight &planLight, sShadowParams &shadowParams ){
	deoglRLight &light = *planLight.GetLight()->GetLight();
	deoglRenderPlan &plan = planLight.GetPlan();
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglShadowMapper &shadowMapper = renderThread.GetShadowMapper();
	deoglShadowCaster &shadowCaster = *light.GetShadowCaster();
	deoglSCTransparent &sctransp = shadowCaster.GetTransparent();
	deoglSCSolid &scsolid = shadowCaster.GetSolid();
	deoglSCAmbient &scambient = shadowCaster.GetAmbient();
	deoglShadowCaster::eShadowTypes shadowType = shadowCaster.GetShadowType();
	const bool refilterShadow = planLight.GetRefilterShadows();
	const bool useTemporary = planLight.GetUseShadowTemporary();
	const bool useAmbient = planLight.GetUseAmbient();
	bool updateBoxBoundary = false;
	
	// calculate camera matrix
	shadowParams.lightPosition = light.GetMatrix().GetPosition() - plan.GetWorld()->GetReferencePosition();
	shadowParams.matrixCamera = light.GetMatrix().GetRotationMatrix();
	shadowParams.lightView = shadowParams.matrixCamera.TransformView();
	shadowParams.matrixCamera.SetCamera( shadowParams.lightPosition,
		shadowParams.lightView, shadowParams.matrixCamera.TransformUp() );
	
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
		
		if( scsolid.GetStaticMap() ){
			if( scsolid.GetStaticMap()->GetWidth() != staticShadowMapSize ){
				LOG_SIZE_CHANGE("scsolid.static", staticShadowMapSize, scsolid.GetStaticMap())
				scsolid.DropStatic();
				requiresUpdate = true;
			}
			
		}else{
			requiresUpdate = true;
		}
		
		if( useAmbient ){
			if( scambient.GetStaticMap() ){
				if( scambient.GetStaticMap()->GetWidth() != staticAmbientMapSize ){
					LOG_SIZE_CHANGE("scambient.static", staticAmbientMapSize, scambient.GetStaticMap())
					scambient.DropStatic();
					requiresUpdate = true;
				}
				
			}else{
				requiresUpdate = true;
			}
		}
		
		if( shadowParams.transparentStaticShadow ){
			if( sctransp.GetStaticShadowMap() ){
				if( sctransp.GetStaticShadowMap()->GetWidth() != staticTranspShadowMapSize ){
					LOG_SIZE_CHANGE("sctransp.static", staticTranspShadowMapSize, sctransp.GetStaticShadowMap())
					sctransp.DropStatic();
					requiresUpdate = true;
				}
				
			}else{
				requiresUpdate = true;
			}
			
			if( sctransp.GetStaticColorMap() ){
				if( sctransp.GetStaticColorMap()->GetWidth() < staticTranspShadowMapSize ){
					LOG_SIZE_CHANGE("sctransp.staticColor", staticTranspShadowMapSize, sctransp.GetStaticColorMap())
					sctransp.DropStatic();
					requiresUpdate = true;
				}
				
			}else{
				requiresUpdate = true;
			}
		}
		
		if( requiresUpdate ){
			/*
			renderThread.GetLogger().LogInfoFormat( "RenderLightSpot Static %p: size=%d cc=%d",
				&light, staticShadowMapSize, light.GetStaticCollideList()->GetComponentCount() );
			*/
			
			// TODO for static shadow maps using always lod level 0 is enough. lod levels are supposed to keep the
			// shape of the object as well as possible thus the shadow error is small. on a distance where the
			// rendered lod level can differ from the static shadow casting lod level this is hard to spot anyways
			shadowMapper.SetForeignSolidDepthTexture( scsolid.ObtainStaticMapWithSize(
				staticShadowMapSize, false, defren.GetUseInverseDepth() ) );
			
			if( ! shadowParams.solid && shadowParams.transparentStaticShadow ){
				shadowMapper.SetForeignTransparentDepthTexture(
					sctransp.ObtainStaticShadowMapWithSize( staticTranspShadowMapSize,
						defren.GetUseInverseDepth() ) );
				shadowMapper.SetForeignTransparentColorTexture(
					sctransp.ObtainStaticColorMapWithSize( staticTranspShadowMapSize ) );
			}
			
			shadowParams.collideList1 = light.GetStaticCollideList();
			shadowParams.collideList2 = nullptr;
			shadowParams.solidShadowMapSize = staticShadowMapSize;
			shadowParams.transpShadowMapSize = staticTranspShadowMapSize;
			shadowParams.ambientMapSize = staticAmbientMapSize;
			shadowParams.withTransparent = shadowParams.transparentStaticShadow;
			
			shadowParams.lodMaxPixelError = 2;
			shadowParams.lodOffset = 0;
			
			RenderShadowMap( planLight, shadowMapper, shadowParams );
			
			shadowMapper.DropForeignTextures();
			
			// ambient map
			if( useAmbient ){
				shadowMapper.SetForeignAmbientTexture( scambient.ObtainStaticMapWithSize(
					staticAmbientMapSize, defren.GetUseInverseDepth() ) );
				RenderAmbientMap( planLight, shadowMapper, shadowParams );
				shadowMapper.DropForeignAmbientTextures();
			}
			
			if( ! light.GetLightVolumeCropBox() ){
				updateBoxBoundary = true;
			}
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
	
	// dynamic shadow map with transparency if required
	if( shadowType == deoglShadowCaster::estDynamicOnly
	|| shadowType == deoglShadowCaster::estStaticAndDynamic ){
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
			if( scsolid.GetDynamicMap() && scsolid.GetDynamicMap()->GetWidth() != dynamicShadowMapSize ){
				LOG_SIZE_CHANGE("scsolid.dynamic", dynamicShadowMapSize, scsolid.GetDynamicMap())
				scsolid.DropDynamic();
			}
			if( useAmbient && scambient.GetDynamicMap() && scambient.GetDynamicMap()->GetWidth() != dynamicAmbientMapSize ){
				LOG_SIZE_CHANGE("scambient.dynamic", dynamicAmbientMapSize, scambient.GetDynamicMap())
				scambient.DropDynamic();
			}
			if( shadowParams.transparentDynamicShadow && sctransp.GetDynamicShadowMap()
			&& sctransp.GetDynamicShadowMap()->GetWidth() != dynamicTranspShadowMapSize ){
				LOG_SIZE_CHANGE("sctransp.dynamic", dynamicTranspShadowMapSize, sctransp.GetDynamicShadowMap())
				sctransp.DropDynamic();
			}
		}
		
		// render shadow map if dirty. the dirty flag is reset each frame update. this ensures
		// dynamic shadow maps are rendered once per frame update
		bool requiresUpdate = false;
		
		if( useTemporary ){
			requiresUpdate = ! scsolid.GetTemporaryMap();
			if( useAmbient ){
				requiresUpdate |= ! scambient.GetTemporaryMap();
			}
			if( shadowParams.transparentDynamicShadow ){
				requiresUpdate |= ! sctransp.GetTemporaryShadowMap() || ! sctransp.GetTemporaryColorMap();
			}
			
		}else{
			requiresUpdate = ! scsolid.GetDynamicMap() || scsolid.GetDirtyDynamic();
			if( useAmbient ){
				requiresUpdate |= ! scambient.GetDynamicMap() || scambient.GetDirtyDynamic();
			}
			if( shadowParams.transparentDynamicShadow ){
				requiresUpdate |= ! sctransp.GetDynamicShadowMap()
					|| ! sctransp.GetDynamicColorMap() || sctransp.GetDirtyDynamic();
			}
		}
		
		if( requiresUpdate ){
			if( useTemporary ){
				shadowMapper.SetForeignSolidDepthTexture( scsolid.ObtainTemporaryMapWithSize(
					dynamicShadowMapSize, false, defren.GetUseInverseDepth() )->GetTexture() );
				
				if( shadowParams.transparentDynamicShadow ){
					shadowMapper.SetForeignTransparentDepthTexture( sctransp.ObtainTemporaryShadowMapWithSize(
						dynamicTranspShadowMapSize, defren.GetUseInverseDepth() )->GetTexture() );
					shadowMapper.SetForeignTransparentColorTexture(
						sctransp.ObtainTemporaryColorMapWithSize( dynamicTranspShadowMapSize )->GetTexture() );
				}
				
			}else{
				shadowMapper.SetForeignSolidDepthTexture( scsolid.ObtainDynamicMapWithSize(
					dynamicShadowMapSize, false, defren.GetUseInverseDepth() ) );
				
				if( shadowParams.transparentDynamicShadow ){
					shadowMapper.SetForeignTransparentDepthTexture( sctransp.ObtainDynamicShadowMapWithSize(
						dynamicTranspShadowMapSize, defren.GetUseInverseDepth() ) );
					shadowMapper.SetForeignTransparentColorTexture(
						sctransp.ObtainDynamicColorMapWithSize( dynamicTranspShadowMapSize ) );
				}
			}
			
			shadowParams.collideList1 = clist1;
			shadowParams.collideList2 = clist2;
			shadowParams.solidShadowMapSize = dynamicShadowMapSize;
			shadowParams.transpShadowMapSize = dynamicTranspShadowMapSize;
			shadowParams.ambientMapSize = dynamicAmbientMapSize;
			shadowParams.withTransparent = shadowParams.transparentDynamicShadow;
			
			shadowParams.lodMaxPixelError = 2;
			shadowParams.lodOffset = 0;
			
			RenderShadowMap( planLight, shadowMapper, shadowParams );
			
			shadowMapper.DropForeignTextures();
			
			if( ! useTemporary ){
				scsolid.SetDirtyDynamic( false );
				if( shadowParams.transparentDynamicShadow ){
					sctransp.SetDirtyDynamic( false );
				}
			}
			
			// ambient map
			if( useAmbient ){
				if( useTemporary ){
					shadowMapper.SetForeignAmbientTexture( scambient.ObtainTemporaryMapWithSize(
						dynamicAmbientMapSize, defren.GetUseInverseDepth() )->GetTexture() );
					
				}else{
					shadowMapper.SetForeignAmbientTexture( scambient.ObtainDynamicMapWithSize(
						dynamicAmbientMapSize, defren.GetUseInverseDepth() ) );
				}
				
				RenderAmbientMap( planLight, shadowMapper, shadowParams );
				
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
		CalculateBoxBoundary( planLight );
	}
	
	// activate stencil as we had to disable it for rendering the shadow maps
	OGL_CHECK( renderThread, glEnable( GL_STENCIL_TEST ) );
}



void deoglRenderLightSpot::RenderShadowMap( deoglRenderPlanLight &planLight,
deoglShadowMapper &shadowMapper, const sShadowParams &shadowParams ){
	deoglRenderPlan &plan = planLight.GetPlan();
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "LightSpot.RenderShadowMap" );
	deoglSPBlockUBO * const renderParamBlock = renderThread.GetRenderers().GetLight().GetShadowPB();
	deoglAddToRenderTask &addToRenderTask = renderThread.GetRenderers().GetLight().GetAddToRenderTask();
	deoglRenderTask &renderTask = renderThread.GetRenderers().GetLight().GetRenderTask();
	deoglRenderGeometry &rengeom = renderThread.GetRenderers().GetGeometry();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	const deoglConfiguration &config = renderThread.GetConfiguration();
	const float smOffsetScale = config.GetShadowMapOffsetScale();
	const float smOffsetBias = config.GetShadowMapOffsetBias();
	
	DebugTimer3Reset( plan, false );
	
	// prevents opengl verifiers from complaining about an error which is none. this problems
	// is seen on nVidia drivers if the debug mode is enabled. the driver keeps on complaining
	// about a shader using a shadow-sampler on a non-depth texture. researching the problem
	// shows this line below is the problem if there is no point light. in this case the sky
	// light rendering happens right before the spot light shadow map rendering. the sky light
	// shader leaves behind an activated shader which points to the dynamic shadow map textures
	// used for various shadow map calculation. these textures become invalid because they will
	// be rendered here. glClear does not care about the active shader nor what textures are
	// assigned. Nevertheless the nVidia driver keeps on complaining which by itself would be
	// no problem but it spams the logs having negative impact on performance while debugging
	renderThread.GetShader().ActivateShader( NULL );
	
	// set states
	OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	
	// activate shadow map with the proper size
	shadowMapper.ActivateSolidTexture( shadowParams.solidShadowMapSize, defren.GetUseInverseDepth() );
	
	// clear or copy shadow map
	OGL_CHECK( renderThread, glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_TRUE ) );
	
	if( pglClipControl && defren.GetUseInverseDepth() ){
		pglClipControl( GL_LOWER_LEFT, GL_ZERO_TO_ONE );
	}
	
	const GLfloat clearDepth = defren.GetClearDepthValueRegular();
	OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
	
	if( shadowParams.solid ){
		DebugTimer3Sample( plan, *pDebugInfoSolidShadowClear, true );
		
	}else{
		DebugTimer3Sample( plan, *pDebugInfoTransparentShadowClear, true );
	}
	
	OGL_CHECK( renderThread, glDepthFunc( defren.GetDepthCompareFuncRegular() ) );
	
	OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
	SetCullMode( false );
	
	// calculate lod level to use
	deoglLODCalculator lodCalculator;
	lodCalculator.SetMaxPixelError( shadowParams.lodMaxPixelError );
	lodCalculator.SetLodOffset( shadowParams.lodOffset );
	
	if( shadowParams.collideList1 ){
		lodCalculator.SetComponentLODProjection( *shadowParams.collideList1,
			shadowParams.lightPosition, shadowParams.lightView, shadowParams.lodFovX,
			shadowParams.lodFovY, shadowParams.solidShadowMapSize, shadowParams.solidShadowMapSize );
	}
	if( shadowParams.collideList2 ){
		lodCalculator.SetComponentLODProjection( *shadowParams.collideList2,
			shadowParams.lightPosition, shadowParams.lightView, shadowParams.lodFovX,
			shadowParams.lodFovY, shadowParams.solidShadowMapSize, shadowParams.solidShadowMapSize );
	}
	
	// render solid content. two different depth offsets for front and back faces are used. double sided always
	// counts as front facing. this way all can be rendered in one go
	renderParamBlock->MapBuffer();
	try{
		renderParamBlock->SetParameterDataMat4x3( deoglSkinShader::erutMatrixV,
			shadowParams.matrixCamera );
		renderParamBlock->SetParameterDataMat3x3( deoglSkinShader::erutMatrixVn,
			shadowParams.matrixCamera.GetRotationMatrix().Invert() );
		renderParamBlock->SetParameterDataVec4( deoglSkinShader::erutDepthOffset, 0.0f, 0.0f, 0.0f, 0.0f ); // not used
		
		if( defren.GetUseInverseDepth() ){
			decDMatrix matProj( shadowParams.matrixProjection );
			matProj.a34 -= 0.0001f;
			renderParamBlock->SetParameterDataMat4x4( deoglSkinShader::erutMatrixP, matProj );
			renderParamBlock->SetParameterDataMat4x4( deoglSkinShader::erutMatrixVP,
				shadowParams.matrixCamera * matProj );
			
		}else{
			OGL_CHECK( renderThread, glEnable( GL_POLYGON_OFFSET_FILL ) );
			OGL_CHECK( renderThread, pglPolygonOffset( smOffsetScale, smOffsetBias ) );
			
			renderParamBlock->SetParameterDataMat4x4( deoglSkinShader::erutMatrixP,
				shadowParams.matrixProjection );
			renderParamBlock->SetParameterDataMat4x4( deoglSkinShader::erutMatrixVP,
				shadowParams.matrixCamera * shadowParams.matrixProjection );
		}
		
		renderParamBlock->SetParameterDataBVec4( deoglSkinShader::erutConditions1, false, false, false, false );
		
	}catch( const deException & ){
		renderParamBlock->UnmapBuffer();
		throw;
	}
	renderParamBlock->UnmapBuffer();
	
	renderTask.Clear();
	renderTask.SetRenderParamBlock( renderParamBlock );
	renderTask.SetForceDoubleSided( true );
	
	addToRenderTask.Reset();
	addToRenderTask.SetSkinShaderType( deoglSkinTexture::estComponentShadowProjection );
	addToRenderTask.SetSolid( true );
	addToRenderTask.SetNoShadowNone( true );
	if( shadowParams.collideList1 ){
		addToRenderTask.AddComponents( *shadowParams.collideList1 );
	}
	if( shadowParams.collideList2 ){
		addToRenderTask.AddComponents( *shadowParams.collideList2 );
	}
	
	if( shadowParams.solid ){
		DebugTimer3Sample( plan, *pDebugInfoSolidShadowTask, true );
		
	}else{
		DebugTimer3Sample( plan, *pDebugInfoTransparentShadowTask, true );
	}
	
	renderTask.PrepareForRender();
	rengeom.RenderTask( renderTask );
	
	renderTask.SetForceDoubleSided( false );
	
	if( shadowParams.solid ){
		DebugTimer3Sample( plan, *pDebugInfoSolidShadowRender, true );
		
	}else{
		DebugTimer3Sample( plan, *pDebugInfoTransparentShadowRender, true );
	}
	
	// transparent pass only if we need a transparent shadow
	if( shadowParams.withTransparent ){
		shadowMapper.ActivateTransparentTexture( shadowParams.transpShadowMapSize, defren.GetUseInverseDepth() );
		
		OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
		const GLfloat clearColor[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
		OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
		OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
		
		renderTask.Clear();
		renderTask.SetRenderParamBlock( renderParamBlock );
		
		if( shadowParams.solid ){
			DebugTimer3Sample( plan, *pDebugInfoSolidShadowClearTransp, true );
			
		}else{
			DebugTimer3Sample( plan, *pDebugInfoTransparentShadowClearTransp, true );
		}
		
		addToRenderTask.SetSolid( false );
		if( shadowParams.collideList1 ){
			addToRenderTask.AddComponents( *shadowParams.collideList1 );
		}
		if( shadowParams.collideList2 ){
			addToRenderTask.AddComponents( *shadowParams.collideList2 );
		}
		
		if( shadowParams.solid ){
			DebugTimer3Sample( plan, *pDebugInfoSolidShadowTranspTask, true );
			
		}else{
			DebugTimer3Sample( plan, *pDebugInfoTransparentShadowTranspTask, true );
		}
		
		renderTask.PrepareForRender();
		rengeom.RenderTask( renderTask );
		
		if( shadowParams.solid ){
			DebugTimer3Sample( plan, *pDebugInfoSolidShadowTranspRender, true );
			
		}else{
			DebugTimer3Sample( plan, *pDebugInfoTransparentShadowTranspRender, true );
		}
	}
	
	// cleanup
	OGL_CHECK( renderThread, glDisable( GL_POLYGON_OFFSET_FILL ) );
}

void deoglRenderLightSpot::RenderAmbientMap( deoglRenderPlanLight &planLight,
deoglShadowMapper &shadowMapper, const sShadowParams &shadowParams ) {
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "LightSpot.RenderAmbientMap" );
	deoglSPBlockUBO * const renderParamBlock = renderThread.GetRenderers().GetLight().GetOccMapPB();
	deoglAddToRenderTask &addToRenderTask = renderThread.GetRenderers().GetLight().GetAddToRenderTask();
	deoglRenderTask &renderTask = renderThread.GetRenderers().GetLight().GetRenderTask();
	deoglRenderGeometry &rengeom = renderThread.GetRenderers().GetGeometry();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	const deoglConfiguration &config = renderThread.GetConfiguration();
	const float smOffsetScale = config.GetShadowMapOffsetScale();
	const float smOffsetBias = config.GetShadowMapOffsetBias();
	
	// set states
	OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	
	// activate shadow map with the proper size
	shadowMapper.ActivateAmbientTexture( shadowParams.ambientMapSize, defren.GetUseInverseDepth() );
	
	// clear map
	OGL_CHECK( renderThread, glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthFunc( defren.GetDepthCompareFuncRegular() ) );
	
	if( pglClipControl && defren.GetUseInverseDepth() ){
		pglClipControl( GL_LOWER_LEFT, GL_ZERO_TO_ONE );
	}
	
	const GLfloat clearDepth = defren.GetClearDepthValueRegular();
	OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
	
	OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
	SetCullMode( false );
	
	// setup polygon offset
	OGL_CHECK( renderThread, glEnable( GL_POLYGON_OFFSET_FILL ) );
	
	if( defren.GetUseInverseDepth() ){
		OGL_CHECK( renderThread, pglPolygonOffset( -smOffsetScale, -smOffsetBias ) );
		
	}else{
		OGL_CHECK( renderThread, pglPolygonOffset( smOffsetScale, smOffsetBias ) );
	}
	
	// render occlusion meshes. two different depth offsets for front and back faces are used. double sided always
	// counts as front facing. this way all can be rendered in one go
	// update render parameter block
	renderParamBlock->MapBuffer();
	try{
		renderParamBlock->SetParameterDataMat4x4( 0, shadowParams.matrixCamera * shadowParams.matrixProjection ); // pMatrixVP[ 0 ]
		
	}catch( const deException & ){
		renderParamBlock->UnmapBuffer();
		throw;
	}
	renderParamBlock->UnmapBuffer();
	
	renderTask.Clear();
	renderTask.SetRenderParamBlock( renderParamBlock );
	
	addToRenderTask.Reset();
	addToRenderTask.SetEnforceShader( pShaderOccMap->GetRTSShader() );
	addToRenderTask.SetSolid( true );
	addToRenderTask.SetNoShadowNone( true );
	
	if( shadowParams.collideList1 ){
		addToRenderTask.AddOcclusionMeshes( *shadowParams.collideList1, false );
	}
	if( shadowParams.collideList2 ){
		addToRenderTask.AddOcclusionMeshes( *shadowParams.collideList2, false );
	}
	
	renderTask.PrepareForRender();
	rengeom.RenderTask( renderTask );
	
	// cleanup
	OGL_CHECK( renderThread, glDisable( GL_POLYGON_OFFSET_FILL ) );
}



void deoglRenderLightSpot::UpdateLightParamBlock( deoglLightShader &lightShader,
deoglSPBlockUBO &paramBlock, deoglRenderPlanLight &planLight ){
	deoglRLight &light = *planLight.GetLight()->GetLight();
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
	
	// NOTE the old version is not working anymore since spot cones can be squashed. this is
	//      now done using shadow texture coordinates which requires some different parameters
	float spotExponent = light.GetSpotExponent();
	
	/*
	float spotOuter = cosf( light.GetSpotAngle() * 0.5f );
	float spotInner = cosf( light.GetSpotAngle() * ( 1.0f - light.GetSpotSmoothness() ) * 0.5f );
	float spotFactor = spotInner - spotOuter;
	const float spotBase = spotOuter;
	
	if( spotFactor < 1e-5f ){
		spotFactor = 0.0f;
		spotExponent = 0.0f;
		
	}else{
		spotFactor = 1.0f / spotFactor;
	}
	*/
	
	// using spotConeRadius as radius of spot circle centered at zero and outer radius
	// of 0.5 the basic calculation used for attenuation is this:
	//   attenuation = ( 0.5 - spotConeRadius ) / ( 0.5 * spotSmoothness )
	// 
	// rewriting as MAD calculation:
	//   attenuation = spotConeRadius * factor + base
	//   factor = -1.0 / ( 0.5 * spotSmoothness )
	//   base = 0.5 / ( 0.5 * spotSmoothness )
	
	float spotFactor = 0.5f * light.GetSpotSmoothness();
	float spotBase = 0.0f;
	
	if( spotFactor < FLOAT_SAFE_EPSILON ){
		spotFactor = 0.0f;
		spotExponent = 0.0f;
		
	}else{
		spotBase = 0.5f / spotFactor;
		spotFactor = -1.0f / spotFactor;
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
		
		target = lightShader.GetLightUniformTarget( deoglLightShader::elutLightSpotFactor );
		if( target != -1 ){
			paramBlock.SetParameterDataFloat( target, spotFactor );
		}
		
		target = lightShader.GetLightUniformTarget( deoglLightShader::elutLightSpotBase );
		if( target != -1 ){
			paramBlock.SetParameterDataFloat( target, spotBase );
		}
		
		target = lightShader.GetLightUniformTarget( deoglLightShader::elutLightSpotExponent );
		if( target != -1 ){
			paramBlock.SetParameterDataFloat( target, spotExponent );
		}
		
	}catch( const deException & ){
		paramBlock.UnmapBuffer();
		throw;
	}
	paramBlock.UnmapBuffer();
}

void deoglRenderLightSpot::UpdateInstanceParamBlock( deoglLightShader &lightShader,
deoglSPBlockUBO &paramBlock, deoglRenderPlan &plan, const deoglCollideListLight &cllight,
sShadowDepthMaps &shadowDepthMaps, const decDMatrix &matrixLP ){
	const deoglDeferredRendering &defren = GetRenderThread().GetDeferredRendering();
	const deoglConfiguration &config = GetRenderThread().GetConfiguration();
	const deoglRLight &light = *cllight.GetLight();
	
	const bool isDepthCompareLEqual = defren.GetDepthCompareFuncRegular() == GL_LEQUAL;
	const bool isCameraInside = cllight.GetCameraInside();
	const decDMatrix &matrixLight = light.GetMatrix();
	
	float pixelSize, noiseScale;
	int target;
	
	// calculate matrices
	decDMatrix matrixBias( decDMatrix::CreateBiasMatrix() );
	if( defren.GetUseInverseDepth() ){
		// for the inverse depth case we need the bias matrix without the z transformation
		// since the z transformation has been already done in the inverse projection matrix
		matrixBias.a33 = 1.0;
		matrixBias.a34 = 0.0;
	}
	
	const decDMatrix matrixLPBias( matrixLP * matrixBias );
	
	const decDMatrix &matrixCamera = plan.GetCameraMatrix();
	
	const decDMatrix matrixMV( matrixLight * matrixCamera );
	const decDMatrix matrixMVP( matrixMV * plan.GetProjectionMatrix() );
	
	const decDMatrix matrixCameraStereo( matrixCamera * plan.GetCameraStereoMatrix() );
	const decDMatrix &matrixProjectionStereo = plan.GetProjectionMatrixStereo();
	const decDMatrix matrixMVStereo( matrixLight * matrixCameraStereo );
	const decDMatrix matrixMVPStereo( matrixMVStereo * matrixProjectionStereo );
	
	//const decVector lightUp( matrixMV.TransformUp() );
	//const decVector lightRight( matrixMV.TransformRight() );
	
	const decDMatrix matrixShadow( matrixMV.Invert() * matrixLPBias );
	const decDMatrix matrixShadowStereo( matrixMVStereo.Invert() * matrixLPBias );
	
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
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutLightView );
		if( target != -1 ){
			paramBlock.SetParameterDataArrayVec3( target, 0, matrixMV.TransformView() );
			paramBlock.SetParameterDataArrayVec3( target, 1, matrixMVStereo.TransformView() );
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
			/*
			// no idea where i used this but right now only omni-direction light images require
			// a matrix. for this reason the same code as with point lights is used
			const decTexMatrix2 &transform = light.GetTransform();
			
			decMatrix matrix;
			matrix.a11 = transform.a11;
			matrix.a12 = transform.a12;
			matrix.a13 = transform.a13;
			matrix.a21 = transform.a21;
			matrix.a22 = transform.a22;
			matrix.a23 = transform.a23;
			
			// shadow maps are upside down compared to regular images. this is due to the images being
			// stored upside down to fit opengl. we need to reverse the flipping to get it right
			// TODO
			// change uniform to be a 3x2 matrix. for spot light set it to transform and apply the
			// y-flipping (1-y) required. for point light use shadow coordinates and apply rotation
			// matrix to go from world to local space and also flip (1-y)
			
			paramBlock.SetParameterDataArrayMat4x3( target, 0, matrix );
			...
			*/
			
			decMatrix matrixRotate;
			if( light.GetUseSkinTexture() ){
				matrixRotate = decMatrix::CreateRotation( light.GetUseSkinTexture()->GetOmniDirRotate() * TWO_PI )
					.QuickMultiply( decMatrix::CreateRotation( light.GetUseSkinTexture()->GetOmniDirRotateSpot() * TWO_PI ) );
			}
			paramBlock.SetParameterDataArrayMat4x3( target, 0, matrixRotate.QuickMultiply( matrixMV ).QuickInvert() );
			paramBlock.SetParameterDataArrayMat4x3( target, 1, matrixRotate.QuickMultiply( matrixMVStereo ).QuickInvert() );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadow1Solid );
		if( target != -1 ){
			if( shadowDepthMaps.shadow1Solid ){
				pixelSize = 1.0f / ( float )shadowDepthMaps.shadow1Solid->GetWidth();
				noiseScale = config.GetShadowCubePCFSize() / ( float )shadowDepthMaps.shadow1Solid->GetWidth();
				
			}else{
				pixelSize = 1.0f / 1024.0f;
				noiseScale = config.GetShadowCubePCFSize() / 1024.0f;
			}
			
			paramBlock.SetParameterDataVec3( target, pixelSize, pixelSize, noiseScale );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadow1Transparent );
		if( target != -1 ){
			if( shadowDepthMaps.shadow1Transp ){
				pixelSize = 1.0f / ( float )shadowDepthMaps.shadow1Transp->GetWidth();
				noiseScale = config.GetShadowCubePCFSize() / ( float )shadowDepthMaps.shadow1Transp->GetWidth();
				
			}else{
				pixelSize = 1.0f / 1024.0f;
				noiseScale = config.GetShadowCubePCFSize() / 1024.0f;
			}
			
			paramBlock.SetParameterDataVec3( target, pixelSize, pixelSize, noiseScale );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadow2Solid );
		if( target != -1 ){
			if( shadowDepthMaps.shadow2Solid ){
				pixelSize = 1.0f / ( float )shadowDepthMaps.shadow2Solid->GetWidth();
				noiseScale = config.GetShadowCubePCFSize() / ( float )shadowDepthMaps.shadow2Solid->GetWidth();
				
			}else{
				pixelSize = 1.0f / 1024.0f;
				noiseScale = config.GetShadowCubePCFSize() / 1024.0f;
			}
			
			paramBlock.SetParameterDataVec3( target, pixelSize, pixelSize, noiseScale );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadow2Transparent );
		if( target != -1 ){
			if( shadowDepthMaps.shadow2Transp ){
				pixelSize = 1.0f / ( float )shadowDepthMaps.shadow2Transp->GetWidth();
				noiseScale = config.GetShadowCubePCFSize() / ( float )shadowDepthMaps.shadow2Transp->GetWidth();
				
			}else{
				pixelSize = 1.0f / 1024.0f;
				noiseScale = config.GetShadowCubePCFSize() / 1024.0f;
			}
			
			paramBlock.SetParameterDataVec3( target, pixelSize, pixelSize, noiseScale );
		}
		
		target = lightShader.GetInstanceUniformTarget( deoglLightShader::eiutShadowDepthTransform );
		if( target != -1 ){
			const float znear = OGL_REN_LIG_ZNEAR;
			//const float zfar = light.GetRange();
			decVector2 depth2Pos;
			
			if( defren.GetUseInverseDepth() ){
				depth2Pos.x = -znear;
				depth2Pos.y = 0.0f;
				
			}else{
				depth2Pos.x = znear;
				depth2Pos.y = 1.0f;
			}
			
			paramBlock.SetParameterDataVec2( target, depth2Pos );
		}
		
	}catch( const deException & ){
		paramBlock.UnmapBuffer();
		throw;
	}
	paramBlock.UnmapBuffer();
}



void deoglRenderLightSpot::ResetDebugInfo(){
	pDebugInfoSolid->Clear();
	pDebugInfoTransparent->Clear();
	
	pDebugInfoSolidShadow->Clear();
	pDebugInfoSolidShadowClear->Clear();
	pDebugInfoSolidShadowClearTransp->Clear();
	pDebugInfoSolidShadowTask->Clear();
	pDebugInfoSolidShadowRender->Clear();
	pDebugInfoSolidShadowTranspTask->Clear();
	pDebugInfoSolidShadowTranspRender->Clear();
	pDebugInfoSolidLight->Clear();
	
	pDebugInfoTransparentShadow->Clear();
	pDebugInfoTransparentShadowClear->Clear();
	pDebugInfoTransparentShadowClearTransp->Clear();
	pDebugInfoTransparentShadowTask->Clear();
	pDebugInfoTransparentShadowRender->Clear();
	pDebugInfoTransparentShadowTranspTask->Clear();
	pDebugInfoTransparentShadowTranspRender->Clear();
	pDebugInfoTransparentLight->Clear();
}

void deoglRenderLightSpot::AddTopLevelDebugInfoSolid(){
	GetRenderThread().GetDebug().GetDebugInformationList().Add( pDebugInfoSolidDetail );
}

void deoglRenderLightSpot::AddTopLevelDebugInfoTransparent(){
	GetRenderThread().GetDebug().GetDebugInformationList().Add( pDebugInfoTransparentDetail );
}

void deoglRenderLightSpot::DevModeDebugInfoChanged(){
	const int details = GetRenderThread().GetDebug().GetDeveloperMode().GetDebugInfoDetails();
	const bool show = ( details & ( deoglDeveloperMode::edimLight | deoglDeveloperMode::edimLightSpot ) )
		== ( deoglDeveloperMode::edimLight | deoglDeveloperMode::edimLightSpot );
	
	pDebugInfoSolidDetail->SetVisible( show );
	//pDebugInfoSolidShadow->SetVisible( true );
	
	pDebugInfoTransparentDetail->SetVisible( show );
}



// Private Functions
//////////////////////

void deoglRenderLightSpot::pCleanUp(){
}
