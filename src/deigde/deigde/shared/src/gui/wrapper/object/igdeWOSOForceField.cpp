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

#include "igdeWOSOForceField.h"
#include "igdeWOSOVisitor.h"
#include "../igdeWObject.h"
#include "../../../codec/igdeCodecPropertyString.h"
#include "../../../engine/igdeEngineController.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../gamedefinition/igdeGameDefinition.h"
#include "../../../gamedefinition/class/igdeGDClass.h"
#include "../../../gamedefinition/class/forcefield/igdeGDCForceField.h"
#include "../../../gameproject/igdeGameProject.h"
#include "../../../triggersystem/igdeTriggerExpression.h"
#include "../../../triggersystem/igdeTriggerExpressionParser.h"
#include "../../../triggersystem/igdeTriggerTargetList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/forcefield/deForceField.h>
#include <dragengine/resources/forcefield/deForceFieldManager.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/world/deWorld.h>



// Class igdeWOSOForceField
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWOSOForceField::igdeWOSOForceField( igdeWObject &wrapper,
	const igdeGDCForceField &gdForceField, const decString &prefix ) :
igdeWOSubObject( wrapper, prefix ),
pGDForceField( gdForceField ),
pAddedToWorld( false ),
pAttachment( NULL )
{
	AsyncLoadFinished( true ); // important since we load nothing
}

igdeWOSOForceField::~igdeWOSOForceField(){
	pDestroyForceField();
	pClearTrigger( pTriggerEnabled );
}



// Management
///////////////

void igdeWOSOForceField::UpdateParameters(){
	AsyncLoadFinished( true ); // important since we load nothing
}

void igdeWOSOForceField::InitTriggers(){
	pInitTrigger( pTriggerEnabled, pGDForceField.GetTriggerName( igdeGDCForceField::etEnabled ) );
}

void igdeWOSOForceField::UpdateTriggers(){
	if( ! pForceField ){
		return;
	}
	
	if( GetWrapper().GetVisible() ){
		if( pTriggerEnabled ){
			pTriggerEnabled->Evaluate();
			pForceField->SetEnabled( pTriggerEnabled->GetResult() );
			
		}else{
			pForceField->SetEnabled( GetBoolProperty(
				pGDForceField.GetPropertyName( igdeGDCForceField::epEnabled ),
				pGDForceField.GetEnabled() ) );
		}
		
	}else{
		pForceField->SetEnabled( false );
	}
}

void igdeWOSOForceField::UpdateVisibility(){
	if( pForceField ){
		pForceField->SetEnabled( GetWrapper().GetVisible() );
	}
}

void igdeWOSOForceField::UpdateLayerMasks(){
}

void igdeWOSOForceField::UpdateCollisionFilter(){
	if( pForceField ){
		pForceField->SetCollisionFilter( GetWrapper().GetCollisionFilterForceFields() );
	}
}

void igdeWOSOForceField::OnAllSubObjectsFinishedLoading(){
	pUpdateForceField();
}

void igdeWOSOForceField::Visit( igdeWOSOVisitor &visitor ){
	visitor.VisitForceField( *this );
}

void igdeWOSOForceField::AsyncLoadFinished( bool success ){
	GetWrapper().SubObjectFinishedLoading( *this, success );
}



// Private Functions
//////////////////////

void igdeWOSOForceField::pUpdateForceField(){
	if( ! pForceField ){
		pForceField.TakeOver( GetEngine().GetForceFieldManager()->CreateForceField() );
		
		UpdateLayerMasks();
		UpdateCollisionFilter();
		UpdateVisibility();
	}
	
	igdeCodecPropertyString codec;
	decShapeList shapeList;
	decString value;
	
	pForceField->SetRadius( GetFloatProperty(
		pGDForceField.GetPropertyName( igdeGDCForceField::epRadius ),
		pGDForceField.GetRadius() ) );
	pForceField->SetExponent( GetFloatProperty(
		pGDForceField.GetPropertyName( igdeGDCForceField::epExponent ),
		pGDForceField.GetExponent() ) );
	pForceField->SetForce( GetFloatProperty(
		pGDForceField.GetPropertyName( igdeGDCForceField::epForce ),
		pGDForceField.GetForce() ) );
	pForceField->SetFluctuationDirection( GetFloatProperty(
		pGDForceField.GetPropertyName( igdeGDCForceField::epFluctuationDirection ),
		pGDForceField.GetFluctuationDirection() ) * DEG2RAD );
	pForceField->SetFluctuationForce( GetFloatProperty(
		pGDForceField.GetPropertyName( igdeGDCForceField::epFluctuationForce ),
		pGDForceField.GetFluctuationForce() ) );
	pForceField->SetDirection( GetVectorProperty(
		pGDForceField.GetPropertyName( igdeGDCForceField::epDirection ),
		pGDForceField.GetDirection() ) );
	
	decString fieldType;
	if( GetPropertyValue( pGDForceField.GetPropertyName( igdeGDCForceField::epFieldType ), fieldType ) ){
		if( fieldType == "radial" ){
			pForceField->SetFieldType( deForceField::eftRadial );
			
		}else if( fieldType == "linear" ){
			pForceField->SetFieldType( deForceField::eftLinear );
			
		}else if( fieldType == "vortex" ){
			pForceField->SetFieldType( deForceField::eftVortex );
			
		}else{
			pForceField->SetFieldType( pGDForceField.GetFieldType() );
		}
		
	}else{
		pForceField->SetFieldType( pGDForceField.GetFieldType() );
	}
	
	decString applicationType;
	if( GetPropertyValue( pGDForceField.GetPropertyName( igdeGDCForceField::epApplicationType ), applicationType ) ){
		if( applicationType == "direct" ){
			pForceField->SetApplicationType( deForceField::eatDirect );
			
		}else if( applicationType == "surface" ){
			pForceField->SetApplicationType( deForceField::eatSurface );
			
		}else if( applicationType == "mass" ){
			pForceField->SetApplicationType( deForceField::eatMass );
			
		}else if( applicationType == "speed" ){
			pForceField->SetApplicationType( deForceField::eatSpeed );
			
		}else{
			pForceField->SetApplicationType( pGDForceField.GetApplicationType() );
		}
		
	}else{
		pForceField->SetApplicationType( pGDForceField.GetApplicationType() );
	}
	
	// influence area property
	if( GetPropertyValue( pGDForceField.GetPropertyName( igdeGDCForceField::epInfluenceArea ), value ) ){
		decShapeList list;
		codec.DecodeShapeList( value, list );
		pForceField->SetInfluenceArea( list );
		
	}else{
		pForceField->SetInfluenceArea( pGDForceField.GetInfluenceArea() );
	}
	
	// shape property
	if( GetPropertyValue( pGDForceField.GetPropertyName( igdeGDCForceField::epShape ), value ) ){
		decShapeList list;
		codec.DecodeShapeList( value, list );
		pForceField->SetShape( list );
		
	}else{
		pForceField->SetShape( pGDForceField.GetShape() );
	}
	
	if( ! pAddedToWorld ){
		GetWrapper().GetWorld()->AddForceField( pForceField );
		pAddedToWorld = true;
	}
	if( pAddedToWorld && ! pAttachedToCollider ){
		AttachToCollider();
	}
}

void igdeWOSOForceField::pDestroyForceField(){
	if( ! pForceField ){
		return;
	}
	
	DetachFromCollider();
	
	if( pAddedToWorld ){
		GetWrapper().GetWorld()->RemoveForceField( pForceField );
	}
	
	pForceField = NULL;
	pAddedToWorld = false;
}

void igdeWOSOForceField::AttachToCollider(){
	DetachFromCollider();
	
	if( ! pForceField ){
		return;
	}
	
	deColliderComponent * const colliderComponent = GetAttachableColliderComponent();
	deColliderVolume * const colliderFallback = GetWrapper().GetColliderFallback();
	deColliderAttachment *attachment = NULL;
	
	try{
		attachment = new deColliderAttachment( pForceField );
		attachment->SetAttachType( deColliderAttachment::eatStatic );
		attachment->SetPosition( GetVectorProperty(
			pGDForceField.GetPropertyName( igdeGDCForceField::epAttachPosition ),
			pGDForceField.GetPosition() ) );
		attachment->SetOrientation( GetRotationProperty(
			pGDForceField.GetPropertyName( igdeGDCForceField::epAttachRotation ),
			pGDForceField.GetOrientation() ) );
		
		if( colliderComponent ){
			if( ! pGDForceField.GetBoneName().IsEmpty() ){
				attachment->SetAttachType( deColliderAttachment::eatBone );
				attachment->SetTrackBone( pGDForceField.GetBoneName() );
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

void igdeWOSOForceField::DetachFromCollider(){
	if( ! pAttachedToCollider ){
		return;
	}
	
	pAttachedToCollider->RemoveAttachment( pAttachment );
	pAttachment = NULL;
	pAttachedToCollider = NULL;
}
