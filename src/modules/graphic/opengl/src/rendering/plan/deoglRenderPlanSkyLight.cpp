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

#include "deoglRenderPlanSkyLight.h"
#include "deoglRenderPlan.h"
#include "parallel/deoglRPTSkyLightFindContent.h"
#include "parallel/deoglRPTSkyLightBuildRT.h"
#include "parallel/deoglRPTSkyLightGIFindContent.h"
#include "parallel/deoglRPTSkyLightGIUpdateRT.h"
#include "../light/deoglRenderGI.h"
#include "../light/deoglRenderLight.h"
#include "../../deGraphicOpenGl.h"
#include "../../billboard/deoglRBillboard.h"
#include "../../collidelist/deoglCollideListComponent.h"
#include "../../collidelist/deoglCollideListHTSCluster.h"
#include "../../collidelist/deoglCollideListPropFieldCluster.h"
#include "../../component/deoglRComponent.h"
#include "../../debug/debugSnapshot.h"
#include "../../debug/deoglDebugInformation.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../gi/deoglGIState.h"
#include "../../gi/deoglGICascade.h"
#include "../../model/deoglRModel.h"
#include "../../occlusiontest/deoglOcclusionTest.h"
#include "../../occlusiontest/deoglOcclusionTestPool.h"
#include "../../rendering/deoglRenderCanvas.h"
#include "../../rendering/deoglRenderCompute.h"
#include "../../rendering/deoglRenderOcclusion.h"
#include "../../rendering/task/persistent/deoglPersistentRenderTaskOwner.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../shadow/deoglShadowCaster.h"
#include "../../shaders/paramblock/deoglSPBMapBuffer.h"
#include "../../sky/deoglRSkyInstance.h"
#include "../../sky/deoglRSkyInstanceLayer.h"
#include "../../sky/deoglSkyLayerGICascade.h"
#include "../../terrain/heightmap/deoglHTSCluster.h"
#include "../../terrain/heightmap/deoglHTViewSector.h"
#include "../../terrain/heightmap/deoglRHTSector.h"
#include "../../utils/convexhull/deoglConvexHull2D.h"
#include "../../utils/collision/deoglCollisionBox.h"
#include "../../utils/collision/deoglDCollisionBox.h"
#include "../../world/deoglRWorld.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/parallel/deParallelProcessing.h>



// Class deoglRenderPlanSkyLight
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderPlanSkyLight::deoglRenderPlanSkyLight( deoglRenderPlan &plan ) :
pPlan( plan ),
pSky( NULL ),
pLayer( NULL ),
pOcclusionTest( NULL ),
pPlanned( false ),
pUseLight( true ),
pUseShadow( false ),
pShadowLayerCount( 0 ),
pGIShadowSize( 1024 ),
pGIShadowUpdateStatic( true ),
pGIRenderTaskStatic( plan.GetRenderThread() ),
pGIRenderTaskDynamic( plan.GetRenderThread() ),
pGIRenderTaskAddStatic( plan.GetRenderThread(), pGIRenderTaskStatic ),
pGIRenderTaskAddDynamic( plan.GetRenderThread(), pGIRenderTaskDynamic ),
pTaskFindContent( NULL ),
pTaskBuildRT1( NULL ),
pTaskBuildRT2( NULL ),
pTaskGIFindContent( NULL ),
pTaskGIUpdateRT( NULL )
{
	memset( &pShadowLayers, 0, sizeof( sShadowLayer ) * 4 );
	
	int i;
	for( i=0; i<4; i++ ){
		pShadowLayers[ i ].renderTask = new deoglRenderTask( plan.GetRenderThread() );
		pShadowLayers[ i ].addToRenderTask = new deoglAddToRenderTask( 
			plan.GetRenderThread(), *pShadowLayers[ i ].renderTask );
	}
	
	const deoglRenderPlanCompute &compute = plan.GetCompute();
	pUBOFindConfig.TakeOver( new deoglSPBlockUBO( compute.GetUBOFindConfig() ) );
	
	pSSBOCounters.TakeOver( new deoglSPBlockSSBO( compute.GetSSBOCounters() ) );
	pSSBOCounters->SetElementCount( 2 );
	
	pSSBOVisibleElements.TakeOver( new deoglSPBlockSSBO( compute.GetSSBOVisibleElements() ) );
	pSSBOVisibleElementsFlags.TakeOver( new deoglSPBlockSSBO( compute.GetSSBOVisibleElementsFlags() ) );
	
	pSSBOVisibleElementsGI.TakeOver( new deoglSPBlockSSBO( compute.GetSSBOVisibleElements() ) );
	pSSBOVisibleElementsFlagsGI.TakeOver( new deoglSPBlockSSBO( compute.GetSSBOVisibleElementsFlags() ) );
}

deoglRenderPlanSkyLight::~deoglRenderPlanSkyLight(){
	Clear();
	
	int i;
	for( i=0; i<4; i++ ){
		if( pShadowLayers[ i ].addToRenderTask ){
			delete pShadowLayers[ i ].addToRenderTask;
		}
		if( pShadowLayers[ i ].renderTask ){
			delete pShadowLayers[ i ].renderTask;
		}
	}
}



// Management
///////////////

void deoglRenderPlanSkyLight::SetOcclusionTest( deoglOcclusionTest *occlusionTest ){
	if( occlusionTest == pOcclusionTest ){
		return;
	}
	
	if( pOcclusionTest ){
		pPlan.GetRenderThread().GetOcclusionTestPool().Return( pOcclusionTest );
	}
	
	pOcclusionTest = occlusionTest;
}

void deoglRenderPlanSkyLight::SetFrustumBoxExtend( const decVector &minExtend, const decVector &maxExtend ){
	pFrustumBoxMinExtend = minExtend;
	pFrustumBoxMaxExtend = maxExtend;
}

void deoglRenderPlanSkyLight::ClearPlanned(){
	pPlanned = false;
}

deoglRenderPlanSkyLight::sShadowLayer &deoglRenderPlanSkyLight::GetShadowLayerAt( int index ){
	if( index < 0 || index >= pShadowLayerCount ){
		DETHROW( deeInvalidParam );
	}
	return pShadowLayers[ index ];
}

const deoglRenderPlanSkyLight::sShadowLayer &deoglRenderPlanSkyLight::GetShadowLayerAt( int index ) const{
	if( index < 0 || index >= pShadowLayerCount ){
		DETHROW( deeInvalidParam );
	}
	return pShadowLayers[ index ];
}



void deoglRenderPlanSkyLight::Clear(){
	pGIShadowUpdateStatic = false;
	pGIRenderTaskStatic.Clear();
	pGIRenderTaskDynamic.Clear();
	pGICollideList.Clear();
	
	int i;
	for( i=0; i<4; i++ ){
		pShadowLayers[ i ].renderTask->Clear();
	}
	
	SetOcclusionTest( NULL );
	pCollideList.Clear();
	pUseLight = false;
	pUseShadow = false;
	
	pSky = NULL;
	pLayer = NULL;
}

void deoglRenderPlanSkyLight::SetLayer( deoglRSkyInstance *sky, deoglRSkyInstanceLayer *layer ){
	pSky = sky;
	pLayer = layer;
}

#define DO_SPECIAL_TIMING 1
#ifdef DO_SPECIAL_TIMING
#include <dragengine/common/utils/decTimer.h>
#define INIT_SPECIAL_TIMING decTimer sttimer;
#define SPECIAL_TIMER_PRINT(w) pPlan.GetRenderThread().GetLogger().LogInfoFormat("RenderPlanSkyLight.Prepare: " w "=%dys", (int)(sttimer.GetElapsedTime()*1e6f));
#else
#define INIT_SPECIAL_TIMING
#define SPECIAL_TIMER_PRINT(w)
#endif

void deoglRenderPlanSkyLight::Plan(){
	pPlanned = true;
	
	pDetermineShadowParameters();
	
	if( ! pUseShadow ){
		return;
	}
	
	pCalcShadowLayerParams();
	pGICalcShadowLayerParams();
}

void deoglRenderPlanSkyLight::StartFindContent(){
	if( ! pLayer || ! pUseShadow ){
		return;
	}
	
	if( pTaskFindContent || pTaskBuildRT1 || pTaskBuildRT2 || pTaskGIFindContent || pTaskGIUpdateRT ){
		DETHROW( deeInvalidParam );
	}
	
	PrepareBuffers();
	pPlan.GetRenderThread().GetRenderers().GetCompute().FindContentSkyLight( *this );
	
	deParallelProcessing &pp = pPlan.GetRenderThread().GetOgl().GetGameEngine()->GetParallelProcessing();
	SetOcclusionTest( pPlan.GetRenderThread().GetOcclusionTestPool().Get() );
	
	pOcclusionTest->RemoveAllInputData();
	
	// pTaskFindContent = new deoglRPTSkyLightFindContent( *this );
	// pp.AddTaskAsync( pTaskFindContent );
	
	if( pPlan.GetUpdateGIState() ){
		pTaskGIFindContent = new deoglRPTSkyLightGIFindContent( *this );
		pp.AddTaskAsync( pTaskGIFindContent );
	}
}

void deoglRenderPlanSkyLight::RenderOcclusionTests(){
	if( ! pLayer || ! pUseShadow ){
		return;
	}
	
	pWaitFinishedFindContent();
	pWaitFinishedGIFindContent();
	
	// start the GI update render task parallel task. this will be waited on before rendering
	if( pPlan.GetUpdateGIState() ){
		pTaskGIUpdateRT = new deoglRPTSkyLightGIUpdateRT( *this );
		pPlan.GetRenderThread().GetOgl().GetGameEngine()->GetParallelProcessing().AddTaskAsync( pTaskGIUpdateRT );
	}
	
	// if occlusion test input data are present render the tests. reading back the result
	// is delayed until used in the sky light renderer. this avoids stalling
	if( pOcclusionTest->GetInputDataCount() > 0
	&& pPlan.GetRenderThread().GetConfiguration().GetOcclusionTestMode() != deoglConfiguration::eoctmNone ){
		pOcclusionTest->UpdateSSBO();
		pPlan.GetRenderThread().GetRenderers().GetOcclusion().RenderTestsSkyLayer( pPlan, *this );
	}
}

void deoglRenderPlanSkyLight::FinishPrepare(){
	RenderOcclusionTests();
}

void deoglRenderPlanSkyLight::StartBuildRT(){
	if( ! pLayer || ! pUseShadow ){
		return;
	}
	
	if( pTaskBuildRT1 || pTaskBuildRT2 ){
		DETHROW( deeInvalidParam );
	}
	
	// potentially stalls if not finished yet
	if( pPlan.GetRenderThread().GetConfiguration().GetOcclusionTestMode() != deoglConfiguration::eoctmNone ){
		pOcclusionTest->UpdateResults();
		pCollideList.RemoveCulledElements();
	}
	
	// start parallel task to build split render plans. the first 3 cascades usually have
	// small amount of content so they can be process sequentially. the 4th cascade on the
	// other hand usually has large amount of content. for this reason the 4th cascade
	// render task is build in a separate task
	pTaskBuildRT1 = new deoglRPTSkyLightBuildRT( *this, pSLCollideList1, 0, 2 );
	pPlan.GetRenderThread().GetOgl().GetGameEngine()->GetParallelProcessing().AddTaskAsync( pTaskBuildRT1 );
	
	pTaskBuildRT2 = new deoglRPTSkyLightBuildRT( *this, pSLCollideList2, 3, 3 );
	pPlan.GetRenderThread().GetOgl().GetGameEngine()->GetParallelProcessing().AddTaskAsync( pTaskBuildRT2 );
}

void deoglRenderPlanSkyLight::WaitFinishedGIUpdateRT(){
	if( ! pTaskGIUpdateRT ){
		return;
	}
	
	pTaskGIUpdateRT->GetSemaphore().Wait();
	
	deoglRenderCanvas &rc = pPlan.GetRenderThread().GetRenderers().GetCanvas();
	rc.SampleDebugInfoPlanPrepareSkyLightGIUpdateRenderTask( pPlan, pTaskGIUpdateRT->GetElapsedTime() );
	
// 	const float timePrepare = pTaskGIUpdateRT->GetElapsedTime();
	
	pTaskGIUpdateRT->FreeReference();
	pTaskGIUpdateRT = NULL;
	
	// this call does modify a shader parameter block and can thus not be parallel
		decTimer timer;
	if( pGIShadowUpdateStatic ){
		pGIRenderTaskStatic.PrepareForRender();
	}
	pGIRenderTaskDynamic.PrepareForRender();
	
// 	pPlan.GetRenderThread().GetLogger().LogInfoFormat(
// 		"GIUpdateRenderTask: build=%dys prepare=%dys: shaders=%d textures=%d vaos=%d instances=%d subinstances=%d",
// 		( int )( timePrepare * 1e6f ), ( int )( timer.GetElapsedTime() * 1e6f ),
// 		pGIRenderTask.GetShaderCount(), pGIRenderTask.GetTotalTextureCount(), pGIRenderTask.GetTotalVAOCount(),
// 		pGIRenderTask.GetTotalInstanceCount(), pGIRenderTask.GetTotalSubInstanceCount() );
}

void deoglRenderPlanSkyLight::WaitFinishedBuildRT1(){
	if( ! pTaskBuildRT1 ){
		return;
	}
	
	pTaskBuildRT1->GetSemaphore().Wait();
	
	deoglRenderCanvas &rc = pPlan.GetRenderThread().GetRenderers().GetCanvas();
	rc.SampleDebugInfoPlanPrepareSkyLightBuildRT( pPlan, pTaskBuildRT1->GetElapsedTime() );
	
	pTaskBuildRT1->FreeReference();
	pTaskBuildRT1 = NULL;
	
	// this call does modify a shader parameter block and can thus not be parallel
	pShadowLayers[ 0 ].renderTask->PrepareForRender();
	pShadowLayers[ 1 ].renderTask->PrepareForRender();
	pShadowLayers[ 2 ].renderTask->PrepareForRender();
}

void deoglRenderPlanSkyLight::WaitFinishedBuildRT2(){
	if( ! pTaskBuildRT2 ){
		return;
	}
	
	pTaskBuildRT2->GetSemaphore().Wait();
	
	deoglRenderCanvas &rc = pPlan.GetRenderThread().GetRenderers().GetCanvas();
	rc.SampleDebugInfoPlanPrepareSkyLightBuildRT( pPlan, pTaskBuildRT2->GetElapsedTime() );
	
	pTaskBuildRT2->FreeReference();
	pTaskBuildRT2 = NULL;
	
	// this call does modify a shader parameter block and can thus not be parallel
	pShadowLayers[ 3 ].renderTask->PrepareForRender();
}

void deoglRenderPlanSkyLight::PrepareBuffers(){
	pPrepareFindConfig();
	
	const int visElCount = ( ( pPlan.GetWorld()->GetCompute().GetElementCount() - 1 ) / 4 ) + 1;
	pPrepareBuffer( pSSBOVisibleElements, visElCount );
	pPrepareBuffer( pSSBOVisibleElementsFlags, visElCount );
	pPrepareBuffer( pSSBOVisibleElementsGI, visElCount );
	pPrepareBuffer( pSSBOVisibleElementsFlagsGI, visElCount );
	
	pClearCounters();
}

void deoglRenderPlanSkyLight::ReadVisibleElements(){
	const deoglWorldCompute &wcompute = pPlan.GetWorld()->GetCompute();
	if( wcompute.GetElementCount() == 0 ){
		return;
	}
	
		// decTimer timer;
	memcpy( pCounters, pSSBOCounters->ReadBuffer( 2 ), sizeof( pCounters ) );
		// pPlan.GetRenderThread().GetLogger().LogInfoFormat("RenderPlanSkyLight.ReadVisibleElements: counter %dys", (int)(timer.GetElapsedTime()*1e6f));
	const int indexCount = pCounters[ 0 ].counter;
	if( indexCount == 0 ){
		return;
	}
	
	// read written visible element indices
	const decDVector &referencePosition = pPlan.GetWorld()->GetReferencePosition();
	deoglHTView * const htview = pPlan.GetHeightTerrainView();
	deoglOcclusionTest &occlusionTest = *pOcclusionTest;
	
	const int ecount = ( ( indexCount - 1 ) / 4 ) + 1;
	const uint32_t * const indices = ( const uint32_t * )pSSBOVisibleElements->ReadBuffer( ecount );
	const uint32_t * const flags = ( const uint32_t * )pSSBOVisibleElementsFlags->ReadBuffer( ecount );
	int i;
		// pPlan.GetRenderThread().GetLogger().LogInfoFormat("RenderPlanSkyLight.ReadVisibleElements: read %dys", (int)(timer.GetElapsedTime()*1e6f));
	
	for( i=0; i<indexCount; i++ ){
		const deoglWorldCompute::Element &element = wcompute.GetElementAt( indices[ i ] );
		const int cascadeMask = flags[ i ] & 0xf;
		
		switch( element.GetType() ){
		case deoglWorldCompute::eetComponent:{
			deoglCollideListComponent &clcomponent = *pCollideList.AddComponent( ( deoglRComponent* )element.GetOwner() );
			clcomponent.SetCascadeMask( cascadeMask );
			clcomponent.SetSpecialFlags( cascadeMask );
			clcomponent.StartOcclusionTest( occlusionTest, referencePosition );
			}break;
			
		case deoglWorldCompute::eetBillboard:{
			deoglRBillboard * const billboard = ( deoglRBillboard* )element.GetOwner();
			billboard->SetSkyShadowSplitMask( cascadeMask ); // TODO move this into deoglCollideListBillboard
			pCollideList.AddBillboard( billboard );
			}break;
			
		case deoglWorldCompute::eetPropFieldCluster:
			pCollideList.AddPropFieldCluster( ( deoglPropFieldCluster* )element.GetOwner() )
				->SetCascadeMask( cascadeMask );
			break;
			
		case deoglWorldCompute::eetHeightTerrainSectorCluster:
			if( htview ){
				const deoglHTSCluster &cluster = *( deoglHTSCluster* )element.GetOwner();
				pCollideList.AddHTSCluster( &htview->GetSectorAt( cluster.GetHTSector()->GetIndex() )
					->GetClusterAt( cluster.GetIndex() ) )->SetCascadeMask( cascadeMask );
			}
			break;
			
		default:
			break;
		}
	}
		// pPlan.GetRenderThread().GetLogger().LogInfoFormat("RenderPlanSkyLight.ReadVisibleElements: list %dys", (int)(timer.GetElapsedTime()*1e6f));
}

void deoglRenderPlanSkyLight::ReadVisibleElementsGI(){
}

void deoglRenderPlanSkyLight::CleanUp(){
	// pWaitFinishedFindContent();
	pWaitFinishedGIFindContent();
	WaitFinishedGIUpdateRT();
	WaitFinishedBuildRT1();
	WaitFinishedBuildRT2();
	
	int i;
	for( i=0; i<4; i++ ){
		pShadowLayers[ i ].renderTask->Clear();
	}
	pSLCollideList1.Clear();
	pSLCollideList2.Clear();
	pGICollideList.Clear();
	pCollideList.Clear();
	pGIRenderTaskStatic.Clear();
	pGIRenderTaskDynamic.Clear();
	pUseLight = false;
	pUseShadow = false;
	SetOcclusionTest( NULL );
}



// Private Functions
//////////////////////

void deoglRenderPlanSkyLight::pPrepareFindConfig(){
	if( ! pLayer ){
		return;
	}
	
	deoglSPBlockUBO &ubo = pUBOFindConfig;
	const deoglSPBMapBuffer mapped( ubo );
	ubo.Clear();
	
	const decDVector &refpos = pPlan.GetWorld()->GetReferencePosition();
	const deoglWorldCompute &wcompute = pPlan.GetWorld()->GetCompute();
	
	ubo.SetParameterDataUInt( deoglRenderPlanCompute::efcpNodeCount, 0 );
	ubo.SetParameterDataUInt( deoglRenderPlanCompute::efcpElementCount, wcompute.GetElementCount() );
	ubo.SetParameterDataUInt( deoglRenderPlanCompute::efcpUpdateElementCount, 0 );
	
	// camera parameters
	const decDVector camPos( pPlan.GetCameraPosition() - refpos );
	const decDMatrix &matCamInv = pPlan.GetInverseCameraMatrix();
	
	ubo.SetParameterDataVec3( deoglRenderPlanCompute::efcpCameraPosition, camPos );
	ubo.SetParameterDataVec3( deoglRenderPlanCompute::efcpCameraView, matCamInv.TransformView() );
	
	// light space
	const float backtrack = 2000.0f;
	const decDMatrix matLig( decDMatrix::CreateRotation( 0.0, PI, 0.0 ) * decDMatrix( pLayer->GetMatrix() ) );
	const decDMatrix matLigInv( matLig.Invert() );
	const decDMatrix matCL( matCamInv.GetRotationMatrix() * matLigInv );
	const decQuaternion quatLig( matLig.ToQuaternion() );
	const decDMatrix matLS( decDMatrix::CreateTranslation( -camPos ) * matLigInv );
	
	ubo.SetParameterDataMat4x3( deoglRenderPlanCompute::efcpMatrixLightSpace, matLS );
	
	// frustum culling
	decDVector frustumPoints[ 5 ];
	pCalcLightFrustum( matCL, frustumPoints );
	
	decDVector frustumBoxMinExtend, frustumBoxMaxExtend;
	pLightFrustumBox( frustumPoints, frustumBoxMinExtend, frustumBoxMaxExtend );
	SetFrustumBoxExtend( frustumBoxMinExtend, frustumBoxMaxExtend );
	
	frustumBoxMinExtend.z -= backtrack; // to catch shadow casters outside the view
	
	deoglDCollisionBox volumeShadowBox;
	volumeShadowBox.SetFromExtends( frustumBoxMinExtend, frustumBoxMaxExtend );
	volumeShadowBox.SetCenter( camPos + matLig * volumeShadowBox.GetCenter() );
	volumeShadowBox.SetOrientation( quatLig );
	
	ubo.SetParameterDataFloat( deoglRenderPlanCompute::efcpLightShaftFar, frustumBoxMaxExtend.z );
	ubo.SetParameterDataArrayVec3( deoglRenderPlanCompute::efcpShadowAxis, 0, volumeShadowBox.GetAxisX().Absolute() );
	ubo.SetParameterDataArrayVec3( deoglRenderPlanCompute::efcpShadowAxis, 1, volumeShadowBox.GetAxisY().Absolute() );
	ubo.SetParameterDataArrayVec3( deoglRenderPlanCompute::efcpShadowAxis, 2, volumeShadowBox.GetAxisZ().Absolute() );
	
	deoglDCollisionFrustum frustum;
	frustum.SetFrustum( pPlan.GetCameraMatrix() * pPlan.GetFrustumMatrix() );
	
	int planeCount = 0;
	pSetLightFrustumPlane( ubo, matLS, planeCount, frustum.GetLeftNormal(), frustum.GetLeftDistance() );
	pSetLightFrustumPlane( ubo, matLS, planeCount, frustum.GetRightNormal(), frustum.GetRightDistance() );
	pSetLightFrustumPlane( ubo, matLS, planeCount, frustum.GetTopNormal(), frustum.GetTopDistance() );
	pSetLightFrustumPlane( ubo, matLS, planeCount, frustum.GetBottomNormal(), frustum.GetBottomDistance() );
	pSetLightFrustumPlane( ubo, matLS, planeCount, frustum.GetNearNormal(), frustum.GetNearDistance() );
	pSetLightFrustumPlane( ubo, matLS, planeCount, frustum.GetFarNormal(), frustum.GetFarDistance() );
	
	pFrustumHull( ubo, frustumPoints );
	
	// gi cascase testing. to disable set min and max extends to the same value best far away
	const deoglGIState * const gistate = pPlan.GetUpdateGIState();
	if( gistate ){
		// TODO
	}
	
	pCullLayerMask( ubo );
	
	// cull by flags. component, billboards, prop fields and height terrains yes, the rest no
	uint32_t cullFlags = deoglWorldCompute::eefParticleEmitter | deoglWorldCompute::eefLight;
	ubo.SetParameterDataUInt( deoglRenderPlanCompute::efcpCullFlags, cullFlags );
	
	pSetSplits( ubo, backtrack );
}

void deoglRenderPlanSkyLight::pPrepareBuffer( deoglSPBlockSSBO &ssbo, int count ){
	if( count > ssbo.GetElementCount() ){
		ssbo.SetElementCount( count );
	}
	ssbo.EnsureBuffer();
}

void deoglRenderPlanSkyLight::pClearCounters(){
	deoglSPBlockSSBO &ssbo = pSSBOCounters;
	const deoglSPBMapBuffer mapped( ssbo );
	int i;
	
	for( i=0; i<2; i++ ){
		pCounters[ i ].workGroupSize[ 0 ] = 0;
		pCounters[ i ].workGroupSize[ 1 ] = 1;
		pCounters[ i ].workGroupSize[ 2 ] = 1;
		pCounters[ i ].counter = 0;
		
		ssbo.SetParameterDataUVec3( 0, i, 0, 1, 1 ); // work group size (x=0)
		ssbo.SetParameterDataUInt( 1, i, 0 ); // count
	}
}

void deoglRenderPlanSkyLight::pSetFrustumPlane( deoglSPBlockUBO &ubo, int i, const decDVector& n, double d ){
	ubo.SetParameterDataArrayVec4( deoglRenderPlanCompute::efcpFrustumPlanes, i, n, d );
	ubo.SetParameterDataArrayVec3( deoglRenderPlanCompute::efcpFrustumPlanesAbs, i, n.Absolute() );
	ubo.SetParameterDataArrayBVec3( deoglRenderPlanCompute::efcpFrustumSelect, i, n.x > 0.0, n.y > 0.0, n.z > 0.0 );
}

void deoglRenderPlanSkyLight::pSetLightFrustumPlane( deoglSPBlockUBO &ubo,
const decDMatrix &matrix, int &index, const decDVector &normal, double distance ){
	const decDVector pn( matrix.TransformNormal( normal ) );
	if( pn.z < 0.0 ){
		pSetFrustumPlane( ubo, index++, pn, pn * ( matrix * ( normal * distance ) ) );
	}
}

void deoglRenderPlanSkyLight::pCalcLightFrustum( const decDMatrix &matrix, decDVector (&frustumPoints)[ 5 ] ){
	const double fov = ( double )pPlan.GetCameraFov();
	const double fovRatio = ( double )pPlan.GetCameraFovRatio();
	const double zfar = ( double )pPlan.GetCameraViewDistance();
	const double xfar = tan( fov * 0.5 ) * zfar; // * znear, dropped since we calc x'=z'*(xnear/znear)
	const double yfar = tan( fov * 0.5 * fovRatio ) * zfar; // * znear, dropped since we calc y'=z'*(ynear/znear)
	
	frustumPoints[ 0 ] = matrix.GetPosition();
	matrix.Transform( frustumPoints[ 1 ], -xfar, yfar, zfar );
	matrix.Transform( frustumPoints[ 2 ], xfar, yfar, zfar );
	matrix.Transform( frustumPoints[ 3 ], xfar, -yfar, zfar );
	matrix.Transform( frustumPoints[ 4 ], -xfar, -yfar, zfar );
}

void deoglRenderPlanSkyLight::pLightFrustumBox( const decDVector (&frustumPoints)[ 5 ],
decDVector &minExtend, decDVector &maxExtend ){
	maxExtend = minExtend = frustumPoints[ 0 ];
	int i;
	
	for( i=1; i<5; i++ ){
		minExtend.SetSmallest( frustumPoints[ i ] );
		maxExtend.SetLargest( frustumPoints[ i ] );
	}
}

void deoglRenderPlanSkyLight::pFrustumHull( deoglSPBlockUBO &ubo, const decDVector (&frustumPoints)[ 5 ] ){
	// calculate the convex hull of the frustum projected to the shadow map plane. each edge
	// in the hull becomes a plane to test components against. the planes are oriented
	// perpendicular to the light direction. this way a simple 2d convex hull test can be used.
	// there will be between 3 to 5 planes to test
	deoglConvexHull2D hull;
	int i;
	
	for( i=0; i<5; i++ ){
		hull.AddPoint( ( float )frustumPoints[ i ].x, ( float )frustumPoints[ i ].y );
	}
	hull.CalculateHull();
	
	const int count = hull.GetHullPointCount();
	
	for( i=0; i<count; i++ ){
		const decVector2 &edgeStart = hull.GetHullPointVectorAt( i );
		const decVector2 edge( ( hull.GetHullPointVectorAt( ( i + 1 ) % count ) - edgeStart ).Normalized() );
		const decVector2 edgeNormal( -edge.y, edge.x );
		const decVector2 edgeNormalAbs( edgeNormal.Absolute() );
		
		ubo.SetParameterDataArrayVec4( deoglRenderPlanCompute::efcpHullEdgeNormal, i,
			edgeNormal.x, edgeNormal.y, edgeNormalAbs.x, edgeNormalAbs.y );
		ubo.SetParameterDataArrayFloat( deoglRenderPlanCompute::efcpHullEdgeDistance, i, edgeNormal * edgeStart );
	}
}

void deoglRenderPlanSkyLight::pCullLayerMask( deoglSPBlockUBO &ubo ){
	ubo.SetParameterDataBool( deoglRenderPlanCompute::efcpCullLayerMask, pPlan.GetUseLayerMask() );
	
	const uint64_t layerMask = pPlan.GetLayerMask().GetMask();
	ubo.SetParameterDataUVec2( deoglRenderPlanCompute::efcpLayerMask,
		( uint32_t )( layerMask >> 32 ), ( uint32_t )layerMask );
}

void deoglRenderPlanSkyLight::pSetSplits( deoglSPBlockUBO &ubo, float backtrack ){
	const float splitSizeLimitPixels = 1.0f;
	const float splitFactor = ( 1.0f / ( float )pPlan.GetShadowSkySize() ) * splitSizeLimitPixels;
	int i;
	
	ubo.SetParameterDataInt( deoglRenderPlanCompute::efcpSplitCount, pShadowLayerCount );
	
	for( i=0; i<pShadowLayerCount; i++ ){
		const sShadowLayer &sl = pShadowLayers[ i ];
		
		ubo.SetParameterDataArrayVec3( deoglRenderPlanCompute::efcpSplitMinExtend, i,
			decVector( sl.minExtend.x, sl.minExtend.y, sl.minExtend.z - backtrack ) );
		ubo.SetParameterDataArrayVec3( deoglRenderPlanCompute::efcpSplitMaxExtend, i, sl.maxExtend );
		ubo.SetParameterDataArrayVec2( deoglRenderPlanCompute::efcpSplitSizeThreshold, i,
			( sl.maxExtend.x - sl.minExtend.x ) * splitFactor,
			( sl.maxExtend.y - sl.minExtend.y ) * splitFactor );
	}
}



void deoglRenderPlanSkyLight::pDetermineShadowParameters(){
	const deoglConfiguration &config = pPlan.GetRenderThread().GetConfiguration();
	
	pUseLight = true;
	pUseShadow = config.GetShadowQuality() != deoglConfiguration::esqOff;
	pShadowLayerCount = 4;
	
	// if array-texture support is missing no shadows for the time being
	if( ! pglFramebufferTexture ){
		pUseShadow = false;
	}
	
	// if there is no direct light scrap the shadows
	if( ! pLayer->GetHasLightDirect() ){
		pUseShadow = false;
	}
	
	// if the total light contribution is a lot smaller than the total ambient only light
	// contribution of the entire sky this layer can be skipped to improve performance.
	// for example in a sky with a sun and moon light the sun typically has 16 intensity
	// and 4 ambient intensity for a total of 20 intensity. the moon light has typically
	// 0.009 intensity and 0.001 ambient intensity for a total of 0.01 intensity. during
	// the day the total moon light (0.01) is a lot less than the sun ambient light (4).
	// while it is possible the sun light is totally blocked and the moon light might
	// be visible this is barely every a use case worth wasting time. in the outlined
	// case the moon light is 0.25% . in general while rendering an intensity difference
	// of 1% can be barely noticed. but most of the time one light dominates so right
	// now a threshold of 10% is used. if the need arises for changing this value
	// a module parameter can be introduced
	if( pLayer->GetTotalLightIntensity() < pSky->GetTotalSkyAmbientIntensity() * 0.1f ){
		pUseLight = false;
		pUseShadow = false;
	}
	
	// temporary hack to deal with slow shadow casting for the video capturing
// 	if( pLayer->GetLightIntensity() < 0.1f ){
// 		pUseShadow = false;
// 	}
}

void deoglRenderPlanSkyLight::pCalcShadowLayerParams(){
//	const deSkyLayer &engSkyLayer = *pLayer->GetLayer();
	const decDMatrix &matCamInv = pPlan.GetInverseCameraMatrix();
	float znear = pPlan.GetCameraImageDistance();
	float zfar = pPlan.GetCameraViewDistance();
		//zfar = 20.0f; // NOTE for better performance cut this value down
	float fov = pPlan.GetCameraFov();
	float fovRatio = pPlan.GetCameraFovRatio();
//	float aspectRatio = plan.GetAspectRatio();
	float factor;
	int p;
	
	float fx, xnear = tanf( fov * 0.5f ); // * znear, dropped since we calc x'=z'*(xnear/znear)
	float fy, ynear = tanf( fov * 0.5f * fovRatio ); // * znear, dropped since we calc y'=z'*(ynear/znear)
	decVector frustumCorners[ 8 ];
	int c;
	
	// get light properties
	//const decMatrix matLig( decMatrix::CreateRotation( 0.0f, PI, 0.0f ) * decMatrix::CreateFromQuaternion( engSkyLayer.GetOrientation() ) );
	const decMatrix matLig( decMatrix::CreateRotation( 0.0f, PI, 0.0f ) * pLayer->GetMatrix() );
	const decMatrix matLigInv( matLig.Invert() );
	const decMatrix matCL( matCamInv.GetRotationMatrix().ToMatrix() * matLigInv );
	
	// calculate the frustum distances
	/*
	frustumNear[ 0 ] = znear;
	frustumNear[ 1 ] = znear + 10.0f;
	frustumNear[ 2 ] = znear + 100.0f;
	frustumFar[ 0 ] = frustumNear[ 1 ] * 1.005f;
	frustumFar[ 1 ] = frustumNear[ 2 ] * 1.005f;
	frustumFar[ 2 ] = zfar;
	
	passCount = 3;
	*/
	
	
	// nvidia:
	// z(i) = near * pow( far / near, i / N ) => has problems
	// z(i) = lambda*near*pow(far/near,i/N) + (1-lambda)*( near + (i/N)*(far-near) )
	//      better version with lambda strength of correction
	
	factor = 0.01f * 1024.0f / ( 2.0f * tanf( fov * 0.5f ) ); // precision * baseShadowSize / ( 2 * tan( fov / 2 ) )
	pShadowLayers[ 0 ].frustumNear = znear;
	pShadowLayers[ 1 ].frustumNear = znear + factor; // roughly 5.12
	pShadowLayers[ 2 ].frustumNear = znear + factor * pShadowLayers[ 1 ].frustumNear; // roughly 26.21
	pShadowLayers[ 3 ].frustumNear = znear + factor * pShadowLayers[ 2 ].frustumNear; // roughly 134.21
	pShadowLayers[ 0 ].frustumFar = pShadowLayers[ 1 ].frustumNear * 1.005f;
	pShadowLayers[ 1 ].frustumFar = pShadowLayers[ 2 ].frustumNear * 1.005f;
	pShadowLayers[ 2 ].frustumFar = pShadowLayers[ 3 ].frustumNear * 1.005f;
	pShadowLayers[ 3 ].frustumFar = zfar;
	pShadowLayers[ 0 ].layerBorder = pShadowLayers[ 1 ].frustumNear * 1.0025f;
	pShadowLayers[ 1 ].layerBorder = pShadowLayers[ 2 ].frustumNear * 1.0025f;
	pShadowLayers[ 2 ].layerBorder = pShadowLayers[ 3 ].frustumNear * 1.0025f;
	pShadowLayers[ 3 ].layerBorder = zfar;
	
	float lambda = 0.96f;//0.5f; // from config
	
	for( p=0; p<pShadowLayerCount; p++ ){
		if( p == pShadowLayerCount - 1 ){
			pShadowLayers[ p ].layerBorder = zfar;
			pShadowLayers[ p ].layerBorder = zfar;
			
		}else{
			factor = ( float )( p + 1 ) / ( float )pShadowLayerCount;
			pShadowLayers[ p ].frustumFar = lambda * znear * powf( zfar / znear, factor )
				+ ( 1.0f - lambda ) * ( znear + factor * ( zfar - znear ) );
			pShadowLayers[ p ].layerBorder = pShadowLayers[ p ].frustumFar * 1.0025f;
		}
		
		if( p == 0 ){
			pShadowLayers[ p ].frustumNear = znear * 1.005f;
			
		}else{
			pShadowLayers[ p ].frustumNear = pShadowLayers[ p - 1 ].frustumFar;
		}
	}
	
	/*
	float / * lambda = 0.5f, * / N = ( float )layerCount;
	ogl.LogInfoFormat( "RenderSkyLight: znear=%f zfar=%f", znear, zfar );
	for( p=0; p<layerCount; p++ ){
		ogl.LogInfoFormat( "layer %i: near=%f far=%f new=%f", p, pShadowLayers[ p ].frustumNear, pShadowLayers[ p ].frustumFar,
			lambda * znear * powf( zfar / znear, ( float )p / N ) + ( 1.0f - lambda ) * ( znear + ( ( float )p / N ) * ( zfar - znear ) ) );
	}
	*/
	
	/*
	ogl.LogInfoFormat( "RenderSkyLight: factor=%f", factor );
	for( p=0; p<layerCount; p++ ){
		ogl.LogInfoFormat( "pass %i: znear=%f zfar=%f", p+1, pShadowLayers[ p ].frustumNear, pShadowLayers[ p ].frustumFar );
	}
	*/
	
	// calculate layer parameters
	// const deoglConfiguration &config = pPlan.GetRenderThread().GetConfiguration();
	const float smDepthScaleFactor = 1.2f; // TODO add config option
	
	for( p=0; p<pShadowLayerCount; p++ ){
		sShadowLayer &sl = pShadowLayers[ p ];
		
		// calculate the 8 corners of the frustum used to create the shadow map
		//frustumNear = znear;
		//frustumFar = znear + 10.0f; // a 10 meter slice close to the player
		fx = xnear * sl.frustumNear;
		fy = ynear * sl.frustumNear;
		matCL.Transform( frustumCorners[ 0 ], -fx,  fy, sl.frustumNear );
		matCL.Transform( frustumCorners[ 1 ],  fx,  fy, sl.frustumNear );
		matCL.Transform( frustumCorners[ 2 ],  fx, -fy, sl.frustumNear );
		matCL.Transform( frustumCorners[ 3 ], -fx, -fy, sl.frustumNear );
		
		fx = xnear * sl.frustumFar;
		fy = ynear * sl.frustumFar;
		matCL.Transform( frustumCorners[ 4 ], -fx,  fy, sl.frustumFar );
		matCL.Transform( frustumCorners[ 5 ],  fx,  fy, sl.frustumFar );
		matCL.Transform( frustumCorners[ 6 ],  fx, -fy, sl.frustumFar );
		matCL.Transform( frustumCorners[ 7 ], -fx, -fy, sl.frustumFar );
		
		// calculate the smallest oriented box fitting around the frustum corners
		sl.minExtend = frustumCorners[ 0 ];
		sl.maxExtend = frustumCorners[ 0 ];
		
		for( c=1; c<8; c++ ){
			sl.minExtend.SetSmallest( frustumCorners[ c ] );
			sl.maxExtend.SetLargest( frustumCorners[ c ] );
		}
		
		// depth offset requires extends to be known
		const float smSize = sl.maxExtend.x - sl.minExtend.x;
		const float smDepthScale = sl.scale.z * 2.0f;
		const float smOffsetScale = smSize / pPlan.GetShadowSkySize();
		const float smOffsetBias = smOffsetScale; // * 0.5f;
		
		sl.zscale = smOffsetScale * smDepthScaleFactor * smDepthScale;
		sl.zoffset = smOffsetBias * smDepthScaleFactor * smDepthScale;
		
		if( pPlan.GetRenderThread().GetConfiguration().GetDebugSnapshot() == edbgsnapLightSkySplits ){
			const float zf = sl.frustumFar;
			const float zn = sl.frustumNear;
			const float xn = xnear * zn;
			const float yn = ynear * zn;
			const float xf = xnear * zf;
			const float yf = ynear * zf;
			const decDVector &pos = pPlan.GetCameraPosition();
			const decQuaternion &rot = pPlan.GetInverseCameraMatrix().ToQuaternion();
			
			printf( "\n# frustm for split %i\n", p );
			printf( "scale = 0.01\n" );
			printf( "meshFrustum = bpy.data.meshes.new( 'frustum%i' )\n", p );
			printf( "objFrustum = bpy.data.objects.new( 'frustum%i', meshFrustum )\n", p );
			printf( "bpy.context.scene.objects.link( objFrustum )\n" );
			printf( "bpy.context.scene.objects.active = objFrustum\n" );
			printf( "objFrustum.select = True\n" );
			printf( "vertices = []\n" );
			printf( "vertices.append( [%g,%g,%g] )\n", -xn, yn, zn );
			printf( "vertices.append( [%g,%g,%g] )\n", xn, yn, zn );
			printf( "vertices.append( [%g,%g,%g] )\n", xn, -yn, zn );
			printf( "vertices.append( [%g,%g,%g] )\n", -xn, -yn, zn );
			printf( "vertices.append( [%g,%g,%g] )\n", -xf, yf, zf );
			printf( "vertices.append( [%g,%g,%g] )\n", xf, yf, zf );
			printf( "vertices.append( [%g,%g,%g] )\n", xf, -yf, zf );
			printf( "vertices.append( [%g,%g,%g] )\n", -xf, -yf, zf );
			printf( "vertices = [ cposs( v, scale ) for v in vertices ]\n" );
			printf( "faces = []\n" );
			printf( "faces.append( [%i,%i,%i,%i] )\n", 1, 5, 6, 2 );
			printf( "faces.append( [%i,%i,%i,%i] )\n", 4, 0, 3, 7 );
			printf( "faces.append( [%i,%i,%i,%i] )\n", 4, 5, 1, 0 );
			printf( "faces.append( [%i,%i,%i,%i] )\n", 3, 2, 6, 7 );
			printf( "faces.append( [%i,%i,%i,%i] )\n", 5, 4, 7, 6 );
			printf( "faces.append( [%i,%i,%i,%i] )\n", 0, 1, 2, 3 );
			printf( "faces = [ f[::-1] for f in faces ]\n" );
			printf( "meshFrustum.from_pydata( vertices, [], faces )\n" );
			printf( "meshFrustum.update()\n" );
			printf( "objFrustum.location = cposs( [%g,%g,%g], scale )\n", pos.x, pos.y, pos.z );
			printf( "objFrustum.rotation_mode = 'QUATERNION'\n" );
			printf( "objFrustum.rotation_quaternion = cquat( [%g,%g,%g,%g] )\n", rot.x, rot.y, rot.z, rot.w );
		}
		//ogl.LogInfoFormat( "RenderSkyLight: box min=(%f,%f,%f) max=(%f,%f,%f)", minExtend.x, minExtend.y, minExtend.z,
		//	maxExtend.x, maxExtend.y, maxExtend.z );
	}
	
	// adjust shadow depth offsets
	pShadowLayers[ 0 ].zscale *= 1.0f;
	pShadowLayers[ 0 ].zoffset *= 1.0f;
	
	pShadowLayers[ 1 ].zscale *= 1.5f;
	pShadowLayers[ 1 ].zoffset *= 1.5f;
	
	pShadowLayers[ 2 ].zscale *= 2.0f;
	pShadowLayers[ 2 ].zoffset *= 2.0f;
	
	pShadowLayers[ 3 ].zscale *= 2.5f;
	pShadowLayers[ 3 ].zoffset *= 2.5f;
}

void deoglRenderPlanSkyLight::pWaitFinishedFindContent(){
	/*
	if( ! pTaskFindContent ){
		return;
	}
	
// 	pPlan.GetRenderThread().GetLogger().LogInfoFormat( "RenderPlanSkyLight(%p, %p) WaitFinishedFindContent(%p)", this, &pPlan, pTaskFindContent );
	pTaskFindContent->GetSemaphore().Wait();
	
	pPlan.GetRenderThread().GetRenderers().GetCanvas().SampleDebugInfoPlanPrepareSkyLightFindContent(
		pPlan, pTaskFindContent->GetElapsedTime() );
	
	pTaskFindContent->FreeReference();
	pTaskFindContent = NULL;
	*/
	
	ReadVisibleElements();
	pPlan.GetRenderThread().GetRenderers().GetCanvas().SampleDebugInfoPlanPrepareSkyLightFindContent( pPlan );
}

void deoglRenderPlanSkyLight::pWaitFinishedGIFindContent(){
	if( ! pTaskGIFindContent ){
		return;
	}
	
	pTaskGIFindContent->GetSemaphore().Wait();
	
	deoglRenderCanvas &rc = pPlan.GetRenderThread().GetRenderers().GetCanvas();
	rc.SampleDebugInfoPlanPrepareSkyLightGIFindContent( pPlan, pTaskGIFindContent->GetElapsedTime() );
	
	pTaskGIFindContent->FreeReference();
	pTaskGIFindContent = NULL;
}

void deoglRenderPlanSkyLight::pGICalcShadowLayerParams(){
	const deoglGIState * const giState = pPlan.GetUpdateGIState();
	if( ! giState ){
		return;
	}
	
	const deoglGICascade &cascade = giState->GetSkyShadowCascade();
	
	const deoglConfiguration &config = pPlan.GetRenderThread().GetConfiguration();
	const decMatrix matLig( decMatrix::CreateRotation( 0.0f, PI, 0.0f ) * pLayer->GetMatrix() );
	
	pGIShadowSize = 2048; // 1024
	
	pGIShadowLayer.frustumNear = 0.0f;
	pGIShadowLayer.frustumFar = 1.0f;
	pGIShadowLayer.layerBorder = 1.0f;
	pGIShadowLayer.zscale = config.GetDistShadowScale();
	pGIShadowLayer.zoffset = config.GetDistShadowBias();
	
	deoglCollisionBox colBoxGI( decVector(), cascade.GetDetectionBox(), matLig.Invert().ToQuaternion() );
	deoglCollisionBox enclosingBox;
	colBoxGI.GetEnclosingBox( &enclosingBox );
	
	pGIShadowLayer.minExtend = enclosingBox.GetCenter() - enclosingBox.GetHalfSize();
	pGIShadowLayer.maxExtend = enclosingBox.GetCenter() + enclosingBox.GetHalfSize();
	
	deoglSkyLayerGICascade &slgs = *pLayer->AddGICascade( cascade );
	slgs.Update();
	pGIShadowUpdateStatic = slgs.GetDirtyStaticShadow();
	slgs.ClearDirtyStaticShadow();
	
	deoglSCSolid &scsolid = slgs.GetShadowCaster().GetSolid();
	
	if( scsolid.GetStaticMap() ){
		if( scsolid.GetStaticMap()->GetWidth() != pGIShadowSize ){
			scsolid.DropStatic();
			pGIShadowUpdateStatic = true;
		}
		
	}else{
		pGIShadowUpdateStatic = true;
	}
}
