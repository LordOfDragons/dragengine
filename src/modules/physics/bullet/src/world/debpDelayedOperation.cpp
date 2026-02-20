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

#include "debpCollisionWorld.h"
#include "debpDelayedOperation.h"
#include "debpWorld.h"
#include "../dePhysicsBullet.h"
#include "../debpCollisionObject.h"

#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include <BulletDynamics/Dynamics/btRigidBody.h>

#include <dragengine/common/exceptions.h>



// Class debpDelayedOperation
///////////////////////////////

// Constructor, Destructor
////////////////////////////

debpDelayedOperation::debpDelayedOperation(debpCollisionWorld &world) :
pWorld(world),
pLockCount(0),
pDummyShape(NULL),
pDummyCollisionObject(NULL)
{
	// create the dummy shape. what shape is used here does not matter as it is only used
	// as replacement for shared collision shapes to avoid wild pointers down the line
	pDummyShape = new btSphereShape(0.1f);
	
	// create the dummy collision object. it is used for rigid bodies to be removed to
	// avoid segmentation faults since various code places expect the collision object
	// in the user pointer to be valid throughout the object lifetime. this is not the
	// case for rigid bodies marked for removal. the dummy object is owned by nobody
	pDummyCollisionObject = new debpCollisionObject;
}

debpDelayedOperation::~debpDelayedOperation(){
	if(pLockCount > 0){
		pWorld.GetWorld().GetBullet().LogWarnFormat("Delayed operation in world %p "
			"has a lock count of %i instead of 0!", &pWorld, pLockCount);
	}
	
	pListClearFromBroadphase.RemoveAll();
	
	pListAddRigidBody.RemoveAll();
	pListAddCollisionObject.RemoveAll();
	
	pListRemoveRigidBody.Visit([](btRigidBody *rb){
		delete rb;
	});
	pListRemoveRigidBody.RemoveAll();
	
	pListRemoveCollisionObject.Visit([](btCollisionObject *co){
		delete co;
	});
	pListRemoveCollisionObject.RemoveAll();
	
	if(pDummyCollisionObject){
		delete pDummyCollisionObject;
	}
	if(pDummyShape){
		delete pDummyShape;
	}
}



// Management
///////////////

void debpDelayedOperation::Lock(){
	pLockCount++;
}

void debpDelayedOperation::Unlock(){
	if(pLockCount == 0){
		pWorld.GetWorld().GetBullet().LogWarnFormat("Unlocked delayed operation in world "
			"%p but lock count is already 0!", &pWorld);
	}
	
	pLockCount--;
	
	if(pLockCount == 0){
		pApplyPendingOperations();
	}
}



void debpDelayedOperation::AddRigidBody(btRigidBody *rigidBody){
	if(pLockCount == 0){
		pWorld.GetWorld().GetBullet().LogWarnFormat("Rigid body %p added in delayed "
			"operation in world %p but lock count is 0!", rigidBody, &pWorld);
	}
	
	pListAddRigidBody.AddOrThrow(rigidBody);
}

void debpDelayedOperation::RemoveRigidBody(btRigidBody *rigidBody){
	if(pLockCount == 0){
		pWorld.GetWorld().GetBullet().LogWarnFormat("Rigid body %p removed in delayed "
			"operation in world %p but lock count is 0!", rigidBody, &pWorld);
	}
// 	pWorld.GetWorld().GetBullet()->LogWarnFormat( "RemoveRigid %p world %p", rigidBody, &pWorld );
	
	pListClearFromBroadphase.Remove(rigidBody);
	
	// if the rigid body is already marked to be added remove it from the list of rigid bodies to be added and delete it
	if(pListAddRigidBody.Remove(rigidBody)){
		delete rigidBody;
		
	// otherwise add it to the list of rigid bodies to be removed
	}else{
		pListRemoveRigidBody.AddOrThrow(rigidBody);
		
		// replace the collision shape with a temporary shape to avoid segmentation faults
		rigidBody->setCollisionShape(pDummyShape);
		
		// replace the collision object backlink with the dummy one to avoid segmentation faults
		rigidBody->setUserPointer(pDummyCollisionObject);
		
		// disable the rigid body. this technically removes it from the simulation before being really removed
		rigidBody->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE | btCollisionObject::CF_STATIC_OBJECT);
	}
}

void debpDelayedOperation::AddCollisionObject(btCollisionObject *collisionObject){
	if(pLockCount == 0){
		pWorld.GetWorld().GetBullet().LogWarnFormat("Collision object %p added in delayed "
			"operation in world %p but lock count is 0!", collisionObject, &pWorld);
	}
	
	pListAddCollisionObject.AddOrThrow(collisionObject);
}

void debpDelayedOperation::RemoveCollisionObject(btCollisionObject *collisionObject){
	if(pLockCount == 0){
		pWorld.GetWorld().GetBullet().LogWarnFormat("Collision object %p removed in delayed "
			"operation in world %p but lock count is 0!", collisionObject, &pWorld);
	}
	
	pListClearFromBroadphase.Remove(collisionObject);
	
	// if the collision object is already marked to be added remove it from the list of objects to be added and delete it
	if(pListAddCollisionObject.Remove(collisionObject)){
		delete collisionObject;
		
	// otherwise add it to the list of objects to be removed
	}else{
		pListRemoveCollisionObject.AddOrThrow(collisionObject);
		
		// replace the collision shape with a temporary shape to avoid segmentation faults
		collisionObject->setCollisionShape(pDummyShape);
		
		// replace the collision object backlink with the dummy one to avoid segmentation faults
		collisionObject->setUserPointer(pDummyCollisionObject);
		
		// disable the collision object. this technically removes it from the simulation before being really removed
		collisionObject->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE | btCollisionObject::CF_STATIC_OBJECT);
	}
}

void debpDelayedOperation::AddClearFromBroadphase(btCollisionObject *collisionObject){
	if(pLockCount == 0){
		pWorld.GetWorld().GetBullet().LogWarnFormat("AddClearFromBroadphase %p in delayed "
			"operation in world %p but lock count is 0!", collisionObject, &pWorld);
	}
	
	pListClearFromBroadphase.AddOrThrow(collisionObject);
}



// Private functions
//////////////////////

void debpDelayedOperation::pApplyPendingOperations(){
	pClearFromBroadphase();
	pRemoveCollisionObjects();
	pRemoveRigidBodies();
	pAddRigidBodies();
	pAddCollisionObjects();
}

void debpDelayedOperation::pClearFromBroadphase(){
	pListClearFromBroadphase.Visit([&](btCollisionObject *co){
		pWorld.getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(
			co->getBroadphaseHandle(), pWorld.getDispatcher());
	});
	
	pListClearFromBroadphase.RemoveAll();
}

void debpDelayedOperation::pRemoveCollisionObjects(){
	pListRemoveCollisionObject.Visit([&](btCollisionObject *co){
		pWorld.removeCollisionObject(co);
		delete co;
	});
	
	pListRemoveCollisionObject.RemoveAll();
}

void debpDelayedOperation::pRemoveRigidBodies(){
	pListRemoveRigidBody.Visit([&](btRigidBody *rigidBody){
		pWorld.removeRigidBody(rigidBody);
		delete rigidBody;
	});
	
	pListRemoveRigidBody.RemoveAll();
}

void debpDelayedOperation::pAddRigidBodies(){
	pListAddRigidBody.Visit([&](btRigidBody *rigidBody){
		pWorld.addRigidBody(rigidBody);
	});
	
	pListAddRigidBody.RemoveAll();
}

void debpDelayedOperation::pAddCollisionObjects(){
	pListAddCollisionObject.Visit([&](btCollisionObject *co){
		// btDiscreteDynamicsWorld overloads addCollisionObject with different
		// default filter parameters in contrary to the base class version which
		// breaks physics. set here explicitly the values used in the base version
		// to get a consistent behavior
		pWorld.addCollisionObject(co, btBroadphaseProxy::DefaultFilter, btBroadphaseProxy::AllFilter);
	});
	
	pListAddCollisionObject.RemoveAll();
}
