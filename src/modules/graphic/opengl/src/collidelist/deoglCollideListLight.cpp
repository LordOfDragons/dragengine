/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "deoglCollideListLight.h"
#include "../light/deoglRLight.h"
#include "../rendering/plan/deoglRenderPlan.h"
#include "../renderthread/deoglRenderThread.h"
#include "../occlusiontest/deoglOcclusionTest.h"
#include "../occquery/deoglOcclusionQuery.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decConvexVolume.h>
#include <dragengine/common/math/decConvexVolumeFace.h>
#include <dragengine/common/math/decConvexVolumeList.h>



// Class deoglCollideListLight
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCollideListLight::deoglCollideListLight() :
pLight( NULL ),
pCulled( false ),
pCameraInside( false ){
}

deoglCollideListLight::~deoglCollideListLight(){
}



// Management
///////////////

void deoglCollideListLight::Clear(){
	SetLight( NULL );
	pCulled = false;
}

void deoglCollideListLight::SetLight( deoglRLight *light ){
	pLight = light;
}

void deoglCollideListLight::SetCulled( bool culled ){
	pCulled = culled;
}

void deoglCollideListLight::TestCameraInside( const deoglRenderPlan &plan ){
	if( ! pLight ){
		DETHROW( deeInvalidParam );
	}
	
	const float safetyMargin = 0.01; // 1cm should be enough to be safe
	const float imageDistance = plan.GetCameraImageDistance();
	const float nx = imageDistance * tanf( plan.GetCameraFov() * 0.5f );
	const float ny = imageDistance * tanf( plan.GetCameraFov() * plan.GetCameraFovRatio() * 0.5f );
	const float nd = sqrtf( nx * nx + ny * ny + imageDistance * imageDistance ) + safetyMargin;
	const decDVector &cameraPosition = plan.GetCameraPosition();
	const decDVector extendOffset( nd, nd, nd );
	
	// by default the camera is not inside unless some other evidence is found
	pCameraInside = false;
	
	// if the camera is not inside the extends box then there is no chance to inside the
	// light volumes at all. to avoid flickering the box is slightly enlarged as the
	// camera near plane has a certain distance from the position itself
	const decDVector &minExtend = pLight->GetMinimumExtend(); // WARNING potential side effect. not parallel. move to PrepareForRender
	const decDVector &maxExtend = pLight->GetMaximumExtend(); // WARNING potential side effect. not parallel. move to PrepareForRender
	
	pCameraInside = ( cameraPosition >= minExtend - extendOffset )
		&& ( cameraPosition <= maxExtend + extendOffset );
	
	if( ! pCameraInside ){
		return;
	}
	
	// test against all convex volumes. to get a safe testing we can not simply test the
	// camera point since the camera clips already into the light volume at the near
	// clipping plane. this can be achieved by not just testing on what side the point
	// is on a plane but adding a small offset. a too large offset only causes a full
	// screen render that might not have been necessary but is never wrong
	const decVector localCameraPosition( pLight->GetInverseMatrix() * cameraPosition );
	const decConvexVolumeList &convexVolumeList = *pLight->GetConvexVolumeList();
	const int volumeCount = convexVolumeList.GetVolumeCount();
	bool insideVolume;
	int i, j;
	
	// this test is not working correctly. the correct test requires finding for each volume
	// the point inside the volume that is closes to the camera point. this has to be done
	// by clamping the point against all faces (keep normal*(cam-facepoint) <= 0). then
	// check if the distance from the camera point to this closest point is at most nd.
	// using squared check removes the need for a square root.
	
	pCameraInside = false;
	
	for( i=0; i<volumeCount; i++ ){
		const decConvexVolume &convexVolume = *convexVolumeList.GetVolumeAt( i );
		const int faceCount = convexVolume.GetFaceCount();
		
		insideVolume = true;
		
		for( j=0; j<faceCount; j++ ){
			const decConvexVolumeFace &convexVolumeFace = *convexVolume.GetFaceAt( j );
			const decVector &normal = convexVolumeFace.GetNormal();
			const decVector &point = convexVolume.GetVertexAt( convexVolumeFace.GetVertexAt( 0 ) );
			
			if( normal * ( localCameraPosition - point ) > nd ){
				insideVolume = false;
				break;
			}
		}
		
		if( insideVolume ){
			pCameraInside = true;
			break;
		}
	}
}

void deoglCollideListLight::StartOcclusionTest( deoglOcclusionTest &occlusionTest,
const decDVector &cameraPosition ){
	if( ! pLight ){
		DETHROW( deeInvalidParam );
	}
	
	pCulled = false;
	
	pLight->UpdateLightVolume();
	
	occlusionTest.AddInputData(
		( pLight->GetMinimumExtend() - cameraPosition ).ToVector(),
		( pLight->GetMaximumExtend() - cameraPosition ).ToVector(), this );
}

void deoglCollideListLight::OcclusionTestInvisible(){
	pCulled = true;
}

deoglOcclusionQuery &deoglCollideListLight::GetOcclusionQuery(){
	if( ! pLight ){
		DETHROW( deeInvalidParam );
	}
	
	return pLight->GetOcclusionQuery(); // temporary. move to this class later on
}

bool deoglCollideListLight::IsHiddenByOccQuery() const{
	if( ! pLight ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! pCameraInside && pLight->HasOcclusionQuery() ){
		// check if the query result exists already
		//if( pOcclusionQuery->HasResult() ){
			// retrieve the result. later on we are going to store this value
			// somewhere so we do not have to trip down to the driver to get
			// the result since lights can be queried multiple times if they
			// should be drawn. might be improved once upon time.
			return ! pLight->GetOcclusionQuery().GetResultAny();
		//}
	}
	return false;
}
