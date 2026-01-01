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

#include "meUObjectDropToGround.h"
#include "../../../collisions/meCLClosestElement.h"
#include "../../../filter/meFilterObjectsByClass.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObjectSelection.h"

#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/systems/modules/physics/deBasePhysicsWorld.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/common/exceptions.h>


// Class meUObjectDropToGround
////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectDropToGround::meUObjectDropToGround(meWorld *world, const meObject::List &objects) :

pDropOnObjects(true),
pAlign(false)
{
	DEASSERT_NOTNULL(world)
	DEASSERT_TRUE(objects.IsNotEmpty())
	
	SetShortInfo("Drop objects to ground");
	
	decString text;
	if(objects.GetCount() > 1){
		text.Format("%d objects", objects.GetCount());
		
	}else{
		text = "1 object";
	}
	SetLongInfo(text);
	
	meUndoDataObject::AddObjectsWithAttachments(
		objects.GetSorted([&](const meObject &a, const meObject &b){
			const double d = a.GetPosition().y - b.GetPosition().y;
			return d < -DOUBLE_SAFE_EPSILON ? -1 : (d > DOUBLE_SAFE_EPSILON ? 1 : 0);
		}),
		pObjects);
	
	pWorld = world;
}

meUObjectDropToGround::~meUObjectDropToGround(){
}



// Management
///////////////

void meUObjectDropToGround::SetDropOnObjects(bool dropOnObjects){
	pDropOnObjects = dropOnObjects;
}

void meUObjectDropToGround::SetAlign(bool align){
	pAlign = align;
}



void meUObjectDropToGround::Undo(){
	meUndoDataObject::RestoreOldGeometry(pObjects, pWorld);
}

void meUObjectDropToGround::Redo(){
	deBasePhysicsWorld *phyWorld = pWorld->GetEngineWorld()->GetPeerPhysics();
	decVector dropDisplacement(0.0f, -10.0f, 0.0f);
	meCLClosestElement closestElement(*pWorld);
	decDVector oldPosition, newPosition;
	decVector oldRotation, newRotation;
	deColliderVolume *collider;
	int testCounter = 0;
	float distance;
	
	if(!phyWorld) return;
	
	closestElement.SetTestHeightTerrain(true);
	closestElement.SetTestObjects(pDropOnObjects);
	closestElement.SetTestDecals(false);
	
	const meFilterObjectsByClass::Ref filter(meFilterObjectsByClass::Ref::New());
	filter->SetRejectGhosts(true);
	//filter->SetClassNamesFrom(guiparams.GetAddFilterObjectList());
	//filter->SetMatchInclusive(guiparams.GetAddFilterObjectInclusive());
	
	pObjects.Visit([&](const meUndoDataObject &data){
		filter->GetRejectObjects().Add(data.GetObject());
		data.GetAttachedObjects().Visit([&](const meUndoDataObject &attachedData){
			filter->GetRejectObjects().Add(attachedData.GetObject());
		});
	});
	
	closestElement.SetFilterObjects(filter);
	
	decLayerMask collisionCategory;
	collisionCategory.SetBit(meWorld::eclmEditing);
	
	decLayerMask collisionFilter;
	collisionFilter.SetBit(meWorld::eclmObjects);
	collisionFilter.SetBit(meWorld::eclmHeightTerrains);
	
	
	pObjects.Visit([&](const meUndoDataObject &data){
		meObject * const object = data.GetObject();
		
		collider = object->GetColDetCollider();
		if(!collider){
			return;
		}
		
		collider->SetCollisionFilter(decCollisionFilter(collisionCategory, collisionFilter));
		
		oldPosition = data.GetOldPosition();
		oldRotation = data.GetOldRotation();
		
		while(testCounter < 50){
			closestElement.Reset();
			
			collider->SetPosition(oldPosition);
			collider->SetOrientation(decMatrix::CreateRotation(oldRotation * DEG2RAD).ToQuaternion());
			
			phyWorld->ColliderMoveHits(collider, dropDisplacement, &closestElement);
			
			if(closestElement.GetHasHit()){
				distance = closestElement.GetHitDistance();
				
				if(distance > 0.001f){
					newPosition = oldPosition + decDVector(dropDisplacement * distance);
					
					if(pAlign){
						// TODO this alignment code is rather simple and not of good quality. one idea would
						// be to use physics simulation to drop the object onto the ground. another solution
						// would be to use multiple collision tests and slightly turning the object similar
						// to an inverse kinematic approach in animation.
						
						const decVector &normal = closestElement.GetHitNormal();
						
						newRotation.Set(acosf(normal.y) * RAD2DEG, atan2f(normal.x, -normal.z) * RAD2DEG, oldRotation.z);
						
						closestElement.Reset();
						
						collider->SetPosition(newPosition);
						collider->SetOrientation(decMatrix::CreateRotation(newRotation * DEG2RAD).ToQuaternion());
						
						phyWorld->ColliderMoveHits(collider, dropDisplacement, &closestElement);
						
						if(closestElement.GetHasHit()){
							distance = closestElement.GetHitDistance();
							
							if(distance > 0.001f){
								newPosition += decDVector(dropDisplacement * distance);
							}
						}
					}
					
					object->SetPosition(newPosition);
					if(pAlign){
						object->SetRotation(newRotation);
					}
					pWorld->NotifyObjectGeometryChanged(object);
					
					break;
				}
			}
			
			oldPosition.y += 0.1f;
			testCounter++;
		}
		
		if(data.GetAttachedObjects().IsNotEmpty()){
			const decDMatrix m(data.GetOldMatrixInverse() * object->GetObjectMatrix());
			data.GetAttachedObjects().Visit([&](const meUndoDataObject &attachedData){
				const decDMatrix m2(attachedData.GetOldMatrix() * m);
				meObject * const attached = attachedData.GetObject();
				attached->SetPosition(m2.GetPosition());
				attached->SetRotation(m2.GetEulerAngles().ToVector() * RAD2DEG);
				pWorld->NotifyObjectGeometryChanged(attached);
			});
		}
		
		filter->GetRejectObjects().Remove(object);
	});
}
