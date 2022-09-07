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
#include "deoglRenderVR.h"
#include "deoglRenderWorld.h"
#include "defren/deoglDeferredRendering.h"
#include "plan/deoglRenderPlanDebug.h"
#include "plan/deoglRenderPlan.h"
#include "plan/deoglRenderPlanMasked.h"
#include "task/deoglAddToRenderTask.h"
#include "task/deoglRenderTask.h"
#include "../collidelist/deoglCollideList.h"
#include "../collidelist/deoglCollideListComponent.h"
#include "../collidelist/deoglCollideListLight.h"
#include "../collidelist/deoglCollideListManager.h"
#include "../component/deoglRComponent.h"
#include "../debug/deoglDebugInformation.h"
#include "../debug/deoglDebugSaveTexture.h"
#include "../debug/deoglDebugSnapshot.h"
#include "../debug/debugSnapshot.h"
#include "../debug/deoglDebugTraceGroup.h"
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
#include "../texture/arraytexture/deoglArrayTexture.h"
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
	deoglShaderDefines defines, commonDefines;
	deoglShaderSources *sources;
	
	
	renderThread.GetShader().SetCommonDefines( commonDefines );
	
	
	defines = commonDefines;
	sources = shaderManager.GetSourcesNamed( "DefRen Depth Downsample" );
	if( defren.GetUseInverseDepth() ){
		defines.SetDefine( "INVERSE_DEPTH", true );
	}
	defines.SetDefine( "NO_TEXCOORD", true );
	defines.SetDefine( "USE_MIN_FUNCTION", true ); // so it works for SSR. should also work for SSAO
	pShaderDepthDownsample = shaderManager.GetProgramWith( sources, defines );
	
	
	defines = commonDefines;
	if( defren.GetUseInverseDepth() ){
		defines.SetDefine( "INVERSE_DEPTH", true );
	}
	defines.SetDefine( "NO_TEXCOORD", true );
	defines.SetDefine( "USE_MIN_FUNCTION", true ); // so it works for SSR. should also work for SSAO
	
	if( renderThread.GetChoices().GetRenderFSQuadStereoVSLayer() ){
		defines.SetDefines( "VS_RENDER_STEREO" );
		
	}else{
		sources = shaderManager.GetSourcesNamed( "DefRen Depth Downsample Stereo" );
		defines.SetDefine( "GS_RENDER_STEREO", true );
	}
	
	pShaderDepthDownsampleStereo = shaderManager.GetProgramWith( sources, defines );
	
	
	
	defines = commonDefines;
	sources = shaderManager.GetSourcesNamed( "DefRen Depth-Only V3" );
	if( config.GetUseEncodeDepth() ){
		defines.SetDefine( "ENCODE_DEPTH", true );
	}
	
	pShaderDepthSolid = shaderManager.GetProgramWith( sources, defines );
	
	defines.SetDefine( "USE_CLIP_PLANE", true );
	if( config.GetUseEncodeDepth() ){
		defines.SetDefine( "ENCODE_DEPTH", true );
	}
	
	pShaderDepthClipSolid = shaderManager.GetProgramWith( sources, defines );
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



void deoglRenderDepthPass::RenderSolidDepthPass( deoglRenderPlan &plan,
const deoglRenderPlanMasked *mask, bool xray ){
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "DepthPass.RenderSolidDepthPass" );
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
	RenderDepth( plan, mask, true, false, false, xray ); // +solid, -maskedOnly, -reverseDepthTest
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
	if( ! xray ){
		RenderOcclusionQueryPass( plan, mask );
		DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoSolidGeometryOcclusion, true );
	}
	
	// if we are rendering a solid pass and we have transparency render the
	// transparency counter pass to determine how many layers we need
	if( plan.GetHasTransparency() ){
		renderThread.GetRenderers().GetTransparencyCounter().CountTransparency( plan, mask );
		DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoSolidGeometryTranspCounter, true );
	}
	
	QUICK_DEBUG_END
}



void deoglRenderDepthPass::RenderDepth( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask,
bool solid, bool maskedOnly, bool reverseDepthTest, bool xray ){
DBG_ENTER_PARAM3("RenderDepthPass", "%p", mask, "%d", solid, "%d", maskedOnly)
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "DepthPass.RenderDepth" );
	deoglRenderGeometry &rengeom = renderThread.GetRenderers().GetGeometry();
	deoglConfiguration &config = renderThread.GetConfiguration();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglCollideList &collideList = plan.GetCollideList();
	deoglRenderPlanDebug * const planDebug = plan.GetDebug();
	deoglRenderWorld &renworld = renderThread.GetRenderers().GetWorld();
	
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
	
	if( solid && ! mask ){
		renderThread.GetRenderers().GetVR().RenderHiddenArea( plan );
	}
	
	OGL_CHECK( renderThread, glEnable( GL_CULL_FACE ) );
	SetCullMode( plan.GetFlipCulling() );
	
	deoglRenderTask *renderTask;
	
	if( solid ){
		deoglRenderPlanTasks &tasks = plan.GetTasks();
		tasks.WaitFinishBuildingTasksDepth();
		
		renderTask = xray ? &tasks.GetSolidDepthXRayTask() : &tasks.GetSolidDepthTask();
		renderTask->SetRenderParamBlock( renworld.GetRenderPB() );
		
	}else{
		DebugTimer1Reset( plan, true );
		renderTask = renworld.GetRenderTask();
		deoglAddToRenderTask &addToRenderTask = *renworld.GetAddToRenderTask();
		
		// build render task
		renderTask->Clear();
		renderTask->SetRenderParamBlock( renworld.GetRenderPB() );
		renderTask->SetRenderVSStereo( plan.GetRenderStereo() && renderThread.GetChoices().GetRenderStereoVSLayer() );
		
		addToRenderTask.Reset();
		addToRenderTask.SetSolid( solid );
		addToRenderTask.SetNoNotReflected( plan.GetNoReflections() );
		addToRenderTask.SetNoRendered( mask );
		if( xray ){
			addToRenderTask.SetFilterXRay( true );
			addToRenderTask.SetXRay( true );
		}
		
		// components
		if( mask && mask->GetUseClipPlane() ){
			if( reverseDepthTest ){
				addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
					? deoglSkinTexture::estStereoComponentDepthClipPlaneReversed
					: deoglSkinTexture::estComponentDepthClipPlaneReversed );
				
			}else{
				addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
					? deoglSkinTexture::estStereoComponentDepthClipPlane
					: deoglSkinTexture::estComponentDepthClipPlane );
			}
			
		}else{
			if( reverseDepthTest ){
				addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
					? deoglSkinTexture::estStereoComponentDepthReversed
					: deoglSkinTexture::estComponentDepthReversed );
				
			}else{
				addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
					? deoglSkinTexture::estStereoComponentDepth
					: deoglSkinTexture::estComponentDepth );
			}
		}
		addToRenderTask.SetFilterDecal( true );
		addToRenderTask.SetDecal( false );
		
		addToRenderTask.AddComponents( collideList );
		
		addToRenderTask.SetFilterDecal( false );
		
		// billboards
		if( mask && mask->GetUseClipPlane() ){
			if( reverseDepthTest ){
				addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
					? deoglSkinTexture::estStereoBillboardDepthClipPlaneReversed
					: deoglSkinTexture::estBillboardDepthClipPlaneReversed );
				
			}else{
				addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
					? deoglSkinTexture::estStereoBillboardDepthClipPlane
					: deoglSkinTexture::estBillboardDepthClipPlane );
			}
			
		}else{
			if( reverseDepthTest ){
				addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
					? deoglSkinTexture::estStereoBillboardDepthReversed
					: deoglSkinTexture::estBillboardDepthReversed );
				
			}else{
				addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
					? deoglSkinTexture::estStereoBillboardDepth
					: deoglSkinTexture::estBillboardDepth );
			}
		}
		addToRenderTask.AddBillboards( collideList );
		
		// prop fields
		deoglSkinTexture::eShaderTypes propFieldShaderType1, propFieldShaderType2;
		
		if( mask && mask->GetUseClipPlane() ){
			if( reverseDepthTest ){
				propFieldShaderType1 = plan.GetRenderStereo()
					? deoglSkinTexture::estStereoPropFieldDepthClipPlaneReversed
					: deoglSkinTexture::estPropFieldDepthClipPlaneReversed;
				propFieldShaderType2 = plan.GetRenderStereo()
					? deoglSkinTexture::estStereoPropFieldImposterDepthClipPlaneReversed
					: deoglSkinTexture::estPropFieldImposterDepthClipPlaneReversed;
				
			}else{
				propFieldShaderType1 = plan.GetRenderStereo()
					? deoglSkinTexture::estStereoPropFieldDepthClipPlane
					: deoglSkinTexture::estPropFieldDepthClipPlane;
				propFieldShaderType2 = plan.GetRenderStereo()
					? deoglSkinTexture::estStereoPropFieldImposterDepthClipPlane
					: deoglSkinTexture::estPropFieldImposterDepthClipPlane;
			}
			
		}else{
			if( reverseDepthTest ){
				propFieldShaderType1 = plan.GetRenderStereo()
					? deoglSkinTexture::estStereoPropFieldDepthReversed
					: deoglSkinTexture::estPropFieldDepthReversed;
				propFieldShaderType2 = plan.GetRenderStereo()
					? deoglSkinTexture::estStereoPropFieldImposterDepthReversed
					: deoglSkinTexture::estPropFieldImposterDepthReversed;
				
			}else{
				propFieldShaderType1 = plan.GetRenderStereo()
					? deoglSkinTexture::estStereoPropFieldDepth
					: deoglSkinTexture::estPropFieldDepth;
				propFieldShaderType2 = plan.GetRenderStereo()
					? deoglSkinTexture::estStereoPropFieldImposterDepth
					: deoglSkinTexture::estPropFieldImposterDepth;
			}
		}
		
		addToRenderTask.SetSkinShaderType( propFieldShaderType1 );
		addToRenderTask.AddPropFields( collideList, false );
		
		addToRenderTask.SetSkinShaderType( propFieldShaderType2 );
		addToRenderTask.AddPropFields( collideList, true );
		
		// height terrains
		if( mask && mask->GetUseClipPlane() ){
			if( reverseDepthTest ){
				addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
					? deoglSkinTexture::estStereoHeightMapDepthClipPlaneReversed
					: deoglSkinTexture::estHeightMapDepthClipPlaneReversed );
				
			}else{
				addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
					? deoglSkinTexture::estStereoHeightMapDepthClipPlane
					: deoglSkinTexture::estHeightMapDepthClipPlane );
			}
			
		}else{
			if( reverseDepthTest ){
				addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
					? deoglSkinTexture::estStereoHeightMapDepthReversed
					: deoglSkinTexture::estHeightMapDepthReversed );
				
			}else{
				addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
					? deoglSkinTexture::estStereoHeightMapDepth
					: deoglSkinTexture::estHeightMapDepth );
			}
		}
		addToRenderTask.AddHeightTerrains( collideList, true );
		
		// particles
		if( renderThread.GetChoices().GetRealTransparentParticles() ){
			if( mask && mask->GetUseClipPlane() ){
				if( reverseDepthTest ){
					addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
						? deoglSkinTexture::estStereoParticleDepthClipPlaneReversed
						: deoglSkinTexture::estParticleDepthClipPlaneReversed );
					addToRenderTask.SetSkinShaderTypeRibbon( plan.GetRenderStereo()
						? deoglSkinTexture::estStereoParticleRibbonDepthClipPlaneReversed
						: deoglSkinTexture::estParticleRibbonDepthClipPlaneReversed );
					addToRenderTask.SetSkinShaderTypeBeam( plan.GetRenderStereo()
						? deoglSkinTexture::estStereoParticleBeamDepthClipPlaneReversed
						: deoglSkinTexture::estParticleBeamDepthClipPlaneReversed );
					
				}else{
					addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
						? deoglSkinTexture::estStereoParticleDepthClipPlane
						: deoglSkinTexture::estParticleDepthClipPlane );
					addToRenderTask.SetSkinShaderTypeRibbon( plan.GetRenderStereo()
						? deoglSkinTexture::estStereoParticleRibbonDepthClipPlane
						: deoglSkinTexture::estParticleRibbonDepthClipPlane );
					addToRenderTask.SetSkinShaderTypeBeam( plan.GetRenderStereo()
						? deoglSkinTexture::estStereoParticleBeamDepthClipPlane
						: deoglSkinTexture::estParticleBeamDepthClipPlane );
				}
				
			}else{
				if( reverseDepthTest ){
					addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
						? deoglSkinTexture::estStereoParticleDepthReversed
						: deoglSkinTexture::estParticleDepthReversed );
					addToRenderTask.SetSkinShaderTypeRibbon( plan.GetRenderStereo()
						? deoglSkinTexture::estStereoParticleRibbonDepthReversed
						: deoglSkinTexture::estParticleRibbonDepthReversed );
					addToRenderTask.SetSkinShaderTypeBeam( plan.GetRenderStereo()
						? deoglSkinTexture::estStereoParticleBeamDepthReversed
						: deoglSkinTexture::estParticleBeamDepthReversed );
					
				}else{
					addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
						? deoglSkinTexture::estStereoParticleDepth
						: deoglSkinTexture::estParticleDepth );
					addToRenderTask.SetSkinShaderTypeRibbon( plan.GetRenderStereo()
						? deoglSkinTexture::estStereoParticleRibbonDepth
						: deoglSkinTexture::estParticleRibbonDepth );
					addToRenderTask.SetSkinShaderTypeBeam( plan.GetRenderStereo()
						? deoglSkinTexture::estStereoParticleBeamDepth
						: deoglSkinTexture::estParticleBeamDepth );
				}
			}
			addToRenderTask.AddParticles( collideList );
		}
		
		renderTask->PrepareForRender();
		DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoTransparentDepthTask, true );
	}
	
	if( renderTask->GetShaderCount() > 0 ){
		if( planDebug && plan.GetRenderPassNumber() == 1 ){
			const int componentCount = collideList.GetComponentCount();
			deoglEnvironmentMapList envMapList;
			int i;
			
			for( i=0; i<componentCount; i++ ){
				const deoglCollideListComponent &clistComponent = *collideList.GetComponentAt( i );
				const deoglRComponent &component = *clistComponent.GetComponent();
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
		if( config.GetDebugSnapshot() == edbgsnapDepthPassRenTask ){
			renderThread.GetLogger().LogInfo( "RenderWorld.pRenderDepthPass: render task" );
			renderTask->DebugPrint( renderThread.GetLogger() );
		}
		
		rengeom.RenderTask( *renderTask );
		
		if( solid ){
			DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoSolidGeometryDepthRender, true );
			
		}else{
			DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoTransparentDepthRender, true );
		}
	}
	
	
	// outline
	const deoglDebugTraceGroup debugTraceOutline( renderThread, "DepthPass.RenderDepth.Outline" );
	if( solid ){
		deoglRenderPlanTasks &tasks = plan.GetTasks();
		renderTask = xray ? &tasks.GetSolidDepthOutlineXRayTask() : &tasks.GetSolidDepthOutlineTask();
		renderTask->SetRenderParamBlock( renworld.GetRenderPB() );
		
	}else{
		DebugTimer1Reset( plan, true );
		renderTask = renworld.GetRenderTask();
		deoglAddToRenderTask &addToRenderTask = *renworld.GetAddToRenderTask();
		
		renderTask->Clear();
		renderTask->SetRenderParamBlock( renworld.GetRenderPB() );
		renderTask->SetRenderVSStereo( plan.GetRenderStereo() && renderThread.GetChoices().GetRenderStereoVSLayer() );
		
		addToRenderTask.Reset();
		addToRenderTask.SetOutline( true );
		addToRenderTask.SetFilterDecal( true );
		addToRenderTask.SetDecal( false );
		addToRenderTask.SetSolid( solid );
		addToRenderTask.SetNoNotReflected( plan.GetNoReflections() );
		addToRenderTask.SetNoRendered( mask );
		if( xray ){
			addToRenderTask.SetFilterXRay( true );
			addToRenderTask.SetXRay( xray );
		}
		
		if( mask && mask->GetUseClipPlane() ){
			if( reverseDepthTest ){
				addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
					? deoglSkinTexture::estStereoOutlineDepthClipPlaneReversed
					: deoglSkinTexture::estOutlineDepthClipPlaneReversed );
				
			}else{
				addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
					? deoglSkinTexture::estStereoOutlineDepthClipPlane
					: deoglSkinTexture::estOutlineDepthClipPlane );
			}
			
		}else{
			if( reverseDepthTest ){
				addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
					? deoglSkinTexture::estStereoOutlineDepthReversed
					: deoglSkinTexture::estOutlineDepthReversed );
				
			}else{
				addToRenderTask.SetSkinShaderType( plan.GetRenderStereo()
					? deoglSkinTexture::estStereoOutlineDepth
					: deoglSkinTexture::estOutlineDepth );
			}
		}
		addToRenderTask.AddComponents( collideList );
		
		renderTask->PrepareForRender();
		DebugTimer1Sample( plan, *renworld.GetDebugInfo().infoTransparentDepthTask, true );
	}
	
	if( renderTask->GetShaderCount() > 0 ){
		SetCullMode( ! plan.GetFlipCulling() );
		rengeom.RenderTask( *renderTask );
		SetCullMode( plan.GetFlipCulling() );
		
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



void deoglRenderDepthPass::DownsampleDepth( deoglRenderPlan &plan ){
DBG_ENTER("DownsampleDepth")
	deoglRenderThread &renderThread = GetRenderThread();
	const deoglDebugTraceGroup debugTrace( renderThread, "DepthPass.DownsampleDepth" );
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglArrayTexture &texture = *defren.GetDepthTexture1();
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
	
	deoglShaderProgram * const program = plan.GetRenderStereo() ? pShaderDepthDownsampleStereo : pShaderDepthDownsample;
	renderThread.GetShader().ActivateShader( program );
	shader = program->GetCompiled();
	
	tsmgr.EnableArrayTexture( 0, texture, GetSamplerClampNearest() );
	
	for( i=1; i<=mipMapLevelCount; i++ ){
		defren.ActivateFBODepthLevel( i );
		
		OGL_CHECK( renderThread, pglClearBufferfv( GL_DEPTH, 0, &clearDepth ) );
		
		shader->SetParameterInt( spddsTCClamp, width - 1, height - 1 );
		shader->SetParameterInt( spddsMipMapLevel, i - 1 );
		
		width = decMath::max( width >> 1, 1 );
		height = decMath::max( height >> 1, 1 );
		
		OGL_CHECK( renderThread, glViewport( 0, 0, width, height ) );
		
		RenderFullScreenQuad( plan );
	}
	
	if( renderThread.GetConfiguration().GetDebugSnapshot() == 61 ){
		decString text;
		
		for( i=0; i<=mipMapLevelCount; i++ ){
			text.Format( "downsample_depth-pass%i", i );
			renderThread.GetDebug().GetDebugSaveTexture().SaveDepthArrayTextureLevel(
				*defren.GetDepthTexture1(), i, text.GetString(), deoglDebugSaveTexture::edtDepth );
		}
	}
DBG_EXIT("DownsampleDepth")
}



void deoglRenderDepthPass::RenderOcclusionQueryPass( deoglRenderPlan &plan, const deoglRenderPlanMasked *mask ){
DBG_ENTER_PARAM("RenderOcclusionQueryPass", "%p", mask)
	deoglCollideList &collideList = plan.GetCollideList();
	
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
	// 
	// NOTE: GI required reordering rendering to support the expensive operation.
	// Now the first light rendering happens rather soon after the depth pass.
	// this causes the first light to request for the occlusion query result nearly
	// directly after the query has been started. this causes delays up to 10ms
	// which is not acceptable. since GI requires light shadows to be rendered
	// even if not visible or outside the view the entire occlusion query is not
	// helping much at all. so dropping it for the time being and improving shadow
	// rendering performance instead
	
	collideList.MarkLightsCulled( false );
	
	DBG_EXIT("RenderOcclusionQueryPass(disabled)")
	
#if 0
	const int lightCount = collideList.GetLightCount();
	if( lightCount == 0 ){
		DBG_EXIT("RenderOcclusionQueryPass(earily)")
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	const decDMatrix &matrixV = plan.GetCameraMatrix();
	const decDMatrix matrixP( plan.GetProjectionMatrix() );
	deoglShapeManager &shapeManager = renderThread.GetBufferObject().GetShapeManager();
	deoglShape &shapeBox = *shapeManager.GetShapeAt( deoglRTBufferObject::esBox );
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
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
		deoglCollideListLight &cllight = *collideList.GetLightAt( l );
// 		if( cllight.GetCameraInside() ){
		if( cllight.GetCameraInsideOccQueryBox() ){
			continue;
		}
		
		const deoglRLight &light = *cllight.GetLight();
		const decDVector &minExtend = light.GetMinimumExtend();
		const decDVector &maxExtend = light.GetMaximumExtend();
		
		const decDMatrix matrixModel( decDMatrix::CreateScale( ( maxExtend - minExtend ) * 0.5 )
			* decDMatrix::CreateTranslation( ( minExtend + maxExtend ) * 0.5 ) );
		const decDMatrix matrixMV( matrixModel * matrixV );
		
		shader->SetParameterDMatrix4x3( spdoMatrixMV, matrixMV );
		shader->SetParameterDMatrix4x4( spdoMatrixMVP, matrixMV * matrixP );
		
		deoglOcclusionQuery &occquery = cllight.GetOcclusionQuery();
		occquery.BeginQuery( deoglOcclusionQuery::eqtAny );
		shapeBox.RenderFaces();
		occquery.EndQuery();
	}
	
	pglBindVertexArray( 0 );
DBG_EXIT("RenderOcclusionQueryPass")
#endif
}
