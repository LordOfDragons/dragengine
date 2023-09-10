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

#include "igdeWOSOSpeaker.h"
#include "igdeWOSOVisitor.h"
#include "../igdeWObject.h"
#include "../../../engine/igdeEngineController.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../gamedefinition/igdeGameDefinition.h"
#include "../../../gamedefinition/class/igdeGDClass.h"
#include "../../../gamedefinition/class/speaker/igdeGDCSpeaker.h"
#include "../../../gameproject/igdeGameProject.h"
#include "../../../resourceloader/igdeResourceLoaderTask.h"
#include "../../../resourceloader/igdeResourceLoaderListener.h"
#include "../../../triggersystem/igdeTriggerExpression.h"
#include "../../../triggersystem/igdeTriggerExpressionParser.h"
#include "../../../triggersystem/igdeTriggerTargetList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/sound/deSpeaker.h>
#include <dragengine/resources/sound/deSpeakerManager.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/sound/deSound.h>
#include <dragengine/resources/sound/deSoundManager.h>
#include <dragengine/resources/sound/deSoundReference.h>
#include <dragengine/resources/world/deWorld.h>



// Resource loading
/////////////////////

class igdeWOSOSpeakerResLoadComponent : public igdeResourceLoaderListener{
private:
	igdeWOSOSpeaker *pOwner;
	decString pPathSound;
	deSoundReference pSound;
	int pCounter;
	bool pSuccess;
	
public:
	igdeWOSOSpeakerResLoadComponent( igdeWOSOSpeaker &owner ) :
	pOwner( &owner ), pCounter( 0 ), pSuccess( true ){
	}
	
	virtual ~igdeWOSOSpeakerResLoadComponent(){
	}
	
	void Drop(){
		pOwner = NULL;
	}
	
	void LoadSound( const char *path ){
		pPathSound = path;
		pOwner->GetWrapper().GetEnvironment().AsyncLoadResource( path, deResourceLoader::ertSound, this );
		pCounter++;
	}
	inline deSound *GetSound() const{ return pSound; }
	
	virtual void LoadingFinished( const igdeResourceLoaderTask &task, deFileResource *resource){
		if( ! pOwner ){
			return;
		}
		
		const deResourceLoader::eResourceType type = task.GetResourceType();
		const decString &filename = task.GetFilename();
		
		if( type == deResourceLoader::ertSound && pPathSound == filename ){
			pSound = ( deSound* )resource;
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
		
		if( type == deResourceLoader::ertSound && pPathSound == filename ){
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


// Class igdeWOSOSpeaker
/////////////////////////

// Constructor, destructor
////////////////////////////

igdeWOSOSpeaker::igdeWOSOSpeaker( igdeWObject &wrapper,
	const igdeGDCSpeaker &gdSpeaker, const decString &prefix ) :
igdeWOSubObject( wrapper, prefix ),
pGDSpeaker( gdSpeaker ),
pAddedToWorld( false ),
pAttachment( NULL )
{
	pLoadResources();
}

igdeWOSOSpeaker::~igdeWOSOSpeaker(){
	if( pResLoad ){
		( ( igdeWOSOSpeakerResLoadComponent& )( igdeResourceLoaderListener& )pResLoad ).Drop();
		pResLoad = NULL;
	}
	pDestroySpeaker();
	pClearTrigger( pTriggerPlaying );
	pClearTrigger( pTriggerMuted );
}



// Management
///////////////

void igdeWOSOSpeaker::UpdateParameters(){
	pLoadResources();
}

void igdeWOSOSpeaker::InitTriggers(){
	pInitTrigger( pTriggerPlaying, pGDSpeaker.GetTriggerName( igdeGDCSpeaker::etPlaying ) );
	pInitTrigger( pTriggerMuted, pGDSpeaker.GetTriggerName( igdeGDCSpeaker::etMuted ) );
}

void igdeWOSOSpeaker::UpdateTriggers(){
	if( ! pSpeaker ){
		return;
	}
	
	if( GetWrapper().GetVisible() ){
		if( pTriggerMuted ){
			pTriggerMuted->Evaluate();
			pSpeaker->SetMuted( pTriggerMuted->GetResult() );
			
		}else{
			pSpeaker->SetMuted( false );
		}
		
		if( pTriggerPlaying ){
			pTriggerPlaying->Evaluate();
		}
		
		if( pEvalPlaying() ){
			pSpeaker->Play();
			
		}else{
			pSpeaker->Stop();
		}
		
	}else{
		pSpeaker->SetMuted( false );
		pSpeaker->Stop();
	}
}

void igdeWOSOSpeaker::UpdateVisibility(){
	if( ! pSpeaker ){
		return;
	}
	
	if( pEvalPlaying() ){
		pSpeaker->Play();
		
	}else{
		pSpeaker->Stop();
	}
}

void igdeWOSOSpeaker::UpdateLayerMasks(){
	if( pSpeaker ){
		pSpeaker->SetLayerMask( LayerMaskFromInt( GetWrapper().GetAudioLayerMask() ) );
	}
}

void igdeWOSOSpeaker::OnAllSubObjectsFinishedLoading(){
	pUpdateSpeaker();
}

void igdeWOSOSpeaker::Visit( igdeWOSOVisitor &visitor ){
	visitor.VisitSpeaker( *this );
}

void igdeWOSOSpeaker::AsyncLoadFinished( bool success ){
	if( ! pResLoad ){
		return;
	}
	
	( ( igdeWOSOSpeakerResLoadComponent& )( igdeResourceLoaderListener& )pResLoad ).Drop();
	
	GetWrapper().SubObjectFinishedLoading( *this, success );
}



// Private Functions
//////////////////////

void igdeWOSOSpeaker::pLoadResources(){
	if( pResLoad ){
		( ( igdeWOSOSpeakerResLoadComponent& )( igdeResourceLoaderListener& )pResLoad ).Drop();
		pResLoad = NULL;
	}
	
	pResLoad.TakeOver( new igdeWOSOSpeakerResLoadComponent( *this ) );
	igdeWOSOSpeakerResLoadComponent &rl =
		( igdeWOSOSpeakerResLoadComponent& )( igdeResourceLoaderListener& )pResLoad;
	
	const decString pathSound( GetStringProperty(
		pGDSpeaker.GetPropertyName( igdeGDCSpeaker::epSound ), pGDSpeaker.GetPathSound() ) );
	if( ! pathSound.IsEmpty() ){
		rl.LoadSound( pathSound );
	}
	
	rl.CheckFinished();
}

void igdeWOSOSpeaker::pUpdateSpeaker(){
	const igdeWOSOSpeakerResLoadComponent &rl =
		( igdeWOSOSpeakerResLoadComponent& )( igdeResourceLoaderListener& )pResLoad;
	
	if( ! pSpeaker ){
		pSpeaker.TakeOver( GetEngine().GetSpeakerManager()->CreateSpeaker() );
		
		UpdateLayerMasks();
		UpdateVisibility();
	}
	
	pSpeaker->SetSound( rl.GetSound() );
	pSpeaker->SetLooping( GetBoolProperty(
		pGDSpeaker.GetPropertyName( igdeGDCSpeaker::epLooping ),
		pGDSpeaker.GetLooping() ) );
	pSpeaker->SetVolume( GetFloatProperty(
		pGDSpeaker.GetPropertyName( igdeGDCSpeaker::epVolume ),
		pGDSpeaker.GetVolume() ) );
	pSpeaker->SetRange( GetFloatProperty(
		pGDSpeaker.GetPropertyName( igdeGDCSpeaker::epRange ),
		pGDSpeaker.GetRange() ) );
	pSpeaker->SetRollOff( GetFloatProperty(
		pGDSpeaker.GetPropertyName( igdeGDCSpeaker::epRollOff ),
		pGDSpeaker.GetRollOff() ) );
	pSpeaker->SetDistanceOffset( GetFloatProperty(
		pGDSpeaker.GetPropertyName( igdeGDCSpeaker::epDistanceOffset ),
		pGDSpeaker.GetDistanceOffset() ) );
	pSpeaker->SetPlaySpeed( GetFloatProperty(
		pGDSpeaker.GetPropertyName( igdeGDCSpeaker::epPlaySpeed ),
		pGDSpeaker.GetPlaySpeed() ) );
	
	if( ! pAddedToWorld ){
		GetWrapper().GetWorld()->AddSpeaker( pSpeaker );
		pAddedToWorld = true;
	}
	if( pAddedToWorld && ! pAttachedToCollider ){
		AttachToCollider();
	}
	
	pResLoad = nullptr;
}

void igdeWOSOSpeaker::pDestroySpeaker(){
	if( ! pSpeaker ){
		return;
	}
	
	DetachFromCollider();
	
	if( pAddedToWorld ){
		GetWrapper().GetWorld()->RemoveSpeaker( pSpeaker );
	}
	
	pSpeaker = NULL;
	pAddedToWorld = false;
}

void igdeWOSOSpeaker::AttachToCollider(){
	DetachFromCollider();
	
	if( ! pSpeaker ){
		return;
	}
	
	deColliderComponent * const colliderComponent = GetAttachableColliderComponent();
	deColliderVolume * const colliderFallback = GetWrapper().GetColliderFallback();
	deColliderAttachment *attachment = nullptr;
	
	try{
		attachment = new deColliderAttachment( pSpeaker );
		attachment->SetAttachType( deColliderAttachment::eatStatic );
		attachment->SetPosition( GetVectorProperty(
			pGDSpeaker.GetPropertyName( igdeGDCSpeaker::epAttachPosition ),
			pGDSpeaker.GetPosition() ) );
		attachment->SetOrientation( GetRotationProperty(
			pGDSpeaker.GetPropertyName( igdeGDCSpeaker::epAttachRotation ),
			pGDSpeaker.GetOrientation() ) );
		
		if( colliderComponent ){
			if( ! pGDSpeaker.GetBoneName().IsEmpty() ){
				attachment->SetAttachType( deColliderAttachment::eatBone );
				attachment->SetTrackBone( pGDSpeaker.GetBoneName() );
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

void igdeWOSOSpeaker::DetachFromCollider(){
	if( ! pAttachedToCollider ){
		return;
	}
	
	pAttachedToCollider->RemoveAttachment( pAttachment );
	pAttachment = nullptr;
	pAttachedToCollider = nullptr;
}

bool igdeWOSOSpeaker::pEvalPlaying(){
	if( ! GetWrapper().GetVisible() ){
		return false;
	}
	
	if( pTriggerPlaying ){
		return pTriggerPlaying->GetResult();
	}
	
	return GetBoolProperty( pGDSpeaker.GetPropertyName( igdeGDCSpeaker::epPlaying ),
		pGDSpeaker.GetPlaying() );
}
