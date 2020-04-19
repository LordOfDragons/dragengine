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

#include "igdeWOSOParticleEmitter.h"
#include "igdeWOSOVisitor.h"
#include "../igdeWObject.h"
#include "../../../engine/igdeEngineController.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../gamedefinition/igdeGameDefinition.h"
#include "../../../gamedefinition/class/igdeGDClass.h"
#include "../../../gamedefinition/class/particleemitter/igdeGDCParticleEmitter.h"
#include "../../../gameproject/igdeGameProject.h"
#include "../../../resourceloader/igdeResourceLoaderTask.h"
#include "../../../resourceloader/igdeResourceLoaderListener.h"
#include "../../../triggersystem/igdeTriggerExpression.h"
#include "../../../triggersystem/igdeTriggerExpressionParser.h"
#include "../../../triggersystem/igdeTriggerTargetList.h"
#include "../../../loadsave/igdeLoadParticleEmitter.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterManager.h>
#include <dragengine/resources/particle/deParticleEmitterReference.h>
#include <dragengine/resources/particle/deParticleEmitterInstance.h>
#include <dragengine/resources/particle/deParticleEmitterInstanceManager.h>
#include <dragengine/resources/particle/deParticleEmitterInstanceType.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/world/deWorld.h>



// Resource loading
/////////////////////

class igdeWOSOParticleEmitterResLoadComponent : public igdeResourceLoaderListener{
private:
	igdeWOSOParticleEmitter *pOwner;
	int pCounter;
	bool pSuccess;
	
public:
	igdeWOSOParticleEmitterResLoadComponent( igdeWOSOParticleEmitter &owner ) :
	pOwner( &owner ), pCounter( 0 ), pSuccess( true ){
	}
	
	virtual ~igdeWOSOParticleEmitterResLoadComponent(){
	}
	
	void Drop(){
		pOwner = NULL;
	}
	
	virtual void LoadingFinished( const igdeResourceLoaderTask &task, deFileResource *resource){
		if( ! pOwner ){
			return;
		}
		CheckFinished();
	}
	
	virtual void LoadingFailed( const igdeResourceLoaderTask &task ){
		if( ! pOwner ){
			return;
		}
		CheckFinished();
	}
	
	void CheckFinished(){
		if( pOwner && pCounter == 0 ){
			pOwner->AsyncLoadFinished( pSuccess );
		}
	}
};


// Class igdeWOSOParticleEmitter
//////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWOSOParticleEmitter::igdeWOSOParticleEmitter( igdeWObject &wrapper,
	const igdeGDCParticleEmitter &gdParticleEmitter, const decString &prefix ) :
igdeWOSubObject( wrapper, prefix ),
pGDParticleEmitter( gdParticleEmitter ),
pAddedToWorld( false ),
pAttachment( NULL )
{
	pLoadResources();
}

igdeWOSOParticleEmitter::~igdeWOSOParticleEmitter(){
	if( pResLoad ){
		( ( igdeWOSOParticleEmitterResLoadComponent& )( igdeResourceLoaderListener& )pResLoad ).Drop();
		pResLoad = NULL;
	}
	pDestroyParticleEmitter();
	pClearTrigger( pTriggerCasting );
}



// Management
///////////////

void igdeWOSOParticleEmitter::UpdateParameters(){
	pLoadResources();
}

void igdeWOSOParticleEmitter::InitTriggers(){
	pInitTrigger( pTriggerCasting, pGDParticleEmitter.GetTriggerName( igdeGDCParticleEmitter::etCasting ) );
}

void igdeWOSOParticleEmitter::UpdateTriggers(){
	if( ! pParticleEmitter ){
		return;
	}
	
	if( GetWrapper().GetVisible() ){
		if( pTriggerCasting ){
			pTriggerCasting->Evaluate();
			pParticleEmitter->SetEnableCasting( pTriggerCasting->GetResult() );
			
		}else{
			pParticleEmitter->SetEnableCasting( GetBoolProperty(
				pGDParticleEmitter.GetPropertyName( igdeGDCParticleEmitter::epCasting ),
				pGDParticleEmitter.GetCasting() ) );
		}
		
	}else{
		pParticleEmitter->SetEnableCasting( false );
	}
}

void igdeWOSOParticleEmitter::UpdateVisibility(){
	if( pParticleEmitter ){
		pParticleEmitter->SetEnableCasting( GetWrapper().GetVisible() );
	}
}

void igdeWOSOParticleEmitter::UpdateLayerMasks(){
}

void igdeWOSOParticleEmitter::UpdateCollisionFilter(){
	if( pParticleEmitter ){
		pParticleEmitter->SetCollisionFilter( GetWrapper().GetCollisionFilterParticles() );
	}
}

void igdeWOSOParticleEmitter::OnAllSubObjectsFinishedLoading(){
	pUpdateParticleEmitter();
}

void igdeWOSOParticleEmitter::Visit( igdeWOSOVisitor &visitor ){
	visitor.VisitParticleEmitter( *this );
}

void igdeWOSOParticleEmitter::AsyncLoadFinished( bool success ){
	if( ! pResLoad ){
		return;
	}
	
	( ( igdeWOSOParticleEmitterResLoadComponent& )( igdeResourceLoaderListener& )pResLoad ).Drop();
	
	GetWrapper().SubObjectFinishedLoading( *this, success );
}



// Private Functions
//////////////////////

void igdeWOSOParticleEmitter::pLoadResources(){
	if( pResLoad ){
		( ( igdeWOSOParticleEmitterResLoadComponent& )( igdeResourceLoaderListener& )pResLoad ).Drop();
		pResLoad = NULL;
	}
	
	pResLoad.TakeOver( new igdeWOSOParticleEmitterResLoadComponent( *this ) );
	igdeWOSOParticleEmitterResLoadComponent &rl =
		( igdeWOSOParticleEmitterResLoadComponent& )( igdeResourceLoaderListener& )pResLoad;
	
	rl.CheckFinished();
}

void igdeWOSOParticleEmitter::pUpdateParticleEmitter(){
	const igdeWOSOParticleEmitterResLoadComponent &rl =
		( igdeWOSOParticleEmitterResLoadComponent& )( igdeResourceLoaderListener& )pResLoad;
	(void)rl;
	
	if( ! pParticleEmitter ){
		pParticleEmitter.TakeOver( GetEngine().GetParticleEmitterInstanceManager()->CreateInstance() );
		
		UpdateLayerMasks();
		UpdateCollisionFilter();
		UpdateVisibility();
	}
	
	decString pathEmitter( GetStringProperty(
		pGDParticleEmitter.GetPropertyName( igdeGDCParticleEmitter::epPath ),
		pGDParticleEmitter.GetPath() ) );
	
	if( pathEmitter != pPathEmitter ){
		deParticleEmitterReference emitter( pParticleEmitter->GetEmitter() );
		
		if( ! pathEmitter.IsEmpty() ){
			igdeLoadParticleEmitter loadEmitter( GetEnvironment(), &GetLogger(), "DEIGDE" );
			const decPath vfsPath( decPath::CreatePathUnix( pathEmitter ) );
			deEngine &engine = GetEngine();
			
			if( engine.GetVirtualFileSystem()->ExistsFile( vfsPath ) ){
				try{
					decBaseFileReaderReference reader;
					reader.TakeOver( engine.GetVirtualFileSystem()->OpenFileForReading( vfsPath ) );
					emitter.TakeOver( engine.GetParticleEmitterManager()->CreateParticleEmitter() );
					loadEmitter.Load( pathEmitter, emitter, reader );
					
				}catch( const deException &e ){
					emitter = NULL;
					GetLogger().LogException( "DEIGDE", e );
				}
			}
		}
		
		pParticleEmitter->SetEmitter( emitter );
		pPathEmitter = pathEmitter;
	}
	
	if( ! pAddedToWorld ){
		GetWrapper().GetWorld()->AddParticleEmitter( pParticleEmitter );
		pAddedToWorld = true;
	}
	if( pAddedToWorld && ! pAttachedToCollider ){
		AttachToCollider();
	}
	
	pResLoad = NULL;
}

void igdeWOSOParticleEmitter::pDestroyParticleEmitter(){
	if( ! pParticleEmitter ){
		return;
	}
	
	DetachFromCollider();
	
	if( pAddedToWorld ){
		GetWrapper().GetWorld()->RemoveParticleEmitter( pParticleEmitter );
	}
	
	pParticleEmitter = NULL;
	pPathEmitter.Empty();
	pAddedToWorld = false;
}

void igdeWOSOParticleEmitter::AttachToCollider(){
	DetachFromCollider();
	
	if( ! pParticleEmitter ){
		return;
	}
	
	deColliderComponent * const colliderComponent = GetAttachableColliderComponent();
	deColliderVolume * const colliderFallback = GetWrapper().GetColliderFallback();
	deColliderAttachment *attachment = NULL;
	
	try{
		attachment = new deColliderAttachment( pParticleEmitter );
		attachment->SetAttachType( deColliderAttachment::eatStatic );
		attachment->SetPosition( GetVectorProperty(
			pGDParticleEmitter.GetPropertyName( igdeGDCParticleEmitter::epAttachPosition ),
			pGDParticleEmitter.GetPosition() ) );
		attachment->SetOrientation( GetRotationProperty(
			pGDParticleEmitter.GetPropertyName( igdeGDCParticleEmitter::epAttachRotation ),
			pGDParticleEmitter.GetOrientation() ) );
		
		if( colliderComponent ){
			if( ! pGDParticleEmitter.GetBoneName().IsEmpty() ){
				attachment->SetAttachType( deColliderAttachment::eatBone );
				attachment->SetTrackBone( pGDParticleEmitter.GetBoneName() );
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

void igdeWOSOParticleEmitter::DetachFromCollider(){
	if( ! pAttachedToCollider ){
		return;
	}
	
	pAttachedToCollider->RemoveAttachment( pAttachment );
	pAttachment = NULL;
	pAttachedToCollider = NULL;
}
