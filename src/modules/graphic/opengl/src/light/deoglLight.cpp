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

#include "deoglLight.h"
#include "deoglLightGatherOcclusionMeshes.h"
#include "deoglLightTestForTouch.h"
#include "deoglRLight.h"
#include "shader/deoglLightShader.h"
#include "shader/deoglLightShaderManager.h"
#include "volume/deoglLightVolume.h"
#include "volume/deoglLightVolumeBuilder.h"

#include "../deGraphicOpenGl.h"
#include "../canvas/deoglCanvasView.h"
#include "../collidelist/deoglCollideList.h"
#include "../collidelist/deoglCollideListComponent.h"
#include "../component/deoglComponent.h"
#include "../configuration/deoglConfiguration.h"
#include "../occlusiontest/deoglOcclusionTest.h"
#include "../occlusiontest/mesh/deoglOcclusionMesh.h"
#include "../occquery/deoglOcclusionQuery.h"
#include "../optimizer/deoglOptimizerLight.h"
#include "../optimizer/deoglOptimizerManager.h"
#include "../rendering/plan/deoglRenderPlan.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTShader.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../shadow/deoglSCSolid.h"
#include "../shadow/deoglSCTransparent.h"
#include "../shadow/deoglShadowCaster.h"
#include "../skin/deoglSkin.h"
#include "../skin/dynamic/deoglDynamicSkin.h"
#include "../skin/state/deoglSkinStateController.h"
#include "../triangles/deoglTriangleSorter.h"
#include "../visitors/deoglTransformVolume.h"
#include "../world/deoglWorld.h"
#include "../utils/collision/deoglDCollisionBox.h"
#include "../utils/collision/deoglDCollisionDetection.h"
#include "../utils/collision/deoglDCollisionSphere.h"
#include "../utils/collision/deoglDCollisionTriangle.h"
#include "../utils/collision/deoglDCollisionVolume.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decConvexVolume.h>
#include <dragengine/common/math/decConvexVolumeFace.h>
#include <dragengine/common/math/decConvexVolumeList.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>
#include <dragengine/resources/light/deLight.h>
#include <dragengine/resources/world/deWorld.h>



// Class deoglLight
/////////////////////

// Constructor, destructor
////////////////////////////

deoglLight::deoglLight( deGraphicOpenGl &ogl, const deLight &light ) :
pOgl( ogl ),
pLight( light ),
pRLight( NULL ),
pSkinStateController( NULL ),
pParentWorld( NULL ),

pAccumUpdate( 0.0f ),

pDirtyAttenuation( true ),
pDirtyCollideLists( true ),
pDirtyColVol( true ),
pDirtyConvexVolumeList( true ),
pDirtyExtends( true ),
pDirtyFullExtends( true ),
pDirtyLight( true ),
pDirtyLightParameters( true ),
pDirtyMatrices( true ),
pDirtyOctree( true ),
pDirtyShadowParameters( true ),
pDirtyShadows( true ),
pDirtySource( true ),
pDirtyTouching( true ),
pDirtyTransform( true ),
pDirtyType( true ),
pDirtySkinStateController( true )
{
	try{
		pRLight = new deoglRLight( ogl.GetRenderThread() );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglLight::~deoglLight(){
	pCleanUp();
}



// Management
///////////////

void deoglLight::SetParentWorld( deoglWorld *world ){
	if( world == pParentWorld ){
		return;
	}
	
	if( pSkinStateController ){
		pSkinStateController->Clear();
	}
	
	pParentWorld = world;
	
	pDirtyOctree = true;
	pDirtySkinStateController = true;
}

void deoglLight::SyncToRender(){
	bool requiresDirtyEnvMaps = false;
	
	deoglDynamicSkin *dynamicSkin = NULL;
	if( pLight.GetDynamicSkin() ){
		dynamicSkin = ( deoglDynamicSkin* )pLight.GetDynamicSkin()->GetPeerGraphic();
		dynamicSkin->SyncToRender();
	}
	
	deoglCanvasView *lightCanvas = NULL;
	if( pLight.GetLightCanvas() ){
		lightCanvas = ( deoglCanvasView* )pLight.GetLightCanvas()->GetPeerGraphic();
		lightCanvas->SyncToRender();
	}
	
	
	if( pDirtyLight ){
		pRLight->SetActive( pLight.GetActivated() );
		pRLight->SetSpotAngle( pLight.GetSpotAngle() );
		pRLight->SetSpotRatio( pLight.GetSpotRatio() );
		pRLight->SetHintMovement( pLight.GetHintMovement() );
		pRLight->SetHintShadowImportance( pLight.GetHintShadowImportance() );
		
		pDirtyLight = false;
		if( pLight.GetHintMovement() != deLight::emhDynamic ){
			requiresDirtyEnvMaps = true;
		}
		// TODO use parameter hint
	}
	
	if( pDirtyType ){
		pRLight->SetLightType( pLight.GetType() );
		pRLight->DropShaders();
		
		pDirtyType = false;
		if( pLight.GetHintMovement() != deLight::emhDynamic ){
			requiresDirtyEnvMaps = true;
		}
	}
	
	if( pDirtyLightParameters ){
		pRLight->SetIntensity( pLight.GetIntensity() );
		pRLight->SetAmbientRatio( pLight.GetAmbientRatio() );
		pRLight->SetColor( pLight.GetColor() );
		pRLight->SetSpotSmoothness( pLight.GetSpotSmoothness() );
		pRLight->SetSpotExponent( pLight.GetSpotExponent() );
		pRLight->SetLayerMask( pLight.GetLayerMask() );
		
		pDirtyLightParameters = false;
		if( pLight.GetHintMovement() == deLight::emhStationary ){
			//requiresDirtyEnvMaps = true; // track if changed enough
		}
		// TODO use parameter hint
	}
	
	if( pDirtyShadowParameters ){
		pRLight->SetCastShadows( pLight.GetCastShadows() );
		pRLight->SetLayerMaskShadow( pLight.GetLayerMaskShadow() );
		
		pRLight->RemoveAllShadowIgnoreComponents();
		const int ignoreCount = pLight.GetShadowIgnoreComponentCount();
		int i;
		for( i=0; i<ignoreCount; i++ ){
			pRLight->AddShadowIgnoreComponent( ( ( deoglComponent* )pLight.
				GetShadowIgnoreComponentAt( i )->GetPeerGraphic() )->GetRComponent() );
		}
		
		pDirtyShadowParameters = false;
		if( pLight.GetHintMovement() != deLight::emhDynamic ){
			requiresDirtyEnvMaps = true;
		}
	}
	
	if( pDirtyAttenuation ){
		pRLight->UpdateAttenuation( pLight.GetRange(), pLight.GetHalfIntensityDistance() );
		
		pDirtyAttenuation = false;
		if( pLight.GetHintMovement() != deLight::emhDynamic ){
			requiresDirtyEnvMaps = true;
		}
	}
	
	if( pDirtyTransform ){
		pRLight->SetTransform( pLight.GetTransform() );
		pDirtyTransform = false;
	}
	
	if( pDirtySource ){
		if( pLight.GetLightSkin() ){
			deoglSkin * const skin = ( deoglSkin* )pLight.GetLightSkin()->GetPeerGraphic();
			pRLight->SetLightSkin( skin->GetRSkin() );
			
			if( ! pSkinStateController ){
				pSkinStateController = new deoglSkinStateController;
			}
			
		}else{
			pRLight->SetLightSkin( NULL );
		}
		
		if( dynamicSkin ){
			pRLight->SetDynamicSkin( dynamicSkin->GetRDynamicSkin() );
			
		}else{
			pRLight->SetDynamicSkin( NULL );
		}
		
		if( lightCanvas ){
			pRLight->SetLightCanvas( lightCanvas->GetRCanvasView() );
			
		}else{
			pRLight->SetLightCanvas( NULL );
		}
		
		pRLight->DropShaders();
		pRLight->SetRenderablesDirty();
		
		pDirtySource = false;
		if( pLight.GetHintMovement() != deLight::emhDynamic ){
			requiresDirtyEnvMaps = true;
		}
	}
	
	if( pDirtySkinStateController ){
		if( pSkinStateController && pRLight->GetSkinState() ){
			pSkinStateController->Init( *pRLight->GetSkinState(), pRLight->GetLightSkin(), pParentWorld );
		}
		
		pDirtySkinStateController = false;
	}
	
	if( pAccumUpdate > 0.0f ){
		if( pSkinStateController ){
			pSkinStateController->AdvanceTime( pAccumUpdate );
			pSkinStateController->SyncToRender();
		}
		
		pRLight->UpdateSkin( pAccumUpdate );
		
		pAccumUpdate = 0.0f;
	}
	
	if( pDirtyMatrices ){
		pRLight->SetMatrix( decDMatrix::CreateWorld( pLight.GetPosition(), pLight.GetOrientation() ) );
		pDirtyMatrices = false;
		
		if( pLight.GetHintMovement() == deLight::emhStationary ){ // jitter: if change is large enough?
			requiresDirtyEnvMaps = true;
		}
	}
	
	if( pDirtyFullExtends ){
		pRLight->SetDirtyFullExtends();
		pDirtyFullExtends = false;
	}
	
	if( pDirtyExtends ){
		pRLight->SetDirtyExtends();
		pDirtyExtends = false;
	}
	
	if( pDirtyCollideLists ){
		pRLight->SetDirtyCollideLists();
		pDirtyCollideLists = false;
	}
	
	if( pDirtyColVol ){
		pRLight->SetDirtyCollisionVolume();
		pDirtyColVol = false;
	}
	
	if( pDirtyConvexVolumeList ){
		pRLight->SetLightVolumeCropBox( NULL );
		pRLight->SetLightVolumeDirty();
		pDirtyConvexVolumeList = false;
	}
	
	if( pDirtyShadows ){
		pRLight->SetDirtyShadows();
		pDirtyShadows = false;
	}
	
	if( pDirtyTouching ){
		pRLight->SetDirtyTouching();
		pDirtyTouching = false;
	}
	
	if( pDirtyOctree ){
		pRLight->UpdateOctreeNode();
		pDirtyOctree = false;
	}
	
	if( requiresDirtyEnvMaps ){
		pRLight->EnvMapNotifyLightChanged();
	}
}



// Notifications
//////////////////

void deoglLight::TypeChanged(){
	pDirtyCollideLists = true;
	pDirtyColVol = true;
	pDirtyConvexVolumeList = true;
	pDirtyExtends = true;
	pDirtyFullExtends = true;
	pDirtyOctree = true;
	pDirtyShadows = true;
	pDirtyTouching = true;
	pDirtyType = true;
}

void deoglLight::GeometryParameterChanged(){
	pDirtyAttenuation = true;
	pDirtyCollideLists = true;
	pDirtyColVol = true;
	pDirtyConvexVolumeList = true;
	pDirtyExtends = true;
	pDirtyFullExtends = true;
	pDirtyLight = true;
	pDirtyOctree = true;
	pDirtyShadows = true;
	pDirtyTouching = true;
}

void deoglLight::AttenuationChanged(){
	pDirtyAttenuation = true;
	pDirtyCollideLists = true;
	pDirtyColVol = true;
	pDirtyConvexVolumeList = true;
	pDirtyExtends = true;
	pDirtyFullExtends = true;
	pDirtyOctree = true;
	pDirtyShadows = true;
	pDirtyTouching = true;
}

void deoglLight::LightParameterChanged(){
	pDirtyLightParameters = true;
}

void deoglLight::ShadowParameterChanged(){
	pDirtyCollideLists = true;
	pDirtyColVol = true;
	pDirtyConvexVolumeList = true;
	pDirtyExtends = true;
	pDirtyFullExtends = true;
	pDirtyOctree = true;
	pDirtyShadowParameters = true;
	pDirtyShadows = true;
	pDirtyTouching = true;
}

void deoglLight::ShapeChanged(){
	pDirtyAttenuation = true;
	pDirtyCollideLists = true;
	pDirtyColVol = true;
	pDirtyConvexVolumeList = true;
	pDirtyExtends = true;
	pDirtyFullExtends = true;
	pDirtyLight = true;
	pDirtyOctree = true;
	pDirtyShadows = true;
	pDirtyTouching = true;
	pDirtyType = true;
}

void deoglLight::LayerMaskChanged(){
	pDirtyLightParameters = true;
}

void deoglLight::ShadowLayerMaskChanged(){
	pDirtyCollideLists = true;
	pDirtyColVol = true;
	pDirtyConvexVolumeList = true;
	pDirtyExtends = true;
	pDirtyFullExtends = true;
	pDirtyOctree = true;
	pDirtyShadowParameters = true;
	pDirtyShadows = true;
	pDirtyTouching = true;
}

void deoglLight::ShadowIgnoreComponentsChanged(){
	pDirtyCollideLists = true;
	pDirtyColVol = true;
	pDirtyConvexVolumeList = true;
	pDirtyExtends = true;
	pDirtyFullExtends = true;
	pDirtyOctree = true;
	pDirtyShadowParameters = true;
	pDirtyShadows = true;
	pDirtyTouching = true;
}

void deoglLight::PositionChanged(){
	pDirtyCollideLists = true;
	pDirtyColVol = true;
	pDirtyConvexVolumeList = true;
	pDirtyExtends = true;
	pDirtyFullExtends = true;
	pDirtyMatrices = true;
	pDirtyOctree = true;
	pDirtyShadows = true;
	pDirtyTouching = true;
}

void deoglLight::OrientationChanged(){
	pDirtyCollideLists = true;
	pDirtyColVol = true;
	pDirtyConvexVolumeList = true;
	pDirtyExtends = true;
	pDirtyFullExtends = true;
	pDirtyMatrices = true;
	pDirtyOctree = true;
	pDirtyShadows = true;
	pDirtyTouching = true;
}

void deoglLight::SourceChanged(){
	pDirtySource = true;
	pDirtySkinStateController = true;
}

void deoglLight::TransformChanged(){
	pDirtyTransform = true;
}

void deoglLight::ActivatedChanged(){
	pDirtyLight = true;
	pDirtyOctree = true;
}

void deoglLight::HintChanged(){
	pDirtyLight = true;
	pDirtyFullExtends = true;
	pDirtyExtends = true;
	pDirtyColVol = true;
	pDirtyCollideLists = true;
	pDirtyConvexVolumeList = true;
	pDirtyShadows = true;
}



// Private Functions
//////////////////////

void deoglLight::pCleanUp(){
	if( pSkinStateController ){
		delete pSkinStateController;
	}
	
	if( pRLight ){
		pRLight->FreeReference();
	}
}
