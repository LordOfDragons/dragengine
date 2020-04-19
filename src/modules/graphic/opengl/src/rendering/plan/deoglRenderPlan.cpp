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

#include <stdio.h>
#include <string.h>

#include "deoglRenderPlan.h"
#include "deoglRenderPlanDebug.h"
#include "deoglRenderPlanLight.h"
#include "deoglRenderPlanSkyLight.h"
#include "deoglRenderPlanMasked.h"
#include "deoglPlanVisitorCullElements.h"
#include "deoglRenderPlanEnvMap.h"
#include "../deoglRenderOcclusion.h"
#include "../deoglRenderReflection.h"
#include "../deoglRenderWorld.h"
#include "../cache/deoglRenderCacheLight.h"
#include "../cache/deoglRenderCacheLightShadow.h"
#include "../defren/deoglDeferredRendering.h"
#include "../light/deoglRenderLight.h"
#include "../lod/deoglLODCalculator.h"
#include "../../billboard/deoglRBillboard.h"
#include "../../collidelist/deoglCollideListComponent.h"
#include "../../collidelist/deoglCollideList.h"
#include "../../collidelist/deoglCollideListManager.h"
#include "../../component/deoglRComponent.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../debug/deoglDebugInformation.h"
#include "../../devmode/deoglDeveloperMode.h"
#include "../../envmap/deoglEnvironmentMap.h"
#include "../../light/deoglRLight.h"
#include "../../light/probes/deoglLightProbeTexture.h"
#include "../../model/deoglModelLOD.h"
#include "../../model/deoglRModel.h"
#include "../../occlusiontest/deoglOcclusionTest.h"
#include "../../particle/deoglRParticleEmitter.h"
#include "../../particle/deoglRParticleEmitterInstance.h"
#include "../../particle/deoglRParticleEmitterInstanceType.h"
#include "../../particle/deoglRParticleEmitterType.h"
#include "../../propfield/deoglRPropField.h"
#include "../../rendering/deoglRenderCanvas.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../renderthread/deoglRTDebug.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../shadow/deoglShadowCaster.h"
#include "../../skin/deoglRSkin.h"
#include "../../skin/deoglSkinTexture.h"
#include "../../skin/shader/deoglSkinShader.h"
#include "../../sky/deoglRSky.h"
#include "../../sky/deoglRSkyInstance.h"
#include "../../sky/deoglRSkyInstanceLayer.h"
#include "../../terrain/heightmap/deoglHTSCluster.h"
#include "../../terrain/heightmap/deoglHTView.h"
#include "../../terrain/heightmap/deoglHTViewSector.h"
#include "../../terrain/heightmap/deoglRHeightTerrain.h"
#include "../../texture/texture2d/deoglRenderableColorTexture.h"
#include "../../utils/collision/deoglDCollisionFrustum.h"
#include "../../utils/collision/deoglDCollisionSphere.h"
#include "../../world/deoglRCamera.h"
#include "../../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>

#ifdef OS_W32
#undef near
#undef far
#endif



// #define SPECIAL_DEBUG_ON 1
#ifdef SPECIAL_DEBUG_ON
extern int extDebugCompCount;
extern float extDebugCompCalculateWeights;
extern float extDebugCompTransformVertices;
extern float extDebugCompCalculateNormalsAndTangents;
extern float extDebugCompBuildVBO;
extern float extDebugCompTBO;
#endif



// Class deoglRenderPlan
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderPlan::deoglRenderPlan( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ){
	pWorld = NULL;
	
	pIsRendering = false;
	
	pLevel = 0;
	
	pCamera = NULL;
	pCameraFov = DEG2RAD * 90.0f;
	pCameraFovRatio = 1.0f;
	pCameraImageDistance = 0.01f;
	pCameraViewDistance = 200.0f;
	pViewportX = 0;
	pViewportY = 0;
	pViewportWidth = 100;
	pViewportHeight = 100;
	pAspectRatio = ( float )pViewportWidth / ( float )pViewportHeight;
	
	pUpscaleWidth = 0;
	pUpscaleHeight = 0;
	pUseUpscaling = false;
	pUpsideDown = false;
	pUseToneMap = true;
	pIgnoreDynamicComponents = false;
	pRenderDebugPass = true;
	pNoReflections = false;
	
	pUseLayerMask = false;
	
	pUseCustomFrustum = false;
	
	pFBOTarget = NULL;
	pFBOTargetCopyDepth = false;
	
	pHTView = NULL;
	
	pDirtyProjMat = true;
	
	pVisitorCullElements = new deoglPlanVisitorCullElements( this );
	
	pNoRenderedOccMesh = false;
	pFlipCulling = false;
	
	pDisableLights = false;
	pRescaleShadowMaps = true;
	pShadowMapSize = 0;
	pShadowCubeSize = 0;
	pShadowSkySize = 0;
	pForceShadowMapSize = 0;
	
	pEnvMaps = new deoglRenderPlanEnvMap[ 16 ];
	pEnvMapCount = 8; //4;
	
	pDirectEnvMapFader.SetFadePerTime( 1.0f );
	
	pLights = NULL;
	pLightCount = 0;
	pLightSize = 0;
	
	pSkyLights = NULL;
	pSkyLightCount = 0;
	pSkyLightSize = 0;
	
	pMaskedPlans = NULL;
	pMaskedPlanCount = 0;
	pMaskedPlanSize = 0;
	
	pEmptyPass = false;
	pClearStencilPassBits = false;
	pClearColor = true;
	
	pTransparencyLayerCount = 0;
	pCurTransparencyLayer = 0;
	pRenderPassNumber = 0;
	pStencilRefValue = 0;
	pStencilPrevRefValue = 0;
	pStencilWriteMask = 0xf;
	
	pDebug = NULL;
	pDebugTiming = false;
}

deoglRenderPlan::~deoglRenderPlan(){
	//RemoveAllMaskedPlans();
	if( pMaskedPlans ){
		while( pMaskedPlanSize > 0 ){
			pMaskedPlanSize--;
			if( pMaskedPlans[ pMaskedPlanSize ] ){
				delete pMaskedPlans[ pMaskedPlanSize ];
			}
		}
		
		delete [] pMaskedPlans;
	}
	
	if( pSkyLights ){
		while( pSkyLightCount > 0 ){
			pSkyLightSize--;
			if( pSkyLights[ pSkyLightSize ] ){
				delete pSkyLights[ pSkyLightSize ];
			}
		}
		
		delete [] pSkyLights;
	}
	
	//RemoveAllLights();
	if( pLights ){
		while( pLightSize > 0 ){
			pLightSize--;
			if( pLights[ pLightSize ] ){
				delete pLights[ pLightSize ];
			}
		}
		
		delete [] pLights;
	}
	
	if( pHTView ){
		delete pHTView;
	}
	
	if( pDebug ){
		delete pDebug;
	}
	
	if( pVisitorCullElements ){
		delete pVisitorCullElements;
	}
	
	pDirectEnvMapFader.DropAll();
	
	if( pEnvMaps ){
		delete [] pEnvMaps;
	}
}



// Management
///////////////

void deoglRenderPlan::SetWorld( deoglRWorld *world ){
	if( world == pWorld ){
		return;
	}
	
	pWorld = world;
	
	if( pHTView ){
		delete pHTView;
		pHTView = NULL;
	}
}

void deoglRenderPlan::SetLevel( int level ){
	pLevel = level;
}



void deoglRenderPlan::SetRescaleShadowMaps( bool rescale ){
	pRescaleShadowMaps = rescale;
}

void deoglRenderPlan::SetForceShadowMapSize( int forcedSize ){
	pForceShadowMapSize = forcedSize;
}



void deoglRenderPlan::PrepareRender(){
	if( pIsRendering ){
		return; // re-entrant rendering causes exceptions. ignore rendering in this case
	}
	
	pIsRendering = true;
	
	try{
		pBarePrepareRender();
		
	}catch( const deException & ){
		pIsRendering = false;
		throw;
	}
	
	pIsRendering = false;
}

void deoglRenderPlan::pBarePrepareRender(){
	deoglRenderCanvas &renderCanvas = pRenderThread.GetRenderers().GetCanvas();
	
	deoglRComponent *oglComponent;
	int c, componentCount;
	decDMatrix matCamProj;
	deoglRLight *oglLight;
	int i, l, lightCount;
	
	if( ! pWorld ){
		return;
	}
	
	renderCanvas.ClearAllDebugInfoPlanPrepare( *this );
	
	CleanUp(); // just to make sure everything is clean
	
	pDebugPrepare();
	
	// prepare the camera if existing
	if( pCamera ){
		pCamera->PrepareForRender();
	}
	
	// check reference position and update reference position camera matrix
	pWorld->CheckReferencePosition( pCameraPosition );
	UpdateRefPosCameraMatrix();
	
	// prepare the projection matrix if dirty
	if( pDirtyProjMat ){
		const deoglDeferredRendering &defren = pRenderThread.GetDeferredRendering();
		
		pProjectionMatrix = defren.CreateProjectionDMatrix( pViewportWidth, pViewportHeight,
			pCameraFov, pCameraFovRatio, pCameraImageDistance, pCameraViewDistance );
		pFrustumMatrix = defren.CreateFrustumDMatrix( pViewportWidth, pViewportHeight,
			pCameraFov, pCameraFovRatio, pCameraImageDistance, pCameraViewDistance );
		
		if( defren.GetUseInverseDepth() ){
			pDepthToPosition.x = -pCameraImageDistance;
			pDepthToPosition.y = 0.0f;
			
		}else{
			pDepthToPosition.x = pCameraImageDistance;
			pDepthToPosition.y = 1.0f;
		}
		
		pDepthToPosition.z = tanf( pCameraFov * 0.5f );
		pDepthToPosition.w = tanf( pCameraFov * pCameraFovRatio * 0.5f ) / pAspectRatio;
		
		/* non-infinite projection matrix
		const int q = pCameraViewDistance / ( pCameraViewDistance - pCameraImageDistance );
		pDepthToPosition.x = q * pCameraImageDistance;
		pDepthToPosition.y = q;
		*/
		
		pDirtyProjMat = false;
	}
	matCamProj = pCameraMatrix * pProjectionMatrix;
	
	// determine frustum to use
	deoglDCollisionFrustum *frustum = NULL;
	deoglDCollisionFrustum cameraFrustum;
	
	if( pUseCustomFrustum ){
		frustum = &pCustomFrustum;
		
	}else{
		cameraFrustum.SetFrustum( pCameraMatrix * pFrustumMatrix );
		frustum = &cameraFrustum;
	}
	
	// prepare world for rendering
	pWorld->PrepareForRender();
	
	// update environment maps
	if( ! pNoReflections ){
		pPlanEnvMaps(); // doing this here is safe and does not trash the occlusion maps
	}
	
	// update the height terrain if present
	pUpdateHTView();
	
	// plan what we can plan already
	pPlanSky();
	pPlanDominance();
	pPlanShadowCasting();
	pPlanOcclusionTesting();
	pPlanCollideList( frustum );
	pDisableLights = pWorld->GetDisableLights();
	renderCanvas.SampleDebugInfoPlanPrepareCollect( *this );
	
	// visibility
	pPlanVisibility( frustum );
	renderCanvas.SampleDebugInfoPlanPrepareCulling( *this );
	
	lightCount = pCollideList.GetLightCount();
	componentCount = pCollideList.GetComponentCount();
	const int billboardCount = pCollideList.GetBillboardCount();
	
	// update the blended environment map to use for rendering
	if( deoglSkinShader::REFLECTION_TEST_MODE == 2 ){
		pRenderThread.GetRenderers().GetReflection().UpdateEnvMap( *this );
	}
	//PlanEnvMaps(); // doing this here kills the occlusion map causing all kinds of problems
	renderCanvas.SampleDebugInfoPlanPrepareEnvMaps( *this );
	
	// update lod for visible elements
	pPlanLODLevels();
	
	// update height terrain vbo
	if( pHTView ){
		pHTView->GetHeightTerrain().UpdateVBOs();
		renderCanvas.SampleDebugInfoPlanPrepareHTViewVBOs( *this );
	}
	
	// prepare height terrain sectors
	
	/*
	if( pHTView ){
		pHTView->GetHeightTerrain()->UpdateVBOs();
DEBUG_PRINT_TIMER( "RenderPlan: PrepareRender: Update height terrain vbos" );
		
		pHTView->DetermineVisibilityUsing( &frustum );
		pHTView->UpdateLODLevels( pCameraPosition.ToVector() );
DEBUG_PRINT_TIMER( "RenderPlan: PrepareRender: Update height terrain" );
	}
	*/
	
	// prepare components for rendering.
	// visibility is already taken care of by the component. it does
	// not place itself in the octree if not visible.
/*
#ifdef DO_TIMING_COMP
debugCompReset();
#endif
	for( c=0; c<colList->GetComponentCount(); c++ ){
		oglComp = colList->GetComponentAt( c );
		oglComp->UpdateVBO();
	}
DEBUG_PRINT_TIMER( "PrepareWorld: Update component VBOs" );
#ifdef DO_TIMING_COMP
ogl.LogInfoFormat( "RenderPlan Timer: Update Component VBO: Calculate VBO Data = %iys", ( int )( elapsedCompCalcVBO * 1000000.0 ) );
ogl.LogInfoFormat( "RenderPlan Timer: Update Component VBO: Build VBO = %iys", ( int )( elapsedCompBuildVBO * 1000000.0 ) );
ogl.LogInfoFormat( "RenderPlan Timer: Update Component VBO: Weights = %iys", ( int )( elapsedCompWeights * 1000000.0 ) );
ogl.LogInfoFormat( "RenderPlan Timer: Update Component VBO: Transform vertices = %iys", ( int )( elapsedCompTVert * 1000000.0 ) );
ogl.LogInfoFormat( "RenderPlan Timer: Update Component VBO: Normals/tangents = %iys", ( int )( elapsedCompNorTan * 1000000.0 ) );
ogl.LogInfoFormat( "RenderPlan Timer: Update Component VBO: Normalize = %iys", ( int )( elapsedCompNormalize * 1000000.0 ) );
#endif
*/
	
	// update dynamic skins and masked rendering if required
	#ifdef SPECIAL_DEBUG_ON
	extDebugCompCalculateWeights = 0.0f; extDebugCompTransformVertices = 0.0f; extDebugCompCount = 0;
	extDebugCompCalculateNormalsAndTangents = 0.0f; extDebugCompBuildVBO = 0.0f; extDebugCompTBO = 0.0f;
	#endif
	
	renderCanvas.DebugTimer3Reset( *this, false );
	
	for( c=0; c<componentCount; c++ ){
		oglComponent = pCollideList.GetComponentAt( c )->GetComponent();
		oglComponent->TestCameraInside( pCameraPosition );
		oglComponent->UpdateVBO();
		renderCanvas.SampleDebugInfoPlanPrepareComponentsVBO( *this );
		oglComponent->UpdateRenderables( *this );
		renderCanvas.SampleDebugInfoPlanPrepareComponentsRenderables( *this );
		oglComponent->AddSkinStateRenderPlans( *this );
	}
	
	for( i=0; i<billboardCount; i++ ){
		deoglRBillboard &billboard = *pCollideList.GetBillboardAt( i );
		//billboard.TestCameraInside( pCameraPosition );
		billboard.UpdateRenderables( *this );
		//renderCanvas.SampleDebugInfoPlanPrepareBillboardsRenderables( *this );
		billboard.AddSkinStateRenderPlans( *this );
	}
	
	#ifdef SPECIAL_DEBUG_ON
	pRenderThread.GetLogger().LogInfoFormat( "deoglRenderPlan::PrepareRender CompCalculateWeights(%i) = %iys", extDebugCompCount, (int)(extDebugCompCalculateWeights*1e6f) );
	pRenderThread.GetLogger().LogInfoFormat( "deoglRenderPlan::PrepareRender CompTransformVertices(%i) = %iys", extDebugCompCount, (int)(extDebugCompTransformVertices*1e6f) );
	pRenderThread.GetLogger().LogInfoFormat( "deoglRenderPlan::PrepareRender CompCalculateNormalsAndTangents(%i) = %iys", extDebugCompCount, (int)(extDebugCompCalculateNormalsAndTangents*1e6f) );
	pRenderThread.GetLogger().LogInfoFormat( "deoglRenderPlan::PrepareRender CompBuildVBO(%i) = %iys", extDebugCompCount, (int)(extDebugCompBuildVBO*1e6f) );
	pRenderThread.GetLogger().LogInfoFormat( "deoglRenderPlan::PrepareRender CompTBO(%i) = %iys", extDebugCompCount, (int)(extDebugCompTBO*1e6f) );
	pRenderThread.GetLogger().LogInfoFormat( "deoglRenderPlan::PrepareRender UpdateVBO(%i) = %iys", extDebugCompCount, (int)((extDebugCompCalculateWeights+extDebugCompTransformVertices+extDebugCompCalculateNormalsAndTangents+extDebugCompBuildVBO+extDebugCompTBO)*1e6f) );
	#endif
	renderCanvas.SampleDebugInfoPlanPrepareComponents( *this );
	
	// update lights adding them to the list of all lights touched by an upcoming render call
	for( l=0; l<lightCount; l++ ){
		oglLight = pCollideList.GetLightAt( l );
		oglLight->TestCameraInside( *this );
		GetLightFor( oglLight );
	}
	
	// finish the collide list
//	pCollideList.SortLinear( world->GetSectorSize(), pCameraSector, pCameraPosition, pCameraInverseMatrix.TransformView() );
	pCollideList.SortComponentsByModels();
	renderCanvas.SampleDebugInfoPlanPrepareSort( *this );
	
	// now we are ready to produce a render plan
	pBuildRenderPlan();
	renderCanvas.SampleDebugInfoPlanPrepareBuildPlan( *this );
	
	// prepare lights for rendering. this is done after the build render plan phase
	// as there some light parameters can be set which affect the preparation
	// phase ( like for example no updating if not shadow casting )
	for( l=0; l<pSkyLightCount; l++ ){
		pSkyLights[ l ]->PrepareForRender( *this );
	}
	for( l=0; l<lightCount; l++ ){
		pCollideList.GetLightAt( l )->PrepareForRender( *this );
	}
	renderCanvas.SampleDebugInfoPlanPrepareLights( *this );
	
	// prepare particles for rendering
	const deoglParticleEmitterInstanceList &particleEmitterList = pCollideList.GetParticleEmitterList();
	const int particleEmitterCount = particleEmitterList.GetCount();
	
	for( i=0; i<particleEmitterCount; i++ ){
		particleEmitterList.GetAt( i )->PrepareForRender();
	}
	
	// determine the stencil properties for pass and mask rendering. right now the
	// mask is always 1 bit and the rest is available to the render pass number
	pStencilRefValue = 0;
	pStencilPrevRefValue = 0;
	pStencilWriteMask = 0xe;
	
	// reset the transparency layer stuff just for the case somebody reads it too early
	pClearStencilPassBits = false;
	pClearColor = true;
	pTransparencyLayerCount = 0;
	pCurTransparencyLayer = 0;
	
	renderCanvas.SampleDebugInfoPlanPrepare( *this );
}

void deoglRenderPlan::pPlanSky(){
	const int count = pWorld->GetSkyCount();
	int i;
	
	pSkyInstances.RemoveAll();
	pSkyBgColor.SetZero();
	
	for( i=0; i<count; i++ ){
		deoglRSkyInstance * const instance = pWorld->GetSkyAt( i );
		if( ! instance->GetRSky() ){
			continue;
		}
		if( pUseLayerMask && instance->GetLayerMask().IsNotEmpty() && pLayerMask.MatchesNot( instance->GetLayerMask() ) ){
			continue;
		}
		
		if( pSkyInstances.GetCount() == 0 ){
			pSkyBgColor = instance->GetRSky()->GetBgColor();
		}
		
		pSkyInstances.Add( instance );
	}
}

void deoglRenderPlan::pPlanDominance(){
	//const deoglGraphicContext &gc = pOgl->GetPrimaryGraphicContext();
	
	// determines the dominance of the render. this calcuates the relative space this render
	// occupies compared to the primary render target size. this is used to down-tune if the
	// render is rather small to speed up more important renders
	pDominance = 1.0f; //( float )( pViewportWidth * pViewportHeight ) / ( float )( gc.GetRenderTargetWidth() * gc.GetRenderTargetHeight() );
}

void deoglRenderPlan::pPlanShadowCasting(){
	const deoglConfiguration &config = pRenderThread.GetConfiguration();
	
	// largest screen size
	int renderSize = pViewportWidth;
	
	if( pViewportHeight > pViewportWidth ){
		renderSize = pViewportHeight;
	}
	
	// calculate the shadow map size to use. the shadow map size is the smalltest power of
	// two value equal or larger than the render size. hence the shadow map size for a
	// 800x600 screen would be 1024. for a large 1650x1050 screen this would be 2048 if the
	// system supports this size. for smaller screens this value drops quickly speeding
	// rendering up. hence a 400x300 screen would require only 512 shadow maps to look good
	// enough. with smaller screens like 200x150 as might be used for small remote views
	// 256 would be selected. reducing the shadow map size by factor 2 already reduces fill
	// rate and per-pixel workload by factor 4.
	
	// TODO replace the shadow map size with a scale factor which indicates the relation
	// between the shadow map size and the render viewport size. the base value is the
	// first value of two size greater or equal to the largest render viewport size. the
	// shadow map is then this base size multiplied by the shadow map factor. this allows
	// to increase or decrease the shadow map size while keeping the rescaling behavior
	// for different render viewport dimensions
	//int unclampedSize = 0;
	const int shadowMapSize = config.GetShadowMapSize();
	int shiftSize = 0;
	
	if( pForceShadowMapSize > 0 ){
		pShadowMapSize = decMath::clamp( pForceShadowMapSize, 16, shadowMapSize );
		pShadowCubeSize = pForceShadowMapSize;
		pShadowSkySize = pForceShadowMapSize;
		//unclampedSize = pShadowMapSize;
		
	}else{
		if( pRescaleShadowMaps ){
			// TODO review this code. it looks wrong from one end to the other. no idea
			//      what the original idea has been behind this code
			
			if( shadowMapSize > 1024 ){
				shiftSize = ( shadowMapSize / 1024 ) - 1;
				
			}else if( shadowMapSize < 1024 ){
				shiftSize = 1 - ( 1024 / shadowMapSize );
			}
			
			for( pShadowMapSize=16; pShadowMapSize<renderSize; pShadowMapSize<<=1 );
			
			if( shiftSize > 0 ){
				pShadowMapSize <<= shiftSize;
				
			}else if( shiftSize < 0 ){
				pShadowMapSize >>= -shiftSize;
			}
			//unclampedSize = pShadowMapSize;
			
			pShadowMapSize = decMath::clamp( pShadowMapSize, 16, shadowMapSize );
			pShadowCubeSize = pShadowMapSize;
			pShadowSkySize = pShadowMapSize;
			
			/*
			if( shiftSize > 0 ){
				pShadowCubeSize = config.GetShadowCubeSize() << shiftSize;
				
			}else if( shiftSize < 0 ){
				pShadowCubeSize = config.GetShadowCubeSize() >> shiftSize;
				
			}else{
				pShadowCubeSize = config.GetShadowCubeSize();
			}
			*/
			
		}else{
			pShadowMapSize = shadowMapSize;
			pShadowCubeSize = shadowMapSize; //config.GetShadowCubeSize();
			pShadowSkySize = shadowMapSize;
			//unclampedSize = pShadowMapSize;
		}
	}
	
	//printf( "shadow map size: rendersize=%i forced=%i shift=%i size=%i cube=%i sky=%i config=%i\n", renderSize, pForceShadowMapSize, shiftSize, pShadowMapSize, pShadowCubeSize, pShadowSkySize, shadowMapSize );
}

void deoglRenderPlan::pPlanOcclusionTesting(){
	deoglOcclusionTest &occtest = pRenderThread.GetOcclusionTest();
	
	// determine the occlusion map and base level to use for rendering. depending on the aspect
	// ratio of the screen the 1:1 or 2:1 occlusion map is used. using the 1:1 for aspect ratios
	// less than 1.5 allows to save half the render work as half the pixels are rendered. the
	// base level is used to start at a smaller occlusion map size if the screen size is small.
	// this allows to save render work as well.
	/*if( pAspectRatio < 1.5f ){
		pOcclusionMap = occtest.GetOcclisionMap1();
		
	}else{
		pOcclusionMap = occtest.GetOcclisionMap2();
	}*/
	
	if( pNoRenderedOccMesh ){
		pOcclusionMap = occtest.GetOcclisionMapMask();
		
	}else{
		pOcclusionMap = occtest.GetOcclisionMapMain();
	}
	
	pOcclusionMapBaseLevel = 0; // logic to choose this comes later
}

void deoglRenderPlan::pPlanCollideList( deoglDCollisionFrustum *frustum ){
	// add elements to the collide list
	pVisitorCullElements->Init( frustum );
	pVisitorCullElements->SetCullPixelSize( 1.0f );
	pVisitorCullElements->SetCullDynamicComponents( pIgnoreDynamicComponents );
	
	pVisitorCullElements->SetCullLayerMask( pUseLayerMask );
	pVisitorCullElements->SetLayerMask( pLayerMask );
	
	pVisitorCullElements->VisitWorldOctree( pWorld->GetOctree() );
// DEBUG_PRINT_TIMER( "RenderPlan.PrepareRender: Add elements colliding" );
	
	if( pHTView ){
		pCollideList.AddHTSectorsColliding( pHTView, frustum );
	}
// DEBUG_PRINT_TIMER( "RenderPlan.PrepareRender: Add height terrain sectors colliding" );
	
	pCollideList.AddPropFieldsColliding( *pWorld, frustum );
// DEBUG_PRINT_TIMER( "RenderPlan.PrepareRender: Add prop fields" );
	
	// HACK: add environment maps using a simple hack until we have something better
	const deoglEnvironmentMapList &envMapList = pWorld->GetEnvMapList();
	deoglDCollisionSphere envMapSphere( decDVector(), 20.0 );
	const int envMapCount = envMapList.GetCount();
	deoglEnvironmentMap *envMap;
	int envMapIndex;
	
	for( envMapIndex=0; envMapIndex<envMapCount; envMapIndex++ ){
		envMap = envMapList.GetAt( envMapIndex );
		
		if( envMap->GetSkyOnly() ){
			//pCollideList.GetEnvironmentMapList().Add( envMap );
			
		}else{
			envMapSphere.SetCenter( envMap->GetPosition() );
			
			if( frustum->SphereHitsFrustum( &envMapSphere ) ){
				pCollideList.GetEnvironmentMapList().Add( envMap );
			}
		}
	}
// DEBUG_PRINT_TIMER( "RenderPlan.PrepareRender: Add Env-Map" );
	
	// collect occlusion test data and upload it. we do something else in the mean
	// time until the GPU uploaded the data to avoid stalling as good as possible
	pPlanOcclusionTestInputData();
// DEBUG_PRINT_TIMER( "RenderPlan.PrepareRender: Prepare Occlusion Test Input Data" );
	
	// debug information if demanded
	pDebugVisibleNoCull();
}

void deoglRenderPlan::pPlanOcclusionTestInputData(){
	const int componentCount = pCollideList.GetComponentCount();
	deoglOcclusionTest &occtest = pRenderThread.GetOcclusionTest();
	const int lightCount = pCollideList.GetLightCount();
	int i;
	
	// add input data for all elements. skip a group of elements if there are not enough
	// elements visible. this avoids spending more time on testing than is used up for
	// rendering them
	occtest.RemoveAllInputData();
	
// 	if( componentCount >= 10 ){ // at least 10 components before testing them
		for( i=0; i<componentCount; i++ ){
			pCollideList.GetComponentAt( i )->GetComponent()->StartOcclusionTest( pCameraPosition );
		}
// 	}
	
// 	if( lightCount >= 3 ){ // at least 3 lights before testing them
		for( i=0; i<lightCount; i++ ){
			pCollideList.GetLightAt( i )->StartOcclusionTest( pCameraPosition );
		}
// 	}
	
	if( occtest.GetInputDataCount() > 0 ){
		occtest.UpdateVBO();
	}
	
	// debug information if demanded
	if( pDebug ){
		pDebug->IncrementOccTestCount( occtest.GetInputDataCount() );
	}
}

void deoglRenderPlan::pPlanLODLevels(){
	const deoglConfiguration &config = pRenderThread.GetConfiguration();
	const decDVector view = pCameraInverseMatrix.TransformView();
	deoglLODCalculator lodCalculator;
	
	if( pHTView ){
		pHTView->UpdateLODLevels( pCameraPosition.ToVector() );
	}
	
	lodCalculator.SetMaxPixelError( config.GetLODMaxPixelError() );
	lodCalculator.SetMaxErrorPerLevel( config.GetLODMaxErrorPerLevel() );
	
	lodCalculator.SetComponentLODProjection( pCollideList, pCameraPosition, view,
		pCameraFov, pCameraFov * pCameraFovRatio, pViewportWidth, pViewportHeight );
}

void deoglRenderPlan::pPlanEnvMaps(){
	const deoglEnvironmentMapList &envmapList = pWorld->GetEnvMapList();
	const int envmapCount = envmapList.GetCount();
	deoglEnvironmentMap *envmap;
	float distance;
	int i, j, k, cap;
	
	// determine the 4 closest environment maps around the camera. the code is for the time
	// being a bit special. first all env maps are moved from indices 0 to 3 to indices 4 to 7.
	// this allows to fill in the 4 closest environment maps in the indices 0 to 3 without
	// affecting the environment maps from the last frame. important is to add the environment
	// maps first in the new location and then remove them from the old one so the usage count
	// does not drop to 0 freeing them although we might reuse them right after. after the 4
	// closest environment maps are found the old environment maps in the indices 4 to 7 are
	// cleared. this way reused environment maps are kept while now unused free their resources
	// if possible
	for( i=0; i<pEnvMapCount; i++ ){
		pEnvMaps[ pEnvMapCount + i ].SetEnvMap( pEnvMaps[ i ].GetEnvMap() );
		pEnvMaps[ i ].SetEnvMap( NULL );
	}
	
	// first find the environment maps containing the camera
	for( i=0, j=0; i<pEnvMapCount && j<envmapCount; j++ ){
		envmap = envmapList.GetAt( j );
		
		for( k=0; k<i; k++ ){
			if( pEnvMaps[ k ].GetEnvMap() == envmap ){
				break;
			}
			if( envmap->GetPosition().IsEqualTo( pEnvMaps[ k ].GetEnvMap()->GetPosition() ) ){ // temporary hack required
				break;
			}
		}
		
		if( k == i && ! envmap->GetSkyOnly() && envmap->GetHasInfluenceBox()
		&& envmap->GetInfluenceCollisionBox().IsPointInside( pCameraPosition ) ){
			pEnvMaps[ i ].SetEnvMap( envmap );
			pEnvMaps[ i ].SetDistance( 0.0f );
			
			for( k=pEnvMapCount; k<pEnvMapCount*2; k++ ){
				if( pEnvMaps[ k ].GetEnvMap() == envmap ){
					pEnvMaps[ k ].SetEnvMap( NULL );
					break;
				}
			}
			if( k == pEnvMapCount*2 ){
				envmap->GetRenderPlanList().Add( this );
				envmap->AddPlanUsage();
			}
			
			envmap->PrepareForRender();
			i++;
		}
	}
	
	// now fill up with environment maps overlapping the found ones
	while( i < pEnvMapCount ){
		cap = i;
		
		for( j=0; i<pEnvMapCount && j<envmapCount; j++ ){
			envmap = envmapList.GetAt( j );
			
			for( k=0; k<i; k++ ){
				if( pEnvMaps[ k ].GetEnvMap() == envmap ){
					break;
				}
				if( envmap->GetPosition().IsEqualTo( pEnvMaps[ k ].GetEnvMap()->GetPosition() ) ){ // temporary hack required
					break;
				}
			}
			
			if( k == i && ! envmap->GetSkyOnly() && envmap->GetHasInfluenceBox() ){
				for( k=0; k<cap; k++ ){
					if( envmap->GetInfluenceCollisionBox().BoxHitsBox(
					&pEnvMaps[ k ].GetEnvMap()->GetInfluenceCollisionBox() ) ){
						break;
					}
				}
				
				if( k < cap ){
					pEnvMaps[ i ].SetEnvMap( envmap );
					pEnvMaps[ i ].SetDistance( 0.0f );
					
					for( k=pEnvMapCount; k<pEnvMapCount*2; k++ ){
						if( pEnvMaps[ k ].GetEnvMap() == envmap ){
							pEnvMaps[ k ].SetEnvMap( NULL );
							break;
						}
					}
					if( k == pEnvMapCount*2 ){
						envmap->GetRenderPlanList().Add( this );
						envmap->AddPlanUsage();
					}
					
					envmap->PrepareForRender();
					i++;
				}
			}
		}
		
		if( cap == i ){
			break;
		}
	}
	
	// eventually fill up the remaining slots with environment maps nearby
	for( ; i<pEnvMapCount; i++ ){
		for( j=0; j<envmapCount; j++ ){
			envmap = envmapList.GetAt( j );
			
			for( k=0; k<i; k++ ){
				if( envmap->GetPosition().IsEqualTo( pEnvMaps[ k ].GetEnvMap()->GetPosition() ) ){
					break;
				}
			}
			
			if( k == i && ! envmap->GetSkyOnly() ){
				distance = ( float )( ( envmap->GetPosition() - pCameraPosition ).Length() );
				
				if( ! pEnvMaps[ i ].GetEnvMap() || distance < pEnvMaps[ i ].GetDistance() ){
					pEnvMaps[ i ].SetEnvMap( envmap );
					pEnvMaps[ i ].SetDistance( distance );
				}
			}
		}
		
		envmap = pEnvMaps[ i ].GetEnvMap();
		if( envmap ){
			for( j=pEnvMapCount; j<pEnvMapCount*2; j++ ){
				if( pEnvMaps[ j ].GetEnvMap() == envmap ){
					pEnvMaps[ j ].SetEnvMap( NULL );
					break;
				}
			}
			if( j == pEnvMapCount*2 ){
				envmap->GetRenderPlanList().Add( this );
				envmap->AddPlanUsage();
			}
			
			envmap->PrepareForRender();
			
		}else{
			break;
		}
	}
	
	// if no environment maps have been found add positionless environment maps
	if( i == 0 ){
		for( j=0; i<pEnvMapCount && j<envmapCount; j++ ){
			envmap = envmapList.GetAt( j );
			
			if( envmap->GetSkyOnly() ){ // hack, requires a GetIsPositionless function
				pEnvMaps[ i ].SetEnvMap( envmap );
				pEnvMaps[ i ].SetDistance( 0.0f );
				
				for( k=pEnvMapCount; k<pEnvMapCount*2; k++ ){
					if( pEnvMaps[ k ].GetEnvMap() == envmap ){
						pEnvMaps[ k ].SetEnvMap( NULL );
						break;
					}
				}
				if( k == pEnvMapCount*2 ){
					envmap->GetRenderPlanList().Add( this );
					envmap->AddPlanUsage();
				}
				
				envmap->PrepareForRender();
				i++;
			}
		}
	}
	
	// sort the found environment maps by priority
	for( j=1; j<i; j++ ){
		if( pEnvMaps[ j ].GetEnvMap()->GetInfluencePriority() <=
		pEnvMaps[ j - 1 ].GetEnvMap()->GetInfluencePriority() ){
			continue;
		}
		
		envmap = pEnvMaps[ j - 1 ].GetEnvMap();
		distance = pEnvMaps[ j - 1 ].GetDistance();
		
		pEnvMaps[ j - 1 ].SetEnvMap( pEnvMaps[ j ].GetEnvMap() );
		pEnvMaps[ j - 1 ].SetDistance( pEnvMaps[ j ].GetDistance() );
		
		pEnvMaps[ j ].SetEnvMap( envmap );
		pEnvMaps[ j ].SetDistance( distance );
		
		if( j > 1 ){
			j -= 2;
		}
	}
	
	// for the direct environment map fader switch to the first environment map we found
	if( pEnvMapCount == 0 ){
		pDirectEnvMapFader.FadeTo( NULL );
		
	}else{
		pDirectEnvMapFader.FadeTo( pEnvMaps[ 0 ].GetEnvMap() );
	}
	
	// clean up
	for( i=pEnvMapCount; i<pEnvMapCount*2; i++ ){
		if( pEnvMaps[ i ].GetEnvMap() ){
			pDirectEnvMapFader.Drop( pEnvMaps[ i ].GetEnvMap() );
			
			pEnvMaps[ i ].GetEnvMap()->RemovePlanUsage();
			pEnvMaps[ i ].GetEnvMap()->GetRenderPlanList().Remove( this );
			pEnvMaps[ i ].SetEnvMap( NULL );
		}
	}
	
	/*
	printf( "envmap count %i\n", pEnvMapCount );
	for( i=0; i<pEnvMapCount; i++ ){
		if( pEnvMaps[ i ].GetEnvMap() ){
			printf( "envmap %i priority %i position (%g,%g,%g)\n", i, pEnvMaps[ i ].GetEnvMap()->GetInfluencePriority(), pEnvMaps[ i ].GetEnvMap()->GetPosition().x, pEnvMaps[ i ].GetEnvMap()->GetPosition().y, pEnvMaps[ i ].GetEnvMap()->GetPosition().z );
		}else{
			printf( "envmap %i NULL\n", i );
		}
	}
	*/
	
	// update hard limits
	/*
	for( i=0; i<envmapCount; i++ ){
		envmapList.GetAt( i )->PrepareCubeMapHardLimit();
	}
	*/
	
	// update soft limits
	/*
	for( i=0; i<envmapCount; i++ ){
		envmapList.GetAt( i )->PrepareCubeMapSoftLimit();
	}
	*/
	
	// update env map if required
	if( pWorld->GetEnvMapUpdateCount() > 0 ){
		deoglEnvironmentMap *updateEnvmap = NULL;
		
		// check if one of the 4 closest environment maps is in need of updating. if so choose this one
		for( i=0; i<pEnvMapCount; i++ ){
			if( pEnvMaps[ i ].GetEnvMap() && pEnvMaps[ i ].GetEnvMap()->GetDirty() ){
				updateEnvmap = pEnvMaps[ i ].GetEnvMap();
				break;
			}
		}
		
		// if no environment map has been chosen to update check if the sky environment map needs updating. if so choose this one
		if( ! updateEnvmap && pWorld->GetSkyEnvironmentMap()->GetDirty() ){
			updateEnvmap = pWorld->GetSkyEnvironmentMap();
		}
		
		// if the sky environment map is not updated yet do this first
		/*
		if( pWorld->GetSky() && pWorld->GetSky()->GetEnvironmentMap()->GetDirty() ){
			pWorld->GetSky()->GetEnvironmentMap()->Update();
			
			// rendering the environment map potentially alters the reference position. ensure the reference
			// position is back to a proper value and that the reference position camera matrix is correct
			pWorld->CheckReferencePosition( pCameraPosition );
			UpdateRefPosCameraMatrix();
			
			return;
		}
		*/
		
#if 0
		double envmapDistSquared, updateEnvmapDistSquared = 0.0;
		deoglEnvironmentMap *updateEnvmap = NULL;
		decDVector positionDifference;
		
		// unmark all environment maps
		for( i=0; i<envmapCount; i++ ){
			envmapList.GetAt( i )->SetMarked( false );
		}
		
		// find the next environment map to update. for the time being we choose the environment
		// map closest to the camera position that is dirty.
		
		// first check all environment maps assigned to visible components
#if 0
		count = pCollideList.GetComponentCount();
		for( i=0; i<count; i++ ){
			envmap = pCollideList.GetComponentAt( i )->GetComponent()->GetRenderEnvMap();
			
			if( envmap && ! envmap->GetMarked() ){
				envmap->SetMarked( true );
				
				if( envmap->GetDirty() ){
					positionDifference = envmap->GetPosition() - pCameraPosition;
					envmapDistSquared = positionDifference * positionDifference;
					
					if( ! updateEnvmap || envmapDistSquared < updateEnvmapDistSquared ){
						updateEnvmap = envmap;
						updateEnvmapDistSquared = envmapDistSquared;
					}
				}
			}
		}
#endif
		
		// if no environment map has been found yet look in the entire list of environment maps
		// for a match. this includes environment maps that are not yet visible but potentially
		// are in the next frame
		for( i=0; i<envmapCount; i++ ){
			envmap = envmapList.GetAt( i );
			
			if( ! envmap->GetMarked() ){
				envmap->SetMarked( true );
				
				if( envmap->GetDirty() ){
					positionDifference = envmap->GetPosition() - pCameraPosition;
					envmapDistSquared = positionDifference * positionDifference;
					
					if( ! updateEnvmap || envmapDistSquared < updateEnvmapDistSquared ){
						updateEnvmap = envmap;
						updateEnvmapDistSquared = envmapDistSquared;
					}
				}
			}
		}
#endif
		
		// if we found an environment map update it
		if( updateEnvmap ){
			updateEnvmap->AddReference(); // guard reference
			try{
				updateEnvmap->Update();
				
			}catch( const deException & ){
				updateEnvmap->FreeReference();
				throw;
			}
			updateEnvmap->FreeReference();
			
			// rendering the environment map potentially alters the reference position. ensure the reference
			// position is back to a proper value and that the reference position camera matrix is correct
			pWorld->CheckReferencePosition( pCameraPosition );
			UpdateRefPosCameraMatrix();
		}
	}
}

void deoglRenderPlan::pPlanVisibility( deoglDCollisionFrustum *frustum ){
	const deoglConfiguration &config = pRenderThread.GetConfiguration();
	
	// mark all elements in the view visible
	pCollideList.MarkElementsVisible( true );
	
	if( config.GetDebugNoCulling() ){
		pSkyVisible = true;
		
	}else{
		// determine if height terrain and sky are visible
		pCheckOutsideVisibility();
// DEBUG_PRINT_TIMER( "RenderPlan.PrepareRender: Check outside visibility" );
		
		// use occlusion testing to cull components and lights
		// 
		// ( lod levels of components have to be calculated already here for the case they are rendered )
		// 
		// in the first stage coarse grain occlusion tests are done. these tests are supposed to figure out
		// which elements and lights are potentially visible.
		// 
		// in the second stage fine grain occlusion tests are done. for this all elements and lights passing
		// the first stage fill in another set of occlusion tests if required to determine precisely what
		// has to be rendered. only some objects and lights require such an additional set of tests. typically
		// these are large, static components where additional tests help to not render more faces than
		// possible with the coarse grain tests. ( after the tests have been rendered there might be a chance
		// to do some other small work before fetching the results ).
		
		// if there are no input points skip the test
		if( pRenderThread.GetOcclusionTest().GetInputDataCount() > 0 ){
			pRenderThread.GetRenderers().GetOcclusion().RenderTestsCamera( *this );
		}
// DEBUG_PRINT_TIMER( "RenderPlan.PrepareRender: Occlusion Testing" );
	}
	
	pDebugVisibleCulled();
}

void deoglRenderPlan::pDebugPrepare(){
	if( pRenderThread.GetDebug().GetDeveloperMode().GetDebugRenderPlan() ){
		if( pDebug ){
			pDebug->Reset();
			
		}else{
			pDebug = new deoglRenderPlanDebug;
		}
		
	}else{
		if( pDebug ){
			delete pDebug;
			pDebug = NULL;
		}
	}
}

void deoglRenderPlan::pDebugVisibleNoCull(){
	/*
	if( pUseLayerMask ){
		const int componentCount = pCollideList.GetComponentCount();
		int i, triangleCount = 0;
		for( i=0; i<componentCount; i++ ){
			const deoglCollideListComponent &clistComponent = *pCollideList.GetComponentAt( i );
			const deoglRComponent &component = *clistComponent.GetComponent();
			if( ! component.GetModel() ){
				continue;
			}
			
			const deoglModelLOD &modelLOD = *component.GetModel()->GetLODAt( clistComponent.GetLODLevel() );
			triangleCount += modelLOD.GetFaceCount();
		}
		pRenderThread.GetLogger().LogInfoFormat( "colliderlist no occlusion test;%i;%i;%i\n", pCollideList.GetComponentCount(), pCollideList.GetLightCount(), triangleCount );
	}
	*/
	if( pDebug ){
		const int componentCount = pCollideList.GetComponentCount();
		const int lightCount = pCollideList.GetLightCount();
		deoglEnvironmentMapList envMapList;
		int i;
		
		pDebug->IncrementViewObjects( componentCount );
		pDebug->IncrementViewLights( lightCount );
		
		for( i=0; i<componentCount; i++ ){
			const deoglCollideListComponent &clistComponent = *pCollideList.GetComponentAt( i );
			const deoglRComponent &component = *clistComponent.GetComponent();
			
			if( component.GetModel() ){
				const deoglModelLOD &modelLOD = component.GetModel()->GetLODAt( clistComponent.GetLODLevel() );
				pDebug->IncrementViewTriangles( modelLOD.GetFaceCount() );
			}
			/*
			if( component.GetRenderEnvMap() ){
				envMapList.AddIfMissing( component.GetRenderEnvMap() );
			}
			if( component.GetRenderEnvMapFade() ){
				envMapList.AddIfMissing( component.GetRenderEnvMapFade() );
			}
			*/
		}
		
		pDebug->IncrementViewEnvMaps( envMapList.GetCount() );
	}
}

void deoglRenderPlan::pDebugVisibleCulled(){
	/*
	if( pUseLayerMask ){
		const int componentCount = pCollideList.GetComponentCount();
		int i, triangleCount = 0;
		for( i=0; i<componentCount; i++ ){
			const deoglCollideListComponent &clistComponent = *pCollideList.GetComponentAt( i );
			const deoglRComponent &component = *clistComponent.GetComponent();
			if( ! component.GetModel() ){
				continue;
			}
			
			const deoglModelLOD &modelLOD = *component.GetModel()->GetLODAt( clistComponent.GetLODLevel() );
			triangleCount += modelLOD.GetFaceCount();
		}
		pRenderThread.GetLogger().LogInfoFormat( "colliderlist occlusion tested;%i;%i;%i\n", pCollideList.GetComponentCount(), pCollideList.GetLightCount(), triangleCount );
	}
	*/
	if( pDebug ){
		const int componentCount = pCollideList.GetComponentCount();
		const int lightCount = pCollideList.GetLightCount();
		int i;
		
		pDebug->IncrementCullPSObjects( componentCount );
		pDebug->IncrementCullPSLights( lightCount );
		
		for( i=0; i<componentCount; i++ ){
			const deoglCollideListComponent &clistComponent = *pCollideList.GetComponentAt( i );
			const deoglRComponent &component = *clistComponent.GetComponent();
			
			if( ! component.GetModel() ){
				continue;
			}
			
			const deoglModelLOD &modelLOD = component.GetModel()->GetLODAt( clistComponent.GetLODLevel() );
			pDebug->IncrementCullPSTriangles( modelLOD.GetFaceCount() );
		}
	}
}

void deoglRenderPlan::PrepareRenderOneTurn(){
	/*
	deWorld *world = pWorld->GetWorld();
	int p, propFieldCount = world->GetPropFieldCount();
	deoglPropField *oglPropField;
	
	// prop fields can be prepared for one render turn
	for( p=0; p<propFieldCount; p++ ){
		oglPropField = ( deoglPropField* )world->GetPropFieldAt( p )->GetPeerGraphic();
		
		if( oglPropField ){
			oglPropField->PrepareInstances( pCameraPosition, decDMatrix( pCameraMatrix ) );
		}
	}
	*/
}

void deoglRenderPlan::PlanTransparency( int layerCount ){
	pTransparencyLayerCount = layerCount;
	pCurTransparencyLayer = 0;
	
	if( ! pHasTransparency ){
		// set light shadow parameters to default
		return;
	}
	
	//printf( "RenderPlan: Transparency Layer Count = %d\n", layerCount );
	
	// plan transparency handling for lights
	if( pDisableLights ){
		const int lightCount = pCollideList.GetLightCount();
		int i;
		
		for( i=0; i<lightCount; i++ ){
			deoglRLight &light = *pCollideList.GetLightAt( i );
			
			deoglShadowCaster &scaster = *light.GetShadowCaster();
			
			deoglSCSolid &scsolid = scaster.GetSolid();
			scsolid.SetPlanTransparentSize(
				decMath::max( scsolid.GetPlanDynamicSize() >> 1 /*2*/, 16 ) );
			
			deoglSCTransparent &sctransparent = scaster.GetTransparent();
			sctransparent.SetPlanTransparentSize(
				decMath::max( sctransparent.GetPlanDynamicSize() >> 1 /*2*/, 16 ) );
			
			deoglSCAmbient &scambient = scaster.GetAmbient();
			scambient.SetPlanTransparentSize(
				decMath::max( scambient.GetPlanDynamicSize() >> 1 /*2*/, 16 ) );
		}
	}
}

void deoglRenderPlan::Render(){
	if( pIsRendering ){
		// re-entrant rendering causes exceptions. render instead a black screen and do not clean up
		pRenderThread.GetRenderers().GetWorld().RenderBlackScreen( *this );
		return;
	}
	
	// to make sure we clean up everyting even after an exception try this all
	try{
		// update the shadow maps for all lights if not already existing
		
		// currently we just state what amount of memory would be used
		/*
		ogl.LogInfoFormat( "RenderPlan: %i lights.", pLightCount );
		deoglRenderCacheLight *cacheLight;
		int l, totalMemory=0, sizeSolid, sizeTransp, memSolid, memTransp;
		for( l=0; l<pLightCount; l++ ){
			cacheLight = pLights[ l ]->GetCacheLight();
			deoglRenderCacheLightShadow &shadowSolid = cacheLight->GetShadowSolid();
			sizeSolid = shadowSolid.GetSize();
			memSolid = shadowSolid.GetMemoryConsumption();
			if( cacheLight->GetUseTransparency() ){
				deoglRenderCacheLightShadow &shadowTransp = cacheLight->GetShadowTransparent();
				sizeTransp = shadowTransp.GetSize();
				memTransp = shadowTransp.GetMemoryConsumption();
			}else{
				sizeTransp = 0;
				memTransp = 0;
			}
			ogl.LogInfoFormat( "- light: cube=%i encDepth=%i transp=%i mem=%i ( sizeSolid=%i memSolid=%i ) ( sizeTransp=%i memTransp=%i )",
				cacheLight->GetUseCubeMaps() ? 1 : 0, cacheLight->GetUseEncodedDepth() ? 1 : 0,
				cacheLight->GetUseTransparency() ? 1 : 0, cacheLight->GetMemoryConsumption(),
				sizeSolid, memSolid, sizeTransp, memTransp );
			totalMemory += cacheLight->GetMemoryConsumption();
		}
		ogl.LogInfoFormat( "total memory consumption of shadow maps: %i", totalMemory );
		*/
		
		pRenderThread.GetRenderers().GetWorld().RenderWorld( *this, NULL );
		
	}catch( const deException &e ){
		e.PrintError();
		// add to exception trace
		CleanUp();
		throw;
	}
	
	CleanUp();
}

void deoglRenderPlan::CleanUp(){
	RemoveAllSkyInstances();
	RemoveAllMaskedPlans();
	RemoveAllLights();
	RemoveAllSkyLights();
	pDropLightsDynamic();
	pCollideList.Clear();
}



void deoglRenderPlan::SetCamera( deoglRCamera *camera ){
	pCamera = camera;
	
	if( camera ){
		pCameraPosition = camera->GetPosition();
		
	}else{
		pCameraPosition.SetZero();
	}
}

void deoglRenderPlan::SetCameraMatrix( const decDMatrix &matrix ){
	pCameraMatrix = matrix;
	pCameraInverseMatrix = matrix.Invert();
	
	// NOTE has to be this way. the camera can be the same but the matrix can be
	// something else for example in a mirror situation. the position from the
	// inverse matrix is correct. still though an additional parameter containing
	// the position specifically would be better and less error prone.
	
//	if( pCamera ){
//		pCameraPosition = pCamera->GetCamera()->GetPosition();
//		
//	}else{
		pCameraPosition = pCameraInverseMatrix.GetPosition();
//	}
}

void deoglRenderPlan::SetCameraParameters( float fov, float fovRatio, float imageDistance, float viewDistance ){
	if( fov <= 0.0f || fov >= PI || fovRatio <= 0.0f || imageDistance <= 0.0f || imageDistance >= viewDistance ) DETHROW( deeInvalidParam );
	
	pCameraFov = fov;
	pCameraFovRatio = fovRatio;
	pCameraImageDistance = imageDistance;
	pCameraViewDistance = viewDistance;
	
	pDirtyProjMat = true;
}

void deoglRenderPlan::SetViewport( int x, int y, int width, int height ){
	if( width < 1 || height < 1 ) DETHROW( deeInvalidParam );
	
	pViewportX = x;
	pViewportY = y;
	pViewportWidth = width;
	pViewportHeight = height;
	pAspectRatio = ( float )pViewportWidth / ( float )pViewportHeight;
	
	pDirtyProjMat = true;
}

void deoglRenderPlan::CopyCameraParametersFrom( const deoglRenderPlan &plan ){
	pViewportX = plan.pViewportX;
	pViewportY = plan.pViewportY;
	pViewportWidth = plan.pViewportWidth;
	pViewportHeight = plan.pViewportHeight;
	pAspectRatio = plan.pAspectRatio;
	pCameraFov = plan.pCameraFov;
	pCameraFovRatio = plan.pCameraFovRatio;
	pCameraImageDistance = plan.pCameraImageDistance;
	pCameraViewDistance = plan.pCameraViewDistance;
	
	pProjectionMatrix = plan.pProjectionMatrix;
	pFrustumMatrix = plan.pFrustumMatrix;
	pDepthToPosition = plan.pDepthToPosition;
	
	pDirtyProjMat = false;
}

void deoglRenderPlan::UpdateRefPosCameraMatrix(){
	const decVector refPosCam = ( pWorld->GetReferencePosition() - pCameraPosition ).ToVector(); // -( campos - refpos )
	
	pRefPosCameraMatrix.a11 = ( float )pCameraMatrix.a11;
	pRefPosCameraMatrix.a12 = ( float )pCameraMatrix.a12;
	pRefPosCameraMatrix.a13 = ( float )pCameraMatrix.a13;
	pRefPosCameraMatrix.a14 = refPosCam.x * pRefPosCameraMatrix.a11 + refPosCam.y * pRefPosCameraMatrix.a12 + refPosCam.z * pRefPosCameraMatrix.a13;
	pRefPosCameraMatrix.a21 = ( float )pCameraMatrix.a21;
	pRefPosCameraMatrix.a22 = ( float )pCameraMatrix.a22;
	pRefPosCameraMatrix.a23 = ( float )pCameraMatrix.a23;
	pRefPosCameraMatrix.a24 = refPosCam.x * pRefPosCameraMatrix.a21 + refPosCam.y * pRefPosCameraMatrix.a22 + refPosCam.z * pRefPosCameraMatrix.a23;
	pRefPosCameraMatrix.a31 = ( float )pCameraMatrix.a31;
	pRefPosCameraMatrix.a32 = ( float )pCameraMatrix.a32;
	pRefPosCameraMatrix.a33 = ( float )pCameraMatrix.a33;
	pRefPosCameraMatrix.a34 = refPosCam.x * pRefPosCameraMatrix.a31 + refPosCam.y * pRefPosCameraMatrix.a32 + refPosCam.z * pRefPosCameraMatrix.a33;
}



void deoglRenderPlan::SetFBOTarget( deoglFramebuffer *fbo ){
	pFBOTarget = fbo;
}

void deoglRenderPlan::SetFBOTargetCopyDepth( bool copyDepth ){
	pFBOTargetCopyDepth = copyDepth;
}



void deoglRenderPlan::SetUpscaleSize( int width, int height ){
	if( width < 0 || height < 0 ) DETHROW( deeInvalidParam );
	
	pUpscaleWidth = width;
	pUpscaleHeight = height;
}

void deoglRenderPlan::SetUseUpscaling( bool useUpscaling ){
	pUseUpscaling = useUpscaling;
}

void deoglRenderPlan::SetUpsideDown( bool upsideDown ){
	pUpsideDown = upsideDown;
}

void deoglRenderPlan::SetUseToneMap( bool useToneMap ){
	pUseToneMap = useToneMap;
}

void deoglRenderPlan::SetIgnoreStaticComponents( bool ignoreStaticComponents ){
	pIgnoreDynamicComponents = ignoreStaticComponents;
}

void deoglRenderPlan::SetRenderDebugPass( bool render ){
	pRenderDebugPass = render;
}

void deoglRenderPlan::SetNoReflections( bool noReflections ){
	pNoReflections = noReflections;
}



void deoglRenderPlan::SetUseLayerMask( bool useLayerMask ){
	pUseLayerMask = useLayerMask;
}

void deoglRenderPlan::SetLayerMask( const decLayerMask &layerMask ){
	pLayerMask = layerMask;
}



void deoglRenderPlan::SetCustomFrustumBoundaries( const decDVector &position, const decDVector &topLeft, const decDVector &topRight,
const decDVector &bottomLeft, const decDVector &bottomRight, double near, double far ){
	decDVector normal;
	
	// left plane
	normal = ( topLeft - bottomLeft ) % ( bottomLeft - position );
	normal.Normalize();
	pCustomFrustum.SetLeftPlane( normal, -( normal * position ) );
	
	// top plane
	normal = ( topRight - topLeft ) % ( topLeft - position );
	normal.Normalize();
	pCustomFrustum.SetTopPlane( normal, -( normal * position ) );
	
	// right plane
	normal = ( bottomRight - topRight ) % ( topRight - position );
	normal.Normalize();
	pCustomFrustum.SetRightPlane( normal, -( normal * position ) );
	
	// bottom plane
	normal = ( bottomLeft - bottomRight ) % ( bottomRight - position );
	normal.Normalize();
	pCustomFrustum.SetBottomPlane( normal, -( normal * position ) );
	
	// near and far plane
	//normal = ( topLeft + topRight + bottomLeft + bottomRight ) * 0.25 - position;
	normal = ( topLeft + bottomRight ) * 0.5 - position; // if this is a real box the center of each diagonal is the same point
	normal.Normalize();
	pCustomFrustum.SetNearPlane( normal, -( normal * position + near ) ); // n*(p+n*near) = n*p+(n*n)*near = n*p+near
	pCustomFrustum.SetFarPlane( -normal, normal * position + far ); // -n*(p+n*far) = -n*p+(-n*n)*far = -n*p-far = -(n*p+far)
}

void deoglRenderPlan::SetUseCustomFrustum( bool useCustomFrustum ){
	pUseCustomFrustum = useCustomFrustum;
}

void deoglRenderPlan::SetCustomFrustumScreenArea( const decBoundary &area ){
	pCustomFrustumScreenArea = area;
}



void deoglRenderPlan::SetNoRenderedOccMesh( bool noRenderedOccMesh ){
	pNoRenderedOccMesh = noRenderedOccMesh;
}

void deoglRenderPlan::SetFlipCulling( bool flipCulling ){
	pFlipCulling = flipCulling;
}



void deoglRenderPlan::SetEmptyPass( bool emptyPass ){
	pEmptyPass = emptyPass;
}

void deoglRenderPlan::SetClearStencilPassBits( bool clear ){
	pClearStencilPassBits = clear;
}

void deoglRenderPlan::SetClearColor( bool clear ){
	pClearColor = clear;
}



void deoglRenderPlan::SetCurrentTransparencyLayer( int layer ){
	pCurTransparencyLayer = layer;
}

void deoglRenderPlan::SetTransparencyLayerCount( int count ){
	pTransparencyLayerCount = count;
}

void deoglRenderPlan::SetRenderPassNumber( int number ){
	pRenderPassNumber = number;
}

void deoglRenderPlan::SetStencilRefValue( int refValue ){
	pStencilRefValue = refValue;
}

void deoglRenderPlan::SetStencilPrevRefValue( int refValue ){
	pStencilPrevRefValue = refValue;
}

void deoglRenderPlan::SetStencilWriteMask( int writeMask ){
	pStencilWriteMask = writeMask;
}

void deoglRenderPlan::SetOcclusionMap( deoglOcclusionMap *map ){
	pOcclusionMap = map;
}

void deoglRenderPlan::SetOcclusionMapBaseLevel( int level ){
	pOcclusionMapBaseLevel = level;
}

void deoglRenderPlan::SetOcclusionTestMatrix( const decMatrix &matrix ){
	pOcclusionTestMatrix = matrix;
}



void deoglRenderPlan::SetDebugTiming( bool debugTiming ){
	pDebugTiming = debugTiming;
}



// Environment Maps
/////////////////////

deoglRenderPlanEnvMap &deoglRenderPlan::GetEnvMapAt( int index ) const{
	if( index < 0 || index >= pEnvMapCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pEnvMaps[ index ];
}

void deoglRenderPlan::RemoveEnvMap( deoglEnvironmentMap *envmap ){
	if( ! envmap ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	pDirectEnvMapFader.Drop( envmap );
	
	for( i=0; i<pEnvMapCount; i++ ){
		if( pEnvMaps[ i ].GetEnvMap() == envmap ){
			pEnvMaps[ i ].SetEnvMap( NULL );
		}
	}
}



// Lights
///////////

deoglRenderPlanLight *deoglRenderPlan::GetLightAt( int index ) const{
	if( index < 0 || index >= pLightCount ) DETHROW( deeInvalidParam );
	
	return pLights[ index ];
}

deoglRenderPlanLight *deoglRenderPlan::GetLightFor( deoglRLight *light ){
	if( ! light ) DETHROW( deeInvalidParam );
	
	int index = pIndexOfLightWith( light );
	if( index == -1 ){
		if( pLightCount == pLightSize ){
			int newSize = pLightSize * 3 / 2 + 1;
			deoglRenderPlanLight **newArray = new deoglRenderPlanLight*[ newSize ];
			if( ! newArray ) DETHROW( deeOutOfMemory );
			
			memset( newArray, '\0', sizeof( deoglRenderPlanLight* ) * newSize );
			if( pLights ){
				memcpy( newArray, pLights, sizeof( deoglRenderPlanLight* ) * pLightSize );
				delete [] pLights;
			}
			
			pLights = newArray;
			pLightSize = newSize;
		}
		
		if( ! pLights[ pLightCount ] ){
			pLights[ pLightCount ] = new deoglRenderPlanLight( pRenderThread );
			if( ! pLights[ pLightCount ] ) DETHROW( deeOutOfMemory );
		}
		
		index = pLightCount;
		pLights[ pLightCount ]->SetLight( light );
		pLightCount++;
	}
	
	return pLights[ index ];
}

void deoglRenderPlan::RemoveAllLights(){
	while( pLightCount > 0 ){
		pLightCount--;
		pLights[ pLightCount ]->SetLight( NULL );
	}
}



// Sky lights
///////////////

deoglRenderPlanSkyLight *deoglRenderPlan::GetSkyLightAt( int index ) const{
	if( index < 0 || index >= pSkyLightCount ){
		DETHROW( deeInvalidParam );
	}
	return pSkyLights[ index ];
}

deoglRenderPlanSkyLight *deoglRenderPlan::AddSkyLight( deoglRSkyInstanceLayer *layer ){
	if( ! layer ) {
		DETHROW( deeInvalidParam );
	}
	
	if( pSkyLightCount == pSkyLightSize ){
		const int newSize = pSkyLightSize + 1;
		deoglRenderPlanSkyLight ** const newArray = new deoglRenderPlanSkyLight*[ newSize ];
		memset( newArray, '\0', sizeof( deoglRenderPlanSkyLight* ) * newSize );
		if( pSkyLights ){
			memcpy( newArray, pSkyLights, sizeof( deoglRenderPlanSkyLight* ) * pSkyLightSize );
			delete [] pSkyLights;
		}
		pSkyLights = newArray;
		pSkyLightSize = newSize;
	}
	
	if( ! pSkyLights[ pSkyLightCount ] ){
		pSkyLights[ pSkyLightCount ] = new deoglRenderPlanSkyLight( pRenderThread );
	}
	
	deoglRenderPlanSkyLight * const planSkyLight = pSkyLights[ pSkyLightCount++ ];
	planSkyLight->SetLayer( layer );
	return planSkyLight;
}

void deoglRenderPlan::RemoveAllSkyLights(){
	while( pSkyLightCount > 0 ){
		pSkyLightCount--;
		pSkyLights[ pSkyLightCount ]->Clear();
	}
}



// Sky instances
//////////////////

int deoglRenderPlan::GetSkyInstanceCount() const{
	return pSkyInstances.GetCount();
}

deoglRSkyInstance *deoglRenderPlan::GetSkyInstanceAt( int index ) const{
	return ( deoglRSkyInstance* )pSkyInstances.GetAt( index );
}

void deoglRenderPlan::RemoveAllSkyInstances(){
	pSkyInstances.RemoveAll();
}

void deoglRenderPlan::SetSkyBgColor( const decColor& color ){
	pSkyBgColor = color;
}



// Sub Plans
//////////////

deoglRenderPlanMasked *deoglRenderPlan::GetMaskedPlanAt( int index ) const{
	if( index < 0 || index >= pMaskedPlanCount ) DETHROW( deeInvalidParam );
	
	return pMaskedPlans[ index ];
}

deoglRenderPlanMasked *deoglRenderPlan::AddMaskedPlanFor( deoglRenderPlan *plan ){
	if( ! plan ) DETHROW( deeInvalidParam );
	
	if( pMaskedPlanCount == pMaskedPlanSize ){
		int newSize = pMaskedPlanSize * 3 / 2 + 1;
		deoglRenderPlanMasked **newArray = new deoglRenderPlanMasked*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		
		memset( newArray, '\0', sizeof( deoglRenderPlanMasked* ) * newSize );
		if( pMaskedPlans ){
			memcpy( newArray, pMaskedPlans, sizeof( deoglRenderPlanMasked* ) * pMaskedPlanSize );
			delete [] pMaskedPlans;
		}
		
		pMaskedPlans = newArray;
		pMaskedPlanSize = newSize;
	}
	
	if( ! pMaskedPlans[ pMaskedPlanCount ] ){
		pMaskedPlans[ pMaskedPlanCount ] = new deoglRenderPlanMasked;
	}
	
	pMaskedPlans[ pMaskedPlanCount ]->SetPlan( plan );
	pMaskedPlans[ pMaskedPlanCount ]->SetComponent( NULL, 0 );
	pMaskedPlanCount++;
	
	plan->SetLevel( pLevel + 1 ); // it is deeper one level than us
	
	return pMaskedPlans[ pMaskedPlanCount - 1 ];
}

void deoglRenderPlan::RemoveAllMaskedPlans(){
	while( pMaskedPlanCount > 0 ){
		pMaskedPlanCount--;
		pMaskedPlans[ pMaskedPlanCount ]->SetPlan( NULL );
		pMaskedPlans[ pMaskedPlanCount ]->SetComponent( NULL, 0 );
	}
}



// Private Functions
//////////////////////

int deoglRenderPlan::pIndexOfLightWith( deoglRLight *light ) const{
	int i;
	
	for( i=0; i<pLightCount; i++ ){
		if( light == pLights[ i ]->GetLight() ){
			return i;
		}
	}
	
	return -1;
}



void deoglRenderPlan::pCheckTransparency(){
	pHasTransparency = false;
	
	// components
	const int componentCount = pCollideList.GetComponentCount();
	int i;
	for( i=0; i<componentCount; i++ ){
		if( ! pCollideList.GetComponentAt( i )->GetComponent()->GetSolid() ){
			pHasTransparency = true;
			return;
		}
	}
	
	// billboards
	const int billboardCount = pCollideList.GetBillboardCount();
	for( i=0; i<billboardCount; i++ ){
		const deoglRBillboard &billboard = *pCollideList.GetBillboardAt( i );
		if( billboard.GetUseSkinTexture() && ! billboard.GetUseSkinTexture()->GetSolid() ){
			pHasTransparency = true;
			return;
		}
	}
	
	// particles
	if( pRenderThread.GetChoices().GetRealTransparentParticles() ){
		const deoglParticleEmitterInstanceList &peinstList = pCollideList.GetParticleEmitterList();
		const int peinstCount = peinstList.GetCount();
		int j;
		
		for( i=0; i< peinstCount; i++ ){
			const deoglRParticleEmitterInstance &instance = *peinstList.GetAt( i );
			if( instance.GetParticleCount() == 0 ){
				continue;
			}
			
			const int typeCount = instance.GetTypeCount();
			for( j=0; j<typeCount; j++ ){
				const deoglRSkin * const skin = instance.GetTypeAt( j ).GetUseSkin();
				if( skin && ! skin->GetIsSolid() ){
					pHasTransparency = true;
					return;
				}
				/*
				if( instance.GetEmitter()->GetTypeAt( j ).GetHasTransparency() ){
					pHasTransparency = true;
					return;
				}
				*/
			}
		}
		
	}else{
		// NOTE this check below only catches the case of a particle using a non-solid
		//      skin. it is though possible a solid skin is used with transparency curve
		//      in which case the skin becomes non-solid
		const deoglParticleEmitterInstanceList &peinstList = pCollideList.GetParticleEmitterList();
		const int peinstCount = peinstList.GetCount();
		int j;
		
		for( i=0; i< peinstCount; i++ ){
			const deoglRParticleEmitterInstance &instance = *peinstList.GetAt( i );
			if( instance.GetParticleCount() == 0 ){
				continue;
			}
			
			const int typeCount = instance.GetTypeCount();
			for( j=0; j<typeCount; j++ ){
				const deoglRSkin * const skin = instance.GetTypeAt( j ).GetUseSkin();
				if( skin && ! skin->GetIsSolid() ){
					pHasTransparency = true;
					return;
				}
			}
		}
	}
}

void deoglRenderPlan::pBuildRenderPlan(){
	// currently we simply assign a shared texture to all lights.
	// this is not very economic and will be replaced later on with
	// a better plan builder taking the on-screen size and location
	// of lights into consideration to down-tune shadow maps where
	// there is less chance of being noticed by the player to
	// conserve memory. also require is to add a memory checked to
	// avoid going beyond the memory capacity of the graphic card.
	// this test has to be only rough and should down-tune more
	// aggressively if the memory becomes scarce. for this to work
	// we need though first a framework to keep track of statically
	// spend GPU memory.
	
	// determine if we need transparency
	pCheckTransparency();
	
	// assing the stencil mask to all masked plans
	int m;
	for( m=0; m<pMaskedPlanCount; m++ ){
		pMaskedPlans[ m ]->SetStencilMask( m + 1 );
	}
	
	// build sky light plan
	pBuildSkyLightPlan();
	
	// first let's simply print out the number of lights and what
	// a conservative assignment would cause
	pBuildLightPlan();
	
	// determine light probes
	pBuildLightProbes();
}

void deoglRenderPlan::pBuildSkyLightPlan(){
	const int skyCount = GetSkyInstanceCount();
	int i, j;
	
	for( i=0; i<skyCount; i++ ){
		deoglRSkyInstance &instance = *GetSkyInstanceAt( i );
		const int layerCount = instance.GetLayerCount();
		for( j=0; j<layerCount; j++ ){
			deoglRSkyInstanceLayer &skyLayer = instance.GetLayerAt( j );
			if( skyLayer.GetHasLightDirect() || skyLayer.GetHasLightAmbient() ){
				AddSkyLight( &skyLayer )->Init( *this );
			}
		}
	}
}

void deoglRenderPlan::pBuildLightPlan(){
	deoglConfiguration &config = pRenderThread.GetConfiguration();
	const bool useEncodeDepth = config.GetUseEncodeDepth();
	const int shadowSize = config.GetShadowMapSize();
	deoglRenderCacheLight *cacheLight;
	int l;
	
	// plan light shadow map sizes.
	// 
	// the static shadow maps are used across multiple frames and uses the best resolution.
	// if memory becomes low static shadow maps can be removed from memory and reduced in size.
	// if reduced in size the shadow maps have to be dropped to force rebuilding them.
	// 
	// the dynamic shadow maps are used only during solid geometry rendering and forgotten
	// right afterwards. for this the shadow mapper is used using a temporary texture reused
	// for each light. these shadow maps can be of lower resolution since they exist only for
	// one render pass
	// 
	// transparent shadow maps are used for transparent render passes only. these shadow maps
	// work similar to static ones in that they are keep over multiple render passes (all
	// transparent render passes) then they are forgotten. they are produced from the dynamic
	// shadow maps by down sampling to save time
	const int lightCount = pCollideList.GetLightCount();
	int i;
	
	for( i=0; i<lightCount; i++ ){
		deoglRLight &light = *pCollideList.GetLightAt( i );
		int sizeTranspStatic, sizeTranspDynamic;
		int sizeSolidStatic, sizeSolidDynamic;
		
		switch( light.GetLightType() ){
		case deLight::eltSpot:
		case deLight::eltProjector:
			sizeSolidStatic = pShadowMapSize;
			break;
			
		case deLight::eltPoint:
			sizeSolidStatic = pShadowCubeSize;// >> 1; // temporary reduced by 1
			break;
			
		default:
			DETHROW( deeInvalidParam );
		}
		
		sizeSolidDynamic = decMath::max( sizeSolidStatic >> 1, 16 );
		
		sizeTranspStatic = decMath::max( sizeSolidStatic >> 1, 16 );
		sizeTranspDynamic = decMath::max( sizeSolidDynamic >> 1, 16 );
		
		// temporary hack. calculating the static point shadow map at higher resolution
		// is currently a problem and causes noticeable stutter. reducing the static
		// point shadow map size until this problem is fixed
		/*
		switch( light.GetLightType() ){
		case deLight::eltPoint:
		case deLight::eltSemiPoint:
		case deLight::eltAmbient:
			sizeSolidStatic = decMath::max( pShadowCubeSize >> 1, 16 );
			break;
			
		default:
			break;
		}
		*/
		
		// assign found values. ambient is same size as solid due to boundary box calculation
		deoglShadowCaster &scaster = *light.GetShadowCaster();
		
		deoglSCSolid &scsolid = scaster.GetSolid();
		scsolid.SetPlanStaticSize( sizeSolidStatic );
		scsolid.SetPlanDynamicSize( sizeSolidDynamic );
		
		deoglSCTransparent &sctransparent = scaster.GetTransparent();
		sctransparent.SetPlanStaticSize( sizeTranspStatic );
		sctransparent.SetPlanDynamicSize( sizeTranspDynamic );
		
		deoglSCAmbient &scambient = scaster.GetAmbient();
		scambient.SetPlanStaticSize( sizeSolidStatic );
		scambient.SetPlanDynamicSize( sizeSolidDynamic );
	}
	
	// below is deprecated
	
	// here we walk over all lights we found and we determine for
	// each light what kind of shadow textures are required and
	// how much memory this is going to cost us
	for( l=0; l<pLightCount; l++ ){
		cacheLight = pLights[ l ]->GetCacheLight();
		
		// if the light has not been prepared yet we do it now
		if( ! cacheLight->GetPrepared() ){
			deoglRLight *light = pLights[ l ]->GetLight();
			
			// determine the parameters
			cacheLight->SetUseCubeMaps( light->GetLightType() == deLight::eltPoint );
			cacheLight->SetUseEncodedDepth( useEncodeDepth );
			
			// determine if we need transparency
			cacheLight->SetUseTransparency( false );
			//if( ! cacheLight->GetUseTransparency() ){
				const deoglCollideList &clistStatic = *light->GetStaticCollideList();
				const deoglCollideList &clistDynamic = *light->GetDynamicCollideList();
				int i, count;
				
				count = clistStatic.GetComponentCount();
				for( i=0; i<count; i++ ){
					const deoglRComponent &component = *clistStatic.GetComponentAt( i )->GetComponent();
					
					if( ! component.GetSolid() && component.GetSkin() && component.GetSkin()->GetCastTransparentShadow() ){
						cacheLight->SetUseTransparency( true );
						break;
					}
				}
				
				count = clistDynamic.GetComponentCount();
				for( i=0; i<count; i++ ){
					const deoglRComponent &component = *clistDynamic.GetComponentAt( i )->GetComponent();
					
					if( ! component.GetSolid() && component.GetSkin() && component.GetSkin()->GetCastTransparentShadow() ){
						cacheLight->SetUseTransparency( true );
						break;
					}
				}
			//}
			
			// now determine the size of the shadow map to use. currently this is
			// just the shadow map size given by the configuration. the idea is to
			// make this depedent later on the actual screen size the light covers
			// to down-tune less important lights to gain memory and speed.
			// also calculate the memory consumption at the same time
			deoglRenderCacheLightShadow &shadowSolid = cacheLight->GetShadowSolid();
			shadowSolid.SetSize( shadowSize );
			
			pCalcShadowMemoryConsumption( *cacheLight, shadowSolid, false );
			
			if( cacheLight->GetUseTransparency() ){
				deoglRenderCacheLightShadow &shadowTransp = cacheLight->GetShadowTransparent();
				shadowTransp.SetSize( shadowSize ); // shadowSize >> 1
				
				pCalcShadowMemoryConsumption( *cacheLight, shadowTransp, true );
			}
			
			// now the light is prepared
			cacheLight->SetPrepared( true );
		}
	}
}

void deoglRenderPlan::pCalcShadowMemoryConsumption( deoglRenderCacheLight &light, deoglRenderCacheLightShadow &shadow, bool withColor ){
	int size, consumption, totalConsumption = 0;
	
	size = shadow.GetSize();
	
	// add depth texture memory consumption
	consumption = size * size;
	
	if( light.GetUseEncodedDepth() ){
		consumption *= 3; // RGB
		
	}else{
		consumption *= 3; // 24-bit depth ( maybe different )
	}
	
	if( light.GetUseCubeMaps() ){
		consumption *= 6; // 6 sides
	}
	
	totalConsumption += consumption;
	
	// add color texture memory consumption
	if( withColor ){
		consumption = size * size * 4; // RGBA
		
		if( light.GetUseCubeMaps() ){
			consumption *= 6; // 6 sides
		}
		
		totalConsumption += consumption;
	}
	
	// set the memory consumption and also add it to the light memory consumption
	shadow.SetMemoryConsumption( totalConsumption );
	light.SetMemoryConsumption( light.GetMemoryConsumption() + totalConsumption );
}

void deoglRenderPlan::pBuildLightProbes(){
#if 0
	deoglLightProbeTexture &probes = pRenderThread.GetRenderers().GetLight()->GetLightProbesTexture();
	
	// clear the light probles
	probes.RemoveAllProbes();
	
	// add a probe for each visible particle
	const deoglParticleEmitterList &particleEmitterList = pCollideList.GetParticleEmitterList();
	const int particleEmitterCount = particleEmitterList.GetCount();
	decMatrix matrixMV;
	deSkin *engSkin;
	deoglSkin *skin;
	int i, p;
	
	for( i=0; i<particleEmitterCount; i++ ){
		const deoglParticleEmitter &emitter = *particleEmitterList.GetAt( i );
		const int particleCount = emitter.GetParticleCount();
		
		if( particleCount > 0 ){
			const deParticleEmitter &engEmitter = *emitter.GetParticleEmitter();
			engSkin = engEmitter.GetSkin();
			
			if( engSkin ){
				skin = ( deoglSkin* )engSkin->GetPeerGraphic();
				
				if( ! skin->GetIsSolid() ){
					const deoglParticleEmitter::sParticle *particles = emitter.GetParticles();
					
					matrixMV = ( decDMatrix::CreateTranslation( engEmitter.GetPosition() ) * pCameraMatrix ).ToMatrix();
					
					// NOTE: it would be enough to store the index of the first probe in the particle emitter. this
					// way during rendering the actual probe texture coordinates can be retrieved.
					for( p=0; p<particleCount; p++ ){
						// particleProbeIndex = probes.GetProbeCount();
						probes.AddProbe( matrixMV * particles[ p ].position );
					}
				}
			}
		}
	}
	
	// update the probes
	probes.Update();
	
	// debug
	//pRenderThread.GetLogger().LogInfoFormat( "RenderPlan: light probe count = %i\n", probes.GetProbeCount() );
#endif
}



void deoglRenderPlan::pUpdateHTView(){
	if( pHTView && pWorld && &pHTView->GetHeightTerrain() == pWorld->GetHeightTerrain() ){
		pHTView->PrepareForRendering();
		return;
	}
	
	if( pHTView ){
		delete pHTView;
		pHTView = NULL;
	}
	
	if( pWorld && pWorld->GetHeightTerrain() ){
		pHTView = new deoglHTView( pWorld->GetHeightTerrain() );
	}
}

void deoglRenderPlan::pCheckOutsideVisibility(){
	bool outsideWorldVisible = true;
	
	pSkyVisible = outsideWorldVisible;
	if( ! outsideWorldVisible ){
		pCollideList.RemoveAllHTSectors();
	}
}

void deoglRenderPlan::pDropLightsDynamic(){
	const int count = pCollideList.GetLightCount();
	int i;
	for( i=0; i<count; i++ ){
		pCollideList.GetLightAt( i )->GetShadowCaster()->DropDynamic();
	}
}
