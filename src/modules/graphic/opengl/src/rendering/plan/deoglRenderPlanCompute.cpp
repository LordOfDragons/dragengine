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
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../world/deoglRWorld.h"
#include "../../world/deoglWorldOctree.h"

#include <dragengine/common/exceptions.h>


// Class deoglRenderPlanCompute
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderPlanCompute::deoglRenderPlanCompute( deoglRenderPlan &plan ) :
pPlan( plan ){
}

deoglRenderPlanCompute::~deoglRenderPlanCompute(){
}



// Management
///////////////

void deoglRenderPlanCompute::PrepareWorldCSOctree(){
	deoglWorldOctree &octree = pPlan.GetWorld()->GetOctree();
	octree.UpdateCSCounts();
	
	if( ! pWorldCSOctree ){
		pWorldCSOctree.TakeOver( new deoglWorldCSOctree( pPlan.GetRenderThread() ) );
	}
	
	pWorldCSOctree->SetReferencePosition( pPlan.GetWorld()->GetReferencePosition() );
	pWorldCSOctree->BeginWriting( octree.GetCSNodeCount(), octree.GetCSElementCount() );
	octree.WriteCSData( pWorldCSOctree, 0 );
	pWorldCSOctree->EndWriting();
}

void deoglRenderPlanCompute::PrepareFindConfig(){
	if( ! pFindConfig ){
		
	}
}



// Protected
//////////////

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
