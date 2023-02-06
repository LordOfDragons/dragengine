/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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
#include "deoglRenderPlanCompute.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../collidelist/deoglCollideListComponent.h"
#include "../../collidelist/deoglCollideListLight.h"
#include "../../component/deoglRComponent.h"
#include "../../gi/deoglGICascade.h"
#include "../../gi/deoglGIState.h"
#include "../../propfield/deoglRPropField.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../shaders/paramblock/deoglSPBMapBuffer.h"
#include "../../terrain/heightmap/deoglHTView.h"
#include "../../terrain/heightmap/deoglHTViewSector.h"
#include "../../terrain/heightmap/deoglHTViewSectorCluster.h"
#include "../../terrain/heightmap/deoglHTSCluster.h"
#include "../../terrain/heightmap/deoglRHTSector.h"
#include "../../world/deoglRWorld.h"
#include "../../world/deoglWorldOctree.h"

#include <dragengine/common/exceptions.h>


// Class deoglRenderPlanCompute
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderPlanCompute::deoglRenderPlanCompute( deoglRenderPlan &plan ) :
pPlan( plan )
{
	const bool rowMajor = plan.GetRenderThread().GetCapabilities().GetUBOIndirectMatrixAccess().Working();
	
	pUBOFindConfig.TakeOver( new deoglSPBlockUBO( plan.GetRenderThread() ) );
	pUBOFindConfig->SetRowMajor( rowMajor );
	pUBOFindConfig->SetParameterCount( 22 );
	pUBOFindConfig->GetParameterAt( efcpNodeCount ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // uint
	pUBOFindConfig->GetParameterAt( efcpElementCount ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // uint
	pUBOFindConfig->GetParameterAt( efcpUpdateElementCount ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // uint
	pUBOFindConfig->GetParameterAt( efcpFrustumPlanes ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 6 ); // vec4[6]
	pUBOFindConfig->GetParameterAt( efcpFrustumPlanesAbs ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 6 ); // vec3[6]
	pUBOFindConfig->GetParameterAt( efcpFrustumSelect ).SetAll( deoglSPBParameter::evtBool, 3, 1, 6 ); // bvec3[6]
	pUBOFindConfig->GetParameterAt( efcpGIMinExtend ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
	pUBOFindConfig->GetParameterAt( efcpGIMaxExtend ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
	pUBOFindConfig->GetParameterAt( efcpLayerMask ).SetAll( deoglSPBParameter::evtInt, 2, 1, 1 ); // uvec2
	pUBOFindConfig->GetParameterAt( efcpCullLayerMask ).SetAll( deoglSPBParameter::evtBool, 1, 1, 1 ); // bool
	pUBOFindConfig->GetParameterAt( efcpCullFlags ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // uint
	pUBOFindConfig->GetParameterAt( efcpCameraPosition ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
	pUBOFindConfig->GetParameterAt( efcpCameraView ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
	pUBOFindConfig->GetParameterAt( efcpErrorScaling ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
	pUBOFindConfig->GetParameterAt( efcpHullEdgeNormal ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 5 ); // vec4[5]
	pUBOFindConfig->GetParameterAt( efcpHullEdgeDistance ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 5 ); // float[5]
	pUBOFindConfig->GetParameterAt( efcpMatrixLightSpace ).SetAll( deoglSPBParameter::evtFloat, 4, 3, 1 ); // mat4x3
	pUBOFindConfig->GetParameterAt( efcpShadowAxis ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 3 ); // vec3[3]
	pUBOFindConfig->GetParameterAt( efcpLightShaftFar ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
	pUBOFindConfig->GetParameterAt( efcpSplitMinExtend ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 4 ); // vec3[4]
	pUBOFindConfig->GetParameterAt( efcpSplitMaxExtend ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 4 ); // vec3[4]
	pUBOFindConfig->GetParameterAt( efcpSplitSizeThreshold ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 4 ); // vec2[4]
	pUBOFindConfig->MapToStd140();
	pUBOFindConfig->SetBindingPoint( 0 );
	
	pSSBOSearchNodes.TakeOver( new deoglSPBlockSSBO( plan.GetRenderThread() ) );
	pSSBOSearchNodes->SetRowMajor( rowMajor );
	pSSBOSearchNodes->SetParameterCount( 1 );
	pSSBOSearchNodes->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtInt, 4, 1, 1 ); // uvec4
	pSSBOSearchNodes->MapToStd140();
	pSSBOSearchNodes->SetBindingPoint( 2 );
	
	pSSBOCounters.TakeOver( new deoglSPBlockSSBO( plan.GetRenderThread() ) );
	pSSBOCounters->SetRowMajor( rowMajor );
	pSSBOCounters->SetParameterCount( 2 );
	pSSBOCounters->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtInt, 3, 1, 1 ); // uvec3
	pSSBOCounters->GetParameterAt( 1 ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // uint
	pSSBOCounters->SetElementCount( 1 );
	pSSBOCounters->MapToStd140();
	pSSBOCounters->SetBindingPoint( 3 );
	pSSBOCounters->SetBindingPointAtomic( 0 );
	
	pSSBOVisibleElements.TakeOver( new deoglSPBlockSSBO( plan.GetRenderThread() ) );
	pSSBOVisibleElements->SetRowMajor( rowMajor );
	pSSBOVisibleElements->SetParameterCount( 1 );
	pSSBOVisibleElements->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtInt, 4, 1, 1 ); // uvec4
	pSSBOVisibleElements->MapToStd140();
	pSSBOVisibleElements->SetBindingPoint( 1 );
	
	pSSBOVisibleElementsFlags.TakeOver( new deoglSPBlockSSBO( plan.GetRenderThread() ) );
	pSSBOVisibleElementsFlags->SetRowMajor( rowMajor );
	pSSBOVisibleElementsFlags->SetParameterCount( 1 );
	pSSBOVisibleElementsFlags->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtInt, 4, 1, 1 ); // uvec4
	pSSBOVisibleElementsFlags->MapToStd140();
	pSSBOVisibleElementsFlags->SetBindingPoint( 2 );
}

deoglRenderPlanCompute::~deoglRenderPlanCompute(){
}



// Management
///////////////

void deoglRenderPlanCompute::PrepareWorldCompute(){
	decTimer timer;
	
	deoglWorldCompute &compute = pPlan.GetWorld()->GetCompute();
	compute.Prepare();
	// pPlan.GetRenderThread().GetLogger().LogInfoFormat( "RenderPlanCompute.PrepareWorldCompute: WorldCompute.Prepare: %dys", ( int )( timer.GetElapsedTime() * 1e6f ) );
	
	
	
	// deoglWorldOctree &octree = pPlan.GetWorld()->GetOctree();
	// octree.UpdateCSCounts();
	
	// if( ! pWorldCSOctree ){
		// pWorldCSOctree.TakeOver( new deoglWorldCSOctree( pPlan.GetRenderThread() ) );
	// }
	// pPlan.GetRenderThread().GetLogger().LogInfoFormat( "PrepareWorldCSOctree: update: %dys", ( int )( timer.GetElapsedTime() * 1e6f ) );
	
	// pWorldCSOctree->SetReferencePosition( pPlan.GetWorld()->GetReferencePosition() );
	// pWorldCSOctree->BeginWriting( octree.GetCSNodeCount(), octree.GetCSElementCount() );
	// octree.WriteCSData( pWorldCSOctree, pWorldCSOctree->NextNode() );
	// pPlan.GetRenderThread().GetLogger().LogInfoFormat( "PrepareWorldCSOctree: write: %dys", ( int )( timer.GetElapsedTime() * 1e6f ) );
	// pWorldCSOctree->EndWriting();
	// pPlan.GetRenderThread().GetLogger().LogInfoFormat( "PrepareWorldCSOctree: upload: %dys", ( int )( timer.GetElapsedTime() * 1e6f ) );
}

void deoglRenderPlanCompute::PrepareBuffers(){
	pPrepareFindConfig();
	
	// pSSBOSearchNodes->SetElementCount( pWorldCSOctree->GetNodeCount() );
	// pSSBOSearchNodes->SetElementCount( ( ( pWorldCSOctree->GetElementCount() - 1 ) / 4 ) + 1 );
	// pSSBOSearchNodes->EnsureBuffer();
	
	const int visElCount = ( ( pPlan.GetWorld()->GetCompute().GetElementCount() - 1 ) / 4 ) + 1;
	pPrepareBuffer( pSSBOVisibleElements, visElCount );
	pPrepareBuffer( pSSBOVisibleElementsFlags, visElCount );
	
	pClearCounters();
}

void deoglRenderPlanCompute::ReadVisibleElements(){
	const deoglWorldCompute &wcompute = pPlan.GetWorld()->GetCompute();
	if( wcompute.GetElementCount() == 0 ){
		return;
	}
	
		// decTimer timer;
	const sCounters &counters = *( sCounters* )pSSBOCounters->ReadBuffer( 1 );
		// pPlan.GetRenderThread().GetLogger().LogInfoFormat("ReadVisibleElements: counter %dys", (int)(timer.GetElapsedTime()*1e6f));
	const int indexCount = counters.counter;
	if( indexCount == 0 ){
		return;
	}
	
	// read written visible element indices
	deoglOcclusionTest &occlusionTest = *pPlan.GetOcclusionTest();
	const decDVector &cameraPosition = pPlan.GetCameraPosition();
	deoglComponentList &componentsOccMap = pPlan.GetComponentsOccMap();
	deoglCollideList &collideList = pPlan.GetCollideList();
	deoglParticleEmitterInstanceList &clistParticleEmitterInstanceList = collideList.GetParticleEmitterList();
	deoglHTView * const htview = pPlan.GetHeightTerrainView();
	
	const uint32_t * const indices = ( const uint32_t * )pSSBOVisibleElements->ReadBuffer( ( ( indexCount - 1 ) / 4 ) + 1 );
	int i;
		// pPlan.GetRenderThread().GetLogger().LogInfoFormat("ReadVisibleElements: read %dys", (int)(timer.GetElapsedTime()*1e6f));
	
	for( i=0; i<indexCount; i++ ){
		const deoglWorldCompute::Element &element = wcompute.GetElementAt( indices[ i ] );
		// continue;
		
		switch( element.GetType() ){
		case deoglWorldCompute::eetComponent:{
			deoglRComponent * const component = ( deoglRComponent* )element.GetOwner();
			collideList.AddComponent( component )->StartOcclusionTest( occlusionTest, cameraPosition );
			if( component->GetOcclusionMesh() ){
				componentsOccMap.Add( component );
			}
			}break;
			
		case deoglWorldCompute::eetBillboard:
			collideList.AddBillboard( ( deoglRBillboard* )element.GetOwner() );
			break;
			
		case deoglWorldCompute::eetParticleEmitter:
			clistParticleEmitterInstanceList.Add( ( deoglRParticleEmitterInstance* )element.GetOwner() );
			break;
			
		case deoglWorldCompute::eetLight:{
			deoglCollideListLight &cllight = *collideList.AddLight( ( deoglRLight* )element.GetOwner() );
			cllight.StartOcclusionTest( occlusionTest, cameraPosition );  // if not culled by frustum
			//cllight.SetCulled( true );  // if culled by frustum but not by GI cascade box
			cllight.TestInside( pPlan );
			}break;
			
		case deoglWorldCompute::eetPropFieldCluster:
			collideList.AddPropFieldCluster( ( deoglPropFieldCluster* )element.GetOwner() );
			break;
			
		case deoglWorldCompute::eetHeightTerrainSectorCluster:
			if( htview ){
				const deoglHTSCluster &cluster = *( deoglHTSCluster* )element.GetOwner();
				collideList.AddHTSCluster( &htview->GetSectorAt(
					cluster.GetHTSector()->GetIndex() )->GetClusterAt( cluster.GetIndex() ) );
			}
			break;
		}
	}
		// pPlan.GetRenderThread().GetLogger().LogInfoFormat("ReadVisibleElements: list %dys", (int)(timer.GetElapsedTime()*1e6f));
}



// Protected
//////////////

void deoglRenderPlanCompute::pPrepareFindConfig(){
	deoglSPBlockUBO &ubo = pUBOFindConfig;
	const deoglSPBMapBuffer mapped( ubo );
	ubo.Clear();
	
	const decDVector &refpos = pPlan.GetWorld()->GetReferencePosition();
	const deoglWorldCompute &wcompute = pPlan.GetWorld()->GetCompute();
	
	ubo.SetParameterDataUInt( efcpNodeCount, 0 );
	ubo.SetParameterDataUInt( efcpElementCount, wcompute.GetElementCount() );
	ubo.SetParameterDataUInt( efcpUpdateElementCount, wcompute.GetUpdateElementCount() );
	
	// frustum culling
	const deoglDCollisionFrustum &frustum = *pPlan.GetUseFrustum();
	pSetFrustumPlane( ubo, 0, frustum.GetLeftNormal(), frustum.GetLeftDistance() );
	pSetFrustumPlane( ubo, 1, frustum.GetRightNormal(), frustum.GetRightDistance() );
	pSetFrustumPlane( ubo, 2, frustum.GetTopNormal(), frustum.GetTopDistance() );
	pSetFrustumPlane( ubo, 3, frustum.GetBottomNormal(), frustum.GetBottomDistance() );
	pSetFrustumPlane( ubo, 4, frustum.GetNearNormal(), frustum.GetNearDistance() );
	pSetFrustumPlane( ubo, 5, frustum.GetFarNormal(), frustum.GetFarDistance() );
	
	// gi cascase testing. to disable set min and max extends to the same value best far away
	const deoglGIState * const gistate = pPlan.GetUpdateGIState();
	if( gistate ){
		// correctly we would have to use here the cascade position but during this
		// parallel task run at an unknown time the cascade position is updated.
		// so instead we use the camera position which is the base for the cascade
		// position update. this can cause a potential error at the boundary of the
		// GI cascade but chances for this are slim. if really a problem half the
		// probe spacing can be added to the extends
		const deoglGICascade &cascade = gistate->GetActiveCascade();
		const decDVector position( pPlan.GetCameraPosition() - refpos );
		const decDVector halfExtend( cascade.GetDetectionBox() );
		
		ubo.SetParameterDataVec3( efcpGIMinExtend, position - halfExtend );
		ubo.SetParameterDataVec3( efcpGIMaxExtend, position + halfExtend );
	}
	
	ubo.SetParameterDataVec3( efcpCameraPosition, pPlan.GetCameraPosition() - refpos );
	ubo.SetParameterDataVec3( efcpCameraView, pPlan.GetInverseCameraMatrix().TransformView() );
	
	ubo.SetParameterDataFloat( efcpErrorScaling, pCalculateErrorScaling() );
	
	pCullLayerMask( ubo );
	
	// cull by flags
	uint32_t cullFlags = 0;
	if( pPlan.GetIgnoreDynamicComponents() ){
		cullFlags |= deoglWorldCSOctree::ecsefComponentDynamic;
	}
	ubo.SetParameterDataUInt( efcpCullFlags, cullFlags );
}

void deoglRenderPlanCompute::pPrepareBuffer( deoglSPBlockSSBO &ssbo, int count ){
	if( count > ssbo.GetElementCount() ){
		ssbo.SetElementCount( count );
	}
	ssbo.EnsureBuffer();
}

void deoglRenderPlanCompute::pClearCounters(){
	deoglSPBlockSSBO &ssbo = pSSBOCounters;
	const deoglSPBMapBuffer mapped( ssbo );
	int i;
	
	for( i=0; i<1; i++ ){
		ssbo.SetParameterDataUVec3( 0, i, 0, 1, 1 ); // work group size (x=0)
		ssbo.SetParameterDataUInt( 1, i, 0 ); // count
	}
}

void deoglRenderPlanCompute::pSetFrustumPlane( deoglSPBlockUBO &ubo, int i, const decDVector& n, double d ){
	ubo.SetParameterDataArrayVec4( efcpFrustumPlanes, i, n, d );
	ubo.SetParameterDataArrayVec3( efcpFrustumPlanesAbs, i, n.Absolute() );
	ubo.SetParameterDataArrayBVec3( efcpFrustumSelect, i, n.x > 0.0, n.y > 0.0, n.z > 0.0 );
}

void deoglRenderPlanCompute::pCalculateFrustumBoundaryBox(
decDVector &frustumMinExtend, decDVector &frustumMaxExtend ){
	// calculate the corner points of the frustum in light space. only the far points and the origin
	// are transformed. correctly the near points would have to be transformed and processed too but
	// they are usually so close to the origin that processing them is a waste of time not yielding
	// any better results than using just the origin
	const decDMatrix &matrix = pPlan.GetInverseCameraMatrix();
	const double fov = ( double )pPlan.GetCameraFov();
	const double fovRatio = ( double )pPlan.GetCameraFovRatio();
	const double zfar = ( double )pPlan.GetCameraViewDistance();
	const double xfar = tan( fov * 0.5 ) * zfar; // * znear, dropped since we calc x'=z'*(xnear/znear)
	const double yfar = tan( fov * 0.5 * fovRatio ) * zfar; // * znear, dropped since we calc y'=z'*(ynear/znear)
	decDVector points[ 4 ];
	int i;
	
	matrix.Transform( points[ 0 ], -xfar, yfar, zfar );
	matrix.Transform( points[ 1 ], xfar, yfar, zfar );
	matrix.Transform( points[ 2 ], xfar, -yfar, zfar );
	matrix.Transform( points[ 3 ], -xfar, -yfar, zfar );
	
	// determine the box surrounding the entire frustum
	frustumMinExtend = pPlan.GetCameraPosition();
	frustumMaxExtend = frustumMinExtend;
	
	for( i=0; i<4; i++ ){
		if( points[ i ].x < frustumMinExtend.x ){
			frustumMinExtend.x = points[ i ].x;
			
		}else if( points[ i ].x > frustumMaxExtend.x ){
			frustumMaxExtend.x = points[ i ].x;
		}
		
		if( points[ i ].y < frustumMinExtend.y ){
			frustumMinExtend.y = points[ i ].y;
			
		}else if( points[ i ].y > frustumMaxExtend.y ){
			frustumMaxExtend.y = points[ i ].y;
		}
		
		if( points[ i ].z < frustumMinExtend.z ){
			frustumMinExtend.z = points[ i ].z;
			
		}else if( points[ i ].z > frustumMaxExtend.z ){
			frustumMaxExtend.z = points[ i ].z;
		}
	}
	
	frustumMinExtend -= pPlan.GetWorld()->GetReferencePosition();
	frustumMaxExtend -= pPlan.GetWorld()->GetReferencePosition();
}

float deoglRenderPlanCompute::pCalculateErrorScaling(){
	// aspectRatio = w / h
	// dx = tan(fov / 2) * (cullPixelSize / (w / 2))
	// dy = tan(fov * fovRatio / 2) * (cullPixelSize / (w / 2)) / aspectRatio
	// dz = 1
	// 
	// x = dx * z
	// y = dy * z
	
	const float cullPixelSize = 1.0f;
	const float fov = pPlan.GetCameraFov();
	const float fovRatio = pPlan.GetCameraFovRatio();
	const float halfWidth = ( float )pPlan.GetViewportWidth() * 0.5f;
	const float halfHeight = ( float )pPlan.GetViewportHeight() * 0.5f;
	const float aspectRatio = halfWidth / halfHeight;
	const float dx = tanf( fov * 0.5f ) * ( cullPixelSize / halfWidth );
	const float dy = tanf( fov * 0.5f * fovRatio ) * ( cullPixelSize / halfWidth ) / aspectRatio;
	
	return decMath::min( dx, dy );
}

void deoglRenderPlanCompute::pCullLayerMask( deoglSPBlockUBO &ubo ){
	ubo.SetParameterDataBool( efcpCullLayerMask, pPlan.GetUseLayerMask() );
	
	const uint64_t layerMask = pPlan.GetLayerMask().GetMask();
	ubo.SetParameterDataUVec2( efcpLayerMask, ( uint32_t )( layerMask >> 32 ), ( uint32_t )layerMask );
}
