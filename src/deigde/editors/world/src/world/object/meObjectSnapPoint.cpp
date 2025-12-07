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

meObjectSnapPoint::meObjectSnapPoint(meObject *object, igdeGDCSnapPoint *snapPoint) :
pWorld(NULL),
pObject(object),
pSnapPoint(snapPoint),
pDDShape(NULL),
pCollider(NULL),
pColliderOwner(this)
{
	if(! object || ! snapPoint){
		DETHROW(deeInvalidParam);
	}
	
	deEngine * const engine = object->GetEnvironment()->GetEngineController()->GetEngine();
	
	try{
		// create collider
		decLayerMask collisionCategory;
		collisionCategory.SetBit(meWorld::eclmSnapPoint);
		
		decLayerMask collisionFilter;
		collisionFilter.SetBit(meWorld::eclmEditing);
		
		pCollider = engine->GetColliderManager()->CreateColliderVolume();
		pCollider->SetEnabled(true);
		pCollider->SetResponseType(deCollider::ertStatic);
		pCollider->SetUseLocalGravity(true);
		pCollider->SetCollisionFilter(decCollisionFilter(collisionCategory, collisionFilter));
		
		decShapeList colliderShape;
		colliderShape.Add(new decShapeSphere(snapPoint->GetSnapDistance()));
		pCollider->SetShapes(colliderShape);
		
		object->GetEnvironment()->SetColliderUserPointer(pCollider, &pColliderOwner);
		
		deColliderAttachment * const attachment = new deColliderAttachment(pCollider);
		attachment->SetAttachType(deColliderAttachment::eatStatic);
		attachment->SetPosition(snapPoint->GetPosition());
		attachment->SetOrientation(decQuaternion::CreateFromEuler(
			snapPoint->GetRotation() * DEG2RAD));
		object->GetColDetCollider()->AddAttachment(attachment);
		
		// create debug drawer shape
		pDDShape = new igdeWDebugDrawerShape;
		pDDShape->SetVisible(true);
		pDDShape->SetEdgeColor(decColor(0.0f, 0.5f, 0.0f, 0.5f));
		pDDShape->SetFillColor(decColor(0.0f, 0.5f, 0.0f, 0.1f));
		pDDShape->SetPosition(snapPoint->GetPosition());
		pDDShape->SetOrientation(decQuaternion::CreateFromEuler(
			snapPoint->GetRotation() * DEG2RAD));
		pDDShape->SetParentDebugDrawer(object->GetDebugDrawer());
		
		const float r1 = snapPoint->GetSnapDistance();
		const float r2 = r1 * 0.025f;
		pDDShape->AddBoxShape(decVector(r1, r2, r2), decVector(), decQuaternion());
		pDDShape->AddBoxShape(decVector(r2, r1, r2), decVector(), decQuaternion());
		pDDShape->AddBoxShape(decVector(r2, r2, r1), decVector(), decQuaternion());
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

meObjectSnapPoint::~meObjectSnapPoint(){
	pCleanUp();
}



// Management
///////////////

void meObjectSnapPoint::SetWorld(meWorld *world){
	if(world == pWorld){
		return;
	}
	
	if(pWorld){
		pWorld->GetEngineWorld()->RemoveCollider(pCollider);
	}
	
	pWorld = world;
	
	if(world){
		world->GetEngineWorld()->AddCollider(pCollider);
	}
}



// Private Functions
//////////////////////

void meObjectSnapPoint::pCleanUp(){
	SetWorld(NULL);
	
	if(pObject && pCollider){
		deColliderAttachment * const attachment =
			pObject->GetColDetCollider()->GetAttachmentWith(pCollider);
		if(attachment){
			pObject->GetColDetCollider()->RemoveAttachment(attachment);
		}
		
		pObject->GetEnvironment()->SetColliderUserPointer(pCollider, NULL);
	}
	
	if(pCollider){
		pCollider->FreeReference();
	}
	
	if(pDDShape){
		delete pDDShape;
	}
}
