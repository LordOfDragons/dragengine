/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
pCameraInside( false ),
pCameraInsideOccQueryBox( true ),
pOcclusionQuery( nullptr ),
pOccQueryValid( false ){
}

deoglCollideListLight::~deoglCollideListLight(){
	if( pOcclusionQuery ){
		delete pOcclusionQuery;
	}
}



// Management
///////////////

void deoglCollideListLight::Clear(){
	SetLight( nullptr );
	pCulled = false;
	pOccQueryValid = false;
}

void deoglCollideListLight::SetLight( deoglRLight *light ){
	pLight = light;
}

void deoglCollideListLight::SetCulled( bool culled ){
	pCulled = culled;
}

void deoglCollideListLight::TestInside( const deoglRenderPlan &plan ){
	DEASSERT_NOTNULL( pLight )
	
	if( pCulled ){ // happens if affecting GI but not camera
		pCameraInside = false;
		pCameraInsideOccQueryBox = false;
		return;
	}
	
	const float safetyMargin = plan.GetCameraImageDistance() + 0.01f;
	const decDVector &cameraPosition = plan.GetCameraPosition();
	const decDVector &minExtend = pLight->GetMinimumExtend();
	const decDVector &maxExtend = pLight->GetMaximumExtend();
	
	// unfortunately checking for camera inside is not enough for occlusion query. the camera
	// inside test checks against the light extends but also against the light volume. it is
	// thus possible the light is outside the view frustum but still inside the extends box
	// which is used for rendering the occlusion query. if the camera is though inside the
	// occlusion query box it is possible the box faces on the far side end up behind geometry
	// which causes the occlusion query to return 0 and the light is incorrectly culled
	// 
	// if the light volume is used for rendering the occlusion query then the camera inside
	// result is working.
	const decDVector vsm( safetyMargin, safetyMargin, safetyMargin );
	pCameraInsideOccQueryBox = ( cameraPosition >= minExtend - vsm ) && ( cameraPosition <= maxExtend + vsm );
	
	// for camera inside light use a slightly different test which uses a larger box. this is
	// required since the light volume is a low resolution mesh and thus is larger than the
	// mathematically perfect volume
	const float imageDistance = plan.GetCameraImageDistance();
	const float nx = imageDistance * tanf( plan.GetCameraFov() * 0.5f );
	const float ny = imageDistance * tanf( plan.GetCameraFov() * plan.GetCameraFovRatio() * 0.5f );
	const float nd = sqrtf( nx * nx + ny * ny + imageDistance * imageDistance ) + safetyMargin;
	const decDVector extend( nd, nd, nd );
	
	pCameraInside = ( cameraPosition >= minExtend - extend ) && ( cameraPosition <= maxExtend + extend );
	
	// if the camera is not inside the extends box then there is no chance to inside the
	// light volumes at all. to avoid flickering the box is slightly enlarged as the
	// camera near plane has a certain distance from the position itself
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
	DEASSERT_NOTNULL( pLight )
	
	pCulled = false;
	
	occlusionTest.AddInputData(
		( pLight->GetMinimumExtend() - cameraPosition ).ToVector(),
		( pLight->GetMaximumExtend() - cameraPosition ).ToVector(), this );
}

void deoglCollideListLight::OcclusionTestInvisible(){
	pCulled = true;
}

deoglOcclusionQuery &deoglCollideListLight::GetOcclusionQuery(){
	if( ! pOcclusionQuery ){
		DEASSERT_NOTNULL( pLight )
		pOcclusionQuery = new deoglOcclusionQuery( pLight->GetRenderThread() );
	}
	return *pOcclusionQuery;
}

bool deoglCollideListLight::IsHiddenByOccQuery() const{
	DEASSERT_NOTNULL( pLight )
	
// 	if( ! pCameraInside && pOcclusionQuery ){
	if( ! pCameraInsideOccQueryBox && pOcclusionQuery && pOccQueryValid ){
		// check if the query result exists already
		//if( pOcclusionQuery->HasResult() ){
			// retrieve the result. later on we are going to store this value
			// somewhere so we do not have to trip down to the driver to get
			// the result since lights can be queried multiple times if they
			// should be drawn. might be improved once upon time.
			return ! pOcclusionQuery->GetResultAny();
		//}
	}
	return false;
}

void deoglCollideListLight::SetOcclusionQueryValid( bool valid ){
	pOccQueryValid = valid;
}
