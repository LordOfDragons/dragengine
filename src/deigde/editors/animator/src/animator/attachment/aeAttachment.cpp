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

#include "aeAttachment.h"
#include "../aeAnimator.h"

#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/wrapper/igdeWObject.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gameproject/igdeGameProject.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/world/deWorld.h>



// Definitions
////////////////

#define LOGSOURCE "Animator Editor"



// Class aeAttachment
///////////////////////

// Constructor, destructor
////////////////////////////

aeAttachment::aeAttachment( igdeEnvironment *environment, const char *name ) :
pAnimator( NULL ),
pObjectWrapper( NULL ),
pName( name ),
pAttachType( eatNone )
{
	try{
		decLayerMask layerMask;
		layerMask.SetBit( aeAnimator::eclElements );
		
		pObjectWrapper = new igdeWObject( *environment );
		pObjectWrapper->SetVisible( true );
		pObjectWrapper->SetDynamicCollider( false );
		pObjectWrapper->SetCollisionFilter( decCollisionFilter( layerMask ) );
		pObjectWrapper->SetCollisionFilterFallback( decCollisionFilter( layerMask ) );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

aeAttachment::~aeAttachment(){
	pCleanUp();
}



// Management
///////////////

void aeAttachment::SetAnimator( aeAnimator *animator ){
	if( animator == pAnimator ){
		return;
	}
	
	pAnimator = animator;
	
	if( animator ){
		pObjectWrapper->SetWorld( animator->GetEngineWorld() );
		AttachCollider();
		
	}else{
		DetachCollider();
		pObjectWrapper->SetWorld( NULL );
	}
}



void aeAttachment::SetName( const char *name ){
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	pName = name;
	
	if( pAnimator ){
		pAnimator->NotifyAttachmentChanged( this );
	}
}

void aeAttachment::SetAttachType( eAttachTypes type ){
	if( type == pAttachType ){
		return;
	}
	
	pAttachType = type;
	
	DetachCollider();
	AttachCollider();
	
	if( pAnimator ){
		pAnimator->AttachmentsForceUpdate();
	}
	ResetPhysics();
	
	if( pAnimator ){
		pAnimator->NotifyAttachmentChanged( this );
	}
}

void aeAttachment::SetBoneName( const char *name ){
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	if( pBoneName.Equals( name ) ){
		return;
	}
	
	pBoneName = name;
	
	DetachCollider();
	AttachCollider();
	
	if( pAnimator ){
		pAnimator->AttachmentsForceUpdate();
	}
	ResetPhysics();
	
	if( pAnimator ){
		pAnimator->NotifyAttachmentChanged( this );
	}
}



void aeAttachment::Update( float elapsed ){
	pObjectWrapper->Update( elapsed );
}

void aeAttachment::ResetPhysics(){
	pObjectWrapper->ResetPhysics();
}

void aeAttachment::AttachCollider(){
	if( ! pAnimator ){
		return;
	}
	
	try{
		switch( pAttachType ){
		case eatBone:
			if( ! pBoneName.IsEmpty() ){
				pObjectWrapper->AttachColliderBone( pAnimator->GetEngineCollider(), pBoneName.GetString(),
					pObjectWrapper->GetPosition(), pObjectWrapper->GetOrientation() );
			}
			break;
			
		case eatRig:
			pObjectWrapper->AttachColliderRig( pAnimator->GetEngineCollider() );
			break;
			
		default:
			break;
		};
		
	}catch( const deException &e ){
		pAnimator->GetLogger()->LogException( "Animator Editor", e );
	}
}

void aeAttachment::DetachCollider(){
	pObjectWrapper->DetachCollider();
}



// Private Functions
//////////////////////

void aeAttachment::pCleanUp(){
	SetAnimator( NULL );
	
	if( pObjectWrapper ){
		delete pObjectWrapper;
	}
}
