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
#include "../../gi/deoglGICascade.h"
#include "../../gi/deoglGIState.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../shaders/paramblock/deoglSPBMapBuffer.h"
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
	pUBOFindConfig->SetParameterCount( 13 );
	pUBOFindConfig->GetParameterAt( efcpNodeCount ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // uint
	pUBOFindConfig->GetParameterAt( efcpElementCount ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // uint
	pUBOFindConfig->GetParameterAt( efcpUpdateElementCount ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // uint
	pUBOFindConfig->GetParameterAt( efcpFrustumPlanes ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 6 ); // vec4
	pUBOFindConfig->GetParameterAt( efcpFrustumSelect ).SetAll( deoglSPBParameter::evtBool, 3, 1, 6 ); // bvec3
	pUBOFindConfig->GetParameterAt( efcpGIMinExtend ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
	pUBOFindConfig->GetParameterAt( efcpGIMaxExtend ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
	pUBOFindConfig->GetParameterAt( efcpLayerMask ).SetAll( deoglSPBParameter::evtInt, 2, 1, 1 ); // uvec2
	pUBOFindConfig->GetParameterAt( efcpCullLayerMask ).SetAll( deoglSPBParameter::evtBool, 1, 1, 1 ); // bool
	pUBOFindConfig->GetParameterAt( efcpCullFlags ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // uint
	pUBOFindConfig->GetParameterAt( efcpCameraPosition ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
	pUBOFindConfig->GetParameterAt( efcpCameraView ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
	pUBOFindConfig->GetParameterAt( efcpErrorScaling ).SetAll( deoglSPBParameter::evtFloat, 1, 1, 1 ); // float
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
	pSSBOCounters->SetElementCount( 2 );
	pSSBOCounters->MapToStd140();
	pSSBOCounters->SetBindingPoint( 3 );
	pSSBOCounters->SetBindingPointAtomic( 0 );
	
	pSSBOVisibleElements.TakeOver( new deoglSPBlockSSBO( plan.GetRenderThread() ) );
	pSSBOVisibleElements->SetRowMajor( rowMajor );
	pSSBOVisibleElements->SetParameterCount( 1 );
	pSSBOVisibleElements->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtInt, 4, 1, 1 ); // uvec4
	pSSBOVisibleElements->MapToStd140();
	pSSBOVisibleElements->SetBindingPoint( 1 );
}

deoglRenderPlanCompute::~deoglRenderPlanCompute(){
}



// Management
///////////////

void deoglRenderPlanCompute::PrepareWorldCompute(){
	decTimer timer;
	
	deoglWorldCompute &compute = pPlan.GetWorld()->GetCompute();
	compute.Prepare();
	pPlan.GetRenderThread().GetLogger().LogInfoFormat( "RenderPlanCompute.PrepareWorldCompute: WorldCompute.Prepare: %dys", ( int )( timer.GetElapsedTime() * 1e6f ) );
	
	
	
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
	if( visElCount > pSSBOVisibleElements->GetElementCount() ){
		pSSBOVisibleElements->SetElementCount( visElCount );
	}
	pSSBOVisibleElements->EnsureBuffer();
	
	pClearCounters();
}

void deoglRenderPlanCompute::ReadVisibleElements(){
	const deoglWorldCompute &wcompute = pPlan.GetWorld()->GetCompute();
	if( wcompute.GetElementCount() == 0 ){
		return;
	}
	
	// read counters to get the count of visible elements written
	decTimer timer;
		glFinish();
		pPlan.GetRenderThread().GetLogger().LogInfoFormat( "RenderPlanCompute.ReadVisibleElements: S0 %dys", ( int )( timer.GetElapsedTime() * 1e6f ) );
	const sCounters * const counters = ( const sCounters * )pSSBOCounters->ReadBuffer( 1 );
	const int indexCount = counters[ 0 ].counter;
	pPlan.GetRenderThread().GetLogger().LogInfoFormat( "RenderPlanCompute.ReadVisibleElements: S1 %dys [%d]", ( int )( timer.GetElapsedTime() * 1e6f ), indexCount );
	if( indexCount == 0 ){
		return;
	}
	
	// read written visible element indices
	const uint32_t * const indices = ( const uint32_t * )pSSBOVisibleElements->ReadBuffer( ( ( indexCount - 1 ) / 4 ) + 1 );
	pPlan.GetRenderThread().GetLogger().LogInfoFormat( "RenderPlanCompute.ReadVisibleElements: S2 %dys", ( int )( timer.GetElapsedTime() * 1e6f ) );
	int ct[ 4 ] = { 0, 0, 0, 0 };
	int i;
	
	for( i=0; i<indexCount; i++ ){
		const deoglWorldCompute::Element &element = wcompute.GetElementAt( indices[ i ] );
		ct[ element.GetType() ]++;
	}
	pPlan.GetRenderThread().GetLogger().LogInfoFormat( "RenderPlanCompute.ReadVisibleElements: S3 %dys [%d %d %d %d]", ( int )( timer.GetElapsedTime() * 1e6f ), ct[0], ct[1], ct[2], ct[3] );
}



// Protected
//////////////

void deoglRenderPlanCompute::pPrepareFindConfig(){
	deoglSPBlockUBO &ubo = pUBOFindConfig;
	const deoglSPBMapBuffer mapped( ubo );
	
	// const deoglWorldCSOctree &octree = pWorldCSOctree;
	const decDVector &refpos = pPlan.GetWorld()->GetReferencePosition();
	const deoglWorldCompute &wcompute = pPlan.GetWorld()->GetCompute();
	
	// octree counts
	ubo.SetParameterDataUInt( efcpNodeCount, 0 );//octree.GetNodeCount() );
	ubo.SetParameterDataUInt( efcpElementCount, wcompute.GetElementCount() );
	ubo.SetParameterDataUInt( efcpUpdateElementCount, wcompute.GetUpdateElementCount() );
	
	// frustum culling
	const deoglDCollisionFrustum &frustum = *pPlan.GetUseFrustum();
	pSetFrustumPlane( 0, frustum.GetLeftNormal(), frustum.GetLeftDistance() );
	pSetFrustumPlane( 1, frustum.GetRightNormal(), frustum.GetRightDistance() );
	pSetFrustumPlane( 2, frustum.GetTopNormal(), frustum.GetTopDistance() );
	pSetFrustumPlane( 3, frustum.GetBottomNormal(), frustum.GetBottomDistance() );
	pSetFrustumPlane( 4, frustum.GetNearNormal(), frustum.GetNearDistance() );
	pSetFrustumPlane( 5, frustum.GetFarNormal(), frustum.GetFarDistance() );
	
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
	
	// layer mask culling
	ubo.SetParameterDataBool( efcpCullLayerMask, pPlan.GetUseLayerMask() );
	
	const uint64_t layerMask = pPlan.GetLayerMask().GetMask();
	ubo.SetParameterDataUVec2( efcpLayerMask, ( uint32_t )( layerMask >> 32 ), ( uint32_t )layerMask );
	
	// cull by flags
	uint32_t cullFlags = 0;
	if( pPlan.GetIgnoreDynamicComponents() ){
		cullFlags |= deoglWorldCSOctree::ecsefComponentDynamic;
	}
	ubo.SetParameterDataUInt( efcpCullFlags, cullFlags );
	
	// camera parameters
	ubo.SetParameterDataVec3( efcpCameraPosition, pPlan.GetCameraPosition() - refpos );
	ubo.SetParameterDataVec3( efcpCameraView, pPlan.GetInverseCameraMatrix().TransformView() );
	
	// error scaling
	ubo.SetParameterDataFloat( efcpErrorScaling, pCalculateErrorScaling() );
}

void deoglRenderPlanCompute::pClearCounters(){
	deoglSPBlockSSBO &ssbo = pSSBOCounters;
	const deoglSPBMapBuffer mapped( ssbo );
	int i;
	
	for( i=0; i<2; i++ ){
		ssbo.SetParameterDataUVec3( 0, i, 1, 1, 1 ); // work group size
		ssbo.SetParameterDataUInt( 1, i, 0 ); // count
	}
}

void deoglRenderPlanCompute::pSetFrustumPlane( int i, const decDVector& n, double d ){
	pUBOFindConfig->SetParameterDataArrayVec4( efcpFrustumPlanes, i, n, d );
	pUBOFindConfig->SetParameterDataArrayBVec3( efcpFrustumSelect, i, n.x > 0.0, n.y > 0.0, n.z > 0.0 );
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
