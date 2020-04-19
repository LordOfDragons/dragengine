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

#include "deoglRenderDepthPass.h"
#include "deoglRenderGeometry.h"
#include "deoglRenderOcclusion.h"
#include "deoglRenderWorld.h"
#include "defren/deoglDeferredRendering.h"
#include "plan/deoglRenderPlanDebug.h"
#include "plan/deoglRenderPlan.h"
#include "plan/deoglRenderPlanMasked.h"
#include "task/deoglAddToRenderTask.h"
#include "task/deoglRenderTask.h"
#include "../collidelist/deoglCollideListComponent.h"
#include "../collidelist/deoglCollideList.h"
#include "../collidelist/deoglCollideListManager.h"
#include "../component/deoglRComponent.h"
#include "../debug/deoglDebugInformation.h"
#include "../debug/deoglDebugSaveTexture.h"
#include "../debug/deoglDebugSnapshot.h"
#include "../debug/debugSnapshot.h"
#include "../light/deoglRLight.h"
#include "../model/deoglModelLOD.h"
#include "../model/deoglRModel.h"
#include "../occlusiontest/deoglOcclusionTest.h"
#include "../occquery/deoglOcclusionQuery.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../renderthread/deoglRTChoices.h"
#include "../renderthread/deoglRTDebug.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTTexture.h"
#include "../shaders/deoglShaderCompiled.h"
#include "../shaders/deoglShaderDefines.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/deoglShaderSources.h"
#include "../shapes/deoglShape.h"
#include "../shapes/deoglShapeManager.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../vao/deoglVAO.h"

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

enum eSPCopyDepth{
	spcdQuadParams
};

enum eSPDepthDownsample{
	spddsTCClamp,
	spddsMipMapLevel
};



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



// Class deoglRenderDepthPass
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderDepthPass::deoglRenderDepthPass( deoglRenderThread &renderThread ) :
deoglRenderBase( renderThread )
{
	deoglConfiguration &config = renderThread.GetConfiguration();
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	const deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglShaderSources *sources;
	deoglShaderDefines defines;
	
	
	
	sources = shaderManager.GetSourcesNamed( "DefRen Copy Depth" );
	
	defines.AddDefine( "DEPTH_TEST", "1" );
	pShaderCopyDepth.TakeOver( shaderManager.GetProgramWith( sources, defines ) );
	
	defines.AddDefine( "COPY_COLOR", "1" );
	if( defren.GetUseInverseDepth() ){
		defines.AddDefine( "SHADOW_INVERSE_DEPTH", "1" );
	}
	pShaderCopyDepthColor.TakeOver( shaderManager.GetProgramWith( sources, defines ) );
	defines.RemoveAllDefines();
	
	
	
	sources = shaderManager.GetSourcesNamed( "DefRen Depth Downsample" );
	if( config.GetDefRenEncDepth() ){
		defines.AddDefine( "DECODE_IN_DEPTH", "1" );
	}
	if( defren.GetUseInverseDepth() ){
		defines.AddDefine( "INVERSE_DEPTH", "1" );
	}
	defines.AddDefine( "NO_TEXCOORD", "1" );
	defines.AddDefine( "USE_MIN_FUNCTION", "1" ); // so it works for SSR. should also work for SSAO
	pShaderDepthDownsample.TakeOver( shaderManager.GetProgramWith( sources, defines ) );
	defines.RemoveAllDefines();
	
	
	
	sources = shaderManager.GetSourcesNamed( "DefRen Depth-Only V3" );
	if( config.GetUseEncodeDepth() ){
		defines.AddDefine( "ENCODE_DEPTH", "1" );
	}
	
	pShaderDepthSolid.TakeOver( shaderManager.GetProgramWith( sources, defines ) );
	
	defines.AddDefine( "USE_CLIP_PLANE", "1" );
	if( config.GetUseEncodeDepth() ){
		defines.AddDefine( "ENCODE_DEPTH", "1" );
	}
	
	pShaderDepthClipSolid.TakeOver( shaderManager.GetProgramWith( sources, defines ) );
	
	
	
	sources = shaderManager.GetSourcesNamed( "DefRen Particle Depth" );
	if( config.GetUseEncodeDepth() ){
		defines.AddDefine( "ENCODE_DEPTH", "1" );
	}
	pShaderParticleDepthSolid.TakeOver( shaderManager.GetProgramWith( sources, defines ) );
	
	defines.AddDefine( "CLIP_DEPTH", "1" );
	pShaderParticleDepthSolidCD.TakeOver( shaderManager.GetProgramWith( sources, defines ) );
	defines.RemoveAllDefines();
	
	defines.AddDefine( "IGNORE_HOLES", "1" );
	if( config.GetUseEncodeDepth() ){
		defines.AddDefine( "ENCODE_DEPTH", "1" );
	}
	pShaderParticleDepthHoles.TakeOver( shaderManager.GetProgramWith( sources, defines ) );
	
	defines.AddDefine( "CLIP_DEPTH", "1" );
	pShaderParticleDepthHolesCD.TakeOver( shaderManager.GetProgramWith( sources, defines ) );
	defines.RemoveAllDefines();
	
	defines.AddDefine( "USE_CLIP_PLANE", "1" );
	if( config.GetUseEncodeDepth() ){
		defines.AddDefine( "ENCODE_DEPTH", "1" );
	}
	pShaderParticleDepthClipSolid.TakeOver( shaderManager.GetProgramWith( sources, defines ) );
	
	defines.AddDefine( "CLIP_DEPTH", "1" );
	pShaderParticleDepthClipSolidCD.TakeOver( shaderManager.GetProgramWith( sources, defines ) );
	defines.RemoveAllDefines();
	
	defines.AddDefine( "USE_CLIP_PLANE", "1" );
	defines.AddDefine( "IGNORE_HOLES", "1" );
	if( config.GetUseEncodeDepth() ){
		defines.AddDefine( "ENCODE_DEPTH", "1" );
	}
	pShaderParticleDepthClipHoles.TakeOver( shaderManager.GetProgramWith( sources, defines ) );
	
	defines.AddDefine( "CLIP_DEPTH", "1" );
	pShaderParticleDepthClipHolesCD.TakeOver( shaderManager.GetProgramWith( sources, defines ) );
	defines.RemoveAllDefines();
}

deoglRenderDepthPass::~deoglRenderDepthPass(){
}



// Rendering
//////////////

#define DO_QUICK_DEBUG 1

#ifdef DO_QUICK_DEBUG
#define QUICK_DEBUG_START( lower, upper ) \
	if( renderThread.GetConfiguration().GetQuickDebug() > upper \
	|| renderThread.GetConfiguration().GetQuickDebug() < lower ){
#define QUICK_DEBUG_END }
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



void deoglRenderDepthPass::RenderSolidDepthPass( deoglRenderPlan &plan, deoglRenderPlanMasked *mask ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglRenderWorld &renworld = renderThread.GetRenderers().GetWorld();
	
	DebugTimer1Reset( plan, true );
	
	// clear depth texture
	defren.ActivateFBODepth();
	
	OGL_CHECK( renderThread, glDepthMask( GL_TRUE ) );
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	
	if( pglClipControl && defren.GetUseInverseDepth() ){
		pglClipControl( GL_LOWER_LEFT, GL_ZERO_TO_ONE );
	}
	
	if( mask ){
		OGL_CHECK( renderThread, glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP ) );
		OGL_CHECK( renderThread, glStencilFunc( GL_EQUAL, 0x01, 0x01 ) );
		
	}else{
		OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	}
	
	OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
	
	const GLfloat clearDepth = defren.GetClearDepthValueRegular();
	OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
		// NOTE: Haiku MESA 17.1.10 fails to properly clear. No idea why
	
	OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
	
	// render depth geometry
	OGL_CHECK( renderThread, glDepthFunc( defren.GetDepthCompareFuncRegular() ) );
	RenderDepth( plan, mask, true, false, false ); // +solid, -maskedOnly, -reverseDepthTest
	if( renderThread.GetConfiguration().GetDebugSnapshot() == edbgsnapDepthPassBuffers ){
		deoglDebugSnapshot snapshot( renderThread );
		snapshot.SetEnableDepth( true );
		snapshot.SetName( "solid/depthpass-depth" );
		snapshot.TakeSnapshot();
		renderThread.GetConfiguration().SetDebugSnapshot( 0 );
	}
	
	QUICK_DEBUG_START( 14, 19 )
	// now is a good time to do occlusion queries for all lights having passed
	// the quick rejection test as well as all components which are costly
	// enough to justify wasting an occlusion query on them.
	RenderOcclusionQueryPass( plan, mask );
	DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoSolidGeometryOcclusion, true );
	
	// if we are rendering a solid pass and we have transparency render the
	// transparency counter pass to determine how many layers we need
	if( plan.GetHasTransparency() ){
		renderThread.GetRenderers().GetTransparencyCounter().CountTransparency( plan, mask );
		DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoSolidGeometryTranspCounter, true );
	}
	
	// copy the first depth texture to the second for later usage. this will be improved later on by adding a
	// render pass decoding the depth into a proper float texture for faster access without the need to worry
	// about stencil masks or using the same depth texture for testing as used for tapping
	#if 0
	if( config.GetDebugSnapshot() == 62 ){
		defren.GetDepthTexture2()->CopyFrom( *defren.GetDepthTexture1(), false, defren.GetWidth(), defren.GetHeight(), 0, 0, 0, 0 );
		/*
		defren.ActivateFBOTemporary2( true );
		
		OGL_CHECK( renderThread, glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ) );
		OGL_CHECK( renderThread, glDepthMask( GL_TRUE ) );
		OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
		OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
		OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
		OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
		
		renderThread.GetShader().ActivateShader( pShaderCopyDepthColor );
		renderThread.GetTexture().GetStages()->EnableTexture( 0, *defren.GetDepthTexture1(), GetSamplerClampNearest() );
		defren.SetShaderParamFSQuad( *pShaderCopyDepthColor->GetCompiled(), spcdQuadParams );
		defren.RenderFSQuadVAO();
		*/
	}
	#endif
	
	QUICK_DEBUG_END
}



void deoglRenderDepthPass::RenderDepth( deoglRenderPlan &plan, deoglRenderPlanMasked *mask,
bool solid, bool maskedOnly, bool reverseDepthTest ){
DBG_ENTER_PARAM3("RenderDepthPass", "%p", mask, "%d", solid, "%d", maskedOnly)
	deoglRenderThread &renderThread = GetRenderThread();
	deoglRenderGeometry &rengeom = renderThread.GetRenderers().GetGeometry();
	deoglConfiguration &config = renderThread.GetConfiguration();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglCollideList &collideList = plan.GetCollideList();
	deoglRenderPlanDebug * const planDebug = plan.GetDebug();
	deoglRenderWorld &renworld = renderThread.GetRenderers().GetWorld();
	deoglRenderTask &renderTask = *renworld.GetRenderTask();
	deoglAddToRenderTask &addToRenderTask = *renworld.GetAddToRenderTask();
	
	// debug
	if( planDebug ){
		if( solid ){
			planDebug->IncrementRenderPasses();
			
		}else{
			planDebug->IncrementTransparentPasses();
		}
	}
	
	// depth pass
	if( config.GetUseEncodeDepth() ){
		OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
		
	}else{
		OGL_CHECK( renderThread, glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ) );
	}
	OGL_CHECK( renderThread, glDepthMask( GL_TRUE ) );
	
	OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
	
	if( defren.GetUseFadeOutRange() && false /* alpha blend problem */ ){
		OGL_CHECK( renderThread, glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );
		OGL_CHECK( renderThread, glEnable( GL_BLEND ) );
		
	}else{
		OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	}
	
	OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
	SetCullMode( plan.GetFlipCulling() );
	
	
	// build render task
	renderTask.Clear();
	renderTask.SetRenderParamBlock( renworld.GetRenderPB() );
	
	addToRenderTask.Reset();
	addToRenderTask.SetSolid( solid );
	addToRenderTask.SetNoNotReflected( plan.GetNoReflections() );
	addToRenderTask.SetNoRendered( mask );
	
	// components
	if( mask && mask->GetUseClipPlane() ){
		if( reverseDepthTest ){
			addToRenderTask.SetSkinShaderType( deoglSkinTexture::estComponentDepthClipPlaneReversed );
			
		}else{
			addToRenderTask.SetSkinShaderType( deoglSkinTexture::estComponentDepthClipPlane );
		}
		
	}else{
		if( reverseDepthTest ){
			addToRenderTask.SetSkinShaderType( deoglSkinTexture::estComponentDepthReversed );
			
		}else{
			addToRenderTask.SetSkinShaderType( deoglSkinTexture::estComponentDepth );
		}
	}
	addToRenderTask.AddComponents( collideList );
	
	// billboards
	if( mask && mask->GetUseClipPlane() ){
		if( reverseDepthTest ){
			addToRenderTask.SetSkinShaderType( deoglSkinTexture::estBillboardDepthClipPlaneReversed );
			
		}else{
			addToRenderTask.SetSkinShaderType( deoglSkinTexture::estBillboardDepthClipPlane );
		}
		
	}else{
		if( reverseDepthTest ){
			addToRenderTask.SetSkinShaderType( deoglSkinTexture::estBillboardDepthReversed );
			
		}else{
			addToRenderTask.SetSkinShaderType( deoglSkinTexture::estBillboardDepth );
		}
	}
	addToRenderTask.AddBillboards( collideList );
	
	// prop fields
	if( mask && mask->GetUseClipPlane() ){
		if( reverseDepthTest ){
			addToRenderTask.SetSkinShaderType( deoglSkinTexture::estPropFieldDepthClipPlaneReversed );
			addToRenderTask.AddPropFields( collideList, false );
			addToRenderTask.SetSkinShaderType( deoglSkinTexture::estPropFieldImposterDepthClipPlaneReversed );
			addToRenderTask.AddPropFields( collideList, true );
			
		}else{
			addToRenderTask.SetSkinShaderType( deoglSkinTexture::estPropFieldDepthClipPlane );
			addToRenderTask.AddPropFields( collideList, false );
			addToRenderTask.SetSkinShaderType( deoglSkinTexture::estPropFieldImposterDepthClipPlane );
			addToRenderTask.AddPropFields( collideList, true );
		}
		
	}else{
		if( reverseDepthTest ){
			addToRenderTask.SetSkinShaderType( deoglSkinTexture::estPropFieldDepthReversed );
			addToRenderTask.AddPropFields( collideList, false );
			addToRenderTask.SetSkinShaderType( deoglSkinTexture::estPropFieldImposterDepthReversed );
			addToRenderTask.AddPropFields( collideList, true );
			
		}else{
			addToRenderTask.SetSkinShaderType( deoglSkinTexture::estPropFieldDepth );
			addToRenderTask.AddPropFields( collideList, false );
			addToRenderTask.SetSkinShaderType( deoglSkinTexture::estPropFieldImposterDepth );
			addToRenderTask.AddPropFields( collideList, true );
		}
	}
	
	// height terrains
	if( mask && mask->GetUseClipPlane() ){
		if( reverseDepthTest ){
			addToRenderTask.SetSkinShaderType( deoglSkinTexture::estHeightMapDepthClipPlaneReversed );
			
		}else{
			addToRenderTask.SetSkinShaderType( deoglSkinTexture::estHeightMapDepthClipPlane );
		}
		
	}else{
		if( reverseDepthTest ){
			addToRenderTask.SetSkinShaderType( deoglSkinTexture::estHeightMapDepthReversed );
			
		}else{
			addToRenderTask.SetSkinShaderType( deoglSkinTexture::estHeightMapDepth );
		}
	}
	addToRenderTask.AddHeightTerrains( collideList, true );
	
	// particles
	if( renderThread.GetChoices().GetRealTransparentParticles() ){
		if( mask && mask->GetUseClipPlane() ){
			if( reverseDepthTest ){
				addToRenderTask.SetSkinShaderType(
					deoglSkinTexture::estParticleDepthClipPlaneReversed );
				addToRenderTask.SetSkinShaderTypeRibbon(
					deoglSkinTexture::estParticleRibbonDepthClipPlaneReversed );
				addToRenderTask.SetSkinShaderTypeBeam(
					deoglSkinTexture::estParticleBeamDepthClipPlaneReversed );
				
			}else{
				addToRenderTask.SetSkinShaderType(
					deoglSkinTexture::estParticleDepthClipPlane );
				addToRenderTask.SetSkinShaderTypeRibbon(
					deoglSkinTexture::estParticleRibbonDepthClipPlane );
				addToRenderTask.SetSkinShaderTypeBeam(
					deoglSkinTexture::estParticleBeamDepthClipPlane );
			}
			
		}else{
			if( reverseDepthTest ){
				addToRenderTask.SetSkinShaderType(
					deoglSkinTexture::estParticleDepthReversed );
				addToRenderTask.SetSkinShaderTypeRibbon(
					deoglSkinTexture::estParticleRibbonDepthReversed );
				addToRenderTask.SetSkinShaderTypeBeam(
					deoglSkinTexture::estParticleBeamDepthReversed );
				
			}else{
				addToRenderTask.SetSkinShaderType( deoglSkinTexture::estParticleDepth );
				addToRenderTask.SetSkinShaderTypeRibbon( deoglSkinTexture::estParticleRibbonDepth );
				addToRenderTask.SetSkinShaderTypeBeam( deoglSkinTexture::estParticleBeamDepth );
			}
		}
		addToRenderTask.AddParticles( collideList );
	}
	
	if( renderTask.GetShaderCount() > 0 ){
		if( planDebug && plan.GetRenderPassNumber() == 1 ){
			const int componentCount = collideList.GetComponentCount();
			deoglEnvironmentMapList envMapList;
			int i;
			
			for( i=0; i<componentCount; i++ ){
				const deoglCollideListComponent &clistComponent = *collideList.GetComponentAt( i );
				const deoglRComponent &component = *clistComponent.GetComponent();
				
				if( ! component.GetRenderVisible() ){
					continue;
				}
				if( ! component.GetModel() ){
					continue;
				}
				
				const deoglModelLOD &modelLOD = component.GetModel()->GetLODAt( clistComponent.GetLODLevel() );
				
				planDebug->IncrementRenderedObjects();
				planDebug->IncrementRenderedTriangles( modelLOD.GetFaceCount() );
				planDebug->IncrementLODTriangles( component.GetModel()->GetLODAt( 0 ).GetFaceCount() );
				/*
				if( component.GetRenderEnvMap() ){
					envMapList.AddIfMissing( component.GetRenderEnvMap() );
				}
				if( component.GetRenderEnvMapFade() ){
					envMapList.AddIfMissing( component.GetRenderEnvMapFade() );
				}
				*/
			}
			
			planDebug->IncrementRenderedEnvMaps( envMapList.GetCount() );
		}
		
		//if( config.GetQuickDebug() == 100 ){
		//	renderTask.SortInstancesByDistance( ogl.GetQuickSorter(), plan.GetCameraPosition(), plan.GetInverseCameraMatrix().TransformView() );
		//}
		renderTask.PrepareForRender( renderThread );
		
		if( config.GetDebugSnapshot() == edbgsnapDepthPassRenTask ){
			renderThread.GetLogger().LogInfo( "RenderWorld.pRenderDepthPass: render task" );
			renderTask.DebugPrint( renderThread.GetLogger() );
		}
		
		if( solid ){
			DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoSolidGeometryDepthTask, true );
			
		}else{
			DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoTransparentDepthTask, true );
		}
		
		rengeom.RenderTask( renderTask );
		
		if( solid ){
			DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoSolidGeometryDepthRender, true );
			
		}else{
			DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoTransparentDepthRender, true );
		}
	}
	
	// TODO
	// optimize render passes later on. each pass has to reconstruct the depth from the depth texture.
	// this boils down in general to these instructions:
	// 
	//    vec3 position = vec3( texelFetch( texDepth, ivec2( gl_FragCoord.xy ), 0 ).r );
	//    position.z = pPosTransform.x / ( pPosTransform.y - position.z );
	//    #ifdef FULLSCREENQUAD
	//       position.xy = vScreenCoord * pPosTransform.zw * position.zz;
	//    #else
	//       position.xy = vLightVolumePos.xy * position.zz / vLightVolumePos.zz;
	//    #endif
	// 
	// rendering directly to an RGB16 color texture is slow since we loose the double speed writing
	// possible with depth textures. using an additional render pass though this problem can be
	// worked around. the depth is rendered as above into a conventional depth texture. afterwards
	// an RGB16 texture is rendered with the depth texture as input. the used shader does nothing
	// else than reconstructing the view position from the input depth texture as done by the other
	// passes writing it out to the RGB16 texture. there is no double write in this scenario but
	// due to the short shader and the sequential access due to the fullscreen quad the cost is
	// not any worse than a fullscreen post processing effect pass. as a result though all following
	// passes do not require to reconstruct the position over and over again saving shader processing
	// time. only downside is the memory consumption. for the old way two depth textures are required
	// each consuming 4 bytes per pixel for a total of 8 bytes per pixel. for the new way one depth
	// texture is required (for the stencil mask) consuming 4 bytes per pixel and two color position
	// textures of RGB16 format each consuming 6 bytes per pixel for a total of 16 bytes per pixel.
	// thus the new version consumes double the GPU memory compared to the old one but speeds up the
	// rendering. furthermore it allows to better use stencil masks to improve later render passes
	// without requiring costly re-rendering or copying of the stencil masks between depth textures.
	// 
	// NOTE
	// the light shaders use a different calculation for the light-volume case. it has to be checked
	// if this works correctly with these shaders or if the position there is a different one
DBG_EXIT("RenderDepthPass")
}



void deoglRenderDepthPass::DownsampleDepth(){
DBG_ENTER("DownsampleDepth")
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglTexture &texture = *defren.GetDepthTexture1();
	deoglShaderCompiled *shader;
	int height, width, i;
	
	const int mipMapLevelCount = texture.GetRealMipMapLevelCount();
	const GLfloat clearDepth = defren.GetClearDepthValueRegular();
	
	height = defren.GetHeight();
	width = defren.GetWidth();
	
	OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDepthFunc( GL_ALWAYS ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
	OGL_CHECK( renderThread, glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_TRUE ) );
	
	OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
	
	OGL_CHECK( renderThread, pglBindVertexArray( defren.GetVAOFullScreenQuad()->GetVAO() ) );
	
	renderThread.GetShader().ActivateShader( pShaderDepthDownsample );
	shader = pShaderDepthDownsample->GetCompiled();
	
	tsmgr.EnableTexture( 0, texture, GetSamplerClampNearest() );
	
	for( i=1; i<=mipMapLevelCount; i++ ){
		defren.ActivateFBODepthLevel( i );
		
		OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
		
		shader->SetParameterInt( spddsTCClamp, width - 1, height - 1 );
		shader->SetParameterInt( spddsMipMapLevel, i - 1 );
		
		width = decMath::max( width >> 1, 1 );
		height = decMath::max( height >> 1, 1 );
		
		OGL_CHECK( renderThread, glViewport( 0, 0, width, height ) );
		
		OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
	}
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == 61 ){
		decString text;
		int i;
		
		for( i=0; i<=mipMapLevelCount; i++ ){
			text.Format( "downsample_depth-pass%i", i );
			renderThread.GetDebug().GetDebugSaveTexture().SaveDepthTextureLevel(
				*defren.GetDepthTexture1(), i, text.GetString(), deoglDebugSaveTexture::edtDepth );
		}
	}
DBG_EXIT("DownsampleDepth")
}



void deoglRenderDepthPass::RenderOcclusionQueryPass( deoglRenderPlan &plan, deoglRenderPlanMasked *mask ){
DBG_ENTER_PARAM("RenderOcclusionQueryPass", "%p", mask)
	deoglRenderThread &renderThread = GetRenderThread();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	
	deoglCollideList &collideList = plan.GetCollideList();
	const int lightCount = collideList.GetLightCount();
	
	// NOTE: the currently does not fully work and breaks for lights the camera
	// is inside the light box. the same check should be used as for the 180
	// point light case and an occlusion test only done if the camera is outside
	// the light box. for the time being this code is disabled.
	// 
	// NOTE: another note concerning the occlusion query time. currently the
	// light class checks if a result exists and only returns hidden if the
	// query count is zero. this way no time is wasted waiting for a result
	// to arrive. rendering seems though to be faster than the occlusion test
	// can return. not much one can do here except reintroducing the assigning
	// of lights to rooms to cull them this way. solution pending.
	
	collideList.MarkLightsVisible( true );
	
	if( lightCount == 0 ){
		DBG_EXIT("RenderOcclusionQueryPass(earily)")
		return;
	}
	
	const decDMatrix &matrixV = plan.GetCameraMatrix();
	const decDMatrix matrixP( plan.GetProjectionMatrix() );
	deoglShapeManager &shapeManager = renderThread.GetBufferObject().GetShapeManager();
	deoglShape &shapeBox = *shapeManager.GetShapeAt( deoglRTBufferObject::esBox );
	const decDVector &campos = plan.GetCameraPosition();
	const decDVector extoff( 0.1, 0.1, 0.1 );
	deoglShaderCompiled *shader;
	int l;
	
	OGL_CHECK( renderThread, glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	OGL_CHECK( renderThread, glEnable( GL_DEPTH_TEST ) );
	OGL_CHECK( renderThread, glDepthFunc( defren.GetDepthCompareFuncRegular() ) );
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	
	shapeBox.ActivateVAO();
	
	if( mask && mask->GetUseClipPlane() ){
		const decVector &maskClipNormal = mask->GetClipNormal();
		
		renderThread.GetShader().ActivateShader( pShaderDepthClipSolid );
		shader = pShaderDepthClipSolid->GetCompiled();
		
		shader->SetParameterFloat( spdoClipPlane, maskClipNormal.x,
			maskClipNormal.y, maskClipNormal.z, mask->GetClipDistance() );
		
	}else{
		renderThread.GetShader().ActivateShader( pShaderDepthSolid );
		shader = pShaderDepthSolid->GetCompiled();
	}
	
	// spdoMatrixDiffuse // hole or clip depth but both not used for light
	// spdoMaterialGamma // not used
	// spdoViewport // not used
	// spdoPFMatrix // not used for light
	
	for( l=0; l<lightCount; l++ ){
		deoglRLight &light = *collideList.GetLightAt( l );
		light.UpdateLightVolume();
		
		const decDVector &lminext = light.GetMinimumExtend();
		const decDVector &lmaxext = light.GetMaximumExtend();
		light.SetInsideCamera( ( campos > lminext - extoff ) && ( campos < lmaxext + extoff ) );
		if( light.GetCameraInside() ){
			continue;
		}
		
		const decDMatrix matrixModel( decDMatrix::CreateScale( ( lmaxext - lminext ) * 0.5 )
			* decDMatrix::CreateTranslation( ( lminext + lmaxext ) * 0.5 ) );
		const decDMatrix matrixMV( matrixModel * matrixV );
		
		shader->SetParameterDMatrix4x3( spdoMatrixMV, matrixMV );
		shader->SetParameterDMatrix4x4( spdoMatrixMVP, matrixMV * matrixP );
		
		deoglOcclusionQuery &occquery = light.GetOcclusionQuery();
		occquery.BeginQuery( deoglOcclusionQuery::eqtAny );
		shapeBox.RenderFaces();
		occquery.EndQuery();
	}
	
	pglBindVertexArray( 0 );
DBG_EXIT("RenderOcclusionQueryPass")
}
