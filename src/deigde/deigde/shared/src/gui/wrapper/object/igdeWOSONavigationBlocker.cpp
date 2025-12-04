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

#include "igdeWOSONavigationBlocker.h"
#include "igdeWOSOVisitor.h"
#include "../igdeWObject.h"
#include "../../../codec/igdeCodecPropertyString.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../gamedefinition/igdeGameDefinition.h"
#include "../../../gamedefinition/class/igdeGDClass.h"
#include "../../../gamedefinition/class/navblocker/igdeGDCNavigationBlocker.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReader::Ref.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/resources/navigation/blocker/deNavigationBlocker.h>
#include <dragengine/resources/navigation/blocker/deNavigationBlockerManager.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/world/deWorld.h>



// Class igdeWOSONavigationBlocker
//////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWOSONavigationBlocker::igdeWOSONavigationBlocker( igdeWObject &wrapper,
	const igdeGDCNavigationBlocker &gdNavigationBlocker, const decString &prefix ) :
igdeWOSubObject( wrapper, prefix ),
pGDNavigationBlocker( gdNavigationBlocker ),
pAddedToWorld( false ),
pAttachment( NULL )
{
	wrapper.SubObjectFinishedLoading( *this, true );
}

igdeWOSONavigationBlocker::~igdeWOSONavigationBlocker(){
	pDestroyNavigationBlocker();
}



// Management
///////////////

void igdeWOSONavigationBlocker::UpdateParameters(){
	GetWrapper().SubObjectFinishedLoading( *this, true );
}

void igdeWOSONavigationBlocker::OnAllSubObjectsFinishedLoading(){
	pUpdateNavigationBlocker();
}

void igdeWOSONavigationBlocker::Visit( igdeWOSOVisitor &visitor ){
	visitor.VisitNavigationBlocker( *this );
}



// Private Functions
//////////////////////

void igdeWOSONavigationBlocker::pUpdateNavigationBlocker(){
	if( ! pNavigationBlocker ){
		pNavigationBlocker.TakeOver( GetEngine().GetNavigationBlockerManager()->CreateNavigationBlocker() );
		pNavigationBlocker->SetSpaceType( pGDNavigationBlocker.GetType() );
	}
	
	igdeCodecPropertyString codec;
	decShapeList shapeList;
	decString value;
	
	pNavigationBlocker->SetLayer( GetIntProperty(
		pGDNavigationBlocker.GetPropertyName( igdeGDCNavigationBlocker::epLayer ),
		pGDNavigationBlocker.GetLayer() ) );
	pNavigationBlocker->SetBlockingPriority( GetIntProperty(
		pGDNavigationBlocker.GetPropertyName( igdeGDCNavigationBlocker::epBlockingPriority ),
		pGDNavigationBlocker.GetBlockingPriority() ) );
	pNavigationBlocker->SetEnabled( GetBoolProperty(
		pGDNavigationBlocker.GetPropertyName( igdeGDCNavigationBlocker::epEnabled ),
		pGDNavigationBlocker.GetEnabled() ) );
	
	// shape
	if( GetPropertyValue( pGDNavigationBlocker.GetPropertyName( igdeGDCNavigationBlocker::epShape ), value ) ){
		codec.DecodeShapeList( value, pNavigationBlocker->GetShapeList() );
		
	}else{
		pNavigationBlocker->GetShapeList() = pGDNavigationBlocker.GetShapeList();
	}
	pNavigationBlocker->NotifyShapeListChanged();
	
	if( ! pAddedToWorld ){
		GetWrapper().GetWorld()->AddNavigationBlocker( pNavigationBlocker );
		pAddedToWorld = true;
	}
	if( pAddedToWorld && ! pAttachedToCollider ){
		AttachToCollider();
	}
}

void igdeWOSONavigationBlocker::pDestroyNavigationBlocker(){
	if( ! pNavigationBlocker ){
		return;
	}
	
	DetachFromCollider();
	
	if( pAddedToWorld ){
		GetWrapper().GetWorld()->RemoveNavigationBlocker( pNavigationBlocker );
	}
	
	pNavigationBlocker = NULL;
	pAddedToWorld = false;
}

void igdeWOSONavigationBlocker::AttachToCollider(){
	DetachFromCollider();
	
	if( ! pNavigationBlocker ){
		return;
	}
	
	deColliderComponent * const colliderComponent = GetAttachableColliderComponent();
	deColliderVolume * const colliderFallback = GetWrapper().GetColliderFallback();
	deColliderAttachment *attachment = NULL;
	
	try{
		attachment = new deColliderAttachment( pNavigationBlocker );
		attachment->SetAttachType( deColliderAttachment::eatStatic );
		attachment->SetPosition( GetVectorProperty(
			pGDNavigationBlocker.GetPropertyName( igdeGDCNavigationBlocker::epAttachPosition ),
			pGDNavigationBlocker.GetPosition() ) );
		attachment->SetOrientation( GetRotationProperty(
			pGDNavigationBlocker.GetPropertyName( igdeGDCNavigationBlocker::epAttachRotation ),
			pGDNavigationBlocker.GetOrientation() ) );
		
		if( colliderComponent ){
			if( ! pGDNavigationBlocker.GetBoneName().IsEmpty() ){
				attachment->SetAttachType( deColliderAttachment::eatBone );
				attachment->SetTrackBone( pGDNavigationBlocker.GetBoneName() );
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

void igdeWOSONavigationBlocker::DetachFromCollider(){
	if( ! pAttachedToCollider ){
		return;
	}
	
	pAttachedToCollider->RemoveAttachment( pAttachment );
	pAttachment = NULL;
	pAttachedToCollider = NULL;
}
