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
#include "../plan/deoglRenderPlan.h"
#include "../../collidelist/deoglCollideList.h"

#include "../../world/deoglWorldOctree.h"
#include "../../billboard/deoglRBillboard.h"
#include "../../component/deoglRComponent.h"
#include "../../light/deoglRLight.h"
#include "../../particle/deoglParticleEmitterInstance.h"
#include "../../sky/deoglRSkyLayer.h"
#include "../../sky/deoglRSkyInstanceLayer.h"
#include "../../utils/convexhull/deoglConvexHull2D.h"
#include "../../utils/collision/deoglDCollisionVolume.h"
#include "../../utils/collision/deoglDCollisionSphere.h"
#include "../../utils/collision/deoglDCollisionBox.h"
#include "../../utils/collision/deoglDCollisionDetection.h"

#include <dragengine/common/exceptions.h>



// Class deoglRLSVisitorCollectElements
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRLSVisitorCollectElements::deoglRLSVisitorCollectElements( deoglCollideList *collideList ){
	if( ! collideList ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	pCollideList = collideList;
	
	pShaftFar = 0.0f;
	
	for( i=0; i<6; i++ ){
		pFrustumPlaneDistance[ i ] = 0.0f;
	}
	pFrustumPlaneCount = 0;
	
	for( i=0; i<5; i++ ){
		pEdgeDistance[ i ] = 0.0f;
	}
	pEdgeCount = 0;
	
	pSplitCount = 0;
}



// Management
///////////////

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
	pAbsAxisX.Set( fabsf( pAxisX.x ), fabsf( pAxisX.y ), fabsf( pAxisX.z ) );
	pAbsAxisY.Set( fabsf( pAxisY.x ), fabsf( pAxisY.y ), fabsf( pAxisY.z ) );
	pAbsAxisZ.Set( fabsf( pAxisZ.x ), fabsf( pAxisZ.y ), fabsf( pAxisZ.z ) );
	
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

void deoglRLSVisitorCollectElements::AddSplit( const decVector &minExtend, const decVector &maxExtend,
const decVector2 &sizeThreshold ){
	if( pSplitCount == 4 ){
		DETHROW( deeInvalidParam );
	}
	
	pSplitMinExtend[ pSplitCount ] = minExtend;
	pSplitMaxExtend[ pSplitCount ] = maxExtend;
	pSplitSizeThreshold[ pSplitCount ] = sizeThreshold;
	pSplitCount++;
}

void deoglRLSVisitorCollectElements::VisitWorldOctree( deoglWorldOctree &octree ){
	octree.VisitNodesColliding( this, pBoundaryBoxMinExtend, pBoundaryBoxMaxExtend );
}



// Visiting
/////////////

void deoglRLSVisitorCollectElements::VisitNode( deoglDOctree *node, int intersection ){
	// check if the node is fully inside or outside to speed up checks if possible
	if( ! TestAxisAlignedBox( node->GetCenter() - node->GetHalfSize(), node->GetCenter() + node->GetHalfSize() ) ){
		return; // axis aligned box of node is outside the boundary box. ignore all contained elements
	}
	
	// test content
	const deoglWorldOctree &sonode = *( ( deoglWorldOctree* )node );
	int i, count, splitMask;
	deoglDCollisionBox box;
	
	// test components
	count = sonode.GetComponentCount();
	deoglRComponent *component;
	
	for( i=0; i<count; i++ ){
		component = sonode.GetComponentAt( i );
		
		if( TestAxisAlignedBox( component->GetMinimumExtend(), component->GetMaximumExtend(), splitMask ) ){
			component->SetSkyShadowSplitMask( splitMask );
			//component->SetVisible( true );
			pCollideList->AddComponent( component );
		}
	}
	
	// test billboards
	count = sonode.GetBillboardList().GetCount();
	deoglRBillboard *billboard;
	
	for( i=0; i<count; i++ ){
		billboard = sonode.GetBillboardList().GetAt( i );
		
		if( TestAxisAlignedBox( billboard->GetMinimumExtend(), billboard->GetMaximumExtend(), splitMask ) ){
			billboard->SetSkyShadowSplitMask( splitMask );
			//billboard->SetVisible( true );
			pCollideList->AddBillboard( billboard );
		}
	}
}



bool deoglRLSVisitorCollectElements::TestAxisAlignedBox( const decDVector &minExtend, const decDVector &maxExtend ){
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
