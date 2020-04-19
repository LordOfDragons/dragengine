/* 
 * Drag[en]gine IGDE World Editor
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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "meObject.h"
#include "meObjectSnapPoint.h"
#include "../meWorld.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/class/snappoint/igdeGDCSnapPoint.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/world/deWorld.h>



// Definitions
////////////////

#define LOGSOURCE "World Editor"



// Class meObjectSnapPoint
////////////////////////////

// Constructor, destructor
////////////////////////////

meObjectSnapPoint::meObjectSnapPoint( meObject *object, igdeGDCSnapPoint *snapPoint ) :
pWorld( NULL ),
pObject( object ),
pSnapPoint( snapPoint ),
pDDShape( NULL ),
pCollider( NULL ),
pColliderOwner( this )
{
	if( ! object || ! snapPoint ){
		DETHROW( deeInvalidParam );
	}
	
	deEngine * const engine = object->GetEnvironment()->GetEngineController()->GetEngine();
	
	try{
		// create collider
		decLayerMask collisionCategory;
		collisionCategory.SetBit( meWorld::eclmSnapPoint );
		
		decLayerMask collisionFilter;
		collisionFilter.SetBit( meWorld::eclmEditing );
		
		pCollider = engine->GetColliderManager()->CreateColliderVolume();
		pCollider->SetEnabled( true );
		pCollider->SetResponseType( deCollider::ertStatic );
		pCollider->SetUseLocalGravity( true );
		pCollider->SetCollisionFilter( decCollisionFilter( collisionCategory, collisionFilter ) );
		
		decShapeList colliderShape;
		colliderShape.Add( new decShapeSphere( snapPoint->GetSnapDistance() ) );
		pCollider->SetShapes( colliderShape );
		
		object->GetEnvironment()->SetColliderUserPointer( pCollider, &pColliderOwner );
		
		deColliderAttachment * const attachment = new deColliderAttachment( pCollider );
		attachment->SetAttachType( deColliderAttachment::eatStatic );
		attachment->SetPosition( snapPoint->GetPosition() );
		attachment->SetOrientation( decQuaternion::CreateFromEuler(
			snapPoint->GetRotation() * DEG2RAD ) );
		object->GetColDetCollider()->AddAttachment( attachment );
		
		// create debug drawer shape
		pDDShape = new igdeWDebugDrawerShape;
		pDDShape->SetVisible( true );
		pDDShape->SetEdgeColor( decColor( 0.0f, 0.5f, 0.0f, 0.5f ) );
		pDDShape->SetFillColor( decColor( 0.0f, 0.5f, 0.0f, 0.1f ) );
		pDDShape->SetPosition( snapPoint->GetPosition() );
		pDDShape->SetOrientation( decQuaternion::CreateFromEuler(
			snapPoint->GetRotation() * DEG2RAD ) );
		pDDShape->SetParentDebugDrawer( object->GetDebugDrawer() );
		
		const float r1 = snapPoint->GetSnapDistance();
		const float r2 = r1 * 0.025f;
		pDDShape->AddBoxShape( decVector( r1, r2, r2 ), decVector(), decQuaternion() );
		pDDShape->AddBoxShape( decVector( r2, r1, r2 ), decVector(), decQuaternion() );
		pDDShape->AddBoxShape( decVector( r2, r2, r1 ), decVector(), decQuaternion() );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

meObjectSnapPoint::~meObjectSnapPoint(){
	pCleanUp();
}



// Management
///////////////

void meObjectSnapPoint::SetWorld( meWorld *world ){
	if( world == pWorld ){
		return;
	}
	
	if( pWorld ){
		pWorld->GetEngineWorld()->RemoveCollider( pCollider );
	}
	
	pWorld = world;
	
	if( world ){
		world->GetEngineWorld()->AddCollider( pCollider );
	}
}



// Private Functions
//////////////////////

void meObjectSnapPoint::pCleanUp(){
	SetWorld( NULL );
	
	if( pObject && pCollider ){
		deColliderAttachment * const attachment =
			pObject->GetColDetCollider()->GetAttachmentWith( pCollider );
		if( attachment ){
			pObject->GetColDetCollider()->RemoveAttachment( attachment );
		}
		
		pObject->GetEnvironment()->SetColliderUserPointer( pCollider, NULL );
	}
	
	if( pCollider ){
		pCollider->FreeReference();
	}
	
	if( pDDShape ){
		delete pDDShape;
	}
}
