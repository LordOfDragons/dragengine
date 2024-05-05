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
	
	bool enableCasting = false;
	
	if( GetWrapper().GetVisible() ){
		if( pTriggerCasting ){
			pTriggerCasting->Evaluate();
			enableCasting = pTriggerCasting->GetResult();
			
		}else{
			enableCasting = GetBoolProperty(
				pGDParticleEmitter.GetPropertyName( igdeGDCParticleEmitter::epCasting ),
				pGDParticleEmitter.GetCasting() );
		}
	}
	
	if( enableCasting != pParticleEmitter->GetEnableCasting() ){
		if( enableCasting ){
			pParticleEmitter->ResetBurst();
		}
		pParticleEmitter->SetEnableCasting( enableCasting );
	}
}

void igdeWOSOParticleEmitter::UpdateVisibility(){
	UpdateTriggers();
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
		
		InitTriggers(); // this has to come first or UpdateVisibility can trigger casting
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
