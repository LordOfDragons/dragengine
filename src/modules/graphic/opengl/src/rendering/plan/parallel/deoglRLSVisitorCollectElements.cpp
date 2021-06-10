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

#include "deoglRLSVisitorCollectElements.h"
#include "../../plan/deoglRenderPlan.h"
#include "../../../collidelist/deoglCollideList.h"
#include "../../../collidelist/deoglCollideListComponent.h"
#include "../../../collidelist/deoglCollideListHTSector.h"
#include "../../../collidelist/deoglCollideListHTSCluster.h"
#include "../../../collidelist/deoglCollideListPropField.h"
#include "../../../collidelist/deoglCollideListPropFieldType.h"
#include "../../../collidelist/deoglCollideListPropFieldCluster.h"
#include "../../../world/deoglWorldOctree.h"
#include "../../../world/deoglRWorld.h"
#include "../../../billboard/deoglRBillboard.h"
#include "../../../component/deoglRComponent.h"
#include "../../../light/deoglRLight.h"
#include "../../../particle/deoglParticleEmitterInstance.h"
#include "../../../propfield/deoglRPropField.h"
#include "../../../propfield/deoglRPropFieldType.h"
#include "../../../propfield/deoglPropFieldCluster.h"
#include "../../../sky/deoglRSkyLayer.h"
#include "../../../sky/deoglRSkyInstanceLayer.h"
#include "../../../terrain/heightmap/deoglHTSCluster.h"
#include "../../../terrain/heightmap/deoglHTView.h"
#include "../../../terrain/heightmap/deoglHTViewSector.h"
#include "../../../terrain/heightmap/deoglRHTSector.h"
#include "../../../utils/convexhull/deoglConvexHull2D.h"
#include "../../../utils/collision/deoglCollisionBox.h"
#include "../../../utils/collision/deoglDCollisionVolume.h"
#include "../../../utils/collision/deoglDCollisionSphere.h"
#include "../../../utils/collision/deoglDCollisionBox.h"
#include "../../../utils/collision/deoglDCollisionDetection.h"

#include <dragengine/common/exceptions.h>



// Class deoglRLSVisitorCollectElements
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRLSVisitorCollectElements::deoglRLSVisitorCollectElements( deoglCollideList &collideList ) :
pCollideList( collideList ),
pSplitCount( 0 ),
pShaftFar( 0.0f ),
pFrustumPlaneCount( 0 ),
pEdgeCount( 0 ),
pCullLayerMask( false ),
pOcclusionTest( NULL )
{
	int i;
	for( i=0; i<6; i++ ){
		pFrustumPlaneDistance[ i ] = 0.0f;
	}
	for( i=0; i<5; i++ ){
		pEdgeDistance[ i ] = 0.0f;
	}
}



// Management
///////////////

void deoglRLSVisitorCollectElements::SetCullLayerMask( bool cull ){
	pCullLayerMask = cull;
}

void deoglRLSVisitorCollectElements::SetLayerMask( const decLayerMask &layerMask ){
	pLayerMask = layerMask;
}



void deoglRLSVisitorCollectElements::InitFromFrustum( deoglRenderPlan &plan,
deoglRSkyInstanceLayer &skyLayer, float backtrack ){
	const decDMatrix &matCamInv = plan.GetInverseCameraMatrix();
	const decDVector &camPos = plan.GetCameraPosition();
	decVector frustumBoxMinExtend, frustumBoxMaxExtend;
	deoglDCollisionBox axisAlignedBoundaryBox;
	decDVector planeNormal, planePosition;
	deoglDCollisionFrustum volumeFrustum;
	deoglConvexHull2D frustumHull;
	int i;
	
	// parameters required for the calculations later on
	const float fov = plan.GetCameraFov();
	const float fovRatio = plan.GetCameraFovRatio();
	const float zfar = plan.GetCameraViewDistance();
	const float xfar = tanf( fov * 0.5f ) * zfar; // * znear, dropped since we calc x'=z'*(xnear/znear)
	const float yfar = tanf( fov * 0.5f * fovRatio ) * zfar; // * znear, dropped since we calc y'=z'*(ynear/znear)
	
	const decMatrix matLig = decMatrix::CreateRotation( 0.0f, PI, 0.0f ) * skyLayer.GetMatrix();
	const decMatrix matLigInv = matLig.Invert();
	const decMatrix matCL = matCamInv.GetRotationMatrix().ToMatrix() * matLigInv;
	const decQuaternion quatLig = matLig.ToQuaternion();
	
	pMatrixLightSpace = decDMatrix::CreateTranslation( -camPos ) * decDMatrix( matLigInv );
	
	// calculate the corner points of the frustum in light space. only the far points and the origin
	// are transformed. correctly the near points would have to be transformed and processed too but
	// they are usually so close to the origin that processing them is a waste of time not yielding
	// any better results than using just the origin
	decVector frustumPoints[ 5 ];
	
	frustumPoints[ 0 ] = matCL.GetPosition();
	matCL.Transform( frustumPoints[ 1 ], -xfar, yfar, zfar );
	matCL.Transform( frustumPoints[ 2 ], xfar, yfar, zfar );
	matCL.Transform( frustumPoints[ 3 ], xfar, -yfar, zfar );
	matCL.Transform( frustumPoints[ 4 ], -xfar, -yfar, zfar );
	
	// determine the box surrounding the entire frustum
	frustumBoxMinExtend = frustumPoints[ 0 ];
	frustumBoxMaxExtend = frustumPoints[ 0 ];
	for( i=1; i<5; i++ ){
		frustumBoxMinExtend.SetSmallest( frustumPoints[ i ] );
		frustumBoxMaxExtend.SetLargest( frustumPoints[ i ] );
	}
	
	pFrustumBoxMinExtend = frustumBoxMinExtend;
	pFrustumBoxMaxExtend = frustumBoxMaxExtend;
	
	frustumBoxMinExtend.z -= backtrack; // to catch shadow casters outside the view
	
	// init the boundary box volume. this will be used to visit the octree with
	pVolumeShadowBox.SetFromExtends( decDVector( frustumBoxMinExtend ), decDVector( frustumBoxMaxExtend ) );
	pVolumeShadowBox.SetCenter( camPos + decDVector( matLig * pVolumeShadowBox.GetCenter().ToVector() ) );
	pVolumeShadowBox.SetOrientation( quatLig );
	
	pVolumeShadowBox.GetEnclosingBox( &axisAlignedBoundaryBox );
	pBoundaryBoxMinExtend = axisAlignedBoundaryBox.GetCenter() - axisAlignedBoundaryBox.GetHalfSize();
	pBoundaryBoxMaxExtend = axisAlignedBoundaryBox.GetCenter() + axisAlignedBoundaryBox.GetHalfSize();
	
	// calculate the frustum volume in light shadow space used for fine grained testing. we are only interested
	// in planes on the back side hence facing away from the light. only these planes can potentially cull
	// objects located behind the frustum. important to know is that the frustum volume stores normals pointing
	// inside the frustum
	pShaftFar = frustumBoxMaxExtend.z;
	pAxisX = pVolumeShadowBox.GetAxisX().ToVector();
	pAxisY = pVolumeShadowBox.GetAxisY().ToVector();
	pAxisZ = pVolumeShadowBox.GetAxisZ().ToVector();
	pAbsAxisX = pAxisX.Absolute();
	pAbsAxisY = pAxisY.Absolute();
	pAbsAxisZ = pAxisZ.Absolute();
	
	volumeFrustum.SetFrustum( plan.GetCameraMatrix() * plan.GetFrustumMatrix() );
	
	pFrustumPlaneCount = 0;
	
	planeNormal = pMatrixLightSpace.TransformNormal( volumeFrustum.GetLeftNormal() );
	if( planeNormal.z < 0.0 ){
		planePosition = pMatrixLightSpace * ( volumeFrustum.GetLeftNormal() * volumeFrustum.GetLeftDistance() );
		pFrustumPlaneNormal[ pFrustumPlaneCount ] = planeNormal.ToVector();
		pFrustumPlaneNormalAbs[ pFrustumPlaneCount ].x = fabsf( pFrustumPlaneNormal[ pFrustumPlaneCount ].x );
		pFrustumPlaneNormalAbs[ pFrustumPlaneCount ].y = fabsf( pFrustumPlaneNormal[ pFrustumPlaneCount ].y );
		pFrustumPlaneNormalAbs[ pFrustumPlaneCount ].z = fabsf( pFrustumPlaneNormal[ pFrustumPlaneCount ].z );
		pFrustumPlaneDistance[ pFrustumPlaneCount++ ] = ( float )( planeNormal * planePosition );
	}
	
	planeNormal = pMatrixLightSpace.TransformNormal( volumeFrustum.GetRightNormal() );
	if( planeNormal.z < 0.0 ){
		planePosition = pMatrixLightSpace * ( volumeFrustum.GetRightNormal() * volumeFrustum.GetRightDistance() );
		pFrustumPlaneNormal[ pFrustumPlaneCount ] = planeNormal.ToVector();
		pFrustumPlaneNormalAbs[ pFrustumPlaneCount ].x = fabsf( pFrustumPlaneNormal[ pFrustumPlaneCount ].x );
		pFrustumPlaneNormalAbs[ pFrustumPlaneCount ].y = fabsf( pFrustumPlaneNormal[ pFrustumPlaneCount ].y );
		pFrustumPlaneNormalAbs[ pFrustumPlaneCount ].z = fabsf( pFrustumPlaneNormal[ pFrustumPlaneCount ].z );
		pFrustumPlaneDistance[ pFrustumPlaneCount++ ] = ( float )( planeNormal * planePosition );
	}
	
	planeNormal = pMatrixLightSpace.TransformNormal( volumeFrustum.GetTopNormal() );
	if( planeNormal.z < 0.0 ){
		planePosition = pMatrixLightSpace * ( volumeFrustum.GetTopNormal() * volumeFrustum.GetTopDistance() );
		pFrustumPlaneNormal[ pFrustumPlaneCount ] = planeNormal.ToVector();
		pFrustumPlaneNormalAbs[ pFrustumPlaneCount ].x = fabsf( pFrustumPlaneNormal[ pFrustumPlaneCount ].x );
		pFrustumPlaneNormalAbs[ pFrustumPlaneCount ].y = fabsf( pFrustumPlaneNormal[ pFrustumPlaneCount ].y );
		pFrustumPlaneNormalAbs[ pFrustumPlaneCount ].z = fabsf( pFrustumPlaneNormal[ pFrustumPlaneCount ].z );
		pFrustumPlaneDistance[ pFrustumPlaneCount++ ] = ( float )( planeNormal * planePosition );
	}
	
	planeNormal = pMatrixLightSpace.TransformNormal( volumeFrustum.GetBottomNormal() );
	if( planeNormal.z < 0.0 ){
		planePosition = pMatrixLightSpace * ( volumeFrustum.GetBottomNormal() * volumeFrustum.GetBottomDistance() );
		pFrustumPlaneNormal[ pFrustumPlaneCount ] = planeNormal.ToVector();
		pFrustumPlaneNormalAbs[ pFrustumPlaneCount ].x = fabsf( pFrustumPlaneNormal[ pFrustumPlaneCount ].x );
		pFrustumPlaneNormalAbs[ pFrustumPlaneCount ].y = fabsf( pFrustumPlaneNormal[ pFrustumPlaneCount ].y );
		pFrustumPlaneNormalAbs[ pFrustumPlaneCount ].z = fabsf( pFrustumPlaneNormal[ pFrustumPlaneCount ].z );
		pFrustumPlaneDistance[ pFrustumPlaneCount++ ] = ( float )( planeNormal * planePosition );
	}
	
	planeNormal = pMatrixLightSpace.TransformNormal( volumeFrustum.GetNearNormal() );
	if( planeNormal.z < 0.0 ){
		planePosition = pMatrixLightSpace * ( volumeFrustum.GetNearNormal() * volumeFrustum.GetNearDistance() );
		pFrustumPlaneNormal[ pFrustumPlaneCount ] = planeNormal.ToVector();
		pFrustumPlaneNormalAbs[ pFrustumPlaneCount ].x = fabsf( pFrustumPlaneNormal[ pFrustumPlaneCount ].x );
		pFrustumPlaneNormalAbs[ pFrustumPlaneCount ].y = fabsf( pFrustumPlaneNormal[ pFrustumPlaneCount ].y );
		pFrustumPlaneNormalAbs[ pFrustumPlaneCount ].z = fabsf( pFrustumPlaneNormal[ pFrustumPlaneCount ].z );
		pFrustumPlaneDistance[ pFrustumPlaneCount++ ] = ( float )( planeNormal * planePosition );
	}
	
	planeNormal = pMatrixLightSpace.TransformNormal( volumeFrustum.GetFarNormal() );
	if( planeNormal.z < 0.0 ){
		planePosition = pMatrixLightSpace * ( volumeFrustum.GetFarNormal() * volumeFrustum.GetFarDistance() );
		pFrustumPlaneNormal[ pFrustumPlaneCount ] = planeNormal.ToVector();
		pFrustumPlaneNormalAbs[ pFrustumPlaneCount ].x = fabsf( pFrustumPlaneNormal[ pFrustumPlaneCount ].x );
		pFrustumPlaneNormalAbs[ pFrustumPlaneCount ].y = fabsf( pFrustumPlaneNormal[ pFrustumPlaneCount ].y );
		pFrustumPlaneNormalAbs[ pFrustumPlaneCount ].z = fabsf( pFrustumPlaneNormal[ pFrustumPlaneCount ].z );
		pFrustumPlaneDistance[ pFrustumPlaneCount++ ] = ( float )( planeNormal * planePosition );
	}
	
	// calculate the convex hull of the frustum projected to the shadow map plane. each edge in the hull becomes
	// a plane to test components against. the planes are oriented perpendicular to the light direction. this
	// way a simple 2d convex hull test can be used. there will be between 3 to 5 planes to test
	for( i=0; i<5; i++ ){
		frustumHull.AddPoint( frustumPoints[ i ].x, frustumPoints[ i ].y );
	}
	
	frustumHull.CalculateHull();
	
	pEdgeCount = frustumHull.GetHullPointCount();
	
	for( i=0; i<pEdgeCount; i++ ){
		const decVector2 edgeStart = frustumHull.GetHullPointVectorAt( i );
		const decVector2 edge = ( frustumHull.GetHullPointVectorAt( ( i + 1 ) % pEdgeCount ) - edgeStart ).Normalized();
		
		pEdgeNormal[ i ].Set( -edge.y, edge.x );
		pEdgeNormalAbs[ i ].Set( fabs( -edge.y ), fabs( edge.x ) );
		pEdgeDistance[ i ] = pEdgeNormal[ i ] * edgeStart;
	}
	
	// reset the split count to 0
	pSplitCount = 0;
}

void deoglRLSVisitorCollectElements::InitFromGIBox( const decDVector &position,
const decVector &detectionBox, deoglRSkyInstanceLayer &skyLayer, float backtrack ){
	// parameters required for the calculations later on
	const decMatrix matLig( decMatrix::CreateRotation( 0.0f, PI, 0.0f ) * skyLayer.GetMatrix() );
	
	pMatrixLightSpace = decDMatrix::CreateTranslation( -position ) * decDMatrix( matLig.Invert() );
	
	deoglCollisionBox giBox( decVector(), detectionBox, matLig.Invert().ToQuaternion() );
	
	deoglCollisionBox giBoxAABB;
	giBox.GetEnclosingBox( &giBoxAABB );
	
	pFrustumBoxMinExtend = giBoxAABB.GetCenter() - giBoxAABB.GetHalfSize();
	pFrustumBoxMaxExtend = giBoxAABB.GetCenter() + giBoxAABB.GetHalfSize();
	
	// init the boundary box volume. this will be used to visit the octree with.
	// backtrack is used to catch shadow casters behind box
	pVolumeShadowBox.SetFromExtends( decDVector( pFrustumBoxMinExtend )
		- decDVector( 0.0, 0.0, backtrack ), decDVector( pFrustumBoxMaxExtend ) );
	pVolumeShadowBox.SetCenter( position + decDVector( matLig * pVolumeShadowBox.GetCenter().ToVector() ) );
	pVolumeShadowBox.SetOrientation( matLig.ToQuaternion() );
	
	deoglDCollisionBox aabb;
	pVolumeShadowBox.GetEnclosingBox( &aabb );
	pBoundaryBoxMinExtend = aabb.GetCenter() - aabb.GetHalfSize();
	pBoundaryBoxMaxExtend = aabb.GetCenter() + aabb.GetHalfSize();
	
	// for the box test we could use a box instead of planes but this requires extra code.
	// we are only interested in planes facing away from the light. only these planes can
	// potentially cull objects located behind the box. since the GI box is axis aligned
	// with the world the planes are simple to calculate. normals have to point inside
	// 
	// NOTE this all has been disabled since in the case of GI box culling components
	//      outside is not so well defined and more is better than less here
	pFrustumPlaneCount = 0;
	pEdgeCount = 0;
	
	// store absolute axis. this is required for size rejection. the rest is not required
	// inside the frustum
	pAxisX = pVolumeShadowBox.GetAxisX().ToVector();
	pAxisY = pVolumeShadowBox.GetAxisY().ToVector();
	pAxisZ = pVolumeShadowBox.GetAxisZ().ToVector();
	pAbsAxisX = pAxisX.Absolute();
	pAbsAxisY = pAxisY.Absolute();
	pAbsAxisZ = pAxisZ.Absolute();
	
#if 0
	pShaftFar = pFrustumBoxMaxExtend.z;
	pAxisX = pVolumeShadowBox.GetAxisX().ToVector();
	pAxisY = pVolumeShadowBox.GetAxisY().ToVector();
	pAxisZ = pVolumeShadowBox.GetAxisZ().ToVector();
	pAbsAxisX = pAxisX.Absolute();
	
	pFrustumPlaneCount = 0;
	
#if 0
	decDVector planeNormal, planePosition;
	
	planeNormal = pMatrixLightSpace.TransformRight(); // left plane
	planePosition = pMatrixLightSpace.Transform( -detectionBox.x, 0.0, 0.0 );
	if( planeNormal.z < 0.0 ){
		pFrustumPlaneNormal[ pFrustumPlaneCount ] = planeNormal.ToVector();
		pFrustumPlaneNormalAbs[ pFrustumPlaneCount ] = pFrustumPlaneNormal[ pFrustumPlaneCount ].Absolute();
		pFrustumPlaneDistance[ pFrustumPlaneCount++ ] = ( float )( planeNormal * planePosition );
		
	}else{
		planeNormal = -planeNormal; // right plane
		pFrustumPlaneNormal[ pFrustumPlaneCount ] = planeNormal.ToVector();
		pFrustumPlaneNormalAbs[ pFrustumPlaneCount ] = pFrustumPlaneNormal[ pFrustumPlaneCount ].Absolute();
		pFrustumPlaneDistance[ pFrustumPlaneCount++ ] = ( float )( planeNormal * planePosition );
	}
	
	planeNormal = pMatrixLightSpace.TransformUp(); // bottom plane
	planePosition = pMatrixLightSpace.Transform( 0.0, -detectionBox.y, 0.0 );
	if( planeNormal.z < 0.0 ){
		pFrustumPlaneNormal[ pFrustumPlaneCount ] = planeNormal.ToVector();
		pFrustumPlaneNormalAbs[ pFrustumPlaneCount ] = pFrustumPlaneNormal[ pFrustumPlaneCount ].Absolute();
		pFrustumPlaneDistance[ pFrustumPlaneCount++ ] = ( float )( planeNormal * planePosition );
		
	}else{
		planeNormal = -planeNormal; // top plane
		pFrustumPlaneNormal[ pFrustumPlaneCount ] = planeNormal.ToVector();
		pFrustumPlaneNormalAbs[ pFrustumPlaneCount ] = pFrustumPlaneNormal[ pFrustumPlaneCount ].Absolute();
		pFrustumPlaneDistance[ pFrustumPlaneCount++ ] = ( float )( planeNormal * planePosition );
	}
	
	planeNormal = pMatrixLightSpace.TransformNormal( decDVector( 0.0, 0.0, 1.0 ) ); // near plane
	planePosition = pMatrixLightSpace.Transform( 0.0, 0.0, -detectionBox.z );
	if( planeNormal.z < 0.0 ){
		pFrustumPlaneNormal[ pFrustumPlaneCount ] = planeNormal.ToVector();
		pFrustumPlaneNormalAbs[ pFrustumPlaneCount ] = pFrustumPlaneNormal[ pFrustumPlaneCount ].Absolute();
		pFrustumPlaneDistance[ pFrustumPlaneCount++ ] = ( float )( planeNormal * planePosition );
		
	}else{
		planeNormal = -planeNormal; // far plane
		pFrustumPlaneNormal[ pFrustumPlaneCount ] = planeNormal.ToVector();
		pFrustumPlaneNormalAbs[ pFrustumPlaneCount ] = pFrustumPlaneNormal[ pFrustumPlaneCount ].Absolute();
		pFrustumPlaneDistance[ pFrustumPlaneCount++ ] = ( float )( planeNormal * planePosition );
	}
#endif
	
	// calculate the convex hull of the box projected to the shadow map plane. each edge in
	// the hull becomes a plane to test components against. the planes are oriented perpendicular
	// to the light direction. this way a simple 2d convex hull test can be used. there will be
	// between 4 to 6 planes to test
	pEdgeCount = 0;
#if 0
	int i;
	for( i=0; i<5; i++ ){
		frustumHull.AddPoint( frustumPoints[ i ].x, frustumPoints[ i ].y );
	}
	
	frustumHull.CalculateHull();
	
	pEdgeCount = frustumHull.GetHullPointCount();
	
	for( i=0; i<pEdgeCount; i++ ){
		const decVector2 edgeStart = frustumHull.GetHullPointVectorAt( i );
		const decVector2 edge = ( frustumHull.GetHullPointVectorAt( ( i + 1 ) % pEdgeCount ) - edgeStart ).Normalized();
		
		pEdgeNormal[ i ].Set( -edge.y, edge.x );
		pEdgeNormalAbs[ i ].Set( fabs( -edge.y ), fabs( edge.x ) );
		pEdgeDistance[ i ] = pEdgeNormal[ i ] * edgeStart;
	}
#endif
	
	// reset the split count to 0
	pSplitCount = 0;
#endif
}

void deoglRLSVisitorCollectElements::AddSplit( const decVector &minExtend,
const decVector &maxExtend, const decVector2 &sizeThreshold ){
	if( pSplitCount == 4 ){
		DETHROW( deeInvalidParam );
	}
	
	pSplitMinExtend[ pSplitCount ] = minExtend;
	pSplitMaxExtend[ pSplitCount ] = maxExtend;
	pSplitSizeThreshold[ pSplitCount ] = sizeThreshold;
	pSplitCount++;
}

void deoglRLSVisitorCollectElements::AddOcclusionTestInputData(
deoglOcclusionTest *occlusionTest, const decDVector &referencePosition ){
	pOcclusionTest = occlusionTest;
	pReferencePosition = referencePosition;
}

void deoglRLSVisitorCollectElements::VisitWorldOctree( deoglWorldOctree &octree ){
	octree.VisitNodesColliding( this, pBoundaryBoxMinExtend, pBoundaryBoxMaxExtend );
}

void deoglRLSVisitorCollectElements::VisitHTView( const deoglHTView &htview ){
	const int sectorCount = htview.GetSectorCount();
	int i, cascadeMask;
	
	for( i=0; i<sectorCount; i++ ){
		deoglHTViewSector &sector = *htview.GetSectorAt( i );
		
		// TODO test if the sector is visible using a bounding box test
		
		const deoglRHTSector &htsector = sector.GetSector();
		const decDVector offset( htsector.CalcWorldPosition( pReferencePosition ) );
		const deoglHTSCluster *cluster = htsector.GetClusters();
		const int count = htsector.GetClusterCount();
		decPoint i;
		
		deoglCollideListHTSector *clsector = NULL;
		
		for( i.y=0; i.y<count; i.y++ ){
			for( i.x=0; i.x<count; i.x++, cluster++ ){
				const decDVector realOffset( offset + decDVector( cluster->GetCenter() ) );
				const decDVector realHalfExtend( cluster->GetHalfExtends() );
				if( ! TestAxisAlignedBox( realOffset - realHalfExtend, realOffset + realHalfExtend, cascadeMask ) ){
					continue;
				}
				
				if( ! clsector ){
					clsector = pCollideList.AddHTSector( &sector );
				}
				clsector->AddCluster( i )->SetCascadeMask( cascadeMask );
			}
		}
		
// 		if( pOcclusionTest && clsector ){
// 			clsector->StartOcclusionTest( *pOcclusionTest, pReferencePosition );
// 		}
	}
}

void deoglRLSVisitorCollectElements::VisitPropFields( const deoglRWorld &world ){
	const int count = world.GetPropFieldCount();
	int i, cascadeMask;
	
	for( i=0; i<count; i++ ){
		deoglRPropField &propField = *world.GetPropFieldAt( i );
		const decDVector offset( propField.GetPosition() - pReferencePosition );
		const int typeCount = propField.GetTypeCount();
		int j;
		
		deoglCollideListPropField *clpropfield = NULL;
		
		for( j=0; j<typeCount; j++ ){
			deoglRPropFieldType &type = propField.GetTypeAt( j );
			const int clusterCount = type.GetClusterCount();
			int k;
			
			deoglCollideListPropFieldType *cltype = NULL;
			
			for( k=0; k<clusterCount; k++ ){
				deoglPropFieldCluster &cluster = *type.GetClusterAt( k );
				
				const decDVector minExtend( offset + decDVector( cluster.GetMinimumExtend() ) );
				const decDVector maxExtend( offset + decDVector( cluster.GetMaximumExtend() ) );
				if( ! TestAxisAlignedBox( minExtend, maxExtend, cascadeMask ) ){
					continue;
				}
				
				if( ! clpropfield ){
					clpropfield = pCollideList.AddPropField( &propField );
				}
				if( ! cltype ){
					cltype = clpropfield->AddType( &type );
				}
				cltype->AddCluster( &cluster )->SetCascadeMask( cascadeMask );
			}
		}
		
// 		if( pOcclusionTest && clpropfield ){
// 			clpropfield->StartOcclusionTest( *pOcclusionTest, pReferencePosition );
// 		}
	}
}



// Visiting
/////////////

void deoglRLSVisitorCollectElements::VisitNode( deoglDOctree *node, int ){
	// check if the node is fully inside or outside to speed up checks if possible
	if( ! TestAxisAlignedBox( node->GetMinimumExtend(), node->GetMaximumExtend() ) ){
		return; // axis aligned box of node is outside the boundary box. ignore all contained elements
	}
	
	// visit content
	const deoglWorldOctree &sonode = *( ( deoglWorldOctree* )node );
	pVisitComponents( sonode );
	pVisitBillboards( sonode );
}



bool deoglRLSVisitorCollectElements::TestAxisAlignedBox( const decDVector &minExtend, const decDVector &maxExtend ){
	// test if the axis aligned box hits the axis aligned boundary box
	if( ! deoglDCollisionDetection::AABoxHitsAABox( pBoundaryBoxMinExtend, pBoundaryBoxMaxExtend, minExtend, maxExtend ) ){
		return false;
	}
	
	if( pFrustumPlaneCount == 0 && pEdgeCount == 0 ){
		return true; // GI box
	}
	
	// calculate the parameters needed for the tests
	float distance1, distance2;
	deoglDCollisionBox cbox;
	decVector che, npos, nhe;
	float adjustdist;
	int i;
	
	cbox.SetFromExtends( minExtend, maxExtend );
	
	che = cbox.GetHalfSize().ToVector();
	nhe.x = pAbsAxisX * che;
	nhe.y = pAbsAxisY * che;
	nhe.z = pAbsAxisZ * che;
	
	npos = ( pMatrixLightSpace * cbox.GetCenter() ).ToVector();
	
	adjustdist = ( pShaftFar - npos.z ) * 0.5f;
	nhe.z += fabsf( adjustdist );
	npos.z += adjustdist;
	
	// test if the box projected to the shadow map plane hits the frustum hull projected to the shadow map plane
	for( i=0; i<pEdgeCount; i++ ){
		distance1 = npos.x * pEdgeNormal[ i ].x + npos.y * pEdgeNormal[ i ].y;
		distance2 = pEdgeDistance[ i ] + pEdgeNormalAbs[ i ].x * nhe.x + pEdgeNormalAbs[ i ].y * nhe.y;
		
		if( distance1 > distance2 ){
			return false;
		}
	}
	
	// test if the light space box shaft hits the light space frustum
	for( i=0; i<pFrustumPlaneCount; i++ ){
		distance1 = pFrustumPlaneDistance[ i ] - npos * pFrustumPlaneNormal[ i ];
		distance2 = nhe.x * pFrustumPlaneNormalAbs[ i ].x + nhe.y * pFrustumPlaneNormalAbs[ i ].y + nhe.z * pFrustumPlaneNormalAbs[ i ].z;
		
		if( distance1 > distance2 ){
			return false;
		}
	}
	
	// the box shaft potentially affects the shadow casting
	return true;
}

bool deoglRLSVisitorCollectElements::TestAxisAlignedBox( const decDVector &minExtend, const decDVector &maxExtend, int &splitMask ){
	// test if the axis aligned box hits the axis aligned boundary box
	if( ! deoglDCollisionDetection::AABoxHitsAABox( pBoundaryBoxMinExtend, pBoundaryBoxMaxExtend, minExtend, maxExtend ) ){
		return false;
	}
	
	// calculate the parameters needed for the tests
	float distance1, distance2;
	deoglDCollisionBox cbox;
	decVector che, npos, nhe;
	float adjustdist;
	int i;
	
	cbox.SetFromExtends( minExtend, maxExtend );
	
	che = cbox.GetHalfSize().ToVector();
	nhe.x = pAbsAxisX * che;
	nhe.y = pAbsAxisY * che;
	nhe.z = pAbsAxisZ * che;
	
	// GI box uses a simpler test which especially drops the component instead of masking it
	if( pFrustumPlaneCount == 0 && pEdgeCount == 0 ){
		splitMask = 1;
		return decVector2( nhe.x * 2.0f, nhe.y * 2.0f ) > pSplitSizeThreshold[ 0 ];
		// this additional check costs 2ms CPU time and saves 400ys GPU time
// 			&& pVolumeShadowBox.BoxHitsBox( &cbox );
	}
	
	// otherwise finish box for testing
	npos = ( pMatrixLightSpace * cbox.GetCenter() ).ToVector();
	
	adjustdist = ( pShaftFar - npos.z ) * 0.5f;
	nhe.z += fabsf( adjustdist );
	npos.z += adjustdist;
	
	// test if the box projected to the shadow map plane hits the frustum hull projected to the shadow map plane
	for( i=0; i<pEdgeCount; i++ ){
		distance1 = npos.x * pEdgeNormal[ i ].x + npos.y * pEdgeNormal[ i ].y;
		distance2 = pEdgeDistance[ i ] + pEdgeNormalAbs[ i ].x * nhe.x + pEdgeNormalAbs[ i ].y * nhe.y;
		
		if( distance1 > distance2 ){
			return false;
		}
	}
	
	// test if the light space box shaft hits the light space frustum
	for( i=0; i<pFrustumPlaneCount; i++ ){
		distance1 = pFrustumPlaneDistance[ i ] - npos * pFrustumPlaneNormal[ i ];
		distance2 = nhe.x * pFrustumPlaneNormalAbs[ i ].x + nhe.y * pFrustumPlaneNormalAbs[ i ].y + nhe.z * pFrustumPlaneNormalAbs[ i ].z;
		
		if( distance1 > distance2 ){
			return false;
		}
	}
	
	// determine the split mask
	const decVector2 splitSize = decVector2( nhe.x * 2.0f, nhe.y * 2.0f );
	const decVector splitMinExtend = npos - nhe;
	const decVector splitMaxExtend = npos + nhe;
	
	splitMask = 0;
	
	for( i=0; i<pSplitCount; i++ ){
		if( splitMinExtend <= pSplitMaxExtend[ i ] && splitMaxExtend >= pSplitMinExtend[ i ] && splitSize > pSplitSizeThreshold[ i ]){
			splitMask |= 1 << i;
		}
	}
	
	// the box shaft potentially affects the shadow casting
	return true;
}



// Private Functions
//////////////////////

void deoglRLSVisitorCollectElements::pVisitComponents( const deoglWorldOctree &sonode ){
	const int count = sonode.GetComponentCount();
	int i, cascadeMask;
	
	for( i=0; i<count; i++ ){
		deoglRComponent * const addComponent = sonode.GetComponentAt( i );
		const deoglRComponent &component = *addComponent;
		
		// cull using layer mask if required. components with empty layer mask never match
		// and thus are never culled
		if( pCullLayerMask && component.GetLayerMask().IsNotEmpty()
		&& pLayerMask.MatchesNot( component.GetLayerMask() ) ){
			continue;
		}
		
		if( ! TestAxisAlignedBox( component.GetMinimumExtend(), component.GetMaximumExtend(), cascadeMask ) ){
			continue;
		}
		
		deoglCollideListComponent &clcomponent = *pCollideList.AddComponent( addComponent );
		clcomponent.SetCascadeMask( cascadeMask );
		clcomponent.SetSpecialFlags( cascadeMask );
		
		if( pOcclusionTest ){
			clcomponent.StartOcclusionTest( *pOcclusionTest, pReferencePosition );
		}
	}
}

void deoglRLSVisitorCollectElements::pVisitBillboards( const deoglWorldOctree &sonode ){
	const int count = sonode.GetBillboardList().GetCount();
	int i, cascadeMask;
	
	for( i=0; i<count; i++ ){
		deoglRBillboard * const addBillboard = sonode.GetBillboardList().GetAt( i );
		const deoglRBillboard &billboard = *addBillboard;
		
		// cull using layer mask if required. billboards with empty layer mask never match
		// and thus are never culled
		if( pCullLayerMask && billboard.GetLayerMask().IsNotEmpty()
		&& pLayerMask.MatchesNot( billboard.GetLayerMask() ) ){
			continue;
		}
		
		if( ! TestAxisAlignedBox( billboard.GetMinimumExtend(), billboard.GetMaximumExtend(), cascadeMask ) ){
			continue;
		}
		
		addBillboard->SetSkyShadowSplitMask( cascadeMask ); // TODO move this into deoglCollideListBillboard
		pCollideList.AddBillboard( addBillboard );
		// TODO add occlusion test input
	}
}
