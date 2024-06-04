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

#include "deoglRPVisitorFindContent.h"
#include "../deoglRenderPlan.h"
#include "../../../collidelist/deoglCollideList.h"
#include "../../../collidelist/deoglCollideListComponent.h"
#include "../../../collidelist/deoglCollideListLight.h"
#include "../../../billboard/deoglRBillboard.h"
#include "../../../component/deoglRComponent.h"
#include "../../../light/deoglRLight.h"
#include "../../../occlusiontest/deoglOcclusionTest.h"
#include "../../../particle/deoglRParticleEmitterInstance.h"
#include "../../../world/deoglWorldOctree.h"
#include "../../../utils/collision/deoglDCollisionVolume.h"
#include "../../../utils/collision/deoglDCollisionSphere.h"
#include "../../../utils/collision/deoglDCollisionBox.h"
#include "../../../utils/collision/deoglDCollisionDetection.h"


#include <dragengine/common/exceptions.h>



// Class deoglRPVisitorFindContent
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRPVisitorFindContent::deoglRPVisitorFindContent( deoglRenderPlan &plan ) :
pPlan( plan ),
pFrustum( NULL ),
pCullPixelSize( 1.0f ),
pErrorScaling( 1.0f ),
pCullDynamicComponents( false ),
pCullLayerMask( false ),
pWithGICascade( false ){
}



// Management
///////////////

void deoglRPVisitorFindContent::Init( deoglDCollisionFrustum *frustum ){
	if( ! frustum ){
		DETHROW( deeInvalidParam );
	}
	
	pFrustum = frustum;
	pCameraView = pPlan.GetInverseCameraMatrix().TransformView();
	
	CalculateFrustumBoundaryBox();
	CalculateErrorScaling();
}



void deoglRPVisitorFindContent::CalculateFrustumBoundaryBox(){
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
	pFrustumMinExtend = pPlan.GetCameraPosition();
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



void deoglRPVisitorFindContent::SetCullPixelSize( float cullPixelSize ){
	if( cullPixelSize < 0.1f ){
		pCullPixelSize = 0.1f;
		
	}else{
		pCullPixelSize = cullPixelSize;
	}
}

void deoglRPVisitorFindContent::SetErrorScaling( float errorScaling ){
	if( errorScaling < 0.0f ){
		pErrorScaling = 0.0f;
		
	}else{
		pErrorScaling = errorScaling;
	}
}

void deoglRPVisitorFindContent::CalculateErrorScaling(){
	// aspectRatio = w / h
	// dx = tan(fov / 2) * (cullPixelSize / (w / 2))
	// dy = tan(fov * fovRatio / 2) * (cullPixelSize / (w / 2)) / aspectRatio
	// dz = 1
	// 
	// x = dx * z
	// y = dy * z
	
	const float fov = pPlan.GetCameraFov();
	const float fovRatio = pPlan.GetCameraFovRatio();
	const float halfWidth = ( float )pPlan.GetViewportWidth() * 0.5f;
	const float halfHeight = ( float )pPlan.GetViewportHeight() * 0.5f;
	const float aspectRatio = halfWidth / halfHeight;
	const float dx = tanf( fov * 0.5f ) * ( pCullPixelSize / halfWidth );
	const float dy = tanf( fov * 0.5f * fovRatio ) * ( pCullPixelSize / halfWidth ) / aspectRatio;
	
	pErrorScaling = decMath::min( dx, dy );
}



void deoglRPVisitorFindContent::SetCullDynamicComponents( bool cullDynamicComponents ){
	pCullDynamicComponents = cullDynamicComponents;
}



void deoglRPVisitorFindContent::SetCullLayerMask( bool cull ){
	pCullLayerMask = cull;
}

void deoglRPVisitorFindContent::SetLayerMask( const decLayerMask &layerMask ){
	pLayerMask = layerMask;
}



void deoglRPVisitorFindContent::EnableGIBox( const decDVector & minExtend, const decDVector & maxExtend ){
	pWithGICascade = true;
	pGICascadeMinExtend = minExtend;
	pGICascadeMaxExtend = maxExtend;
	pGICascadeBox.SetFromExtends( minExtend, maxExtend );
}

void deoglRPVisitorFindContent::DisableGIBox(){
	pWithGICascade = false;
}



void deoglRPVisitorFindContent::VisitWorldOctree( const deoglWorldOctree &octree ){
	if( ! pFrustum ){
		DETHROW( deeInvalidParam );
	}
	
	pVisitNode( octree, true );
}



// Private Functions
//////////////////////

void deoglRPVisitorFindContent::pVisitNode( const deoglWorldOctree &node, bool intersect ){
	pVisitComponents( node, intersect );
	pVisitBillboards( node, intersect );
	pVisitLights( node, intersect );
	pVisitParticleEmitters( node, intersect );
	
	int i;
	for( i=0; i<8; i++ ){
		const deoglWorldOctree * const child = ( const deoglWorldOctree* ) node.GetNodeAt( i );
		if( ! child ){
			continue;
		}
		
		if( ! intersect ){
			pVisitNode( *child, false );
			continue;
		}
		
		const decDVector &childMin = child->GetMinimumExtend();
		const decDVector &childMax = child->GetMaximumExtend();
		switch( pFrustum->BoxIntersect( childMin, childMax ) ){
		case deoglDCollisionFrustum::eitInside:
			pVisitNode( *child, false );
			break;
			
		case deoglDCollisionFrustum::eitIntersect:
			pVisitNode( *child, true );
			break;
			
		case deoglDCollisionFrustum::eitOutside:
			if( pWithGICascade ){
				switch( deoglDCollisionDetection::AABoxIntersectsAABox(
					pGICascadeMinExtend, pGICascadeMaxExtend, childMin, childMax ) ){
				case deoglDCollisionDetection::eirInside:
					pVisitNodeGICascade( *child, false );
					break;
					
				case deoglDCollisionDetection::eirPartial:
					pVisitNodeGICascade( *child, true );
					break;
					
				case deoglDCollisionDetection::eirOutside:
					break;
				}
			}
			break;
		}
	}
}

void deoglRPVisitorFindContent::pVisitNodeGICascade( const deoglWorldOctree &node, bool intersect ){
	pVisitLightsGICascade( node, intersect );
	
	int i;
	for( i=0; i<8; i++ ){
		const deoglWorldOctree * const child = ( const deoglWorldOctree* ) node.GetNodeAt( i );
		if( ! child ){
			continue;
		}
		
		if( ! intersect ){
			pVisitNodeGICascade( *child, false );
			continue;
		}
		
		switch( deoglDCollisionDetection::AABoxIntersectsAABox( pGICascadeMinExtend,
			pGICascadeMaxExtend, child->GetMinimumExtend(), child->GetMaximumExtend() ) ){
		case deoglDCollisionDetection::eirInside:
			pVisitNodeGICascade( *child, false );
			break;
			
		case deoglDCollisionDetection::eirPartial:
			pVisitNodeGICascade( *child, true );
			break;
			
		case deoglDCollisionDetection::eirOutside:
			break;
		}
	}
}

void deoglRPVisitorFindContent::pVisitComponents( const deoglWorldOctree &node, bool intersect ){
	deoglOcclusionTest &occlusionTest = *pPlan.GetOcclusionTest();
	const decDVector &cameraPosition = pPlan.GetCameraPosition();
	deoglComponentList &componentsOccMap = pPlan.GetComponentsOccMap();
	deoglCollideList &collideList = pPlan.GetCollideList();
	const int count = node.GetComponentCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglRComponent * const addComponent = node.GetComponentAt( i );
		const deoglRComponent &component = *addComponent;
		
		const decDVector &minExtend = component.GetMinimumExtend();
		const decDVector &maxExtend = component.GetMaximumExtend();
		
		// cull using layer mask if required. components with empty layer mask never match
		// and thus are never culled
		if( pCullLayerMask && component.GetLayerMask().IsNotEmpty()
		&& pLayerMask.MatchesNot( component.GetLayerMask() ) ){
			continue;
		}
		
		// cull using cull volume if required
		if( intersect ){
			// possible optmizations:
			// - check against pFrustum*Extend before checking against the frustum volume
			// - depending on the size of the object use a sphere instead of a box
			// - maybe use the sphere for the test against the pFrustum*Extend ?
			if( ! pFrustum->BoxHits( minExtend, maxExtend ) ){
				continue;
			}
		}
		
		// cull using too small filter
		const decDVector center( ( minExtend + maxExtend ) * 0.5 );
		const float radius = ( float )( ( maxExtend - minExtend ).Length() * 0.5 );
		const float componentDistance = ( float )( ( center - cameraPosition ) * pCameraView ) - radius;
		
		if( radius < componentDistance * pErrorScaling ){
			continue;
		}
		
		// cull dynamic if required
		//if( pCullDynamicComponents && ! component->GetStatic() ){
		if( pCullDynamicComponents && ! component.GetRenderStatic() ){
			continue;
		}
		
		// add component and add occlusion test input
		collideList.AddComponent( addComponent )->StartOcclusionTest( occlusionTest, cameraPosition );
		
		// add to occlusion map if required
		if( addComponent->GetOcclusionMesh() ){
			componentsOccMap.Add( addComponent );
		}
	}
}

void deoglRPVisitorFindContent::pVisitBillboards( const deoglWorldOctree &node, bool intersect ){
	const decDVector &cameraPosition = pPlan.GetCameraPosition();
	deoglCollideList &collideList = pPlan.GetCollideList();
	const int count = node.GetBillboardList().GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglRBillboard * const addBillboard = node.GetBillboardList().GetAt( i );
		const deoglRBillboard &billboard = *addBillboard;
		
		// cull using layer mask if required. billboards with empty layer mask never match
		// and thus are never culled
		if( pCullLayerMask && billboard.GetLayerMask().IsNotEmpty()
		&& pLayerMask.MatchesNot( billboard.GetLayerMask() ) ){
			continue;
		}
		
		// cull using cull volume if required
		const decDVector &minExtend = billboard.GetMinimumExtend();
		const decDVector &maxExtend = billboard.GetMaximumExtend();
		
		if( intersect ){
			// possible optmizations:
			// - check against pFrustum*Extend before checking against the frustum volume
			// - depending on the size of the object use a sphere instead of a box
			// - maybe use the sphere for the test against the pFrustum*Extend ?
			if( ! pFrustum->BoxHits( minExtend, maxExtend ) ){
				continue;
			}
		}
		
		// cull using too small filter
		const decDVector center = ( minExtend + maxExtend ) * 0.5;
		const float radius = ( float )( ( maxExtend - minExtend ).Length() * 0.5 );
		const float billboardDistance = ( float )( ( center - cameraPosition ) * pCameraView ) - radius;
		
		if( radius < billboardDistance * pErrorScaling ){
			continue;
		}
		
		// add billboard
		collideList.AddBillboard( addBillboard );
	}
}

void deoglRPVisitorFindContent::pVisitLights( const deoglWorldOctree &node, bool intersect ){
	deoglOcclusionTest &occlusionTest = *pPlan.GetOcclusionTest();
	const decDVector &cameraPosition = pPlan.GetCameraPosition();
	deoglCollideList &collideList = pPlan.GetCollideList();
	const int count = node.GetLightCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglRLight * const addLight = node.GetLightAt( i );
		const deoglRLight &light = *addLight;
		
		if( pCullLayerMask && light.GetLayerMask().IsNotEmpty()
		&& pLayerMask.MatchesNot( light.GetLayerMask() ) ){
			continue;
		}
		
		if( intersect && ! light.GetCollisionVolume()->FrustumHitsVolume( pFrustum ) ){
			if( pWithGICascade && light.GetCollisionVolume()->BoxHitsVolume( &pGICascadeBox ) ){
				collideList.AddLight( addLight )->SetCulled( true );
			}
			continue;
		}
		
		collideList.AddLight( addLight )->StartOcclusionTest( occlusionTest, cameraPosition );
	}
}

void deoglRPVisitorFindContent::pVisitLightsGICascade( const deoglWorldOctree& node, bool intersect ){
	deoglCollideList &collideList = pPlan.GetCollideList();
	const int count = node.GetLightCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglRLight * const addLight = node.GetLightAt( i );
		const deoglRLight &light = *addLight;
		
		if( pCullLayerMask && light.GetLayerMask().IsNotEmpty()
		&& pLayerMask.MatchesNot( light.GetLayerMask() ) ){
			continue;
		}
		
		if( intersect && ! light.GetCollisionVolume()->BoxHitsVolume( &pGICascadeBox ) ){
			continue;
		}
		
		collideList.AddLight( addLight )->SetCulled( true );
	}
}

void deoglRPVisitorFindContent::pVisitParticleEmitters( const deoglWorldOctree &node, bool intersect ){
	const deoglParticleEmitterInstanceList &nodeParticleEmitterInstanceList = node.GetParticleEmittersList();
	deoglCollideList &collideList = pPlan.GetCollideList();
	deoglParticleEmitterInstanceList &clistParticleEmitterInstanceList = collideList.GetParticleEmitterList();
	const int count = nodeParticleEmitterInstanceList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglRParticleEmitterInstance * const addInstance = nodeParticleEmitterInstanceList.GetAt( i );
		const deoglRParticleEmitterInstance &instance = *addInstance;
		
		if( pCullLayerMask && instance.GetLayerMask().IsNotEmpty()
		&& pLayerMask.MatchesNot( instance.GetLayerMask() ) ){
			continue;
		}
		
		if( intersect && ! pFrustum->BoxHits( instance.GetMinExtend(), instance.GetMaxExtend() ) ){
			continue;
		}
		
		clistParticleEmitterInstanceList.Add( addInstance );
	}
}
