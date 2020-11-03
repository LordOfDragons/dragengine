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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglRLight.h"
#include "deoglLightTestForTouch.h"
#include "deoglLightGatherOcclusionMeshes.h"
#include "volume/deoglLightVolume.h"
#include "volume/deoglLightVolumeBuilder.h"
#include "shader/deoglLightShader.h"
#include "shader/deoglLightShaderManager.h"
#include "deoglNotifyEnvMapLightChanged.h"
#include "../canvas/render/deoglRCanvasView.h"
#include "../collidelist/deoglCollideListComponent.h"
#include "../collidelist/deoglCollideList.h"
#include "../component/deoglRComponent.h"
#include "../configuration/deoglConfiguration.h"
#include "../delayedoperation/deoglDelayedDeletion.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../occlusiontest/deoglOcclusionTest.h"
#include "../occlusiontest/mesh/deoglROcclusionMesh.h"
#include "../occquery/deoglOcclusionQuery.h"
#include "../optimizer/deoglOptimizerLight.h"
#include "../optimizer/deoglOptimizerManager.h"
#include "../rendering/plan/deoglRenderPlan.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTShader.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../shadow/deoglSCSolid.h"
#include "../shadow/deoglSCTransparent.h"
#include "../shadow/deoglShadowCaster.h"
#include "../skin/deoglRSkin.h"
#include "../skin/deoglSkinTexture.h"
#include "../skin/deoglSkinRenderable.h"
#include "../skin/channel/deoglSkinChannel.h"
#include "../skin/dynamic/deoglRDynamicSkin.h"
#include "../skin/state/deoglSkinState.h"
#include "../skin/state/deoglSkinStateRenderable.h"
#include "../triangles/deoglTriangleSorter.h"
#include "../visitors/deoglTransformVolume.h"
#include "../world/deoglRWorld.h"
#include "../world/deoglRWorld.h"
#include "../world/deoglWorldOctree.h"

#include <dragengine/common/exceptions.h>
#include "../utils/collision/deoglDCollisionDetection.h"
#include "../utils/collision/deoglDCollisionBox.h"
#include "../utils/collision/deoglDCollisionSphere.h"
#include "../utils/collision/deoglDCollisionTriangle.h"
#include "../utils/collision/deoglDCollisionVolume.h"
#include <dragengine/common/math/decConvexVolume.h>
#include <dragengine/common/math/decConvexVolumeFace.h>
#include <dragengine/common/math/decConvexVolumeList.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/light/deLight.h>
#include <dragengine/resources/world/deWorld.h>



#include <dragengine/common/utils/decTimer.h>
#include <dragengine/common/shape/decShapeBox.h>
static decTimer timer;
#define DEBUG_RESET_TIMERS				timer.Reset();
#define DEBUG_PRINT_TIMER(what)			pOgl->LogInfoFormat( "%s = %iys", what, ( int )( timer.GetElapsedTime() * 1000000.0 ) )



// Class deoglRLight
/////////////////////

// Constructor, destructor
////////////////////////////

deoglRLight::deoglRLight( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),

pParentWorld( NULL ),
pOctreeNode( NULL ),

pActive( false ),
pLightType( deLight::eltPoint ),
pCastShadows( false ),
pSpotAngle( DEG2RAD * 30.0f ),
pSpotRatio( 1.0f ),
pSpotSmoothness( 1.0f ),
pSpotExponent( 1.0f ),
pHintMovement( deLight::emhStationary ),
pHintShadowImportance( 100 ),
pIntensity( 0.0f ),
pAmbientRatio( 0.0f ),
pColor( 1.0f, 1.0f, 1.0f ),
pLightSkin( NULL ),
pLightCanvas( NULL ),
pDynamicSkin( NULL ),
pDirtyRenderables( false ),
pSkinState( NULL ),
pUseSkinTexture( NULL ),

pWorldMarkedRemove( false )
{
	int i;
	
	pDirtyColVol = true;
	pDirtyConvexVolumeList = true;
	pDirtyFullExtends = true;
	pDirtyExtends = true;
	pConvexVolumeList = NULL;
	pOptimizer = NULL;
	pColVol = NULL;
	pShadowCaster = NULL;
	pDirtyStaticShadows = true;
	pDirtyDynamicShadows = true;
	pDirtyCollideLists = true;
	pStaticCollideList = NULL;
	pDynamicCollideList = NULL;
	pVisible = true;
	
	pLightVolume = NULL;
	pLightVolumeCropBox = NULL;
	
	pOcclusionQuery = NULL;
	
	pDirtyTouching = true;
	pMarked = false;
	pInsideCamera = false;
	pUpdateOnRemoveComponent = true;
	
	for( i=0; i<EST_COUNT; i++ ){
		pShaders[ i ] = NULL;
	}
	pParamBlockLight = NULL;
	pParamBlockInstance = NULL;
	
	try{
		pConvexVolumeList = new decConvexVolumeList;
		pShadowCaster = new deoglShadowCaster( renderThread );
		pLightVolume = new deoglLightVolume( renderThread );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	LEAK_CHECK_CREATE( renderThread, Light );
}

deoglRLight::~deoglRLight(){
	LEAK_CHECK_FREE( pRenderThread, Light );
	pCleanUp();
}



// Management
///////////////

void deoglRLight::SetParentWorld( deoglRWorld *parentWorld ){
	if( parentWorld == pParentWorld ){
		return;
	}
	
	pParentWorld = parentWorld;
	
	RemoveAllComponents();
	
	if( pOctreeNode ){
		pOctreeNode->RemoveLight( this );
		pOctreeNode = NULL;
	}
	
	pDirtyTouching = true;
}



void deoglRLight::SetOctreeNode( deoglWorldOctree *octreeNode ){
	pOctreeNode = octreeNode;
}

void deoglRLight::UpdateOctreeNode(){
	if( ! pParentWorld ){
		return;
	}
	
	if( pActive ){
		pParentWorld->GetOctree().InsertLightIntoTree( this, 8 );
		
	}else{
		if( pOctreeNode ){
			pOctreeNode->RemoveLight( this );
			pOctreeNode = NULL;
		}
	}
}

void deoglRLight::UpdateSkin( float elapsed ){
	if( pSkinState ){
		pSkinState->AdvanceTime( elapsed );
	}
}



void deoglRLight::SetActive( bool active ){
	pActive = active;
}

void deoglRLight::SetLightType( deLight::eLightTypes type ){
	pLightType = type;
}

void deoglRLight::SetCastShadows( bool castShadows ){
	pCastShadows = castShadows;
}

void deoglRLight::SetSpotAngle( float angle ){
	pSpotAngle = angle;
}

void deoglRLight::SetSpotRatio( float ratio ){
	pSpotRatio = ratio;
}

void deoglRLight::SetSpotSmoothness( float smoothness ){
	pSpotSmoothness = smoothness;
}

void deoglRLight::SetSpotExponent( float exponent ){
	pSpotExponent = exponent;
}

void deoglRLight::SetHintMovement( deLight::eMovementHints movement ){
	pHintMovement = movement;
}

void deoglRLight::SetHintShadowImportance( int importance ){
	pHintShadowImportance = importance;
}

void deoglRLight::SetIntensity( float intensity ){
	pIntensity = intensity;
}

void deoglRLight::SetAmbientRatio( float ratio ){
	pAmbientRatio = ratio;
}



void deoglRLight::SetLightSkin( deoglRSkin *skin ){
	if( skin == pLightSkin ){
		return;
	}
	
	pUseSkinTexture = NULL;
	
	if( pLightSkin ){
		pLightSkin->FreeReference();
	}
	
	pLightSkin = skin;
	
	if( ! skin ){
		return;
	}
	
	skin->AddReference();
	
	if( skin->GetTextureCount() > 0 ){
		pUseSkinTexture = &skin->GetTextureAt( 0 );
	}
	
	if( ! pSkinState ){
		pSkinState = new deoglSkinState( pRenderThread, *this );
	}
}

void deoglRLight::SetLightCanvas( deoglRCanvasView *canvas ){
	if( canvas == pLightCanvas ){
		return;
	}
	
	if( pLightCanvas ){
		pLightCanvas->FreeReference();
	}
	
	pLightCanvas = canvas;
	
	if( canvas ){
		canvas->AddReference();
	}
}

void deoglRLight::SetDynamicSkin( deoglRDynamicSkin *dynamicSkin ){
	if( dynamicSkin == pDynamicSkin ){
		return;
	}
	
	if( pDynamicSkin ){
		pDynamicSkin->FreeReference();
	}
	
	pDynamicSkin = dynamicSkin;
	
	if( dynamicSkin ){
		dynamicSkin->AddReference();
	}
}

void deoglRLight::SetTransform( const decTexMatrix2 &matrix ){
	pTransform = matrix;
}

void deoglRLight::SetRenderablesDirty(){
	pDirtyRenderables = true;
}



void deoglRLight::SetColor( const decColor &color ){
	pColor = color;
}

void deoglRLight::SetLayerMask( const decLayerMask &layerMask ){
	pLayerMask = layerMask;
}

void deoglRLight::SetLayerMaskShadow( const decLayerMask &layerMask ){
	pLayerMaskShadow = layerMask;
}

void deoglRLight::RemoveAllShadowIgnoreComponents(){
	if( pShadowIgnoreComponents.GetCount() == 0 ){
		return;
	}
	
	const decObjectSet components( pShadowIgnoreComponents );
	
	pShadowIgnoreComponents.RemoveAll();
	
	// test all components since they can now be potentially in shadow maps
	const int count = components.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		TestComponent( ( deoglRComponent* )components.GetAt( i ) );
	}
}

void deoglRLight::AddShadowIgnoreComponent( deoglRComponent *component ){
	if( ! component ){
		DETHROW( deeInvalidParam );
	}
	
	pShadowIgnoreComponents.Add( component );
	
	// remove component in case it is enlisted for the static or dynamic shadow map
	RemoveComponent( component );
}

bool deoglRLight::HasShadowIgnoreComponent( deoglRComponent *component ) const{
	return pShadowIgnoreComponents.Has( component );
}



void deoglRLight::SetMatrix( const decDMatrix &matrix ){
	pMatrix = matrix;
	pInverseMatrix = matrix.QuickInvert();
}



void deoglRLight::UpdateConvexVolumeList(){
	UpdateLightVolume();
}

bool deoglRLight::HasExtends() const{
	/*if( pLight->GetType() == deLight::eltDirectional ){
		return false;
	}*/
	return true;
}

const decDVector &deoglRLight::GetFullMinExtend(){
	pUpdateFullExtends();
	return pFullMinExtend;
}

const decDVector &deoglRLight::GetFullMaxExtend(){
	pUpdateFullExtends();
	return pFullMaxExtend;
}

void deoglRLight::SetDirtyFullExtends(){
	pDirtyFullExtends = true;
}

const decDVector &deoglRLight::GetMinimumExtend(){
	pUpdateExtends();
	return pMinExtend;
}

const decDVector &deoglRLight::GetMaximumExtend(){
	pUpdateExtends();
	return pMaxExtend;
}


void deoglRLight::SetDirtyExtends(){
	pDirtyExtends = true;
}



deoglDCollisionVolume *deoglRLight::GetCollisionVolume(){
	if( ! pDirtyColVol ){
		return pColVol;
	}
	
	const decDVector lightPosition( pMatrix.GetPosition() );
	deoglDCollisionVolume *newColVol = NULL;
	
	try{
		// if there exists no light volume yet create a default collision volume
		if( pConvexVolumeList->GetVolumeCount() == 0 ){
			switch( pLightType ){
			case deLight::eltPoint:
				// create a sphere enclosing the point light
				newColVol = new deoglDCollisionSphere( lightPosition, pRange );
				break;
				
			default:
				// create a sphere around the spot light. a frustum would be the best
				// solution but currently the frustum is not able to test for collisions
				// with another frustum
				newColVol = new deoglDCollisionSphere( lightPosition, pRange );
			}
			
		// otherwise calculate a box around the light volume list
		}else{
			pUpdateExtends();
			const decDVector halfSize( ( pMaxExtend - pMinExtend ) * 0.5f );
			newColVol = new deoglDCollisionBox( pMinExtend + halfSize, halfSize );
		}
		
		// replace old collision volumes
		if( pColVol ){
			delete pColVol;
		}
		pColVol = newColVol;
		
	}catch( const deException & ){
		if( newColVol ){
			delete newColVol;
		}
		throw;
	}
	
	pDirtyColVol = false;
	
	return pColVol;
}

void deoglRLight::SetDirtyCollisionVolume(){
	pDirtyColVol = true;
}



void deoglRLight::TestCameraInside( deoglRenderPlan &plan ){
	const float safetyMargin = 0.01; // 1cm should be enough to be safe
	const float imageDistance = plan.GetCameraImageDistance();
	const float nx = imageDistance * tanf( plan.GetCameraFov() * 0.5f );
	const float ny = imageDistance * tanf( plan.GetCameraFov() * plan.GetCameraFovRatio() * 0.5f );
	const float nd = sqrtf( nx * nx + ny * ny + imageDistance * imageDistance ) + safetyMargin;
	const decDVector &cameraPosition = plan.GetCameraPosition();
	const decDVector extendOffset( nd, nd, nd );
	
	// by default the camera is not inside unless some other evidence is found
	pCameraInside = false;
	
	// if the camera is not inside the extends box then there is no chance to inside the light volumes at all.
	// to avoid flickering the box is slightly enlarged as the camera near plane has a certain distance from
	// the position itself
	pUpdateExtends();
	
	pCameraInside = ( cameraPosition >= pMinExtend - extendOffset )
		&& ( cameraPosition <= pMaxExtend + extendOffset );
	
	if( ! pCameraInside ){
		return;
	}
	
	// test against all convex volumes. to get a safe testing we can not simply test the camera point since
	// the camera clips already into the light volume at the near clipping plane. this can be achieved by
	// not just testing on what side the point is on a plane but adding a small offset. a too large offset
	// only causes a full screen render that might not have been necessary but is never wrong
	const decVector localCameraPosition = ( pInverseMatrix * cameraPosition ).ToVector();
	const int volumeCount = pConvexVolumeList->GetVolumeCount();
	bool insideVolume;
	int i, j;
	
	// this test is not working correctly. the correct test requires finding for each volume
	// the point inside the volume that is closes to the camera point. this has to be done
	// by clamping the point against all faces (keep normal*(cam-facepoint) <= 0). then
	// check if the distance from the camera point to this closest point is at most nd.
	// using squared check removes the need for a square root.
	
	pCameraInside = false;
	
	for( i=0; i<volumeCount; i++ ){
		const decConvexVolume &convexVolume = *pConvexVolumeList->GetVolumeAt( i );
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



const deoglCollideList *deoglRLight::GetStaticCollideList(){
	pUpdateCollideLists();
	return pStaticCollideList;
}

const deoglCollideList *deoglRLight::GetDynamicCollideList(){
	pUpdateCollideLists();
	return pDynamicCollideList;
}

void deoglRLight::SetDirtyCollideLists(){
	pDirtyCollideLists = true;
}



void deoglRLight::SetLightVolumeDirty(){
	pDirtyConvexVolumeList = true; // HACK until the problem is solved
	
	if( pOptimizer ){
		pOptimizer->ResetAllOptimizations();
	}else{
		// we have to create one... TODO
	}
}

void deoglRLight::UpdateLightVolume(){
	if( ! pDirtyConvexVolumeList ){
		return;
	}
	
	deoglLightVolumeBuilder builder;
	
	// cropping produces unfortunately a concave volume which we can't use. to solve this
	// a convex hull has to be calculated from the volume points. this is correctly done
	// using a gift-wrap algorithm. for the time being though we simply calculate an axis
	// aligned bounding box and clip the volume against it. this produces a result that
	// is not as optimal as the gift-wrap but it's better than nothing for the time being
//	decDVector boundingBoxMinExtend, boundingBoxMaxExtend;
	
//	builder.GetTransformedVolumeExtends( *pConvexVolumeList, pMatrix, boundingBoxMinExtend, boundingBoxMaxExtend );
	
	switch( pLightType ){
	case deLight::eltPoint:
		builder.BuildSphere( *pConvexVolumeList, decVector(), pRange );
		break;
		
	case deLight::eltSpot:
		builder.BuildCone( *pConvexVolumeList, decMatrix(), pRange,
			pSpotAngle * 0.5f, pSpotAngle * pSpotRatio * 0.5f, 12 );
		break;
		
	case deLight::eltProjector:
		builder.BuildFrustum( *pConvexVolumeList, decMatrix(), pRange,
			pSpotAngle * 0.5f, pSpotAngle * pSpotRatio * 0.5f );
		break;
	}
	
	if( pLightVolumeCropBox ){
		const decDVector minExtend( pLightVolumeCropBox->GetPosition()
			- pLightVolumeCropBox->GetHalfExtends() );
		const decDVector maxExtend( pLightVolumeCropBox->GetPosition()
			+ pLightVolumeCropBox->GetHalfExtends() );
		
		builder.CropByBoundingBox( *pConvexVolumeList,
			pInverseMatrix.GetRotationMatrix(), minExtend, maxExtend );
	}
	
//	builder.CropByBoundingBox( *pConvexVolumeList, invLightMatrix, boundingBoxMinExtend, boundingBoxMaxExtend );
	
	// optimizer stuff
#if 0
	if( ! pOptimizer ){
		try{
			pOptimizer = new deoglOptimizerLight( this, oglWorld );
			if( ! pOptimizer ) DETHROW( deeOutOfMemory );
			
			if( lightType == deLight::eltPoint ){
				pOptimizer->SetOptimizeShadowCaster( false );
			}else{
				pOptimizer->SetOptimizeShadowCaster( true );
			}
			
			if( pLight->GetHintMovement() == deLight::emhStationary ){
				pOptimizer->SetInitialWarmUpTime( 100 );
				if( lightType == deLight::eltPoint ){
					pOptimizer->SetMaximalVolumeCount( 200 );
				}else{
					pOptimizer->SetMaximalVolumeCount( 50 );
				}
				
			}else{
				pOptimizer->SetInitialWarmUpTime( 500 );
				if( lightType == deLight::eltPoint ){
					pOptimizer->SetMaximalVolumeCount( 100 );
				}else{
					pOptimizer->SetMaximalVolumeCount( 25 );
				}
			}
			
			pOptimizer->ResetAllOptimizations();
			
			pOgl->GetOptimizerManager()->AddOptimizer( pOptimizer );
			
		}catch( const deException & ){
			if( pOptimizer ) delete pOptimizer;
			throw;
		}
	}
#endif
	
	// sanity check. if there is not a single volume in the list the chance is high the light volume
	// becomes concave. we only write a warning here for the case we experiment during debugging
	if( pConvexVolumeList->GetVolumeCount() > 1 ){
		const decDVector position( pMatrix.GetPosition() );
		pRenderThread.GetLogger().LogWarnFormat( "Light at (%g,%g,%g) has a light volume with %i volumes (potentially concave)",
			position.x, position.y, position.z, pConvexVolumeList->GetVolumeCount() );
	}
	
	// rebuild the light volume using the convex volume list
	pLightVolume->CreateFrom( *pConvexVolumeList );
	
	// extends are usually dirty now
	pDirtyExtends = true;
	pDirtyCollideLists = true;
	pDirtyColVol = true;
	pDirtyTouching = true;
	
	// no more dirty
	pDirtyConvexVolumeList = false;
}



void deoglRLight::SetLightVolumeCropBox( decShapeBox *box ){
	if( box == pLightVolumeCropBox ){
		return;
	}
	
	if( pLightVolumeCropBox ){
		delete pLightVolumeCropBox;
	}
	
	pLightVolumeCropBox = box;
	
	SetLightVolumeDirty();
}



void deoglRLight::SetInsideCamera( bool inside ){
	pInsideCamera = inside;
}



deoglShadowCaster *deoglRLight::GetShadowCaster(){
	if( pDirtyStaticShadows || pDirtyDynamicShadows ){
		if( pCastShadows ){
			if( pHintMovement == deLight::emhStationary ){
				if( pHintShadowImportance >= 75 ){
					if( pDynamicComponentList.GetCount() > 0 ){
						if( pStaticComponentList.GetCount() > 0 ){
							pShadowCaster->SetShadowType( deoglShadowCaster::estStaticAndDynamic );
							
						}else{
							pShadowCaster->SetShadowType( deoglShadowCaster::estDynamicOnly );
						}
						
					}else if( pStaticComponentList.GetCount() > 0 ){
						pShadowCaster->SetShadowType( deoglShadowCaster::estStaticOnly );
						
					}else{
						pShadowCaster->SetShadowType( deoglShadowCaster::estNoShadows );
					}
					
				}else{
					if( pStaticComponentList.GetCount() > 0 ){
						pShadowCaster->SetShadowType( deoglShadowCaster::estStaticOnly );
						
					}else{
						pShadowCaster->SetShadowType( deoglShadowCaster::estNoShadows );
					}
				}
				
			}else{
				if( pDynamicComponentList.GetCount() > 0 || pStaticComponentList.GetCount() > 0 ){
					pShadowCaster->SetShadowType( deoglShadowCaster::estDynamicOnly );
					
				}else{
					pShadowCaster->SetShadowType( deoglShadowCaster::estNoShadows );
				}
			}
			
		}else{
			pShadowCaster->SetShadowType( deoglShadowCaster::estNoShadows );
		}
	}
	
	if( pDirtyStaticShadows ){
		pShadowCaster->Clear();
		pShadowCaster->SetStaticParams( 0.01f, pRange );
		pDirtyStaticShadows = false;
	}
	
	if( pDirtyDynamicShadows ){
		pShadowCaster->SetDynamicParams( 0.01f, pRange );
		pDirtyDynamicShadows = false;
	}
	
	return pShadowCaster;
}

void deoglRLight::SetDirtyShadows(){
	pDirtyStaticShadows = true;
	pDirtyDynamicShadows = true;
}

void deoglRLight::SetShadowParameters( const decVector &shadowOrigin, float shadowGap ){
	pShadowCaster->SetShadowOrigin( shadowOrigin );
	pShadowCaster->SetStaticCutOff( -shadowGap );
	pShadowCaster->SetDynamicCutOff( -shadowGap );
}



void deoglRLight::UpdateAttenuation( float range, float halfIntensityDistance ){
	if( range < 0.001f ){
		range = 0.001f;
	}
	
	pAttenCoeff = ( 1.0f / ( halfIntensityDistance * halfIntensityDistance ) - 2.0f ) / ( range * range );
	pRange = range;
	
	const float rangeNormInt = 1.0f / ( 1.0f + pAttenCoeff * range * range );
	
	pDampCoeff = 1.0f / ( 1.0f - rangeNormInt );
	pDampThreshold = -rangeNormInt * pDampCoeff;
}



void deoglRLight::ClearOptimizer(){
	pOptimizer = NULL;
}



void deoglRLight::PrepareForRender( deoglRenderPlan &plan ){
	/*if( pLightSkin ){
		pLightSkin->PrepareForRender();
	}*/
	if( pLightCanvas ){
		pLightCanvas->PrepareForRender();
	}
	
	pUpdateRenderables();
	pCheckTouching();
	
	pShadowCaster->Update();
	
//DEBUG_RESET_TIMERS;
	bool castShadows = pCastShadows; // && plan.GetDisableLights(); // disabled while debugging
	
	UpdateConvexVolumeList();
//DEBUG_PRINT_TIMER( "UpdateConvexVolumeList" );
	
	// updating components is only required if we cast shadows
	if( castShadows ){
		int c, componentCount;
		
//DEBUG_RESET_TIMERS;
		// update renderables of shadow casting components. this does not
		// update the vbo as this has to be done when lights are really
		// visible.
		componentCount = pStaticComponentList.GetCount();
		for( c=0; c<componentCount; c++ ){
			pStaticComponentList.GetAt( c )->UpdateRenderables( plan );
		}
		
		componentCount = pDynamicComponentList.GetCount();
		for( c=0; c<componentCount; c++ ){
			pDynamicComponentList.GetAt( c )->UpdateRenderables( plan );
		}
//DEBUG_PRINT_TIMER( "PrerenderComponents" );
	}
	
	// make sure the shadow caster is cleared of off dyamic shadow maps
	//pShadowCaster->GetSolid().DropDynamic();
	
	// update light volume vbo if existing
	if( pLightVolume ){
		pLightVolume->UpdateVBO();
	}
}

void deoglRLight::PrepareForShadowCasting( deoglRenderPlan &plan ){
	int c, componentCount;
	
	componentCount = pStaticComponentList.GetCount();
	for( c=0; c<componentCount; c++ ){
		deoglRComponent &component = *pStaticComponentList.GetAt( c );
		component.UpdateVBO();
		component.UpdateRenderables( plan );
	}
	
	componentCount = pDynamicComponentList.GetCount();
	for( c=0; c<componentCount; c++ ){
		deoglRComponent &component = *pDynamicComponentList.GetAt( c );
		component.UpdateVBO();
		component.UpdateRenderables( plan );
	}
}



deoglLightShader *deoglRLight::GetShaderFor( deoglRLight::eShaderTypes shaderType ){
	if( shaderType < 0 || shaderType >= EST_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! pShaders[ shaderType ] ){
		deoglLightShaderConfig config;
		
		if( GetShaderConfigFor( shaderType, config ) ){
			//#ifdef OS_ANDROID
			//	decString debugString;
			//	config.DebugGetConfigString(debugString);
			//	pRenderThread.GetLogger().LogInfoFormat("GetShaderFor(%p): IN %s", this, debugString.GetString());
			//#endif
			pShaders[ shaderType ] = pRenderThread.GetShader().GetLightShaderManager().GetShaderWith( config );
			//#ifdef OS_ANDROID
			//	pRenderThread.GetLogger().LogInfoFormat("GetShaderFor(%p): OUT %p", this, pShaders[shaderType]);
			//#endif
		}
	}
	
	return pShaders[ shaderType ];
}

bool deoglRLight::GetShaderConfigFor( deoglRLight::eShaderTypes shaderType,
deoglLightShaderConfig &config ){
	const deoglConfiguration &oglconfig = pRenderThread.GetConfiguration();
	
	config.Reset();
	
	config.SetMaterialNormalMode( deoglLightShaderConfig::emnmIntBasic );
	
	switch( shaderType ){
	case estLumSolid1:
	case estLumSolid1NoAmbient:
	case estLumSolid2:
		break;
		
	default:
		config.SetSubSurface( oglconfig.GetSSSSSEnable() );
	}
	
	switch( pLightType ){
	case deLight::eltPoint:
		config.SetLightMode( deoglLightShaderConfig::elmPoint );
		config.SetShadowMappingAlgorithm1( deoglLightShaderConfig::esmaCube );
		config.SetShadowMappingAlgorithm2( deoglLightShaderConfig::esmaCube );
		
		if( oglconfig.GetUseShadowCubeEncodeDepth() ){
			config.SetHWDepthCompare( false );
			config.SetDecodeInShadow( true );
			config.SetShadowMatrix2EqualsMatrix1( true );
			
		}else{
			config.SetHWDepthCompare( true );
			config.SetDecodeInShadow( false );
			config.SetShadowMatrix2EqualsMatrix1( true );
		}
		
		switch( shaderType ){
		case estLumSolid1:
		case estLumSolid1NoAmbient:
		case estLumSolid2:
			config.SetShadowTapMode( deoglLightShaderConfig::estmSingle );
			break;
			
		default:
			config.SetShadowTapMode( deoglLightShaderConfig::estmPcf9 );
			//config.SetShadowTapMode( deoglLightShaderConfig::estmPcfVariableTap );
		}
		config.SetTextureNoise( false );
		
		if( pLightCanvas ){
			// right now canvas can not have depth. once it has light canvas can only be used
			// as cube map texture. since this is not possible right now equirect is used.
			config.SetTextureColorOmnidirEquirect( true );
			
		}else if( pUseSkinTexture ){
			if( pUseSkinTexture->IsChannelEnabled( deoglSkinChannel::ectColorOmnidirCube ) ){
				config.SetTextureColorOmnidirCube( true );
				
			}else if( pUseSkinTexture->IsChannelEnabled( deoglSkinChannel::ectColorOmnidirEquirect ) ){
				config.SetTextureColorOmnidirEquirect( true );
				
			}else if( pUseSkinTexture->GetMaterialPropertyAt(
			deoglSkinTexture::empColorOmnidirCube ).GetRenderable() != -1 ){
				config.SetTextureColorOmnidirCube( true );
				
			}else if( pUseSkinTexture->GetMaterialPropertyAt(
			deoglSkinTexture::empColorOmnidirEquirect ).GetRenderable() != -1 ){
				config.SetTextureColorOmnidirEquirect( true );
			}
		}
		break;
		
	case deLight::eltSpot:
	case deLight::eltProjector:
		if( pLightType == deLight::eltSpot ){
			config.SetLightMode( deoglLightShaderConfig::elmSpot );
			
		}else{
			config.SetLightMode( deoglLightShaderConfig::elmProjector );
		}
		
		config.SetShadowMappingAlgorithm1( deoglLightShaderConfig::esma2D );
		config.SetShadowMappingAlgorithm2( deoglLightShaderConfig::esma2D );
		
		config.SetHWDepthCompare( true );
		config.SetDecodeInShadow( false );
		config.SetShadowMatrix2EqualsMatrix1( true );
		config.SetShadowInverseDepth( true );
		
		switch( shaderType ){
		case estLumSolid1:
		case estLumSolid1NoAmbient:
		case estLumSolid2:
			config.SetShadowTapMode( deoglLightShaderConfig::estmSingle );
			break;
			
		default:
			config.SetShadowTapMode( deoglLightShaderConfig::estmPcf9 );
			//config.SetShadowTapMode( deoglLightShaderConfig::estmPcfVariableTap );
		}
		config.SetTextureNoise( false );
		
		if( pLightCanvas ){
			config.SetTextureColor( true );
			
		}else if( pUseSkinTexture ){
			// usually spot/projector lights use 2D textures. it is though also allowed to use
			// omni-directional textures like point lights. in this case the spot properties
			// clamp the texture into the positive Z direction
			if( pUseSkinTexture->IsChannelEnabled( deoglSkinChannel::ectColorOmnidirCube ) ){
				config.SetTextureColorOmnidirCube( true );
				
			}else if( pUseSkinTexture->IsChannelEnabled( deoglSkinChannel::ectColorOmnidirEquirect ) ){
				config.SetTextureColorOmnidirEquirect( true );
				
			}else if( pUseSkinTexture->GetMaterialPropertyAt(
			deoglSkinTexture::empColorOmnidirCube ).GetRenderable() != -1 ){
				config.SetTextureColorOmnidirCube( true );
				
			}else if( pUseSkinTexture->GetMaterialPropertyAt(
			deoglSkinTexture::empColorOmnidirEquirect ).GetRenderable() != -1 ){
				config.SetTextureColorOmnidirEquirect( true );
				
			}else{
				config.SetTextureColor( true );
			}
		}
		break;
	}
	
	config.SetDecodeInDepth( oglconfig.GetDefRenEncDepth() );
	
	switch( shaderType ){
	case estNoShadow:
		break;
		
	case estSolid1:
	case estLumSolid1:
		config.SetTextureShadow1Solid( true );
		config.SetTextureShadowAmbient( true );
		break;
		
	case estSolid1NoAmbient:
	case estLumSolid1NoAmbient:
		config.SetTextureShadow1Solid( true );
		break;
		
	case estSolid1Transp1:
		config.SetTextureShadow1Solid( true );
		config.SetTextureShadow1Transparent( true );
		config.SetTextureShadowAmbient( true );
		break;
		
	case estSolid1Transp1NoAmbient:
		config.SetTextureShadow1Solid( true );
		config.SetTextureShadow1Transparent( true );
		break;
		
	case estSolid2:
	case estLumSolid2:
		config.SetTextureShadow1Solid( true );
		config.SetTextureShadow2Solid( true );
		config.SetTextureShadowAmbient( true );
		break;
		
	case estSolid2Transp1:
		config.SetTextureShadow1Solid( true );
		config.SetTextureShadow1Transparent( true );
		config.SetTextureShadow2Solid( true );
		config.SetTextureShadowAmbient( true );
		break;
		
	case estSolid2Transp2:
		config.SetTextureShadow1Solid( true );
		config.SetTextureShadow1Transparent( true );
		config.SetTextureShadow2Solid( true );
		config.SetTextureShadow2Transparent( true );
		config.SetTextureShadowAmbient( true );
		break;
		
	default:
		return false;
	}
	
	return true;
}

deoglSPBlockUBO *deoglRLight::GetLightParameterBlock(){
	if( pParamBlockLight ){
		return pParamBlockLight;
	}
	
	deoglLightShader *shader = NULL;
	int i;
	
	for( i=0; i<EST_COUNT; i++ ){
		if( pShaders[ i ] ){
			shader = pShaders[ i ];
			break;
		}
	}
	if( ! shader ){
		// this is correct since the light parameter block only contains parameters which
		// depend on the light configuration and are the same for all possible shaders
		shader = GetShaderFor( estNoShadow );
	}
	
	shader->EnsureShaderExists();
	pParamBlockLight = shader->CreateSPBLightParam();
	
	return pParamBlockLight;
}

deoglSPBlockUBO *deoglRLight::GetInstanceParameterBlock(){
	if( pParamBlockInstance ){
		return pParamBlockInstance;
	}
	
	deoglLightShader *shader = NULL;
	int i;
	
	for( i=0; i<EST_COUNT; i++ ){
		if( pShaders[ i ] ){
			shader = pShaders[ i ];
			break;
		}
	}
	if( ! shader ){
		// this is correct since the light parameter block only contains parameters which
		// depend on the light configuration and are the same for all possible shaders
		shader = GetShaderFor( estNoShadow );
	}
	
	shader->EnsureShaderExists();
	pParamBlockInstance = shader->CreateSPBInstParam();
	
	return pParamBlockInstance;
}

void deoglRLight::DropShaders(){
	int i;
	
	if( pParamBlockInstance ){
		pParamBlockInstance->FreeReference();;
		pParamBlockInstance = NULL;
	}
	
	if( pParamBlockLight ){
		pParamBlockLight->FreeReference();;
		pParamBlockLight = NULL;
	}
	
	for( i=0; i<EST_COUNT; i++ ){
		if( pShaders[ i ] ){
			pShaders[ i ]->FreeReference();
			pShaders[ i ] = NULL;
		}
	}
}



void deoglRLight::SetVisible( bool visible ){
	pVisible = visible;
}



void deoglRLight::SetDirtyTouching(){
	pDirtyTouching = true;
}

void deoglRLight::EnvMapNotifyLightChanged(){
	if( ! pParentWorld ){
		return;
	}
	
	deoglNotifyEnvMapLightChanged visitor( *this );
	pUpdateExtends();
	pParentWorld->VisitRegion( pMinExtend, pMaxExtend, visitor );
}



void deoglRLight::PrepareQuickDispose(){
	pParentWorld = NULL;
	pOctreeNode = NULL;
	pStaticComponentList.RemoveAll();
	pDynamicComponentList.RemoveAll();
}



// Components
///////////////

void deoglRLight::AddComponent( deoglRComponent *component ){
	if( component->GetRenderStatic() && pHintMovement == deLight::emhStationary ){
		if( pStaticComponentList.AddIfMissing( component ) ){
			pDirtyStaticShadows = true;
			pDirtyCollideLists = true;
			SetLightVolumeCropBox( NULL );
			
			if( component->GetOcclusionMesh() ){
				SetLightVolumeDirty();
			}
		}
		
	}else{
		if( pDynamicComponentList.AddIfMissing( component ) ){
			pDirtyDynamicShadows = true;
			pDirtyCollideLists = true;
		}
	}
}

void deoglRLight::RemoveComponent( deoglRComponent *component ){
	if( pStaticComponentList.RemoveIfExisting( component ) ){
		pDirtyCollideLists = true;
		
		if( pUpdateOnRemoveComponent ){
			pDirtyStaticShadows = true;
			SetLightVolumeCropBox( NULL );
			
			if( component->GetOcclusionMesh() ){
				SetLightVolumeDirty();
			}
		}
	}
	
	if( pDynamicComponentList.RemoveIfExisting( component ) ){
		pDirtyDynamicShadows = true;
		pDirtyCollideLists = true;
	}
}

void deoglRLight::RemoveAllComponents(){
	int i, count;
	
	count = pStaticComponentList.GetCount();
	if( count > 0 ){
		for( i=0; i<count; i++ ){
			pStaticComponentList.GetAt( i )->GetLightList().RemoveIfExisting( this );
		}
		pStaticComponentList.RemoveAll();
		pDirtyStaticShadows = true;
		pDirtyCollideLists = true;
		SetLightVolumeCropBox( NULL );
	}
	
	count = pDynamicComponentList.GetCount();
	if( count > 0 ){
		for( i=0; i<count; i++ ){
			pDynamicComponentList.GetAt( i )->GetLightList().RemoveIfExisting( this );
		}
		pDynamicComponentList.RemoveAll();
		pDirtyDynamicShadows = true;
		pDirtyCollideLists = true;
	}
	
	//DirtyLightVolume();
}

void deoglRLight::TestComponent( deoglRComponent *component ){
	if( ! component ){
		DETHROW( deeInvalidParam );
	}
	
	if( pShadowIgnoreComponents.Has( component ) ){
		return;
	}
	
	if( pLayerMaskShadow.IsNotEmpty() && component->GetLayerMask().IsNotEmpty()
	&& pLayerMaskShadow.MatchesNot( component->GetLayerMask() ) ){
		return;
	}
	
	bool touchesLight = false;
	
	pUpdateExtends();
	
	if( component->GetMaximumExtend() >= pMinExtend && component->GetMinimumExtend() <= pMaxExtend ){
		touchesLight = true;
		
		switch( pLightType ){
		case deLight::eltSpot:{
			// cone-sphere collision test after geometrictools.com . the problem here is that
			// the cone can be squashed. to solve this the radius used for the final test is
			// adjusted depending on where the sphere center is located before being ratio
			// corrected. this is a good enough
			decVector sphereCenter( pInverseMatrix * component->GetCullSphereCenter() );
			
			
			const float sphereRadiusX = component->GetCullSphereRadius();
			const float sphereRadiusY = sphereRadiusX * pSpotRatio;
			
			const float srAngle = atan2f( sphereCenter.y, sphereCenter.x );
			const float srConeSin = sinf( srAngle );
			const float srConeCos = cosf( srAngle );
			
			const float sphereRadius = sphereRadiusX * sphereRadiusY / sqrtf(
				sphereRadiusX * sphereRadiusX * srConeCos * srConeCos
				+ sphereRadiusY * sphereRadiusY * srConeSin * srConeSin );
			const float sphereRadiusSquared = sphereRadius * sphereRadius;
			
			sphereCenter.y *= pSpotRatio;
			
			
			//const float sphereRadius = component->GetCullSphereRadius();
			//const float sphereRadiusSquared = sphereRadius * sphereRadius;
			
			// from here on it is the geometrictools.com version simply modified with V=zero
			// and D=(0,0,1)
			const float coneSin = sinf( pSpotAngle * 0.5f );
			const float coneCos = cosf( pSpotAngle * 0.5f );
			const float coneSinSquared = coneSin * coneSin;
			const float coneCosSquared = coneCos * coneCos;
			const float coneSinReci = 1.0f / coneSin;
			
			touchesLight = false;
			
			const decVector d( sphereCenter.x, sphereCenter.y,
				sphereCenter.z + sphereRadius * coneSinReci );
			
			if( d.z <= 0.0 || d.z * d.z < d.LengthSquared() * coneCosSquared ){
				break;
			}
			
			const float dsquared = sphereCenter.LengthSquared();
			const float e = -sphereCenter.z;
			
			if( e > 0.0 && e * e >= dsquared * coneSinSquared ){
				touchesLight = ( dsquared <= sphereRadiusSquared );
				
			}else{
				touchesLight = true;
			}
			}break;
			
			/*
			// previous code
			
			// cone-sphere collision test after geometrictools.com
			const decDVector &sphereCenter = component->GetCullSphereCenter();
			const float sphereRadius = component->GetCullSphereRadius();
			const float sphereRadiusSquared = sphereRadius * sphereRadius;
			const decDVector coneCenter( pMatrix.GetPosition() );
			const decDVector coneAxis( pMatrix.TransformView() );
			const float coneSin = sinf( pSpotAngles.y * 0.5f );
			const float coneCos = cosf( pSpotAngles.y * 0.5f );
			const float coneCosSquared = coneCos * coneCos;
			const float coneSinSquared = coneSin * coneSin;
			const float coneSinReci = 1.0f / coneSin;
			double dsquared, e;
			decDVector u, d;
			
			touchesLight = false;
			
			u = coneCenter - coneAxis * ( double )( sphereRadius * coneSinReci );
			d = sphereCenter - u;
			dsquared = d * d;
			e = coneAxis * d;
			
			if( e > 0.0 && e * e >= dsquared * ( double )coneCosSquared ){
				d = sphereCenter - coneCenter;
				dsquared = d * d;
				e = -( coneAxis * d );
				
				if( e > 0.0 && e * e >= dsquared * ( double )coneSinSquared ){
					touchesLight = ( dsquared <= ( double )sphereRadiusSquared );
					
				}else{
					touchesLight = true;
				}
			}
			*/
			
		case deLight::eltProjector:{
			// TODO this needs a frustum-aabb check
			}break;
			
		case deLight::eltPoint:
			break;
		}
	}
	
	if( touchesLight ){
		AddComponent( component );
		component->GetLightList().AddIfMissing( this );
		
	}else{
		RemoveComponent( component );
		component->GetLightList().RemoveIfExisting( this );
	}
}



// Culling
////////////

void deoglRLight::StartOcclusionTest( const decDVector &cameraPosition ){
	UpdateLightVolume();
	
	if( pDirtyExtends ){
		pUpdateExtends();
	}
	
	const decVector minExtend = ( pMinExtend - cameraPosition ).ToVector();
	const decVector maxExtend = ( pMaxExtend - cameraPosition ).ToVector();
	
	pRenderThread.GetOcclusionTest().AddInputData( minExtend, maxExtend, this );
}

void deoglRLight::OcclusionTestInvisible(){
	pVisible = false;
}



deoglOcclusionQuery &deoglRLight::GetOcclusionQuery(){
	if( ! pOcclusionQuery ){
		pOcclusionQuery = new deoglOcclusionQuery( pRenderThread );
	}
	return *pOcclusionQuery;
}

bool deoglRLight::IsHiddenByOccQuery(){
	if( ! pInsideCamera && pOcclusionQuery ){
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



// Optimizations
//////////////////

void deoglRLight::LightVolumeImproved(){
	pDirtyColVol = true;
	pDirtyCollideLists = true;
	pDirtyExtends = true;
	pDirtyStaticShadows = true;
	pDirtyDynamicShadows = true;
	pDirtyTouching = true;
}

void deoglRLight::ReplaceLightVolume( decConvexVolumeList *list ){
	if( ! list ) DETHROW( deeInvalidParam );
	if( pConvexVolumeList ) delete pConvexVolumeList;
	pConvexVolumeList = list;
	LightVolumeImproved();
}

void deoglRLight::ReplaceShadowCaster( deoglShadowCaster *shadowCaster ){
	if( ! shadowCaster ) DETHROW( deeInvalidParam );
	if( pShadowCaster ) delete pShadowCaster;
	pShadowCaster = shadowCaster;
	//pCalcShadowCasterParams();
	pDirtyStaticShadows = true;
	pDirtyDynamicShadows = true;
	
	// some stats
//	pOgl->LogInfoFormat( "Updated Shadow Caster for %p:", this );
//	pOgl->LogInfoFormat( "   Solid: %i points", pShadowCaster->GetSolid()->GetPointCount() );
//	pOgl->LogInfoFormat( "   Transp: %i textures, %i points",
//		pShadowCaster->GetTransparent()->GetTextureCount(),
//		pShadowCaster->GetTransparent()->GetPointCount() );
}



// Render world usage
///////////////////////

void deoglRLight::SetWorldMarkedRemove( bool marked ){
	pWorldMarkedRemove = marked;
}



// Private Functions
//////////////////////

class deoglRLightDeletion : public deoglDelayedDeletion{
public:
	deoglShadowCaster *shadowCaster;
	deoglLightVolume *lightVolume;
	deoglOcclusionQuery *occlusionQuery;
	deoglLightShader *shaders[ deoglRLight::EST_COUNT ];
	deoglSPBlockUBO *paramBlockLight;
	deoglSPBlockUBO *paramBlockInstance;
	deoglSkinState *skinState;
	
	deoglRLightDeletion() :
	shadowCaster( NULL ),
	lightVolume( NULL ),
	occlusionQuery( NULL ),
	paramBlockLight( NULL ),
	paramBlockInstance( NULL ),
	skinState( NULL )
	{
		int i;
		for( i=0; i<deoglRLight::EST_COUNT; i++ ){
			shaders[ i ] = NULL;
		}
	}
	
	virtual ~deoglRLightDeletion(){
	}
	
	virtual void DeleteObjects( deoglRenderThread &renderThread ){
		int i;
		if( paramBlockInstance ){
			paramBlockInstance->FreeReference();
		}
		if( paramBlockLight ){
			paramBlockLight->FreeReference();
		}
		for( i=0; i<deoglRLight::EST_COUNT; i++ ){
			if( shaders[ i ] ){
				shaders[ i ]->FreeReference();
			}
		}
		if( lightVolume ){
			delete lightVolume;
		}
		if( shadowCaster ){
			delete shadowCaster;
		}
		if( occlusionQuery ){
			delete occlusionQuery;
		}
		if( skinState ){
			delete skinState;
		}
	}
};

void deoglRLight::pCleanUp(){
	SetParentWorld( NULL );
	
	if( pDynamicCollideList ){
		delete pDynamicCollideList;
	}
	if( pStaticCollideList ){
		delete pStaticCollideList;
	}
	
	pShadowIgnoreComponents.RemoveAll();
	
	RemoveAllComponents();
	
	if( pOptimizer ){
		pRenderThread.GetOptimizerManager().RemoveOptimizer( pOptimizer );
	}
	if( pLightVolumeCropBox ){
		delete pLightVolumeCropBox;
	}
	if( pConvexVolumeList ){
		delete pConvexVolumeList;
	}
	if( pColVol ){
		delete pColVol;
	}
	
	if( pLightCanvas ){
		pLightCanvas->FreeReference();
	}
	if( pLightSkin ){
		pLightSkin->FreeReference();
	}
	if( pDynamicSkin ){
		pDynamicSkin->FreeReference();
	}
	
	// delayed deletion of opengl containing objects
	deoglRLightDeletion *delayedDeletion = NULL;
	int i;
	
	if( pSkinState ){
		pSkinState->DropDelayedDeletionObjects(); // avoid race conditions
	}
	
	try{
		delayedDeletion = new deoglRLightDeletion;
		delayedDeletion->lightVolume = pLightVolume;
		delayedDeletion->occlusionQuery = pOcclusionQuery;
		delayedDeletion->paramBlockInstance = pParamBlockInstance;
		delayedDeletion->paramBlockLight = pParamBlockLight;
		for( i=0; i<EST_COUNT; i++ ){
			delayedDeletion->shaders[ i ] = pShaders[ i ];
		}
		delayedDeletion->shadowCaster = pShadowCaster;
		delayedDeletion->skinState = pSkinState;
		
		pRenderThread.GetDelayedOperations().AddDeletion( delayedDeletion );
		
	}catch( const deException &e ){
		if( delayedDeletion ){
			delete delayedDeletion;
		}
		pRenderThread.GetLogger().LogException( e );
		throw;
	}
}

void deoglRLight::pUpdateFullExtends(){
	if( ! pDirtyFullExtends ){
		return;
	}
	
	const decDVector lightPosition( pMatrix.GetPosition() );
	
	switch( pLightType ){
	case deLight::eltPoint:
		// set extends from a box around the light sphere
		pFullMinExtend.x = lightPosition.x - pRange;
		pFullMinExtend.y = lightPosition.y - pRange;
		pFullMinExtend.z = lightPosition.z - pRange;
		
		pFullMaxExtend.x = lightPosition.x + pRange;
		pFullMaxExtend.y = lightPosition.y + pRange;
		pFullMaxExtend.z = lightPosition.z + pRange;
		break;
		
	case deLight::eltSpot:{
		// the calculation of the boundary box around the light cone boils down to the
		// calculation of the boundary box around the circular base of the cone using
		// the cone tip as the starting value of the boundary. box. the boundary box
		// around a circle in 3d is calculated like this:
		// 
		//   he_x = radius * sin( acos( cos( normal dot axis ) ) )
		// 
		// he_x is the half extends. normal dot axis returns cos( a ) since both are
		// normalized quantities. so we are looking actually for sin( a ). this can
		// be written more easily using only a square root instead of trigonometric
		// functions like this:
		//   
		//   he_x = radius * sqrt( 1 - squared( normal dot axis ) )
		// 
		// the resulting boundary box is then (O-HE)-(O+HE) for O as the center of the
		// circle and HE = (he_x, he_y, he_z).
		// 
		// in this case some shortcuts are used. the normal is actually the negation of
		// the circle normal. hence -nx is used instead of nx in the above calculation.
		// for squaring this is though identical
		// 
		// TODO fix this since now spot cone can be squashed. we need something better
		pFullMinExtend.x = lightPosition.x - pRange;
		pFullMinExtend.y = lightPosition.y - pRange;
		pFullMinExtend.z = lightPosition.z - pRange;
		
		pFullMaxExtend.x = lightPosition.x + pRange;
		pFullMaxExtend.y = lightPosition.y + pRange;
		pFullMaxExtend.z = lightPosition.z + pRange;
		
		/*
		const decDVector normal( pMatrix.TransformView() );
		const decDVector center = lightPosition + normal * ( double )pRange;
		const int resolution = 12;
		const float circleAngleStep = PI * 2.0f / ( float )resolution;
		const float circleRadius = pRange * tanf( pSpotAngles.y * 0.5f ) / cosf( circleAngleStep * 0.5f );
		decDVector halfExtend;
		
		halfExtend.x = sqrt( 1.0 - normal.x * normal.x ) * ( double )circleRadius;
		halfExtend.y = sqrt( 1.0 - normal.y * normal.y ) * ( double )circleRadius;
		halfExtend.z = sqrt( 1.0 - normal.z * normal.z ) * ( double )circleRadius;
		
		pFullMinExtend = center - halfExtend;
		pFullMaxExtend = center + halfExtend;
		
		// add the cone base to the boundary box
		if( lightPosition.x < pFullMinExtend.x ){
			pFullMinExtend.x = lightPosition.x;
			
		}else if( lightPosition.x > pFullMaxExtend.x ){
			pFullMaxExtend.x = lightPosition.x;
		}
		
		if( lightPosition.y < pFullMinExtend.y ){
			pFullMinExtend.y = lightPosition.y;
			
		}else if( lightPosition.y > pFullMaxExtend.y ){
			pFullMaxExtend.y = lightPosition.y;
		}
		
		if( lightPosition.z < pFullMinExtend.z ){
			pFullMinExtend.z = lightPosition.z;
			
		}else if( lightPosition.z > pFullMaxExtend.z ){
			pFullMaxExtend.z = lightPosition.z;
		}
		*/
		}break;
		
	case deLight::eltProjector:
		// TODO: same code as for the spot light case just with a box around a frustum.
		// this only requires finding the box around the 5 frustum points (if the near
		// vertices all fall together)
		
		// set extends from a box around the light sphere. a frustum would be the best
		// solution but currently the frustum does not contain enclosing box code
		pFullMinExtend.x = lightPosition.x - pRange;
		pFullMinExtend.y = lightPosition.y - pRange;
		pFullMinExtend.z = lightPosition.z - pRange;
		
		pFullMaxExtend.x = lightPosition.x + pRange;
		pFullMaxExtend.y = lightPosition.y + pRange;
		pFullMaxExtend.z = lightPosition.z + pRange;
	}
	
	pDirtyFullExtends = false;
}

void deoglRLight::pUpdateExtends(){
	if( ! pDirtyExtends ){
		return;
	}
	
	pUpdateFullExtends();
	
	pMinExtend = pFullMinExtend;
	pMaxExtend = pFullMaxExtend;
	
	UpdateLightVolume();
	
	// if the light casts shadows determine a matching box using the light volume if existing
	if( pCastShadows ){
		//if( pLight->GetType() == deLight::eltSpot )
		deoglLightVolumeBuilder builder;
		builder.GetTransformedVolumeExtends( *pConvexVolumeList, pMatrix, pMinExtend, pMaxExtend );
	}
	
	pDirtyExtends = false;
}

void deoglRLight::pUpdateCollideLists(){
	if( ! pDirtyCollideLists ) return;
	
	int i, count;
	
	// create collide lists if not existng or clear them if existing
	if( pStaticCollideList ){
		pStaticCollideList->Clear();
		
	}else{
		pStaticCollideList = new deoglCollideList;
	}
	
	if( pDynamicCollideList ){
		pDynamicCollideList->Clear();
		
	}else{
		pDynamicCollideList = new deoglCollideList;
	}
	
	// update static collide list
	count = pStaticComponentList.GetCount();
	for( i=0; i<count; i++ ){
		pStaticCollideList->AddComponent( pStaticComponentList.GetAt( i ) );
	}
	
	// update dynamic collide list
	count = pDynamicComponentList.GetCount();
	for( i=0; i<count; i++ ){
		pDynamicCollideList->AddComponent( pDynamicComponentList.GetAt( i ) );
	}
	
	// no more dirty
	pDirtyCollideLists = false;
}

void deoglRLight::pCheckTouching(){
	if( ! pDirtyTouching ){
		return;
	}
	
	pDirtyTouching = false; // at the beginning to avoid potential dead-loops
	
	//if( pLight->GetHintMovement() == deLight::emhStationary ){
	
	pStaticComponentList.MarkAll( true );
	pDynamicComponentList.MarkAll( true );
	
	if( pActive && pParentWorld ){
		pUpdateExtends();
		pDirtyTouching = false; // pUpdateExtends potentially sets pDirtyTouching to true
		
		deoglLightTestForTouch visitor( *this );
		pUpdateOnRemoveComponent = false;
		pParentWorld->VisitRegion( pMinExtend, pMaxExtend, visitor );
		pUpdateOnRemoveComponent = true;
	}
	
	// remove light from all non-marked components. act like pUpdateOnRemoveComponent is false.
	// this is required or else components still hold to this light pointer although it is
	// freed some time later. RemoveAllMarked only removes the components but does not remove
	// the light from them.
	const int dynamicComponentCount = pDynamicComponentList.GetCount();
	const int staticComponentCount = pStaticComponentList.GetCount();
	int i;
	
	for( i=0; i<staticComponentCount; i++ ){
		deoglRComponent &component = *pStaticComponentList.GetAt( i );
		if( component.GetMarked() ){
			component.GetLightList().RemoveIfExisting( this );
		}
	}
	
	for( i=0; i<dynamicComponentCount; i++ ){
		deoglRComponent &component = *pDynamicComponentList.GetAt( i );
		if( component.GetMarked() ){
			component.GetLightList().RemoveIfExisting( this );
		}
	}
	
	// now the components can be safely removed in an efficient way
	pStaticComponentList.RemoveAllMarked( true );
	pDynamicComponentList.RemoveAllMarked( true );
}

void deoglRLight::pUpdateRenderables(){
	if( ! pSkinState ){
		return;
	}
	
	// check if an update is needed
	if( pDynamicSkin && pLightSkin && pLightSkin->GetHasRenderables() ){
		const int updateNumber = pDynamicSkin->Update();
		if( updateNumber != pSkinState->GetUpdateNumber() ){
			pSkinState->SetUpdateNumber( updateNumber );
			pDirtyRenderables = true;
		}
	}
	
	// update renderable mappings in the dynamic skins
	if( pDirtyRenderables ){
		pSkinState->RemoveAllRenderables();
		if( pLightSkin && pDynamicSkin ){
			pSkinState->AddRenderables( *pLightSkin, *pDynamicSkin );
		}
		
		pDirtyRenderables = false;
	}
	
	// prepare renderables
	pSkinState->PrepareRenderables( pLightSkin, pDynamicSkin );
}
