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


class cSorter : public meObjectList::Comparator{
public:
	cSorter() = default;
	int operator() (meObject &a, meObject &b) override{
		const double d = a.GetPosition().y - b.GetPosition().y;
		return d > DOUBLE_SAFE_EPSILON ? 1 : (d < -DOUBLE_SAFE_EPSILON ? -1 : 0);
	}
};


// Class meUObjectDropToGround
////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectDropToGround::meUObjectDropToGround(meWorld *world, const meObjectList &objects) :

pObjects(nullptr),
pObjectCount(0),
pDropOnObjects(true),
pAlign(false)
{
	DEASSERT_NOTNULL(world)
	
	const int count = objects.GetCount();
	DEASSERT_TRUE(count > 0)
	
	SetShortInfo("Drop objects to ground");
	
	decString text;
	if(count > 1){
		text.Format("%d objects", count);
		
	}else{
		text = "1 object";
	}
	SetLongInfo(text);
	
	cSorter sorter;
	const meObjectList sorted(objects.GetSorted(sorter));
	
	try{
		pObjects = new sObject[count];
		
		for(pObjectCount=0; pObjectCount<count; pObjectCount++){
			meObject * const object = sorted.GetAt(pObjectCount);
			
			pObjects[pObjectCount].oldpos = object->GetPosition();
			pObjects[pObjectCount].oldrot = object->GetRotation();
			pObjects[pObjectCount].object = object;
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
	
	pWorld = world;
}

meUObjectDropToGround::~meUObjectDropToGround(){
	pCleanUp();
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
	meObject::Ref object;
	int o;
	
	for(o=0; o<pObjectCount; o++){
		object = pObjects[o].object;
		object->SetPosition(pObjects[o].oldpos);
		object->SetRotation(pObjects[o].oldrot);
		pWorld->NotifyObjectGeometryChanged(object);
	}
}

void meUObjectDropToGround::Redo(){
	deBasePhysicsWorld *phyWorld = pWorld->GetEngineWorld()->GetPeerPhysics();
	decVector dropDisplacement(0.0f, -10.0f, 0.0f);
	meCLClosestElement closestElement(*pWorld);
	decDVector oldPosition, newPosition;
	decVector oldRotation, newRotation;
	meFilterObjectsByClass filter;
	deColliderVolume *collider;
	int testCounter = 0;
	meObject::Ref object;
	float distance;
	int o;
	
	if(!phyWorld) return;
	
	closestElement.SetTestHeightTerrain(true);
	closestElement.SetTestObjects(pDropOnObjects);
	closestElement.SetTestDecals(false);
	closestElement.SetFilterObjects(&filter);
	
	filter.SetRejectGhosts(true);
	//filter.SetClassNamesFrom( guiparams.GetAddFilterObjectList() );
	//filter.SetMatchInclusive( guiparams.GetAddFilterObjectInclusive() );
	
	decLayerMask collisionCategory;
	collisionCategory.SetBit(meWorld::eclmEditing);
	
	decLayerMask collisionFilter;
	collisionFilter.SetBit(meWorld::eclmObjects);
	collisionFilter.SetBit(meWorld::eclmHeightTerrains);
	
	for(o=0; o<pObjectCount; o++){
		object = pObjects[o].object;
		
		collider = object->GetColDetCollider();
		if(!collider){
			continue;
		}
		
		collider->SetCollisionFilter(decCollisionFilter(collisionCategory, collisionFilter));
		
		oldPosition = pObjects[o].oldpos;
		oldRotation = pObjects[o].oldrot;
		
		filter.SetRejectObject(object);
		
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
					//pObjects[ o ]->SetNewSector( wsNew->GetCoordinates() );
					
					pWorld->NotifyObjectGeometryChanged(object);
					
					break;
				}
			}
			
			oldPosition.y += 0.1f;
			testCounter++;
		}
	}
}



// Private Functions
//////////////////////

void meUObjectDropToGround::pCleanUp(){
	if(pObjects){
		while(pObjectCount > 0){
			pObjectCount--;
			pObjects[pObjectCount].object->FreeReference();
		}
		
		delete [] pObjects;
	}
	
	if(pWorld) pWorld->FreeReference();
}
