/* 
 * Drag[en]gine IGDE Animator Editor
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
