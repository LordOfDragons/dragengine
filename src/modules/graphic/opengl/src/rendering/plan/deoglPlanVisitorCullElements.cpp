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

#include "deoglPlanVisitorCullElements.h"
#include "deoglRenderPlan.h"
#include "../../collidelist/deoglCollideList.h"
#include "../../collidelist/deoglCollideListComponent.h"
#include "../../collidelist/deoglCollideListLight.h"
#include "../../billboard/deoglRBillboard.h"
#include "../../component/deoglRComponent.h"
#include "../../light/deoglRLight.h"
#include "../../occlusiontest/deoglOcclusionTest.h"
#include "../../particle/deoglRParticleEmitterInstance.h"
#include "../../world/deoglWorldOctree.h"
#include "../../utils/collision/deoglDCollisionVolume.h"
#include "../../utils/collision/deoglDCollisionSphere.h"
#include "../../utils/collision/deoglDCollisionBox.h"
#include "../../utils/collision/deoglDCollisionDetection.h"


#include <dragengine/common/exceptions.h>



// Class deoglPlanVisitorCullElements
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPlanVisitorCullElements::deoglPlanVisitorCullElements( deoglRenderPlan *plan ){
	if( ! plan ){
		DETHROW( deeInvalidParam );
	}
	
	pPlan = plan;
	
	pFrustum = NULL;
	
	pCullPixelSize = 1.0f;
	pErrorScaling = 1.0f;
	
	pCullDynamicComponents = false;
	
	pCullLayerMask = false;
	
	SetVisitAll( true );
}



// Management
///////////////

void deoglPlanVisitorCullElements::Init( deoglDCollisionFrustum *frustum ){
	if( ! frustum ){
		DETHROW( deeInvalidParam );
	}
	
	pFrustum = frustum;
	pCameraView = pPlan->GetInverseCameraMatrix().TransformView();
	
	CalculateFrustumBoundaryBox();
	CalculateErrorScaling();
}



void deoglPlanVisitorCullElements::SetFrustumExtends( const decDVector &minExtend, const decDVector &maxExtend ){
	pFrustumMinExtend = minExtend;
	pFrustumMaxExtend = maxExtend;
}

void deoglPlanVisitorCullElements::CalculateFrustumBoundaryBox(){
	// calculate the corner points of the frustum in light space. only the far points and the origin
	// are transformed. correctly the near points would have to be transformed and processed too but
	// they are usually so close to the origin that processing them is a waste of time not yielding
	// any better results than using just the origin
	const decDMatrix &matrix = pPlan->GetInverseCameraMatrix();
	const double fov = ( double )pPlan->GetCameraFov();
	const double fovRatio = ( double )pPlan->GetCameraFovRatio();
	const double zfar = ( double )pPlan->GetCameraViewDistance();
	const double xfar = tan( fov * 0.5 ) * zfar; // * znear, dropped since we calc x'=z'*(xnear/znear)
	const double yfar = tan( fov * 0.5 * fovRatio ) * zfar; // * znear, dropped since we calc y'=z'*(ynear/znear)
	decDVector points[ 4 ];
	int i;
	
	matrix.Transform( points[ 0 ], -xfar, yfar, zfar );
	matrix.Transform( points[ 1 ], xfar, yfar, zfar );
	matrix.Transform( points[ 2 ], xfar, -yfar, zfar );
	matrix.Transform( points[ 3 ], -xfar, -yfar, zfar );
	
	// determine the box surrounding the entire frustum
	pFrustumMinExtend = pPlan->GetCameraPosition();
	pFrustumMaxExtend = pFrustumMinExtend;
	
	for( i=0; i<4; i++ ){
		if( points[ i ].x < pFrustumMinExtend.x ){
			pFrustumMinExtend.x = points[ i ].x;
			
		}else if( points[ i ].x > pFrustumMaxExtend.x ){
			pFrustumMaxExtend.x = points[ i ].x;
		}
		
		if( points[ i ].y < pFrustumMinExtend.y ){
			pFrustumMinExtend.y = points[ i ].y;
			
		}else if( points[ i ].y > pFrustumMaxExtend.y ){
			pFrustumMaxExtend.y = points[ i ].y;
		}
		
		if( points[ i ].z < pFrustumMinExtend.z ){
			pFrustumMinExtend.z = points[ i ].z;
			
		}else if( points[ i ].z > pFrustumMaxExtend.z ){
			pFrustumMaxExtend.z = points[ i ].z;
		}
	}
}



void deoglPlanVisitorCullElements::SetCullPixelSize( float cullPixelSize ){
	if( cullPixelSize < 0.1f ){
		pCullPixelSize = 0.1f;
		
	}else{
		pCullPixelSize = cullPixelSize;
	}
}

void deoglPlanVisitorCullElements::SetErrorScaling( float errorScaling ){
	if( errorScaling < 0.0f ){
		pErrorScaling = 0.0f;
		
	}else{
		pErrorScaling = errorScaling;
	}
}

void deoglPlanVisitorCullElements::CalculateErrorScaling(){
	const float fov = pPlan->GetCameraFov();
	const float fovRatio = pPlan->GetCameraFovRatio();
	const float scalingX = ( float )pPlan->GetViewportWidth() * 0.5f / pCullPixelSize / tanf( fov * 0.5f );
	const float scalingY = ( float )pPlan->GetViewportHeight() * 0.5f / pCullPixelSize / tanf( fov * fovRatio * 0.5f );
	
	if( scalingX > scalingY ){
		pErrorScaling = scalingX;
		
	}else{
		pErrorScaling = scalingY;
	}
}



void deoglPlanVisitorCullElements::SetCullDynamicComponents( bool cullDynamicComponents ){
	pCullDynamicComponents = cullDynamicComponents;
}



void deoglPlanVisitorCullElements::SetCullLayerMask( bool cull ){
	pCullLayerMask = cull;
}

void deoglPlanVisitorCullElements::SetLayerMask( const decLayerMask &layerMask ){
	pLayerMask = layerMask;
}



void deoglPlanVisitorCullElements::VisitWorldOctree( deoglWorldOctree &octree ){
	if( ! pFrustum ){
		DETHROW( deeInvalidParam );
	}
	
	//octree.VisitNodesColliding( this, pFrustumMinExtend, pFrustumMaxExtend );
	octree.VisitNodesCollidingVolume( this, pFrustum );
}



// Visiting
/////////////

void deoglPlanVisitorCullElements::VisitNode( deoglDOctree *node, int intersection ){
	bool cullWithVolume = ( intersection != deoglDCollisionDetection::eirInside );
	const deoglWorldOctree &sonode = *( ( deoglWorldOctree* )node );
	deoglOcclusionTest &occlusionTest = *pPlan->GetOcclusionTest();
	const decDVector &cameraPosition = pPlan->GetCameraPosition();
	deoglCollideList &collideList = pPlan->GetCollideList();
	deoglDCollisionBox box;
	int i, count;
	
	// test node against frustum
	/*
	box.SetCenter( node->GetCenter() );
	box.SetHalfSize( node->GetHalfSize() );
	if( ! pFrustum->BoxHitsFrustum( &box ) ){
		return;
	}
	intersection = deoglDCollisionDetection::eirPartial;
	cullWithVolume = true;
	*/
	
	
	// visit components
	count = sonode.GetComponentCount();
	
	for( i=0; i<count; i++ ){
		deoglRComponent * const component = sonode.GetComponentAt( i );
		
		const decDVector &minExtend = component->GetMinimumExtend();
		const decDVector &maxExtend = component->GetMaximumExtend();
		
		// cull using layer mask if required. components with empty layer mask never match
		// and thus are never culled
		if( pCullLayerMask && component->GetLayerMask().IsNotEmpty()
		&& pLayerMask.MatchesNot( component->GetLayerMask() ) ){
			continue;
		}
		
		// cull using cull volume if required
		if( cullWithVolume ){
			// possible optmizations:
			// - check against pFrustum*Extend before checking against the frustum volume
			// - depending on the size of the object use a sphere instead of a box
			// - maybe use the sphere for the test against the pFrustum*Extend ?
			
			box.SetFromExtends( minExtend, maxExtend );
			
			if( ! pFrustum->BoxHitsFrustum( &box ) ){
				continue;
			}
		}
		
		// cull using too small filter
		const decDVector center = ( minExtend + maxExtend ) * 0.5;
		const float radius = ( float )( ( maxExtend - minExtend ).Length() * 0.5 );
		const float componentDistance = ( float )( ( center - cameraPosition ) * pCameraView ) - radius;
		
		if( componentDistance > ( radius * component->GetLODErrorScaling() ) * pErrorScaling ){
			continue;
		}
		
		// cull dynamic if required
		//if( pCullDynamicComponents && ! component->GetStatic() ){
		if( pCullDynamicComponents && ! component->GetRenderStatic() ){
			continue;
		}
		
		// add component and add occlusion test input
		collideList.AddComponent( component )->StartOcclusionTest( occlusionTest, cameraPosition );
	}
	
	
	// visit billboards
	count = sonode.GetBillboardList().GetCount();
	
	for( i=0; i<count; i++ ){
		deoglRBillboard * const billboard = sonode.GetBillboardList().GetAt( i );
		
		// cull using layer mask if required. billboards with empty layer mask never match
		// and thus are never culled
		if( pCullLayerMask && billboard->GetLayerMask().IsNotEmpty()
		&& pLayerMask.MatchesNot( billboard->GetLayerMask() ) ){
			continue;
		}
		
		// cull using cull volume if required
		const decDVector &minExtend = billboard->GetMinimumExtend();
		const decDVector &maxExtend = billboard->GetMaximumExtend();
		
		if( cullWithVolume ){
			// possible optmizations:
			// - check against pFrustum*Extend before checking against the frustum volume
			// - depending on the size of the object use a sphere instead of a box
			// - maybe use the sphere for the test against the pFrustum*Extend ?
			
			box.SetFromExtends( minExtend, maxExtend );
			
			if( ! pFrustum->BoxHitsFrustum( &box ) ){
				continue;
			}
		}
		
		// cull using too small filter
		const decDVector center = ( minExtend + maxExtend ) * 0.5;
		const float radius = ( float )( ( maxExtend - minExtend ).Length() * 0.5 );
		const float billboardDistance = ( float )( ( center - cameraPosition ) * pCameraView ) - radius;
		
		if( billboardDistance > radius * pErrorScaling ){
			continue;
		}
		
		// add billboard
		collideList.AddBillboard( billboard );
	}
	
	
	// visit lights
	count = sonode.GetLightCount();
	
	if( intersection == deoglDCollisionDetection::eirInside ){
		for( i=0; i<count; i++ ){
			deoglRLight * const light = sonode.GetLightAt( i );
		
			if( pCullLayerMask && light->GetLayerMask().IsNotEmpty()
			&& pLayerMask.MatchesNot( light->GetLayerMask() ) ){
				continue;
			}
			
			collideList.AddLight( light )->StartOcclusionTest( occlusionTest, cameraPosition );
		}
		
	}else{
		for( i=0; i<count; i++ ){
			deoglRLight * const light = sonode.GetLightAt( i );
			
			if( pCullLayerMask && light->GetLayerMask().IsNotEmpty()
			&& pLayerMask.MatchesNot( light->GetLayerMask() ) ){
				continue;
			}
			
			if( ! light->GetCollisionVolume()->VolumeHitsVolume( pFrustum ) ){
				continue;
			}
			
			collideList.AddLight( light )->StartOcclusionTest( occlusionTest, cameraPosition );
		}
	}
	
	
	// visit particle emitters
	const deoglParticleEmitterInstanceList &nodeParticleEmitterInstanceList = sonode.GetParticleEmittersList();
	deoglParticleEmitterInstanceList &clistParticleEmitterInstanceList = collideList.GetParticleEmitterList();
	count = nodeParticleEmitterInstanceList.GetCount();
	
	if( intersection == deoglDCollisionDetection::eirInside ){
		for( i=0; i<count; i++ ){
			deoglRParticleEmitterInstance * const instance = nodeParticleEmitterInstanceList.GetAt( i );
		
			if( pCullLayerMask && instance->GetLayerMask().IsNotEmpty()
			&& pLayerMask.MatchesNot( instance->GetLayerMask() ) ){
				continue;
			}
			
			clistParticleEmitterInstanceList.Add( instance );
		}
		
	}else{
		for( i=0; i<count; i++ ){
			deoglRParticleEmitterInstance * const instance = nodeParticleEmitterInstanceList.GetAt( i );
			
			if( pCullLayerMask && instance->GetLayerMask().IsNotEmpty()
			&& pLayerMask.MatchesNot( instance->GetLayerMask() ) ){
				continue;
			}
			
			box.SetFromExtends( instance->GetMinExtend(), instance->GetMaxExtend() );
			if( pFrustum->BoxHitsFrustum( &box ) ){
				clistParticleEmitterInstanceList.Add( instance );
			}
		}
	}
}
