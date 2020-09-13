/* 
 * Drag[en]gine IGDE
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
#include <stdlib.h>
#include <string.h>

#include "igdeWOSOLight.h"
#include "igdeWOSOVisitor.h"
#include "../igdeWObject.h"
#include "../../../engine/igdeEngineController.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../gamedefinition/igdeGameDefinition.h"
#include "../../../gamedefinition/class/igdeGDClass.h"
#include "../../../gamedefinition/class/light/igdeGDCLight.h"
#include "../../../gameproject/igdeGameProject.h"
#include "../../../resourceloader/igdeResourceLoaderTask.h"
#include "../../../resourceloader/igdeResourceLoaderListener.h"
#include "../../../triggersystem/igdeTriggerExpression.h"
#include "../../../triggersystem/igdeTriggerExpressionParser.h"
#include "../../../triggersystem/igdeTriggerTargetList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/light/deLight.h>
#include <dragengine/resources/light/deLightManager.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/skin/deSkinReference.h>
#include <dragengine/resources/world/deWorld.h>



// Resource loading
/////////////////////

class igdeWOSOLightResLoadComponent : public igdeResourceLoaderListener{
private:
	igdeWOSOLight *pOwner;
	decString pPathLightSkin;
	deSkinReference pLightSkin;
	int pCounter;
	bool pSuccess;
	
public:
	igdeWOSOLightResLoadComponent( igdeWOSOLight &owner ) :
	pOwner( &owner ), pCounter( 0 ), pSuccess( true ){
	}
	
	virtual ~igdeWOSOLightResLoadComponent(){
	}
	
	void Drop(){
		pOwner = NULL;
	}
	
	void LoadLightSkin( const char *path ){
		pPathLightSkin = path;
		pOwner->GetWrapper().GetEnvironment().AsyncLoadResource( path, deResourceLoader::ertSkin, this );
		pCounter++;
	}
	inline deSkin *GetLightSkin() const{ return pLightSkin; }
	inline bool HasPathLightSkin() const{ return ! pPathLightSkin.IsEmpty(); }
	
	virtual void LoadingFinished( const igdeResourceLoaderTask &task, deFileResource *resource){
		if( ! pOwner ){
			return;
		}
		
		const deResourceLoader::eResourceType type = task.GetResourceType();
		const decString &filename = task.GetFilename();
		
		if( type == deResourceLoader::ertSkin && pPathLightSkin == filename ){
			pLightSkin = ( deSkin* )resource;
			pCounter--;
		}
		
		CheckFinished();
	}
	
	virtual void LoadingFailed( const igdeResourceLoaderTask &task ){
		if( ! pOwner ){
			return;
		}
		
		const deResourceLoader::eResourceType type = task.GetResourceType();
		const decString &filename = task.GetFilename();
		
		if( type == deResourceLoader::ertSkin && pPathLightSkin == filename ){
			pCounter--;
			pSuccess = false;
		}
		
		CheckFinished();
	}
	
	void CheckFinished(){
		if( pOwner && pCounter == 0 ){
			pOwner->AsyncLoadFinished( pSuccess );
		}
	}
};


// Class igdeWOSOLight
////////////////////////

// Constructor, destructor
////////////////////////////

igdeWOSOLight::igdeWOSOLight( igdeWObject &wrapper,
	const igdeGDCLight &gdLight, const decString &prefix ) :
igdeWOSubObject( wrapper, prefix ),
pGDLight( gdLight ),
pAddedToWorld( false ),
pAttachment( NULL )
{
	pLoadResources();
}

igdeWOSOLight::~igdeWOSOLight(){
	if( pResLoad ){
		( ( igdeWOSOLightResLoadComponent& )( igdeResourceLoaderListener& )pResLoad ).Drop();
		pResLoad = NULL;
	}
	pDestroyLight();
	pClearTrigger( pTriggerActivate );
}



// Management
///////////////

void igdeWOSOLight::UpdateParameters(){
	pLoadResources();
}

void igdeWOSOLight::InitTriggers(){
	pInitTrigger( pTriggerActivate, pGDLight.GetTriggerName( igdeGDCLight::etActivated ) );
}

void igdeWOSOLight::UpdateTriggers(){
	if( ! pLight ){
		return;
	}
	
	if( GetWrapper().GetVisible() ){
		if( pTriggerActivate ){
			pTriggerActivate->Evaluate();
			pLight->SetActivated( pTriggerActivate->GetResult() );
			
		}else{
			pLight->SetActivated( GetBoolProperty(
				pGDLight.GetPropertyName( igdeGDCLight::epActivated ),
				pGDLight.GetActivated() ) );
		}
		
	}else{
		pLight->SetActivated( false );
	}
}

void igdeWOSOLight::UpdateVisibility(){
	if( pLight ){
		pLight->SetActivated( GetWrapper().GetVisible() );
	}
}

void igdeWOSOLight::UpdateLayerMasks(){
	if( pLight ){
		int layerMask = GetWrapper().GetRenderLayerMask();
		layerMask |= GetWrapper().GetRenderEnvMapMask();
		pLight->SetLayerMask( LayerMaskFromInt( layerMask ) );
	}
}

void igdeWOSOLight::OnAllSubObjectsFinishedLoading(){
	pUpdateLight();
}

void igdeWOSOLight::Visit( igdeWOSOVisitor &visitor ){
	visitor.VisitLight( *this );
}

void igdeWOSOLight::AsyncLoadFinished( bool success ){
	if( ! pResLoad ){
		return;
	}
	
	( ( igdeWOSOLightResLoadComponent& )( igdeResourceLoaderListener& )pResLoad ).Drop();
	
	GetWrapper().SubObjectFinishedLoading( *this, success );
}



// Private Functions
//////////////////////

void igdeWOSOLight::pLoadResources(){
	if( pResLoad ){
		( ( igdeWOSOLightResLoadComponent& )( igdeResourceLoaderListener& )pResLoad ).Drop();
		pResLoad = NULL;
	}
	
	pResLoad.TakeOver( new igdeWOSOLightResLoadComponent( *this ) );
	igdeWOSOLightResLoadComponent &rl =
		( igdeWOSOLightResLoadComponent& )( igdeResourceLoaderListener& )pResLoad;
	
	const decString pathLightSkin( GetStringProperty(
		pGDLight.GetPropertyName( igdeGDCLight::epLightSkin ), pGDLight.GetLightSkinPath() ) );
	if( ! pathLightSkin.IsEmpty() ){
		rl.LoadLightSkin( pathLightSkin );
	}
	
	rl.CheckFinished();
}

void igdeWOSOLight::pUpdateLight(){
	const igdeWOSOLightResLoadComponent &rl =
		( igdeWOSOLightResLoadComponent& )( igdeResourceLoaderListener& )pResLoad;
	
	if( ! pLight ){
		pLight.TakeOver( GetEngine().GetLightManager()->CreateLight() );
		
		pLight->SetHintMovement( pGDLight.GetHintMovement() );
		pLight->SetHintParameter( pGDLight.GetHintParameter() );
		UpdateLayerMasks();
		UpdateVisibility();
	}
	
	decString lightType;
	if( GetPropertyValue( pGDLight.GetPropertyName( igdeGDCLight::epType ), lightType ) ){
		if( lightType == "point" ){
			pLight->SetType( deLight::eltPoint );
			
		}else if( lightType == "spot" ){
			pLight->SetType( deLight::eltSpot );
			
		}else if( lightType == "projector" ){
			pLight->SetType( deLight::eltProjector );
			
		}else{
			pLight->SetType( pGDLight.GetType() );
		}
		
	}else{
		pLight->SetType( pGDLight.GetType() );
	}
	
	pLight->SetColor( GetColor3Property(
		pGDLight.GetPropertyName( igdeGDCLight::epColor ),
		pGDLight.GetColor() ) );
	pLight->SetIntensity( GetFloatProperty(
		pGDLight.GetPropertyName( igdeGDCLight::epIntensity ),
		pGDLight.GetIntensity() ) );
	pLight->SetRange( GetFloatProperty(
		pGDLight.GetPropertyName( igdeGDCLight::epRange ),
		pGDLight.GetRange() ) );
	pLight->SetAmbientRatio( GetFloatProperty(
		pGDLight.GetPropertyName( igdeGDCLight::epAmbientRatio ),
		pGDLight.GetAmbientRatio() ) );
	pLight->SetHalfIntensityDistance( GetFloatProperty(
		pGDLight.GetPropertyName( igdeGDCLight::epHalfIntDist ),
		pGDLight.GetHalfIntensityDistance() ) );
	pLight->SetSpotAngle( GetFloatProperty(
		pGDLight.GetPropertyName( igdeGDCLight::epSpotAngle ),
		pGDLight.GetSpotAngle() ) * DEG2RAD );
	pLight->SetSpotRatio( GetFloatProperty(
		pGDLight.GetPropertyName( igdeGDCLight::epSpotRatio ),
		pGDLight.GetSpotRatio() ) );
	pLight->SetSpotSmoothness( GetFloatProperty(
		pGDLight.GetPropertyName( igdeGDCLight::epSpotSmoothness ),
		pGDLight.GetSpotSmoothness() ) );
	pLight->SetSpotExponent( GetFloatProperty(
		pGDLight.GetPropertyName( igdeGDCLight::epSpotExponent ),
		pGDLight.GetSpotExponent() ) );
	pLight->SetCastShadows( GetBoolProperty(
		pGDLight.GetPropertyName( igdeGDCLight::epCastShadows ),
		pGDLight.GetCastShadows() ) );
	
	deSkin *lightSkin = rl.GetLightSkin();
	if( ! lightSkin && rl.HasPathLightSkin() && GetWrapper().GetGDClass() ){
		lightSkin = GetEnvironment().GetErrorSkin();
	}
	pLight->SetLightSkin( lightSkin );
	
	pLight->SetHintLightImportance( GetIntProperty(
		pGDLight.GetPropertyName( igdeGDCLight::epHintLightImportance ),
		pGDLight.GetHintLightImportance() ) );
	pLight->SetHintShadowImportance( GetIntProperty(
		pGDLight.GetPropertyName( igdeGDCLight::epHintShadowImportance ),
		pGDLight.GetHintShadowImportance() ) );
	
	if( ! pAddedToWorld ){
		GetWrapper().GetWorld()->AddLight( pLight );
		pAddedToWorld = true;
	}
	if( pAddedToWorld && ! pAttachedToCollider ){
		AttachToCollider();
	}
	
	pResLoad = NULL;
}

void igdeWOSOLight::pDestroyLight(){
	if( ! pLight ){
		return;
	}
	
	DetachFromCollider();
	
	if( pAddedToWorld ){
		GetWrapper().GetWorld()->RemoveLight( pLight );
	}
	
	pLight = NULL;
	pAddedToWorld = false;
}

void igdeWOSOLight::AttachToCollider(){
	DetachFromCollider();
	
	if( ! pLight ){
		return;
	}
	
	deColliderComponent * const colliderComponent = GetAttachableColliderComponent();
	deColliderVolume * const colliderFallback = GetWrapper().GetColliderFallback();
	deColliderAttachment *attachment = NULL;
	
	try{
		attachment = new deColliderAttachment( pLight );
		attachment->SetAttachType( deColliderAttachment::eatStatic );
		attachment->SetPosition( GetVectorProperty(
			pGDLight.GetPropertyName( igdeGDCLight::epAttachPosition ),
			pGDLight.GetPosition() ) );
		attachment->SetOrientation( GetRotationProperty(
			pGDLight.GetPropertyName( igdeGDCLight::epAttachRotation ),
			pGDLight.GetOrientation() ) );
		
		if( colliderComponent ){
			if( ! pGDLight.GetBoneName().IsEmpty() ){
				attachment->SetAttachType( deColliderAttachment::eatBone );
				attachment->SetTrackBone( pGDLight.GetBoneName() );
			}
			colliderComponent->AddAttachment( attachment );
			pAttachedToCollider = colliderComponent;
			
		}else{
			colliderFallback->AddAttachment( attachment );
			pAttachedToCollider = colliderFallback;
		}
		
		pAttachment = attachment;
		
	}catch( const deException & ){
		if( attachment ){
			delete attachment;
		}
		throw;
	}
}

void igdeWOSOLight::DetachFromCollider(){
	if( ! pAttachedToCollider ){
		return;
	}
	
	pAttachedToCollider->RemoveAttachment( pAttachment );
	pAttachment = NULL;
	pAttachedToCollider = NULL;
}
